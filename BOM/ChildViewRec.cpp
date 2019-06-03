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
#include "BOM.h"
#include "ChildViewRec.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CChildViewRec, CWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CChildViewRec
CChildViewRec::CChildViewRec()
{
    m_pOutOfReceptionDlg = new COutOfReceptionDlg;
    m_pReceptionHeadDlg = new CReceptionHeadDlg;
    m_pReceptionGuideDlg = new CGuideReceptionDlg;
}

CChildViewRec::~CChildViewRec()
{
    delete m_pOutOfReceptionDlg;
    m_pOutOfReceptionDlg = NULL;

    delete m_pReceptionHeadDlg;
    m_pReceptionHeadDlg = NULL;

    delete m_pReceptionGuideDlg;
    m_pReceptionGuideDlg = NULL;
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


	m_pReceptionGuideDlg->Create(CGuideReceptionDlg::IDD, this);
	m_pReceptionGuideDlg->ShowWindow(SW_SHOW);
	m_pReceptionGuideDlg->UpdateWindow();

    m_pOutOfReceptionDlg->Create(COutOfReceptionDlg::IDD, this);
    m_pOutOfReceptionDlg->ShowWindow(SW_SHOW);
    m_pOutOfReceptionDlg->UpdateWindow();

    return 0;
}
