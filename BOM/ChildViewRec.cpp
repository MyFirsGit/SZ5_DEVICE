/******************************************************************************/
/**	@class CChildViewRec
*	@brief implementation file of CChildViewRec class.
*	@version 0.01 (2006-01-04): 
*	@remarks �˿ͻ������Ӵ�.
*/
/******************************************************************************/
// ChildViewRec.cpp : CChildViewRec ���ʵ��
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

// CChildViewRec ��Ϣ�������
/******************************************************************************/
/**	@brief Constructor, initialize data member.
*	@date 2006-01-04 (Wu Gang): Create.
*	@remarks �޸���ʾ�˿ͻ����Ӵ�����.
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
*	@remarks ��ʾ�˿ͻ���Ի���.
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
