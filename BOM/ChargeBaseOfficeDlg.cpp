#include "stdafx.h"
#include "ChargeSvc.h"
#include "ChargeBaseOfficeDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//��ֵ��Ϣ��ʾ�༭���������
static CRect editCharge[] = {
    BASE_INFO_RECT(6,7),
	BASE_INFO_RECT(7,7),
};


static const CString PRICE_MSG[] = {_T("50Ԫ"),_T("100Ԫ"),_T("150Ԫ"),_T("200Ԫ"),_T("250Ԫ"),_T("300Ԫ")};
static const CString PRICE_VALUE[] = {_T("5000"),_T("10000"),_T("15000"),_T("20000"),_T("25000"),_T("30000")};
static const int IDX_CHARGE_VALUE = 0; //��ֵ�����ֵ����
static const int IDX_AMOUNT = 1;		//���ʱ���
static const int IDX_PAID = 2;

IMPLEMENT_DYNAMIC(CChargeBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CChargeBaseOfficeDlg, COperationDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
    ON_BTN_OK_CLICK(OnOK)
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnPriceBtnClick)
	ON_MESSAGE(WM_EDIT_CHANGE,OnEditChange)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeBaseOfficeDlg::CChargeBaseOfficeDlg(CService* pService)
	: COperationDlg(CChargeBaseOfficeDlg::IDD,pService)
{
	InitBaseInfo();
    // ��ֵ��Ϣ��ʾ����
    m_baseInfo->titleInfo.titleName = TITLE_CHARGE_INFO;                    // ����
    m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_7;           // ����
    m_baseInfo->detailInfo.labelGroup = m_ChargeInfo;        // ��ǩ���֣�2�У�
    m_baseInfo->detailInfo.editGroup.cnt = 2;                               // �༭����Ŀ
    m_baseInfo->detailInfo.editGroup.editRect = editCharge;                 // �༭������ 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeBaseOfficeDlg::~CChargeBaseOfficeDlg()
{
}

void CChargeBaseOfficeDlg::InitBaseInfo()
{
	// Ʊ�濨��
	m_ChargeInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ��Ʊ����
	m_ChargeInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ��Ч��
	m_ChargeInfo[2].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// Ѻ��
	m_ChargeInfo[3].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// ������� 	
	m_ChargeInfo[4].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_ChargeInfo[4].nameRight = add_hsp( MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// ��ֵ��Ĭ�ϣ�/��ֵ����
	m_ChargeInfo[5].nameLeft = add_wsp(CString(TXT_CHARGE_AMOUNT) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_ChargeInfo[5].nameRight =add_hsp( MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// �˴ε��ۣ����أ�
	m_ChargeInfo[6].nameLeft = "";
	m_ChargeInfo[6].nameRight = "";

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
int CChargeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
    try{
        if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA  | BTN_OK | BASE_AREA | CASH_AREA)){
            return -1;
        }

		GetEdit(IDX_CHARGE_VALUE)->SetScope(CGEdit::CASH);
		GetEdit(IDX_AMOUNT)->SetScope(CGEdit::CASH);

        GetEdit(IDX_CHARGE_VALUE)->SetLimitText(7);
		GetEdit(IDX_AMOUNT)->SetLimitText(7);

		GetEdit(IDX_AMOUNT)->ShowWindow(SW_HIDE);
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(CHARGE_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ok��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ����ʾ��ť��ID��
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
            TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CChargeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
    try {				
		// ��ֵ�������Ϸ��Լ��
		if (!ValidateInput()){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_INPUT_ERROR));
			return FALSE;
		}
		CChargeSvc::Model & dataModel = pChargeSvc->GetDataModel();
		// ��ֵ��Ϣȷ��
		CString msg("");
		if(dataModel.ticketCategory == PURSE){
			msg.Format(_T("��ֵ��%sԪ���Ƿ�ȷ��?"),ComFormatAmount(dataModel.due));
		}
		if(dataModel.ticketCategory == MULTIRIDE){
			msg.Format(_T("��ֵ������%s��Ӧ�ս��%sԪ���Ƿ�ȷ��"),ComFormatCount(dataModel.chargeValue),ComFormatAmount(dataModel.due));
		}
		INT_PTR confirmResult = CMessageBox::Show(TITLE_CHARGE_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(confirmResult == IDCANCEL)
		{
			return FALSE;
		}
		// ��ʾ��ֵ�ȴ�������	
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_WAIT));
		// ��ֵ��δ���
		theSERVICE_MGR.SetState(ISSUEING);	
		int result = pChargeSvc->DoCardCharge();
		if (result == RW_RSP_CHARGE_OVERTIME) // ��ʱ������
		{
			bool bcheck =  true;
			do{					
				//  ����ͬһ�ſ�
				if (result == RW_RSP_CHARGE_DIFF_CARD){
					CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pChargeSvc->GetServiceID()), theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_NOT_SAME_CARD_PASSENGER),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
				} 
				else{
					if(bcheck == false){
						break;
					}
				}
				INT_PTR confirmResult = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pChargeSvc->GetServiceID()), _T("�뽫��Ƭ�Żغ�ȷ�ϼ�,��ȡ����ֱ�����ɽ��ף�"),CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_600);
				bcheck = confirmResult == CMessageBox::BTN_OK;
				result = pChargeSvc->ChargeOverTime(bcheck);
			}while(result != RW_RSP_OK);					
		}
		// ��ʾTPU������Ϣ
		if(result != RW_RSP_OK){
			pChargeSvc->OnError(pChargeSvc->GetTpuErrorInfo(result));
			return FALSE;
		}					
		pChargeSvc->DoDialogFlow(WM_TO_CHARGE_FINISH_DLG);
					
	}
	catch (CSysException e) {
		pChargeSvc->OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(CHARGE_SVC, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		pChargeSvc->OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
    return __super::XButtonClick(wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeBaseOfficeDlg::ValidateInput()
{
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	CChargeSvc::Model & dataModel = pChargeSvc->GetDataModel();
	if(dataModel.chargeValue<=0)
	{
		SetEditFocus(IDX_CHARGE_VALUE);
		return false;
	}
	if(dataModel.due<=0)
	{
		SetEditFocus(IDX_AMOUNT);
		return false;
	}
	if(GetEditText(IDX_PAID)=="")
	{
		SetEditText(IDX_PAID,ComFormatAmount(dataModel.due));
		dataModel.paid = dataModel.due;
	}
	if(dataModel.due > dataModel.paid)
	{
		GetEdit(IDX_PAID)->SetFocus();
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �༭����Ϣ�ı�ʱ�Ĵ���

@param      ��

@retval     bool true:��ȷ; false:����ȷ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CChargeBaseOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	CChargeSvc::Model& dataModel = pChargeSvc->GetDataModel();
	dataModel.chargeValue = dataModel.ticketCategory == PURSE ? ComGetAmount(GetEditText(IDX_CHARGE_VALUE)) : (ComGetAmount(GetEditText(IDX_CHARGE_VALUE))/100);
	dataModel.due = dataModel.ticketCategory == PURSE ? ComGetAmount(GetEditText(IDX_CHARGE_VALUE)) : ComGetAmount(GetEditText(IDX_AMOUNT));
	dataModel.paid = ComGetAmount(GetEditText(IDX_PAID));
	pChargeSvc->NotifyDataModelChanged();
	return TRUE;
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
void CChargeBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	try{	
		if (pChargeSvc->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_CHARGE_NOT) {
			HideCompoment(BTN_RESET | BTN_OK | BASE_AREA | CASH_AREA  | KEYBOARD_AREA);
			m_Right_1_Group->ShowButtons(false);
			theSERVICE_MGR.SetState(FINISH);
			return;
		}
		else if(pChargeSvc->GetCurDialogGroupID()==CChargeSvc::DIALOG_GROUP_FINISH)
		{
			HideCompoment(BTN_RESET | BTN_OK   | KEYBOARD_AREA);
			m_Right_1_Group->ShowButtons(false);
			GetEdit(IDX_CHARGE_VALUE)->EnableWindow(FALSE);
			GetEdit(IDX_AMOUNT)->EnableWindow(FALSE);
			GetEdit(IDX_PAID)->EnableWindow(FALSE);
			theSERVICE_MGR.SetState(FINISH);
			return;
		}
		CChargeSvc::Model & dataModel = pChargeSvc->GetDataModel();
		GetEdit(IDX_CHARGE_VALUE)->SetScope(dataModel.ticketCategory == MULTIRIDE ? CGEdit::DIGIT_CHAR : CGEdit::CASH);
		GetEdit(IDX_AMOUNT)->ShowWindow(dataModel.ticketCategory == MULTIRIDE ? SW_SHOW : SW_HIDE);

	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pChargeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		�Ի����ʼ��

@param      ��

@retval     BOOL  \n   TRUE:��ʼ���ɹ�  FALSE:��ʼ��ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CChargeBaseOfficeDlg::OnInitDialog()
{
	COperationDlg::OnInitDialog();

	m_vec_Right_1_BTN_INFO.clear();

	for (int m = 0;m < 6;m++){
		BTN_INFO pPriceBtnInfo;
		pPriceBtnInfo.btnMsg = PRICE_VALUE[m];
		pPriceBtnInfo.text = PRICE_MSG[m];
		m_vec_Right_1_BTN_INFO.push_back(pPriceBtnInfo);
	}

	m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ֵ��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n   TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CChargeBaseOfficeDlg::OnPriceBtnClick(WPARAM wParam,LPARAM lParam)
{
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	DWORD price = _ttoi(btnInfo->btnMsg);
	SetEditText(IDX_CHARGE_VALUE,ComFormatAmount(price));
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	CChargeSvc::Model& dataModel = pChargeSvc->GetDataModel();
	dataModel.chargeValue = dataModel.ticketCategory == PURSE ? ComGetAmount(GetEditText(IDX_CHARGE_VALUE)) : (ComGetAmount(GetEditText(0))/100);
	dataModel.due = dataModel.ticketCategory == PURSE ? ComGetAmount(GetEditText(IDX_CHARGE_VALUE)) : ComGetAmount(GetEditText(IDX_AMOUNT));
	pChargeSvc->NotifyDataModelChanged();
	return TRUE;
}

void CChargeBaseOfficeDlg::UpdateUI()
{
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
	CChargeSvc::Model & dataModel = pChargeSvc->GetDataModel();
	// Ʊ�濨��
	m_ChargeInfo[0].nameRight = dataModel.strTicketNumber;
	// ��Ʊ����
	m_ChargeInfo[1].nameRight = dataModel.strTicketName;
	// ��Ч��
	m_ChargeInfo[2].nameRight = dataModel.validDate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
	// Ѻ��
	m_ChargeInfo[3].nameRight = ComFormatAmount(dataModel.deposit);
	// ������� 	
	m_ChargeInfo[4].nameRight = dataModel.ticketCategory == PURSE ? (ComFormatAmount(dataModel.balance) + MONEY_UNIT):(ComFormatCount(dataModel.balance) + TIMES_UNIT);
	// ��ֵ��Ĭ�ϣ�/��ֵ����
	if(dataModel.ticketCategory == MULTIRIDE)
	{
		m_ChargeInfo[5].nameLeft = TXT_CHARGE_RIDE;
		m_ChargeInfo[6].nameLeft = TXT_CHARGE_PRICE_TIME;
	}
	else
	{
		// �˴ε��ۣ����أ�
		m_ChargeInfo[6].nameLeft = "";
		m_ChargeInfo[6].nameRight = "";
	}
	
	m_cashInfo->detailInfo.labelGroup[0].nameRight = ComFormatAmount(dataModel.due) + MONEY_UNIT;
	m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(dataModel.change) + MONEY_UNIT;

	m_baseInfoArea->Invalidate();
	m_cashInfoArea->Invalidate();
}