#include "stdafx.h"
#include "RefreshSvc.h"
#include "RefreshReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRefreshReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CRefreshReceptionDlg,CReceptionDlg)
    ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRefreshReceptionDlg::CRefreshReceptionDlg(CService* pService)
    :CReceptionDlg(CRefreshReceptionDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRefreshReceptionDlg::~CRefreshReceptionDlg()
{
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CRefreshReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // 创建基本信息区域、操作数据区域
    if(-1 == __super::OnCreate(lpCreateStruct,BUSINESS_AREA )){
        return -1;
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
void CRefreshReceptionDlg::UpdateUI()
{
	CRefreshSvc* pSvc = (CRefreshSvc*)GetService();
	try{	
		
		if (pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_REFRESH_NOT) {
			//隐藏所有组件	
			HideCompoment(BUSINESS_AREA);
			return;
		}
		CRefreshSvc::DataModel& model = pSvc->GetDataModel();
		vector<MSG_NAME_GROUP> &detailList = m_BusinessInfo->detailList;
		detailList.clear();
		// 票卡类型
		MSG_NAME_GROUP msgTicketType;
		msgTicketType.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
		msgTicketType.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
		msgTicketType.nameRight.nameChinese = model.ticketTypeDetail.ticketTypeName_cn;
		msgTicketType.nameRight.nameEnglish = model.ticketTypeDetail.ticketTypeName_en;
		detailList.push_back(msgTicketType);

		// 票内余额
		MSG_NAME_GROUP msgBalance;

		if(model.cardCategory == PURSE)
		{
			msgBalance.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
			msgBalance.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
			msgBalance.nameRight.nameChinese = TXT_YUAN_MARK + ComFormatAmount(model.balance);
			m_BusinessInfo->detailList.push_back(msgBalance);
		}
		else if(model.cardCategory == MULTIRIDE)
		{
			msgBalance.nameLeft.nameChinese = add_wsp(TXT_TICKET_RIDES,MAX_SBC_CASE_LEN);
			msgBalance.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
			msgBalance.nameRight.nameChinese =  ComFormatCount(model.balance);
			m_BusinessInfo->detailList.push_back(msgBalance);
		}
		else if(model.cardCategory == PERIOD)
		{
		}
	
		//原有效期
		MSG_NAME_GROUP msgOldValidate;
		msgOldValidate.nameLeft.nameChinese = add_wsp(TXT_OLD_VALIDATE,MAX_SBC_CASE_LEN);
		msgOldValidate.nameLeft.nameEnglish = add_wsp(TXT_OLD_VALIDATE_EN,MAX_SBC_CASE_LEN);
		msgOldValidate.nameRight.nameChinese = model.orignalValidate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
		m_BusinessInfo->detailList.push_back(msgOldValidate);

		if(pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_FINISH)
		{
			//新有效期
			MSG_NAME_GROUP msgNewValidate;
			msgNewValidate.nameLeft.nameChinese = add_wsp(TXT_NEW_VALIDATE,MAX_SBC_CASE_LEN);
			msgNewValidate.nameLeft.nameEnglish = add_wsp(TXT_NEW_VALIDATE_EN,MAX_SBC_CASE_LEN);
			msgNewValidate.nameRight.nameChinese = model.newValidate.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
			m_BusinessInfo->detailList.push_back(msgNewValidate);
		}
		m_BusinessArea->Invalidate();
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
