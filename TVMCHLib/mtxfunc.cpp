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
	CHTrs_Trace("WriteHtdl - Enter");
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
	CHTrs_Trace("ReadHtdl - Enter");
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
	CHTrs_Trace("ReadHtdl - Exit - return %d", 0);
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
	CHTrs_Trace("UpdateStatus - Enter - [%08X] = %d", dwStatus, bOn);
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
	CHTrs_Trace("UpdateStatus - Exit");
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
	CHTrs_Trace("SendClear - Enter");
#endif

	::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_TXCLEAR);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SendClear - Exit");
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
	CHTrs_Trace("SetEventSendFin - Enter");
#endif
	
	::SetEvent(pWork->hFinSend);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SetEventSendFin - Exit");
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
	CHTrs_Trace("InitRecvBuff - Enter");
#endif

	// ������ݽ��ջ���
	pWork->RecvCnt = 0;
	pWork->RecvPtr = pWork->RecvBuff;
	memset(pWork->RecvBuff, 0, sizeof(pWork->RecvBuff));
	pWork->Dle = false;
	// ���·���״̬Ϊ�ȴ�����
	UpdateStatus(pWork, CHTRS_ST_RCVEXE, TRUE);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("InitRecvBuff - Exit");
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
	CHTrs_Trace("PreSendData - Enter");
#endif
	BYTE* p = pWork->SendBuff;
	BYTE a,b,c,c1,c2,d1,d2,len;
	_ASSERTE(NULL != p);
	_ASSERTE(NULL != lpData);

/*
	pWork->SendLength = 2;
	// ƴ��ͷ
	*p++ = DLE;
	*p++ = STX;
	// ƴ����
	for (int i=0; i< iSendLen; i++){
		// ͸��ģʽ����
		if (DLE == lpData[i]){
			*p++ = DLE;
			pWork->SendLength++;
		}
		*p++ = lpData[i];
		pWork->SendLength++;
	}
	// ƴ��β
	*p++ = DLE;
	*p++ = ETX;
	pWork->SendLength += 3;
	// ����BCC
	*p = BccCalc(lpData, iSendLen);
*/

	*p++ = ENQ;
	if( pWork->SndItem.nSendLength == 0)
	{
		// �Ƿ��д˿��ܣ�
		// ���͵����ݳ���Ϊ0
		a = 0x82;
		b = 0x01;
		len = 2;
	}
	else if( pWork->SndItem.nSendLength <= 254 )
	{
		a = 0xc2;
		b = 0x1;
		c = pWork->SndItem.nSendLength;
		*p++ = a;
		*p++ = b;
		*p++ = c + 1;
		len = 3;
	}
	else if( (pWork->SndItem.nSendLength > 254 ) && (pWork->SndItem.nSendLength <= 1034) )
	{
		a = 0xd2;
		b = 0x01;
		c1 = (pWork->SndItem.nSendLength + 2) & 0xff00;
		c2 = (pWork->SndItem.nSendLength + 2) & 0x00ff;
		*p++ = a;
		*p++ = b;
		*p++ = c1;
		*p++ = c2;
		len = 4;
	}
	else
	{
		// �Ƿ��д˿��ܣ�
		//���͵����ݳ��ȴ�������ı�����
	}

	if(pWork->SndItem.nSendLength != 0)
	{
		memcpy(p, pWork->SndItem.pSending, pWork->SndItem.nSendLength);
	    p += pWork->SndItem.nSendLength;
		pWork->SndItem.wCommand	= MAKEWORD(*(pWork->SndItem.pSending + 1),*(pWork->SndItem.pSending));
	}
	d1 = BccCheck(pWork->SendBuff + 1, pWork->SndItem.nSendLength + len);
	d2 = BccCalc(pWork->SendBuff + 1, pWork->SndItem.nSendLength + len);
	*p++ = d1,
	*p++ = d2;
	pWork->SendLength = pWork->SndItem.nSendLength + len + 1 + 2;//�������ݳ��� + ����ͷ���� + ENQ���� + BCC����
	
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("PreSendData - Exit");
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
	CHTrs_Trace("SendCntl - Enter");
#endif
	BYTE buff = bCode;
	DWORD dwSize;
	BOOL bChk = WriteHtdl(work->hComm,&buff,1,&dwSize,work->bCommunicationMethod,work->hSimObject,work->SendSlotName);

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("SendCntl - Exit");
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
//	CHTrs_Trace("UpdateBlockNo - Enter");
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
//	CHTrs_Trace("UpdateBlockNo - Exit");
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
	CHTrs_Trace("mtxEnquire - Enter");
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, ENQ);

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxEnquire - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ENQ�������Դ�������

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxRetryEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetryEnquire - Enter");
#endif
	int next;
	// ���Դ���++
	pWork->RetryCnt++;
	// δ�ﵽ������Դ���
	if (pWork->RetryCnt < MAX_RETRYSEND){
		BYTE buff[5];
		DWORD dwSize;
		buff[0] = ENQ;
		buff[1] = 0x82;
		buff[2] = 0x01;
		buff[3] = 0x01;
		buff[4] = 0x83;
		BOOL bChk = WriteHtdl(pWork->hComm,buff,5,&dwSize,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);
		next = pMatrix->next[1];
	}
	// �ﵽ������Դ���
	else{
		// ��մ��ڷ��ͻ���
		SendClear(pWork);
		next = pMatrix->next[0];
		// ���·���״̬Ϊ����ʧ��
		UpdateStatus(pWork, CHTRS_ST_SNDERR, TRUE);
/*		pWork->ReqSend = false;
		// ��������ִ�н��

		*(pWork->SndItem.pResult) = CHTRS_MTX_ERR_ENQUIRE;
		// �������������¼�
		SetEventSendFin(pWork);
*/
	}

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRetryEnquire - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetryEnquire - Exit - return %d", next);
#endif
	return next;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ�ط�ENQ

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxTimeOutEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxTimeOutEnquire - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxTimeOutEnquire", pMatrix->phase, pMatrix->event);
#endif

	int next = mtxRetryEnquire(pWork,pMatrix);
	/*int next;
	BYTE buff[5];
	DWORD dwSize;
	buff[0] = ENQ;
	buff[1] = 0x82;
	buff[2] = 0x01;
	buff[3] = 0x01;
	buff[4] = 0x83;
	BOOL bChk = WriteHtdl(pWork->hComm,buff,5,&dwSize,pWork->bCommunicationMethod,pWork->hSimObject,pWork->SendSlotName);
	next = pMatrix->next[1];*/

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxTimeOutEnquire - Exit - return %d", next);
#endif
	return next;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ͳ�ѯ��

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   mtxEnquire return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int mtxSndEnquire(HTDLWORK *pWork, TMATRIX *pMatrix)
{
	return mtxTimeOutEnquire(pWork,pMatrix);
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
	CHTrs_Trace("mtxSend - Enter");
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
				sprintf(&log[2*i+2],"%02x",pSendItem->pSending[i]);
			}
			CHTrs_Trace(log);
		}
		// �����ѷ��������ֽڳ���
		if( pSendItem->wLastSendingLen != 0 )
		{
			(*pSendItem->pOutLength) += pSendItem->wLastSendingLen;
		    // ���·������к�
		    //pWork->SeqSend = 1 + (pWork->SeqSend % 0xFF);
		    iNext = pMatrix->next[1];
		}
		else
			iNext = pMatrix->next[1] + 1;
		*(pSendItem->pResult) = CHTRS_RES_OK;
		pWork->ReqSend = false;

		//�������������¼�
		SetEventSendFin(pWork);
	}
	// ���������
	else{
		iNext = pMatrix->next[0];
		*(pSendItem->pResult) = CHTRS_RES_OK;
		pWork->ReqSend = false;

		//�������������¼�
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
	CHTrs_Trace("mtxRetrySend - Enter");
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
		UpdateStatus(pWork, CHTRS_ST_SNDERR, TRUE);
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = CHTRS_MTX_ERR_SEND;
		// �������������¼�
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
//	CHTrs_Trace("mtxRecvFrameSeq - Enter");
//#endif
//
//	pWork->SeqRecv = pWork->c;
//
//#ifdef _TRACE_STEP_
//	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvFrameSeq %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	CHTrs_Trace("mtxRecvFrameSeq - Exit - return %d", pMatrix->next[1]);
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
//	CHTrs_Trace("mtxRecvBlockNo - Enter");
//#endif
//
//	pWork->BlockNumRecv = pWork->c;
//
//#ifdef _TRACE_STEP_
//	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvBlockNo %02X - Goto %d", pMatrix->phase, pMatrix->event, pWork->c, pMatrix->next[1]);
//#endif
//
//#ifdef _TRACE_MAXTRIX_
//	CHTrs_Trace("mtxRecvBlockNo - Exit - return %d", pMatrix->next[1]);
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
	CHTrs_Trace("mtxOnChar - Enter");
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
		UpdateStatus(pWork, CHTRS_ST_RCVOVR, TRUE);
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
@brief      ����ACK

@param      (i)HTDLWORK *pWork   �����ռ�ṹָ��
@param      (i)TMATRIX *pMatrix  A pointer to structure TMATRIX. (communication's rule)

@retval     int   return the next phase of processing.

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//int mtxAcceptEnq(HTDLWORK *pWork, TMATRIX *pMatrix)
//{
//#ifdef _TRACE_MAXTRIX_
//	CHTrs_Trace("mtxAcceptEnq - Enter");
//#endif
//
//	pWork->bNewENQ = TRUE;
//	SendCntl(pWork, ACK);
//	InitRecvBuff(pWork);
//	
//#ifdef _TRACE_STEP_
//	CHTrs_Trace("Phase [%d] - Event [%d] - mtxAcceptEnq - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
//#endif
//	
//#ifdef _TRACE_MAXTRIX_
//	CHTrs_Trace("mtxAcceptEnq - Exit - return %d", pMatrix->next[1]);
//#endif
//	return pMatrix->next[1];
//}

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
	CHTrs_Trace("mtxCheckBcc - Enter");
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
	CHTrs_Trace("Check BCC - SEQ [%02X] - Block No [%02X]", pWork->SeqRecv, pWork->BlockNumRecv);
	CHTrs_Trace(m_szText);
#endif

	// BCCֵ��ȷ
	if(bcc == pWork->c){
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - OK - Send ACK");
#endif
		// �����ݿ�
		//if(pWork->LastSeqRecv != pWork->SeqRecv){
			// �������ݳɹ�
			if(TRUE == RecvData(pWork, pWork->RecvBuff, pWork->RecvCnt)){
#ifdef _TRACE_MAXTRIX_
//	CHTrs_Trace("mtxCheckBcc - Receive block %04X - LEN = %04X", pWork->BlockNumRecv, pWork->RecvCnt);
#endif
				//pWork->LastSeqRecv = pWork->SeqRecv;
				//pWork->bNewENQ = FALSE;
				//SendCntl(pWork, ACK);
				iNext = pMatrix->next[1];
			}
			// ��������ʧ��
			else{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - RecvData Fail - Send NAK");
#endif	
				// ����NAK
				//SendCntl(pWork, NAK);
				iNext = pMatrix->next[0];
			}
		//}
		// �����ݿ�
//		else{
//#ifdef _TRACE_MAXTRIX_
//	CHTrs_Trace("mtxCheckBcc - Old Block Received - Send ACK");
//#endif
//			// ����ACK
//			SendCntl(pWork, ACK);
//			iNext = pMatrix->next[0]; //continue receive other chunks
//		}
//		if((pWork->BlockNumRecv & 0x80) == 0x00){
//#ifdef _TRACE_MAXTRIX_
//			CHTrs_Trace("mtxCheckBcc - Last Block received");
//#endif
//			//Receive Done
//			iNext = pMatrix->next[1];
//		}
	}
	// BCCֵ����
	else{
#ifdef _TRACE_MAXTRIX_
		CHTrs_Trace("mtxCheckBcc - NG - Send NAK");
#endif
		// ����ENQ
		SendCntl(pWork, ENQ);
		iNext = pMatrix->next[0];
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

//---------------lichao---------------------------
int mtxSndACK(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSndACK - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxSndACK - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, ACK);	

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSndACK - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

int mtxRetrySndACK(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySndACK - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRetrySndACK - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

	int next;
	pWork->RetryCnt++;
	if (pWork->RetryCnt < MAX_RETRYSEND){
		SendCntl(pWork, ACK);
		next = pMatrix->next[1];
	}
	else{
		// ��մ��ڷ��ͻ���
		SendClear(pWork);
		next = pMatrix->next[0];
		// ���·���״̬Ϊ����ʧ��
		UpdateStatus(pWork, CHTRS_ST_SNDERR, TRUE);
/*
		pWork->ReqSend = false;
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = CHTRS_MTX_ERR_SEND;
		// �������������¼�
		SetEventSendFin(pWork);
*/
	}

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySndACK - Exit - return %d", next);
#endif
	return next;
}

int mtxSndNAK(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSndNAK - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxSndNAK - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, NAK);	

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSndNAK - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

int mtxRetrySndNAK(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySndNAK - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRetrySndNAK - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

	int next;
	pWork->RetryCnt++;
	if (pWork->RetryCnt < MAX_RETRYSEND){
		SendCntl(pWork, NAK);
		next = pMatrix->next[1];
	}
	else{
		// ��մ��ڷ��ͻ���
		SendClear(pWork);
		next = pMatrix->next[0];
		// ���·���״̬Ϊ����ʧ��
		UpdateStatus(pWork, CHTRS_ST_SNDERR, TRUE);
/*
		pWork->ReqSend = false;
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = CHTRS_MTX_ERR_SEND;
		// �������������¼�
		SetEventSendFin(pWork);
*/
	}

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySndNAK - Exit - return %d", next);
#endif
	return next;
}

int mtxSndEOT(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSndEOT - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxSndEOT - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

	pWork->RetryCnt = 0;
	SendCntl(pWork, EOT);	

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxSndEOT - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];
}

int mtxRetrySndEOT(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySndEOT - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRetrySndEOT - Goto %d", pMatrix->phase, pMatrix->event, pMatrix->next[1]);
#endif

	int next;
	pWork->RetryCnt++;
	if (pWork->RetryCnt < MAX_RETRYSEND){
		SendCntl(pWork, EOT);
		next = pMatrix->next[1];
	}
	else{
		// ��մ��ڷ��ͻ���
		SendClear(pWork);
		next = pMatrix->next[0];
		// ���·���״̬Ϊ����ʧ��
		UpdateStatus(pWork, CHTRS_ST_SNDERR, TRUE);
/*
		pWork->ReqSend = false;
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = CHTRS_MTX_ERR_SEND;
		// �������������¼�
		SetEventSendFin(pWork);
*/
	}

#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRetrySndEOT - Exit - return %d", next);
#endif
	return next;
}

int mtxCheckDataA(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckDataA - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxCheckDataA:", pMatrix->phase, pMatrix->event);
#endif
	InitRecvBuff(pWork);
	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// �ж��ǲ����������գ�����У��b����
	if(pWork->c == INQUIRY_RSP || pWork->c == RECV_DATA2 || pWork->c == RECV_DATA3){
		return pMatrix->next[1];
	}
	else{
		return pMatrix->next[0];
	}
//	BYTE temp = pWork->c;
//	BYTE tempA = temp >> 7;
//	BYTE tempB = temp & 15;
//
//	if( tempA || tempB)						// ���Ǵ���--->��λ����λ�ֵ�ַ��Ϊ0�����ݴ���
//	{
//#ifdef _TRACE_MAXTRIX_
//		CHTrs_Trace("mtxCheckDataA - Exit - return %d", pMatrix->next[0]);
//#endif
//		return pMatrix->next[0];
//	}
//
//	tempA = temp >> 4;
//	if( tempA == 1 || tempA == 4 || tempA == 5 ) // ��ѯ��Ӧ �� ���254��1034λ�������ı��������ݿ�
//	{
//#ifdef _TRACE_MAXTRIX_
//		CHTrs_Trace("mtxCheckDataA - Exit - return %d", pMatrix->next[1]);
//#endif
//		return pMatrix->next[1];
//	}
//	else
//	{
//#ifdef _TRACE_MAXTRIX_
//		CHTrs_Trace("mtxCheckDataA - Exit - return %d", pMatrix->next[0]);
//#endif
//		return pMatrix->next[0];
//	}
}

int mtxCheckDataB(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckDataB - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxCheckDataB:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// �ж�B�����Ƿ�����
	if(pWork->c != RECV_B){
		return pMatrix->next[0];
	}

	// �ж�A���յ���������
	BYTE a	= *(pWork->RecvPtr - pWork->RecvCnt);

	// ��ѯ�鷵��
	if(a == INQUIRY_RSP){
		return PHASE_S9;			// ׼������BCC�еĵ�һλ
	}
	
	// ���ݿ鷵��
	if(a == RECV_DATA2 || a == RECV_DATA3){
		return pMatrix->next[1];	// ׼�����ճ���
	}

//	BYTE temp = pWork->c;
//	BYTE tempA = temp >> 4;
//	if( tempA != 2 ) 
//	{
//		return pMatrix->next[0];			// ���ݴ���
//	}
//	
//	temp = *(pWork->RecvPtr - pWork->RecvCnt);			// ��ȡ����a 		
//	tempA = temp >> 4;
//	if( tempA == 4 || tempA == 5 )			// ���254��1034λ�������ı��������ݿ�
//	{
//#ifdef _TRACE_MAXTRIX_
//		CHTrs_Trace("mtxCheckDataB - Exit - return %d", pWork->Phase + 1);
//#endif
//		return pWork->Phase + 1;			// ����״̬6����ǰ��״̬5��
//	}
//	else if( tempA == 1 )					// ��ѯ��Ӧ
//	{
//#ifdef _TRACE_MAXTRIX_
//		CHTrs_Trace("mtxCheckDataB - Exit - return %d", pMatrix->next[1]);
//#endif
//		return pMatrix->next[1];
//	}
//	else									// ����a����
//	{	
//#ifdef _TRACE_MAXTRIX_						
//		CHTrs_Trace("mtxCheckDataB - Exit - return %d", pMatrix->next[0]);
//#endif
//		return pMatrix->next[0];
//	}
}

int mtxCheckDataC1(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckDataC1 - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxCheckDataC1:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// ȡ����A��ֵ
	BYTE a = *(pWork->RecvPtr - pWork->RecvCnt);

	// ����2������
	if(a == RECV_DATA2){
		if(pWork->c <= 0xFF){
			pWork->wNeedRecvCnt = pWork->c + 2;	// �Լ��ĳ��ȣ�����a b���ݵĳ���
			return pMatrix->next[0];			// ׼����������λ
		}
		else
			return pMatrix->next[1];
	}
	// ����3������
	if(a == RECV_DATA3){
		return PHASE_S7;				// ׼����������3�����ݳ��ȵ�C2λ
	}
	// ��������
	return pMatrix->next[1];
/*
	�ı��ߴ����ݵ�һλ������C1ȷ��
*/
//	BYTE temp, tempA;
//	temp = *(pWork->RecvPtr - pWork->RecvCnt);			// ��ȡ����a 		
//	tempA = temp >> 4;
//	if( tempA == 4 )						// ���254λ�������ı��������ݿ�
//	{
//		if( pWork->c <= 254 + 1)			//�ߴ�����С��254+ 1(1�ǳߴ����ݱ�ʵ�����ݴ�С����Ĵ�С��
//		{
//#ifdef _TRACE_MAXTRIX_
//			CHTrs_Trace("mtxCheckDataC1 - Exit - return %d", pMatrix->next[0]);
//#endif
//			return pMatrix->next[0];
//		}
//		else
//		{
//#ifdef _TRACE_MAXTRIX_
//			CHTrs_Trace("mtxCheckDataC1 - Exit - return %d", pMatrix->next[1]);
//#endif
//			return pMatrix->next[1];
//		}
//	}
//	else if( tempA == 5 )					// ���1034λ�������ı��������ݿ�)
//	{
//#ifdef _TRACE_MAXTRIX_
//		CHTrs_Trace("mtxCheckDataC1 - Exit - return %d", pWork->Phase + 1);
//#endif
//		return pWork->Phase + 1;	// ����״̬6����ǰ��״̬5��
//	}
//	else									// ����c1����
//	{	
//#ifdef _TRACE_MAXTRIX_						
//		CHTrs_Trace("mtxCheckDataC1 - Exit - return %d", pMatrix->next[1]);
//#endif
//		return pMatrix->next[1];
//	}
}

int mtxCheckDataC2(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckDataC2 - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxCheckDataC2:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

/*
	�ı��ߴ�����ȷ��
*/
	WORD temp = *((WORD *)(pWork->RecvPtr - 2));
	if( temp <= 1034 + 2)		// 1034��������ݳ��ȣ�2�ǳ������ݱ�ʵ�ʳ��ȶ���Ĵ�С				
	{
#ifdef _TRACE_MAXTRIX_
		CHTrs_Trace("mtxCheckDataC2 - Exit - return %d", pMatrix->next[0]);
#endif
		pWork->wNeedRecvCnt = temp + 2;		// Ӧ�ý��յ����ݵĳ���,����a b c1 c2�ĳ���
		return pMatrix->next[0];			
	}
	else									
	{	
#ifdef _TRACE_MAXTRIX_						
		CHTrs_Trace("mtxCheckDataC2 - Exit - return %d", pMatrix->next[1]);
#endif
		return pMatrix->next[1];
	}
}


int mtxRecvContext(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRecvContext - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvContext:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// ���ݽ������
	if(pWork->wNeedRecvCnt == pWork->RecvCnt){
		return pMatrix->next[1];
	}
	// ������������
	else{
		return pMatrix->next[0];
	}
//	BYTE* base = (pWork->RecvPtr - pWork->RecvCnt);			// ��ȡ����a 
//	BYTE temp = *base;
//	BYTE tempA = temp >> 4;
//	WORD len = 0;
//	WORD headLen = 0;				// ���ݿ�֮ǰ������ENQ�ĳ���
//	WORD externSize = 0;			// ��������ֵ��ʵ�����ݳ��ȶ���Ĵ�С
//	if( tempA == 4 )				// ���254λ�������ı��������ݿ�
//	{
//		len = *(base + 2);
//		headLen = 3;
//		externSize = 1;
//	}
//	else if( tempA == 5 )			// ���1034λ�������ı��������ݿ�
//	{
//		len = *(WORD *)(base + 2);
//		headLen = 4;
//		externSize = 2;
//	}
//	else							// ���ݴ���
//	{	
//#ifdef _TRACE_MAXTRIX_						
//		CHTrs_Trace("mtxRecvContext - Exit - return %d", pMatrix->next[0]);
//#endif
//		return pMatrix->next[0];		
//	}
//
///*
//	�ı����ݽ�������ȷ��
//		OK?
//*/
//
//	if ( pWork->RecvCnt - headLen == len - externSize)				// �ı����ݽ������
//	{
//#ifdef _TRACE_MAXTRIX_						
//		CHTrs_Trace("mtxRecvContext - Exit - return %d", pMatrix->next[1]);
//#endif
//		return pMatrix->next[1];
//	}
//
//#ifdef _TRACE_MAXTRIX_						
//	CHTrs_Trace("mtxRecvContext - Exit - return %d", pWork->Phase);
//#endif
//	return pWork->Phase;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����BCC�ĵ�һλ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int mtxRecvBCC(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRecvBCC - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvBCC:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

#ifdef _TRACE_MAXTRIX_						
	CHTrs_Trace("mtxRecvBCC - Exit - return %d", pMatrix->next[1]);
#endif
	return pMatrix->next[1];		
}


int mtxCheckBCC(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxRecvBCC - Enter");
#endif

#ifdef _TRACE_STEP_
	CHTrs_Trace("Phase [%d] - Event [%d] - mtxRecvBCC:", pMatrix->phase, pMatrix->event);
#endif

	*(pWork->RecvPtr++) = pWork->c;
	pWork->RecvCnt++;

	// ȡa��ֵ
	BYTE a = pWork->RecvBuff[0];

	// BCCȷ��
	BYTE btCheck	= BccCheck(pWork->RecvBuff,pWork->RecvCnt - 2);
	BYTE btCalc		= BccCalc(pWork->RecvBuff,pWork->RecvCnt - 2);

	bool IsBccOk = false;
	if(btCheck == *(pWork->RecvPtr - 2) && btCalc == *(pWork->RecvPtr - 1)){
		IsBccOk = true;
	}

	// ����ǲ�ѯ��������
	if(a == INQUIRY_RSP){
		pWork->RetryCnt = 0;
		InitRecvBuff(pWork);
		return pMatrix->next[0];
	}
	// �������ݿ�2
	else if(a == RECV_DATA2){
		WORD wCommand = MAKEWORD(pWork->RecvBuff[4],pWork->RecvBuff[3]);
		if(IsBccOk){
			// �������������������ͬ
			if( wCommand == pWork->SndItem.wCommand){
				RecvData(pWork,pWork->RecvBuff + 3,pWork->RecvCnt - 5);
			}
			return mtxSndACK(pWork,pMatrix);// Send ACK
		}
		else{
			mtxSndNAK(pWork,pMatrix);// Send NAK
			return PHASE_S12;
		}
	}
	// �������ݿ�3
	else if(a == RECV_DATA3){
		WORD wCommand = MAKEWORD(pWork->RecvBuff[5],pWork->RecvBuff[4]);
		if(IsBccOk ) {
			if (wCommand == pWork->SndItem.wCommand){
				RecvData(pWork,pWork->RecvBuff + 4,pWork->RecvCnt - 6);
			}
			return mtxSndACK(pWork,pMatrix);// Send ACK
		}
		else{
			mtxSndNAK(pWork,pMatrix);// Send NAK
			return PHASE_S12;
		}
	}

	return pMatrix->next[0];
	//BYTE* base = (pWork->RecvPtr - pWork->RecvCnt);			// ��ȡ����a 
	//BYTE temp = *base;
	//BYTE tempA = temp >> 4;

	//// BCCȷ��
	//BOOL IsOk = FALSE;;
	//BYTE checkResult1 = BccCheck(pWork->RecvBuff, pWork->RecvCnt - 2);
	//BYTE checkResult2 = BccCalc(pWork->RecvBuff, pWork->RecvCnt - 2); 
	//if( checkResult1 == *(pWork->RecvPtr - 2) )
	//{
	//	if( checkResult2 == *(pWork->RecvPtr - 1) )
	//	{
	//		IsOk = TRUE;
	//	}
	//}

	//switch( tempA)
	//{
	//case 1:	//��ѯ��Ӧ
	//	{
	//		if (IsOk)
	//		{
	//			return pMatrix->next[0];	//�ص�����
	//		}
	//		else
	//		{
	//			pMatrix->next[0] = PHASE_S1;
	//			pMatrix->next[1] = PHASE_S3;
	//			return mtxRetryEnquire(pWork, pMatrix);	//�ط���ѯ
	//		}
	//	}
	//case 4://�ı��������Ϊ254�����ݽ���
	//	{
	//		if (IsOk && RecvData(pWork, pWork->RecvBuff + 3, pWork->RecvCnt - 3 - 2))	//ȥ��a��b��c, d1, d2
	//		{
	//			return mtxSndACK(pWork, pMatrix);
	//		}
	//		else
	//		{
	//			pMatrix->next[0] = PHASE_S1;
	//			pMatrix->next[1] = PHASE_S12;
	//			return mtxSndNAK(pWork, pMatrix);
	//		}
	//	}
	//case 5://�ı��������Ϊ1023�����ݽ���
	//	{
	//		if (IsOk && RecvData(pWork, pWork->RecvBuff + 4, pWork->RecvCnt - 4))	//ȥ��a��b��c1��c2
	//		{
	//			return mtxSndACK(pWork, pMatrix);
	//		}
	//		else
	//		{
	//			pMatrix->next[0] = PHASE_S1;
	//			pMatrix->next[1] = PHASE_S12;
	//			return mtxSndNAK(pWork, pMatrix);
	//		}
	//	}
	//default:// ���ݴ���
	//	{
	//		// ���ݴ���
	//		pMatrix->next[0] = PHASE_S1;
	//		pMatrix->next[1] = PHASE_S3;
	//		return mtxRetryEnquire(pWork, pMatrix);
	//	}
	//}	
}