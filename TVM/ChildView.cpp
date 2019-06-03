#include "stdafx.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "TVM.h"
#include "functiondlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CChildView,CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CChildView::CChildView()
{
	m_pStatusDlg = new CStatusDlg;
	children.push_back(m_pStatusDlg);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CChildView::~CChildView()
{
	for(int i = 0; i < children.size(); ++i)
	{
		CBaseDlg* child = children[i];
		delete child;
	}
	children.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加 Owner Draw 属性

@param      (i)CREATESTRUCT& cs

@retval     无

@exception  BOOL  TRUE:可继续创建窗口  FALSE:终止创建
*/
//////////////////////////////////////////////////////////////////////////
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs)) return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_THICKFRAME;
	cs.style &= ~WS_OVERLAPPEDWINDOW;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_MENU), NULL);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Childview背景颜色

@param      (i)CDC* pDC

@retval     BOOL  TRUE:禁止绘制底色; FALSE:允许绘制底色

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	//pDC->FillRect(&CRect(0, 0, 1024, 768), &CBrush(GRAY));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:创建成功  -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	m_pStatusDlg->Create(IDD_STATUS_DLG,this);
	m_pStatusDlg->ShowWindow(SW_SHOW);
	m_pStatusDlg->Invalidate(FALSE);
	m_pStatusDlg->UpdateWindow();

	theGuideDlg.Create(this);
	theGuideDlg.ShowWindow(SW_HIDE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      窗口消息处理

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     int  0:创建成功  -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CChildView::CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowProc(message,wParam,lParam);
}