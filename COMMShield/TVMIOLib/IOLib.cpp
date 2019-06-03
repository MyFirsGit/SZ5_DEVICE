#include <process.h>
#include "stdio.h"
#include "common.h"
#include <stdlib.h>
#include <memory.h>
#include "IOLib.h"
#include "NamedPipeMgr.h"
#include <shlwapi.h>
#include <TCHAR.h>

//----------------- �������ݶ�------------------
#pragma data_seg("Shared")

HANDLE g_hinstDll = 0;				  // DLL���		
long g_LoadCount = 0;                 // ��������
DEVICETBL g_Devs[MAX_DEVICES] = {0};  // �豸���Ӽ�ͨ�ſ��ƽṹ����

#pragma data_seg()
#pragma comment(linker, "/section:Shared,rws")
//-----------------------------------------------

// ���л����ӳز������ź���
HANDLE g_hMutex = 0;

// Prototype
void IOTrs_Trace(const char* lpszFormat, ...);
IOTRS_RESULT OpenCheck(IN int Handle);
IOTRS_RESULT PostSendRequest(IN IOTRS_HANDLE Handle, IN SENDITEM* pItem);
IOTRS_RESULT WaitSendComplete(IN HANDLE hEvent);
IOTRS_RESULT PostRecvRequest(IN IOTRS_HANDLE Handle, IN RECVITEM* pItem);
IOTRS_RESULT WaitRecvComplete(IN HANDLE hEvent);
INT  GetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);
//const char* TH_TRACE_PATH_AND_NAME = "ThTrace_*.log";

CModuleLog g_Debug;
char* m_szText = NULL;					// ��־�ַ���
char* m_szTraceBuffer = NULL;

// ģ������
NamedPipeMgr namepipeMgr;				// �ܵ�ͨ�ſ���
int iCommunicationType;                 // ͨ�ŷ�ʽ
// TH�����ļ�
#define	DEVICE_INI_FILE_NAME	_T("DEVICE.INI")		// �����ļ���
#define TH_CONFIG_SECTION	_T("TH")			// SECTIONֵ
#define TH_CONMU_METHOD		_T("CMU_METHOD") 	// ͨ�ŷ�ʽKEYֵ

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ����־��Դ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void SetupTracer()
{
	m_szText = new char[MAX_TRACE_LEN];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ͷ���־��Դ

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void ShutdownTracer()
{
	g_Debug.CloseLog();	
    if(NULL != m_szText){
        delete []m_szText;
        m_szText = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡTH��־

@param      (i)const char* lpszFormat ��־��ʽ

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void IOTrs_Trace(const char* lpszFormat, ...)
{
	_ASSERTE(lpszFormat);
    g_Debug.print(lpszFormat);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��TH��־�ļ�

@param      (i)const char* tracePath  ��־����Ŀ¼�ַ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void IOTrs_Initialize(const char* tracePath)
{
    g_Debug.Initialize(tracePath);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���TH��־�ļ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void IOTrs_Clear()
{
    g_Debug.ClearLog();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      DLL��ں���

@param      (i)HANDLE hModule				DLL���
@param      (i)DWORD ul_reason_for_call		����ԭ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	BOOL bRet = TRUE;
	int i = 0;
	switch (ul_reason_for_call){
	// ����DLL
	case DLL_PROCESS_ATTACH:
		// �״γ�ʼ��ʵ��,��ʼ�����ӳ�
		if(InterlockedIncrement(&g_LoadCount) == 1){			
			g_hinstDll = hModule;			
			ZeroMemory(g_Devs, sizeof(g_Devs));
			for (i = 0; i < MAX_DEVICES; i++){
				g_Devs[i].bUsed = false;
			}
		}
		// �����ٽ���������ͬ�������ӳصĲ���
		g_hMutex = CreateMutexA(NULL, FALSE, "basao_mimtrs");
		if (g_hMutex == NULL){
			bRet = FALSE;
			break;
		}

		SetupTracer();

		// ȡ��ͨ������
		TCHAR szAppPath[_MAX_PATH];
		GetAppPath(szAppPath,_MAX_PATH);
		_tcscat(szAppPath,DEVICE_INI_FILE_NAME);
		iCommunicationType = GetPrivateProfileInt(TH_CONFIG_SECTION,TH_CONMU_METHOD, COM_PORT, szAppPath);

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	// ж��DLL
	case DLL_PROCESS_DETACH:
		// ���ʵ���ر�
		if (InterlockedDecrement(&g_LoadCount) == 0){
			// ������ӳ�
			for (i = 0; i < MAX_DEVICES; i++){
				if(TRUE == g_Devs[i].bUsed){
					IOTrs_Close((i+1));
				}
			}
		}
		// �ͷ����ӳز����ٽ�����Դ
		if (g_hMutex != NULL){
			CloseHandle(g_hMutex);	
			g_hMutex = NULL;
		}

		// �ر���־
		ShutdownTracer();
		break;
	}
    return bRet;
}

extern DWORD WINAPI ServThread(IN LPVOID lpParam);

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��THͨ������

@param      (o)IOTRS_HANDLE* pHandle   �Ѵ򿪵�����ʵ�����
@param      (i)void* pParam             ָ��ṹ��(IOTRS_CONNECT_PARAM)��ָ��

@retval     IOTRS_RESULT   IOTRS_RES_OK:�ɹ�   ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP IOTRS_RESULT IOTrs_Open(OUT IOTRS_HANDLE* pHandle,IN  void* pParam)
{
	if(iCommunicationType == COM_PORT){
		IOTRS_RESULT nResult = IOTRS_RES_WINERR;
		DWORD dchk;
		DWORD dwTime;
		int i;

		// ������Ƿ�ΪNULL
		_ASSERTE(NULL != pHandle);

		// �������ӳز����ٽ���
		dchk = ::WaitForSingleObject(g_hMutex, INFINITE);	
		if (dchk == WAIT_FAILED){
			return nResult;
		}
		// �������ӳ��еĿ��ж��� 
		for (i = 0; i < MAX_DEVICES; i++){	
			// �ҵ��������Ӷ���
			if (g_Devs[i].bUsed == false){			
				ZeroMemory(&g_Devs[i], sizeof(DEVICETBL));
				g_Devs[i].bUsed = true;
				g_Devs[i].bClose = false;
				nResult = IOTRS_RES_OK;
				break;
			}
		}
		// ���ӳ�����
		if (i >= MAX_DEVICES){
			nResult = IOTRS_HTDL_ERR_FULL;
		}
		// �뿪���ӳز����ٽ���
		if(::ReleaseMutex(g_hMutex) == FALSE){
			g_Devs[i].bUsed = false;
			nResult = IOTRS_RES_WINERR;
			return nResult;
		}

		// ����ʧ���򷵻�
		if (nResult != IOTRS_RES_OK){
			return nResult;
		}

		// ��������
		nResult = IOTRS_RES_WINERR;
		do {
			// �������Ӳ�����Ϣ
			CopyMemory(&g_Devs[i].Parm, pParam, sizeof(IOTRS_CONNECT_PARAM));

			// ���������߳�����ͬ���¼�
			g_Devs[i].hSync = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hSync == NULL){
				break;	
			}

			// ���������¼�
			g_Devs[i].hSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hSend == NULL){
				break;
			}

			// ���������¼�
			g_Devs[i].hRecv = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hRecv == NULL){
				break;	
			}

			// �����ر������¼�
			g_Devs[i].hClose = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hClose == NULL){
				break;
			}

			// ���������߳�
			g_Devs[i].hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServThread, (LPVOID)&g_Devs[i], CREATE_SUSPENDED, &dchk);
			if (g_Devs[i].hThread == NULL){
				break;	
			}
			// ��������߳�
			::ResumeThread(g_Devs[i].hThread);

			// �ȴ������߳�ִ�н���
			dchk = ::WaitForSingleObject(g_Devs[i].hSync, INFINITE);
			// ִ�г���
			if (dchk == WAIT_TIMEOUT) {
				::TerminateThread(g_Devs[i].hThread, NULL);
				if (g_Devs[i].pWork != NULL){
					free(g_Devs[i].pWork);
				}
				nResult = IOTRS_HTDL_ERR_OPNTO;
				break;
			}
			// ϵͳ����
			if (dchk == WAIT_FAILED){
				break;	
			}

			// �����߳�ִ��ʧ��
			nResult =  g_Devs[i].nResult;
			if (nResult != IOTRS_RES_OK){
				// �ȴ������߳�ִ�н�����������
				for (int j = 0; j < 100; j++){
					::GetExitCodeThread(g_Devs[i].hThread, &dchk);
					if (dchk != STILL_ACTIVE){
						break;
					}
					Sleep(100);
				}
			}
		} while (0);

		// ������ʧ�����ͷŸ�ͬ������
		if (nResult != IOTRS_RES_OK){
			if (g_Devs[i].hSync != NULL){
				CloseHandle(g_Devs[i].hSync);
			}
			if (g_Devs[i].hSend != NULL){
				CloseHandle(g_Devs[i].hSend);
			}
			if (g_Devs[i].hRecv != NULL){
				CloseHandle(g_Devs[i].hRecv);
			}
			if (g_Devs[i].hClose != NULL){
				CloseHandle(g_Devs[i].hClose);
			}
			g_Devs[i].bUsed = false;
			*pHandle = 0;
		}
		// �򿪳ɹ������þ��ID
		else {		
			*pHandle = i + 1; 
		}
		return nResult;
	}
	// �ܵ�ͨ��
	else{
		return namepipeMgr.create();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ر�THͨ������

@param      (i)IOTRS_HANDLE Handle   Ҫ�ر�����ʵ�����

@retval     IOTRS_RESULT   IOTRS_RES_OK:�ɹ�   ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP IOTRS_RESULT IOTrs_Close(IN IOTRS_HANDLE Handle)
{
	if(iCommunicationType == COM_PORT){
		IOTRS_RESULT nResult = IOTRS_RES_OK;
		DWORD dchk;
		int i;
		nResult = OpenCheck(Handle);
		// ������Ч
		if (nResult == IOTRS_RES_OK){
			// �����ر������¼�
			SetEvent(g_Devs[Handle-1].hClose);	
			// �ȴ������̹߳رմ������
			dchk = WaitForSingleObject(g_Devs[Handle-1].hSync, INFINITE);
			if (dchk == WAIT_FAILED){
				nResult = IOTRS_RES_WINERR;
			}
			else{
				nResult = g_Devs[Handle-1].nResult;	//last result
			}
			g_Devs[Handle-1].bUsed = false;
			
			// �ȴ������߳������˳�
			for (i = 0; i < 100; i++){
				::GetExitCodeThread(g_Devs[Handle-1].hThread, &dchk);
				if (dchk != STILL_ACTIVE){
					break;
				}
				Sleep(100);
			}
		}
		return nResult;
	}
	// �ܵ�ͨ��
	else{
		if(namepipeMgr.close()){
			return IOTRS_RES_OK;
		}
		else{
			return IOTRS_RES_WINERR;
		}		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����TH����

@param      (i)IOTRS_HANDLE Handle		����ʵ�����
@param      (i)DWORD nSendLength		���ݳ���
@param      (i)void* pBuffer			��������ָ��
@param      (O)DWORD* pOutLength		�ѷ��͵����ݳ���

@retval     IOTRS_RESULT   IOTRS_RES_OK:�ɹ�   ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP IOTRS_RESULT IOTrs_Send(IN IOTRS_HANDLE Handle,IN DWORD nSendLength,IN void* pBuffer,OUT DWORD* pOutLength)
{
	//IOTrs_Trace("IOTrs_Send - enter");
	IOTRS_RESULT nResult = IOTRS_RES_OK;
	if(iCommunicationType == COM_PORT){		
		IOTRS_RESULT nRes;
		HANDLE hEvent;		// ��������¼�
		SENDITEM wItem;		// ����������

		_ASSERTE(NULL != pBuffer);

		nResult = OpenCheck(Handle);				
		// ��������
		if (nResult == IOTRS_RES_OK){
			// ������������¼�
			hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			// �����ɹ�
			if (hEvent != NULL){
				// ׼����������������
				wItem.hEvent = hEvent;					//Notify event
				wItem.pResult = &nResult;				//Result of task
				wItem.nSendLength = nSendLength;		//Number of byte to be sent
				wItem.pCommand = (BYTE*)pBuffer;		//Data to be sent
				wItem.pOutLength = pOutLength;			//Number of byte were sent
				
				// ��������������뷢�Ͷ���
				nRes = PostSendRequest(Handle, &wItem);	
				// �ɹ�
				if(nRes == IOTRS_RES_OK){
					// �ȴ�������� 
					nRes = WaitSendComplete(hEvent);	
				}
				// �ͷŷ�������¼�
				if (::CloseHandle(hEvent) == FALSE)	{
					nRes = IOTRS_RES_WINERR;
				}
				if(nRes != IOTRS_RES_OK){
					nResult = nRes;
				}
			}
			// ����ʧ��
			else{
				nResult = IOTRS_RES_WINERR;
			}
		}
	}
	// �ܵ�ͨ��
	else{
		*pOutLength = namepipeMgr.write((char*)pBuffer,nSendLength);
		if(*pOutLength != 0){
			return IOTRS_RES_OK;
		}
		else{
			return IOTRS_RES_WINERR;
		}
	}
	//IOTrs_Trace("IOTrs_Send - exit - return = %08X", nResult);
	return nResult;
}

int frHEX(const char* szData, BYTE* pBuff, int iBufSize)
{
	int n, c, ret;
	char ahex[3];
	BYTE* p;
	char *szText;
	char ch;

	ret = 0;
	memset(pBuff, 0, iBufSize);
	n = strlen(szData);
	p = pBuff;
	c = 0;
	szText = strdup(szData);
	szText = strupr(szText);
	
	for (int i=0; i<n; i++){
		ch = szText[i];
		if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') ){
			ahex[c++] = ch;
			if (c == 2){
				ahex[2] = 0;
				c = 0;
				*p++ = (BYTE)strtol(ahex,NULL,16);
				ret++;
				if (ret == iBufSize){
					break;
				}
			}
		}
	}
	free(szText);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����TH��������

@param      (i)IOTRS_HANDLE Handle		����ʵ�����
@param      (i)DWORD nSendLength		�������ݳ���
@param      (i)void* pBuffer			������������ָ��
@param      (O)DWORD* pOutLength		�ѽ��յ����ݳ���

@retval     IOTRS_RESULT   IOTRS_RES_OK:�ɹ�   ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP IOTRS_RESULT IOTrs_Recv(IN IOTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength)
{
	//IOTrs_Trace("IOTrs_Recv - enter");
	IOTRS_RESULT nResult = IOTRS_RES_OK;
	if(iCommunicationType == COM_PORT){		
		IOTRS_RESULT nRes;
		HANDLE hEvent;
		RECVITEM wItem;
		*pRecvLength = 0;
		
		nResult = OpenCheck(Handle);
		// ������Ч
		if (nResult == IOTRS_RES_OK){
			// ������������¼�
			hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (hEvent != NULL)	{
				// ��ʼ������������
				wItem.hEvent = hEvent;
				wItem.pResult = &nResult;
				wItem.nBuffSize = nBuffLength;
				wItem.pBuffer = (BYTE*)pBuffer;
				wItem.pRecvLength = pRecvLength;
				// Post receiving request
				nRes = PostRecvRequest(Handle, &wItem);	
				if (nRes == IOTRS_RES_OK)				//Request posted
				{
					nRes= WaitRecvComplete(hEvent);		//Wait until received
				}
				if (::CloseHandle(hEvent) == FALSE)
				{
					nRes= IOTRS_RES_WINERR;
				}
				if (nRes != IOTRS_RES_OK) 
				{
					nResult = nRes;
				}
			}
			//Unable to create event
			else{
				nResult = IOTRS_RES_WINERR;
			}
		}
	}
	// �ܵ�ͨ��
	else{
		*pRecvLength = namepipeMgr.read((char*)pBuffer,nBuffLength);
		if(*pRecvLength != 0){
			return IOTRS_RES_OK;
		}
		else{
			return IOTRS_RES_WINERR;
		}
	}

	//IOTrs_Trace("IOTrs_Recv - exit - return = %08X", nResult);
	return nResult;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����THͨ�Ŵ���

@param      (i)IOTRS_HANDLE Handle		����ʵ�����
@param      (i)BOOL bSendClear		    �Ƿ���ֹ����δ���ʹ���
@param      (i)BOOL bRecvClear			�Ƿ���ֹ����δ���մ���

@retval     IOTRS_RESULT   IOTRS_RES_OK:�ɹ�   ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP IOTRS_RESULT IOTrs_Reset(IN IOTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear)
{
	if(iCommunicationType == COM_PORT){
		HANDLE hMutex;
		HTDLWORK* pWork;
		DWORD nChk, dwErr;
		COMSTAT commstat;

		IOTRS_RESULT nResult = OpenCheck(Handle);
		do{
			// ������Ч
			if (IOTRS_RES_OK != nResult){
				break;
			}
			hMutex = g_Devs[Handle - 1].pWork->hMutex;
			pWork = g_Devs[Handle - 1].pWork;
			nChk = ::WaitForSingleObject(hMutex, INFINITE);
			if (WAIT_FAILED == nChk){
				nResult = IOTRS_RES_WINERR;
				break;
			}
			//pWork->LastSeqRecv = 0;
			//pWork->SeqRecv = 0;
			//pWork->SeqSend = 1;
			pWork->DrvStat = 0;
			if (TRUE == bSendClear){
				__try{
					pWork->pSndQue->nCnt = 0;
					pWork->ReqSend = false;
					if (INVALID_HANDLE_VALUE != pWork->SndItem.hEvent){
						pWork->Phase = PHASE_S1;
						*(pWork->SndItem.pResult) = IOTRS_RES_RESET;
						::SetEvent( pWork->SndItem.hEvent );
						::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
						::ClearCommError(pWork->hComm, &dwErr, &commstat);
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER){
					nResult = IOTRS_RES_WINERR;
					break;
				}
			}
			if (TRUE == bRecvClear){
				__try{
					pWork->pRcvQue->nCnt = 0;
					if (INVALID_HANDLE_VALUE != pWork->RcvItem.hEvent){
						pWork->Phase = PHASE_S1;
						*(pWork->RcvItem.pResult) = IOTRS_RES_RESET;
						::SetEvent( pWork->RcvItem.hEvent );
						pWork->pBuffQue->nCnt = 0;
						::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
						::ClearCommError(pWork->hComm, &dwErr, &commstat);
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER){
					nResult = IOTRS_RES_WINERR;
					break;
				}
			}
			if (FALSE == ::ReleaseMutex(hMutex)){
				nResult = IOTRS_RES_WINERR;
				break;
			}
		} while(0);

		return nResult;
	}
	else{
		return IOTRS_RES_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰͨ��״̬

@param      (i)IOTRS_HANDLE Handle		����ʵ�����
@param      (o)DWORD* pStatus		    ��ǰ״̬

@retval     IOTRS_RESULT   IOTRS_RES_OK:�ɹ�   ����:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP IOTRS_RESULT IOTrs_GetStat(IN IOTRS_HANDLE Handle,OUT DWORD* pStatus)
{
	if(iCommunicationType == COM_PORT){
		DWORD nWait;
		DWORD Stat;	

		_ASSERTE(NULL != pStatus);

		*pStatus = 0;
		IOTRS_RESULT nResult = OpenCheck(Handle);
		if(nResult == IOTRS_RES_OK){
			nWait = ::WaitForSingleObject(g_Devs[Handle-1].pWork->hMutex, INFINITE);
			if(nWait != WAIT_FAILED){
				::GetCommModemStatus(g_Devs[Handle-1].pWork->hComm, &Stat);
				if((Stat & MS_CTS_ON) != 0){
					g_Devs[Handle-1].pWork->DrvStat |= IOTRS_ST_CTS;
				}
				else{
					g_Devs[Handle-1].pWork->DrvStat &= ~IOTRS_ST_CTS;
				}
				if((Stat & MS_DSR_ON) != 0) {
					g_Devs[Handle-1].pWork->DrvStat |= IOTRS_ST_DSR;
				}
				else{
					g_Devs[Handle-1].pWork->DrvStat &= ~IOTRS_ST_DSR;
				}
				::ReleaseMutex(g_Devs[Handle-1].pWork->hMutex);	
				*pStatus = g_Devs[Handle-1].pWork->DrvStat;
			}		
		}
		return nResult;
	}
	else{
		// �ܵ�ͨ��	
		Sleep(300);
		*pStatus = namepipeMgr.GetStat();
		return IOTRS_RES_OK;
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ӿ���Ƿ���Ч

@param      (i)int Handle ���Ӿ��

@retval     IOTRS_RESULT  IOTRS_ERR_HANDLE:��Ч   IOTRS_RES_OK:��Ч

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
IOTRS_RESULT OpenCheck(IN int Handle)
{
	//	IOTrs_Trace("OpenCheck - enter");
	IOTRS_RESULT nRes;
	if(iCommunicationType == COM_PORT){		
		if ((Handle < 1) || (Handle > MAX_DEVICES) || (g_Devs[Handle-1].bUsed == false)){
			nRes = IOTRS_ERR_HANDLE;
		}
		else{
			nRes = IOTRS_RES_OK;
		}
	}
	else{
		if(namepipeMgr.pipe_ != NULL){
			nRes = IOTRS_RES_OK;
		}
		else{
			nRes = IOTRS_ERR_HANDLE;
		}
	}
	//	IOTrs_Trace("OpenCheck - exit - Res = %08X ", nRes);
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������������뷢�Ͷ���

@param      (i)int Handle			���Ӿ��
@param      (i)SENDITEM* pItem		Ҫ���͵�������

@retval     IOTRS_RESULT  IOTRS_HTDL_ERR_SQUE:���Ͷ�����   IOTRS_RES_OK:�ɹ�

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
IOTRS_RESULT PostSendRequest(IN IOTRS_HANDLE Handle,IN SENDITEM* pItem)
{
	IOTRS_RESULT nRes = IOTRS_RES_WINERR;					// Suppose the task is OK
	HANDLE hSend = g_Devs[Handle - 1].hSend;				// Handle of LetSend event (Service)
	HANDLE hMutex = g_Devs[Handle - 1].pWork->hMutex;		// Handle of the synchronize mutex
	SEND_QUEUE* pQue = g_Devs[Handle - 1].pWork->pSndQue;	// The queue stores sending items

	// �����ٽ���
	DWORD nResult = ::WaitForSingleObject(hMutex, INFINITE);	
	if (nResult != WAIT_FAILED){
		// ���Ͷ���δ��
		if(pQue->nCnt < pQue->nMax){
			// ��������������뷢�Ͷ���
			::CopyMemory(&pQue->Que[pQue->nWP++],pItem,sizeof(SENDITEM));
			// ���Ͷ���Ԫ�ؼ���+1
			pQue->nCnt++;							
			// д���α�ѭ��
			if(pQue->nWP >= pQue->nMax){
				pQue->nWP = 0;
			}
			// ���÷���״̬Ϊ���д��ڴ�����������
			g_Devs[Handle - 1].pWork->DrvStat |= IOTRS_ST_SNDQUE;
			// �뿪�����ռ����ͬ���ٽ���
			if(::ReleaseMutex(hMutex) != FALSE){
				// ������¼�
				if(::SetEvent(hSend) != FALSE){
					nRes = IOTRS_RES_OK;
				}
			}
		}
		// ���Ͷ�����
		else{
			// �뿪�����ռ����ͬ���ٽ���
			if(::ReleaseMutex(hMutex) != FALSE){
				nRes = IOTRS_HTDL_ERR_SQUE;
			}
		}
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ȴ����ݷ������

@param      (i)HANDLE hEvent			��������¼�

@retval     IOTRS_RESULT  IOTRS_RES_WINERR:ʧ��   IOTRS_RES_OK:�ɹ�

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
IOTRS_RESULT WaitSendComplete(IN HANDLE hEvent)
{
	IOTRS_RESULT nRes = IOTRS_RES_WINERR;					   //Suppose the task is fail;
	DWORD nResult = ::WaitForSingleObject(hEvent, INFINITE);	//Wait for the event
	if (nResult != WAIT_FAILED){
		nRes = IOTRS_RES_OK;
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����������������ն���

@param      (i)int Handle			���Ӿ��
@param      (o)SENDITEM* pItem		Ҫ���յ�������

@retval     IOTRS_RESULT  IOTRS_HTDL_ERR_SQUE:���Ͷ�����   IOTRS_RES_OK:�ɹ�

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
IOTRS_RESULT PostRecvRequest(IN IOTRS_HANDLE Handle,IN RECVITEM* pItem)
{
	IOTRS_RESULT nRes = IOTRS_RES_WINERR;
	HANDLE hRecv = g_Devs[Handle - 1].hRecv;				//Handle of LetRecv event (Service)
	HANDLE hMutex = g_Devs[Handle - 1].pWork->hMutex;		//Handle of Critical section
	RECV_QUEUE* pQue = g_Devs[Handle - 1].pWork->pRcvQue;	//The queue storing received items.
	
	// Wait for permission
	DWORD nResult = ::WaitForSingleObject(hMutex,INFINITE);	
	// Enter critical section
	if(nResult != WAIT_FAILED){
		// The queue is not full
		if (pQue->nCnt < pQue->nMax){
			// Push the item into queue, the item will store received response
			::CopyMemory(&pQue->Que[pQue->nWP++],pItem,sizeof(RECVITEM));
			// Update counter of the queue
			pQue->nCnt++;	
			// The free slot is out of range,shift it cyclic
			if(pQue->nWP >= pQue->nMax){
				pQue->nWP = 0;						
			}
			// Leave critical section
			if (::ReleaseMutex(hMutex) != 0){
				// Notify the service to receive data
				if(::SetEvent(hRecv) != 0){
					// Task success
					nRes = IOTRS_RES_OK;			
				}
			}
		}
		// The queue is full
		else{
			// Leave the critical section
			if(::ReleaseMutex(hMutex) != 0){
				// Task fail
				nRes = IOTRS_HTDL_ERR_RQUE;		
			}
		}
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ȴ����ݽ������

@param      (i)HANDLE hEvent			��������¼�

@retval     IOTRS_RESULT  IOTRS_RES_WINERR:ʧ��   IOTRS_RES_OK:�ɹ�

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
IOTRS_RESULT WaitRecvComplete(IN HANDLE hEvent)
{
	IOTRS_RESULT nRes = IOTRS_RES_WINERR;
	DWORD nResult = ::WaitForSingleObject(hEvent,INFINITE);
	if(nResult != WAIT_FAILED){
		nRes = IOTRS_RES_OK;
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��������Ϣ

@param      (i)HWND hParent �����ھ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void IOTrs_ShowLastError(HWND hParent)
{
	char* lpMsgBuf;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,    
		NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
	::MessageBoxA(hParent, lpMsgBuf, "THLib", 0);
	::LocalFree(lpMsgBuf);
}


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
INT GetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax)
{
	DWORD dwLen;
	TCHAR szStore[MAX_PATH + 1];   /* + 1 for zero character */

	/* Check the sanity of all input parameters */
	if (lpszBuf != NULL){
		_ASSERTE(ucchMax > 0);		
		if ((0 == ucchMax) || IsBadWritePtr(lpszBuf, ucchMax * sizeof(TCHAR))){
			return -1;
		}
	}

	dwLen = GetModuleFileName(NULL, szStore, MAX_PATH);
	if(0 == dwLen){
		return -2;
	}
	szStore[MAX_PATH] = '\0';

	PathRemoveFileSpec(szStore);
	PathAddBackslash(szStore);

	/* The request buffer size return is len of path and +1 for null character */
	dwLen = lstrlen(szStore) + 1;
	if(lpszBuf != NULL){
		lstrcpyn(lpszBuf, szStore, ucchMax);
	}
	return dwLen;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ر�IO��־����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void ReleaseIOLog(){
	ShutdownTracer();
}