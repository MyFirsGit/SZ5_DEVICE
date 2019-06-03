/**
@brief	站员主框架窗口
*/

#include "stdafx.h"
#include "BOM.h"
#include "MainFrm.h"
#include "SysException.h"
#include "ExceptionMgr.h"
#include "DebugLog.h"
#include "coreapi.h"
#include "bominfo.h"
#include "servicemgr.h"
#include "InnerException.h"
#include "MessageIDDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WINDOWTEXT = _T("操作员");

IMPLEMENT_DYNAMIC(CMainFrm, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrm, CFrameWnd)
	ON_WM_CREATE()
    ON_MESSAGE(WM_START_SERVICE, OnStartService)
    ON_MESSAGE(WM_STOP_SERVICE, OnStopService)
    ON_MESSAGE(SP_MODE_CHANGE, OnModeChange)
	ON_MESSAGE(SP_CONVERT,OnServiceMessage)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMainFrm::CMainFrm()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMainFrm::~CMainFrm()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      窗口创建响应函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // 删除菜单
	CMenu* pMenu = GetMenu();

	if (pMenu != NULL) {
		pMenu->DestroyMenu();
		SetMenu(NULL);
	}

	// 去除窗口标题条和边框
  	ModifyStyle(WS_OVERLAPPEDWINDOW | WS_BORDER, NULL); 

    // 去除客户区，在任务条上不表示
    if (theBOM_INFO.IsTopmost()) {
        ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_TOOLWINDOW);
    }
    else {
        ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);
    }

#ifdef _DEBUG
	int iScrCx = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	::MoveWindow(this->m_hWnd, 0, 0, CXSCREEN, CYSCREEN, TRUE);
	//::MoveWindow(this->m_hWnd, iScrCx - CXSCREEN, 0, CXSCREEN, CYSCREEN, TRUE);
#else
	::MoveWindow(this->m_hWnd, 0, 0, CXSCREEN, CYSCREEN, TRUE);
#endif
    

    ::SetWindowText(this->m_hWnd, WINDOWTEXT);

    // 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, CXSCREEN, CYSCREEN), this, AFX_IDW_PANE_FIRST, NULL)) {
		TRACE0("Failed to create view window\n");
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Start a Service

@param      (i)WPARAM wParam    Service ID
@param      (i)WPARAM lParam    not used

@retval     0

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LONG CMainFrm::OnStartService(WPARAM wParam, LPARAM lParam)
{
    try {
        theSERVICE_MGR.StartService((SERVICE_ID)wParam, false);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Stop a Service

@param      (i)WPARAM wParam    Service ID
@param      (i)WPARAM lParam    not used

@retval     0

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LONG CMainFrm::OnStopService(WPARAM wParam, LPARAM lParam)
{
    try {
        theSERVICE_MGR.StopService((SERVICE_ID)wParam, false);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      模式变化

@param      (i)WPARAM wParam    模式类别
@param      (i)WPARAM lParam    not used

@retval     0

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LONG CMainFrm::OnModeChange(WPARAM wParam, LPARAM lParam)
{
    try {
        UINT mode = (int)wParam;
        theDEBUG_LOG->WriteData(_T("CMainFrm"), _T("OnModeChange"), __LINE__, _T("mode=%d"), mode);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转换服务消息

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainFrm::OnServiceMessage(WPARAM wParam,LPARAM lParam)
{
	ServiceMsg* lpMsg = (ServiceMsg*)lParam;
	ServiceMsg msg = *lpMsg;
	delete lpMsg;
	lpMsg = NULL;
	if(msg.pService!=NULL && msg.pService->IsForeground())
	{
		msg.pService->DispachCmdMsg(msg.message,msg.wParam,msg.lParam);
	}
	else
	{
		msg.pService->SendMessage(msg.message,msg.wParam,msg.lParam);
	}
	return 0;
}
