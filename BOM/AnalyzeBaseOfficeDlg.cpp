#include "stdafx.h"
#include "AnalyzeSvc.h"
#include "AnalyzeBaseOfficeDlg.h"
#include "CAccLineInfoParam.h"
#include "CAccTicketParam.h"
#include "tickethelper.h"
#include "TemplateHelper.h"
#include "BomInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static TCHAR* const MSG_ANALYZE_HISTORY_RECORD = _T("MSG_ANAYLIZE_HISTORY_RECORD");
const static TCHAR* const TXT_ANALYZE_HISTORY_RECORD = _T("历史记录");
const static TCHAR* const MSG_ANALYZE_PERSONAL_RECORD = _T("MSG_ANALYZE_PERSONAL_RECORD");
const static TCHAR* const TXT_ANALYZE_PERSONAL_RECORD = _T("个人信息");
const static TCHAR* const MSG_ANALYZE_BASE_RECORD = _T("MSG_ANALYZE_BASE_RECORD");
const static TCHAR* const TXT_ANALYZE_BASE_RECORD = _T("票卡信息");

IMPLEMENT_DYNAMIC(CAnalyzeBaseOfficeDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CAnalyzeBaseOfficeDlg, COperationDlg)
    ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(RIGHT_1_GROUP_ID,OnRight1GroupClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeBaseOfficeDlg::CAnalyzeBaseOfficeDlg(CService* pService)
	: COperationDlg(CAnalyzeBaseOfficeDlg::IDD, pService)
{
	m_baseInfo->detailInfo.editGroup.cnt=0;
	m_baseInfo->detailInfo.row=MAX_LABEL_GROUP_COUNT;
	m_baseInfo->detailInfo.labelGroup = m_TicketInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeBaseOfficeDlg::~CAnalyzeBaseOfficeDlg()
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
int CAnalyzeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
    try{    
		
		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA) == -1) {
			return -1;
		}
    }
    // 异常处理
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
         theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}


LRESULT CAnalyzeBaseOfficeDlg::OnRight1GroupClick(WPARAM wParam, LPARAM lParam)
{

	CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
	BTN_INFO* btnInfo = (BTN_INFO*)lParam;
	CAnalyzeSvc::DataModel &model = pAnalyzeSvc->GetDataModel();
	if(model.cpuCard!=NULL)
	{
		if(btnInfo->btnMsg == MSG_ANALYZE_BASE_RECORD)
		{
			ShowCardInfo(model.cpuCard);
		}
		if(btnInfo->btnMsg == MSG_ANALYZE_HISTORY_RECORD)
		{
			ShowHistory(model.cpuCard);
		}
		if(btnInfo->btnMsg == MSG_ANALYZE_PERSONAL_RECORD)
		{
			ShowPersonal(model.cpuCard);
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
void CAnalyzeBaseOfficeDlg::ShowCardInfo(CCPUCard* pCPUCard)
{
	CString templateName= _T("UI.ANAYLIZE.CPUCARD.template");
	CStringArray printTexts;
	theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
	CString strTicketNum;
	strTicketNum.Format(_T("%.9u"),pCPUCard->applicationData.CardLogicalNumber);
	CAccTicketParam::TICKET_TYPE ticketTypeDetail;
	theACC_TICKET.GetTicketInfo(pCPUCard->applicationData.CardAppMainType,ticketTypeDetail);
	ProductCategory_t ticketCategory;
	theACC_TICKET.GetTicketCategry(pCPUCard->applicationData.CardAppMainType,ticketCategory);
	int i=0;
	while(i<printTexts.GetCount())
	{
		CString& line = printTexts.ElementAt(i);
		line.Replace(_T("{TICKET_NUMBER}"),strTicketNum);
		line.Replace(_T("{TICKET_KIND}"),ticketTypeDetail.ticketTypeName_cn==""?TXT_NOTHING:ticketTypeDetail.ticketTypeName_cn);
		line.Replace(_T("{TICKET_RGION}"),theTICKET_HELPER.GetCPUCardExchangeStatusName( pCPUCard->applicationData.ExchangeCard));
		line.Replace(_T("{TICKET_STATUS}"), theTICKET_HELPER.GetCPUCardTicketStatusName(pCPUCard->applicationData.TicketStatus));
		line.Replace(_T("{TICKET_LOCK_STATUS}"),theTICKET_HELPER.GetCPUCardLockStatusName(pCPUCard->applicationData.CardLocked));
		line.Replace(_T("{TICKET_ENABLE_DATE}"),pCPUCard->applicationData.EnabledDate.ToString(_T("%.4d年%.2d月%.2d日")));
		line.Replace(_T("{TICKET_VALIDATE}"),pCPUCard->applicationData.ValidDate.ToString(_T("%.4d年%.2d月%.2d日")));
		if(ticketCategory == PURSE)
		{	
			line.Replace(_T("{TICKET_BALANCE}"),ComFormatAmount(pCPUCard->applicationData.Balance) + MONEY_UNIT);
		}
		else if(ticketCategory == MULTIRIDE)
		{
			line.Replace(_T("{TICKET_BALANCE}"),ComFormatCount(pCPUCard->applicationData.Balance) + TIMES_UNIT);
			line.Replace(_T("票内余额"),_T("票内余次"));
		}
		else if(line.Find(_T("{TICKET_BALANCE}"))>=0)
		{
			printTexts.RemoveAt(i);
			continue;
		}
		line.Replace(_T("{TICKET_DEPOSIT}"),ComFormatAmount(pCPUCard->applicationData.Deposit) + MONEY_UNIT);
		if(pCPUCard->journeyData.JourneyStatus == CCPUCard::UNENTER_STATION)
		{
			if(pCPUCard->journeyData.ExitStation == 0 && pCPUCard->journeyData.ExitTime == _DATE_TIME())//出站车站和出站时间都为0，则认为该票还没有使用
			{
				if(line.Find(_T("{ENTER_STATION}"))>=0 || line.Find(_T("{ENTER_TIME}"))>=0 || line.Find(_T("{EXIT_STATION}"))>=0 || line.Find(_T("{EXIT_TIME}"))>=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				line.Replace(_T("{JOURNEY_STATUS}"),_T("未使用"));
			}
			else
			{
				CAccLineInfoParam::STATION_INFO exitStation;
				theACC_LINE.GetStationInfobyCode(pCPUCard->journeyData.ExitStation,exitStation);
				line.Replace(_T("{EXIT_STATION}"),exitStation.station_Chinese_Name==""?TXT_NOTHING:exitStation.station_Chinese_Name);
				line.Replace(_T("{EXIT_TIME}"),pCPUCard->journeyData.ExitTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d")));
				if(line.Find(_T("{ENTER_STATION}"))>=0 || line.Find(_T("{ENTER_TIME}"))>=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				line.Replace(_T("{JOURNEY_STATUS}"),theTICKET_HELPER.GetCPUCardJourneyStatusName(pCPUCard->journeyData.JourneyStatus));
			}
		}
		else
		{
			CAccLineInfoParam::STATION_INFO enterStation;
			theACC_LINE.GetStationInfobyCode(pCPUCard->journeyData.EnterStation,enterStation);
			line.Replace(_T("{ENTER_STATION}"),enterStation.station_Chinese_Name==_T("")?TXT_NOTHING:enterStation.station_Chinese_Name);
			line.Replace(_T("{ENTER_TIME}"),pCPUCard->journeyData.EnterTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d")));
			if(line.Find(_T("{EXIT_STATION}"))>=0 || line.Find(_T("{EXIT_TIME}"))>=0)
			{
				printTexts.RemoveAt(i);
				continue;
			}
			line.Replace(_T("{JOURNEY_STATUS}"),theTICKET_HELPER.GetCPUCardJourneyStatusName(pCPUCard->journeyData.JourneyStatus));
		}
		if(pCPUCard->applicationData.CardAppMainType == TICKET_STATION_WORK)//车站工作票才显示使用车站
		{
			CAccLineInfoParam::STATION_INFO useStation;
			theACC_LINE.GetStationInfobyCode(pCPUCard->saleData.SaleStation,useStation);
			line.Replace(_T("{USE_STATION}"),useStation.station_Chinese_Name);
		}
		else if(line.Find(_T("{USE_STATION}"))>=0)
		{
			printTexts.RemoveAt(i);
			continue;
		}
		i++;
	}
	for(int i=0;i<MAX_LABEL_GROUP_COUNT;i++)
	{
		m_TicketInfo[i].nameLeft = i<printTexts.GetSize() ? printTexts.GetAt(i) : _T("");
		m_TicketInfo[i].nameRight = "";
	}
	m_baseInfo->detailInfo.row = printTexts.GetSize();
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeBaseOfficeDlg::ShowCardInfo(CULCard* pULCard)
{
	CString templateName= _T("UI.ANAYLIZE.ULCARD.template");
	CStringArray printTexts;
	theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
	CString strTicketNum;
	strTicketNum.Format(_T("%.9u"),pULCard->applicationData.CardLogicalNumber);
	CAccTicketParam::TICKET_TYPE ticketTypeDetail;
	theACC_TICKET.GetTicketInfo(pULCard->applicationData.CardAppMainType,ticketTypeDetail);
	ProductCategory_t ticketCategory;
	theACC_TICKET.GetTicketCategry(pULCard->applicationData.CardAppMainType,ticketCategory);
	int i=0;
	while(i<printTexts.GetCount()){
		CString& line = printTexts.ElementAt(i);
		line.Replace(_T("{TICKET_NUMBER}"),strTicketNum);
		line.Replace(_T("{TICKET_KIND}"),ticketTypeDetail.ticketTypeName_cn);
		line.Replace(_T("{TICKET_PUBLISH_DATE}"),pULCard->applicationData.PublishDate.ToString(_T("%.4d年%.2d月%.2d日")));
		line.Replace(_T("{TICKET_SALETIME}"),pULCard->applicationData.ValidStartTime.ToString(_T("%.4d年%.2d月%.2d日 %02d:%02d")));
		line.Replace(_T("{TICKET_VALIDATE}"),pULCard->applicationData.ValidEndTime.ToString(_T("%.4d年%.2d月%.2d日 %02d:%02d")));
		if(ticketCategory == PURSE){	
			line.Replace(_T("{TICKET_BALANCE}"),ComFormatAmount(pULCard->applicationData.Balance) + MONEY_UNIT);
		}
		else if(ticketCategory == MULTIRIDE){
			line.Replace(_T("{TICKET_BALANCE}"),ComFormatCount(pULCard->applicationData.Balance) + TIMES_UNIT);
			line.Replace(_T("票内余额"),_T("票内余次"));
		}
		else if(line.Find(_T("{TICKET_BALANCE}"))>=0){
			printTexts.RemoveAt(i);
			continue;
		}
		line.Replace(_T("{TICKET_STATUS}"), theTICKET_HELPER.GetULCardTicketStatusName(pULCard->applicationData.TicketStatus));
		line.Replace(_T("{ACTIVE_STATUS}"),theTICKET_HELPER.GetULCardActiveStatusName(pULCard->applicationData.CardActived));
		line.Replace(_T("{JOURNEY_STATUS}"),theTICKET_HELPER.GetULCardJourneyStatusName(pULCard->journeyData.JourneyStatus));
		
		switch(pULCard->journeyData.JourneyStatus)
		{
		case CULCard::ENTERED_STATION:
			{
				CAccLineInfoParam::STATION_INFO enterStation;
				theACC_LINE.GetStationInfobyCode(pULCard->journeyData.EnterStation,enterStation);
				line.Replace(_T("{ENTER_STATION}"),enterStation.station_Chinese_Name==_T("")?TXT_NOTHING:enterStation.station_Chinese_Name);
				line.Replace(_T("{ENTER_TIME}"),pULCard->journeyData.EnterTime.ToString(_T("%.4d年%.2d月%.2d日 %02d:%02d"))==_T("2000年00月00日 00:00")?TXT_NOTHING:pULCard->journeyData.EnterTime.ToString(_T("%.4d年%.2d月%.2d日 %02d:%02d")));
				break;
			}
		default:
			{
				if(line.Find(_T("{ENTER_STATION}"))>=0 || line.Find(_T("{ENTER_TIME}"))>=0)
				{
					printTexts.RemoveAt(i);
					continue;
				}
				break;
			}
		}
		CAccLineInfoParam::STATION_INFO startStation;
		theACC_LINE.GetStationInfobyCode(pULCard->applicationData.SaleStation,startStation);
		line.Replace(_T("{START_STATION}"),startStation.station_Chinese_Name==""?TXT_NOTHING:startStation.station_Chinese_Name);
		
		i++;
	}
	for(int i=0;i<MAX_LABEL_GROUP_COUNT;i++)
	{
		m_TicketInfo[i].nameLeft = i<printTexts.GetSize() ? printTexts.GetAt(i) : _T("");
		m_TicketInfo[i].nameRight = "";
	}
	m_baseInfo->detailInfo.row = printTexts.GetSize();
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeBaseOfficeDlg::ShowHistory(CCPUCard* pCPUCard)
{
	m_historyGUIInfo[0].nameLeft = add_hsp(TXT_TRANSACTION_TIME,1);
	// 交易类型
	m_historyGUIInfo[0].nameCenter= add_hsp(TXT_TRANSACTION_TYPE,4);
	ProductCategory_t ticketCategory;
	theACC_TICKET.GetTicketCategry(pCPUCard->applicationData.CardAppMainType,ticketCategory);
	// 交易金额
	m_historyGUIInfo[0].nameRight= add_hsp(ticketCategory == MULTIRIDE ? TXT_TRANSCATION_TIMES:TXT_TRANSACTION_AMOUNT,8);
	for(unsigned int i=1 ; i < pCPUCard->historyData.size() +1 && i<MAX_LABEL_GROUP_COUNT ; i++){
		CCPUCard::STR_CARD_HISTORY_DATA & histroy = pCPUCard->historyData[i-1];
		if(histroy.TransTime!=_DATE_TIME())
		{
			m_historyGUIInfo[i].nameLeft = histroy.TransTime.ToString(_T("%.4d/%.2d/%.2d %02d:%02d"));
			m_historyGUIInfo[i].nameCenter = theBOM_INFO.GetTransactionType(histroy.TransType)==""?TXT_UNKNOW_STATUS:theBOM_INFO.GetTransactionType(histroy.TransType);
			if(ticketCategory == MULTIRIDE)
			{
				//if(histroy.TransValue == 0)
				m_historyGUIInfo[i].nameRight = ComFormatCount(histroy.TransValue)+ TIMES_UNIT;
			}
			else
			{
				m_historyGUIInfo[i].nameRight = ComFormatAmount(histroy.TransValue)+MONEY_UNIT;
			}
		}

	}
	m_baseInfo->detailInfo.row = (pCPUCard->historyData.size() +1) > MAX_LABEL_GROUP_COUNT ? MAX_LABEL_GROUP_COUNT:(pCPUCard->historyData.size() +1);
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_historyGUIInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeBaseOfficeDlg::ShowHistory(CULCard* pULCard)
{
	m_historyGUIInfo[0].nameLeft = add_hsp(TXT_TRANSACTION_TIME,1);
	// 交易类型
	m_historyGUIInfo[0].nameCenter= add_hsp(TXT_TRANSACTION_TYPE,4);
	// 交易金额
	m_historyGUIInfo[0].nameRight= add_hsp(TXT_TRANSACTION_AMOUNT,8);
	for(unsigned int i=1;i<MAX_LABEL_GROUP_COUNT;i++){
		if(i < pULCard->historyData.size() +1){
			CULCard::STR_CARD_HISTORY_DATA & histroy = pULCard->historyData[i-1];
			m_historyGUIInfo[i].nameLeft = histroy.TransTime.ToString(_T("%.4d年%.2d月%.2d日 %02d:%02d"));
			m_historyGUIInfo[i].nameCenter = theBOM_INFO.GetTransactionType(histroy.TransType)==""?TXT_UNKNOW_STATUS:theBOM_INFO.GetTransactionType(histroy.TransType);
			m_historyGUIInfo[i].nameRight = ComFormatAmount(histroy.TransValue);
		}
		else
		{
			m_historyGUIInfo[i].nameLeft = "";
			m_historyGUIInfo[i].nameCenter = "";
			m_historyGUIInfo[i].nameRight = "";
		}
		
	}
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo,m_historyGUIInfo);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      显示个人信息

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeBaseOfficeDlg::ShowPersonal(CCPUCard* pCPUCard)
{

	// 先读出个数
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + BOM_INI_FILE_NAME;
	CGetIni getini(szBOMINIFName);

	CString credentialsType(_T(""));
	credentialsType.Format(_T("%d"),pCPUCard->personalData.CredentialsType);

	m_TicketInfo[0].nameLeft = add_wsp(CString(TXT_HOLDER_NAME) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_TicketInfo[0].nameRight = pCPUCard->personalData.Name==""?TXT_NOTHING:pCPUCard->personalData.Name;
	m_TicketInfo[1].nameLeft = add_wsp(CString(TXT_CERTIFATE_TYPE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_TicketInfo[1].nameRight = getini.GetDataString(_T("CERTIFICATE_TYPE"),credentialsType)==""?TXT_NOTHING:getini.GetDataString(_T("CERTIFICATE_TYPE"),credentialsType);
	m_TicketInfo[2].nameLeft = add_wsp(CString(TXT_CERTIFATE_NUMBER) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_TicketInfo[2].nameRight = pCPUCard->personalData.CredentialsNumber==""?TXT_NOTHING:pCPUCard->personalData.CredentialsNumber;
	m_baseInfo->detailInfo.labelGroup = m_TicketInfo;
	m_baseInfo->detailInfo.row = 3;
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;
	m_baseInfoArea = new CGStatic(this,m_baseInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      更新画面函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeBaseOfficeDlg::UpdateUI()
{
	CAnalyzeSvc* pAnalyzeSvc = GetService<CAnalyzeSvc>();
	if (pAnalyzeSvc->GetCurDialogGroupID()==CAnalyzeSvc::DIALOG_GROUP_ERROR)
	{
		HideCompoment(BTN_BACK|BASE_AREA);
		m_vec_Right_1_BTN_INFO.clear();
		return;
	}
	CAnalyzeSvc::DataModel &model = pAnalyzeSvc->GetDataModel();
	if(pAnalyzeSvc->GetCurDialogGroupID()==CAnalyzeSvc::DIALOG_GROUP_BASE)
	{
		if(model.ulCard!=NULL)
		{
			ShowCardInfo(model.ulCard);
		}
		if(model.cpuCard!=NULL)
		{
			m_vec_Right_1_BTN_INFO.clear();
			BTN_INFO btnInfo;
			btnInfo.btnMsg = MSG_ANALYZE_BASE_RECORD;
			btnInfo.text = TXT_ANALYZE_BASE_RECORD;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			btnInfo.btnMsg = MSG_ANALYZE_HISTORY_RECORD;
			btnInfo.text = TXT_ANALYZE_HISTORY_RECORD;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			btnInfo.btnMsg = MSG_ANALYZE_PERSONAL_RECORD;
			btnInfo.text = TXT_ANALYZE_PERSONAL_RECORD;
			m_vec_Right_1_BTN_INFO.push_back(btnInfo);
			m_Right_1_Group->SetButtonInfos(m_vec_Right_1_BTN_INFO);
			m_Right_1_Group->ClickFirstAvailButton();
			//ShowCardInfo(model.cpuCard);
		}
		return;
	}
	//if(pAnalyzeSvc->GetCurDialogGroupID()==CAnalyzeSvc::DIALOG_GROUP_HISTORY)
	//{
	//	if(model.ulCard!=NULL)
	//	{
	//		ShowHistory(model.ulCard);
	//	}
	//	if(model.cpuCard!=NULL)
	//	{
	//		ShowHistory(model.cpuCard);
	//	}
	//	return;
	//}
	//if(pAnalyzeSvc->GetCurDialogGroupID() == CAnalyzeSvc::DIALOG_GROUP_PERSONAL)
	//{
	//	if(model.cpuCard!=NULL)
	//	{
	//		ShowPersonal(model.cpuCard);
	//	}
	//	return;
	//}

}
