#include "stdafx.h"
#include "CoinBoxManageDetachCoinAddingBoxDlg.h"
#include "CoinBoxManageDetachCoinAddingBoxSvc.h"
#include "CTCoinCountInfo.h"
#include "TemplateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCoinBoxManageDetachCoinAddingBoxSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageDetachCoinAddingBoxSvc::CCoinBoxManageDetachCoinAddingBoxSvc()
	:CTVMForeService(DETACH_ADDING_COIN_BOX_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20111_MACHINE_PARAMETER_SET_DLG, new CCoinBoxManageDetachAddingBoxDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DETACH_ADDING_BOX,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DETACH_ADDING_BOX,IDD_20111_MACHINE_PARAMETER_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DETACH_ADDING_BOX);

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageDetachCoinAddingBoxSvc::~CCoinBoxManageDetachCoinAddingBoxSvc()
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
void CCoinBoxManageDetachCoinAddingBoxSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	m_DataModel = _tagModel();
	
	//// 同步计数，如果需要的话。
	//if(!COIN_HELPER->SyncCoinCountInfoToFile())
	//	m_DataModel.IsHasException = false;

	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	try{
//		COIN_HELPER->ReadStatus(m_common_rsp);
	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsHasException = TRUE;
	}
	DoDetachCoinAddingBoxHandleData();

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DETACH_ADDING_BOX);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_DETACH_ADDING_BOX);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageDetachCoinAddingBoxSvc::Model& CCoinBoxManageDetachCoinAddingBoxSvc::GetDataModel()
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
void CCoinBoxManageDetachCoinAddingBoxSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取硬币循环找零箱币种

@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CCoinBoxManageDetachCoinAddingBoxSvc::GetHopperCoinType(int hopetype)
{
	CString CurrencyType = _T("");
	switch(hopetype)									// 从TVM.INI文件中读取的键值
	{
	case MENU_CODE_0:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//多币种
		break;
	case MENU_CODE_1:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FAVO);		//50AVOS
		break;
	case MENU_CODE_2:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_OMOP);		//1MOP
		break;
	case MENU_CODE_3:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_SMOP);		//2MOP
		break;
	case MENU_CODE_4:
		CurrencyType = _opl(GUIDE_ADDBOX_SET_FMOP);		//5MOP
		break;
	default:
		break;
	}
	return CurrencyType;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     处理卸载硬币补充箱数据 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageDetachCoinAddingBoxSvc::DoDetachCoinAddingBoxHandleData()
{
	//try
	//{
	//	long errCode = 0;
	//	//CH_COMMON_RSP chcommonrsp;
	//	CH_RSP_READ_RFID result;
	//	//errCode = COIN_HELPER->ReadStatus(chcommonrsp);

	//	// 硬币补充箱A未安装
	//	if(FALSE == m_common_rsp.sensor_info.CH_CHANGE_BOX1_SET_STATUS)						
	//	{
	//		m_DataModel.CoinAddingBoxAStatus = m_DataModel.CoinBoxABeforeAddingStatus = _opl(TXT_COINMGR_BOX_DETACHED);				// 硬币补充箱A已卸载
	//		m_DataModel.CoinAddingBoxAID = _T("--------");
	//	}
	//	// 硬币补充箱A已安装
	//	else
	//	{
	//		m_DataModel.CoinAddingBoxAStatus = m_DataModel.CoinBoxABeforeAddingStatus = _opl(TXT_COINMGR_BOX_ATTACHED);				
	//		errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_CHANGE1, result);	
	//		USES_CONVERSION;
	//		m_DataModel.CoinAddingBoxAID = A2T(result.rfid_info.data);						// 硬币补充箱AID
	//	}

	//	// 硬币补充箱B未安装
	//	if(FALSE == m_common_rsp.sensor_info.CH_CHANGE_BOX2_SET_STATUS)						
	//	{
	//		m_DataModel.CoinAddingBoxBStatus = m_DataModel.CoinBoxBBeforeAddingStatus = _opl(TXT_COINMGR_BOX_DETACHED);				// 硬币补充箱B已卸载
	//		m_DataModel.CoinAddingBoxBID = _T("--------");
	//	}
	//	// 硬币补充箱B已安装
	//	else
	//	{
	//		m_DataModel.CoinAddingBoxBStatus = m_DataModel.CoinBoxBBeforeAddingStatus = _opl(TXT_COINMGR_BOX_ATTACHED);				
	//		errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_CHANGE2, result);	
	//		USES_CONVERSION;
	//		m_DataModel.CoinAddingBoxBID = A2T(result.rfid_info.data);						// 硬币补充箱BID
	//	}
	//	m_DataModel.DetachCoinAddingBoxFlowTime = 1;
	//	NotifyDataModelChanged();
	//}
	//// 异常处理
	//catch (CSysException& e) {
	//	m_DataModel.IsHasException = TRUE;
	//	NotifyDataModelChanged();
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch (...) {
	//	m_DataModel.IsHasException = TRUE;
	//	NotifyDataModelChanged();
	//	theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	//}
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
LRESULT CCoinBoxManageDetachCoinAddingBoxSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 有异常直接返回
	//if(m_DataModel.IsHasException){
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	//	return FALSE;
	//}
	//int index = theAPP_SESSION.GetInputToInt();	// 获取向导栏输入内容

	//// 输入内容非法
	//if(index < MENU_CODE_1 || index > MENU_CODE_3)
	//{
	//	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
	//	return TRUE;
	//}
	//// 输入内容合法
	//else
	//{
	//	DoDetachCoinAddingBoxHandleData();
	//	if(MENU_CODE_1 == index){
	//		if(m_common_rsp.sensor_info.CH_CHANGE_BOX1_SET_STATUS){
	//			m_DataModel.bIsRemoveAddingBoxA = true;
	//			m_DataModel.bIsRemoveAddingBoxB = false;
	//			m_DataModel.CoinAddingBoxAStatus = _opl(TXT_TOKENMGR_WAITTODETACH);
	//			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//		}
	//		else{// 提示箱子A未安装
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDING_BOXA_NOT_INSTALLED));
	//		}
	//	}
	//	else if(MENU_CODE_2 == index){
	//		if(m_common_rsp.sensor_info.CH_CHANGE_BOX2_SET_STATUS){
	//			m_DataModel.bIsRemoveAddingBoxA = false;
	//			m_DataModel.bIsRemoveAddingBoxB = true;
	//			m_DataModel.CoinAddingBoxBStatus = _opl(TXT_TOKENMGR_WAITTODETACH);
	//			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//		}
	//		else{// 提示箱子未安装
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDING_BOXB_NOT_INSTALLED));
	//		}			
	//	}
	//	else if(MENU_CODE_3 == index){
	//		if(m_common_rsp.sensor_info.CH_CHANGE_BOX1_SET_STATUS){
	//			m_DataModel.bIsRemoveAddingBoxA = true;
	//			m_DataModel.CoinAddingBoxAStatus = _opl(TXT_TOKENMGR_WAITTODETACH);
	//			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//		}
	//		if(m_common_rsp.sensor_info.CH_CHANGE_BOX2_SET_STATUS){
	//			m_DataModel.bIsRemoveAddingBoxB = true;
	//			m_DataModel.CoinAddingBoxBStatus = _opl(TXT_TOKENMGR_WAITTODETACH);
	//			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	//		}
	//		// A与B都已经卸载
	//		if(!m_common_rsp.sensor_info.CH_CHANGE_BOX1_SET_STATUS &&
	//			!m_common_rsp.sensor_info.CH_CHANGE_BOX2_SET_STATUS){
	//				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ADDING_BOX_NOT_INSTALLED));
	//		}
	//	}
	//	NotifyDataModelChanged();
	//}
	return TRUE;
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
LRESULT CCoinBoxManageDetachCoinAddingBoxSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinBoxManageDetachCoinAddingBoxSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	//if(1 != m_DataModel.DetachCoinAddingBoxFlowTime){
	//	return FALSE;
	//}
	//long errCode = 0;
	//CH_RSP_READ_RFID result;
	//if(m_DataModel.bIsRemoveAddingBoxA && !m_DataModel.bIsRemoveAddingBoxB)
	//{
	//	try
	//	{
	//		errCode = COIN_HELPER->CtrlSafeLock(UNLOCK_A);								// 硬币补充箱A释放
	//		m_DataModel.CoinAddingBoxAfterDetachAID = _T("--------");							// 硬币补充箱AID
	//		m_DataModel.CoinAddingBoxAfterDetachBID = m_DataModel.CoinAddingBoxBID;
	//		m_DataModel.CoinAddingBoxAfterDetachStatus = _opl(TXT_COINMGR_BOX_DETACHED);		// 硬币补充箱A已卸载
	//		m_DataModel.CoinAddingBoxBfterDetachStatus = m_DataModel.CoinAddingBoxBStatus;		// 硬币补充箱B状态
	//		// 上传电文
	//		Sendcables(Coin_Adding_Box1);
	//	}
	//	catch (...)
	//	{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_ADDING_BOX_DETACH_FAILED));	// 硬币补充箱卸载失败
	//		// 本机设置日志
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,false);
	//	}	
	//}
	//else if(!m_DataModel.bIsRemoveAddingBoxA && m_DataModel.bIsRemoveAddingBoxB)
	//{
	//	try
	//	{
	//		errCode = COIN_HELPER->CtrlSafeLock(UNLOCK_B);								// 硬币补充箱B释放
	//		m_DataModel.CoinAddingBoxAfterDetachBID =  _T("--------");							// 硬币补充箱BID
	//		m_DataModel.CoinAddingBoxAfterDetachAID = m_DataModel.CoinAddingBoxAID;
	//		m_DataModel.CoinAddingBoxBfterDetachStatus = _opl(TXT_COINMGR_BOX_DETACHED);		// 硬币补充箱B已卸载
	//		m_DataModel.CoinAddingBoxAfterDetachStatus = m_DataModel.CoinAddingBoxAStatus;		// 硬币补充箱A已卸载
	//		// 上传电文
	//		Sendcables(Coin_Adding_Box2);
	//	}
	//	catch (...)
	//	{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_ADDING_BOX_DETACH_FAILED));	// 硬币补充箱卸载失败
	//		// 本机设置日志
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,false);
	//	}	
	//}
	//else if(m_DataModel.bIsRemoveAddingBoxA && m_DataModel.bIsRemoveAddingBoxB)
	//{
	//	try
	//	{
	//		errCode = COIN_HELPER->CtrlSafeLock(UNLOCK_AB);								// 硬币补充箱A、B释放
	//		m_DataModel.CoinAddingBoxAfterDetachAID =_T("--------");							// 硬币补充箱AID
	//		m_DataModel.CoinAddingBoxAfterDetachBID = _T("--------");							// 硬币补充箱BID
	//		m_DataModel.CoinAddingBoxAfterDetachStatus = _opl(TXT_COINMGR_BOX_DETACHED);		// 硬币补充箱A已卸载
	//		m_DataModel.CoinAddingBoxBfterDetachStatus = _opl(TXT_COINMGR_BOX_DETACHED);		// 硬币补充箱B已卸载
	//		// 上传电文
	//		Sendcables(Coin_Adding_Box1);
	//		Sendcables(Coin_Adding_Box2);
	//	}
	//	catch (...)
	//	{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_ADDING_BOX_DETACH_FAILED));	// 硬币补充箱卸载失败
	//		// 本机设置日志
	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,false);
	//	}
	//}
	//else{
	//	return FALSE;
	//}
	//m_DataModel.DetachCoinAddingBoxFlowTime = 2;
	//NotifyDataModelChanged();
	//// 本机设置日志
	//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,true);
	//
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
void CCoinBoxManageDetachCoinAddingBoxSvc::Sendcables(int boxnumber)
{
	int hopetype = -1;
	CASHBOX_CHANGE  coinbox;
	vector<CASH_TYPE_COUNT> vecCash;

	// 硬币补充箱--操作类型
	coinbox.handleType = UNINSTALL;
	// 硬币箱状态
	coinbox.State = VALID_DETACH;
	// 硬币补充箱币种总数
	coinbox.btCount = 1;
	switch(boxnumber)
	{
		// 硬币补充箱1--ID、位置ID
	case Coin_Adding_Box1:
		//coinbox.SetBoxID(theCOIN_COUNT.GetCoinAddboxAInfo().coinBoxID.bLineCode, theCOIN_COUNT.GetCoinAddboxAInfo().coinBoxID.bType,theCOIN_COUNT.GetCoinAddboxAInfo().coinBoxID.wNum);
		// 硬币补充箱1位置ID
		//coinbox.position = ONE;
		break;

		// 硬币补充箱2--ID、位置ID
	case Coin_Adding_Box2:
		// 硬币补充箱2--ID
		//coinbox.SetBoxID(theCOIN_COUNT.GetCoinAddboxBInfo().coinBoxID.bLineCode, theCOIN_COUNT.GetCoinAddboxBInfo().coinBoxID.bType,theCOIN_COUNT.GetCoinAddboxBInfo().coinBoxID.wNum);
		// 硬币补充箱2位置ID
		//coinbox.position = TWO;
		break;
	}
	// 硬币补充箱币种总数
	//coinbox.GetSumCurrency();
	// 硬币补充箱数量为0
	coinbox.CoinBox_Initial();
	coinbox.btCount = coinbox.GetSumCurrency();

	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(coinbox);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     硬币补充箱卸载凭证打印

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageDetachCoinAddingBoxSvc::PrintCoinBoxExchangeReceipt()
{
	//try
	//{
	//	COIN_DATA coindata;
	//	CString coincount = _T("");			
	//	CString cointotal;

	//	CString CoinBox_templatename = _T("TVM_COINBOX.template");
	//	CStringArray printtext;
	//	CStringArray TemplateContents;

	//	COIN_HELPER->CoinDataRead(coindata);

	//	BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

	//	int Index = 0;
	//	while(Index<TemplateContents.GetCount())
	//	{
	//		CString& line = TemplateContents.ElementAt(Index);	
	//		line.Replace(_T("{安装}"),_T("卸载"));																				// 操作类型
	//		line.Replace(_T("{Install}"),_T("Removed"));																		// 操作类型
	//		line.Replace(_T("{Instalar}"),_T("Removido"));																		// 操作类型

	//		line.Replace(_T("{99FF9991}"),theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString());							// 硬币专用找零箱AID
	//		line.Replace(_T("{99FF9992}"),theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString());							// 硬币专用找零箱BID
	//		line.Replace(_T("{99FF9993}"),theCOIN_COUNT.GetCoinHopperboxCInfo().coinBoxID.ToString());							// 硬币循环找零箱ID
	//		line.Replace(_T("{99FF9994}"),theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString());						// 硬币回收箱AID			
	//		line.Replace(_T("{99FF9995}"),theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString());						// 硬币回收箱BID

	//		line.Replace(_T("{1}"),GetHopperCoinType(theTVM_INFO.GetCHHopper1Type()));											// 硬币专用找零箱A币种
	//		line.Replace(_T("{2}"),GetHopperCoinType(theTVM_INFO.GetCHHopper2Type()));											// 硬币专用找零箱B币种
	//		line.Replace(_T("{3}"),GetHopperCoinType(theTVM_INFO.GetCHStockMoneyType()));										// 硬币循环找零箱币种

	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount);
	//		line.Replace(_T("{H191}"),coincount);																				// 硬币专用找零箱A数量--操作前
	//		line.Replace(_T("{H194}"),coincount);																				// 硬币专用找零箱A数量--操作后

	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount);
	//		line.Replace(_T("{H192}"),coincount);																				// 硬币专用找零箱B数量--操作前
	//		line.Replace(_T("{H195}"),coincount);																				// 硬币专用找零箱B数量--操作后

	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCoinHopperboxCInfo().ulCurCount);
	//		line.Replace(_T("{H193}"),coincount);																				// 硬币循环找零箱数量--操作前
	//		line.Replace(_T("{H196}"),coincount);

	//		// 硬币回收箱A--50AVOS
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao);
	//		line.Replace(_T("{Z191}"),coincount);
	//		line.Replace(_T("{Z195}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao*50);																			 																			
	//		line.Replace(_T("{Z191MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z195MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱A--1MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan);
	//		line.Replace(_T("{Z192}"),coincount);
	//		line.Replace(_T("{Z196}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*100);																			 																			
	//		line.Replace(_T("{Z192MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z196MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱A--2MOP
	//		line.Replace(_T("{Z193}"),_T("0"));
	//		line.Replace(_T("{Z197}"),_T("0"));
	//		line.Replace(_T("{Z193MOP}"),_T("0.00 MOP"));
	//		line.Replace(_T("{Z197MOP}"),_T("0.00 MOP"));

	//		// 硬币回收箱A--5MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan);
	//		line.Replace(_T("{Z194}"),coincount);
	//		line.Replace(_T("{Z198}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan*500);																																					
	//		line.Replace(_T("{Z194MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z198MOP}"),cointotal+_T("MOP"));
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao*50 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*100 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan*500);
	//		line.Replace(_T("{S191MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{S192MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--50AVOS
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao);
	//		line.Replace(_T("{Z291}"),coincount);
	//		line.Replace(_T("{Z295}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxBInfo().ulCount_5jiao*50);																			 																			
	//		line.Replace(_T("{Z291MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z295MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--1MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan);
	//		line.Replace(_T("{Z292}"),coincount);
	//		line.Replace(_T("{Z296}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*100);																			 																			
	//		line.Replace(_T("{Z292MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z296MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--2MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_2yuan);
	//		line.Replace(_T("{Z293}"),coincount);
	//		line.Replace(_T("{Z297}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*200);																			 																			
	//		line.Replace(_T("{Z293MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z297MOP}"),cointotal+_T("MOP"));

	//		// 硬币回收箱B--5MOP
	//		coincount.Format(_T("%d"),theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan);
	//		line.Replace(_T("{Z294}"),coincount);
	//		line.Replace(_T("{Z298}"),coincount);
	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan*500);																																					
	//		line.Replace(_T("{Z294MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{Z298MOP}"),cointotal+_T("MOP"));

	//		cointotal = ComFormatAmount(theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5jiao*50 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_1yuan*100 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_2yuan*200 + theCOIN_COUNT.GetCollectionCoinboxAInfo().ulCount_5yuan*500);
	//		line.Replace(_T("{S291MOP}"),cointotal+_T("MOP"));
	//		line.Replace(_T("{S292MOP}"),cointotal+_T("MOP"));

	//		// 操作后
	//		coincount.Format(_T("%d"),coindata.hopper1_num);
	//		line.Replace(_T("{H194}"),coincount);																				// 硬币专用找零箱A数量--操作后

	//		coincount.Format(_T("%d"),coindata.hopper2_num);
	//		line.Replace(_T("{H195}"),coincount);																				// 硬币专用找零箱B数量--操作后

	//		coincount.Format(_T("%d"),coindata.stock1_num);
	//		line.Replace(_T("{H196}"),coincount);																				// 硬币循环找零箱数量--操作后

	//		line.Replace(_T("{操作時間}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
	//		line.Replace(_T("{操作員編號}"),theAPP_SESSION.GetUserInfo().sUserID);
	//		line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
	//		line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
	//		line.Replace(_T("{Nome_da_estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
	//		line.Replace(_T("{設備編號}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

	//		CStringArray receiptline;
	//		ComPrintLineFormatForTemplate(line,receiptline);
	//		printtext.Append(receiptline);
	//		Index++;
	//	}
	//	PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//}
	//catch(CSysException& e) {
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//	theEXCEPTION_MGR.ProcessException(e);
	//	m_DataModel.IsPrintFailed = true;
	//	//throw;
	//}
	//catch (...) {
	//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//	theEXCEPTION_MGR.ProcessException( CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//	m_DataModel.IsPrintFailed = true;
	//}

}