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
	// ���������Ƿ��з���
	if(lpData[0] == CMD_BH_GET_STATUS || lpData[0] ==CMD_BH_INFO || lpData[0] ==CMD_BH_READ || lpData[0] ==CMD_BH_WRITE){
		pWork->bisNeedResponse = true;
	}
	else{
		pWork->bisNeedResponse = false;
	}

	// ���������Ƿ�ȴ�ON/OFF(TF && B* && V1)
	if(lpData[0] == CMD_BH_RESET || lpData[0] == CMD_BH_CONFIG /*|| memcmp(lpData,"V1",2) == 0*/){
		pWork->bisNeedWaitOnOff = true;
	}
	else{
		pWork->bisNeedWaitOnOff = false;
	}

	pWork->SendLength = 1;
	// ƴ��ͷ
	*p++ = STX;
	// ƴ����
	for (int i=0; i< iSendLen; i++){
		*p++ = lpData[i];
		pWork->SendLength++;
	}
	// ƴ��β
	*p++ = ETX;
	pWork->SendLength ++;
	// ����BCC
	*p = BccCalc(lpData, iSendLen);
	pWork->SendLength ++;

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
	buff[0] = bCode;	
	BOOL bChk = WriteHtdl(work->hComm,buff,1,&dwSize,work->bCommunicationMethod,work->hSimObject,work->SendSlotName);

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
//int mtxEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxEnquire - Enter");
//#endif
//
//	pWork->RetryCnt = 0;
//	SendCntl(pWork, ENQ);
//
//#ifdef _TRACE_STEP_
//	BHTrs_Trace("Phase [%d] - Event [%d] - mtxEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxEnquire - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ENQ������3��

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ENQ������3�γ�ʱ

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxTimeOutEnquire - Enter");
//#endif
//
//#ifdef _TRACE_STEP_
//	BHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutEnquire", pMatrix->phase, pMatrix->event);
//#endif
//
//	int iNext = mtxRetryEnquire(pWork, pMatrix);
//
//#ifdef _TRACE_MAXTRIX_
//	BHTrs_Trace("mtxTimeOutEnquire - Exit - return %d", iNext);
//#endif
//	return iNext;
//}

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

	// ����ǰ����Ƿ���δ���յ����ݣ�
	//if(pWork->pBuffQue->nCnt > 0){
	//	// ���ڴ����յ����ݣ��ȴ�һ���
	//	if((pWork->DrvStat & BHTRS_ST_RCVOVR) && pWork->RetryCnt < MAX_RETRYSEND){
	//		pWork->RetryCnt++;
	//		Sleep(50);
	//		return pMatrix->next[0];
	//	}
	//	// ����3�ζ�û�˽��գ�ֱ��ɾ��
	//	else /*if(pWork->RetryCnt >= MAX_RETRYSEND)*/{
	//		UpdateStatus(pWork, BHTRS_ST_RCVOVR, FALSE);
	//		pWork->pBuffQue->nCnt = 0;
	//	}
	//}
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
			/*for(unsigned short i=0;i < pSendItem->wLastSendingLen; i++) {
				sprintf(&log[i+2],"%c",pSendItem->pSending[i]);
			}*/
			memcpy(&log[2],pSendItem->pSending,pSendItem->wLastSendingLen);
			BHTrs_Trace(log);
		}
		// �����ѷ��������ֽڳ���
		(*pSendItem->pOutLength) += pSendItem->wLastSendingLen;
		// ���·������к�
		//pWork->SeqSend = 1 + (pWork->SeqSend % 0xFF);

		// �����ǰ�ѷ�����ɣ��������
		iRemainLen = pSendItem->nSendLength - (*pSendItem->pOutLength);
// 		if(iRemainLen <= 0){
// 			if(NULL != pSendItem->pResult)
// 				*(pSendItem->pResult) = BHTRS_RES_OK;
// 			pWork->ReqSend = false;
// 
// 			//�������������¼�
// 			SetEventSendFin(pWork);
// 		}
		iNext = pMatrix->next[1];
	}
	// ���������
	else{
		iNext = pMatrix->next[0];
		if(NULL != pSendItem->pResult)
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
		if(NULL != pWork->SndItem.pResult)
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
	else if (pWork->bisNeedWaitOnOff){
		iNextPhase = PHASE_S2;//���ڵ�ǰ״̬�ڣ��ȴ�OFF����
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
	BHTrs_Trace("mtxCheckBcc - Receive block %04X - LEN = %04X", pWork->BlockNumRecv, pWork->RecvCnt);
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
/*
@brief      ��鷢�Ͷ��г�ʱ������Ҫ���÷���ʧ��

@param      HTDLWORK* pWork;(�����ռ�ָ��)

@retval     int iNextPhase;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int checkSendTimeout(HTDLWORK* pWork,TMATRIX *pMatrix){

	// ����һ��״̬��æµ���������ݷ��ͳ�ʱʱ
	if(pWork->ReqSend == true && pWork->LastEvent == EVENT_OFF){
		if(NULL != pWork->SndItem.pResult)
			*(pWork->SndItem.pResult) = BHTRS_RES_ERR_TIMEOUT;
		pWork->LastEvent == EVENT_NONE;
		pWork->ReqSend = false;
		SetEventSendFin(pWork);
		BHTrs_Trace("Send item timeout!");
	}

	// ���ն���������
	if((pWork->DrvStat & BHTRS_ST_RCVQUE) != 0){
		UpdateStatus(pWork, BHTRS_ST_RCVQUE, FALSE);
	}
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��鷢�͹����н���æµ״̬

@param      HTDLWORK* pWork;(�����ռ�ָ��)

@retval     int iNextPhase;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int checkSendPaddingCmd(HTDLWORK *pWork, TMATRIX *pMatrix){

	if(pWork->ReqSend && pWork->LastEvent == EVENT_ON && !pWork->bisNeedWaitOnOff){
		return mtxRetrySend(pWork,pMatrix);
	}

	return pMatrix->next[0];
}