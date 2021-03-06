#include "stdafx.h"
#include "IssueSvc.h"
#include "WaitingOfficeDlg.h"
#include "tickethelper.h"
#include "guideinfo.h"
#include <basetsd.h>
#include "TemplateHelper.h"
#include "tvmsetting.h"
#include "IssueWithNetDlg.h"
#include "IssueWithSvtDlg.h"
#include "IssueWithCashDlg.h"
#include "IssueAreaMapDlg.h"
#include "AudioHelper.h"
#include "PassengerResultDlg.h"
#include "RechargeRWMonitorSvc.h"


// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_MAP_TO_CASH		= _T("WM_MAP_TO_CASH");
static const TCHAR* const WM_MAP_TO_SVT			= _T("WM_MAP_TO_SVT");
static const TCHAR* const WM_CASH_TO_NET		= _T("WM_CASH_TO_NET");
static const TCHAR* const WM_CASH_TO_FINISH		= _T("WM_CASH_TO_FINISH");
static const TCHAR* const WM_CASH_TO_ERROR		= _T("WM_CASH_TO_ERROR");
static const TCHAR* const WM_SVT_TO_FINISH		= _T("WM_SVT_TO_FINISH");
static const TCHAR* const WM_SVT_TO_ERROR		= _T("WM_SVT_TO_ERROR");
static const TCHAR* const WM_NET_TO_FINISH		= _T("WM_NET_TO_FINISH");
static const TCHAR* const WM_NET_TO_ERROR		= _T("WM_NET_TO_ERROR");

BEGIN_MESSAGE_MAP(CIssueSvc, CPassengerForeSvc)
	ON_SERVICE_MESSAGE(SM_ISSUE_OF_BLIND_BEGIN_OPERATION,OnBlindBeginOperation)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::CIssueSvc():CPassengerForeSvc(ISSUE_SVC)
{
	LOG("售票构造函数开始");
	m_pDialogFlow->AddDialog(IDD_10101_ISSUE_DLG,  new CIssueAreaMapDlg(this));
	m_pDialogFlow->AddDialog(IDD_00101_ISSUE_WITH_CASH_DLG,  new CIssueWithCashDlg(this));
	m_pDialogFlow->AddDialog(IDD_00102_ISSUE_WITH_SVT_DLG,  new CIssueWithSvtDlg(this));
	m_pDialogFlow->AddDialog(IDD_00103_ISSUE_WITH_NET_DLG,  new CIssueWithNetDlg(this));
	//m_pDialogFlow->AddDialog(IDD_00103_ISSUE_RESULT_DLG,  new CPassengerResultDlg(this));

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_AREA_MAP);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_AREA_MAP, IDD_10101_ISSUE_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_CASH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_CASH, IDD_00101_ISSUE_WITH_CASH_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_SVT);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_SVT, IDD_00102_ISSUE_WITH_SVT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_NET);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_NET, IDD_00103_ISSUE_WITH_NET_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_CASH_FINISH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_CASH_FINISH, /*IDD_00103_ISSUE_RESULT_DLG*/IDD_00101_ISSUE_WITH_CASH_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_CASH_ERROR);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_CASH_ERROR, /*IDD_00103_ISSUE_RESULT_DLG*/IDD_00101_ISSUE_WITH_CASH_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_SVT_FINISH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_SVT_FINISH, IDD_00102_ISSUE_WITH_SVT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_SVT_ERROR);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_SVT_ERROR, IDD_00102_ISSUE_WITH_SVT_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_NET_FINISH);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_NET_FINISH, IDD_00103_ISSUE_WITH_NET_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ISSUE_WITH_NET_ERROR);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ISSUE_WITH_NET_ERROR, IDD_00103_ISSUE_WITH_NET_DLG);

	m_pDialogFlow->AddFlow(DIALOG_GROUP_AREA_MAP,WM_MAP_TO_CASH,DIALOG_GROUP_ISSUE_WITH_CASH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_AREA_MAP,WM_MAP_TO_SVT,DIALOG_GROUP_ISSUE_WITH_SVT);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_CASH,WM_CASH_TO_NET,DIALOG_GROUP_ISSUE_WITH_NET);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_CASH,WM_CASH_TO_FINISH,DIALOG_GROUP_ISSUE_WITH_CASH_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_CASH,WM_CASH_TO_ERROR,DIALOG_GROUP_ISSUE_WITH_CASH_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_NET,WM_NET_TO_FINISH,DIALOG_GROUP_ISSUE_WITH_NET_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_NET,WM_NET_TO_ERROR,DIALOG_GROUP_ISSUE_WITH_NET_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_SVT,WM_SVT_TO_FINISH,DIALOG_GROUP_ISSUE_WITH_SVT_FINISH);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ISSUE_WITH_SVT,WM_SVT_TO_ERROR,DIALOG_GROUP_ISSUE_WITH_SVT_ERROR);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_AREA_MAP);

	//m_pCommonModel = &m_DataModel;
	//InitIssueProduct();
	m_vecTicketAmount.clear();
	m_vecTicketCount.clear();
	// 语音播放
	m_pIssueAudioHelper = new CIssueAudioHelper(*this);

	// 设置是否需要点击确认按钮收钱发票？
	m_bNeedClickOkButton = false;
	// 设置服务新启动标识，用于判断按钮组是否需要重新创建
	m_bServiceNewStart = true;


	StartWebThread();

	LOG("售票构造结束。");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::~CIssueSvc()
{
	m_pCommonModel = NULL;
	delete m_DataModel.pCardInfo;
	m_DataModel.pCardInfo = NULL;
	if (m_pIssueAudioHelper != NULL){
		delete m_pIssueAudioHelper;
		m_pIssueAudioHelper = NULL;
	}
	//if(theAPP_SESSION.IsBlindManOperation()){
	//	// 停止监控密码键盘
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(STATUS_BLIND_BUY_CARD);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动时响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnStart()
{
	//     m_pDialogFlow->AddDialog(IDD_10101_ISSUE_DLG,  new CIssueAreaMapDlg(this));
	//     m_pDialogFlow->AddDialog(IDD_00101_ISSUE_INIT_DLG,  new CIssueACCIniDlg(this));
	// 	m_pDialogFlow->AddDialog(IDD_00102_ISSUE_CONFIRM_DLG, new CIssueSectionDlg(this));
	// 	m_pDialogFlow->AddDialog(IDD_00103_ISSUE_RESULT_DLG,new CPassengerResultDlg(this));
	// 	
	// 	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_AREA_MAP);
	// 	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_AREA_MAP, IDD_10101_ISSUE_DLG);
	// 
	// 	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_INIT);
	// 	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_INIT, IDD_00101_ISSUE_INIT_DLG);
	// 
	// 	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SECTION);
	// 	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SECTION,IDD_00102_ISSUE_CONFIRM_DLG);
	// 
	// 	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_FINISH);
	// 	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_FINISH,IDD_00101_ISSUE_INIT_DLG);
	// 
	// 	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR);
	// 	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR,IDD_00103_ISSUE_RESULT_DLG);
	// 
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_AREA_MAP,WM_MAP_TO_INIT,DIALOG_GROUP_INIT);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_AREA_MAP,WM_MAP_TO_SECTION,DIALOG_GROUP_SECTION);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_AREA_MAP,WM_MAP_TO_ERROR,DIALOG_GROUP_ERROR);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_FINISH,DIALOG_GROUP_FINISH);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_TO_ERROR,DIALOG_GROUP_ERROR);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_SECTION,WM_SECTION_TO_ERROR,DIALOG_GROUP_ERROR);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_SECTION,WM_SECTION_TO_INI,DIALOG_GROUP_INIT);
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_SECTION,WM_SECTION_BACK_TO_MAP,DIALOG_GROUP_AREA_MAP);	// 选择子产品界面返回主界面
	// 	m_pDialogFlow->AddFlow(DIALOG_GROUP_INIT,WM_INIT_BACK_TO_MAP,DIALOG_GROUP_AREA_MAP);			// 支付界面返回主界面
	// 
	// 	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_AREA_MAP);
	LOG("-------------------------------------------售票业务---------------------------------------------------");
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_AREA_MAP);
	m_bServiceNewStart = true;
	__super::OnStart();

	// 如果充值读写器监控开启
	//if(theTVM_INFO.IsRWMonitorOpened(RW_TYPE::TVM_RECHARGE_RW)){
	//	// 启动充值读写器监控
	//	theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();
	//}
	m_pCommonModel = &m_DataModel;
	// 清空填充的数据
	m_DataModel = _tagModel();
	m_DataModel.transTime = ComGetCurTime();// 交易时间
	m_DataModel.dwUDSN = theCOUNT_INFO.GetUDSNForDisplay();
	m_DataModel.dwTransactionID = theCOUNT_INFO.GetTransactionNumForDsp();
	m_DataModel.mediumInfo = MEDIUM_INFO();
	m_DataModel.mediumCardInfo = MEDIUM_INFO();
	m_DataModel.productInfo = PRODUCT_INFO();
	m_DataModel.paymentInfo = PAYMENT_INFO();
	m_DataModel.operationInfo = OPERATION_INFO();
	m_DataModel.isInsertToDatabase = false;
	m_DataModel.lDeviceStatus = 0;

	m_bIsPinPadStop = false;
	m_nReadedCardCount = 0;
	m_operation_status = BLIND_OPERATION_STATUS::STATUS_BLIND_NORMAL;
	m_bStartAcceptCash = false;
	m_ePayType = PAY_WITH_NORMAL;
	m_bReaderBad = false;

	// 显示支付状态信息
	theTVM_STATUS_MGR.TVMPaymentChanged.Invoke();
	NOTIFY_PROCESSING_WINDOW_CLOSE();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得Service的数据模型

@param      无

@retval     CIssueSvc::Model& 业务数据 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CIssueSvc::Model& CIssueSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置卖票的TPU参数信息

@param      无

@retval     RW_ISSUE_REQUEST 售票信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
RW_ISSUE_REQUEST CIssueSvc::GetIssueParameter()
{
	RW_ISSUE_REQUEST rwIssueRequest;
	memset(&rwIssueRequest,0x00,sizeof(RW_ISSUE_REQUEST));
	rwIssueRequest.SerialNo =  theCOUNT_INFO.GetUDSNForDisplay();
	rwIssueRequest.PaymentMethod = m_DataModel.paymentInfo.paymentMethod;
	//if (PAYMENT_METHOD_BANK_CARD == m_DataModel.issueInfo.paymentMethod) {
	//	memset(issuePayDetailInfo.BankCardNum, 0, 10);					// 银行卡号
	//}
	//if (PAYMENT_METHOD_EFT == m_DataModel.issueInfo.paymentMethod) {
	//	memset(issuePayDetailInfo.EPCardNum, 0, 8);					// 储值卡号
	//}
	rwIssueRequest.TranDateTime = ComGetCurTime();
	rwIssueRequest.CardType = m_DataModel.mediumInfo.cardPHType;
	rwIssueRequest.PassengerType = m_DataModel.mediumInfo.passengerType = 0;
	rwIssueRequest.ProductyCategory = m_DataModel.productInfo.productCategory;
	rwIssueRequest.ProductType = m_DataModel.productInfo.productType;
	rwIssueRequest.ProductSubType = m_DataModel.productInfo.productSubType;
	memcpy(rwIssueRequest.BankCardNum ,issuePayDetailInfo.BankCardNum,10);
	memcpy(rwIssueRequest.TicketNum,issuePayDetailInfo.EPCardNum,8);
	rwIssueRequest.BegainStation = m_DataModel.productInfo.beginLocation;
	rwIssueRequest.EndStation = m_DataModel.productInfo.endLocation;
	rwIssueRequest.MultiRideTicketCount = m_DataModel.productInfo.lBalanceRides;
	rwIssueRequest.SaleAmount = m_DataModel.productInfo.lProductSaleAmount;
	rwIssueRequest.SaleMode = AutoCalcSaleMode();
	rwIssueRequest.ValidityPeriod = m_DataModel.productInfo.lValidatePeriod;
	rwIssueRequest.PassengerType = 0;
	rwIssueRequest.PaymentMethod = m_DataModel.paymentInfo.paymentMethod;
	return rwIssueRequest;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      引发数据模型改变事件

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::NotifyDataModelChanged()
{
	//if(GetCurDialogGroupID() == CIssueSvc::DIALOG_GROUP_ERROR){
	//	return;
	//}

	// 重新计算支付信息
	AutoCalcPaymentInfo();

	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币接受金额

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnAcceptMoneyOK()
//{
//	theSERVICE_MGR.SetForeServiceBusy(false);
//	issuePayDetailInfo.hasFinishPaid = true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      支付完成处理(关闭纸币硬币口及退出银行卡等)

@param      无

@retval     无

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::AcceptMoneyAndCloseGate()
{
	try{
		if (m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
			// 压钞并关闭接收口
			StartAcceptCashMoney();
		}
		/*else if (m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_EFT){
		}*/
		NotifyDataModelChanged();
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币接受金额

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnReceiveMoneyOK()
//{
//	if ((issuePayDetailInfo.isCionCollected == true||issuePayDetailInfo.isBankNoteCollected == true)
//		&&issuePayDetailInfo.isWaitingEPCard == false
//		&&issuePayDetailInfo.isBankCardOpen == false){
//		StartIssueAndChange();			
//	}
//}
//////////////////////////////////////////////////////////////////////////
/*
@brief      是否完成支付

@param      无

@retval     bool 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::hasFinishPaid()
{
	if (m_DataModel.paymentInfo.lDueAmount>0){
		return issuePayDetailInfo.hasFinishPaid;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始找零

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//bool CIssueSvc::DoChange()
//{
//	bool res = true;
//	if (m_DataModel.paymentInfo.lChangeAmount>0){		
//		// 找零
//		issuePayDetailInfo.changeAmount = m_DataModel.paymentInfo.lChangeAmount;
//		//res = StartChange();
//	}
//	return res;
//}

//void CIssueSvc::OnSuccessFinish()
//{
//	m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_FINISH);
//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
//	theAudio_HELPER.PlayAudio(AUDIO_ID_THANKS);
//	theSERVICE_MGR.SetState(FINISH);
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读卡的回调函数

@param      UINT resultCode 读卡结果
@param      CRWReadCard* rwReadCardCommand 读卡命令

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnReadCard(UINT resultCode,CRWReadCard* rwReadCardCommand)
//{
//	try{
//		CMessageBox::End();
//		if(GetCurDialogGroupID() == DIALOG_GROUP_FINISH || GetCurDialogGroupID() == DIALOG_GROUP_ERROR)
//		{
//			return;
//		}
//		rwReadCardCommand->GetRWResponse(&m_DataModel.rwReadCardResponse);
//		// 黑名单票卡
//		if(resultCode == RW_RSP_ACC_CARD_BLACKLIST){
//			long blockCardResult = BlockCard(); 
//			CString tips = blockCardResult == RW_RSP_OK ? CString(_tl(GUIDE_IN_BLACK_CARD_ERROR)): RECHARGERW_HELPER->GetTpuErrorInfo(resultCode);
//			OnError(tips);
//		}
//		// 非黑名单票卡
//		else{		
//			// 读卡失败
//			if(resultCode!=RW_RSP_OK){
//				// 读卡错误
//				if(m_DataModel.rwReadCardResponse.DetailErrorCode != RW_ERROR_CARD_NO_CARD){
//					//OnError(RECHARGERW_HELPER->GetTpuErrorInfo(m_DataModel.rwReadCardResponse.DetailErrorCode));
//				}
//				// 读卡操作取消
//				else{
//					m_DataModel.operationInfo.finishType = PASSAGE_CANCEL;
//					OnError();
//				}
//			}
//			// 读卡成功
//			else{
//				// 解析票卡数据成功
//				if(ValidateCardInfo(m_DataModel.rwReadCardResponse)){				
//					//?? 扣钱	
//					//RECHARGERW_HELPER->
//				}
//			}
//		}		
//	}
//	catch (CSysException& e){
//		OnError(e);
//	}
//	catch(...){
//		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
//		OnError(e);
//	}
//}

/////////////////////////////////////////////////////////////////////////
/*
@brief      读到卡的处理    

@param      RW_READ_CARD_RESPONSE& rwReadCardResponse  读卡数据

@retval     bool true:卡数据正确 fales:卡数据错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::ValidateCardInfo(RW_READ_CARD_RESPONSE& rwReadCardResponse)
{
	// EP支付储值卡信息有效性检查
	if(issuePayDetailInfo.isWaitingEPCard){
		if( CARD_TYPE_ACC_M1 != rwReadCardResponse.CardPHType){
			OnError(_tl(GUIDE_IN_CARD_DATA_ERROR));
			return false;
		}
	}
	else{
		if( m_DataModel.mediumInfo.cardPHType != rwReadCardResponse.CardPHType){
			OnError(_tl(GUIDE_IN_CARD_DATA_ERROR));
			return false;
		}

		delete m_DataModel.pCardInfo;
		m_DataModel.pCardInfo = NULL;
		m_DataModel.pCardInfo = new CACCCard;
		m_DataModel.pCardInfo->GetCardInfo(rwReadCardResponse.CardInfo);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      出错处理

@param      （i）CString errorMsg  错误信息文言
@param      （i）bool bContinue    是否继续读卡标记

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnCancel()
//{
//	__super::OnCancel();
//
//	try{
//		m_DataModel.operationInfo.finishType = CPassengerForeSvc::PASSAGE_CANCEL;
//		// 画面流转
//		theSERVICE_MGR.SetState(FINISH);
//
//		if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_INIT&& issuePayDetailInfo.hasStartPaid){
//			// 退还现金
//			//StartReturnCash();		
//			//CloseAllDevice();
//			m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_ERROR);
//			theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);
//		}
//		else{
//			CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
//			pMainSvc->StartDefaultService();
//		}
//	}
//	catch (CSysException& e){
//		OnError(e);
//	}
//	catch(...){
//		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
//		OnError(e);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      出错处理

@param      （i）CString errorMsg  错误信息文言

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnError(CSysException e)
{
	try{
		if (m_DataModel.paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH){
			// 纸币及硬币模块异常在父类处理。
			if(e.GetModuleID() == CBRHHardwareException::MODULE_ID || e.GetModuleID() == CBRHException::MODULE_ID || 
				e.GetModuleID() == CCHException::MODULE_ID || e.GetModuleID() == CCHHardwareException::MODULE_ID ||
				e.GetModuleID() == CBHChangeHardwareException::MODULE_ID
				){
					__super::OnError(e);
					return;
			}
		}
		else if(m_DataModel.paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_WITH_WEIXIN || m_DataModel.paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_WITH_ZHIFUBAO){
			EndQueryOrderTimer();
			ConfirmOrder();
		}

		theSERVICE_MGR.SetForeServiceBusy(false);
		// 添加到异常队列
		theEXCEPTION_MGR.ProcessException(e);
		//m_DataModel.operationInfo.isFinishedJob = true;
		m_DataModel.lDeviceStatus &= ~DEVICE_CARD_BUSY;

		m_pCommonModel->operationInfo.finishType = TRANSACTION_STATE_FAILED;
		// 1.判断是否是出票失败（M1）？
		//if(e.GetModuleID() == CTHException::MODULE_ID || e.GetModuleID() == CTHHardwareException::MODULE_ID){
		//	// 加入异常故障
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		//	// 预留：提示详细错误信息。
		//	OnReturnFinish();
		//	return;
		//}
		// 2.是否有Token异常？
		if(e.GetModuleID() == CTOKENException::MODULE_ID || e.GetModuleID() == CTOKENHardwareException::MODULE_ID){
			m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
			OnReturnFinish();
			return;
		}
		// 3.判断是否是读写卡失败？
		if(e.GetModuleID() == CRWException::TVM_CARD_RW_MODULE_ID || e.GetModuleID() == CRWHardwareException::TVM_CARD_RW_MODULE_ID){
			m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
			OnReturnFinish();
			return;
		}
		// 4.其他硬件异常。(其他异常:判断是否票发完。)
		if(m_pCommonModel->productInfo.nProductPiece > m_pCommonModel->productInfo.nProductIssuedPiece){
			m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
			OnReturnFinish();
		}
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
/*
@brief      出错处理(非异常结束业务处理)

@param      （i）CString errorMsg  错误信息文言

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnError(CString errorMsg)
{
	try{
		/*CloseAllDevice();
		OnErrorFinish();*/
		__super::OnError(errorMsg);
	}
	catch (CSysException& e){
		OnErrorFinish();
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnErrorFinish();
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      出错处理

@param      （i）CString errorMsg  错误信息文言
@param      （i）bool bContinue    是否继续读卡标记

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnErrorFinish(){
// 1.一张票也没有发出去
//if(m_DataModel.productInfo.nProductIssuedPiece == 0){
//	m_DataModel.operationInfo.finishType = CPassengerForeSvc::NOT_ENOUGH_TICKETS_FOR_BOOKING;
//	m_DataModel.operationInfo.isSuccessIssueCard = true;
//	// 连续发票失败，退钱给乘客。
//	StartReturnCash();
//	return;
//}

//// 2.第一张票发出，剩余票发出失败,打印售票失败凭证
//if(m_DataModel.productInfo.nProductIssuedPiece >0 && m_DataModel.productInfo.nProductIssuedPiece < m_DataModel.productInfo.nProductPiece && 
//	(m_DataModel.operationInfo.coinChangeState == CPassengerForeSvc::COIN_CHANGE_STATE_SUCCESS ||
//	m_DataModel.operationInfo.coinChangeState == CPassengerForeSvc::COIN_CHANGE_STATE_FAIL)){
//	PrinterPrintErrorRecipt();
//	m_DataModel.operationInfo.finishType = CPassengerForeSvc::TRANSACTION_STATE_FAILED;
//	// 转到错误界面
//	m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_ERROR);
//}
//// 调用父类错误处理
//else if(m_DataModel.operationInfo.coinChangeState == CPassengerForeSvc::COIN_CHANGE_STATE_FAIL){
//	__super::OnErrorFinish();
//}


// 打印故障单
// 现金是收款则打故障单
// EP，银行卡是支付成功但出票失败打故障单
//if (issuePayDetailInfo.BHReceiveMoney+issuePayDetailInfo.BHReceiveMoney>0||issuePayDetailInfo.BankReceiveMoney>0||issuePayDetailInfo.EPReceiveMoney>0)	{
//	PrinterPrintErrorRecipt();
//}

//// 画面流转
////theSERVICE_MGR.SetState(FINISH);
//m_DataModel.operationInfo.finishType = CPassengerForeSvc::EXCEPTION_ERROR;
//if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_AREA_MAP){
//	m_pDialogFlow->ShowNextDialogGroup(WM_MAP_TO_ERROR);
//}
//else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_INIT){
//	m_pDialogFlow->ShowNextDialogGroup(WM_INIT_TO_ERROR);
//}
//else{
//	m_pDialogFlow->ShowNextDialogGroup(WM_SECTION_TO_ERROR);
//}
//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_FAILED);
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      锁卡命令

@param      无 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::BlockCard()
{
	UINT resultCode;
	RW_CMD_BLOCK rw_cmd_block;
	rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rw_cmd_block.TranDateTime = ComGetCurTime();
	RW_BUSSINESS_RESPONSE response;
	resultCode = RECHARGERW_HELPER->Block(rw_cmd_block,response);
	// 锁卡成功
	if(RW_RSP_OK == resultCode){
		// 更新交易流水号
		theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	
		// 更新UD数据
		if(response.UDlen != 0){
			RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
		}
		// 更新AR数据
		if(response.ARLen != 0){
			RECHARGERW_HELPER->SaveRWARData(response.ARLen,response.ARData);
		}
	}
	return resultCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     更新TH票箱状态，票箱计数 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::UpdateTHCount(SC_RSP_ISSUE_DATA& issueData)
//{
//	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxAInfo = theCOUNT_INFO.GetTicketboxAInfo();
//	CCTLOGCountInfo::TICKETBOX_INFO& ticketboxBInfo = theCOUNT_INFO.GetTicketboxBInfo();
//	WORD collectionCurCount = theCOUNT_INFO.GetCollectionCurCount();
//	ticketboxAInfo.ulCurCount -= issueData.wTicketCntFromMagA;
//	ticketboxBInfo.ulCurCount-=issueData.wTicketCntFromMagB;
//	collectionCurCount+=issueData.wTicketCntToColBox;
//	theCOUNT_INFO.SetTicketboxAInfo(ticketboxAInfo);
//	theCOUNT_INFO.SetTicketboxBInfo(ticketboxBInfo);
//	theCOUNT_INFO.SetCollectionCurCount(collectionCurCount);
//
//	CAfcBomOperationParam::MAGAZINE_PARAM magazineParam;
//	theAFC_BOM_OPERATION.GetMagazineParam(magazineParam);
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      保存业务数据

@param      RW_BUSSINESS_RESPONSE& rwBussinessResponse 售卡命令反馈数据

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SaveBusinessData(RW_BUSSINESS_RESPONSE& rwBussinessResponse)
{
	theCOUNT_INFO.UpdateUDSN(rwBussinessResponse.SerialAddnum);
	//m_DataModel.dwUDSN = theCOUNT_INFO.GetUDSNForDisplay();

	if(rwBussinessResponse.UDlen>0)
	{
		CARDRW_HELPER->SaveRWUDData(rwBussinessResponse.UDlen,rwBussinessResponse.UDData);
	}
	if(rwBussinessResponse.ARLen>0)
	{
		CARDRW_HELPER->SaveRWARData(rwBussinessResponse.ARLen,rwBussinessResponse.ARData);
	}
	//??

	// mzy
	//SCAuditKey purchaseKey;
	//purchaseKey.serviceID = ISSUE_SVC;
	//purchaseKey.ticketType = m_DataModel.ProductType;
	//purchaseKey.payType = m_DataModel.issueInfo.paymentMethod;
	//purchaseKey.cardIssuancer = theTICKET_HELPER.GetCardIssuancer((ProductType)m_DataModel.ProductType);

	//SCAuditValue purchaseValue;
	//purchaseValue.transCount = 1;
	//purchaseValue.transAmount = m_DataModel.issueProductInfo.SaleAmount;
	//if(m_DataModel.bPayForDeposit){
	//	purchaseValue.cardCostAmount =m_DataModel.issueProductInfo.cardFee;
	//	purchaseValue.depositAmount =m_DataModel.issueProductInfo.deposit;
	//}
	//else{
	//	purchaseValue.cardCostAmount = 0;
	//	purchaseValue.depositAmount = 0;
	//}
	//purchaseValue.recyleCardNum = 0;

	//theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      打印销售凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::PrinterPrintIssueRecipt()
{
	try{
		_DATE_TIME transtime = ComGetCurTime();
		CStringArray printTexts;
		// M1售卡凭证（FDR）
		if (m_DataModel.mediumInfo.cardPHType == CARD_TYPE_ACC_M1){	
			CString templateM1InfoName = _T("Issue_M1_INFO.template");
			CStringArray tempM1InfoTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateM1InfoName,tempM1InfoTexts);
			// 修改模板
			int index = 0;			
			while(index < tempM1InfoTexts.GetCount()){
				CString& line = tempM1InfoTexts.ElementAt(index);	
				//MST
				if(m_pCommonModel->productInfo.productType == MST){
					if (line.Find(_T("SVT"))!= -1){
						tempM1InfoTexts.RemoveAt(index,3);
						continue;
					}
					else if (line.Find(_T("PST")) != -1){
						tempM1InfoTexts.RemoveAt(index,6);
						continue;
					}
				}
				//PST
				else if (m_pCommonModel->productInfo.productType == PST){
					if (line.Find(_T("SVT"))!= -1){
						tempM1InfoTexts.RemoveAt(index,3);
						continue;
					}
					else if(line.Find(_T("MST"))!= -1){
						tempM1InfoTexts.RemoveAt(index,6);
						continue;
					}
				}
				//SVT
				else if (m_pCommonModel->productInfo.productType == SVT){
					if (line.Find(_T("MST"))!= -1 || line.Find(_T("PST")) != -1){
						tempM1InfoTexts.RemoveAt(index,3);
						continue;
					}
				}
				++index;
			}
			// 填充数据
			index = 0;
			while(index < tempM1InfoTexts.GetCount()){
				CString& line = tempM1InfoTexts.ElementAt(index);
				CString strSerialNo = _T("");
				strSerialNo.Format(_T("%010d"),m_DataModel.dwUDSN);
				line.Replace(_T("{流水號}"),strSerialNo);
				line.Replace(_T("{車票類型}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_DataModel.productInfo.productType,m_DataModel.productInfo.productSubType), LANGUAGE_ID_CHINESE));
				line.Replace(_T("{Ticket_Type}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_DataModel.productInfo.productType,m_DataModel.productInfo.productSubType), LANGUAGE_ID_ENGLISH));
				line.Replace(_T("{Tipos de bilhetes}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_DataModel.productInfo.productType,m_DataModel.productInfo.productSubType),LANGUAGE_ID_PORTUGUESE));
				line.Replace(_T("{有效期}"),m_DataModel.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d")));
				if (m_DataModel.productInfo.productType == SVT){
					line.Replace(_T("[SVT]"),_T(""));
					line.Replace(_T("{票內餘額}"),ComFormatAmount(m_DataModel.productInfo.lProductSaleAmount)+_tl(MONEY_UNIT));
				}
				else if(m_DataModel.productInfo.productType == MST){
					line.Replace(_T("[MST]"),_T(""));
					line.Replace(_T("{票內餘次}"),ComFormatCount(m_DataModel.productInfo.lBalanceRides));
					line.Replace(_T("{車票單價}"),ComFormatAmount(m_DataModel.productInfo.lProductSaleAmount)+_tl(MONEY_UNIT));
				}
				else if(m_DataModel.productInfo.productType == PST){
					line.Replace(_T("[PST]"),_T(""));
					//line.Replace(_T("{有效期間}"),theACC_PRODUCT.GetProductName(ComMakeWORDm_DataModel.productInfo.productType, m_DataModel.productInfo.productSubType, LANGUAGE_ID_CHINESE)+_tl(DAY_CNT_UNIT));
					line.Replace(_T("{車票單價}"),ComFormatAmount(m_DataModel.productInfo.lProductSaleAmount)+_tl(MONEY_UNIT));
				}
				line.Replace(_T("{押金}"), ComFormatAmount(m_DataModel.mediumInfo.lCardDeposit) + _tl(MONEY_UNIT));
				line.Replace(_T("{稅費}"), ComFormatAmount(m_DataModel.paymentInfo.lTaxAmount) + _tl(MONEY_UNIT));
				line.Replace(_T("{張數}"), ComFormatCount(m_DataModel.productInfo.nProductPiece));
				/*---------------------------------------------------*/
				line.Replace(_T("{應收金額}"),ComFormatAmount(m_DataModel.paymentInfo.lDueAmount)+_tl(MONEY_UNIT));
				//line.Replace(_T("{支付方式}"),theACC_BUSINESS.GetPayTypeName(m_DataModel.paymentInfo.paymentMethod,LANGUAGE_ID_CHINESE));		
				//line.Replace(_T("{Payment}"),theACC_BUSINESS.GetPayTypeName(m_DataModel.paymentInfo.paymentMethod,LANGUAGE_ID_ENGLISH));
				//line.Replace(_T("{Pagamento}"),theACC_BUSINESS.GetPayTypeName(m_DataModel.paymentInfo.paymentMethod,LANGUAGE_ID_PORTUGUESE));		
				line.Replace(_T("{支付金額}"),ComFormatAmount(m_DataModel.paymentInfo.lPaidAmount)+_tl(MONEY_UNIT));
				line.Replace(_T("{找零金額}"),ComFormatAmount(m_DataModel.paymentInfo.lChangeAmount)+_tl(MONEY_UNIT));
				/*---------------------------------------------------*/
				line.Replace(_T("{交易時間}"),transtime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				line.Replace(_T("{車站名稱}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
				line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
				line.Replace(_T("{Nome_da_estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
				line.Replace(_T("{設備編號}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

				CStringArray printLines;
				ComPrintLineFormatForTemplate(line,printLines);
				printTexts.Append(printLines);
				++index;
			}
		}
		// UL卡售卡凭证
		else{
			CString templateULTranceInfoName = _T("Issue_UL_INFO.template");
			CStringArray tempULTranceInfoTexts;
			theTEMPLATE_HELPER.LoadBillTemplate(templateULTranceInfoName,tempULTranceInfoTexts);
			int i = 0;			
			while(i<tempULTranceInfoTexts.GetCount()){
				CString& line = tempULTranceInfoTexts.ElementAt(i);	
				CString strSerialNo = _T("");
				strSerialNo.Format(_T("%010d"),m_DataModel.dwUDSN);
				line.Replace(_T("{流水號}"),strSerialNo);
				line.Replace(_T("{車票類型}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_DataModel.productInfo.productType,m_DataModel.productInfo.productSubType), LANGUAGE_ID_CHINESE));
				line.Replace(_T("{Ticket_Type}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_DataModel.productInfo.productType,m_DataModel.productInfo.productSubType), LANGUAGE_ID_ENGLISH));
				line.Replace(_T("{Tipos de bilhetes}"),theACC_PRODUCT.GetProductName(ComMakeWORD(m_DataModel.productInfo.productType,m_DataModel.productInfo.productSubType),LANGUAGE_ID_PORTUGUESE));
				line.Replace(_T("{有效期}"),m_DataModel.productInfo.dtValidateDateTime.ToString(_T("%.4d-%.2d-%.2d")));
				line.Replace(_T("{票內餘額}"),ComFormatAmount(m_DataModel.productInfo.lProductSaleAmount)+_tl(MONEY_UNIT));
				line.Replace(_T("{張數}"),ComFormatCount(m_DataModel.productInfo.nProductPiece));
				/*---------------------------------------------------*/
				line.Replace(_T("{應收金額}"),ComFormatAmount(m_DataModel.paymentInfo.lDueAmount)+_tl(MONEY_UNIT));
				//line.Replace(_T("{支付方式}"),theACC_BUSINESS.GetPayTypeName(m_DataModel.paymentInfo.paymentMethod,LANGUAGE_ID_CHINESE));		
				//line.Replace(_T("{Payment}"),theACC_BUSINESS.GetPayTypeName(m_DataModel.paymentInfo.paymentMethod,LANGUAGE_ID_ENGLISH));
				//line.Replace(_T("{Pagamento}"),theACC_BUSINESS.GetPayTypeName(m_DataModel.paymentInfo.paymentMethod,LANGUAGE_ID_PORTUGUESE));		
				line.Replace(_T("{支付金額}"),ComFormatAmount(m_DataModel.paymentInfo.lPaidAmount)+_tl(MONEY_UNIT));
				line.Replace(_T("{找零金額}"),ComFormatAmount(m_DataModel.paymentInfo.lChangeAmount)+_tl(MONEY_UNIT));
				/*---------------------------------------------------*/
				line.Replace(_T("{交易時間}"),transtime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));
				line.Replace(_T("{車站名稱}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
				line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
				line.Replace(_T("{Nome_da_estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
				line.Replace(_T("{設備編號}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

				CStringArray printLines;
				ComPrintLineFormatForTemplate(line,printLines);
				printTexts.Append(printLines);

				i++;
			}
		}
		//mxw
		//PRINTER_HELPER->PrintReceiptByBmp(&printTexts,true,true);
		PRINTER_HELPER->PrintReceiptByText(&printTexts,true,true);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      打印销售错误凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::PrinterPrintErrorReceipt()
{
	try{
		_DATE_TIME transtime = ComGetCurTime();
		CStringArray printTexts;
		// 售卡失败打印凭证
		CString templateFailedInfoName = _T("TVM_ISSUE_FAILED.template");
		CStringArray tempInfoTexts;
		theTEMPLATE_HELPER.LoadBillTemplate(templateFailedInfoName,tempInfoTexts);
		// 填充数据
		int index = 0;	
		CString strSerialNo = _T("");
		strSerialNo.Format(_T("%010u"),m_DataModel.dwTransactionID);
		while(index < tempInfoTexts.GetCount()){
			CString& line = tempInfoTexts.ElementAt(index);
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
			line.Replace(_T("{交易号}"),strSerialNo);
			line.Replace(_T("{票价}"), ComFormatAmount(m_DataModel.productInfo.lProductSaleAmount)+_cn(MONEY_UNIT));
			line.Replace(_T("{数量}"), ComFormatCount(m_DataModel.productInfo.nProductPiece));
			line.Replace(_T("{付款}"), ComFormatAmount(m_DataModel.paymentInfo.lPaidAmount)+_cn(MONEY_UNIT));
			line.Replace(_T("{实售数}"), ComFormatCount(m_DataModel.productInfo.nProductIssuedPiece));;
			line.Replace(_T("{实找零}"), ComFormatAmount(m_DataModel.paymentInfo.lPaidAmount - m_DataModel.productInfo.lProductSaleAmount*m_DataModel.productInfo.nProductPiece - m_DataModel.paymentInfo.lNotChangeAmount) + _cn(MONEY_UNIT));
			line.Replace(_T("{设备号}"), theMAINTENANCE_INFO.GetCurrentDevice().ToString());
			line.Replace(_T("{日期}"),transtime.ToString(_T("%.4d-%.2d-%.2d %.2d:%.2d:%.2d")));

			CStringArray printLines;
			ComPrintLineFormatForTemplate(line,printLines);
			printTexts.Append(printLines);
			++index;
		}

		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_ISSUE_ERROR);
		PRINTER_HELPER->PrintReceiptByBmp(&printTexts,true,true);
	}
	catch(CSysException& e) {
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

// ****************************************************************************************************
// ****************************************************************************************************
// ****************************************************************************************************
// ****************************************************************************************************

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化可售的产品

@param      无

@retval     无

@exception  //#TODO 2014.11.01 王 需要完善其他接口以支持此调用
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::InitIssueProduct()
{
	//LOG("Begin InitIssueProduct");
	//m_vecIssueProduct.clear();
	//m_vecCardPayment.clear();

	//if(CheckPayment(PAYMENT_METHOD_CASH,CARD_TYPE_ACC_UL)){
	//	m_vecCardPayment.push_back(PAYMENT_METHOD_CASH);
	//}
	////m_vecIssueProduct.clear();
	////m_vecTokenPayment.clear();
	////m_vecIssueProduct.clear();

	////// 0.从参数获取所有支付方式
	////map<WORD,CString> allPayment;
	/////*theACC_BUSINESS.GetPayTypeParam(theAPP_SESSION.GetCurrentClientLanguageType(), allPayment);*/

	////// 1.从参数获取所有产品类型
	////vector<ACC_PRODUCT_t>& allProduct = theACC_PRODUCT.GetProductInfo();
	////vector<ACC_PRODUCT_t>::iterator product_iter = allProduct.begin();
	////vector<ACC_PRODUCT_t>::iterator product_iter_end = allProduct.end();
	////for (; product_iter != product_iter_end; ++product_iter)
	////{
	////	ACC_PRODUCT_t& product = *product_iter;

	////	// 2.与配置文件是否启用标志比较
	////	bool enable = theFunction_INFO.IsTicketIssueEnabled(product.productType);
	////	if (!enable) continue;

	////	// 3.与TVM/AVM设备类型比较
	////	enable = false;
	////	AFC_DEVICE_TYPE currDevice = theMAINTENANCE_INFO.GetDeviceType();
	////	vector<WORD>::iterator device_iter = product.vecSellableDeviceTypes.begin();
	////	vector<WORD>::iterator device_iter_end = product.vecSellableDeviceTypes.end();
	////	for (; device_iter != device_iter_end; ++device_iter)
	////	{
	////		if (currDevice == *device_iter)
	////		{
	////			enable = true;
	////			break;
	////		}
	////	}
	////	if (!enable) continue;

	////	// 4.与Token箱和Card箱的车票物理类型比较
	////	enable = false;
	////	CARD_MEDIUM_TYPE mediumTypeOfTokenBox = (CARD_MEDIUM_TYPE)theTVM_INFO.GetTokenBoxMediumType();
	////	CARD_MEDIUM_TYPE mediumTypeOfCardBox = (CARD_MEDIUM_TYPE)theTVM_INFO.GetTHBoxMediumType();
	////	vector<BYTE>::iterator medium_iter = product.vecSellableCardTypes.begin();
	////	vector<BYTE>::iterator medium_iter_end = product.vecSellableCardTypes.end();
	////	for (; medium_iter != medium_iter_end; ++medium_iter)
	////	{
	////		if (*medium_iter == mediumTypeOfTokenBox || *medium_iter == mediumTypeOfCardBox)
	////		{
	////			enable = true;
	////			break;
	////		}
	////	}
	////	if (!enable) continue;

	////	// 5.与硬件状态（读写器和票箱）做比较
	////	CardType_t cardTypeByProduct = theTICKET_HELPER.GetCardType(product.productType);
	////	CARD_MEDIUM_TYPE mediumTypeByProduct = theTICKET_HELPER.GetMediumType(cardTypeByProduct);
	////	if (*medium_iter == mediumTypeOfTokenBox && mediumTypeByProduct == mediumTypeOfTokenBox)
	////	{
	////		//if (RW_NORMAL != theTVM_STATUS_MGR.GetRWStatus(TVM_TOKEN_RW)) continue;
	////		//if (HARDWARE_MODULE_NORMAL != theTVM_STATUS_MGR.GetTokenModuleStatus()) continue;
	////	}
	////	else if (*medium_iter == mediumTypeOfCardBox && mediumTypeByProduct == mediumTypeOfCardBox)
	////	{
	////		//if (RW_NORMAL != theTVM_STATUS_MGR.GetRWStatus(TVM_CARD_RW)) continue;
	////		//if (HARDWARE_MODULE_NORMAL != theTVM_STATUS_MGR.GetTHModuleStatus()) continue;
	////	}
	////	else
	////	{
	////		continue;
	////	}

	////	// 6.获取支付方式
	////	bool isToken = product.productType == SJT || product.productType == RTT;
	////	if (isToken && m_vecTokenPayment.empty())
	////	{
	////		for (map<WORD,CString>::iterator iter = allPayment.begin(); iter != allPayment.end(); ++iter)
	////		{
	////			DWORD tempPayment = iter->first;
	////			if (!CheckPayment(iter->first, cardTypeByProduct)) continue;
	////			m_vecTokenPayment.push_back(iter->first);
	////		}
	////		if (m_vecTokenPayment.empty()) continue;
	////	}
	////	else if (!isToken && m_vecCardPayment.empty())
	////	{
	////		for (map<WORD,CString>::iterator iter = allPayment.begin(); iter != allPayment.end(); ++iter)
	////		{
	////			DWORD tempPayment = iter->first;
	////			if (!CheckPayment(iter->first, cardTypeByProduct)) continue;
	////			m_vecCardPayment.push_back(iter->first);
	////		}
	////		if (m_vecCardPayment.empty()) continue;
	////	}

	////	// 7.如果存在单程票，将单程票移到第一个产品
	////	if (product.productType == SJT)
	////	{
	////		m_vecIssueProduct.insert(m_vecIssueProduct.begin(), &(*product_iter));
	////	}
	////	else
	////	{
	////		m_vecIssueProduct.push_back(&product);
	////	}
	////}
	LOG("End InitIssueProduct ");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取可售的产品

@param      无

@retval     vector<ACC_PRODUCT_t*>&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
vector<ACC_PRODUCT_t*>& CIssueSvc::GetIssueProduct()
{
	return m_vecIssueProduct;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化可用的支付方式

@param      无

@retval     vector<WORD>&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
vector<WORD>& CIssueSvc::GetIssuePayment()
{
	/*if (m_DataModel.productInfo.productType == SJT_MAIN || m_DataModel.productInfo.productType == RTT)
		return m_vecTokenPayment;*/
	return m_vecCardPayment;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取车票张数信息

@param      (o)map<int,CString>&   票价信息 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::GetIssuePiece(map<int,CString>& mapPrice)
{
	mapPrice.clear();
	vector<int> vecPiece = theFunction_INFO.GetTicketCounts();
	for(int i = 0; i < vecPiece.size(); ++i)
	{
		CString pieces = _T("");
		pieces.Format(_T("%d"),vecPiece[i]);
		mapPrice[vecPiece[i]] = pieces;
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      检查有效的支付方式

@param      无

@retval     无

@exception  后面有时间建议提取出去
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::CheckPayment(WORD payment, CardType_t cardtype)
{
	// 与配置文件是否启用标志比较
	bool enable = theFunction_INFO.IsPayTypeEnable(ISSUE_SVC, cardtype, payment);
	if (!enable) return false;

	// 与硬件状态（硬币模块）、上位支付模式做比较
	DWORD paymentmode = theTVM_STATUS_MGR.GetPaymentMode();
	switch (payment)
	{
	case PAYMENT_METHOD_CASH:
		if(theTVM_STATUS_MGR.GetPaymentMode() == DEVICE_CONTRL_CODE_PAYMENT){
			return false;
		}
		//if (RW_NORMAL != theTVM_STATUS_MGR.GetCoinModuleStatus()) return false;
		//if (RW_NORMAL != theTVM_STATUS_MGR.GetBanknoteModuleStatus()) return false;
		//if (!(paymentmode & DEVICE_CONTRL_CODE_PAYMENT_ALL) &&
		//	!(paymentmode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) &&
		//	!(paymentmode & DEVICE_CONTRL_CODE_PAYMENT_COIN))
		//{
		//	return false;
		//}
		break;
	//case PAYMENT_METHOD_EFT:
	//	//判断当前是否支持EP支付
	//	if(paymentmode & DEVICE_CONTRL_CODE_PAYMENT_EP != DEVICE_CONTRL_CODE_PAYMENT_EP){
	//		return false;
	//	}
	//	break;
	//case PAYMENT_METHOD_COUPON:
	//	break;
	//case PAYMENT_METHOD_POSTBILL_AUTOLOAD:
	//	break;
	case PAYMENT_METHOD_BANK_CARD:
		//if (RW_NORMAL != theTVM_STATUS_MGR.GetBankcardModuleStatus()) return false;
		//if ((!paymentmode & DEVICE_CONTRL_CODE_PAYMENT_ALL) ||
		//	(!paymentmode & DEVICE_CONTRL_CODE_PAYMENT_BANKCARD))
		//{
		//	return false;
		//}
		break;
	/*case PAYMENT_METHOD_CREDIT_CARD:
		break;*/
	default:
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置产品类型

@param      (i)DWORD type  产品类型

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetProductType(WORD type)
{
	CAccTicketParam::TICKET_TYPE product;
	theACC_PRODUCT.GetTicketInfo(ComMakeWORD(type,SJT), product);
	m_DataModel.productInfo.productCategory = UNDEFINE;
	m_DataModel.productInfo.productType = type;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置子产品类型或产品单价

@param      (i)DWORD typeorprice  单程票、钱包为单价，其余为子产品类型

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetSubProductType(DWORD typeorprice)
{
	// 当用户点击界面时，重置超时计时器
	if(theAPP_SESSION.IsBlindManOperation())
		TurnOnPinPadAndTimer(false,true);
	m_KeyValue.clear();
	// 0.子产品信息
	//CAccProductParam::SUB_PRODUCT subproduct;
	//theACC_PRODUCT.GetSubProduct(m_DataModel.productInfo.productType, typeorprice, subproduct);
	// 1.单程票
	if (m_DataModel.productInfo.productType == SJT)
	{
		m_DataModel.productInfo.productSubType = 0;
		m_DataModel.productInfo.lProductSaleAmount = typeorprice;
		m_DataModel.productInfo.dtValidateDateTime = ComGetCurTime();
	}
	//// 2.SVT
	//else if (m_DataModel.productInfo.productType == SVT)
	//{
	//	m_DataModel.productInfo.productSubType     = 0x01;
	//	m_DataModel.productInfo.lProductSaleAmount = typeorprice;
	//	m_DataModel.paymentInfo.lTaxAmount         = theACC_BUSINESS.GetCardHolderFee(PhyCardType_t::CARD_TYPE_MIFARE_1_S70,CardholderFeeType_t::CARDHOLDER_FEE_TYPE_TAX,typeorprice);
	//	m_DataModel.productInfo.dtValidateDateTime = theACC_PRODUCT.GetProductValidateDate(m_DataModel.productInfo.productType, m_DataModel.productInfo.productSubType);
	//	m_DataModel.productInfo.lValidatePeriod    = theACC_PRODUCT.GetProductValidatePeriod(m_DataModel.productInfo.productType, m_DataModel.productInfo.productSubType);
	//	// 盲人购票需要转到选择张数的界面
	//	if(theAPP_SESSION.IsBlindManOperation()){
	//		// 点击确认按钮
	//		GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
	//	}
	//}
	//// 3.MST
	else if(m_DataModel.productInfo.productType == MST){
	//	m_DataModel.productInfo.productSubType = typeorprice;
	//	m_DataModel.productInfo.lBalanceRides = theACC_PRODUCT.GetProductMultiRideTicketCount(m_DataModel.productInfo.productType, typeorprice);
	//	m_DataModel.productInfo.dtValidateDateTime = theACC_PRODUCT.GetProductValidateDate(m_DataModel.productInfo.productType, typeorprice);
	//	m_DataModel.productInfo.lValidatePeriod = 0;//theACC_PRODUCT.GetProductValidatePeriod(m_DataModel.productInfo.productType, typeorprice);
	//	m_DataModel.needBeginStation = subproduct.isOriginRequired;
	//	m_DataModel.needEndStation = subproduct.isDestinationRequired;
	//	m_DataModel.needInputBeginStation = subproduct.isOriginUserInput;
	//	m_DataModel.needInputEndStation = subproduct.isDestinationUserInput;
	//	m_DataModel.BeginStationIsCurrent = subproduct.isValidityOriginCurrentStation;
	//	m_DataModel.EndStationIsCurrent = subproduct.isValidityDestinationCurrentStation;
	//	// 判断是否需要手动选择起始车站
	//	if (m_DataModel.needBeginStation){
	//		if (m_DataModel.needInputBeginStation){
	//			// 盲人购票需要提示选择起始区段
	//			if(theAPP_SESSION.IsBlindManOperation()){
	//				m_operation_status = STATUS_BLIND_SELECT_BEGIN_SECTION;
	//				LOG("提示选择起始区段：MST");
	//				PlayEventAudio(AUDIO_CARD_SELECT_BEGIN_SECTION);
	//				m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_CARD_SELECT_BEGIN_SECTION);
	//			}
	//		}
	//		else{
	//			if (m_DataModel.BeginStationIsCurrent){
	//				WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
	//				m_DataModel.productInfo.beginLocation = theACC_LOCATION.GetStationLocationNumber(currentStation);
	//			}
	//			else{
	//				m_DataModel.productInfo.beginLocation = subproduct.validityOrigin;
	//			}
	//		}
	//	}
	//	else{
	//		m_DataModel.productInfo.beginLocation = subproduct.validityOrigin;
	//	}

		// 判断是否需要手动选择目的车站
		if(m_DataModel.needEndStation){
			if (m_DataModel.needInputEndStation)
			{
				// 需要手动输入，则保持当前选中状态
				if(theAPP_SESSION.IsBlindManOperation()){
					if(!m_DataModel.needInputBeginStation){
						// 提示需要选择目的区间
						m_operation_status = STATUS_BLIND_SELECT_END_SECTION;
						LOG("提示选择目的区段：MST");
						PlayEventAudio(AUDIO_CARD_SELECT_END_SECTION);
						m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_CARD_SELECT_END_SECTION);
					}
				}
			}
			else
			{
				if (m_DataModel.EndStationIsCurrent)
				{
					WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
					m_DataModel.productInfo.endLocation = currentStation/*theACC_LOCATION.GetStationFareCode(currentStation)*/;
				}
				else{
					//m_DataModel.productInfo.endLocation = subproduct.validityDestination;
				}
			}
		}
		else{
			//m_DataModel.productInfo.endLocation = subproduct.validityDestination;
		}

		// 不需要选择起始和目的区段
		if(theAPP_SESSION.IsBlindManOperation()){
			if (!m_DataModel.needInputBeginStation && !m_DataModel.needInputEndStation){
				// 点击确认按钮
				GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
			}
		}
	}
	// 4.PST
	else if(m_DataModel.productInfo.productType == PST){
		m_DataModel.productInfo.productSubType = typeorprice;
		m_DataModel.productInfo.lBalanceRides = 0;//theACC_PRODUCT.GetProductMultiRideTicketCount(m_DataModel.productInfo.productType, typeorprice);
		//m_DataModel.productInfo.dtValidateDateTime = theACC_PRODUCT.GetProductValidateDate(m_DataModel.productInfo.productType, typeorprice);
		//m_DataModel.productInfo.lValidatePeriod = theACC_PRODUCT.GetProductValidatePeriod(m_DataModel.productInfo.productType, typeorprice);
		/*m_DataModel.needBeginStation = subproduct.isOriginRequired;
		m_DataModel.needEndStation = subproduct.isDestinationRequired;
		m_DataModel.needInputBeginStation = subproduct.isOriginUserInput;
		m_DataModel.needInputEndStation = subproduct.isDestinationUserInput;
		m_DataModel.BeginStationIsCurrent = subproduct.isValidityOriginCurrentStation;
		m_DataModel.EndStationIsCurrent = subproduct.isValidityDestinationCurrentStation;*/
		// 判断是否需要手动选择起始车站
		if (m_DataModel.needBeginStation){
			if (m_DataModel.needInputBeginStation){
				// 盲人购票需要提示选择起始区段
				if(theAPP_SESSION.IsBlindManOperation()){
					m_operation_status = STATUS_BLIND_SELECT_BEGIN_SECTION;
					LOG("提示选择起始区段：PST");
					PlayEventAudio(AUDIO_CARD_SELECT_BEGIN_SECTION);
					m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_CARD_SELECT_BEGIN_SECTION);
				}
			}
			else{
				if (m_DataModel.BeginStationIsCurrent){
					WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
					//m_DataModel.productInfo.beginLocation = theACC_LOCATION.GetStationLocationNumber(currentStation);
				}
				else{
					//m_DataModel.productInfo.beginLocation = subproduct.validityOrigin;
				}
			}
		}
		else{
//			m_DataModel.productInfo.beginLocation = subproduct.validityOrigin;
		}

		// 判断是否需要手动选择目的车站
		if(m_DataModel.needEndStation){
			if (m_DataModel.needInputBeginStation)
			{
				// 需要手动输入，则保持当前选中状态
				if(theAPP_SESSION.IsBlindManOperation()){
					if(!m_DataModel.needInputBeginStation){
						// 提示需要选择目的区间
						m_operation_status = STATUS_BLIND_SELECT_END_SECTION;
//						LO..G("提示选择目的区段：PST");
						PlayEventAudio(AUDIO_CARD_SELECT_END_SECTION);
						m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_CARD_SELECT_END_SECTION);
					}
				}
			}
			else
			{
				if (m_DataModel.EndStationIsCurrent)
				{
					WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
					//m_DataModel.productInfo.endLocation = theACC_LOCATION.GetStationLocationNumber(currentStation);
				}
				else{
					//m_DataModel.productInfo.endLocation = subproduct.validityDestination;
				}
			}
		}
		else{
			//m_DataModel.productInfo.endLocation = subproduct.validityDestination;
		}

		// 不需要选择起始和目的区段
		if(theAPP_SESSION.IsBlindManOperation()){
			if (!m_DataModel.needInputBeginStation && !m_DataModel.needInputEndStation){
				// 点击确认按钮
				GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
			}
		}
	}

	m_DataModel.mediumInfo.cardPHType = theTICKET_HELPER.GetCardType(SJT/*MAKEWORD(m_DataModel.productInfo.productType, m_DataModel.productInfo.productSubType)*/);
	m_DataModel.mediumInfo.mediumType = theTICKET_HELPER.GetMediumType(m_DataModel.mediumInfo.cardPHType);
	//m_DataModel.mediumInfo.lCardDeposit = (product.chargeCardDeposit != 1) ? 0 : theACC_SYSTEM.GetCardDeposit(ACC, m_DataModel.mediumInfo.mediumType);
	//m_DataModel.mediumInfo.lCardFee = (product.chargeCardFee != 1) ? 0 : theACC_SYSTEM.GetCardDeposit(ACC, m_DataModel.mediumInfo.mediumType);
	//m_DataModel.needCardDeposit = product.chargeCardDeposit == 1;
	//m_DataModel.needCardFee = product.chargeCardFee == 1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置起始车站/区段

@param      (i)DWORD locationNumber 车站

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetBeginStation(DWORD locationNumber /* = LOCATION_NUMBER_UNSPECIFIED */)
{
	if (m_DataModel.productInfo.productType == SJT)
	{
		m_DataModel.needBeginStation = true;			
		m_DataModel.needInputBeginStation = false;
		m_DataModel.BeginStationIsCurrent = true;
	}

	if (m_DataModel.needBeginStation)
	{
		// 手动设置
		if (m_DataModel.needInputBeginStation)
		{
			m_DataModel.productInfo.beginLocation = locationNumber;
			// 如果盲人购票，判断是否需要提示输入目的区段
			if (theAPP_SESSION.IsBlindManOperation()){
				if (m_DataModel.needInputEndStation && m_DataModel.productInfo.endLocation == LOCATION_NUMBER_UNSPECIFIED){
					m_operation_status = STATUS_BLIND_SELECT_END_SECTION;
					PlayEventAudio(AUDIO_CARD_SELECT_END_SECTION);
					TurnOnPinPadAndTimer(false,true);
					m_KeyValue.clear();
					m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_CARD_SELECT_END_SECTION);
				}
				// 目的区段已经选择，跳转到选择支付方式页面
				else 
					GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
			}			
		}
		else
		{
			if (m_DataModel.BeginStationIsCurrent)
			{
				WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
				m_DataModel.productInfo.beginLocation = currentStation;//theACC_LOCATION.GetStationLocationNumber(currentStation);
			}
			else
			{
//				CAccProductParam::SUB_PRODUCT subproduct;
//				theACC_PRODUCT.GetSubProduct(m_DataModel.productInfo.productType, m_DataModel.productInfo.productSubType, subproduct);
//				m_DataModel.productInfo.beginLocation = subproduct.validityOrigin;
			}
		}
	}
	else
	{
		m_DataModel.productInfo.beginLocation = LOCATION_NUMBER_UNSPECIFIED;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置目的车站/区段

@param      (i)DWORD locationNumber 车站

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetEndStation(DWORD locationNumber /* = LOCATION_NUMBER_UNSPECIFIED */)
{
	if (m_DataModel.productInfo.productType == SJT)
	{
		m_DataModel.needEndStation = true;
		m_DataModel.needInputEndStation = true;
		m_DataModel.EndStationIsCurrent = false;
	}
	if (m_DataModel.needEndStation)
	{
		if (m_DataModel.needInputEndStation)
		{
			m_DataModel.productInfo.endLocation = locationNumber;
			if(theAPP_SESSION.IsBlindManOperation()){
				// 如果需要输入起始车站，且没有选择起始车站
				if(m_DataModel.needInputBeginStation && m_DataModel.productInfo.beginLocation == LOCATION_NUMBER_UNSPECIFIED){
					m_operation_status = STATUS_BLIND_SELECT_BEGIN_SECTION;
					PlayEventAudio(AUDIO_CARD_SELECT_BEGIN_SECTION);
					TurnOnPinPadAndTimer(false,true);
					m_KeyValue.clear();
					m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_CARD_SELECT_BEGIN_SECTION);
				}
				// 起始区段已经选择，跳转到选择支付方式页面
				else 				
					GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
			}			
		}
		else
		{
			if (m_DataModel.EndStationIsCurrent)
			{
				WORD currentStation = theMAINTENANCE_INFO.GetStationCodeWithLine();
				//m_DataModel.productInfo.endLocation = theACC_LOCATION.GetStationLocationNumber(currentStation);
			}
			else
			{
				//CAccProductParam::SUB_PRODUCT subproduct;
				//theACC_PRODUCT.GetSubProduct(m_DataModel.productInfo.productType, m_DataModel.productInfo.productSubType, subproduct);
				//m_DataModel.productInfo.endLocation = subproduct.validityDestination;
			}
		}
	}
	else
	{
		m_DataModel.productInfo.endLocation = LOCATION_NUMBER_UNSPECIFIED;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置乘客类型

@param      (i)PassengerType_t type 乘客类型

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetPassengerType(PassengerType_t type)
{
	m_DataModel.mediumInfo.passengerType = type;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置售票张数

@param      (i)BYTE piece 张数

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetTicketPiece(WORD piece)
{
	if(theAPP_SESSION.IsBlindManOperation())
		TurnOnPinPadAndTimer(false,true);
	m_DataModel.productInfo.nProductPiece = piece;

	//// 选择完张数后开始收钱
	//if(!m_bNeedClickOkButton && !m_bStartAcceptCash){
	//	m_bStartAcceptCash = true;
	//	GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_OK);
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置支付类型

@param      (i)BYTE piece 张数

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetPayment(PaymentMethod_t paymentMethod)
{
	m_DataModel.paymentInfo.paymentMethod = paymentMethod;
	// 盲人购票，需要提示相关信息:交易确认
	if(theAPP_SESSION.IsBlindManOperation()){
		if(m_DataModel.productInfo.productType == SVT)
			PlayEventAudio(AUDIO_CARD_CONFIRM_BUSINESS_SVT);// SVT
		else if (m_DataModel.productInfo.productType == MST)
			PlayEventAudio(AUDIO_CARD_CONFIRM_BUSINESS_MST);// MST
		else if (m_DataModel.productInfo.productType == PST)
			PlayEventAudio(AUDIO_CARD_CONFIRM_BUSINESS_PST);// PST
		else if (m_DataModel.productInfo.productType == SJT)
			PlayEventAudio(AUDIO_TOKEN_CONFIRM_BUSINESS);	// SJT
		m_operation_status = STATUS_BLIND_CONFIRM_BUSINESS;
		TurnOnPinPadAndTimer(false,true);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否支持发售单程票

@param      无

@retval     bool

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::SupportSJT()
{
	// 如果当前设备为AVM,不可能支持售TOKEN
//	if(DEVICE_TYPE_AVM == theMAINTENANCE_INFO.GetDeviceType()){
//		return false;
//	}
//	else{
//		vector<ACC_PRODUCT_t*>::iterator it = m_vecIssueProduct.begin();
//		while(it != m_vecIssueProduct.end())
//		{
//			//return m_vecIssueProduct.at(0)->productType == SJT ? true : false;
//// 			if ((*it)->productType == SJT){
//// 				return true;
//// 			}
//			it++;
//		}
//	}	
	if((theTVM_STATUS_MGR.GetWorkMode() & DEVICE_CONTRL_CODE_BUSINESS_CARD) == DEVICE_CONTRL_CODE_BUSINESS_CARD){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否需要乘客选择起始区段

@param      无

@retval     bool

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::NeedBeginSectionByPassenger()
{
	/*DWORD backupStation = m_DataModel.productInfo.beginLocation;
	SetBeginStation(0);
	if (m_DataModel.productInfo.beginLocation == 0)
	{
		m_DataModel.productInfo.beginLocation = backupStation;
		return true;
	}
	return false;*/
	return m_DataModel.needInputBeginStation;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否需要乘客选择目的区段

@param      无

@retval     bool

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::NeedEndSectionByPassenger()
{
	/*DWORD backupStation = m_DataModel.productInfo.endLocation;
	SetEndStation(0);
	if (m_DataModel.productInfo.endLocation == 0)
	{
		m_DataModel.productInfo.endLocation = backupStation;
		return true;
	}
	return false;*/
	return m_DataModel.needInputEndStation;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取单程票的售票模式，用于读写器的售票方法

@param      无

@retval     BYTE

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
BYTE CIssueSvc::AutoCalcSaleMode()
{
	// 起始站都选择为按起始站售票
	if (m_DataModel.productInfo.beginLocation != LOCATION_NUMBER_UNSPECIFIED&&m_DataModel.productInfo.endLocation != LOCATION_NUMBER_UNSPECIFIED){
		return BY_BEGIN_END_STATION;
	}
	return BY_PRICE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票价模式，用于读写器的票价查询方法

@param      无

@retval     BYTE

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
BYTE CIssueSvc::AutoCalcPriceMode()
{
	if (m_DataModel.productInfo.productType == SJT || m_DataModel.productInfo.productCategory == PURSE)
	{
		return BY_CONSUME_PRICE;
	}
	return BY_SALE_PRICE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取产品单价，不包括押金卡费(已不用统一在父类处理)

@param      无

@retval     bool true:成功   false:失败

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::AutoCalcTicketPrice()
{
	try{
		// 重置票价
		m_DataModel.productInfo.lProductSaleAmount = 0;

		// 填充结构体
		RW_CMD_TICKET_PRICE rwCmdTicketPrice;
		rwCmdTicketPrice.CardType			= m_DataModel.mediumInfo.cardPHType;
		rwCmdTicketPrice.ProductCategory	= m_DataModel.productInfo.productCategory;
		rwCmdTicketPrice.ProductType		= m_DataModel.productInfo.productType;
		rwCmdTicketPrice.ProductSubType		= m_DataModel.productInfo.productSubType;
		rwCmdTicketPrice.PriceType			= AutoCalcPriceMode();
		rwCmdTicketPrice.PassengerType		= m_DataModel.mediumInfo.passengerType;
		rwCmdTicketPrice.Time				= ComGetCurTime();
		memcpy(rwCmdTicketPrice.BegainStation, &m_DataModel.productInfo.beginLocation, 4);
		memcpy(rwCmdTicketPrice.EndStation, &m_DataModel.productInfo.endLocation, 4);

		// 计算票价
		long errorCode = -1;
		RW_CMD_TICKET_PRICE_RES rwCmdRes;
		if (m_DataModel.productInfo.productType == SJT)
		{
			if(!theEXCEPTION_MGR.HasException(CRWException::TVM_TOKEN_RW_MODULE_ID)
				&&!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID))
			{
				//			errorCode = TOKENRW_HELPER->GetTicketPrice(rwCmdTicketPrice, rwCmdRes);
			}
		}
		else
		{
			if(!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID)
				&&!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID))
			{
				errorCode = CARDRW_HELPER->GetTicketPrice(rwCmdTicketPrice, rwCmdRes);
			}
		}
		// 错误处理
		if(errorCode != RW_RSP_OK)
		{
			//@TODO：此处可以增加错误日志，或者统一的错误处理
			//theAPP_SESSION.ShowOfficeGuide(RECHARGERW_HELPER->GetTpuErrorInfo(rwCmdRes.DetailErrorCode));
			return false;
		}

		m_DataModel.productInfo.lProductSaleAmount = rwCmdRes.TicketPrice;
		return true;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      自动计算应付金额

@param      无

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::AutoCalcPaymentInfo()
{
	// 应付金额 = （产品单价 + 卡押金 + 卡费）* 车票张数
	m_DataModel.paymentInfo.lDueAmount = (m_DataModel.productInfo.lProductSaleAmount
		+ m_DataModel.mediumInfo.lCardDeposit + m_DataModel.mediumInfo.lCardFee) * m_DataModel.productInfo.nProductPiece;

	// 找零金额 = 已付金额 - 应付金额
	if(m_DataModel.paymentInfo.lPaidAmount > 0)
	{
		m_DataModel.paymentInfo.lChangeAmount = m_DataModel.paymentInfo.lPaidAmount - m_DataModel.paymentInfo.lDueAmount;
	}
	else
	{
		m_DataModel.paymentInfo.lChangeAmount = 0;
	}

	// 父类存在问题，临时决绝办法
	m_DataModel.paymentInfo.lPriceAmount = m_DataModel.paymentInfo.lDueAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      当前界面操作完成，跳转到下一个界面

@param      无

@retval     无

@exception  无 
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::DoneToNextDialog()
{
	// 总图，默认业务界面
	if (GetCurDialogGroupID() == DIALOG_GROUP_AREA_MAP){
		if (m_ePayType == CIssueSvc::PAY_WITH_SVT){
			m_pDialogFlow->ShowNextDialogGroup(WM_MAP_TO_SVT);
		}
		else if (m_ePayType == CIssueSvc::PAY_WITH_NORMAL){
			m_pDialogFlow->ShowNextDialogGroup(WM_MAP_TO_CASH);
		}
	}
	else if (GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_CASH){
		m_pDialogFlow->ShowNextDialogGroup(WM_CASH_TO_NET);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始支付钱钱

@param      无

@retval     无

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::StartPayMoney()
{
	try{
		//theSERVICE_MGR.SetForeServiceBusy(true);
		if (m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_CASH)
		{
			StartReceiveCash();		
		} 
// 		else if(m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_EFT)
// 		{
// 			//StartReceiveBankCard();
// 		}
		else if(m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD)
		{
			StartReadEPCard();
		}	
		issuePayDetailInfo.hasStartPaid = true;
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      分析读到的车票

@param      (io)UINT& resultCode 读卡结果
@param      (o)bool& needStopLoop 是否需要停止当前循环

@retval     long 错误代码

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::AnalyzeTicket(long& resultCode, bool& needStopLoop)
{
	try{
		// 初始化数据
		m_invalide_ticket_trans = _invalid_ticket_recycletrade();

		// 填充数据		
		m_invalide_ticket_trans.cardPhysicalType = m_DataModel.rwReadCardResponse.CardPHType;

		// 读卡结果判断
		if (resultCode != RW_RSP_OK)
		{
			// 暂时的处理：停止当前循环，然后重新发票。后期可以增加其他错误信息判断（黑名单、读卡失败）
			needStopLoop = true;
			return false;
		}

		// 卡类型判断
		if (m_DataModel.rwReadCardResponse.CardPHType != m_DataModel.mediumInfo.mediumType)
		{
			resultCode = RW_RSP_UNKNOWERR;
			needStopLoop = true;
			return false;
		}

		// 解析数据
		if (m_DataModel.pCardInfo != NULL)
		{
			delete m_DataModel.pCardInfo;
			m_DataModel.pCardInfo = NULL;
		}
		m_DataModel.pCardInfo = new CACCCard;
		m_DataModel.pCardInfo->GetCardInfo(m_DataModel.rwReadCardResponse.CardInfo);
		
		// 卡号
		m_invalide_ticket_trans.cardSerialNo = 0;
		// 发行商
		m_invalide_ticket_trans.issueId			 = m_DataModel.pCardInfo->applicationData.CardIssuerCode;

		/*BYTE btCardNum[8];
		memset(btCardNum,0,sizeof(btCardNum));

		ComRevLong((long)m_DataModel.pCardInfo->applicationData.CardPhysicalNumber,btCardNum);

		memcpy(m_DataModel.paymentInfo.EPCardNum,btCardNum,sizeof(btCardNum));*/
		return true;
	}
	catch(CSysException& e){
		throw;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始售卡写卡和找零

@param      无

@retval     bool  true:售卡成功  false:售卡失败

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::StartIssueAndChange(){
	try{
#if 1
		bool bIssueSuccess = false;

		theSERVICE_MGR.SetState(ISSUEING);
		NotifyDataModelChanged();
		m_DataModel.lDeviceStatus |= DEVICE_CARD_BUSY;
		if (m_DataModel.productInfo.nProductIssuedPiece < m_DataModel.productInfo.nProductPiece){
			// 过程中如果产生异常，需要特殊处理
			Func<bool()> issueFunc(this, &CIssueSvc::IssueTicketProc);
			issueFunc.BeginInvoke();
			bIssueSuccess = issueFunc.EndInvoke();
			LOG("票卡发售：%d",m_DataModel.productInfo.nProductIssuedPiece);
			LOG("EndInvoke:%d",bIssueSuccess);
		}

		m_DataModel.lDeviceStatus = DEVICE_ALL_OK;
		StartTimer(1111,3);

#else
		// 盲人发售中提示正在交易中
		if(theAPP_SESSION.IsBlindManOperation())PlayEventAudio(AUDIO_BUSINESS_DOING);
		// 设置发售中，界面自动变为不可用，两种情形恢复原状态：1、发票完成；2：出现故障。
		theSERVICE_MGR.SetState(ISSUEING);
		// 收钱并找零
		AcceptMoney();

		bool bIssueSuccess = false;

		// 设备忙于发卡中
		m_DataModel.lDeviceStatus |= DEVICE_CARD_BUSY;
		// 1.发售第一张车票
		//bIssueSuccess = IssueOneTicket();
		//if (!bIssueSuccess)
		//{
		//	m_pCommonModel->operationInfo.finishType = NOT_ENOUGH_TICKETS_FOR_BOOKING;
		//	m_DataModel.lDeviceStatus &= ~DEVICE_CARD_BUSY;
		//	// 一张票也没有放出，异常金额为票价乘以售票张数
		//	m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * m_pCommonModel->productInfo.lProductSaleAmount;
		//	// 做失败处理
		//	OnReturnFinish();
		//	return;
		//}
		//++m_DataModel.productInfo.nProductIssuedPiece;

		// 2.收钱、找零：过程中有可能产生异常，注意售卡和找零异常处理。
		//AcceptMoney();
		//try{
		//	IO_HELPER->OpenReturnCupIndicator();// 打开指示灯
		//}
		//catch(CSysException& e){
		//	theEXCEPTION_MGR.ProcessException(e);
		//}
		// 3.发售剩余车票，如果有的话
		if (m_DataModel.productInfo.nProductIssuedPiece < m_DataModel.productInfo.nProductPiece){
			// 过程中如果产生异常，需要特殊处理
			Func<bool()> issueFunc(this, &CIssueSvc::IssueTicketProc);
			issueFunc.BeginInvoke();
			bIssueSuccess = issueFunc.EndInvoke();
			LOG("票卡发售：%d",m_DataModel.productInfo.nProductIssuedPiece);
			LOG("EndInvoke:%d",bIssueSuccess);
		}

		m_DataModel.lDeviceStatus &= ~DEVICE_CARD_BUSY;
		if(m_DataModel.paymentInfo.paymentMethod==PAYMENT_METHOD_WITH_WEIXIN || m_DataModel.paymentInfo.paymentMethod==PAYMENT_METHOD_WITH_ZHIFUBAO){
			ConfirmOrder();
		}
		// 设置完成售卡标志		
		if (bIssueSuccess)
		{
			LOG("出票成功，打开照明灯。");
			IO_HELPER->OpenReturnLight();	
			IO_HELPER->OpenReturnCupIndicator();

			OnReturnFinish();
		}
		else{
			m_DataModel.operationInfo.finishType = CPassengerForeSvc::TRANSACTION_STATE_FAILED;
			m_DataModel.dwTransactionID = theCOUNT_INFO.GetTransactionNum();
			// 如果没有成功，则打印没有发售的车票凭证。
			m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_CARD_MOUDOL] = (m_pCommonModel->productInfo.nProductPiece - m_pCommonModel->productInfo.nProductIssuedPiece) * (m_pCommonModel->productInfo.lProductSaleAmount > 0 ? m_pCommonModel->productInfo.lProductSaleAmount : 0);

			OnReturnFinish();
		}
#endif
		
	}
	catch (CSysException& e){
		//异常处理，需要很多的判断。注意处理。
		//m_DataModel.operationInfo.m_vecException.push_back(e);
		OnError(e);
	}
	catch(...){
		theSERVICE_MGR.SetForeServiceBusy(false);
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发售车票过程函数

@param      无

@retval     bool true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IssueTicketProc()
{
	try{
		LOG("-------共需要发售%d张车票--------",m_DataModel.productInfo.nProductPiece);
		if (m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO || m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN){
			// 如果是二维码购票，准备保存票卡信息 
			if(NULL!=m_DataModel.actrualTicketInfo){
				delete m_DataModel.actrualTicketInfo;
				m_DataModel.actrualTicketInfo = NULL;
			}
			m_DataModel.actrualTicketInfo = new TicketInfo[m_DataModel.productInfo.nProductPiece];
		}

		while (m_DataModel.productInfo.nProductIssuedPiece < m_DataModel.productInfo.nProductPiece){
			if(IssueOneTicket()){
				++m_DataModel.productInfo.nProductIssuedPiece;
				
				LOG("已发售%d张。",m_DataModel.productInfo.nProductIssuedPiece);

				if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
					LOG("打开照明灯。");
					try{
						//IO_HELPER->OpenReturnLight();
						//IO_HELPER->OpenReturnCupIndicator();
					}
					catch(CSysException& e){
						theEXCEPTION_MGR.ProcessException(e);
					}
				}
			}
			else{
				// 连续发n张票后，失败，返回错误。n为参数限制发票失败张数。
				return false;
			}
		}
		LOG("-----------发售正常完成------------");
		return true;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发售一张车票Token或Card

@param      无

@retval     bool true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IssueOneTicket()
{
	try{
		/*if (m_DataModel.mediumInfo.mediumType == ULTRALIGHT)
		return IssueOneToken();*/
		return IssueOneCard();
	}
	catch(...){
		// 异常交由上层处理。
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发售一张Token

@param      无

@retval     bool true:成功  false:失败

@exception  车票成功失败计数，暂时未添加，需要结合SQLite数据库一起考虑，然后增加。

对于发票和读写卡的异常捕获，需要结合计数统一考虑。
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IssueOneToken()
{
	return false;
	//try{
	//	// 获取车票连续出错最大张数
	//	CAfcTVMOperationParam::TPU_PARAM tpuParam;
	//	theAFC_TVM_OPERATION.GetTpuParam(tpuParam);
	//	int maxAllowErrorCount =  tpuParam.maxWriteErrorCnt == 0 ? 0x7FFFFFFF : tpuParam.maxWriteErrorCnt;
	//	// 获取Token箱阈值
	//	CAfcTVMOperationParam::MAGAZINE_PARAM tokenBoxParam;
	//	theAFC_TVM_OPERATION.GetMagazineParam(tokenBoxParam);

	//	// 开始售票
	//	int retryCount = 0;
	//	while (retryCount < maxAllowErrorCount)
	//	{
	//		// 设置抓票票箱：先A后B
	//		TOKEN_CMD_SEND_PROCESS cmdCatchProcess;
	//		cmdCatchProcess.TicketCnt = 1;
	//		if ((m_DataModel.tokenBoxAInfo.ulCurCount > tokenBoxParam.TokenAEmptyValue)&& MAG_NUM_EMPTY != theTVM_STATUS_MGR.GetTokenHopper1NumStatus())
	//		{
	//			cmdCatchProcess.magazine = TOKEN_MAGAZINE_A;
	//		}
	//		else if ((m_DataModel.tokenBoxBInfo.ulCurCount > tokenBoxParam.TokenBEmptyValue)&& MAG_NUM_EMPTY != theTVM_STATUS_MGR.GetTokenHopper2NumStatus())
	//		{
	//			cmdCatchProcess.magazine = TOKEN_MAGAZINE_B;
	//		}
	//		else
	//		{
	//			// 增加异常处理，不能发票了。现在未确定异常类型，待修改。
	//			return false;
	//		}

	//		// 抓票
	//		bool isRecycle = true;
	//		long catchErrorCode = -1;
	//		long issueErrorCode = -1;
	//		TOKEN_RSP_ISSUE_PROCESS rspIssueProgress;
	//		TOKEN_RSP_SEND_PROCESS rspCatchProcess;
	//	
	//		// 抓票
	//		catchErrorCode = TOKEN_HELPER->TOKENCatchCard(cmdCatchProcess, rspCatchProcess);		
	//		if (catchErrorCode != 0 || rspCatchProcess.isSendIntoErrorBox){
	//			// 抓票时卡在出票口，未到读写区
	//			if(rspCatchProcess.isTokenHopper1SendOutStuck || rspCatchProcess.isTokenHopper2SendOutStuck){
	//				// 更新计数
	//				if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_A){
	//					m_DataModel.tokenBoxAInfo.ulCurCount-- ;
	//					theTOKEN_COUNT.SetHopperAInfo(m_DataModel.tokenBoxAInfo);
	//				}
	//				else if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_B){
	//					m_DataModel.tokenBoxBInfo.ulCurCount--;
	//					theTOKEN_COUNT.SetHopperBInfo(m_DataModel.tokenBoxBInfo);
	//				}
	//				return false;
	//			}
	//			++retryCount;
	//			// 判断是否票箱为空呢？
	//			if(rspCatchProcess.isTokenHopper1Empty && theTVM_STATUS_MGR.GetTokenHopper1NumStatus() != MAG_NUM_EMPTY){
	//				theTVM_STATUS_MGR.SetTokenHopper1NumStatus(MAG_NUM_EMPTY);
	//				continue;
	//			}
	//			if(rspCatchProcess.isTokenHopper2Empty && theTVM_STATUS_MGR.GetTokenHopper2NumStatus() != MAG_NUM_EMPTY){
	//				theTVM_STATUS_MGR.SetTokenHopper2NumStatus(MAG_NUM_EMPTY);
	//				continue;
	//			}

	//			// 送到废票箱
	//			TOKEN_HELPER->TOKENIssueCard(cmdCatchProcess.magazine,isRecycle = true, rspIssueProgress);
	//			// 售Token失败，添加废票箱计数，减少Hopper计数
	//			if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_A){
	//				m_DataModel.tokenBoxAInfo.ulCurCount-- ;
	//				theTOKEN_COUNT.SetHopperAInfo(m_DataModel.tokenBoxAInfo);
	//			}
	//			else if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_B){
	//				m_DataModel.tokenBoxBInfo.ulCurCount--;
	//				theTOKEN_COUNT.SetHopperBInfo(m_DataModel.tokenBoxBInfo);
	//			}
	//			m_DataModel.tokenWasteBoxInfo.ulCurCount++;
	//			theTOKEN_COUNT.SetTokenboxWastedInfo(m_DataModel.tokenWasteBoxInfo);
	//			//??theTICKET_HELPER.UpdateTHCount(recycleCardResponse.boxIdACount,recycleCardResponse.boxIdBCount,recycleCardResponse.issueRes==3?1:0);    //更新回收车票计数
	//			continue;
	//		}
	//		else if(0 == catchErrorCode){// 表示一定抓取票卡成功，更新Token箱计数
	//			if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_A){
	//				m_DataModel.tokenBoxAInfo.ulCurCount-- ;
	//				theTOKEN_COUNT.SetHopperAInfo(m_DataModel.tokenBoxAInfo);
	//			}
	//			else if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_B){
	//				m_DataModel.tokenBoxBInfo.ulCurCount--;
	//				theTOKEN_COUNT.SetHopperBInfo(m_DataModel.tokenBoxBInfo);
	//			}
	//		}

	//		// 读写后售卡
	//		long saleErrorCode = -1;
	//		RW_BUSSINESS_RESPONSE saleCardResponse;
	//		saleErrorCode = ReadWriteToken(saleCardResponse);
	//		if(saleErrorCode != RW_RSP_OK)
	//		{
	//			TOKEN_HELPER->TOKENIssueCard(cmdCatchProcess.magazine,isRecycle = true, rspIssueProgress);
	//			// 读Token失败，增加废票箱计数
	//			m_DataModel.tokenWasteBoxInfo.ulCurCount++;
	//			theTOKEN_COUNT.SetTokenboxWastedInfo(m_DataModel.tokenWasteBoxInfo);
	//			++retryCount;
	//			continue;
	//		}

	//		// 保存交易及审计
	//		SaveBusinessData(saleCardResponse);

	//		long relaseToken = TOKEN_HELPER->TOKENThrowCard(cmdCatchProcess.magazine, rspIssueProgress);
	//		//if(relaseToken == TOKENTRS_RES_OK){
	//		//	// 计数
	//		//	if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_A){
	//		//		m_DataModel.tokenBoxAInfo.ulCurCount-- ;
	//		//		theTOKEN_COUNT.SetHopperAInfo(m_DataModel.tokenBoxAInfo);
	//		//	}
	//		//	else if(cmdCatchProcess.magazine == TOKEN_MAGAZINE::TOKEN_MAGAZINE_B){
	//		//		m_DataModel.tokenBoxBInfo.ulCurCount--;
	//		//		theTOKEN_COUNT.SetHopperBInfo(m_DataModel.tokenBoxBInfo);
	//		//	}
	//		//}
	//		// 成功发售一张车票
	//		break;
	//	}

	//	// 车票连续出错（待完善：）
	//	if(retryCount >= maxAllowErrorCount)
	//	{			
	//		return false;
	//	}

	//	return true;
	//}
	//// 所有异常交由上层处理！
	//catch(CSysException& e){
	//	throw;
	//}
	//catch(...){
	//	throw;
	//}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读写器卖一张卡

@param      

@retval     

@exception  卡发和读写卡不建议采用异步方式进行，同步基本就可以；
保持交易数据和放出票卡，可以写为异步，暂时写为同步，如果效率低的话，再建议改为异步

车票成功失败计数，暂时未添加，需要结合SQLite数据库一起考虑，然后增加。

对于发票和读写卡的异常捕获，需要结合计数统一考虑。
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IssueOneCard()
{
	try{
		// 1. 获取票箱数量信息
		m_DataModel.cardBoxAInfo = theCARD_COUNT.GetCardboxAInfo();
		m_DataModel.cardBoxBInfo = theCARD_COUNT.GetCardboxBInfo();
		m_DataModel.cardWasteBoxInfo = theCARD_COUNT.GetCardboxWastedInfo();
		// 2.获取车票连续出错最大张数
		CAfcTVMOperationParam::TPU_PARAM tpuParam;
		theAFC_TVM_OPERATION.GetTpuParam(tpuParam);
		int maxAllowErrorCount = tpuParam.maxWriteErrorCnt == 0 ? 0x03 : tpuParam.maxWriteErrorCnt;
		// 3.获取Card箱阈值
		CAfcTVMOperationParam::MAGAZINE_PARAM cardBoxParam;
		theAFC_TVM_OPERATION.GetMagazineParam(cardBoxParam);

		LONG nTotalCount = m_DataModel.cardBoxAInfo.ulCurCount + m_DataModel.cardBoxBInfo.ulCurCount;
		LONG nAvaliableCountA = m_DataModel.cardBoxAInfo.ulCurCount - cardBoxParam.TicketEmptyValue;
		LONG nAvaliableCountB = m_DataModel.cardBoxBInfo.ulCurCount - cardBoxParam.TicketEmptyValue;
		if (nAvaliableCountA<=0){
			nAvaliableCountA = 0;
		}
		if (nAvaliableCountB<=0){
			nAvaliableCountB = 0;
		}
		LOG("票箱A当前有%d张，参数定义的A票箱已空值为%d，可用张数%d", m_DataModel.cardBoxAInfo.ulCurCount, cardBoxParam.TicketEmptyValue, nAvaliableCountA);
		LOG("票箱B当前有%d张，参数定义的B票箱已空值为%d，可用张数%d", m_DataModel.cardBoxBInfo.ulCurCount, cardBoxParam.TicketEmptyValue, nAvaliableCountB);
		//if (m_DataModel.productInfo.nProductPiece - m_DataModel.productInfo.nProductIssuedPiece > nAvaliableCountA + nAvaliableCountB){
		//	LOG("票箱A和B加起来张数都不够，发售失败。");
		//	return false;// 需要退钱
		//}


		CString strTicketID, strIssueTime, strWriteTime;
		int retryCount = 0;
		TH_CMD_CARD_OUT OutCardBox = CMD_CARD_OUT_BOX_A;
		tTIMCardNum cardNum;
		maxAllowErrorCount = 1;
		while(retryCount < maxAllowErrorCount)
		{
			if ((m_DataModel.cardBoxAInfo.ulCurCount + m_DataModel.cardBoxBInfo.ulCurCount ) <= cardBoxParam.TicketEmptyValue){
				return false;
			}

			//if (m_DataModel.cardBoxAInfo.ulCurCount > cardBoxParam.TicketEmptyValue && theTVM_STATUS_MGR.GetCardBox1NumStatus()!=MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY){
			//	OutCardBox = CMD_CARD_OUT_BOX_A;
			//}
			//else if (m_DataModel.cardBoxBInfo.ulCurCount > cardBoxParam.TicketEmptyValue && theTVM_STATUS_MGR.GetCardBox2NumStatus()!=MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY){
			//	OutCardBox = CMD_CARD_OUT_BOX_B;
			//}
			//else{
			//	LOG("两个票箱都不可用，发售失败");
			//	return false;
			//}

			// 抓取票卡
			long lRet = 0;
			UINT uiTransportNum = 0;
			try{
				//lRet = TOKEN_HELPER->CardOut(OutCardBox,&uiTransportNum);	// 尝试抓票，如果空了继续抓取另一个票箱
				lRet = CARD_HELPER->CardOut(OutCardBox,&uiTransportNum);	// 尝试抓票，如果空了继续抓取另一个票箱
			}
			catch(CSysException& e){
				// 票箱1已空(A)
				if(e.GetDetailID() == TIM_ERR_CODE_CASEBOXA_EM){
					theTVM_STATUS_MGR.SetCardBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
					m_DataModel.cardBoxAInfo.ulCurCount = 0;
					theCARD_COUNT.SetCardboxAInfo(m_DataModel.cardBoxAInfo);
					continue;
				}
				// 票箱2已空
				else if(e.GetDetailID() == TIM_ERR_CODE_CASEBOXB_EM){
					theTVM_STATUS_MGR.SetCardBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
					m_DataModel.cardBoxBInfo.ulCurCount = 0;
					theCARD_COUNT.SetCardboxBInfo(m_DataModel.cardBoxBInfo);
					continue;
				}
				else{
					// 其他异常类，直接抛出
					throw e;
				}
			}
			catch(...){
				// 交给上层处理
				throw;
			}
			
			if(lRet != 0){
				++retryCount;
				continue;
			}

			// 票箱票减少
			if(OutCardBox == CMD_CARD_OUT_BOX_A){
				m_DataModel.cardBoxAInfo.ulCurCount -= 1;
				theCARD_COUNT.SetCardboxAInfo(m_DataModel.cardBoxAInfo);
			}

			if(OutCardBox == CMD_CARD_OUT_BOX_B){
				m_DataModel.cardBoxBInfo.ulCurCount -= 1;
				theCARD_COUNT.SetCardboxBInfo(m_DataModel.cardBoxBInfo);
			}

			//// 读写卡
			//RW_BUSSINESS_RESPONSE saleCardResponse;
			//long saleErrorCode = ReadWriteCard(saleCardResponse);
			//// 写卡失败！
			//if(saleErrorCode != RW_RSP_OK){
			//	UINT uiRetractBoxNum = 0;
			//	CARD_HELPER->RetractCard(&uiRetractBoxNum);// 进废票箱
			//	// 废票箱增加一张票。
			//	m_DataModel.cardWasteBoxInfo.ulCurCount += uiRetractBoxNum;
			//	theCARD_COUNT.SetCardboxWastedInfo(m_DataModel.cardWasteBoxInfo);
			//	// 票箱票减少
			//	if(OutCardBox == CMD_CARD_OUT_BOX_A){
			//		m_DataModel.cardBoxAInfo.ulCurCount -= 1;
			//		theCARD_COUNT.SetCardboxAInfo(m_DataModel.cardBoxAInfo);
			//	}

			//	if(OutCardBox == CMD_CARD_OUT_BOX_B){
			//		m_DataModel.cardBoxBInfo.ulCurCount -= 1;
			//		theCARD_COUNT.SetCardboxBInfo(m_DataModel.cardBoxBInfo);
			//	}

			//	// 保存废卡交易
			//	m_invalide_ticket_trans.reson = 0x01;// 无效票卡
			//	// 废票箱ID
			//	m_invalide_ticket_trans.magazineId.bDeviceType = m_DataModel.cardWasteBoxInfo.ticketBoxID.bDeviceType;
			//	m_invalide_ticket_trans.magazineId.bType = m_DataModel.cardWasteBoxInfo.ticketBoxID.bType;
			//	m_invalide_ticket_trans.magazineId.bNum = m_DataModel.cardWasteBoxInfo.ticketBoxID.bNum;
			//	m_invalide_ticket_trans.magazineId.bStationID = m_DataModel.cardWasteBoxInfo.ticketBoxID.bStationID;
			//	m_invalide_ticket_trans.productType		= 0x0062;
			//	SaveInvalidateTicketBussiness(m_invalide_ticket_trans);

			//	++retryCount;
			//	continue;
			//}

			//// 保存交易及审计
			//SaveBusinessData(saleCardResponse);
			long errCode = -1;

			// 不管放出去失败与否，都将认为是放出去成功了，因为卡已经写成功了，不会进废票箱。
			try{
				// 放出票卡：有可能票卡会卡在出票口。
				errCode = CARD_HELPER->SendCard(&uiTransportNum);
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}

			// 成功发售一张车票
			// 如果是二维码购票，保存票卡信息 
			if (m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO || m_DataModel.paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN){
				strTicketID = ComConvToHexStr(m_DataModel.pCardInfo->applicationData.CardLogicalNumber, 8);
				strIssueTime = ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d"));
				m_DataModel.actrualTicketInfo[m_DataModel.productInfo.nProductIssuedPiece].ticketID = strTicketID;
				m_DataModel.actrualTicketInfo[m_DataModel.productInfo.nProductIssuedPiece].strWriteTicketTime = strWriteTime;
				m_DataModel.actrualTicketInfo[m_DataModel.productInfo.nProductIssuedPiece].strIssueTime = strIssueTime;
			}
			break;
		}
		// 如果重试次数大于参数中允许错误最大次数，返回失败。
		if (retryCount >= maxAllowErrorCount)
		{
			return false;
		}
		return true;
	}
	catch(CSysException& e){
		throw;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读写Token

@param      (o)RW_BUSSINESS_RESPONSE& rwBusinessResponse 写卡反馈

@retval     long

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::ReadWriteToken(RW_BUSSINESS_RESPONSE& rwBusinessResponse)
{
	try{
		long errorCode = RW_RSP_OK;
		for(int retryTimes = 0; retryTimes < 3; ++retryTimes)
		{
			// 读卡
			//errorCode = TOKENRW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),m_DataModel.rwReadCardResponse);

			// 分析
			bool needStopLoop = false;
			if (!AnalyzeTicket(errorCode, needStopLoop))
			{
				if (needStopLoop) return errorCode;
				continue;
			}

			// 写卡
			RW_ISSUE_REQUEST rwIssueRequest = GetIssueParameter();
			//errorCode = TOKENRW_HELPER->Issue(rwIssueRequest, rwBusinessResponse);
			if(errorCode == RW_RSP_OK) break;
		}
		return errorCode;
	}
	catch(CSysException& e){
		throw;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      读写Card

@param      (o)RW_BUSSINESS_RESPONSE& rwBusinessResponse 写卡反馈

@retval     long

@exception  无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::ReadWriteCard(RW_BUSSINESS_RESPONSE& rwBusinessResponse)
{
	try{
		LOG("售单程票卡读卡。");
		long errorCode = RW_RSP_OK;
		for(int retryTimes = 0; retryTimes < 3; ++retryTimes)
		{
			// 读卡
			errorCode = CARDRW_HELPER->TryReadCard((SERVICE_ID)GetServiceID(),m_DataModel.rwReadCardResponse);

			// 分析
			bool needStopLoop = false;
			if (!AnalyzeTicket(errorCode, needStopLoop))
			{
				if (needStopLoop) {
					LOG("读卡失败，停止发售:%d", errorCode);
					return errorCode;
				}
				LOG("读卡失败");
				continue;
			}

			
			LOG("准备写卡。");
			// 写卡
			RW_ISSUE_REQUEST rwIssueRequest = GetIssueParameter();
			errorCode = CARDRW_HELPER->Issue(rwIssueRequest, rwBusinessResponse);
			if(errorCode == RW_RSP_OK){ 
				// 写卡完成，检查票卡是否写完整
				RW_READ_CARD_RESPONSE rwReadCardResponse;
				errorCode = CARDRW_HELPER->TryReadCard(ADJUST_SVC,rwReadCardResponse, false);
				if(errorCode == RW_RSP_OK){ 
					if(rwReadCardResponse.PaidAreaAdjustCode == 0x00){
						CACCCard accCard;
						// 解析票卡
						if(!accCard.GetCardInfo(rwReadCardResponse.CardInfo)) {
							LOG("解析票卡信息失败！");
							errorCode = RW_RSP_WRITE_CARD_FAILED;
							continue;
						}	
	
						if(accCard.v_ProductInfo.size()==0){
							LOG("解析票卡信息失败！");
							errorCode = RW_RSP_WRITE_CARD_FAILED;
							continue;
						}

						LOG("写卡完成，回读检查票卡成功。卡号=%s, 卡内余额=%d", ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8), accCard.v_ProductInfo[0].Balance);
						break;
					}
					else{
						errorCode = RW_RSP_WRITE_CARD_FAILED;
					}
				}
				LOG("写卡完成，回读检查票卡失败：%d", errorCode);
			}
			else{
				LOG("写卡失败，重试写卡！");
				continue;
			}
		}
		LOG("售卡返回数据：%d",errorCode);
		return errorCode;	
	}
	catch(CSysException& e){
		throw;
	}
	catch(...){
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      售票找零异常

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnIssueAndChangeError()
//{
//	//// 未发票出错
//	//if (m_DataModel.productInfo.nProductIssuedPiece < 0)
//	//{
//	//	// 还钱结束
//	//}
//
//	//// 是否收到硬币、是否压硬币成功
//	//bool bCoinAccept = m_DataModel.operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE;
//	//bool bCoinAcceptSuccess = m_DataModel.operationInfo.coinAcceptState == COIN_ACCEPT_STATE_COLLECTED;
//	//// 是否硬币找零、是否硬币找零成功
//	//bool bCoinChange = m_DataModel.operationInfo.banknoteAcceptState == COIN_CHANGE_STATE_NONE;
//	//bool bCoinChangeSuccess = m_DataModel.operationInfo.coinChangeState == COIN_CHANGE_STATE_NONE;
//	//// 是否收到纸币、是否压纸币成功
//	//bool bNoteAccept = m_DataModel.operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE;
//	//bool bNoteAcceptSuccess = m_DataModel.operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_COLLECTED;
//	//// 是否售卡成功
//	//bool bIssueSuccess = m_DataModel.productInfo.nProductIssuedPiece == m_DataModel.productInfo.nProductPiece;
//
//	//// 仅收钱时出错
//	//if ((bCoinAccept && !bCoinAcceptSuccess) || (bNoteAccept && !bNoteAcceptSuccess))
//	//{
//	//}
//
//	//// 仅找零时出错
//	//if ()
//	//{
//	//}
//
//	//// 仅发票时出错
//
//	//// 收钱（收钱出错不会找零）和发票都出错
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      EP支付读卡成功后剩余扣款工作

@param      LPRW_READ_CARD_RESPONSE pReadResponse(读卡后的返回数据)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::FinishEPPayment(LPRW_READ_CARD_RESPONSE pReadResponse){
	// 是否需要进行一些数据解析？判断当前卡片是否支持呢？TPU会自动反馈，不需要我们去做
	try{
		// 扣款
		RW_CMD_CONSUME epPay;
		RW_BUSSINESS_RESPONSE rsp;
		epPay.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
		epPay.TranDateTime = m_DataModel.transTime;
		epPay.ConsumeAmount = m_DataModel.paymentInfo.lDueAmount;

		CACCCard accCard;
		if(!accCard.GetCardInfo(pReadResponse->CardInfo)) {
			//OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
			return -1;
		}

		if(accCard.applicationData.CardIssuerCode==ACC){
			SetPayment(PAYMENT_METHOD_ACC_CARD);
		}
		else if(accCard.applicationData.CardIssuerCode==ECT){
			SetPayment(PAYMENT_METHOD_YKT_CARD);
		}

		// EP 卡号
		memcpy(m_DataModel.paymentInfo.EPCardNum,accCard.applicationData.CardLogicalNumber,8);

		long errCode = RECHARGERW_HELPER->ConsumeEP(epPay,rsp);
		if(RW_RSP_OK == errCode){	
			// EP 支付金额
			m_DataModel.paymentInfo.lEPReceiveTotalAmount = m_DataModel.paymentInfo.lDueAmount;
			// 成功:发票
			m_DataModel.operationInfo.paymentState = CPassengerForeSvc::PAYMENT_STATE_SUCCESS;
			m_DataModel.paymentInfo.lPaidAmount    = m_DataModel.paymentInfo.lEPReceiveTotalAmount; 
			// 计算修改存储的卡内余额（避免再读卡获取）
			m_DataModel.productCardInfo.lBalanceAmount -= m_DataModel.paymentInfo.lDueAmount;
			// 保存交易
			SaveBusinessData(rsp);
			NotifyDataModelChanged();
		}
		// 失败：提示支付失败，请继续刷卡？
		else{

		}

		return errCode;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币和硬币是否都达到最大限制数量

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IsBothCoinBanknoteMaxCount()
{
	if (IsBanknoteMaxCount()&& IsCoinMaxCount()){
		return true;
	}
	else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		判断接收纸币是否超过最大限额  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IsBanknoteMaxAmount()
{
	DWORD maxAmount = theAFC_TVM_OPERATION.GetAcceptBanknoteMaxAmoutOfIssueCard();

	if (m_DataModel.paymentInfo.lBHReceiveTotalAmount > maxAmount*100){ 
		return true;
	}
	else{
		return false;
	}
};

//////////////////////////////////////////////////////////////////////////
/**
@brief		售票时投入纸币数量是否超过最大限制？  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IsBanknoteMaxCount(){
	// 不支持纸币，表示已满
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) != DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
		return true;
	}

	DWORD maxCount = theAFC_TVM_OPERATION.GetAcceptBanknoteMaxPieceOfIssueCard();
	// 判断当期钱箱最大可投入数量？
	// 判断当前钱箱数量
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();

	long lCurCount = banknoteInfo.BankntoeTotalCount();//banknoteInfo.ulCount_10 + banknoteInfo.ulCount_100 + banknoteInfo.ulCount_1 + banknoteInfo.ulCount_20 + banknoteInfo.ulCount_50 + banknoteInfo.ulCount_5;
	// 当前投入数量大于等于最大可接受数量时，投入数量达到最大。
	if(m_DataModel.paymentInfo.lBHReceiveTotalCount >= ((long)(theAFC_TVM_OPERATION.GetBanknotesFullConut()) - lCurCount)){
		LOG("纸币箱接收数量达到最大限制。");
		return true;
	}
	// 判断当前投入是否大于最大参数限制。
	if (m_DataModel.paymentInfo.lBHReceiveTotalCount >= maxCount){ // acceptChrgrMaxBillAmt返回“元”，但是BHReceiveMoney的参数是“分”。
		return true;
	}
	else{
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
bool CIssueSvc::IsCoinMaxCount(){
	// 不支持硬币，表示已满
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
		return true;
	}
	//// 1.取各个票箱数据
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_A_info = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopperAinfo			 = theCOIN_COUNT.GetCoinHopperboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopperBinfo			 = theCOIN_COUNT.GetCoinHopperboxBInfo();
	// 2.回收箱最大可接收数量
	long ulBoxA = theAFC_TVM_OPERATION.GetRecycleBoxFullCount() - box_A_info.GetCurCount();
	// 3.循环找零箱最大接收数量？
	long ulStockA = (long)theTVM_INFO.GetCHHopper1MaxCount() - hopperAinfo.ulCurCount;
	long ulStockB = (long)theTVM_INFO.GetCHHopper2MaxCount() - hopperBinfo.ulCurCount;

	// 4.已经接收的1元的数量
	long ulReciveOneCount   = m_DataModel.paymentInfo.PayCoinInfo[Coin1Yuan];
	// 5.已经接收非1元的数量
	long ulReciveOtherCount = m_DataModel.paymentInfo.PayCoinInfo[CoinHalfYuan];

	// 6.如果暂存1的数量大于循环找零箱和回收箱1的数量、如果暂存2的数量大于等于回收箱2的数量
	if(ulReciveOneCount >= ulStockA){
		if(ulReciveOneCount >= ulBoxA){
			return true;
		}
		else{
			m_bCoinHopperAccept = false;
		}
	}
	if(ulReciveOtherCount >= ulStockB){
		if(ulReciveOtherCount >= ulBoxA){
			return true;
		}
		else{
			m_bCoinHopperAccept = false;
		}
	}


	// 7.如果暂存1大于暂存1最大接收数量，或者暂存2的数量大于等于暂存2最大接收枚数
	if(ulReciveOneCount >= theTVM_INFO.GetCHTemporary1Max() || ulReciveOtherCount >= theTVM_INFO.GetCHTemporary2Max()){
		return true;
	}
	// 8.如果没有满，则判断当前接受数量是否超过最大参数限制？
	if (m_DataModel.paymentInfo.lCHReceiveTotalCount > theAFC_TVM_OPERATION.GetAcceptCoinMaxPieceOfIssue()){
		return true;
	}
	else{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		判断找零数量是否超过最大限制  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CIssueSvc::IsCoinChageMaxCount(){
	// 找零总数量
	DWORD dwChangeCount = m_DataModel.paymentInfo.dwChangeCoinOneYuanCount + m_DataModel.paymentInfo.dwChangeCoinHalfYuanCount;//m_DataModel.paymentInfo.dwChangeStockCount + m_DataModel.paymentInfo.dwChangeHopper1Count + m_DataModel.paymentInfo.dwChangeHopper2Count;
	if (dwChangeCount > theAFC_TVM_OPERATION.GetChageCoinMaxPieceOfIssue()){
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
bool CIssueSvc::IsBHChangeMaxCount(){
	DWORD dwChangeCount = m_DataModel.paymentInfo.BNRChangeCount();//m_DataModel.paymentInfo.dwChangeBHBoxACount + m_DataModel.paymentInfo.dwChangeBHBoxBCount;
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
bool CIssueSvc::IsBHChangeMaxAmout(){
	if(m_DataModel.paymentInfo.dwChangeBHAmout > theAFC_TVM_OPERATION.GetBHChangeMaxAmount() * 100){
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		转到错误界面  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::FlowToErrorDlg()
{
	__super::FlowToErrorDlg();

	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_CASH){
		DoDialogFlow(WM_CASH_TO_ERROR);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_SVT){
		DoDialogFlow(WM_SVT_TO_ERROR);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_NET){
		DoDialogFlow(WM_NET_TO_ERROR);
	}
	else{
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		pMainSvc->StartDefaultService();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		转到错误界面  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::FlowToNetPayDlg()
{
	if(m_pDialogFlow->GetCurDialogGroupID() == 9){
		//DoDialogFlow(WM_INIT_TO_NET_PAY);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		返回到完成界面  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::FlowToFinishDlg(){
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_CASH){
		m_pDialogFlow->ShowNextDialogGroup(WM_CASH_TO_FINISH);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_SVT){
		m_pDialogFlow->ShowNextDialogGroup(WM_SVT_TO_FINISH);
	}
	else if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_ISSUE_WITH_NET){
		m_pDialogFlow->ShowNextDialogGroup(WM_NET_TO_FINISH);
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
void CIssueSvc::FlowToDefaultDlg(){
	LOG("Flow to default dialog.");
	DoDialogFlow(WM_CASH_TO_FINISH);

	// 清空填充的数据
	m_DataModel.operationInfo = OPERATION_INFO();// 操作信息
	m_DataModel.productInfo   = PRODUCT_INFO();	 // 产品信息
	m_DataModel.mediumInfo    = MEDIUM_INFO();	 // 票卡信息
	m_DataModel.mediumCardInfo = MEDIUM_INFO();	 // 票卡信息
	m_DataModel.paymentInfo   = PAYMENT_INFO();	 // 支付信息
	m_ePayType = PAY_WITH_NORMAL;
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	// 判断是否有硬件异常:
	if (theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID) || 
		theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID) ||
		theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID) || 
		theEXCEPTION_MGR.HasException(CRWException::TVM_TOKEN_RW_MODULE_ID)){
			pMainSvc->StartDefaultService();
			return;
	}

	//pMainSvc->StartDefaultService();
	return;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		获取应该显示的产品的单位  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CIssueSvc::GetCurrentUnitOfProduct(){
	CString strUintOfProduct;
	switch(m_DataModel.productInfo.productType){
	/*case SVT:
		strUintOfProduct = _T("(") + _tl(MONEY_UNIT) + _T(")");
		break;
	case MST:
		strUintOfProduct = _T("(") + _tl(TIMES_UNIT) + _T(")");
		break;
	case PST:
		strUintOfProduct = _T("(") + _tl(DAY_CNT_UNIT) + _T(")");
		break;*/
	default:
		strUintOfProduct = _T("");
		break;
	}
	return strUintOfProduct;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取可接收的纸币面额

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
BRH_FACE_ACEPTED_TABLE CIssueSvc::GetAcceptBanknotesFace(tBncSetInhibitList* table){
	vector<WORD> vecFace;
	BRH_FACE_ACEPTED_TABLE bhAcceptTable;

	theAFC_TVM_OPERATION.GetAcceptBanknoteFaceOfIssueCard(vecFace);
	
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
COIN_ACCEPTABLE CIssueSvc::GetAcceptCoinFaceTable(){
	// 1.判断回收箱是否在位？
	COIN_ACCEPTABLE acceptTable;
	
	// 2.将可接收面额转换为Table表
	vector<WORD> vecFace;
	theAFC_TVM_OPERATION.GetAcceptCoinFaceOfIssue(vecFace);
	for (vector<WORD>::iterator it = vecFace.begin();it != vecFace.end(); it++)
	{
		if(50 == *it){
			acceptTable.CH_COIN_ALLOW_05 = true;
		}
		else if(100 == *it){
			acceptTable.CH_COIN_ALLOW_1 = true;
		}
	}
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_A_info = theCOIN_COUNT.GetCollectionCoinboxAInfo();// 只判断这一个
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO box_B_info = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopperAInfo			 = theCOIN_COUNT.GetCoinHopperboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopperBInfo			 = theCOIN_COUNT.GetCoinHopperboxBInfo();

	// 可接收的钱数量
	long lOneCanAcceptCount = 0,lHalfCanAcceptCount = 0;
	lOneCanAcceptCount  = theTVM_INFO.GetCHHopper1MaxCount() - hopperBInfo.ulCurCount;// 一元Hopper最大可以接收数量
	lHalfCanAcceptCount = theTVM_INFO.GetCHHopper2MaxCount() - hopperAInfo.ulCurCount;// 五角Hopper最大可以接收数量

	if(lOneCanAcceptCount < 0){
		lOneCanAcceptCount = 0;
	}
	if(lHalfCanAcceptCount < 0){
		lHalfCanAcceptCount = 0;
	}
	// 判断回收箱是否在位
	if(m_IOMoudelStatus.isCoinboxReady ){
		int dwMaxAccept = theAFC_TVM_OPERATION.GetRecycleBoxFullCount() - box_A_info.GetCurCount();
		if(dwMaxAccept > 0){
			lOneCanAcceptCount += dwMaxAccept;
			lHalfCanAcceptCount+= dwMaxAccept;
		}
	}

	// 判断是否可以接收？1元
	if(lOneCanAcceptCount <= 0 && acceptTable.CH_COIN_ALLOW_1){
		acceptTable.CH_COIN_ALLOW_1 = false;
	}

	// 判断是否可以接收？五角
	if(lHalfCanAcceptCount <=0 && acceptTable.CH_COIN_ALLOW_05){
		acceptTable.CH_COIN_ALLOW_05 = false;
	}
	return acceptTable;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      播放语音并设置状态

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::PlayAudioInDlg(DWORD eventID){
	// 停止播放语音
	if(eventID == 0){
		m_pIssueAudioHelper->StopPlayEventAudio();
		return;
	}
	// 确认支付信息
	else if(eventID == AUDIO_CONFORM_PAYINFO){
		//TurnOnPinPadAndTimer(false,true);// 当支付完成时，超时时间由本服务继续监管
		m_operation_status = STATUS_BLIND_CONFIRM_PAYMENT;
	}
	// 投入现金
	else if(eventID == AUDIO_INSERT_CASH){
		m_operation_status = STATUS_BLIND_INSERT_CASH;
	}
	// 交易成功
	else if(eventID == AUDIO_BUSINESS_SUCCESS){
		m_operation_status = STATUS_BLIND_BUSINESS_SUCCESS;
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		PlayEventAudio(eventID,true);// 交易成功
		return;
	}
	// 交易取消，退钱
	else if(eventID == AUDIO_BUSINESS_CANCEL){
		if(m_pCommonModel->operationInfo.banknoteAcceptState == CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_NONE &&
			m_pCommonModel->operationInfo.coinAcceptState == CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_NONE){
			eventID = AUDIO_BUSINESS_CANCEL_NORMAL;
		}
		m_operation_status = STATUS_BLIND_BUSINESS_CANCEL;
		PlayEventAudio(eventID,true);
		if(!m_bIsPinPadStop){
			//PINPAD_HELPER->TurnOffInputTrigger();
			return;
		}
		else{
			__super::OnCancel();
			return;
		}
	}
	// 打印成功
	else if(eventID == AUDIO_BUSINESS_PRINT_SUCCESS){
		m_operation_status = STATUS_BLIND_PRINT_SUCCESS;
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
		PlayEventAudio(eventID,true);
		return;
	}
	// 提示选择支付方式
	else if(eventID == AUDIO_SELECT_PAYMENT){
		m_operation_status = STATUS_BLIND_SELECT_PAYMENT;
		m_KeyValue.clear();
		m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_SELECT_PAYMENT);
	}
	// 当前不支持此支付方式
	else if(eventID == AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT){
		m_operation_status = STATUS_BLIND_SELECT_PAYMENT;
		m_KeyValue.clear();
		m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_SELECT_PAYMENT);
		PlayEventAudio(eventID);
		TurnOnPinPadAndTimer(false,true);	
		return;
	}
	PlayEventAudio(eventID);
	TurnOnPinPadAndTimer(false,true);	
	//// 在支付完成时，设置确认支付状态：现金
	//if(m_operation_status == STATUS_BLIND_INSERT_CASH && m_pCommonModel->operationInfo.isFinishedReceveMoney){
	//	TurnOnPinPadAndTimer(false,true);// 当支付完成时，超时时间由本服务继续监管
	//	m_operation_status = STATUS_BLIND_CONFIRM_PAYMENT;
	//}
	//// 支付未完成时
	//else if(m_operation_status == STATUS_BLIND_INSERT_CASH && !m_pCommonModel->operationInfo.isFinishedReceveMoney){
	//	if(eventID == AUDIO_BUSINESS_CANCEL){// 乘客取消
	//		m_operation_status = STATUS_BLIND_CANCEL;
	//	}
	//	else
	//		TurnOnPinPadAndTimer(false,false);// 停止当前操作超时计时器，由现金投入记录超时信息
	//}
	//// 完成发票，且状态是发售中，转换状态为发售成功
	//else if(m_operation_status == STATUS_BLIND_ISSUING && m_pCommonModel->operationInfo.isSuccessIssueCard){
	//	m_operation_status = STATUS_BLIND_BUSINESS_SUCCESS;
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
	//	PlayEventAudio(eventID,true);
	//	return;
	//}
	//// 发售成功，且打印成功
	//else if(m_operation_status == STATUS_BLIND_BUSINESS_SUCCESS && m_pCommonModel->operationInfo.failurePrintState == CPassengerForeSvc::FAILURE_PRINT_STATE_SUCCESS){
	//	m_operation_status = STATUS_BLIND_PRINT_SUCCESS;
	//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
	//	PlayEventAudio(eventID,true);
	//	return;
	//}
	//// 交易取消:确认支付
	//else if(m_operation_status == STATUS_BLIND_CONFIRM_PAYMENT){
	//	m_operation_status = STATUS_BLIND_CANCEL;
	//	StopTimer(TIMERS_OPERATION_OUT_OF_TIME);// 停止当前操作超时计时器，由现金投入记录超时信息
	//}
	//// 当前状态为已取消时，不在播放语音
	//else if(m_operation_status == STATUS_BLIND_CANCEL){
	//	return;
	//}
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      语音播放接口

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::PlayEventAudio(DWORD eventID,bool bStopRepeat /* = false */){
	LOG("PlayEventAudio----->");
	// 不是盲人购票则不播放任何语言
	if(!theAPP_SESSION.IsBlindManOperation()){
		return;
	}
	// 1.本句语音ID
	if(eventID == AUDIO_COMMON_MSG_INPUT_ERROR || eventID == AUDIO_COMMON_MSG_NOT_SUPPORT_PAYMENT){
		bStopRepeat = false;
	}
	else
		 m_dwLastPlayedVoice = eventID;
	// 2.播放前先停止计时器
	StopTimer(TIMERS_OUT_OF_REPLAY);
	unsigned int nPlayTimes = 0;
	m_pIssueAudioHelper->playEventAudio(eventID,nPlayTimes);
	int nTimeSecends = nPlayTimes/1000 + 6;
	// 3.是否重复播放？
	if(!bStopRepeat)
		StartTimer(TIMERS_OUT_OF_REPLAY,m_VoicePlayTime = nTimeSecends);
	else
		StopTimer(TIMERS_OUT_OF_REPLAY);
	LOG("PlayEventAudio-----<");
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      盲人购票开始操作响应函数

@param		WPARAM wParam 购票种类

@param		LPARAM lParam 无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIssueSvc::OnBlindBeginOperation(WPARAM wParam,LPARAM lParam){
	try{
		// 购票
		if(wParam == STATUS_BLIND_BUY_TOKEN){
			m_operation_status = STATUS_BLIND_BUY_TOKEN;
			//CMessagePictureBox::Show(_T("提示"),_T("请选择站点选择方式，然后按下确认键，取消请按取消键，按热点购票请按1,按站点购票请按2。"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
			PlayEventAudio(AUDIO_TOKEN_SELECT_END_STATION);
			m_KeyValue.clear();
			m_pIssueAudioHelper->getKeyMap(m_KeyValue,AUDIO_TOKEN_SELECT_END_STATION);
		}
		// 购卡
		else if(wParam == STATUS_BLIND_BUY_CARD){
			//CMessagePictureBox::Show(_T("提示"),_T("请选择车票种类，然后按下确认键，取消请按取消键，储值票请按1，计次票请按2，定期票请按3"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
			m_operation_status = STATUS_BLIND_BUY_CARD;
			PlayEventAudio(AUDIO_CARD_SELECT_PRODUCT_TYPE);
		}
		TurnOnPinPadAndTimer();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      密码键盘回调函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CIssueSvc::OnPinPadPressed(UINT res,CTVMPinPadStartInput* pPinPad){
//
//	try{
//		// 取值正常
//		if(res == 0){
//			LOG("密碼鍵盤反饋正常");
//			// 1.取密码键盘回调回来的值
//			UE_PINPADKEY emGetValue = PINPAD_HELPER->PraserInputValue(pPinPad->getInputValue());
//			m_bIsPinPadStop = true;
//			StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
//			// 2.如果手动取消
//			if(UE_PINPADKEY::UE_UNKNOWN_KEY == emGetValue || UE_PINPAD_NOINPUT == emGetValue){
//				LOG("键盘无输入取消。");
//				// 操作超时
//				if(m_DataModel.operationInfo.finishType == RUNNING_OUT_OF_TIME){
//					LOG("交易超时而取消");
//					//PlayEventAudio(AUDIO_COMMON_MSG_TIMEOUT_CANCEL,true);
//					// 主界面超时
//					if (GetCurDialogGroupID() == DIALOG_GROUP_AREA_MAP){
//						PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//						theAPP_SESSION.SetBlindManStatus(false);
//						//PINPAD_HELPER->PowerOff();
//						theSERVICE_MGR.SetState(READY);
//						return;
//					}
//					// 选择区段超时
//					else if(GetCurDialogGroupID() == DIALOG_GROUP_SECTION){
//						FlowToDefaultDlg();
//						return;
//					}
//					// 确认交易界面超时
//					else if(GetCurDialogGroupID() == DIALOG_GROUP_INIT){
//						// 还未投钱
//						if (!m_DataModel.operationInfo.isBeginBuyProduct){
//							FlowToDefaultDlg();
//							return;
//						}
//						// 已投入钱
//						else{
//							OnCancel();
//							return;
//						}
//					}
//					else {
//						theAPP_SESSION.SetBlindManStatus(false);
//						//PINPAD_HELPER->PowerOff();
//						OnReturnFinish();
//					}					
//				}
//				// 投入、找零超限
//				else if(m_PinpadInputType == UE_PINPADKEY::UE_PINPAD_RIGHT){
//					FlowToErrorDlg();
//				}	
//				// 其他情况
//				else if(m_operation_status == STATUS_BLIND_BUSINESS_CANCEL){
//					__super::OnCancel();
//				}
//				// 投币超时
//				else if(m_DataModel.operationInfo.finishType == INSERT_OUT_OF_TIME){
//					// 判断是否投币？
//					if(m_DataModel.operationInfo.banknoteAcceptState == CPassengerForeSvc::BANKNOTE_ACCEPT_STATE_NONE && m_DataModel.operationInfo.coinAcceptState == CPassengerForeSvc::COIN_ACCEPT_STATE_NONE){
//						PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//					}
//					// 已投币
//					else{
//						//PlayEventAudio(AUDIO_COMMON_MSG_TIMEOUT_CANCEL,true);
//					}
//				}
//				else {
//					theSERVICE_MGR.SetState(READY);
//				}
//				return;
//			}
//			// 3.买票
//			else if(STATUS_BLIND_BUY_TOKEN == m_operation_status){
//				if(emGetValue == UE_PINPAD_0)
//					m_vPassengerInput.push_back(0);
//				else if(emGetValue == UE_PINPAD_1)
//					m_vPassengerInput.push_back(1);
//				else if(emGetValue == UE_PINPAD_2)
//					m_vPassengerInput.push_back(2);
//				else if(emGetValue == UE_PINPAD_3)
//					m_vPassengerInput.push_back(3);
//				else if(emGetValue == UE_PINPAD_4)
//					m_vPassengerInput.push_back(4);
//				else if(emGetValue == UE_PINPAD_5)
//					m_vPassengerInput.push_back(5);
//				else if(emGetValue == UE_PINPAD_6)
//					m_vPassengerInput.push_back(6);
//				else if(emGetValue == UE_PINPAD_7)
//					m_vPassengerInput.push_back(7);
//				else if(emGetValue == UE_PINPAD_8)
//					m_vPassengerInput.push_back(8);
//				else if(emGetValue == UE_PINPAD_9)
//					m_vPassengerInput.push_back(9);
//				else if(emGetValue == UE_PINPAD_CLEAR)
//					m_vPassengerInput.clear();
//				else if(emGetValue == UE_PINPAD_CANCLE){
//					//GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//					PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//					theAPP_SESSION.SetBlindManStatus(false);
//					//PINPAD_HELPER->PowerOff();
//					theSERVICE_MGR.SetState(READY);
//					return;
//				}
//				// 确认键
//				else if(emGetValue == UE_PINPAD_ENTER){
//					int nMenuCode = 0;
//					// 判断是否合法
//					vector<int>::iterator it = m_vPassengerInput.begin();
//					while(it != m_vPassengerInput.end()){
//						nMenuCode = nMenuCode*10 + *it;
//						it++;
//					}
//
//					// 查找输入是合法目的站点
//					map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//					if(itor != m_KeyValue.end()){
//						BTN_INFO btn;
//						btn.btnID = itor->second;
//						// 非本站
//						if(itor->second == theACC_LOCATION.GetStationLocationNumber(theMAINTENANCE_INFO.GetStationCodeWithLine()))
//							PlayEventAudio(AUDIO_COMMON_MSG_INPUT_AN_ERROR_STATION);//不能输入本站为目的站点
//						else
//							GetCurReceptionDlg()->CallWndProc(WM_XLINEGROUP,0,(LPARAM)&btn);
//					}
//					else{
//						// 输入站点错误
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//					}
//					m_vPassengerInput.clear();
//				}
//				// 输入有误，请重新输入
//				else{
//					m_vPassengerInput.clear();
//					PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//				}
//			}
//			// 4.买卡
//			else if(STATUS_BLIND_BUY_CARD == m_operation_status){
//				// 点击购卡选项后保存点击的选项
//				if (UE_PINPAD_1 == emGetValue || UE_PINPAD_2 == emGetValue || UE_PINPAD_3 == emGetValue){
//					m_PinpadInputType = emGetValue;
//				}
//				// 确认键按下，触发按钮响应
//				else if (UE_PINPAD_ENTER == emGetValue){
//					// 当前触发的是右边的第一个Group组的按钮
//					BTN_INFO btn;
//					// 储值票 SVT
//					if(UE_PINPAD_1 == m_PinpadInputType){
//						btn.btnMsg.Format(_T("%d"),SVT);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);
//					}
//					// 计次票 MST
//					else if(UE_PINPAD_2 == m_PinpadInputType){
//						btn.btnMsg.Format(_T("%d"),MST);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);
//					}
//					// 定期票 PST
//					else if(UE_PINPAD_3 == m_PinpadInputType){
//						btn.btnMsg.Format(_T("%d"),PST);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);
//					}
//					// 输入错误
//					else {
//						//CMessagePictureBox::Show(_T("提示"),_T("输入有误，请重新输入!"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
//						// 输入有误，请重新输入
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//					}
//				}
//				// 取消键按下
//				else if(UE_PINPAD_CANCLE == emGetValue){
//					//CMessagePictureBox::Show(_T("提示"),_T("交易取消。"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);	
//					PlayEventAudio(AUDIO_BUSINESS_CANCEL_NORMAL,true);
//					theAPP_SESSION.SetBlindManStatus(false);
//					//PINPAD_HELPER->PowerOff();
//					theSERVICE_MGR.SetState(READY);				
//					return;
//				}
//				// 清除键按下
//				else if(UE_PINPAD_CLEAR == emGetValue){
//					m_PinpadInputType = UE_UNKNOWN_KEY;
//				}
//				else {
//					//CMessagePictureBox::Show(_T("提示"),_T("输入有误，请重新输入!"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
//					// 输入有误，请重新输入
//					PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//				}
//			}
//			// 5.选择子产品
//			else if(STATUS_BLIND_SELECT_SUB_PRODUCT == m_operation_status){
//				if(UE_PINPAD_1 == emGetValue){
//					m_vPassengerInput.push_back(1);
//				}
//				else if(UE_PINPAD_2 == emGetValue){
//					m_vPassengerInput.push_back(2);
//				}
//				else if(UE_PINPAD_3 == emGetValue){
//					m_vPassengerInput.push_back(3);
//				}
//				else if(UE_PINPAD_4 == emGetValue){
//					m_vPassengerInput.push_back(4);
//				}
//				else if(UE_PINPAD_5 == emGetValue){
//					m_vPassengerInput.push_back(5);
//				}
//				else if(UE_PINPAD_6 == emGetValue){
//					m_vPassengerInput.push_back(6);
//				}
//				else if(UE_PINPAD_7 == emGetValue){
//					m_vPassengerInput.push_back(7);
//				}
//				else if(UE_PINPAD_8 == emGetValue){
//					m_vPassengerInput.push_back(8);
//				}
//				else if(UE_PINPAD_9 == emGetValue){
//					m_vPassengerInput.push_back(9);
//				}
//				else if(UE_PINPAD_0 == emGetValue){
//					m_vPassengerInput.push_back(0);
//				}
//				// 点击enter的时候进行下一步处理
//				else if(UE_PINPAD_ENTER == emGetValue){
//					int nMenuCode = 0;
//					// 判断是否合法
//					vector<int>::iterator it = m_vPassengerInput.begin();
//					while(it != m_vPassengerInput.end()){
//						nMenuCode = nMenuCode*10 + *it;
//						it++;
//					}
//					// 查找输入是否有当前的值
//					map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//					// 存在当前对应的ID
//					if( itor != m_KeyValue.end()){
//						BTN_INFO btn;
//						if(m_pCommonModel->productInfo.productType == SVT)
//							btn.btnMsg.Format(_T("%d"),itor->second * 100);
//						else
//							btn.btnMsg.Format(_T("%d"),itor->first);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_1_GROUP_ID,(LPARAM)&btn);
//					}
//					else{
//						// 输入有误，请重新输入
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//					}
//					// 清空输入
//					m_vPassengerInput.clear();
//					// 判断是否需要指定起始区段？
//
//					// 判断是否需要指定目的区段?
//
//				}
//				else if(UE_PINPAD_CLEAR == emGetValue){
//					m_vPassengerInput.clear();
//					m_PinpadInputType = UE_UNKNOWN_KEY;
//				}
//				// 取消按钮按下
//				else if(UE_PINPAD_CANCLE == emGetValue){
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						// 点击当前界面的取消按键
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//						return;
//					}
//				}
//				// 其他非法输入
//				else {
//					// 输入有误，请重新输入
//					PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//				}	
//			}
//			// 6.选择起始区段
//			else if(STATUS_BLIND_SELECT_BEGIN_SECTION == m_operation_status){
//				if(emGetValue == UE_PINPAD_0)
//					m_vPassengerInput.push_back(0);
//				else if(emGetValue == UE_PINPAD_1)
//					m_vPassengerInput.push_back(1);
//				else if(emGetValue == UE_PINPAD_2)
//					m_vPassengerInput.push_back(2);
//				else if(emGetValue == UE_PINPAD_3)
//					m_vPassengerInput.push_back(3);
//				else if(emGetValue == UE_PINPAD_4)
//					m_vPassengerInput.push_back(4);
//				else if(emGetValue == UE_PINPAD_5)
//					m_vPassengerInput.push_back(5);
//				else if(emGetValue == UE_PINPAD_6)
//					m_vPassengerInput.push_back(6);
//				else if(emGetValue == UE_PINPAD_7)
//					m_vPassengerInput.push_back(7);
//				else if(emGetValue == UE_PINPAD_8)
//					m_vPassengerInput.push_back(8);
//				else if(emGetValue == UE_PINPAD_9)
//					m_vPassengerInput.push_back(9);
//				else if(emGetValue == UE_PINPAD_CLEAR)
//					m_vPassengerInput.clear();
//				else if(emGetValue == UE_PINPAD_CANCLE){
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//						return;
//					}
//				}
//				// 确认键
//				else if(emGetValue == UE_PINPAD_ENTER){
//					int nMenuCode = 0;
//					// 判断是否合法
//					vector<int>::iterator it = m_vPassengerInput.begin();
//					while(it != m_vPassengerInput.end()){
//						nMenuCode = nMenuCode*10 + *it;
//						it++;
//					}
//
//					// 查找输入是否有当前区间
//					map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//					if(itor != m_KeyValue.end()){
//						BTN_INFO btn;
//						btn.btnMsg.Format(_T("%d"),itor->second);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_2_GROUP_ID,(LPARAM)&btn);
//					}
//					else{
//						// 无当前区段
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//					}
//					m_vPassengerInput.clear();
//				}
//				// 输入有误，请重新输入
//				else{
//					m_vPassengerInput.clear();
//					PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//				}
//			}
//			// 7.选择目的区段
//			else if(STATUS_BLIND_SELECT_END_SECTION == m_operation_status){
//				if(emGetValue == UE_PINPAD_0)
//					m_vPassengerInput.push_back(0);
//				else if(emGetValue == UE_PINPAD_1)
//					m_vPassengerInput.push_back(1);
//				else if(emGetValue == UE_PINPAD_2)
//					m_vPassengerInput.push_back(2);
//				else if(emGetValue == UE_PINPAD_3)
//					m_vPassengerInput.push_back(3);
//				else if(emGetValue == UE_PINPAD_4)
//					m_vPassengerInput.push_back(4);
//				else if(emGetValue == UE_PINPAD_5)
//					m_vPassengerInput.push_back(5);
//				else if(emGetValue == UE_PINPAD_6)
//					m_vPassengerInput.push_back(6);
//				else if(emGetValue == UE_PINPAD_7)
//					m_vPassengerInput.push_back(7);
//				else if(emGetValue == UE_PINPAD_8)
//					m_vPassengerInput.push_back(8);
//				else if(emGetValue == UE_PINPAD_9)
//					m_vPassengerInput.push_back(9);
//				else if(emGetValue == UE_PINPAD_CLEAR)
//					m_vPassengerInput.clear();
//				else if(emGetValue == UE_PINPAD_CANCLE){
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//						return;
//					}
//				}
//				// 确认键
//				else if(emGetValue == UE_PINPAD_ENTER){
//					int nMenuCode = 0;
//					// 判断是否合法
//					vector<int>::iterator it = m_vPassengerInput.begin();
//					while(it != m_vPassengerInput.end()){
//						nMenuCode = nMenuCode*10 + *it;
//						it++;
//					}
//					// 查找输入是否有当前区间
//					map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//					if(itor != m_KeyValue.end()){
//						BTN_INFO btn;
//						btn.btnMsg.Format(_T("%d"),itor->second);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_3_GROUP_ID,(LPARAM)&btn);
//					}
//					else{
//						// 无当前区段
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//					}
//					m_vPassengerInput.clear();
//				}
//				// 输入有误，请重新输入
//				else{
//					m_vPassengerInput.clear();
//					PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR,true);
//				}
//			}
//			// 8.选择张数（预留）
//			else if(STATUS_BLIND_SELECT_PRICE == m_operation_status){
//
//			}
//			// 9.选择支付方式
//			else if(STATUS_BLIND_SELECT_PAYMENT == m_operation_status){
//				if (emGetValue == UE_PINPAD_0)
//					m_vPassengerInput.push_back(0);
//				else if(emGetValue == UE_PINPAD_1)
//					m_vPassengerInput.push_back(1);
//				else if(emGetValue == UE_PINPAD_2)
//					m_vPassengerInput.push_back(2);
//				else if(emGetValue == UE_PINPAD_3)
//					m_vPassengerInput.push_back(3);
//				else if(emGetValue == UE_PINPAD_4)
//					m_vPassengerInput.push_back(4);
//				else if(emGetValue == UE_PINPAD_5)
//					m_vPassengerInput.push_back(5);
//				else if(emGetValue == UE_PINPAD_6)
//					m_vPassengerInput.push_back(6);
//				else if(emGetValue == UE_PINPAD_7)
//					m_vPassengerInput.push_back(7);
//				else if(emGetValue == UE_PINPAD_8)
//					m_vPassengerInput.push_back(8);
//				else if(emGetValue == UE_PINPAD_9)
//					m_vPassengerInput.push_back(9);
//				else if(emGetValue == UE_PINPAD_CLEAR)
//					m_vPassengerInput.clear();
//				else if(emGetValue == UE_PINPAD_CANCLE){
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//						return;
//					}
//				}
//				// 确认键
//				else if(emGetValue == UE_PINPAD_ENTER){
//					int nMenuCode = 0;
//					// 判断是否合法
//					vector<int>::iterator it = m_vPassengerInput.begin();
//					while(it != m_vPassengerInput.end()){
//						nMenuCode = nMenuCode*10 + *it;
//						it++;
//					}
//
//					// 查找是否有当前输入？
//					map<int,DWORD>::iterator itor = m_KeyValue.find(nMenuCode);
//					if(itor != m_KeyValue.end()){
//						BTN_INFO btn;
//						btn.btnMsg.Format(_T("%d"), itor->second);
//						GetCurReceptionDlg()->CallWndProc(WM_XGROUP,RIGHT_2_GROUP_ID,(LPARAM)&btn);
//						// 继续监控
//					}
//					else{
//						// 无当前区段，请重新输入
//						PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//					}
//					m_vPassengerInput.clear();
//				}
//				// 输入有误，请重新输入
//				else{
//					m_vPassengerInput.clear();
//					PlayEventAudio(AUDIO_COMMON_MSG_INPUT_ERROR);
//				}
//			}
//			// 10.交易确认
//			else if(STATUS_BLIND_CONFIRM_BUSINESS == m_operation_status){
//				LOG("交易确认：%d",GetLastError());
//				if(UE_PINPAD_ENTER == emGetValue){// 确认交易				
//					// 点击当前界面的确认按钮:开始收现金
//					if(GetCurReceptionDlg()->IsButtonEnable()){
//						LOG("OK按钮可以用！");
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_OK);
//						LOG("点击事件完成。");
//						if (m_pCommonModel->paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH){
//							// 投入现金
//							m_operation_status = STATUS_BLIND_INSERT_CASH;
//							TurnOnPinPadAndTimer(true,false);// 只开启键盘，不开启超时监控
//							return;
//						}
//						else {
//							// 插入银行卡
//							m_operation_status = STATUS_BLIND_INSERT_BANKCARD;
//						}
//					}					
//				}
//				else if(UE_PINPAD_CANCLE == emGetValue){ // 取消交易
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						// 点击当前界面的取消按键
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_BACK);
//						return;
//					}
//				}
//				// 其他按键，不响应
//				else {
//					// Do nothing
//				}
//			}
//			// 11.投入现金
//			else if(STATUS_BLIND_INSERT_CASH == m_operation_status){
//				// 11.1 取消交易
//				if(UE_PINPAD_CANCLE == emGetValue){
//					//CMessagePictureBox::Show(_T("提示"),_T("交易取消,请收好所退现金。"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
//					// 投入现金中取消,此时需要退钱
//					//theAPP_SESSION.SetBlindManStatus(false);
//					//PINPAD_HELPER->PowerOff();
//					//m_operation_status = STATUS_BLIND_CANCEL;
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_BACK);					
//						return;
//					}
//				}
//				// 11.2 其他都不响应
//				else{
//					// Do nothing
//					theSERVICE_MGR.SetState(COUNTING);
//					TurnOnPinPadAndTimer(true,false);
//					return;
//				}
//			}
//			// 12.支付确认
//			else if(STATUS_BLIND_CONFIRM_PAYMENT == m_operation_status){
//				// 12.1 取消交易
//				if(UE_PINPAD_CANCLE == emGetValue){
//					//CMessagePictureBox::Show(_T("提示"),_T("交易取消,请收好所退现金。"),TVM_IMAGE_BOX_PICTURE,CMessagePictureBox::BTN_CANCEL,DT_CENTER,2);
//					// 投入现金中取消,此时需要退钱
//					//PlayEventAudio(AUDIO_BUSINESS_CANCEL,true);
//					//theAPP_SESSION.SetBlindManStatus(false);
//					//PINPAD_HELPER->PowerOff();
//					//m_operation_status = STATUS_BLIND_CANCEL;
//					if(theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->IsCancleButtonEnable()){
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_BACK);					
//						return;
//					}
//				}
//				// 12.2 确认支付
//				else if(UE_PINPAD_ENTER == emGetValue){
//					if(GetCurReceptionDlg()->IsButtonEnable()){
//						// 点击屏幕上的确认按钮
//						GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_OK);
//						// 设置状态为发售中...
//						//m_operation_status = STATUS_BLIND_ISSUING;
//						// 键盘交由MainSvc托管
//						//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartManagedBlind(m_operation_status);
//						return;
//					}				
//				}
//				// 12.3 不响应
//				else {
//					// Do nothing
//				}
//			}
//			// 13.插入银行卡（暂无，后续加上）
//			else if(STATUS_BLIND_INSERT_BANKCARD == m_operation_status){
//
//			}
//			// 14.输入银行卡密码（暂无，后续加上）
//			else if(STATUS_BLIND_INPUT_PASSWORD ==  m_operation_status){
//
//			}
//			// 15.发售中...
//			else if(STATUS_BLIND_ISSUING == m_operation_status){
//				// Do nothing
//			}
//			// 16.交易成功
//			else if(STATUS_BLIND_BUSINESS_SUCCESS == m_operation_status){
//				// 由MainSvc托管
//			}
//			// 17.交易失败
//			else if(STATUS_BLIND_BUSINESS_FAILED == m_operation_status){
//				// 由MainSvc托管
//			}
//			// 18.交易取消（有硬件动作）
//			else if(STATUS_BLIND_BUSINESS_CANCEL == m_operation_status){
//				// 由MainSvc托管
//			}
//			// 19.打印完成
//			else if(STATUS_BLIND_PRINT_SUCCESS == m_operation_status){
//				// 由MainSvc托管
//			}
//			// 20.交易取消
//			else if(STATUS_BLIND_CANCEL ==  m_operation_status){
//				// 由MainSvc托管
//			}
//			// 未知状态，需要停止监控密码键盘
//			else {
//				// 暂时未处理
//				return ;
//			}
//			// 继续调用密码键盘
//			TurnOnPinPadAndTimer();
//		}
//		// 错误暂时不处理
//		else{
//			LOG("密碼鍵盤異常：%d",res);
//		}
//	}
//	catch(CSysException& e){
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch(...){
//		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      重复读取语音计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::OnTimer(UINT nEventID){
	try{
		if (nEventID == TIMERS_OUT_OF_REPLAY){
			StopTimer(nEventID);
			PlayEventAudio(m_dwLastPlayedVoice);
		}
		if(nEventID == TIMERS_OPERATION_OUT_OF_TIME){
			StopTimer(TIMERS_OUT_OF_REPLAY);
			StopTimer(nEventID);
			if(theSERVICE_MGR.GetState() == COUNTING || theSERVICE_MGR.GetState() == ISSUEING){
				return;
			}
			m_DataModel.operationInfo.finishType = RUNNING_OUT_OF_TIME;
			if(!m_bIsPinPadStop)
			{
			}
				//PINPAD_HELPER->TurnOffInputTrigger();
			else{
				//PlayEventAudio(AUDIO);
				OnReturnFinish();
			}			
		}
		if (nEventID == 1111)
		{
			StopTimer(nEventID);
			GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,1,(LPARAM)WM_BTN_OK);
		}
		// 同时调用父类计时器
		__super::OnTimer(nEventID);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动盲人购票超时计时器

@param      bool bIsStartPinPad (是否启动密码键盘)

@param      bool bIsStartOutOfTimer(是否开启操作超时计时器)

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::TurnOnPinPadAndTimer(bool bIsStartPinPad /* = true */,bool bIsStartOutOfTimer /* = true */){
	try{
		if(!theAPP_SESSION.IsBlindManOperation()) return;
		if(bIsStartPinPad){
			// 开启密码键盘监控
			//PINPAD_HELPER->TurnOnInputTrigger(this);
			// 密码键盘为开启状态
			m_bIsPinPadStop = false;
		}		
		int nTimer = theAFC_TVM_OPERATION.GetCancelDealTime() + (m_VoicePlayTime >=0 ? m_VoicePlayTime : 0);// 单位:秒
		if(nTimer == 0 || !bIsStartOutOfTimer)
			StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
		else
			StartTimer(TIMERS_OPERATION_OUT_OF_TIME,nTimer);
		
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      设置手动选择票价

@param      int nAmount;金额

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SetTicketAmount(int nAmount){
	m_DataModel.productInfo.lProductSaleAmount = nAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      保存废票回收交易或者已售车票回收交易

@param      INVALIDE_TICKET_RECYCLETRADE& inBussinessDetail

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::SaveInvalidateTicketBussiness(INVALIDE_TICKET_RECYCLETRADE& inBussinessDetail){
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(inBussinessDetail);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief		显示纸币可接受面额范围  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CIssueSvc::BanknoteShowword()
{
	CIssueSvc::Model& model = GetDataModel();
	vector<WORD> vecBanknotes;

	theAFC_TVM_OPERATION.GetAcceptBanknoteFaceOfIssueCard(vecBanknotes);

	if (vecBanknotes.size() == 0){
		return _T("");
	}
	// 充值接收的纸币面额 BIN	单位：元
	CString banknote_showword = _T("");/*_tl(TXT_RECHARGE_ACCEPTABLE_BANKNOTES)+_T("\n");*/
	int nCount = 0;
	for (vector<WORD>::iterator it = vecBanknotes.begin();it != vecBanknotes.end(); it++){
		nCount++;
		CString strTemp;
		strTemp.Format(_T("%d%s%s"),*it,_tl(MONEY_UNIT),_T(","));//10MOP,		
		banknote_showword += strTemp;
		//// 每3个换一行
		//if (nCount == 3){
		//	banknote_showword.TrimRight(_T(","));
		//	banknote_showword += _T("\n");
		//}
	}
	banknote_showword.TrimRight(_T(","));
	return banknote_showword;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		显示硬币可接受面额范围  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CIssueSvc::CoinShowword()
{
	// 判断当前模式是否支持硬币投入？
	if((theTVM_STATUS_MGR.GetPaymentMode() & DEVICE_CONTRL_CODE_PAYMENT_COIN) != DEVICE_CONTRL_CODE_PAYMENT_COIN){
		return _T("");
	}
	COIN_ACCEPTABLE coinTable = GetAcceptCoinFaceTable();

	if(!coinTable.CH_COIN_ALLOW_05 && !coinTable.CH_COIN_ALLOW_1){
		return _T("");
	}
	CString coin_showword = _T("");/*_tl(TXT_RECHARGE_ACCEPTABLE_COINS)+_T("\n");*/
	CString temp;
	int nCount = 0;
	if(coinTable.CH_COIN_ALLOW_05){
		temp.Format(_T("50%s%s"),_tl(AVOS_UNIT),_T(","));
		coin_showword += temp;
		nCount++;
	}
	if (coinTable.CH_COIN_ALLOW_1){
		temp.Format(_T("1%s%s"),_tl(MONEY_UNIT),_T(","));
		coin_showword += temp;
		nCount++;
	}
	//去掉末尾的逗号。
	coin_showword.TrimRight(_T(","));
	return coin_showword;
}


void CIssueSvc::UpdateReceptDlg(CString strTxt)						// 更新乘客界面倒计时提示
{
	GetCurReceptionDlg()->SetTipsShow(strTxt);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      保存票卡数据到DataModel

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW读卡反馈信息

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	CACCCard accCard;

	MEDIUM_INFO tmp_mediumCardInfo;
	PRODUCT_INFO tmp_ticketProduct;
	m_strReadCardRsp = _T("");

	// 解析票卡
	if(!accCard.GetCardInfo(pReadCardResponse->CardInfo)) {
		//OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
		return -1;
	}	


	// 票卡的基本信息
	tmp_mediumCardInfo.cardLogicalNumber = ComConvToHexStr(accCard.applicationData.CardLogicalNumber, 8);
	tmp_mediumCardInfo.mediumType = (CARD_MEDIUM_TYPE)pReadCardResponse->CardPHType;	//theTICKET_HELPER.GetMediumType(m_pDataModel->mediumInfo.cardPHType);
	tmp_mediumCardInfo.cardIssuer = accCard.applicationData.CardIssuerCode;
	tmp_mediumCardInfo.passengerType = accCard.applicationData.byPassengerType;		//乘客类型（暂不用）


	if(accCard.v_ProductInfo.size()==0){
		CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_MSG_GUIDE_THERE_IS_NO_PRODUCT_FOR_RECHARGE),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
		return -1;
	}
	
	//if (accCard.v_ProductInfo[0].productCategory!=PURSE){
	//	CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
	//	return -1;
	//}

	tmp_ticketProduct.productType = accCard.v_ProductInfo[0].productType;
	tmp_ticketProduct.productSubType = accCard.v_ProductInfo[0].productType;
	tmp_ticketProduct.lBalanceAmount = accCard.v_ProductInfo[0].Balance;
	tmp_ticketProduct.beginLocation = accCard.v_ProductInfo[0].dwStartStation;
	tmp_ticketProduct.endLocation = accCard.v_ProductInfo[0].dwEndStation;	


	//// 查找可充值产品
	//for (int i=0; i < pReadCardResponse->ProductCount; ++i) {
	//	if (pReadCardResponse->RechargeProduct[i].RechargeMark) {
	//		PRODUCT_INFO ticketProduct;
	//		ticketProduct.productType        = static_cast<ProductType>(pReadCardResponse->RechargeProduct[i].ProductType);
	//		ticketProduct.productSubType     = pReadCardResponse->RechargeProduct[i].ProductSubType;
	//		ticketProduct.lProductDeposit    = static_cast<long>(pReadCardResponse->RechargeProduct[i].RefoundMoney); // 可退金额==产品押金
	//		tmp_vecTicketProduct.push_back(ticketProduct);
	//	}
	//}

	//// 如果没有可充值产品
	//if (tmp_vecTicketProduct.empty()) {
	//	//OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
	//	CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(TXT_MSG_GUIDE_THERE_IS_NO_PRODUCT_FOR_RECHARGE),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
	//	return -1;
	//}

	//// 车票的详细信息
	//for (vector<PRODUCT_INFO>::iterator iter = tmp_vecTicketProduct.begin(); iter != tmp_vecTicketProduct.end(); ++iter) {
	//	CACCCard::ACCCARD_PRODUCT_INFO productInfo;
	//	if (!accCard.GetProductInfo(iter->productType, iter->productSubType, productInfo)) {
	//		//OnError(_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT), GUIDE_CHARGE_TPU_ERROR_CANNOT);
	//		CMessagePictureBox::Show(_tl(TXT_CHARGE_INFO),_tl(GUIDE_CHARGE_TPU_ERROR_CANNOT),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
	//		return -1;
	//	}
	//	iter->productCategory = productInfo.productCategory;				// 产品种类
	//	iter->dtValidateDateTime = productInfo.ValidEndTime;				// 产品有效期结束时间
	//	if (iter->productCategory == PURSE) {
	//		iter->lBalanceAmount = productInfo.Balance;						// 车票余额
	//	}
	//	else if (iter->productCategory == MULTIRIDE) {
	//		iter->lBalanceRides = productInfo.Balance;						// 车票余次
	//	}
	//	iter->beginLocation = productInfo.dwStartStation;					// 区段起点
	//	iter->endLocation = productInfo.dwEndStation;						// 区段终点
	//	iter->useArea	  = productInfo.wArea;								// 使用区段		
	//}

	m_DataModel.mediumCardInfo = tmp_mediumCardInfo;
	m_DataModel.productCardInfo = tmp_ticketProduct;

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      保存票卡数据到DataModel

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW读卡反馈信息

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CIssueSvc::EmptyCardInfo()
{
	m_DataModel.mediumCardInfo = MEDIUM_INFO();
	m_DataModel.productCardInfo = PRODUCT_INFO();
	//m_strReadCardRsp = _T("");
	return 0;
}


DWORD WINAPI CIssueSvc::IssueTicketWorker(LPVOID lpParameter)
{
	CIssueSvc* pSvc = (CIssueSvc*)lpParameter;
	pSvc->StartIssueAndChange();
	return 0;
}


long CIssueSvc::StartIssueThread()
{
	DWORD dwThreadID;
	HANDLE m_hHandle = CreateThread(NULL,0,IssueTicketWorker,this,0,&dwThreadID);

	if (m_hHandle == NULL){
		return -1;
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      开始收钱

@param      无

@retval     void

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CIssueSvc::StartReceiveMoney()
{
	m_DataModel.paymentInfo.paymentMethod = PAYMENT_METHOD_CASH;

// 	m_pDialogFlow->GetDialog(CIssueWithCashDlg::IDD)->Invalidate();
	NotifyDataModelChanged();
	m_pDialogFlow->GetDialog(CIssueWithCashDlg::IDD)->PostMessage(WM_START_ISSUING,0,0);

}