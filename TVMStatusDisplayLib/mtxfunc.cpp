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
		strcpy_s(lpResult,iLen,"\r\n");
		for (int i=0; i<iLen; i++){
			bi = lpBuffer[i];
			// 32���ַ�һ��
			if ((i != 0) && (i % 16 == 0)){
				sprintf_s(szText, 12,"\r\n%02X ", bi);
			}
			// ÿ16���ַ���һ���ո�
			else if ((i != 0) && (i % 8 == 0)){
				sprintf_s(szText, 12,"  %02X ", bi);
			}
			// �������
			else{
				sprintf_s(szText, 12,"%02X ", bi);
			}
			// ������󳤶ȵ��ַ�����
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
	// ���·���״̬Ϊ�ȴ�����
	UpdateStatus(pWork, ST_RCVEXE, TRUE);

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
	// ��������
	BYTE* p = pWork->SendBuff;
	_ASSERTE(NULL != p);
	_ASSERTE(NULL != lpData);

	// ��ʱ���(Э��ͷ + ��ͷ + ���� + ���� + У����)
	BYTE* pTemp = pWork->SendTemp;//new BYTE[8 + 12  +iSendLen+ ������ + 2 + 1];
	BYTE* pHeadData = pTemp;
	// Э��ͷ 8
	memset(pTemp,STX,8);
	pTemp += 8;

	// ��ͷ 12
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

	// ���ݳ��� 2
	DWORD dwDataLen = iSendLen;
	memmove(pTemp,&iSendLen,sizeof(DWORD));
	pTemp += 4;


	// �������� N
	memmove(pTemp,lpData,iSendLen);
	pTemp += iSendLen;

	// ����CRCУ���� 2
	WORD wCrcCode = CalcCRC(pHeadData + 8,16 + iSendLen);
	memmove(pTemp,&wCrcCode,sizeof(WORD));
	pTemp += sizeof(WORD);

	// ��ʼ����
	memmove(p,pHeadData,20);
	pWork->SendLength = 20;
	p += 20;
	// ��������Ҫת����(���ȼ����������У��λ)
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


	// ��CRC������ӵ���β
	//memmove(p,&wCrcCode,sizeof(WORD));
	//pWork->SendLength += sizeof(WORD);

	// ��β
	*p++ = ETX;
	pWork->SendLength ++;


	/*if(NULL != pTemp){
	delete [] pTemp;
	pTemp = NULL;
	}*/
	
	/*
	// ƴ֡ͷ
	memset(p,STX,8);
	p+=8;
	pWork->SendLength = 8;
	// ƴ��ͷ
	BYTE* pPackageHead=p;	//��ס��ͷλ��
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
	// ƴ����
	for (int i=0; i< iSendLen; i++){
		// ͸��ģʽ����
		if (STX == lpData[i])
		{//���� 0xa5ת��� 0xa6,0x02
			*p++ = 0xa6;
			pWork->SendLength++;
			*p++ = 0x02;
			pWork->SendLength++;
		} else if(ETX == lpData[i])
		{//���� 0x5b ת��� 0x5b 0x02
			*p++ = 0x5b;
			pWork->SendLength++;
			*p++ = 0x02;
			pWork->SendLength++;
		}else if(0xa6 == lpData[i])
		{//���� 0xa6 ת��� 0xa6 0x01
			*p++ = 0xa6;
			pWork->SendLength++;
			*p++ = 0x01;
			pWork->SendLength++;
		}else if(0x5b == lpData[i])
		{//���� 0x5b ת��� 0x5b 0x01
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
	//��У��
	//ָ��ƫ�����һ����Ҫ��������ֽ�
	unsigned int uiCrcCount=p-pPackageHead;
	WORD crc16Code=CalcCRC(pPackageHead,uiCrcCount);
	memmove(p,&crc16Code,sizeof(WORD ));
	p+=sizeof(WORD );
	pWork->SendLength +=sizeof(WORD );
	//ƴ��β
	*p++ = ETX;
	pWork->SendLength ++;
	// ����BCC
	//*p = BccCalc(lpData, iSendLen);
	*/
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("PreSendData - Exit");
#endif
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

	//����δ���
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
				sprintf_s(&log[2*i+2],3,"%02x",pSendItem->pSending[i]);
			}
			// ��ӡ��־
			CTSPTrs_Trace(log);
		}
		// �����ѷ��������ֽڳ���
		(*pSendItem->pOutLength) += pSendItem->wLastSendingLen;
		iNext = pMatrix->next[1];
		*(pSendItem->pResult) = RES_OK;
		pWork->ReqSend = false;

		//�������������¼�
		SetEventSendFin(pWork);
	}
	// ���������
	else{
		iNext = pMatrix->next[0];
		*(pSendItem->pResult) = RES_OK;
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
		UpdateStatus(pWork, ST_SNDERR, TRUE);
		// ��������ִ�н��
		*(pWork->SndItem.pResult) = MTX_ERR_SEND;
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

inline BOOL unConvStr(BYTE* pData,	//Դ����
	const UINT uiLength,
	const BYTE* pNeedUnConv,				//��Ҫ���滻��byte����
	const UINT uiUnConvLength,
	const BYTE* pConvto,					//�滻֮���byte����
	const UINT uiConvtoLength,
	UINT &uiNewNum,							//ת��֮��byte������ֽ���
	UINT dwNewBufferSize=0,
	BYTE* pNew=NULL							//���ת��֮���byte����
	)							
{
	//���û��ָ���µĴ�������ͻ����Դ�����ڴ���
	//���ָ�����µĴ�������������Դ�����ڴ�����
	BOOL bret=TRUE;
	BOOL bNeedFreeNew=FALSE;
	if(NULL == pNew)
	{
		pNew=(BYTE*)malloc(sizeof(BYTE)*uiLength);
		if(NULL!=pNew)
		{
			//д���ͷ�
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
		//��Ҫ�ͷ���;�������ڴ�����
		free(pNew);
		pNew=NULL;
	}
	return bret;
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
int mtxCalcCrc16(HTDLWORK *pWork, TMATRIX *pMatrix)
{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - Enter");
#endif
	int iNext;
	
	// �����ֽ�������256
	if (pWork->RecvCnt > MAX_MSG_LEN){
		return pWork->Phase;
	}

	// ����BCC
	//Ŀǰ�Ĳ����Ƕ�ȡ��һ����0xa5���ֽڿ�ʼ ��ͷһֱ���������У��λ���� ����������У��λ
	//crcУ���ʱ��ӵ�һ����ʼ�����������У��λ֮ǰ
	WORD crc16Code = CalcCRC(pWork->RecvBuff, pWork->RecvCnt-2);

#ifdef _TRACE_SEND_DATA_
	Conv2Str(m_szText, MAX_TRACE_LEN, pWork->RecvBuff, pWork->RecvCnt);
	CHTrs_Trace("Check BCC - SEQ [%02X] - Block No [%02X]", pWork->SeqRecv, pWork->BlockNumRecv);
	CHTrs_Trace(m_szText);
#endif

	// Crc16 У��
	WORD wRecvCrcCode=0x0000;
	memmove(&wRecvCrcCode,pWork->RecvBuff+(pWork->RecvCnt-2),2);
	if(wRecvCrcCode==crc16Code)
	{
		//������ȷ֮����Ҫ ��ת��
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
			// �������ݳɹ�
			if(TRUE == RecvData(pWork, pWork->RecvBuff, pWork->RecvCnt))
			{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - Receive block %04X - LEN = %04X", pWork->BlockNumRecv, pWork->RecvCnt);
#endif
				iNext = pMatrix->next[1];
			}
			// ��������ʧ��
			else{
#ifdef _TRACE_MAXTRIX_
	CHTrs_Trace("mtxCheckBcc - RecvData Fail - Send NAK");
#endif	
				// ����NAK
				iNext = pMatrix->next[0];
			}
		}else
		{
			//��ת���ַ�ʧ��
		}
	}
	// crcУ��ʧ��
	else{
#ifdef _TRACE_MAXTRIX_
		CHTrs_Trace("mtxCheckBcc - NG - Send NAK");
#endif
		iNext = pMatrix->next[0];
	}
	//�������ʾ��ɷ��� ����һ�����⴦�� ��ΪĿǰָ��� ��ʾ��ɷ�������У�鲻��ȷ
	BYTE btCmd[19]={0x00, 0x80, 0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,
					0x00 ,0x00 ,0x51 ,0x00 ,0x05 ,0x00 ,0xA0 ,0x00 ,0x12 ,0x00 ,0x00 };
	if(memcmp(pWork->RecvBuff,btCmd,19)==0)
	{//����һ����ʾ�����Ϣ
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