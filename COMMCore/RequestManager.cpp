#include "StdAfx.h"
#include "CoreCommon.h"
#include "RequestManager.h"
#include <winuser.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
//IMPLEMENT_DYNCREATE(CRequestManager, CUtilThread)
//
//BEGIN_MESSAGE_MAP(CRequestManager, CUtilThread)
//    ON_THREAD_MESSAGE (REQUEST_QUEUE, OnRequestQueue)
//    ON_THREAD_MESSAGE (REQUEST_COMPLETE, OnRequestComplete)
//END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//CRequestManager::CRequestManager(void)
//    :CUtilThread()
//    ,m_pMainRequestHandler(NULL)
//{
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//CRequestManager::~CRequestManager(void)
//{
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      线程初期化响应函数

@param      无

@retval     TRUE if succesful.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//BOOL CRequestManager::InitInstance()
//{
//	// TODO:  perform and per-thread initialization here
//	BOOL bResult = TRUE;
//	m_bIdleMode = FALSE;	
//
//    // 创建m_pMainRequestHandler
//    m_pMainRequestHandler = CreateRequestHandler();
//	
//
//
//    if (m_pMainRequestHandler == NULL) {
//        bResult = FALSE;
//    }
//
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      线程中止响应函数

@param      无

@retval     TRUE if succesful.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//int CRequestManager::ExitInstance()
//{
//	// TODO:  perform any per-thread cleanup here
//	DeleteAllRequestHandler();
//	return CUtilThread::ExitInstance();
//}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      Idle 响应函数
//
//@param      (i)LONG lCount
//
//@retval     TRUE if succesful.
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//BOOL CRequestManager::OnIdle(LONG lCount)
//{
//	//Sleep(1);
//	//
//	//// IdleSense feature will be activated.
//	//if ((m_bIdleMode == TRUE) && (m_oCUtilTimer.isTimeOut() == TRUE)) {
//	//	m_pController->DoIdleSense();
//	//	m_oCUtilTimer.Reset();
//	//}
//
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      REQUEST_QUEUE 消息响应 (从SP Control的DoCommand发出)

@param      (i)wParam : request item.
@param      (i)lParam : Execution mode.

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CRequestManager::OnRequestQueue(WPARAM wParam, LPARAM lParam)
//{
//    CRequestHandler* pRequestHandler = NULL;
//
//	switch ( wParam ) {
//		// Using MAIN queue
//		case SP_EXECMODE_QUEUE: {
//            pRequestHandler = m_pMainRequestHandler;
//			break;
//		}
//		case SP_EXECMODE_IMMEDIATE:{
//			pRequestHandler = CreateRequestHandler();
//			break;
//		}
//	}
//
//    if (pRequestHandler != NULL) {
//        pRequestHandler->PostThreadMessage(REQUEST_QUEUE, 0, lParam);
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      REQUEST_COMPLETE 消息响应 (从CRequestHandler发出)
//
//@param      (i)wParam : unused.
//@param      (i)lParam : thread id of request handler.
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::OnRequestComplete(WPARAM wParam, LPARAM lParam)
//{
//    // 删除立即执行的RequestHandler
//    if (lParam != m_pMainRequestHandler->m_nThreadID) {
//        DeleteRequestHandler((DWORD)lParam);
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      创建RequestHandler，并添加到m_mapRequestHandler
//
//@param      无
//
//@retval     RequestHandler*     失败时＝NULL
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CRequestHandler* CRequestManager::CreateRequestHandler()
//    {
//    LPCRequestHandler pRequestHandler = (CRequestHandler*) CUtilThread::StartThread(RUNTIME_CLASS(CRequestHandler), "CRequestHandler");
//
//    if (pRequestHandler != NULL) {
//        pRequestHandler->SetNotifyThreadID(m_nThreadID);
//
//        // Put to RequestHandle list.
//        m_mapRequestHandler.SetAt(pRequestHandler->m_nThreadID, pRequestHandler);
//    }
//
//    return pRequestHandler;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      从RH list中删除指定的request handler线程
//
//@param      (i)DWORD dwThreadID : thread id of request handler.
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::DeleteRequestHandler(DWORD dwThreadID)
//{
//	LPCRequestHandler pRequestHandler;
//
//	m_mapRequestHandler.Lookup(dwThreadID, pRequestHandler);
//
//	// Trying to shutdown thread.
//	if (pRequestHandler != NULL) {
//		CUtilThread::ShutdownThread(pRequestHandler);
//	}
//	
//	m_mapRequestHandler.RemoveKey(dwThreadID);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      关闭并删除从RH list中的所有线程
//
//@param      (i)DWORD dwThreadID : thread id of request handler.
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::DeleteAllRequestHandler(void)
//{
//	POSITION pos = m_mapRequestHandler.GetStartPosition();
//	DWORD    dwKey = 0;
//	LPCRequestHandler pRequestHandler = NULL;
//
//	// Shutdown each RequestHandler item.
//	while (pos != NULL) {
//		m_mapRequestHandler.GetNextAssoc(pos, dwKey, pRequestHandler);
//		
//		// Trying to shutdown thread.
//		if (pRequestHandler != NULL) {
//			CUtilThread::ShutdownThread(pRequestHandler);
//
//			pRequestHandler = NULL;
//		}
//	}
//
//    // Remove all elements
//	m_mapRequestHandler.RemoveAll();
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置 SP Controller
//
//@param      (i)CControl * pController : pointer SP Control
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::SetController(CControl * pController)
//{
//	m_pController = pController;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置IDLE模式
//
//@param      (i)bool bIdleMode : true if IdleMode used; false if unused.
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::SetIdleMode(bool bIdleMode)
//{
//	m_bIdleMode = bIdleMode;
//
//	if ( !bIdleMode) {
//		if ( m_pController != NULL ) {
//			DWORD dwIdleTimer = m_pController->GetIdleTimer() * 1000;
//
//            if ( dwIdleTimer == 0 ) {
//                dwIdleTimer = SP_DEFAULT_IDLE_TIMER * 1000;
//            }
//
//            m_oCUtilTimer.SetTimeOut(dwIdleTimer);
//		}
//	}
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      设置是否暂停MainRequestHandler
//
//@param      (i)bool isPause     true:暂停, false:继续
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::SetPause(bool isPause)
//{
//    m_pMainRequestHandler->SetPause(isPause);
//}

