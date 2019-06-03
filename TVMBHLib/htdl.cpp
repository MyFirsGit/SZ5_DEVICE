#include <windows.h>
#include <process.h>
#include "BHLib.h"
#include "common.h"

int CheckEvent(HTDLWORK *work);
int PreEvent(HTDLWORK *pWork);
int ChkEventChr(char c);
BOOL IsTimeUp(IN DWORD	dwTimer, IN DWORD	dwBaseTime);
void ExecEvent(HTDLWORK *pWork);
void TimerSetForPhase(HTDLWORK *pWork);

//////////////////////////////////////////////////////////////////////////
/**
@brief      协议处理线程

@param      (i)LPVOID pHTDLWork			工作空间结构指针

@retval     DWORD   0:成功   非0:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI HtdlThread(LPVOID pHTDLWork)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("HtdlThread - Enter");
#endif
	HTDLWORK *pWork = (PHTDLWORK)pHTDLWork;
	DWORD dchk;
	int iPhase = PHASE_S1;
	pWork->nError = BHTRS_RES_OK;
	// 激活协议线程同步事件
	::SetEvent(pWork->hSync);
	for(;;){
		if(iPhase != pWork->Phase){
			if(pWork->Phase == PHASE_S1){
				UpdateStatus(pWork, BHTRS_ST_RCVEXE, FALSE);
			}
			iPhase = pWork->Phase;
		}
		::CheckEvent(pWork);
		if(pWork->Event != EVENT_NONE){
			ExecEvent(pWork);
		}
		else{
			dchk = ::WaitForSingleObject(pWork->hClose, 10);
			if(dchk == WAIT_OBJECT_0){
				break;
			}
		}
	}
	::SetEvent(pWork->hSync);
#ifdef _TRACE_HTDL_
	BHTrs_Trace("HtdlThread - Exit - return %d", 0);
#endif
	return 0;
}

/******************************************************************************/
/**	@brief Checking if the is a certain event need to be raised.
 *	@param pWork: Pointer to a structure HTDLWORK. (Workspace data);
 *	@return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
int CheckEvent(HTDLWORK *pWork)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("CheckEvent - Enter");
#endif
	BOOL chk;
	DWORD nBytesRead;
	int iEvent = EVENT_NONE;	// 初始化为未知事件
	for (;;) {
		// 读取一个字节
		chk = ReadHtdl(pWork->hComm,&pWork->c,1,&nBytesRead,pWork->bCommunicationMethod,pWork->hSimObject,pWork->MailBuff);
		// 失败
		if(chk == FALSE){
#ifdef _TRACE_HTDL_
	BHTrs_Trace("CheckEvent - BHTRS_ST_RCVERR");
#endif
			// 设置状态为接收失败
			pWork->DrvStat |= BHTRS_ST_RCVERR;	
		}
		// 成功
		else{
			if(nBytesRead > 0){
				iEvent = PreEvent(pWork);
				pWork->LastEvent = iEvent;
				break;
			}
		}
		// 检查超时
		chk = IsTimeUp(pWork->TimeOut, pWork->BaseTime);
		// 超时
		if(chk == TRUE){
#ifdef _TRACE_HTDL_
	BHTrs_Trace("CheckEvent - EVENT_TIMEOUT");
#endif
			iEvent = EVENT_TIMEOUT;				// Report error
			break;
		}
		// It's in Phase 1 and There is a request send and event is not EVENT_OFF
		if((pWork->Phase == PHASE_S1) && (pWork->ReqSend == true) && (pWork->LastEvent != EVENT_OFF)){
#ifdef _TRACE_HTDL_
	BHTrs_Trace("CheckEvent - EVENT_REQSEND");
#endif
			iEvent = EVENT_REQSEND;						//
			break;
		}
		break;											//
	}

#ifdef _TRACE_HTDL_
	BHTrs_Trace("CheckEvent - Exit - return %d", iEvent);
#endif
	pWork->Event = iEvent;

	return iEvent;
}

/******************************************************************************/
/**	@brief 
 *	@param pWork: Pointer to a structure HTDLWORK. (Workspace data);
 *	@return TRUE if success, otherwise return FALSE.
 *	@remarks None.
 */
/******************************************************************************/
int PreEvent(HTDLWORK *pWork)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("PreEvent - Enter");
#endif
	int iEvent = EVENT_NONE;
	// 当前状态为等待BCC
	if(pWork->Phase == PHASE_S6){
		iEvent = EVENT_CHR;						// This is BCC
	}
	else{
		iEvent = ChkEventChr(pWork->c);			// 当前读取字符必然是控制字符	
	}
	
#ifdef _TRACE_HTDL_
	BHTrs_Trace("PreEvent - Exit - return %d", iEvent);
#endif
	return iEvent;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      根据当前读取字符设置当前事件

@param      (i)char c		读取到的字符

@retval     int  事件ID

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int ChkEventChr(char c)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("ChkEventChr - Enter");
#endif
	int ret = EVENT_NONE;
	switch (c) 
	{								
		case ON:									// ON
			ret = EVENT_ON;						// ON
			break;
		case OFF:									// OFF
			ret = EVENT_OFF;						// OFF
			break;
		case STX:									// STX
			ret = EVENT_STX;						// STX
			break;
		case ETX:									// ETX
			ret = EVENT_ETX;						// ETX
			break;
		case ACK:									// ACK
			ret = EVENT_ACK;						// ACK
			break;
		case NAK:									// NAK
			ret = EVENT_NAK;						// NAK
			break;
		default:									// 
			ret = EVENT_CHR;						// 
			break;
	}

#ifdef _TRACE_HTDL_
	BHTrs_Trace("ChkEventChr - Exit - return %d", ret);
#endif
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断时间处理执行是否超时

@param      (i)DWORD dwTimer		超时时间间隔
@param      (i)DWORD dwBaseTime		基本时间

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("IsTimeUp - Enter");
#endif
	BOOL bRet = FALSE;
	DWORD dwCurTime;
	DWORD dwUpTime;
	//            dwTime
	//    |-------------------|
	// dwBaseTime          dwUpTime
	//    |----------|--------|-----
	//           dwCurTime
	// Has timer
	if (dwTimer != 0){
		dwCurTime = GetTickCount();		// 当前时间
		dwUpTime = dwBaseTime + dwTimer;// 结束时间
		if (dwUpTime >= dwBaseTime){
			if (dwCurTime >= dwBaseTime){
				// 超时（当前时间 > 结束时间）
				if (dwCurTime >= dwUpTime){
					bRet = TRUE;
				}
			}
			else{
				bRet = TRUE;
			}
		}
		else{
			if (dwCurTime < dwBaseTime){
				if (dwCurTime >= dwUpTime){
					bRet = TRUE;
				}
			}
		}
	}

#ifdef _TRACE_HTDL_
	BHTrs_Trace("IsTimeUp - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行事件处理函数

@param      (i)HTDLWORK *pWork		工作空间结构指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void ExecEvent(HTDLWORK *pWork)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("ExecEvent - Enter");
#endif
	int iPhase = pWork->Phase;							// 取得阶段						
	int iEvent = pWork->Event;							// 取得事件
	TMATRIX *pMatrix = &(tbl_matrix[iPhase][iEvent]);	
	int iNextPhase = (*pMatrix->func)(pWork, pMatrix);  // 执行处理函数
	pWork->Phase = iNextPhase;							// 设置下一阶段
	pWork->Event = EVENT_NONE;							
	if ((iPhase != iNextPhase) || (iEvent == EVENT_TIMEOUT)){
		TimerSetForPhase(pWork);					//
	}

#ifdef _TRACE_HTDL_
	BHTrs_Trace("ExecEvent - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置超时时间

@param      (i)HTDLWORK *pWork		工作空间结构指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void TimerSetForPhase(HTDLWORK *pWork)
{
#ifdef _TRACE_HTDL_
	BHTrs_Trace("TimerSetForPhase - Enter");
#endif
	
	pWork->TimeOut = wiTimeOut[pWork->ComSpeed][pWork->Phase];
	pWork->BaseTime = GetTickCount();

#ifdef _TRACE_HTDL_
	BHTrs_Trace("TimerSetForPhase - Exit");
#endif
}