#include "StdAfx.h"
#include "OneKeySettlementSvc.h"
#include "OneKeySettlementDlg.h"
#include "TemplateHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString NULL_BOX_ID = _T("FFFFFFFF");
#define ALL_COIN_COUNT 0xFFFF

#define SHOW_GUIDE	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC))
#define SHOW_ERROR	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL))

BEGIN_MESSAGE_MAP(COneKeySettlementSvc, CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COneKeySettlementSvc::COneKeySettlementSvc():CTVMForeService(ONE_KEY_SETTLEMENT_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20701_REMOVE_EXCEPTION_DLG, new COneKeySettlementDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION,_opl(DEVICESTATUS_ENTER_START_ONEKEY));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REMOVE_EXCEPTION, IDD_20701_REMOVE_EXCEPTION_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COneKeySettlementSvc::~COneKeySettlementSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementSvc::OnStart(){
	__super::OnStart();
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);
	SHOW_GUIDE;
	m_settings = theTVM_SETTING.GetSettleSettings();		// 从配置文件读取收机配置
	theAPP_SESSION.SetMaxInputLength(2);

	m_modle = _tag_data_modol();
	m_modle.InitializeMap();							// 初始化容器
	m_modle.bPreStatus = true;							// 当前状态为初始化状态
	//InitBoxOpTime();
	LoadSetting(m_settings);							// 收机配置
	GetStates();										// 判断状态

	//// 同步计数，如果需要的话。
	//COIN_HELPER->SyncCoinCountInfoToFile();


	m_ex_coin_changeA = theCOIN_COUNT.GetChangeCoinboxAInfo();
	m_ex_coin_changeB = theCOIN_COUNT.GetChangeCoinboxBInfo();

	m_ex_coin_hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
	m_ex_coin_hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();

	m_ex_coin_collectA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	m_ex_coin_collectB = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	m_ex_note_collect = theBANKNOTE_COUNT.GetBanknoteboxInfo();

	m_ex_note_change = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	m_ex_note_recycleA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	m_ex_note_recycleB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	m_ex_note_recycleC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	m_ex_note_recycleD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

	/*m_ex_note_changeB = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
	m_ex_note_invalid = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();*/
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
COneKeySettlementSvc::Model& COneKeySettlementSvc::GetDataModol()
{
	return m_modle;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      更新窗体显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementSvc::NotifyDataModelChanged()
{
	//__super::NotifyDataModelChanged();
	COneKeySettlementDlg* pDlg = dynamic_cast<COneKeySettlementDlg*>(m_pDialogFlow->GetDialog(COneKeySettlementDlg::IDD));
	pDlg->UpdateLabelData();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      确认按键（异常解除）

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT COneKeySettlementSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	int nInput = theAPP_SESSION.GetInputToInt();
	if (nInput == 8){
		m_settings = theTVM_SETTING.GetSettleSettings();
		LoadSetting(m_settings);
		GetStates();
		SHOW_GUIDE;
		NotifyDataModelChanged();
		return 1;
	}
	else if(nInput >=73 || nInput <= 0){// 初步判断
		SHOW_ERROR;
		return 1;
	}
	int nIndex	= nInput /10;		// 操作项
	int nSwitch = nInput %10;		// 开关
	CString strError = _T("");
	switch(nIndex)
	{
	case 1:
		if (nSwitch == 1){// 11 开启 卸载票箱
			m_settings |= PROC_REMOVE_CARDBOX;
		}
		else if (nSwitch == 2){// 12 
			m_settings &= PROC_NO_REMOVE_CARDBOX;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;
	case 2: // 回收硬币
		if (nSwitch == 1){
			m_settings |= PROC_COLLECT_COIN;
		}
		else if (nSwitch == 2){
			m_settings &= PROC_NO_COLLECT_COIN;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;
		
	case 3:// 卸载硬币找零箱
		if (nSwitch == 1){
			m_settings |= PROC_REMOVE_CN_CHGBOX;			
		}
		else if (nSwitch == 2){
			m_settings &= PROC_NO_REMOVE_CN_CHGBOX;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;

	//case 4:	//卸载硬币回收箱
	//	if (nSwitch == 1){
	//		m_settings |= PROC_REMOVE_CN_COLBOX;
	//	}
	//	else if (nSwitch == 2){
	//		m_settings &= PROC_NO_REMOVE_CN_COLBOX;
	//	}
	//	else{
	//		SHOW_ERROR;
	//	}
	//	break;
	case 4: // 回收纸币
		if (nSwitch == 1){
			m_settings |= PROC_COLLECT_CASH;
		}
		else if (nSwitch == 2){
			m_settings &= PROC_NO_COLLECT_CASH;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;

	case 5:	// 卸载纸币回收箱
		if (nSwitch == 1){
			m_settings |= PROC_REMOVE_BN_COLBOX;
		}
		else if (nSwitch == 2){
			m_settings &= PROC_NO_REMOVE_BN_COLBOX;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;

	case 6: // 卸载纸币找零箱
		if (nSwitch == 1){
			m_settings |= PROC_REMOVE_BN_CHGBOX;
		}
		else if (nSwitch == 2){
			m_settings &= PROC_NO_REMOVE_BN_CHGBOX;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;

	case 7: // 结算
		if (nSwitch == 1){
			m_settings |= PROC_BALANCE;
		}
		else if (nSwitch == 2){
			m_settings &= PROC_NO_BALANCE;
		}
		else{
			SHOW_ERROR;return 0;
		}
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return 0;
	}
	if (strError.GetLength() == 0){
		strError = _opl(GUIDE_ENTER_F2_F3_ESC);
	}
	LoadSetting(m_settings);
	GetStates();
	SHOW_GUIDE;
	NotifyDataModelChanged();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      确认按键（异常解除）

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT COneKeySettlementSvc::OnKeyboardF2(WPARAM , LPARAM )
{
	theTVM_SETTING.SetSettleSettings(m_settings);
	if(!m_modle.bPreStatus){
		theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ONEKEY_SUCCESS_NOT_AGAIN));
		return FALSE;
	}

	m_modle.bPreStatus = false;
	theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ONEKEY_HAVEINHAND));
	theSERVICE_MGR.SetForeServiceBusy(true);
	CString strError = _T("");
	// 卸载票箱
	if ((m_settings & PROC_REMOVE_CARDBOX) == PROC_REMOVE_CARDBOX && m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState != DONE){
		m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState = PROCESSING;
		NotifyDataModelChanged();
		RemoveCardBox();
		GetState(PROC_REMOVE_CARDBOX,strError);
		NotifyDataModelChanged();
	}
	// 回收硬币
	if ((m_settings & PROC_COLLECT_COIN) == PROC_COLLECT_COIN  && m_modle.mapSettlement[PROC_COLLECT_COIN].procState != DONE)
	{
		m_modle.mapSettlement[PROC_COLLECT_COIN].procState = PROCESSING;
		NotifyDataModelChanged();
		CoinCollection();
		GetState(PROC_COLLECT_COIN,strError);
		NotifyDataModelChanged();
	}
	// 卸载硬币找零箱
	if ((m_settings & PROC_REMOVE_CN_CHGBOX) == PROC_REMOVE_CN_CHGBOX && m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState != DONE)
	{
		m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState = PROCESSING;
		NotifyDataModelChanged();
		RemoveCoinChangeAndCollectionBox();
		GetState(PROC_REMOVE_CN_CHGBOX,strError);
		NotifyDataModelChanged();
	}
	//// 卸载硬币回收箱
	//if ((m_settings & PROC_REMOVE_CN_COLBOX) == PROC_REMOVE_CN_COLBOX)
	//{
	//	m_modle.mapSettlement[PROC_REMOVE_CN_COLBOX].procState = PROCESSING;
	//	NotifyDataModelChanged();
	//	RemoveCoinCollectBox();
	//	GetState(PROC_REMOVE_CN_COLBOX,strError);
	//	NotifyDataModelChanged();
	//}
	// 回收纸币
	if ((m_settings & PROC_COLLECT_CASH) == PROC_COLLECT_CASH  && m_modle.mapSettlement[PROC_COLLECT_CASH].procState != DONE)
	{
		m_modle.mapSettlement[PROC_COLLECT_CASH].procState = PROCESSING;
		NotifyDataModelChanged();
		CollectBanknote();
		GetState(PROC_COLLECT_CASH,strError);
		NotifyDataModelChanged();
	}
	// 卸载纸币回收箱
	if ((m_settings & PROC_REMOVE_BN_COLBOX) == PROC_REMOVE_BN_COLBOX && m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState != DONE)
	{
		m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState = PROCESSING;
		NotifyDataModelChanged();
		RemoveBanknoteCollectBox();
		GetState(PROC_REMOVE_BN_COLBOX,strError);
		NotifyDataModelChanged();
	}
	// 卸载纸币找零箱
	if ((m_settings & PROC_REMOVE_BN_CHGBOX) == PROC_REMOVE_BN_CHGBOX && m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState != DONE)
	{
		m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState = PROCESSING;
		NotifyDataModelChanged();
		RemoveBanknoteChangeBox();
		GetState(PROC_REMOVE_BN_CHGBOX,strError);
		NotifyDataModelChanged();
	}
	// 结算 所选的项目都执行成功菜进行结算
	if (!BalanceOrNot()){
		theTVM_SETTING.SetBalanceOpTime(_T("0"));// 设置失败
		GetState(PROC_BALANCE,strError);	 // 取状态
		theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ONEKEY_SUCCESS));
		Sleep(1000);
		SHOW_GUIDE;
		NotifyDataModelChanged();
		theSERVICE_MGR.SetForeServiceBusy(false);
		return 1;
	}
	if ((m_settings & PROC_BALANCE) == PROC_BALANCE)
	{
		m_modle.mapSettlement[PROC_BALANCE].procState = PROCESSING;
		NotifyDataModelChanged();
		Balance();
		GetState(PROC_BALANCE,strError);
		NotifyDataModelChanged();
	}
	else{
		// 其他执行成功，但是未选择业结，需要改变状态
		theSERVICE_MGR.SetForeServiceBusy(false);
	}

	// 配置存储失败项
	theAPP_SESSION.ShowOfficeGuide(_opl(DEVICESTATUS_ONEKEY_SUCCESS));
	Sleep(1000);
	SHOW_GUIDE;
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT COneKeySettlementSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	theAPP_SESSION.ReturnMaintainMenu();
	
	return 0;
}


bool COneKeySettlementSvc::CoinCollection()
{
	bool result = false;
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();
	CTCoinCountInfo::COIN_BOX_INFO			  mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	CTCoinCountInfo::COIN_BOX_INFO			  mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	//CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	// 实际回收数量
	ChangeAColCount = ChangeBColCount = CircleAColCount = CircleBColCount = 0;

	// 判断模块是否满足回收条件
	tCHADevStatus coin_info;
	try{
		COIN_HELPER->CH_GetStatus(&coin_info);
		// 模块必须到位
		if(!coin_info.bIsCoinModuleStandby){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_NOT_INPLACE));
			Sleep(1000);
			theTVM_SETTING.SetCollectCoinResult(0);
			return false;
		}
		// 回收箱必须到位
		if(!coin_info.bIsCoinModuleStandby){
			theAPP_SESSION.ShowOfficeGuide(_T("硬币回收箱未到位，回收硬币失败"));
			Sleep(1000);
			theTVM_SETTING.SetCollectCoinResult(0);
			return false;
		}
		// 回收箱必须安装
		if(theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString()==INVALID_ID){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOT_INSTALL));
			Sleep(1000);
			theTVM_SETTING.SetCollectCoinResult(0);
			return false;
		}
		//// 模块非忙碌状态
		//if(coin_info.bBusyRecycle){
		//	theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COIN_COLLECTING));
		//	Sleep(1000);
		//	theSETTING.SetCollectCoinResult(0);
		//	return false;
		//}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		Sleep(1000);
		theTVM_SETTING.SetCollectCoinResult(0);
		return false;
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		Sleep(1000);
		theTVM_SETTING.SetCollectCoinResult(0);
		return false;
	}

	if( CollectCoin()){
		theTVM_SETTING.SetCollectCoinResult(1);
		result = true;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,true);
		
		//---------------------------上传电文---------------------------------------
		vector<CASH_TYPE_COUNT> cashData;
		CASH_TYPE_COUNT cashTypeOne,cashTypeHalf;
		// 计算清出的数
		// 循环找零箱A
		if(CircleAColCount > 0){
			cashTypeHalf.cash_type  = Coin1Yuan;
			cashTypeHalf.cash_count = CircleAColCount;
			cashData.push_back(cashTypeHalf);
		}
		// 循环找零箱B
		if(CircleBColCount > 0){
			cashTypeOne.cash_type  = Coin1Yuan;
			cashTypeOne.cash_count = CircleBColCount;
			cashData.push_back(cashTypeOne);
		}

		// 备用找零箱A
		if(ChangeAColCount > 0){
			BankNoteAndCoinType_t coinChangeBoxAType = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
			if(coinChangeBoxAType == CoinHalfYuan){
				cashTypeHalf.cash_type = CoinHalfYuan;
				cashTypeHalf.cash_count += ChangeAColCount;
			}
			else if(coinChangeBoxAType == Coin1Yuan){
				cashTypeOne.cash_type = Coin1Yuan;
				cashTypeOne.cash_count += ChangeAColCount;
			}
		}

		// 备用找零箱B
		if(ChangeBColCount > 0){
			BankNoteAndCoinType_t coinChangeBoxBType = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
			if(coinChangeBoxBType == CoinHalfYuan){
				cashTypeHalf.cash_type = CoinHalfYuan;
				cashTypeHalf.cash_count += ChangeBColCount;
			}
			else if(coinChangeBoxBType == Coin1Yuan){
				cashTypeOne.cash_type = Coin1Yuan;
				cashTypeOne.cash_count += ChangeBColCount;
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

		//---------------------------------------------插入数据库------------------------------------------
		try{
			CoinCollectOperationRecord collectRecord;
			collectRecord.dtOperationTime = ComGetCurTime();

			// 循环找零箱A有回收
			if(CircleAColCount > 0){
				collectRecord.iCoinTypeCode				= CoinHalfYuan;
				collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();
				collectRecord.iQuantityBeforeCollect	= mEx_hopper_A.ulCurCount;
				collectRecord.iQuantityCollect			= CircleAColCount;
				collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;
				collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
				collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;
				DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
			}

			// 循环找零箱B有回收
			if(CircleBColCount > 0){
				collectRecord.iCoinTypeCode				= Coin1Yuan;
				collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();
				collectRecord.iQuantityBeforeCollect	= mEx_hopper_B.ulCurCount;
				collectRecord.iQuantityCollect			= CircleBColCount;
				collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;
				collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
				collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;
				DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
			}
			// 硬币备用找零箱A有回收
			if(ChangeAColCount > 0){
				collectRecord.iCoinTypeCode				= TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
				collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
				collectRecord.iQuantityBeforeCollect	= mEx_change_A.ulCount;
				collectRecord.iQuantityCollect			= ChangeAColCount;
				collectRecord.iQuantityAfterCollect		= theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
				collectRecord.txtCoinCollectBoxID		= theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
				collectRecord.txtOperatorID				= theAPP_SESSION.GetUserInfo().sUserID;

				DATA_HELPER->DbInsertCoinCollectOperationRecord(collectRecord);
			}

			// 硬币备用找零箱B有回收
			if(ChangeBColCount > 0){
				collectRecord.iCoinTypeCode				= TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
				collectRecord.txtCoinChangeBoxID		= theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
				collectRecord.iQuantityBeforeCollect	= mEx_change_B.ulCount;
				collectRecord.iQuantityCollect			= ChangeBColCount;
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
	else{
		theTVM_SETTING.SetCollectCoinResult(0);
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_RECYCLE,false);
		return false;
	}

	// 打印回收凭证
	try
	{
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

		CTCoinCountInfo::COIN_BOX_INFO hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
		CTCoinCountInfo::COIN_BOX_INFO hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();

		//tCHADevStatus coindata;
		CString coincount = _T("");			
		CString cointotal;

		CString CoinBox_templatename = _T("TVM_COINBOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;

		//COIN_HELPER->CoinDataRead(coindata);

		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

		DWORD dwTotalAmountBefore = (mEx_change_A.ulCount + mEx_change_B.ulCount) * 100 /*+ mEx_hopper_A.ulCurCount * 50*/ + mEx_hopper_B.ulCurCount * 100 + mEx_collect_A.GetCurAmount()/* + mEx_collect_B.GetCurAmount()*/;
		DWORD dwTotalAmountAfter  = (change_A.ulCount + change_B.ulCount) * 100 /*+ hopper_A.ulCurCount * 50*/ + hopper_B.ulCurCount * 100 + collect_A.GetCurAmount() /*+ collect_B.GetCurAmount()*/;

		int Index = 0;
		CString strCount;
		int nCount = TemplateContents.GetCount()/2;

		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{安装}"),_T("回收"));							// 操作类型

			line.Replace(_T("{面值1}"),CashValueToString(TransCoinType(theTVM_INFO.GetCHChangeBoxAType())));
			line.Replace(_T("{面值2}"),CashValueToString(TransCoinType(theTVM_INFO.GetCHChangeBoxBType())));
			line.Replace(_T("{面值3}"),CashValueToString(TransCoinType(theTVM_INFO.GetCHHopper2Type())));
			line.Replace(_T("{面值4}"),CashValueToString(TransCoinType(theTVM_INFO.GetCHHopper1Type())));


			// 操作前合计金额
			line.Replace(_T("{前合计}"),ComFormatAmount(dwTotalAmountBefore));

			// 操作前 ID
			line.Replace(_T("{前ID1}"),mEx_change_A.coinBoxID.ToString());
			line.Replace(_T("{前ID2}"),mEx_change_B.coinBoxID.ToString());
			//line.Replace(_T("{前ID3}"),mEx_hopper_A.coinBoxID.ToString());
			line.Replace(_T("{前ID4}"),mEx_hopper_B.coinBoxID.ToString());
			line.Replace(_T("{前ID5}"),mEx_collect_A.coinBoxID.ToString());
			//line.Replace(_T("{前ID6}"),mEx_collect_B.coinBoxID.ToString());

			// 操作前 数量
			line.Replace(_T("{前数量1}"),ComFormatCount(mEx_change_A.ulCount     ));
			line.Replace(_T("{前数量2}"),ComFormatCount(mEx_change_B.ulCount     ));
			//line.Replace(_T("{前数量3}"),ComFormatCount(mEx_hopper_A.ulCurCount  ));
			line.Replace(_T("{前数量4}"),ComFormatCount(mEx_hopper_B.ulCurCount  ));
			line.Replace(_T("{前数量5}"),ComFormatCount(mEx_collect_A.ulCount_1yuan));
			//line.Replace(_T("{前数量6}"),ComFormatCount(mEx_collect_A.ulCount_5jiao));
			//line.Replace(_T("{前数量7}"),ComFormatCount(mEx_collect_B.ulCount_1yuan));
			//line.Replace(_T("{前数量8}"),ComFormatCount(mEx_collect_B.ulCount_5jiao));
			line.Replace(_T("{前总数量A}"),ComFormatCount(mEx_collect_A.GetCurCount()));
			//line.Replace(_T("{前总数量B}"),ComFormatCount(mEx_collect_B.GetCurCount()));


			// 操作后 

			// 数据有更新
			if(Index > nCount){
				// 数量 || ID 不同
				if(mEx_change_A.ulCount != change_A.ulCount || change_A.coinBoxID.ToString() != mEx_change_A.coinBoxID.ToString()){
					line.Replace(_T("硬币找零箱A"),_T("*硬币找零箱A"));
				}

				if(mEx_change_B.ulCount != change_B.ulCount || change_B.coinBoxID.ToString() != mEx_change_B.coinBoxID.ToString()){
					line.Replace(_T("硬币找零箱B"),_T("*硬币找零箱B"));
				}

				//if(mEx_hopper_A.ulCurCount != hopper_A.ulCurCount){
				//	line.Replace(_T("循环找零箱A"),_T("*循环找零箱A"));
				//}

				if(mEx_hopper_B.ulCurCount != hopper_B.ulCurCount){
					line.Replace(_T("循环找零箱"),_T("*循环找零箱"));
				}

				if(mEx_collect_A.GetCurCount() != collect_A.GetCurCount() || mEx_collect_A.coinBoxID.ToString() != collect_A.coinBoxID.ToString()){
					line.Replace(_T("回收箱"),_T("*回收箱"));
				}

				//if(mEx_collect_B.GetCurCount() != collect_B.GetCurCount() || mEx_collect_B.coinBoxID.ToString() != collect_B.coinBoxID.ToString()){
				//	line.Replace(_T("回收箱B"),_T("*回收箱B"));
				//}
			}

			// 操作后总金额
			line.Replace(_T("{后合计}"),ComFormatAmount(dwTotalAmountAfter));

			// 操作后 ID
			line.Replace(_T("{后ID1}"),change_A.coinBoxID.ToString());
			line.Replace(_T("{后ID2}"),change_B.coinBoxID.ToString());
			//line.Replace(_T("{后ID3}"),hopper_A.coinBoxID.ToString());
			line.Replace(_T("{后ID4}"),hopper_B.coinBoxID.ToString());
			line.Replace(_T("{后ID5}"),collect_A.coinBoxID.ToString());
			//line.Replace(_T("{后ID6}"),collect_B.coinBoxID.ToString());

			// 操作后 数量
			line.Replace(_T("{后数量1}"),ComFormatCount(change_A.ulCount    ));
			line.Replace(_T("{后数量2}"),ComFormatCount(change_B.ulCount    ));
			//line.Replace(_T("{后数量3}"),ComFormatCount(hopper_A.ulCurCount ));
			line.Replace(_T("{后数量4}"),ComFormatCount(hopper_B.ulCurCount ));
			line.Replace(_T("{后数量5}"),ComFormatCount(collect_A.ulCount_1yuan));
			//line.Replace(_T("{后数量6}"),ComFormatCount(collect_A.ulCount_5jiao));
			//line.Replace(_T("{后数量7}"),ComFormatCount(collect_B.ulCount_1yuan));
			//line.Replace(_T("{后数量8}"),ComFormatCount(collect_B.ulCount_5jiao));
			line.Replace(_T("{后总数量A}"),ComFormatCount(collect_A.GetCurCount()));
			//line.Replace(_T("{后总数量B}"),ComFormatCount(collect_B.GetCurCount()));

			line.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			line.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
			line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			//line.Replace(_T("{Station_Name}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			//line.Replace(_T("{Nome_da_estação}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			line.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLECTION);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      回收硬币

@param      none

@retval     回收硬币操作的结果

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::CollectCoin()
{
	// 1. 检查硬件状态及箱子到位情况
	IO_RSP_STATUS_INFO rsp;
	tCHADevStatus chrsp;
	try{
		IO_HELPER->GetIOStatus(rsp);
		COIN_HELPER->CH_GetStatus(&chrsp);

	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
		Sleep(1000);
		return false;
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
		Sleep(1000);
		return false;
	}

	// 回收箱不在位
	if(!rsp.isCoinboxReady)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOINSTALL));
		Sleep(1000);
		return false;
	}
	// 硬币箱找零箱安装了但是不在位
	if(chrsp.bIsChangeBoxAInstall && theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() != INVALID_ID ||
		chrsp.bIsChangeBoxBInstall && theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() != INVALID_ID){
			theAPP_SESSION.ShowOfficeGuide(_T("（红）找零箱非法卸载，此时无法进行回收操作！"));
			Sleep(1000);
			theTVM_SETTING.SetCollectCoinResult(0);
			return false;
	}

	// 2. 检查软计数
	CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();// 五角
	CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();// 一元
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxA = theCOIN_COUNT.GetCollectionCoinboxAInfo();//右边
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBoxA = theCOIN_COUNT.GetChangeCoinboxAInfo();//右边
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBoxB = theCOIN_COUNT.GetChangeCoinboxBInfo();//左边

	WORD wCollFullNumber = theAFC_TVM_OPERATION.GetRecycleBoxFullCount();
	if (wCollFullNumber <= (hopperA.ulCurCount + hopperB.ulCurCount + collectionBoxA.GetCurCount() + changeBoxA.ulCount + changeBoxB.ulCount)){
		theAPP_SESSION.ShowOfficeGuide(_T("硬币回收箱容量不足，请更换回收箱")/*_opl(TXT_COINMGR_COLLECTION_BOX_ALREADYFULL)*/);
		Sleep(1000);
		return false;
	}

	// 3.开始回收
	bool bCollSuccess  = true;
	long errCode = 0;
	tCoinBoxInfo boxinfo;
	try{
		errCode = COIN_HELPER->CH_ClearCoin(&boxinfo);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		bCollSuccess = false;
	}
	catch(...){
		bCollSuccess = false;
	}
	if (bCollSuccess==false && boxinfo.OneY_inHopper==0 && boxinfo.HalfY_inHopper==0 && boxinfo.OneY_preparation == 0 && boxinfo.HalfY_preparation == 0){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
		Sleep(1000);
		return false;
	}

	// 4.分析回收结果
	// 实际回收数量
	CircleAColCount = boxinfo.OneY_inHopper;
	CircleBColCount = boxinfo.HalfY_inHopper;
	ChangeAColCount = boxinfo.OneY_preparation;
	ChangeBColCount = boxinfo.HalfY_preparation;
	// 只相信软计数，有可能实际与计数不符合
	if(hopperA.ulCurCount <= CircleAColCount){// 实际确实回收完
		hopperA.ulCurCount = 0;
	}
	else if(hopperA.ulCurCount >CircleAColCount){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
		if(bCollSuccess)
			hopperA.ulCurCount = 0;
		else
			hopperA.ulCurCount -= CircleAColCount;
	}
	theCOIN_COUNT.SetCoinHopperboxAInfo(hopperA);
	theTVM_STATUS_MGR.SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);

	if(hopperB.ulCurCount <= CircleBColCount){// 实际确实回收完
		hopperB.ulCurCount = 0;
	}
	else if(hopperB.ulCurCount >CircleBColCount){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
		if(bCollSuccess)
			hopperB.ulCurCount = 0;
		else
			hopperB.ulCurCount -= CircleBColCount;
	}
	theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);
	theTVM_STATUS_MGR.SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
	if(changeBoxA.ulCount <= ChangeAColCount){// 实际确实回收完
		changeBoxA.ulCount = 0;
	}
	else if(changeBoxA.ulCount >ChangeAColCount){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
		if(bCollSuccess)
			changeBoxA.ulCount = 0;
		else
			changeBoxA.ulCount -= ChangeAColCount;
	}
	theCOIN_COUNT.SetChangeCoinboxAInfo(changeBoxA);
	theTVM_STATUS_MGR.SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
	if(changeBoxB.ulCount <= ChangeBColCount){// 实际确实回收完
		changeBoxB.ulCount = 0;
	}
	else if(changeBoxB.ulCount >ChangeBColCount){// 实际没有回收完，但是也要将计数清零（体现在回收单上，实际回收）
		if(bCollSuccess)
			changeBoxB.ulCount = 0;
		else
			changeBoxB.ulCount -= ChangeBColCount;
	}
	theCOIN_COUNT.SetChangeCoinboxBInfo(changeBoxB);
	theTVM_STATUS_MGR.SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);

	// 回收箱计数更新
	collectionBoxA.ulCount_1yuan += CircleAColCount+ChangeAColCount+ChangeBColCount;
	collectionBoxA.ulCount_5jiao += CircleBColCount;
	theCOIN_COUNT.SetCollectionCoinboxAInfo(collectionBoxA);

	// 6. 更新硬件计数
	try{
		COIN_HELPER->CH_SetCoinNum();
	}
	catch(CSysException& e){
		CString strLog;
		strLog.Format(_T("更新硬币模块硬件计数失败"));
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CString strLog;
		strLog.Format(_T("更新硬币模块硬件计数失败"));
		theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
	}

	return true;
}


// 回收token
bool COneKeySettlementSvc::CollectToken()
{
	bool bRet = true;
	CTTokenCountInfo::TOKEN_BOX_INFO hopperAInfo = theTOKEN_COUNT.GetHopperAInfo();
	CTTokenCountInfo::TOKEN_BOX_INFO hopperBInfo = theTOKEN_COUNT.GetHopperBInfo();
	CTTokenCountInfo::TOKEN_BOX_INFO wastedInfo = theTOKEN_COUNT.GetTokenboxWastedInfo();
	CTTokenCountInfo::TOKEN_BOX_INFO collectedInfo = theTOKEN_COUNT.GetTokenboxCollectedInfo();

	CTTokenCountInfo::TOKEN_BOX_INFO hopperAInfoAfter = hopperAInfo;
	CTTokenCountInfo::TOKEN_BOX_INFO hopperBInfoAfter = hopperBInfo;

	UINT nCleanNumA = 0;
	UINT nCleanNumB = 0;

	// 1. 检查箱子在位情况
	TOKEN_RSP_GET_DEV_STATUS status;
	IO_RSP_STATUS_INFO io_status;
	try{

		TOKEN_HELPER->GetDevStatus(&status);
		IO_HELPER->GetIOStatus(io_status);
		// 回收箱物理必须在位，且正常安装
		if (!io_status.isTokenColBoxReady || collectedInfo.tokenBoxID.ToString()==INVALID_ID){
			theAPP_SESSION.ShowOfficeGuide(_T("回收箱安装错误，回收token失败"));
			Sleep(1000);
			theTVM_SETTING.SetCollectTokenResult(0);
			return false;
		}
		// 暂存区若有票，废票箱必须在位，且正常安装
		if(status.bHasCardAtTemporaryA || status.bHasCardAtTemporaryB){
			if (!io_status.isTokenInvBoxReady || wastedInfo.tokenBoxID.ToString()==INVALID_ID){
				theAPP_SESSION.ShowOfficeGuide(_T("废票箱安装错误，回收token失败"));	
				Sleep(1000);
				theTVM_SETTING.SetCollectTokenResult(0);
				return false;
			}
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("token回收失败"));
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("token回收失败"));
	}


	LONG iCountA = 0;
	// 2. 回收Hopper箱A中的token
	try{
		// 使用清空命令回收token
		TOKEN_HELPER->CleanOut(TOKEN_BOX_A);
		iCountA = hopperAInfo.ulCurCount;
		if(status.bHasCardAtTemporaryA){
			iCountA--;
			try{
				TOKEN_HELPER->CardOut(TOKEN_BOX_A, &nCleanNumA);
				TOKEN_HELPER->RetractCard(&nCleanNumA);
				wastedInfo.ulCurCount += nCleanNumA;
				theTOKEN_COUNT.SetTokenboxWastedInfo(wastedInfo);
				iCountA++;
			}
			catch(...){

			}
		}
		hopperAInfoAfter.ulCurCount = hopperAInfo.ulCurCount - iCountA;
		// token回收记录
		TokenCollectOperationRecord hopperAColl;
		hopperAColl.dtOperationTime			= ComGetCurTime();
		hopperAColl.txtTokenBoxID			= hopperAInfo.tokenBoxID.ToString();
		hopperAColl.iQuantityBeforeCollect	= hopperAInfo.ulCurCount;
		hopperAColl.iQuantityCollect		= iCountA;
		hopperAColl.iQuantityAfterCollect	= hopperAInfoAfter.ulCurCount;
		hopperAColl.txtTokenCollectBoxID	= collectedInfo.tokenBoxID.ToString();
		hopperAColl.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
		hopperAColl.txtReserve				= _T("");
		hopperAColl.txtComment				= _T("");
		try{
			DATA_HELPER->DbInsertTokenCollectOperationRecord(hopperAColl);
		}
		catch(...){
			CString strLog;
			strLog.Format(_T("插入token回收记录到数据库失败！"));
			theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
		}

		// 更新计数和状态
		collectedInfo.ulCurCount += hopperAColl.iQuantityBeforeCollect;
		theTOKEN_COUNT.SetTokenboxCollectedInfo(collectedInfo);
		theTOKEN_COUNT.SetHopperAInfo(hopperAInfoAfter);
		theTVM_STATUS_MGR.SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("回收token失败"));
		Sleep(1000);
		bRet = false;	
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("回收token失败"));
		Sleep(1000);
		bRet = false;	
	}	
	// 记录操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,bRet);
	if(!bRet){
		theTVM_SETTING.SetCollectTokenResult(0);
		return false;
	}

	// 打印回收hopper A的凭证
	try{
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		CString strTicketCount_before;
		CString strTicketCount_after;
		strOperatorID_before.Format(_T("%06d"), hopperAInfo.ulOperatorID);
		strOperatorID_after = theAPP_SESSION.GetUserInfo().sUserID;
		strTicketCount_before.Format(_T("%d"),  hopperAInfo.ulCurCount);
		strTicketCount_after.Format(_T("%d"), hopperAInfoAfter.ulCurCount);
		// 加载Template
		CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	

			line.Replace(_T("{更换票箱}"), _T("回收票箱"));
			line.Replace(_T("{票箱}"), _T("Hopper A"));
			line.Replace(_T("{操作前票箱号}"), 	hopperAInfo.tokenBoxID.ToString());
			line.Replace(_T("{操作后票箱号}"), hopperAInfoAfter.tokenBoxID.ToString());
			line.Replace(_T("{操作前数量}"), strTicketCount_before);
			line.Replace(_T("{操作后数量}"), strTicketCount_after);
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), theAPP_SESSION.GetUserInfo().sUserID);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), hopperAInfo.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		// 开始打印
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
		//Sleep(3000);//等待打印机完成打印
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("Hopper A回收凭证打印失败"));
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("Hopper A回收凭证打印失败"));
	}


	LONG iCountB = 0;
	// 3. 回收Hopper箱B中的token
	try{
		// 使用清空命令回收token
		TOKEN_HELPER->CleanOut(TOKEN_BOX_B);
		iCountB = hopperBInfo.ulCurCount;
		if(status.bHasCardAtTemporaryB){
			iCountB--;
			try{
				TOKEN_HELPER->CardOut(TOKEN_BOX_B, &nCleanNumB);
				TOKEN_HELPER->RetractCard(&nCleanNumB);
				wastedInfo.ulCurCount += nCleanNumB;
				theTOKEN_COUNT.SetTokenboxWastedInfo(wastedInfo);
				iCountB++;
			}
			catch(...){

			}
		}
		hopperBInfoAfter.ulCurCount = hopperBInfo.ulCurCount - iCountB;
		// token回收记录
		TokenCollectOperationRecord hopperBColl;
		hopperBColl.dtOperationTime			= ComGetCurTime();
		hopperBColl.txtTokenBoxID			= hopperBInfo.tokenBoxID.ToString();
		hopperBColl.iQuantityBeforeCollect	= hopperBInfo.ulCurCount;;
		hopperBColl.iQuantityCollect		= iCountB;
		hopperBColl.iQuantityAfterCollect	= hopperBInfoAfter.ulCurCount;
		hopperBColl.txtTokenCollectBoxID	= collectedInfo.tokenBoxID.ToString();
		hopperBColl.txtOperatorID			= theAPP_SESSION.GetUserInfo().sUserID;
		hopperBColl.txtReserve				= _T("");
		hopperBColl.txtComment				= _T("");
		try{
			DATA_HELPER->DbInsertTokenCollectOperationRecord(hopperBColl);
		}
		catch(...){
			CString strLog;
			strLog.Format(_T("插入token回收记录到数据库失败！"));
			theEXCEPTION_MGR.WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, strLog));
		}
		// 更新计数和状态
		collectedInfo.ulCurCount += iCountB;
		theTOKEN_COUNT.SetTokenboxCollectedInfo(collectedInfo);
		theTOKEN_COUNT.SetHopperBInfo(hopperBInfoAfter);
		theTVM_STATUS_MGR.SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("回收token失败"));
		bRet = false;	
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("回收token失败"));
		Sleep(1000);
		bRet = false;	
	}	
	// 记录操作日志
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_TOKEN_RECYCLE,bRet);
	if(!bRet){
		theTVM_SETTING.SetCollectTokenResult(0);
		return false;
	}


	// 打印回收hopper B的凭证
	try{
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		CString strTicketCount_before;
		CString strTicketCount_after;
		strOperatorID_before.Format(_T("%06d"), hopperBInfo.ulOperatorID);
		strOperatorID_after = theAPP_SESSION.GetUserInfo().sUserID;
		strTicketCount_before.Format(_T("%d"),  hopperBInfo.ulCurCount);
		strTicketCount_after.Format(_T("%d"), hopperBInfoAfter.ulCurCount);
		// 加载Template
		CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	

			line.Replace(_T("{更换票箱}"), _T("回收票箱"));
			line.Replace(_T("{票箱}"), _T("Hopper B"));
			line.Replace(_T("{操作前票箱号}"), 	hopperBInfo.tokenBoxID.ToString());
			line.Replace(_T("{操作后票箱号}"), hopperBInfoAfter.tokenBoxID.ToString());
			line.Replace(_T("{操作前数量}"), strTicketCount_before);
			line.Replace(_T("{操作后数量}"), strTicketCount_after);
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), theAPP_SESSION.GetUserInfo().sUserID);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), hopperAInfo.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		// 开始打印
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
		//Sleep(3000);//等待打印机完成打印
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("Hopper B回收凭证打印失败"));
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(TXT_COLOR_RED + _T("Hopper B回收凭证打印失败"));
	}

	theTVM_SETTING.SetCollectTokenResult(1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
0@brief      卸载票箱

@param      none

@retval     卸载票箱操作的结果

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::RemoveCardBox()
{
	bool result = false;
	CTCardCountInfo::CARD_BOX_INFO box_A_Ex = theCARD_COUNT.GetCardboxAInfo();	// 票箱A
	CTCardCountInfo::CARD_BOX_INFO box_B_Ex = theCARD_COUNT.GetCardboxBInfo();	// 票箱B
	CTCardCountInfo::CARD_BOX_INFO box_C_Ex = theCARD_COUNT.GetCardboxWastedInfo();	// 废票箱

	TH_RSP_GET_DEV_STATUS status;
	bool isBoxAExist = false;
	bool isBoxBExist = false;
	bool isBoxCExist = false;
	bool isADone = true,isBDone = true,isCDone = true;
	try{
		CARD_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist ,isBoxCExist);
	}
	catch(...){
		theTVM_SETTING.SetCardBoxLastAction(0);
		return false;
	}
	if (isBoxCExist){
		try
		{
			// 先判断缓冲区是否有票？
			TH_RSP_GET_DEV_STATUS rsp;
			CARD_HELPER->GetDevStatus(&rsp);

			if(rsp.bCardInReadArea){
				// 回收缓冲区票卡
				UINT ulRecycCount = 0;
				long lRet = CARD_HELPER->RetractCard(&ulRecycCount);

				// 最后回收的票需要计入废票箱中
				CTCardCountInfo::CARD_BOX_INFO cardInfo = theCARD_COUNT.GetCardboxWastedInfo();
				cardInfo.ulCurCount += ulRecycCount;
				theCARD_COUNT.SetCardboxWastedInfo(cardInfo);

				// 需要生成废票交易
				INVALIDE_TICKET_RECYCLETRADE cardInvalid;
				// 保存废卡交易
				cardInvalid.reson = 0x02;// 正常票卡
				cardInvalid.cardPhysicalType = ULTRALIGHT;
				// 卡号
				cardInvalid.cardSerialNo = 0;
				// 发行商
				cardInvalid.issueId		 = 0x0002;//ACC
				// 废票箱ID
				cardInvalid.magazineId.bDeviceType = cardInfo.ticketBoxID.bDeviceType;
				cardInvalid.magazineId.bType = cardInfo.ticketBoxID.bType;
				cardInvalid.magazineId.bStationID = cardInfo.ticketBoxID.bStationID;
				cardInvalid.magazineId.bNum = cardInfo.ticketBoxID.bNum;

				cardInvalid.productType		= 0x6200;

				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(cardInvalid);
			}
		}
		catch(...){
			theTVM_SETTING.SetCardBoxLastAction(0);
			return false;
		}
	}
	box_C_Ex = theCARD_COUNT.GetCardboxWastedInfo();
	try{
		if (box_C_Ex.ticketBoxID.ToString() != INVALID_ID){
			// 清空软计数数据
			UINT uiDeviceType	= 0xFF;
			UINT uiType		= 0xFF;
			UINT uiStationID = 0xff;
			UINT uiNum		= 0xFF;
			int iquantity	= 0;
			int iBeforeQuantity = 0;
			CTCardCountInfo::CARD_BOX_INFO boxInfo;
			boxInfo = theCARD_COUNT.GetCardboxWastedInfo();
			boxInfo.ticketBoxID.bDeviceType	= uiDeviceType;
			boxInfo.ticketBoxID.bType		= uiType;
			boxInfo.ticketBoxID.bStationID = uiStationID;
			boxInfo.ticketBoxID.bNum		= uiNum;
			boxInfo.ulCurCount				= iquantity;
			isCDone = theCARD_COUNT.SetCardboxWastedInfo(boxInfo);
			if (isCDone){
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);
				//向服务器发送卸载数据包
				MAGAZINE_CHANGE change;
				change.operType =  DETACH;
				change.magazineId.bDeviceType = box_C_Ex.ticketBoxID.bDeviceType;
				change.magazineId.bType		= box_C_Ex.ticketBoxID.bType;//_ttoi(box_C_Ex.ticketBoxID.ToString().Mid(2,2));
				change.magazineId.bNum		= box_C_Ex.ticketBoxID.bNum;//_ttoi(box_C_Ex.ticketBoxID.ToString().Mid(4,4));
				change.magazineId.bStationID = box_C_Ex.ticketBoxID.bStationID;

				change.magazne = MAGAZINEC;

				change.cardType = ULTRALIGHT;
				change.providerId = 0x0002;		//ACC
				change.cardProductType = 0x6200;  //单程票
				change.preProperty = ACC_INIT_CARD;
				change.childproductId = 0;
				change.CardCount = box_C_Ex.ulCurCount;
				change.magazineStatus = VALID_DETACH;
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);


				// 插入数据库
				CardCollectBoxRemoveOperationRecord removeWastedInfo;
				removeWastedInfo.dtTransTime  = ComGetCurTime();
				removeWastedInfo.txtCardBoxID = box_C_Ex.ticketBoxID.ToString();
				removeWastedInfo.iQuantity    = box_C_Ex.ulCurCount;
				removeWastedInfo.txtOperatorID= theAPP_SESSION.GetUserInfo().sUserID;

				DATA_HELPER->DbInsertCardCollectBoxRemoveOperationRecord(removeWastedInfo);
			}
		}
	}
	catch(CSysException& e){
		isCDone = false;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}

	// 票箱操作
	TH_REPLACE_BOX_CMD_INFO replaceSelect;

	// 票箱A操作
	try{

		if (box_A_Ex.ticketBoxID.ToString() != NULL_BOX_ID){
			// 卸载票箱A
			try
			{
				// 如果箱子在位
				if (isBoxAExist){
					CARD_HELPER->GetDevStatus(&status);
					// 如果盖板到位
					if (status.bBoxACoverArrive){
						// 降下票箱电机
						replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_A;
						replaceSelect.actionType = TH_UNINSTALL;
						CARD_HELPER->ReplaceTicketBox(replaceSelect);
						isADone = true;
					}
				}
			}
			catch(CSysException& e){
				isADone = false;
				theEXCEPTION_MGR.ProcessException(e);
			}
			// 清空软计数数据
			UINT uiDeviceType	= 0xFF;
			UINT uiType		= 0xFF;
			UINT uiNum		= 0xFF;
			UINT uiStationID = 0xff;
			int iquantity	= 0;
			CTCardCountInfo::CARD_BOX_INFO boxInfo;
			boxInfo = theCARD_COUNT.GetCardboxAInfo();
			boxInfo.ticketBoxID.bDeviceType	= uiDeviceType;
			boxInfo.ticketBoxID.bType		= uiType;
			boxInfo.ticketBoxID.bNum		= uiNum;
			boxInfo.ticketBoxID.bStationID = uiStationID;
			boxInfo.ulCurCount				= iquantity;			
			isADone = theCARD_COUNT.SetCardboxAInfo(boxInfo);

			if (isADone){
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);

				//向服务器发送卸载数据包
				MAGAZINE_CHANGE change;
				change.operType =  DETACH;
				change.magazineId.bDeviceType = box_A_Ex.ticketBoxID.bDeviceType;
				change.magazineId.bType		= box_A_Ex.ticketBoxID.bType;
				change.magazineId.bNum		= box_A_Ex.ticketBoxID.bNum;
				change.magazineId.bStationID = box_A_Ex.ticketBoxID.bStationID;
				change.magazne = MAGAZINEB;


				change.cardType = ULTRALIGHT;
				change.providerId = 0x0002;		//ACC
				change.cardProductType = 0x6200;  //单程票
				change.preProperty = ACC_INIT_CARD;
				change.childproductId = 0;
				change.CardCount = box_A_Ex.ulCurCount;
				change.magazineStatus = VALID_DETACH;
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);


				// 插入本地数据库
				CardBoxRemoveOperationRecord removeCardInfo;
				removeCardInfo.dtOperationTime = ComGetCurTime();					// 操作时间
				removeCardInfo.txtCardBoxID = box_A_Ex.ticketBoxID.ToString();		// 卸载前ID
				removeCardInfo.iOriginalQuantity = box_A_Ex.ulCurCount;				// 卸载前数量
				removeCardInfo.iRemoveQuantity = 0;									// 卸载后数量
				removeCardInfo.txtOperatorID=theAPP_SESSION.GetUserInfo().sUserID;	// 操作员ID
				DATA_HELPER->DbInsertCardBoxRemoveOperationRecord(removeCardInfo);
			}
		}
	}
	catch(CSysException& e){
		isADone = false;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}


	// 票箱B操作
	try{
		if (box_B_Ex.ticketBoxID.ToString() != INVALID_ID){
			// 卸载票箱B
			try
			{
				// 如果箱子在位
				if (isBoxBExist){
					CARD_HELPER->GetDevStatus(&status);
					// 如果盖板到位
					if (status.bBoxBCoverArrive){
						// 降下票箱电机
						replaceSelect.boxId = TH_BOX_TYPE::TH_BOX_B;
						replaceSelect.actionType = TH_UNINSTALL;
						CARD_HELPER->ReplaceTicketBox(replaceSelect);
						isBDone = true;
					}
				}
			}
			catch(...)
			{
				isBDone = false;
				//return false;
			}
			// 清空软计数数据
			UINT uiDeviceType	= 0xFF;
			UINT uiType		= 0xFF;
			UINT uiNum		= 0xFF;
			UINT uiStationID = 0xff;
			int iquantity	= 0;
			CTCardCountInfo::CARD_BOX_INFO boxInfo;
			boxInfo = theCARD_COUNT.GetCardboxBInfo();
			boxInfo.ticketBoxID.bDeviceType	= uiDeviceType;
			boxInfo.ticketBoxID.bType		= uiType;
			boxInfo.ticketBoxID.bNum		= uiNum;
			boxInfo.ticketBoxID.bStationID = uiStationID;
			boxInfo.ulCurCount				= iquantity;
			isBDone = theCARD_COUNT.SetCardboxBInfo(boxInfo);
			if (isBDone)
			{
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);
				//向服务器发送卸载数据包
				MAGAZINE_CHANGE change;
				change.operType =  DETACH;
				change.magazineId.bDeviceType = box_B_Ex.ticketBoxID.bDeviceType;//_ttoi(box_B_Ex.ticketBoxID.ToString().Mid(0,2));
				change.magazineId.bType		= box_B_Ex.ticketBoxID.bType;//_ttoi(box_B_Ex.ticketBoxID.ToString().Mid(2,2));
				change.magazineId.bNum		= box_B_Ex.ticketBoxID.bNum;//_ttoi(box_B_Ex.ticketBoxID.ToString().Mid(4,4));
				change.magazineId.bStationID = box_B_Ex.ticketBoxID.bStationID;
				change.magazne = MAGAZINEA;


				change.cardType = ULTRALIGHT;
				change.providerId = 0x0002;		//ACC
				change.cardProductType = 0x6200;  //单程票
				change.preProperty = ACC_INIT_CARD;
				change.childproductId = 0;
				change.CardCount = box_B_Ex.ulCurCount;
				change.magazineStatus = VALID_DETACH;
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);


				// 插入本地数据库
				CardBoxRemoveOperationRecord removeCardInfo;
				removeCardInfo.dtOperationTime = ComGetCurTime();					// 操作时间
				removeCardInfo.txtCardBoxID = box_B_Ex.ticketBoxID.ToString();		// 卸载前ID
				removeCardInfo.iOriginalQuantity = box_B_Ex.ulCurCount;				// 卸载前数量
				removeCardInfo.iRemoveQuantity = 0;									// 卸载后数量
				removeCardInfo.txtOperatorID=theAPP_SESSION.GetUserInfo().sUserID;	// 操作员ID
				DATA_HELPER->DbInsertCardBoxRemoveOperationRecord(removeCardInfo);

				// 写票卡硬计数
				try{
					tTIMCardNum timCardNum;
					timCardNum.uiCassetteACardNum = theCARD_COUNT.GetCardboxAInfo().ulCurCount;
					timCardNum.uiCassetteBCardNum = theCARD_COUNT.GetCardboxBInfo().ulCurCount;
					timCardNum.uiRejectCassetteNum = theCARD_COUNT.GetCardboxWastedInfo().ulCurCount;

					CARD_HELPER->SetTicketBoxCountInfo(TH_BOX_A,&timCardNum);
				}
				catch(CSysException& e){
					theEXCEPTION_MGR.ProcessException(e);
				}
				catch(...){
					// Do nothing
				}
			}
		}
	}
	catch(CSysException& e){
		isBDone = false;
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}

	// 写票卡硬计数
	try{
		tTIMCardNum timCardNum;
		timCardNum.uiCassetteACardNum = theCARD_COUNT.GetCardboxAInfo().ulCurCount;
		timCardNum.uiCassetteBCardNum = theCARD_COUNT.GetCardboxBInfo().ulCurCount;
		timCardNum.uiRejectCassetteNum = theCARD_COUNT.GetCardboxWastedInfo().ulCurCount;

		CARD_HELPER->SetTicketBoxCountInfo(TH_BOX_A,&timCardNum);
		CARD_HELPER->SetTicketBoxCountInfo(TH_BOX_B,&timCardNum);
		CARD_HELPER->SetTicketBoxCountInfo(TH_BOX_C,&timCardNum);

	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		// Do nothing
	}

	if (isADone && isBDone && isCDone)
	{
		// 打印凭条
		try
		{
			CTCardCountInfo::CARD_BOX_INFO BoxA = theCARD_COUNT.GetCardboxAInfo();
			CTCardCountInfo::CARD_BOX_INFO BoxB = theCARD_COUNT.GetCardboxBInfo();
			CTCardCountInfo::CARD_BOX_INFO BoxC = theCARD_COUNT.GetCardboxWastedInfo();


			CString CoinBox_templatename = _T("TVM_CARD_RECEIPT.template");
			CStringArray printtext;
			CStringArray TemplateContents;

			BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

			int Index = 0;
			CString strCount;

			int nHalfCount = TemplateContents.GetCount()/2 - 2;

			while(Index<TemplateContents.GetCount())
			{
				CString& line = TemplateContents.ElementAt(Index);	

				line.Replace(_T("{操作}"),_T("卸载"));				// 操作类型

				// 操作前 ID
				line.Replace(_T("{前ID1}"),box_A_Ex.ticketBoxID.ToString());
				line.Replace(_T("{前ID2}"),box_B_Ex.ticketBoxID.ToString());
				line.Replace(_T("{前ID3}"),box_C_Ex.ticketBoxID.ToString());

				// 操作前 数量
				line.Replace(_T("{前数量1}"),ComFormatCount(box_A_Ex.ulCurCount ));
				line.Replace(_T("{前数量2}"),ComFormatCount(box_B_Ex.ulCurCount ));
				line.Replace(_T("{前数量3}"),ComFormatCount(box_C_Ex.ulCurCount ));

				line.Replace(_T("{前合计}"),ComFormatCount(box_A_Ex.ulCurCount + box_B_Ex.ulCurCount + box_C_Ex.ulCurCount));

				// 操作后 
				if(Index > nHalfCount){
					if(box_A_Ex.ticketBoxID.ToString() != BoxA.ticketBoxID.ToString() || box_A_Ex.ulCurCount != BoxA.ulCurCount){
						line.Replace(_T("票箱A"),_T("*票箱A"));
					}

					if(box_B_Ex.ticketBoxID.ToString() != BoxB.ticketBoxID.ToString() || box_B_Ex.ulCurCount != BoxB.ulCurCount){
						line.Replace(_T("票箱B"),_T("*票箱B"));
					}

					if(box_C_Ex.ticketBoxID.ToString() != BoxC.ticketBoxID.ToString() || box_C_Ex.ulCurCount != BoxC.ulCurCount){
						line.Replace(_T("废票箱"),_T("*废票箱"));
					}
				}
				// 操作后 ID
				// 操作前 ID
				line.Replace(_T("{后ID1}"),BoxA.ticketBoxID.ToString());
				line.Replace(_T("{后ID2}"),BoxB.ticketBoxID.ToString());
				line.Replace(_T("{后ID3}"),BoxC.ticketBoxID.ToString());

				// 操作前 数量	 h后
				line.Replace(_T("{后数量1}"),ComFormatCount(BoxA.ulCurCount ));
				line.Replace(_T("{后数量2}"),ComFormatCount(BoxB.ulCurCount ));
				line.Replace(_T("{后数量3}"),ComFormatCount(BoxC.ulCurCount ));

				line.Replace(_T("{后合计}"),ComFormatCount(BoxA.ulCurCount + BoxB.ulCurCount + BoxC.ulCurCount));

				line.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
				line.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
				line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
				line.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

				CStringArray receiptline;
				ComPrintLineFormatForTemplate(line,receiptline);
				printtext.Append(receiptline);
				Index++;
			}
			theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_UNINSTALL);
			PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
		}
		catch(CSysException& e) {
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
			theEXCEPTION_MGR.ProcessException(e);
			//throw;
		}
		catch (...) {
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		}
	}
	if (isADone && isBDone && isCDone){
		result = true;
		theTVM_SETTING.SetCardBoxLastAction(1);
		return result;
	}
	theTVM_SETTING.SetCardBoxLastAction(0);
	return result;
	//bool result = false;
	//CTTokenCountInfo::TOKEN_BOX_INFO box_A_Ex = theTOKEN_COUNT.GetTokenboxAInfo();	// 票箱A
	//CTTokenCountInfo::TOKEN_BOX_INFO box_B_Ex = theTOKEN_COUNT.GetTokenboxAInfo();	// 票箱B
	//CTTokenCountInfo::TOKEN_BOX_INFO box_C_Ex = theTOKEN_COUNT.GetTokenboxWastedInfo();	// 废票箱
	//CTTokenCountInfo::TOKEN_BOX_INFO box_D_Ex = theTOKEN_COUNT.GetTokenboxCollectedInfo();	// 回收箱

	//CString strOperatorID_before;
	//CString strOperatorID_after;
	//CString strTicketCount_before;
	//CString strTicketCount_after;

	//TOKEN_RSP_GET_DEV_STATUS status;
	//bool isBoxAExist = false;
	//bool isBoxBExist = false;
	//bool isBoxCExist = false;
	//bool isBoxDExist = false;
	//bool isADone = false, isBDone = false, isCDone = false, isDDone = false;
	////20180103,lichao:暂时去掉卸载票箱时，对箱子在位的限制条件。
	//isBoxAExist = isBoxBExist = isBoxCExist = isBoxDExist = true;
	////try{
	////	TOKEN_HELPER->CheckTicketBoxExist(&status, isBoxAExist, isBoxBExist, isBoxCExist, isBoxDExist);
	////}
	////catch(CSysException& e){
	////	theSETTING.SetCardBoxLastAction(0);
	////	theEXCEPTION_MGR.ProcessException(e);
	////	return false;
	////}
	////catch(...){
	////	theSETTING.SetCardBoxLastAction(0);
	////	return false;
	////}

	////if (!isBoxAExist && box_A_Ex.tokenBoxID.ToString()!=_T("FFFFFFFF")){
	////	theAPP_SESSION.ShowOfficeGuide(_T("token箱A不在位"));
	////	theSETTING.SetCardBoxLastAction(0);
	////	return false;
	////}
	////if (!isBoxBExist && box_B_Ex.tokenBoxID.ToString()!=_T("FFFFFFFF")){
	////	theAPP_SESSION.ShowOfficeGuide(_T("token箱B不在位"));
	////	theSETTING.SetCardBoxLastAction(0);
	////	return false;
	////}
	////if (!isBoxCExist && box_C_Ex.tokenBoxID.ToString()!=_T("FFFFFFFF")){
	////	theAPP_SESSION.ShowOfficeGuide(_T("token废票箱不在位"));
	////	theSETTING.SetCardBoxLastAction(0);
	////	return false;
	////}
	////if (!isBoxDExist && box_D_Ex.tokenBoxID.ToString()!=_T("FFFFFFFF")){
	////	theAPP_SESSION.ShowOfficeGuide(_T("token回收箱不在位"));
	////	theSETTING.SetCardBoxLastAction(0);
	////	return false;
	////}


	//if (isBoxAExist && box_A_Ex.tokenBoxID.ToString()!=INVALID_ID){
	//	isADone = true;
	//	//向服务器发送卸载数据包
	//	MAGAZINE_CHANGE change;
	//	change.operType =  DETACH;
	//	change.magazineId.Deserialize(&box_A_Ex.tokenBoxID.bLineCode);
	//	change.magazineId.bLineCode = box_A_Ex.tokenBoxID.bLineCode;
	//	change.magazineId.bType = box_A_Ex.tokenBoxID.bType;
	//	change.magazineId.wNum = box_A_Ex.tokenBoxID.wNum;
	//	change.magazne =MAGAZINEC;
	//	change.cardType =0;//UL卡类型
	//	change.providerId = 0x01;
	//	change.cardProductType = 0;
	//	change.preProperty = 0x01;
	//	change.childproductId = 0x01;
	//	change.CardCount = 0x00;
	//	change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	//	// 插入数据库
	//	TokenBoxRemoveOperationRecord removeRecord;
	//	removeRecord.dtOperationTime	= ComGetCurTime();
	//	removeRecord.txtTokenBoxID		= box_A_Ex.tokenBoxID.ToString();
	//	removeRecord.iOriginalQuantity  = box_A_Ex.ulInitCount;
	//	removeRecord.iRemoveQuantity    = box_A_Ex.ulCurCount;
	//	removeRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
	//	removeRecord.txtReserve			= _T("");
	//	removeRecord.txtComment			= _T("");
	//	DATA_HELPER->DbInsertTokenBoxRemoveOperationRecord(removeRecord);
	//	// 更新软计数
	//	CTTokenCountInfo::TOKEN_BOX_INFO BoxA = box_A_Ex;
	//	BoxA.tokenBoxID.bLineCode	= 0xFF;
	//	BoxA.tokenBoxID.bType		= 0xFF;
	//	BoxA.tokenBoxID.wNum		= 0xFFFF;
	//	BoxA.ulCurCount = 0;
	//	BoxA.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	//	BoxA.operateTime = ComGetCurTime();
	//	theTOKEN_COUNT.SetTokenboxAInfo(BoxA);
	//	theTVM_STATUS_MGR.SetTokenAddBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);

	//	// 打印凭条
	//	try{
	//		strOperatorID_before.Format(_T("%06d"), box_A_Ex.ulOperatorID);
	//		strOperatorID_after.Format(_T("%06d"), BoxA.ulOperatorID);
	//		strTicketCount_before.Format(_T("%d"),  box_A_Ex.ulCurCount);
	//		strTicketCount_after.Format(_T("%d"), BoxA.ulCurCount);
	//		// 加载Template
	//		CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
	//		CStringArray printtext;
	//		CStringArray TemplateContents;
	//		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
	//		int Index = 0;
	//		while(Index<TemplateContents.GetCount())
	//		{
	//			CString& line = TemplateContents.ElementAt(Index);	
	//			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
	//			line.Replace(_T("{票箱}"), _T("票箱1"));
	//			line.Replace(_T("{操作前票箱号}"), box_A_Ex.tokenBoxID.ToString());
	//			line.Replace(_T("{操作后票箱号}"), BoxA.tokenBoxID.ToString());
	//			line.Replace(_T("{操作前数量}"), strTicketCount_before);
	//			line.Replace(_T("{操作后数量}"), strTicketCount_after);

	//			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
	//			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
	//			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
	//			line.Replace(_T("{操作前时间}"), box_A_Ex.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{操作后时间}"), BoxA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

	//			CStringArray receiptline;
	//			ComPrintLineFormatForTemplate(line,receiptline);
	//			printtext.Append(receiptline);
	//			Index++;
	//		}
	//		// 开始打印
	//		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//		Sleep(3000);//等待打印机完成打印
	//	}
	//	catch(CSysException& e) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(e);
	//	}
	//	catch (...) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//	}
	//}

	//if (isBoxBExist && box_B_Ex.tokenBoxID.ToString()!=INVALID_ID){
	//	isBDone = true;
	//	//向服务器发送卸载数据包
	//	MAGAZINE_CHANGE change;
	//	change.operType =  DETACH;
	//	change.magazineId.Deserialize(&box_B_Ex.tokenBoxID.bLineCode);
	//	change.magazineId.bLineCode = box_B_Ex.tokenBoxID.bLineCode;
	//	change.magazineId.bType = box_B_Ex.tokenBoxID.bType;
	//	change.magazineId.wNum = box_B_Ex.tokenBoxID.wNum;
	//	change.magazne =MAGAZINEB;
	//	change.cardType =0;//UL卡类型
	//	change.providerId = 0x01;
	//	change.cardProductType = 0;
	//	change.preProperty = 0x01;
	//	change.childproductId = 0x01;
	//	change.CardCount = 0x00;
	//	change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	//	// 插入数据库
	//	TokenBoxRemoveOperationRecord removeRecord;
	//	removeRecord.dtOperationTime	= ComGetCurTime();
	//	removeRecord.txtTokenBoxID		= box_B_Ex.tokenBoxID.ToString();
	//	removeRecord.iOriginalQuantity  = box_B_Ex.ulInitCount;
	//	removeRecord.iRemoveQuantity    = box_B_Ex.ulCurCount;
	//	removeRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
	//	removeRecord.txtReserve			= _T("");
	//	removeRecord.txtComment			= _T("");
	//	DATA_HELPER->DbInsertTokenBoxRemoveOperationRecord(removeRecord);
	//	// 更新软计数
	//	CTTokenCountInfo::TOKEN_BOX_INFO BoxB = box_B_Ex;
	//	BoxB.tokenBoxID.bLineCode	= 0xFF;
	//	BoxB.tokenBoxID.bType		= 0xFF;
	//	BoxB.tokenBoxID.wNum		= 0xFFFF;
	//	BoxB.ulCurCount = 0;
	//	BoxB.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	//	BoxB.operateTime = ComGetCurTime();
	//	theTOKEN_COUNT.SetTokenboxBInfo(BoxB);
	//	theTVM_STATUS_MGR.SetTokenAddBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);

	//	// 打印凭条
	//	try{
	//		strOperatorID_before.Format(_T("%06d"), box_B_Ex.ulOperatorID);
	//		strOperatorID_after.Format(_T("%06d"), BoxB.ulOperatorID);
	//		strTicketCount_before.Format(_T("%d"),  box_B_Ex.ulCurCount);
	//		strTicketCount_after.Format(_T("%d"), BoxB.ulCurCount);
	//		// 加载Template
	//		CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
	//		CStringArray printtext;
	//		CStringArray TemplateContents;
	//		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
	//		int Index = 0;
	//		while(Index<TemplateContents.GetCount())
	//		{
	//			CString& line = TemplateContents.ElementAt(Index);	
	//			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
	//			line.Replace(_T("{票箱}"), _T("票箱2"));
	//			line.Replace(_T("{操作前票箱号}"), box_B_Ex.tokenBoxID.ToString());
	//			line.Replace(_T("{操作后票箱号}"), BoxB.tokenBoxID.ToString());
	//			line.Replace(_T("{操作前数量}"), strTicketCount_before);
	//			line.Replace(_T("{操作后数量}"), strTicketCount_after);

	//			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
	//			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
	//			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
	//			line.Replace(_T("{操作前时间}"), box_B_Ex.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{操作后时间}"), BoxB.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

	//			CStringArray receiptline;
	//			ComPrintLineFormatForTemplate(line,receiptline);
	//			printtext.Append(receiptline);
	//			Index++;
	//		}
	//		// 开始打印
	//		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//		Sleep(3000);//等待打印机完成打印
	//	}
	//	catch(CSysException& e) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(e);
	//	}
	//	catch (...) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//	}
	//}

	//if (isBoxCExist && box_C_Ex.tokenBoxID.ToString()!=INVALID_ID){
	//	isCDone = true;
	//	//向服务器发送卸载数据包
	//	MAGAZINE_CHANGE change;
	//	change.operType =  DETACH;
	//	change.magazineId.Deserialize(&box_C_Ex.tokenBoxID.bLineCode);
	//	change.magazineId.bLineCode = box_C_Ex.tokenBoxID.bLineCode;
	//	change.magazineId.bType = box_C_Ex.tokenBoxID.bType;
	//	change.magazineId.wNum = box_C_Ex.tokenBoxID.wNum;
	//	change.magazne =MAGAZINEA;
	//	change.cardType =0;//UL卡类型
	//	change.providerId = 0x01;
	//	change.cardProductType = 0;
	//	change.preProperty = 0x01;
	//	change.childproductId = 0x01;
	//	change.CardCount = 0x00;
	//	change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	//	// 插入数据库
	//	TokenBoxRemoveOperationRecord removeRecord;
	//	removeRecord.dtOperationTime	= ComGetCurTime();
	//	removeRecord.txtTokenBoxID		= box_C_Ex.tokenBoxID.ToString();
	//	removeRecord.iOriginalQuantity  = box_C_Ex.ulInitCount;
	//	removeRecord.iRemoveQuantity    = box_C_Ex.ulCurCount;
	//	removeRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
	//	removeRecord.txtReserve			= _T("");
	//	removeRecord.txtComment			= _T("");
	//	DATA_HELPER->DbInsertTokenBoxRemoveOperationRecord(removeRecord);
	//	// 更新软计数
	//	CTTokenCountInfo::TOKEN_BOX_INFO BoxC = box_C_Ex;
	//	BoxC.tokenBoxID.bLineCode	= 0xFF;
	//	BoxC.tokenBoxID.bType		= 0xFF;
	//	BoxC.tokenBoxID.wNum		= 0xFFFF;
	//	BoxC.ulCurCount = 0;
	//	BoxC.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	//	BoxC.operateTime = ComGetCurTime();
	//	theTOKEN_COUNT.SetTokenboxWastedInfo(BoxC);
	//	theTVM_STATUS_MGR.SetTokenWasteBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);

	//	// 打印凭条
	//	try{
	//		strOperatorID_before.Format(_T("%06d"), box_C_Ex.ulOperatorID);
	//		strOperatorID_after.Format(_T("%06d"), BoxC.ulOperatorID);
	//		strTicketCount_before.Format(_T("%d"),  box_C_Ex.ulCurCount);
	//		strTicketCount_after.Format(_T("%d"), BoxC.ulCurCount);
	//		// 加载Template
	//		CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
	//		CStringArray printtext;
	//		CStringArray TemplateContents;
	//		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
	//		int Index = 0;
	//		while(Index<TemplateContents.GetCount())
	//		{
	//			CString& line = TemplateContents.ElementAt(Index);	
	//			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
	//			line.Replace(_T("{票箱}"), _T("废票箱"));
	//			line.Replace(_T("{操作前票箱号}"), box_C_Ex.tokenBoxID.ToString());
	//			line.Replace(_T("{操作后票箱号}"), BoxC.tokenBoxID.ToString());
	//			line.Replace(_T("{操作前数量}"), strTicketCount_before);
	//			line.Replace(_T("{操作后数量}"), strTicketCount_after);

	//			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
	//			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
	//			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
	//			line.Replace(_T("{操作前时间}"), box_C_Ex.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{操作后时间}"), BoxC.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

	//			CStringArray receiptline;
	//			ComPrintLineFormatForTemplate(line,receiptline);
	//			printtext.Append(receiptline);
	//			Index++;
	//		}
	//		// 开始打印
	//		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//		Sleep(3000);//等待打印机完成打印
	//	}
	//	catch(CSysException& e) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(e);
	//	}
	//	catch (...) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//	}
	//}

	//if (isBoxDExist && box_D_Ex.tokenBoxID.ToString()!=INVALID_ID){
	//	isDDone = true;
	//	//向服务器发送卸载数据包
	//	MAGAZINE_CHANGE change;
	//	change.operType =  DETACH;
	//	change.magazineId.Deserialize(&box_D_Ex.tokenBoxID.bLineCode);
	//	change.magazineId.bLineCode = box_D_Ex.tokenBoxID.bLineCode;
	//	change.magazineId.bType = box_D_Ex.tokenBoxID.bType;
	//	change.magazineId.wNum = box_D_Ex.tokenBoxID.wNum;
	//	change.magazne =MAGAZINEF;
	//	change.cardType =0;//UL卡类型
	//	change.providerId = 0x01;
	//	change.cardProductType = 0;
	//	change.preProperty = 0x01;
	//	change.childproductId = 0x01;
	//	change.CardCount = 0x00;
	//	change.magazineStatus = (SC_MAGAZINE_STATUS)VALID_DETACH;
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgMagazineExchange(change);
	//	// 插入数据库
	//	TokenBoxRemoveOperationRecord removeRecord;
	//	removeRecord.dtOperationTime	= ComGetCurTime();
	//	removeRecord.txtTokenBoxID		= box_D_Ex.tokenBoxID.ToString();
	//	removeRecord.iOriginalQuantity  = box_D_Ex.ulInitCount;
	//	removeRecord.iRemoveQuantity    = box_D_Ex.ulCurCount;
	//	removeRecord.txtOperatorID		= theAPP_SESSION.GetUserInfo().sUserID;
	//	removeRecord.txtReserve			= _T("");
	//	removeRecord.txtComment			= _T("");
	//	DATA_HELPER->DbInsertTokenBoxRemoveOperationRecord(removeRecord);
	//	// 更新软计数
	//	CTTokenCountInfo::TOKEN_BOX_INFO BoxD = box_D_Ex;
	//	BoxD.tokenBoxID.bLineCode	= 0xFF;
	//	BoxD.tokenBoxID.bType		= 0xFF;
	//	BoxD.tokenBoxID.wNum		= 0xFFFF;
	//	BoxD.ulCurCount = 0;
	//	BoxD.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	//	BoxD.operateTime = ComGetCurTime();
	//	theTOKEN_COUNT.SetTokenboxCollectedInfo(BoxD);
	//	theTVM_STATUS_MGR.SetTokenCollectedBoxStatus(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARDBOX_REMOVAL,true);

	//	// 打印凭条
	//	try{
	//		strOperatorID_before.Format(_T("%06d"), box_D_Ex.ulOperatorID);
	//		strOperatorID_after.Format(_T("%06d"), BoxD.ulOperatorID);
	//		strTicketCount_before.Format(_T("%d"),  box_D_Ex.ulCurCount);
	//		strTicketCount_after.Format(_T("%d"), BoxD.ulCurCount);
	//		// 加载Template
	//		CString CoinBox_templatename = _T("TVM_TOKEN_BOX.template");
	//		CStringArray printtext;
	//		CStringArray TemplateContents;
	//		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
	//		int Index = 0;
	//		while(Index<TemplateContents.GetCount())
	//		{
	//			CString& line = TemplateContents.ElementAt(Index);	
	//			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));	
	//			line.Replace(_T("{票箱}"), _T("废票箱"));
	//			line.Replace(_T("{操作前票箱号}"), box_D_Ex.tokenBoxID.ToString());
	//			line.Replace(_T("{操作后票箱号}"), BoxD.tokenBoxID.ToString());
	//			line.Replace(_T("{操作前数量}"), strTicketCount_before);
	//			line.Replace(_T("{操作后数量}"), strTicketCount_after);

	//			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
	//			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
	//			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
	//			line.Replace(_T("{操作前时间}"), box_D_Ex.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{操作后时间}"), BoxD.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
	//			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

	//			CStringArray receiptline;
	//			ComPrintLineFormatForTemplate(line,receiptline);
	//			printtext.Append(receiptline);
	//			Index++;
	//		}
	//		// 开始打印
	//		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	//		Sleep(3000);//等待打印机完成打印
	//	}
	//	catch(CSysException& e) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(e);
	//	}
	//	catch (...) {
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	//		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	//	}
	//}


	//if (isADone && isBDone && isCDone && isDDone){
	//	result = true;
	//	theTVM_SETTING.SetCardBoxLastAction(1);
	//	return true;
	//}
	//theTVM_SETTING.SetCardBoxLastAction(0);
	//return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      卸载硬币回收箱

@param      none

@retval     回收纸币操作的结果

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::RemoveCoinCollectBox()
{
	bool result = false;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	bool is_colA_done = false;
	bool is_colB_done = false;
	theTVM_SETTING.SetCoinBoxLastAction(0);
	if (mEx_collect_A.coinBoxID.ToString() != INVALID_ID){
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coinCollBox = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		coinCollBox = CTCoinCountInfo::COIN_COLLECTION_BOX_INFO();
		coinCollBox.ulInitCount = mEx_collect_A.ulInitCount;
		coinCollBox.operateTime = ComGetCurTime();
		coinCollBox.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
		if(theCOIN_COUNT.SetCollectionCoinboxAInfo(coinCollBox)){
			is_colA_done = true;
			theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
			// 入库
			CoinCollectBoxRemoveOperationRecord removecoincollectionboxrecord;
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();			// 操作时间
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtCoinCollectBoxID = mEx_collect_A.coinBoxID.ToString();
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iTotalAmount = mEx_collect_A.ulCount_1yuan*100 + mEx_collect_A.ulCount_5jiao * 100;
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;														// 操作员ID
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtComment = _T("");
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtReserve = _T("");
			CoinBoxCashDetail coinboxcashdetail;
			coinboxcashdetail.iCoinTypeCode = Coin1Yuan;
			coinboxcashdetail.iQuantity = mEx_collect_A.ulCount_1yuan + mEx_collect_A.ulCount_5jiao;
			removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);
			DATA_HELPER->DbInsertCoinCollectBoxRemoveOperationRecord(removecoincollectionboxrecord);

			CASHBOX_CHANGE  coinbox;					// 硬币回收箱更换业务数据
			vector<CASH_TYPE_COUNT> vecCash;			// 硬币回收箱币种代码、数量
			// 硬币回收箱--操作类型
			coinbox.handleType = UNINSTALL;
			coinbox.SetBoxID(mEx_collect_A.coinBoxID.bLineCode, mEx_collect_A.coinBoxID.bType,mEx_collect_A.coinBoxID.bNumInQu,mEx_collect_A.coinBoxID.bNum);
			// 硬币回收箱A位置ID
			coinbox.position = MAGAZINEG;
			// 硬币回收箱币种代码、数量
			CASH_TYPE_COUNT cash;
			cash.cash_type = Coin1Yuan;
			cash.cash_count =  mEx_collect_A.ulCount_1yuan + mEx_collect_A.ulCount_5jiao;
			vecCash.push_back(cash);
			coinbox.vecCash = vecCash;
			// 硬币回收箱币种总数
			coinbox.btCount = coinbox.GetSumCurrency();
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(ONE_KEY_SETTLEMENT_SVC)->InsertMsgBanknoteBoxExchange(coinbox);

			theTVM_SETTING.SetCoinBoxLastAction(1);
		}
	}
	//if (mEx_collect_B.coinBoxID.ToString() != INVALID_ID){
	//	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coinCollBox = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	//	coinCollBox.ulCount_1yuan = coinCollBox.ulCount_5jiao = 0;
	//	coinCollBox.coinBoxID.bLineCode = 0xFF;
	//	coinCollBox.coinBoxID.bType		= 0xFF;
	//	coinCollBox.coinBoxID.bNum		= 0xFF;
	//	coinCollBox.coinBoxID.bNumInQu = 0xff;
	//	if(theCOIN_COUNT.SetCollectionCoinboxBInfo(coinCollBox)){
	//		is_colB_done = true;
	//		// 操作数据入库
	//		CoinCollectBoxRemoveOperationRecord removecoincollectionboxrecord;
	//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();			// 操作时间
	//		CoinBoxCashDetail coinboxcashdetail;

	//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtCoinCollectBoxID = mEx_collect_B.coinBoxID.ToString();
	//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iTotalAmount = mEx_collect_B.ulCount_1yuan*100 + mEx_collect_B.ulCount_5jiao*50;

	//		coinboxcashdetail.iCoinTypeCode = Coin1Yuan;
	//		coinboxcashdetail.iQuantity = mEx_collect_B.ulCount_1yuan;
	//		removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

	//		coinboxcashdetail.iCoinTypeCode = CoinHalfYuan;
	//		coinboxcashdetail.iQuantity = mEx_collect_B.ulCount_5jiao;
	//		removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

	//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;														// 操作员ID
	//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtComment = _T("");
	//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtReserve = _T("");

	//		DATA_HELPER->DbInsertCoinCollectBoxRemoveOperationRecord(removecoincollectionboxrecord);

	//		// 上传电文
	//		CASHBOX_CHANGE  coinbox;					// 硬币回收箱更换业务数据
	//		vector<CASH_TYPE_COUNT> vecCash;			// 硬币回收箱币种代码、数量
	//		// 硬币回收箱A、B--操作类型
	//		coinbox.handleType = UNINSTALL;
	//		CASH_TYPE_COUNT cash;
	//		cash.cash_type = Coin1Yuan;
	//		cash.cash_count =  mEx_collect_A.ulCount_1yuan;
	//		vecCash.push_back(cash);

	//		cash.cash_type = CoinHalfYuan;
	//		cash.cash_count =  mEx_collect_A.ulCount_5jiao;
	//		vecCash.push_back(cash);
	//		// 硬币回收箱B--ID
	//		coinbox.SetBoxID(mEx_collect_B.coinBoxID.bLineCode, mEx_collect_B.coinBoxID.bType,mEx_collect_B.coinBoxID.bNumInQu,mEx_collect_B.coinBoxID.bNum);
	//		// 硬币回收箱位置ID
	//		coinbox.position = MAGAZINEG;
	//		// 硬币回收箱状态
	//		coinbox.State = VALID_DETACH;
	//		// 硬币回收箱币种代码、数量
	//		coinbox.vecCash = vecCash;
	//		// 硬币回收箱币种总数
	//		coinbox.btCount = coinbox.GetSumCurrency();

	//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(ONE_KEY_SETTLEMENT_SVC)->InsertMsgBanknoteBoxExchange(coinbox);
	//	}
	//}
	if (is_colA_done || is_colB_done){
		result = true;
		PrintCoinBoxExchangeReceipt();
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      卸载纸币箱操作

@param      none

@retval     卸载纸币箱操作的结果

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::RemoveBanknoteCollectBox()
{
	bool result = false;

	CTBanknoteCountInfo::BANKNOTE_BOX_INFO bnBoxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	if (bnBoxInfo.banknoteBoxID.ToString() != INVALID_ID){
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		banknoteInfo.BanknoteInit();
		banknoteInfo.operateTime = ComGetCurTime();
		banknoteInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
		// 写计数文件失败
		if(theBANKNOTE_COUNT.SetBanknoteboxInfo(banknoteInfo)) {
			result = true;
			theTVM_SETTING.SetBanknoteBoxLastAction(1);
			// 生成纸币回收箱卸载电文
			CASHBOX_CHANGE change;
			// 纸币箱ID（卸载前）
			change.SetBoxID(bnBoxInfo.banknoteBoxID.bLineCode, bnBoxInfo.banknoteBoxID.bType, bnBoxInfo.banknoteBoxID.bNumInQu, bnBoxInfo.banknoteBoxID.bNum); 
			// 操作类型
			change.handleType = UNINSTALL; 
			// 位置
			change.position = MAGAZINEA; 
			// 状态
			change.State = VALID_DETACH;
			CASH_TYPE_COUNT cash;
			// 1元
			cash.cash_type  = Banknote1Yuan;					
			cash.cash_count = bnBoxInfo.ulCount_CNY1;
			change.vecCash.push_back(cash);
			// 5元
			cash.cash_type  = Banknote5Yuan;					
			cash.cash_count = bnBoxInfo.ulCount_CNY5;
			change.vecCash.push_back(cash);
			// 10元
			cash.cash_type = Banknote10Yuan;		
			cash.cash_count = bnBoxInfo.ulCount_CNY10;
			change.vecCash.push_back(cash);
			// 20 元
			cash.cash_type  = Banknote20Yuan;						
			cash.cash_count = bnBoxInfo.ulCount_CNY20;
			change.vecCash.push_back(cash);
			// 50 元
			cash.cash_type  = Banknote50Yuan;						
			cash.cash_count = bnBoxInfo.ulCount_CNY50;
			change.vecCash.push_back(cash);
			// 100 元
			cash.cash_type  = Banknote100Yuan;						
			cash.cash_count = bnBoxInfo.ulCount_CNY100;
			change.vecCash.push_back(cash);
			// 币种总数
			change.btCount = change.vecCash.size();
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);	

			// 生成纸币钱箱卸载操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,true);//写日志

			// 插入纸币钱箱卸载操作记录到数据库
			try{
				BanknoteBoxCashDetail detail;
				BanknoteBoxRemoveOperationRecord banknoteBoxRemoveOperationRecord;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.clear();
				// 1元
				detail.iBanknoteTypeCode = Banknote1Yuan;
				detail.iQuantity = bnBoxInfo.ulCount_CNY1;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 5元
				detail.iBanknoteTypeCode = Banknote5Yuan;
				detail.iQuantity = bnBoxInfo.ulCount_CNY5;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 10元
				detail.iBanknoteTypeCode = Banknote10Yuan;
				detail.iQuantity = bnBoxInfo.ulCount_CNY10;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 20元
				detail.iBanknoteTypeCode = Banknote20Yuan;
				detail.iQuantity = bnBoxInfo.ulCount_CNY20;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 50元
				detail.iBanknoteTypeCode = Banknote50Yuan;
				detail.iQuantity = bnBoxInfo.ulCount_CNY50;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 100元
				detail.iBanknoteTypeCode = Banknote100Yuan;
				detail.iQuantity = bnBoxInfo.ulCount_CNY100;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 操作时间
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();
				// 钱箱ID
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtBanknoteBoxID = bnBoxInfo.banknoteBoxID.ToString();;
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.iTotalAmount = bnBoxInfo.BankNotesCountAvos();
				// 操作员ID
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
				// 插库
				DATA_HELPER->DbInsertBanknoteBoxRemoveOperationRecord(banknoteBoxRemoveOperationRecord);
			}
			catch(...){
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_UNINSTALL_INSERT_DB_FAILED));
			}

			PrintBanknoteChangeReceiption();
		}
		else{
			theTVM_SETTING.SetBanknoteBoxLastAction(0);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_STANDBY_FOR_UNINSTALL));
			Sleep(1000);
		}
	}
	return result;
}



bool COneKeySettlementSvc::RemoveBanknoteChangeBox()
{
	bool result = false;
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO bnBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	if (bnBoxInfo.banknoteBoxID.ToString() != INVALID_ID){
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO banknoteInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
		banknoteInfo.BanknoteInit();
		banknoteInfo.operateTime = ComGetCurTime();
		banknoteInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
		// 写计数文件失败
		if(theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(banknoteInfo)) {
			result = true;
			theTVM_SETTING.SetBanknoteBoxLastAction(1);
			// 生成纸币回收箱卸载电文
			CASHBOX_CHANGE change;
			// 纸币箱ID（卸载前）
			change.SetBoxID(bnBoxInfo.banknoteBoxID.bLineCode, bnBoxInfo.banknoteBoxID.bType, bnBoxInfo.banknoteBoxID.bNumInQu, bnBoxInfo.banknoteBoxID.bNum); 
			// 操作类型
			change.handleType = UNINSTALL; 
			// 位置
			change.position = MAGAZINEH; 
			// 状态
			change.State = VALID_DETACH;
			CASH_TYPE_COUNT cash;
			cash.cash_type  = TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());	// 5元
			cash.cash_count = bnBoxInfo.ulCurCount;
			change.vecCash.push_back(cash);
			change.btCount = 0x01;
			// 币种总数
			change.btCount = change.vecCash.size();
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(change);	

			// 生成纸币钱箱卸载操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTEBOX_REMOVAL,true);//写日志

			// 插入纸币钱箱卸载操作记录到数据库
			try{
				BanknoteBoxCashDetail detail;
				BanknoteBoxRemoveOperationRecord banknoteBoxRemoveOperationRecord;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.clear();
				// 5元
				detail.iBanknoteTypeCode = Banknote5Yuan;
				detail.iQuantity = bnBoxInfo.ulCurCount;
				banknoteBoxRemoveOperationRecord.v_banknoteBoxCashDetail.push_back(detail);
				// 操作时间
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();
				// 钱箱ID
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtBanknoteBoxID = bnBoxInfo.banknoteBoxID.ToString();;
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.iTotalAmount = bnBoxInfo.BankNotesCountAvos();
				// 操作员ID
				banknoteBoxRemoveOperationRecord.banknoteBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
				// 插库
				DATA_HELPER->DbInsertBanknoteBoxRemoveOperationRecord(banknoteBoxRemoveOperationRecord);

			}
			catch(...){
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_UNINSTALL_INSERT_DB_FAILED));
			}

			PrintBanknoteChangeReceiption();
		}
		else{
			theTVM_SETTING.SetBanknoteBoxLastAction(0);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_STANDBY_FOR_UNINSTALL));
			Sleep(1000);
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      结算

@param      none

@retval     结算操作的结果

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::Balance()
{
	theSERVICE_MGR.SetForeServiceBusy(true);
	
	theAPP_SESSION.SetManualCloseOperation(true);
	// 走关机时序
	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->CloseOperation();

	theTVM_SETTING.SetBalanceOpTime(_T("1"));

	return true;
	//CAuditHelper *audit = new CAuditHelper(*this);
	//try
	//{
	//	CString strCurTime = ComGetSysTimeText(YYYYMMDDHHMM_TYPE2);
	//	if (BalanceOrNot())
	//	{
	//		// 打印凭条
	//		if( 0 == audit->PrintBalanceBill()){
	//			theSETTING.SetBalanceOpTime(_T("0"));
	//			theSERVICE_MGR.SetForeServiceBusy(true);
	//			theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	//			theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	//			theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
	//			return true;
	//		}else{
	//			return false;
	//			m_modle.mapSettlement[PROC_BALANCE].procState = FAIL;
	//			theSETTING.SetBalanceOpTime(strCurTime);
	//		}
	//	}else{
	//		m_modle.mapSettlement[PROC_BALANCE].procState = FAIL;
	//		theSETTING.SetBalanceOpTime(strCurTime);

	//	}
	//}
	//catch(...){
	//	return FAIL;
	//}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      读取收机配置

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementSvc::LoadSetting(const int settings)
{
	// 读取成功项目
	// 卸载票箱
	map<PROCESS,PROCESS_INFO> &modelMap = m_modle.mapSettlement;
	map<PROCESS,PROCESS_INFO>::iterator itr= modelMap.begin();
	for ( ; itr != modelMap.end(); itr++ )
	{
		PROCESS item = itr->first;
		{
			if ((settings & item) == item ){
				itr->second.procSwitch = SWT_ON;
			}else{
				itr->second.procSwitch = SWT_OFF;
			}
		}
	}
}


void COneKeySettlementSvc::PrintCoinBoxExchangeReceipt()
{
	try
	{
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), m_ex_coin_collectA.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), collect_A.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"),  m_ex_coin_collectA.ulCount_1yuan + m_ex_coin_collectA.ulCount_5jiao);
		strFace1Count_after.Format(_T("%d"), collect_A.ulCount_1yuan + collect_A.ulCount_5jiao);
		// 加载Template
		CString CoinBox_templatename = _T("TVM_COIN_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));																			// 操作类型
			line.Replace(_T("{钱箱}"), _T("硬币回收箱"));
			line.Replace(_T("{操作前钱箱号}"), m_ex_coin_collectA.coinBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), collect_A.coinBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), m_ex_coin_collectA.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), collect_A.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_UNINSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

// 纸币回收箱凭证
void COneKeySettlementSvc::PrintBanknoteChangeReceiption()
{
	try
	{
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO Col_info = theBANKNOTE_COUNT.GetBanknoteboxInfo();

		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), m_ex_note_collect.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), Col_info.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"),  m_ex_note_collect.ulCount_CNY1);
		strFace1Count_after.Format(_T("%d"), Col_info.ulCount_CNY1);
		// 5元
		CString strFace5Count_before = _T("");
		CString strFace5Count_after = _T("");
		strFace5Count_before.Format(_T("%d"), m_ex_note_collect.ulCount_CNY5);
		strFace5Count_after.Format(_T("%d"), Col_info.ulCount_CNY5);
		// 10元
		CString strFace10Count_before = _T("");
		CString strFace10Count_after = _T("");
		strFace10Count_before.Format(_T("%d"), m_ex_note_collect.ulCount_CNY10);
		strFace10Count_after.Format(_T("%d"), Col_info.ulCount_CNY10);
		// 20元
		CString strFace20Count_before = _T("");
		CString strFace20Count_after = _T("");
		strFace20Count_before.Format(_T("%d"), m_ex_note_collect.ulCount_CNY20);
		strFace20Count_after.Format(_T("%d"), Col_info.ulCount_CNY20);
		// 50元
		CString strFace50Count_before = _T("");
		CString strFace50Count_after = _T("");
		strFace50Count_before.Format(_T("%d"), m_ex_note_collect.ulCount_CNY50);
		strFace50Count_after.Format(_T("%d"), Col_info.ulCount_CNY50);
		// 100元
		CString strFace100Count_before = _T("");
		CString strFace100Count_after = _T("");
		strFace100Count_before.Format(_T("%d"), m_ex_note_collect.ulCount_CNY100);
		strFace100Count_after.Format(_T("%d"), Col_info.ulCount_CNY100);

		// 加载Template
		CString CoinBox_templatename = _T("TVM_BANK_NOTE_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));																			// 操作类型
			line.Replace(_T("{钱箱}"), _T("纸币回收箱"));
			line.Replace(_T("{操作前钱箱号}"), m_ex_note_collect.banknoteBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), Col_info.banknoteBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), m_ex_note_collect.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), Col_info.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{操作前5元}"), strFace5Count_before);
			line.Replace(_T("{操作后5元}"), strFace5Count_after);
			line.Replace(_T("{操作前10元}"), strFace10Count_before);
			line.Replace(_T("{操作后10元}"), strFace10Count_after);
			line.Replace(_T("{操作前20元}"), strFace20Count_before);
			line.Replace(_T("{操作后20元}"), strFace20Count_after);
			line.Replace(_T("{操作前50元}"), strFace50Count_before);
			line.Replace(_T("{操作后50元}"), strFace50Count_after);
			line.Replace(_T("{操作前100元}"), strFace100Count_before);
			line.Replace(_T("{操作后100元}"), strFace100Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_UNINSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);

		//throw;
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

void COneKeySettlementSvc::PrintBanknoteChangeBoxReceiption()
{
	try
	{
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), m_ex_note_change.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), changeBox.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"), 0);
		strFace1Count_after.Format(_T("%d"), 0);
		// 5元
		CString strFace5Count_before = _T("");
		CString strFace5Count_after = _T("");
		strFace5Count_before.Format(_T("%d"), m_ex_note_change.ulCurCount);
		strFace5Count_after.Format(_T("%d"), changeBox.ulCurCount);
		// 10元
		CString strFace10Count_before = _T("");
		CString strFace10Count_after = _T("");
		strFace10Count_before.Format(_T("%d"),0);
		strFace10Count_after.Format(_T("%d"), 0);
		// 20元
		CString strFace20Count_before = _T("");
		CString strFace20Count_after = _T("");
		strFace20Count_before.Format(_T("%d"), 0);
		strFace20Count_after.Format(_T("%d"), 0);
		// 50元
		CString strFace50Count_before = _T("");
		CString strFace50Count_after = _T("");
		strFace50Count_before.Format(_T("%d"), 0);
		strFace50Count_after.Format(_T("%d"), 0);
		// 100元
		CString strFace100Count_before = _T("");
		CString strFace100Count_after = _T("");
		strFace100Count_before.Format(_T("%d"), 0);
		strFace100Count_after.Format(_T("%d"), 0);

		// 加载Template
		CString CoinBox_templatename = _T("TVM_BANK_NOTE_BOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;
		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				
		int Index = 0;
		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{车站名称}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));																			// 操作类型
			line.Replace(_T("{钱箱}"), _T("纸币回收箱"));
			line.Replace(_T("{操作前钱箱号}"), m_ex_note_change.banknoteBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), changeBox.banknoteBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), m_ex_note_change.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), changeBox.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{操作前5元}"), strFace5Count_before);
			line.Replace(_T("{操作后5元}"), strFace5Count_after);
			line.Replace(_T("{操作前10元}"), strFace10Count_before);
			line.Replace(_T("{操作后10元}"), strFace10Count_after);
			line.Replace(_T("{操作前20元}"), strFace20Count_before);
			line.Replace(_T("{操作后20元}"), strFace20Count_after);
			line.Replace(_T("{操作前50元}"), strFace50Count_before);
			line.Replace(_T("{操作后50元}"), strFace50Count_after);
			line.Replace(_T("{操作前100元}"), strFace100Count_before);
			line.Replace(_T("{操作后100元}"), strFace100Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_UNINSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);

		//throw;
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      决定是否进行结算操作 

@param      none

@retval     true ：	是
false:	否

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::BalanceOrNot()
{
	bool yes = true;
	ASSERT(!m_modle.mapSettlement.empty());
	map<PROCESS,PROCESS_INFO> &mapModel = m_modle.mapSettlement;
	map<PROCESS,PROCESS_INFO>::iterator itMap =mapModel.begin();
	ASSERT(!mapModel.empty());
	int nFailCount = 0;	// 执行失败的项目
	while(itMap != mapModel.end())
	{
		// 如果 某个收机项目执行开关已经开启 但是执行失败
		if (itMap->first != PROC_BALANCE && itMap->second.procSwitch == SWT_ON && itMap->second.procState == FAIL ){
			yes = false;
			// 发现一项失败的就跳出去 不再执行多余操作
			break;
		}
		itMap++;
	}
	return yes;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置待执行的状态 

@param      none

@retval     true ：	是
false:	否

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::GetState(PROCESS proc,CString strError)
{
	//strError = _opl(GUIDE_ENTER_F2_F3_ESC);
	//bool operated = false;		// 判断收机项是否执行过
	bool ret = false;
	
	switch(proc)
	{
	
		// 卸载token箱
	case PROC_REMOVE_CARDBOX:
		{
			// 是否选择?
			if((m_settings & PROC_REMOVE_CARDBOX) == PROC_REMOVE_CARDBOX){
				// 初始化状态为待执行
				if(m_modle.bPreStatus){
					if (theTOKEN_COUNT.GetTokenboxAInfo().tokenBoxID.ToString() == INVALID_ID &&
						theTOKEN_COUNT.GetTokenboxBInfo().tokenBoxID.ToString() == INVALID_ID &&
						theTOKEN_COUNT.GetTokenboxWastedInfo().tokenBoxID.ToString() == INVALID_ID &&
						theTOKEN_COUNT.GetTokenboxCollectedInfo().tokenBoxID.ToString() == INVALID_ID){
							m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState = DONE;
					}
					else{
						m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState = READY;
					}
				}
				else{
					if (theTOKEN_COUNT.GetTokenboxAInfo().tokenBoxID.ToString() == INVALID_ID &&
						theTOKEN_COUNT.GetTokenboxBInfo().tokenBoxID.ToString() == INVALID_ID &&
						theTOKEN_COUNT.GetTokenboxWastedInfo().tokenBoxID.ToString() == INVALID_ID && 
						theTOKEN_COUNT.GetTokenboxCollectedInfo().tokenBoxID.ToString() == INVALID_ID && 
						theTVM_SETTING.GetCardBoxLastAction() == 1		// 执行成功
						){
							m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState = DONE;
					}
					else{
						if (theTVM_SETTING.GetCardBoxLastAction() == 0){
							m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState = FAIL;
						}
					}
				}
			}
			else{
				m_modle.mapSettlement[PROC_REMOVE_CARDBOX].procState = UNSELECTED;
			}
		}
		break;
		// 回收硬币
	case PROC_COLLECT_COIN:
		// 是否选中?
		if((m_settings & PROC_COLLECT_COIN) == PROC_COLLECT_COIN){
			if(m_modle.bPreStatus){
				if (
					//theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount == 0    &&							// 备用找零箱A已空
					//theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount == 0    &&							// 备用找零箱B已空
					theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount == 0 &&							// 循环找零箱A已空
					theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount == 0 &&
					theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() == INVALID_ID 		// 硬币回收箱A已卸载
					//theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() == INVALID_ID 		// 硬币回收箱B已卸载					
					){
						m_modle.mapSettlement[PROC_COLLECT_COIN].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_COLLECT_COIN].procState = READY;
				}
			}
			else{
				// 成功
				if (
					//theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount == 0    &&							// 备用找零箱A已空
					//theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount == 0    &&							// 备用找零箱B已空
					theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount == 0 &&							// 循环找零箱A已空
					theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount == 0 &&
					//theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() == INVALID_ID &&	// 硬币回收箱B已卸载	
					theTVM_SETTING.GetCollectCoinResult() == 1												// 硬币回收和卸载都成功
					){
					m_modle.mapSettlement[PROC_COLLECT_COIN].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_COLLECT_COIN].procState = FAIL;
				}
			}
		}
		else{
			m_modle.mapSettlement[PROC_COLLECT_COIN].procState = UNSELECTED;
		}
		break;
	//	//卸载找零、回收箱
	//case PROC_REMOVE_CN_CHGBOX:
	//	if((m_settings & PROC_REMOVE_CN_CHGBOX) == PROC_REMOVE_CN_CHGBOX){
	//		if(m_modle.bPreStatus){
	//			if (theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() == INVALID_ID &&
	//				theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() == INVALID_ID &&			// 循环找零箱B已空
	//				theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() == INVALID_ID 
	//				//&&		// 硬币回收箱A已卸载
	//				//theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() == INVALID_ID 		// 硬币回收箱B已卸载
	//				){
	//					m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState = DONE;
	//			}
	//			else{
	//				m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState = READY;
	//			}
	//		}
	//		else{
	//			if (theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() == INVALID_ID &&
	//				theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() == INVALID_ID &&
	//				theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() == INVALID_ID &&
	//				//&&		// 硬币回收箱A已卸载
	//				//theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() == INVALID_ID &&		// 硬币回收箱B已卸载
	//				theSETTING.GetCoinBoxLastAction() == 1			// 执行成功
	//				)
	//			{
	//				m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState = DONE;
	//			}
	//			else{
	//				m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState = FAIL;
	//			}
	//		}
	//	}
	//	else{
	//		m_modle.mapSettlement[PROC_REMOVE_CN_CHGBOX].procState = UNSELECTED;
	//	}
	//	break;
		// 卸载硬币回收箱
	case PROC_REMOVE_CN_COLBOX:
		if((m_settings & PROC_REMOVE_CN_COLBOX) == PROC_REMOVE_CN_COLBOX){
			if(m_modle.bPreStatus){
				if (
					//theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() == INVALID_ID &&
					//theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() == INVALID_ID &&			
					theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() == INVALID_ID 		// 硬币回收箱A已卸载
					//theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() == INVALID_ID 		// 硬币回收箱B已卸载
					){
						m_modle.mapSettlement[PROC_REMOVE_CN_COLBOX].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_REMOVE_CN_COLBOX].procState = READY;
				}
			}
			else{
				if (
					//theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() == INVALID_ID &&
					//theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() == INVALID_ID &&			
					theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() == INVALID_ID &&		// 硬币回收箱A已卸载
					//theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() == INVALID_ID 		// 硬币回收箱B已卸载
					theTVM_SETTING.GetCoinBoxLastAction() == 1			// 执行成功
					)
				{
					m_modle.mapSettlement[PROC_REMOVE_CN_COLBOX].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_REMOVE_CN_COLBOX].procState = FAIL;
				}
			}
		}
		else{
			m_modle.mapSettlement[PROC_REMOVE_CN_COLBOX].procState = UNSELECTED;
		}
		break;
		//回收纸币
	case PROC_COLLECT_CASH:
		{
			if((m_settings & PROC_COLLECT_CASH) == PROC_COLLECT_CASH){
				if(m_modle.bPreStatus){
					if (theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount == 0 &&
						theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount == 0 &&
						theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount == 0 &&
						theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount == 0 
						){
						m_modle.mapSettlement[PROC_COLLECT_CASH].procState = DONE;
					}
					else{
						m_modle.mapSettlement[PROC_COLLECT_CASH].procState = READY;
					}
				}
				else{
					if (theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount == 0 &&
						theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount == 0 &&
						theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount == 0 &&
						theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount == 0 &&
						theTVM_SETTING.GetBanknoteBoxLastAction() == 1			// 执行成功
						){
						m_modle.mapSettlement[PROC_COLLECT_CASH].procState = DONE;
					}
					else{
						m_modle.mapSettlement[PROC_COLLECT_CASH].procState = FAIL;
					}
				}
			}
			else{
				m_modle.mapSettlement[PROC_COLLECT_CASH].procState = UNSELECTED;
			}
		}
		break;
		// 卸载纸币回收箱
	case PROC_REMOVE_BN_COLBOX:
		if((m_settings & PROC_REMOVE_BN_COLBOX) == PROC_REMOVE_BN_COLBOX){
			if(m_modle.bPreStatus){
				if (theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString() == INVALID_ID ){
					m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState = READY;
				}
			}
			else{
				if (theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString() == INVALID_ID ){
					m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState = FAIL;
				}
			}
		}
		else{
			m_modle.mapSettlement[PROC_REMOVE_BN_COLBOX].procState = UNSELECTED;
		}
		break;
	case PROC_REMOVE_BN_CHGBOX:
		if((m_settings & PROC_REMOVE_BN_CHGBOX) == PROC_REMOVE_BN_CHGBOX){
			if(m_modle.bPreStatus){
				if (theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().banknoteBoxID.ToString() == INVALID_ID /*&&
					theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().banknoteBoxID.ToString() == INVALID_ID &&
					theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo().banknoteBoxID.ToString() == INVALID_ID*/){
						m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState = READY;
				}
			}
			else{
				if (theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().banknoteBoxID.ToString() == INVALID_ID /*&&
					theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().banknoteBoxID.ToString() == INVALID_ID &&
					theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo().banknoteBoxID.ToString() == INVALID_ID*/
					){
						m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState = FAIL;
				}
			}
		}
		else{
			m_modle.mapSettlement[PROC_REMOVE_BN_CHGBOX].procState = UNSELECTED;
		}
		break;
	case PROC_BALANCE:
		if((m_settings&PROC_BALANCE) == PROC_BALANCE){
			if(m_modle.bPreStatus){
				m_modle.mapSettlement[PROC_BALANCE].procState = READY;
			}
			else{
				if (theTVM_SETTING.GetBalanceOpTime() == _T("1")){
					m_modle.mapSettlement[PROC_BALANCE].procState = DONE;
				}
				else{
					m_modle.mapSettlement[PROC_BALANCE].procState = FAIL;
				}
			}
		}
		else{
			m_modle.mapSettlement[PROC_BALANCE].procState = UNSELECTED;
		}
		break;
	default:
		break;
	}
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取所有操作的状态 

@param      

@retval     true ：	是
			false:	否

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementSvc::GetStates()
{
	CString strError;
	GetState(PROC_REMOVE_CARDBOX,strError);
	GetState(PROC_COLLECT_COIN,strError);
	GetState(PROC_REMOVE_CN_CHGBOX,strError);
	GetState(PROC_REMOVE_CN_COLBOX,strError);
	GetState(PROC_COLLECT_CASH,strError);
	GetState(PROC_REMOVE_BN_COLBOX,strError);
	GetState(PROC_REMOVE_BN_CHGBOX,strError);
	GetState(PROC_BALANCE,strError);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      卸载硬币找零箱 

@param      

@retval     true ：	是
			false:	否

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool COneKeySettlementSvc::RemoveCoinChangeAndCollectionBox(){

	try{
		// 一键收机过程中：
		//1、如果找零箱和循环找零箱内有钱不允许卸载回收箱，需要手动卸载；
		//2、如果找零箱内有钱，也不允许卸载；
		//3、判断模块是否到位，如果不到位则不允许操作

		// IO状态：回收箱是否在位
		// CH状态：找零箱是否在位
		IO_RSP_STATUS_INFO io_info;
		tCHADevStatus rsp;
		try{
			IO_HELPER->GetIOStatus(io_info);
			COIN_HELPER->CH_GetStatus(&rsp);
			
			if(!rsp.bIsCoinModuleStandby){
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
			theEXCEPTION_MGR.ProcessException(e);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
			Sleep(1000);
			theTVM_SETTING.SetCoinBoxLastAction(0);
			return false;
		}
		catch(...){
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
			Sleep(1000);
			theTVM_SETTING.SetCoinBoxLastAction(0);
			return false;
		}

		m_ex_coin_changeA = theCOIN_COUNT.GetChangeCoinboxAInfo();
		m_ex_coin_changeB = theCOIN_COUNT.GetChangeCoinboxBInfo();

		m_ex_coin_hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
		m_ex_coin_hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();

		m_ex_coin_collectA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		m_ex_coin_collectB = theCOIN_COUNT.GetCollectionCoinboxBInfo();

		bool is_colA_done = false;
		bool is_colB_done = false;

		bool bCanColloctBoxAUninstall = true;
		bool bCanColloctBoxBUninstall = true;
		bool bCanChangeBoxAUninstall  = true;
		bool bCanChangeBoxBUninstall  = true;

		if(m_ex_coin_changeA.ulCount != 0){
			bCanChangeBoxAUninstall = false;
			bCanColloctBoxBUninstall = false;
		}

		if(m_ex_coin_changeB.ulCount != 0){
			bCanChangeBoxBUninstall = false;
			bCanColloctBoxBUninstall = false;
		}

		if(m_ex_coin_hopperA.ulCurCount != 0 || m_ex_coin_hopperB.ulCurCount != 0){
			bCanColloctBoxAUninstall = false;
		}

		// 卸载回收箱A
		if (m_ex_coin_collectA.coinBoxID.ToString() != INVALID_ID &&  bCanColloctBoxAUninstall){

			CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coinCollBox = theCOIN_COUNT.GetCollectionCoinboxAInfo();

			coinCollBox.ulCount_1yuan = coinCollBox.ulCount_5jiao = 0;
			coinCollBox.coinBoxID.bLineCode = 0xFF;
			coinCollBox.coinBoxID.bType		= 0xFF;
			coinCollBox.coinBoxID.bNum		= 0xFF;
			coinCollBox.coinBoxID.bNumInQu = 0xff;
			if(theCOIN_COUNT.SetCollectionCoinboxAInfo(coinCollBox)){
				is_colA_done = true;
				// 入库
				CoinCollectBoxRemoveOperationRecord removecoincollectionboxrecord;
				removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();			// 操作时间
				CoinBoxCashDetail coinboxcashdetail;

				removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtCoinCollectBoxID = m_ex_coin_collectA.coinBoxID.ToString();
				removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iTotalAmount = m_ex_coin_collectA.ulCount_1yuan*100 + m_ex_coin_collectA.ulCount_5jiao * 50;

				coinboxcashdetail.iCoinTypeCode = Coin1Yuan;
				coinboxcashdetail.iQuantity = m_ex_coin_collectA.ulCount_1yuan;
				removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

				coinboxcashdetail.iCoinTypeCode = CoinHalfYuan;
				coinboxcashdetail.iQuantity = m_ex_coin_collectA.ulCount_5jiao;
				removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

				removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;														// 操作员ID
				removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtComment = _T("");
				removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtReserve = _T("");

				DATA_HELPER->DbInsertCoinCollectBoxRemoveOperationRecord(removecoincollectionboxrecord);

				CASHBOX_CHANGE  coinbox;					// 硬币回收箱更换业务数据
				vector<CASH_TYPE_COUNT> vecCash;			// 硬币回收箱币种代码、数量

				// 硬币回收箱A、B--操作类型
				coinbox.handleType = UNINSTALL;
				// 硬币回收箱状态
				coinbox.State = VALID_DETACH;

				CASH_TYPE_COUNT cash;
				cash.cash_type = Coin1Yuan;
				cash.cash_count =  m_ex_coin_collectA.ulCount_1yuan;
				vecCash.push_back(cash);

				//cash.cash_type = CoinHalfYuan;
				//cash.cash_count =  m_ex_coin_collectA.ulCount_5jiao;
				//vecCash.push_back(cash);

				coinbox.SetBoxID(m_ex_coin_collectA.coinBoxID.bLineCode, m_ex_coin_collectA.coinBoxID.bType,m_ex_coin_collectA.coinBoxID.bNumInQu,m_ex_coin_collectA.coinBoxID.bNum);
				// 硬币回收箱A位置ID
				coinbox.position = MAGAZINEG;
				// 硬币回收箱币种代码、数量
				coinbox.vecCash = vecCash;
				// 硬币回收箱币种总数
				coinbox.btCount = 1;
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(ONE_KEY_SETTLEMENT_SVC)->InsertMsgBanknoteBoxExchange(coinbox);
			}
		}

		//// 卸载回收箱B
		//if (m_ex_coin_collectB.coinBoxID.ToString() != INVALID_ID && bCanColloctBoxBUninstall){
		//	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coinCollBox = theCOIN_COUNT.GetCollectionCoinboxBInfo();

		//	coinCollBox.ulCount_1yuan = coinCollBox.ulCount_5jiao = 0;
		//	coinCollBox.coinBoxID.bLineCode = 0xFF;
		//	coinCollBox.coinBoxID.bType		= 0xFF;
		//	coinCollBox.coinBoxID.bNum		= 0xFF;
		//	coinCollBox.coinBoxID.bNumInQu = 0xff;
		//	if(theCOIN_COUNT.SetCollectionCoinboxBInfo(coinCollBox)){
		//		is_colB_done = true;
		//		// 操作数据入库
		//		CoinCollectBoxRemoveOperationRecord removecoincollectionboxrecord;
		//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();			// 操作时间
		//		CoinBoxCashDetail coinboxcashdetail;

		//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtCoinCollectBoxID = m_ex_coin_collectB.coinBoxID.ToString();
		//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iTotalAmount = m_ex_coin_collectB.ulCount_1yuan*100 + m_ex_coin_collectB.ulCount_5jiao*50;

		//		coinboxcashdetail.iCoinTypeCode = Coin1Yuan;
		//		coinboxcashdetail.iQuantity = m_ex_coin_collectB.ulCount_1yuan;
		//		removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

		//		coinboxcashdetail.iCoinTypeCode = CoinHalfYuan;
		//		coinboxcashdetail.iQuantity = m_ex_coin_collectB.ulCount_5jiao;
		//		removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

		//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;														// 操作员ID
		//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtComment = _T("");
		//		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtReserve = _T("");

		//		DATA_HELPER->DbInsertCoinCollectBoxRemoveOperationRecord(removecoincollectionboxrecord);

		//		// 上传电文
		//		CASHBOX_CHANGE  coinbox;					// 硬币回收箱更换业务数据
		//		vector<CASH_TYPE_COUNT> vecCash;			// 硬币回收箱币种代码、数量
		//		// 硬币回收箱A、B--操作类型
		//		coinbox.handleType = UNINSTALL;
		//		// 硬币回收箱状态
		//		coinbox.State = VALID_DETACH;

		//		CASH_TYPE_COUNT cash;
		//		cash.cash_type = Coin1Yuan;
		//		cash.cash_count =  m_ex_coin_collectB.ulCount_1yuan;
		//		vecCash.push_back(cash);

		//		//cash.cash_type = CoinHalfYuan;
		//		//cash.cash_count =  m_ex_coin_collectB.ulCount_5jiao;
		//		//vecCash.push_back(cash);
		//		// 硬币回收箱B--ID
		//		coinbox.SetBoxID(m_ex_coin_collectB.coinBoxID.bLineCode, m_ex_coin_collectB.coinBoxID.bType,m_ex_coin_collectB.coinBoxID.bNumInQu,m_ex_coin_collectB.coinBoxID.bNum);
		//		// 硬币回收箱位置ID
		//		coinbox.position = MAGAZINEG;
		//		// 硬币回收箱状态
		//		coinbox.State = VALID_DETACH;
		//		// 硬币回收箱币种代码、数量
		//		coinbox.vecCash = vecCash;
		//		// 硬币回收箱币种总数
		//		coinbox.btCount = 1;//coinbox.GetSumCurrency();

		//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(ONE_KEY_SETTLEMENT_SVC)->InsertMsgBanknoteBoxExchange(coinbox);
		//	}
		//}


		//bool bIsChangeBoxAUninstall = false,bIsChangeBoxBUninstall = false;
		//// 卸载硬币找零箱A
		//if(m_ex_coin_changeA.coinBoxID.ToString() != INVALID_ID && rsp.bIsAddBox1Exist && bCanChangeBoxAUninstall){
		//	CTCoinCountInfo::COIN_CHANGE_BOX_INFO info;
		//	info.ulLastLeftCount = theCOIN_COUNT.GetChangeCoinboxAInfo().ulLastLeftCount;

		//	if(theCOIN_COUNT.SetChangeCoinboxAInfo(info)){
		//		bIsChangeBoxAUninstall = true;

		//		// 上送电文
		//		CASHBOX_CHANGE	cash_change;
		//		cash_change.handleType = UNINSTALL;
		//		cash_change.State	 = VALID_DETACH;
		//		vector<CASH_TYPE_COUNT>  vecCash;
		//		cash_change.SetBoxID(m_ex_coin_changeA.coinBoxID.bLineCode,m_ex_coin_changeA.coinBoxID.bType,m_ex_coin_changeA.coinBoxID.bNumInQu,m_ex_coin_changeA.coinBoxID.bNum);
		//		cash_change.position = MAGAZINEC;
		//		cash_change.btCount = 1;
		//		CASH_TYPE_COUNT coin;
		//		coin.cash_type = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());	// 面额
		//		coin.cash_count= m_ex_coin_changeA.ulCount; // 数量
		//		vecCash.push_back(coin);
		//		cash_change.vecCash = vecCash;

		//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(cash_change);

		//		// 写入数据库
		//		try{
		//			// 箱子卸载记录
		//			Data::CoinChangeBoxOperationRecord coinChangeBoxOperation;
		//			coinChangeBoxOperation.dtOperationTime = ComGetCurTime();
		//			coinChangeBoxOperation.iOperationType  = 2;//UNINSTALL_CHANGE_BOX
		//			coinChangeBoxOperation.txtOperatorID	  = theAPP_SESSION.GetUserInfo().sUserID;
		//			coinChangeBoxOperation.txtCoinChangeBoxID = m_ex_coin_changeA.coinBoxID.ToString();
		//			coinChangeBoxOperation.iCoinTypeCode	  = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
		//			coinChangeBoxOperation.iQuantity		  = m_ex_coin_changeA.ulCount;

		//			DATA_HELPER->DbInsertCoinChangeBoxOperationRecord(coinChangeBoxOperation);
		//		}
		//		catch(CSysException& e){
		//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
		//			theEXCEPTION_MGR.ProcessException(e);
		//		}
		//		catch(...){
		//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
		//		}
		//	}
		//}

		//// 卸载硬币找零箱B
		//if(m_ex_coin_changeB.coinBoxID.ToString() != INVALID_ID && rsp.bIsAddBox2Exist && bCanChangeBoxBUninstall){
		//	CTCoinCountInfo::COIN_CHANGE_BOX_INFO info;
		//	info.ulLastLeftCount = theCOIN_COUNT.GetChangeCoinboxBInfo().ulLastLeftCount;

		//	if(theCOIN_COUNT.SetChangeCoinboxBInfo(info)){
		//		bIsChangeBoxBUninstall = true;

		//		// 上送电文
		//		CASHBOX_CHANGE	cash_change;
		//		cash_change.handleType = UNINSTALL;
		//		cash_change.State	 = VALID_DETACH;
		//		vector<CASH_TYPE_COUNT>  vecCash;
		//		cash_change.SetBoxID(m_ex_coin_changeB.coinBoxID.bLineCode,m_ex_coin_changeB.coinBoxID.bType,m_ex_coin_changeB.coinBoxID.bNumInQu,m_ex_coin_changeB.coinBoxID.bNum);
		//		cash_change.position = MAGAZINEB;
		//		cash_change.btCount = 1;
		//		CASH_TYPE_COUNT coin;
		//		coin.cash_type = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());	// 面额
		//		coin.cash_count= m_ex_coin_changeB.ulCount; // 数量
		//		vecCash.push_back(coin);
		//		cash_change.vecCash = vecCash;

		//		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(cash_change);

		//		// 写入数据库
		//		try{
		//			// 箱子卸载记录
		//			Data::CoinChangeBoxOperationRecord coinChangeBoxOperation;
		//			coinChangeBoxOperation.dtOperationTime = ComGetCurTime();
		//			coinChangeBoxOperation.iOperationType  = 2;//UNINSTALL_CHANGE_BOX
		//			coinChangeBoxOperation.txtOperatorID	  = theAPP_SESSION.GetUserInfo().sUserID;
		//			coinChangeBoxOperation.txtCoinChangeBoxID = m_ex_coin_changeB.coinBoxID.ToString();
		//			coinChangeBoxOperation.iCoinTypeCode	  = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
		//			coinChangeBoxOperation.iQuantity		  = m_ex_coin_changeB.ulCount;

		//			DATA_HELPER->DbInsertCoinChangeBoxOperationRecord(coinChangeBoxOperation);
		//		}
		//		catch(CSysException& e){
		//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
		//			theEXCEPTION_MGR.ProcessException(e);
		//		}
		//		catch(...){
		//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
		//		}
		//	}
		//}

		// 打印凭证
		if (is_colA_done/* || is_colB_done || bIsChangeBoxAUninstall || bIsChangeBoxBUninstall*/){
			PrintCoinBoxExchangeReceipt();
		}
		theTVM_SETTING.SetCoinBoxLastAction(1);
		return true;
	}
	catch(CSysException& e){
		theTVM_SETTING.SetCoinBoxLastAction(0);
		theEXCEPTION_MGR.ProcessException(e);
		return false;
	}
	catch(...){
		theTVM_SETTING.SetCoinBoxLastAction(0);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币找零箱子操作插入库

@param      BOX_INDEX boxType 箱子类型

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COneKeySettlementSvc::InsertOperaterToDatabase(int boxType,CString strBoxID,int nCount){
	try{
		Data::BanknoteOperationRecord banknoteOpRecord;

		banknoteOpRecord.dtTransTime = ComGetCurTime();
		banknoteOpRecord.iOperationType = 0x02;// 卸载
		banknoteOpRecord.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;
		if(boxType == MAGAZINEH){
			banknoteOpRecord.txtBanknoteBoxID = strBoxID;
			banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());
			banknoteOpRecord.iQuantity		   = nCount;
		}
		/*else if(boxType == BANKNOTE_CHANGE_BOX_B_POS){
			banknoteOpRecord.txtBanknoteBoxID = strBoxID;
			banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_INFO.GetBHChangeBoxBType());
			banknoteOpRecord.iQuantity		   = nCount;
		}
		else if(boxType == BANKNOTE_INVALID_POS){
			banknoteOpRecord.txtBanknoteBoxID = strBoxID;
			banknoteOpRecord.iBanknoteTypeCode = TransBanknoteType(theTVM_INFO.GetBHChangeBoxAType());
			banknoteOpRecord.iQuantity		   = nCount;
		}*/
		else{
			return;
		}
		DATA_HELPER->DbInsertBanknoteOperationRecord(banknoteOpRecord);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}
// 回收纸币
bool COneKeySettlementSvc::CollectBanknote()
{
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	CTBanknoteCountInfo::BANKNOTE_BOX_INFO collBox = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	int nBillFullCnt = theAFC_TVM_OPERATION.GetRecycleBoxFullCount();
	if (nBillFullCnt <= (recyclerA.ulCurCount + recyclerB.ulCurCount + recyclerC.ulCurCount + recyclerD.ulCurCount
		+ changeBox.ulCurCount + collBox.BankntoeTotalCount()))
	{
		theAPP_SESSION.ShowOfficeGuide(_T("纸币回收箱容量不足，请更换回收箱")/*_opl(TXT_BANKNOTEMGR_COLLECTION_BOX_ALREADYFULL)*/);
		Sleep(1000);
		theTVM_SETTING.SetCollectBanknoteResult(0);
		return false;
	}
	nBillFullCnt -= collBox.BankntoeTotalCount();

	DWORD dwLaftCount = recyclerA.BanknotesTotalCount() + recyclerB.BanknotesTotalCount() + recyclerC.BanknotesTotalCount() + recyclerD.BanknotesTotalCount() + changeBox.BanknotesTotalCount();
	try
	{
		if(BANKNOTE_HELPER->IsCashBoxAttached()){
			if(dwLaftCount > 0 && nBillFullCnt > 0){
				tBncChangeNum result;

				long lErrCode = 0;
				map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> cashUnit = BANKNOTE_HELPER->GetEmptyFull();
				lErrCode = BANKNOTE_HELPER->Empty((char*)"R",&result);

				if(recyclerA.ulCurCount > 0|| cashUnit[BNR_RECYCLER_BOX_A].count > 0){
					if(lErrCode == 0){
						LOG("Empty_A: %d", result.uiOutNumber[0]);
						if (theTVM_SETTING.GetBNRRecycleBoxAAmount()==10){
							collBox.ulCount_CNY10 += result.uiOutNumber[0];
						}
						else if (theTVM_SETTING.GetBNRRecycleBoxAAmount()==5){
							collBox.ulCount_CNY5 += result.uiOutNumber[0];
						}	
					}
					nBillFullCnt -= result.uiOutNumber[0];
					recyclerA.ulCurCount -= result.uiOutNumber[0];
					if(recyclerA.ulCurCount <0 )recyclerA.ulCurCount = 0;
					else if(recyclerA.ulCurCount > 0 && lErrCode == 0)recyclerA.ulCurCount = 0;
					theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(recyclerA);

				}

				if(nBillFullCnt > 0 && (recyclerB.ulCurCount > 0 || cashUnit[BNR_RECYCLER_BOX_B].count > 0)){
					//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_B_NAME,&result);
					if(lErrCode == 0){
						LOG("Empty_B: %d", result.uiOutNumber[1]);
						if (theTVM_SETTING.GetBNRRecycleBoxBAmount()==10){
							collBox.ulCount_CNY10 += result.uiOutNumber[1];
						}
						else if (theTVM_SETTING.GetBNRRecycleBoxBAmount()==5){
							collBox.ulCount_CNY5 += result.uiOutNumber[1];
						}	
					}
					nBillFullCnt -= result.uiOutNumber[1];
					recyclerB.ulCurCount -= result.uiOutNumber[1];
					if(recyclerB.ulCurCount <0 )recyclerB.ulCurCount = 0;
					else if(recyclerB.ulCurCount > 0 && lErrCode == 0)recyclerB.ulCurCount = 0;
					theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(recyclerB);
				}
				if(nBillFullCnt > 0 && (recyclerC.ulCurCount > 0 || cashUnit[BNR_RECYCLER_BOX_C].count > 0)){
					//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_C_NAME,&result);
					if(lErrCode == 0){
						LOG("Empty_C: %d", result.uiOutNumber[2]);
						if (theTVM_SETTING.GetBNRRecycleBoxCAmount()==10){
							collBox.ulCount_CNY10 += result.uiOutNumber[2];
						}
						else if (theTVM_SETTING.GetBNRRecycleBoxCAmount()==5){
							collBox.ulCount_CNY5 += result.uiOutNumber[2];
						}	
					}
					nBillFullCnt -= result.uiOutNumber[2];
					recyclerC.ulCurCount -= result.uiOutNumber[2];
					if(recyclerC.ulCurCount <0 )recyclerC.ulCurCount = 0;
					else if(recyclerC.ulCurCount > 0 && lErrCode == 0)recyclerC.ulCurCount = 0;
					theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(recyclerC);
				}
				if(nBillFullCnt > 0 && (recyclerD.ulCurCount > 0 || cashUnit[BNR_RECYCLER_BOX_D].count > 0)){
					//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_RECYCLER_BOX_D_NAME,&result);
					if(lErrCode == 0){
						LOG("Empty_D: %d", result.uiOutNumber[3]);
						if (theTVM_SETTING.GetBNRRecycleBoxDAmount()==10){
							collBox.ulCount_CNY10 += result.uiOutNumber[3];
						}
						else if (theTVM_SETTING.GetBNRRecycleBoxDAmount()==5){
							collBox.ulCount_CNY5 += result.uiOutNumber[3];
						}	
					}
					nBillFullCnt -= result.uiOutNumber[3];
					recyclerD.ulCurCount -= result.uiOutNumber[3];
					if(recyclerD.ulCurCount <0 )recyclerD.ulCurCount = 0;
					else if(recyclerD.ulCurCount > 0 && lErrCode == 0)recyclerD.ulCurCount = 0;
					theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(recyclerD);
				}
				if (nBillFullCnt > 0 && (changeBox.ulCurCount > 0 || cashUnit[BNR_MODULE_CHANGE_BOX].count > 0))
				{
					//lErrCode = BANKNOTE_HELPER->Empty((char*)BNR_MODULE_LOADER_BOX_NAME,&result);
					if(lErrCode == 0){
						LOG("Empty_L: total-%d", result.uiOutNumber[4]);
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==10){
							collBox.ulCount_CNY10 += result.uiOutNumber[4];
							changeBox.ulCurCount -= result.uiOutNumber[4];
						}
						else if (theTVM_SETTING.GetBNRChangeBoxAmount()==5){
							collBox.ulCount_CNY5 += result.uiOutNumber[4];
							changeBox.ulCurCount -= result.uiOutNumber[4];
						}	
					}
					changeBox.ulCurCount -= result.uiOutNumber[4];
					if(changeBox.ulCurCount <0 )changeBox.ulCurCount = 0;
					else if(changeBox.ulCurCount > 0 && lErrCode == 0)changeBox.ulCurCount = 0;
					theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBox);

				}
				// 同步更新纸币回收模块数量
				theBANKNOTE_COUNT.SetBanknoteboxInfo(collBox);
				if (nBillFullCnt < 0)
				{
					theAPP_SESSION.ShowOfficeGuide(_T("回收箱已满，纸币未完全回收。"));
				}
				PrintBanknoteCollectionReception();

				if(lErrCode != 0){
					theTVM_SETTING.SetCollectBanknoteResult(0);
					return false;
				}
				theTVM_SETTING.SetCollectBanknoteResult(1);
			}
			else{
				if(dwLaftCount <=0 ){
					theAPP_SESSION.ShowOfficeGuide(_T("纸币循环模块无纸币可以回收。"));
				}
				if(nBillFullCnt <=0 ){
					theAPP_SESSION.ShowOfficeGuide(_T("纸币回收箱容量不足，回收纸币失败。"));
				}
				Sleep(1000);
				theTVM_SETTING.SetCollectBanknoteResult(0);
				return false;
			}
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_NOT_STANDBY_FOR_UNINSTALL));
			Sleep(1000);
			theTVM_SETTING.SetCollectBanknoteResult(0);
			return false;
		}
	}
	catch(...){

	}
	return true;
}
void COneKeySettlementSvc::PrintBanknoteCollectionReception()
{
	try
	{
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteCashInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO banknoteChangeBoxInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxAInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxBInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxCInfo;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO cycleBoxDInfo;
		banknoteCashInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		banknoteChangeBoxInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
		cycleBoxAInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
		cycleBoxBInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
		cycleBoxCInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
		cycleBoxDInfo = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

		// 加载Template
		CString CoinBox_templatename = _T("TVM_BANK_NOTE_COLLECT.template");
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
			if(line.Find(_T("循环找零箱1"))>=0){
				line.Replace(_T("{操作前金额}"), ComFormatAmount(m_ex_note_recycleA.BankNotesCountAvos()));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(cycleBoxAInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("循环找零箱2"))>=0){
				line.Replace(_T("{操作前金额}"), ComFormatAmount(m_ex_note_recycleB.BankNotesCountAvos()));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(cycleBoxBInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("循环找零箱3"))>=0){
				line.Replace(_T("{操作前金额}"), ComFormatAmount(m_ex_note_recycleC.BankNotesCountAvos()));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(cycleBoxCInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("循环找零箱4"))>=0){
				line.Replace(_T("{操作前金额}"), ComFormatAmount(m_ex_note_recycleD.BankNotesCountAvos()));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(cycleBoxDInfo.BankNotesCountAvos()));
			}
			if(line.Find(_T("合计"))>=0){
				int nTotalBefore = m_ex_note_recycleA.BankNotesCountAvos() + m_ex_note_recycleB.BankNotesCountAvos()
					+ m_ex_note_recycleC.BankNotesCountAvos() + m_ex_note_recycleD.BankNotesCountAvos();
				int nTotalAfter = cycleBoxAInfo.BankNotesCountAvos() + cycleBoxBInfo.BankNotesCountAvos()
					+ cycleBoxCInfo.BankNotesCountAvos() + cycleBoxDInfo.BankNotesCountAvos();
				line.Replace(_T("{操作前金额}"), ComFormatAmount(nTotalBefore));
				line.Replace(_T("{操作后金额}"), ComFormatAmount(nTotalAfter));
			}
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray saText;
			ComPrintLineFormatForTemplate(line, saText);
			printtext.Append(saText);
			Index++;
		}
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);//打印凭证
	}
	catch (CSysException& e)
	{
		//throw;
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_PRINT_FAILED));
	}
	catch (...)
	{
		throw CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__,_T("凭证打印失败"));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_BANKNOTEMGR_BOX_UNINSTALL_OK_PRINT_FAILED));
	}


}
CString COneKeySettlementSvc::FormatAmount(long amount)
{
	switch(amount)
	{
	case 100:return _T(" 1元");
	case 500:return _T(" 5元");
	case 1000:return _T("10元");
	case 2000:return _T("20元");
	case 5000:return _T("50元");
	case 10000:return _T("100元");
	}
	return _T("");
}
