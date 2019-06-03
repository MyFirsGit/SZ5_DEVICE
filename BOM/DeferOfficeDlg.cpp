#include "stdafx.h"
#include "DeferOfficeDlg.h"
#include "defersvc.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//信息显示编辑框坐标组合
static CRect editRect[] = {
	BASE_INFO_RECT(4,7),
	BASE_INFO_RECT(6,7),
};
const static int const IDX_DEFER_DAYS = 0;
const static int const IDX_SERVICE_FEE = 1;
const static int const IDX_PAID = 2;

IMPLEMENT_DYNAMIC(CDeferOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CDeferOfficeDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_BTN_OK_CLICK(OnOK)
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
CDeferOfficeDlg::CDeferOfficeDlg(CService* pService)
: COperationDlg(CDeferOfficeDlg::IDD,pService)
{
	// 车票种类
	m_DeferInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 卡内余额 	
	m_DeferInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_DeferInfo[1].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// 有效期
	m_DeferInfo[2].nameLeft = add_wsp(CString(TXT_OLD_VALIDATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//延长时间
	m_DeferInfo[3].nameLeft = add_wsp(CString(TXT_DEFER_DAYS) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_DeferInfo[3].nameRight = add_hsp(DATE_UNIT,MAX_CASH_RIGHT_LEN);
	//新有效期
	m_DeferInfo[4].nameLeft = add_wsp(CString(TXT_NEW_VALIDATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	//手续费
	m_DeferInfo[5].nameLeft = add_wsp(CString(TXT_SEVICE_FEE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_DeferInfo[5].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// 基本信息显示区域
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_6;           // 行数
	m_baseInfo->detailInfo.labelGroup = m_DeferInfo;        // 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.editRect = editRect;
	m_baseInfo->detailInfo.editGroup.cnt = 2;                               // 编辑框数目 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CDeferOfficeDlg::~CDeferOfficeDlg()
{

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CDeferOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDeferSvc* pSvc = (CDeferSvc*)GetService();
	try{
		// 创建基本信息区域、返回、确定按钮
		if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA|CASH_AREA )){
			return -1;
		}
		GetEdit(IDX_SERVICE_FEE)->SetScope(CGEdit::CASH);
		GetEdit(IDX_SERVICE_FEE)->SetLimitText(6);
		GetEdit(IDX_DEFER_DAYS)->SetLimitText(3);
		//SetEditText(IDX_SERVICE_FEE,ComFormatAmount(10*100));
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

void CDeferOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CDeferSvc* pSvc = (CDeferSvc*)GetService();
	CDeferSvc::DataModel& model = pSvc->GetDataModel();
	SetEditText(IDX_SERVICE_FEE,ComFormatAmount(model.due));
	//SetEditText
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDeferOfficeDlg::OnEditChange(WPARAM wParam,LPARAM lParam)
{
	CDeferSvc* pSvc = GetService<CDeferSvc>();
	CDeferSvc::DataModel &model = pSvc->GetDataModel();
	CGEdit* currentEdit = (CGEdit*)lParam;
	if(currentEdit == GetEdit(IDX_DEFER_DAYS))
	{
		//model.deferDay = atoi(GetEditText(IDX_DEFER_DAYS));
	}
	if(currentEdit == GetEdit(IDX_SERVICE_FEE))
	{
		model.due = ComGetAmount(GetEditText(IDX_SERVICE_FEE));
		pSvc->NotifyDataModelChanged();
	}
	if(currentEdit == GetEdit(IDX_PAID))
	{
		model.paid = ComGetAmount(GetEditText(IDX_PAID));
		pSvc->NotifyDataModelChanged();
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置、返回按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDeferOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	RUNNING_STATUS startRunning;
	if(!ValidateInput())
	{
		return FALSE;
	}
	CDeferSvc* pSvc = (CDeferSvc*)GetService();
	return pSvc->DoDefer();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证输入

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CDeferOfficeDlg::ValidateInput()
{
	CDeferSvc* pSvc = GetService<CDeferSvc>();
	CDeferSvc::DataModel & model = pSvc->GetDataModel();
	if(model.deferDay<=0)
	{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_DEFER_DAYS_ERROR));
		GetEdit(IDX_DEFER_DAYS)->SetFocus();
		return false;
	}
	if(GetEditText(IDX_PAID)!="")
	{
		if(model.paid<model.due)
		{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_DEFER_CASH_ERROR));
			GetEdit(IDX_PAID)->SetFocus();
			return false;
		}
	}
	else if(GetEditText(IDX_PAID)=="")
	{
		SetEditText(IDX_PAID,ComFormatAmount(model.due));
		pSvc->GetDataModel().paid = model.due;
		pSvc->NotifyDataModelChanged();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      更新画面

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CDeferOfficeDlg::UpdateUI()
{
	CDeferSvc* pSvc = (CDeferSvc*)GetService();
	try{	
		// 不可激活
		if (pSvc->GetCurDialogGroupID()==CDeferSvc::DIALOG_GROUP_DEFER_NOT) {
			HideCompoment(BTN_OK | BASE_AREA|CASH_AREA);	
			return;
		}
		CDeferSvc::DataModel& model = pSvc->GetDataModel();
		m_DeferInfo[0].nameRight = model.ticketTypeDetail.ticketTypeName_cn;
		if(model.cardCategory == PURSE)
		{
			m_DeferInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_DeferInfo[1].nameRight = ComFormatAmount(model.balance) + MONEY_UNIT;
		}
		else if(model.cardCategory == MULTIRIDE)
		{
			m_DeferInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_RIDES) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_DeferInfo[1].nameRight = ComFormatCount(model.balance) + TIMES_UNIT;
		}
		else if(model.cardCategory == PERIOD)
		{
			m_DeferInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_REGION) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_DeferInfo[1].nameRight = CString(_T("定期"));
		}
		m_DeferInfo[2].nameRight = model.orignalValidate.ToString(_T("%.4d年%.2d月%.2d日"));

		m_DeferInfo[4].nameRight = model.newValidate.ToString(_T("%.4d年%.2d月%.2d日"));
		if(pSvc->GetCurDialogGroupID()==CDeferSvc::DIALOG_GROUP_FINISH)
		{
			HideCompoment( BTN_OK);
		}
		GetEdit(IDX_DEFER_DAYS)->SetEnabled(false);
		CString strDeferDays;
		strDeferDays.Format(_T("%d"),model.deferDay);
		SetEditText(IDX_DEFER_DAYS,strDeferDays);
		
		GetEdit(IDX_SERVICE_FEE)->SetEnabled(pSvc->GetCurDialogGroupID()!=CDeferSvc::DIALOG_GROUP_FINISH);
		GetEdit(IDX_PAID)->SetEnabled(pSvc->GetCurDialogGroupID()!=CDeferSvc::DIALOG_GROUP_FINISH);
		m_baseInfoArea->Invalidate();
		
		m_cashInfo->detailInfo.labelGroup[0].nameRight = ComFormatAmount(model.due)+MONEY_UNIT;
		m_cashInfo->detailInfo.labelGroup[2].nameRight = ComFormatAmount(model.change)+MONEY_UNIT;
		m_cashInfoArea->Invalidate();
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
