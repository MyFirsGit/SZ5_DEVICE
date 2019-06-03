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
@brief      ���캯��

@param      (i)CService/** pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::CAnalyzeSvc() : CPassengerForeSvc(ANALYZE_SVC)
{
    // ���ó˿ͻ���
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_10805_ANALYZE_BASEM1_DLG, new CBaseM1InfoDlg(this));
	
    // ���û������
    // �ȴ�������
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PUT_CARD)); //
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE_M1,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE_M1, IDD_10805_ANALYZE_BASEM1_DLG);


    // ���û�����ת
	/*m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ANALYZE_WAIT_TO_BASEUL, DIALOG_GROUP_BASE_UL);*/
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ANALYZE_WAIT_TO_BASEM1, DIALOG_GROUP_BASE_M1);

	// ���ó�ʼ������
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);

	m_DataModel = DataModel();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

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
@brief      ��������ʱ��Ӧ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::OnStart()
{
	__super::OnStart();

	// ���ó�ʼ������
    //m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
	// ���ع�������ť��������ʾ��ť
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	/*m_DataModel.accCard = NULL;*/
	m_DataModel = DataModel();
	// ���ó�ʼ������
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
	IO_HELPER->OpenChargeIndicator();
	m_dwTime = GetTickCount();
	CAnalyzeSvc::m_IsAbort = false;
}
 

////////////////////////////////////////////////////////////////////////////
///**
//@brief      �x���������̎��
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
				// // ȡ�����˳�����,����Ĭ��ҵ��
				LOG("���������޿�,����Ĭ��ҵ��");
				theSERVICE_MGR.SetState(RECEPT);
				CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
				NOTIFY_PROCESSING_WINDOW();
				pMainSvc->StartDefaultService();	
				return;
			}

			if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {
				//BlockCard(); // ������Ʊ������
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(TXT_MSG_GUIDE_THIS_CARD_HAS_BEEN_LOCKED),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				// ��������
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
				if(IDOK == ret || IDABORT == ret)		//���ȷ�ϣ�����ʱ�����
				{
					NOTIFY_PROCESSING_WINDOW();
					RECHARGERW_HELPER->StopReadCard();	//ֹͣ����,�ص�Ĭ��ҵ�񣬼�����ѯ
					CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
					pMainSvc->StartDefaultService();
				}
			}
		}
		else
		{
			//IO_HELPER->CloseChargeIndicator();

			if (m_DataModel.rwReadResponse.ProductCount <= 0) // (�ѳ�ʼ�����Ŀտ�)�޲�Ʒ����Ĭ��ҵ��
			{
				CMessagePictureBox::End();
				int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();
				INT_PTR ret = CMessagePictureBox::Show(	theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),
					_tl(TXT_PSG_TICKET_CANT_RECOGNIZE),
					TVM_TIP_WARINGS,
					CMessagePictureBox::BTN_OK,DT_LEFT,
					iReadTimeLongth);
				if(IDOK == ret || IDABORT == ret)		//���ȡ��������ʱ�����
				{
					RECHARGERW_HELPER->StopReadCard();	//ֹͣ����,�ص�Ĭ��ҵ�񣬼�����ѯ
					CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
					pMainSvc->StartDefaultService();
				}
			}
			else{
				//m_DataModel = DataModel();
				delete m_DataModel.accCard;
				m_DataModel.accCard = NULL;												// ָ���ÿ�
				m_DataModel.accCard = new CACCCard;
				m_DataModel.accCard->GetCardInfo(m_DataModel.rwReadResponse.CardInfo); //���������������ݣ���ȡƱ������

				if(CARD_TYPE_SY_EPT_UL != m_DataModel.rwReadResponse.CardPHType){
					m_DataModel.accCard->GetHistoryInfo(m_DataModel.rwReadResponse.LogCount,m_DataModel.rwReadResponse.LogInfo);//��ȡ��ʷ��¼����
				}

				//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
				//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();// ������ֵ��д�����
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
//@brief      ������
//
//@param      ��i��CString errorMsg  ������Ϣ����
//
//@retval     ��
//
//@exception  ��
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
//@brief      �i��
//
//@param      (i)RW_BUSSINESS_RESPONSE	response	������������
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
		LOG("������������");
		RW_CMD_BLOCK rw_cmd_block;
		rw_cmd_block.SerialNo = m_pCommonModel->dwUDSN;
		rw_cmd_block.TranDateTime = m_pCommonModel->transTime;
		resultCode = RECHARGERW_HELPER->Block(rw_cmd_block,response);
		// �����ɹ�
		if(resultCode == RW_RSP_OK) {
			LOG("�����ɹ���");
			// ���½�����ˮ��
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			// ����UD����
			if(response.UDlen != 0) {
				RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
			}
			// ����AR����
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
//@brief      ȡ�÷����е�����ģ��
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
//@brief      ��������ģ�͸ı��¼�
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}
//
////////////////////////////////////////////////////////////////////////////
///*/**
//@brief      ��ӡ��ѯ�ɹ��vʷ��Ϣ
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
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
//@brief      ��ӡ��ѯ�ɹ��vʷ��Ϣ
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
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
	//	// Ʊ�����
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
	//		// ��ͷ
	//		line.Replace(_T("{Personal_Transaction_History_Report}"),_tl(TITLE_HISTORY_TITLENAME));
	//		//����
	//		line.Replace(_T("{����}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		line.Replace(_T("{Name}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		// ��Ʊ���
	//		line.Replace(_T("{Ʊ�����}"),_tl(TXT_TICKET_NUMBER));
	//		line.Replace(_T("{Ticket_ID}"),strTicketNum);
	//		// ��ӡ����
	//		line.Replace(_T("{��ӡ����}"),_tl(TXT_PRINT_DATE));
	//		line.Replace(_T("{Print_Date}"),m_DataModel.printTime.ToString(_T("%.4d-%.2d-%.2d %02d:%02d")));
	//		// ����ʱ��
	//		line.Replace(_T("{ʱ��}"),_tl(TXT_TRANS_TIME));			
	//		// ��Ʒ����
	//		line.Replace(_T("{��Ʒ����}"),_tl(TXT_PRODUCT_CATEGORY));
	//		// ��������
	//		line.Replace(_T("{�������}"),_tl(TXT_TRANSACTION0X_TYPE));
	//		// ���׳�վ
	//		line.Replace(_T("{��վ}"),_tl(TXT_STATION_NAME));
	//		// ֧����ʽ
	//		line.Replace(_T("{֧����ʽ}"),_tl(TXT_PAYMENT_METHOD));
	//		// ���׽��
	//		line.Replace(_T("{���׽��}"),_tl(TXT_TRANS_MONEY));
	//		// ���
	//		line.Replace(_T("{���}"),_tl(TXT_TICKET_BALANCE));
	//		// �ͻ�ǩ��
	//		line.Replace(_T("{�ͻ�ǩ��}"),_tl(TXT_GUEST_SIGNATURE));
	//		if(line.Find(_T("{ADD}"))!=-1){
	//			printTexts.RemoveAt(i);
	//			printTexts.InsertAt(i,&printTransTexts);
	//			i--;	
	//			continue;
	//		}
	//		ComPrintLineFormatForTemplate(line);
	//	}

	//	// �����ӡ���ӿ�
	//	//m_pLaserPrinterHelper->PrintText(&printTexts,_T("��ʷ��¼"));   
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
//@brief      ��ӡ��ѯʧ�ܚvʷ��Ϣ
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
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
	//	// Ʊ�����
	//	CString strTicketNum;
	//	strTicketNum.Format(_T("%.9u"),m_DataModel.accCard->applicationData.CardLogicalNumber);

	//	while(i < printTexts.GetCount())
	//	{
	//		CString& line = printTexts.ElementAt(i);
	//		// ��ͷ
	//		line.Replace(_T("{Personal_Transaction_History_Report}"),_tl(TITLE_HISTORY_TITLENAME));
	//		//����
	//		line.Replace(_T("{����}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		line.Replace(_T("{Name}"),_tl(TXT_PERSONAL_PASSENGER_NAME));
	//		// ��Ʊ���
	//		line.Replace(_T("{Ʊ�����}"),_tl(TXT_TICKET_NUMBER));
	//		line.Replace(_T("{Ticket_ID}"),strTicketNum);
	//		// ��ӡ����
	//		line.Replace(_T("{��ӡ����}"),_tl(TXT_PRINT_DATE));
	//		line.Replace(_T("{Print_Date}"),m_DataModel.printTime.ToString(_T("%.4d-%.2d-%.2d %02d:%02d")));
	//		// ����ʱ��
	//		line.Replace(_T("{ʱ��}"),_tl(TXT_TRANS_TIME));			
	//		// ��Ʒ����
	//		line.Replace(_T("{��Ʒ����}"),_tl(TXT_PRODUCT_CATEGORY));
	//		// ��������
	//		line.Replace(_T("{�������}"),_tl(TXT_TRANSACTION0X_TYPE));
	//		// ���׳�վ
	//		line.Replace(_T("{��վ}"),_tl(TXT_STATION_NAME));
	//		// ֧����ʽ
	//		line.Replace(_T("{֧����ʽ}"),_tl(TXT_PAYMENT_METHOD));
	//		// ���׽��
	//		line.Replace(_T("{���׽��}"),_tl(TXT_TRANS_MONEY));
	//		// ���
	//		line.Replace(_T("{���}"),_tl(TXT_TICKET_BALANCE));
	//		// �ͻ�ǩ��
	//		line.Replace(_T("{�ͻ�ǩ��}"),_tl(TXT_GUEST_SIGNATURE));			
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
	//	// �����ӡ���ӿ�
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
//@brief      ������ʷ����
//
//@param      ��
//
//@retval     bool	true:�ɹ�	false:ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CAnalyzeSvc::RequestHistoryData()
{
	//try {
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_PERSONAL_REQUEST_DOING));

	//	// �ж�����CCS������
	//	if(!theAPP_SESSION.IsSCConnected()){
	//		return false;
	//	}

	//	// �����ѯ����
	//	HistoryInfoQuery query;
	//	query.StartDate = m_DataModel.StartDate;	// ��ʼʱ��
	//	query.EndDate = m_DataModel.EndDate;		// ����ʱ��
	//	vector<HistoryProductInfo>::iterator iter = m_DataModel.result.begin();
	//	for(iter=m_DataModel.result.begin();iter!=m_DataModel.result.end();iter++){
	//		query.ProductInfo->ProductType = iter->ProductType;				// ��Ʒ����
	//		query.ProductInfo->ProductVariantType= iter->ProductVariantType;    // ��Ʒ������
	//	}
	//	// ���������m_DataModel.result
	//	m_DataModel.commandExecuteResult = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestHistoryInfo(query, m_DataModel.result);
	//	if (SP_SUCCESS!=m_DataModel.commandExecuteResult){
	//		return false;
	//	}
	//	
	//	// ��������������������
	//	vector<HistoryProductInfo>::iterator iterHistoryProductInfo =  m_DataModel.result.begin();
	//	for(iterHistoryProductInfo= m_DataModel.result.begin();iterHistoryProductInfo!= m_DataModel.result.end();iterHistoryProductInfo++){	
	//		// ��HistoryTranceInfo�����ز�������Ȼ��������
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
//@brief      ����˿�����
//
//@param      ��
//
//@retval     bool	true:�ɹ�	false:ʧ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CAnalyzeSvc::RequestUserInfoData()
{
	//try {
	//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_PERSONAL_REQUEST_DOING));

	//	// ����CCS������
	//	if(!theAPP_SESSION.IsSCConnected()){
	//		return false;
	//	}

	//	// ֤����
	//	char certificateCardNumber[10];
	//	BCD2PSTR((char*)m_DataModel.accCard->userInfo.certificateCardNumber,CARD_CERTIFICATE_ID_LEN,certificateCardNumber);
	//	USES_CONVERSION;
	//	CString strCertificateCardNumber = A2T(certificateCardNumber);

	//	// ��������
	//	vector<Personalization> result;
	//	PersonalizationQuery query;
	//	
	//	query.cardholderNumberType = m_DataModel.accCard->userInfo.certificateCardType;					// ֤������
	//	memcpy(query.cardholderNumber, strCertificateCardNumber.GetBuffer(), 19);	// ֤�����	
	//	long resultCode = theSERVICE_MGR.GetService<CServerClientSvc>(SC_SVC)->RequestPersonlization(query, result);
	//	if (resultCode != SP_SUCCESS) {
	//		return false;
	//	}

	//	// ���浽����		
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
//@brief		��ӡʱ�ַ����ĸ�ʽ������ģ���ȡ���ݣ�   
//
//@param      (i)const CString PrintLine		��ӡ���ַ���
//
//@retval     CStringArray ��ʽ������ַ���
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
	//// �������
	//// ��������
	//if (strLine.FindOneOf(_T("$"))!=-1){
	//	while((iPos=strLine.Find(_T("$")))!=-1){
	//		arryPrintLines.Add(strLine.Left(iPos));
	//		strLine.Delete(0,iPos+1);
	//	}
	//	arryPrintLines.Add(strLine);	
	//}
	//
	//// �������ݲ�����
	//else{
	//	return PrintLine;
	//}
	//// ��ʽ��������
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
//@brief      ��ȡM1��������Ϣ
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::GetM1PrivateInfo()
{
	m_DataModel.userInfo.strName = m_DataModel.accCard->userInfo.name;
	m_DataModel.userInfo.strIdentityCardType.Format(_T("%c"), m_DataModel.accCard->userInfo.certificateCardType);
	m_DataModel.userInfo.strIdentityCardNum = m_DataModel.accCard->userInfo.certificateCardNumber;
}



COIN_ACCEPTABLE CAnalyzeSvc:: GetAcceptCoinFaceTable() //��ȡ�ɽ��յ�Ӳ�ҵ����
{
	COIN_ACCEPTABLE coin;
	return coin;
}

BRH_FACE_ACEPTED_TABLE CAnalyzeSvc::GetAcceptBanknotesFace(tBncSetInhibitList* Table)// ��ȡ�ɽ��յ�ֽ�ҵ����
{
	BRH_FACE_ACEPTED_TABLE banknote;
	return banknote;
}
