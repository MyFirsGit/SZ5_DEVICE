#include "stdafx.h"
#include "BOM.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "functiondlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CChildView,CWnd )
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
    m_pLogoDlg = new CLogoDlg;
	children.push_back(m_pLogoDlg);
	m_Left1_Dlg = new CFunctionDlg(IDD_LEFT_1_DLG,LEFT_1_FRAME_RECT,8,3,LEFT_1_GROUP_ID);
	children.push_back(m_Left1_Dlg);
	m_Left2_Dlg = new CFunctionDlg(IDD_LEFT_2_DLG,LEFT_2_FRAME_RECT,4,3,LEFT_2_GROUP_ID);
	children.push_back(m_Left2_Dlg);
	m_Right1_Dlg = new CFunctionDlg(IDD_RIGHT_1_DLG,RIGHT_1_FRAME_RECT,4,3,RIGHT_1_GROUP_ID);
	children.push_back(m_Right1_Dlg);
	m_Right2_Dlg = new CFunctionDlg(IDD_RIGHT_2_DLG,RIGHT_2_FRAME_RECT,3,3,RIGHT_2_GROUP_ID);
	children.push_back(m_Right2_Dlg);
	//m_Right3_Dlg = new CFunctionDlg(IDD_RIGHT_3_DLG,RIGHT_3_FRAME_RECT,4,3,RIGHT_3_GROUP_ID);
	//children.push_back(m_Right3_Dlg);

	m_Keyboard = new CKeyboard(0,KEYBOARD_FRAME_RECT,this);
	children.push_back(m_Keyboard);
	

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
	for(int i=0;i<children.size();i++)
	{
		CBaseDlg* child = children[i];
		delete child;
	}
	children.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Owner Draw����

@param      (i)CREATESTRUCT& cs

@retval     ��

@exception  BOOL    TRUE���ɼ����������ڣ�  FALSE����ֹ����
*/
//////////////////////////////////////////////////////////////////////////
BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
    if (!CWnd::PreCreateWindow(cs)) {
        return FALSE;
    }

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int     0:�����ɹ�    -1:����ʧ��

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

    m_pLogoDlg->Create(IDD_LOGO_DLG,this);
    m_pLogoDlg->ShowWindow(SW_SHOW);
	m_pLogoDlg->Invalidate(FALSE);
	m_pLogoDlg->UpdateWindow();

	m_Left1_Dlg->Create(IDD_LEFT_1_DLG,this);
	m_Left1_Dlg->ShowWindow(SW_SHOW);

	m_Left2_Dlg->Create(IDD_LEFT_2_DLG,this);
	m_Left2_Dlg->ShowWindow(SW_SHOW);

	m_Right1_Dlg->Create(IDD_RIGHT_1_DLG,this);
	m_Right1_Dlg->ShowWindow(SW_SHOW);

	m_Right2_Dlg->Create(IDD_RIGHT_2_DLG,this);
	m_Right2_Dlg->ShowWindow(SW_SHOW);

	//m_Right3_Dlg->Create(IDD_RIGHT_3_DLG,this);
	//m_Right3_Dlg->ShowWindow(SW_SHOW);

	m_Keyboard->Create(IDD_RIGHT_3_DLG,this);
	m_Keyboard->ShowWindow(SW_HIDE);
	SetWindowLong(m_Keyboard->m_hWnd,GWL_EXSTYLE,GetWindowLong(m_Keyboard->m_hWnd,GWL_EXSTYLE) | WS_EX_NOACTIVATE);
	m_Keyboard->BringWindowToTop();


	theGuideDlg.Create(this);
	theGuideDlg.ShowWindow(SW_HIDE);

	return 0;
}


LRESULT CChildView::CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowProc(message,wParam,lParam);
}