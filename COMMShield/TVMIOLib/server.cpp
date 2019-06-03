
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
BOOL InitServThread(IN PDEVICETBL pTbl);		// 初始化服务线程
void EndServThread(IN PDEVICETBL pTbl);
DWORD WINAPI ServThread(IN LPVOID lpParam);
BOOL InitComPort(IN PDEVICETBL pTbl);			// 初始化并打开串口(创建邮槽)
BOOL EndComPort(IN PDEVICETBL pTbl);			// 关闭并释放串口(释放邮槽)
void GetComunicationMethod(PDEVICETBL pTbl);	// 获取通信方式
BOOL IdlCheck(IN PDEVICETBL pTbl);              // 检查是否有新的数据需要接收或者发送
BOOL SendEvent(IN PDEVICETBL pTbl);             // 检查是否有新的数据需要接收或者发送
BOOL RecvEvent(IN PDEVICETBL pTbl);
BOOL GetSendRequest(IN HTDLWORK* pWork, IN SENDITEM* pItem, OUT IOTRS_RESULT* pRes);
BOOL GetRecvRequest(IN HTDLWORK* pWork,	IN RECVITEM* pItem,	OUT IOTRS_RESULT* pRes);
//INT  ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax);

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
@brief      初始化并打开串口(创建邮槽)

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

@retval     TRUE:成功 FALSE:失败

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
	// 串口通信
	do{
		if(pTbl->pWork->bCommunicationMethod != COM_PORT){	
			break;
		}
		// 打开串口
		sprintf(buff, "\\\\.\\COM%d", (pTbl->Parm.ComPort + 1));
		pTbl->pWork->hComm = CreateFileA(buff,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
		if (pTbl->pWork->hComm == INVALID_HANDLE_VALUE){
			pTbl->nResult = IOTRS_HTDL_ERR_PORT;
			break;
		}
		// 设置输入输出缓存大小
		::SetupComm(pTbl->pWork->hComm, 4096, 4096);
		// 波特率分组
		speed = pTbl->Parm.Speed;
		if (speed < 19200){
			pTbl->pWork->ComSpeed = 0;
		}
		else{
			pTbl->pWork->ComSpeed = 1;
		}

		// 取得当前串口状态
		if (FALSE == ::GetCommState(pTbl->pWork->hComm, &dcb)){
			#ifdef _TRACE_SERVER_
				IOTrs_Trace("InitComPort - GetCommState() = FALSE");
			#endif
			pTbl->nResult = IOTRS_RES_WINERR;
			::CloseHandle(pTbl->pWork->hComm);
			break;
		}
		// 设置DCB参数
		dcb.fBinary = TRUE;						// 二进制模式
		dcb.fParity = TRUE;						// 允许奇偶校验
		dcb.BaudRate = pTbl->Parm.Speed;		// 设置波特率
		dcb.StopBits = ONESTOPBIT;				// 停止位(1)
		dcb.Parity = NOPARITY;				// 奇偶校验方法
		dcb.ByteSize = 8;                       // 数据位
		dcb.fAbortOnError = TRUE;               // 有错误发生时中止读写操作
		dcb.fOutxCtsFlow = FALSE;				// 指定CTS是否用于检测发送控制。当为TRUE时CTS为OFF，发送将被挂起。（发送清除）
		dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS置为OFF 
		dcb.fDtrControl = DTR_CONTROL_DISABLE;  // DTR置为OFF 
		dcb.fOutxDsrFlow = FALSE;				// 指定DSR是否用于检测发送控制。当为TRUE时DSR为OFF，发送将被挂起。
		dcb.fDsrSensitivity = FALSE;			// 当该值为TRUE时DSR为OFF时接收的字节被忽略 
		
		// 设置串口状态
		if (FALSE == ::SetCommState(pTbl->pWork->hComm, &dcb)){
			#ifdef _TRACE_SERVER_
				IOTrs_Trace("InitComPort - SetCommState() = FALSE");
			#endif
			pTbl->nResult = IOTRS_RES_WINERR;
			::CloseHandle(pTbl->pWork->hComm);
			break;
		}
		
		// 设置超时参数
		memset(&timeout, 0, sizeof(timeout));
		timeout.ReadTotalTimeoutMultiplier = 2;		// 读时间系数
		timeout.ReadTotalTimeoutConstant = 0;		// 读时间常量
		timeout.ReadIntervalTimeout = 15;			// 读间隔超时 
		timeout.WriteTotalTimeoutConstant = 0;      // 写时间常量
		timeout.WriteTotalTimeoutMultiplier = 3;    // 写时间系数
		if (FALSE == ::SetCommTimeouts(pTbl->pWork->hComm, &timeout)){
			#ifdef _TRACE_SERVER_
				IOTrs_Trace("InitComPort - SetCommTimeouts() = FALSE");
			#endif
			pTbl->nResult = IOTRS_RES_WINERR;
			::CloseHandle(pTbl->pWork->hComm);
			break;
		}
		// 等待设置生效
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

	// 邮槽通信
	do{
		if( pTbl->pWork->bCommunicationMethod != MAIL_SLOT){
			break;	
		}
		// 创建邮槽
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

	// 管道通信
	do{
		if(pTbl->pWork->bCommunicationMethod != NAME_PIPE){
			break;	
		}
		if(pTbl->pWork->hSimObject == NULL){
			// 创建管道
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
			// 连接客户端,若失败则关闭管道 
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
@brief      关闭并释放串口(释放邮槽)

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

@retval     TRUE:成功 FALSE:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL EndComPort(IN PDEVICETBL pTbl)
{
#ifdef _TRACE_SERVER_
	IOTrs_Trace("EndComPort - Enter");
#endif
	BOOL bRet = FALSE;
	// 串口通信
	do{
		// 邮槽通信则跳出
		if(pTbl->pWork->bCommunicationMethod != COM_PORT){
			break;
		}
		if(::CloseHandle(pTbl->pWork->hComm) == FALSE){
			break;
		}
		bRet = TRUE;
	} while (0) ;


	// 邮槽处理
	do{
		if(pTbl->pWork->bCommunicationMethod != MAIL_SLOT){	
			break;
		}
		if(::CloseHandle(pTbl->pWork->hSimObject) == FALSE){
			break;
		}
		bRet = TRUE;
	}while( 0 );

	// 管道处理
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
@brief      获取通信方式

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

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

	// 取得通信配置
	//TCHAR szAppPath[_MAX_PATH];
	//ComGetAppPath(szAppPath,_MAX_PATH);
	//strcat(szAppPath,DEVICE_INI_FILE_NAME);
	//int CommunicationMethod = GetPrivateProfileInt(TH_CONFIG_SECTION,TH_CONMU_METHOD, COM_PORT, szAppPath);
	pTbl->pWork->bCommunicationMethod = COM_PORT;
	
	// 邮槽
	if(pTbl->pWork->bCommunicationMethod == MAIL_SLOT){
		pTbl->pWork->bCommunicationMethod = NAME_PIPE;
		//do{
		//	// 打开注册表
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
		//	// 取得发送邮槽名称
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
		//	// 取得接收邮槽名称
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
@brief      检查是否有新的数据需要接收或者发送

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

@retval     BOOL  成功:TRUE  失败:FALSE

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
	// 接收队列中存在数据项并且接收缓存队列不为空
	if((pWork->pRcvQue->nCnt != 0) && (pWork->pBuffQue->nCnt != 0)){
#ifdef _TRACE_SERVER_
	IOTrs_Trace("IdlCheck - Has buffered data");
#endif
		// 激活接收数据事件对象
		if(::SetEvent(pTbl->hRecv) == FALSE){
			bRet = FALSE;
		}
	}
	// 服务状态为空闲并且发送队列存在数据项
	else if (((pWork->DrvStat & IOTRS_ST_SNDEXE) == 0) && (pWork->pSndQue->nCnt != 0)){
#ifdef _TRACE_SERVER_
	IOTrs_Trace("IdlCheck - Has request on queue");
#endif
		// 激活发送数据事件对象
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
@brief      从发送队列取出一个数据项

@param      (i)HTDLWORK* pWork		工作空间结构指针
@param      (o)SENDITEM* pItem		准备发送数据项
@param      (o)IOTRS_RESULT* pRes  命令执行结果

@retval     BOOL   成功:TRUE  失败:FALSE

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

	// 进入临界区
	nResult = ::WaitForSingleObject(hMutex,INFINITE);	
	if (nResult != WAIT_FAILED){
		// 发送队列不为空
		if(pQue->nCnt > 0){
			// 取得一个发送数据项的拷贝
			::CopyMemory(pItem,&pQue->Que[pQue->nRP++],sizeof(SENDITEM));
			// 队列空
			if(--pQue->nCnt == 0){
				// 清除存在待发送数据项状态
				pWork->DrvStat &= ~IOTRS_ST_SNDQUE;
			}
			// 游标到达末尾则返回开始位置，循环
			if(pQue->nRP >= pQue->nMax){
				pQue->nRP = 0;
			}
			// 离开临界区
			if (::ReleaseMutex(hMutex) != 0){
				bRet = TRUE;
				*pRes = IOTRS_RES_OK;
			}
		}
		// 发送队列为空
		else{
			// 清除存在待发送数据项状态
			pWork->DrvStat &= ~IOTRS_ST_SNDQUE;
			// 离开临界区
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
@brief      从接收队列取出一个数据项

@param      (i)HTDLWORK* pWork		工作空间结构指针
@param      (o)SENDITEM* pItem		准备接收的数据项
@param      (o)IOTRS_RESULT* pRes  命令执行结果

@retval     BOOL   成功:TRUE  失败:FALSE

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

	// 进入临界区
	DWORD nResult = ::WaitForSingleObject(hMutex,INFINITE);
	if (nResult != WAIT_FAILED){
		// 接收队列不空
		if(pQue->nCnt > 0){
			// 取得一个接收数据项的拷贝
			::CopyMemory(pItem,&pQue->Que[pQue->nRP++],sizeof(RECVITEM));
			// 元素数量减一
			pQue->nCnt--;							
			// 游标到达末尾则返回开始位置，循环
			if(pQue->nRP >= pQue->nMax){
				pQue->nRP = 0;
			}
			// 离开临界区
			if(::ReleaseMutex(hMutex) != 0){
				bRet = TRUE;						// Operation success
				*pRes = IOTRS_RES_OK;				// Set return code
			}
		}
		// 队列空
		else{
			// 离开临界区
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
		// 检查服务状态
		if ((pWork->DrvStat & IOTRS_ST_SNDEXE) != 0){
			// 服务忙，正在发送数据项
			bRet = TRUE;
			break;
		}

		// 从发送队列取出数据项
		if(GetSendRequest(pWork,&pWork->SndItem,&nResult) == FALSE){
			bRet = TRUE;
			break;
		}

		// 初始化发送操作结果为失败
		*(pWork->SndItem.pResult) = IOTRS_RES_WINERR;

		// 设置服务状态忙，为正在发送数据
		UpdateStatus(pWork, IOTRS_ST_SNDEXE, TRUE);	
		
		//
		//Ask the service to send item
		//
//---------------------------------------------------------------------
		pWork->SndItem.pSending = pWork->SndItem.pCommand;		// 设置发送数据起始指针
		*pWork->SndItem.pResult =	IOTRS_HTDL_ERR_SENDFAIL;	// 初始为发送失败
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

	// 激活数据项发送完成事件
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

        // 写log文件（读写器更新命令不写日志）
		if (pWork->RcvItem.pBuffer[14] != 0xD0 && pWork->RcvItem.pBuffer[14] != 0x65) {
            char log[MAX_BUFFER_SIZE*2] = {0};
            log[0] = '<';
			log[1] = ' ';
            // 日志输出只记录到块号
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
@brief      创建协议处理线程

@param      (i)PDEVICETBL pTbl	设备连接及通信控制结构

@retval     DWORD   0:成功   非0:失败

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
		// 创建命令发送完成事件
		pWork->hFinSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hFinSend == NULL){
			break;						
		}
		pWork->hFrameSend = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hFrameSend == NULL){
			break;						
		}
		// 创建协议线程同步事件
		pWork->hSync = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hSync == NULL){
			break;					
		}
		// 创建协议线程关闭事件
		pWork->hClose = ::CreateEvent(NULL, FALSE, FALSE, NULL);
		if (pWork->hClose == NULL){
			break;						
		}

		// 创建协议线程
		pWork->hThread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)HtdlThread,(LPVOID)pWork,0, &dchk);
		if(pWork->hThread == NULL){
			break;
		}
		// 等待协议线程正常结束
		dchk = ::WaitForSingleObject(pWork->hSync, INFINITE);
		if(dchk == WAIT_TIMEOUT){
			// 终结线程
			::TerminateThread(pWork->hThread, NULL);
		}

		// 协议线程异常结束
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

	// 释放资源
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
@brief      初始化服务线程

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

@retval     TRUE: 成功   FALSE: 失败

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
		// 分配工作结构空间
		pTbl->pWork = new HTDLWORK;
		ZeroMemory(pTbl->pWork, sizeof(HTDLWORK));
		if (pTbl->pWork == NULL){
			pTbl->nResult = IOTRS_HTDL_ERR_WORKNG;
			break;
		}
		pWork = pTbl->pWork;

		// 为发送队列分配内存空间，初始化发送队列信息
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

		// 为接收队列分配内存空间，初始化接收队列信息
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

		// 为接收数据缓存队列分配内存空间，初始化数据缓存队列信息
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

		// 为接收数据缓存内数据项分配内存空间
		BUFFITEM* pBuffItem;
		for (int i=0; i<pBuffQueue->nMax; i++){
			pBuffItem = &pBuffQueue->Que[i];
			pBuffItem->nWP = 0;
			pBuffItem->pData = new BYTE[pBuffQueue->nMaxBuffSize];
		}

		// 创建工作空间操作同步信号量
		pWork->hMutex = ::CreateMutex(NULL, FALSE, NULL);
		if (pWork->hMutex == NULL){
			pTbl->nResult = IOTRS_RES_WINERR;
			break;
		}

		// 初始化工作空间变量
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
@brief      结束服务线程并释放资源

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

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
@brief      服务线程函数(发送，接收，关闭)

@param      (i)PDEVICETBL pTbl 设备连接及通信控制结构指针

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
	// 初始化服务线程
	if (InitServThread(pTbl)){
		// 获取通信方式
		GetComunicationMethod(pTbl);	
		// 初始化串口
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
	// 结束服务线程
	EndServThread(pTbl);

	::SetEvent(pTbl->hSync);

#ifdef _TRACE_SERVER_
	IOTrs_Trace("ServThread - Exit - return 0");
#endif
	return 0;
}