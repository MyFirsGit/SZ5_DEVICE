#include "stdafx.h"
#include "ChargeSvc.h"
#include "ChargeBaseOfficeDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//充值信息显示编辑框坐标组合
static CRect editCharge[] = {
    BASE_INFO_RECT(6,7),
	BASE_INFO_RECT(7,7),
};


static const CString PRICE_MSG[] = {_T("50元"),_T("100元"),_T("150元"),_T("200元"),_T("250元"),_T("300元")};
static const CString PRICE_VALUE[] = {_T("5000"),_T("10000"),_T("15000"),_T("20000"),_T("25000"),_T("30000")};
static const int IDX_CHARGE_VALUE = 0; //充值金额，或充值次数
static const int IDX_AMOUNT = 1;		//充次时金额
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
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeBaseOfficeDlg::CChargeBaseOfficeDlg(CService* pService)
	: COperationDlg(CChargeBaseOfficeDlg::IDD,pService)
{
	InitBaseInfo();
    // 充值信息显示区域
    m_baseInfo->titleInfo.titleName = TITLE_CHARGE_INFO;                    // 标题
    m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_7;           // 行数
    m_baseInfo->detailInfo.labelGroup = m_ChargeInfo;        // 标签文字（2列）
    m_baseInfo->detailInfo.editGroup.cnt = 2;                               // 编辑框数目
    m_baseInfo->detailInfo.editGroup.editRect = editCharge;                 // 编辑框坐标 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

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
	// 票面卡号
	m_ChargeInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 车票种类
	m_ChargeInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 有效期
	m_ChargeInfo[2].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 押金
	m_ChargeInfo[3].nameLeft = add_wsp(CString(TXT_TICKET_DEPOSIT) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 卡内余额 	
	m_ChargeInfo[4].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_ChargeInfo[4].nameRight = add_hsp( MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// 充值金额（默认）/充值次数
	m_ChargeInfo[5].nameLeft = add_wsp(CString(TXT_CHARGE_AMOUNT) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_ChargeInfo[5].nameRight =add_hsp( MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// 乘次单价（隐藏）
	m_ChargeInfo[6].nameLeft = "";
	m_ChargeInfo[6].nameRight = "";

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:创建成功    -1:创建失败

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
@brief      处理ok按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
            TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CChargeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	CChargeSvc* pChargeSvc = (CChargeSvc*)GetService();
    try {				
		// 充值金额输入合法性检查
		if (!ValidateInput()){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_INPUT_ERROR));
			return FALSE;
		}
		CChargeSvc::Model & dataModel = pChargeSvc->GetDataModel();
		// 充值信息确认
		CString msg("");
		if(dataModel.ticketCategory == PURSE){
			msg.Format(_T("充值金额：%s元，是否确认?"),ComFormatAmount(dataModel.due));
		}
		if(dataModel.ticketCategory == MULTIRIDE){
			msg.Format(_T("充值次数：%s，应收金额%s元，是否确认"),ComFormatCount(dataModel.chargeValue),ComFormatAmount(dataModel.due));
		}
		INT_PTR confirmResult = CMessageBox::Show(TITLE_CHARGE_INFO,msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
		if(confirmResult == IDCANCEL)
		{
			return FALSE;
		}
		// 显示充值等待向导文言	
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_WAIT));
		// 充值充次处理
		theSERVICE_MGR.SetState(ISSUEING);	
		int result = pChargeSvc->DoCardCharge();
		if (result == RW_RSP_CHARGE_OVERTIME) // 超时错误处理
		{
			bool bcheck =  true;
			do{					
				//  不是同一张卡
				if (result == RW_RSP_CHARGE_DIFF_CARD){
					CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pChargeSvc->GetServiceID()), theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_NOT_SAME_CARD_PASSENGER),CMessageBox::BTN_OK,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
				} 
				else{
					if(bcheck == false){
						break;
					}
				}
				INT_PTR confirmResult = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pChargeSvc->GetServiceID()), _T("请将卡片放回后按确认键,若取消则直接生成交易！"),CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_600);
				bcheck = confirmResult == CMessageBox::BTN_OK;
				result = pChargeSvc->ChargeOverTime(bcheck);
			}while(result != RW_RSP_OK);					
		}
		// 显示TPU错误信息
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
@brief      编辑框信息改变时的处理

@param      无

@retval     bool true:正确; false:不正确

@exception  无
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
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

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
@brief		对话框初始化

@param      无

@retval     BOOL  \n   TRUE:初始化成功  FALSE:初始化失败

@exception  无
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
@brief      充值金额按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n   TRUE:处理成功  FALSE:处理失败

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
	// 票面卡号
	m_ChargeInfo[0].nameRight = dataModel.strTicketNumber;
	// 车票种类
	m_ChargeInfo[1].nameRight = dataModel.strTicketName;
	// 有效期
	m_ChargeInfo[2].nameRight = dataModel.validDate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
	// 押金
	m_ChargeInfo[3].nameRight = ComFormatAmount(dataModel.deposit);
	// 卡内余额 	
	m_ChargeInfo[4].nameRight = dataModel.ticketCategory == PURSE ? (ComFormatAmount(dataModel.balance) + MONEY_UNIT):(ComFormatCount(dataModel.balance) + TIMES_UNIT);
	// 充值金额（默认）/充值次数
	if(dataModel.ticketCategory == MULTIRIDE)
	{
		m_ChargeInfo[5].nameLeft = TXT_CHARGE_RIDE;
		m_ChargeInfo[6].nameLeft = TXT_CHARGE_PRICE_TIME;
	}
	else
	{
		// 乘次单价（隐藏）
		m_ChargeInfo[6].nameLeft = "";
		m_ChargeInfo[6].nameRight = "";
	}
	
	m_cashInfo->detailInfo.labelGroup[0].nameRight = ComFormatAmount(dataModel.due) + MONEY_UNIT;
	m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(dataModel.change) + MONEY_UNIT;

	m_baseInfoArea->Invalidate();
	m_cashInfoArea->Invalidate();
}