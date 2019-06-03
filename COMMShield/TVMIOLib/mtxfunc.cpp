#include <stdio.h>
#include "common.h"

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
		strcpy(lpResult, "\r\n");
		for (int i=0; i<iLen; i++){
			bi = lpBuffer[i];
			// 32个字符一行
			if ((i != 0) && (i % 16 == 0)){
				sprintf(szText, "\r\n%02X ", bi);
			}
			// 每16个字符加一个空格
			else if ((i != 0) && (i % 8 == 0)){
				sprintf(szText, "  %02X ", bi);
			}
			// 正常情况
			else{
				sprintf(szText, "%02X ", bi);
			}
			// 超过最大长度的字符丢弃
			iTextLen += strlen(szText);
			if (iTextLen < iMaxTextLen){
				strcat(lpResult, szText);
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		strcpy(lpResult, "Exception");
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
	IOTrs_Trace("WriteHtdl - Enter");
#endif
	//HANDLE	hSimObject;
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
		//if(!bRet){
		//	if (ERROR_IO_PENDING == ::GetLastError()) {
		//		// 等待写入完成
		//		::WaitForSingleObject(olwrite.hEvent, INFINITE);
		//	}
		//}
		//::GetOverlappedResult(hStream, &olwrite, pdwActualWrite, TRUE); 
	}
#ifdef _TRACE_SEND_DATA_
	Conv2Str(m_szText, MAX_TRACE_LEN, pBuffer, *pdwActualWrite);
	IOTrs_Trace("WriteHtdl - written %d bytes\r\n", *pdwActualWrite);
	IOTrs_Trace(m_szText);
#endif

#ifdef _TRACE_HTDL_
	IOTrs_Trace("WriteHtdl - Exit - return %d", bRet);
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
	IOTrs_Trace("ReadHtdl - Enter");
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
				strcat( (char *)tmpbuf, (char *)pMailBuffer );	
				strncpy( (char *)pBuffer, (char *)tmpbuf, dwLength );
				n = 0;									
				for( i = dwLength; i < (int)strlen( (char *)tmpbuf ); i++, n++ ){
					pMailBuffer[n] = tmpbuf[i];
				}
				pMailBuffer[n] = NULL;
				buflen -= dwLength;
				*pdwBytesRead = dwLength;
			}
			else{			
				strcat( (char *)tmpbuf, (char *)pMailBuffer );
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
		//if(!bRet){
		//	if (ERROR_IO_PENDING == ::GetLastError()) {
		//		// 等待读取完成
		//		::WaitForSingleObject(olread.hEvent, INFINITE);
		//	}
		//}
		//::GetOverlappedResult(hSimObject, &olread, pdwBytesRead, TRUE); 

		//bRet = ::ReadFile(hSimObject, pBuffer, dwLength, pdwBytesRead, NULL);
	}

#ifdef _TRACE_HTDL_
	IOTrs_Trace("ReadHtdl - Exit - return %d", 0);
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
	IOTrs_Trace("UpdateStatus - Enter - [%08X] = %d", dwStatus, bOn);
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
	IOTrs_Trace("UpdateStatus - Exit");
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
	IOTrs_Trace("SendClear - Enter");
#endif

	::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_TXCLEAR);

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("SendClear - Exit");
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
	IOTrs_Trace("SetEventSendFin - Enter");
#endif
	
	::SetEvent(pWork->hFinSend);

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("SetEventSendFin - Exit");
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
	IOTrs_Trace("InitRecvBuff - Enter");
#endif

	// 清空数据接收缓存
	pWork->RecvCnt = 0;
	pWork->RecvPtr = pWork->RecvBuff;
	memset(pWork->RecvBuff, 0, sizeof(pWork->RecvBuff));
	//pWork->Dle = false;
	// 更新服务状态为等待接收
	UpdateStatus(pWork, IOTRS_ST_RCVEXE, TRUE);

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("InitRecvBuff - Exit");
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
	IOTrs_Trace("PreSendData - Enter");
#endif
	BYTE* p = pWork->SendBuff;

	_ASSERTE(NULL != p);
	_ASSERTE(NULL != lpData);

	pWork->SendLength = iSendLen+3;
	// 拼包头
	*p++ = STX;
	// 拼长度
	*p++ = (iSendLen+3);
	// 拼包体
	for (int i=0; i< iSendLen; i++){
		*p++ = lpData[i];
	}
	// 拼包尾
	*p++ = ETX;
#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("PreSendData - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送通信控制码

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)BYTE bCode        通信控制码

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void  SendCntl(HTDLWORK *work, BYTE bCode)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("SendCntl - Enter");
//#endif
//
//	BYTE buff[2];
//	DWORD dwSize;
//	buff[0] = DLE;	
//	buff[1] = bCode;
//	BOOL bChk = WriteHtdl(work->hComm,buff,2,&dwSize,work->bCommunicationMethod,work->hSimObject,work->SendSlotName);
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("SendCntl - Exit");
//#endif
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新数据块序号

@param      (i)HTDLWORK *pWork		工作空间结构指针
@param      (i)SENDITEM *pSendItem  发送命令项指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void UpdateBlockNo(HTDLWORK *pWork, SENDITEM *pSendItem)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("UpdateBlockNo - Enter");
//#endif
//
//	// 发送长度未超过256，不分块
//	if (pSendItem->nSendLength <= MAX_MSG_LEN){		
//		pWork->BlockNumSend = 0x00; 
//	}
//	// 分块
//	else{
//		int iBlockNo = (*pSendItem->pOutLength) / MAX_MSG_LEN;
//		// 最后一块
//		if (*pSendItem->pOutLength + MAX_MSG_LEN >= pSendItem->nSendLength){
//			pWork->BlockNumSend = iBlockNo;
//		}
//		// 中间数据块
//		else{
//			pWork->BlockNumSend = 0x80 | iBlockNo;
//		}
//	}
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("UpdateBlockNo - Exit");
//#endif
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送ENQ

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxEnquire - Enter");
//#endif
//
//	pWork->RetryCnt = 0;
//	SendCntl(pWork, ENQ);
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxEnquire - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送ENQ，重试3次

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRetryEnquire - Enter");
//#endif
//	int next;
//	// 重试次数++
//	pWork->RetryCnt++;
//	// 未达到最大重试次数
//	if (pWork->RetryCnt < MAX_RETRYSEND){
//		SendCntl(pWork, ENQ);
//		next = pMatrix->next[1];
//	}
//	// 达到最大重试次数
//	else{
//		// 清空串口发送缓存
//		SendClear(pWork);
//		next = pMatrix->next[0];
//		// 更新服务状态为发送失败
//		UpdateStatus(pWork, IOTRS_ST_SNDERR, TRUE);
//		pWork->ReqSend = false;
//		// 设置命令执行结果
//		*(pWork->SndItem.pResult) = IOTRS_MTX_ERR_ENQUIRE;
//		// 激活命令发送完成事件
//		SetEventSendFin(pWork);
//	}
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxRetryEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRetryEnquire - Exit - return %d", next);
//#endif
//	return next;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送ENQ，重试3次超时

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxTimeOutEnquire - Enter");
//#endif
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutEnquire", pMatrix->phase, pMatrix->event);
//#endif
//
//	int iNext = mtxRetryEnquire(pWork, pMatrix);
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxTimeOutEnquire - Exit - return %d", iNext);
//#endif
//	return iNext;
//}

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
	IOTrs_Trace("mtxSend - Enter");
#endif
	
	// 发送前清空接收缓存
	InitRecvBuff(pWork);
	SENDITEM* pSendItem = &pWork->SndItem;
	int iRemainLen;
	DWORD dwActualWrite;
	int iNext;

	// 发送未完成
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
				sprintf(&log[2*i+2],"%02x",pSendItem->pSending[i]);
			}
			IOTrs_Trace(log);
		}
		// 更新已发送数据字节长度
		(*pSendItem->pOutLength) += pSendItem->wLastSendingLen;
		// 更新发送序列号
		//pWork->SeqSend = 1 + (pWork->SeqSend % 0xFF);
		iNext = pMatrix->next[1];
	}
	// 发送已完成
	else{
		iNext = pMatrix->next[0];
		*(pSendItem->pResult) = IOTRS_RES_OK;
		pWork->ReqSend = false;

		//激活命令发送完成事件
		SetEventSendFin(pWork);
	}

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxSend - Goto %d", pMatrix->phase, pMatrix->event, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxSend - Exit - return %d", iNext);
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
//int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRetrySend - Enter");
//#endif
//	int next;
//	DWORD dwActualWrite;		// 实际发送数据长度
//	pWork->RetryCnt++;
//	if (pWork->RetryCnt < MAX_RETRYSEND){
//		if (pWork->SendLength >0){
//			WriteHtdl(pWork->hComm,pWork->SendBuff,pWork->SendLength,&dwActualWrite,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);
//		}
//		next = pMatrix->next[1];
//	}
//	else{
//		// 清空串口发送缓存
//		SendClear(pWork);
//		pWork->ReqSend = false;
//		next = pMatrix->next[0];
//		// 更新服务状态为发送失败
//		UpdateStatus(pWork, IOTRS_ST_SNDERR, TRUE);
//		// 设置命令执行结果
//		*(pWork->SndItem.pResult) = IOTRS_MTX_ERR_SEND;
//		// 激活命令发送完成事件
//		SetEventSendFin(pWork);
//	}
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxRetrySend - Goto %d", pMatrix->phase, pMatrix->event, next);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRetrySend - Exit - return %d", next);
//#endif
//	return next;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送命令，三次重试超时

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxTimeOutSend - Enter");
//#endif
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutSend", pMatrix->phase, pMatrix->event);
//#endif
//	int iNext = mtxRetrySend(pWork, pMatrix);
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxTimeOutSend - Exit - return %d", iNext);
//#endif
//	return iNext;
//}

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
	IOTrs_Trace("mtxTimeOutRecv - Enter");
#endif

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutRecv - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxTimeOutRecv - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取接收序列号

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRecvFrameSeq - Enter");
//#endif
//
//	pWork->SeqRecv = pWork->c;
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxRecvFrameSeq %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRecvFrameSeq - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取接收块序号

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRecvBlockNo - Enter");
//#endif
//
//	pWork->BlockNumRecv = pWork->c;
//
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxRecvBlockNo %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxRecvBlockNo - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

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
	IOTrs_Trace("mtxOnChar - Enter");
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
		UpdateStatus(pWork, IOTRS_ST_RCVOVR, TRUE);
	}

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxOnChar %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxOnChar - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      收到长度处理

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
extern int mtxRecvLenth(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxRecvLenth - Enter");
#endif
	int iNext = pMatrix->next[0];
	pWork->RecvRspLenth = pWork->c;
#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxOnChar %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxRecvLenth - Exit - return %d", iNext);
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
	IOTrs_Trace("mtxReRecv - Enter");
#endif
	
	InitRecvBuff(pWork);

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxReRecv - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxReRecv - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送ACK

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxAcceptEnq(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxAcceptEnq - Enter");
//#endif
//
//	pWork->bNewENQ = TRUE;
//	SendCntl(pWork, ACK);
//	InitRecvBuff(pWork);
//	
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxAcceptEnq - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
//#endif
//	
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxAcceptEnq - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

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
	IOTrs_Trace("mtxDummy - Enter");
#endif

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxDummy - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif
	
#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxDummy - Exit - return %d", pMatrix->next[1]);
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
	IOTrs_Trace("mtxProtErr - Enter");
#endif

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxProtErr - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif
	
#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxProtErr - Exit - return %d", pMatrix->next[1]);
#endif
	//Do some error reports if any
	return pMatrix->next[1];
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
//int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - Enter");
//#endif
//	BYTE bcc;
//	int iNext;
//	
//	// 接收字节数大于256
//	if (pWork->RecvCnt > MAX_MSG_LEN){
//		return pWork->Phase;
//	}
//	// 刚收到ENQ
//	if(pWork->bNewENQ == TRUE){
//		// 数据不是第一个数据块
//		if (pWork->BlockNumRecv != 0x00 && pWork->BlockNumRecv != 0x80){
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - NOT first block - Send NAK");
//#endif
//			// 发送NAK
//			SendCntl(pWork, NAK);
//			return pMatrix->next[0];
//		}
//	}
//	// 计算BCC
//	bcc = BccCalc(pWork->RecvBuff, pWork->RecvCnt);
//	bcc = bcc ^ pWork->BlockNumRecv;
//	bcc = bcc ^ pWork->SeqRecv;
//
//#ifdef _TRACE_SEND_DATA_
//	Conv2Str(m_szText, MAX_TRACE_LEN, pWork->RecvBuff, pWork->RecvCnt);
//	IOTrs_Trace("Check BCC - SEQ [%02X] - Block No [%02X]", pWork->SeqRecv, pWork->BlockNumRecv);
//	IOTrs_Trace(m_szText);
//#endif
//
//	// BCC值正确
//	if(bcc == pWork->c){
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - OK - Send ACK");
//#endif
//		// 新数据块
//		if(pWork->LastSeqRecv != pWork->SeqRecv){
//			// 接收数据成功
//			if(TRUE == RecvData(pWork, pWork->RecvBuff, pWork->RecvCnt)){
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - Receive block %04X - LEN = %04X", pWork->BlockNumRecv, pWork->RecvCnt);
//#endif
//				pWork->LastSeqRecv = pWork->SeqRecv;
//				pWork->bNewENQ = FALSE;
//				SendCntl(pWork, ACK);
//				iNext = pMatrix->next[0];
//			}
//			// 接收数据失败
//			else{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - RecvData Fail - Send NAK");
//#endif	
//				// 发送NAK
//				SendCntl(pWork, NAK);
//				iNext = pMatrix->next[0];
//			}
//		}
//		// 旧数据块
//		else{
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - Old Block Received - Send ACK");
//#endif
//			// 发送ACK
//			SendCntl(pWork, ACK);
//			iNext = pMatrix->next[0]; //continue receive other chunks
//		}
//		if((pWork->BlockNumRecv & 0x80) == 0x00){
//#ifdef _TRACE_MAXTRIX_
//			IOTrs_Trace("mtxCheckBcc - Last Block received");
//#endif
//			//Receive Done
//			iNext = pMatrix->next[1];
//		}
//	}
//	// BCC值错误
//	else{
//#ifdef _TRACE_MAXTRIX_
//		IOTrs_Trace("mtxCheckBcc - NG - Send NAK");
//#endif
//		// 发送NAK
//		SendCntl(pWork, NAK);
//		iNext = pMatrix->next[0];
//	}
//	InitRecvBuff(pWork);
//	
//#ifdef _TRACE_STEP_
//	IOTrs_Trace("Phase [%d] - Event [%d] - mtxCheckBcc - Goto %d", pMatrix->phase, pMatrix->event, iNext);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	IOTrs_Trace("mtxCheckBcc - Exit - return %d", iNext);
//#endif
//
//	return iNext;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief        数据接收完成处理

@param      (i)HTDLWORK *pWork   工作空间结构指针
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
extern int mtxRecvComplete(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxCheckBcc - Enter");
#endif
	int iNext;
	// 未接收完成
	if(pWork->RecvCnt<(pWork->RecvRspLenth -3)){		
		return mtxOnChar(pWork, pMatrix);			 // 接收一个字节
	}
	// 接收字节数大于256
	if (pWork->RecvCnt > MAX_MSG_LEN){
		return pWork->Phase;
	}
	// 自动上报状态不保存
	if((pWork->RecvBuff[0] == 0x49) && (pWork->RecvBuff[1] == 0x2C)){
		iNext = pMatrix->next[0];
	}
	// 其他反馈数据
	else{
		// 接收数据成功
		if(TRUE == RecvData(pWork, pWork->RecvBuff, pWork->RecvCnt)){
#ifdef _TRACE_MAXTRIX_
			IOTrs_Trace("mtxCheckBcc - LEN = %04X", pWork->RecvCnt);
#endif
			iNext = pMatrix->next[0];
		}
		// 接收数据失败
		else{
			iNext = pMatrix->next[0];
		}
	}
	InitRecvBuff(pWork);

#ifdef _TRACE_STEP_
	IOTrs_Trace("Phase [%d] - Event [%d] - mtxCheckBcc - Goto %d", pMatrix->phase, pMatrix->event, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	IOTrs_Trace("mtxCheckBcc - Exit - return %d", iNext);
#endif

	return iNext;
}