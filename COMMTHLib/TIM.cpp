// TIM.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TIM.h"

CMDSTATUS cmdstatus;//全局的CMDSTATUS结构体对象
HANDLE recvThread;//串口接收线程HANDLE

//超时判断
bool TIM_IsTimeUp(IN DWORD dwTimer, IN DWORD dwBaseTime)
{
	BOOL bRet = FALSE;
	DWORD dwCurTime;
	DWORD dwUpTime;
	//            dwTime
	//    |-------------------|
	// dwBaseTime          dwUpTime
	//    |----------|--------|-----
	//           dwCurTime
	// Has timer
	if (dwTimer != 0) {
		dwCurTime = GetTickCount();		// 当前时间
		dwUpTime = dwBaseTime + dwTimer;// 结束时间
		if (dwUpTime >= dwBaseTime) {
			if (dwCurTime >= dwBaseTime) {
				// 超时（当前时间 > 结束时间）
				if (dwCurTime >= dwUpTime) {
					bRet = TRUE;
				}
			}
			else {
				bRet = TRUE;
			}
		}
		else {
			if (dwCurTime < dwBaseTime) {
				if (dwCurTime >= dwUpTime) {
					bRet = TRUE;
				}
			}
		}
	}

	return bRet;
}
//拼包
void TIM_GetWritePackets(DWORD nDataLength, BYTE* pDataBuffer, BYTE* pWriteBuff, DWORD& nWriteLength)
{
	byte bcc = TIM_BccCalc(pDataBuffer, nDataLength);
	*pWriteBuff++ = DLE;
	nWriteLength++;
	*pWriteBuff++ = STX;
	nWriteLength++;
	DWORD nEscapeLength = TIM_Escape(pDataBuffer, nDataLength);
	for (int i = 0; i < nEscapeLength; i++)
	{
		*pWriteBuff++ = pDataBuffer[i];
		nWriteLength++;
	}
	*pWriteBuff++ = DLE;
	nWriteLength++;
	*pWriteBuff++ = ETX;
	nWriteLength++;
	*pWriteBuff++ = bcc;
	nWriteLength++;
}
//计算BCC
byte TIM_BccCalc(BYTE* pData, WORD dwLength)
{
	byte Bcc = 0;
	while (dwLength-- != 0) {
		Bcc = Bcc ^ *pData++;
	}
	return Bcc;
}
//写串口函数
bool TIM_WriteBuffToCom(BYTE* writeBuffer, int len)
{
	return sp_write((char*)writeBuffer, len);
}
//线程回调函数
static unsigned int CALLBACK TIM_OnRecv(void* LPParam)
{
	CMDSTATUS* sts = static_cast<CMDSTATUS*>(LPParam);

	while (sts->ComIsOpen)
	{
		if (sts->ComIsNeedRecv)
		{
			bool ret = sp_read((char*)(sts->ReadBuff + sts->ReadLength), 1);
			if (ret)
			{
				if (sts->ReadLength == 0)
				{
					sts->CurrentByte = sts->ReadBuff[sts->ReadLength];
				}
				else
				{
					sts->BeforeByte = sts->CurrentByte;
					sts->CurrentByte = sts->ReadBuff[sts->ReadLength];
				}
				sts->ReadLength++;

				//判断接收到的数据
				if (sts->BeforeByte == DLE)
				{
					switch (sts->CurrentByte)
					{
					case STX:
						if (sts->Phase == ENQ)
							sts->Phase = STX;
						break;
					case ETX:
						if (sts->Phase == STX)
							sts->Phase = ETX;
						break;
					case EOT:
						//sts->Phase = EOT;
						break;
					case ACK:
					{
						//清空待发送数据
						memset(sts->WriteBuff, 0x00, sizeof(sts->WriteBuff));
						sts->WriteLength = 0;
						sts->RetryCnt = 0;

						//清空DLE ACK
						memset(sts->ReadBuff, 0x00, sizeof(sts->ReadBuff));
						sts->ReadLength = 0;
						//重置BeforeByte
						sts->BeforeByte == 0;

						byte enq[2] = { DLE,ENQ };
						TIM_WriteBuffToCom(enq, sizeof(enq));
						sts->Phase = ENQ;
						//标记基时间重置
						sts->DwBaseTimeIsReset = TRUE;
						//把ENQ数据COPY到WriteBuff
						memcpy(sts->WriteBuff, enq, sizeof(enq));
						sts->WriteLength = sizeof(enq);
						break;
					}
					case NAK:
						sts->RetryCnt++;
						if (sts->WriteLength > 0 && sts->RetryCnt < MAXRETRYCNT)
						{
							//标记基时间重置
							sts->DwBaseTimeIsReset = TRUE;
							TIM_WriteBuffToCom(sts->WriteBuff, sts->WriteLength);
							sts->Phase = NAK;
							//重置BeforeByte
							sts->BeforeByte == 0;
						}
						break;
					default:
						break;
					}
				}
				if (sts->Phase == ETX && sts->BeforeByte == ETX)
				{
					//获取需要转义的数据体
					byte dataBuffer[MAX_BUFFER_SIZE] = { 0 };
					memcpy(dataBuffer, sts->ReadBuff + TIM_HEAD_LEN, sts->ReadLength - TIM_HEAD_AND_END_LEN);
					//接收到的BCC
					byte readBcc = sts->ReadBuff[sts->ReadLength - 1];
					//还原转义数据
					int ndataLength = TIM_Reverse_Escape(dataBuffer, sts->ReadLength - TIM_HEAD_AND_END_LEN);
					sts->ReadLength = ndataLength + TIM_HEAD_AND_END_LEN;
					memset(sts->ReadBuff, 0x00, sizeof(sts->ReadBuff));
					//重新拼接数据
					sts->ReadBuff[0] = DLE;
					sts->ReadBuff[1] = STX;
					memcpy(sts->ReadBuff + TIM_HEAD_LEN, dataBuffer, ndataLength);
					sts->ReadBuff[sts->ReadLength - 3] = DLE;
					sts->ReadBuff[sts->ReadLength - 2] = ETX;
					sts->ReadBuff[sts->ReadLength - 1] = readBcc;
					//计算BCC码
					byte bcc = TIM_BccCalc(sts->ReadBuff + TIM_HEAD_LEN, sts->ReadLength - TIM_HEAD_AND_END_LEN/*ndataLength*/);
					if (bcc == sts->ReadBuff[sts->ReadLength - 1])
					{
						sts->ComIsIncludeReturnBytes = TRUE;
					}
					else
					{
						sts->RetryCnt++;
						if (sts->RetryCnt < MAXRETRYCNT)
						{
							//清空待发送数据
							memset(sts->WriteBuff, 0x00, sizeof(sts->WriteBuff));
							sts->WriteLength = 0;
							//sts->RetryCnt = 0;

							//重新回到ENQ阶段
							sts->Phase = ENQ;
							memset(sts->ReadBuff, 0x00, sizeof(sts->ReadBuff));
							sts->ReadLength = 0;
							//重置BeforeByte
							sts->BeforeByte == 0;

							//NAK
							byte nak[2] = { DLE,NAK };
							TIM_WriteBuffToCom(nak, sizeof(nak));

							//标记基时间重置
							sts->DwBaseTimeIsReset = TRUE;
							//把ENQ数据COPY到WriteBuff
							memcpy(sts->WriteBuff, nak, sizeof(nak));
							sts->WriteLength = sizeof(nak);
						}
						else
						{
							//重新回到ENQ阶段
							sts->Phase = ENQ;
							memset(sts->ReadBuff, 0x00, sizeof(sts->ReadBuff));
							sts->ReadLength = 0;
							//重置BeforeByte
							sts->BeforeByte == 0;
						}
					}
				}
			}
		}
		else
		{
			Sleep(100);
		}

	}

	return TIMTRS_RES_OK;
}
int TIM_SentOneCommand(DWORD nDataLength, BYTE* pDataBuffer, BYTE* pReadBuff, DWORD& nReadLength)
{

	DWORD nWriteLength = 0;
	byte pWriteBuffer[MAX_BUFFER_SIZE] = { 0 };
	//拼包
	TIM_GetWritePackets(nDataLength, pDataBuffer, pWriteBuffer, nWriteLength);

	DWORD nResult = TIMTRS_RES_OK;
	DWORD dwBaseTime = GetTickCount();
	bool ret = TIM_WriteBuffToCom(pWriteBuffer, nWriteLength);
	if (ret)
	{
		memcpy(cmdstatus.WriteBuff, pWriteBuffer, nWriteLength);
		cmdstatus.WriteLength = nWriteLength;
		cmdstatus.Phase = ACK;
		cmdstatus.ComIsNeedRecv = TRUE;
		cmdstatus.ComIsTimeOut = FALSE;
		while (true)
		{
			//如果dwBaseTime值变成0，则证明已过5秒超时或DwBaseTimeIsReset需要重置
			if (dwBaseTime == 0 || cmdstatus.DwBaseTimeIsReset)
			{
				cmdstatus.DwBaseTimeIsReset = FALSE;
				dwBaseTime = GetTickCount();
			}
			//如果包含返回数据，则返回
			if (cmdstatus.ComIsIncludeReturnBytes)
			{
				memcpy(pReadBuff, cmdstatus.ReadBuff, cmdstatus.ReadLength);
				nReadLength = cmdstatus.ReadLength;

				//重置cmdstatus
				cmdstatus.CurrentByte = 0;
				cmdstatus.BeforeByte = 0;
				cmdstatus.RetryCnt = 0;
				memset(cmdstatus.WriteBuff, 0x00, sizeof(cmdstatus.WriteBuff));
				memset(cmdstatus.ReadBuff, 0x00, sizeof(cmdstatus.ReadBuff));
				cmdstatus.WriteLength = 0;
				cmdstatus.ReadLength = 0;
				cmdstatus.ComIsNeedRecv = FALSE;
				cmdstatus.ComIsIncludeReturnBytes = FALSE;
				cmdstatus.DwBaseTimeIsReset = FALSE;
				cmdstatus.ComIsTimeOut = FALSE;
				break;
			}
			else
			{
				//五秒超时、并且Phase为等待ACK、NAK阶段
				if (TIM_IsTimeUp(DEFAULTINTERVAL, dwBaseTime) && (cmdstatus.Phase == ACK || cmdstatus.Phase == NAK || cmdstatus.Phase == ENQ))
				{
					//如如果在等待ACK应答发送了非法数据过来，这时候清零
					if (cmdstatus.Phase == ACK && cmdstatus.ReadLength > 0)
					{
						memset(cmdstatus.ReadBuff, 0x00, cmdstatus.ReadLength);
						cmdstatus.ReadLength = 0;
					}
					//五秒超时时dwBaseTime置为0
					dwBaseTime = 0;
					cmdstatus.RetryCnt++;
					if (cmdstatus.RetryCnt < MAXRETRYCNT)
					{
						//重发
						//ENQ阶段有两种可能 
						//1、收到ACK后，发送DLE ENQ，设备未应答 
						//2、发送完DLE ENQ收到的数据有误，回复DLE NAK，此时重回ENQ阶段，等待数据重发
						ret = TIM_WriteBuffToCom(cmdstatus.WriteBuff, cmdstatus.WriteLength);
					}
					else
					{
						//三次重试结束，退出
						cmdstatus.ComIsTimeOut = TRUE;
					}
				}
				else if (TIM_IsTimeUp(DEFAULTINTERVAL, dwBaseTime) && (cmdstatus.Phase == STX || cmdstatus.Phase == ETX))
				{
					//五秒超时时dwBaseTime置为0
					dwBaseTime = 0;
					cmdstatus.RetryCnt++;
					if (cmdstatus.RetryCnt < MAXRETRYCNT)
					{
						//等待数据，不做任何处理
					}
					else
					{
						//三次重试结束，退出
						cmdstatus.ComIsTimeOut = TRUE;
					}
				}
				//如果超时也要退出
				if (cmdstatus.ComIsTimeOut)
				{
					switch (cmdstatus.Phase)
					{
					case ACK:
						nResult = TIM_ERR_CODE_RECIEVE_TIMEOUT;
						break;
					case ENQ:
						nResult = TIM_ERR_CODE_RECIEVE_TIMEOUT;
						break;
					case NAK:
						nResult = TIM_ERR_CODE_SEND_DATA_ERR;
						break;
					default:
						nResult = TIM_ERR_CODE_RECIEVE_TIMEOUT;
						break;
					}
					//重置cmdstatus
					cmdstatus.CurrentByte = 0;
					cmdstatus.BeforeByte = 0;
					cmdstatus.RetryCnt = 0;
					memset(cmdstatus.WriteBuff, 0x00, sizeof(cmdstatus.WriteBuff));
					memset(cmdstatus.ReadBuff, 0x00, sizeof(cmdstatus.ReadBuff));
					cmdstatus.WriteLength = 0;
					cmdstatus.ReadLength = 0;
					cmdstatus.ComIsNeedRecv = FALSE;
					cmdstatus.ComIsIncludeReturnBytes = FALSE;
					cmdstatus.DwBaseTimeIsReset = FALSE;
					cmdstatus.ComIsTimeOut = FALSE;
					return nResult;
				}
				else
				{
					Sleep(100);
				}
			}
		}
	}
	else
	{
		nResult = TIM_ERR_CODE_CMD_SEND_ERR;
	}
	return nResult;
}
//按位取Byte值
void TIM_GetByteBits(BYTE mByte, BYTE* pByte, int len)
{
	for (int i = 0; i < len; i++) {
		*pByte = ((mByte >> 7) & 1);
		mByte <<= 1;
		pByte++;
	}
}
//发送的数据体转义
int TIM_Escape(BYTE * data, const int& len)
{
	int i, j;

	byte* buf = (byte*) new byte[len * 2];
	memset(buf, 0x0, len * 2);

	for (i = 0, j = 0; i < len; ++i) {
		if (data[i] == DLE) {
			buf[j++] = DLE;
			buf[j++] = DLE;
		}
		else {
			buf[j++] = data[i];
		}
	}
	memset(data, 0x0, len);
	memcpy(data, buf, j);
	delete[] buf;
	return j;
}
//接收到的数据体，移除转义
int TIM_Reverse_Escape(BYTE * data, const int& len)
{
	int i, j;

	byte* buf = (byte*) new byte[len * 2];
	memset(buf, 0x0, len * 2);

	for (i = 0, j = 0; i < len; ++i) {
		if ((data[i] == DLE)) {
			buf[j++] = DLE;
			++i;
		}
		else {
			buf[j++] = data[i];
		}
	}
	memset(data, 0x0, len);
	memcpy(data, buf, j);
	delete[] buf;
	return j;
}

void TIM_SetStatus(int nResult, tTIMModuleStatus * pModuleStatus, tTIMDevReturn * pDevStatus, BYTE * readBuff)
{
	//返回状态码不是失败
	if (nResult == TIMTRS_RES_OK && readBuff[1] != TIMTRS_CMD_FAIL)
	{
		int pOffSet = 0;
		int index = 0;
		byte pBytes[8];
		//第4-6是传感器相关的
		//第四字节
		memset(pBytes, 0, sizeof(pBytes));
		TIM_GetByteBits(readBuff[4], pBytes, sizeof(pBytes));
		//1、卡箱A将空检测传感器（U 型）
		//A 票箱状态, 0:正常, 1 : 将空, 2 : 全空, 3 : 未安装, 4 : 安装不到位, 5 : 票箱故障
		pModuleStatus->cCassetteAStatus = pBytes[index];

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR55_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//2、卡箱A存在传感器（对射/漫反射）
		//A 票箱状态
		if (pBytes[index] == 0)
			pModuleStatus->cCassetteAStatus = 3;//未安装

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR56_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//3、卡箱A空检测传感器（U 型） 
		//A 票箱状态
		if (pBytes[index] == 0)
			pModuleStatus->cCassetteAStatus = 2;//已空

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR57_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//4、卡箱A辅助出票传感器（U 型）--未定义
		index++;
		//5、卡箱A进入通道传感器（对射）
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR51_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//6、卡箱A下限传感器（U 型） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR11_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//7、卡箱B将空检测传感器（U 型）
		// B 票箱状态, 0:正常, 1: 将空, 2:全空, 3:未安装, 4:安装不到位, 5:票箱故障
		pModuleStatus->cCassetteBStatus = pBytes[index];

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR61_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//8、卡箱B存在传感器（对射/漫反射）
		// B 票箱状态
		if (pBytes[index] == 0)
			pModuleStatus->cCassetteBStatus = 3;//未安装

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR62_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];

		//第五字节
		memset(pBytes, 0, sizeof(pBytes));
		TIM_GetByteBits(readBuff[5], pBytes, sizeof(pBytes));
		index = 0;
		//1、卡箱B空检测传感器（U 型）
		//B 票箱状态
		// B 票箱状态
		if (pBytes[index] == 0)
			pModuleStatus->cCassetteBStatus = 2;//已空

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR63_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//2、保留
		index++;
		//3、卡箱B进入通道传感器（对射） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR65_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//4、卡箱B下限传感器（U型） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR12_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//5、卡读写位置传感器（U 型）
		//是否有卡在读卡区
		pModuleStatus->bCardInReadArea = pBytes[index];
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR53_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//6、出口计数传感器（U型）
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR54_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//7、换向器传感器（U 型）
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR69_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//8、通道编码器传感器（对射）
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR70_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];

		//第六字节
		memset(pBytes, 0, sizeof(pBytes));
		TIM_GetByteBits(readBuff[6], pBytes, sizeof(pBytes));
		index = 0;
		//1、回收箱存在传感器（漫反射）
		// 废票箱状态, 0:正常1:将满, 2:满, 3:未安装, 4:安装不到位, 5:票箱故障
		if (pBytes[index] == 0)
			pModuleStatus->cRejectCassetteStatus = 3;//未安装

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR71_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//2、回收箱满/计数传感器（对射）
		// 废票箱状态
		if (pBytes[index] == 0)
			pModuleStatus->cRejectCassetteStatus = 0;//正常
		else
			pModuleStatus->cRejectCassetteStatus = 2;//满

		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR73_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//3、卡箱A黑块检测传感器（对射） （GRG 版 本） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR60_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//4、卡箱B黑块检测传感器（对射）（GRG 版 本） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR66_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//5、A卡箱盖检测sensor(反射)（塑料票箱 未用） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR74_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//6、B卡箱盖检测sensor(反射)（塑料票箱 未用） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR75_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//7、C卡箱盖检测sensor(反射)（塑料票箱 未用） 
		pModuleStatus->tSensorType[pOffSet].uiSensotID = CODE_SENSOR76_ID;
		pModuleStatus->tSensorType[pOffSet++].btSensorStatus = pBytes[index++];
		//8 保留

		//传感器数量
		pModuleStatus->uiSensorCount = pOffSet;

		//票卡数量
		pModuleStatus->tCardNum.uiCassetteACardNum = MAKEWORD(readBuff[8], readBuff[9]);
		pModuleStatus->tCardNum.uiCassetteBCardNum = MAKEWORD(readBuff[10], readBuff[11]);
		pModuleStatus->tCardNum.uiRejectCassetteNum = MAKEWORD(readBuff[12], readBuff[13]);


		//设置pDevStatus值
		pDevStatus->uiProviderId = 0x04;
		pDevStatus->uiModuleType = 0x06;
		if (readBuff[1] = TIMTRS_CMD_SUCCESS)
			pDevStatus->uiType = 0;
		else if (readBuff[1] = TIMTRS_CMD_WARNING)
			pDevStatus->uiType = 1;
		else if (readBuff[1] = TIMTRS_CMD_FAIL)
			pDevStatus->uiType = 2;
		//故障类型
		if (pDevStatus->uiType == 0)
			pDevStatus->uiErrorType = 0;
		else if (pDevStatus->uiType == 1)
			pDevStatus->uiErrorType = 0;
		else if (pDevStatus->uiType == 2)
			pDevStatus->uiErrorType = 2;
		//错误代码
		pDevStatus->uiErrorCode = readBuff[2];
	}
	else if (nResult == TIMTRS_RES_OK && readBuff[1] == TIMTRS_CMD_FAIL)
	{
		//设置pDevStatus值
		pDevStatus->uiProviderId = 0x04;
		pDevStatus->uiModuleType = 0x06;
		pDevStatus->uiType = 2;
		//故障类型
		pDevStatus->uiErrorType = 2;
		//错误代码
		pDevStatus->uiErrorCode = readBuff[2];
	}
	else
	{
		//设置pDevStatus值
		pDevStatus->uiProviderId = 0x04;
		pDevStatus->uiModuleType = 0x06;
		pDevStatus->uiType = 2;
		//故障类型
		pDevStatus->uiErrorType = 2;
		//错误代码
		pDevStatus->uiErrorCode = nResult;
	}
}
void TIM_SetDevReturn(int nResult, tTIMDevReturn * pDevStatus, BYTE * recvBuff)
{
	//返回状态码不是失败
	if (nResult == TIMTRS_RES_OK && recvBuff[1] != TIMTRS_CMD_FAIL)
	{
		//设置pDevStatus值
		pDevStatus->uiProviderId = 0x04;
		pDevStatus->uiModuleType = 0x06;
		if (recvBuff[1] = TIMTRS_CMD_SUCCESS)
			pDevStatus->uiType = 0;
		else if (recvBuff[1] = TIMTRS_CMD_WARNING)
			pDevStatus->uiType = 1;
		else if (recvBuff[1] = TIMTRS_CMD_FAIL)
			pDevStatus->uiType = 2;
		//故障类型
		if (pDevStatus->uiType == 0)
			pDevStatus->uiErrorType = 0;
		else if (pDevStatus->uiType == 1)
			pDevStatus->uiErrorType = 0;
		else if (pDevStatus->uiType == 2)
			pDevStatus->uiErrorType = 2;
		//错误代码
		pDevStatus->uiErrorCode = recvBuff[2];
	}
	else if (nResult == TIMTRS_RES_OK && recvBuff[1] == TIMTRS_CMD_FAIL)
	{
		//设置pDevStatus值
		pDevStatus->uiProviderId = 0x04;
		pDevStatus->uiModuleType = 0x06;
		pDevStatus->uiType = 2;
		//故障类型
		pDevStatus->uiErrorType = 2;
		//错误代码
		pDevStatus->uiErrorCode = recvBuff[2];
	}
	else
	{
		//设置pDevStatus值
		pDevStatus->uiProviderId = 0x04;
		pDevStatus->uiModuleType = 0x06;
		pDevStatus->uiType = 2;
		//故障类型
		pDevStatus->uiErrorType = 2;
		//错误代码
		pDevStatus->uiErrorCode = nResult;
	}
}
void TIM_SetReturns(int nResult, tTIMModuleStatus * pModuleStatus, tTIMDevReturn * pDevStatus, BYTE * readBuff, int readLength)
{
	//tTIMModuleStatus、tTIMDevReturn均需要返回
	if (readLength == MAX_BUFFER_SIZE && pModuleStatus != NULL)
	{
		switch (readBuff[0])
		{
		case TIM_CMD_INIT://初始化
		{
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_GETSTATUS://获取状态
		{
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_OUTCARD://获取票卡，出卡
		{
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_SENTCARD://送卡
		{
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_RETRACTCARD://回收卡
		{
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_TEST://测试
		{
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
		default:
			TIM_SetStatus(nResult, pModuleStatus, pDevStatus, readBuff);
			break;
		}
	}
	//仅tTIMDevReturn需要返回
	else if (readLength == MAX_BUFFER_SIZE && pModuleStatus == NULL)
	{
		switch (readBuff[0])
		{
		case TIM_CMD_RESET://复位
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_GETVERSION://获取版本
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_GETSN://获取序列号
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_SETTICKETBOXCOUNT://设置票箱的票卡数量
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_REPLACEBOX://更换票箱
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_WRITERFID://向 RFID 写一个 Block 数据 
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_READRFID://从 RFID 读一个 Block 数据
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_READPHYSIClNUMBER://读取 RFID 卡的物理编号
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		case TIM_CMD_GETAUDITDATA://读审计累计值
		{
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
		default:
			TIM_SetDevReturn(nResult, pDevStatus, readBuff);
			break;
		}
	}
}
//---------------------------------------------------以下是标准化接口-------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//通讯建立
int TIM_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName)
{
	//如果传入的值为空值
	if (path == _T("") || fileName == _T(""))
		return TIMTRS_RES_FAILED;
	//初始化日志
	LogApi.Initialize(path, fileName);

	CString sLogString(_T(""));

	sLogString.Format(_T("TIM_CommOpen: uiCommPort = %d,uiBaudRate = %d"), uiCommPort, uiBaudRate);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);

	if (cmdstatus.ComIsOpen)
	{
		TIM_CommClose();
	}
	DWORD nResult = TIMTRS_RES_OK;
	bool ret = sp_open(uiCommPort, uiBaudRate, 'N', 8, 1, EV_RXCHAR);
	if (!ret)
	{
		sLogString.Format(_T("TIM_CommOpen: ReturnCode = 0x%02x"), nResult);
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
		return TIMTRS_RES_FAILED;
	}
	cmdstatus.ComIsOpen = TRUE;
	recvThread = (HANDLE)_beginthreadex(NULL, 0, TIM_OnRecv, &cmdstatus, 0, NULL);
	sLogString.Format(_T("TIM_CommOpen: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//模块初始化
int TIM_Init(char cClearMode, UINT * uiRetractNum, tTIMModuleStatus * pModuleStatus, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_Init"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);

	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Init: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == uiRetractNum)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Init: uiRetractNum Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Init: pModuleStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Init: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}

	//初始化
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_INIT };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, pModuleStatus, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		//防止设备应答类型有误
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, pModuleStatus, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_Init: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//模块复位
int(TIM_Reset)(char cClearMode, UINT * uiRetractNum, tTIMModuleStatus * pModuleStatus, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = 0;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_Reset"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Reset: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Reset: pModuleStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_Reset: pDevStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//复位
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_RESET };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;

	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		//防止设备应答类型有误
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	//获取状态时已给pModuleStatus设置过值，若复位失败，则pDevStatus按失败值返回
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_Reset: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	nResult = TIM_GetDevStatus(pModuleStatus, pDevStatus);
	sLogString.Format(_T("TIM_GetDevStatus: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;

}
//读取设备驱动版本信息
int(TIM_GetVersion)(char cFirmWareVersion[32], char cDriverVersion[32], tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_GetVersion"));
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetVersion: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == cFirmWareVersion)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetVersion: cFirmWareVersion Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == cDriverVersion)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetVersion: cDriverVersion Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetVersion: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}

	//获取状态
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_GETVERSION };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		//防止设备应答类型有误
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	//设置版本信息
	memcpy(cFirmWareVersion, readBuff + TIM_HEAD_LEN + 10, 7);
	memcpy(cDriverVersion, readBuff + TIM_HEAD_LEN + 17, 4);
	sLogString.Format(_T("TIM_GetVersion: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//读取设备状态
int(TIM_GetDevStatus)(tTIMModuleStatus * pModuleStatus, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_GetDevStatus"));
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetDevStatus: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetDevStatus: pModuleStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetDevStatus: pDevStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}

	//获取状态
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_GETSTATUS };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;

	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, pModuleStatus, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		//防止设备应答类型有误
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, pModuleStatus, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_GetDevStatus: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//获取票卡
int(TIM_CardOut)(char cBoxNo, tTIMModuleStatus * pModuleStatus, UINT * uiTransportNum, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_CardOut"));
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_CardOut: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (0 == cBoxNo)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_CardOut: cBoxNo值等于零"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_CardOut: pModuleStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == uiTransportNum)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_CardOut: uiTransportNum Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_CardOut: pDevStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}

	//出卡
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_OUTCARD,cBoxNo };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 2;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, pModuleStatus, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		//防止设备应答类型有误
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
		else
		{
			*uiTransportNum = readBuff[TIM_HEAD_LEN + 14];
		}
	}
	TIM_SetReturns(nResult, pModuleStatus, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_CardOut: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//送出票卡
int(TIM_SendCard)(tTIMModuleStatus * pModuleStatus, UINT * uiTransportNum, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_SendCard"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SendCard: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SendCard: pModuleStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == uiTransportNum)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SendCard: uiTransportNum Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SendCard: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}

	//送卡
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_SENTCARD };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, pModuleStatus, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
		else
		{
			*uiTransportNum = readBuff[TIM_HEAD_LEN + 14];
		}
	}
	TIM_SetReturns(nResult, pModuleStatus, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_SendCard: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//回收票卡
int(TIM_RetractCard)(tTIMModuleStatus * pModuleStatus, UINT * uiRetractNum, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_RetractCard"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_RetractCard: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_RetractCard: pModuleStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == uiRetractNum)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_RetractCard: uiRetractNum Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_RetractCard: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}

	//回收票卡
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_RETRACTCARD };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, pModuleStatus, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
		else
		{
			*uiRetractNum = readBuff[TIM_HEAD_LEN + 14];
		}
	}
	TIM_SetReturns(nResult, pModuleStatus, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_RetractCard: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//读取审计累计值
int(TIM_GetAuditData)(tTIMAuditCardNum * pAuditCardNum, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_GetAuditData"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetAuditData: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pAuditCardNum)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetAuditData: pAuditCardNum Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetAuditData: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//读取审计累计值
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_GETAUDITDATA };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 1;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
		else
		{

			pAuditCardNum->wCassetteAAuditCardNum = (UINT)(readBuff[5] | readBuff[6] << 8 | readBuff[7] << 16 | readBuff[8] << 24);
			pAuditCardNum->wCassetteBAuditCardNum = (UINT)(readBuff[9] | readBuff[10] << 8 | readBuff[11] << 16 | readBuff[12] << 24);
			pAuditCardNum->wRejectCassetteAuditNum = (UINT)(readBuff[13] | readBuff[14] << 8 | readBuff[15] << 16 | readBuff[16] << 24);
		}
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_GetAuditData: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//读取票箱RFID信息
int(TIM_GetTicketBoxRFIDInfo)(char cTicketBoxNo, tTIMRFIDInfo * pRFIDInfo, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_GetTicketBoxRFIDInfo"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetTicketBoxRFIDInfo: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (TIM_BOX_A != cTicketBoxNo && TIM_BOX_B != cTicketBoxNo && TIM_BOX_C != cTicketBoxNo)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetTicketBoxRFIDInfo: cTicketBoxNo值非法，cTicketBoxNo：%d"), cTicketBoxNo);
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pRFIDInfo)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetTicketBoxRFIDInfo: pRFIDInfo Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetTicketBoxRFIDInfo: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//向 RFID 读一个 Sector 数据
	DWORD nResult, nDataLength, nReadLength, section;
	if (cTicketBoxNo == TIM_BOX_A)
	{
		section = 2;
	}
	else if (cTicketBoxNo == TIM_BOX_B)
	{
		section = 3;
	}
	else if (cTicketBoxNo == TIM_BOX_C)
		section = 4;

	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_READSECTTOR,cTicketBoxNo,section };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 3;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
		else
		{
			memcpy(pRFIDInfo, readBuff + 3 + TIM_HEAD_LEN, sizeof(tTIMRFIDInfo));
		}
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_GetTicketBoxRFIDInfo: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//设置票箱RFID信息
int(TIM_SetTicketBoxRFIDInfo)(char cTicketBoxNo, tTIMRFIDInfo * pRFIDInfo, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_SetTicketBoxRFIDInfo"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxRFIDInfo: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (TIM_BOX_A != cTicketBoxNo && TIM_BOX_B != cTicketBoxNo && TIM_BOX_C != cTicketBoxNo)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxRFIDInfo: cTicketBoxNo值非法，cTicketBoxNo：%d"), cTicketBoxNo);
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pRFIDInfo)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxRFIDInfo: pRFIDInfo Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxRFIDInfo: pDevStatus Is Null"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//向 RFID 写一个 Sector 数据
	DWORD nResult, nDataLength, nReadLength, section;
	if (cTicketBoxNo == TIM_BOX_A)
	{
		section = 2;
	}
	else if (cTicketBoxNo == TIM_BOX_B)
	{
		section = 3;
	}
	else if (cTicketBoxNo == TIM_BOX_C)
		section = 4;

	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_WRITESECTTOR,cTicketBoxNo,section,0x30 };
	memcpy(dataBuff + 4, pRFIDInfo, sizeof(tTIMRFIDInfo));
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 52;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_SetTicketBoxRFIDInfo: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//读取票箱数量信息
int(TIM_GetTicketBoxCountInfo)(char cTicketBoxNo, tTIMCardNum * pCardNum, tTIMDevReturn * pDevStatus)
{
	//无此命令
	return TIMTRS_RES_OK;
}
//设置票箱数量信息
int(TIM_SetTicketBoxCountInfo)(char cTicketBoxNo, tTIMCardNum * pCardNum, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_SetTicketBoxCountInfo"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxCountInfo: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pCardNum)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxCountInfo: pCardNum Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_SetTicketBoxCountInfo: pDevStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//设置票箱数量
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_SETTICKETBOXCOUNT };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 52;

	byte setBytes[6];
	setBytes[0] = pCardNum->uiCassetteACardNum % 256;
	setBytes[1] = pCardNum->uiCassetteACardNum / 256;
	setBytes[2] = pCardNum->uiCassetteBCardNum % 256;
	setBytes[3] = pCardNum->uiCassetteBCardNum / 256;
	setBytes[4] = pCardNum->uiRejectCassetteNum % 256;
	setBytes[5] = pCardNum->uiRejectCassetteNum / 256;
	memcpy(dataBuff + 1, setBytes, sizeof(setBytes));
	nDataLength = 7;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_SetTicketBoxCountInfo: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;

}
//更换票箱
int(TIM_ReplaceTBox)(char cBoxNo, char cBoxMode, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_ReplaceTBox"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_ReplaceTBox: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (cBoxNo != TIM_BOX_A && cBoxNo != TIM_BOX_B && cBoxNo != TIM_BOX_C)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_ReplaceTBox: cBoxNo 值非法,cBoxNo:%d"), cBoxNo);
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (cBoxMode != TIM_BOX_REMOVE && cBoxMode != TIM_BOX_INSTALL)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_ReplaceTBox: cBoxMode 值非法,cBoxMode:%d"), cBoxMode);
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_ReplaceTBox: pDevStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//更换票箱
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_REPLACEBOX,cBoxNo,cBoxMode };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 3;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_ReplaceTBox: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//读取序列号
int(TIM_GetSN)(char* cModuleSN, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_GetSN"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetSN: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == cModuleSN)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetSN: cModuleSN Is NULl"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_GetSN: pDevStatus Is NULl"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//获取SN
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_GETSN };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 3;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
		else
		{
			memcpy(cModuleSN, readBuff + TIM_HEAD_LEN + 3, 20);
		}
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_GetSN: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
//关闭通讯
int(TIM_CommClose)()
{
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_CommClose"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (cmdstatus.ComIsOpen)
	{
		sp_close();
		if (NULL != recvThread)
		{
			cmdstatus.ComIsOpen = FALSE;
			WaitForSingleObject(recvThread, DEFAULTINTERVAL);
			CloseHandle(recvThread);
			recvThread = NULL;
		}
		memset(&cmdstatus, 0x00, sizeof(cmdstatus));
	}
	sLogString.Format(_T("TIM_Init: ReturnCode = 0x%02x"), TIMTRS_RES_OK);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	//关闭日志
	LogApi.CloseLog();
	return TIMTRS_RES_OK;
}
//设置模块工作模式
int(TIM_SetWorkMode)(char cMode, tTIMDevReturn * pDevStatus)
{
	//无此命令
	return TIMTRS_RES_OK;
}
//模块测试设置
int(TIM_TestSetting)(char cTestType, tTIMModuleStatus * pModuleStatus, tTIMDevReturn * pDevStatus)
{
	DWORD errCode = TIMTRS_RES_OK;
	CString sLogString(_T(""));
	sLogString.Format(_T("TIM_TestSetting"));
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	if (!cmdstatus.ComIsOpen)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_TestSetting: 串口未打开"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pModuleStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_TestSetting: pModuleStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	if (NULL == pDevStatus)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_TestSetting: pDevStatus Is NULL"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	char mCTestType = 0;
	switch (cTestType)
	{
	case CMD_TEST_TICKSCRUB_MOTOR_BOXB:
	case CMD_TEST_TICKSCRUB_MOTOR_BOXA:
	case CMD_TEST_TRANSTICK_MOTOR_BOXA:
	case CMD_TEST_TRANSTICK_MOTOR_BOXB:
	case CMD_TEST_TRANSTICK_MOTOR:
	case CMD_TEST_CONT_TRANSCARD_BOXB:
	case CMD_TEST_CONT_TRANSCARD_BOXA:
	case CMD_TEST_CONT_TRANSCARD_BOXAB:
	case CMD_TEST_CONT_RETURNCARD_BOXB:
	case CMD_TEST_CONT_RETURNCARD_BOXA:
	case CMD_TEST_A_TRAY_UP:
	case CMD_TEST_A_TRAY_DOWN:
	case CMD_TEST_B_TRAY_UP:
	case CMD_TEST_B_TRAY_DOWN:
		mCTestType = cTestType;
		break;
	default:
		break;
	}
	if (mCTestType == 0)
	{
		errCode = TIMTRS_RES_FAILED;
		sLogString.Format(_T("TIM_TestSetting: mCTestType 类型有误"));
		LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	}
	//测试
	DWORD nResult, nDataLength, nReadLength;
	byte dataBuff[MAX_BUFFER_SIZE] = { TIM_CMD_TEST,mCTestType };
	byte readBuff[MAX_BUFFER_SIZE] = { 0 };
	nDataLength = 2;
	//判断errCode,如果errCode值为TIMTRS_RES_FAILED，直接退出
	if (errCode == TIMTRS_RES_FAILED)
	{
		TIM_SetReturns(errCode, NULL, pDevStatus, readBuff, MAX_BUFFER_SIZE);
		return errCode;
	}

	nResult = TIM_SentOneCommand(nDataLength, dataBuff, readBuff, nReadLength);
	if (nResult != TIMTRS_RES_OK)
	{
		nResult = TIMTRS_RES_FAILED;
	}
	else
	{
		if (readBuff[TIM_HEAD_LEN] != dataBuff[0])
			nResult = TIMTRS_RES_FAILED;
	}
	TIM_SetReturns(nResult, NULL, pDevStatus, readBuff + TIM_HEAD_LEN, MAX_BUFFER_SIZE);
	sLogString.Format(_T("TIM_TestSetting: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	//获取状态
	nResult = TIM_GetDevStatus(pModuleStatus, pDevStatus);
	sLogString.Format(_T("TIM_GetDevStatus: ReturnCode = 0x%02x"), nResult);
	LogApi.WriteLog(_T(""), CString(__FUNCTION__), __LINE__, sLogString);
	return nResult;
}
/**
 * [TIM_TrayContral 票箱托盘控制]
 * @param  p_iTrayType [托盘序号]
 * @param  p_iCommand  [动作参数]
		p_iTrayType     托盘序号 1:A卡箱  2:B卡箱  0xff 全部
		int		p_iCommand      动作参数 1:下降   2上升
 * @param  pDevStatus  [命令执行结果]
 * @return             [成功返回0 其余失败]
 */
 //int TIM_TrayContral(int p_iTrayType, int p_iCommand, tTIMDevReturn* pDevStatus)
 //{
 //	return 0;
 //}