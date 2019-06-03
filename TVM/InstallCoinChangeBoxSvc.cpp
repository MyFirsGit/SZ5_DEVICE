#include "stdafx.h"
#include "InstallCoinChangeBoxSvc.h"
#include "CTcoinCountInfo.h"
#include "TemplateHelper.h"
#include "InstallCoinChangeBoxDlg.h"

#define INTERNAL_ERROR(errCode) \
{if (COIN_HELPER->IsExceptionCode(errCode)) {\
	throw CCHException(errCode, _T(__FILE__), __LINE__);\
}else\
{throw CCHHardwareException(errCode,_T(__FILE__),__LINE__);}}


BEGIN_MESSAGE_MAP(CInstallCoinChangeBoxSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CInstallCoinChangeBoxSvc::CInstallCoinChangeBoxSvc()
	:CTVMForeService(INSTALL_COIN_CHANGE_BOX_SVC/*ATTACH_COLLECTION_COIN_BOX_SVC*/)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20115_COMMUNICATION_SET_END_DLG, new CInstallCoinChangeBoxDlg(this));

	// 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ATTACH_COLLECTION_BOX,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ATTACH_COLLECTION_BOX,IDD_20115_COMMUNICATION_SET_END_DLG);

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_ATTACH_COLLECTION_BOX);
	m_selectedBox[0] = UNSELECTED;
	m_selectedBox[1] = UNSELECTED;

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CInstallCoinChangeBoxSvc::~CInstallCoinChangeBoxSvc()
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
void CInstallCoinChangeBoxSvc::OnStart()
{
	__super::OnStart();

	InitStack();												// 初始化待安装箱子的栈容器

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键
	m_phase			= PHASE_CHOOSE_BOX;							// 首先进入的是选择箱子阶段
	IsBoxAInstalled = FALSE;
	IsBoxBInstalled = FALSE;
	m_selected		= UNSELECTED;								// 未选择要操作的箱子

	m_DataModel.AttachCoinCollectionBoxFlowTime = FirstFrame;

	InitializeDataModel();
	try{
		//COIN_HELPER->ReadStatus(m_common_rsp);
	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsHasException = TRUE;
	}
	DoAttachCollectionHandleData();

	// 设置初始画面组
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
void CInstallCoinChangeBoxSvc::InitializeDataModel()
{
	//// 同步计数，如果需要的话。
	//if(!COIN_HELPER->SyncCoinCountInfoToFile())
	//	m_DataModel.IsHasException = false;

	// 操作之前各个箱子的数据保存
	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	m_DataModel.CoinCollectionBoxAStatus = _T("");
	m_DataModel.CoinCollectionBoxBStatus = _T("");
	m_DataModel.CoinChangeBoxAID = _T("");
	m_DataModel.CoinChangeBoxBID = _T("");
	m_DataModel.CoinCollctionBoxAfterAttachAID = _T("");
	m_DataModel.CoinCollctionBoxAfterAttachBID = _T("");
	m_DataModel.CoinCollectionAfterBoxAStatus = _T("");
	m_DataModel.CoinCollectionAfterBoxBStatus = _T("");
	m_DataModel.AttachCoinCollectionBoxFlowTime = 1;
	m_DataModel.IsHasException = FALSE;
	m_DataModel.IsPrintFalied = false;
	m_DataModel.bIsInstallCollectionA = false;
	m_DataModel.bIsInstallCollectionB = false;

	if(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		m_DataModel.IsHasException = true;
	}
	else
		m_DataModel.IsHasException = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CInstallCoinChangeBoxSvc::Model& CInstallCoinChangeBoxSvc::GetDataModel()
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
void CInstallCoinChangeBoxSvc::NotifyDataModelChanged()
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
CString CInstallCoinChangeBoxSvc::GetHopperCoinType(int hopetype)
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
@brief     处理安装硬币回收箱数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxSvc::DoAttachCollectionHandleData()
{
	try
	{
		long errCode = 0;
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_box_B_info;									// 硬币回收箱A计数信息
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO change_box_A_info;									// 硬币回收箱B计数信息

		change_box_A_info = theCOIN_COUNT.GetChangeCoinboxAInfo();									// 获取硬币回收箱A计数信息
		change_box_B_info = theCOIN_COUNT.GetChangeCoinboxBInfo();								// 获取硬币回收箱B计数信息

		if(_T("FFFFFFFF") == change_box_A_info.coinBoxID.ToString())									// 硬币回收箱A未安装
		{
			m_DataModel.CoinChangeBoxAID = _T("FFFFFFFF");
			m_DataModel.CoinCollectionBoxAStatus = m_DataModel.CoinBoxABeforeInstall = _opl(TXT_COINMGR_BOX_DETACHED);		// 硬币回收箱A已卸载
			m_DataModel.strCoinACount = _T("0");
			IsBoxAInstalled = FALSE;
			m_DataModel.stateA = STATE_DETATCH;
		}
		else
		{
			m_DataModel.CoinCollectionBoxAStatus = m_DataModel.CoinBoxABeforeInstall = _opl(TXT_COINMGR_BOX_ATTACHED);				// 硬币回收箱A已安装
			m_DataModel.CoinChangeBoxAID = change_box_A_info.coinBoxID.ToString();
			m_DataModel.strCoinACount.Format(_T("%d"),change_box_A_info.ulCount);
			IsBoxAInstalled = TRUE;
			m_DataModel.stateA = STATE_INSTALLED;
		
		}
		if(_T("FFFFFFFF") == change_box_B_info.coinBoxID.ToString())									// 硬币回收箱B未安装
		{
			m_DataModel.CoinCollectionBoxBStatus = m_DataModel.CoinBoxBBeforeInstall = _opl(TXT_COINMGR_BOX_DETACHED);						// 硬币回收箱B已卸载
			m_DataModel.CoinChangeBoxBID = _T("FFFFFFFF");
			IsBoxBInstalled = FALSE;
			m_DataModel.strCoinBCount = _T("0");
			m_DataModel.stateB = STATE_DETATCH;
		}
		else
		{
			m_DataModel.CoinCollectionBoxBStatus = m_DataModel.CoinBoxBBeforeInstall = _opl(TXT_COINMGR_BOX_ATTACHED);				// 硬币回收箱B已安装
			m_DataModel.CoinChangeBoxBID = change_box_B_info.coinBoxID.ToString();
			m_DataModel.strCoinBCount.Format(_T("%d"),change_box_B_info.ulCount);
			IsBoxBInstalled = TRUE;
			m_DataModel.stateB = STATE_INSTALLED;
		}
		// 操作前的ID  
		m_DataModel.strAChangeBoxExID = m_DataModel.CoinChangeBoxAID;
		m_DataModel.strBChangeBoxExID = m_DataModel.CoinChangeBoxBID;
		// 操作前的数量
		m_DataModel.nChangeABoxExCount = m_DataModel.strCoinACount;
		m_DataModel.nChangeBBoxExCount = m_DataModel.strCoinBCount;

		m_DataModel.AttachCoinCollectionBoxFlowTime = FirstFrame;
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
LRESULT CInstallCoinChangeBoxSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}
	int index = theAPP_SESSION.GetInputToInt();			// 获取向导栏输入内容
	
	switch( m_phase )
	{
	case PHASE_CHOOSE_BOX:
		{
			if(index < MENU_CODE_1 || index > MENU_CODE_3){	// 检查输入有效性
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
				return TRUE;
			}
			else{
				m_selected = (BOX_SELECT)theAPP_SESSION.GetInputToInt();	// 要设置的箱子
				if (isBoxCouldBeInstalled(m_selected)){
					if(m_selected == CHANGE_ALL_BOX){ // 如果选择安装所有硬币箱
						if (isBoxCouldBeInstalled(CHANGE_BOX_B)){	//由于选用的是栈容器来存储，判断B箱
							m_opBox.push(CHANGE_BOX_B);
						}
						if (isBoxCouldBeInstalled(CHANGE_BOX_A)){	//由于选用的是栈容器来存储，判断B箱
							m_opBox.push(CHANGE_BOX_A);
						}
					}
					else{// 选择安装硬币箱A或B
						m_opBox.push(m_selected);
					}
					//m_phase = PHASE_SET_ID;								// 下一阶段将是输入ID阶段
					m_phase = PHASE_SET_COUNT;

					theAPP_SESSION.SetMaxInputLength(4);				// ID长度为8,只需要输入4位
					// 设置输入长度
					CString strTips = _T("");
					//strTips.Format(_T("请输入中硬币箱%c的编号:%s"),GetBoxLetter(m_opBox.top()),m_DataModel.strPreID);
					strTips.Format(_T("请输入中硬币箱%c中硬币的数量:"),GetBoxLetter(m_opBox.top()),m_DataModel.strPreID);
					theAPP_SESSION.SetPrefixToGuide(strTips);
				}else{
					theAPP_SESSION.ShowOfficeGuide(_T("所选硬币箱不可安装"));
					m_phase = PHASE_CHOOSE_BOX;
				}
			}
		}
		break;
	case PHASE_SET_ID:														// 设置ID阶段
		{
			int inputNum = theAPP_SESSION.GetInputToInt();
			if(inputNum == 0){
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_ERROR_INPUT));
				return 0;
			}
			if(!m_opBox.empty()){
				BOX_SELECT boxType = m_opBox.top();

				// 检查并设定箱子编号
				if(CheckInputBoxID(boxType,inputNum)){
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_TH_TEST_TEXT_ID_REPEAT));
					break;
				}
				else{
					m_phase = PHASE_SET_COUNT;
					theAPP_SESSION.SetMaxInputLength(3);
					CString strTips = _T("");
					strTips.Format(_T("请输入中硬币箱%c中硬币的数量:"),GetBoxLetter(boxType));
					theAPP_SESSION.SetPrefixToGuide(strTips);
				}
			}
		}break;
	case PHASE_SET_COUNT:
		{
			if(!m_opBox.empty()){
				int nInputCount = theAPP_SESSION.GetInputToInt();

				switch(m_opBox.top()){
				case CHANGE_BOX_A:
					{
						if(nInputCount < 0 || nInputCount > theTVM_INFO.GetCHChangeBoxAMax()){
							CString strTips = _T("");
							strTips.Format(_T("硬币找零箱A输入数量不合法（最多为%d枚），请重新输入:"),theTVM_INFO.GetCHChangeBoxAMax());
							theAPP_SESSION.SetPrefixToGuide(strTips);
							m_phase = PHASE_SET_COUNT;
							return 0;
						}
					}
					break;
				case CHANGE_BOX_B:
					{
						if(nInputCount < 0 || nInputCount > theTVM_INFO.GetCHChangeBoxBMax()){
							CString strTips = _T("");
							strTips.Format(_T("硬币找零箱B输入数量不合法（最多为%d枚），请重新输入:"),theTVM_INFO.GetCHChangeBoxBMax());
							theAPP_SESSION.SetPrefixToGuide(strTips);
							m_phase = PHASE_SET_COUNT;
							return 0;
						}
					}
					break;
				default:
					break;
				}
				SetCountsAndState(m_opBox.top(), theAPP_SESSION.GetInputToString());
			
				m_opBox.pop();

				if(!m_opBox.empty()){ 
					//m_phase = PHASE_SET_ID;
					m_phase = PHASE_SET_COUNT;

					theAPP_SESSION.SetMaxInputLength(4);
					CString strTips = _T("");
					//strTips.Format(_T("请输入中硬币箱%c的编号:%s"),GetBoxLetter(m_opBox.top()),m_DataModel.strPreID);
					strTips.Format(_T("请输入中硬币箱%c中硬币的数量:"),GetBoxLetter(m_opBox.top()),m_DataModel.strPreID);
					theAPP_SESSION.SetPrefixToGuide(strTips);
				}
				else{
					m_phase = PHASE_CHOOSE_BOX;
					theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
					theAPP_SESSION.SetMaxInputLength(1);
				}
			}
		}break;
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
LRESULT CInstallCoinChangeBoxSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	InitializeDataModel();
	DoAttachCollectionHandleData();
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
LRESULT CInstallCoinChangeBoxSvc::OnKeyboardF2(WPARAM,LPARAM){

	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COIN_TEST_CLEAR_FALIED));
		return FALSE;
	}

	if(1 != m_DataModel.AttachCoinCollectionBoxFlowTime){
		return FALSE;
	}
	long errCode = 0;

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionboxInfo;
	bool isOperated = (m_DataModel.stateA == STATE_NEED_TO_INSTALL) || (m_DataModel.stateB == STATE_NEED_TO_INSTALL);

	if(!isOperated){
		return FALSE;
	}

	BOOL isSaved = FALSE;
	CString strTips = _T("");
	try
	{
		if (m_DataModel.stateA == STATE_NEED_TO_INSTALL)	// 如果找零箱A的状态为待安装
		{
			//进行找零箱A的安装操作
			if(!SetCoinChangeBoxCountInfo(CHANGE_BOX_A)){
				strTips += _T("硬币箱A安装失败;");
			}
			else{
				strTips += _T("硬币找零箱A安装成功;");
				Sendcables(CHANGE_BOX_A);
				InsertCoinBoxToDatabase(CHANGE_BOX_A);
				// 本机设置日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_INSTALL,true);
				// 如果安装成功
				m_DataModel.stateA = STATE_INSTALLED;
				isSaved = TRUE;
			}
		}
		if (m_DataModel.stateB == STATE_NEED_TO_INSTALL)
		{
			// 进行找零箱B的安装操作
			if(!SetCoinChangeBoxCountInfo(CHANGE_BOX_B)){
				strTips += _T("硬币箱B安装失败。");
			}
			else{
				strTips += _T("硬币找零箱B安装成功;");

				//theAPP_SESSION.ShowOfficeGuide(strTips);
				Sendcables(CHANGE_BOX_B);
				InsertCoinBoxToDatabase(CHANGE_BOX_B);
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_INSTALL,true);
				m_DataModel.stateB = STATE_INSTALLED;
				isSaved = TRUE;
			}
		}
		strTips += _opl(GUIDE_F3_ESC);
		theAPP_SESSION.ShowOfficeGuide(strTips);

		if(isSaved){
			PrintCoinBoxExchangeReceipt();
		}

		// 操作之前各个箱子的数据保存
		mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
		mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

		mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
		mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

		mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

		// 更新硬件计数
		try{
			COIN_HELPER->CH_SetCoinNum();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){

		}

		if(m_DataModel.IsPrintFalied){
			Sleep(1000);
			theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));
		}
	}
	// 异常处理
	catch (CSysException& e) {
		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_INSTALL,false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
	}
	catch (...) {
		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COIN_CHANGE_INSTALL,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
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
void CInstallCoinChangeBoxSvc::Sendcables(int boxnumber)
{
	CASHBOX_CHANGE  coinbox;
	vector<CASH_TYPE_COUNT> vecCash;

	// 硬币回收箱A、B--操作类型
	coinbox.handleType = INSTALL;
	// 硬币回收箱A--ID、位置ID
	if(CHANGE_BOX_A == boxnumber)
	{
		// 硬币回收箱A--ID
		CTCoinCountInfo::COIN_BOX_ID& box_id = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID;
		coinbox.SetBoxID(box_id.bLineCode, box_id.bType,box_id.bNumInQu,box_id.bNum);
		// 硬币回收箱位置ID
		coinbox.position = MAGAZINEB;
		CASH_TYPE_COUNT coin ;
		coin.cash_type = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
		coin.cash_count = _ttoi(m_DataModel.strCoinACount);
		vecCash.push_back(coin);
	}
	// 硬币回收箱B--ID、位置ID
	else if(CHANGE_BOX_B == boxnumber)
	{
		// 硬币补充箱B--ID
		CTCoinCountInfo::COIN_BOX_ID& box_id = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID;
		coinbox.SetBoxID(box_id.bLineCode, box_id.bType,box_id.bNumInQu,box_id.bNum);
		// 硬币补充箱位置ID
		coinbox.position = MAGAZINEA;	
		CASH_TYPE_COUNT coin ;
		coin.cash_type = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());;
		coin.cash_count = _ttoi(m_DataModel.strCoinBCount);
		vecCash.push_back(coin);
	}
	// 硬币回收箱状态
	coinbox.State = VALID_ATTACH;
	coinbox.vecCash = vecCash;
	// 硬币补充箱币种总数
	coinbox.btCount = coinbox.GetSumCurrency();
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
void CInstallCoinChangeBoxSvc::PrintCoinBoxExchangeReceipt()
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

		DWORD dwTotalAmountBefore = (mEx_change_A.ulCount + mEx_change_B.ulCount) * 100 /*+ mEx_hopper_A.ulCurCount * 50 */+ mEx_hopper_B.ulCurCount * 100 + mEx_collect_A.GetCurAmount() /*+ mEx_collect_B.GetCurAmount()*/;
		DWORD dwTotalAmountAfter  = (change_A.ulCount + change_B.ulCount) * 100 /*+ hopper_A.ulCurCount * 50*/ + hopper_B.ulCurCount * 100 + collect_A.GetCurAmount()/* + collect_B.GetCurAmount()*/;
		
		int Index = 0;
		CString strCount;

		int nHalfCount = TemplateContents.GetCount() / 2 - 2;

		while(Index<TemplateContents.GetCount())
		{
			CString& line = TemplateContents.ElementAt(Index);	
			line.Replace(_T("{安装}"),_T("安装"));																				// 操作类型

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
			line.Replace(_T("{前数量6}"),ComFormatCount(mEx_collect_A.ulCount_5jiao ));
			line.Replace(_T("{前数量7}"),ComFormatCount(mEx_collect_B.ulCount_1yuan));
			line.Replace(_T("{前数量8}"),ComFormatCount(mEx_collect_B.ulCount_5jiao ));
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
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_INSTALL);
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


void CInstallCoinChangeBoxSvc::SetCountsAndState(BOX_SELECT forBox, CString strCounts)
{
	if (forBox == CHANGE_BOX_A){
		m_DataModel.strCoinACount = strCounts;
		m_DataModel.CoinCollectionBoxAStatus = _opl(TXT_TOKENMGR_WAITTOATTACH);
		m_DataModel.stateA = STATE_NEED_TO_INSTALL;
	}
	else if (forBox == CHANGE_BOX_B){
		m_DataModel.strCoinBCount = strCounts;
		m_DataModel.CoinCollectionBoxBStatus = _opl(TXT_TOKENMGR_WAITTOATTACH);
		m_DataModel.stateB = STATE_NEED_TO_INSTALL;
	}
}


bool CInstallCoinChangeBoxSvc::isBoxCouldBeInstalled(BOX_SELECT box)
{
	try{
		bool could = false;
		tCHADevStatus rsp;
		COIN_HELPER->CH_GetStatus(&rsp); 
		switch(box)
		{
		case CHANGE_BOX_A:					// 硬币箱A
			{
				if (!IsBoxAInstalled && rsp.bIsChangeBoxAInstall){
					could = true;
				}else
					could = false;
			}break;
		case CHANGE_BOX_B:					// 硬币箱B
			{
				if (!IsBoxBInstalled && rsp.bIsChangeBoxBInstall){
					could = true;
				}else
					could = false;
			}break;
		case CHANGE_ALL_BOX:				// 选择安装所有硬币箱
			{
				if (isBoxCouldBeInstalled(CHANGE_BOX_A) || isBoxCouldBeInstalled(CHANGE_BOX_B)){
					could = true;
				}else
					could = false;
			}break;
		}
		return could;
	}
	catch(...)
	{
		theAPP_SESSION.ShowOfficeGuide(_T("硬币模块通讯异常"));
		return false;
	}
	
}

bool CInstallCoinChangeBoxSvc::SetCoinChangeBoxCountInfo(BOX_SELECT box)
{
	bool result = false;
	if ( box == CHANGE_BOX_A){
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO boxInfo = theCOIN_COUNT.GetChangeCoinboxAInfo();

		BYTE line = theMAINTENANCE_INFO.GetStationCode();//_tcstoul(m_DataModel.CoinChangeBoxAID.Left(2),NULL,10);
		BYTE Type = 0x50;//_tcstoul(m_DataModel.CoinChangeBoxAID.Mid(2,2),NULL,10);
		WORD wNum = theTVM_SETTING.GetCoinChangeBox1Id()/*_tcstoul(m_DataModel.CoinChangeBoxAID.Right(4),NULL,16)*/;

		boxInfo.coinBoxID.bLineCode = line;
		boxInfo.coinBoxID.bType = Type;
		boxInfo.coinBoxID.bNumInQu = theMAINTENANCE_INFO.GetEquNumberInGroup();
		boxInfo.coinBoxID.bNum = wNum;
		boxInfo.ulCount = _ttoi(m_DataModel.strCoinACount);
		boxInfo.ulValue = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
		result = theCOIN_COUNT.SetChangeCoinboxAInfo(boxInfo);
		m_DataModel.CoinChangeBoxAID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();

	}
	else if(box == CHANGE_BOX_B){
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO boxInfo = theCOIN_COUNT.GetChangeCoinboxBInfo();
		BYTE line = theMAINTENANCE_INFO.GetStationCode();//_tcstoul(m_DataModel.CoinChangeBoxBID.Left(2),NULL,10);
		BYTE Type = 0x50;//_tcstoul(m_DataModel.CoinChangeBoxBID.Mid(2,2),NULL,10);
		WORD wNum = theTVM_SETTING.GetCoinChangeBox2Id()/*_tcstoul(m_DataModel.CoinChangeBoxBID.Right(4),NULL,16)*/;

		boxInfo.coinBoxID.bLineCode = line;
		boxInfo.coinBoxID.bType = Type;
		boxInfo.coinBoxID.bNumInQu = theMAINTENANCE_INFO.GetEquNumberInGroup();
		boxInfo.coinBoxID.bNum = wNum;
		//boxInfo.coinBoxID.Deserialize((LPBYTE)(LPCTSTR)m_DataModel.CoinChangeBoxBID);
		boxInfo.ulValue = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
		boxInfo.ulCount = _ttoi(m_DataModel.strCoinBCount);
		result = theCOIN_COUNT.SetChangeCoinboxBInfo(boxInfo);
		m_DataModel.CoinChangeBoxBID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();

	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      将箱子安装的数量插入到数据库中

@param      int nBoxNumber（箱子序列号）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CInstallCoinChangeBoxSvc::InsertCoinBoxToDatabase(int nBoxNumber){
	try{
		// 箱子安装记录
		Data::CoinChangeBoxOperationRecord coinChangeBoxOperation;
		coinChangeBoxOperation.dtOperationTime = ComGetCurTime();
		coinChangeBoxOperation.iOperationType  = INSTALL_CHANGE_BOX;
		coinChangeBoxOperation.txtOperatorID	  = theAPP_SESSION.GetUserInfo().sUserID;
		if(nBoxNumber == CHANGE_BOX_A){
			coinChangeBoxOperation.txtCoinChangeBoxID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
			coinChangeBoxOperation.iCoinTypeCode	  = TransforCoinType(theTVM_INFO.GetCHChangeBoxAType());
			coinChangeBoxOperation.iQuantity		  = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
		}
		else if(nBoxNumber == CHANGE_BOX_B){
			coinChangeBoxOperation.txtCoinChangeBoxID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
			coinChangeBoxOperation.iCoinTypeCode	  = TransforCoinType(theTVM_INFO.GetCHChangeBoxBType());
			coinChangeBoxOperation.iQuantity		  = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
		}

		DATA_HELPER->DbInsertCoinChangeBoxOperationRecord(coinChangeBoxOperation);
		/*
		Data::CoinAddOperationRecord coinAddOperationRecord;
		coinAddOperationRecord.dtOperationTime = ComGetCurTime();

		if(nBoxNumber == CHANGE_BOX_A){
			coinAddOperationRecord.txtCoinAddBoxID	  = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
			coinAddOperationRecord.txtCoinChangeBoxID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
			coinAddOperationRecord.iCoinTypeCode	  = TransforCoinType(theTVM_INFO.GetCHChangeBoxAType());
			coinAddOperationRecord.iQuantityBeforeAdd = _ttoi(m_DataModel.nChangeABoxExCount);
			coinAddOperationRecord.iQuantityAdd		  = _ttoi(m_DataModel.strCoinACount);
			coinAddOperationRecord.iQuantityAfterAdd  = _ttoi(m_DataModel.nChangeABoxExCount) + _ttoi(m_DataModel.strCoinACount);
			coinAddOperationRecord.txtOperatorID	  = theAPP_SESSION.GetUserInfo().sUserID;
		}
		else if(nBoxNumber == CHANGE_BOX_B){
			coinAddOperationRecord.txtCoinAddBoxID	  = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
			coinAddOperationRecord.txtCoinChangeBoxID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
			coinAddOperationRecord.iCoinTypeCode	  = TransforCoinType(theTVM_INFO.GetCHChangeBoxBType());
			coinAddOperationRecord.iQuantityBeforeAdd = _ttoi(m_DataModel.nChangeBBoxExCount);
			coinAddOperationRecord.iQuantityAdd		  = _ttoi(m_DataModel.strCoinBCount);
			coinAddOperationRecord.iQuantityAfterAdd  = _ttoi(m_DataModel.nChangeBBoxExCount) + _ttoi(m_DataModel.strCoinBCount);
			coinAddOperationRecord.txtOperatorID	  = theAPP_SESSION.GetUserInfo().sUserID;		
		}
		else{
			return;
		}


		// 插入数据库
		DATA_HELPER->DbInsertCoinAddOperationRecord(coinAddOperationRecord);*/

	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转换硬币类型

@param      int nCoinType硬币类型

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CInstallCoinChangeBoxSvc::TransforCoinType(int nCoinType){
	switch(nCoinType){
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
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
 bool CInstallCoinChangeBoxSvc::CheckInputBoxID(BOX_SELECT boxType,int nBoxId){
	CString strBoxId(_T(""));
	strBoxId.Format(_T("%02d50%04d"),theMAINTENANCE_INFO.GetLineCode(),nBoxId);
	if(boxType == BOX_SELECT::CHANGE_BOX_A){
		 if(!strBoxId.IsEmpty()){
			 if(strBoxId.Compare(m_DataModel.CoinChangeBoxBID) == 0){
				 return true;
			 }
			 else{
				m_DataModel.CoinChangeBoxAID = strBoxId;
			 }
		 }
		 return false;
	 }
	 else if(boxType == BOX_SELECT::CHANGE_BOX_B){
		 if(!strBoxId.IsEmpty()){
			 if(strBoxId.Compare(m_DataModel.CoinChangeBoxAID) == 0){
				 return true;
			 }
			 else{
				 m_DataModel.CoinChangeBoxBID = strBoxId;
			 }
		 }
		 return false;
	 }
	 else 
		 return false;
 }