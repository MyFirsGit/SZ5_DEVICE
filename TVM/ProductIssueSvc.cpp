#include "StdAfx.h"

#include "ProductIssueSvc.h"

#include "WaitingOfficeDlg.h"
#include "ProductIssueReceptionDlg.h"
#include "PassengerResultDlg.h"

#include "TicketHelper.h"
#include "TemplateHelper.h"
#include "AudioHelper.h"
#include "tpuerrorinfo.h"
#include "SCAuditManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_WAIT_TO_INIT = _T("WM_WAIT_TO_INIT");
static const TCHAR* const WM_WAIT_TO_ERROR = _T("WM_WAIT_TO_ERROR");
static const TCHAR* const WM_INIT_TO_LOCATION = _T("WM_INIT_TO_LOCATION");
static const TCHAR* const WM_LOCATION_TO_PAYMENT = _T("WM_LOCATION_TO_PAYMENT");
static const TCHAR* const WM_LOCATION_PAYMENT_TO_FINISH = _T("WM_LOCATION_PAYMENT_TO_FINISH");
static const TCHAR* const WM_LOCATION_PAYMENT_TO_ERROR = _T("WM_LOCATION_PAYMENT_TO_ERROR");
static const TCHAR* const WM_INIT_TO_PAYMENT = _T("WM_INIT_TO_PAYMENT");
static const TCHAR* const WM_PAYMENT_TO_FINISH = _T("WM_PAYMENT_TO_FINISH");
static const TCHAR* const WM_INIT_TO_ERROR = _T("WM_INIT_TO_ERROR");
static const TCHAR* const WM_LOCATION_TO_ERROR = _T("WM_LOCATION_TO_ERROR");
static const TCHAR* const WM_PAYMENT_TO_ERROR = _T("WM_PAYMENT_TO_ERROR");

BEGIN_MESSAGE_MAP(CProductIssueSvc, CPassengerForeSvc)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CProductIssueSvc::CProductIssueSvc():CPassengerForeSvc(ISSUE_PRODUCT_SVC)
{
	// 在这个类的构造函数中，让*m_pDataModel和PassengerForeSvc里的*m_pCommonModel指向同一个struct。
	// 这样当调用这里的method时，使用*m_pDataModel，继承的更多struct变量在这里可视。
	// 在使用PassengerForeSvc里的method时，通过*m_pCommonModel则仍可操作DataModel中CommonModel的部分。
	m_pDataModel = new DataModel();
	m_pCommonModel = dynamic_cast<CommonModel*>(m_pDataModel);
	// 设置画面
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
	m_pDialogFlow->AddDialog(IDD_00301_CHARGE_RECEPT_DLG, new CProductIssueReceptionDlg(this));
	m_pDialogFlow->AddDialog(IDD_00303_CHARGE_RESULT_DLG, new CPassengerResultDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INIT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHOOSE_LOCATION);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHOOSE_LOCATION, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CHOOSE_PAYMENT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CHOOSE_PAYMENT, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH, IDD_00301_CHARGE_RECEPT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_00303_CHARGE_RESULT_DLG);

	// 设置画面流转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_INIT,DIALOG_GROUP_INIT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT,WM_WAIT_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_LOCATION,DIALOG_GROUP_CHOOSE_LOCATION);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHOOSE_LOCATION,WM_LOCATION_TO_PAYMENT,DIALOG_GROUP_CHOOSE_PAYMENT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHOOSE_LOCATION,WM_LOCATION_PAYMENT_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHOOSE_LOCATION,WM_LOCATION_PAYMENT_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_PAYMENT,DIALOG_GROUP_CHOOSE_PAYMENT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHOOSE_PAYMENT,WM_PAYMENT_TO_FINISH,DIALOG_GROUP_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHOOSE_LOCATION,WM_LOCATION_TO_ERROR,DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CHOOSE_PAYMENT,WM_PAYMENT_TO_ERROR,DIALOG_GROUP_ERROR);

	// 设置初始画面组
	//m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_INIT);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT); // should be this one

	//__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CProductIssueSvc::~CProductIssueSvc(void)
{
	delete m_pDataModel;
	m_pDataModel = NULL;
	m_pCommonModel = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动时响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::OnStart()
{
	__super::OnStart();
	m_pDataModel->transTime = ComGetCurTime();					// 在业务启动时确定
	m_pDataModel->dwUDSN = theCOUNT_INFO.GetUDSNForDisplay();	// 在业务启动时确定
	m_pDataModel->mediumInfo = MEDIUM_INFO();
	m_pDataModel->productInfo = PRODUCT_INFO();
	m_pDataModel->paymentInfo = PAYMENT_INFO();
	m_pDataModel->operationInfo = OPERATION_INFO();

	m_pDataModel->SalableProductCount = 3;
	m_pDataModel->SalableProductType = 0x07;

	m_IsTheSameCard = FALSE;
	m_nReadedCardCount = 0;
	m_vecReadCard.clear();

	// 隐藏功能区按钮及语言显示按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT); // should be this one
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CProductIssueSvc::DataModel& CProductIssueSvc::GetDataModel()
{
	return *m_pDataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      根据pReadCardResponse->SalableProductType，筛选可以充值的产品列表

@param      vector<CAccProductParam::PRODUCT_INFO>& vecProduct

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::FilterSalableProductList(vector<CAccTicketParam::TICKET_TYPE>& vecProduct)
{
	vector<CAccTicketParam::TICKET_TYPE>::iterator it;
	//for (it = vecProduct.begin();it != vecProduct.end();){
	//	if (((((it->productCategory == static_cast<BYTE>(PURSE))?1:0) + 
	//		  (((it->productCategory == static_cast<BYTE>(MULTIRIDE))?1:0)<<1) + 
	//		  (((it->productCategory == static_cast<BYTE>(PERIOD))?1:0)<<2)) & m_pDataModel->SalableProductType) == 0)
	//		  //可售产品类型
	//		  //按BIT位解析，从低到高位依次代表钱包，计次，定期
	//		  //钱包：00000001 =0x01
	//		  //计次：00000010 = 0x02
	//		  //钱包+计次 = 00000011 = 0x03
	//		  //定期：00000100 = 0x04
	//		  //钱包+定期 = 00000101 = 0x05
	//		  //计次+定期 = 00000110 = 0x06
	//		  //钱包+计次+定期 = 00000111 = 0x07

	//		  //以上判别对于it->productType在m_pDataModel->SalableProductType中归为不可充值情形
	//	{
	//		it = vecProduct.erase(it);
	//	}
	//	else {
	//		++it;
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得可新增的产品类型名称

@param      无

@retval     map<int,CString>&   获取可新增主产品信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
map<WORD,CString> CProductIssueSvc::GetAvailableProductNames()
{
	map<WORD,CString> map_AvailableProductNames;
	map_AvailableProductNames.clear();

	vector<CAccTicketParam::TICKET_TYPE> vecProduct;
//	if (m_pDataModel->mediumInfo.mediumType != ULTRALIGHT) // UL卡判断
//	{
////		theACC_PRODUCT.GetIssueProductInfo(m_pDataModel->mediumInfo.mediumType, theMAINTENANCE_INFO.GetDeviceType(), vecProduct);//获取售产品
//		FilterSalableProductList(vecProduct);
//		vector<CAccProductParam::PRODUCT_INFO>::iterator it;
//		for (it = vecProduct.begin();it != vecProduct.end();it++){
////			map_AvailableProductNames[it->productType] = theACC_PRODUCT.GetProductName(it->productType, theAPP_SESSION.GetCurrentClientLanguageType());
//		}
//	}
	
	return map_AvailableProductNames;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得可新增的子产品类型名称

@param      无

@retval     map<int,CString>&   获取可新增子产品信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
map<WORD,CString> CProductIssueSvc::GetAvailableSubproductNames()
{
	// It returns the same as local variable. However, it cannot be set as local because it is used 
	// (in ProductIssueReceptionDlg.cpp) as reference.
	map<WORD,CString> map_AvailableSubproductNames;
	//map_AvailableSubproductNames.clear();
	////CAccProductParam::SUB_PRODUCT subPdt;
	//vector<CAccProductParam::PRODUCT_INFO> vecProduct;
	//theACC_PRODUCT.GetIssueProductInfo(m_pDataModel->mediumInfo.mediumType,theMAINTENANCE_INFO.GetDeviceType(), vecProduct); 
	//vector<CAccProductParam::PRODUCT_INFO>::iterator it;
	//for (it = vecProduct.begin();it != vecProduct.end();it++){
	//	if (it->productType == m_pDataModel->productInfo.productType) {
	//		vector<CAccProductParam::SUB_PRODUCT> vecSubProduct;
	//		vecSubProduct = it->vecSubProduct;
	//		vector<CAccProductParam::SUB_PRODUCT>::iterator it2;
	//		for (it2 = vecSubProduct.begin(); it2 != vecSubProduct.end(); it2++) {
	//			if (!isSubProductExist(it->productType,it2->productTypeVariants)) // 如果当前票卡中存在该子产品就不再允许添加
	//			{
	//				map_AvailableSubproductNames[it2->productTypeVariants] = theACC_PRODUCT.GetSubProductName(it->productType, it2->productTypeVariants, theAPP_SESSION.GetCurrentClientLanguageType());
	//			}
	//		}
	//		break;
	//	}
	//}zhengxianle
	return map_AvailableSubproductNames;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得可选择的区段名称

@param      无

@retval     map<int,CString>&   获取可新增主产品信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
map<DWORD,CString> CProductIssueSvc::GetAvailableLocationNames()
{
	map<DWORD,CString> map_AvailableLocationNames;
	map_AvailableLocationNames.clear();

//	vector<CAccLocationParam::LOCATION> vecLocation;
//	theACC_LOCATION.GetAreaInfo(vecLocation);
//	vector<CAccLocationParam::LOCATION>::iterator it = vecLocation.begin();
//	for (it = vecLocation.begin();it != vecLocation.end();it++){
////		map_AvailableLocationNames[it->locationNumber] = theACC_LOCATION.GetLocationName(it->locationNumber, theAPP_SESSION.GetCurrentClientLanguageType());
//	}
	return map_AvailableLocationNames;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取产品票价

@param		(o)long& price 票价

@retval     bool	true:成功	false:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CProductIssueSvc::CalculatePrice()
{
	// 如果是储值票，无需通过TPU计算票价
	if (m_pDataModel->productInfo.productCategory == PURSE) {
		m_pDataModel->paymentInfo.lPriceAmount = static_cast<long>(m_pDataModel->productInfo.lBalanceAmount);
		return 0;
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
long CProductIssueSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	CACCCard accCard;
	if(!accCard.GetCardInfo(pReadCardResponse->CardInfo)) {
		OnError(_T(""));
		return -1;
	}

	m_pDataModel->mediumInfo.cardLogicalNumber = ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8);
	m_pDataModel->mediumInfo.cardPHType = static_cast<CardType_t>(pReadCardResponse->CardPHType);  // 查阅参考文档，这里返回的BYTE是CardType_t不是CARD_MEDIUM_TYPE
	m_pDataModel->mediumInfo.mediumType = theTICKET_HELPER.GetMediumType(m_pDataModel->mediumInfo.cardPHType);
	m_pDataModel->mediumInfo.passengerType = accCard.applicationData.byPassengerType == 0||accCard.applicationData.byPassengerType == 0xF  ? PASSENGER_ADULT : accCard.applicationData.byPassengerType;
	m_pDataModel->SalableProductCount = pReadCardResponse->SalableProductCount;
	m_pDataModel->SalableProductType = pReadCardResponse->SalableProductType;

	m_vecReadCard = accCard.v_ProductInfo;

	DoDialogFlow(WM_WAIT_TO_INIT);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      翻页函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::DlgNextPage(){
	if ( GetCurDialogGroupID() == DIALOG_GROUP_INIT )				// 产品选择界面
	{
		m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_LOCATION);	// 流向区段选择和支付方式选择界面
	}
	else if ( GetCurDialogGroupID() == DIALOG_GROUP_CHOOSE_LOCATION)// 区段选择和支付方式选择界面
	{
	}
	

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TPU充值处理(储值/计次/定期) 

@param      none

@retval  	true:处理成功  false:失败信息

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CProductIssueSvc::DoCardIssueProduct()
{
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		theAudio_HELPER.PlayAudio(AUDIO_ID__WAITINGTRANSACTION);

		//?? 目前TVMRWHelper.h里没有有关添加产品的接口函数。
		//?? RWDef.h里也没有添加产品相关的struct
		//?? 这里暂时用售卡接口
		RW_ISSUE_REQUEST issueInput;
		RW_BUSSINESS_RESPONSE response;
		issueInput.SerialNo = m_pDataModel->dwUDSN; // 交易流水号
		issueInput.TranDateTime = m_pDataModel->transTime; // 交易时间
		issueInput.CardType = static_cast<BYTE>(m_pDataModel->mediumInfo.cardPHType);
		issueInput.ProductyCategory = static_cast<BYTE>(m_pDataModel->productInfo.productCategory);
		issueInput.ProductType = static_cast<WORD>(m_pDataModel->productInfo.productType);
		issueInput.ProductSubType = static_cast<WORD>(m_pDataModel->productInfo.productSubType);
		issueInput.SaleMode = 0x02; // 售票方式，应该无关，选择0x02：表示按照起止站来售票
		issueInput.PassengerType = static_cast<BYTE>(m_pDataModel->mediumInfo.passengerType);
		issueInput.SaleAmount = static_cast<DWORD>(m_pDataModel->paymentInfo.lPriceAmount); // 计算所得票价，SVT金额或者TPU所返
		issueInput.BegainStation = static_cast<DWORD>(m_pDataModel->productInfo.beginLocation);
		issueInput.EndStation = static_cast<DWORD>(m_pDataModel->productInfo.endLocation);
		issueInput.MultiRideTicketCount = static_cast<WORD>(m_pDataModel->productInfo.lBalanceRides);
		issueInput.ValidityPeriod = static_cast<WORD>(m_pDataModel->productInfo.lValidatePeriod);
		issueInput.PaymentMethod = m_pDataModel->paymentInfo.paymentMethod;
		if (m_pDataModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
			memcpy(issueInput.BankCardNum, m_pDataModel->paymentInfo.BankCardNum, 10*sizeof(BYTE));	// 银行卡号
		}
		//if (m_pDataModel->paymentInfo.paymentMethod == PAYMENT_METHOD_EFT) {
		//	memcpy(issueInput.TicketNum, m_pDataModel->paymentInfo.EPCardNum, 8*sizeof(BYTE));	// 支付卡号
		//}

		// 第二次刷卡
		int nReadCardOutTime = theAFC_TVM_OPERATION.GetCancelDealTime();
		 if (1 == m_pDataModel->operationInfo.readCardCount)
		{
			RECHARGERW_HELPER->ReadCard((SERVICE_ID)GetServiceID());												// 调用读卡命令
			INT_PTR ret = CMessagePictureBox::Show(
				theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),					//	PassengerForeSvc里OnReadCard对读卡已处理，比较两次刷卡是否一致的逻辑需实现
				_tl(TXT_GUIDE_ADD_PRODUCT_TAP_CARD_AGAIN),// 請再次刷卡，確保在產品添加過程中不要將卡移開。
				TVM_TIP_SWIPE_CARD,
				CMessagePictureBox::BTN_NONE,DT_LEFT,
				nReadCardOutTime,CMessagePictureBox::BTN_NONE);

			if(IDCANCEL == ret || IDABORT == ret){
				// 停止读卡
				RECHARGERW_HELPER->StopReadCard();  
				m_pDataModel->operationInfo.finishType = CPassengerForeSvc::RUNNING_OUT_OF_TIME;
				//__super::OnCancel();
				return false;
			}		
		}
		else
		{
			return false;
		}

		 int nReadCardTime = theTVM_INFO.GetRWTimeout(TVM_RECHARGE_RW);
		 
		 while(true)
		 {
			 if ( m_IsTheSameCard)
			 {
				 break;
			 }
			 else
			 {
				 //3次读卡确认
				 RECHARGERW_HELPER->ReadCard((SERVICE_ID)GetServiceID());
				 // 2.2再次刷卡
				 INT_PTR ret = CMessagePictureBox::Show(
					 theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),
					 _tl(TXT_THE_CARD_YOU_SWIPE_IS_NOT_COMPARE),
					 TVM_IMAGE_BOX_PICTURE,
					CMessagePictureBox::BTN_NONE,DT_LEFT,
					 nReadCardTime,CMessagePictureBox::BTN_NONE);

				 if(IDCANCEL == ret || IDABORT == ret){
					 // 停止读卡
					 RECHARGERW_HELPER->StopReadCard();  
					 m_pDataModel->operationInfo.finishType = CPassengerForeSvc::RUNNING_OUT_OF_TIME;
					 return false;
				 }
				 continue;
			 }
		 }

		 long resultCode = RECHARGERW_HELPER->Issue(issueInput,response);

		 if(resultCode == RW_RSP_OK) {
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
			 m_pDataModel->dwUDSN = theCOUNT_INFO.GetUDSN();// 取值并加一
			 return true;
		 }
		 else {
			 //?? 请重新刷卡相关信息
			 m_pDataModel->operationInfo.finishType = TRANSACTION_STATE_FAILED;
			 return false;
		 }
	}
	catch (CSysException& e) {
		OnError(e);
		//throw;
	}
	catch (...) {
		OnError(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		//throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      保存添加产品SC数据

@param      none

@retval  	none

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::SaveSCData()
{
	//?? 审计数据US_SETTLEMENT_DATA中，尚缺“销售Product总数量”和“销售Product总金额”接口。
	//theSCAudit_MGR.addCardProductIssueNum(1);
	//theSCAudit_MGR.addCardProductIssueNum(m_pCommonModel->paymentInfo.lDueAmount);

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
void CProductIssueSvc::WriteDBData(Data::TransactionRecord &dbInfo) {

	// 参照数字编码参阅Sec1.2.2.1.2-4, MACAU-DGB20-0010 AFC Internal Interface(ccs-sc-sle)-Appendix C-Transaction-v00.00.00.03.docx
	dbInfo.iTransactionDetail.iUdType = 3; 
	switch (m_pDataModel->productInfo.productCategory) {
	case PURSE:
		dbInfo.iTransactionDetail.iUdSubtype = 1;
		break;
	case MULTIRIDE:
		dbInfo.iTransactionDetail.iUdSubtype = 3;
		break;
	case PERIOD:
		dbInfo.iTransactionDetail.iUdSubtype = 2;
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
//bool CProductIssueSvc::POSPayment()
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
//bool CProductIssueSvc::POSCancle()
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
@brief      供OnSuccessFinish使用的一个函数封装

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::FlowToFinishDlg()
{
	DoDialogFlow(WM_LOCATION_PAYMENT_TO_FINISH);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      供OnCancel和各种超限使用的一个函数封装

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::FlowToErrorDlg()
{
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_INIT){
		//theSERVICE_MGR.SetState(FINISH);
		DoDialogFlow(WM_INIT_TO_ERROR);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_CHOOSE_LOCATION){
		//theSERVICE_MGR.SetState(FINISH);
		DoDialogFlow(WM_LOCATION_PAYMENT_TO_ERROR);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_CHOOSE_PAYMENT){
		//theSERVICE_MGR.SetState(FINISH);
		DoDialogFlow(WM_PAYMENT_TO_ERROR);
	}
	else{
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		pMainSvc->StartDefaultService();
		//?? 这里要删除delete ProductIssue吗？
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印销售凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::PrinterPrintReceipt()
{
	try {
		// 加载模板
		CString templateName= _T("TVM_ADD_PRODUCT.template");
		CStringArray printTexts;
		CStringArray templateTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

		// 修改模板
		int index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			if (m_pDataModel->productInfo.productCategory == PURSE) {
				if (line.Find(_T("PST"))!=-1 || line.Find(_T("MST"))!=-1) {
					templateTexts.RemoveAt(index,3);
					continue;
				}
			}
			else if (m_pDataModel->productInfo.productCategory == PERIOD) {
				if (line.Find(_T("SVT"))!=-1 || line.Find(_T("MST"))!=-1) {
					templateTexts.RemoveAt(index,3);
					continue;
				}
			}
			else if (m_pDataModel->productInfo.productCategory == MULTIRIDE) {
				if (line.Find(_T("SVT"))!=-1 || line.Find(_T("PST"))!=-1) {
					templateTexts.RemoveAt(index,3);
					continue;
				}
			}

			++index;
		}

		// 填充数据
		index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			CString strSerialNo = _T("");
			strSerialNo.Format(_T("%010d"),m_pDataModel->dwUDSN);
			CString strCardNum = m_pDataModel->mediumInfo.cardLogicalNumber;
			line.Replace(_T("{流水號}"),strSerialNo);
			line.Replace(_T("{車票編號}"),strCardNum);
			line.Replace(_T("{車票類型}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_CHINESE));
			line.Replace(_T("{Ticket Type}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_ENGLISH));
			line.Replace(_T("{Tipos de bilhetes}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType),LANGUAGE_ID_PORTUGUESE));
			line.Replace(_T("{有效期}"),m_pDataModel->productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d")));
			if (m_pDataModel->productInfo.productCategory == PURSE) {
				line.Replace(_T("[SVT]"),_T(""));
				line.Replace(_T("{票內餘額}"),ComFormatAmount(m_pDataModel->productInfo.lBalanceAmount)+_tl(MONEY_UNIT));
			}
			else if (m_pDataModel->productInfo.productCategory == MULTIRIDE) {
				line.Replace(_T("[MST]"),_T(""));
				line.Replace(_T("{票內餘次}"),ComFormatCount(m_pDataModel->productInfo.lBalanceRides)+_tl(TIMES_UNIT));
			}
			//lProductSaleAmount
			line.Replace(_T("{産品單價}"),ComFormatAmount(m_pDataModel->productInfo.lProductSaleAmount));
			line.Replace(_T("{稅費}"),ComFormatAmount(m_pDataModel->paymentInfo.lTaxAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{應收金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lDueAmount)+_tl(MONEY_UNIT));
			/*line.Replace(_T("{支付方式}"),theACC_BUSINESS.GetPayTypeName(m_pDataModel->paymentInfo.paymentMethod,LANGUAGE_ID_CHINESE));		
			line.Replace(_T("{Payment}"),theACC_BUSINESS.GetPayTypeName(m_pDataModel->paymentInfo.paymentMethod,LANGUAGE_ID_ENGLISH));
			line.Replace(_T("{Pagamento}"),theACC_BUSINESS.GetPayTypeName(m_pDataModel->paymentInfo.paymentMethod,LANGUAGE_ID_PORTUGUESE));*/		
			line.Replace(_T("{支付金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lPaidAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{找零金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lChangeAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{交易時間}"),m_pDataModel->transTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d")));
			line.Replace(_T("{車站名稱}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			line.Replace(_T("{Nome_da_estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			line.Replace(_T("{設備編號}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());
			
			// 打印时字符串的格式化处理（模板读取数据）
			CStringArray printLines;
			ComPrintLineFormatForTemplate(line,printLines);
			printTexts.Append(printLines);
			++index;
		}

		// 打印帐票
		//PRINTER_HELPER->PrinterPrint(&printTexts,true);

		//mxw
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
void CProductIssueSvc::PrinterPrintErrorReceipt()
{
	try {
		// 加载模板
		CString templateName= _T("TVM_ADD_PRODUCT_FAILED.template");
		CStringArray printTexts;
		CStringArray templateTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateName,templateTexts);

		// 修改模板
		int index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			if (m_pDataModel->productInfo.productCategory == PURSE) {
				if (line.Find(_T("PST"))!=-1 || line.Find(_T("MST"))!=-1) {
					templateTexts.RemoveAt(index,3);
					continue;
				}
			}
			else if (m_pDataModel->productInfo.productCategory == PERIOD) {
				if (line.Find(_T("SVT"))!=-1 || line.Find(_T("MST"))!=-1) {
					templateTexts.RemoveAt(index,3);
					continue;
				}
			}
			else if (m_pDataModel->productInfo.productCategory == MULTIRIDE) {
				if (line.Find(_T("SVT"))!=-1 || line.Find(_T("PST"))!=-1) {
					templateTexts.RemoveAt(index,3);
					continue;
				}
			}
			else if (m_pDataModel->paymentInfo.paymentMethod != PAYMENT_METHOD_CASH) {
				if (line.Find(_T("[CASH]"))!=-1){
					templateTexts.RemoveAt(index);
					continue;
				}	
			}

			++index;
		}

		// 填充数据
		index = 0;
		while (index < templateTexts.GetCount()) {
			CString& line = templateTexts.ElementAt(index);
			CString strSerialNo = _T("");
			strSerialNo.Format(_T("%010d"),m_pDataModel->dwUDSN);
			CString strCardNum = m_pDataModel->mediumInfo.cardLogicalNumber;
			line.Replace(_T("{流水號}"),strSerialNo);
			line.Replace(_T("{車票編號}"),strCardNum);
			line.Replace(_T("{車票類型}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_CHINESE));
			line.Replace(_T("{Ticket Type}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType), LANGUAGE_ID_ENGLISH));
			line.Replace(_T("{Tipos de bilhetes}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_pDataModel->productInfo.productType,m_pDataModel->productInfo.productSubType),LANGUAGE_ID_PORTUGUESE));
			line.Replace(_T("{應收金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lDueAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{應找金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lChangeAmount)+_tl(MONEY_UNIT));
			/*line.Replace(_T("{支付方式}"),theACC_BUSINESS.GetPayTypeName(m_pDataModel->paymentInfo.paymentMethod,LANGUAGE_ID_CHINESE));		
			line.Replace(_T("{Payment}"),theACC_BUSINESS.GetPayTypeName(m_pDataModel->paymentInfo.paymentMethod,LANGUAGE_ID_ENGLISH));
			line.Replace(_T("{Pagamento}"),theACC_BUSINESS.GetPayTypeName(m_pDataModel->paymentInfo.paymentMethod,LANGUAGE_ID_PORTUGUESE));*/
			if (m_pDataModel->paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){	
				line.Replace(_T("[CASH]"),_T(""));
				line.Replace(_T("{投入紙幣}"),ComFormatAmount(m_pDataModel->paymentInfo.lBHReceiveTotalAmount));
				line.Replace(_T("{投入硬幣}"),ComFormatAmount(m_pDataModel->paymentInfo.lCHReceiveTotalAmount));
				line.Replace(_T("{未找零金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lChangeAmount));
			}
			line.Replace(_T("{退款金額}"),ComFormatAmount(m_pDataModel->paymentInfo.lPaidAmount)+_tl(MONEY_UNIT));
			line.Replace(_T("{交易時間}"),m_pDataModel->transTime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d")));
			line.Replace(_T("{車站名稱}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			line.Replace(_T("{Station_Name_pt}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			line.Replace(_T("{設備編號}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

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
@brief      判断是否硬币纸币都达到最大数量，供ForeService里的函数OnCoinInserted和OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CProductIssueSvc::IsBothCoinBanknoteMaxCount() {
	CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);
	if (GetCoinAcceptCount() >= billboxParam.acceptCoinMaxCnt && GetBankNoteAcceptCount() >= billboxParam.acceptChrgrMaxBillCnt) {
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币达到最大数量，供ForeService里的函数OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CProductIssueSvc::IsBanknoteMaxCount() {
	/*CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);
	if (GetBankNoteAcceptCount() > billboxParam.acceptChrgrMaxBillCnt) {
	return true;
	}
	else {
	return false;
	}*/
	// 判断当前钱箱数量
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	// Need edit zhengxianle
	//long lCurCount = banknoteInfo.ulCount_10 + banknoteInfo.ulCount_100 + banknoteInfo.ulCount_1 + banknoteInfo.ulCount_20 + banknoteInfo.ulCount_50 + banknoteInfo.ulCount_5;

	//// 先判断当前投入是否大于剩余可投入数量？
	//if(m_pDataModel->paymentInfo.lBHReceiveTotalCount >= (theAFC_TVM_OPERATION.GetBanknotesFullConut() - lCurCount)){
	//	return true;
	//}
	// 再判断当前投入数量是否超过充值最大数量限制？
	if (m_pDataModel->paymentInfo.lBHReceiveTotalCount >= theAFC_TVM_OPERATION.GetAcceptBanknotesMaxPieceOfRecharge()) {
		return true;
	}
	else {
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币都达到最大金额，供ForeService里的函数OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CProductIssueSvc::IsBanknoteMaxAmount() {
	CAfcTVMOperationParam::BILLBOX_PARAM billboxParam;
	theAFC_TVM_OPERATION.GetBillboxParam(billboxParam);
	if (static_cast<unsigned long>(issuePayDetailInfo.BHReceiveMoney) > static_cast<unsigned long>(billboxParam.acceptChrgrMaxBillAmt)*100) { // acceptChrgrMaxBillAmt返回“元”，但是BHReceiveMoney的参数是“分”。
		return true;
	}
	else {
		return false;
	}
}


bool CProductIssueSvc::IsCoinMaxCount(){
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
	//if(ulReciveOneCount > theTVM_INFO.GetCHTemporary1Max() || ulReciveOtherCount > theTVM_INFO.GetCHTemporary2Max()){
	//	return true;
	//}
	//// 8.如果上述不超过最大限制，则判断投入是否超过参数最大限制
	//if (m_pDataModel->paymentInfo.lCHReceiveTotalCount > theAFC_TVM_OPERATION.GetAcceptCoinMaxPieceOfRecharge()){
	//	return true;
	//}
	//else{
	//	return false;
	//}
	return true;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取可接收的纸币面额

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BRH_FACE_ACEPTED_TABLE CProductIssueSvc::GetAcceptBanknotesFace(tBncSetInhibitList* table){
	vector<WORD> vecFace;
	BRH_FACE_ACEPTED_TABLE bhAcceptTable;
	theAFC_TVM_OPERATION.GetAcceptBanknoteFaceOfIssueCard(vecFace);
	/*for (vector<WORD>::iterator it = vecFace.begin();it != vecFace.end();it++){
	if (10 == *it){
	bhAcceptTable.bIsBanknote10MOP = true;
	}
	else if(20 == *it){
	bhAcceptTable.bIsBanknote20MOP = true;
	}
	else if(50 == *it){
	bhAcceptTable.bIsBanknote50MOP = true;
	}
	else if(100 == *it){
	bhAcceptTable.bIsBanknote100MOP = true;
	}
	else if(500 == *it){
	bhAcceptTable.bIsBanknote500MOP = true;
	}
	else if(1000 == *it){
	bhAcceptTable.bIsBanknote1000MOP = true;
	}
	}*/
	return bhAcceptTable;
}

bool CProductIssueSvc::IsCoinChageMaxCount(){
	// 找零总数量
	DWORD dwChangeCount = m_pDataModel->paymentInfo.dwChangeCoinHalfYuanCount + m_pDataModel->paymentInfo.dwChangeCoinOneYuanCount;//m_pDataModel->paymentInfo.dwChangeStockCount + m_pDataModel->paymentInfo.dwChangeHopper1Count + m_pDataModel->paymentInfo.dwChangeHopper2Count;
	if (dwChangeCount > theAFC_TVM_OPERATION.GetChageCoinMaxPieceOfIssue()){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取可接收的硬币的面额

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COIN_ACCEPTABLE CProductIssueSvc::GetAcceptCoinFaceTable(){
	// 1.判断回收箱是否在位？
	COIN_ACCEPTABLE acceptTable;
	//CH_COMMON_RSP rsp;
	//try{
	//	COIN_HELPER->CH_GetStatus(&rsp);
	//}
	//catch(CSysException& e){
	//	theEXCEPTION_MGR.ProcessException(e);
	//}	
	//// 2.将可接收面额转换为Table表
	//vector<WORD> vecFace;
	//theAFC_TVM_OPERATION.GetAcceptCoinFaceOfIssue(vecFace);
	//for (vector<WORD>::iterator it = vecFace.begin();it != vecFace.end(); it++)
	//{
	//	if(50 == *it){
	//		acceptTable.CH_COIN_ALLOW_05 = true;
	//	}
	//	else if(100 == *it){
	//		acceptTable.CH_COIN_ALLOW_1 = true;
	//	}
	//	else if(200 == *it){
	//		acceptTable.CH_COIN_ALLOW_2 = true;
	//	}
	//	else if(500 == *it){
	//		acceptTable.CH_COIN_ALLOW_5 = true;
	//	}
	//}
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_A_info = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_B_info = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//CTCoinCountInfo::COIN_BOX_INFO stock = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//// 3.回收箱2不在位，或者回收箱2已满，不可以接收非一元的
	//if(!rsp.sensor_info.CH_BOX2_SET_STATUS || (box_B_info.GetCurCount() >= theAFC_TVM_OPERATION.GetRecycleBoxAFullCount())){
	//	acceptTable.CH_COIN_ALLOW_05 = false;
	//	acceptTable.CH_COIN_ALLOW_2	 = false;
	//	acceptTable.CH_COIN_ALLOW_5  = false;
	//}
	//// 4.回收箱1不在位，判断当前stock是否已满？或者两者都满？
	//if (!rsp.sensor_info.CH_BOX1_SET_STATUS && stock.ulCurCount >= theTVM_INFO.GetCHStock1Max() ||
	//	(stock.ulCurCount >= theTVM_INFO.GetCHStock1Max()) && (box_B_info.GetCurCount() >= theAFC_TVM_OPERATION.GetRecycleBoxBFullCount())){
	//		acceptTable.CH_COIN_ALLOW_1 = false;
	//}
	return acceptTable;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      再次刷卡确认操作时的判断

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse 读卡反馈数据

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CProductIssueSvc::ConfirmCard(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	CACCCard accCard;
	if(!accCard.GetCardInfo(pReadCardResponse->CardInfo)) {
		return false;
	}	

	if (m_pDataModel->mediumInfo.cardLogicalNumber != ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8))
	{
		//// 执行两次票卡不一致情况的处理
		//__super::OnCancel();
		//
		//return false;	
		m_IsTheSameCard = FALSE;
		return false;
	}else
		m_IsTheSameCard = TRUE;

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      界面返回完成

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CProductIssueSvc::ReturnFinishDlg(){
	m_pDataModel->operationInfo.isFinishedJob = true;
	OnReturnFinish();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断当前票卡中是不是存在该PST产品

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CProductIssueSvc::isSubProductExist(WORD productType,WORD subProductType)
{
	BOOL  bExist = FALSE;
	for ( vector<CACCCard::ACCCARD_PRODUCT_INFO>::iterator iter = m_vecReadCard.begin();iter!=m_vecReadCard.end();iter++)
	{
		if (iter->productType == PST  && iter->productType == productType && iter->productSubType == subProductType)
		{
			bExist = TRUE;
		}
	}
	return bExist;
}