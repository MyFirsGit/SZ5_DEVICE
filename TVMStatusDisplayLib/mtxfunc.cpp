#include <stdio.h>
#include "common.h"
#include "mtxfunc.h"

EventCallback   g_OnEvent=NULL;

void  WINAPI SetEventCallback(EventCallback func)   
{    

      g_OnEvent   =   func;

}   

void WINAPI FireEvent()
{
	if(g_OnEvent)
		g_OnEvent();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将数据缓存转换为字符串(32个字节一行,16个字符空一格)

@param      (o)char* lpResult		结果字符串
@param      (i)int iMaxTextLen		字符串最大长度
@param      (i)const BYTE* lpBuffer 数据缓存指针
@param      (i)int iLen				数据缓存长度

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void Conv2Str(char* lpResult, int iMaxTextLen, const BYTE* lpBuffer,int iLen)
{
	char szText[12] = {0};
	int iTextLen = 0;
	BYTE bi;

	__try{
		strcpy_s(lpResult,iLen,"\r\n");
		for (int i=0; i<iLen; i++){
			bi = lpBuffer[i];
			// 32个字符一行
			if ((i != 0) && (i % 16 == 0)){
				sprintf_s(szText, 12,"\r\n%02X ", bi);
			}
			// 每16个字符加一个空格
			else if ((i != 0) && (i % 8 == 0)){
				sprintf_s(szText, 12,"  %02X ", bi);
			}
			// 正常情况
			else{
				sprintf_s(szText, 12,"%02X ", bi);
			}
			// 超过最大长度的字符丢弃
			iTextLen += strlen(szText);
			if (iTextLen < iMaxTextLen){
				strcat_s(lpResult, iLen,szText);
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		strcpy_s(lpResult,iMaxTextLen, "Exception");
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写数据到串口或模拟通信口

@param      (i)HANDLE hStream			串口句柄
@param      (i)BYTE *pBuffer			写入数据指针
@param      (i)DWORD dwLength			写入数据长度
@param      (o)DWORD *pdwActualWrite	实际写入的数据长度
@param      (i)BYTE bCommunicationMethod 通信方式
@param      (i)HANDLE hSimObject		模拟器句柄  
@param      (i)BYTE *sSlotName          邮槽名字    

@retval     BOOL    TRUE:成功  FALSE:失败    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL WriteHtdl(HANDLE hStream, BYTE *pBuffer, DWORD dwLength, DWORD *pdwActualWrite,BYTE bCommunicationMethod, HANDLE hSimObject,BYTE *sSlotName)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("WriteHtdl - Enter");
#endif
	BOOL	bRet = TRUE;
	DWORD	i;

	// 邮槽通信
	if(bCommunicationMethod == MAIL_SLOT){
		for( i = 0; i < dwLength; i++ ){
			hSimObject=::CreateFile((TCHAR *)sSlotName,GENERIC_WRITE,FILE_SHARE_WRITE | FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);								//
			if(hSimObject == INVALID_HANDLE_VALUE ){
				bRet = FALSE;									
				break;											
			}													
			else{
				bRet = ::WriteFile(hSimObject,pBuffer,1,pdwActualWrite, NULL);
				CloseHandle(hSimObject);								
				pBuffer++;										
			}													
		}														
	}
	// 串口通信
	else if(bCommunicationMethod == COM_PORT){
		bRet = WriteFile(hStream,pBuffer,dwLength,pdwActualWrite,NULL);
		FlushFileBuffers(hStream);
	}
	// 管道通信
	else if(bCommunicationMethod == NAME_PIPE){
		OVERLAPPED olwrite;
		::memset(&olwrite, 0, sizeof(olwrite));
		olwrite.Offset = 0;
		olwrite.OffsetHigh = 0;
		// 生成事件对象		
		olwrite.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		bRet = WriteFile(hSimObject,pBuffer,dwLength,pdwActualWrite,&olwrite);
	}
#ifdef _TRACE_SEND_DATA_
	Conv2Str(m_szText, MAX_TRACE_LEN, pBuffer, *pdwActualWrite);
	CHTrs_Trace("WriteHtdl - written %d bytes\r\n", *pdwActualWrite);
	CHTrs_Trace(m_szText);
#endif

#ifdef _TRACE_HTDL_
	CHTrs_Trace("WriteHtdl - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      从串口或邮槽读取数据

@param      (i)HANDLE hStream			串口句柄
@param      (i)BYTE *pBuffer			从串口读取的数据指针
@param      (i)DWORD dwLength			读取数据长度
@param      (o)DWORD *pdwBytesRead		实际读取的数据长度
@param      (i)BYTE bCommunicationMethod	通信方式
@param      (i)HANDLE hSimObject				模拟器句柄   
@param      (i)BYTE *pMailBuffer		从模拟通信口读取的数据指针

@retval     BOOL    TRUE:成功  FALSE:失败    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL ReadHtdl(HANDLE hStream, BYTE *pBuffer, DWORD dwLength, DWORD *pdwBytesRead,BYTE bCommunicationMethod, HANDLE hSimObject, BYTE *pMailBuffer)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("ReadHtdl - Enter");
#endif
	DWORD	nRead;
	BOOL	bRet;
	// 邮槽
	if(bCommunicationMethod == MAIL_SLOT){
		BYTE	tmpbuf[MAX_RECVBUFF];
		DWORD	buflen, ret;
		int		i, n;
		bRet = 0;	
		*pdwBytesRead = 0;	
		tmpbuf[0] = NULL;
		buflen = 0;	
		do{
			if(strlen( (char *)pMailBuffer ) == 0){
				bRet = ReadFile(hSimObject,pMailBuffer,MAX_RECVBUFF,&nRead,NULL);
				if(bRet == 0 ){
					ret = GetLastError();	
					if (ret ==  ERROR_SEM_TIMEOUT){
						bRet = 1;
					}		
					break;
				}											
				if( nRead == 0 ){	
					break;
				}
				buflen += nRead;
				pMailBuffer[nRead] = NULL;	
			}											
			else{
				buflen += (DWORD)strlen( (char *)pMailBuffer );	
			}	

			if( buflen >= dwLength ){	
				strcat_s((char *)tmpbuf, MAX_RECVBUFF,(char *)pMailBuffer );
				strncpy_s( (char *)pBuffer, dwLength,(char *)tmpbuf, dwLength );
				n = 0;									
				for( i = dwLength; i < (int)strlen( (char *)tmpbuf ); i++, n++ ){
					pMailBuffer[n] = tmpbuf[i];
				}
				pMailBuffer[n] = NULL;
				buflen -= dwLength;
				*pdwBytesRead = dwLength;
			}
			else{			
				strcat_s( (char *)tmpbuf,MAX_RECVBUFF, (char *)pMailBuffer );
				pMailBuffer[0] = NULL;
			}
		}
		while( *pdwBytesRead < dwLength );
	}
	// 串口
	else if(bCommunicationMethod == COM_PORT){
		DWORD dwErrorMask  = 0L;
		COMSTAT comstat;
		// 清除串口错误
		::ClearCommError(hStream, &dwErrorMask, &comstat);
		// 读取数据
		bRet = ::ReadFile(hStream, pBuffer, dwLength, pdwBytesRead, NULL);
	}
	// 管道
	else if(bCommunicationMethod == NAME_PIPE){
		OVERLAPPED olread;	
		::memset(&olread, 0, sizeof(olread));		
		olread.Offset = 0;
		olread.OffsetHigh = 0;
		// 生成事件对象 
		olread.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		bRet = ::ReadFile(hSimObject, pBuffer, dwLength, pdwBytesRead, &olread);
	}

#ifdef _TRACE_HTDL_
	CHTrs_Trace("ReadHtdl - Exit - return %d", 0);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新服务状态

@param      (i)HTDLWORK *pWork          工作空间结构指针
@param      (i)DWORD dwStatus			服务状态码
@param      (i)BOOL bOn					TRUE:开  FALSE:关

@retval     BOOL    TRUE:成功  FALSE:失败    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void UpdateStatus(HTDLWORK *pWork,DWORD dwStatus,BOOL bOn)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("UpdateStatus - Enter - [%08X] = %d", dwStatus, bOn);
#endif
	DWORD nWait = ::WaitForSingleObject(pWork->hMutex, INFINITE);
	if (nWait != WAIT_FAILED){
		// 开
		if(TRUE == bOn){
			pWork->DrvStat |= dwStatus;
		}
		// 关
		else{
			pWork->DrvStat &= ~dwStatus;
		}
		::ReleaseMutex(pWork->hMutex);
	}

#ifdef _TRACE_HTDL_
	CHTrs_Trace("UpdateStatus - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清空串口发送缓存

@param      (i)HTDLWORK *pWork   工作空间结构指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void SendClear(HTDLWORK *pWork)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SendClear - Enter");
#endif

	::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_TXCLEAR);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SendClear - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      激活命令发送完成事件

@param      (i)HTDLWORK *pWork   工作空间结构指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void SetEventSendFin(HTDLWORK *pWork)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SetEventSendFin - Enter");
#endif
	
	::SetEvent(pWork->hFinSend);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SetEventSendFin - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据接收缓存以接收新的数据

@param      (i)HTDLWORK *pWork   工作空间结构指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void InitRecvBuff(HTDLWORK *pWork)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("InitRecvBuff - Enter");
#endif

	// 清空数据接收缓存
	pWork->RecvCnt = 0;
	pWork->RecvPtr = pWork->RecvBuff;
	memset(pWork->RecvBuff, 0, sizeof(pWork->RecvBuff));
	// 更新服务状态为等待接收
	UpdateStatus(pWork, ST_RCVEXE, TRUE);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("InitRecvBuff - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      准备发送数据包

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)BYTE* lpData      发送数据指针(开始位置)
@param      (i)int iSendLen      发送数据长度

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void PreSendData(HTDLWORK *pWork, BYTE* lpData, int iSendLen)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("PreSendData - Enter");
#endif
	// 发送数据
	BYTE* p = pWork->SendBuff;
	_ASSERTE(NULL != p);
	_ASSERTE(NULL != lpData);

	// 临时存放(协议头 + 包头 + 长度 + 包体 + 校验码)
	BYTE* pTemp = pWork->SendTemp;//new BYTE[8 + 12  +iSendLen+ 数据域 + 2 + 1];
	BYTE* pHeadData = pTemp;
	// 协议头 8
	memset(pTemp,STX,8);
	pTemp += 8;

	// 包头 12
	*pTemp++ = 0x01;
	*pTemp++ = 0x00;
	*pTemp++ = 0x00;
	*pTemp++ = 0x80;
	*pTemp++ = 0xf0;
	*pTemp++ = 0x00;
	*pTemp++ = 0x56;
	*pTemp++ = 0x10;
	*pTemp++ = 0x00;
	*pTemp++ = 0x00;
	*pTemp++ = 0x00;
	*pTemp++ = 0x00;

	// 数据长度 2
	DWORD dwDataLen = iSendLen;
	memmove(pTemp,&iSendLen,sizeof(DWORD));
	pTemp += 4;


	// 数据内容 N
	memmove(pTemp,lpData,iSendLen);
	pTemp += iSendLen;

	// 计算CRC校验码 2
	WORD wCrcCode = CalcCRC(pHeadData + 8,16 + iSendLen);
	memmove(pTemp,&wCrcCode,sizeof(WORD));
	pTemp += sizeof(WORD);

	// 开始拷贝
	memmove(p,pHeadData,20);
	pWork->SendLength = 20;
	p += 20;
	// 其他的需要转译检测(长度加上数据体和校验位)
	for (int i = 20; i< iSendLen + 8 + 14 + 2; i++){
		// 0xa5---To---0xa6 0x02
		if (STX == pHeadData[i]){
			*p++ = 0xa6;
			pWork->SendLength++;
			*p++ = 0x02;
			pWork->SendLength++;
		} 
		// 0x5a---To---0x5b 0x02
		else if(ETX == pHeadData[i]){
			*p++ = 0x5b;
			pWork->SendLength++;
			*p++ = 0x02;
			pWork->SendLength++;
		}
		// 0xa6---To---0xa6 0x01
		else if(0xa6 == pHeadData[i]){
			*p++ = 0xa6;
			pWork->SendLength++;
			*p++ = 0x01;
			pWork->SendLength++;
		}
		// 0x5b---To---0x5b 0x01
		else if(0x5b == pHeadData[i]){
			*p++ = 0x5b;
			pWork->SendLength++;
			*p++ = 0x01;
			pWork->SendLength++;
		}
		else{
			*p++ = pHeadData[i];
			pWork->SendLength++;
		}
	}


	// 将CRC数据添加到包尾
	//memmove(p,&wCrcCode,sizeof(WORD));
	//pWork->SendLength += sizeof(WORD);

	// 包尾
	*p++ = ETX;
	pWork->SendLength ++;


	/*if(NULL != pTemp){
	delete [] pTemp;
	pTemp = NULL;
	}*/
	
	/*
	// 拼帧头
	memset(p,STX,8);
	p+=8;
	pWork->SendLength = 8;
	// 拼包头
	BYTE* pPackageHead=p;	//记住包头位置
	*p++=0x01;*p++=0x00;
	*p++=0x00;*p++=0x80;
	*p++=0x00;*p++=0x00;*p++=0x00;*p++=0x00;*p++=0x00;*p++=0x00;
	*p++=0x51;*p++=0x01;
	WORD wSendLen=iSendLen;
	//wSendLen=htons(wSendLen);
	memmove(p,&wSendLen,sizeof(WORD ));
	p+=2;
	//*p++=0x05;*p++=0x00;
	pWork->SendLength += 14;
	// 拼包体
	for (int i=0; i< iSendLen; i++){
		// 透过模式处理
		if (STX == lpData[i])
		{//遇到 0xa5转义成 0xa6,0x02
			*p++ = 0xa6;
			pWork->SendLength++;
			*p++ = 0x02;
			pWork->SendLength++;
		} else if(ETX == lpData[i])
		{//遇到 0x5b 转义成 0x5b 0x02
			*p++ = 0x5b;
			pWork->SendLength++;
			*p++ = 0x02;
			pWork->SendLength++;
		}else if(0xa6 == lpData[i])
		{//遇到 0xa6 转义成 0xa6 0x01
			*p++ = 0xa6;
			pWork->SendLength++;
			*p++ = 0x01;
			pWork->SendLength++;
		}else if(0x5b == lpData[i])
		{//遇到 0x5b 转义成 0x5b 0x01
			*p++ = 0x5b;
			pWork->SendLength++;
			*p++ = 0x01;
			pWork->SendLength++;
		}
		else
		{
			*p++ = lpData[i];
			pWork->SendLength++;
		}
	}
	//包校验
	//指针偏移算出一共需要计算多少字节
	unsigned int uiCrcCount=p-pPackageHead;
	WORD crc16Code=CalcCRC(pPackageHead,uiCrcCount);
	memmove(p,&crc16Code,sizeof(WORD ));
	p+=sizeof(WORD );
	pWork->SendLength +=sizeof(WORD );
	//拼包尾
	*p++ = ETX;
	pWork->SendLength ++;
	// 计算BCC
	//*p = BccCalc(lpData, iSendLen);
	*/
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("PreSendData - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送命令

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSend - Enter");
#endif
	InitRecvBuff(pWork);
	SENDITEM* pSendItem = &pWork->SndItem;
	int iRemainLen;
	DWORD dwActualWrite;
	int iNext;

	//发送未完成
	if ((*pSendItem->pOutLength) < pSendItem->nSendLength){		
		// 计算数据块序号
		//UpdateBlockNo(pWork, pSendItem);
		// 设置发送开始位置
		pSendItem->pSending += pSendItem->wLastSendingLen; 
		// 设置数据发送剩余长度
		iRemainLen = pSendItem->nSendLength - (*pSendItem->pOutLength);
		// 设置最后一次发送的数据长度
		pSendItem->wLastSendingLen = MAX_MSG_LEN;
		// 最后一个数据块
		if(iRemainLen < MAX_MSG_LEN){
			pSendItem->wLastSendingLen = iRemainLen;
		}
		// 准备发送数据
		PreSendData(pWork, pSendItem->pSending, pSendItem->wLastSendingLen);
		
		pWork->RetryCnt = 0;
		// 发送数据
		WriteHtdl(pWork->hComm,pWork->SendBuff,pWork->SendLength,&dwActualWrite,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);

		// 写日志
        if(pSendItem->wLastSendingLen > 0){			
            char log[MAX_BUFFER_SIZE*2] = {0};
			log[0] = '>';
			log[1] = ' ';
			for(unsigned short i=0;i < pSendItem->wLastSendingLen; i++) {
				sprintf_s(&log[2*i+2],3,"%02x",pSendItem->pSending[i]);
			}
			// 打印日志
			CTSPTrs_Trace(log);
		}
		// 更新已发送数据字节长度
		(*pSendItem->pOutLength) += pSendItem->wLastSendingLen;
		iNext = pMatrix->next[1];
		*(pSendItem->pResult) = RES_OK;
		pWork->ReqSend = false;

		//激活命令发送完成事件
		SetEventSendFin(pWork);
	}
	// 发送已完成
	else{
		iNext = pMatrix->next[0];
		*(pSendItem->pResult) = RES_OK;
		pWork->ReqSend = false;

		//激活命令发送完成事件
		SetEventSendFin(pWork);
	}

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxSend - Goto %d", pMatrix->phase, pMatrix->event, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSend - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送命令，三次重试

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySend - Enter");
#endif
	int next;
	DWORD dwActualWrite;		// 实际发送数据长度
	pWork->RetryCnt++;
	if (pWork->RetryCnt < MAX_RETRYSEND){
		if (pWork->SendLength >0){
			WriteHtdl(pWork->hComm,pWork->SendBuff,pWork->SendLength,&dwActualWrite,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);
		}
		next = pMatrix->next[1];
	}
	else{
		// 清空串口发送缓存
		SendClear(pWork);
		pWork->ReqSend = false;
		next = pMatrix->next[0];
		// 更新服务状态为发送失败
		UpdateStatus(pWork, ST_SNDERR, TRUE);
		// 设置命令执行结果
		*(pWork->SndItem.pResult) = MTX_ERR_SEND;
		// 激活命令发送完成事件
		SetEventSendFin(pWork);
	}

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRetrySend - Goto %d", pMatrix->phase, pMatrix->event, next);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySend - Exit - return %d", next);
#endif
	return next;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送命令，三次重试超时

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxTimeOutSend - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutSend", pMatrix->phase, pMatrix->event);
#endif
	int iNext = mtxRetrySend(pWork, pMatrix);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxTimeOutSend - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      接收超时处理

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxTimeOutRecv - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutRecv - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxTimeOutRecv - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      接收一个字节

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxOnChar - Enter");
#endif
	int iNext;
	// 接收长度正常
	if (pWork->RecvCnt < MAX_RECVBUFF){
		*(pWork->RecvPtr++) = pWork->c;
		pWork->RecvCnt++;
		iNext = pMatrix->next[1];
	}
	// 接收数据缓存溢出
	else{
		iNext = pMatrix->next[0];
		UpdateStatus(pWork, ST_RCVOVR, TRUE);
	}

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxOnChar %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxOnChar - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清除数据接收缓存以接收新的数据

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxReRecv - Enter");
#endif
	
	InitRecvBuff(pWork);

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxReRecv - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxReRecv - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      无处理

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxDummy - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxDummy - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif
	
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxDummy - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      通信协议错误处理

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxProtErr - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxProtErr - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif
	
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxProtErr - Exit - return %d", pMatrix->next[1]);
#endif
	//Do some error reports if any
	return pMatrix->next[1];
}

inline BYTE* memstr(const BYTE* full_data, int full_data_len, const BYTE* substr,int substr_len)  
{  
	if (full_data == NULL || full_data_len <= 0 || substr == NULL) {  
        return NULL;  
    }  
  
    if (*substr == '\0') {  
        return NULL;  
    }
  
    int i;  
    BYTE* cur = (BYTE*)full_data;  
    int last_possible = full_data_len - substr_len + 1;  
    for (i = 0; i < last_possible; i++) {  
        if (*cur == *substr) {  
            //assert(full_data_len - i >= sublen);  
            if (memcmp(cur, substr, substr_len) == 0) {  
                //found  
                return cur;  
            }  
        }  
        cur++;  
    }  
  
    return NULL;  
}  

inline BOOL unConvStr(BYTE* pData,	//源数据
	const UINT uiLength,
	const BYTE* pNeedUnConv,				//需要被替换的byte数组
	const UINT uiUnConvLength,
	const BYTE* pConvto,					//替换之后的byte数组
	const UINT uiConvtoLength,
	UINT &uiNewNum,							//转换之后byte数组的字节数
	UINT dwNewBufferSize=0,
	BYTE* pNew=NULL							//存放转换之后的byte数组
	)							
{
	//如果没有指定新的存放区，就会更改源数据内存区
	//如果指定了新的存放区，不会更改源数据内存区，
	BOOL bret=TRUE;
	BOOL bNeedFreeNew=FALSE;
	if(NULL == pNew)
	{
		pNew=(BYTE*)malloc(sizeof(BYTE)*uiLength);
		if(NULL!=pNew)
		{
			//写好释放
			dwNewBufferSize=uiLength;
			bNeedFreeNew=TRUE;
		}else
		{
		}
	}else
	{
	}
	if(uiConvtoLength<=uiUnConvLength)
	{
		int icount=0;
		UINT slen=uiLength;
		memmove(pNew,pData,uiLength);
		BYTE* pNewWork=pNew;
		BYTE* pConv=NULL;
		pConv=memstr(pNewWork,uiLength,pNeedUnConv,uiUnConvLength);
		while(NULL!=pConv)
		{
			int ileft=pConv-pNewWork;
			int iright=slen-ileft;
			int iRemoveConv=iright-uiUnConvLength;
			slen-=uiUnConvLength;
			if(slen>dwNewBufferSize)
			{
				bret=FALSE;
				break;
			}
			BYTE* pRemoveConv=pConv+uiUnConvLength;
			memmove(pConv,pConvto,uiConvtoLength);
			slen+=uiConvtoLength;
			memmove(pConv+uiConvtoLength,pRemoveConv,iRemoveConv);
			pConv=memstr(pConv+uiConvtoLength,slen,pNeedUnConv,uiUnConvLength);
		}
		uiNewNum=slen;
	}else
	{
	}
	if(bNeedFreeNew)
	{
		memset(pData,0,uiNewNum);
		memmove(pData,pNew,uiNewNum);
		//需要释放中途产生的内存消耗
		free(pNew);
		pNew=NULL;
	}
	return bret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      BCC验证处理

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxCalcCrc16(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - Enter");
#endif
	int iNext;
	
	// 接收字节数大于256
	if (pWork->RecvCnt > MAX_MSG_LEN){
		return pWork->Phase;
	}

	// 计算BCC
	//目前的策略是读取第一个非0xa5的字节开始 包头一直到最后两个校验位结束 包括了两个校验位
	//crc校验的时候从第一个开始，到最后两个校验位之前
	WORD crc16Code = CalcCRC(pWork->RecvBuff, pWork->RecvCnt-2);

#ifdef _TRACE_SEND_DATA_
	Conv2Str(m_szText, MAX_TRACE_LEN, pWork->RecvBuff, pWork->RecvCnt);
	CHTrs_Trace("Check BCC - SEQ [%02X] - Block No [%02X]", pWork->SeqRecv, pWork->BlockNumRecv);
	CHTrs_Trace(m_szText);
#endif

	// Crc16 校验
	WORD wRecvCrcCode=0x0000;
	memmove(&wRecvCrcCode,pWork->RecvBuff+(pWork->RecvCnt-2),2);
	if(wRecvCrcCode==crc16Code)
	{
		//计算正确之后需要 反转义
		unsigned char UnConvA5[2]={0xa6,0x02};unsigned char ConvtoA5[1]={0xa5};
		unsigned char UnConvA6[2]={0xa6,0x01};unsigned char ConvtoA6[1]={0xa6};
		unsigned char UnConv5A[2]={0x5b,0x02};unsigned char Convto5A[1]={0x5a};
		unsigned char UnConv5B[2]={0x5b,0x01};unsigned char Convto5B[1]={0x5b};
		BOOL bUnConv=FALSE;
		bUnConv=unConvStr(pWork->RecvBuff,pWork->RecvCnt-2,UnConvA5,2,ConvtoA5,1,pWork->RecvCnt);
		bUnConv=unConvStr(pWork->RecvBuff,pWork->RecvCnt-2,UnConvA5,2,ConvtoA5,1,pWork->RecvCnt);
		bUnConv=unConvStr(pWork->RecvBuff,pWork->RecvCnt-2,UnConvA5,2,ConvtoA5,1,pWork->RecvCnt);
		bUnConv=unConvStr(pWork->RecvBuff,pWork->RecvCnt-2,UnConvA5,2,ConvtoA5,1,pWork->RecvCnt);
		if(bUnConv==TRUE)
		{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - OK - Send ACK");
#endif
			// 接收数据成功
			if(TRUE == RecvData(pWork, pWork->RecvBuff, pWork->RecvCnt))
			{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - Receive block %04X - LEN = %04X", pWork->BlockNumRecv, pWork->RecvCnt);
#endif
				iNext = pMatrix->next[1];
			}
			// 接收数据失败
			else{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - RecvData Fail - Send NAK");
#endif	
				// 发送NAK
				iNext = pMatrix->next[0];
			}
		}else
		{
			//反转义字符失败
		}
	}
	// crc校验失败
	else{
#ifdef _TRACE_MAXTRIX_
		CHTrs_Trace("mtxCheckBcc - NG - Send NAK");
#endif
		iNext = pMatrix->next[0];
	}
	//如果是显示完成反馈 则做一下特殊处理 因为目前指令集中 显示完成反馈内容校验不正确
	BYTE btCmd[19]={0x00, 0x80, 0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
					0x00 ,0x00 ,0x51 ,0x00 ,0x05 ,0x00 ,0xA0 ,0x00 ,0x12 ,0x00 ,0x00 };
	if(memcmp(pWork->RecvBuff,btCmd,19)==0)
	{//发送一个显示完成消息
		FireEvent();
		iNext = pMatrix->next[1];
	}
	InitRecvBuff(pWork);
	
#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxCheckBcc - Goto %d", pMatrix->phase, pMatrix->event, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - Exit - return %d", iNext);
#endif

	return iNext;
}