/**
@brief	�˿�����ܴ���
*/
#include "stdafx.h"
#include "TVM.h"
#include "MainFrmRec.h"
#include "SYSInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrmRec
static const TCHAR* const WINDOWTEXT = _T("Guest");

IMPLEMENT_DYNAMIC(CMainFrmRec, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrmRec, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_START_SERVICE, OnStartService)
	ON_MESSAGE(WM_STOP_SERVICE, OnStopService)
	ON_MESSAGE(SP_CONVERT,OnServiceMessage)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMainFrmRec::CMainFrmRec()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMainFrmRec::~CMainFrmRec()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CMainFrmRec::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
   if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // ɾ���˵�
	CMenu* pMenu = GetMenu();

	if (pMenu != NULL) {
		pMenu->DestroyMenu();
		SetMenu(NULL);
	}

    // ȥ�����ڱ������ͱ߿�
  	ModifyStyle(WS_OVERLAPPEDWINDOW | WS_BORDER, NULL); 

    // ȥ���ͻ��������������ϲ���ʾ
    if (theTVM_INFO.IsTopmost()) {
        ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_TOOLWINDOW);
    }
    else {
        ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);    
    }

#ifndef _DEBUG
	::MoveWindow(this->m_hWnd,0, 0, CXCLIENTSCREEN*REC_RATIOWIDTH, CYCLIENTSCREEN*REC_RATIOHEIGHT, TRUE);
	//::MoveWindow(this->m_hWnd, CXSCREEN, 0, CXSCREEN, CYSCREEN, TRUE);
	//::MoveWindow(this->m_hWnd, 0, 0, CXSCREEN, CYSCREEN, TRUE);
#else
	::MoveWindow(this->m_hWnd,0, 0, CXCLIENTSCREEN*REC_RATIOWIDTH, CYCLIENTSCREEN*REC_RATIOHEIGHT, TRUE);
	//::MoveWindow(this->m_hWnd, CXSCREEN, 0, CXSCREEN, CYSCREEN, TRUE);
	//::MoveWindow(this->m_hWnd,0,0,CXSCREEN*2, CYSCREEN, TRUE);
#endif

    ::SetWindowText(this->m_hWnd, WINDOWTEXT);

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		    CRect(0, 0, CXCLIENTSCREEN*REC_RATIOWIDTH, CYCLIENTSCREEN*REC_RATIOHEIGHT), this, AFX_IDW_PANE_FIRST, NULL))
    {
		TRACE0("δ�ܴ�����ͼ����\n");
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
LONG CMainFrmRec::OnStartService(WPARAM wParam, LPARAM lParam)
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
LONG CMainFrmRec::OnStopService(WPARAM wParam, LPARAM lParam)
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
/*
@brief      ת��������Ϣ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMainFrmRec::OnServiceMessage(WPARAM wParam,LPARAM lParam)
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
