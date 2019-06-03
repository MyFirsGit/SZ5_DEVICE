#include "stdafx.h"
#include "CoinBoxManageCollectCoinDlg.h"
#include "CoinBoxManageCollectCoinSvc.h"
#include "CTCoinCountInfo.h"
#include "TemplateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INTERNAL_ERROR(errCode) \
{if (COIN_HELPER->IsExceptionCode(errCode)) {\
	throw CCHException(errCode, _T(__FILE__), __LINE__);\
}else\
{throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);}}


BEGIN_MESSAGE_MAP(CCoinBoxManageCollectCoinSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageCollectCoinSvc::CCoinBoxManageCollectCoinSvc()
	:CTVMForeService(COLLECTION_COIN_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20114_COMMUNICATION_SET_DLG, new CCoinBoxManageCollectCoinDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COLLECT_COIN,_opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COLLECT_COIN,IDD_20114_COMMUNICATION_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COLLECT_COIN);

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageCollectCoinSvc::~CCoinBoxManageCollectCoinSvc()
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
void CCoinBoxManageCollectCoinSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_RETURN);	// 启用指定按键

	Menu_No = 0;
	InitializeDataModel();

	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();


	coincollectioninfoA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	coincollectioninfoB = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	CollectCoinHandleData();
	m_DataModel.ChangeACount = _ttoi(m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity);
	m_DataModel.ChangeBCount = _ttoi(m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity);
	m_DataModel.CircleACount = _ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxAQuantity);
	m_DataModel.CircleBCount = _ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxBQuantity);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COLLECT_COIN);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_COLLECT_COIN);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::InitializeDataModel()
{
	//// 同步计数，如果需要的话。
	//if(!COIN_HELPER->SyncCoinCountInfoToFile())
	//	m_DataModel.IsHasException = false;

	m_DataModel.boxid.CoinDedicatedChangeBoxAID = _T("");
	m_DataModel.boxid.CoinDedicatedChangeBoxBID = _T("");
	m_DataModel.boxid.CoinCycleChangeBoxAID = _T("");
	m_DataModel.boxid.CoinCycleChangeBoxBID = _T("");

	m_DataModel.boxtype.CoinDedicatedChangeBoxAType = _T("");
	m_DataModel.boxtype.CoinDedicatedChangeBoxBType = _T("");
	m_DataModel.boxtype.CoinCycleChangeBoxAType = _T("");
	m_DataModel.boxtype.CoinCycleChangeBoxBType = _T("");

	m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity = _T("");
	m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity = _T("");
	m_DataModel.boxquantity.CoinCycleChangeBoxAQuantity = _T("");
	m_DataModel.boxquantity.CoinCycleChangeBoxBQuantity = _T("");
	m_DataModel.boxquantity.DeBoxAAfterColQuantity = _T("");
	m_DataModel.boxquantity.DeBoxBAfterColQuantity = _T("");
	m_DataModel.boxquantity.CyBoxAfterColAQuantity = _T("");
	m_DataModel.boxquantity.CyBoxAfterColBQuantity = _T("");

	m_DataModel.CollectCoinFlowTime = -1;
	m_DataModel.IsHasException = FALSE;
	m_DataModel.IsAlreadyFull = FALSE;
	m_DataModel.bIsSelectColl = false;

	if(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		m_DataModel.IsHasException = true;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageCollectCoinSvc::Model& CCoinBoxManageCollectCoinSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取硬币补充箱币种

@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinBoxManageCollectCoinSvc::GetHopperCoinType(int hopetype)
{
	CString CurrencyType = _T("");
	switch(hopetype)									// 从TVM.INI文件中读取的键值
	{
	case 0:
		CurrencyType = _opl(TXT_COINMGR_INVALIDTYPE);		// 无效
		break;
	case 1:
		CurrencyType = _T("0.5") + _opl(MONEY_UNIT);    // 5角
		break;
	case 2:
		CurrencyType = _T("1 ") + _opl(MONEY_UNIT);		// 1MOP
		break;
	default:
		break;
	}
	return CurrencyType;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     处理面值、ID、数量数据 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::CollectCoinHandleData()
{
	try
	{
		LONG count = -1;

		m_DataModel.boxid.CoinDedicatedChangeBoxAID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();	// 硬币专用找零箱AID
		m_DataModel.boxid.CoinDedicatedChangeBoxBID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();	// 硬币专用找零箱BID	
		m_DataModel.boxid.CoinCycleChangeBoxAID      = theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();	// 硬币循环找零箱AID
		m_DataModel.boxid.CoinCycleChangeBoxBID		 = theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();	// 硬币循环找零箱BID							// 硬币循环找零箱BID


		m_DataModel.boxtype.CoinDedicatedChangeBoxAType = GetHopperCoinType(theTVM_INFO.GetCHChangeBoxAType()); // 硬币专用找零箱A面值
		m_DataModel.boxtype.CoinDedicatedChangeBoxBType = GetHopperCoinType(theTVM_INFO.GetCHChangeBoxBType()); // 硬币专用找零箱B面值
		m_DataModel.boxtype.CoinCycleChangeBoxAType		= GetHopperCoinType(theTVM_INFO.GetCHHopper2Type());	// 硬币循环找零箱A面值
		m_DataModel.boxtype.CoinCycleChangeBoxBType		= GetHopperCoinType(theTVM_INFO.GetCHHopper1Type());	// 硬币循环找零箱B面值

		m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity.Format(_T("%d"),theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount);	// 硬币专用找零箱A数量

		m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity.Format(_T("%d"),theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount);	// 硬币专用找零箱B数量

		m_DataModel.boxquantity.CoinCycleChangeBoxAQuantity.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount);

		m_DataModel.boxquantity.CoinCycleChangeBoxBQuantity.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount);
		//m_DataModel.boxquantity.CoinCycleChangeBoxAQuantity.Format(_T("%d"),coindata.OneY_inHopper);							// 硬币循环找零箱数量
		//m_DataModel.boxquantity.CoinCycleChangeBoxBQuantity.Format(_T("%d"),coindata.HalfY_inHopper);							// 硬币循环找零箱数量

		m_DataModel.CollectCoinFlowTime = FirstFrame;

		if(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
			m_DataModel.IsHasException = true;
		}
		NotifyDataModelChanged();
	}
	// 异常处理
	catch (CSysException& e) {
		m_DataModel.IsHasException = TRUE;
		NotifyDataModelChanged();
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		m_DataModel.IsHasException = TRUE;
		NotifyDataModelChanged();
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinBoxManageCollectCoinSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}
	try
	{
		// 对输入数据是否合理做出相应项的实际判断
		if(DoCheckInput()){			
			m_DataModel.bIsSelectColl = true;
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_SHOW_MSG_SAVE_INPUT_COLLECTION));
		}
	}
	// 异常处理
	catch (CSysException& e) {
		// 硬币回收日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	catch (...) {
		// 硬币回收日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     对输入数据是否合理做出相应项的实际判断 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinBoxManageCollectCoinSvc::DoCheckInput()
{
	Menu_No = theAPP_SESSION.GetInputToInt();				// 获取向导栏输入内容
	LONG count = -1;
	// 菜单编号不为1，提示输入内容非法
	if(MENU_CODE_1 != Menu_No)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return false;
	}
	// 输入内容合法，界面流转
	else
	{
		// 判断模块是否到位，如果不到位则不允许回收
		tCHADevStatus coin_info;
		try{
			COIN_HELPER->CH_GetStatus(&coin_info);

			if (!coin_info.bIsCoinModuleStandby){
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_NOT_INPLACE));
				return false;
			}
			if (!coin_info.bIsChangeBoxAInstall){
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOT_INSTALL));
			}
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
			return false;
		}
		catch(...){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
			return false;
		}


		if(INVALID_ID == theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString())
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOT_INSTALL));
			Menu_No = 0;
			return false;
		}
		else{
			try{
				IO_RSP_STATUS_INFO io_info;
				IO_HELPER->GetIOStatus(io_info);
				if (io_info.isCoinboxReady){
					// 判断是否有硬币可以回收
					if (/*theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount == 0 && 
						theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount == 0 && */
						theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount == 0 &&
						theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount == 0){
						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ALL_BOX_ALEADY_EMPTY));
						return false;
					}

					// 判断回收箱是否已满？
					WORD wFullCount = theAFC_TVM_OPERATION.GetRecycleBoxFullCount();

					if((theCOIN_COUNT.GetCollectionCoinboxAInfo().GetCurCount() >= wFullCount)){
						theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_ALREADYFULL));
						return false;
					}
				}
				else{
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOINSTALL));
					return false;
				}
			}
			catch(...)
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOINSTALL));
				return false;
			}
			
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     硬币回收 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::DoCollectCoin()
{
	
	
//	int Priority_Hopper1 = theTVM_INFO.GetCollectCoinPriorityforHopper1();		// 硬币专用找零箱A硬币回收优先级顺序
//	int Priority_Hopper2 = theTVM_INFO.GetCollectCoinPriorityforHopper2();		// 硬币专用找零箱B硬币回收优先级顺序
//	int Priority_Stock = theTVM_INFO.GetCollectCoinPriorityforStock();			// 硬币循环找零箱硬币回收优先级顺序
//	
//	theSERVICE_MGR.SetForeServiceBusy(true);
//	// 硬币循环找零箱最先回收--1
//	if(Priority_Stock < Priority_Hopper1 && Priority_Stock < Priority_Hopper2)
//	{
//		if(!DoCollectStockBox())goto ERROR_RETURN;		// 硬币循环找零箱硬币回收
//		if(Priority_Hopper1 < Priority_Hopper2)
//		{
//			if(!DoCollectHopper1Box())goto ERROR_RETURN;	// 硬币专用找零箱A硬币回收
//			if(!DoCollectHopper2Box())goto ERROR_RETURN;	// 硬币专用找零箱B硬币回收
//		}
//		else
//		{
//			if(!DoCollectHopper2Box())goto ERROR_RETURN;	// 硬币专用找零箱B硬币回收
//			if(!DoCollectHopper1Box())goto ERROR_RETURN;	// 硬币专用找零箱A硬币回收
//		}
//	}
//	else if(Priority_Stock > Priority_Hopper1 && Priority_Stock > Priority_Hopper2)
//	{
//		if(Priority_Hopper1 < Priority_Hopper2)
//		{
//			if(!DoCollectHopper1Box())goto ERROR_RETURN;	// 硬币专用找零箱A硬币回收
//			if(!DoCollectHopper2Box())goto ERROR_RETURN;	// 硬币专用找零箱B硬币回收
//		}
//		else
//		{
//			if(!DoCollectHopper2Box())goto ERROR_RETURN;	// 硬币专用找零箱B硬币回收
//			if(!DoCollectHopper1Box())goto ERROR_RETURN;	// 硬币专用找零箱A硬币回收
//		}
//		if(!DoCollectStockBox())goto ERROR_RETURN;		// 硬币循环找零箱硬币回收
//	}
//	else
//	{
//		if(Priority_Hopper1 < Priority_Stock)
//		{
//			if(!DoCollectHopper1Box())goto ERROR_RETURN;	// 硬币专用找零箱A硬币回收
//			if(!DoCollectStockBox())goto ERROR_RETURN;	// 硬币循环找零箱硬币回收
//			if(!DoCollectHopper2Box())goto ERROR_RETURN;	// 硬币专用找零箱B硬币回收
//		}
//		else
//		{
//			if(!DoCollectHopper2Box())goto ERROR_RETURN;	// 硬币专用找零箱B硬币回收
//			if(!DoCollectStockBox())goto ERROR_RETURN;	// 硬币循环找零箱硬币回收
//			if(!DoCollectHopper1Box())goto ERROR_RETURN;	// 硬币专用找零箱A硬币回收
//		}
//	}
//	m_DataModel.CollectCoinFlowTime = ThridTFrame;
//	NotifyDataModelChanged();
//	
//	PrintCoinBoxExchangeReceipt();
//
//	// 上传电文
//	//Sendcables(coin_dedicated_change_boxA);					// 硬币专用找零箱A
//	//Sendcables(coin_dedicated_change_boxB);					// 硬币专用找零箱B
//	//Sendcables(coin_cycle_change_box);						// 硬币循环找零箱
//	Sendcables();
//	// 本机设置日志
//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,true);
//	theSERVICE_MGR.SetForeServiceBusy(false);
//	return;
//ERROR_RETURN:
//	m_DataModel.CollectCoinFlowTime = FirstFrame;
//	NotifyDataModelChanged();
//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,false);
//	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     硬币循环找零箱硬币回收 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinBoxManageCollectCoinSvc::DoCollectStockBox()
{
	//long errCode = 0;
	//COIN_SAVE save_box;

	//CH_COMMON_RSP rspstock;
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionboxinfo;	
	//CTCoinCountInfo::COIN_BOX_INFO coinboxinfo;	

	//// 获取硬币回收箱已满硬币枚数
	//CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
	//theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);

	//LONG count_collectionboxA = 0;
	//LONG count_collectionboxB = 0;

	//// 硬币回收箱A--现有硬币枚数
	//count_collectionboxA = theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao;
	//// 硬币回收箱B--现有硬币枚数
	//count_collectionboxB = theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_2yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao;
	//
	//int Direction_Stock = theTVM_INFO.GetCollectDirectionforStock();			// 硬币循环找零箱硬币回收进入硬币回收箱编号
	//
	//save_box.CH_SAVE_STOCK1_  = true;
	//save_box.CH_SAVE_HOPPER1_ = false;
	//save_box.CH_SAVE_HOPPER2_ = false;
	//
	//// 回收到硬币回收箱A
	//if(1 == Direction_Stock)
	//{
	//	save_box.CH_POS_BOX1_ = true;
	//	save_box.CH_POS_BOX2_ = false;
	//	// 硬币回收箱A--已满
	//	/*if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt) 
	//	{
	//	m_DataModel.IsAlreadyFull = TRUE;
	//	m_DataModel.boxquantity.CyBoxAfterColQuantity = m_DataModel.boxquantity.CoinCycleChangeBoxQuantity;
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLBOXAFULL_CYCLE));
	//	return;
	//	}*/
	//}
	//// 回收到硬币回收箱B
	//else
	//{
	//	save_box.CH_POS_BOX2_ = true;
	//	save_box.CH_POS_BOX1_ = false;
	//	// 硬币回收箱B--已满
	//	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxB > billbox_param.coinCollectionBAlreadyFullCoinCnt)
	//	{
	//		m_DataModel.IsAlreadyFull = TRUE;
	//		m_DataModel.boxquantity.CyBoxAfterColQuantity = m_DataModel.boxquantity.CoinCycleChangeBoxQuantity;
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLBOXBFULL_CYCLE));
	//		return false;
	//	}
	//}
	//// 硬币回收箱A--已满，硬币回收箱B--已满
	//if((_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt) && (_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionBAlreadyFullCoinCnt))
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_ALREADYFULL));
	//	return false;
	//}
	//else
	//{
	//	// 硬币回收箱A--已满,硬币循环找零箱只能回收到硬币回收箱B
	//	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt)
	//	{
	//		save_box.CH_POS_BOX2_ = true;
	//		save_box.CH_POS_BOX1_ = false;
	//	}
	//	/* // 硬币回收箱B--已满，硬币循环找零箱只能回收到硬币回收箱A
	//	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionBAlreadyFullCoinCnt)
	//	{
	//		save_box.CH_POS_BOX1_ = true;
	//		save_box.CH_POS_BOX2_ = false;
	//	}*/
	//}

	//errCode = COIN_HELPER->CH_ClearCoin(save_box,rspstock);	// 硬币回收
	//// 错误代码不为0
	//if(E_NO_CH_ERROR != errCode)	
	//{
	//	INTERNAL_ERROR(errCode);									// 判断是硬件异常、软件异常分别处理
	//}

	//// 规避负数错误（人为添加）
	//if(rspstock.coin_info.stock1 > 1500)
	//{
	//	rspstock.coin_info.stock1 = 0;
	//}

	//m_DataModel.boxquantity.CyBoxAfterColQuantity.Format(_T("%d"),rspstock.coin_info.stock1);

	//// 硬币回收箱A -- 软件计数

	//// 硬币回收箱A -- ID
	//coincollectionboxinfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//coincollectionboxinfo.ulCount_1yuan = rspstock.coin_info.collect1_1Num;
	//coincollectionboxinfo.ulCount_5yuan = rspstock.coin_info.collect1_5Num;
	//coincollectionboxinfo.ulCount_5jiao = rspstock.coin_info.collect1_05Num;
	//if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(coincollectionboxinfo))	
	//{
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱A的计数信息写入失败"));
	//}

	//// 硬币回收箱B -- 软件计数

	//// 硬币回收箱B -- ID
	//coincollectionboxinfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//coincollectionboxinfo.ulCount_1yuan = rspstock.coin_info.collect2_1Num;
	//coincollectionboxinfo.ulCount_2yuan = rspstock.coin_info.collect2_2Num;
	//coincollectionboxinfo.ulCount_5yuan = rspstock.coin_info.collect2_5Num;
	//coincollectionboxinfo.ulCount_5jiao = rspstock.coin_info.collect2_05Num;
	//if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(coincollectionboxinfo))	
	//{
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱B的计数信息写入失败"));
	//}

	//// 硬币循环找零箱 -- 软件计数

	//// 初始化硬币循环找零箱ID
	//coinboxinfo = theCOIN_COUNT.GetCoinHopperboxCInfo();
	//coinboxinfo.ulCurCount = rspstock.coin_info.stock1; 		
	//if(!theCOIN_COUNT.SetCoinHopperboxCInfo(coinboxinfo))	
	//{
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币循环找零箱的计数信息写入失败"));
	//}

	//// 数据导入数据库
	//Dataintodatabase(coin_cycle_change_box,Direction_Stock);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     硬币专用找零箱A硬币回收 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinBoxManageCollectCoinSvc::DoCollectHopper1Box()
{
	//long errCode = 0;
	////COIN_SAVE save_box;

	//CH_COMMON_RSP rsphopper1;
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionboxinfo;	
	//CTCoinCountInfo::COIN_BOX_INFO coinboxinfo;	

	//// 获取硬币回收箱已满硬币枚数
	//CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
	//theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);

	//LONG count_collectionboxA = 0;
	//LONG count_collectionboxB = 0;

	//// 硬币回收箱A--现有硬币枚数
	//count_collectionboxA = theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao;
	//// 硬币回收箱B--现有硬币枚数
	//count_collectionboxB = theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao;
	//
	//int Direction_Hopper1 = theTVM_INFO.GetCollectDirectionforHopper1();		// 硬币专用找零箱A硬币回收进入硬币回收箱编号

	//DatalightInfo rsp;
	//COIN_HELPER->CoinDataRead(rsp);
	//// 回收箱1元满载阀值
	//rsp .OneY_maxthreshold_inRecover;

	//// 回收到硬币回收箱A
	//if(1 == Direction_Hopper1)
	//{
	//	// 如果
	//	save_box.CH_POS_BOX1_ = true;
	//	save_box.CH_POS_BOX2_ = false;

	//	// 硬币回收箱A--已满
	//	if(_ttoi(m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity) + count_collectionboxA > billbox_param.coinCollectionAlreadyFullCoinCnt) 
	//	{
	//		m_DataModel.IsAlreadyFull = TRUE;
	//		m_DataModel.boxquantity.DeBoxAAfterColQuantity = m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity;
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLBOXAFULL_DEA));
	//		return false;
	//	}
	//}
	//// 回收到硬币回收箱B
	//else
	//{
	//	save_box.CH_POS_BOX2_ = true;
	//	save_box.CH_POS_BOX1_ = false;

	//	// 硬币回收箱B--已满
	//	if(_ttoi(m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity) + count_collectionboxB > billbox_param.coinCollectionWillFullCoinCnt) 
	//	{
	//		m_DataModel.IsAlreadyFull = TRUE;
	//		m_DataModel.boxquantity.DeBoxAAfterColQuantity = m_DataModel.boxquantity.CoinDedicatedChangeBoxAQuantity;
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLBOXBFULL_DEA));
	//		return false;
	//	}
	//}
	//// 硬币回收箱A--已满，硬币回收箱B--已满
	////if((_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt) && (_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionBAlreadyFullCoinCnt))
	////{
	////	theAPP_SESSION.ShowOfficeGuide(_T("硬币回收箱已满，请更换硬币回收箱"));
	////	return;
	////}
	////else
	////{
	////	// 硬币回收箱A--已满,硬币循环找零箱只能回收到硬币回收箱B
	////	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt)
	////	{
	////		save_box.CH_POS_BOX2_ = true;
	////		save_box.CH_POS_BOX1_ = false;
	////	}
	////	// 硬币回收箱B--已满，硬币循环找零箱只能回收到硬币回收箱A
	////	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionBAlreadyFullCoinCnt)
	////	{
	////		save_box.CH_POS_BOX1_ = true;
	////		save_box.CH_POS_BOX2_ = false;
	////	}
	////}
	//CH_SPEC5_RSP save_rsp;
	//errCode = COIN_HELPER->CH_ClearCoin(save_rsp,0,0,0xFFFF,0xFFFF);	// 硬币回收
	//// 错误代码不为0
	//if(E_NO_CH_ERROR != errCode)	
	//{
	//	INTERNAL_ERROR(errCode);									// 判断是硬件异常、软件异常分别处理
	//}

	//// 规避负数错误（人为添加）
	///*if(rsphopper1.coin_info.hopper1 > 1500)
	//{
	//rsphopper1.coin_info.hopper1 = 0;
	//}*/

	//m_DataModel.boxquantity.DeBoxAAfterColQuantity.Format(_T("%d"),rsphopper1.coin_info.hopper1);

	//// 硬币回收箱A -- 软件计数

	//// 硬币回收箱A -- ID
	//coincollectionboxinfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//coincollectionboxinfo.ulCount_1yuan = rsphopper1.coin_info.collect1_1Num;
	//coincollectionboxinfo.ulCount_5jiao = rsphopper1.coin_info.collect1_05Num;
	//if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(coincollectionboxinfo))	
	//{
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱A的计数信息写入失败"));
	//}

	//// 硬币回收箱B -- 软件计数

	//// 硬币回收箱B -- ID
	//coincollectionboxinfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	//coincollectionboxinfo.ulCount_1yuan = rsphopper1.coin_info.collect2_1Num;
	//coincollectionboxinfo.ulCount_2yuan = rsphopper1.coin_info.collect2_2Num;
	//coincollectionboxinfo.ulCount_5yuan = rsphopper1.coin_info.collect2_5Num;
	//coincollectionboxinfo.ulCount_5jiao = rsphopper1.coin_info.collect2_05Num;
	//if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(coincollectionboxinfo))	
	//{
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱B的计数信息写入失败"));
	//}

	//// 硬币专用找零箱A -- 软件计数

	//// 初始化硬币专用找零箱AID
	//coinboxinfo = theCOIN_COUNT.GetCoinHopperboxAInfo();
	//coinboxinfo.ulCurCount = rsphopper1.coin_info.hopper1; 		
	//if(!theCOIN_COUNT.SetCoinHopperboxAInfo(coinboxinfo))	
	//{
	//	theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币专用找零箱A的计数信息写入失败"));
	//}

	//// 数据导入数据库
	//Dataintodatabase(coin_dedicated_change_boxA,Direction_Hopper1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     硬币专用找零箱B硬币回收 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinBoxManageCollectCoinSvc::DoCollectHopper2Box()
{
// 	long errCode = 0;
// 	COIN_SAVE save_box;
// 
// 	CH_COMMON_RSP rsphopper2;
// 	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionboxinfo;	
// 	CTCoinCountInfo::COIN_BOX_INFO coinboxinfo;	
// 
// 	// 获取硬币回收箱已满硬币枚数
// 	CAfcTVMOperationParam::BILLBOX_PARAM billbox_param;
// 	theAFC_TVM_OPERATION.GetBillboxParam(billbox_param);
// 
// 	LONG count_collectionboxA = 0;
// 	LONG count_collectionboxB = 0;
// 
// 	// 硬币回收箱A--现有硬币枚数
// 	count_collectionboxA = theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao;
// 	// 硬币回收箱B--现有硬币枚数
// 	count_collectionboxB = theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_1yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_2yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5yuan + theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao;
// 	
// 	int Direction_Hopper2 = theTVM_INFO.GetCollectDirectionforHopper2();		// 硬币专用找零箱B硬币回收进入硬币回收箱编号
// 
// 	save_box.CH_SAVE_HOPPER2_ = true;
// 	save_box.CH_SAVE_HOPPER1_ = false;
// 	save_box.CH_SAVE_STOCK1_  = false;
// 
// 	// 回收到硬币回收箱A
// 	if(1 == Direction_Hopper2)
// 	{
// 		save_box.CH_POS_BOX1_ = true;
// 		save_box.CH_POS_BOX2_ = false;
// 
// 		// 硬币回收箱A--已满
// 		if(_ttoi(m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt) 
// 		{
// 			m_DataModel.IsAlreadyFull = TRUE;
// 			m_DataModel.boxquantity.DeBoxBAfterColQuantity = m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity;
// 			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLBOXAFULL_DEB));
// 			return false;
// 		}
// 	}
// 	// 回收到硬币回收箱B
// 	else
// 	{
// 		save_box.CH_POS_BOX2_ = true;
// 		save_box.CH_POS_BOX1_ = false;
// 
// 		// 硬币回收箱B--已满
// 		if(_ttoi(m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity) + count_collectionboxB > billbox_param.coinCollectionBAlreadyFullCoinCnt) 
// 		{
// 			m_DataModel.IsAlreadyFull = TRUE;
// 			m_DataModel.boxquantity.DeBoxBAfterColQuantity = m_DataModel.boxquantity.CoinDedicatedChangeBoxBQuantity;
// 			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLBOXBFULL_DEB));
// 			return false;
// 		}
// 	}
// 	// 硬币回收箱A--已满，硬币回收箱B--已满
// 	//if((_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt) && (_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionBAlreadyFullCoinCnt))
// 	//{
// 	//	theAPP_SESSION.ShowOfficeGuide(_T("硬币回收箱已满，请更换硬币回收箱"));
// 	//	return;
// 	//}
// 	//else
// 	//{
// 	//	// 硬币回收箱A--已满,硬币循环找零箱只能回收到硬币回收箱B
// 	//	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionAAlreadyFullCoinCnt)
// 	//	{
// 	//		save_box.CH_POS_BOX2_ = true;
// 	//		save_box.CH_POS_BOX1_ = false;
// 	//	}
// 	//	// 硬币回收箱B--已满，硬币循环找零箱只能回收到硬币回收箱A
// 	//	if(_ttoi(m_DataModel.boxquantity.CoinCycleChangeBoxQuantity) + count_collectionboxA > billbox_param.coinCollectionBAlreadyFullCoinCnt)
// 	//	{
// 	//		save_box.CH_POS_BOX1_ = true;
// 	//		save_box.CH_POS_BOX2_ = false;
// 	//	}
// 	//}
// 	errCode = COIN_HELPER->CH_ClearCoin(save_box,rsphopper2);	// 硬币回收
// 	// 错误代码不为0
// 	if(E_NO_CH_ERROR != errCode)	
// 	{
// 		INTERNAL_ERROR(errCode);									// 判断是硬件异常、软件异常分别处理
// 	}
// 
// 	// 规避负数错误（人为添加）
// 	if(rsphopper2.coin_info.hopper2 > 1500)
// 	{
// 		rsphopper2.coin_info.hopper2 = 0;
// 	}
// 
// 	m_DataModel.boxquantity.DeBoxBAfterColQuantity.Format(_T("%d"),rsphopper2.coin_info.hopper2);
// 
// 	// 硬币回收箱A -- 软件计数
// 
// 	// 硬币回收箱A -- ID
// 	coincollectionboxinfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
// 	coincollectionboxinfo.ulCount_1yuan = rsphopper2.coin_info.collect1_1Num;
// 	coincollectionboxinfo.ulCount_5yuan = rsphopper2.coin_info.collect1_5Num;
// 	coincollectionboxinfo.ulCount_5jiao = rsphopper2.coin_info.collect1_05Num;
// 	if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(coincollectionboxinfo))	
// 	{
// 		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱A的计数信息写入失败"));
// 	}
// 
// 	// 硬币回收箱B -- 软件计数
// 
// 	// 硬币回收箱B -- ID
// 	coincollectionboxinfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
// 	coincollectionboxinfo.ulCount_1yuan = rsphopper2.coin_info.collect2_1Num;
// 	coincollectionboxinfo.ulCount_2yuan = rsphopper2.coin_info.collect2_2Num;
// 	coincollectionboxinfo.ulCount_5yuan = rsphopper2.coin_info.collect2_5Num;
// 	coincollectionboxinfo.ulCount_5jiao = rsphopper2.coin_info.collect2_05Num;
// 	if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(coincollectionboxinfo))	
// 	{
// 		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱B的计数信息写入失败"));
// 	}
// 
// 	// 硬币专用找零箱B -- 软件计数
// 
// 	// 初始化硬币专用找零箱BID
// 	coinboxinfo = theCOIN_COUNT.GetCoinHopperboxBInfo();
// 	coinboxinfo.ulCurCount = rsphopper2.coin_info.hopper2; 		
// 	if(!theCOIN_COUNT.SetCoinHopperboxBInfo(coinboxinfo))	
// 	{
// 		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币专用找零箱B的计数信息写入失败"));
// 	}
// 
// 	// 数据导入数据库
// 	Dataintodatabase(coin_dedicated_change_boxB,Direction_Hopper2);
	return true;
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
LRESULT CCoinBoxManageCollectCoinSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	// 硬币回收基本界面和硬币回收完成界面可以返回，硬币回收中界面不可以返回
	if(SecondFrame != m_DataModel.CollectCoinFlowTime)
	{
		theAPP_SESSION.ReturnMaintainMenu();
		Menu_No = 0;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      F2响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinBoxManageCollectCoinSvc::OnKeyboardF2(WPARAM , LPARAM ){
	try
	{
		if(m_DataModel.bIsSelectColl)
		{
			m_DataModel.bIsSelectColl = false;
			m_DataModel.CollectCoinFlowTime = SecondFrame;
			CCoinBoxManageCollectCoinDlg *pDlg = dynamic_cast<CCoinBoxManageCollectCoinDlg*>(m_pDialogFlow->GetDialog(CCoinBoxManageCollectCoinDlg::IDD));
			pDlg->SendMessage(WM_NOTIFY_DIALOG_UPDATE,NULL,NULL);
			pDlg->UpdateWindow();

			theSERVICE_MGR.SetForeServiceBusy(true);
			// 开始回收
			if(ExecuteCollection()){
				// 发送回收电文
				Sendcables();

				// 插入数据库
				Dataintodatabase();

				// 打印回收凭证
				PrintCoinCollectReceipt();
			}
		}	
	}
	// 异常处理
	catch (CSysException& e) {
		// 硬币回收日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (...) {
		// 硬币回收日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}

	// 初始化数据
	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();


	if(m_DataModel.IsPrintFailed){
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	}
	NotifyDataModelChanged();
	theSERVICE_MGR.SetForeServiceBusy(false);
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     上传电文 

@param      int boxnumber

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::Sendcables(){
	vector<CASH_TYPE_COUNT> cashData;
	CASH_TYPE_COUNT cashTypeOne,cashTypeHalf;

	// 计算清出的数
	// 循环找零箱A
	if(m_DataModel.CircleAColCount > 0){
		cashTypeHalf.cash_type  = CoinHalfYuan;
		cashTypeHalf.cash_count = m_DataModel.CircleAColCount;
	}

	// 循环找零箱B
	if(m_DataModel.CircleBColCount > 0){
		cashTypeOne.cash_type  = Coin1Yuan;
		cashTypeOne.cash_count = m_DataModel.CircleBColCount;
	}

	// 备用找零箱A
	if(m_DataModel.ChangeAColCount > 0){
		BankNoteAndCoinType_t coinChangeBoxAType = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
		if(coinChangeBoxAType == CoinHalfYuan){
			cashTypeHalf.cash_type = CoinHalfYuan;
			cashTypeHalf.cash_count += m_DataModel.ChangeAColCount;
		}
		else if(coinChangeBoxAType == Coin1Yuan){
			cashTypeOne.cash_type = Coin1Yuan;
			cashTypeOne.cash_count += m_DataModel.ChangeAColCount;
		}
	}

	// 备用找零箱B
	if(m_DataModel.ChangeBColCount > 0){
		BankNoteAndCoinType_t coinChangeBoxBType = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
		if(coinChangeBoxBType == CoinHalfYuan){
			cashTypeHalf.cash_type = CoinHalfYuan;
			cashTypeHalf.cash_count += m_DataModel.ChangeBColCount;
		}
		else if(coinChangeBoxBType == Coin1Yuan){
			cashTypeOne.cash_type = Coin1Yuan;
			cashTypeOne.cash_count += m_DataModel.ChangeBColCount;
		}
	}

	// 插入容器
	if(cashTypeHalf.cash_count > 0){
		cashData.push_back(cashTypeHalf);
	}

	if(cashTypeOne.cash_count > 0){
		cashData.push_back(cashTypeOne);
	}

	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendClearCoinsBoxReport(cashData);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打包币种代码、数量

@param      int hopetype

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
/*vector<CASH_TYPE_COUNT>*/CASH_TYPE_COUNT CCoinBoxManageCollectCoinSvc::PackageCoinTypeAndCount(int hopetype,int count)
{
	CASH_TYPE_COUNT cointypecount;
	//vector<CASH_TYPE_COUNT> vecCash;
	switch(hopetype)
	{
	case coin_50avos:
		cointypecount.cash_type = CoinHalfYuan;
		cointypecount.cash_count = count;
		//vecCash.push_back(cointypecount);
		break;
	case coin_1mop:
		cointypecount.cash_type = Coin1Yuan;
		cointypecount.cash_count = count;
		//vecCash.push_back(cointypecount);
		break;
	//case coin_2mop:
	//	cointypecount.cash_type = Coin2MOP;
	//	cointypecount.cash_count = count;
	//	//vecCash.push_back(cointypecount);
	//	break;
	//case coin_5mop:
	//	cointypecount.cash_type = Coin5MOP;
	//	cointypecount.cash_count = count;
	//	//vecCash.push_back(cointypecount);
	//	break;
	}
	//return vecCash;
	return cointypecount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币循环找零箱加币凭证打印

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::PrintCoinCollectReceipt()
{
	try
	{
		CTCoinCountInfo::COIN_BOX_INFO hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
		CTCoinCountInfo::COIN_BOX_INFO hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();

		// 加载Template
		CString CoinBox_templatename = _T("TVM_COIN_COLLECT.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			line.Replace(_T("{操作员}"), theAPP_SESSION.GetUserInfo().sUserID);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());	
			if(line.Find(_T("备用找零箱1"))>=0){
				line.Replace(_T("{操作前金额}"), ComFormatAmount(mEx_hopper_A.ulCurCount*100));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(hopper_A.ulCurCount*100));
			}
			if(line.Find(_T("备用找零箱2"))>=0){
				line.Replace(_T("{操作前金额}"), ComFormatAmount(mEx_hopper_B.ulCurCount*100));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(hopper_B.ulCurCount*100));
			}
			if(line.Find(_T("合计"))>=0){
				int nTotalBefore = mEx_hopper_A.ulCurCount + mEx_hopper_B.ulCurCount;
				int nTotalAfter = hopper_A.ulCurCount + hopper_B.ulCurCount;
				line.Replace(_T("{操作前金额}"), ComFormatAmount(nTotalBefore*100));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(nTotalAfter*100));
			}
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray saText;
			ComPrintLineFormatForTemplate(line, saText);
			printtext.Append(saText);
			Index++;
		}

		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLECTION);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsPrintFailed = TRUE;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.IsPrintFailed = TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     数据导入数据库 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageCollectCoinSvc::Dataintodatabase()
{
	try{
		CoinCollectOperationRecord collectRecord;

		collectRecord.dtOperationTime = ComGetCurTime();

		// 循环找零箱A有回收
		if(m_DataModel.CircleAColCount > 0){
			collectRecord.iCoinTypeCode				= CoinHalfYuan;
			collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();
			collectRecord.iQuantityBeforeCollect	= mEx_hopper_A.ulCurCount;
			collectRecord.iQuantityCollect			= m_DataModel.CircleAColCount;
			collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;
			collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
			collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;

			DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
		}

		// 循环找零箱B有回收
		if(m_DataModel.CircleBColCount > 0){
			collectRecord.iCoinTypeCode				= Coin1Yuan;
			collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();
			collectRecord.iQuantityBeforeCollect	= mEx_hopper_B.ulCurCount;
			collectRecord.iQuantityCollect			= m_DataModel.CircleBColCount;
			collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;
			collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
			collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;

			DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
		}

		// 硬币备用找零箱A有回收
		if(m_DataModel.ChangeAColCount > 0){
			collectRecord.iCoinTypeCode				= TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
			collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
			collectRecord.iQuantityBeforeCollect	= mEx_change_A.ulCount;
			collectRecord.iQuantityCollect			= m_DataModel.ChangeAColCount;
			collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
			collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
			collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;

			DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
		}

		// 硬币备用找零箱B有回收
		if(m_DataModel.ChangeBColCount > 0){
			collectRecord.iCoinTypeCode				= TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
			collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
			collectRecord.iQuantityBeforeCollect	= mEx_change_B.ulCount;
			collectRecord.iQuantityCollect			= m_DataModel.ChangeBColCount;
			collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
			collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
			collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;

			DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
		}

	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币回收（执行）

@param      无

@retval     bool true:成功;false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinBoxManageCollectCoinSvc::ExecuteCollection()
{
	// 原则：1、循环找零箱的硬币只能到右边回收箱
	//       2、备用找零箱的硬币只能到左边回收箱
	//       3、回收全部清空数据，计数不符的，按实际计数来计数，打印凭证实际与计数不符

	// IO状态：回收箱是否在位
	IO_RSP_STATUS_INFO io_info;
	tCHADevStatus rsp;
	try{
		IO_HELPER->GetIOStatus(io_info);

		COIN_HELPER->CH_GetStatus(&rsp);

		if (!rsp.bIsCoinModuleStandby){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_NOT_INPLACE));
			Sleep(1000);
			theTVM_SETTING.SetCoinBoxLastAction(0);
			return false;
		}

		//if(rsp.bBusyRecycle){
		//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_COLLECTING));
		//	Sleep(1000);
		//	theSETTING.SetCollectCoinResult(0);
		//	return false;
		//}
	}
	catch(CSysException& e){
		m_DataModel.IsHasException = true;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		// Do nothing
	}


	// 各个箱子回收数量
	WORD wHopperOneOut = 0xffff,wHopperHalfOut = 0xffff,wChangeBoxLeft = 0xffff,wChangeBoxRight = 0xffff;

	CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();// 一元
	CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();// 5角

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxA = theCOIN_COUNT.GetCollectionCoinboxAInfo();//右边
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxB = theCOIN_COUNT.GetCollectionCoinboxBInfo();//左边

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBoxA = theCOIN_COUNT.GetChangeCoinboxAInfo();//右边
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBoxB = theCOIN_COUNT.GetChangeCoinboxBInfo();//左边

	WORD wCollFullNumber = theAFC_TVM_OPERATION.GetRecycleBoxFullCount();

	 //1、先判断循环找零箱

	WORD wMaxCollCount = wCollFullNumber - collectionBoxA.GetCurCount();

	if(io_info.isCoinboxReady){// 回收箱在位
		if(wMaxCollCount > 0){
			// 判断是否可以回收完？
			if((hopperB.ulCurCount /* + hopperA.ulCurCount*/) <= wMaxCollCount){//回收所有
				wHopperOneOut = ALL_COIN_COUNT;//hopperB.ulCurCount;
				wHopperHalfOut= 0;//ALL_COIN_COUNT;//hopperA.ulCurCount;
			}
			else
			{
				wHopperOneOut = wMaxCollCount;
				wHopperHalfOut = 0;
			}
			wMaxCollCount -= hopperB.ulCurCount;
		}
	}

	tCHADevStatus chSTrsp;
	try{
		COIN_HELPER->CH_GetStatus(&chSTrsp);
	}
	catch(CSysException& e){
		m_DataModel.IsHasException = true;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}

	bool bIsChangeBoxAInstall = chSTrsp.bIsChangeBoxAInstall;//(右边)
	bool bIsChangeBoxBInstall = chSTrsp.bIsChangeBoxBInstall;//(左边)

	// 2、再判断备用找零箱
	if(io_info.isCoinboxReady/*io_info.isCoinbox2LidOpened*/){// 回收箱左边在位
		//WORD wMaxCollCount = wCollFullNumber - collectionBoxB.GetCurCount();
		if(wMaxCollCount > 0){
			// 备用箱是否在位？
			if(bIsChangeBoxAInstall && !bIsChangeBoxBInstall){// 右在左不在
				if(changeBoxA.ulCount <= wMaxCollCount){
					wChangeBoxLeft = changeBoxA.ulCount;
					wChangeBoxRight = 0;
				}
				else{
					wChangeBoxLeft = wMaxCollCount;
					wChangeBoxRight  = 0;
				}
			}


			if(bIsChangeBoxBInstall && !bIsChangeBoxAInstall){// 左在右不在
				if(changeBoxB.ulCount <= wMaxCollCount){
					wChangeBoxRight = changeBoxB.ulCount;
					wChangeBoxLeft= 0;
				}
				else{
					wChangeBoxRight = wMaxCollCount;
					wChangeBoxLeft= 0;
				}
			}

			if(bIsChangeBoxAInstall && bIsChangeBoxBInstall){// 左右都在
				if((changeBoxA.ulCount + changeBoxB.ulCount) <= wMaxCollCount){// 回收所有
					wChangeBoxLeft = ALL_COIN_COUNT;//changeBoxB.ulCount;
					wChangeBoxRight= ALL_COIN_COUNT;//changeBoxA.ulCount;
				}
				else if(changeBoxA.ulCount <= changeBoxB.ulCount) {// 先回收少的
					if(changeBoxA.ulCount <= wMaxCollCount){
						wChangeBoxLeft = changeBoxA.ulCount;
						wChangeBoxRight  = wMaxCollCount - wChangeBoxLeft;
					}
					else{
						wChangeBoxLeft = wMaxCollCount;
						wChangeBoxRight  = 0;
					}
				}
				else{
					if(changeBoxB.ulCount <= wMaxCollCount){
						wChangeBoxRight = changeBoxB.ulCount;
						wChangeBoxLeft= wMaxCollCount - wChangeBoxRight;
					}
					else{
						wChangeBoxLeft = wMaxCollCount;
						wChangeBoxRight  = 0;
					}
				}
			}
		}
	}


	bool bCollSuccess = true;
	long errCode  = 0;
	tCoinBoxInfo collRsp;
	if (!chSTrsp.bIsChangeBoxAInstall)
	{
		wChangeBoxLeft = 0;
	}
	if (!chSTrsp.bIsChangeBoxBInstall)
	{
		wChangeBoxRight = 0;
	}
	// 3.开始回收
	try{
			errCode = COIN_HELPER->CH_ClearCoin(&collRsp);
			wChangeBoxLeft = 0;

	}
	catch(CSysException& e){
		m_DataModel.IsHasException = true;
		bCollSuccess = false;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		bCollSuccess = false;
	}


	if(m_DataModel.IsHasException){
		// 提示有异常
		//return false;
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
	}

	// 合法性检查
	if(collRsp.OneY_preparation > 1000){
		collRsp.OneY_preparation = 0;
	}
	if(collRsp.HalfY_preparation > 1000){
		collRsp.HalfY_preparation = 0;
	}
	if(collRsp.OneY_inHopper > 1000){
		collRsp.OneY_inHopper = 0;
	}
	if(collRsp.HalfY_inHopper > 1000){
		collRsp.HalfY_inHopper = 0;
	}

	// 4.分析回收结果
	// 实际回收数量
	m_DataModel.ChangeAColCount = collRsp.OneY_preparation;
	m_DataModel.ChangeBColCount = collRsp.HalfY_preparation;
	m_DataModel.CircleAColCount = collRsp.OneY_inHopper;
	m_DataModel.CircleBColCount = collRsp.HalfY_inHopper;


	// 只相信软计数，有可能实际与计数不符合
	if(wHopperOneOut == ALL_COIN_COUNT){
		//hopperA.ulCurCount = 0;
		if(hopperA.ulCurCount <= collRsp.OneY_inHopper){// 实际确实回收完
			hopperA.ulCurCount = 0;
		}
		else if(hopperA.ulCurCount > collRsp.OneY_inHopper){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
			//hopperA.ulCurCount -= collRsp.wHalfHopperCount;
			if(bCollSuccess)
				hopperA.ulCurCount = 0;
			else
				hopperA.ulCurCount -= collRsp.OneY_inHopper;
		}
	}
	else{
		if(wHopperOneOut == collRsp.OneY_inHopper){
			hopperA.ulCurCount -= wHopperOneOut;
		}
		else{
			if(wHopperOneOut > collRsp.OneY_inHopper){
				hopperA.ulCurCount -= collRsp.OneY_inHopper;
			}
		}
	}
	theCOIN_COUNT.SetCoinHopperboxAInfo(hopperA);

	if(wHopperHalfOut == ALL_COIN_COUNT){
		//hopperB.ulCurCount = 0;
		if(hopperB.ulCurCount <= collRsp.HalfY_inHopper){// 实际确实回收完
			hopperB.ulCurCount = 0;
		}
		else if(hopperB.ulCurCount > collRsp.HalfY_inHopper){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
			//hopperB.ulCurCount -= collRsp.wOneHopperCount;
			if(bCollSuccess)
				hopperB.ulCurCount = 0;
			else
				hopperB.ulCurCount -= collRsp.HalfY_inHopper;
		}
	}
	else{
		if(wHopperHalfOut == collRsp.HalfY_inHopper){// 判断预期计数是否与实际相符合
			hopperB.ulCurCount = hopperB.ulCurCount - wHopperHalfOut;
		}
		else{// 不符合，需要按实际的来算
			if(wHopperHalfOut > collRsp.HalfY_inHopper){// 实际清出比预计少，按实际的算
				hopperB.ulCurCount -= collRsp.HalfY_inHopper;
			}
			// 不可能有多的情况，有的话也是上上面一种情况
		}
		
	}
	theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);

	if(wChangeBoxRight == ALL_COIN_COUNT){
		//changeBoxB.ulCount = 0;
		if(changeBoxB.ulCount <= collRsp.HalfY_preparation){
			changeBoxB.ulCount = 0;
		}
		else if(changeBoxB.ulCount > collRsp.HalfY_preparation){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
			//changeBoxB.ulCount -= collRsp.wHalfChangeBoxCount;
			if(bCollSuccess)
				changeBoxB.ulCount = 0;
			else
				changeBoxB.ulCount -= collRsp.HalfY_preparation;
		}
	}
	else{
		if(wChangeBoxRight == collRsp.HalfY_preparation){
			changeBoxB.ulCount -= wChangeBoxRight;
		}
		else{
			if(wChangeBoxRight > collRsp.HalfY_preparation){
				changeBoxB.ulCount -= collRsp.HalfY_preparation;
			}
		}
		//changeBoxB.ulCount = changeBoxB.ulCount - wChangeBoxLeft;
	}
	theCOIN_COUNT.SetChangeCoinboxBInfo(changeBoxB);

	if(wChangeBoxLeft == ALL_COIN_COUNT){
		//changeBoxA.ulCount = 0;
		if(changeBoxA.ulCount <= collRsp.OneY_preparation){
			changeBoxA.ulCount = 0;
		}
		else if(changeBoxA.ulCount > collRsp.OneY_preparation){ // 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
			//changeBoxA.ulCount -= collRsp.wOneChangeBoxCount;
			if(bCollSuccess)
				changeBoxA.ulCount = 0;
			else
				changeBoxA.ulCount -= collRsp.OneY_preparation;
		}
	}
	else{
		if(wChangeBoxLeft == collRsp.OneY_preparation){
			changeBoxA.ulCount -= wChangeBoxLeft;
		}
		else{
			if(wChangeBoxLeft > collRsp.OneY_preparation){
				changeBoxA.ulCount -= collRsp.OneY_preparation;
			}
		}
		//changeBoxA.ulCount = changeBoxA.ulCount - wChangeBoxLeft;
	}
	theCOIN_COUNT.SetChangeCoinboxAInfo(changeBoxA);

	// 回收箱计数更新
	//collectionBoxA.ulCount_1yuan += m_DataModel.CircleAColCount + m_DataModel.CircleBColCount;
	//collectionBoxB.ulCount_1yuan += m_DataModel.ChangeAColCount + m_DataModel.ChangeBColCount;
	// 没有五角 全为一元
	collectionBoxA.ulCount_1yuan += collRsp.OneY_inHopper;
	collectionBoxA.ulCount_1yuan += collRsp.OneY_preparation+collRsp.HalfY_preparation;

	// 判断左边与右边找零箱的面额
	//if(theTVM_INFO.GetCHChangeBoxAType() == 2){		// 一元
	//	collectionBoxB.ulCount_1yuan += collRsp.wOneChangeBoxCount;
	//}
	//else if(theTVM_INFO.GetCHChangeBoxAType() == 1){ // 五角
	//	collectionBoxB.ulCount_5jiao += collRsp.wOneChangeBoxCount;
	//}

	//if(theTVM_INFO.GetCHChangeBoxBType() == 2){		// 一元
	//	collectionBoxB.ulCount_1yuan += collRsp.wHalfChangeBoxCount;
	//}
	//else if(theTVM_INFO.GetCHChangeBoxBType() == 1){ // 五角
	//	collectionBoxB.ulCount_5jiao += collRsp.wHalfChangeBoxCount;
	//}
	/*collectionBoxB.ulCount_1yuan += collRsp.wOneChangeBoxCount;
	collectionBoxB.ulCount_5jiao += collRsp.wHalfChangeBoxCount;*/
	theCOIN_COUNT.SetCollectionCoinboxAInfo(collectionBoxA);
	//theCOIN_COUNT.SetCollectionCoinboxBInfo(collectionBoxB);
	
	
	m_DataModel.boxquantity.DeBoxAAfterColQuantity.Format(_T("%d"),changeBoxA.ulCount);
	m_DataModel.boxquantity.DeBoxBAfterColQuantity.Format(_T("%d"),changeBoxB.ulCount);
	m_DataModel.boxquantity.CyBoxAfterColAQuantity.Format(_T("%d"),hopperA.ulCurCount);
	m_DataModel.boxquantity.CyBoxAfterColBQuantity.Format(_T("%d"),hopperB.ulCurCount);

	// 更新计数
	//DatalightInfo coinData;
	//COIN_HELPER->CoinDataRead(coinData);

	try{
		//COIN_HELPER->CoinDataWrite(coinData);
		COIN_HELPER->CH_SetCoinNum();
	}
	catch(CSysException& e){
		m_DataModel.IsHasException = true;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
	m_DataModel.CollectCoinFlowTime = ThridTFrame;

	if(m_DataModel.ChangeAColCount == 0 &&
		m_DataModel.ChangeBColCount == 0 &&
		m_DataModel.CircleAColCount == 0 &&
		m_DataModel.CircleBColCount == 0 && m_DataModel.IsHasException){
		return false;
	}
	return true;
}