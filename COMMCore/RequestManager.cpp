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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//CRequestManager::CRequestManager(void)
//    :CUtilThread()
//    ,m_pMainRequestHandler(NULL)
//{
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//CRequestManager::~CRequestManager(void)
//{
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �̳߳��ڻ���Ӧ����

@param      ��

@retval     TRUE if succesful.

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//BOOL CRequestManager::InitInstance()
//{
//	// TODO:  perform and per-thread initialization here
//	BOOL bResult = TRUE;
//	m_bIdleMode = FALSE;	
//
//    // ����m_pMainRequestHandler
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
@brief      �߳���ֹ��Ӧ����

@param      ��

@retval     TRUE if succesful.

@exception  ��
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
//@brief      Idle ��Ӧ����
//
//@param      (i)LONG lCount
//
//@retval     TRUE if succesful.
//
//@exception  ��
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
@brief      REQUEST_QUEUE ��Ϣ��Ӧ (��SP Control��DoCommand����)

@param      (i)wParam : request item.
@param      (i)lParam : Execution mode.

@retval     ��

@exception  ��
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
//@brief      REQUEST_COMPLETE ��Ϣ��Ӧ (��CRequestHandler����)
//
//@param      (i)wParam : unused.
//@param      (i)lParam : thread id of request handler.
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::OnRequestComplete(WPARAM wParam, LPARAM lParam)
//{
//    // ɾ������ִ�е�RequestHandler
//    if (lParam != m_pMainRequestHandler->m_nThreadID) {
//        DeleteRequestHandler((DWORD)lParam);
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����RequestHandler������ӵ�m_mapRequestHandler
//
//@param      ��
//
//@retval     RequestHandler*     ʧ��ʱ��NULL
//
//@exception  ��
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
//@brief      ��RH list��ɾ��ָ����request handler�߳�
//
//@param      (i)DWORD dwThreadID : thread id of request handler.
//
//@retval     ��
//
//@exception  ��
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
//@brief      �رղ�ɾ����RH list�е������߳�
//
//@param      (i)DWORD dwThreadID : thread id of request handler.
//
//@retval     ��
//
//@exception  ��
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
//@brief      ���� SP Controller
//
//@param      (i)CControl * pController : pointer SP Control
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::SetController(CControl * pController)
//{
//	m_pController = pController;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ����IDLEģʽ
//
//@param      (i)bool bIdleMode : true if IdleMode used; false if unused.
//
//@retval     ��
//
//@exception  ��
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
//@brief      �����Ƿ���ͣMainRequestHandler
//
//@param      (i)bool isPause     true:��ͣ, false:����
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//void CRequestManager::SetPause(bool isPause)
//{
//    m_pMainRequestHandler->SetPause(isPause);
//}

