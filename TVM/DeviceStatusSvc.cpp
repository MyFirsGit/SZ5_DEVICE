#include "stdafx.h"
#include "DeviceStatusSvc.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  INVALID_ID		_T("FFFFFFFF")
BEGIN_MESSAGE_MAP(CDeviceStatusSvc,CTVMForeService)
	

END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusSvc::CDeviceStatusSvc()
:CTVMForeService(DEVICE_STATUS_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG,new CDeviceStatusDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET,_opl(GUIDE_F3_F6_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DEVICESTATUS_SET,IDD_20112_STATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET);		// 设置初始画面组
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusSvc::~CDeviceStatusSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::OnStart()
{
	m_DataModel.cashboxstatus.timesstatus = 1;
    m_bIsShowNearlyFullStatus = theFunction_INFO.IsShowNearlyFullEmptyParaValue();
	int DeviceType = theMAINTENANCE_INFO.GetDeviceType();						// 获取设备类型
    m_bIsAutoInputRFID = theFunction_INFO.IsAutoReadRfidWithoutManual();
	if(DeviceType == DEVICE_TYPE_TVM)
	{
        int index = 3;
		//// 同步硬币模块计数
		//COIN_HELPER->SyncCoinCountInfoToFile();
		// 读其他状态
		ReadCashBoxID();														// 读取钱箱、票箱ID
		ReadCashBoxData();														// 读取钱箱、票箱剩余枚数/张数
		ReadEmptyFull();														// 读取钱箱、票箱将空、将满、已空、已满枚数/张数
		ReadStatus();															// 判断钱箱、票箱状态
		if(!m_bIsShowNearlyFullStatus)index--;
		SetPageInfo(index,m_DataModel.cashboxstatus.timesstatus,12);				// 实现翻页
	}
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET);			// 设置初始画面组
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_DEVICESTATUS_SET);
	theAPP_SESSION.SetMenuLevel(DEVICE_STATUS_SVC);
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      读取钱箱、票箱ID

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadCashBoxID()
{
 	long errCode = 0;
 	CString Coin_Box_ID;
 	CString Token_Box_AID = _T("");
 	CString Token_Box_BID = _T("");
 	
	// ****** 1. 硬币
	CTCoinCountInfo::COIN_BOX_INFO coinHopperBoxAInfo =  theCOIN_COUNT.GetCoinHopperboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO coinHopperBoxBInfo =  theCOIN_COUNT.GetCoinHopperboxBInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coinCollectBoxInfo =  theCOIN_COUNT.GetCollectionCoinboxAInfo();

	m_DataModel.cashboxdata.CycleChangeCoinAID = coinHopperBoxAInfo.coinBoxID.ToString();
	m_DataModel.cashboxdata.CycleChangeCoinBID = coinHopperBoxBInfo.coinBoxID.ToString();
	m_DataModel.cashboxdata.CoinCollectionAID = coinCollectBoxInfo.coinBoxID.ToString();
	// 判断硬币回收箱的状态
 	try
 	{
	//	CH_SPEC1_RSP status;
	//	COIN_HELPER->CH_GetStatus(&status);	
 		 //检测硬币回收箱是否安装到位
		if (coinCollectBoxInfo.coinBoxID.ToString().Compare(INVALID_ID) != 0){ //软计数 已安装	
		//	if (status.bIsMoneyBoxExist)
			{
				// 读取硬件ID
				CString boxID = COIN_HELPER->CH_ReadRfidData(3);
				if (boxID == coinCollectBoxInfo.coinBoxID.ToString()){ 
					// 如果两个ID相同 正常安装
					theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					// 硬件ID与软计数不相同 非法安装
					m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
					theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				}
			}
			//else
			{
				m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{
			//if (status.bIsMoneyBoxExist)
			{
				m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			//else
			{
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}
 	}
 	 //异常处理
 	catch (CSysException& e) {
 		m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		m_DataModel.cashboxdata.CoinCollectionBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		theEXCEPTION_MGR.ProcessException(e);
 	}
 	catch (...) {
 		m_DataModel.cashboxdata.CoinCollectionAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		m_DataModel.cashboxdata.CoinCollectionBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
 		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
 	}
 	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 	 //纸币回收箱ID -- 软计数

	//m_DataModel.cashboxdata.BanknoteCollectionID = theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString();	// 纸币回收箱
 	
 	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 纸币找零箱 AB
	DEVICE_STATUS_INFO box_status;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO box_A = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	/*CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_B = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO box_C = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();*/
	try
	{
		bool bIsCahsChangeBoxAttched = BANKNOTE_HELPER->IsCashChangeBoxAttached();
		// 纸币找零箱
		if (bIsCahsChangeBoxAttched){
			// 如果软计数 为已安装
			if(box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
				// 如果硬件在位
				m_DataModel.cashboxdata.BanknoteChangeBoxID = box_A.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
			else{
				// 非法安装
				m_DataModel.cashboxdata.BanknoteChangeBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
		}
		else{
		// 如果硬件不在位
			// 如果软计数  已安装 卸载
			if (box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
				m_DataModel.cashboxdata.BanknoteChangeBoxID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
			else{
			// 正常卸载 显示ffffffff
				m_DataModel.cashboxdata.BanknoteChangeBoxID = box_A.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}

		//BH_CHANGE_HELPER->GetStatus(box_status);
		// 纸币找零箱A
		// 如果硬件在位
		//if (box_status.CassetteStatus.isFirstBoxExist){
		//	// 如果软计数 为已安装
		//	if(box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		// 如果硬件在位
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = box_A.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
		//	}
		//	else{
		//		// 非法安装
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
		//	}
		//}
		//else{
		//// 如果硬件不在位
		//	// 如果软计数  已安装 卸载
		//	if (box_A.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
		//	}
		//	else{
		//	// 正常卸载 显示ffffffff
		//		m_DataModel.cashboxdata.BanknoteChangeBoxAID = box_A.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
		//	}
		//}


		//// 纸币找零箱B
		//if (box_status.CassetteStatus.isSecondBoxExist){
		//	// 如果软计数 为已安装
		//	if(box_B.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		// 如果硬件在位
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = box_B.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
		//	}
		//	else{
		//		// 非法安装
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
		//	}
		//}
		//else{
		//	// 如果硬件不在位
		//	// 如果软计数  已安装 卸载
		//	if (box_B.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
		//	}
		//	else{
		//		// 正常卸载 显示ffffffff
		//		m_DataModel.cashboxdata.BanknoteChangeBoxBID = box_B.banknoteBoxID.ToString();
		//		theTVM_STATUS_MGR.SetBanknoteChangeBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
		//	}
		//}

		// 废币回收箱
		//if (box_status.SensorStatus.isRejectBoxAvailable){
		//	// 如果软计数 为已安装
		//	if(box_C.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		// 如果硬件在位
		//		m_DataModel.cashboxdata.BanknoteInvalidID = box_C.banknoteBoxID.ToString();
		//	}
		//	else{
		//		// 非法安装
		//		m_DataModel.cashboxdata.BanknoteInvalidID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
		//	}
		//}
		//else{
		//	// 如果硬件不在位
		//	// 如果软计数  已安装 卸载
		//	if (box_B.banknoteBoxID.ToString().Compare(INVALID_ID) != 0){
		//		m_DataModel.cashboxdata.BanknoteInvalidID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
		//	}else{
		//		// 正常卸载 显示ffffffff
		//		m_DataModel.cashboxdata.BanknoteInvalidID = box_C.banknoteBoxID.ToString();
		//	}
		//}
	}
	catch(...)
	{
		m_DataModel.cashboxdata.BanknoteChangeBoxID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		//m_DataModel.cashboxdata.BanknoteChangeBoxBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));

	}
	

 	try
 	{
		// CARD模块
		TH_RSP_GET_DEV_STATUS status;
		tTIMRFIDInfo rfidInfo;
		bool isBoxAExist = false;
		bool isBoxBExist = false;
		bool isBoxCExist = false;

		CTCardCountInfo::CARD_BOX_ID boxID;
		CTCardCountInfo::CARD_BOX_INFO card_boxA_info = theCARD_COUNT.GetCardboxAInfo();

		// 读取硬件状态
		CARD_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist ,isBoxCExist);
		if (card_boxA_info.ticketBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
		{
			
			if (isBoxAExist){
				// 读取硬件ID
				bool rst = CARD_HELPER->GetStaticBoxId(TH_BOX_A,rfidInfo);
				boxID.Deserialize(rfidInfo.ucBoxID);
			
				if (boxID == card_boxA_info.ticketBoxID){ 
					// 如果两个ID相同 正常安装
					m_DataModel.cashboxdata.CardBoxAID = card_boxA_info.ticketBoxID.ToString();
					theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					// 硬件ID与软计数不相同 非法安装
					m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
					theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				}
			}
			else{
				// 非法卸载
				m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{ // 软计数 卸载
			//CARD_HELPER->CheckTicketBoxExist(status, isBoxAExist, isBoxBExist ,isBoxCExist);
			if (isBoxAExist){
				// 读取硬件ID
				m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{ // 箱子不在位
				// 正常卸载
				m_DataModel.cashboxdata.CardBoxAID = INVALID_ID;
				theTVM_STATUS_MGR.SetCardBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}

		//SC_RES_READ_RFID RFID;
		//RFID.boxType = RFID_MAGAZINE_B;
		boxID = CTCardCountInfo::CARD_BOX_ID();

		// 票箱B
		CTCardCountInfo::CARD_BOX_INFO card_boxB_info = theCARD_COUNT.GetCardboxBInfo();
		//boxID.Deserialize(RF_ID.RFIDData.byDeviceID);
		if (card_boxB_info.ticketBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
		{
			// 读取硬件状态
			if (isBoxBExist){
				// 读取硬件ID
				bool result = CARD_HELPER->GetStaticBoxId(TH_BOX_B,rfidInfo);
				if (result)
				{
					boxID.Deserialize(rfidInfo.ucBoxID);
				}
				
				if (boxID == card_boxB_info.ticketBoxID){ 
					// 如果两个ID相同 正常安装
					m_DataModel.cashboxdata.CardBoxBID = card_boxB_info.ticketBoxID.ToString();
					theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				}
				else{
					// 硬件ID与软计数不相同 非法安装
					m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
					theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
				}
			}
			else{
				// 非法卸载
				m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{ // 软计数 卸载
			if (isBoxAExist){
				// 读取硬件ID
				m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{ // 箱子不在位
				// 正常卸载
				m_DataModel.cashboxdata.CardBoxBID = INVALID_ID;
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}



		//废票箱
		//SC_RES_READ_RFID RF_ID_C;
		//RF_ID.boxType = RFID_MAGAZINE_C;
		CTCardCountInfo::CARD_BOX_INFO card_boxC_info = theCARD_COUNT.GetCardboxWastedInfo();
		boxID = CTCardCountInfo::CARD_BOX_ID();
		if (card_boxC_info.ticketBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
		{
			// 读取硬件状态
			if (isBoxCExist){
				// 读取硬件ID
				bool rst = CARD_HELPER->GetStaticBoxId(TH_BOX_C,rfidInfo);
				boxID.Deserialize(rfidInfo.ucBoxID);
				//if (boxID == card_boxC_info.ticketBoxID){ 
					// 如果两个ID相同 正常安装
					m_DataModel.cashboxdata.INVALIDCardID = card_boxC_info.ticketBoxID.ToString();
					theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
				//}else{
				//	// 硬件ID与软计数不相同 非法安装
				//	m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				//}
			}
			else{
				// 非法卸载
				m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}
		else{ // 软计数 卸载
			if (isBoxAExist){
				// 读取硬件ID
				m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetCardWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{ // 箱子不在位
				// 正常卸载
				m_DataModel.cashboxdata.INVALIDCardID = INVALID_ID;
				theTVM_STATUS_MGR.SetCardBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
		}

	}
	catch(...)
	{
		m_DataModel.cashboxdata.CardBoxAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		m_DataModel.cashboxdata.CardBoxBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		m_DataModel.cashboxdata.INVALIDCardID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}


	//try
	//{
	//	// TOKEN模块
	//	bool isBoxAExist = false;
	//	bool isBoxBExist = false;
	//	bool isBoxCExist = false;
	//	bool isBoxDExist = false;
	//	CTTokenCountInfo::TOKEN_BOX_ID boxID;
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_boxA_info = theTOKEN_COUNT.GetTokenboxAInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_boxB_info = theTOKEN_COUNT.GetTokenboxBInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_hopperA_info = theTOKEN_COUNT.GetHopperAInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO token_hopperB_info = theTOKEN_COUNT.GetHopperBInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO invalidTokenBoxInfo = theTOKEN_COUNT.GetTokenboxWastedInfo();
	//	CTTokenCountInfo::TOKEN_BOX_INFO tokenCollected = theTOKEN_COUNT.GetTokenboxCollectedInfo();

	//	m_DataModel.cashboxdata.TokenHopperAID	= token_hopperA_info.tokenBoxID.ToString();
	//	m_DataModel.cashboxdata.TokenHopperBID	= token_hopperB_info.tokenBoxID.ToString();

	//	TOKEN_RSP_GET_DEV_STATUS status;
	//	TOKEN_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist, isBoxCExist, isBoxDExist);
	//	// TOKEN箱A
	//	if (token_boxA_info.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
	//	{
	//		if (isBoxAExist){
	//			// 读取硬件ID
	//			CString boxID = TOKEN_HELPER->ReadTokenBoxARFID();
	//			if (boxID == token_boxA_info.tokenBoxID.ToString()){ 
	//				// 如果两个ID相同 正常安装
	//				m_DataModel.cashboxdata.TokenBoxAID = token_boxA_info.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}
	//			else{
	//				// 硬件ID与软计数不相同 非法安装
	//				m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// 非法卸载
	//			m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // 软计数 卸载
	//		if (isBoxAExist){
	//			// 读取硬件ID
	//			m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // 箱子不在位
	//			// 正常卸载
	//			m_DataModel.cashboxdata.TokenBoxAID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}


	//	// TOKEN箱B
	//	if (token_boxB_info.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
	//	{
	//		// 读取硬件状态
	//		if (isBoxBExist){
	//			// 读取硬件ID
	//			CString boxID = TOKEN_HELPER->ReadTokenBoxBRFID();
	//			if (boxID == token_boxB_info.tokenBoxID.ToString()){ 
	//				// 如果两个ID相同 正常安装
	//				m_DataModel.cashboxdata.TokenBoxBID = token_boxB_info.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}
	//			else{
	//				// 硬件ID与软计数不相同 非法安装
	//				m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// 非法卸载
	//			m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // 软计数 卸载
	//		if (isBoxBExist){
	//			// 读取硬件ID
	//			m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // 箱子不在位
	//			// 正常卸载
	//			m_DataModel.cashboxdata.TokenBoxBID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}



	//	//废票箱
	//	if (invalidTokenBoxInfo.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
	//	{
	//		// 读取硬件状态
	//		if (isBoxCExist){
	//			// 读取硬件ID
	//			CString boxID = TOKEN_HELPER->ReadInvalidBoxRFID();
	//			if (boxID == invalidTokenBoxInfo.tokenBoxID.ToString()){ 
	//				// 如果两个ID相同 正常安装
	//				m_DataModel.cashboxdata.InvalidTokenBoxID = invalidTokenBoxInfo.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}else{
	//				// 硬件ID与软计数不相同 非法安装
	//				m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// 非法卸载
	//			m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // 软计数 卸载
	//		if (isBoxCExist){
	//			// 读取硬件ID
	//			m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // 箱子不在位
	//			// 正常卸载
	//			m_DataModel.cashboxdata.InvalidTokenBoxID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}

	//	//回收箱
	//	if (tokenCollected.tokenBoxID.ToString().Compare(INVALID_ID) != 0) //软计数 已安装
	//	{
	//		// 读取硬件状态
	//		if (isBoxDExist){
	//			// 读取硬件ID
	//			CString boxID = TOKEN_HELPER->ReadTokenBoxCollectRFID();
	//			if (boxID == tokenCollected.tokenBoxID.ToString()){ 
	//				// 如果两个ID相同 正常安装
	//				m_DataModel.cashboxdata.CollectedTokenBoxID = tokenCollected.tokenBoxID.ToString();
	//				theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
	//			}else{
	//				// 硬件ID与软计数不相同 非法安装
	//				m_DataModel.cashboxdata.CollectedTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//				theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//			}
	//		}
	//		else{
	//			// 非法卸载
	//			m_DataModel.cashboxdata.CollectedTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
	//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
	//		}
	//	}
	//	else{ // 软计数 卸载
	//		if (isBoxDExist){
	//			// 读取硬件ID
	//			m_DataModel.cashboxdata.CollectedTokenBoxID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
	//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
	//		}
	//		else{ // 箱子不在位
	//			// 正常卸载
	//			m_DataModel.cashboxdata.CollectedTokenBoxID = INVALID_ID;
	//			theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//		}
	//	}

	//}
	//catch(...)
	//{
	//	m_DataModel.cashboxdata.TokenBoxAID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
	//	m_DataModel.cashboxdata.TokenBoxBID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
	//	m_DataModel.cashboxdata.InvalidTokenBoxID = _opl(DEVICESTATUS_COMMUNICATION_EXCEPTION);
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}


	//纸币回收箱
	try{
		bool bIsBanknoteInstall = BANKNOTE_HELPER->IsCashBoxAttached();

		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		if(bIsBanknoteInstall){
			// 非法安装
			if(banknoteInfo.banknoteBoxID.ToString().Compare(INVALID_ID) == 0){
				m_DataModel.cashboxdata.BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_INVALIDE_ATACH);
			}
			else{
				m_DataModel.cashboxdata.BanknoteCollectionID = banknoteInfo.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			}
		}
		else{
			// 非法卸载
			if(banknoteInfo.banknoteBoxID.ToString().Compare(INVALID_ID) == 0){
				m_DataModel.cashboxdata.BanknoteCollectionID = banknoteInfo.banknoteBoxID.ToString();
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			}
			else{
				m_DataModel.cashboxdata.BanknoteCollectionID = _opl(DEVICESTATUS_ILLEGAL_DETACH);
				theTVM_STATUS_MGR.SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNREADY);
			}
		}


		// 纸币循环鼓
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_A_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().banknoteBoxID.ToString();
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_B_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().banknoteBoxID.ToString();
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_C_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().banknoteBoxID.ToString();
		m_DataModel.cashboxdata.BNR_RECYCLER_BOX_D_ID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().banknoteBoxID.ToString();

	}
	catch(CSysException& e){

	}
	catch(...){

	}
	
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      读取钱箱、票箱剩余枚数/张数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadCashBoxData()
{
	/*
	  
	硬币循环找零箱	         				编号: 99FF9999          剩余数量: 999      
	硬币找零箱A        			     		编号: 99FF9999          剩余数量: 999            
	硬币找零箱B                        		编号: 99FF9999          剩余数量: 999            
	硬币回收箱A	 		     				编号: 99FF9999          剩余数量: 999
	硬币回收箱B	  							编号: 99FF9999          剩余数量: 999
	
	纸币回收箱	　　　    					编号: 99FF9999          剩余数量: 999    
	纸币找零箱        						编号: 99FF9999          剩余数量: 999
	纸币循环鼓A								编号：99FF9999          剩余数量：999
	纸币循环鼓B								编号：99FF9999          剩余数量：999
	纸币循环鼓C								编号：99FF9999          剩余数量：999
	纸币循环鼓D								编号：99FF9999          剩余数量：999

	//纸币找零箱B       						编号: 99FF9999          剩余数量: 999
	//废币回收箱								编号: 99FF9999          剩余数量: 999
	票箱A        							编号: 99FF9999          剩余数量: 999  
	票箱B									编号: 99FF9999          剩余数量: 999
	废票箱	 								编号: 99FF9999          剩余数量: 999
	*/

	LONG count = -1;
	count = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;						// 硬币循环找零A箱
	m_DataModel.cashboxdata.CycleChangeCoinAData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;						// 硬币循环找零B箱
	m_DataModel.cashboxdata.CycleChangeCoinBData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;							// 硬币找零箱A
	m_DataModel.cashboxdata.DedicatedChangeCoinAData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;							// 硬币找零箱B
	m_DataModel.cashboxdata.DedicatedChangeCoinBData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetCollectionCoinboxAInfo().GetCurCount();					// 硬币回收箱A
	m_DataModel.cashboxdata.CoinCollectionAData.Format(_T("%ld"),count);

	count = theCOIN_COUNT.GetCollectionCoinboxBInfo().GetCurCount();					// 硬币回收箱B
	m_DataModel.cashboxdata.CoinCollectionBData.Format(_T("%ld"),count);

	count = theBANKNOTE_COUNT.GetBanknoteboxInfo().BankntoeTotalCount();			// 纸币回收箱
	m_DataModel.cashboxdata.BanknoteCollectionData.Format(_T("%ld"),count);

	count = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().ulCurCount;				// 纸币找零箱A
	m_DataModel.cashboxdata.BanknoteChangeData.Format(_T("%ld"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxA.Format(_T("%d"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxB.Format(_T("%d"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxC.Format(_T("%d"),count);

	count = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount;
	m_DataModel.cashboxdata.BNR_RecyclerBoxD.Format(_T("%d"),count);


	//count = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().ulCurCount;				// 纸币找零箱B
	//m_DataModel.cashboxdata.BanknoteChangeBData.Format(_T("%ld"),count);

	//count = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo().ulCurCount;				// 纸币废币箱
	//m_DataModel.cashboxdata.BanknoteInvalidData.Format(_T("%ld"),count);

	count = theCARD_COUNT.GetCardboxAInfo().ulCurCount;								// 票箱A数据
	m_DataModel.cashboxdata.CardBoxAData.Format(_T("%ld"),count);

	count = theCARD_COUNT.GetCardboxBInfo().ulCurCount;								// 票箱B数据
	m_DataModel.cashboxdata.CardBoxBData.Format(_T("%ld"),count);

	count = theCARD_COUNT.GetCardboxWastedInfo().ulCurCount;						// 废票箱
	m_DataModel.cashboxdata.InvalidCardData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxAInfo().ulCurCount;							// TOKEN箱A数据
	//m_DataModel.cashboxdata.TokenBoxAData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxAInfo().ulCurCount;							// TOKEN箱B数据
	//m_DataModel.cashboxdata.TokenBoxBData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetHopperAInfo().ulCurCount;								// TOKEN箱hopperA数据
	//m_DataModel.cashboxdata.TokenHopperAData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetHopperBInfo().ulCurCount;								// TOKEN箱hopperA数据
	//m_DataModel.cashboxdata.TokenHopperBData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxWastedInfo().ulCurCount;						// 废票箱
	//m_DataModel.cashboxdata.InvalidTokenData.Format(_T("%ld"),count);

	//count = theTOKEN_COUNT.GetTokenboxCollectedInfo().ulCurCount;						// 回收箱
	//m_DataModel.cashboxdata.CollectedTokenData.Format(_T("%ld"),count);
	
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      读取钱箱、票箱将空、将满、已空、已满枚数/张数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadEmptyFull()
{
	CAfcTVMOperationParam::MAGAZINE_PARAM magazine_param;
	theAFC_TVM_OPERATION.GetMagazineParam(magazine_param);

	CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
	theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);

 	m_DataModel.emptyfull.CardMNearlyEmpty.Format(_T("%d"),magazine_param.TicketNearEmptyValue);							// 票箱将空张数
 	m_DataModel.emptyfull.CardMEmpty.Format(_T("%d"),magazine_param.TicketEmptyValue);										// 票箱已空张数
 	m_DataModel.emptyfull.INVALIDCardMNearlyFull.Format(_T("%d"),magazine_param.TicketInvalidNearFullValue);				// 废票箱将满
 	m_DataModel.emptyfull.INVALIDCardMFull.Format(_T("%d"),magazine_param.TicketInvalidFullValue);							// 废票箱已满

	//m_DataModel.emptyfull.TokenANearlyEmpty.Format(_T("%d"),magazine_param.TicketNearEmptyValue);							// 票箱将空张数
	//m_DataModel.emptyfull.TokenAEmpty.Format(_T("%d"),magazine_param.TicketEmptyValue);										// 票箱已空张数
	//m_DataModel.emptyfull.TokenBNearlyEmpty.Format(_T("%d"),magazine_param.TicketNearEmptyValue);							// 票箱将空张数
	//m_DataModel.emptyfull.TokenBEmpty.Format(_T("%d"),magazine_param.TicketEmptyValue);										// 票箱已空张数
	//m_DataModel.emptyfull.InvalidTokenNearlyFull.Format(_T("%d"),magazine_param.TicketInvalidNearFullValue);				// 废票箱将满
	//m_DataModel.emptyfull.InvalidTokenFull.Format(_T("%d"),magazine_param.TicketInvalidFullValue);							// 废票箱已满
	//m_DataModel.emptyfull.CollectedTokenNearlyFull.Format(_T("%d"),800);													// 回收箱将满
	//m_DataModel.emptyfull.CollectedTokenFull.Format(_T("%d"),1000);															// 回收箱已满

 	m_DataModel.emptyfull.DedicatedChangeCoinANearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);		// 硬币找零箱将空
 	m_DataModel.emptyfull.DedicatedChangeCoinAEmpty.Format(_T("%d"),theTVM_INFO.GetCHChangeBoxAEmpty());														//  
 	m_DataModel.emptyfull.DedicatedChangeCoinBNearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);		// 硬币找零箱已空
 	m_DataModel.emptyfull.DedicatedChangeCoinBEmpty.Format(_T("%d"),theTVM_INFO.GetCHChangeBoxBEmpty());														//  
	try
	{
		m_DataModel.emptyfull.CycleChangeCoinANearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);				// 硬币循环找零箱将空枚数 lichao
		m_DataModel.emptyfull.CycleChangeCoinBNearlyEmpty.Format(_T("%d"),billbox_param.coinChangeWillEmptyCoinCnt);					// 硬币循环找零箱将空枚数

	}
	catch(...){
		m_DataModel.emptyfull.CycleChangeCoinANearlyEmpty.Format(_T("%d"),0);
		m_DataModel.emptyfull.CycleChangeCoinBNearlyEmpty.Format(_T("%d"),0);
	}

	
 	m_DataModel.emptyfull.CycleChangeCoinAEmpty.Format(_T("%d"),0);	// 硬币循环找零A箱已空枚数
	m_DataModel.emptyfull.CycleChangeCoinBEmpty.Format(_T("%d"),0);	// 硬币循环找零B箱已空枚数

	

 	m_DataModel.emptyfull.CoinCollectionANearlyFull.Format(_T("%d"),billbox_param.coinCollectionWillFullCoinCnt);			// 硬币回收箱A将满枚数
 	m_DataModel.emptyfull.CoinCollectionAFull.Format(_T("%d"),billbox_param.coinCollectionAlreadyFullCoinCnt);				// 硬币回收箱A已满枚数
 	m_DataModel.emptyfull.CoinCollectionBNearlyFull.Format(_T("%d"),billbox_param.coinCollectionWillFullCoinCnt);			// 硬币回收箱B将满枚数
 	m_DataModel.emptyfull.CoinCollectionBFull.Format(_T("%d"),billbox_param.coinCollectionAlreadyFullCoinCnt);				// 硬币回收箱B已满枚数

 	m_DataModel.emptyfull.BanknoteCollectionNearlyFulll.Format(_T("%d"),billbox_param.willFullBillCnt);						// 纸币回收箱将满张数
 	m_DataModel.emptyfull.BanknoteCollectionFull.Format(_T("%d"),billbox_param.alreadyFullBillCnt);							// 纸币回收箱已满张数
 
 	m_DataModel.emptyfull.BanknoteChangeNearlyEmpty.Format(_T("%d"),billbox_param.willEmptyBillBoxCnt);						// 纸币找零箱将空张数
 	m_DataModel.emptyfull.BanknoteChangeEmpty.Format(_T("%d"),0);															// 纸币找零箱已空张数

	try{

		map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> mapItems = BANKNOTE_HELPER->GetEmptyFull();

		map<BNR_MODULE_ID,T_XfsPhysicalCashUnit>::iterator it = mapItems.find(BNR_RECYCLER_BOX_A);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}

		it = mapItems.find(BNR_RECYCLER_BOX_B);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}

		it = mapItems.find(BNR_RECYCLER_BOX_C);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}

		it = mapItems.find(BNR_RECYCLER_BOX_D);
		if(it != mapItems.end()){
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_Full.Format(_T("%d"),it->second.threshold.full);
			m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_NearlyFull.Format(_T("%d"),it->second.threshold.high);
		}
	}
	catch(...){

	}
	

	//m_DataModel.emptyfull.BanknoteInvalidNearlyFull.Format(_T("%d"),theTVM_INFO.GetBHChangeBoxNearFull());					// 废币回收箱将满
	//m_DataModel.emptyfull.BanknoteInvalidFull.Format(_T("%d"),theTVM_INFO.GetBHChangeBoxFull());							// 废币回收箱已满


	// 废币回收箱 将满 已满数量

}

//////////////////////////////////////////////////////////////////////////
/**
@brief     通知数据模型更新

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     cashboxdata

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CDeviceStatusSvc::Model& CDeviceStatusSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断钱箱、票箱状态

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CDeviceStatusSvc::ReadStatus()
{
	// 硬币循环找零箱
	m_DataModel.cashboxstatus.CCyleChangeAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CycleChangeCoinAData,m_DataModel.emptyfull.CycleChangeCoinANearlyEmpty,m_DataModel.emptyfull.CycleChangeCoinAEmpty);					// 硬币循环找零箱
	m_DataModel.cashboxstatus.CCyleChangeBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CycleChangeCoinBData,m_DataModel.emptyfull.CycleChangeCoinBNearlyEmpty,m_DataModel.emptyfull.CycleChangeCoinBEmpty);					// 硬币循环找零箱

	// 硬币找零箱
	m_DataModel.cashboxstatus.CDChangeAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.DedicatedChangeCoinAData,m_DataModel.emptyfull.DedicatedChangeCoinANearlyEmpty,m_DataModel.emptyfull.DedicatedChangeCoinAEmpty);		// 硬币专用找零箱A
	m_DataModel.cashboxstatus.CDChangeBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.DedicatedChangeCoinBData,m_DataModel.emptyfull.DedicatedChangeCoinBNearlyEmpty,m_DataModel.emptyfull.DedicatedChangeCoinBEmpty);		// 硬币专用找零箱B
	
	// 硬币回收箱
	m_DataModel.cashboxstatus.CCollectionAStatus = StatusChangeFull(m_DataModel.cashboxdata.CoinCollectionAData,m_DataModel.emptyfull.CoinCollectionANearlyFull,m_DataModel.emptyfull.CoinCollectionAFull);						// 硬币回收箱A
	m_DataModel.cashboxstatus.CCollectionBStatus = StatusChangeFull(m_DataModel.cashboxdata.CoinCollectionBData,m_DataModel.emptyfull.CoinCollectionBNearlyFull,m_DataModel.emptyfull.CoinCollectionBFull);						// 硬币回收箱B
	// 纸币回收箱
	m_DataModel.cashboxstatus.BanknoteStatus = StatusChangeFull(m_DataModel.cashboxdata.BanknoteCollectionData,m_DataModel.emptyfull.BanknoteCollectionNearlyFulll,m_DataModel.emptyfull.BanknoteCollectionFull);				// 纸币回收箱
	// 纸币找零箱
	m_DataModel.cashboxstatus.BanknoteChangeStatus = StatusChangeEmpty(m_DataModel.cashboxdata.BanknoteChangeData,m_DataModel.emptyfull.BanknoteChangeNearlyEmpty,m_DataModel.emptyfull.BanknoteChangeEmpty);					// 纸币找零箱	
	//m_DataModel.cashboxstatus.BanknoteChangeBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.BanknoteChangeBData,m_DataModel.emptyfull.BanknoteChangeNearlyEmpty,m_DataModel.emptyfull.BanknoteChangeEmpty);					// 纸币找零箱	
	// 废币回收箱的将满已满张数
	//m_DataModel.cashboxstatus.BanknoteInvalidStatus = StatusChangeFull(m_DataModel.cashboxdata.BanknoteInvalidData,m_DataModel.emptyfull.BanknoteInvalidNearlyFull,m_DataModel.emptyfull.BanknoteInvalidFull);						// 废币回收箱
	// 循环鼓
	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxA,m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_A_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_A_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}

	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxB,m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_B_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_B_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}

	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxC,m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_C_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_C_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}

	m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS = StatusChangeFull(m_DataModel.cashboxdata.BNR_RecyclerBoxD,m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_NearlyFull,m_DataModel.emptyfull.BNR_RECYCLER_BOX_D_Full);
	if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS == _opl(DEVICESTATUS_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS == _opl(DEVICESTATUS_NEARLY_FULL_TXT)){
		theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NEAR_FULL);
	}
	else if (m_DataModel.cashboxstatus.BNR_RECYCLER_BOX_D_STATUS == _opl(DEVICESTATUS_NORMAL_SET)){
		theTVM_STATUS_MGR.SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
	}


	// 票箱
	m_DataModel.cashboxstatus.CardBoxAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CardBoxAData,m_DataModel.emptyfull.CardMNearlyEmpty,m_DataModel.emptyfull.CardMEmpty);													// Smart-card箱
	m_DataModel.cashboxstatus.CardBoxBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.CardBoxBData,m_DataModel.emptyfull.CardMNearlyEmpty,m_DataModel.emptyfull.CardMEmpty);													// Smart-card箱
	// 废票箱
	m_DataModel.cashboxstatus.WastedSmartcardStatus = StatusChangeFull(m_DataModel.cashboxdata.InvalidCardData,m_DataModel.emptyfull.INVALIDCardMNearlyFull,m_DataModel.emptyfull.INVALIDCardMFull);							// Smart-card废票箱
	
	//m_DataModel.cashboxstatus.TokenAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenBoxAData,m_DataModel.emptyfull.TokenANearlyEmpty,m_DataModel.emptyfull.TokenAEmpty);												// Token箱A
	//m_DataModel.cashboxstatus.TokenBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenBoxBData,m_DataModel.emptyfull.TokenBNearlyEmpty,m_DataModel.emptyfull.TokenBEmpty);												// Token箱B
	//m_DataModel.cashboxstatus.TokenHopperAStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenHopperAData,m_DataModel.emptyfull.TokenANearlyEmpty,m_DataModel.emptyfull.TokenAEmpty);												// Token箱A
	//m_DataModel.cashboxstatus.TokenHopperBStatus = StatusChangeEmpty(m_DataModel.cashboxdata.TokenHopperBData,m_DataModel.emptyfull.TokenBNearlyEmpty,m_DataModel.emptyfull.TokenBEmpty);												// Token箱B
	//m_DataModel.cashboxstatus.WastedTokenStatus = StatusChangeFull(m_DataModel.cashboxdata.InvalidTokenData,m_DataModel.emptyfull.InvalidTokenNearlyFull,m_DataModel.emptyfull.InvalidTokenFull);								// Token废票箱
	//m_DataModel.cashboxstatus.CollectedTokenStatus = StatusChangeFull(m_DataModel.cashboxdata.CollectedTokenData,m_DataModel.emptyfull.CollectedTokenNearlyFull,m_DataModel.emptyfull.CollectedTokenFull);						// Token废票箱
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      转换钱箱、票箱将空、已空数据

@param      remainboxdata：			剩余枚/张数

@param      nearlyemptyboxdata：	将空枚/张数

@param      emptyboxdata：			已空枚/张数

@param      BoxID：					1-硬币循环找零箱	2—硬币循环找零箱外的其他箱子

@retval     CString

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CDeviceStatusSvc::StatusChangeEmpty(CString remainboxdata,CString nearlyemptyboxdata,CString emptyboxdata,int BoxID)
{
	int count = -1;
	int will_count = -1;
	int count_ed = -1;
	CString DataStr = _T("");

	count = _ttoi(remainboxdata);				
	will_count = _ttoi(nearlyemptyboxdata);		
	count_ed = _ttoi(emptyboxdata);	
	
	// 硬币循环找零箱外的其他箱子
	if(2 == BoxID)
	{
		if(count > will_count)
		{
			DataStr = _opl(DEVICESTATUS_NORMAL_SET);	//正常
		}
		else if(count<=will_count && count>count_ed)
		{
			DataStr = _opl(DEVICESTATUS_NEARLY_EMPTY_TXT);		//将空
		}
		else if(count <= count_ed)
		{
			DataStr = _opl(DEVICESTATUS_EMPTY_TXT);	//已空
		}
	}
	// 硬币循环找零箱
	else
	{
		if(count > will_count && count < count_ed)
		{
			DataStr = _opl(DEVICESTATUS_NORMAL_SET);			// 正常
		}
		else if(count <= will_count && count > 0)
		{
			DataStr = _opl(DEVICESTATUS_NEARLY_EMPTY_TXT);		// 将空
		}
		else if(count >= count_ed)
		{
			DataStr = _opl(DEVICESTATUS_FULL_TXT);				// 已满
		}
        else if(count == 0){
			DataStr = _opl(DEVICESTATUS_EMPTY_TXT);				// 已空
		}
	}
	return DataStr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      转换钱箱、票箱将满、已满数据

@param      remaindata：				剩余枚/张数		
@param      nearlyfullboxdata：		将满枚/张数
@param      fullboxdata：			已满枚/张数

@retval     CString

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CDeviceStatusSvc::StatusChangeFull(CString remaindata,CString nearlyfullboxdata,CString fullboxdata)
{
	int count = -1;
	int will_count = -1;
	int count_ed = -1;
	CString DataStr = _T("");

	count = _ttoi(remaindata);				
	will_count = _ttoi(nearlyfullboxdata);		
	count_ed = _ttoi(fullboxdata);	
	
	if(count >= count_ed)
	{
		DataStr = _opl(DEVICESTATUS_FULL_TXT);				// 已满
	}
	else if(count>=will_count && count<=count_ed)	
	{
		DataStr = _opl(DEVICESTATUS_NEARLY_FULL_TXT);		// 将满
	}
	else if(count < will_count)
	{
		DataStr = _opl(DEVICESTATUS_NORMAL_SET);			// 正常
	}		
	return DataStr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CDeviceStatusSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}
