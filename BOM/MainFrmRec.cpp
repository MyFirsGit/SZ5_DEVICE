/**
@brief	乘客主框架窗口
*/
#include "stdafx.h"
#include "BOM.h"
#include "MainFrmRec.h"
#include "bominfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrmRec
static const TCHAR* const WINDOWTEXT = _T("顾客");

IMPLEMENT_DYNAMIC(CMainFrmRec, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrmRec, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMainFrmRec::CMainFrmRec()
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
CMainFrmRec::~CMainFrmRec()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CMainFrmRec::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

    // 取得当前显示器的大小
    //int iScrCx = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
#ifndef _DEBUG
	//::MoveWindow(this->m_hWnd, 0, 0, CXSCREENREC, CYSCREENREC, TRUE);
	::MoveWindow(this->m_hWnd, CXSCREEN, 0, CXSCREENREC, CYSCREENREC, TRUE);
#else
	::MoveWindow(this->m_hWnd, CXSCREEN, 0, CXSCREENREC, CYSCREENREC, TRUE);
	//::MoveWindow(this->m_hWnd,0,0,CXSCREENREC*2, CYSCREENREC, TRUE);
#endif
    

    ::SetWindowText(this->m_hWnd, WINDOWTEXT);

	// 创建一个视图以占用框架的工作区
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		    CRect(0, 0, CXSCREENREC, CYSCREENREC), this, AFX_IDW_PANE_FIRST, NULL))
    {
		TRACE0("未能创建视图窗口\n");
		return -1;
	}

	return 0;
}
