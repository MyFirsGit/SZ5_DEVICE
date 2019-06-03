#include "stdafx.h"
#include "AnalyzeSvc.h"
#include "WelcomeReceptionDlg.h"
#include "waitingofficedlg.h"
#include "ectcardinfo.h"
#include "guideinfo.h"
#include "TemplateHelper.h"
#include "tickethelper.h"
#include "CBaseUlInfoDlg.h"
#include "CBaseM1InfoDlg.h"
#include "CBaseHistoryDlg.h"
#include "CBaseUserInfoDlg.h"
#include "RechargeRWMonitorSvc.h"
#include "CAfcTVMOperationParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//static const TCHAR* const WM_ANALYZE_WAIT_TO_BASE = _T("WM_ANALYZE_WAIT_TO_BASE");


BEGIN_MESSAGE_MAP(CAnalyzeSvc, CPassengerForeSvc/*CTVMForeService*/)
	
END_MESSAGE_MAP()


bool CAnalyzeSvc::m_IsAbort = false;
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService/** pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::CAnalyzeSvc() : CPassengerForeSvc(ANALYZE_SVC)
{
    // 设置乘客画面
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_10805_ANALYZE_BASEM1_DLG, new CBaseM1InfoDlg(this));
	
    // 设置画面分组
    // 等待画面组
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PUT_CARD)); //
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE_M1,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE_M1, IDD_10805_ANALYZE_BASEM1_DLG);


    // 设置画面流转
	/*m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ANALYZE_WAIT_TO_BASEUL, DIALOG_GROUP_BASE_UL);*/
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ANALYZE_WAIT_TO_BASEM1, DIALOG_GROUP_BASE_M1);

	// 设置初始画面组
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);

	m_DataModel = DataModel();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::~CAnalyzeSvc()
{
	delete m_DataModel.accCard;
	m_DataModel.accCard = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动时响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::OnStart()
{
	__super::OnStart();

	// 设置初始画面组
    //m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
	// 隐藏功能区按钮及语言显示按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	/*m_DataModel.accCard = NULL;*/
	m_DataModel = DataModel();
	// 设置初始画面组
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
	IO_HELPER->OpenChargeIndicator();
	m_dwTime = GetTickCount();
	CAnalyzeSvc::m_IsAbort = false;
}
 

////////////////////////////////////////////////////////////////////////////
///**
//@brief      讀到卡以後的處理
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::OnReadCard(UINT resultCode, CRWReadCard* pRWReadCard)
{
	try	{
		theSERVICE_MGR.SetState(RECEPT);
		CMessagePictureBox::End();
		IO_HELPER->CloseChargeIndicator();
		pRWReadCard->GetRWResponse(&(m_DataModel.rwReadResponse));
		if(resultCode != RW_RSP_OK)
		{
			if(resultCode==RW_ERROR_CARD_NO_CARD){
				// // 取消后退出读卡,返回默认业务
				LOG("读卡反馈无卡,启动默认业务。");
				theSERVICE_MGR.SetState(RECEPT);
				CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
				NOTIFY_PROCESSING_WINDOW();
				pMainSvc->StartDefaultService();	
				return;
			}

			if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {
				//BlockCard(); // 黑名单票卡锁卡
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(TXT_MSG_GUIDE_THIS_CARD_HAS_BEEN_LOCKED),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				// 将卡弹出
				//IO_HELPER->CheckAndSendOutCard();lichao
				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
			}			  
			else {
				CString strMsg = theTPUERROR_INFO.GetTpuErrorInfo(m_DataModel.rwReadResponse.DetailErrorCode);
				int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();
				INT_PTR ret = CMessagePictureBox::Show(	theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),
														strMsg,
														TVM_TIP_SWIPE_CARD,
														CMessagePictureBox::BTN_OK,DT_LEFT,
														iReadTimeLongth);
				if(IDOK == ret || IDABORT == ret)		//点击确认，或者时间读完
				{
					NOTIFY_PROCESSING_WINDOW();
					RECHARGERW_HELPER->StopReadCard();	//停止读卡,回到默认业务，继续轮询
					CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
					pMainSvc->StartDefaultService();
				}
			}
		}
		else
		{
			//IO_HELPER->CloseChargeIndicator();

			if (m_DataModel.rwReadResponse.ProductCount <= 0) // (已初始化过的空卡)无产品返回默认业务
			{
				CMessagePictureBox::End();
				int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();
				INT_PTR ret = CMessagePictureBox::Show(	theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),
					_tl(TXT_PSG_TICKET_CANT_RECOGNIZE),
					TVM_TIP_WARINGS,
					CMessagePictureBox::BTN_OK,DT_LEFT,
					iReadTimeLongth);
				if(IDOK == ret || IDABORT == ret)		//点击取消，或者时间读完
				{
					RECHARGERW_HELPER->StopReadCard();	//停止读卡,回到默认业务，继续轮询
					CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
					pMainSvc->StartDefaultService();
				}
			}
			else{
				//m_DataModel = DataModel();
				delete m_DataModel.accCard;
				m_DataModel.accCard = NULL;												// 指针置空
				m_DataModel.accCard = new CACCCard;
				m_DataModel.accCard->GetCardInfo(m_DataModel.rwReadResponse.CardInfo); //解析读卡反馈数据，获取票卡数据

				if(CARD_TYPE_SY_EPT_UL != m_DataModel.rwReadResponse.CardPHType){
					m_DataModel.accCard->GetHistoryInfo(m_DataModel.rwReadResponse.LogCount,m_DataModel.rwReadResponse.LogInfo);//读取历史记录数据
				}

				//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
				//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();// 启动充值读写器监控
				//}
				m_pDialogFlow->ShowNextDialogGroup(WM_ANALYZE_WAIT_TO_BASEM1);

			}
					
		}
	}
	catch (CSysException& e){
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CInnerException e(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      出错处理
//
//@param      （i）CString errorMsg  错误信息文言
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::OnError(CString strMessage)
{
	/*theSERVICE_MGR.SetState(FINISH);
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_WAIT){
		DoDialogFlow(WM_ANALYZE_WAIT_TO_ERROR);
	}
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_BASE){
		DoDialogFlow(WM_ANALYZE_BASE_TO_ERROR);
	}
	theAPP_SESSION.ShowOfficeGuide(strMessage);*/
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      鎖卡
//
//@param      (i)RW_BUSSINESS_RESPONSE	response	锁卡反馈数据
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
long CAnalyzeSvc::BlockCard()
{

	UINT resultCode;
	RW_BUSSINESS_RESPONSE response;	
	try{	
		LOG("黑名单锁卡。");
		RW_CMD_BLOCK rw_cmd_block;
		rw_cmd_block.SerialNo = m_pCommonModel->dwUDSN;
		rw_cmd_block.TranDateTime = m_pCommonModel->transTime;
		resultCode = RECHARGERW_HELPER->Block(rw_cmd_block,response);
		// 锁卡成功
		if(resultCode == RW_RSP_OK) {
			LOG("锁卡成功！");
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			// 更新UD数据
			if(response.UDlen != 0) {
				RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
			}
			// 更新AR数据
			if(response.ARLen != 0) {
				RECHARGERW_HELPER->SaveRWARData(response.ARLen,response.ARData);
			}
		}
	}
	catch(...)	{	
		CString txtGuideInfo = (resultCode==RW_RSP_OK)?_tl(GUIDE_CHARGE_TPU_ERROR_BLACKCARD):RECHARGERW_HELPER->GetTpuErrorInfo(response.DetailErrorCode);
		/*OnError(txtGuideInfo, GUIDE_CHARGE_TPU_ERROR_BLACKCARD);*/
	}
	return resultCode;
}
//
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得服务中的数据模型
//
//@param      
//
//@retval     
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::DataModel& CAnalyzeSvc::GetDataModel()
{
	return m_DataModel;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      引发数据模型改变事件
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}
//
////////////////////////////////////////////////////////////////////////////
///*/**
//@brief      打印查询成功歷史信息
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::PrinterPrint()
{
	/*CString msg;
	msg.Format(_tl(GUIDE_ANALYZE_SURE_PRINT));
	INT_PTR confirmResult = CMessageBox::Show(_tl(TITLE_HISTORY_TITLENAME),msg,CMessageBox::BTN_OKCANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_30);
	if(confirmResult == IDCANCEL){
		return;
	}
	if(m_DataModel.commandExecuteResult == SP_SUCCESS){
		PrintQuerySuccessHistory();
	}
	else{
		PrintQuerFailHistory();
	}*/
}
//
////////////////////////////////////////////////////////////////////////////
///*/**
//@brief      打印查询成功歷史信息
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::PrintQuerySuccessHistory()
{
	//try {
	//	
	//	CString templatetransName= _T("TRANCE_HISTORY.template");
	//	CStringArray printTransTexts;
	//	theTEMPLATE_HELPER.LoadBillTemplate(templatetransName,printTransTexts);
	//
	//	// 票卡编号
	//	CString strTicketNum;
	//	strTicketNum.Format(_T("%.9u"),m_DataModel.accCard->applicationData.CardLogicalNumber);

	//	for(int i=0;i<printTransTexts.GetCount();i++)
	//	{
	//		CString& printTransText = printTransTexts.ElementAt(i);		
	//		vector<HistoryProductInfo>::iterator iter = m_DataModel.result.begin();
	//		for(iter = m_DataModel.result.begin();iter != m_DataModel.result.end();iter++){
	//			printTransText.Replace(_T("{Product_category}"),add_hsp_right(theTICKET_HELPER.GetCategoryName((ProductCategory_t)iter->ProductType),20));
	//		}
	//		vector<HistoryTranceInfo>::iterator it = iter->TransactionInfo.begin();
	//		for(it = iter->TransactionInfo.begin();it != iter->TransactionInfo.end();it++){
	//			CString strNum;
	//			long num = 1;
	//			strNum.Format(_T("%d"),num++);
	//			printTransText.Replace(_T("{NO.}"),strNum);
	//			printTransText.Replace(_T("{TIME}"),add_hsp_right(it->TransactionDateTime.ToString(_T("%.4d-%.2d-%.2d %02d:%02d")),14));
	//			printTransText.Replace(_T("{Transaction_type}"),add_hsp_right(theTICKET_HELPER.GetTransactionType((TRANSACTION_TYPE)it->TransactionType)==_T("")? _tl(TXT_UNKNOW_STATUS) : theTVM_INFO.GetTransactionType((TRANSACTION_TYPE)it->TransactionType),14));
	//			printTransText.Replace(_T("{Location}"),add_hsp_right(theACC_LOCATION.GetLocationName(it->TransactionStation,theMAINTENANCE_INFO.GetLanguageMode();),20));
	//			printTransText.Replace(_T("{Payment_method}"),theACC_BUSINESS.GetPayTypeName(it->PaymentMethod,theMAINTENANCE_INFO.GetLanguageMode();));
	//			printTransText.Replace(_T("{Amount}"),ComFormatAmount(it->TransactionValue));
	//			printTransText.Replace(_T("{Balance}"),ComFormatAmount(it->RemainingValue));
	//		}
	//	}



	//	
	//	CString templateName= _T("HISTORY.template");
	//	CStringArray printTexts;
	//	theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
	//	for(int i=0;i<printTransTexts.GetCount();i++){
	//		CString& line = printTexts.ElementAt(i);
	//		// 表头
	//		line.Replace(_T("{Personal_Transaction_History_Report}"),_tl(TITLE_HISTORY_TITLENAME));
	//		//姓名
	//		line.Replace(_T("{姓名}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		line.Replace(_T("{Name}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		// 车票编号
	//		line.Replace(_T("{票卡编号}"),_tl(TXT_TICKET_NUMBER));
	//		line.Replace(_T("{Ticket_ID}"),strTicketNum);
	//		// 打印日期
	//		line.Replace(_T("{打印日期}"),_tl(TXT_PRINT_DATE));
	//		line.Replace(_T("{Print_Date}"),m_DataModel.printTime.ToString(_T("%.4d-%.2d-%.2d %02d:%02d")));
	//		// 交易时间
	//		line.Replace(_T("{时间}"),_tl(TXT_TRANS_TIME));			
	//		// 产品种类
	//		line.Replace(_T("{产品种类}"),_tl(TXT_PRODUCT_CATEGORY));
	//		// 交易类型
	//		line.Replace(_T("{交易类别}"),_tl(TXT_TRANSACTION0X_TYPE));
	//		// 交易车站
	//		line.Replace(_T("{车站}"),_tl(TXT_STATION_NAME));
	//		// 支付方式
	//		line.Replace(_T("{支付方式}"),_tl(TXT_PAYMENT_METHOD));
	//		// 交易金额
	//		line.Replace(_T("{交易金额}"),_tl(TXT_TRANS_MONEY));
	//		// 余额
	//		line.Replace(_T("{余额}"),_tl(TXT_TICKET_BALANCE));
	//		// 客户签名
	//		line.Replace(_T("{客户签名}"),_tl(TXT_GUEST_SIGNATURE));
	//		if(line.Find(_T("{ADD}"))!=-1){
	//			printTexts.RemoveAt(i);
	//			printTexts.InsertAt(i,&printTransTexts);
	//			i--;	
	//			continue;
	//		}
	//		ComPrintLineFormatForTemplate(line);
	//	}

	//	// 镭射打印机接口
	//	//m_pLaserPrinterHelper->PrintText(&printTexts,_T("历史记录"));   
	//}
	//catch (CSysException& e) {
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}

}
//
////////////////////////////////////////////////////////////////////////////
///*/**
//@brief      打印查询失败歷史信息
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::PrintQuerFailHistory()
{
	//try {
	//	
	//	CString templateName= _T("FAILURE_HISTORY.template");
	//	CStringArray printTexts;
	//	theTEMPLATE_HELPER.LoadBillTemplate(templateName,printTexts);
	//	int i = 0;
	//	CACCCard::CARD_HISTORY_DATA histroy[9];
	//	// 票卡编号
	//	CString strTicketNum;
	//	strTicketNum.Format(_T("%.9u"),m_DataModel.accCard->applicationData.CardLogicalNumber);

	//	while(i < printTexts.GetCount())
	//	{
	//		CString& line = printTexts.ElementAt(i);
	//		// 表头
	//		line.Replace(_T("{Personal_Transaction_History_Report}"),_tl(TITLE_HISTORY_TITLENAME));
	//		//姓名
	//		line.Replace(_T("{姓名}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		line.Replace(_T("{Name}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		// 车票编号
	//		line.Replace(_T("{票卡编号}"),_tl(TXT_TICKET_NUMBER));
	//		line.Replace(_T("{Ticket_ID}"),strTicketNum);
	//		// 打印日期
	//		line.Replace(_T("{打印日期}"),_tl(TXT_PRINT_DATE));
	//		line.Replace(_T("{Print_Date}"),m_DataModel.printTime.ToString(_T("%.4d-%.2d-%.2d %02d:%02d")));
	//		// 交易时间
	//		line.Replace(_T("{时间}"),_tl(TXT_TRANS_TIME));			
	//		// 产品种类
	//		line.Replace(_T("{产品种类}"),_tl(TXT_PRODUCT_CATEGORY));
	//		// 交易类型
	//		line.Replace(_T("{交易类别}"),_tl(TXT_TRANSACTION0X_TYPE));
	//		// 交易车站
	//		line.Replace(_T("{车站}"),_tl(TXT_STATION_NAME));
	//		// 支付方式
	//		line.Replace(_T("{支付方式}"),_tl(TXT_PAYMENT_METHOD));
	//		// 交易金额
	//		line.Replace(_T("{交易金额}"),_tl(TXT_TRANS_MONEY));
	//		// 余额
	//		line.Replace(_T("{余额}"),_tl(TXT_TICKET_BALANCE));
	//		// 客户签名
	//		line.Replace(_T("{客户签名}"),_tl(TXT_GUEST_SIGNATURE));			
	//		
	//		if ((i-7)>=0&&(i<16)){
	//			line.Replace(_T("{TIME}"),add_hsp_right(histroy[i-7].TransTime.ToString(_T("%.4d-%.2d-%.2d %02d:%02d")),14));
	//			line.Replace(_T("{Product_category}"),add_hsp_right(theTICKET_HELPER.GetCategoryName((ProductCategory_t)histroy[i-7].category),20));
	//			line.Replace(_T("{Transaction_type}"),add_hsp_right(theTICKET_HELPER.GetTransactionType((TRANSACTION_TYPE)histroy[i-7].TransType)==_T("")? _tl(TXT_UNKNOW_STATUS) : theTVM_INFO.GetTransactionType((TRANSACTION_TYPE)histroy[i-7].TransType),14));
	//			line.Replace(_T("{Location}"),add_hsp_right(theACC_LOCATION.GetLocationName(histroy[i-7].transStation,theMAINTENANCE_INFO.GetLanguageMode();),20));
	//			line.Replace(_T("{Payment_method}"),theACC_BUSINESS.GetPayTypeName(histroy[i-7].paymentMethod,theMAINTENANCE_INFO.GetLanguageMode();));
	//			line.Replace(_T("{Amount}"),ComFormatAmount(histroy[i-7].TransValue));
	//			line.Replace(_T("{Balance}"),ComFormatAmount(histroy[i-7].remaingValue));
	//		}
	//				
	//		i++;
	//	}
	//	
	//	// 镭射打印机接口
	//	//m_pLaserPrinterHelper->PrintText(&printTexts,_tl(TITLE_HISTORY_TITLENAME));   
	//}
	//catch (CSysException& e) {
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      请求历史数据
//
//@param      无
//
//@retval     bool	true:成功	false:失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
bool CAnalyzeSvc::RequestHistoryData()
{
	//try {
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_PERSONAL_REQUEST_DOING));

	//	// 判断连接CCS服务器
	//	if(!theAPP_SESSION.IsSCConnected()){
	//		return false;
	//	}

	//	// 请求查询数据
	//	HistoryInfoQuery query;
	//	query.StartDate = m_DataModel.StartDate;	// 开始时间
	//	query.EndDate = m_DataModel.EndDate;		// 结束时间
	//	vector<HistoryProductInfo>::iterator iter = m_DataModel.result.begin();
	//	for(iter=m_DataModel.result.begin();iter!=m_DataModel.result.end();iter++){
	//		query.ProductInfo->ProductType = iter->ProductType;				// 产品类型
	//		query.ProductInfo->ProductVariantType= iter->ProductVariantType;    // 产品子类型
	//	}
	//	// 将结果保存m_DataModel.result
	//	m_DataModel.commandExecuteResult = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestHistoryInfo(query, m_DataModel.result);
	//	if (SP_SUCCESS!=m_DataModel.commandExecuteResult){
	//		return false;
	//	}
	//	
	//	// 遍历保存结果按降序排列
	//	vector<HistoryProductInfo>::iterator iterHistoryProductInfo =  m_DataModel.result.begin();
	//	for(iterHistoryProductInfo= m_DataModel.result.begin();iterHistoryProductInfo!= m_DataModel.result.end();iterHistoryProductInfo++){	
	//		// 在HistoryTranceInfo中重载操作符，然后降序排列
	//		sort(iterHistoryProductInfo->TransactionInfo.begin(),iterHistoryProductInfo->TransactionInfo.end(), greater<HistoryTranceInfo>());
	//	}	

	//}
	//catch (CSysException& e) {
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}

	return true;
}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      请求乘客数据
//
//@param      无
//
//@retval     bool	true:成功	false:失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
bool CAnalyzeSvc::RequestUserInfoData()
{
	//try {
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_PERSONAL_REQUEST_DOING));

	//	// 连接CCS服务器
	//	if(!theAPP_SESSION.IsSCConnected()){
	//		return false;
	//	}

	//	// 证件号
	//	char certificateCardNumber[10];
	//	BCD2PSTR((char*)m_DataModel.accCard->userInfo.certificateCardNumber,CARD_CERTIFICATE_ID_LEN,certificateCardNumber);
	//	USES_CONVERSION;
	//	CString strCertificateCardNumber = A2T(certificateCardNumber);

	//	// 请求数据
	//	vector<Personalization> result;
	//	PersonalizationQuery query;
	//	
	//	query.cardholderNumberType = m_DataModel.accCard->userInfo.certificateCardType;					// 证件类型
	//	memcpy(query.cardholderNumber, strCertificateCardNumber.GetBuffer(), 19);	// 证件编号	
	//	long resultCode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestPersonlization(query, result);
	//	if (resultCode != SP_SUCCESS) {
	//		return false;
	//	}

	//	// 保存到本地		
	//	vector<Personalization>::iterator iter = result.begin();
	//	m_DataModel.passengerName = iter->cardHolderName;
	//	m_DataModel.passengerGender = iter->sex;
	//	m_DataModel.passengerBirthday.Format(_T("%.4d-%.2d-%.2d"), iter->dateOfBirth.wYear, iter->dateOfBirth.biMonth, iter->dateOfBirth.biDay);
	//	m_DataModel.passengerPhone = iter->cardholderPhoneDay;
	//	m_DataModel.passengerEmail = iter->cardholderEmail;
	//	m_DataModel.passengerAddress = iter->address1;
	//}
	//catch (CSysException& e) {
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	return true;
}
//
////////////////////////////////////////////////////////////////////////////
///*/**
//@brief		打印时字符串的格式化处理（模板读取数据）   
//
//@param      (i)const CString PrintLine		打印行字符串
//
//@retval     CStringArray 格式化后的字符串
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
CString CAnalyzeSvc::ComPrintLineFormatForTemplate(CString& PrintLine)
{
	CStringArray arryPrintLines;
	CString strRight(_T(""));
	CString strMid(_T(""));
	CString strLine = PrintLine;

	//int iPos = 0;
	//// 拆分数据
	//// 三列数据
	//if (strLine.FindOneOf(_T("$"))!=-1){
	//	while((iPos=strLine.Find(_T("$")))!=-1){
	//		arryPrintLines.Add(strLine.Left(iPos));
	//		strLine.Delete(0,iPos+1);
	//	}
	//	arryPrintLines.Add(strLine);	
	//}
	//
	//// 单列数据不处理
	//else{
	//	return PrintLine;
	//}
	//// 格式化数据项
	//for(int i=0;i<arryPrintLines.GetCount();i++){
	//	CString& line = arryPrintLines.ElementAt(i);	
	//	int ChrSize = 0;
	//	for(int i=0;i<line.GetLength();i++){			
	//		ChrSize += line.GetAt(i)>=0x80?2:1;
	//	}					
	//	PrintLine += add_hsp(line.Left(140/arryPrintLines.GetCount()), (140-ChrSize)/arryPrintLines.GetCount());
	//}
	return PrintLine;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      获取M1卡个人信息
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::GetM1PrivateInfo()
{
	m_DataModel.userInfo.strName = m_DataModel.accCard->userInfo.name;
	m_DataModel.userInfo.strIdentityCardType.Format(_T("%c"), m_DataModel.accCard->userInfo.certificateCardType);
	m_DataModel.userInfo.strIdentityCardNum = m_DataModel.accCard->userInfo.certificateCardNumber;
}



COIN_ACCEPTABLE CAnalyzeSvc:: GetAcceptCoinFaceTable() //获取可接收的硬币的面额
{
	COIN_ACCEPTABLE coin;
	return coin;
}

BRH_FACE_ACEPTED_TABLE CAnalyzeSvc::GetAcceptBanknotesFace(tBncSetInhibitList* Table)// 获取可接收的纸币的面额
{
	BRH_FACE_ACEPTED_TABLE banknote;
	return banknote;
}
