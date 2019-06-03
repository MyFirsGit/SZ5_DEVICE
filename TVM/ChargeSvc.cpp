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
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CChargeSvc::CChargeSvc():CPassengerForeSvc(CHARGE_SVC)
{
	LOG("充值构造函数");
	m_pChargeAudioHelper = new CChargeAudioHelper(*this);
	m_pDataModel = new DataModel();
	m_pCommonModel = dynamic_cast<CommonModel*>(m_pDataModel);
	// 设置画面
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG,new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_00301_CHARGE_RECEPT_DLG, new CChargeReceptionDlg(this));
	m_pDialogFlow->AddDialog(IDD_00303_CHARGE_RESULT_DLG, new CPassengerResultDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INIT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR);
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_00303_CHARGE_RESULT_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_00301_CHARGE_RECEPT_DLG);


	// 设置画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_INIT,DIALOG_GROUP_INIT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_ERROR,DIALOG_GROUP_ERROR);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT); // should be this one

	m_bReaderBad = false;		// 读写器是否故障
	LOG("充值构造函数结束!");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

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
@brief      服务启动时响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::OnStart()
{
	// 初始化
	InitDataModel();

	// 盲人购票设置当前状态
	if(theAPP_SESSION.IsBlindManOperation()){
		// 盲人购票还在MainSvc的时候点击界面按钮
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->OnClickButtonWithoutPinPad();
		m_operation_status = STATUS_BLIND_NORMAL;
	}
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT); // should be this one
	m_bReaderBad = false;		// 读写器是否故障
	StartTimer(TIMERS_WAIT_CARD_INSERT,1);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

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
@brief      获取卡内产品信息

@param      (i)WORD productType 产品类型
@param      (i)WORD productSubType 产品子类型

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
@brief      获取产品票价

@param		(o)long& price 票价

@retval     bool	true:成功	false:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::CalculatePrice() {
	long resultCode = -1;	
	// 如果是储值票，无需通过TPU计算票价
	if (m_pDataModel->productInfo.productCategory == PURSE) {
		m_pDataModel->paymentInfo.lPriceAmount = static_cast<long>(m_pDataModel->lAddAmount);
		//计算税费
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

	// 其它情形
	return __super::CalculatePrice();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      保存票卡数据到DataModel

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW读卡反馈信息

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	DoDialogFlow(WM_WAIT_TO_INIT);
	return 0;

	CACCCard accCard;
	
	// 解析票卡
	if(!accCard.GetCardInfo(pReadCardResponse->CardInfo)) {
		LOG("解析票卡信息错误！");
		//__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
		return -1;
	}	
	
	// 票卡的基本信息
	CString strCardNum = ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8);
	if (m_CardLogicalNumber==_T("")){//初次读卡
		m_CardLogicalNumber = strCardNum;
		m_pDataModel->mediumInfo.cardLogicalNumber = strCardNum;
		m_pDataModel->mediumCardInfo.cardLogicalNumber = m_pDataModel->mediumInfo.cardLogicalNumber;
		m_pDataModel->mediumInfo.mediumType = (CARD_MEDIUM_TYPE)pReadCardResponse->CardPHType;	//theTICKET_HELPER.GetMediumType(m_pDataModel->mediumInfo.cardPHType);
		m_pDataModel->mediumInfo.passengerType = accCard.applicationData.byPassengerType;		//乘客类型（暂不用）
	}
	else{
		if(m_CardLogicalNumber!=strCardNum){// 非同一张卡
			m_strReadCardRsp = _T("充值中途更换票卡");
			return -1;
		}
		else{
			m_CardLogicalNumber = strCardNum;
			m_pDataModel->mediumInfo.cardLogicalNumber = strCardNum;
			m_pDataModel->mediumCardInfo.cardLogicalNumber = m_pDataModel->mediumInfo.cardLogicalNumber;
			m_pDataModel->mediumInfo.mediumType = (CARD_MEDIUM_TYPE)pReadCardResponse->CardPHType;	//theTICKET_HELPER.GetMediumType(m_pDataModel->mediumInfo.cardPHType);
			m_pDataModel->mediumInfo.passengerType = accCard.applicationData.byPassengerType;		//乘客类型（暂不用）
			m_strReadCardRsp = _T("");
		}
	}

	
	m_pDataModel->mapAvailableProduct.clear();
	m_pDataModel->vecTicketProduct.clear();
	// 查找可充值产品
	for (int i=0; i < pReadCardResponse->ProductCount; ++i) {
		if (pReadCardResponse->RechargeProduct[i].RechargeMark) {
			PRODUCT_INFO ticketProduct;
			ticketProduct.productType        = static_cast<ProductType>(pReadCardResponse->RechargeProduct[i].ProductType);
			ticketProduct.productSubType     = pReadCardResponse->RechargeProduct[i].ProductSubType;
			ticketProduct.lProductDeposit    = static_cast<long>(pReadCardResponse->RechargeProduct[i].RefoundMoney); // 可退金额==产品押金
			m_pDataModel->vecTicketProduct.push_back(ticketProduct);
		}
	}

	// 如果没有可充值产品
	if (m_pDataModel->vecTicketProduct.empty()) {
		LOG("无可充值产品错误！");
		//__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
		//CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_MSG_GUIDE_THERE_IS_NO_PRODUCT_FOR_RECHARGE),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		//IO_HELPER->CheckAndSendOutCard();lichao
		return -1;
	}

	// 车票的详细信息
	for (vector<PRODUCT_INFO>::iterator iter = m_pDataModel->vecTicketProduct.begin(); iter != m_pDataModel->vecTicketProduct.end(); ++iter) {
		CACCCard::ACCCARD_PRODUCT_INFO productInfo;
		if (!accCard.GetProductInfo(iter->productType, iter->productSubType, productInfo)) {
			LOG("处理车票详细信息错误！");
			//__super::OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
			//CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
			//IO_HELPER->CheckAndSendOutCard();lichao
			return -1;
		}
		iter->productCategory = productInfo.productCategory;				// 产品种类
		iter->dtValidateDateTime = productInfo.ValidEndTime;				// 产品有效期结束时间
		//if (iter->productCategory == PURSE) {
		//	iter->lBalanceAmount = productInfo.Balance;						// 车票余额
		//}
		//else if (iter->productCategory == MULTIRIDE) {
		//	iter->lBalanceRides = productInfo.Balance;						// 车票余次
		//}
		iter->lBalanceAmount = productInfo.Balance;	
		iter->beginLocation = productInfo.dwStartStation;					// 区段起点
		iter->endLocation = productInfo.dwEndStation;						// 区段终点
		iter->useArea	  = productInfo.wArea;								// 使用区段		
	}


	m_pDataModel->productInfo = m_pDataModel->vecTicketProduct[0];

	DoDialogFlow(WM_WAIT_TO_INIT);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TPU充值处理(储值/计次/定期) 

@param      none

@retval  	true:处理成功  false:失败信息

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
long CChargeSvc::DoCardCharge()
{
	long resultCode = 0;
	try{		
		// 显示等待读卡消息框等待时间
		int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();
		// 设置状态为正在运行中
		theSERVICE_MGR.SetState(ISSUEING);
	
		theSERVICE_MGR.SetForeServiceBusy(true);// 结束或者异常，自动置为false。
		//theAudio_HELPER.PlayAudio(AUDIO_ID__WAITINGTRANSACTION);

		RW_CMD_CHARGE chargeInput;
		chargeInput.SerialNo = m_pDataModel->dwUDSN;				// 交易流水号
		chargeInput.TranDateTime = m_pDataModel->transTime;			// 交易时间
		////chargeInput.CardType = static_cast<WORD>(m_pDataModel->mediumInfo.cardPHType); // 因为是TPU相关而非参数相关，使用CardType_t
		//chargeInput.ChargeType = static_cast<BYTE>(m_pDataModel->productInfo.productCategory/*m_pDataModel->productInfo.productType*/);
		//chargeInput.ChargeProductType = static_cast<WORD>(m_pDataModel->productInfo.productType);		//产品主类型
		//chargeInput.ChargeSubProductType = static_cast<BYTE>(m_pDataModel->productInfo.productSubType); //产品子类型

		// 无需从m_pDataModel->productInfo.productType判断充值类型是SVT或MST，因为否则按初值自动记零
		//if (chargeInput.ChargeProductType != SVT){
		//	m_pDataModel->lAddAmount = m_pDataModel->paymentInfo.lDueAmount;
		//}
		m_pDataModel->lAddAmount = m_pDataModel->paymentInfo.lPaidAmount;
		chargeInput.ChargeAmount = static_cast<DWORD>(m_pDataModel->lAddAmount);	// SVT充值金额,包括MST和PST子产品需要收的钱。
		chargeInput.ChargeCount = static_cast<DWORD>(m_pDataModel->lAddRides);		// MST充值次数 
		// PST相关信息可以通过subproductType自动计算出。

		chargeInput.PaymentMethod = static_cast<BYTE>(m_pDataModel->paymentInfo.paymentMethod);		// 支付方式
		if (m_pDataModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
			memcpy(chargeInput.BankCardNum, m_pDataModel->paymentInfo.BankCardNum, 10*sizeof(BYTE));	// 银行卡号
		}

		RW_BUSSINESS_RESPONSE response;
		resultCode = RECHARGERW_HELPER->Charge(chargeInput,response);

		theSERVICE_MGR.SetForeServiceBusy(false);

		if(resultCode == RW_RSP_OK) {			
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			// 更新UD数据
			if(response.UDlen != 0) {
				RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
			}
			// 更新AR数据
			if(response.ARLen != 0) {
				RECHARGERW_HELPER->SaveRWARData(response.ARLen,response.ARData);//zhengxianle充值PST时报错，暂时屏蔽
			}
		}
		else {
			m_pDataModel->operationInfo.finishType = TRANSACTION_STATE_FAILED;
			m_pDataModel->operationInfo.mapException[TRANSACTION_EXCEPTION_WRITE_CARD_FAILED] = m_pDataModel->paymentInfo.lPaidAmount;
			// 充值失败对应的交易没有对应交易流水号。
			m_pDataModel->dwUDSN = 0;
			//m_pDataModel->operationInfo.finishType = WRITE_CARD_FAILED;
		}
		//m_pDataModel->dwUDSN = theCOUNT_INFO.GetUDSN();// 取值并加一
		return resultCode;
	}
	catch (CSysException& e) {
		// 异常在父类处理
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
@brief      确认两次刷卡是否相同 

@param      none

@retval  	true:处理成功  false:失败信息

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::ConfirmCard(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	theSERVICE_MGR.SetState(ISSUEING);
	// 初始化默认无卡。
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
//@brief      充值流程
//
//@param      none
//
//@retval  	bool	true:成功	false:失败
//
//@exception  CSysException
//*/
////////////////////////////////////////////////////////////////////////////
//bool CChargeSvc::ChargeCard()
//{
//	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CHARGE_DOING));
//	theAPP_SESSION.ShowReceptionGuide(GUIDE_CHARGE_DOING);
//
//	// 首次写卡
//	long resultCode = DoCardCharge();
//	if (resultCode != RW_RSP_OK) {
//		while (true) {
//			// 错误信息、重试对话框
//			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(resultCode));
//			INT_PTR result = CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(GUIDE_CHARGE_MSGBOX_RETRY),_T(".\\res\\SJT_issue.png"),CMessagePictureBox::BTN_OKCANCEL,DT_CENTER);
//			if (result != IDOK) {
//				return false;
//			}
//
//			// 重新读卡写卡
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
@brief      保存SC数据

@param      none

@retval  	无

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
@brief      写入数据库数据

@param      none

@retval  	无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::WriteDBData(Data::TransactionRecord &dbInfo) {

	// 参照数字编码参阅Sec1.2.2.1.7-9, MACAU-DGB20-0010 AFC Internal Interface(ccs-sc-sle)-Appendix C-Transaction-v00.00.00.03.docx
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

	//dbInfo.iTransactionDetail.iState; // 对于不同结束方式的具体情形具体写出

	__super::WriteDBData(dbInfo);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      银行卡支付
//
//@param      none
//
//@retval     bool	true:成功	false:失败
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//bool CChargeSvc::POSPayment()
//{
//	try {
//		// ......（TPU接口暂不确定）
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
//@brief      银行卡冲正
//
//@param      none
//
//@retval     bool	true:成功	false:失败
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//bool CChargeSvc::POSCancle()
//{
//	try {
//		// ......（TPU接口暂不确定）
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
@brief      供OnCancel和各种超限使用的一个函数封装

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::FlowToFinishDlg()
{
	//if(theAPP_SESSION.IsBlindManOperation()){
	//	if(m_operation_status == BLIND_OPERATION_STATUS::STATUS_BLIND_FIRST_SWIPE_CARD){
	//		// 密码键盘没有停止，需要停止后再返回。
	//		if(!m_bIsPinPadStoped){
	//			m_pDataModel->operationInfo.finishType = CPassengerForeSvc::RUNNING_OUT_OF_TIME;
	//			//PINPAD_HELPER->TurnOffInputTrigger();// 关闭密码键盘:回调中处理				
	//			return;
	//		}
	//		else{// 密码键盘已关闭，直接启动默认业务
	//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
	//			return;
	//		}
	//	}
	//	// 第二次刷卡取消
	//	else if(m_operation_status == BLIND_OPERATION_STATUS::STATUS_BLIND_SECOND_SWIPE_CARD){
	//		PlayEventAudio(AUDIO_BUSINESS_CANCEL,true);
	//		m_operation_status = STATUS_BLIND_BUSINESS_CANCEL;// 投钱后取消
	//		if(!m_bIsPinPadStoped){
	//			//PINPAD_HELPER->TurnOffInputTrigger();// 停止输入
	//			return;
	//		}
	//		else{
	//			return;
	//		}
	//	}
	//}
	m_enumIsReadedCard = ABORD;
	// 检测并弹出储蓄卡
	//IO_HELPER->CheckAndSendOutCard();lichao

	DoDialogFlow(WM_INIT_TO_FINISH);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      供OnCancel和各种超限使用的一个函数封装

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::FlowToErrorDlg()
{
	//__super::FlowToErrorDlg();

	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_INIT){
		//if(theAPP_SESSION.IsBlindManOperation()){
		//	// 密码键盘是否停止
		//	if (m_bIsPinPadStoped){
		//		// 现金投入数量已经达到最大限制
		//		if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_COUNT_LIMIT  ||
		//			m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_MONEY_AMOUNT_LIMIT){
		//				//交易失败:投入超限。
		//				m_operation_status = STATUS_BLIND_INSERT_CASH_MAX;
		//				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		//				PlayEventAudio(AUDIO_COMMON_INSERT_MAX_AMOUNT,true);
		//		}
		//		else if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_COUNT_LIMIT  ||
		//			m_pDataModel->operationInfo.finishType == CPassengerForeSvc::OVER_CHANGE_AMOUNT_LIMIT){
		//				//交易失败:找零不足。
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
		//		// 因错误而失败
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
		//		DoDialogFlow(WM_INIT_TO_ERROR); // 画面流转
		//	}
		//	else{
		//		//m_PinpadInputType = UE_PINPADKEY::UE_PINPAD_RIGHT;
		//		//PINPAD_HELPER->TurnOffInputTrigger();// 停止监控密码键盘
		//	}
		//}
		//else
			
		// 检测并弹出储蓄卡
			//IO_HELPER->CheckAndSendOutCard();lichao

		DoDialogFlow(WM_INIT_TO_ERROR); // 画面流转
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
@brief		转向默认界面  

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
@brief      打印销售凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PrinterPrintReceipt()
{
	try {
		// 加载模板
		CString templateName= _T("TVM_CHARGE.template");
		CStringArray printTexts;
		CStringArray templateTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

		// 填充数据
		int index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			CString strSerialNo = _T("");
			strSerialNo.Format(_T("%010d"),m_pDataModel->dwUDSN);
			CString strCardNum = m_pDataModel->mediumInfo.cardLogicalNumber;
			line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{交易号}"),strSerialNo);
			line.Replace(_T("{用户卡号}"),strCardNum);
			line.Replace(_T("{票卡类型}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_CHINESE));
			line.Replace(_T("{充值前余额}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{充值金额}"),ComFormatAmount(m_pDataModel->lAddAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{充值后余额}"),ComFormatAmount(m_pDataModel->lAddAmount + m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			
			// 打印时字符串的格式化处理（模板读取数据）
			CStringArray printLines;
			ComPrintLineFormatForTemplate(line,printLines);
			printTexts.Append(printLines);
			++index;
		}

		// 打印帐票
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
@brief      打印错误凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PrinterPrintErrorReceipt()
{
	try {
		// 加载模板
		CString templateName= _T("TVM_CHARGE_FAILED.template");
		CStringArray printTexts;
		CStringArray templateTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

		// 填充数据
		int index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			CString strSerialNo = _T("");
			strSerialNo.Format(_T("%010d"),m_pDataModel->dwTransactionID);
			CString strCardNum = m_pDataModel->mediumInfo.cardLogicalNumber;
			line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{交易号}"),strSerialNo);
			line.Replace(_T("{用户卡号}"),strCardNum);
			line.Replace(_T("{票卡类型}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_CHINESE));
			line.Replace(_T("{充值前余额}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{充值金额}"),ComFormatAmount(m_pDataModel->lAddAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{充值后余额}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{故障金额}"),ComFormatAmount(m_pCommonModel->paymentInfo.lReturnAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			// 打印时字符串的格式化处理（模板读取数据）
			CStringArray printLines;
			ComPrintLineFormatForTemplate(line,printLines);
			printTexts.Append(printLines);
			++index;
		}

		// 打印帐票
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
@brief      判断是否硬币纸币都达到最大数量，供PassengerForeSvc里的函数OnCoinInserted和OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
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
@brief      判断是否纸币达到最大数量，供PassengerForeSvc里的函数OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsBanknoteMaxCount() {
	// 不支持纸币，表示已满
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
		return true;
	}
	// 判断当前钱箱数量
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	long lCurCount = banknoteInfo.BankntoeTotalCount();//banknoteInfo.ulCount_10 + banknoteInfo.ulCount_100 + banknoteInfo.ulCount_1 + banknoteInfo.ulCount_20 + banknoteInfo.ulCount_50 + banknoteInfo.ulCount_5;
	
	// 先判断当前投入是否大于剩余可投入数量？
	if(m_pDataModel->paymentInfo.lBHReceiveTotalCount >= (theAFC_TVM_OPERATION.GetBanknotesFullConut() - lCurCount)){
		return true;
	}
	// 再判断当前投入数量是否超过充值最大数量限制？
	if (m_pDataModel->paymentInfo.lBHReceiveTotalCount > theAFC_TVM_OPERATION.GetAcceptBanknotesMaxPieceOfRecharge()) {
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否硬币达到最大数量，供PassengerForeSvc里的函数OnCoinInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsCoinMaxCount(){
	// 不支持硬币，表示已满
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
		return true;
	}
	// 1.取各个票箱数据
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_A_info = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_B_info = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//CTCoinCountInfo::COIN_BOX_INFO stock				 = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//// 2.回收箱最大可接收数量
	//long ulBoxA = theAFC_TVM_OPERATION.GetRecycleBoxAFullCount() - box_A_info.GetCurCount();
	//long ulBoxB = theAFC_TVM_OPERATION.GetRecycleBoxBFullCount() - box_B_info.GetCurCount();
	//// 3.循环找零箱最大接收数量？
	//long ulStock = (long)theTVM_INFO.GetCHStock1Max() - stock.ulCurCount;

	//// 4.已经接收的1MOP的数量
	//long ulReciveOneCount   = m_pDataModel->paymentInfo.PayCoinInfo[Coin1MOP] ;
	//// 5.已经接收非1MOP的数量
	//long ulReciveOtherCount = m_pDataModel->paymentInfo.PayCoinInfo[Coin2MOP] + m_pDataModel->paymentInfo.PayCoinInfo[Coin5MOP] + m_pDataModel->paymentInfo.PayCoinInfo[Coin50AVOS];

	//// 6.如果暂存1的数量大于循环找零箱和回收箱1的数量、如果暂存2的数量大于等于回收箱2的数量
	//if(ulReciveOneCount > ulStock && ulReciveOneCount > ulBoxA || ulReciveOtherCount > ulBoxB){
	//	return true;
	//}
	//// 7.如果暂存1大于暂存1最大接收数量，或者暂存2的数量大于等于暂存2最大接收枚数
	//if(ulReciveOneCount >= theTVM_INFO.GetCHTemporary1Max() || ulReciveOtherCount >= theTVM_INFO.GetCHTemporary2Max()){
	//	return true;
	//}
	//// 8.如果上述不超过最大限制，则判断投入是否超过参数最大限制
	//if (m_pDataModel->paymentInfo.lCHReceiveTotalCount >= theAFC_TVM_OPERATION.GetAcceptCoinMaxPieceOfRecharge()){
	//	return true;
	//}
	//else{
	//	return false;
	//}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币都达到最大金额，供PassengerForeSvc里的函数OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CChargeSvc::IsBanknoteMaxAmount() {
	// 包括最大接收金额
	if (m_pDataModel->paymentInfo.lBHReceiveTotalAmount > theAFC_TVM_OPERATION.GetAcceptBanknotesMaxAmountOfRecharge()*100) { // acceptChrgrMaxBillAmt返回“元”，但是BHReceiveMoney的参数是“分”。
		return true;
	}
	else {
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断找零数量是否超过最大限制

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CChargeSvc::IsCoinChageMaxCount(){
	// 找零总数量
	DWORD dwChangeCount = m_pDataModel->paymentInfo.dwChangeCoinHalfYuanCount + m_pDataModel->paymentInfo.dwChangeCoinOneYuanCount;//m_pDataModel->paymentInfo.dwChangeStockCount + m_pDataModel->paymentInfo.dwChangeHopper1Count + m_pDataModel->paymentInfo.dwChangeHopper2Count;
	if (dwChangeCount > theAFC_TVM_OPERATION.GetChageCoinMaxPieceOfRecharge()){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零数量是否超限

@param      无

@retval     无

@exception  无
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
@brief      纸币找零金额是否超限

@param      无

@retval     无

@exception  无
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
@brief      获取可接收的纸币面额

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
@brief      获取可接收的硬币的面额

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COIN_ACCEPTABLE CChargeSvc::GetAcceptCoinFaceTable(){
	// 1.判断回收箱是否在位？
	COIN_ACCEPTABLE acceptTable;
	/*CH_COMMON_RSP rsp;
	try{
		COIN_HELPER->CH_GetStatus(&rsp);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}*/	
	// 2.将可接收面额转换为Table表
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
	// 3.回收箱2不在位，或者回收箱2已满，不可以接收非一元的
// 	if(!m_CoinMoudelStatus.sensor_info.CH_BOX2_SET_STATUS || (box_B_info.GetCurCount() >= theAFC_TVM_OPERATION.GetRecycleBoxAFullCount())){
// 		acceptTable.CH_COIN_ALLOW_05 = false;
// 		acceptTable.CH_COIN_ALLOW_2	 = false;
// 		acceptTable.CH_COIN_ALLOW_5  = false;
// 	}
// 	// 4.回收箱1不在位，判断当前stock是否已满？或者两者都满？
// 	CTCoinCountInfo::COIN_BOX_INFO stock = theCOIN_COUNT.GetCoinHopperboxCInfo();
// 	if (!m_CoinMoudelStatus.sensor_info.CH_BOX1_SET_STATUS && stock.ulCurCount >= theTVM_INFO.GetCHStock1Max() ||
// 		(stock.ulCurCount >= theTVM_INFO.GetCHStock1Max()) && (box_B_info.GetCurCount() >= theAFC_TVM_OPERATION.GetRecycleBoxBFullCount())){
// 		acceptTable.CH_COIN_ALLOW_1 = false;
// 	}
	return acceptTable;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		获取增加产品时长单位  

@param      Duration_t 增加时长

@retval     CString 转换后的时长加单位

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CChargeSvc::GetAddProductTime(Duration_t date){
	//充值时长:时长 单位
	DurationUnit_t  dtUnit  = GetDurationType(date);  
	DurationValue_t dtValue = GetDurationValue(date);
	CString strReturn;
	CString strUnit;
	switch(dtUnit){
	case DurationUnit_t::DURATION_UNIT_SECOND:			// 秒
		strUnit = _tl(SECOND_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_MINUTE:			// 分钟
		strUnit = _tl(MINUTE_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_HOUR:			// 小时
		strUnit = _tl(HOUR_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_DAY:				// 天
		strUnit = _tl(DAY_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_WEEK:			// 星期
		strUnit = _tl(WEEK_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_CALENDAR_MONTH:	// 月
		strUnit = _tl(MONTH_CNT_UNIT);
		break;
	case DurationUnit_t::DURATION_UNIT_YEAR:			// 年
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
@brief      封装语言播报接口，启动计时器用

@param      DWORD enventID (播放语音的ID)

@param		bool  bIsStopRepeat	(是否重复播放)

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PlayEventAudio(DWORD enventID,bool bIsStopRepeat /* = false */){
	// 非盲人充值，不做语言播放处理
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
	if(nTimerUpTime == 0 || bIsStopRepeat/* == true ? 不重复 : 重复*/)
		StopTimer(TIMERS_OUT_OF_REPLAY);
	else
		StartTimer(TIMERS_OUT_OF_REPLAY,m_VoicePlayTime = nTimerUpTime + nPlayedTimes/1000+1);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      语音播放计时器

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
			// 读卡框还在，直接结束掉
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
				// 发出读卡命令
				SERVICE_ID serviceIDCurrent = (SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID();
				RECHARGERW_HELPER->ReadCard(serviceIDCurrent,false);
			}
		}
		// 同时调用父类计时器
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
@brief		密码键盘回调函数      

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CChargeSvc::OnPinPadPressed(UINT ret,CTVMPinPadStartInput* pPinPadInput){
//	try{
//		// 盲人购票用
//		if(theAPP_SESSION.IsBlindManOperation()){
//			// 密码键盘返回成功
//			if(ret == 0){
//				UE_PINPADKEY emGetValue = PINPAD_HELPER->PraserInputValue(pPinPadInput->getInputValue());
//				m_bIsPinPadStoped = true;
//				StopTimer(TIMERS_OPERATION_OUT_OF_TIME);// 终止操作超时计时器
//				LOG("密码键盘运行正常：%x",emGetValue);
//				// 1.手动终止密码键盘
//				if(emGetValue == UE_UNKNOWN_KEY|| UE_PINPAD_NOINPUT == emGetValue){
//					LOG("Unknow key");
//					// 手动取消,未投钱,转到默认界面
//					if (m_operation_status == STATUS_BLIND_CANCEL){
//						theAPP_SESSION.SetBlindManStatus(false);
//						//PINPAD_HELPER->PowerOff();
//						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// 手动取消，有硬件动作。
//					else if(m_operation_status == STATUS_BLIND_BUSINESS_CANCEL){
//						if(theSERVICE_MGR.GetState() != CANCELING)
//							__super::OnCancel();
//						return;
//					}
//					// 正在充值中，密码键盘关闭
//					else if(m_operation_status == STATUS_BLIND_ISSUING){
//						return;
//					}
//					// 投入或找零数量超限
//					else if(m_PinpadInputType == UE_PINPAD_RIGHT){
//						FlowToErrorDlg();
//						return;
//					}
//					// 投钱超时
//					else if(m_pDataModel->operationInfo.finishType == INSERT_OUT_OF_TIME){
//						// 判断是否投币？
//						if(m_pDataModel->operationInfo.banknoteAcceptState == CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_NONE && m_pDataModel->operationInfo.coinAcceptState == CPassengerForeSvc::COIN_ACCEPT_STATE_NONE){
//							PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//						}
//						// 已投币
//						else{
//							//PlayEventAudio(AUDIO_COMMON_MSG_TIMEOUT_CANCEL,true);
//						}
//						return;
//					}
//					// 乘客取消
//					else if(m_pDataModel->operationInfo.finishType == PASSAGE_CANCEL){
//						//PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);// 交易已取消
//						//PINPAD_HELPER->PowerOff();
//						//theAPP_SESSION.SetBlindManStatus(false);
//						//// 启动默认业务
//						//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// 交易超时
//					else if(m_pDataModel->operationInfo.finishType == RUNNING_OUT_OF_TIME){
//						// 第一次刷卡时，交易取消
//						if(STATUS_BLIND_FIRST_SWIPE_CARD == m_operation_status){//Ok
//							PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);// 交易已取消
//							//PINPAD_HELPER->PowerOff();
//							theAPP_SESSION.SetBlindManStatus(false);
//							// 启动默认业务
//							theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//							return;
//						}
//						// 其他超时
//						else {
//							if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//								m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_OPERATION_OUT_OF_TIME;
//								GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);// 点击屏幕上的取消
//								return;
//							}						
//						}
//					}
//					// 读卡失败或该卡不支持充值处理
//					else if(m_operation_status == STATUS_BLIND_READ_CARD_FAILED){
//						//PINPAD_HELPER->PowerOff();
//						theAPP_SESSION.SetBlindManStatus(false);
//						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//						return;
//					}
//					// 手动取消
//					else{
//						// 未投钱
//						if(!m_pDataModel->operationInfo.isBeginBuyProduct){
//							//PINPAD_HELPER->PowerOff();
//							theAPP_SESSION.SetBlindManStatus(false);
//							theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//							return;	
//						}
//						// 已投钱
//						else{
//							OnCancel();
//							return;
//						}
//					}
//				}// End 1
//				// 2.第一次刷卡
//				else if(STATUS_BLIND_FIRST_SWIPE_CARD == m_operation_status){
//					// 只响应Cancel键
//					if(emGetValue == UE_PINPAD_CANCLE){
//						PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);// 交易已取消
//						CMessagePictureBox::End(IDABORT);// 这时会回调读卡无卡，在PassengerForeSvc中处理
//						//PINPAD_HELPER->PowerOff();
//						theAPP_SESSION.SetBlindManStatus(false);
//						return;
//					}
//					else {
//						// Do nothing
//					}
//				}
//				// 3.选择产品
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
//					// 确认键
//					else if(emGetValue == UE_PINPAD_ENTER){
//						int nMenuCode = 0;
//						// 判断是否合法
//						vector<int>::iterator it = m_vPassengerInput.begin();
//						while(it != m_vPassengerInput.end()){
//							nMenuCode = nMenuCode*10 + *it;
//							it++;
//						}
//						int nCount = m_pDataModel->vecTicketProduct.size();
//						// 判断是否有此产品:需要注意语言播放与现在读出的数据对应!!!
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
//								// 当只有一个子产品时
//								if(nSingleCount == 1){
//									bIsCorrect = true;
//									type = MST;
//								}
//								// 同一种产品有多个子产品,提示选择子产品
//								else{
//									m_pDataModel->productInfo.productType = MST;
//									PlayAudioInDlg(AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST);
//									// 开启密码键盘监控
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
//								// 提示选择子产品
//								else{
//									m_pDataModel->productInfo.productType = PST;
//									PlayAudioInDlg(AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST);
//									// 开启密码键盘监控
//									TurnOnPinPadAndTimer();
//									m_vPassengerInput.clear();
//									return;
//								}
//							}
//							if(bIsCorrect){
//								BTN_INFO btn;
//								btn.btnMsg.Format(_T("%02d%02d"),type,subType);
//								GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);// 点击对应的按钮
//							}
//							else {
//								PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//							}
//						
//						}
//						m_vPassengerInput.clear();
//					}
//					// 输入有误，请重新输入
//					else{
//						m_vPassengerInput.clear();
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//				}
//				// 4.选择充值子产品
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
//					// 确认键
//					else if(emGetValue == UE_PINPAD_ENTER){
//						int nMenuCode = 0;
//						// 判断是否合法
//						vector<int>::iterator it = m_vPassengerInput.begin();
//						while(it != m_vPassengerInput.end()){
//							nMenuCode = nMenuCode*10 + *it;
//							it++;
//						}
//						// 查找是否有当前输入？
//						map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//						// SVT
//						if(m_pDataModel->productInfo.productType == SVT){
//							if(itor != m_KeyValue.end()){
//								BTN_INFO btn;
//								btn.btnMsg.Format(_T("%d"),itor->second * 100);
//								GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_2_GROUP_ID,(LPARAM)&btn);
//								// 继续监控
//							}
//							else{
//								// 无此子产品
//								PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//							}
//						}
//						// PST、MST
//						else {
//							if(itor != m_KeyValue.end()){
//								bool bIsFind = false;
//								BTN_INFO btn;
//								vector<PRODUCT_INFO>::iterator iter = m_pDataModel->vecTicketProduct.begin();
//								
//								// 定期票
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
//								// 计次票
//								else if(m_pDataModel->productInfo.productType == MST){
//									while(iter != m_pDataModel->vecTicketProduct.end()){
//										// 输入的为指定次数
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
//					// 输入有误，请重新输入
//					else{
//						m_vPassengerInput.clear();
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//				}
//				// 5.选择支付方式
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
//					// 确认键
//					else if(emGetValue == UE_PINPAD_ENTER){
//						int nMenuCode = 0;
//						// 判断是否合法
//						vector<int>::iterator it = m_vPassengerInput.begin();
//						while(it != m_vPassengerInput.end()){
//							nMenuCode = nMenuCode*10 + *it;
//							it++;
//						}
//
//						// 查找是否有当前输入？
//						map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//						if(itor != m_KeyValue.end()){
//							BTN_INFO btn;
//							btn.btnMsg.Format(_T("%d"), itor->second);
//							GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_3_GROUP_ID,(LPARAM)&btn);
//							// 继续监控
//						}
//						else{
//							// 无当支付方式
//							PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//						}
//						m_vPassengerInput.clear();
//					}
//					// 输入有误，请重新输入
//					else{
//						m_vPassengerInput.clear();
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//				}
//				// 6.交易确认
//				else if(STATUS_BLIND_CONFIRM_BUSINESS == m_operation_status){
//					if(emGetValue == UE_PINPAD_ENTER){// 确认交易
//						if(GetCurReceptionDlg()->IsButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
//						}						
//					}
//					else if(emGetValue == UE_PINPAD_CANCLE){// 取消交易
//						if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//							return;
//						}
//						
//					}
//				}
//				// 投入现金
//				else if(STATUS_BLIND_INSERT_CASH == m_operation_status){
//					if(emGetValue == UE_PINPAD_CANCLE){
//						if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//							return;
//						}						
//					}
//				}
//				// 确认支付信息
//				else if(STATUS_BLIND_CONFIRM_PAYMENT == m_operation_status){
//					if(emGetValue == UE_PINPAD_ENTER){
//						//GetCurReceptionDlg()->PostMessage(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);//!!!
//						if(GetCurReceptionDlg()->IsButtonEnable()){
//							GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);// 同步调用界面上的函数，直到返回为止
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
//				// 插入银行卡（预留）
//				else if(STATUS_BLIND_INSERT_BANKCARD == m_operation_status){
//					return;
//				}
//				// 输入银行卡密码
//				else if(STATUS_BLIND_INPUT_PASSWORD == m_operation_status){
//					return;
//				}
//				// 第二次刷卡
//				else if(STATUS_BLIND_SECOND_SWIPE_CARD == m_operation_status){
//					//if(emGetValue == UE_PINPAD_CANCLE){// 只响应取消键
//					//	LOG("取消交易，第二次读卡时。");
//					//	CMessagePictureBox::End(IDABORT);
//					//	return;
//					//}// 其他的不响应
//					//LOG("其他无效按键");
//					return;
//				}
//				// 交易中
//				else if(STATUS_BLIND_ISSUING == m_operation_status){
//					// Do nothing
//					return;
//				}
//				// 充值成功
//				else if(STATUS_BLIND_BUSINESS_SUCCESS == m_operation_status){
//					return;
//				}
//				// 充值失败
//				else if(STATUS_BLIND_BUSINESS_FAILED == m_operation_status){
//					return;
//				}
//				// 交易取消（硬件）
//				else if(STATUS_BLIND_BUSINESS_CANCEL == m_operation_status){
//					return;
//				}
//				// 打印完成
//				else if(STATUS_BLIND_PRINT_SUCCESS == m_operation_status){
//					return;
//				}
//				// 交易取消
//				else if(STATUS_BLIND_CANCEL == m_operation_status){
//					return;
//				}
//				// 刷卡失败
//				else if(STATUS_BLIND_READ_CARD_FAILED == m_operation_status){
//					return;
//				}
//				// 其他
//				else {
//					return;
//				}
//				// 开启密码键盘监控
//				TurnOnPinPadAndTimer();
//			}
//			// 执行失败
//			else {
//				LOG("执行失败。");
//			}
//		}
//		// 其他，有可能是输入银行卡密码(预留)
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
@brief      启动盲人购票超时计时器

@param      bool bIsStartPinPad (是否启动密码键盘)

@param      bool bIsStartOutOfTimer(是否开启操作超时计时器)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CChargeSvc::TurnOnPinPadAndTimer(bool bIsStartPinPad /* = true */,bool bIsStartOutOfTimer /* = true */){
//	try{
//		if(bIsStartPinPad && m_bIsPinPadStoped){
//			//PINPAD_HELPER->TurnOnInputTrigger(this);
//			m_bIsPinPadStoped = false;
//		}
//		int nTimer = theAFC_TVM_OPERATION.GetCancelDealTime() + (m_VoicePlayTime >=0 ? m_VoicePlayTime : 0);// 60S + 本次语音播放时间
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
@brief      播放语音并设置状态(界面驱动状态)

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::PlayAudioInDlg(DWORD eventID){
	
	LOG("Play vedio:%d",eventID);
	//if(eventID == 0){
	//	m_pChargeAudioHelper->StopPlayEventAudio();
	//	return;
	//}
	//// 1.播放第一次刷卡语音
	//else if(eventID == AUDIO_RECHARGE_FIRST_SWIPE_CARD){
	//	// AVM 语音播放需要特殊处理
	//	if(DEVICE_TYPE_AVM == theMAINTENANCE_INFO.GetDeviceType())
	//		eventID = AUDIO_RECHARGE_FIRST_SWIPE_CARD_FOR_AVM;
	//	m_operation_status = STATUS_BLIND_FIRST_SWIPE_CARD;
	//	PlayEventAudio(eventID);
	//	TurnOnPinPadAndTimer();// 开启密码键盘并开启操作超时计时器
	//	return;
	//}
	//// 2.播放语言：选择产品类型
	//else if(eventID == AUDIO_RECHARGE_SELECE_PRODUCT_TYPE)	
	//	m_operation_status = STATUS_BLIND_SELECT_PRODUCT;
	//// 3.1请选择要充值的子产品：MST
	//else if(eventID == AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST){
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_MST);
	//	m_operation_status = STATUS_BLIND_SELECT_SUB_PRODUCT;
	//}
	//// 3.2请选择要充值的子产品：PST
	//else if(eventID == AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST){
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE_PST);
	//	m_operation_status = STATUS_BLIND_SELECT_SUB_PRODUCT;
	//}
	//// 3.3乘客选择要充值的子产品：SVT
	//else if(eventID == AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE){// 选择子产品	
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_RECHARGE_SELECT_SON_PRODUCT_TYPE);
	//	LOG("Get Map");
	//	m_operation_status = STATUS_BLIND_SELECT_SUB_PRODUCT;
	//}
	//// 4.乘客选择支付方式
	//else if(eventID == AUDIO_SELECT_PAYMENT){
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_SELECT_PAYMENT);
	//	m_operation_status = STATUS_BLIND_SELECT_PAYMENT;
	//}
	//// 5.提示确认交易信息
	//else if(eventID == AUDIO_RECHARGE_CONFIRM_BUSINESS_SVT || eventID == AUDIO_RECHARGE_CONFIRM_BUSINESS_PST || eventID == AUDIO_RECHARGE_CONFIRM_BUSINESS_MST){
	//	m_operation_status = STATUS_BLIND_CONFIRM_BUSINESS;// 确认交易信息
	//}
	//// 6.确认交易信息后 ：投入现金
	//else if(eventID == AUDIO_INSERT_CASH){
	//	m_operation_status = STATUS_BLIND_INSERT_CASH;
	//	StopTimer(TIMERS_OPERATION_OUT_OF_TIME);// 停止操作超时计时器，由投入现金计时器托管
	//}
	//// 7.确认支付信息
	//else if(eventID == AUDIO_CONFORM_PAYINFO)
	//	m_operation_status = STATUS_BLIND_CONFIRM_PAYMENT;
	//// 6.1确认信息后：插入银行卡(预留)
	//else if(eventID == AUDIO_INSERT_BANKCARD)
	//	m_operation_status = STATUS_BLIND_INSERT_BANKCARD;		
	//// 7.1输入银行卡密码(预留)
	//else if(eventID == AUDIO_INPUT_PASSWORD){

	//}
	//// 8.提示交易成功,是否需要打印？
	//else if(eventID == AUDIO_BUSINESS_SUCCESS){
	//	m_operation_status = STATUS_BLIND_BUSINESS_SUCCESS;
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
	//	PlayEventAudio(eventID,true);
	//	return;
	//}
	//// 9.打印：打印成功
	//else if(eventID == AUDIO_BUSINESS_PRINT_SUCCESS){
	//	m_operation_status = STATUS_BLIND_PRINT_SUCCESS;
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
	//	PlayEventAudio(eventID,true);
	//	return;
	//}
	//// 10.交易取消，未开始收款
	//else if(eventID == AUDIO_BUSINESS_CANCEL_NORMAL){
	//	m_operation_status = STATUS_BLIND_CANCEL;
	//	PlayEventAudio(eventID,true);// 交易取消
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
	//// 11.交易取消，已收款，需要退钱。
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
	//	if(!m_bIsPinPadStoped)// 关闭密码键盘
	//		//PINPAD_HELPER->TurnOffInputTrigger();
	//	return;
	//}
	//// 当前不支持此支付方式
	//else if(eventID == AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT){
	//	m_operation_status = STATUS_BLIND_SELECT_PAYMENT;
	//	m_KeyValue.clear();
	//	m_pChargeAudioHelper->getKeyMap(m_KeyValue,AUDIO_SELECT_PAYMENT);
	//	PlayEventAudio(eventID);
	//	TurnOnPinPadAndTimer(false,true);	
	//	return;
	//}

	//PlayEventAudio(eventID);
	//// 重置计时器
	//TurnOnPinPadAndTimer(false,true);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面上返回完成

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::ReturnFinishDlg(){
	// 充值已完成(成功、失败)
	//if(theAPP_SESSION.IsBlindManOperation()){
	//	if(m_pDataModel->operationInfo.finishType == TRANSACTION_STATE_FAILED){
	//		//PlayEventAudio(AUDIO_BUSINESS_FAILED,true);
	//		//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(STATUS_BLIND_RECHARGE);// 停止密码键盘监控
	//	}
	//}
	//// 交易失败，更新交易计数
	//if(m_pDataModel->operationInfo.finishType == CPassengerForeSvc::TRANSACTION_STATE_FAILED){
	//	m_pDataModel->dwTransactionID = theCOUNT_INFO.GetTransactionNum();// 自动加一
	//}
	//m_pDataModel->operationInfo.isFinishedJob = true;
	OnReturnFinish();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据模型中的数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::InitDataModel(){
	m_CardLogicalNumber = _T("");
	m_pDataModel->transTime = ComGetCurTime();					// 在业务启动时确定
	m_pDataModel->dwUDSN = theCOUNT_INFO.GetUDSNForDisplay();	// 在业务启动时确定
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
	
	// 取当前硬币模块状态
	if(!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		try{
			//if(theMAINTENANCE_INFO.GetDeviceType() == AFC_DEVICE_TYPE::DEVICE_TYPE_TVM)
				//COIN_HELPER->ReadStatus(m_CoinMoudelStatus);
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}	
	}
	// 隐藏功能区按钮及语言显示按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取应该显示的产品的单位

@param      无

@retval     无

@exception  无
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
@brief      乘客插卡回调处理函数

@param      UINT uiResult;						结果

@param      CIOCardPosMonitor* pIoCardMonotor

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CChargeSvc::OnChargeInsertCard(UINT uiResult,CIOCardPosMonitor* pIoCardMonitor){
//
//	try{
//		// 0.关闭锁卡电源
//		//IO_HELPER->CloseCardLocker();lichao
//
//		// 1.正常返回（已插入卡）
//		if(uiResult == 0){
//			// 读卡
//			RECHARGERW_HELPER->ReadCard((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID());
//		}
//		// 2.异常处理
//		else{
//			CMessagePictureBox::End();
//			// 2.1等待插入超时
//			if(uiResult == CIOException::ERROR_IO_READ_CARD_POS_TIMEOUT){
//				LOG("等待乘客插入充值卡超时。");
//			}
//			// 2.2手动取消
//			else if(uiResult == CIOException::ERROR_IO_READ_CARD_POS_CANCEL){
//				LOG("乘客手动取消插入充值卡。");
//			}
//			// 2.3异常取消
//			else if(uiResult == CIOException::ERROR_IO_READ_STATUS_EXCEPTION){
//				LOG("IO模块异常返回。");
//			}
//
//			// 返回默认业务
//			NOTIFY_PROCESSING_WINDOW();
//			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
//		}
//		
//	}
//	catch(CSysException& e){
//		// 读卡异常，检测SAM卡状态及其他异常(需要处理)
//
//	}
//	catch(...){
//
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}


void CChargeSvc::UpdateReceptDlg(CString strTxt)						// 更新乘客界面倒计时提示
{

	CChargeReceptionDlg* pMapDlg = (CChargeReceptionDlg*)(m_pDialogFlow->GetDialog(IDD_00301_CHARGE_RECEPT_DLG));
	pMapDlg->SetTipsShow(strTxt);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      出错处理

@param      （i）CString errorMsg  错误信息文言

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CChargeSvc::OnError(CSysException e)
{
	try{
		if (m_pDataModel->paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH){
			// 纸币及硬币模块异常在父类处理。
			if(e.GetModuleID() == CBRHHardwareException::MODULE_ID || e.GetModuleID() == CBRHException::MODULE_ID || 
				e.GetModuleID() == CCHException::MODULE_ID || e.GetModuleID() == CCHHardwareException::MODULE_ID ||
				e.GetModuleID() == CBHChangeHardwareException::MODULE_ID
				){
					__super::OnError(e);
					return;
			}
		}

		theSERVICE_MGR.SetForeServiceBusy(false);
		// 添加到异常队列
		theEXCEPTION_MGR.ProcessException(e);

		m_pCommonModel->operationInfo.finishType = TRANSACTION_STATE_FAILED;
		//// 1.判断是否是出票失败（M1）？
		//if(e.GetModuleID() == CTHException::MODULE_ID || e.GetModuleID() == CTHHardwareException::MODULE_ID){
		//	// 加入异常故障
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		//	// 预留：提示详细错误信息。
		//	OnReturnFinish();
		//	return;
		//}
		//// 2.是否有Token异常？
		////if(e.GetModuleID() == CTOKENException::MODULE_ID || e.GetModuleID() == CTOKENHardwareException::MODULE_ID){
		////	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_TOKEN_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		////	OnErrorFinish();
		////}
		//// 3.判断是否是读写卡失败？
		//if(e.GetModuleID() == CRWException::TVM_CARD_RW_MODULE_ID || e.GetModuleID() == CRWHardwareException::TVM_CARD_RW_MODULE_ID){
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
			OnReturnFinish();
			return;
		//}
		//// 4.其他硬件异常。(其他异常:判断是否票发完。)
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
@brief      清空保存的卡数据

@param      void

@retval     无

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