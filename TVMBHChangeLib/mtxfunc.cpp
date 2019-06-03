#include <stdio.h>
#include "common.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ݻ���ת��Ϊ�ַ���(32���ֽ�һ��,16���ַ���һ��)

@param      (o)char* lpResult		����ַ���
@param      (i)int iMaxTextLen		�ַ�����󳤶�
@param      (i)const BYTE* lpBuffer ���ݻ���ָ��
@param      (i)int iLen				���ݻ��泤��

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
			// 32���ַ�һ��
			if ((i != 0) && (i % 16 == 0)){
				sprintf(szText, "\r\n%02X ", bi);
			}
			// ÿ16���ַ���һ���ո�
			else if ((i != 0) && (i % 8 == 0)){
				sprintf(szText, "  %02X ", bi);
			}
			// �������
			else{
				sprintf(szText, "%02X ", bi);
			}
			// ������󳤶ȵ��ַ�����
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
@brief      д���ݵ����ڻ�ģ��ͨ�ſ�

@param      (i)HANDLE hStream			���ھ��
@param      (i)BYTE *pBuffer			д������ָ��
@param      (i)DWORD dwLength			д�����ݳ���
@param      (o)DWORD *pdwActualWrite	ʵ��д������ݳ���
@param      (i)BYTE bCommunicationMethod ͨ�ŷ�ʽ
@param      (i)HANDLE hSimObject		ģ�������  
@param      (i)BYTE *sSlotName          �ʲ�����    

@retval     BOOL    TRUE:�ɹ�  FALSE:ʧ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL WriteHtdl(HANDLE hStream, BYTE *pBuffer, DWORD dwLength, DWORD *pdwActualWrite,BYTE bCommunicationMethod, HANDLE hSimObject,BYTE *sSlotName)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("WriteHtdl - Enter");
#endif
	//HANDLE	hSimObject;
	BOOL	bRet = TRUE;
	DWORD	i;

	// �ʲ�ͨ��
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
	// ����ͨ��
	else if(bCommunicationMethod == COM_PORT){
		bRet = WriteFile(hStream,pBuffer,dwLength,pdwActualWrite,NULL);
		FlushFileBuffers(hStream);
	}
	// �ܵ�ͨ��
	else if(bCommunicationMethod == NAME_PIPE){
		OVERLAPPED olwrite;
		::memset(&olwrite, 0, sizeof(olwrite));
		olwrite.Offset = 0;
		olwrite.OffsetHigh = 0;
		// �����¼�����		
		olwrite.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		bRet = WriteFile(hSimObject,pBuffer,dwLength,pdwActualWrite,&olwrite);
		//if(!bRet){
		//	if (ERROR_IO_PENDING == ::GetLastError()) {
		//		// �ȴ�д�����
		//		::WaitForSingleObject(olwrite.hEvent, INFINITE);
		//	}
		//}
		//::GetOverlappedResult(hStream, &olwrite, pdwActualWrite, TRUE); 
	}
#ifdef _TRACE_SEND_DATA_
	Conv2Str(m_szText, MAX_TRACE_LEN, pBuffer, *pdwActualWrite);
	BHTrs_Trace("WriteHtdl - written %d bytes\r\n", *pdwActualWrite);
	BHTrs_Trace(m_szText);
#endif

#ifdef _TRACE_HTDL_
	BHTrs_Trace("WriteHtdl - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ӵ��ڻ��ʲ۶�ȡ����

@param      (i)HANDLE hStream			���ھ��
@param      (i)BYTE *pBuffer			�Ӵ��ڶ�ȡ������ָ��
@param      (i)DWORD dwLength			��ȡ���ݳ���
@param      (o)DWORD *pdwBytesRead		ʵ�ʶ�ȡ�����ݳ���
@param      (i)BYTE bCommunicationMethod	ͨ�ŷ�ʽ
@param      (i)HANDLE hSimObject				ģ�������   
@param      (i)BYTE *pMailBuffer		��ģ��ͨ�ſڶ�ȡ������ָ��

@retval     BOOL    TRUE:�ɹ�  FALSE:ʧ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL ReadHtdl(HANDLE hStream, BYTE *pBuffer, DWORD dwLength, DWORD *pdwBytesRead,BYTE bCommunicationMethod, HANDLE hSimObject, BYTE *pMailBuffer)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("ReadHtdl - Enter");
#endif
	DWORD	nRead;
	BOOL	bRet;
	// �ʲ�
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
	// ����
	else if(bCommunicationMethod == COM_PORT){
		DWORD dwErrorMask  = 0L;
		COMSTAT comstat;
		// ������ڴ���
		::ClearCommError(hStream, &dwErrorMask, &comstat);
		// ��ȡ����
		bRet = ::ReadFile(hStream, pBuffer, dwLength, pdwBytesRead, NULL);
	}
	// �ܵ�
	else if(bCommunicationMethod == NAME_PIPE){
		OVERLAPPED olread;	
		::memset(&olread, 0, sizeof(olread));		
		olread.Offset = 0;
		olread.OffsetHigh = 0;
		// �����¼����� 
		olread.hEvent = ::CreateEvent(0, TRUE, FALSE, 0);
		bRet = ::ReadFile(hSimObject, pBuffer, dwLength, pdwBytesRead, &olread);
		//if(!bRet){
		//	if (ERROR_IO_PENDING == ::GetLastError()) {
		//		// �ȴ���ȡ���
		//		::WaitForSingleObject(olread.hEvent, INFINITE);
		//	}
		//}
		//::GetOverlappedResult(hSimObject, &olread, pdwBytesRead, TRUE); 

		//bRet = ::ReadFile(hSimObject, pBuffer, dwLength, pdwBytesRead, NULL);
	}

#ifdef _TRACE_HTDL_
	BHTrs_Trace("ReadHtdl - Exit - return %d", 0);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���·���״̬

@param      (i)HTDLWORK *pWork          �����ռ�ṹָ��
@param      (i)DWORD dwStatus			����״̬��
@param      (i)BOOL bOn					TRUE:��  FALSE:��

@retval     BOOL    TRUE:�ɹ�  FALSE:ʧ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void UpdateStatus(HTDLWORK *pWork,DWORD dwStatus,BOOL bOn)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("UpdateStatus - Enter - [%08X] = %d", dwStatus, bOn);
#endif
	DWORD nWait = ::WaitForSingleObject(pWork->hMutex, INFINITE);
	if (nWait != WAIT_FAILED){
		// ��
		if(TRUE == bOn){
			pWork->DrvStat |= dwStatus;
		}
		// ��
		else{
			pWork->DrvStat &= ~dwStatus;
		}
		::ReleaseMutex(pWork->hMutex);
	}

#ifdef _TRACE_HTDL_
	BHTrs_Trace("UpdateStatus - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��մ��ڷ��ͻ���

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void SendClear(HTDLWORK *pWork)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("SendClear - Enter");
#endif

	::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_TXCLEAR);

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("SendClear - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������������¼�

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void SetEventSendFin(HTDLWORK *pWork)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("SetEventSendFin - Enter");
#endif
	
	::SetEvent(pWork->hFinSend);

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("SetEventSendFin - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ�����ݽ��ջ����Խ����µ�����

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void InitRecvBuff(HTDLWORK *pWork)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("InitRecvBuff - Enter");
#endif

	// ������ݽ��ջ���
	pWork->RecvCnt = 0;
	pWork->RecvPtr = pWork->RecvBuff;
	memset(pWork->RecvBuff, 0, sizeof(pWork->RecvBuff));
	// ���·���״̬Ϊ�ȴ�����
	UpdateStatus(pWork, BHTRS_ST_RCVEXE, TRUE);

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("InitRecvBuff - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ׼���������ݰ�

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)BYTE* lpData      ��������ָ��(��ʼλ��)
@param      (i)int iSendLen      �������ݳ���

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void PreSendData(HTDLWORK *pWork, BYTE* lpData, int iSendLen)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("PreSendData - Enter");
#endif
	BYTE* p = pWork->SendBuff;

	_ASSERTE(NULL != p);
	_ASSERTE(NULL != lpData);
	// ���������Ƿ��з���������Ҫ��
	/*if(lpData[0] == CMD_BH_GET_STATUS || lpData[0] ==CMD_BH_INFO || lpData[0] ==CMD_BH_READ || lpData[0] ==CMD_BH_WRITE){
	pWork->bisNeedResponse = true;
	}
	else{
	pWork->bisNeedResponse = false;
	}*/

	pWork->SendLength = 4;
	// ƴ��ͷ
	*p++ = DLE;
	*p++ = STX;
	*p++ = iSendLen >> 8;
	*p++ = iSendLen & 0x00FF;
	// ƴ����

	//pWork->SendLength = 0;
	for (int i=0; i< iSendLen; i++){
		*p++ = lpData[i];
		pWork->SendLength++;
	}

	// ƴ��β
	*p++ = DLE;
	*p++ = ETX;
	pWork->SendLength += 2;
	// ����CRC
	unsigned short do_crc16(unsigned char *message, unsigned int len) ;
	SHORT crc = do_crc16(pWork->SendBuff+2, pWork->SendLength - 2);
	*(SHORT*)p = crc;
	pWork->SendLength += 2;

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("PreSendData - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ͨ�ſ�����

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)BYTE bCode        ͨ�ſ�����

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void  SendCntl(HTDLWORK *work, BYTE bCode)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("SendCntl - Enter");
#endif

	BYTE buff[2];
	DWORD dwSize;
	buff[0] = DLE;
	buff[1] = bCode;	
	BOOL bChk = WriteHtdl(work->hComm,buff,2,&dwSize,work->bCommunicationMethod,work->hSimObject,work->SendSlotName);

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("SendCntl - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ݿ����

@param      (i)HTDLWORK *pWork		�����ռ�ṹָ��
@param      (i)SENDITEM *pSendItem  ����������ָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void UpdateBlockNo(HTDLWORK *pWork, SENDITEM *pSendItem)
//{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("UpdateBlockNo - Enter");
//#endif
//
//	// ���ͳ���δ����256�����ֿ�
//	if (pSendItem->nSendLength <= MAX_MSG_LEN){		
//		pWork->BlockNumSend = 0x00; 
//	}
//	// �ֿ�
//	else{
//		int iBlockNo = (*pSendItem->pOutLength) / MAX_MSG_LEN;
//		// ���һ��
//		if (*pSendItem->pOutLength + MAX_MSG_LEN >= pSendItem->nSendLength){
//			pWork->BlockNumSend = iBlockNo;
//		}
//		// �м����ݿ�
//		else{
//			pWork->BlockNumSend = 0x80 | iBlockNo;
//		}
//	}
//
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("UpdateBlockNo - Exit");
//#endif
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ENQ

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxEnquire - Enter");
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, ENQ);

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxEnquire - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ENQ������3��

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRetryEnquire - Enter");
#endif
	int next;
	// ���Դ���++
	pWork->RetryCnt++;
	// δ�ﵽ������Դ���
	if (pWork->RetryCnt < MAX_RETRYSEND){
		SendCntl(pWork, ENQ);
		next = pMatrix->next[1];
	}
	// �ﵽ������Դ���
	else{
		// ��մ��ڷ��ͻ���
		SendClear(pWork);
		next = pMatrix->next[0];
		// ���·���״̬Ϊ����ʧ��
		UpdateStatus(pWork, BHTRS_ST_SNDERR, TRUE);
		pWork->ReqSend = false;
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = BHTRS_MTX_ERR_ENQUIRE;
		// �������������¼�
		SetEventSendFin(pWork);
	}

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRetryEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRetryEnquire - Exit - return %d", next);
#endif
	return next;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ENQ������3�γ�ʱ

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxTimeOutEnquire - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutEnquire", pMatrix->phase, pMatrix->event);
#endif

	int iNext = mtxRetryEnquire(pWork, pMatrix);

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxTimeOutEnquire - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxSend(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxSend - Enter");
#endif
	InitRecvBuff(pWork);
	SENDITEM* pSendItem = &pWork->SndItem;
	int iRemainLen;
	DWORD dwActualWrite;
	int iNext;

	// ����δ���
	if ((*pSendItem->pOutLength) < pSendItem->nSendLength){		
		// �������ݿ����
		//UpdateBlockNo(pWork, pSendItem);
		// ���÷��Ϳ�ʼλ��
		pSendItem->pSending += pSendItem->wLastSendingLen; 
		// �������ݷ���ʣ�೤��
		iRemainLen = pSendItem->nSendLength - (*pSendItem->pOutLength);
		// �������һ�η��͵����ݳ���
		pSendItem->wLastSendingLen = MAX_MSG_LEN;
		// ���һ�����ݿ�
		if(iRemainLen < MAX_MSG_LEN){
			pSendItem->wLastSendingLen = iRemainLen;
		}
		// ׼����������
		PreSendData(pWork, pSendItem->pSending, pSendItem->wLastSendingLen);
		
		pWork->RetryCnt = 0;
		// ��������
		WriteHtdl(pWork->hComm,pWork->SendBuff,pWork->SendLength,&dwActualWrite,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);

		// д��־
        if(pSendItem->wLastSendingLen > 0){			
			char log[MAX_BUFFER_SIZE*2] = {0};
			log[0] = '>';
			log[1] = ' ';
			for(unsigned short i=0;i < pSendItem->wLastSendingLen; i++) {
				sprintf(&log[2*i+2],"%02X",pSendItem->pSending[i]);
			}
			//memcpy(&log[2],pSendItem->pSending,pSendItem->wLastSendingLen);
			BHTrs_Trace(log);
		}
		// �����ѷ��������ֽڳ���
		(*pSendItem->pOutLength) += pSendItem->wLastSendingLen;
		// ���·������к�
		//pWork->SeqSend = 1 + (pWork->SeqSend % 0xFF);
		iNext = pMatrix->next[1];
	}
	// ���������
	else{
		iNext = pMatrix->next[0];
		*(pSendItem->pResult) = BHTRS_RES_OK;
		pWork->ReqSend = false;

		//�������������¼�
		SetEventSendFin(pWork);
	}

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxSend - Goto %d", pMatrix->phase, pMatrix->event, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxSend - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������������

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxRetrySend(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRetrySend - Enter");
#endif
	int next;
	DWORD dwActualWrite;		// ʵ�ʷ������ݳ���
	pWork->RetryCnt++;
	if (pWork->RetryCnt < MAX_RETRYSEND){
		if (pWork->SendLength >0){
			WriteHtdl(pWork->hComm,pWork->SendBuff,pWork->SendLength,&dwActualWrite,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);
		}
		next = pMatrix->next[1];
	}
	else{
		// ��մ��ڷ��ͻ���
		SendClear(pWork);
		pWork->ReqSend = false;
		next = pMatrix->next[0];
		// ���·���״̬Ϊ����ʧ��
		UpdateStatus(pWork, BHTRS_ST_SNDERR, TRUE);
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = BHTRS_MTX_ERR_SEND;
		// �������������¼�
		SetEventSendFin(pWork);
	}

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRetrySend - Goto %d", pMatrix->phase, pMatrix->event, next);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRetrySend - Exit - return %d", next);
#endif
	return next;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������������Գ�ʱ

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxTimeOutSend(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxTimeOutSend - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutSend", pMatrix->phase, pMatrix->event);
#endif
	int iNext = mtxRetrySend(pWork, pMatrix);

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxTimeOutSend - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ճ�ʱ����

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxTimeOutRecv(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxTimeOutRecv - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutRecv - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxTimeOutRecv - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�������к�

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxRecvFrameSeq(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxRecvFrameSeq - Enter");
//#endif
//
//	pWork->SeqRecv = pWork->c;
//
//#ifdef _TRACE_STEP_
//	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvFrameSeq %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxRecvFrameSeq - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���տ����

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxRecvBlockNo(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxRecvBlockNo - Enter");
//#endif
//
//	pWork->BlockNumRecv = pWork->c;
//
//#ifdef _TRACE_STEP_
//	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvBlockNo %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxRecvBlockNo - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����һ���ֽ�

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxOnChar(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxOnChar - Enter");
#endif
	int iNext;
	// ���ճ�������
	if (pWork->RecvCnt < MAX_RECVBUFF){
		*(pWork->RecvPtr++) = pWork->c;
		pWork->RecvCnt++;
		iNext = pMatrix->next[1];
	}
	// �������ݻ������
	else{
		iNext = pMatrix->next[0];
		UpdateStatus(pWork, BHTRS_ST_RCVOVR, TRUE);
	}

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxOnChar %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxOnChar - Exit - return %d", iNext);
#endif
	return iNext;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ݽ��ջ����Խ����µ�����

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxReRecv(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxReRecv - Enter");
#endif
	
	InitRecvBuff(pWork);

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxReRecv - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxReRecv - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ACK

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxAcceptAck(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxAcceptEnq - Enter");
#endif
	InitRecvBuff(pWork);
	int iNextPhase = 0;
	if(pWork->bisNeedResponse){
		iNextPhase = pMatrix->next[0];	
	}
	else{
		iNextPhase = pMatrix->next[1];
	}
#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxAcceptEnq - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxAcceptEnq - Exit - return %d", pMatrix->next[1]);
#endif
	return iNextPhase;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �޴���

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxDummy(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxDummy - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxDummy - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif
	
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxDummy - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ͨ��Э�������

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxProtErr(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxProtErr - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxProtErr - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif
	
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxProtErr - Exit - return %d", pMatrix->next[1]);
#endif
	//Do some error reports if any
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCC��֤����

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxCheckBcc(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxCheckBcc - Enter");
#endif
	BYTE bcc;
	int iNext;
	
	// �����ֽ�������256
	if (pWork->RecvCnt > MAX_MSG_LEN){
		return pWork->Phase;
	}

	// ����BCC
	bcc = BccCalc(pWork->RecvBuff, pWork->RecvCnt);

#ifdef _TRACE_SEND_DATA_
	Conv2Str(m_szText, MAX_TRACE_LEN, pWork->RecvBuff, pWork->RecvCnt);
	BHTrs_Trace("Check BCC - SEQ [%02X] - Block No [%02X]", pWork->SeqRecv, pWork->BlockNumRecv);
	BHTrs_Trace(m_szText);
#endif

	// BCCֵ��ȷ
	if(bcc == pWork->c){
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxCheckBcc - OK - Send ACK");
#endif
		// �����ݿ�
		//if(pWork->LastSeqRecv != pWork->SeqRecv){
			// �������ݳɹ�
			if(TRUE == RecvData(pWork, pWork->RecvBuff, pWork->RecvCnt)){
#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxCheckBcc - Receive block %04X - LEN = %04X", pWork->BlockNumRecv, pWork->RecvCnt);
#endif
				//pWork->LastSeqRecv = pWork->SeqRecv;
				//pWork->bNewENQ = FALSE;
				SendCntl(pWork, ACK);
				iNext = pMatrix->next[1];
			}
			// ��������ʧ��
			else{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxCheckBcc - RecvData Fail - Send NAK");
#endif	
				// ����NAK
				SendCntl(pWork, NAK);
				iNext = pMatrix->next[1];
			}
		//}
		// �����ݿ�
//		else{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxCheckBcc - Old Block Received - Send ACK");
//#endif
//			// ����ACK
//			SendCntl(pWork, ACK);
//			iNext = pMatrix->next[0]; //continue receive other chunks
//		}
//		if((pWork->BlockNumRecv & 0x80) == 0x00){
//#ifdef _TRACE_MAXTRIX_
//			BHTrs_Trace("mtxCheckBcc - Last Block received");
//#endif
//			//Receive Done
//			iNext = pMatrix->next[1];
//		}
	}
	// BCCֵ����
	else{
#ifdef _TRACE_MAXTRIX_
		BHTrs_Trace("mtxCheckBcc - NG - Send NAK");
#endif
		// ����ENQ
		SendCntl(pWork, NAK);
		iNext = pMatrix->next[0];
	}
	InitRecvBuff(pWork);
	
#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxCheckBcc - Goto %d", pMatrix->phase, pMatrix->event, iNext);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxCheckBcc - Exit - return %d", iNext);
#endif

	return iNext;
}



//////////////////////////////////////////////////////////////////////////
//int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxRetryEnquire - Enter");
//#endif
//	int next;
//	// ���Դ���++
//	pWork->RetryCnt++;
//	// δ�ﵽ������Դ���
//	if (pWork->RetryCnt < MAX_RETRYSEND){
//		SendCntl(pWork, ENQ);
//		next = pMatrix->next[1];
//	}
//	// �ﵽ������Դ���
//	else{
//		// ��մ��ڷ��ͻ���
//		SendClear(pWork);
//		next = pMatrix->next[0];
//		// ���·���״̬Ϊ����ʧ��
//		UpdateStatus(pWork, BHTRS_ST_SNDERR, TRUE);
//		pWork->ReqSend = false;
//		// ��������ִ�н��
//		*(pWork->SndItem.pResult) = BHTRS_MTX_ERR_ENQUIRE;
//		// �������������¼�
//		SetEventSendFin(pWork);
//	}
//
//#ifdef _TRACE_STEP_
//	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRetryEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxRetryEnquire - Exit - return %d", next);
//#endif
//	return next;
//}



////////////////////////////////////////////////////////////////////////
int mtxReTry(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxReTry - Enter");
#endif
	int next;
	// ���Դ���++
	pWork->RetryCnt++;
	// δ�ﵽ������Դ���
	if (pWork->RetryCnt < MAX_RETRYSEND){
		next = pMatrix->next[1];
	}
	// �ﵽ������Դ���
	else{
		next = pMatrix->next[0];
	}

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxReTry - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxReTry - Exit - return %d", next);
#endif
	return next;
}


int mtxSendACK(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxSendACK - Enter");
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, ACK);

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxSendACK - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxSendACK - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}


int mtxSendNAK(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxSendNAK - Enter");
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, NAK);

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxSendNAK - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxSendNAK - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}


int mtxRecvWrong(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRecvWrong - Enter");
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, NAK);

//	memset(pWork->RecvBuff, 0, sizeof(pWork->RecvBuff));
//	pWork->RecvCnt = 0;
	InitRecvBuff(pWork);

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvWrong - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRecvWrong - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}




/*///////////////////////////////////////////////////////////////////////////////////////
/////////////////					��CRC����У��				////////////////////////
*////////////////////////////////////////////////////////////////////////////////////////
unsigned short crc16_ccitt_table[256] =
{
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

unsigned short do_crc16(unsigned char *message, unsigned int len) 
{
	unsigned short crc_reg = 0; 

	while (len--) 
		crc_reg = (crc_reg >> 8) ^ crc16_ccitt_table[(crc_reg ^ *message++) & 0xff];

	return crc_reg;
}  



int mtxCheckCRC(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxCheckCRC - Enter");
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// ���յ�CRC����λΪ2λ
	if( pWork->RecvCnt == ( 2 + pWork->wNeedRecvCnt  + 2 + 2) ){ // data lenght + sizeof(data) + DLE + ETX + sizeof(CRC)
		// У��CRC
		SHORT RecvCRC = ((*(pWork->RecvPtr - 1) << 8) & 0xFF00) | *(pWork->RecvPtr - 2);

		SHORT checkCRC = do_crc16(pWork->RecvBuff,pWork->RecvCnt - 2);

		// CRC���,����������ݳɹ�
		if(RecvCRC == checkCRC && RecvData(pWork,pWork->RecvBuff + 2, pWork->RecvCnt - 6)){
			//Send ACK
			SendCntl(pWork,ACK);
			return pMatrix->next[1];
		}
		// CRC����ȣ�����
		else{
			SendCntl(pWork,NAK);
			return PHASE_S3;			// �ȴ�����DLE(STX)
		}
	}
	else{// ��������У��λ
		return pMatrix->next[0];
	}
//	BYTE* pMsg = pWork->RecvBuff;
//	INT len = pWork->RecvCnt - 2;		//CRC����2������
//	SHORT crc = do_crc16(pMsg, len);
//	SHORT * temp = (SHORT *)(pMsg + len); //��������2���ֽ�
//	if( crc == *temp && RecvData(pWork, pWork->RecvBuff + 2, pWork->RecvCnt - 6))//ȥ��ǰ��2���ֽڵĳ������ݣ�����2���ֽڵ�DLE-ETX��2���ֽڵ�CRC
//	{
//#ifdef _TRACE_MAXTRIX_
//		BHTrs_Trace("mtxCheckCRC - Exit - return %d", pMatrix->next[0]);
//#endif
//		return pMatrix->next[0];
//	}
//	else
//	{
//#ifdef _TRACE_MAXTRIX_
//		BHTrs_Trace("mtxCheckCRC - Exit - return %d", pMatrix->next[1]);
//#endif
//		return pMatrix->next[1];
//	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������ݳ���

@param      HTDLWORK *pWork �����ռ䣻
			TMATRIX  *pMatrix �¼���

@retval     ��һ�׶�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int mtxRecvDataLen(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRecvDataLen - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvDataLen:", pMatrix->phase, pMatrix->event);
#endif

	//          Length 2 byte                 data                DLE          ETX          CRC
	// *RecvPtr         
	// RecvBuffer  [0][1]
	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	if ( 2 == pWork->RecvCnt )							// �������ݽ������
	{
		pWork->wNeedRecvCnt = (pWork->RecvBuff[0] << 8)| pWork->RecvBuff[1];
		if(pWork->wNeedRecvCnt > MAX_DATA_LEN) // ���ݳ����쳣����Ҫ���⴦�����ޣ�
			return PHASE_S1;				   // ת������״̬
		return pMatrix->next[1];
	}
	else
	{
		return pWork->Phase;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���վ���������

@param      HTDLWORK *pWork �����ռ䣻
			TMATRIX  *pMatrix �¼���

@retval     ��һ�׶�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int mtxRecvContext(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	BHTrs_Trace("mtxRecvContext - Enter");
#endif

#ifdef _TRACE_STEP_
	BHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvContext:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// �ı����ݽ������
	if ( pWork->wNeedRecvCnt == (pWork->RecvCnt - 2 ))
	{
#ifdef _TRACE_MAXTRIX_						
		BHTrs_Trace("mtxRecvContext - Exit - return %d", pMatrix->next[1]);
#endif
		return pMatrix->next[1];
	}
	else
	{
#ifdef _TRACE_MAXTRIX_						
		BHTrs_Trace("mtxRecvContext - Exit - return %d", pMatrix->next[0]);
#endif
		return pMatrix->next[0];
	}
}

