#include "stdafx.h"
#include "DeferSvc.h"
#include "DeferReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CDeferReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CDeferReceptionDlg,CReceptionDlg)
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
CDeferReceptionDlg::CDeferReceptionDlg(CService* pService)
:CReceptionDlg(CDeferReceptionDlg::IDD, pService)
{
		vector<MSG_NAME_GROUP> &detailList = m_BusinessInfo->detailList;
		// 票卡类型
		MSG_NAME_GROUP msgTicketType;
		msgTicketType.nameLeft.nameChinese = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
		msgTicketType.nameLeft.nameEnglish = add_wsp(TXT_TICKET_KIND_EN,MAX_SBC_CASE_LEN);
		msgTicketType.nameRight.nameChinese = TXT_UNDEFINE;
		msgTicketType.nameRight.nameEnglish =TXT_UNDEFINE;
		detailList.push_back(msgTicketType);

		// 票内余额
		MSG_NAME_GROUP msgBalance;
		msgBalance.nameLeft.nameChinese = add_wsp(TXT_TICKET_BALANCE,MAX_SBC_CASE_LEN);
		msgBalance.nameLeft.nameEnglish = add_wsp(TXT_TICKET_BALANCE_EN,MAX_SBC_CASE_LEN);
		msgBalance.nameRight.nameChinese = TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(msgBalance);
	

		//原有效期
		MSG_NAME_GROUP msgOldValidate;
		msgOldValidate.nameLeft.nameChinese = add_wsp(TXT_OLD_VALIDATE,MAX_SBC_CASE_LEN);
		msgOldValidate.nameLeft.nameEnglish = add_wsp(TXT_OLD_VALIDATE_EN,MAX_SBC_CASE_LEN);
		msgOldValidate.nameRight.nameChinese =TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(msgOldValidate);

		MSG_NAME_GROUP msgDeferDays;
		msgDeferDays.nameLeft.nameChinese = add_wsp(TXT_DEFER_DAYS,MAX_SBC_CASE_LEN);
		msgDeferDays.nameLeft.nameEnglish = add_wsp(TXT_DEFER_DAYS_EN,MAX_SBC_CASE_LEN);
		msgDeferDays.nameRight.nameChinese = TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(msgOldValidate);
		//新有效期
		MSG_NAME_GROUP msgNewValidate;
		msgNewValidate.nameLeft.nameChinese = add_wsp(TXT_NEW_VALIDATE,MAX_SBC_CASE_LEN);
		msgNewValidate.nameLeft.nameEnglish = add_wsp(TXT_NEW_VALIDATE_EN,MAX_SBC_CASE_LEN);
		msgNewValidate.nameRight.nameChinese =TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(msgNewValidate);

		MSG_NAME_GROUP amountDueGroup;
		amountDueGroup.nameLeft.nameChinese = TXT_TOTAL_RECEPTION;
		amountDueGroup.nameLeft.nameEnglish = TXT_TOTAL_RECEPTION_EN;
		amountDueGroup.nameRight.nameChinese = TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(amountDueGroup);

		MSG_NAME_GROUP paidGroup;
		paidGroup.nameLeft.nameChinese = TXT_INCOME_RECEPTION;
		paidGroup.nameLeft.nameEnglish = TXT_INCOME_RECEPTION_EN;
		paidGroup.nameRight.nameChinese =TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(paidGroup);

		MSG_NAME_GROUP changeGroup;
		changeGroup.nameLeft.color = RED;
		changeGroup.nameLeft.nameChinese = TXT_CHANGE_RECEPTION;
		changeGroup.nameLeft.nameEnglish = TXT_CHANGE_RECEPTION_EN;
		changeGroup.nameRight.color = RED;
		changeGroup.nameRight.nameChinese =TXT_UNDEFINE;
		m_BusinessInfo->detailList.push_back(changeGroup);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeferReceptionDlg::~CDeferReceptionDlg()
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
int CDeferReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
void CDeferReceptionDlg::UpdateUI()
{
	CDeferSvc* pSvc = (CDeferSvc*)GetService();
	try{	

		if (pSvc->GetCurDialogGroupID()==CDeferSvc::DIALOG_GROUP_DEFER_NOT) {
			//隐藏所有组件	
			HideCompoment(BUSINESS_AREA);
			return;
		}
		CDeferSvc::DataModel& model = pSvc->GetDataModel();
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
		msgOldValidate.nameRight.nameChinese = model.orignalValidate.ToString(_T("%.4d/%.2d/%.2d"));
		m_BusinessInfo->detailList.push_back(msgOldValidate);

		MSG_NAME_GROUP msgDeferDays;
		msgDeferDays.nameLeft.nameChinese = add_wsp(TXT_DEFER_DAYS,MAX_SBC_CASE_LEN);
		msgDeferDays.nameLeft.nameEnglish = add_wsp(TXT_DEFER_DAYS_EN,MAX_SBC_CASE_LEN);
		msgDeferDays.nameRight.nameChinese = ComFormatCount(model.deferDay)+DATE_UNIT;
		msgDeferDays.nameRight.nameEnglish = ComFormatCount(model.deferDay)+DAY_UNIT_EN;
		m_BusinessInfo->detailList.push_back(msgDeferDays);

		//新有效期
		MSG_NAME_GROUP msgNewValidate;
		msgNewValidate.nameLeft.nameChinese = add_wsp(TXT_NEW_VALIDATE,MAX_SBC_CASE_LEN);
		msgNewValidate.nameLeft.nameEnglish = add_wsp(TXT_NEW_VALIDATE_EN,MAX_SBC_CASE_LEN);
		msgNewValidate.nameRight.nameChinese = model.newValidate.ToString(_T("%.4d/%.2d/%.2d"));
		m_BusinessInfo->detailList.push_back(msgNewValidate);


		MSG_NAME_GROUP amountDueGroup;
		amountDueGroup.nameLeft.nameChinese = TXT_TOTAL_RECEPTION;
		amountDueGroup.nameLeft.nameEnglish = TXT_TOTAL_RECEPTION_EN;
		amountDueGroup.nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.due);
		m_BusinessInfo->detailList.push_back(amountDueGroup);

		MSG_NAME_GROUP paidGroup;
		paidGroup.nameLeft.nameChinese = TXT_INCOME_RECEPTION;
		paidGroup.nameLeft.nameEnglish = TXT_INCOME_RECEPTION_EN;
		paidGroup.nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.paid);
		m_BusinessInfo->detailList.push_back(paidGroup);

		MSG_NAME_GROUP changeGroup;
		changeGroup.nameLeft.color = RED;
		changeGroup.nameLeft.nameChinese = TXT_CHANGE_RECEPTION;
		changeGroup.nameLeft.nameEnglish = TXT_CHANGE_RECEPTION_EN;
		changeGroup.nameRight.color = RED;
		changeGroup.nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(model.change);
		m_BusinessInfo->detailList.push_back(changeGroup);
		Invalidate();
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CDeferReceptionDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
}
