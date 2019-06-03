
/*****************************************************************************/
/* FILE NAME     : htdl.cpp                                                  */
/* PROGRAM NAME  : This is a library which handling HTDL protocol,           */
/*                      sending and receiving data byte after byte.          */
/* FUNCTION      : 1.                                                        */
/*                                                                           */
/*****************************************************************************/

/******************************************************************************/
/**	@brief Implement file. This is a library which handling HTDL protocol,
 *		sending and receiving data byte after byte.
 */
/******************************************************************************/
#include <windows.h>
#include <process.h>
#include "TSPControlLib.h"
#include "common.h"

int CheckEvent(HTDLWORK *work);
int PreEvent(HTDLWORK *pWork);
int ChkEventChr(unsigned char c);
BOOL IsTimeUp(IN DWORD	dwTimer, IN DWORD	dwBaseTime);
void ExecEvent(HTDLWORK *pWork);
void TimerSetForPhase(HTDLWORK *pWork);

//////////////////////////////////////////////////////////////////////////
/**
@brief      Э�鴦���߳�

@param      (i)LPVOID pHTDLWork			�����ռ�ṹָ��

@retval     DWORD   0:�ɹ�   ��0:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI HtdlThread(LPVOID pHTDLWork)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("HtdlThread - Enter");
#endif
	HTDLWORK *pWork = (PHTDLWORK)pHTDLWork;
	DWORD dchk;
	int iPhase = PHASE_S1;
	pWork->nError = RES_OK;
	// ����Э���߳�ͬ���¼�
	::SetEvent(pWork->hSync);
	for(;;){
		if(iPhase != pWork->Phase){
			if(pWork->Phase == PHASE_S1){
				UpdateStatus(pWork, ST_RCVEXE, FALSE);
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
	CHTrs_Trace("HtdlThread - Exit - return %d", 0);
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
	CHTrs_Trace("CheckEvent - Enter");
#endif
	BOOL chk;
	DWORD nBytesRead;
	int iEvent = EVENT_NONE;	// ��ʼ��Ϊδ֪�¼�
	for (;;) {
		// ��ȡһ���ֽ�
		chk = ReadHtdl(pWork->hComm,&pWork->c,1,&nBytesRead,pWork->bCommunicationMethod,pWork->hSimObject,pWork->MailBuff);
		// ʧ��
		if(chk == FALSE){
#ifdef _TRACE_HTDL_
	CHTrs_Trace("CheckEvent - ST_RCVERR");
#endif
			// ����״̬Ϊ����ʧ��
			pWork->DrvStat |= ST_RCVERR;	
		}
		// �ɹ�
		else{
			if(nBytesRead > 0){
				iEvent = PreEvent(pWork);
				break;
			}
		}
		// ��鳬ʱ
		chk = IsTimeUp(pWork->TimeOut, pWork->BaseTime);
		// ��ʱ
		if(chk == TRUE){
#ifdef _TRACE_HTDL_
	CHTrs_Trace("CheckEvent - EVENT_TIMEOUT");
#endif
			iEvent = EVENT_TIMEOUT;				// Report error
			break;
		}
		// It's in Phase 1 and There is a request send
		if((pWork->Phase == PHASE_S1) && (pWork->ReqSend == true)){
#ifdef _TRACE_HTDL_
	CHTrs_Trace("CheckEvent - EVENT_REQSEND");
#endif
			iEvent = EVENT_REQSEND;						//
			break;
		}
		break;											//
	}

#ifdef _TRACE_HTDL_
	CHTrs_Trace("CheckEvent - Exit - return %d", iEvent);
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
	CHTrs_Trace("PreEvent - Enter");
#endif
	int iEvent = EVENT_NONE;
	iEvent = ChkEventChr(pWork->c);
	
#ifdef _TRACE_HTDL_
	CHTrs_Trace("PreEvent - Exit - return %d", iEvent);
#endif
	return iEvent;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ݵ�ǰ��ȡ�ַ����õ�ǰ�¼�

@param      (i)char c		��ȡ�����ַ�

@retval     int  �¼�ID

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int ChkEventChr(unsigned char c)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("ChkEventChr - Enter");
#endif
	int ret = EVENT_NONE;
	switch (c) 
	{								
		//case ENQ:									// ENQ
		//	ret = EVENT_ENQ;						// ENQ
		//	break;
		case STX:									// STX
			ret = EVENT_STX;						// STX
			break;
		case ETX:									// ETX
			ret = EVENT_ETX;						// ETX
			break;
		default:									// 
			ret = EVENT_CHR;						// 
			break;
	}

#ifdef _TRACE_HTDL_
	CHTrs_Trace("ChkEventChr - Exit - return %d", ret);
#endif
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�ʱ�䴦��ִ���Ƿ�ʱ

@param      (i)DWORD dwTimer		��ʱʱ����
@param      (i)DWORD dwBaseTime		����ʱ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("IsTimeUp - Enter");
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
		dwCurTime = GetTickCount();		// ��ǰʱ��
		dwUpTime = dwBaseTime + dwTimer;// ����ʱ��
		if (dwUpTime >= dwBaseTime){
			if (dwCurTime >= dwBaseTime){
				// ��ʱ����ǰʱ�� > ����ʱ�䣩
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
	CHTrs_Trace("IsTimeUp - Exit - return %d", bRet);
#endif
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ���¼�������

@param      (i)HTDLWORK *pWork		�����ռ�ṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void ExecEvent(HTDLWORK *pWork)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("ExecEvent - Enter");
#endif
	int iPhase = pWork->Phase;							// ȡ�ý׶�						
	int iEvent = pWork->Event;							// ȡ���¼�
	TMATRIX *pMatrix = &(tbl_matrix[iPhase][iEvent]);	
	int iNextPhase = (*pMatrix->func)(pWork, pMatrix);  // ִ�д�����
	pWork->Phase = iNextPhase;							// ������һ�׶�
	pWork->Event = EVENT_NONE;							
	if ((iPhase != iNextPhase) || (iEvent == EVENT_TIMEOUT)){
		TimerSetForPhase(pWork);					//
	}

#ifdef _TRACE_HTDL_
	CHTrs_Trace("ExecEvent - Exit");
#endif
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó�ʱʱ��

@param      (i)HTDLWORK *pWork		�����ռ�ṹָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void TimerSetForPhase(HTDLWORK *pWork)
{
#ifdef _TRACE_HTDL_
	CHTrs_Trace("TimerSetForPhase - Enter");
#endif
	
	pWork->TimeOut = wiTimeOut[pWork->ComSpeed][pWork->Phase];
	pWork->BaseTime = GetTickCount();

#ifdef _TRACE_HTDL_
	CHTrs_Trace("TimerSetForPhase - Exit");
#endif
}