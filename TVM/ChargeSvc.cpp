#include "StdAfx.h"

#include "ChargeSvc.h"

#include "WaitingOfficeDlg.h"
#include "ChargeReceptionDlg.h"
#include "PassengerResultDlg.h"

#include "TicketHelper.h"
#include "TemplateHelper.h"
#include "IOMonitorSvc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CChargeSvc, CPassengerForeSvc)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeSvc::CChargeSvc():CPassengerForeSvc(CHARGE_SVC)
{
	LOG("��ֵ���캯��");
	m_pChargeAudioHelper = new CChargeAudioHelper(*this);
	m_pDataModel = new DataModel();
	m_pCommonModel = dynamic_cast<CommonModel*>(m_pDataModel);
	// ���û���
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG,new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_00301_CHARGE_RECEPT_DLG, new CChargeReceptionDlg(this));
	m_pDialogFlow->AddDialog(IDD_00303_CHARGE_RESULT_DLG, new CPassengerResultDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INIT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR);
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_00303_CHARGE_RESULT_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_00301_CHARGE_RECEPT_DLG);


	// ���û�����ת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_INIT,DIALOG_GROUP_INIT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_ERROR,DIALOG_GROUP_ERROR);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT); // should be this one

	m_bReaderBad = false;		// ��д���Ƿ����
	LOG("��ֵ���캯������!");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeSvc::~CChargeSvc(void)
{
	delete m_pDataModel;
	m_pDataModel = NULL;
	m_pCommonModel = NULL;
	if(m_pChargeAudioHelper != NULL){
		delete m_pChargeAudioHelper;
		m_pChargeAudioHelper = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ʱ��Ӧ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::OnStart()
{
	// ��ʼ��
	InitDataModel();

	// ä�˹�Ʊ���õ�ǰ״̬
	if(theAPP_SESSION.IsBlindManOperation()){
		// ä�˹�Ʊ����MainSvc��ʱ�������水ť
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->OnClickButtonWithoutPinPad();
		m_operation_status = STATUS_BLIND_NORMAL;
	}
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT); // should be this one
	m_bReaderBad = false;		// ��д���Ƿ����
	StartTimer(TIMERS_WAIT_CARD_INSERT,1);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeSvc::DataModel& CChargeSvc::GetDataModel()
{
	return *m_pDataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���ڲ�Ʒ��Ϣ

@param      (i)WORD productType ��Ʒ����
@param      (i)WORD productSubType ��Ʒ������

@retval     TICKET_PRODUCT&

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::GetCurrentProductInfo(WORD productType, WORD productSubType, PRODUCT_INFO& ticketProduct)
{
	for (vector<PRODUCT_INFO>::iterator iter = m_pDataModel->vecTicketProduct.begin(); iter != m_pDataModel->vecTicketProduct.end(); ++iter) {
		if (iter->productType == productType && iter->productSubType == productSubType) {
			ticketProduct = *iter;
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ƷƱ��

@param		(o)long& price Ʊ��

@retval     bool	true:�ɹ�	false:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::CalculatePrice() {
	long resultCode = -1;	
	// ����Ǵ�ֵƱ������ͨ��TPU����Ʊ��
	if (m_pDataModel->productInfo.productCategory == PURSE) {
		m_pDataModel->paymentInfo.lPriceAmount = static_cast<long>(m_pDataModel->lAddAmount);
		//����˰��
		RW_CMD_TICKET_TAX taxInput;
		RW_CMD_TICKET_TAX_RES taxRsp;
		taxInput.CardType = m_pCommonModel->mediumInfo.cardPHType;
		taxInput.SaleOrAddAmount = m_pDataModel->lAddAmount;
		try{
			resultCode = RECHARGERW_HELPER->GetTicketPriceTax(taxInput,taxRsp);
			m_pCommonModel->paymentInfo.lTaxAmount = taxRsp.TicketTax;
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		//m_pCommonModel->paymentInfo.lTaxAmount = theACC_BUSINESS.GetCardHolderFee(PhyCardType_t::CARD_TYPE_MIFARE_1_S70,CARDHOLDER_FEE_TYPE_TAX,m_pDataModel->paymentInfo.lPriceAmount);
		return resultCode;
	}

	// ��������
	return __super::CalculatePrice();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ�����ݵ�DataModel

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW����������Ϣ

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	DoDialogFlow(WM_WAIT_TO_INIT);
	return 0;

	CACCCard accCard;
	
	// ����Ʊ��
	if(!accCard.GetCardInfo(pReadCardResponse->CardInfo)) {
		LOG("����Ʊ����Ϣ����");
		//__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
		return -1;
	}	
	
	// Ʊ���Ļ�����Ϣ
	CString strCardNum = ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8);
	if (m_CardLogicalNumber==_T("")){//���ζ���
		m_CardLogicalNumber = strCardNum;
		m_pDataModel->mediumInfo.cardLogicalNumber = strCardNum;
		m_pDataModel->mediumCardInfo.cardLogicalNumber = m_pDataModel->mediumInfo.cardLogicalNumber;
		m_pDataModel->mediumInfo.mediumType = (CARD_MEDIUM_TYPE)pReadCardResponse->CardPHType;	//theTICKET_HELPER.GetMediumType(m_pDataModel->mediumInfo.cardPHType);
		m_pDataModel->mediumInfo.passengerType = accCard.applicationData.byPassengerType;		//�˿����ͣ��ݲ��ã�
	}
	else{
		if(m_CardLogicalNumber!=strCardNum){// ��ͬһ�ſ�
			m_strReadCardRsp = _T("��ֵ��;����Ʊ��");
			return -1;
		}
		else{
			m_CardLogicalNumber = strCardNum;
			m_pDataModel->mediumInfo.cardLogicalNumber = strCardNum;
			m_pDataModel->mediumCardInfo.cardLogicalNumber = m_pDataModel->mediumInfo.cardLogicalNumber;
			m_pDataModel->mediumInfo.mediumType = (CARD_MEDIUM_TYPE)pReadCardResponse->CardPHType;	//theTICKET_HELPER.GetMediumType(m_pDataModel->mediumInfo.cardPHType);
			m_pDataModel->mediumInfo.passengerType = accCard.applicationData.byPassengerType;		//�˿����ͣ��ݲ��ã�
			m_strReadCardRsp = _T("");
		}
	}

	
	m_pDataModel->mapAvailableProduct.clear();
	m_pDataModel->vecTicketProduct.clear();
	// ���ҿɳ�ֵ��Ʒ
	for (int i=0; i < pReadCardResponse->ProductCount; ++i) {
		if (pReadCardResponse->RechargeProduct[i].RechargeMark) {
			PRODUCT_INFO ticketProduct;
			ticketProduct.productType        = static_cast<ProductType>(pReadCardResponse->RechargeProduct[i].ProductType);
			ticketProduct.productSubType     = pReadCardResponse->RechargeProduct[i].ProductSubType;
			ticketProduct.lProductDeposit    = static_cast<long>(pReadCardResponse->RechargeProduct[i].RefoundMoney); // ���˽��==��ƷѺ��
			m_pDataModel->vecTicketProduct.push_back(ticketProduct);
		}
	}

	// ���û�пɳ�ֵ��Ʒ
	if (m_pDataModel->vecTicketProduct.empty()) {
		LOG("�޿ɳ�ֵ��Ʒ����");
		//__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
		//CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_MSG_GUIDE_THERE_IS_NO_PRODUCT_FOR_RECHARGE),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		//IO_HELPER->CheckAndSendOutCard();lichao
		return -1;
	}

	// ��Ʊ����ϸ��Ϣ
	for (vector<PRODUCT_INFO>::iterator iter = m_pDataModel->vecTicketProduct.begin(); iter != m_pDataModel->vecTicketProduct.end(); ++iter) {
		CACCCard::ACCCARD_PRODUCT_INFO productInfo;
		if (!accCard.GetProductInfo(iter->productType, iter->productSubType, productInfo)) {
			LOG("����Ʊ��ϸ��Ϣ����");
			//__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
			//CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
			//IO_HELPER->CheckAndSendOutCard();lichao
			return -1;
		}
		iter->productCategory = productInfo.productCategory;				// ��Ʒ����
		iter->dtValidateDateTime = productInfo.ValidEndTime;				// ��Ʒ��Ч�ڽ���ʱ��
		//if (iter->productCategory == PURSE) {
		//	iter->lBalanceAmount = productInfo.Balance;						// ��Ʊ���
		//}
		//else if (iter->productCategory == MULTIRIDE) {
		//	iter->lBalanceRides = productInfo.Balance;						// ��Ʊ���
		//}
		iter->lBalanceAmount = productInfo.Balance;	
		iter->beginLocation = productInfo.dwStartStation;					// �������
		iter->endLocation = productInfo.dwEndStation;						// �����յ�
		iter->useArea	  = productInfo.wArea;								// ʹ������		
	}


	m_pDataModel->productInfo = m_pDataModel->vecTicketProduct[0];

	DoDialogFlow(WM_WAIT_TO_INIT);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TPU��ֵ����(��ֵ/�ƴ�/����) 

@param      none

@retval  	true:����ɹ�  false:ʧ����Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::DoCardCharge()
{
	long resultCode = 0;
	try{		
		// ��ʾ�ȴ�������Ϣ��ȴ�ʱ��
		int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();
		// ����״̬Ϊ����������
		theSERVICE_MGR.SetState(ISSUEING);
	
		theSERVICE_MGR.SetForeServiceBusy(true);// ���������쳣���Զ���Ϊfalse��
		//theAudio_HELPER.PlayAudio(AUDIO_ID__WAITINGTRANSACTION);

		RW_CMD_CHARGE chargeInput;
		chargeInput.SerialNo = m_pDataModel->dwUDSN;				// ������ˮ��
		chargeInput.TranDateTime = m_pDataModel->transTime;			// ����ʱ��
		////chargeInput.CardType = static_cast<WORD>(m_pDataModel->mediumInfo.cardPHType); // ��Ϊ��TPU��ض��ǲ�����أ�ʹ��CardType_t
		//chargeInput.ChargeType = static_cast<BYTE>(m_pDataModel->productInfo.productCategory/*m_pDataModel->productInfo.productType*/);
		//chargeInput.ChargeProductType = static_cast<WORD>(m_pDataModel->productInfo.productType);		//��Ʒ������
		//chargeInput.ChargeSubProductType = static_cast<BYTE>(m_pDataModel->productInfo.productSubType); //��Ʒ������

		// �����m_pDataModel->productInfo.productType�жϳ�ֵ������SVT��MST����Ϊ���򰴳�ֵ�Զ�����
		//if (chargeInput.ChargeProductType != SVT){
		//	m_pDataModel->lAddAmount = m_pDataModel->paymentInfo.lDueAmount;
		//}
		m_pDataModel->lAddAmount = m_pDataModel->paymentInfo.lPaidAmount;
		chargeInput.ChargeAmount = static_cast<DWORD>(m_pDataModel->lAddAmount);	// SVT��ֵ���,����MST��PST�Ӳ�Ʒ��Ҫ�յ�Ǯ��
		chargeInput.ChargeCount = static_cast<DWORD>(m_pDataModel->lAddRides);		// MST��ֵ���� 
		// PST�����Ϣ����ͨ��subproductType�Զ��������

		chargeInput.PaymentMethod = static_cast<BYTE>(m_pDataModel->paymentInfo.paymentMethod);		// ֧����ʽ
		if (m_pDataModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
			memcpy(chargeInput.BankCardNum, m_pDataModel->paymentInfo.BankCardNum, 10*sizeof(BYTE));	// ���п���
		}

		RW_BUSSINESS_RESPONSE response;
		resultCode = RECHARGERW_HELPER->Charge(chargeInput,response);

		theSERVICE_MGR.SetForeServiceBusy(false);

		if(resultCode == RW_RSP_OK) {			
			// ���½�����ˮ��
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			// ����UD����
			if(response.UDlen != 0) {
				RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
			}
			// ����AR����
			if(response.ARLen != 0) {
				RECHARGERW_HELPER->SaveRWARData(response.ARLen,response.ARData);//zhengxianle��ֵPSTʱ������ʱ����
			}
		}
		else {
			m_pDataModel->operationInfo.finishType = TRANSACTION_STATE_FAILED;
			m_pDataModel->operationInfo.mapException[TRANSACTION_EXCEPTION_WRITE_CARD_FAILED] = m_pDataModel->paymentInfo.lPaidAmount;
			// ��ֵʧ�ܶ�Ӧ�Ľ���û�ж�Ӧ������ˮ�š�
			m_pDataModel->dwUDSN = 0;
			//m_pDataModel->operationInfo.finishType = WRITE_CARD_FAILED;
		}
		//m_pDataModel->dwUDSN = theCOUNT_INFO.GetUDSN();// ȡֵ����һ
		return resultCode;
	}
	catch (CSysException& e) {
		// �쳣�ڸ��ദ��
		OnError(e);
		//throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ������ˢ���Ƿ���ͬ 

@param      none

@retval  	true:����ɹ�  false:ʧ����Ϣ

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::ConfirmCard(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	theSERVICE_MGR.SetState(ISSUEING);
	// ��ʼ��Ĭ���޿���
	m_enumIsReadedCard = NONE_READ;
	CACCCard accCard;
	if(!accCard.GetCardInfo(pReadCardResponse->CardInfo)) {
		__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
		Sleep(2000);
		if(CMessagePictureBox::Exist()){
			CMessagePictureBox::End();
		}
		m_enumIsReadedCard = NOT_COMPARE;
		RECHARGERW_HELPER->ReadCard((SERVICE_ID)GetServiceID());
		return false;
	}	

	if (m_pDataModel->mediumInfo.cardLogicalNumber != ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8)){
		Sleep(2000);
		if(CMessagePictureBox::Exist()){
			CMessagePictureBox::End();
		}
		m_enumIsReadedCard = NOT_COMPARE;
		RECHARGERW_HELPER->ReadCard((SERVICE_ID)GetServiceID());	
		return false;
	}
	m_enumIsReadedCard = COMPARE;
	CMessagePictureBox::End();
	return true;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ��ֵ����
//
//@param      none
//
//@retval  	bool	true:�ɹ�	false:ʧ��
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//bool CChargeSvc::ChargeCard()
//{
//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_DOING));
//	theAPP_SESSION.ShowReceptionGuide(GUIDE_CHARGE_DOING);
//
//	// �״�д��
//	long resultCode = DoCardCharge();
//	if (resultCode != RW_RSP_OK) {
//		while (true) {
//			// ������Ϣ�����ԶԻ���
//			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(resultCode));
//			INT_PTR result = CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(GUIDE_CHARGE_MSGBOX_RETRY),_T(".\\res\\SJT_issue.png"),CMessagePictureBox::BTN_OKCANCEL,DT_CENTER);
//			if (result != IDOK) {
//				return false;
//			}
//
//			// ���¶���д��
//			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_DOING));
//			RW_READ_CARD_RESPONSE response;
//			resultCode = m_pRWHelper->TryReadCard(response);
//			if (resultCode != RW_RSP_OK) {
//				resultCode = response.DetailErrorCode;
//				continue;
//			}
//			resultCode = DoCardCharge();
//			if (resultCode == RW_RSP_OK) {
//				break;
//			}
//		}
//	}
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SC����

@param      none

@retval  	��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::SaveSCData()
{
	theSCAudit_MGR.addCardRechargeNum(1);
	theSCAudit_MGR.addCardRechargeNum(m_pDataModel->paymentInfo.lDueAmount);

	__super::SaveSCData();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д�����ݿ�����

@param      none

@retval  	��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::WriteDBData(Data::TransactionRecord &dbInfo) {

	// �������ֱ������Sec1.2.2.1.7-9, MACAU-DGB20-0010 AFC Internal Interface(ccs-sc-sle)-Appendix C-Transaction-v00.00.00.03.docx
	dbInfo.iTransactionDetail.iUdType = 4; 
	switch (m_pDataModel->productInfo.productCategory) {
	case PURSE:
		dbInfo.iTransactionDetail.iUdSubtype = 10;
		break;
	case MULTIRIDE:
		dbInfo.iTransactionDetail.iUdSubtype = 12;
		break;
	case PERIOD:
		dbInfo.iTransactionDetail.iUdSubtype = 11;
		break;
	}

	dbInfo.iTransactionDetail.iUnitAmount = m_pDataModel->paymentInfo.lDueAmount; 
	dbInfo.iTransactionDetail.iQuantity = 1; 
	dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = 0;

	//dbInfo.iTransactionDetail.iState; // ���ڲ�ͬ������ʽ�ľ������ξ���д��

	__super::WriteDBData(dbInfo);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���п�֧��
//
//@param      none
//
//@retval     bool	true:�ɹ�	false:ʧ��
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//bool CChargeSvc::POSPayment()
//{
//	try {
//		// ......��TPU�ӿ��ݲ�ȷ����
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//
//	return true;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���п�����
//
//@param      none
//
//@retval     bool	true:�ɹ�	false:ʧ��
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//bool CChargeSvc::POSCancle()
//{
//	try {
//		// ......��TPU�ӿ��ݲ�ȷ����
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��OnCancel�͸��ֳ���ʹ�õ�һ��������װ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::FlowToFinishDlg()
{
	//if(theAPP_SESSION.IsBlindManOperation()){
	//	if(m_operation_status == BLIND_OPERATION_STATUS::STATUS_BLIND_FIRST_SWIPE_CARD){
	//		// �������û��ֹͣ����Ҫֹͣ���ٷ��ء�
	//		if(!m_bIsPinPadStoped){
	//			m_pDataModel->operationInfo.finishType = CPassengerForeSvc::RUNNING_OUT_OF_TIME;
	//			//PINPAD_HELPER->TurnOffInputTrigger();// �ر��������:�ص��д���				
	//			return;
	//		}
	//		else{// ��������ѹرգ�ֱ������Ĭ��ҵ��
	//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
	//			return;
	//		}
	//	}
	//	// �ڶ���ˢ��ȡ��
	//	else if(m_operation_status == BLIND_OPERATION_STATUS::STATUS_BLIND_SECOND_SWIPE_CARD){
	//		PlayEventAudio(AUDIO_BUSINESS_CANCEL,true);
	//		m_operation_status = STATUS_BLIND_BUSINESS_CANCEL;// ͶǮ��ȡ��
	//		if(!m_bIsPinPadStoped){
	//			//PINPAD_HELPER->TurnOffInputTrigger();// ֹͣ����
	//			return;
	//		}
	//		else{
	//			return;
	//		}
	//	}
	//}
	m_enumIsReadedCard = ABORD;
	// ��Ⲣ�������
	//IO_HELPER->CheckAndSendOutCard();lichao

	DoDialogFlow(WM_INIT_TO_FINISH);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��OnCancel�͸��ֳ���ʹ�õ�һ��������װ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::FlowToErrorDlg()
{
	//__super::FlowToErrorDlg();

	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_INIT){
		//if(theAPP_SESSION.IsBlindManOperation()){
		//	// ��������Ƿ�ֹͣ
		//	if (m_bIsPinPadStoped){
		//		// �ֽ�Ͷ�������Ѿ��ﵽ�������
		//		if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT  ||
		//			m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
		//				//����ʧ��:Ͷ�볬�ޡ�
		//				m_operation_status = STATUS_BLIND_INSERT_CASH_MAX;
		//				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//				PlayEventAudio(AUDIO_COMMON_INSERT_MAX_AMOUNT,true);
		//		}
		//		else if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT  ||
		//			m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
		//				//����ʧ��:���㲻�㡣
		//				m_operation_status = STATUS_BLIND_CHANGE_NOT_ENOUGHT;
		//				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//				m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT ? PlayEventAudio(AUDIO_COMMON_CHANGE_NOT_ENOUGH,true) : PlayEventAudio(AUDIO_COMMON_CHANGE_MAX_COUNT,true);
		//		}
		//		else if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::PASSAGE_CANCEL){
		//			m_operation_status = STATUS_BLIND_BUSINESS_CANCEL;
		//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//		}
		//		else if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::INSERT_OUT_OF_TIME){
		//			m_operation_status = STATUS_BLIND_OPERATION_OUT_OF_TIME;
		//			PlayEventAudio(AUDIO_COMMON_MSG_TIMEOUT_CANCEL,true);
		//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//		}
		//		// ������ʧ��
		//		else if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::SERVICE_ERROR_CANCEL){
		//			m_operation_status = STATUS_BLIND_BUSINESS_FAILED;
		//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//			PlayEventAudio(AUDIO_BUSINESS_FAILED,true);
		//		}	
		//		else{
		//			m_operation_status = STATUS_BLIND_BUSINESS_FAILED;
		//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//			PlayEventAudio(AUDIO_BUSINESS_FAILED,true);
		//		}
		//		DoDialogFlow(WM_INIT_TO_ERROR); // ������ת
		//	}
		//	else{
		//		//m_PinpadInputType = UE_PINPADKEY::UE_PINPAD_RIGHT;
		//		//PINPAD_HELPER->TurnOffInputTrigger();// ֹͣ����������
		//	}
		//}
		//else
			
		// ��Ⲣ�������
			//IO_HELPER->CheckAndSendOutCard();lichao

		DoDialogFlow(WM_INIT_TO_ERROR); // ������ת
	}
	else if (m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_WAIT){
		DoDialogFlow(WM_WAIT_TO_ERROR);
	}
	else{
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		pMainSvc->StartDefaultService();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		ת��Ĭ�Ͻ���  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::FlowToDefaultDlg()
{
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	pMainSvc->StartDefaultService();
	return;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ����ƾ֤

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PrinterPrintReceipt()
{
	try {
		// ����ģ��
		CString templateName= _T("TVM_CHARGE.template");
		CStringArray printTexts;
		CStringArray templateTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

		// �������
		int index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			CString strSerialNo = _T("");
			strSerialNo.Format(_T("%010d"),m_pDataModel->dwUDSN);
			CString strCardNum = m_pDataModel->mediumInfo.cardLogicalNumber;
			line.Replace(_T("{��վ����}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{���׺�}"),strSerialNo);
			line.Replace(_T("{�û�����}"),strCardNum);
			line.Replace(_T("{Ʊ������}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_CHINESE));
			line.Replace(_T("{��ֵǰ���}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{��ֵ���}"),ComFormatAmount(m_pDataModel->lAddAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{��ֵ�����}"),ComFormatAmount(m_pDataModel->lAddAmount + m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{����}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			
			// ��ӡʱ�ַ����ĸ�ʽ������ģ���ȡ���ݣ�
			CStringArray printLines;
			ComPrintLineFormatForTemplate(line,printLines);
			printTexts.Append(printLines);
			++index;
		}

		// ��ӡ��Ʊ
		//PRINTER_HELPER->PrinterPrint(&printTexts,true);

		PRINTER_HELPER->PrintReceiptByBmp(&printTexts,true,true);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ����ƾ֤

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PrinterPrintErrorReceipt()
{
	try {
		// ����ģ��
		CString templateName= _T("TVM_CHARGE_FAILED.template");
		CStringArray printTexts;
		CStringArray templateTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

		// �������
		int index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			CString strSerialNo = _T("");
			strSerialNo.Format(_T("%010d"),m_pDataModel->dwTransactionID);
			CString strCardNum = m_pDataModel->mediumInfo.cardLogicalNumber;
			line.Replace(_T("{��վ����}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{���׺�}"),strSerialNo);
			line.Replace(_T("{�û�����}"),strCardNum);
			line.Replace(_T("{Ʊ������}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_CHINESE));
			line.Replace(_T("{��ֵǰ���}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{��ֵ���}"),ComFormatAmount(m_pDataModel->lAddAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{��ֵ�����}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{���Ͻ��}"),ComFormatAmount(m_pCommonModel->paymentInfo.lReturnAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{����}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			// ��ӡʱ�ַ����ĸ�ʽ������ģ���ȡ���ݣ�
			CStringArray printLines;
			ComPrintLineFormatForTemplate(line,printLines);
			printTexts.Append(printLines);
			++index;
		}

		// ��ӡ��Ʊ
		//PRINTER_HELPER->PrinterPrint(&printTexts,true);

		PRINTER_HELPER->PrintReceiptByBmp(&printTexts,true,true);
	}
	catch(CSysException&e) {
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�Ӳ��ֽ�Ҷ��ﵽ�����������PassengerForeSvc��ĺ���OnCoinInserted��OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CChargeSvc::IsBothCoinBanknoteMaxCount() {
	if (IsBanknoteMaxCount() && IsCoinMaxCount()) {
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ֽ�Ҵﵽ�����������PassengerForeSvc��ĺ���OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsBanknoteMaxCount() {
	// ��֧��ֽ�ң���ʾ����
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
		return true;
	}
	// �жϵ�ǰǮ������
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	long lCurCount = banknoteInfo.BankntoeTotalCount();//banknoteInfo.ulCount_10 + banknoteInfo.ulCount_100 + banknoteInfo.ulCount_1 + banknoteInfo.ulCount_20 + banknoteInfo.ulCount_50 + banknoteInfo.ulCount_5;
	
	// ���жϵ�ǰͶ���Ƿ����ʣ���Ͷ��������
	if(m_pDataModel->paymentInfo.lBHReceiveTotalCount >= (theAFC_TVM_OPERATION.GetBanknotesFullConut() - lCurCount)){
		return true;
	}
	// ���жϵ�ǰͶ�������Ƿ񳬹���ֵ����������ƣ�
	if (m_pDataModel->paymentInfo.lBHReceiveTotalCount > theAFC_TVM_OPERATION.GetAcceptBanknotesMaxPieceOfRecharge()) {
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�Ӳ�Ҵﵽ�����������PassengerForeSvc��ĺ���OnCoinInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsCoinMaxCount(){
	// ��֧��Ӳ�ң���ʾ����
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
		return true;
	}
	// 1.ȡ����Ʊ������
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_A_info = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_B_info = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//CTCoinCountInfo::COIN_BOX_INFO stock				 = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//// 2.���������ɽ�������
	//long ulBoxA = theAFC_TVM_OPERATION.GetRecycleBoxAFullCount() - box_A_info.GetCurCount();
	//long ulBoxB = theAFC_TVM_OPERATION.GetRecycleBoxBFullCount() - box_B_info.GetCurCount();
	//// 3.ѭ��������������������
	//long ulStock = (long)theTVM_INFO.GetCHStock1Max() - stock.ulCurCount;

	//// 4.�Ѿ����յ�1MOP������
	//long ulReciveOneCount   = m_pDataModel->paymentInfo.PayCoinInfo[Coin1MOP] ;
	//// 5.�Ѿ����շ�1MOP������
	//long ulReciveOtherCount = m_pDataModel->paymentInfo.PayCoinInfo[Coin2MOP] + m_pDataModel->paymentInfo.PayCoinInfo[Coin5MOP] + m_pDataModel->paymentInfo.PayCoinInfo[Coin50AVOS];

	//// 6.����ݴ�1����������ѭ��������ͻ�����1������������ݴ�2���������ڵ��ڻ�����2������
	//if(ulReciveOneCount > ulStock && ulReciveOneCount > ulBoxA || ulReciveOtherCount > ulBoxB){
	//	return true;
	//}
	//// 7.����ݴ�1�����ݴ�1�����������������ݴ�2���������ڵ����ݴ�2������ö��
	//if(ulReciveOneCount >= theTVM_INFO.GetCHTemporary1Max() || ulReciveOtherCount >= theTVM_INFO.GetCHTemporary2Max()){
	//	return true;
	//}
	//// 8.�������������������ƣ����ж�Ͷ���Ƿ񳬹������������
	//if (m_pDataModel->paymentInfo.lCHReceiveTotalCount >= theAFC_TVM_OPERATION.GetAcceptCoinMaxPieceOfRecharge()){
	//	return true;
	//}
	//else{
	//	return false;
	//}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ֽ�Ҷ��ﵽ������PassengerForeSvc��ĺ���OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CChargeSvc::IsBanknoteMaxAmount() {
	// ���������ս��
	if (m_pDataModel->paymentInfo.lBHReceiveTotalAmount > theAFC_TVM_OPERATION.GetAcceptBanknotesMaxAmountOfRecharge()*100) { // acceptChrgrMaxBillAmt���ء�Ԫ��������BHReceiveMoney�Ĳ����ǡ��֡���
		return true;
	}
	else {
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж����������Ƿ񳬹��������

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsCoinChageMaxCount(){
	// ����������
	DWORD dwChangeCount = m_pDataModel->paymentInfo.dwChangeCoinHalfYuanCount + m_pDataModel->paymentInfo.dwChangeCoinOneYuanCount;//m_pDataModel->paymentInfo.dwChangeStockCount + m_pDataModel->paymentInfo.dwChangeHopper1Count + m_pDataModel->paymentInfo.dwChangeHopper2Count;
	if (dwChangeCount > theAFC_TVM_OPERATION.GetChageCoinMaxPieceOfRecharge()){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ�����������Ƿ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsBHChangeMaxCount(){
	DWORD dwChangeCount = m_pDataModel->paymentInfo.BNRChangeCount();//m_pDataModel->paymentInfo.dwChangeBHBoxACount + m_pDataModel->paymentInfo.dwChangeBHBoxBCount;
	if(dwChangeCount > theAFC_TVM_OPERATION.GetBHChangeMaxPiece()){
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ���������Ƿ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsBHChangeMaxAmout(){
	if(m_pDataModel->paymentInfo.dwChangeBHAmout > theAFC_TVM_OPERATION.GetBHChangeMaxAmount() * 100){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�ɽ��յ�ֽ�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BRH_FACE_ACEPTED_TABLE CChargeSvc::GetAcceptBanknotesFace(tBncSetInhibitList *table){
	vector<WORD> vecFace;
	BRH_FACE_ACEPTED_TABLE bhAcceptTable;
	theAFC_TVM_OPERATION.GetAcceptBanknotesFaceOfRecharge(vecFace);
	for (vector<WORD>::iterator it = vecFace.begin();it != vecFace.end();it++){
		if (1 == *it){
			bhAcceptTable.bIsBanknote1Yuan = true;
		}
		else if(5 == *it){
			bhAcceptTable.bIsBanknote5Yuan = true;
		}
		else if(10 == *it){
			bhAcceptTable.bIsBanknote10Yuan = true;
		}
		else if(20 == *it){
			bhAcceptTable.bIsBanknote20Yuan = true;
		}
		else if(50 == *it){
			bhAcceptTable.bIsBanknote50Yuan = true;
		}
		else if(100 == *it){
			bhAcceptTable.bIsBanknote100Yuan = true;
		}
	}
	if (table != NULL)
	{
		table->iNumOfItems = 6;
		table->psBncCashInfo = new tBncSetInhibitItem[6];
		int n = 0;
		table->psBncCashInfo[n].iValue = 100;
		table->psBncCashInfo[n++].bAccepted = bhAcceptTable.bIsBanknote1Yuan;
		table->psBncCashInfo[n].iValue = 500;
		table->psBncCashInfo[n++].bAccepted = bhAcceptTable.bIsBanknote5Yuan;
		table->psBncCashInfo[n].iValue = 1000;
		table->psBncCashInfo[n++].bAccepted = bhAcceptTable.bIsBanknote10Yuan;
		table->psBncCashInfo[n].iValue = 2000;
		table->psBncCashInfo[n++].bAccepted = bhAcceptTable.bIsBanknote20Yuan;
		table->psBncCashInfo[n].iValue = 5000;
		table->psBncCashInfo[n++].bAccepted = bhAcceptTable.bIsBanknote50Yuan;
		table->psBncCashInfo[n].iValue = 10000;
		table->psBncCashInfo[n++].bAccepted = bhAcceptTable.bIsBanknote100Yuan;
	}
	return bhAcceptTable;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�ɽ��յ�Ӳ�ҵ����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COIN_ACCEPTABLE CChargeSvc::GetAcceptCoinFaceTable(){
	// 1.�жϻ������Ƿ���λ��
	COIN_ACCEPTABLE acceptTable;
	/*CH_COMMON_RSP rsp;
	try{
		COIN_HELPER->CH_GetStatus(&rsp);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}*/	
	// 2.���ɽ������ת��ΪTable��
	vector<WORD> vecFace;
	theAFC_TVM_OPERATION.GetAcceptCoinFaceOfRecharge(vecFace);
	for (vector<WORD>::iterator it = vecFace.begin();it != vecFace.end(); it++)
	{
		if(50 == *it){
			acceptTable.CH_COIN_ALLOW_05 = true;
		}
		else if(100 == *it){
			acceptTable.CH_COIN_ALLOW_1 = true;
		}
// 		else if(200 == *it){
// 			acceptTable.CH_COIN_ALLOW_2 = true;
// 		}
// 		else if(500 == *it){
// 			acceptTable.CH_COIN_ALLOW_5 = true;
// 		}
	}
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_A_info = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_B_info = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	// 3.������2����λ�����߻�����2�����������Խ��շ�һԪ��
// 	if(!m_CoinMoudelStatus.sensor_info.CH_BOX2_SET_STATUS || (box_B_info.GetCurCount() >= theAFC_TVM_OPERATION.GetRecycleBoxAFullCount())){
// 		acceptTable.CH_COIN_ALLOW_05 = false;
// 		acceptTable.CH_COIN_ALLOW_2	 = false;
// 		acceptTable.CH_COIN_ALLOW_5  = false;
// 	}
// 	// 4.������1����λ���жϵ�ǰstock�Ƿ��������������߶�����
// 	CTCoinCountInfo::COIN_BOX_INFO stock = theCOIN_COUNT.GetCoinHopperboxCInfo();
// 	if (!m_CoinMoudelStatus.sensor_info.CH_BOX1_SET_STATUS && stock.ulCurCount >= theTVM_INFO.GetCHStock1Max() ||
// 		(stock.ulCurCount >= theTVM_INFO.GetCHStock1Max()) && (box_B_info.GetCurCount() >= theAFC_TVM_OPERATION.GetRecycleBoxBFullCount())){
// 		acceptTable.CH_COIN_ALLOW_1 = false;
// 	}
	return acceptTable;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		��ȡ���Ӳ�Ʒʱ����λ  

@param      Duration_t ����ʱ��

@retval     CString ת�����ʱ���ӵ�λ

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CChargeSvc::GetAddProductTime(Duration_t date){
	//��ֵʱ��:ʱ�� ��λ
	DurationUnit_t  dtUnit  = GetDurationType(date);  
	DurationValue_t dtValue = GetDurationValue(date);
	CString strReturn;
	CString strUnit;
	switch(dtUnit){
	case DurationUnit_t::DURATION_UNIT_SECOND:			// ��
		strUnit = _tl(SECOND_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_MINUTE:			// ����
		strUnit = _tl(MINUTE_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_HOUR:			// Сʱ
		strUnit = _tl(HOUR_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_DAY:				// ��
		strUnit = _tl(DAY_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_WEEK:			// ����
		strUnit = _tl(WEEK_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_CALENDAR_MONTH:	// ��
		strUnit = _tl(MONTH_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_YEAR:			// ��
		strUnit = _tl(YEAR_CNT_UNIT);
		break;
	default:
		strUnit = _tl(DAY_CNT_UNIT);
		break;
	}
	strReturn.Format(_T("%d%s"),dtValue,strUnit);
	return strReturn;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��װ���Բ����ӿڣ�������ʱ����

@param      DWORD enventID (����������ID)

@param		bool  bIsStopRepeat	(�Ƿ��ظ�����)

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PlayEventAudio(DWORD enventID,bool bIsStopRepeat /* = false */){
	// ��ä�˳�ֵ���������Բ��Ŵ���
	if(!theAPP_SESSION.IsBlindManOperation()){
		return;
	}
	if(enventID == AUDIO_COMMON_MSG_INPUT_ERROR){
		bIsStopRepeat = false;
	}
	else
		m_dwLastAudio = enventID;
	unsigned int nPlayedTimes = 0;
	StopTimer(TIMERS_OUT_OF_REPLAY);
	m_pChargeAudioHelper->playEventAudio(enventID,nPlayedTimes);
	int nTimerUpTime = 5;// Get from config file 
	if(nTimerUpTime == 0 || bIsStopRepeat/* == true ? ���ظ� : �ظ�*/)
		StopTimer(TIMERS_OUT_OF_REPLAY);
	else
		StartTimer(TIMERS_OUT_OF_REPLAY,m_VoicePlayTime = nTimerUpTime + nPlayedTimes/1000+1);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ż�ʱ��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::OnTimer(UINT nEventID){
	try{
		if(nEventID == TIMERS_OUT_OF_REPLAY){
			StopTimer(nEventID);
			PlayEventAudio(m_dwLastAudio);
		}
		if(nEventID == TIMERS_OPERATION_OUT_OF_TIME){
			StopTimer(nEventID);
			StopTimer(TIMERS_OUT_OF_REPLAY);
			if(theSERVICE_MGR.GetState() == COUNTING || theSERVICE_MGR.GetState() == ISSUEING && !CMessagePictureBox::Exist()){
				return;
			}
			m_pDataModel->operationInfo.finishType = RUNNING_OUT_OF_TIME;
			// �������ڣ�ֱ�ӽ�����
			if(CMessagePictureBox::Exist()){
				CMessagePictureBox::End(IDABORT);
			}
			else
				;//PINPAD_HELPER->TurnOffInputTrigger();
		}
		if(nEventID == TIMERS_WAIT_CARD_INSERT)
		{
			IO_RSP_STATUS_INFO stu;
			CIOMonitorSvc* psvr = (CIOMonitorSvc*)(theSERVICE_MGR.GetService(SERVICE_ID::MONITOR_IO_SVC));
			stu = psvr->ioStatus;
			if (stu.isRechargeCardReady)
			{
				theSERVICE_MGR.SetState(ISSUEING);

				StopTimer(nEventID);
				// ������������
				SERVICE_ID serviceIDCurrent = (SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID();
				RECHARGERW_HELPER->ReadCard(serviceIDCurrent,false);
			}
		}
		// ͬʱ���ø����ʱ��
		__super::OnTimer(nEventID);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(theSERVICE_MGR.GetCurService()->GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		������̻ص�����      

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CChargeSvc::OnPinPadPressed(UINT ret,CTVMPinPadStartInput* pPinPadInput){
//	try{
//		// ä�˹�Ʊ��
//		if(theAPP_SESSION.IsBlindManOperation()){
//			// ������̷��سɹ�
//			if(ret == 0){
//				UE_PINPADKEY emGetValue = PINPAD_HELPER->PraserInputValue(pPinPadInput->getInputValue());
//				m_bIsPinPadStoped = true;
//				StopTimer(TIMERS_OPERATION_OUT_OF_TIME);// ��ֹ������ʱ��ʱ��
//				LOG("�����������������%x",emGetValue);
//				// 1.�ֶ���ֹ�������
//				if(emGetValue == UE_UNKNOWN_KEY|| UE_PINPAD_NOINPUT == emGetValue){
//					LOG("Unknow key");
//					// �ֶ�ȡ��,δͶǮ,ת��Ĭ�Ͻ���
//					if (m_operation_status == STATUS_BLIND_CANCEL){
//						theAPP_SESSION.SetBlindManStatus(false);
//						//PINPAD_HELPER->PowerOff();
//						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// �ֶ�ȡ������Ӳ��������
//					else if(m_operation_status == STATUS_BLIND_BUSINESS_CANCEL){
//						if(theSERVICE_MGR.GetState() != CANCELING)
//							__super::OnCancel();
//						return;
//					}
//					// ���ڳ�ֵ�У�������̹ر�
//					else if(m_operation_status == STATUS_BLIND_ISSUING){
//						return;
//					}
//					// Ͷ���������������
//					else if(m_PinpadInputType == UE_PINPAD_RIGHT){
//						FlowToErrorDlg();
//						return;
//					}
//					// ͶǮ��ʱ
//					else if(m_pDataModel->operationInfo.finishType == INSERT_OUT_OF_TIME){
//						// �ж��Ƿ�Ͷ�ң�
//						if(m_pDataModel->operationInfo.banknoteAcceptState == CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_NONE && m_pDataModel->operationInfo.coinAcceptState == CPassengerForeSvc::COIN_ACCEPT_STATE_NONE){
//							PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//						}
//						// ��Ͷ��
//						else{
//							//PlayEventAudio(AUDIO_COMMON_MSG_TIMEOUT_CANCEL,true);
//						}
//						return;
//					}
//					// �˿�ȡ��
//					else if(m_pDataModel->operationInfo.finishType == PASSAGE_CANCEL){
//						//PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);// ������ȡ��
//						//PINPAD_HELPER->PowerOff();
//						//theAPP_SESSION.SetBlindManStatus(false);
//						//// ����Ĭ��ҵ��
//						//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// ���׳�ʱ
//					else if(m_pDataModel->operationInfo.finishType == RUNNING_OUT_OF_TIME){
//						// ��һ��ˢ��ʱ������ȡ��
//						if(STATUS_BLIND_FIRST_SWIPE_CARD == m_operation_status){//Ok
//							PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);// ������ȡ��
//							//PINPAD_HELPER->PowerOff();
//							theAPP_SESSION.SetBlindManStatus(false);
//							// ����Ĭ��ҵ��
//							theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//							return;
//						}
//						// ������ʱ
//						else {
//							if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//								m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_OPERATION_OUT_OF_TIME;
//								GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);// �����Ļ�ϵ�ȡ��
//								return;
//							}						
//						}
//					}
//					// ����ʧ�ܻ�ÿ���֧�ֳ�ֵ����
//					else if(m_operation_status == STATUS_BLIND_READ_CARD_FAILED){
//						//PINPAD_HELPER->PowerOff();
//						theAPP_SESSION.SetBlindManStatus(false);
//						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// �ֶ�ȡ��
//					else{
//						// δͶǮ
//						if(!m_pDataModel->operationInfo.isBeginBuyProduct){
//							//PINPAD_HELPER->PowerOff();
//							theAPP_SESSION.SetBlindManStatus(false);
//							theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//							return;	
//						}
//						// ��ͶǮ
//						else{
//							OnCancel();
//							return;
//						}
//					}
//				}// End 1
//				// 2.��һ��ˢ��
//				else if(STATUS_BLIND_FIRST_SWIPE_CARD == m_operation_status){
//					// ֻ��ӦCancel��
//					if(emGetValue == UE_PINPAD_CANCLE){
//						PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);// ������ȡ��
//						CMessagePictureBox::End(IDABORT);// ��ʱ��ص������޿�����PassengerForeSvc�д���
//						//PINPAD_HELPER->PowerOff();
//						theAPP_SESSION.SetBlindManStatus(false);
//						return;
//					}
//					else {
//						// Do nothing
//					}
//				}
//				// 3.ѡ���Ʒ
//				else if(STATUS_BLIND_SELECT_PRODUCT == m_operation_status){
//					if(emGetValue == UE_PINPAD_0)
//						m_vPassengerInput.push_back(0);
//					else if(emGetValue == UE_PINPAD_1)
//						m_vPassengerInput.push_back(1);
//					else if(emGetValue == UE_PINPAD_2)
//						m_vPassengerInput.push_back(2);
//					else if(emGetValue == UE_PINPAD_3)
//						m_vPassengerInput.push_back(3);
//					else if(emGetValue == UE_PINPAD_4)
//						m_vPassengerInput.push_back(4);
//					else if(emGetValue == UE_PINPAD_5)
//						m_vPassengerInput.push_back(5);
//					else if(emGetValue == UE_PINPAD_6)
//						m_vPassengerInput.push_back(6);
//					else if(emGetValue == UE_PINPAD_7)
//						m_vPassengerInput.push_back(7);
//					else if(emGetValue == UE_PINPAD_8)
//						m_vPassengerInput.push_back(8);
//					else if(emGetValue == UE_PINPAD_9)
//						m_vPassengerInput.push_back(9);
//					else if(emGetValue == UE_PINPAD_CLEAR)
//						m_vPassengerInput.clear();
//					else if(emGetValue == UE_PINPAD_CANCLE){
//						//GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//						PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//						theAPP_SESSION.SetBlindManStatus(false);
//						//PINPAD_HELPER->PowerOff();
//						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// ȷ�ϼ�
//					else if(emGetValue == UE_PINPAD_ENTER){
//						int nMenuCode = 0;
//						// �ж��Ƿ�Ϸ�
//						vector<int>::iterator it = m_vPassengerInput.begin();
//						while(it != m_vPassengerInput.end()){
//							nMenuCode = nMenuCode*10 + *it;
//							it++;
//						}
//						int nCount = m_pDataModel->vecTicketProduct.size();
//						// �ж��Ƿ��д˲�Ʒ:��Ҫע�����Բ��������ڶ��������ݶ�Ӧ!!!
//						if(nMenuCode > nCount){
//							PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//						}
//						else {
//							ProductType type = UNKNOWPRODUCT;
//							ProductTypeVariant_t subType = 0;
//							bool bIsCorrect = false;
//							vector<PRODUCT_INFO>::iterator itor = m_pDataModel->vecTicketProduct.begin();
//							int nSingleCount = 0;
//
//							if((nMenuCode == 1) && (m_pDataModel->lAvailableRechargeProduct & TYPE_SVT)){
//								while(itor != m_pDataModel->vecTicketProduct.end()){
//									if (itor->productType == SVT ){
//										subType = itor->productSubType;
//									}
//									itor++;
//								}
//								bIsCorrect = true;
//								type = SVT;
//							}
//							if((nMenuCode == 2) && (m_pDataModel->lAvailableRechargeProduct & TYPE_MST)){
//								while(itor != m_pDataModel->vecTicketProduct.end()){
//									if (itor->productType == MST ){
//										nSingleCount++;
//										subType = itor->productSubType;
//									}
//									itor++;
//								}	
//								// ��ֻ��һ���Ӳ�Ʒʱ
//								if(nSingleCount == 1){
//									bIsCorrect = true;
//									type = MST;
//								}
//								// ͬһ�ֲ�Ʒ�ж���Ӳ�Ʒ,��ʾѡ���Ӳ�Ʒ
//								else{
//									m_pDataModel->productInfo.productType = MST;
//									PlayAudioInDlg(AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST);
//									// ����������̼��
//									TurnOnPinPadAndTimer();
//									m_vPassengerInput.clear();
//									return;
//								}								
//							}
//							if((nMenuCode == 3) && (m_pDataModel->lAvailableRechargeProduct & TYPE_PST)){
//								while(itor != m_pDataModel->vecTicketProduct.end()){
//									if(itor->productType == PST){
//										nSingleCount++;
//										subType = itor->productSubType;
//									}
//									itor++;
//								}
//
//								if(nSingleCount == 1){
//									bIsCorrect = true;
//									type = PST;
//								}
//								// ��ʾѡ���Ӳ�Ʒ
//								else{
//									m_pDataModel->productInfo.productType = PST;
//									PlayAudioInDlg(AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST);
//									// ����������̼��
//									TurnOnPinPadAndTimer();
//									m_vPassengerInput.clear();
//									return;
//								}
//							}
//							if(bIsCorrect){
//								BTN_INFO btn;
//								btn.btnMsg.Format(_T("%02d%02d"),type,subType);
//								GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);// �����Ӧ�İ�ť
//							}
//							else {
//								PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//							}
//						
//						}
//						m_vPassengerInput.clear();
//					}
//					// ������������������
//					else{
//						m_vPassengerInput.clear();
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//				}
//				// 4.ѡ���ֵ�Ӳ�Ʒ
//				else if(STATUS_BLIND_SELECT_SUB_PRODUCT == m_operation_status){
//					if(emGetValue == UE_PINPAD_0)
//						m_vPassengerInput.push_back(0);
//					else if(emGetValue == UE_PINPAD_1)
//						m_vPassengerInput.push_back(1);
//					else if(emGetValue == UE_PINPAD_2)
//						m_vPassengerInput.push_back(2);
//					else if(emGetValue == UE_PINPAD_3)
//						m_vPassengerInput.push_back(3);
//					else if(emGetValue == UE_PINPAD_4)
//						m_vPassengerInput.push_back(4);
//					else if(emGetValue == UE_PINPAD_5)
//						m_vPassengerInput.push_back(5);
//					else if(emGetValue == UE_PINPAD_6)
//						m_vPassengerInput.push_back(6);
//					else if(emGetValue == UE_PINPAD_7)
//						m_vPassengerInput.push_back(7);
//					else if(emGetValue == UE_PINPAD_8)
//						m_vPassengerInput.push_back(8);
//					else if(emGetValue == UE_PINPAD_9)
//						m_vPassengerInput.push_back(9);
//					else if(emGetValue == UE_PINPAD_CLEAR)
//						m_vPassengerInput.clear();
//					else if(emGetValue == UE_PINPAD_CANCLE){
//						PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//						theAPP_SESSION.SetBlindManStatus(false);
//						//PINPAD_HELPER->PowerOff();
//						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// ȷ�ϼ�
//					else if(emGetValue == UE_PINPAD_ENTER){
//						int nMenuCode = 0;
//						// �ж��Ƿ�Ϸ�
//						vector<int>::iterator it = m_vPassengerInput.begin();
//						while(it != m_vPassengerInput.end()){
//							nMenuCode = nMenuCode*10 + *it;
//							it++;
//						}
//						// �����Ƿ��е�ǰ���룿
//						map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//						// SVT
//						if(m_pDataModel->productInfo.productType == SVT){
//							if(itor != m_KeyValue.end()){
//								BTN_INFO btn;
//								btn.btnMsg.Format(_T("%d"),itor->second * 100);
//								GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_2_GROUP_ID,(LPARAM)&btn);
//								// �������
//							}
//							else{
//								// �޴��Ӳ�Ʒ
//								PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//							}
//						}
//						// PST��MST
//						else {
//							if(itor != m_KeyValue.end()){
//								bool bIsFind = false;
//								BTN_INFO btn;
//								vector<PRODUCT_INFO>::iterator iter = m_pDataModel->vecTicketProduct.begin();
//								
//								// ����Ʊ
//								if(m_pDataModel->productInfo.productType == PST){
//									while(iter != m_pDataModel->vecTicketProduct.end()){
//										if((itor->second%100000) == iter->productSubType && (itor->second/100000) == iter->productType){
//											btn.btnMsg.Format(_T("%02d%02d"),iter->productType,iter->productSubType);
//											GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);
//											bIsFind  = true;
//											break;
//										}
//										iter++;
//									}
//								}
//								// �ƴ�Ʊ
//								else if(m_pDataModel->productInfo.productType == MST){
//									while(iter != m_pDataModel->vecTicketProduct.end()){
//										// �����Ϊָ������
//										if((itor->second%100000) == iter->productSubType && (itor->second/100000) == iter->productType){
//											btn.btnMsg.Format(_T("%02d%02d"),iter->productType,iter->productSubType);
//											GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);
//											bIsFind  = true;
//											break;
//										}
//										iter++;
//									}
//								}
//								if (!bIsFind){
//									PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//								}
//							}
//							else{
//								PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//							}
//						}
//						m_vPassengerInput.clear();
//					}
//					// ������������������
//					else{
//						m_vPassengerInput.clear();
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//				}
//				// 5.ѡ��֧����ʽ
//				else if(STATUS_BLIND_SELECT_PAYMENT == m_operation_status){
//					if (emGetValue == UE_PINPAD_0)
//						m_vPassengerInput.push_back(0);
//					else if(emGetValue == UE_PINPAD_1)
//						m_vPassengerInput.push_back(1);
//					else if(emGetValue == UE_PINPAD_2)
//						m_vPassengerInput.push_back(2);
//					else if(emGetValue == UE_PINPAD_3)
//						m_vPassengerInput.push_back(3);
//					else if(emGetValue == UE_PINPAD_4)
//						m_vPassengerInput.push_back(4);
//					else if(emGetValue == UE_PINPAD_5)
//						m_vPassengerInput.push_back(5);
//					else if(emGetValue == UE_PINPAD_6)
//						m_vPassengerInput.push_back(6);
//					else if(emGetValue == UE_PINPAD_7)
//						m_vPassengerInput.push_back(7);
//					else if(emGetValue == UE_PINPAD_8)
//						m_vPassengerInput.push_back(8);
//					else if(emGetValue == UE_PINPAD_9)
//						m_vPassengerInput.push_back(9);
//					else if(emGetValue == UE_PINPAD_CLEAR)
//						m_vPassengerInput.clear();
//					else if(emGetValue == UE_PINPAD_CANCLE){
//						if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//							return;
//						}
//					}
//					// ȷ�ϼ�
//					else if(emGetValue == UE_PINPAD_ENTER){
//						int nMenuCode = 0;
//						// �ж��Ƿ�Ϸ�
//						vector<int>::iterator it = m_vPassengerInput.begin();
//						while(it != m_vPassengerInput.end()){
//							nMenuCode = nMenuCode*10 + *it;
//							it++;
//						}
//
//						// �����Ƿ��е�ǰ���룿
//						map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//						if(itor != m_KeyValue.end()){
//							BTN_INFO btn;
//							btn.btnMsg.Format(_T("%d"), itor->second);
//							GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_3_GROUP_ID,(LPARAM)&btn);
//							// �������
//						}
//						else{
//							// �޵�֧����ʽ
//							PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//						}
//						m_vPassengerInput.clear();
//					}
//					// ������������������
//					else{
//						m_vPassengerInput.clear();
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//				}
//				// 6.����ȷ��
//				else if(STATUS_BLIND_CONFIRM_BUSINESS == m_operation_status){
//					if(emGetValue == UE_PINPAD_ENTER){// ȷ�Ͻ���
//						if(GetCurReceptionDlg()->IsButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
//						}						
//					}
//					else if(emGetValue == UE_PINPAD_CANCLE){// ȡ������
//						if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//							return;
//						}
//						
//					}
//				}
//				// Ͷ���ֽ�
//				else if(STATUS_BLIND_INSERT_CASH == m_operation_status){
//					if(emGetValue == UE_PINPAD_CANCLE){
//						if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//							return;
//						}						
//					}
//				}
//				// ȷ��֧����Ϣ
//				else if(STATUS_BLIND_CONFIRM_PAYMENT == m_operation_status){
//					if(emGetValue == UE_PINPAD_ENTER){
//						//GetCurReceptionDlg()->PostMessage(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);//!!!
//						if(GetCurReceptionDlg()->IsButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);// ͬ�����ý����ϵĺ�����ֱ������Ϊֹ
//							return;
//						}						
//					}
//					else if(emGetValue == UE_PINPAD_CANCLE){
//						if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//							return;
//						}						
//					}
//				}
//				// �������п���Ԥ����
//				else if(STATUS_BLIND_INSERT_BANKCARD == m_operation_status){
//					return;
//				}
//				// �������п�����
//				else if(STATUS_BLIND_INPUT_PASSWORD == m_operation_status){
//					return;
//				}
//				// �ڶ���ˢ��
//				else if(STATUS_BLIND_SECOND_SWIPE_CARD == m_operation_status){
//					//if(emGetValue == UE_PINPAD_CANCLE){// ֻ��Ӧȡ����
//					//	LOG("ȡ�����ף��ڶ��ζ���ʱ��");
//					//	CMessagePictureBox::End(IDABORT);
//					//	return;
//					//}// �����Ĳ���Ӧ
//					//LOG("������Ч����");
//					return;
//				}
//				// ������
//				else if(STATUS_BLIND_ISSUING == m_operation_status){
//					// Do nothing
//					return;
//				}
//				// ��ֵ�ɹ�
//				else if(STATUS_BLIND_BUSINESS_SUCCESS == m_operation_status){
//					return;
//				}
//				// ��ֵʧ��
//				else if(STATUS_BLIND_BUSINESS_FAILED == m_operation_status){
//					return;
//				}
//				// ����ȡ����Ӳ����
//				else if(STATUS_BLIND_BUSINESS_CANCEL == m_operation_status){
//					return;
//				}
//				// ��ӡ���
//				else if(STATUS_BLIND_PRINT_SUCCESS == m_operation_status){
//					return;
//				}
//				// ����ȡ��
//				else if(STATUS_BLIND_CANCEL == m_operation_status){
//					return;
//				}
//				// ˢ��ʧ��
//				else if(STATUS_BLIND_READ_CARD_FAILED == m_operation_status){
//					return;
//				}
//				// ����
//				else {
//					return;
//				}
//				// ����������̼��
//				TurnOnPinPadAndTimer();
//			}
//			// ִ��ʧ��
//			else {
//				LOG("ִ��ʧ�ܡ�");
//			}
//		}
//		// �������п������������п�����(Ԥ��)
//		else{
//
//		}
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch(...){
//		theEXCEPTION_MGR.ProcessException(CInnerException(CHARGE_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ä�˹�Ʊ��ʱ��ʱ��

@param      bool bIsStartPinPad (�Ƿ������������)

@param      bool bIsStartOutOfTimer(�Ƿ���������ʱ��ʱ��)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CChargeSvc::TurnOnPinPadAndTimer(bool bIsStartPinPad /* = true */,bool bIsStartOutOfTimer /* = true */){
//	try{
//		if(bIsStartPinPad && m_bIsPinPadStoped){
//			//PINPAD_HELPER->TurnOnInputTrigger(this);
//			m_bIsPinPadStoped = false;
//		}
//		int nTimer = theAFC_TVM_OPERATION.GetCancelDealTime() + (m_VoicePlayTime >=0 ? m_VoicePlayTime : 0);// 60S + ������������ʱ��
//		if(nTimer == 0 || !bIsStartOutOfTimer)
//			StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
//		else
//			StartTimer(TIMERS_OPERATION_OUT_OF_TIME,nTimer);
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch(...){
//		theEXCEPTION_MGR.ProcessException(CInnerException(CHARGE_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������������״̬(��������״̬)

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PlayAudioInDlg(DWORD eventID){
	
	LOG("Play vedio:%d",eventID);
	//if(eventID == 0){
	//	m_pChargeAudioHelper->StopPlayEventAudio();
	//	return;
	//}
	//// 1.���ŵ�һ��ˢ������
	//else if(eventID == AUDIO_RECHARGE_FIRST_SWIPE_CARD){
	//	// AVM ����������Ҫ���⴦��
	//	if(DEVICE_TYPE_AVM == theMAINTENANCE_INFO.GetDeviceType())
	//		eventID = AUDIO_RECHARGE_FIRST_SWIPE_CARD_FOR_AVM;
	//	m_operation_status = STATUS_BLIND_FIRST_SWIPE_CARD;
	//	PlayEventAudio(eventID);
	//	TurnOnPinPadAndTimer();// ����������̲�����������ʱ��ʱ��
	//	return;
	//}
	//// 2.�������ԣ�ѡ���Ʒ����
	//else if(eventID == AUDIO_RECHARGE_SELECE_PRODUCT_TYPE)	
	//	m_operation_status = STATUS_BLIND_SELECT_PRODUCT;
	//// 3.1��ѡ��Ҫ��ֵ���Ӳ�Ʒ��MST
	//else if(eventID == AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST){
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST);
	//	m_operation_status = STATUS_BLIND_SELECT_SUB_PRODUCT;
	//}
	//// 3.2��ѡ��Ҫ��ֵ���Ӳ�Ʒ��PST
	//else if(eventID == AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST){
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST);
	//	m_operation_status = STATUS_BLIND_SELECT_SUB_PRODUCT;
	//}
	//// 3.3�˿�ѡ��Ҫ��ֵ���Ӳ�Ʒ��SVT
	//else if(eventID == AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE){// ѡ���Ӳ�Ʒ	
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE);
	//	LOG("Get Map");
	//	m_operation_status = STATUS_BLIND_SELECT_SUB_PRODUCT;
	//}
	//// 4.�˿�ѡ��֧����ʽ
	//else if(eventID == AUDIO_SELECT_PAYMENT){
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_SELECT_PAYMENT);
	//	m_operation_status = STATUS_BLIND_SELECT_PAYMENT;
	//}
	//// 5.��ʾȷ�Ͻ�����Ϣ
	//else if(eventID == AUDIO_RECHARGE_CONFIRM_BUSINESS_SVT || eventID == AUDIO_RECHARGE_CONFIRM_BUSINESS_PST || eventID == AUDIO_RECHARGE_CONFIRM_BUSINESS_MST){
	//	m_operation_status = STATUS_BLIND_CONFIRM_BUSINESS;// ȷ�Ͻ�����Ϣ
	//}
	//// 6.ȷ�Ͻ�����Ϣ�� ��Ͷ���ֽ�
	//else if(eventID == AUDIO_INSERT_CASH){
	//	m_operation_status = STATUS_BLIND_INSERT_CASH;
	//	StopTimer(TIMERS_OPERATION_OUT_OF_TIME);// ֹͣ������ʱ��ʱ������Ͷ���ֽ��ʱ���й�
	//}
	//// 7.ȷ��֧����Ϣ
	//else if(eventID == AUDIO_CONFORM_PAYINFO)
	//	m_operation_status = STATUS_BLIND_CONFIRM_PAYMENT;
	//// 6.1ȷ����Ϣ�󣺲������п�(Ԥ��)
	//else if(eventID == AUDIO_INSERT_BANKCARD)
	//	m_operation_status = STATUS_BLIND_INSERT_BANKCARD;		
	//// 7.1�������п�����(Ԥ��)
	//else if(eventID == AUDIO_INPUT_PASSWORD){

	//}
	//// 8.��ʾ���׳ɹ�,�Ƿ���Ҫ��ӡ��
	//else if(eventID == AUDIO_BUSINESS_SUCCESS){
	//	m_operation_status = STATUS_BLIND_BUSINESS_SUCCESS;
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
	//	PlayEventAudio(eventID,true);
	//	return;
	//}
	//// 9.��ӡ����ӡ�ɹ�
	//else if(eventID == AUDIO_BUSINESS_PRINT_SUCCESS){
	//	m_operation_status = STATUS_BLIND_PRINT_SUCCESS;
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
	//	PlayEventAudio(eventID,true);
	//	return;
	//}
	//// 10.����ȡ����δ��ʼ�տ�
	//else if(eventID == AUDIO_BUSINESS_CANCEL_NORMAL){
	//	m_operation_status = STATUS_BLIND_CANCEL;
	//	PlayEventAudio(eventID,true);// ����ȡ��
	//	if(!m_bIsPinPadStoped){
	//		//PINPAD_HELPER->TurnOffInputTrigger();
	//		return;
	//	}
	//	else{
	//		theAPP_SESSION.SetBlindManStatus(false);
	//		//PINPAD_HELPER->PowerOff();
	//		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
	//		return;
	//	}
	//}
	//// 11.����ȡ�������տ��Ҫ��Ǯ��
	//else if(eventID == AUDIO_BUSINESS_CANCEL){
	//	m_operation_status = STATUS_BLIND_BUSINESS_CANCEL;
	//	if(m_pDataModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_NONE && m_pDataModel->operationInfo.coinAcceptState != COIN_ACCEPT_STATE_NONE){
	//		PlayEventAudio(eventID,true);
	//	}
	//	else{
	//		PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
	//	}		
	//	if(!m_bIsPinPadStoped){
	//		//PINPAD_HELPER->TurnOffInputTrigger();
	//		return;
	//	}
	//	else{
	//		__super::OnCancel();
	//		return;
	//	}
	//}
	//else if(eventID == AUDIO_RECHARGE_FAILED_READ_CARD){
	//	m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_READ_CARD_FAILED;
	//	PlayEventAudio(eventID,true);
	//	TurnOnPinPadAndTimer(false,false);
	//	if(!m_bIsPinPadStoped)// �ر��������
	//		//PINPAD_HELPER->TurnOffInputTrigger();
	//	return;
	//}
	//// ��ǰ��֧�ִ�֧����ʽ
	//else if(eventID == AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT){
	//	m_operation_status = STATUS_BLIND_SELECT_PAYMENT;
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_SELECT_PAYMENT);
	//	PlayEventAudio(eventID);
	//	TurnOnPinPadAndTimer(false,true);	
	//	return;
	//}

	//PlayEventAudio(eventID);
	//// ���ü�ʱ��
	//TurnOnPinPadAndTimer(false,true);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ϸ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::ReturnFinishDlg(){
	// ��ֵ�����(�ɹ���ʧ��)
	//if(theAPP_SESSION.IsBlindManOperation()){
	//	if(m_pDataModel->operationInfo.finishType == TRANSACTION_STATE_FAILED){
	//		//PlayEventAudio(AUDIO_BUSINESS_FAILED,true);
	//		//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(STATUS_BLIND_RECHARGE);// ֹͣ������̼��
	//	}
	//}
	//// ����ʧ�ܣ����½��׼���
	//if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
	//	m_pDataModel->dwTransactionID = theCOUNT_INFO.GetTransactionNum();// �Զ���һ
	//}
	//m_pDataModel->operationInfo.isFinishedJob = true;
	OnReturnFinish();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ������ģ���е�����

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::InitDataModel(){
	m_CardLogicalNumber = _T("");
	m_pDataModel->transTime = ComGetCurTime();					// ��ҵ������ʱȷ��
	m_pDataModel->dwUDSN = theCOUNT_INFO.GetUDSNForDisplay();	// ��ҵ������ʱȷ��
	m_pDataModel->dwTransactionID = theCOUNT_INFO.GetTransactionNumForDsp();
	m_pDataModel->mediumInfo = MEDIUM_INFO();
	m_pDataModel->productInfo = PRODUCT_INFO();
	m_pDataModel->paymentInfo = PAYMENT_INFO();
	m_pDataModel->operationInfo = OPERATION_INFO();
	m_pDataModel->isInsertToDatabase = false;
	m_pDataModel->lDeviceStatus = 0;

	m_pDataModel->lAddAmount = 0;
	m_pDataModel->lAddRides = 0;
	m_nReadedCardCount = 0;
	m_pDataModel->vecTicketProduct.clear();
	m_pDataModel->oldProductInfo = PRODUCT_INFO();
	m_pDataModel->lAvailableRechargeProduct = TYPE_NULL;

	m_enumIsReadedCard = NONE_READ;
	m_dwLastAudio = 0;
	m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_NORMAL;
	m_bIsPinPadStoped = true;
	m_KeyValue.clear();
	m_vPassengerInput.clear();
//	m_PinpadInputType = UE_PINPADKEY::UE_UNKNOWN_KEY;
	m_VoicePlayTime = 0;
	
	// ȡ��ǰӲ��ģ��״̬
	if(!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		try{
			//if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM)
				//COIN_HELPER->ReadStatus(m_CoinMoudelStatus);
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}	
	}
	// ���ع�������ť��������ʾ��ť
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡӦ����ʾ�Ĳ�Ʒ�ĵ�λ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CChargeSvc::GetCurrentUnitOfProduct(){
	CString strUintOfProduct;
	switch(m_pDataModel->productInfo.productCategory){
	case PURSE:
		strUintOfProduct = _T("(") + _tl(MONEY_UNIT) + _T(")");
		break;
	case MULTIRIDE:
		strUintOfProduct = _T("(") + _tl(TIMES_UNIT) + _T(")");
		break;
	case PERIOD:
		strUintOfProduct = _T("(") + _tl(DAY_CNT_UNIT) + _T(")");
		break;
	default:
		strUintOfProduct = _T("");
		break;
	}
	return strUintOfProduct;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˿Ͳ忨�ص�������

@param      UINT uiResult;						���

@param      CIOCardPosMonitor* pIoCardMonotor

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CChargeSvc::OnChargeInsertCard(UINT uiResult,CIOCardPosMonitor* pIoCardMonitor){
//
//	try{
//		// 0.�ر�������Դ
//		//IO_HELPER->CloseCardLocker();lichao
//
//		// 1.�������أ��Ѳ��뿨��
//		if(uiResult == 0){
//			// ����
//			RECHARGERW_HELPER->ReadCard((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID());
//		}
//		// 2.�쳣����
//		else{
//			CMessagePictureBox::End();
//			// 2.1�ȴ����볬ʱ
//			if(uiResult == CIOException::ERROR_IO_READ_CARD_POS_TIMEOUT){
//				LOG("�ȴ��˿Ͳ����ֵ����ʱ��");
//			}
//			// 2.2�ֶ�ȡ��
//			else if(uiResult == CIOException::ERROR_IO_READ_CARD_POS_CANCEL){
//				LOG("�˿��ֶ�ȡ�������ֵ����");
//			}
//			// 2.3�쳣ȡ��
//			else if(uiResult == CIOException::ERROR_IO_READ_STATUS_EXCEPTION){
//				LOG("IOģ���쳣���ء�");
//			}
//
//			// ����Ĭ��ҵ��
//			NOTIFY_PROCESSING_WINDOW();
//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//		}
//		
//	}
//	catch(CSysException& e){
//		// �����쳣�����SAM��״̬�������쳣(��Ҫ����)
//
//	}
//	catch(...){
//
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}


void CChargeSvc::UpdateReceptDlg(CString strTxt)						// ���³˿ͽ��浹��ʱ��ʾ
{

	CChargeReceptionDlg* pMapDlg = (CChargeReceptionDlg*)(m_pDialogFlow->GetDialog(IDD_00301_CHARGE_RECEPT_DLG));
	pMapDlg->SetTipsShow(strTxt);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ������

@param      ��i��CString errorMsg  ������Ϣ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::OnError(CSysException e)
{
	try{
		if (m_pDataModel->paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH){
			// ֽ�Ҽ�Ӳ��ģ���쳣�ڸ��ദ��
			if(e.GetModuleID() == CBRHHardwareException::MODULE_ID || e.GetModuleID() == CBRHException::MODULE_ID || 
				e.GetModuleID() == CCHException::MODULE_ID || e.GetModuleID() == CCHHardwareException::MODULE_ID ||
				e.GetModuleID() == CBHChangeHardwareException::MODULE_ID
				){
					__super::OnError(e);
					return;
			}
		}

		theSERVICE_MGR.SetForeServiceBusy(false);
		// ��ӵ��쳣����
		theEXCEPTION_MGR.ProcessException(e);

		m_pCommonModel->operationInfo.finishType = TRANSACTION_STATE_FAILED;
		//// 1.�ж��Ƿ��ǳ�Ʊʧ�ܣ�M1����
		//if(e.GetModuleID() == CTHException::MODULE_ID || e.GetModuleID() == CTHHardwareException::MODULE_ID){
		//	// �����쳣����
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		//	// Ԥ������ʾ��ϸ������Ϣ��
		//	OnReturnFinish();
		//	return;
		//}
		//// 2.�Ƿ���Token�쳣��
		////if(e.GetModuleID() == CTOKENException::MODULE_ID || e.GetModuleID() == CTOKENHardwareException::MODULE_ID){
		////	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_TOKEN_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		////	OnErrorFinish();
		////}
		//// 3.�ж��Ƿ��Ƕ�д��ʧ�ܣ�
		//if(e.GetModuleID() == CRWException::TVM_CARD_RW_MODULE_ID || e.GetModuleID() == CRWHardwareException::TVM_CARD_RW_MODULE_ID){
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
			OnReturnFinish();
			return;
		//}
		//// 4.����Ӳ���쳣��(�����쳣:�ж��Ƿ�Ʊ���ꡣ)
		//if(m_pCommonModel->productInfo.nProductPiece > m_pCommonModel->productInfo.nProductIssuedPiece){
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		//	OnReturnFinish();
		//}
	}
	catch (CSysException& e){
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
		OnErrorFinish();
	}
	catch(...){
		theSERVICE_MGR.SetForeServiceBusy(false);
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		theEXCEPTION_MGR.ProcessException(e);
		OnErrorFinish();
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ձ���Ŀ�����

@param      void

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::EmptyCardInfo()
{
	m_pDataModel->mediumCardInfo = MEDIUM_INFO();
	m_pDataModel->mediumInfo = MEDIUM_INFO();
	//m_pDataModel->productCardInfo = PRODUCT_INFO();

	return 0;
}