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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CChildView::CChildView()
{
	m_pStatusDlg = new CStatusDlg;
	children.push_back(m_pStatusDlg);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
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
@brief      ��� Owner Draw ����

@param      (i)CREATESTRUCT& cs

@retval     ��

@exception  BOOL  TRUE:�ɼ�����������  FALSE:��ֹ����
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
@brief      ����Childview������ɫ

@param      (i)CDC* pDC

@retval     BOOL  TRUE:��ֹ���Ƶ�ɫ; FALSE:������Ƶ�ɫ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	//pDC->FillRect(&CRect(0, 0, 1024, 768), &CBrush(GRAY));

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴��

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:�����ɹ�  -1:����ʧ��

@exception  ��
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
@brief      ������Ϣ����

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     int  0:�����ɹ�  -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CChildView::CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowProc(message,wParam,lParam);
}