/******************************************************************************/
/**	@class CChildViewRec
*	@brief implementation file of CChildViewRec class.
*	@version 0.01 (2006-01-04): 
*	@remarks 顾客画面用视窗.
*/
/******************************************************************************/
// ChildViewRec.cpp : CChildViewRec 类的实现
//
#include "stdafx.h"
#include "TVM.h"
#include "ChildViewRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CChildViewRec, CWnd)
    ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CChildViewRec
CChildViewRec::CChildViewRec()
{
	END_SVC_UI ui(TRUE,_T(""),_T("暂停服务"),_T("Out of service "),_T("Fora de serviço"));
    m_pOutOfReceptionDlg = new COutOfReceptionDlg(NULL,ui);
    m_pReceptionHeadDlg = new CReceptionHeadDlg;
	m_pMenuDlg = new CMenuDlg;
}

CChildViewRec::~CChildViewRec()
{
    delete m_pOutOfReceptionDlg;
    m_pOutOfReceptionDlg = NULL;

    delete m_pReceptionHeadDlg;
    m_pReceptionHeadDlg = NULL;

	delete m_pMenuDlg;
	m_pMenuDlg = NULL;
}

// CChildViewRec 消息处理程序
/******************************************************************************/
/**	@brief Constructor, initialize data member.
*	@date 2006-01-04 (Wu Gang): Create.
*	@remarks 修改显示顾客画面视窗属性.
*/
/******************************************************************************/
BOOL CChildViewRec::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_OVERLAPPEDWINDOW;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/******************************************************************************/
/**	@brief Constructor, initialize data member.
*	@date 2006-01-04 (Wu Gang): Create.
*	@remarks 显示顾客画面对话框.
*/
/******************************************************************************/
int CChildViewRec::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

	// Create header
	m_pReceptionHeadDlg->Create(CReceptionHeadDlg::IDD, this);
	m_pReceptionHeadDlg->ShowWindow(SW_SHOW);
	m_pReceptionHeadDlg->UpdateWindow();

	m_pOutOfReceptionDlg->Create(COutOfReceptionDlg::IDD, this);
    m_pOutOfReceptionDlg->ShowWindow(SW_SHOW);
    m_pOutOfReceptionDlg->UpdateWindow();

	m_pMenuDlg->Create(COutOfReceptionDlg::IDD, this);
	m_pMenuDlg->ShowWindow(SW_SHOW);
	m_pMenuDlg->UpdateWindow();	
    return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
  @brief      设置Childview背景颜色

  @param      (i)CDC* pDC

  @retval     BOOL  TRUE:禁止绘制底色; FALSE:允许绘制底色

  @exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CChildViewRec::OnEraseBkgnd(CDC* pDC)
{
	//pDC->FillRect(&CRect(0, 0, 1024, 768), &CBrush(GRAY));

	return TRUE;
}

LRESULT CChildViewRec::CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowProc(message,wParam,lParam);
}