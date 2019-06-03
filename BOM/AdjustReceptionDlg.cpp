#include "stdafx.h"
#include "AdjustSvc.h"
#include "AdjustReceptionDlg.h"
#include "ExceptionMgr.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAdjustReceptionDlg, CReceptionDlg)

BEGIN_MESSAGE_MAP(CAdjustReceptionDlg, CReceptionDlg)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAdjustReceptionDlg::CAdjustReceptionDlg(CService* pService)
	: CReceptionDlg(CAdjustReceptionDlg::IDD, pService)
{
	//// 初始化信息区域
	InitBaseInfo();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAdjustReceptionDlg::~CAdjustReceptionDlg()
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
int CAdjustReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return __super::OnCreate(lpCreateStruct,BUSINESS_AREA);
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
void CAdjustReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	if(pSvc->GetDialogFlow()->GetCurDialogGroupID() == CAdjustSvc::DIALOG_GROUP_ERROR)
	{
		HideCompoment(BUSINESS_AREA);
	}
	theSERVICE_MGR.SetState(FINISH);
    __super::OnActivate(nState, pWndOther, bMinimized);
}

void CAdjustReceptionDlg::InitBaseInfo()
{
	// 票卡类型
	vector<MSG_NAME_GROUP> &  detailList = m_BusinessInfo->detailList;
	MSG_NAME_GROUP receptionTicketKindGroup;
	receptionTicketKindGroup.nameLeft.nameChinese = TXT_TICKET_KIND;
	receptionTicketKindGroup.nameLeft.nameEnglish = TXT_TICKET_KIND_EN;
	detailList.push_back(receptionTicketKindGroup);
	// 票内余额
	MSG_NAME_GROUP receptionTicketbalanceGroup;
	receptionTicketbalanceGroup.nameLeft.nameChinese = TXT_TICKET_BALANCE;
	receptionTicketbalanceGroup.nameLeft.nameEnglish = TXT_TICKET_BALANCE_EN;
	detailList.push_back(receptionTicketbalanceGroup);
	// 进站车站
	MSG_NAME_GROUP enterStationGroup;
	enterStationGroup.nameLeft.nameChinese = TXT_ENTER_STATION;
	enterStationGroup.nameLeft.nameEnglish = TXT_ENTER_STATION_EN;
	detailList.push_back(enterStationGroup);
	// 进站时间
	MSG_NAME_GROUP enterStationTimeGroup;
	enterStationTimeGroup.nameLeft.nameChinese = TXT_ENTER_STATION_TIME;
	enterStationTimeGroup.nameLeft.nameEnglish = TXT_ENTER_STATION_TIME_EN;
	detailList.push_back(enterStationTimeGroup);

	// 补票方式
	MSG_NAME_GROUP receptionUpdateReasonGroup;
	receptionUpdateReasonGroup.nameLeft.nameChinese = TXT_ADJUST_TYPE;
	receptionUpdateReasonGroup.nameLeft.nameEnglish = TXT_ADJUST_TYPE_EN;
	receptionUpdateReasonGroup.nameLeft.color = RED;
	receptionUpdateReasonGroup.nameRight.color = RED;
	detailList.push_back(receptionUpdateReasonGroup);
	// 补票金额 
	MSG_NAME_GROUP receptionAmountGroup;
	receptionAmountGroup.nameLeft.nameChinese = TXT_ADJUST_AMOUNT;
	receptionAmountGroup.nameLeft.nameEnglish = TXT_ADJUST_AMOUNT_EN;
	receptionAmountGroup.nameLeft.color = RED;
	receptionAmountGroup.nameRight.color = RED;
	detailList.push_back(receptionAmountGroup);
	// 应收金额
	MSG_NAME_GROUP receptionAmountDueGroup;
	receptionAmountDueGroup.nameLeft.nameChinese = TXT_AMOUNT_DUE_VALUE;
	receptionAmountDueGroup.nameLeft.nameEnglish = TXT_AMOUNT_DUE_VALUE_EN;
	detailList.push_back(receptionAmountDueGroup);
	// 已付金额
	MSG_NAME_GROUP receptionIncomeGroup;
	receptionIncomeGroup.nameLeft.nameChinese = TXT_INCOME_RECEPTION;
	receptionIncomeGroup.nameLeft.nameEnglish = TXT_INCOME_RECEPTION_EN;
	detailList.push_back(receptionIncomeGroup);
	// 找零金额
	MSG_NAME_GROUP receptionChangeGroup;
	receptionChangeGroup.nameLeft.nameChinese = TXT_CHANGE_RECEPTION;
	receptionChangeGroup.nameLeft.nameEnglish = TXT_CHANGE_RECEPTION_EN;
	receptionChangeGroup.nameLeft.color = RED;
	receptionChangeGroup.nameRight.color = RED;
	detailList.push_back(receptionChangeGroup);
}

void CAdjustReceptionDlg::UpdateUI()
{
	CAdjustSvc* pSvc = GetService<CAdjustSvc>();
	CAdjustSvc::Model& dataModel = pSvc->GetDataModel();
	vector<MSG_NAME_GROUP> &  detailList = m_BusinessInfo->detailList;

	detailList[0].nameRight.nameChinese =  dataModel.strTicketTypeName;
	detailList[0].nameRight.nameEnglish = dataModel.strTicketTypeEnName;

	if(dataModel.ticketCategory == MULTIRIDE )	{
		detailList[1].nameLeft.nameChinese = add_wsp(TXT_TICKET_RIDES,MAX_SBC_CASE_LEN);
		detailList[1].nameRight.nameChinese=ComFormatCount(dataModel.balance)+TIMES_UNIT;
	}
	else if(dataModel.ticketCategory==PURSE){
		detailList[1].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(dataModel.balance);
	}
	else{
		detailList[1].nameRight.nameChinese = TXT_NOTHING;
	}
	CAccLineInfoParam::STATION_INFO enterStationInfo;
	theACC_LINE.GetStationInfobyCode(dataModel.enterStation,enterStationInfo);
	detailList[2].nameRight.nameChinese =  enterStationInfo.station_Chinese_Name==_T("")?TXT_NOTHING:enterStationInfo.station_Chinese_Name;
	detailList[2].nameRight.nameEnglish =  enterStationInfo.station_Eng_Name==_T("")?TXT_NOTHING:enterStationInfo.station_Eng_Name;
	detailList[3].nameRight.nameChinese =  dataModel.enterDateTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));

	detailList[4].nameRight.nameChinese = dataModel.strAdjustGuideOfReceipt;
	detailList[4].nameRight.nameEnglish = dataModel.strAdjustEnGuideOfReceipt;
	if(dataModel.ticketCategory == MULTIRIDE )	{
		detailList[5].nameLeft.nameChinese = add_wsp(TXT_ADJUST_RIDES,MAX_SBC_CASE_LEN);
		detailList[5].nameRight.nameChinese=ComFormatCount(dataModel.actualAdjustAmount)+TIMES_UNIT;
		detailList[5].nameRight.nameEnglish ="";
	}
	else if(dataModel.ticketCategory==PURSE){
		detailList[5].nameRight.nameChinese = TXT_YUAN_MARK+ComFormatAmount(dataModel.actualAdjustAmount);
		detailList[5].nameRight.nameEnglish ="";
	}
	else{
		detailList[5].nameRight.nameChinese = TXT_NOTHING;
		detailList[5].nameRight.nameEnglish ="";
	}
	if(dataModel.payMethod == BY_CARD)
	{
		detailList[6].nameLeft.nameChinese = "";
		detailList[6].nameLeft.nameEnglish = "";
		// 已付金额
		detailList[7].nameLeft.nameChinese = "";
		detailList[7].nameLeft.nameEnglish = "";
		// 找零金额
		detailList[8].nameLeft.nameChinese = "";
		detailList[8].nameLeft.nameEnglish = "";

		detailList[6].nameRight.nameChinese = "" ;
		detailList[7].nameRight.nameChinese = "" ;
		detailList[8].nameRight.nameChinese = "";
	}
	else
	{

		detailList[6].nameLeft.nameChinese = TXT_AMOUNT_DUE_VALUE;
		detailList[6].nameLeft.nameEnglish = TXT_AMOUNT_DUE_VALUE_EN;
		// 已付金额
		detailList[7].nameLeft.nameChinese = TXT_INCOME_RECEPTION;
		detailList[7].nameLeft.nameEnglish = TXT_INCOME_RECEPTION_EN;
		// 找零金额
		detailList[8].nameLeft.nameChinese = TXT_CHANGE_RECEPTION;
		detailList[8].nameLeft.nameEnglish = TXT_CHANGE_RECEPTION_EN;

		detailList[6].nameRight.nameChinese = TXT_YUAN_MARK + ComFormatAmount(dataModel.due) ;
		detailList[7].nameRight.nameChinese = TXT_YUAN_MARK + ComFormatAmount(dataModel.paid) ;
		detailList[8].nameRight.nameChinese =   TXT_YUAN_MARK + ComFormatAmount(dataModel.change) ;
	}

	m_BusinessArea->Invalidate();
}
