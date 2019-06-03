#include "stdafx.h"
#include "RefundBaseOfficeDlg.h"
#include "RefundSvc.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CRect editRefund[] = {
	BASE_INFO_RECT(8,6),
	BASE_INFO_RECT(9,6),
};

IMPLEMENT_DYNAMIC(CRefundBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRefundBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnRefundReasonBtnClick)
	ON_GROUP_BUTTON_CLICK(RIGHT_2_GROUP_ID,OnSetRefundDespoitClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundBaseOfficeDlg::CRefundBaseOfficeDlg(CService* pService)
	: COperationDlg(CRefundBaseOfficeDlg::IDD, pService)
{
	// �˿���Ϣ��ʾ����
	InitRefundInfo();
	m_baseInfo->titleInfo.titleName = TITLE_REFUND_INFO;								// ����
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_9;								// ����
	m_baseInfo->detailInfo.labelGroup = m_RefundInfo;
	m_baseInfo->detailInfo.editGroup.cnt = 2;											// �༭����Ŀ
	m_baseInfo->detailInfo.editGroup.editRect = editRefund;							    // �༭������ 
 }

void CRefundBaseOfficeDlg::InitRefundInfo()
{
	int i = 0;
	// �˿�ԭ��
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_REFUND_REASON) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// ��Ʊ���
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// ��Ʊ����
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// ��Ч��
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// Ʊ�����
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// Ʊ��Ѻ��
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// �˿���
	m_RefundInfo[i].nameLeft = add_wsp(CString(TXT_REFUND_MONEY)+ CString(TXT_COLON),MAX_SBC_CASE_LEN);
	i++;
	// ��֤����ԱID
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("��֤����ԱID��")),MAX_SBC_CASE_LEN);
	i++;
	// ��֤����Ա����
	m_RefundInfo[i].nameLeft = add_wsp(CString(_T("��֤����Ա���룺")),MAX_SBC_CASE_LEN);
	i++;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRefundBaseOfficeDlg::~CRefundBaseOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:�����ɹ�    -1:����ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CRefundBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRefundSvc* pRefundSvc = (CRefundSvc*)GetService();
	try{
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA)){
			return -1;
		}
		// ����ԱID
		GetEdit(0)->SetLimitText(6);
		// ����Ա���룬����������е�������ʾΪ '*'
		GetEdit(1)->SetPasswordChar('*');
		GetEdit(1)->SetLimitText(6);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿�ԭ��ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundBaseOfficeDlg::OnRefundReasonBtnClick(WPARAM wParam,LPARAM lParam)
{
	CRefundSvc* pRefundApplySvc = (CRefundSvc*)GetService();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		pRefundApplySvc->GetDataModel().refundReason = btnInfo->btnID;
		pRefundApplySvc->NotifyDataModelChanged();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿�ԭ��ť

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n  TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundBaseOfficeDlg::OnSetRefundDespoitClick(WPARAM wParam,LPARAM lParam)
{
	CRefundSvc* pRefundApplySvc = (CRefundSvc*)GetService();
	try{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		pRefundApplySvc->GetDataModel().refundDeposit = btnInfo->btnID == 0 ? 0 : pRefundApplySvc->GetDataModel().deposit ;
		pRefundApplySvc->NotifyDataModelChanged();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundApplySvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ի����ʼ��

@param      none

@retval     BOOL  \n   TRUE:��ʼ���ɹ�  FALSE:��ʼ��ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRefundBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	CRefundSvc* pRefundSvc = (CRefundSvc*)GetService();
	if(pRefundSvc->GetDataModel().refundReason == 0x00){
		HideCompoment(EDIT_BOX);
	}
	else{
		m_btnOK->SetEnabled(false);
	}

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˿��Ʒѡ��ť�����ء�ȷ����ť��Ϣ����

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n     
            TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefundBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	GetService<CRefundSvc>()->OnOK();
	return __super::XButtonClick(wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRefundBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	m_vec_Right_1_BTN_INFO.clear();
	m_vec_Right_2_BTN_INFO.clear();
	CRefundSvc* pRefundSvc = GetService<CRefundSvc>();
	if (pRefundSvc->GetCurDialogGroupID() != CRefundSvc::DIALOG_GROUP_REFUND_NOT && pRefundSvc->GetCurDialogGroupID() != CRefundSvc::DIALOG_GROUP_FINISH)
	{
		// ��ʼ���˿�ԭ��ť
		CString strRefundMsg[] = {_T("�˿�ԭ��"),_T("����ԭ��")};
		int strRefundNo[] = {0,1};
		m_vec_Right_1_BTN_INFO.clear();
		for (int i = 0;i < 2;i++){
			BTN_INFO pRefundBtnInfo;
			pRefundBtnInfo.btnID = strRefundNo[i];
			pRefundBtnInfo.text = strRefundMsg[i];
			m_vec_Right_1_BTN_INFO.push_back(pRefundBtnInfo);
		}
		m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
		m_Right_1_Group->SetSelectType(CButtonGroup::SIGLE);
		m_Right_1_Group->ClickButton(pRefundSvc->GetDataModel().refundReason == 0X00?0:1);
		if(pRefundSvc->GetDataModel().cardPHType == CPU_TICKET)
		{
			m_vec_Right_2_BTN_INFO.clear();
			CString strRefundDeposit[] = {_T("����Ѻ��"),_T("��Ѻ��")};
			for (int i = 0;i < 2;i++){
				BTN_INFO pRefundDepositBtnInfo;
				pRefundDepositBtnInfo.btnID = strRefundNo[i];
				pRefundDepositBtnInfo.text = strRefundDeposit[i];
				m_vec_Right_2_BTN_INFO.push_back(pRefundDepositBtnInfo);
			}
			m_Right_2_Group->SetButtonInfos(m_vec_Right_2_BTN_INFO);
			m_Right_2_Group->SetSelectType(CButtonGroup::SIGLE);
			m_Right_2_Group->ClickFirstAvailButton();

		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �༭����Ϣ�ı�ʱ�Ĵ���

@param      ��

@retval     bool true:��ȷ; false:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CRefundBaseOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	if(GetEditText(0).GetLength() != STAFFID_LEN || GetEditText(1).GetLength() != DEFAULT_PASSWORD_LEN)
	{
		m_btnOK->SetEnabled(false);
		return FALSE;
	}
	CRefundSvc* pRefundSvc = (CRefundSvc*)GetService();
	try{
		int nOperatorID = _ttoi(GetEditText(0));
		if(nOperatorID==_ttoi(theAPP_SESSION.GetUserInfo().sUserID))
		{
			m_btnOK->SetEnabled(false);
		}
		else
		{
			CString sPwd = GetEditText(1);
			if(theBOM_INFO.IsFounderStaffID(nOperatorID)){
				if(theBOM_INFO.GetFounderStaffPassword() == sPwd){
					m_btnOK->SetEnabled(true);
				}
				else{
					m_btnOK->SetEnabled(false);
				}
			}
			else if(pRefundSvc->CheckOperatorIdAndPwd(nOperatorID,sPwd) && theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID,SYS_FUNCTION_REFUND_BY_METRO)){
				m_btnOK->SetEnabled(true);
			}
			else{
				m_btnOK->SetEnabled(false);
			}
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...)	{
		theEXCEPTION_MGR.ProcessException(CInnerException(pRefundSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}


void CRefundBaseOfficeDlg::UpdateUI()
{
	CRefundSvc* pRefundSvc = (CRefundSvc*)GetService();
	CRefundSvc::Model& m_DataModel = pRefundSvc->GetDataModel();
	// �����˿�
	if (pRefundSvc->GetCurDialogGroupID() == CRefundSvc::DIALOG_GROUP_REFUND_NOT) {
		// �������	
		HideCompoment(BTN_OK | BASE_AREA | EDIT_BOX | KEYBOARD_AREA);
		m_Right_1_Group->ShowButtons(false);
		m_Right_2_Group->ShowButtons(false);
		theSERVICE_MGR.SetState(FINISH);
	}
	// �˿����
	else if(pRefundSvc->GetCurDialogGroupID() == CRefundSvc::DIALOG_GROUP_FINISH)
	{
		// �������	
		HideCompoment(BTN_OK | KEYBOARD_AREA);
		m_Right_1_Group->ShowButtons(false);
		m_Right_2_Group->ShowButtons(false);
		GetEdit(0)->EnableWindow(FALSE);
		GetEdit(1)->EnableWindow(FALSE);
		theSERVICE_MGR.SetState(FINISH);
	}
	else
	{
		int i = 0;
		// �˿�ԭ��
		m_RefundInfo[i].nameRight = m_DataModel.refundReason == 0x00 ? _T("�˿�ԭ��") : _T("����ԭ��");
		i++;
		// ��Ʊ���
		m_RefundInfo[i].nameRight = m_DataModel.strTicketNum.IsEmpty() ? TXT_UNDEFINE : m_DataModel.strTicketNum;
		i++;
		// ��Ʊ����
		m_RefundInfo[i].nameRight = m_DataModel.strTicketType_CN.IsEmpty() ? TXT_UNDEFINE : m_DataModel.strTicketType_CN;
		i++;
		// ��Ч��
		m_RefundInfo[i].nameRight = m_DataModel.strValidDate.IsEmpty() ? TXT_UNDEFINE : m_DataModel.strValidDate;
		i++;
		// Ʊ�����
		if(m_DataModel.cardCategory == PURSE){
			m_RefundInfo[i].nameRight = ComFormatAmount(m_DataModel.ticketBalance) + MONEY_UNIT;
		}
		else if(m_DataModel.cardCategory == MULTIRIDE){
			m_RefundInfo[i].nameRight = ComFormatCount(m_DataModel.ticketBalance) + TIMES_UNIT;
		}
		else if(m_DataModel.cardCategory == PERIOD){
			m_RefundInfo[i].nameRight = TXT_UNDEFINE;
		}
		i++;
		// Ѻ��
		m_RefundInfo[i].nameRight = ComFormatAmount(m_DataModel.deposit) + MONEY_UNIT;
		i++;
		// �˿���
		m_RefundInfo[i].nameRight = ComFormatAmount(m_DataModel.refundValue + m_DataModel.refundDeposit) + MONEY_UNIT;
		i++;
		m_baseInfo->detailInfo.row = m_DataModel.refundReason == 0x00? LABEL_GROUP_EDIT_ROWS_7:LABEL_GROUP_EDIT_ROWS_9;
		
		GetEdit(0)->ShowWindow(m_DataModel.refundReason == 0x00 ? SW_HIDE : SW_SHOW);
		GetEdit(1)->ShowWindow(m_DataModel.refundReason == 0x00 ? SW_HIDE : SW_SHOW);
		if(m_DataModel.refundReason == 0x00)
		{
			m_btnOK->SetEnabled(true);
		}
		else
		{
			int nOperatorID = _ttoi(GetEditText(0));
			CString sPwd = GetEditText(1);
			if(theBOM_INFO.IsFounderStaffID(nOperatorID))
			{
				if(theBOM_INFO.GetFounderStaffPassword() == sPwd){
					m_btnOK->SetEnabled(true);
				}
				else{
					m_btnOK->SetEnabled(false);
				}
			}
			else if(pRefundSvc->CheckOperatorIdAndPwd(nOperatorID,sPwd) && theAFC_ACCESS_LEVEL.IsStaffHasRight(nOperatorID,SYS_FUNCTION_REFUND_BY_METRO)){
				m_btnOK->SetEnabled(true);
			}
			else{
				m_btnOK->SetEnabled(false);
			}
		}
		
		this->m_baseInfoArea->Invalidate();
		this->Invalidate();

	}
	
}