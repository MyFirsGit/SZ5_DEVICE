#include <process.h>
#include "stdio.h"
#include "common.h"
#include <stdlib.h>
#include <memory.h>
#include "CHLib.h"
#include "NamedPipeMgr.h"
#include <shlwapi.h>
#include <TCHAR.h>

#ifdef _X86_
 extern "C" { int _afxForceUSRDLL; }
 #else
 extern "C" { int __afxForceUSRDLL; }
 #endif
//----------------- 共享数据段------------------
#pragma data_seg("Shared")

HANDLE g_hinstDll = 0;				  // DLL句柄		
long g_LoadCount = 0;                 // 连接数量
DEVICETBL g_Devs[MAX_DEVICES] = {0};  // 设备连接及通信控制结构定义

#pragma data_seg()
#pragma comment(linker, "/section:Shared,rws")
//-----------------------------------------------

// 串行化连接池操作用信号量
HANDLE g_hMutex = 0;

// Prototype
void CHTrs_Trace(const char* lpszFormat, ...);
CHTRS_RESULT OpenCheck(IN int Handle);
CHTRS_RESULT PostSendRequest(IN CHTRS_HANDLE Handle, IN SENDITEM* pItem);
CHTRS_RESULT WaitSendComplete(IN HANDLE hEvent);
CHTRS_RESULT PostRecvRequest(IN CHTRS_HANDLE Handle, IN RECVITEM* pItem);
CHTRS_RESULT WaitRecvComplete(IN HANDLE hEvent);
INT  GetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);
//const char* CH_TRACE_PACH_AND_NAME = "ThTrace_*.log";

CCHLog g_Debug;
char* m_szText = NULL;					// 日志字符串
char* m_szTraceBuffer = NULL;

// 模拟器用
NamedPipeMgr namepipeMgr;				// 管道通信控制
int iCommunicationType;                 // 通信方式
// CH配置文件
#define	DEVICE_INI_FILE_NAME	_T("DEVICE.INI")		// 配置文件名
#define CH_CONFIG_SECTION	_T("CH")			// SECTION值
#define CH_CONMU_MECHOD		_T("CMU_MECHOD") 	// 通信方式KEY值

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化日志资源

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
@brief      释放日志资源

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
@brief      打印CH日志

@param      (i)const char* lpszFormat 日志格式

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void CHTrs_Trace(const char* lpszFormat, ...)
{
	_ASSERTE(lpszFormat);
    g_Debug.print(lpszFormat);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化CH日志文件

@param      (i)const char* tracePath  日志保存目录字符串

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void CHTrs_Initialize(const char* tracePath)
{
    g_Debug.Initialize(tracePath);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清空CH日志文件

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void CHTrs_Clear()
{
    g_Debug.ClearLog();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      DLL入口函数

@param      (i)HANDLE hModule				DLL句柄
@param      (i)DWORD ul_reason_for_call		调用原因

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved)
{
	BOOL bRet = TRUE;
	int i = 0;
	switch (ul_reason_for_call){
	// 加载DLL
	case DLL_PROCESS_ATTACH:
		// 首次初始化实例,初始化连接池
		if(InterlockedIncrement(&g_LoadCount) == 1){			
			g_hinstDll = hModule;			
			ZeroMemory(g_Devs, sizeof(g_Devs));
			for (i = 0; i < MAX_DEVICES; i++){
				g_Devs[i].bUsed = false;
			}
		}
		// 创建临界区，用于同步对连接池的操作
		g_hMutex = CreateMutexA(NULL, FALSE, "basao_mimtrs");
		if (g_hMutex == NULL){
			bRet = FALSE;
			break;
		}

		SetupTracer();

		// 取得通信配置
//		TCHAR szAppPath[MAX_PATH];
//		GetAppPath(szAppPath,MAX_PATH);
//		_tcscat(szAppPath,DEVICE_INI_FILE_NAME);
//		iCommunicationType = GetPrivateProfileInt(CH_CONFIG_SECTION,CH_CONMU_MECHOD, COM_PORT, szAppPath);
		iCommunicationType = COM_PORT;

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	// 卸载DLL
	case DLL_PROCESS_DETACH:
		// 最后实例关闭
		if (InterlockedDecrement(&g_LoadCount) == 0){
			// 清除连接池
			for (i = 0; i < MAX_DEVICES; i++){
				if(TRUE == g_Devs[i].bUsed){
					CHTrs_Close((i+1));
				}
			}
		}
		// 释放连接池操作临界区资源
		if (g_hMutex != NULL){
			CloseHandle(g_hMutex);	
			g_hMutex = NULL;
		}

		// 关闭日志
		ShutdownTracer();
		break;
	}
    return bRet;
}

extern DWORD WINAPI ServThread(IN LPVOID lpParam);

//////////////////////////////////////////////////////////////////////////
/**
@brief      打开CH通信连接

@param      (o)CHTRS_HANDLE* pHandle   已打开的连接实例句柄
@param      (i)void* pParam             指向结构体(CHTRS_CONNECT_PARAM)的指针

@retval     CHTRS_RESULT   CHTRS_RES_OK:成功   其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP CHTRS_RESULT CHTrs_Open(OUT CHTRS_HANDLE* pHandle,IN  void* pParam)
{
	if(iCommunicationType == COM_PORT){
		CHTRS_RESULT nResult = CHTRS_RES_WINERR;
		DWORD dchk;
		DWORD dwTime;
		int i;

		// 检查句柄是否为NULL
		_ASSERTE(NULL != pHandle);

		// 进入连接池操作临界区
		dchk = ::WaitForSingleObject(g_hMutex, INFINITE);	
		if (dchk == WAIT_FAILED){
			return nResult;
		}
		// 查找连接池中的空闲对象 
		for (i = 0; i < MAX_DEVICES; i++){	
			// 找到空闲连接对象
			if (g_Devs[i].bUsed == false){			
				ZeroMemory(&g_Devs[i], sizeof(DEVICETBL));
				g_Devs[i].bUsed = true;
				g_Devs[i].bClose = false;
				nResult = CHTRS_RES_OK;
				break;
			}
		}
		// 连接池已满
		if (i >= MAX_DEVICES){
			nResult = CHTRS_HTDL_ERR_FULL;
		}
		// 离开连接池操作临界区
		if(::ReleaseMutex(g_hMutex) == FALSE){
			g_Devs[i].bUsed = false;
			nResult = CHTRS_RES_WINERR;
			return nResult;
		}

		// 连接失败则返回
		if (nResult != CHTRS_RES_OK){
			return nResult;
		}

		// 建立连接
		nResult = CHTRS_RES_WINERR;
		do {
			// 保存连接参数信息
			CopyMemory(&g_Devs[i].Parm, pParam, sizeof(CHTRS_CONNECT_PARAM));

			// 创建服务线程启动同步事件
			g_Devs[i].hSync = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hSync == NULL){
				break;	
			}

			// 创建发送事件
			g_Devs[i].hSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hSend == NULL){
				break;
			}

			// 创建接收事件
			g_Devs[i].hRecv = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hRecv == NULL){
				break;	
			}

			// 创建关闭连接事件
			g_Devs[i].hClose = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (g_Devs[i].hClose == NULL){
				break;
			}
		
			// 创建服务线程
			g_Devs[i].hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServThread, (LPVOID)&g_Devs[i], CREATE_SUSPENDED, &dchk);
			if (g_Devs[i].hThread == NULL){				
				break;	
			}
			// 激活服务线程
			::ResumeThread(g_Devs[i].hThread);

			// 等待服务线程执行结束
			dchk = ::WaitForSingleObject(g_Devs[i].hSync, INFINITE);
			// 执行出错
			if (dchk == WAIT_TIMEOUT) {
				::TerminateThread(g_Devs[i].hThread, NULL);
				if (g_Devs[i].pWork != NULL){
					free(g_Devs[i].pWork);
				}
				nResult = CHTRS_HTDL_ERR_OPNTO;
				break;
			}
			// 系统错误
			if (dchk == WAIT_FAILED){
				break;	
			}

			// 服务线程执行失败
			nResult =  g_Devs[i].nResult;
			if (nResult != CHTRS_RES_OK){
				// 等待服务线程执行结束后再跳出
				for (int j = 0; j < 100; j++){
					::GetExitCodeThread(g_Devs[i].hThread, &dchk);
					if (dchk != STILL_ACTIVE){
						break;
					}
					Sleep(100);
				}
			}
		} while (0);

		// 打开连接失败则释放各同步对象
		if (nResult != CHTRS_RES_OK){
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
		// 打开成功则设置句柄ID
		else {		
			*pHandle = i + 1; 
		}
		return nResult;
	}
	// 管道通信
	else{
		return namepipeMgr.create();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭CH通信连接

@param      (i)CHTRS_HANDLE Handle   要关闭连接实例句柄

@retval     CHTRS_RESULT   CHTRS_RES_OK:成功   其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP CHTRS_RESULT CHTrs_Close(IN CHTRS_HANDLE Handle)
{
	if(iCommunicationType == COM_PORT){
		CHTRS_RESULT nResult = CHTRS_RES_OK;
		DWORD dchk;
		int i;
		nResult = OpenCheck(Handle);
		// 连接有效
		if (nResult == CHTRS_RES_OK){
			// 触发关闭连接事件
			SetEvent(g_Devs[Handle-1].hClose);	
			// 等待服务线程关闭处理结束
			dchk = WaitForSingleObject(g_Devs[Handle-1].hSync, INFINITE);
			if (dchk == WAIT_FAILED){
				nResult = CHTRS_RES_WINERR;
			}
			else{
				nResult = g_Devs[Handle-1].nResult;	//last result
			}
			g_Devs[Handle-1].bUsed = false;
			
			// 等待服务线程正常退出
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
	// 管道通信
	else{
		if(namepipeMgr.close()){
			return CHTRS_RES_OK;
		}
		else{
			return CHTRS_RES_WINERR;
		}		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送CH命令

@param      (i)CHTRS_HANDLE Handle		连接实例句柄
@param      (i)DWORD nSendLength		数据长度
@param      (i)void* pBuffer			数据内容指针
@param      (O)DWORD* pOutLength		已发送的数据长度

@retval     CHTRS_RESULT   CHTRS_RES_OK:成功   其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP CHTRS_RESULT CHTrs_Send(IN CHTRS_HANDLE Handle,IN DWORD nSendLength,IN void* pBuffer,OUT DWORD* pOutLength)
{
	//CHTrs_Trace("CHTrs_Send - enter");
	CHTRS_RESULT nResult = CHTRS_RES_OK;
	if(iCommunicationType == COM_PORT){		
		CHTRS_RESULT nRes;
		HANDLE hEvent;		// 发送完成事件
		SENDITEM wItem;		// 发送数据项

		_ASSERTE(NULL != pBuffer);

		nResult = OpenCheck(Handle);				
		// 连接正常
		if (nResult == CHTRS_RES_OK){
			// 创建发送完成事件
			hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			// 创建成功
			if (hEvent != NULL){
				// 准备发送数据项内容
				wItem.hEvent = hEvent;					//Notify event
				wItem.pResult = &nResult;				//Result of task
				wItem.nSendLength = nSendLength;		//Number of byte to be sent
				wItem.pCommand = (BYTE*)pBuffer;		//Data to be sent
				wItem.pOutLength = pOutLength;			//Number of byte were sent
				
				// 将发送数据项加入发送队列
				nRes = PostSendRequest(Handle, &wItem);	
				// 成功
				if(nRes == CHTRS_RES_OK){
					// 等待发送完成 
					nRes = WaitSendComplete(hEvent);	
				}
				// 释放发送完成事件
				if (::CloseHandle(hEvent) == FALSE)	{
					nRes = CHTRS_RES_WINERR;
				}
				if(nRes != CHTRS_RES_OK){
					nResult = nRes;
				}
			}
			// 创建失败
			else{
				nResult = CHTRS_RES_WINERR;
			}
		}
	}
	// 管道通信
	else{
		*pOutLength = namepipeMgr.write((char*)pBuffer,nSendLength);
		if(*pOutLength != 0){
			return CHTRS_RES_OK;
		}
		else{
			return CHTRS_RES_WINERR;
		}
	}
	//CHTrs_Trace("CHTrs_Send - exit - return = %08X", nResult);
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
@brief      接收CH反馈数据

@param      (i)CHTRS_HANDLE Handle		连接实例句柄
@param      (i)DWORD nSendLength		接收数据长度
@param      (i)void* pBuffer			接收数据内容指针
@param      (O)DWORD* pOutLength		已接收的数据长度

@retval     CHTRS_RESULT   CHTRS_RES_OK:成功   其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP CHTRS_RESULT CHTrs_Recv(IN CHTRS_HANDLE Handle,IN DWORD nBuffLength,OUT void* pBuffer,OUT DWORD* pRecvLength)
{
	//CHTrs_Trace("CHTrs_Recv - enter");
	CHTRS_RESULT nResult = CHTRS_RES_OK;
	if(iCommunicationType == COM_PORT){		
		CHTRS_RESULT nRes;
		HANDLE hEvent;
		RECVITEM wItem;
		*pRecvLength = 0;
		
		nResult = OpenCheck(Handle);
		// 连接有效
		if (nResult == CHTRS_RES_OK){
			// 创建接收完成事件
			hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
			if (hEvent != NULL)	{
				// 初始化接收数据项
				wItem.hEvent = hEvent;
				wItem.pResult = &nResult;
				wItem.nBuffSize = nBuffLength;
				wItem.pBuffer = (BYTE*)pBuffer;
				wItem.pRecvLength = pRecvLength;
				// Post receiving request
				nRes = PostRecvRequest(Handle, &wItem);	
				if (nRes == CHTRS_RES_OK)				//Request posted
				{
					nRes= WaitRecvComplete(hEvent);		//Wait until received
				}
				if (::CloseHandle(hEvent) == FALSE)
				{
					nRes= CHTRS_RES_WINERR;
				}
				if (nRes != CHTRS_RES_OK) 
				{
					nResult = nRes;
				}
			}
			//Unable to create event
			else{
				nResult = CHTRS_RES_WINERR;
			}
		}
	}
	// 管道通信
	else{
		*pRecvLength = namepipeMgr.read((char*)pBuffer,nBuffLength);
		if(*pRecvLength != 0){
			return CHTRS_RES_OK;
		}
		else{
			return CHTRS_RES_WINERR;
		}
	}

	//CHTrs_Trace("CHTrs_Recv - exit - return = %08X", nResult);
	return nResult;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重置CH通信处理

@param      (i)CHTRS_HANDLE Handle		连接实例句柄
@param      (i)BOOL bSendClear		    是否中止所有未发送处理
@param      (i)BOOL bRecvClear			是否中止所有未接收处理

@retval     CHTRS_RESULT   CHTRS_RES_OK:成功   其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP CHTRS_RESULT CHTrs_Reset(IN CHTRS_HANDLE Handle,IN BOOL bSendClear,IN BOOL bRecvClear)
{
	if(iCommunicationType == COM_PORT){
		HANDLE hMutex;
		HTDLWORK* pWork;
		DWORD nChk, dwErr;
		COMSTAT commstat;

		CHTRS_RESULT nResult = OpenCheck(Handle);
		do{
			// 连接有效
			if (CHTRS_RES_OK != nResult){
				break;
			}
			hMutex = g_Devs[Handle - 1].pWork->hMutex;
			pWork = g_Devs[Handle - 1].pWork;
			nChk = ::WaitForSingleObject(hMutex, INFINITE);
			if (WAIT_FAILED == nChk){
				nResult = CHTRS_RES_WINERR;
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
						*(pWork->SndItem.pResult) = CHTRS_RES_RESET;
						::SetEvent( pWork->SndItem.hEvent );
						::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
						::ClearCommError(pWork->hComm, &dwErr, &commstat);
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER){
					nResult = CHTRS_RES_WINERR;
					break;
				}
			}
			if (TRUE == bRecvClear){
				__try{
					pWork->pRcvQue->nCnt = 0;
					if (INVALID_HANDLE_VALUE != pWork->RcvItem.hEvent){
						pWork->Phase = PHASE_S1;
						*(pWork->RcvItem.pResult) = CHTRS_RES_RESET;
						::SetEvent( pWork->RcvItem.hEvent );
						pWork->pBuffQue->nCnt = 0;
						::PurgeComm(pWork->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
						::ClearCommError(pWork->hComm, &dwErr, &commstat);
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER){
					nResult = CHTRS_RES_WINERR;
					break;
				}
			}
			if (FALSE == ::ReleaseMutex(hMutex)){
				nResult = CHTRS_RES_WINERR;
				break;
			}
		} while(0);

		return nResult;
	}
	else{
		return CHTRS_RES_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前通信状态

@param      (i)CHTRS_HANDLE Handle		连接实例句柄
@param      (o)DWORD* pStatus		    当前状态

@retval     CHTRS_RESULT   CHTRS_RES_OK:成功   其他:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP CHTRS_RESULT CHTrs_GetStat(IN CHTRS_HANDLE Handle,OUT DWORD* pStatus)
{
	if(iCommunicationType == COM_PORT){
		DWORD nWait;
		DWORD Stat;	

		_ASSERTE(NULL != pStatus);

		*pStatus = 0;
		CHTRS_RESULT nResult = OpenCheck(Handle);
		if(nResult == CHTRS_RES_OK){
			nWait = ::WaitForSingleObject(g_Devs[Handle-1].pWork->hMutex, INFINITE);
			if(nWait != WAIT_FAILED){
				::GetCommModemStatus(g_Devs[Handle-1].pWork->hComm, &Stat);
				if((Stat & MS_CTS_ON) != 0){
					g_Devs[Handle-1].pWork->DrvStat |= CHTRS_ST_CTS;
				}
				else{
					g_Devs[Handle-1].pWork->DrvStat &= ~CHTRS_ST_CTS;
				}
				if((Stat & MS_DSR_ON) != 0) {
					g_Devs[Handle-1].pWork->DrvStat |= CHTRS_ST_DSR;
				}
				else{
					g_Devs[Handle-1].pWork->DrvStat &= ~CHTRS_ST_DSR;
				}
				::ReleaseMutex(g_Devs[Handle-1].pWork->hMutex);	
				*pStatus = g_Devs[Handle-1].pWork->DrvStat;
			}		
		}
		return nResult;
	}
	else{
		// 管道通信	
		Sleep(300);
		*pStatus = namepipeMgr.GetStat();
		return CHTRS_RES_OK;
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查连接句柄是否有效

@param      (i)int Handle 连接句柄

@retval     CHTRS_RESULT  CHTRS_ERR_HANDLE:无效   CHTRS_RES_OK:有效

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CHTRS_RESULT OpenCheck(IN int Handle)
{
	//	CHTrs_Trace("OpenCheck - enter");
	CHTRS_RESULT nRes;
	if(iCommunicationType == COM_PORT){		
		if ((Handle < 1) || (Handle > MAX_DEVICES) || (g_Devs[Handle-1].bUsed == false)){
			nRes = CHTRS_ERR_HANDLE;
		}
		else{
			nRes = CHTRS_RES_OK;
		}
	}
	else{
		if(namepipeMgr.pipe_ != NULL){
			nRes = CHTRS_RES_OK;
		}
		else{
			nRes = CHTRS_ERR_HANDLE;
		}
	}
	//	CHTrs_Trace("OpenCheck - exit - Res = %08X ", nRes);
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将发送数据项加入发送队列

@param      (i)int Handle			连接句柄
@param      (i)SENDITEM* pItem		要发送的数据项

@retval     CHTRS_RESULT  CHTRS_HTDL_ERR_SQUE:发送队列满   CHTRS_RES_OK:成功

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CHTRS_RESULT PostSendRequest(IN CHTRS_HANDLE Handle,IN SENDITEM* pItem)
{
	CHTRS_RESULT nRes = CHTRS_RES_WINERR;					// Suppose the task is OK
	HANDLE hSend = g_Devs[Handle - 1].hSend;				// Handle of LetSend event (Service)
	HANDLE hMutex = g_Devs[Handle - 1].pWork->hMutex;		// Handle of the synchronize mutex
	SEND_QUEUE* pQue = g_Devs[Handle - 1].pWork->pSndQue;	// The queue stores sending items

	// 进入临界区
	DWORD nResult = ::WaitForSingleObject(hMutex, INFINITE);	
	if (nResult != WAIT_FAILED){
		// 发送队列未满
		if(pQue->nCnt < pQue->nMax){
			// 将发送数据项加入发送队列
			::CopyMemory(&pQue->Que[pQue->nWP++],pItem,sizeof(SENDITEM));
			// 发送队列元素计数+1
			pQue->nCnt++;							
			// 写入游标循环
			if(pQue->nWP >= pQue->nMax){
				pQue->nWP = 0;
			}
			// 设置服务状态为队列存在待发送数据项
			g_Devs[Handle - 1].pWork->DrvStat |= CHTRS_ST_SNDQUE;
			// 离开工作空间操作同步临界区
			if(::ReleaseMutex(hMutex) != FALSE){
				// 激活发送事件
				if(::SetEvent(hSend) != FALSE){
					nRes = CHTRS_RES_OK;
				}
			}
		}
		// 发送队列满
		else{
			// 离开工作空间操作同步临界区
			if(::ReleaseMutex(hMutex) != FALSE){
				nRes = CHTRS_HTDL_ERR_SQUE;
			}
		}
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      等待数据发送完成

@param      (i)HANDLE hEvent			发送完成事件

@retval     CHTRS_RESULT  CHTRS_RES_WINERR:失败   CHTRS_RES_OK:成功

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CHTRS_RESULT WaitSendComplete(IN HANDLE hEvent)
{
	CHTRS_RESULT nRes = CHTRS_RES_WINERR;					   //Suppose the task is fail;
	DWORD nResult = ::WaitForSingleObject(hEvent, INFINITE);	//Wait for the event
	if (nResult != WAIT_FAILED){
		nRes = CHTRS_RES_OK;
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将接收数据项加入接收队列

@param      (i)int Handle			连接句柄
@param      (o)SENDITEM* pItem		要接收的数据项

@retval     CHTRS_RESULT  CHTRS_HTDL_ERR_SQUE:发送队列满   CHTRS_RES_OK:成功

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CHTRS_RESULT PostRecvRequest(IN CHTRS_HANDLE Handle,IN RECVITEM* pItem)
{
	CHTRS_RESULT nRes = CHTRS_RES_WINERR;
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
					nRes = CHTRS_RES_OK;			
				}
			}
		}
		// The queue is full
		else{
			// Leave the critical section
			if(::ReleaseMutex(hMutex) != 0){
				// Task fail
				nRes = CHTRS_HTDL_ERR_RQUE;		
			}
		}
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      等待数据接收完成

@param      (i)HANDLE hEvent			接收完成事件

@retval     CHTRS_RESULT  CHTRS_RES_WINERR:失败   CHTRS_RES_OK:成功

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CHTRS_RESULT WaitRecvComplete(IN HANDLE hEvent)
{
	CHTRS_RESULT nRes = CHTRS_RES_WINERR;
	DWORD nResult = ::WaitForSingleObject(hEvent,INFINITE);
	if(nResult != WAIT_FAILED){
		nRes = CHTRS_RES_OK;
	}
	return nRes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示最后错误信息

@param      (i)HWND hParent 父窗口句柄

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void CHTrs_ShowLastError(HWND hParent)
{
	char* lpMsgBuf;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,    
		NULL,GetLastError(),MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
	::MessageBoxA(hParent, lpMsgBuf, "CHLib", 0);
	::LocalFree(lpMsgBuf);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前应用程序的路径

@param      (o)  LPTSTR lpszBuf            路径名的空间
@param      (i)  DWORD ucchMax             最大的路径长度

@retval     INT   \n
E_INVALID_PARAMS:参数无效, E_WIN32_ERROR:系统错误, 其他:路径的实际长度

@exception  无
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
@brief      关闭日志连接

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DLLIMP void ReleaseCHLog(){
	ShutdownTracer();
}