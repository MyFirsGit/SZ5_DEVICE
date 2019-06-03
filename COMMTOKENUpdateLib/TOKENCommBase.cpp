#include "StdAfx.h"
#include "TOKENCommBase.h"
#include "common.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommBase::CTOKENCommBase(void)
{
	m_pConn = new ComConnect();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommBase::~CTOKENCommBase(void)
{
	if (m_pConn != NULL)
	{
		delete	m_pConn;
		m_pConn = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		TOKEN固件下载

@param      (i)DWORD port 端口号
@param      (i)DWORD speed 波特率
@param      (i)char *data文件内容
@param      (i)long bufferLength文件内容长度

@retval     int 0:执行成功，非0:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::TOKENUpdate(DWORD comPort,DWORD speed, char* data,long bufferLength)
{
	// 打开串口
	TOKEN_CONNECT_PARAM connectParm;
	connectParm.ComPort = comPort;
	connectParm.Speed = speed;
	connectParm.MaxSendCnt = MAX_CONN_SEND; 
	connectParm.MaxRecvCnt = MAX_CONN_SEND; 
	connectParm.MaxSendBuffSize = MAX_BUFFER_SIZE;
	connectParm.MaxRecvBuffSize = MAX_BUFFER_SIZE;
	int retOpen = Open(connectParm,m_hCom);
	if (retOpen != TOKENTRS_RES_OK)
	{
		return retOpen;
	}

	// 发送开始下载命令
	BYTE startDownload[10] = {0};
	startDownload[0] = 0x31;
	startDownload[1] = 0x00;
	if (ExecuteCmd(startDownload,2) != TOKENTRS_RES_OK)
	{
		// 发送开始下载备用命令
		BYTE startDownloadEx[10] = {0};
		startDownloadEx[0] = 0xF0;
		startDownloadEx[0] = 0X00;
		int retStartDownLoadEx = ExecuteCmd(startDownloadEx,2);
		if (retStartDownLoadEx != TOKENTRS_RES_OK)
		{
			return retStartDownLoadEx;
		}
	}	

	//// 读取文件
	//FILE *fp;
	//fp = fopen(fullFilePath,"rb"); 
	//if (NULL == fp)
	//{
	//	return MIMTRS_OPEN_FILE_FAIL;
	//}
	//fseek(fp,0,SEEK_END);		//定位到文件末 
	//long fileSize = ftell(fp);	//文件长度 
	//fseek(fp,0,SEEK_SET);		//定位到文件头
	//char *fileContent = new char[fileSize];
	//size_t readCount = fread(fileContent,1,fileSize,fp);
	//if (0 == readCount)
	//{
	//	return MIMTRS_READ_FILE_FAIL;
	//}
	// 分割块数
	long blockCnt = bufferLength/64;
	int surplusLen = bufferLength%64;
	if (0 != surplusLen) 
	{
		blockCnt++;
	}
	// 下发固件总数据量
	BYTE downloadCount[6] = {0};
	downloadCount[0] = 0xE0;
	downloadCount[1] = 0x04;
	char lenData[4] = {0};
	long fileLlen = lrev(bufferLength);
	memcpy(lenData, &fileLlen, sizeof(fileLlen));
	memcpy(downloadCount+2,lenData,4);

	int retDownloadCount = ExecuteCmd(downloadCount,6);
	if (retDownloadCount != TOKENTRS_RES_OK)
	{
		return retDownloadCount;
	}

	// 发送固件代码下发命令
	int retDownloadFile = CmdDownLoadFile(data,bufferLength,blockCnt);
	if (retDownloadFile != TOKENTRS_RES_OK)
	{
		return retDownloadFile;
	}

	//	Sleep(2000);
	// 发送固件下发完成命令
	BYTE downloadEnd[10] = {0};
	downloadEnd[0] = 0xE2;
	downloadEnd[1] = 0x00;
	int retDownLoadEnd = ExecuteCmd(downloadEnd,2);
	if (retDownLoadEnd != TOKENTRS_RES_OK)
	{
		return retDownLoadEnd;
	}

	Sleep(5000);
	// 发送切换到正常程序命令
	BYTE switchToNormal[10] = {0};
	switchToNormal[0] = 0xE9;
	switchToNormal[1] = 0x00;
	int retSwitchToNormal = ExecuteCmd(switchToNormal,2);
	if (retSwitchToNormal != TOKENTRS_RES_OK)
	{
		return retSwitchToNormal;
	}
	// 关闭串口
	ClosePort(m_hCom);

	return TOKENTRS_RES_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      下载文件到TOKEN

@param      (i)char *fileContent 文件内容
@param      (i)long fileSize 文件长度
@param      (i)long blockCnt 文件分块数量

@retval     int  0:成功  非0: 失败     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int  CTOKENCommBase::CmdDownLoadFile(char *fileContent,long fileSize,int blockCnt)
{		
	int retDownload = TOKENTRS_RES_OK;
	for (int i=0;i<blockCnt;i++) 
	{
		// Firmware数据
		BYTE firmwareData[66] = {0};
		firmwareData[0] = 0xE1;
		if ( i == blockCnt-1) 
		{			
			firmwareData[1] = fileSize-64*(blockCnt-1);
			int buffLen = fileSize-64*(blockCnt-1);
			memcpy(firmwareData+2,fileContent+64*i,buffLen);
			retDownload = ExecuteCmd(firmwareData,buffLen+2);
		}
		else 
		{
			firmwareData[1] = 0x40;
			memcpy(firmwareData+2,fileContent+64*i,64);
			retDownload = ExecuteCmd(firmwareData,64+2);
		}
		// 中间更新失败则停止更新
		if(TOKENTRS_RES_OK != retDownload)
		{
			return retDownload;
		}
	}	
	return TOKENTRS_RES_OK;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      命令执行

@param      (i)LPBYTE lpSendBuffer 命令
@param      (i)long wSendLength 命令长度

@retval     int  0:成功  非0: 失败     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::ExecuteCmd(LPBYTE lpSendBuffer,long wSendLength)
{
	int result = TOKENTRS_RES_OK;
	WORD wRecvLen;
	bool bSentDone;
	BYTE lpReceivedBuffer[MAX_BUFFER_SIZE];        // 接收数据
	WORD pwReceivedLength = 0;         // 接受数据长度		
	result = TOKENTRS_RES_OK;
	bSentDone = false;
	wRecvLen = 0;
	int reTry = MAX_RETRY_COUNT;
	do 
	{
		do{
			if(false == bSentDone){
				Sleep(50);
				result = WriteOneMsg(lpSendBuffer, wSendLength,m_hCom);
				bSentDone = true;
			}
			else{
				Sleep(50);
				result = ReadOneMsg(lpReceivedBuffer, &pwReceivedLength,5000,m_hCom);	
				// 读成功
				if (result==TOKENTRS_RES_OK){
					// 应答完成
					if(CheckResponse(lpSendBuffer[0], lpReceivedBuffer[1]))
					{
						break;
					}					
				}
				else
				{
					result = TOKENTRS_CMD_EXECUTE_FAIL;
				}
			}
			reTry--;
		} while (1);
	} while (result != TOKENTRS_RES_OK && reTry>0);

	return result;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查应答命令是否完成

@param      (i)BYTE CmdSeq  反馈数据中的命令序列号  

@retval     bool  true:完成  false: 未完成     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTOKENCommBase::CheckResponse(BYTE sendCommand,BYTE recvCommand)
{
	bool bReturn = false;

	// 检查是否应答完成
	switch(sendCommand){	
	case CMD_STATR_DOWNLOAD:		// IAP 下载开始			
	case CMD_STATR_DOWNLOAD_EX:		// 备用IAP 下载开始
		if ( recvCommand == RSP_START_DOWNLOAD_NORMAL ) {
			bReturn = true; 
		}
		break;			
	case CMD_DOWNLOAD_COUNT:		// 下发固件总数据量		
	case CMD_DOWNLOAD_FILE:			// 固件代码下发	
	case CMD_DOWNLOAD_END:			// 固件下发完成
		if ( recvCommand == RSP_DOWNLOAD_NORMAL ) {
			bReturn = true; 
		}
		break;	
	case CMD_SWITCH_TO_NORMAL:		// 切换到正常程序
		if ( recvCommand == RSP_SWITCH_TO_NORMAL_NORMAL ) {
			bReturn = true; 
		}
		break;		
	default:
		break;			
	}
	return bReturn;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      打开串口

@param      (i)TOKEN_CONNECT_PARAM Para 串口参数
@param      (o)HANDLE& hCom串口句柄

@retval     int  0:成功  非0: 失败     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::Open(TOKEN_CONNECT_PARAM Para,HANDLE& hCom)
{
	int iRet = m_pConn->open(Para,hCom);
	if(iRet != 0){
		//	theTOKEN_TRACE->WriteData(_T("Open Port Error。"),NULL,0);
		return iRet;
	}
	//	theTOKEN_TRACE->WriteData(_T("Open Port OK。"),NULL,0);
	return TOKENTRS_RES_OK;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭串口

@param      无

@retval     int  0:成功  非0: 失败     

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTOKENCommBase::ClosePort(HANDLE& hCom)
{
	m_pConn->close(hCom);
	//theTOKEN_TRACE->WriteData(_T("Close Port。"),NULL,0);
	return TOKENTRS_RES_OK;
}

//****************************************************************************
// 发送一条命令，不读取反馈信息,用于TOKEN测试功能中发送条数据，反馈多条数据情况，必须和读反馈信息配对使用
// function:	WriteOneMsg  
// input:		char *dataBuf				待发送到数据
//				UINT16 nSendLen				发送数据的长度
// output:		int 						发送到的数据长度 非0：正确；0：发生错误
//****************************************************************************
int CTOKENCommBase::WriteOneMsg(LPBYTE dataBuf,UINT16 nSendLen,HANDLE hCom)
{
	BYTE sendBuf[BUFF_SIZE];
	memset(sendBuf,0,BUFF_SIZE);
	LPBYTE lpData = sendBuf;
	*lpData = STX;
	lpData++;
	memcpy(lpData,dataBuf,nSendLen);
	lpData += nSendLen;
	*lpData = BccCalc((LPBYTE)sendBuf+1, nSendLen);
	lpData++;
	int iRet = m_pConn->write(nSendLen+2, sendBuf,hCom);
	if(iRet!=0){
		//theTOKEN_TRACE->WriteData(_T("->"),(LPBYTE)sendBuf,iRet);
	}
	return iRet;
}
//****************************************************************************
// 读取反馈信息，用于TOKEN测试功能中发送条数据，反馈多条数据情况
// function:	WriteOneMsg  
// input:		char *dataBuf				待发送到数据
//				UINT16 nSendLen				发送数据的长度
// output:		int 						接收到的数据长度 0：正确；非0：发生错误
//****************************************************************************
int CTOKENCommBase::ReadOneMsg(LPBYTE rbuf,UINT16 *nrLen,DWORD WaitTimeMax,HANDLE hCom)
{	
	int  datalen = 0;                            // 一次读串口接收到的数据长度
	BYTE readBuffer[BUFF_SIZE];

	memset(readBuffer,0x00,sizeof(readBuffer));

	// 读取开始符
	datalen = m_pConn->read(RECEIVE_DATA_LENGTH, readBuffer,hCom);

	if((datalen<=0) || (readBuffer[0] != STX))
	{ 	
		return TOKENTRS_RECV_EMPTY;
	}
	else
	{
		memcpy(rbuf,readBuffer,datalen);
	}
	return TOKENTRS_RES_OK;
}
