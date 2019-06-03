#include "StdAfx.h"
#include "OperationDlg.h"
#include "ForeService.h"
#include "bitmapmanager.h"
#include "InnerException.h"
#include "ExceptionMgr.h"
#include "AppSession.h"
#include "GuideDlg.h"
#include "MessageIDDefinition.h"
#include "FunctionDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//�ֽ���Ϣ��ʾ�༭���������
static CRect editCash[] = {
	CASH_INFO_RECT,
};

IMPLEMENT_DYNAMIC(COperationDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(COperationDlg, CBaseDlg)
	ON_WM_ACTIVATE()
    ON_MESSAGE(WM_XBUTTON, XButtonClick)
    ON_MESSAGE(WM_EDIT_SETFOCUSE, OnEditSetFocus)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

 //@brief      ���캯��

 //@param      (i)UINT ID             �Ի���ID
 //@param      (i)CService* pService  �Ի���ʹ�õ�Service

 //@retval     ��

 //@exception  ��

//////////////////////////////////////////////////////////////////////////
COperationDlg::COperationDlg(UINT ID, CService* pService,CRect rect,bool needFunctionArea)
	: CBaseDlg(ID, theAPP_SESSION.GetChildView(), rect, GRAY, pService)
{
	this->needFunctionArea = needFunctionArea;
    Initialize();
}

//////////////////////////////////////////////////////////////////////////

//@brief      ���캯��
//
//@param      (i)UINT ID             �Ի���ID
//@param      (i)CRect position       λ�ô�С
//@param      (i)COLORREF bkColor     ����ɫ
//@param      (i)CService* pService   �Ի���ʹ�õ�Service
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
COperationDlg::COperationDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea)
: CBaseDlg(ID, theAPP_SESSION.GetChildView(), position, bkColor, pService)
{
	this->needFunctionArea = needFunctionArea;
    Initialize();
}

//////////////////////////////////////////////////////////////////////////

//@brief      ���ڻ�
//
//@param      ��
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::Initialize()
{
	m_baseInfoArea = NULL;
	m_cashInfoArea = NULL;
	//������Ϣ
	m_baseInfo = new GSTATIC_INFO;
	m_baseInfo->point = LEFTUP_AREA_POINT;
	m_baseInfo->titleInfo.titleName = TITLE_TICKET_INFO;
	m_baseInfo->titleInfo.color = BLACK;
	m_baseInfo->detailInfo.color = BLACK;
	m_baseInfo->detailInfo.row = 7;
	m_baseInfo->detailInfo.editGroup.cnt = 0;                   // �༭�����
	m_baseInfo->detailInfo.editGroup.isLeftAlign = true;
	//�ֽ��ֽ���Ϣ
	m_cashInfo = new GSTATIC_INFO;
	m_cashInfo->point = LEFTDOWN_AREA_POINT;
	m_cashInfo->titleInfo.titleName = TITLE_CASH_INFO;
	m_cashInfo->titleInfo.color = BLACK;
	m_cashInfo->detailInfo.color = BLACK;
	m_cashInfo->detailInfo.row = 3;
	m_cashInfo->detailInfo.editGroup.cnt = 1;                   // �༭�����
	m_cashInfo->detailInfo.editGroup.editRect = editCash;
	m_cashInfo->detailInfo.labelGroup = m_cashLabelGroup;
	m_cashLabelGroup[0].nameLeft = add_wsp(CString(TXT_TOTAL)+TXT_COLON,MAX_SBC_CASE_LEN);
	m_cashLabelGroup[0].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[1].nameLeft = add_wsp(CString(TXT_INCOME)+TXT_COLON,MAX_SBC_CASE_LEN);
	m_cashLabelGroup[1].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	m_cashLabelGroup[2].nameLeft = add_wsp(CString(TXT_CHANGE)+TXT_COLON,MAX_SBC_CASE_LEN);
	m_cashLabelGroup[2].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	m_cashInfo->detailInfo.editGroup.isLeftAlign = true;
	//��������
	m_Left_1_Group = theAPP_SESSION.GetChildView()->m_Left1_Dlg->pButtonGroup;
	m_Left_2_Group = theAPP_SESSION.GetChildView()->m_Left2_Dlg->pButtonGroup;
	m_Right_1_Group = theAPP_SESSION.GetChildView()->m_Right1_Dlg->pButtonGroup;
	m_Right_2_Group = theAPP_SESSION.GetChildView()->m_Right2_Dlg->pButtonGroup;
	m_Keyboard = theAPP_SESSION.GetChildView()->m_Keyboard;
	//������ť
	m_btnOK = BuildOperationButton(BTN_OK_RECT, STR_OK,WM_BTN_OK);
	m_btnBack = BuildOperationButton(BTN_BACK_RECT,STR_BACK,WM_BTN_BACK);
	m_btnPrint = BuildOperationButton(BTN_PRINT_RECT,STR_PRINT,WM_BTN_PRINT);
	m_btnReset = BuildOperationButton(BTN_RESET_RECT,STR_RESET,WM_BTN_RESET);
}


//////////////////////////////////////////////////////////////////////////
//
//@brief      ��������
//
//@param      ��
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
COperationDlg::~COperationDlg(void)
{
	delete m_btnOK;
	m_btnOK = NULL;
	delete m_btnBack;
	m_btnBack = NULL;
	delete m_btnPrint;
	m_btnPrint = NULL;
	delete m_btnReset;
	m_btnReset = NULL;
	// ������Ϣ���ֽ���Ϣ����
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	delete m_cashInfoArea;
	m_cashInfoArea = NULL;
	delete m_baseInfo;
	m_baseInfo = NULL;
	delete m_cashInfo;
	m_cashInfo = NULL;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CXButton* COperationDlg::BuildOperationButton(CRect rect,CString text,CString msg)
{
	UIINFO ui;
	ui.m_pParentWnd = this;
	ui.m_DockStyle = DOCKSTYLE::NONE;
	ui.m_Location= rect.TopLeft();
	ui.m_Size = rect.Size();
	ui.m_Visible = true;
	BTN_INFO btnInfo;
	btnInfo.text = text;
	btnInfo.btnMsg = msg;
	btnInfo.isVisible = true;
	btnInfo.type = BUTTON_TYPE_NOSELECT;
	return new CXButton(ui,btnInfo);
}

//////////////////////////////////////////////////////////////////////////

 //@brief      ���洴������

 //@param      (i)LPCREATESTRUCT lpCreateStruct  
 //@param      (i)ulong ulCompomentFlag           ���������־

 //@retval     int    \n
	//		 0:�����ɹ�    -1:����ʧ��

 //@exception  ��

//////////////////////////////////////////////////////////////////////////
int COperationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag)
{
    try {
		theGuideDlg.ShowWindow(ulCompomentFlag & GUIDE_AREA ? SW_SHOW : SW_HIDE);
	    if (__super::OnCreate(lpCreateStruct) == -1) {
		    return -1;
	    }
		if(ulCompomentFlag & BASE_AREA)
		{
			m_baseInfoArea = new CGStatic(this, m_baseInfo);
		}
		if(ulCompomentFlag &CASH_AREA )
		{
			 m_cashInfoArea = new CGStatic(this, m_cashInfo);
			 if(m_cashInfo->detailInfo.editGroup.cnt>0)
			 {
				 ((CGEdit*) m_cashInfoArea->GetEdit(0))->SetScope(CGEdit::CASH);
				 ((CGEdit*) m_cashInfoArea->GetEdit(0))->SetLimitText(7);
			 }
		}
		if(needFunctionArea)
		{
			m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
			m_Left_1_Group->ShowButtons(true);
			m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
			m_Left_2_Group->ShowButtons(true);
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
			m_Right_1_Group->ShowButtons(true);
			m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
			m_Right_2_Group->ShowButtons(true);
		}
		theAPP_SESSION.GetChildView()->m_Left1_Dlg->ShowWindow(needFunctionArea ? SW_SHOW : SW_HIDE);
		theAPP_SESSION.GetChildView()->m_Left2_Dlg->ShowWindow(needFunctionArea ? SW_SHOW : SW_HIDE);
		theAPP_SESSION.GetChildView()->m_Right1_Dlg->ShowWindow(needFunctionArea ? SW_SHOW : SW_HIDE);
		theAPP_SESSION.GetChildView()->m_Right2_Dlg->ShowWindow(needFunctionArea ? SW_SHOW : SW_HIDE);
		theAPP_SESSION.GetChildView()->m_Keyboard->ShowWindow(needFunctionArea ? SW_SHOW : SW_HIDE);

		
		if (ulCompomentFlag & BTN_OK) m_btnOK->Create();
		if (ulCompomentFlag & BTN_BACK) m_btnBack->Create();
		if (ulCompomentFlag & BTN_PRINT) m_btnPrint->Create();
		if(ulCompomentFlag & BTN_RESET) m_btnReset->Create();

        // �༭���б�
        if (m_baseInfoArea != NULL && m_baseInfo->detailInfo.editGroup.cnt > 0) {  // ���������������б༭��
            for (int i = 0; i < m_baseInfo->detailInfo.editGroup.cnt; i++) {
                m_editArray.push_back(m_baseInfoArea->GetEdit(i));
            }
        }
        if (m_cashInfoArea != NULL && m_cashInfo->detailInfo.editGroup.cnt > 0) {  // �ֽ������������б༭��
            for (int i = 0; i < m_cashInfo->detailInfo.editGroup.cnt; i++) {
                m_editArray.push_back(m_cashInfoArea->GetEdit(i));
            }
        }
		if (m_editArray.size() > 0)
		{
			// �������뽹�㵽�༭��
			SetEditFocus(0);
		}
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	theAPP_SESSION.GetChildView()->m_Keyboard->ShowKeys(m_editArray.size()>0 );
	if(needFunctionArea)
	{
		m_Left_1_Group->SetButtonInfos(m_vec_Left_1_BTN_INFO);
		m_Left_2_Group->SetButtonInfos(m_vec_Left_2_BTN_INFO);
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
	}
	if(IsWindow(m_btnOK->m_hWnd))
	{
		m_btnOK->Invalidate();
		m_btnOK->SetFocus();
	}
	if(IsWindow(m_btnBack->m_hWnd))
	{
		m_btnBack->Invalidate();
	}
	if(IsWindow(m_btnPrint->m_hWnd))
	{
		m_btnPrint->Invalidate();
	}
	if(IsWindow(m_btnReset->m_hWnd))
	{
		m_btnReset->Invalidate();
	}
	m_Keyboard->ShowKeys(m_editArray.size()>0);
	SetEditFocus(m_curEditIndex);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PGSTATIC_INFO COperationDlg::GetBaseInfo()
{
	return m_baseInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PGSTATIC_INFO COperationDlg::GetCashInfo()
{
	return m_cashInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CGStatic* COperationDlg::GetBaseArea()
{
	return m_baseInfoArea;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CGStatic* COperationDlg::GetCashArea()
{
	return m_cashInfoArea;
}

//////////////////////////////////////////////////////////////////////////

 //@brief      ������ƺ���

 //@param      (i) CDC* pDC  �豸������

 //@retval     ��

 //@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnDraw(CDC* pDC)
{
    // �������뽹�㵽�༭��
    SetEditFocus(m_curEditIndex);
}

//////////////////////////////////////////////////////////////////////////

 //@brief      ��ť���º�Ĵ���

 //@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
 //@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

 //@retval     LRESULT  \n
 //            TRUE:����ɹ�  FALSE:����ʧ��

 //@exception  ��

//////////////////////////////////////////////////////////////////////////
LRESULT COperationDlg::XButtonClick(WPARAM wParam, LPARAM lParam)
{
    try {
        // ������ת
        if (GetService() != NULL) {
            ((CForeService*)GetService())->DoDialogFlow((LPCTSTR)lParam);
        }
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////

//@brief      �༭�����ݸı����Ϣ��Ӧ����
//
//@param      (i)WPARAM wParam  ��Ϣ��Ϣ������༭��
//@param      (i)LPARAM lParam  ��������
//
//@retval     LRESULT  \n
//            TRUE:����ɹ�  FALSE:����ʧ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
LRESULT COperationDlg::OnEditSetFocus(WPARAM wParam, LPARAM lParam)
{
    CGEdit* pEdit = (CGEdit*)wParam;
    for (UINT i = 0; i < m_editArray.size(); i++) {
        if (pEdit == m_editArray[i]) {
			m_curEditIndex = i;
            break;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnServiceBusy(bool busy)
{
	if(busy)
	{
		bool resetEnable = m_btnReset->IsEnabled();
		m_btnReset->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,resetEnable);
		m_btnReset->SetEnabled(false);
		bool printEnable = m_btnPrint->IsEnabled();
		m_btnPrint->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,printEnable);
		m_btnPrint->SetEnabled(false);
		bool backEnable = m_btnBack->IsEnabled();
		m_btnBack->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,backEnable);
		m_btnBack->SetEnabled(false);
		bool okEnable = m_btnOK->IsEnabled();
		m_btnOK->GetBag()->SetValue(BUTTON_LAST_STATUS_KEY,okEnable);
		m_btnOK->SetEnabled(false);
		if(needFunctionArea)
		{
			m_Left_1_Group->EnableWindow(FALSE);
			m_Left_1_Group->Invalidate();
			m_Left_1_Group->UpdateWindow();
			m_Left_2_Group->EnableWindow(FALSE);
			m_Left_2_Group->Invalidate();
			m_Left_2_Group->UpdateWindow();
			m_Right_1_Group->EnableWindow(FALSE);
			m_Right_1_Group->Invalidate();
			m_Right_1_Group->UpdateWindow();
			m_Right_2_Group->EnableWindow(FALSE);
			m_Right_2_Group->Invalidate();
			m_Right_2_Group->UpdateWindow();
		}
		m_Keyboard->SetEnable(false);
	}
	else
	{
		bool resetEnable = true;
		m_btnReset->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,resetEnable);
		m_btnReset->SetEnabled(resetEnable);
		bool printEnable = true;
		m_btnPrint->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,printEnable);
		m_btnPrint->SetEnabled(printEnable);
		bool backEnable = true;
		m_btnBack->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,backEnable);
		m_btnBack->SetEnabled(backEnable);
		bool okEnable = true;
		m_btnOK->GetBag()->GetValue(BUTTON_LAST_STATUS_KEY,okEnable);
		m_btnOK->SetEnabled(okEnable);
		if(needFunctionArea)
		{
			m_Left_1_Group->EnableWindow(TRUE);
			m_Left_1_Group->Invalidate();
			m_Left_1_Group->UpdateWindow();
			m_Left_2_Group->EnableWindow(TRUE);
			m_Left_2_Group->Invalidate();
			m_Left_2_Group->UpdateWindow();
			m_Right_1_Group->EnableWindow(TRUE);
			m_Right_1_Group->Invalidate();
			m_Right_1_Group->UpdateWindow();
			m_Right_2_Group->EnableWindow(TRUE);
			m_Right_2_Group->Invalidate();
			m_Right_2_Group->UpdateWindow();
		}
		m_Keyboard->SetEnable(true);
	}
	
}

//////////////////////////////////////////////////////////////////////////

//@brief      ��ʾ���
//
//@param      (i)ulong ulCompomentFlag           �����־
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////

//@brief      �������
//
//@param      (i)ulong ulCompomentFlag           �����־
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////

//@brief      ��ʾ/�������
//
//@param      (i)ulong ulCompomentFlag           �����־
//@param      (i)int nCmdShow                    ����(SW_SHOW/SW_HIDE)
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
    // ȷ�ϰ�ť
    if (ulCompomentFlag & BTN_OK) {
        ShowHideCompoment(m_btnOK, nCmdShow);
    }
    // ���ذ�ť
    if (ulCompomentFlag & BTN_BACK) {
        ShowHideCompoment(m_btnBack, nCmdShow);
    }
    // ��ӡ��ť
    if (ulCompomentFlag & BTN_PRINT) {
        ShowHideCompoment(m_btnPrint, nCmdShow);
    }
	if(ulCompomentFlag & BTN_RESET)
	{
		ShowHideCompoment(m_btnReset,nCmdShow);
	}
    // ������������
    if (ulCompomentFlag & BASE_AREA) {
        ShowHideCompoment(m_baseInfoArea, nCmdShow);
    }
    // �ֽ���������
    if (ulCompomentFlag & CASH_AREA) {
        ShowHideCompoment(m_cashInfoArea, nCmdShow);
    }
	if(ulCompomentFlag & KEYBOARD_AREA)
	{
		m_Keyboard->ShowKeys(nCmdShow == SW_SHOW);
	}
    // �༭��
    if (ulCompomentFlag & EDIT_BOX) {
        for (UINT i = 0; i < m_editArray.size(); i++) {
            ShowHideCompoment(m_editArray[i], nCmdShow);
        }
        // �������뽹�㵽�༭��
        SetEditFocus(0);
    }
}

//////////////////////////////////////////////////////////////////////////

//@brief      ��ʾ/�������
//
//@param      (i)CWnd *pCompoment                �������
//@param      (i)int nCmdShow                    ����(SW_SHOW/SW_HIDE)
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowHideCompoment(CWnd *pCompoment, int nCmdShow)
{
    if (pCompoment == NULL) 
	{
		return;
	}
	CXButton* pButton = dynamic_cast<CXButton*>(pCompoment);
	if(pButton!=NULL)
	{
		pButton->SetVisible(nCmdShow == SW_SHOW ? true:false);
		return;
	}
	CRect rect;
    pCompoment->ShowWindow(nCmdShow);
    GetCompomentRect(pCompoment, &rect);
    InvalidateRect(&rect, TRUE);
    UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////

//@brief      ��ȡ������ڵ�����
//
//@param      (i)CWnd *pCompoment                �������
//@param      (o)CRect* lpRect                   ����
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);
    // ת��������ڵ�ǰOperationDlg��λ��
    ScreenToClient(lpRect);
    // ���ϱ߿�Ĵ�С
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);
}

//////////////////////////////////////////////////////////////////////////

//@brief      �������뽹�㵽�༭��
//
//@param      (i)int index                  �༭����
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::SetEditFocus(int index)
{
    if (index >= 0 && index < m_editArray.size()) {
        m_editArray[index]->SetFocus();
    }
}

//////////////////////////////////////////////////////////////////////////

//@brief      ���ñ༭�����ʾ����
//
//@param      (i)int        index    �༭����
//@param      (i)CString    txt      �༭������
//
//@retval     ��
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
void COperationDlg::SetEditText(int index, CString text)
{
	ASSERT(index >= 0 && index < m_editArray.size());
    m_editArray[index]->SetWindowText(text);
    SetEditFocus(m_curEditIndex);
}

//////////////////////////////////////////////////////////////////////////

//@brief      ��ȡ�༭�����ʾ����
//
//@param      (i)int        index    �༭����
//
//@retval     CString    �༭������
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CString COperationDlg::GetEditText(int index)
{
	ASSERT(index >= 0 && index < m_editArray.size());
    CString text;
    m_editArray[index]->GetWindowText(text);
    return text;
}

//////////////////////////////////////////////////////////////////////////

//@brief      ��ȡ�༭�����
//
//@param      (i)int        index    �༭����
//
//@retval     CGEdit*    �༭�����, index ��Чʱ���� NULL
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
CGEdit* COperationDlg::GetEdit(int index)
{
	ASSERT(index >= 0 && index < m_editArray.size());
	return m_editArray[index];
}
