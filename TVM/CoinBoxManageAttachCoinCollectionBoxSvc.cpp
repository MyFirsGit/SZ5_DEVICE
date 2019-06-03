#include "stdafx.h"
#include "CoinBoxManageAttachCoinCollectionBoxDlg.h"
#include "CoinBoxManageAttachCoinCollectionBoxSvc.h"
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


BEGIN_MESSAGE_MAP(CCoinBoxManageAttachCoinCollectionBoxSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageAttachCoinCollectionBoxSvc::CCoinBoxManageAttachCoinCollectionBoxSvc()
	:CTVMForeService(ATTACH_COLLECTION_COIN_BOX_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20115_COMMUNICATION_SET_END_DLG, new CCoinBoxManageAttachCoinCollectionBoxDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ATTACH_COLLECTION_BOX,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ATTACH_COLLECTION_BOX,IDD_20115_COMMUNICATION_SET_END_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_ATTACH_COLLECTION_BOX);

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageAttachCoinCollectionBoxSvc::~CCoinBoxManageAttachCoinCollectionBoxSvc()
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
void CCoinBoxManageAttachCoinCollectionBoxSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	
	m_DataModel.IsHasException = false;
	m_DataModel.IsPrintFalied = false;

	InitializeData();
//
//	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_ATTACH_COLLECTION_BOX);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_ATTACH_COLLECTION_BOX);
	if(!m_DataModel.IsHasException)
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化数据模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAttachCoinCollectionBoxSvc::InitializeDataModel()
{
	m_DataModel.CoinCollectionBoxAStatus = _T("");
	m_DataModel.CoinCollectionBoxBStatus = _T("");
	m_DataModel.CoinCollectionBoxAID = _T("");
	m_DataModel.CoinCollectionBoxBID = _T("");
	m_DataModel.CoinCollctionBoxAfterAttachAID = _T("");
	m_DataModel.CoinCollctionBoxAfterAttachBID = _T("");
	m_DataModel.CoinCollectionAfterBoxAStatus = _T("");
	m_DataModel.CoinCollectionAfterBoxBStatus = _T("");
	m_DataModel.AttachCoinCollectionBoxFlowTime = 1;
	m_DataModel.IsHasException = FALSE;
	m_DataModel.bIsInstallCollectionA = false;
	m_DataModel.bIsInstallCollectionB = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageAttachCoinCollectionBoxSvc::Model& CCoinBoxManageAttachCoinCollectionBoxSvc::GetDataModel()
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
void CCoinBoxManageAttachCoinCollectionBoxSvc::NotifyDataModelChanged()
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
CString CCoinBoxManageAttachCoinCollectionBoxSvc::GetHopperCoinType(int hopetype)
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
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinBoxManageAttachCoinCollectionBoxSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}
	int index = theAPP_SESSION.GetInputToInt();			// 获取向导栏输入内容
	
	CString strTips = _T("");
	switch ( m_phase )
	{
	case PHASE_SELECT_BOX:
		{
			// 输入内容非法
			if(index != MENU_CODE_1/* || index > MENU_CODE_3*/)
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
				return TRUE;
			}

			// 输入前，将栈清空
			while(!m_stkOpBox.empty()) 
				m_stkOpBox.pop();

			if(IsBoxCouldbeInstalled((COLLECTION_BOX_INDEX)index)){
				//// 如果选择安装单个箱子
				//if (index != 3){
				//	char num = (index == 1) ? 'A' : 'B';
				//	strTips.Format(_T("请输入回收箱%c编号：%s"),num,m_DataModel.strPreID);
				//	theAPP_SESSION.SetPrefixToGuide(strTips);
				//}
				//else{// 如果选择安装所有箱子
				//	char num = (m_stkOpBox.top() ==coin_collection_boxA) ? 'A' : 'B';
				//	strTips.Format(_T("请输入回收箱%c编号：%s"),num,m_DataModel.strPreID);
				//	theAPP_SESSION.SetPrefixToGuide(strTips);
				//}
				//m_phase = PHASE_SET_BOXID;
				//theAPP_SESSION.SetMaxInputLength(4);
				m_phase = PHASE_SELECT_BOX;
				m_DataModel.stateA = BOX_STATE::ST_PRE_INSTALL;

				theAPP_SESSION.SetMaxInputLength(1);
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));

			}
		}break;
	case PHASE_SET_BOXID:
		{
			int inputNum = theAPP_SESSION.GetInputToInt();

			if(!m_stkOpBox.empty()){
				if(inputNum == 0){
					theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ERROR_INPUT));
					return 0;
				}
				COLLECTION_BOX_INDEX boxType = m_stkOpBox.top();

				if(CheckInputBoxID(boxType,inputNum)){
					// 提示ID重复
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_TH_TEST_TEXT_ID_REPEAT));
					return 0;
				}
				m_stkOpBox.pop();
				if (!m_stkOpBox.empty()){
					m_phase = PHASE_SET_BOXID;
					theAPP_SESSION.SetMaxInputLength(4);
					char num = (m_stkOpBox.top() ==coin_collection_boxA) ? 'A' : 'B';
					strTips.Format(_T("请输入回收箱%c编号：%s"),num,m_DataModel.strPreID);
					theAPP_SESSION.SetPrefixToGuide(strTips);
				}
				else{
					m_phase = PHASE_SELECT_BOX;
					theAPP_SESSION.SetMaxInputLength(1);
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
				}
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_T("所选硬币回收箱已安装"));
			}
		}break;
	default:
		break;
	}
	NotifyDataModelChanged();
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
LRESULT CCoinBoxManageAttachCoinCollectionBoxSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	InitializeData();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCoinBoxManageAttachCoinCollectionBoxSvc::OnKeyboardF2(WPARAM,LPARAM)
{
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
		return FALSE;
	}
	long errCode = 0;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionboxInfo;
	bool bShowTips = (m_DataModel.stateA == ST_PRE_INSTALL) || (m_DataModel.stateB == ST_PRE_INSTALL);

	if(!bShowTips){
		return 0;
	}
	try
	{
		// 安装硬币回收箱A
		CString strTips(_T(""));// = _opl(GUIDE_ENTER_F2_F3_ESC);
		if(m_DataModel.stateA == ST_PRE_INSTALL )
		{	
			USES_CONVERSION;
			//BYTE bHiByte;
			//BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
			//BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
			//BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode()*2-1;
			//int2BCD(theMAINTENANCE_INFO.GetStationCode(), (char *)&bHiByte, sizeof(bHiByte));
			//coincollectionboxInfo.coinBoxID.bLineCode = theMAINTENANCE_INFO.GetStationCode();
			//int2BCD(bLineCode, (char*)&coincollectionboxInfo.coinBoxID.bLineCode, sizeof(coincollectionboxInfo.coinBoxID.bLineCode));
			//coincollectionboxInfo.coinBoxID.bType = 0x05;
			//int2BCD(bStationCode, (char*)&coincollectionboxInfo.coinBoxID.bNumInQu, sizeof(coincollectionboxInfo.coinBoxID.bNumInQu));
			//int2BCD(bMachineCode, (char*)&coincollectionboxInfo.coinBoxID.bNum, sizeof(coincollectionboxInfo.coinBoxID.bNum));

			CString boxID = COIN_HELPER->CH_ReadRfidData(3);
			if (boxID==_T("")){
				theAPP_SESSION.ShowOfficeGuide(_T("读取回收箱RFID失败"));
				//return 0;
				boxID = _T("12345678");
			}
			TCHAR* pChar = NULL;
			coincollectionboxInfo.coinBoxID.bLineCode = _tcstoul(boxID.Left(2), &pChar, 16);
			coincollectionboxInfo.coinBoxID.bType = _tcstoul(boxID.Mid(2,2), &pChar, 16);
			coincollectionboxInfo.coinBoxID.bNumInQu = _tcstoul(boxID.Mid(4,2), &pChar, 16);
			coincollectionboxInfo.coinBoxID.bNum = _tcstoul(boxID.Mid(6,2), &pChar, 16);	
			coincollectionboxInfo.ulCount_1yuan = coincollectionboxInfo.ulCount_5jiao = 0;
			coincollectionboxInfo.operateTime = ComGetCurTime();
			coincollectionboxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
			// 硬币回收箱AID写入软件计数，置硬币回收箱A硬币数量为0
			if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(coincollectionboxInfo))	{
				strTips += _T("硬币回收箱安装失败");/*_opl(TXT_COINMGR_COLLECTION_BOXA_INSTALL_FAILED);*/
				//theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱A的计数信息写入失败"));
			}
			else{
				strTips += _T("硬币回收箱安装成功");/*_opl(TXT_COINMGR_COLLECTION_BOXA_INSTALL_SUCCESS);*/
				m_DataModel.stateA = ST_INSTALLED; // 更新回收箱的状态
			}
			theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			theTVM_STATUS_MGR.SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			m_DataModel.CoinCollectionBoxAID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
			LOG("硬币回收箱安装成功！");
			// 上传电文
			Sendcables(coin_collection_boxA);
		}
		// 安装硬币回收箱B
		if(m_DataModel.stateB == ST_PRE_INSTALL )
		{
			USES_CONVERSION;
			BYTE bHiByte;
			BYTE bLineCode = theMAINTENANCE_INFO.GetLineCode();
			BYTE bStationCode = theMAINTENANCE_INFO.GetStationCode();
			BYTE bMachineCode = theMAINTENANCE_INFO.GetMachineCode()*2-1;
			int2BCD(theMAINTENANCE_INFO.GetStationCode(), (char *)&bHiByte, sizeof(bHiByte));
			coincollectionboxInfo.coinBoxID.bLineCode = theMAINTENANCE_INFO.GetStationCode();
			int2BCD(bLineCode, (char*)&coincollectionboxInfo.coinBoxID.bLineCode, sizeof(coincollectionboxInfo.coinBoxID.bLineCode));
			coincollectionboxInfo.coinBoxID.bType = 0x05;
			int2BCD(bStationCode, (char*)&coincollectionboxInfo.coinBoxID.bNumInQu, sizeof(coincollectionboxInfo.coinBoxID.bNumInQu));
			int2BCD(bMachineCode, (char*)&coincollectionboxInfo.coinBoxID.bNum, sizeof(coincollectionboxInfo.coinBoxID.bNum));
			coincollectionboxInfo.ulCount_1yuan = coincollectionboxInfo.ulCount_5jiao = 0;
			coincollectionboxInfo.operateTime = ComGetCurTime();
			coincollectionboxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
			// 硬币回收箱BID写入软件计数，置硬币回收箱B硬币数量为0
			if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(coincollectionboxInfo))	{
				strTips += _opl(TXT_COINMGR_COLLECTION_BOXB_INSTALL_FAILED);
				//theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱B的计数信息写入失败"));
			}
			else{
				strTips += _opl(TXT_COINMGR_COLLECTION_BOXB_INSTALL_SUCCESS);
				m_DataModel.stateB = ST_INSTALLED; // 更新回收箱的状态
			}
			theTVM_STATUS_MGR.SetCoinCollectionBox2Status(STATUS_MAGAZINE::STATUS_MAGAZINE_SET);
			theTVM_STATUS_MGR.SetCoinCollectionBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_NORMAL);
			// 上传电文
			Sendcables(coin_collection_boxB);														// 硬币回收箱B安装
			m_DataModel.AttachCoinCollectionBoxFlowTime = SecondFrame;
		}
		if (bShowTips){
			strTips += _opl(GUIDE_F3_ESC);

			theAPP_SESSION.ShowOfficeGuide(strTips);
			PrintCoinBoxExchangeReceipt();
		}
		
		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_INSTALLATION,true);
	}
	// 异常处理
	catch (CSysException& e) {
		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_INSTALLATION,false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	catch (...) {
		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_INSTALLATION,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}

	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	if(m_DataModel.IsPrintFalied){
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
	}

	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上传电文

@param      int boxnumber

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAttachCoinCollectionBoxSvc::Sendcables(int boxnumber)
{
	CASHBOX_CHANGE  coinbox;
	vector<CASH_TYPE_COUNT> vecCash;

	// 硬币回收箱A、B--操作类型
	coinbox.handleType = INSTALL;
	CASH_TYPE_COUNT cash;
	// 硬币回收箱A--ID、位置ID
	if(coin_collection_boxA == boxnumber)
	{
		// 硬币回收箱A--ID
		coinbox.SetBoxID(theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.bLineCode, theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.bType,theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.bNumInQu,theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.bNum);
		// 硬币回收箱位置ID
		coinbox.position = MAGAZINEG;
		cash.cash_type = BankNoteAndCoinType_t::Coin1Yuan;
		cash.cash_count= 0;
		vecCash.push_back(cash);

		/*cash.cash_type = BankNoteAndCoinType_t::CoinHalfYuan;
		cash.cash_count= 0;
		vecCash.push_back(cash);*/
	}
	// 硬币回收箱B--ID、位置ID
	else
	{
		// 硬币补充箱B--ID
		coinbox.SetBoxID(theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.bLineCode, theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.bType,theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.bNumInQu,theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.bNum);
		// 硬币补充箱位置ID
		coinbox.position = MAGAZINEG;
		cash.cash_type = BankNoteAndCoinType_t::Coin1Yuan;
		cash.cash_count= 0;
		vecCash.push_back(cash);

		/*cash.cash_type = BankNoteAndCoinType_t::CoinHalfYuan;
		cash.cash_count= 0;
		vecCash.push_back(cash);*/
	}
	// 硬币回收箱状态
	coinbox.State = VALID_ATTACH;	
	// 硬币回收箱数量为0
	//coinbox.CoinBox_Initial();
	coinbox.vecCash = vecCash;
	coinbox.btCount = 1;
	// 硬币补充箱币种总数
	//coinbox.btCount = coinbox.GetSumCurrency();
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(coinbox);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     硬币回收箱安装凭证打印

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAttachCoinCollectionBoxSvc::PrintCoinBoxExchangeReceipt()
{
	try
	{
		//CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
		//CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

		//CTCoinCountInfo::COIN_BOX_INFO hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
		//CTCoinCountInfo::COIN_BOX_INFO hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();


		// 操作员ID
		CString strOperatorID_before = _T("");
		CString strOperatorID_after = _T("");
		strOperatorID_before.Format(_T("%06d"), mEx_collect_A.ulOperatorID);
		strOperatorID_after.Format(_T("%06d"), collect_A.ulOperatorID);
		// 1元
		CString strFace1Count_before = _T("");
		CString strFace1Count_after = _T("");
		strFace1Count_before.Format(_T("%d"),  mEx_collect_A.ulCount_1yuan + mEx_collect_A.ulCount_5jiao);
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
			line.Replace(_T("{操作前钱箱号}"), mEx_collect_A.coinBoxID.ToString());
			line.Replace(_T("{操作后钱箱号}"), collect_A.coinBoxID.ToString());
			line.Replace(_T("{操作前操作员}"), strOperatorID_before);
			line.Replace(_T("{操作后操作员}"), strOperatorID_after);
			line.Replace(_T("{设备号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());			
			line.Replace(_T("{操作前时间}"), mEx_collect_A.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作后时间}"), collect_A.operateTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")).Mid(2));
			line.Replace(_T("{操作前1元}"), strFace1Count_before);
			line.Replace(_T("{操作后1元}"), strFace1Count_after);
			line.Replace(_T("{日期}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));

			CStringArray receiptline;
			ComPrintLineFormatForTemplate(line,receiptline);
			printtext.Append(receiptline);
			Index++;
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_INSTALL);
		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsPrintFalied = TRUE;
		//throw;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.IsPrintFalied = TRUE;
	}

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断箱子是否可安装

@param		COLLECTION_BOX_INDEX& index 箱子位置结构体实例

@retval     true : 可以 ； false : 不可以

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
 bool CCoinBoxManageAttachCoinCollectionBoxSvc::IsBoxCouldbeInstalled(COLLECTION_BOX_INDEX index)
{
	IO_RSP_STATUS_INFO rsp;
	// 判断回收箱是否在位?
	try{
		IO_HELPER->GetIOStatus(rsp);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_IO_SENSOR_FAILED_INSTALL_STOP));
		return false;
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_IO_SENSOR_FAILED_INSTALL_STOP));
		return false;
	}

	bool Could = false;
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO boxAinfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO boxBinfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	if (index == coin_collection_boxA){
		if(rsp.isCoinboxReady/*isCoinbox1Attach*/){// 打开盖板即为到位
			if(boxAinfo.coinBoxID.ToString() == INVALID_ID){
				m_stkOpBox.push(index);
				return true;
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ATTACH_ALREADY));
				return false;
			}
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOINSTALL));
			return false;
		}
	}
	//else if (index == coin_collection_boxB){
	//	if(rsp.isCoinbox2LidOpened/*isCoinbox2Attach*/){// 打开盖板即为到位
	//		if(boxBinfo.coinBoxID.ToString() == INVALID_ID){
	//			m_stkOpBox.push(index);
	//			return true;
	//		}
	//		else{
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ATTACH_ALREADY));
	//			return false;
	//		}
	//	}
	//	else{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOINSTALL));
	//		return false;
	//	}
	//}
	//else if ( index == coin_collection_all){
	//	if(rsp.isCoinbox1LidOpened || rsp.isCoinbox2LidOpened){
	//		if(boxBinfo.coinBoxID.ToString() == INVALID_ID && rsp.isCoinbox2LidOpened){
	//			m_stkOpBox.push(coin_collection_boxB);
	//		}
	//		if(boxAinfo.coinBoxID.ToString() == INVALID_ID && rsp.isCoinbox1LidOpened ){
	//			m_stkOpBox.push(coin_collection_boxA);
	//		}
	//		if(!m_stkOpBox.empty()){
	//			return true;
	//		}
	//		else{
	//			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ATTACH_ALREADY));
	//			return false;
	//		}
	//	}
	//	else{
	//		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_COLLECTION_BOX_NOINSTALL));
	//		return false;
	//	}
	//}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断箱子是否可安装

@param		COLLECTION_BOX_INDEX& index 箱子位置结构体实例

@retval      

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAttachCoinCollectionBoxSvc::SetIdAndStateForBox(COLLECTION_BOX_INDEX index)
{
	 if (index == coin_collection_boxA){
		 m_DataModel.CoinCollectionBoxAID = theAPP_SESSION.GetInputToString();
		 m_DataModel.stateA = ST_PRE_INSTALL;
	 }
	 if (index == coin_collection_boxB){
		 m_DataModel.CoinCollectionBoxBID = theAPP_SESSION.GetInputToString();
		 m_DataModel.stateB = ST_PRE_INSTALL;
	 }
}

 //////////////////////////////////////////////////////////////////////////
/**
@brief      数据初始化

@param		 None

@retval     None

@exception  None
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageAttachCoinCollectionBoxSvc::InitializeData()
{
	//// 同步计数，如果需要的话。
	//if(!COIN_HELPER->SyncCoinCountInfoToFile())
	//	m_DataModel.IsHasException = false;

	 m_phase = PHASE_SELECT_BOX;
	 Clear_Stk();
	 // 从master文件中读取ID 状态
	 CTCoinCountInfo::COIN_COLLECTION_BOX_INFO boxAinfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	 CTCoinCountInfo::COIN_COLLECTION_BOX_INFO boxBinfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	 m_DataModel.CoinCollectionBoxAID = boxAinfo.coinBoxID.ToString();
	 m_DataModel.CoinCollectionBoxBID = boxBinfo.coinBoxID.ToString();

	 m_DataModel.strColBoxAExID = m_DataModel.CoinCollectionBoxAID;
	 m_DataModel.strColBoxBExID = m_DataModel.CoinCollectionBoxBID;

	 m_DataModel.nColBoxAExCount = boxAinfo.GetCurCount();
	 m_DataModel.nColBoxBExCount = boxBinfo.GetCurCount();

	 if ( m_DataModel.CoinCollectionBoxAID == _T("FFFFFFFF")){
		 m_DataModel.stateA = ST_REMOVED;
	 }else{
		 m_DataModel.stateA = ST_INSTALLED;
	 }
	 if (m_DataModel.CoinCollectionBoxBID == _T("FFFFFFFF")){
		 m_DataModel.stateB = ST_REMOVED;
	 }else{
		 m_DataModel.stateB = ST_INSTALLED;
	 }

	 mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	 mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	 mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	 mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	 mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	 mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	 if(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		 m_DataModel.IsHasException = true;
	 }
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      检查箱子ID是否重复

@param      无

@retval     bool true:重复 false:不重复

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CCoinBoxManageAttachCoinCollectionBoxSvc::CheckInputBoxID(COLLECTION_BOX_INDEX boxType,int nBoxId){
	CString strBoxId(_T(""));
	strBoxId.Format(_T("%02d51%04d"),theMAINTENANCE_INFO.GetLineCode(),nBoxId);

	 if(boxType == COLLECTION_BOX_INDEX::coin_collection_boxA){
		 if(!strBoxId.IsEmpty()){
			 if(strBoxId.Compare(m_DataModel.CoinCollectionBoxBID) == 0){
				 return true;
			 }
			 else{
				m_DataModel.CoinCollectionBoxAID = strBoxId;
				m_DataModel.stateA = BOX_STATE::ST_PRE_INSTALL;
			 }
		 }
		 return false;
	 }
	 else if(boxType == COLLECTION_BOX_INDEX::coin_collection_boxB){
		 if(!strBoxId.IsEmpty()){
			 if(strBoxId.Compare(m_DataModel.CoinCollectionBoxAID) == 0){
				 return true;
			 }
			 else{
				m_DataModel.CoinCollectionBoxBID = strBoxId;
				m_DataModel.stateB = BOX_STATE::ST_PRE_INSTALL;
			 }
		 }
		 return false;
	 }
	 else 
		 return false;
}