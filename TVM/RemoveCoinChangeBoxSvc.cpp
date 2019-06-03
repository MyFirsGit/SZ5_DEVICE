#include "StdAfx.h"
#include "RemoveCoinChangeBoxSvc.h"
#include "RemoveCoinChangeBoxDlg.h"
#include "ctcoincountinfo.h"
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


BEGIN_MESSAGE_MAP(CRemoveCoinChangeBoxSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRemoveCoinChangeBoxSvc::CRemoveCoinChangeBoxSvc()
	:CTVMForeService(REMOVE_COIN_CHANGE_BOX_SVC)
{

	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20101_SYSTEM_SET_DLG, new CRemoveCoinChangeBoxDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DETACH_COLLECTION_BOX,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DETACH_COLLECTION_BOX,IDD_20101_SYSTEM_SET_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DETACH_COLLECTION_BOX);

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRemoveCoinChangeBoxSvc::~CRemoveCoinChangeBoxSvc()
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
void CRemoveCoinChangeBoxSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	
	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DETACH_COLLECTION_BOX);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_DETACH_COLLECTION_BOX);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	InitializeData();
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CRemoveCoinChangeBoxSvc::Model& CRemoveCoinChangeBoxSvc::GetDataModel()
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
void CRemoveCoinChangeBoxSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
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
LRESULT CRemoveCoinChangeBoxSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}
	int input = theAPP_SESSION.GetInputToInt();														// 获取向导栏输入内容
	if (input > 3 || input < 1){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
	}else if(input < 3){
		CHANGE_BOX_INDEX index = (CHANGE_BOX_INDEX)input;
		if ( index == CHANGE_BOX_A){
			if (IsBoxCouldBeRemoved(index)){
				m_DataModel.boxAinfo.state = ST_PRE_REMOVE;
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			}else{
				theAPP_SESSION.ShowOfficeGuide(_T("所选硬币箱不可卸载"));
			}
		}else{
			if (IsBoxCouldBeRemoved(index)){
				m_DataModel.boxBinfo.state = ST_PRE_REMOVE;
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			}else{
				//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				theAPP_SESSION.ShowOfficeGuide(_T("所选硬币箱不可卸载"));
			}
		}
	}else{
		if(IsBoxCouldBeRemoved(CHANGE_BOX_A)){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			m_DataModel.boxAinfo.state = ST_PRE_REMOVE;
		}
		if(IsBoxCouldBeRemoved(CHANGE_BOX_B)){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			m_DataModel.boxBinfo.state = ST_PRE_REMOVE;
		}
		if (!(IsBoxCouldBeRemoved(CHANGE_BOX_A) || IsBoxCouldBeRemoved(CHANGE_BOX_B))){
			theAPP_SESSION.ShowOfficeGuide(_T("无可卸载的硬币找零箱"));
		}
	}	
	NotifyDataModelChanged();	
	return 0;
	
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
LRESULT CRemoveCoinChangeBoxSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	InitializeData();
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
LRESULT CRemoveCoinChangeBoxSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{	
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}

	CString strTips = _T("");
	bool isOperated = (m_DataModel.boxAinfo.state == ST_PRE_REMOVE) || (m_DataModel.boxBinfo.state == ST_PRE_REMOVE);
	
	if(!isOperated){
		return FALSE;
	}

	if (m_DataModel.boxAinfo.state == ST_PRE_REMOVE)
	{
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO info;
		info.ulLastLeftCount = theCOIN_COUNT.GetChangeCoinboxAInfo().ulLastLeftCount;
		
		if(theCOIN_COUNT.SetChangeCoinboxAInfo(info)){
			strTips += _T("硬币找零箱A卸载成功;");
			m_DataModel.boxAinfo.nCount = 0;
			m_DataModel.boxAinfo.state = ST_REMOVED;
			Sendcables(CHANGE_BOX_A);
			InsertCoinBoxToDatabase(CHANGE_BOX_A);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_REMOVAL,true);
		}
		else{ 
			strTips += _T("硬币找零箱A卸载失败");

			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_REMOVAL,false);
		}
	}
	if (m_DataModel.boxBinfo.state == ST_PRE_REMOVE)
	{
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO info;
		info.ulLastLeftCount = theCOIN_COUNT.GetChangeCoinboxBInfo().ulLastLeftCount;
		if(theCOIN_COUNT.SetChangeCoinboxBInfo(info)){
			// 硬计数
			m_DataModel.boxBinfo.state = ST_REMOVED;
			m_DataModel.boxBinfo.nCount = 0;
			strTips += _T("硬币找零箱B卸载成功;");
			Sendcables(CHANGE_BOX_B);
			InsertCoinBoxToDatabase(CHANGE_BOX_A);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_REMOVAL,true);
		}
		else{
			strTips += _T("硬币找零箱B卸载失败;");
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_REMOVAL,false);
		}
	}
	if (isOperated)	{
		PrintCoinBoxExchangeReceipt();
	}
	
	// 更新初始化数据
	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	strTips += _opl(GUIDE_F3_ESC);

	theAPP_SESSION.ShowOfficeGuide(strTips);

	// 更新硬件计数
	try{
		COIN_HELPER->CH_SetCoinNum();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}

	if(m_DataModel.IsPrintFailed){
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	}
	NotifyDataModelChanged();
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币循环找零箱加币凭证打印

@param      int Box_No

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxSvc::PrintCoinBoxExchangeReceipt()
{
	try
	{
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

		CTCoinCountInfo::COIN_BOX_INFO hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
		CTCoinCountInfo::COIN_BOX_INFO hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

		CString coincount = _T("");			
		CString cointotal;

		CString CoinBox_templatename = _T("TVM_COINBOX.template");
		CStringArray printtext;
		CStringArray TemplateContents;

		BOOL IsLoad = theTEMPLATE_HELPER.LoadBillTemplate(CoinBox_templatename,TemplateContents);				// 加载Template

		DWORD dwTotalAmountBefore = (mEx_change_A.ulCount + mEx_change_B.ulCount) * 100 /*+ mEx_hopper_A.ulCurCount * 50*/ + mEx_hopper_B.ulCurCount * 100 + mEx_collect_A.GetCurAmount() /*+ mEx_collect_B.GetCurAmount()*/;
		DWORD dwTotalAmountAfter  = (change_A.ulCount + change_B.ulCount) * 100 /*+ hopper_A.ulCurCount * 50*/ + hopper_B.ulCurCount * 100 + collect_A.GetCurAmount() /*+ collect_B.GetCurAmount()*/;

		int Index = 0;
		CString strCount;

		int nHalfCount = TemplateContents.GetCount() / 2 - 2;

		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{安装}"),_T("卸载"));																				// 操作类型

			line.Replace(_T("{面值1}"),_T("1元"));
			line.Replace(_T("{面值2}"),_T("1元"));
			line.Replace(_T("{面值3}"),_T("0.5元"));
			line.Replace(_T("{面值4}"),_T("1元"));

			// 操作前合计金额
			line.Replace(_T("{前合计}"),ComFormatAmount(dwTotalAmountBefore));

			// 操作前 ID
			line.Replace(_T("{前ID1}"),mEx_change_A.coinBoxID.ToString());
			line.Replace(_T("{前ID2}"),mEx_change_B.coinBoxID.ToString());
			line.Replace(_T("{前ID3}"),mEx_hopper_A.coinBoxID.ToString());
			line.Replace(_T("{前ID4}"),mEx_hopper_B.coinBoxID.ToString());
			line.Replace(_T("{前ID5}"),mEx_collect_A.coinBoxID.ToString());
			line.Replace(_T("{前ID6}"),mEx_collect_B.coinBoxID.ToString());

			// 操作前 数量
			line.Replace(_T("{前数量1}"),ComFormatCount(mEx_change_A.ulCount     ));
			line.Replace(_T("{前数量2}"),ComFormatCount(mEx_change_B.ulCount     ));
			line.Replace(_T("{前数量3}"),ComFormatCount(mEx_hopper_A.ulCurCount  ));
			line.Replace(_T("{前数量4}"),ComFormatCount(mEx_hopper_B.ulCurCount  ));
			line.Replace(_T("{前数量5}"),ComFormatCount(mEx_collect_A.ulCount_1yuan));
			line.Replace(_T("{前数量6}"),ComFormatCount(mEx_collect_A.ulCount_5jiao));
			line.Replace(_T("{前数量7}"),ComFormatCount(mEx_collect_B.ulCount_1yuan));
			line.Replace(_T("{前数量8}"),ComFormatCount(mEx_collect_B.ulCount_5jiao));
			line.Replace(_T("{前总数量A}"),ComFormatCount(mEx_collect_A.GetCurCount()));
			line.Replace(_T("{前总数量B}"),ComFormatCount(mEx_collect_B.GetCurCount()));


			// 操作后 
			if(Index > nHalfCount){

				if(mEx_change_A.coinBoxID.ToString() != change_A.coinBoxID.ToString() || mEx_change_A.ulCount != change_A.ulCount){
					line.Replace(_T("硬币找零箱A"),_T("*硬币找零箱A"));
				}

				if(mEx_change_B.coinBoxID.ToString() != change_B.coinBoxID.ToString() || mEx_change_B.ulCount != change_B.ulCount){
					line.Replace(_T("硬币找零箱B"),_T("*硬币找零箱B"));
				}

				if(mEx_hopper_A.coinBoxID.ToString() != hopper_A.coinBoxID.ToString() || mEx_hopper_A.ulCurCount != hopper_A.ulCurCount){
					line.Replace(_T("循环找零箱A"),_T("*循环找零箱A"));
				}

				if(mEx_hopper_B.coinBoxID.ToString() != hopper_B.coinBoxID.ToString() || mEx_hopper_B.ulCurCount != hopper_B.ulCurCount){
					line.Replace(_T("循环找零箱B"),_T("*循环找零箱B"));
				}

				if(mEx_collect_A.coinBoxID.ToString() != collect_A.coinBoxID.ToString() || mEx_collect_A.GetCurCount() != collect_A.GetCurCount()){
					line.Replace(_T("回收箱A"),_T("*回收箱A"));
				}

				if(mEx_collect_B.coinBoxID.ToString() != collect_B.coinBoxID.ToString() || mEx_collect_B.GetCurCount() != collect_B.GetCurCount()){
					line.Replace(_T("回收箱B"),_T("*回收箱B"));
				}
				// 操作后总金额
				line.Replace(_T("{后合计}"),ComFormatAmount(dwTotalAmountAfter));

				// 操作后 ID
				line.Replace(_T("{后ID1}"),change_A.coinBoxID.ToString());
				line.Replace(_T("{后ID2}"),change_B.coinBoxID.ToString());
				line.Replace(_T("{后ID3}"),hopper_A.coinBoxID.ToString());
				line.Replace(_T("{后ID4}"),hopper_B.coinBoxID.ToString());
				line.Replace(_T("{后ID5}"),collect_A.coinBoxID.ToString());
				line.Replace(_T("{后ID6}"),collect_B.coinBoxID.ToString());

				// 操作后 数量
				line.Replace(_T("{后数量1}"),ComFormatCount(change_A.ulCount    ));
				line.Replace(_T("{后数量2}"),ComFormatCount(change_B.ulCount    ));
				line.Replace(_T("{后数量3}"),ComFormatCount(hopper_A.ulCurCount ));
				line.Replace(_T("{后数量4}"),ComFormatCount(hopper_B.ulCurCount ));
				line.Replace(_T("{后数量5}"),ComFormatCount(collect_A.ulCount_1yuan));
				line.Replace(_T("{后数量6}"),ComFormatCount(collect_A.ulCount_5jiao));
				line.Replace(_T("{后数量7}"),ComFormatCount(collect_B.ulCount_1yuan));
				line.Replace(_T("{后数量8}"),ComFormatCount(collect_B.ulCount_5jiao));
				line.Replace(_T("{后总数量A}"),ComFormatCount(collect_A.GetCurCount()));
				line.Replace(_T("{后总数量B}"),ComFormatCount(collect_B.GetCurCount()));

				line.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
				line.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
				line.Replace(_T("{车站名称}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
				line.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());
			}

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_UNINSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsPrintFailed = true;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.IsPrintFailed = true;
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      数据导入数据库

@param      int collectionboxnumber

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxSvc::Dataintodatabase(int collectionboxnumber)
{
	try
	{
		// 暂缺硬币找零箱卸载数据库接口
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断箱子是否可被执行卸载操作

@param      const CHANGE_BOX_INDEX& index 硬币找零箱的位置

@retval     true ： 可以  ； false ：不可以

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool  CRemoveCoinChangeBoxSvc::IsBoxCouldBeRemoved(const CHANGE_BOX_INDEX& index)
{
	bool  couldbe = false;
	
	switch(index)
	{
	case CHANGE_BOX_A:
		{
			if (theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() != _T("FFFFFFFF")){	// 如果软计数中对应位置有数据
				couldbe = true;
			}
		}break;
	case CHANGE_BOX_B:
		{
			if (theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() != _T("FFFFFFFF")){	// 如果软计数中对应位置有数据
				couldbe = true;
			}
		}break;
	default:
		break;
	}
	return couldbe;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      数据初始化

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxSvc::InitializeData()
{
	//// 同步计数，如果需要的话。
	//if(!COIN_HELPER->SyncCoinCountInfoToFile())
	//	m_DataModel.IsHasException = false;

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO infoA = theCOIN_COUNT.GetChangeCoinboxAInfo();
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO infoB = theCOIN_COUNT.GetChangeCoinboxBInfo();

	CHANGE_BOX_INFO &boxA = m_DataModel.boxAinfo;		// 判断状态
	boxA.strID = infoA.coinBoxID.ToString();
	boxA.nCount = infoA.ulCount;
	boxA.value = (BankNoteAndCoinType_t)infoA.ulValue;
	if (boxA.strID == _T("FFFFFFFF")){
		boxA.state = ST_REMOVED;
	}else{ boxA.state = ST_INSTALLED;}

	CHANGE_BOX_INFO &boxB = m_DataModel.boxBinfo;
	boxB.strID = infoB.coinBoxID.ToString();
	boxB.nCount = infoB.ulCount;
	boxB.value = (BankNoteAndCoinType_t)infoB.ulValue;
	if (boxB.strID == _T("FFFFFFFF")){
		boxB.state = ST_REMOVED;
	}else{ boxB.state = ST_INSTALLED;}

	m_DataModel.strAChangeBoxExID = boxA.strID;			// 操作之前的数量和ID
	m_DataModel.strBChangeBoxExID = boxA.strID;

	m_DataModel.nChangeABoxExCount.Format(_T("%d 枚"),boxA.nCount);
	m_DataModel.nChangeBBoxExCount.Format(_T("%d 枚"),boxB.nCount);

	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	if(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		m_DataModel.IsHasException = true;
	}
	else
		m_DataModel.IsHasException = false;

	m_DataModel.IsPrintFailed = false;
}

/////////////////////////////////////////////////////////////////////////
/*
@brief      上传电文

@param      None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxSvc::Sendcables(CHANGE_BOX_INDEX boxIndex)
{
	BYTE line = 0;
	BYTE Type = 0;
	WORD wNum = 0;
	CString coindedicatedboxid;
	CASHBOX_CHANGE  coinbox;

	CASHBOX_CHANGE	cash_change;
	cash_change.handleType = UNINSTALL;
	cash_change.State	 = VALID_DETACH;
	vector<CASH_TYPE_COUNT>  vecCash;
	switch(boxIndex)
	{
	case CHANGE_BOX_A:
		{
			CHANGE_BOX_INFO &box = m_DataModel.boxAinfo;
			line = mEx_change_A.coinBoxID.bLineCode;
			Type = mEx_change_A.coinBoxID.bType;
			wNum = mEx_change_A.coinBoxID.bNum;
			cash_change.SetBoxID(line,Type,mEx_change_A.coinBoxID.bNumInQu,wNum);
			cash_change.position = MAGAZINEB;	// A
			cash_change.btCount = 1; // 1 or '1'
			CASH_TYPE_COUNT coin;
			coin.cash_type = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());	// 面额
			coin.cash_count= mEx_change_A.ulCount; // 数量
			vecCash.push_back(coin);
			cash_change.vecCash = vecCash;
		}break;
	case CHANGE_BOX_B:
		{
			CHANGE_BOX_INFO &box = m_DataModel.boxBinfo;
			line = mEx_change_B.coinBoxID.bLineCode;
			Type = mEx_change_B.coinBoxID.bType;
			wNum = mEx_change_B.coinBoxID.bNum;
			cash_change.SetBoxID(line,Type,mEx_change_B.coinBoxID.bNumInQu,wNum);
			cash_change.position = MAGAZINEA;	// B
			cash_change.btCount = 1; // 1 or '1'
			CASH_TYPE_COUNT coin;
			coin.cash_type = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());	// 面额
			coin.cash_count= mEx_change_B.ulCount; // 数量
			vecCash.push_back(coin);
			cash_change.vecCash = vecCash;
		}break;
	default:
		break;
	}
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(cash_change);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      将箱子安装的数量插入到数据库中

@param      int nBoxNumber（箱子序列号）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRemoveCoinChangeBoxSvc::InsertCoinBoxToDatabase(int nBoxNumber){
	try{
		// 箱子卸载记录
		Data::CoinChangeBoxOperationRecord coinChangeBoxOperation;
		coinChangeBoxOperation.dtOperationTime = ComGetCurTime();
		coinChangeBoxOperation.iOperationType  = UNINSTALL_CHANGE_BOX;
		coinChangeBoxOperation.txtOperatorID	  = theAPP_SESSION.GetUserInfo().sUserID;
		if(nBoxNumber == CHANGE_BOX_A){
			coinChangeBoxOperation.txtCoinChangeBoxID = mEx_change_A.coinBoxID.ToString();//theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
			coinChangeBoxOperation.iCoinTypeCode	  = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
			coinChangeBoxOperation.iQuantity		  = mEx_change_A.ulCount;//theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
		}
		else if(nBoxNumber == CHANGE_BOX_B){
			coinChangeBoxOperation.txtCoinChangeBoxID = mEx_change_B.coinBoxID.ToString();//theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
			coinChangeBoxOperation.iCoinTypeCode	  = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
			coinChangeBoxOperation.iQuantity		  = mEx_change_B.ulCount;//theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
		}

		DATA_HELPER->DbInsertCoinChangeBoxOperationRecord(coinChangeBoxOperation);
	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
	}
}