
/*****************************************************************************/
/* FILE NAME     : server.cpp                                                */
/* PROGRAM NAME  : This is a library which handling the request from         */
/*                      upper layer (API interface).                         */
/* FUNCTION      : 1.                                                        */
/*                                                                           */
/*****************************************************************************/


#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "common.h"
#include <shlwapi.h>

#define MAX_SEND_COUNT	1
#define MAX_RECV_COUNT	1

BOOL OpenHTDL(PDEVICETBL pTbl);
BOOL CloseHTDL(PDEVICETBL pTbl);
BOOL InitServThread(IN PDEVICETBL pTbl);		// ��ʼ�������߳�
void EndServThread(IN PDEVICETBL pTbl);
DWORD WINAPI ServThread(IN LPVOID lpParam);
BOOL InitComPort(IN PDEVICETBL pTbl);			// ��ʼ�����򿪴���(�����ʲ�)
BOOL EndComPort(IN PDEVICETBL pTbl);			// �رղ��ͷŴ���(�ͷ��ʲ�)
void GetComunicationMethod(PDEVICETBL pTbl);	// ��ȡͨ�ŷ�ʽ
BOOL IdlCheck(IN PDEVICETBL pTbl);              // ����Ƿ����µ�������Ҫ���ջ��߷���
BOOL SendEvent(IN PDEVICETBL pTbl);             // ����Ƿ����µ�������Ҫ���ջ��߷���
BOOL RecvEvent(IN PDEVICETBL pTbl);
BOOL GetSendRequest(IN HTDLWORK* pWork, IN SENDITEM* pItem, OUT IOTRS_RESULT* pRes);
BOOL GetRecvRequest(IN HTDLWORK* pWork,	IN RECVITEM* pItem,	OUT IOTRS_RESULT* pRes);
//INT  ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰӦ�ó����·��

@param      (o)  LPTSTR lpszBuf            ·�����Ŀռ�
@param      (i)  DWORD ucchMax             ����·������

@retval     INT   \n
E_INVALID_PARAMS:������Ч, E_WIN32_ERROR:ϵͳ����, ����:·����ʵ�ʳ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//INT ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax)
//{
//	DWORD dwLen;
//	TCHAR szStore[MAX_PATH + 1];   /* + 1 for zero character */
//
//	/* Check the sanity of all input parameters */
//	if (lpszBuf != NULL){
//		_ASSERTE(ucchMax > 0);		
//		if ((0 == ucchMax) || IsBadWritePtr(lpszBuf, ucchMax * sizeof(TCHAR))){
//			return -1;
//		}
//	}
//
//	dwLen = GetModuleFileName(NULL, szStore, MAX_PATH);
//	if(0 == dwLen){
//		return -2;
//	}
//	szStore[MAX_PATH] = '\0';
//
//	PathRemoveFileSpec(szStore);
//	PathAddBackslash(szStore);
//
//	/* The request buffer size return is len of path and +1 for null character */
//	dwLen = lstrlen(szStore) + 1;
//	if(lpszBuf != NULL){
//		lstrcpyn(lpszBuf, szStore, ucchMax);
//	}
//	return dwLen;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ�����򿪴���(�����ʲ�)

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     TRUE:�ɹ� FALSE:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL InitComPort(IN PDEVICETBL pTbl)
{
	BOOL bRet = FALSE;
	char buff[128];
	COMMTIMEOUTS timeout;
	DCB dcb;
	int speed;
	DWORD nWait;

	#ifdef _TRACE_SERVER_
		IOTrs_Trace("InitComPort - Enter");
	#endif

	pTbl->nResult = IOTRS_RES_WINERR;
	// ����ͨ��
	do{
		if(pTbl->pWork->bCommunicationMethod != COM_PORT){	
			break;
		}
		// �򿪴���
		sprintf(buff, "\\\\.\\COM%d", (pTbl->Parm.ComPort + 1));
		pTbl->pWork->hComm = CreateFileA(buff,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if (pTbl->pWork->hComm == INVALID_HANDLE_VALUE){
			pTbl->nResult = IOTRS_HTDL_ERR_PORT;
			break;
		}
		// ����������������С
		::SetupComm(pTbl->pWork->hComm, 4096, 4096);
		// �����ʷ���
		speed = pTbl->Parm.Speed;
		if (speed < 19200){
			pTbl->pWork->ComSpeed = 0;
		}
		else{
			pTbl->pWork->ComSpeed = 1;
		}

		// ȡ�õ�ǰ����״̬
		if (FALSE == ::GetCommState(pTbl->pWork->hComm, &dcb)){
			#ifdef _TRACE_SERVER_
				IOTrs_Trace("InitComPort - GetCommState() = FALSE");
			#endif
			pTbl->nResult = IOTRS_RES_WINERR;
			::CloseHandle(pTbl->pWork->hComm);
			break;
		}
		// ����DCB����
		dcb.fBinary = TRUE;						// ������ģʽ
		dcb.fParity = TRUE;						// ������żУ��
		dcb.BaudRate = pTbl->Parm.Speed;		// ���ò�����
		dcb.StopBits = ONESTOPBIT;				// ֹͣλ(1)
		dcb.Parity = NOPARITY;				// ��żУ�鷽��
		dcb.ByteSize = 8;                       // ����λ
		dcb.fAbortOnError = TRUE;               // �д�����ʱ��ֹ��д����
		dcb.fOutxCtsFlow = FALSE;				// ָ��CTS�Ƿ����ڼ�ⷢ�Ϳ��ơ���ΪTRUEʱCTSΪOFF�����ͽ������𡣣����������
		dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS��ΪOFF 
		dcb.fDtrControl = DTR_CONTROL_DISABLE;  // DTR��ΪOFF 
		dcb.fOutxDsrFlow = FALSE;				// ָ��DSR�Ƿ����ڼ�ⷢ�Ϳ��ơ���ΪTRUEʱDSRΪOFF�����ͽ�������
		dcb.fDsrSensitivity = FALSE;			// ����ֵΪTRUEʱDSRΪOFFʱ���յ��ֽڱ����� 
		
		// ���ô���״̬
		if (FALSE == ::SetCommState(pTbl->pWork->hComm, &dcb)){
			#ifdef _TRACE_SERVER_
				IOTrs_Trace("InitComPort - SetCommState() = FALSE");
			#endif
			pTbl->nResult = IOTRS_RES_WINERR;
			::CloseHandle(pTbl->pWork->hComm);
			break;
		}
		
		// ���ó�ʱ����
		memset(&timeout, 0, sizeof(timeout));
		timeout.ReadTotalTimeoutMultiplier = 2;		// ��ʱ��ϵ��
		timeout.ReadTotalTimeoutConstant = 0;		// ��ʱ�䳣��
		timeout.ReadIntervalTimeout = 15;			// �������ʱ 
		timeout.WriteTotalTimeoutConstant = 0;      // дʱ�䳣��
		timeout.WriteTotalTimeoutMultiplier = 3;    // дʱ��ϵ��
		if (FALSE == ::SetCommTimeouts(pTbl->pWork->hComm, &timeout)){
			#ifdef _TRACE_SERVER_
				IOTrs_Trace("InitComPort - SetCommTimeouts() = FALSE");
			#endif
			pTbl->nResult = IOTRS_RES_WINERR;
			::CloseHandle(pTbl->pWork->hComm);
			break;
		}
		// �ȴ�������Ч
		::Sleep(500L); 

		DWORD dwErrorMask;
		COMSTAT comstat;
		::ClearCommError(pTbl->pWork->hComm, &dwErrorMask, &comstat);
		::PurgeComm(pTbl->pWork->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
		// Success
		pTbl->nResult = IOTRS_RES_OK;
		bRet = TRUE;

		nWait = ::WaitForSingleObject(pTbl->pWork->hMutex, INFINITE);
		if (nWait != WAIT_FAILED) {
			pTbl->pWork->DrvStat &= IOTRS_ST_DTR;
			pTbl->pWork->DrvStat |= IOTRS_ST_RTS;
			::ReleaseMutex(pTbl->pWork->hMutex);
		}
		
	} while (0);

	// �ʲ�ͨ��
	do{
		if( pTbl->pWork->bCommunicationMethod != MAIL_SLOT){
			break;	
		}
		// �����ʲ�
		pTbl->pWork->hSimObject = ::CreateMailslotA( (char *)pTbl->pWork->ReceiveSlotName, 4095, 100, NULL );
		if(pTbl->pWork->hSimObject == INVALID_HANDLE_VALUE ){
			IOTrs_ShowLastError(NULL);
			break;
		}
#ifdef _TRACE_SERVER_
	IOTrs_Trace("InitComPort - Create MailSlot %s", pTbl->pWork->ReceiveSlotName);
#endif
		pTbl->pWork->MailBuff[0] = NULL;
		bRet = TRUE;
	}while(0);

	// �ܵ�ͨ��
	do{
		if(pTbl->pWork->bCommunicationMethod != NAME_PIPE){
			break;	
		}
		if(pTbl->pWork->hSimObject == NULL){
			// �����ܵ�
			char *name_ = "\\\\.\\pipe\\THSIM";
			pTbl->pWork->hSimObject = CreateNamedPipeA(
				name_,						 // pipe name 
				PIPE_ACCESS_DUPLEX |		 // read/write access  
				FILE_FLAG_OVERLAPPED,       
				PIPE_TYPE_MESSAGE |          // byte type pipe 
				PIPE_READMODE_MESSAGE |      // byte-read mode 
				PIPE_WAIT,                   // blocking mode 
				1,                           // max. instances  
				3000,						 // output buffer size 
				3000,						 // input buffer size 
				0,							 // client time-out 
				(LPSECURITY_ATTRIBUTES)NULL);// no security attribute 		
			if (pTbl->pWork->hSimObject == INVALID_HANDLE_VALUE){
				IOTrs_ShowLastError(NULL);
				break;
			}
	#ifdef _TRACE_SERVER_
			IOTrs_Trace("InitComPort - Create NamedPipe %s", name_);
	#endif
			// ���ӿͻ���,��ʧ����رչܵ� 
			if(ConnectNamedPipe(pTbl->pWork->hSimObject, NULL) == FALSE){	
				CloseHandle(pTbl->pWork->hSimObject); 
				break;
			}
		}
		bRet = TRUE;
	}while(0);

#ifdef _TRACE_SERVER_
	IOTrs_Trace("InitComPort - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �رղ��ͷŴ���(�ͷ��ʲ�)

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     TRUE:�ɹ� FALSE:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL EndComPort(IN PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("EndComPort - Enter");
#endif
	BOOL bRet = FALSE;
	// ����ͨ��
	do{
		// �ʲ�ͨ��������
		if(pTbl->pWork->bCommunicationMethod != COM_PORT){
			break;
		}
		if(::CloseHandle(pTbl->pWork->hComm) == FALSE){
			break;
		}
		bRet = TRUE;
	} while (0) ;


	// �ʲ۴���
	do{
		if(pTbl->pWork->bCommunicationMethod != MAIL_SLOT){	
			break;
		}
		if(::CloseHandle(pTbl->pWork->hSimObject) == FALSE){
			break;
		}
		bRet = TRUE;
	}while( 0 );

	// �ܵ�����
	do{
		if(pTbl->pWork->bCommunicationMethod != NAME_PIPE){	
			break;
		}
		//if(::CloseHandle(pTbl->pWork->hSimObject) == FALSE){
		//	break;
		//}
		bRet = TRUE;
	}while( 0 );

#ifdef _TRACE_SERVER_
	IOTrs_Trace("EndComPort - Exit - return %d", bRet);
#endif
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡͨ�ŷ�ʽ

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void GetComunicationMethod(PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("GetComunicationMethod - Enter");
#endif
	LONG	lRet;
	HKEY	hkey = NULL;
	DWORD	dwType, dwSize ;
	char	sBuf[128], sData[_MAX_PATH];

	// ȡ��ͨ������
	//TCHAR szAppPath[_MAX_PATH];
	//ComGetAppPath(szAppPath,_MAX_PATH);
	//strcat(szAppPath,DEVICE_INI_FILE_NAME);
	//int CommunicationMethod = GetPrivateProfileInt(TH_CONFIG_SECTION,TH_CONMU_METHOD, COM_PORT, szAppPath);
	pTbl->pWork->bCommunicationMethod = COM_PORT;
	
	// �ʲ�
	if(pTbl->pWork->bCommunicationMethod == MAIL_SLOT){
		pTbl->pWork->bCommunicationMethod = NAME_PIPE;
		//do{
		//	// ��ע���
		//	lRet = ::RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\IOInfo",0,KEY_READ,&hkey);
		//	if(lRet != ERROR_SUCCESS){
		//		break;
		//	}

		//	sprintf(sBuf,"IOSim");	
		//	lRet = ::RegQueryValueEx(hkey,sBuf,NULL,&dwType,(BYTE*)sData,&dwSize);
		//	if(lRet != ERROR_SUCCESS){
		//		break;
		//	}
		//	if(strcmp(sData,"COM") != 0){
		//		break;
		//	}
		//	// ȡ�÷����ʲ�����
		//	dwSize = _MAX_PATH;
		//	sprintf(sBuf,"DriverSendSlot");
		//	lRet = ::RegQueryValueEx(hkey,sBuf,NULL,&dwType,(BYTE*)sData,&dwSize);
		//	if(lRet == ERROR_SUCCESS){
		//		sprintf( (char *)pTbl->pWork->SendSlotName, sData, (65 + pTbl->Parm.ComPort));			
		//	}
		//	else{
		//		strcpy( (char *)pTbl->pWork->SendSlotName, "\\\\.\\mailslot\\Driver2IO");
		//	}
		//	
		//	// ȡ�ý����ʲ�����
		//	dwSize = _MAX_PATH;
		//	sprintf( sBuf, "DriverReceiveSlot");
		//	lRet = ::RegQueryValueEx( hkey, sBuf, NULL, &dwType, (BYTE *)sData, &dwSize );
		//	if(lRet == ERROR_SUCCESS){
		//		sprintf( (char *)pTbl->pWork->ReceiveSlotName, sData, (65 + pTbl->Parm.ComPort));			
		//	}
		//	else{
		//		strcpy( (char *)pTbl->pWork->ReceiveSlotName, "\\\\.\\mailslot\\IO2Driver" );
		//	}
		//	pTbl->pWork->bCommunicationMethod = MAIL_SLOT;
		//}while(0);
		//if(hkey){
		//	::RegCloseKey(hkey);
		//}
	}

#ifdef _TRACE_SERVER_
	IOTrs_Trace("GetComunicationMethod - Exit - return %d", pTbl->pWork->bCommunicationMethod);
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ƿ����µ�������Ҫ���ջ��߷���

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     BOOL  �ɹ�:TRUE  ʧ��:FALSE

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL IdlCheck(IN PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("IdlCheck - Enter");
#endif
	BOOL bRet = TRUE;
	HTDLWORK* pWork = pTbl->pWork;
	// ���ն����д���������ҽ��ջ�����в�Ϊ��
	if((pWork->pRcvQue->nCnt != 0) && (pWork->pBuffQue->nCnt != 0)){
#ifdef _TRACE_SERVER_
	IOTrs_Trace("IdlCheck - Has buffered data");
#endif
		// ������������¼�����
		if(::SetEvent(pTbl->hRecv) == FALSE){
			bRet = FALSE;
		}
	}
	// ����״̬Ϊ���в��ҷ��Ͷ��д���������
	else if (((pWork->DrvStat & IOTRS_ST_SNDEXE) == 0) && (pWork->pSndQue->nCnt != 0)){
#ifdef _TRACE_SERVER_
	IOTrs_Trace("IdlCheck - Has request on queue");
#endif
		// ����������¼�����
		if(::SetEvent(pTbl->hSend) == FALSE){
			bRet = FALSE;
		}
	}

#ifdef _TRACE_SERVER_
	IOTrs_Trace("IdlCheck - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ӷ��Ͷ���ȡ��һ��������

@param      (i)HTDLWORK* pWork		�����ռ�ṹָ��
@param      (o)SENDITEM* pItem		׼������������
@param      (o)IOTRS_RESULT* pRes  ����ִ�н��

@retval     BOOL   �ɹ�:TRUE  ʧ��:FALSE

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL GetSendRequest(IN HTDLWORK* pWork,OUT SENDITEM* pItem,OUT IOTRS_RESULT* pRes)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("GetSendRequest - Enter");
#endif
	BOOL bRet = FALSE;
	DWORD nResult;
	HANDLE hMutex = pWork->hMutex;				// Synchronize Sending/Receiving
	SEND_QUEUE* pQue = pWork->pSndQue;			// Send queue
	*pRes = IOTRS_RES_WINERR;					// Suppose operation fail

	// �����ٽ���
	nResult = ::WaitForSingleObject(hMutex,INFINITE);	
	if (nResult != WAIT_FAILED){
		// ���Ͷ��в�Ϊ��
		if(pQue->nCnt > 0){
			// ȡ��һ������������Ŀ���
			::CopyMemory(pItem,&pQue->Que[pQue->nRP++],sizeof(SENDITEM));
			// ���п�
			if(--pQue->nCnt == 0){
				// ������ڴ�����������״̬
				pWork->DrvStat &= ~IOTRS_ST_SNDQUE;
			}
			// �α굽��ĩβ�򷵻ؿ�ʼλ�ã�ѭ��
			if(pQue->nRP >= pQue->nMax){
				pQue->nRP = 0;
			}
			// �뿪�ٽ���
			if (::ReleaseMutex(hMutex) != 0){
				bRet = TRUE;
				*pRes = IOTRS_RES_OK;
			}
		}
		// ���Ͷ���Ϊ��
		else{
			// ������ڴ�����������״̬
			pWork->DrvStat &= ~IOTRS_ST_SNDQUE;
			// �뿪�ٽ���
			if (::ReleaseMutex(hMutex) != 0){
				*pRes = IOTRS_RES_OK;
			}
		}
	}
#ifdef _TRACE_SERVER_
	IOTrs_Trace("GetSendRequest - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ӽ��ն���ȡ��һ��������

@param      (i)HTDLWORK* pWork		�����ռ�ṹָ��
@param      (o)SENDITEM* pItem		׼�����յ�������
@param      (o)IOTRS_RESULT* pRes  ����ִ�н��

@retval     BOOL   �ɹ�:TRUE  ʧ��:FALSE

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL GetRecvRequest(IN HTDLWORK* pWork,IN RECVITEM* pItem,OUT IOTRS_RESULT* pRes)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("GetRecvRequest - Enter");
#endif
	BOOL bRet = FALSE;
	HANDLE hMutex = pWork->hMutex;			// Synchronize Sending/Receiving
	RECV_QUEUE* pQue = pWork->pRcvQue;		// Received queue
	*pRes = IOTRS_RES_WINERR;				// Suppose operation fail

	// �����ٽ���
	DWORD nResult = ::WaitForSingleObject(hMutex,INFINITE);
	if (nResult != WAIT_FAILED){
		// ���ն��в���
		if(pQue->nCnt > 0){
			// ȡ��һ������������Ŀ���
			::CopyMemory(pItem,&pQue->Que[pQue->nRP++],sizeof(RECVITEM));
			// Ԫ��������һ
			pQue->nCnt--;							
			// �α굽��ĩβ�򷵻ؿ�ʼλ�ã�ѭ��
			if(pQue->nRP >= pQue->nMax){
				pQue->nRP = 0;
			}
			// �뿪�ٽ���
			if(::ReleaseMutex(hMutex) != 0){
				bRet = TRUE;						// Operation success
				*pRes = IOTRS_RES_OK;				// Set return code
			}
		}
		// ���п�
		else{
			// �뿪�ٽ���
			if (::ReleaseMutex(hMutex) != 0){
				*pRes = IOTRS_RES_OK;				
			}
		}
	}
#ifdef _TRACE_SERVER_
	IOTrs_Trace("GetRecvRequest - Exit - return %d", bRet);
#endif
	return bRet;
}


/******************************************************************************/
/**	@brief Send 1 item in the sending queue (if the queue isn't empty)
 *	@param pTbl: Pointer to a structure DEVICETBL.
 *	@return SendEvent return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
BOOL SendEvent(IN PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("SendEvent - Enter");
#endif
	BOOL bRet = FALSE;
	PHTDLWORK pWork = pTbl->pWork;
	IOTRS_RESULT nResult;
	DWORD nWait;
	do{
		// ������״̬
		if ((pWork->DrvStat & IOTRS_ST_SNDEXE) != 0){
			// ����æ�����ڷ���������
			bRet = TRUE;
			break;
		}

		// �ӷ��Ͷ���ȡ��������
		if(GetSendRequest(pWork,&pWork->SndItem,&nResult) == FALSE){
			bRet = TRUE;
			break;
		}

		// ��ʼ�����Ͳ������Ϊʧ��
		*(pWork->SndItem.pResult) = IOTRS_RES_WINERR;

		// ���÷���״̬æ��Ϊ���ڷ�������
		UpdateStatus(pWork, IOTRS_ST_SNDEXE, TRUE);	
		
		//
		//Ask the service to send item
		//
//---------------------------------------------------------------------
		pWork->SndItem.pSending = pWork->SndItem.pCommand;		// ���÷���������ʼָ��
		*pWork->SndItem.pResult =	IOTRS_HTDL_ERR_SENDFAIL;	// ��ʼΪ����ʧ��
		*pWork->SndItem.pOutLength = 0;
		pWork->SndItem.wLastSendingLen = 0;

		//Start sending process
		pWork->ReqSend = true;

		//> p = pWork->SndItem.pCommand;
		//> SendRsp(pWork, p);

		// Wait for Finish event from HTDL
		nWait = ::WaitForSingleObject(pWork->hFinSend, INFINITE); 
		if (nWait == WAIT_FAILED)
		{
			break;
		}
//---------------------------------------------------------------------
		//Change the status of service to free (sending)
		UpdateStatus(pWork, IOTRS_ST_SNDEXE, FALSE);

		bRet = TRUE;
		break;
	} while (0) ;

	// ���������������¼�
	::SetEvent(pWork->SndItem.hEvent);	

#ifdef _TRACE_SERVER_
	IOTrs_Trace("SendEvent - Exit - return %d", bRet);
#endif
	return bRet;
}

/******************************************************************************/
/**	@brief Process the receive event. Copy data from temporary buffer
 *	       to receiving item if any.
 *	@param pTbl: Pointer to a structure DEVICETBL.
 *	@return RecvEvent return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
BOOL RecvEvent(IN PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("RecvEvent - Enter");
#endif
	BOOL bRet = FALSE;
	PHTDLWORK pWork = pTbl->pWork;
	IOTRS_RESULT nResult;
	DWORD nWait;
	do{
		// 
		bRet = GetRecvRequest(pWork,&pWork->RcvItem,&nResult);
		if (bRet == FALSE){
			break;
		}
		if (pWork->pBuffQue->nCnt == 0){
			bRet = TRUE;
			*(pWork->RcvItem.pResult) = IOTRS_RECV_EMPTY;
			::SetEvent(pWork->RcvItem.hEvent);
			break;
		}
		//
		nWait = ::WaitForSingleObject(pWork->hMutex, INFINITE);
		if (nWait == WAIT_FAILED){
			break;
		}

		//Copy data from temporary buffer to RcvItem
		BUFFITEM* pBufferItem = &pWork->pBuffQue->Que[pWork->pBuffQue->nRP];
		::CopyMemory(pWork->RcvItem.pBuffer, pBufferItem->pData,pBufferItem->nWP/* nWritten*/);

        // дlog�ļ�����д���������д��־��
		if (pWork->RcvItem.pBuffer[14] != 0xD0 && pWork->RcvItem.pBuffer[14] != 0x65) {
            char log[MAX_BUFFER_SIZE*2] = {0};
            log[0] = '<';
			log[1] = ' ';
            // ��־���ֻ��¼�����
            int logLength = 0;
            if (pWork->RcvItem.pBuffer[2] == 0x61) {
                logLength = 14;
            }
            else {
                logLength = pBufferItem->nWP;
            }
			for(int i=0;i < logLength; i++) {
				sprintf(&log[2*i+2],"%02x",pWork->RcvItem.pBuffer[i]);
			}
			IOTrs_Trace(log);
		}

		*(pWork->RcvItem.pRecvLength) = pBufferItem->nWP;

		pWork->pBuffQue->nRP++;
		if (pWork->pBuffQue->nRP >= pWork->pBuffQue->nMax){
			pWork->pBuffQue->nRP = 0;
		}
		pWork->pBuffQue->nCnt--;

		if (::ReleaseMutex(pWork->hMutex) == FALSE){
			break;
		}

		if (pWork->pBuffQue->nCnt == 0){
			UpdateStatus(pWork, IOTRS_ST_RCVQUE, FALSE);
		}

		//
		*(pWork->RcvItem.pResult) = IOTRS_RES_OK;
		::SetEvent(pWork->RcvItem.hEvent);
		bRet = TRUE;
	} while (0) ;

#ifdef _TRACE_SERVER_
	IOTrs_Trace("RecvEvent - Exit - return %d", bRet);
#endif
	return bRet;
}

/******************************************************************************/
/**	@brief Put 1 block of data into the temporary buffer
  *	@param pTbl: Pointer to a structure DEVICETBL.
 *	@return RecvData return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
BOOL RecvData(IN HTDLWORK *pWork,IN BYTE *data,IN int cnt)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("RecvData - Enter");
#endif
	BOOL bRet = FALSE;
	BOOL bAlert = FALSE;
	DWORD nWait;
	do {
		nWait = ::WaitForSingleObject(pWork->hMutex,INFINITE);		//Enter critical section
		if (nWait == WAIT_FAILED){
			break;
		}
		//Need 1 new block for storing
		//Checking overflow
		if (pWork->pBuffQue->nCnt < pWork->pBuffQue->nMax){
			BUFFITEM* pItem = &pWork->pBuffQue->Que[pWork->pBuffQue->nWP];
			::CopyMemory(pItem->pData, data, cnt);
			pItem->nWP = cnt;			
			pWork->pBuffQue->nWP++;
			if(pWork->pBuffQue->nWP >= pWork->pBuffQue->nMax){
				pWork->pBuffQue->nWP = 0;
			}
			pWork->pBuffQue->nCnt++;
			bAlert = TRUE;

		}
		else{
			UpdateStatus(pWork, IOTRS_ST_RCVOVR, TRUE);	//
		}
		//Leave critical section
		if(::ReleaseMutex(pWork->hMutex) == FALSE){
			break;
		}

		if(TRUE == bAlert){
			//Notify there is new item received
			UpdateStatus(pWork, IOTRS_ST_RCVQUE, TRUE);	
		}
		bRet = TRUE;
	}
	while (0);
	
#ifdef _TRACE_SERVER_
	IOTrs_Trace("RecvData - Exit - return %d", bRet);
#endif
	return bRet;
}

/******************************************************************************/
/**	@brief Open the protocol service, it manages the sending and receiving package
 *	@param pTbl: Pointer to a structure DEVICETBL.
 *	@return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Э�鴦���߳�

@param      (i)PDEVICETBL pTbl	�豸���Ӽ�ͨ�ſ��ƽṹ

@retval     DWORD   0:�ɹ�   ��0:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL OpenHTDL(PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("OpenHTDL - Enter");
#endif
	DWORD dchk;
	DWORD dwTime;
	HTDLWORK *pWork = pTbl->pWork;
	int i;								
	BOOL bRet = FALSE;
	do {
		// �������������¼�
		pWork->hFinSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hFinSend == NULL){
			break;						
		}
		pWork->hFrameSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hFrameSend == NULL){
			break;						
		}
		// ����Э���߳�ͬ���¼�
		pWork->hSync = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hSync == NULL){
			break;					
		}
		// ����Э���̹߳ر��¼�
		pWork->hClose = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hClose == NULL){
			break;						
		}

		// ����Э���߳�
		pWork->hThread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HtdlThread,(LPVOID)pWork,0, &dchk);
		if(pWork->hThread == NULL){
			break;
		}
		// �ȴ�Э���߳���������
		dchk = ::WaitForSingleObject(pWork->hSync, INFINITE);
		if(dchk == WAIT_TIMEOUT){
			// �ս��߳�
			::TerminateThread(pWork->hThread, NULL);
		}

		// Э���߳��쳣����
		if(pWork->nError != IOTRS_RES_OK) {
			pTbl->nResult = pWork->nError;
			for(i = 0; i < 100; i++){
				::GetExitCodeThread(pWork->hThread, &dchk);	
				if (dchk != STILL_ACTIVE){
					break;
				}
				::Sleep(100);
			}
			break;
		}
		bRet = TRUE;
	} while(0);

	// �ͷ���Դ
	if(bRet == FALSE){
		if(pWork->hFinSend != NULL){
			::CloseHandle(pWork->hFinSend);
		}
		if(pWork->hFrameSend != NULL){
			::CloseHandle(pWork->hFrameSend);
		}
		if(pWork->hSync != NULL){
			::CloseHandle(pWork->hSync);
		}
		if(pWork->hClose != NULL){
			::CloseHandle(pWork->hClose);
		}
	}

#ifdef _TRACE_SERVER_
	IOTrs_Trace("OpenHTDL - Exit - return %d", bRet);
#endif
	return bRet;
}

/******************************************************************************/
/**	@brief Close the protocol service.
 *	@param pTbl: Pointer to a structure DEVICETBL.
 *	@return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
BOOL CloseHTDL(PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("CloseHTDL - Enter");
#endif
	BOOL bRet;
	DWORD dchk;
	int i;
	HTDLWORK *pWork;
	bRet = TRUE;
	pWork = pTbl->pWork; //Get workspace

	::SetEvent(pWork->hClose);	//Stop service
	dchk = ::WaitForSingleObject(pWork->hSync, INFINITE);
	if (dchk == WAIT_FAILED){
		bRet = FALSE;
	}
	//Close all notifying events
	::CloseHandle(pWork->hFinSend);
	::CloseHandle(pWork->hFrameSend);
	::CloseHandle(pWork->hSync);
	::CloseHandle(pWork->hClose);

	//Wait until the service stop
	for(i = 0; i < 100; i++){
		::GetExitCodeThread(pWork->hThread, &dchk);
		if (dchk != STILL_ACTIVE){
			break;
		}
		::Sleep(100);
	}

#ifdef _TRACE_SERVER_
	IOTrs_Trace("CloseHTDL - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ�������߳�

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     TRUE: �ɹ�   FALSE: ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL InitServThread(IN PDEVICETBL pTbl)
{
	BOOL bRet = FALSE;
	int iLen;
	HTDLWORK *pWork;

#ifdef _TRACE_SERVER_
	IOTrs_Trace("InitServThread - Enter");
#endif
	_ASSERTE(NULL != pTbl);
	do{
		// ���乤���ṹ�ռ�
		pTbl->pWork = new HTDLWORK;
		ZeroMemory(pTbl->pWork, sizeof(HTDLWORK));
		if (pTbl->pWork == NULL){
			pTbl->nResult = IOTRS_HTDL_ERR_WORKNG;
			break;
		}
		pWork = pTbl->pWork;

		// Ϊ���Ͷ��з����ڴ�ռ䣬��ʼ�����Ͷ�����Ϣ
		iLen = sizeof(SEND_QUEUE) +	sizeof(SENDITEM) * (pTbl->Parm.MaxSendCnt - 1);
		pWork->pSndQue = (SEND_QUEUE*)new BYTE[iLen];
		if(pWork->pSndQue == NULL){
			pTbl->nResult = IOTRS_HTDL_ERR_SQUENG;
			break;
		}
		pWork->pSndQue->nMax = pTbl->Parm.MaxSendCnt;  
		pWork->pSndQue->nCnt = 0;
		pWork->pSndQue->nWP = 0;
		pWork->pSndQue->nRP = 0;

		// Ϊ���ն��з����ڴ�ռ䣬��ʼ�����ն�����Ϣ
		iLen = sizeof(RECV_QUEUE) +	sizeof(RECVITEM) * (pTbl->Parm.MaxRecvCnt - 1);
		pWork->pRcvQue = (RECV_QUEUE*)new BYTE[iLen];
		if (pWork->pRcvQue == NULL){
			pTbl->nResult = IOTRS_HTDL_ERR_SQUENG;
			break;
		}
		pWork->pRcvQue->nMax = pTbl->Parm.MaxRecvCnt;
		pWork->pRcvQue->nCnt = 0;
		pWork->pRcvQue->nWP = 0;
		pWork->pRcvQue->nRP = 0;

		// Ϊ�������ݻ�����з����ڴ�ռ䣬��ʼ�����ݻ��������Ϣ
		iLen = sizeof(BUFF_QUEUE) +	sizeof(BUFFITEM) * (pTbl->Parm.MaxRecvCnt - 1);
		pWork->pBuffQue = (BUFF_QUEUE*)new BYTE[iLen];
		if (pWork->pBuffQue == NULL){
			pTbl->nResult = IOTRS_HTDL_ERR_SQUENG;
			break;
		}
		BUFF_QUEUE* pBuffQueue = pWork->pBuffQue;
		pBuffQueue->nMaxBuffSize = pTbl->Parm.MaxRecvBuffSize;
		pBuffQueue->nMax = pTbl->Parm.MaxRecvCnt;
		pBuffQueue->nCnt = 0;
		pBuffQueue->nRP = 0;
		pBuffQueue->nWP = 0;

		// Ϊ�������ݻ���������������ڴ�ռ�
		BUFFITEM* pBuffItem;
		for (int i=0; i<pBuffQueue->nMax; i++){
			pBuffItem = &pBuffQueue->Que[i];
			pBuffItem->nWP = 0;
			pBuffItem->pData = new BYTE[pBuffQueue->nMaxBuffSize];
		}

		// ���������ռ����ͬ���ź���
		pWork->hMutex = ::CreateMutex(NULL, FALSE, NULL);
		if (pWork->hMutex == NULL){
			pTbl->nResult = IOTRS_RES_WINERR;
			break;
		}

		// ��ʼ�������ռ����
		pWork->DrvStat = 0;
		//pWork->SeqSend = 1;
		//pWork->LastSeqRecv = 0;
		//pWork->bNewENQ = FALSE;
		bRet = TRUE;
	} while (0);

#ifdef _TRACE_SERVER_
	IOTrs_Trace("InitServThread - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������̲߳��ͷ���Դ

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void EndServThread(IN PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("EndServThread - Enter");
#endif
	if(pTbl->pWork->hMutex != NULL){
		CloseHandle(pTbl->pWork->hMutex);
	}
	if(pTbl->pWork->pBuffQue != NULL){
		for (int i=0; i<pTbl->pWork->pBuffQue->nMax; i++)	{
			delete []pTbl->pWork->pBuffQue->Que[i].pData;
		}
		delete [](LPBYTE)pTbl->pWork->pBuffQue;
	}
	if(pTbl->pWork->pRcvQue != NULL){
		delete [](LPBYTE)pTbl->pWork->pRcvQue;
	}
	if(pTbl->pWork->pSndQue != NULL){
		delete [](LPBYTE)pTbl->pWork->pSndQue;
	}
	if (pTbl->pWork != NULL){
		delete [](LPBYTE)pTbl->pWork;
	}

#ifdef _TRACE_SERVER_
	IOTrs_Trace("EndServThread - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����̺߳���(���ͣ����գ��ر�)

@param      (i)PDEVICETBL pTbl �豸���Ӽ�ͨ�ſ��ƽṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI ServThread(IN LPVOID lpParam)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("ServThread - Enter");
#endif
	PDEVICETBL pTbl;
	HANDLE	hEvents[3];
	DWORD nEvent;
	bool bLoop;
	pTbl = (PDEVICETBL)lpParam;	
	// ��ʼ�������߳�
	if (InitServThread(pTbl)){
		// ��ȡͨ�ŷ�ʽ
		GetComunicationMethod(pTbl);	
		// ��ʼ������
		if(InitComPort(pTbl)){
			if (OpenHTDL(pTbl)) {
				pTbl->nResult = IOTRS_RES_OK;
				SetEvent(pTbl->hSync);				//
				hEvents[0] = pTbl->hClose;
				hEvents[1] = pTbl->hSend;
				hEvents[2] = pTbl->hRecv;
				bLoop = true;
				while (bLoop) {							//
					nEvent = ::WaitForMultipleObjects(3, hEvents, FALSE, 10);
					switch (nEvent){
						case WAIT_OBJECT_0:		// Stop thread 
							bLoop = false;		//
							break;
						case WAIT_OBJECT_0+1:	// Sending request
							SendEvent(pTbl);	//
							break;
						case WAIT_OBJECT_0+2:	// Receiving request
							RecvEvent(pTbl);	//
							break;
						default:
							break;
					}
					IdlCheck(pTbl);			//
				}
				CloseHTDL(pTbl);
			}
			EndComPort(pTbl);				//
		}
	}
	// ���������߳�
	EndServThread(pTbl);

	::SetEvent(pTbl->hSync);

#ifdef _TRACE_SERVER_
	IOTrs_Trace("ServThread - Exit - return 0");
#endif
	return 0;
}