#include "stdafx.h"
#include "CoinBoxManageDetachCoinCollectionBoxDlg.h"
#include "CoinBoxManageDetachCoinCollectionBoxSvc.h"
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


BEGIN_MESSAGE_MAP(CCoinBoxManageDetachCoinCollectionBoxSvc,CTVMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageDetachCoinCollectionBoxSvc::CCoinBoxManageDetachCoinCollectionBoxSvc()
	:CTVMForeService(DETACH_COLLECTION_COIN_BOX_SVC)
{

	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20101_SYSTEM_SET_DLG, new CCoinBoxManageDetachCoinCollectionBoxDlg(this));

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
CCoinBoxManageDetachCoinCollectionBoxSvc::~CCoinBoxManageDetachCoinCollectionBoxSvc()
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
void CCoinBoxManageDetachCoinCollectionBoxSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT| KEY_F2 | KEY_RETURN);	// 启用指定按键

	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	IsBoxARemoved = FALSE;
	IsBoxBRemoved = FALSE;

	//InitializeDataModel();
	InitializeData();

	coincollectioninfoA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	coincollectioninfoB = theCOIN_COUNT.GetCollectionCoinboxBInfo();

	try{
		//COIN_HELPER->ReadStatus(m_chcommon_rsp);
	}
	catch(CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.IsHasException = TRUE;
	}
	// 处理硬币回收箱数据 
	HandleCoinCollectionBoxData();

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_DETACH_COLLECTION_BOX);
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_DETACH_COLLECTION_BOX);
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
void CCoinBoxManageDetachCoinCollectionBoxSvc::InitializeDataModel()
{
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxA50AVOSData = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxA1MOPData = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxA2MOPData = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxA5MOPData = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxATotalData = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAStatus = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionAfterBoxAStatus = _T("");
	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAfterDetachAID = _T("");

	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxB50AVOSData = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxB1MOPData = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxB2MOPData = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxB5MOPData = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBTotalData = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBStatus = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionAfterBoxBStatus = _T("");
	m_DataModel.coincollectionboxBdata.CoinCollectionBoxAfterDetachBID = _T("");

	m_DataModel.DetachCoinCollectionBoxFlowTime = 1;
	m_DataModel.IsHasException = FALSE;
	m_DataModel.bIsRemoveBoxA  = false;
	m_DataModel.bIsRemoveBoxB  = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCoinBoxManageDetachCoinCollectionBoxSvc::Model& CCoinBoxManageDetachCoinCollectionBoxSvc::GetDataModel()
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
void CCoinBoxManageDetachCoinCollectionBoxSvc::NotifyDataModelChanged()
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
CString CCoinBoxManageDetachCoinCollectionBoxSvc::GetHopperCoinType(int hopetype)
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
@brief      获取硬币循环找零箱币种

@param      hopetype

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageDetachCoinCollectionBoxSvc::HandleDataModel(int index)
{
 	LONG CoinCount = -1;					// 单种面值硬币枚数
 	LONG CoinSum = -1;						// 单种面值硬币总额
 	LONG CoinCollectionBoxATotal = 0;		// 硬币回收箱A合计
 	LONG CoinCollectionBoxBTotal = 0;		// 硬币回收箱B合计
 	CString TempCount = _T("");
 
 	if(1 == index)
 	{
 		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfo;
 		coincollectioninfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();
 
 		m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = coincollectioninfo.coinBoxID.ToString();
 		// 5AVOS枚数、总额
 		CoinCount = coincollectioninfo.GetCurCount();
 		CoinSum = CoinCount*50;
 		CoinCollectionBoxATotal += CoinSum;
 		TempCount.Format(_T("%ld"),CoinCount);
 		m_DataModel.coincollectionboxAdata.CoinCollectionBoxA50AVOSData = TempCount + _T("    ") + ComFormatAmount(CoinSum) + _T("MOP");		// 硬币回收箱A50AVOSData
 
 		// 1MOP枚数、总额
 		CoinCount = coincollectioninfo.GetCurCount();
 		CoinSum = CoinCount*100;
 		CoinCollectionBoxATotal += CoinSum;
 		TempCount.Format(_T("%ld"),CoinCount);
 		m_DataModel.coincollectionboxAdata.CoinCollectionBoxA1MOPData = TempCount + _T("    ") + ComFormatAmount(CoinSum) + _T("MOP");				// 硬币回收箱A1MOPData
 
 		m_DataModel.coincollectionboxAdata.CoinCollectionBoxATotalData = ComFormatAmount(CoinCollectionBoxATotal) + _T("MOP");	
 	}
 	else if(2 == index)
 	{
 		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectioninfo;
 		coincollectioninfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();
 
 		m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = coincollectioninfo.coinBoxID.ToString();
 		// 5AVOS枚数、总额
 		CoinCount = coincollectioninfo.GetCurCount();
 		CoinSum = CoinCount*50;
 		CoinCollectionBoxBTotal += CoinSum;
 		TempCount.Format(_T("%ld"),CoinCount);
 		m_DataModel.coincollectionboxBdata.CoinCollectionBoxB50AVOSData = TempCount + _T("    ") + ComFormatAmount(CoinSum) + _T("MOP");		// 硬币回收箱A50AVOSData
 
 		// 1MOP枚数、总额
 		CoinCount = coincollectioninfo.GetCurCount();
 		CoinSum = CoinCount*100;
 		CoinCollectionBoxBTotal += CoinSum;
 		TempCount.Format(_T("%ld"),CoinCount);
 		m_DataModel.coincollectionboxBdata.CoinCollectionBoxB1MOPData = TempCount + _T("    ") + ComFormatAmount(CoinSum) + _T("MOP");				// 硬币回收箱A1MOPData
 
 		m_DataModel.coincollectionboxBdata.CoinCollectionBoxBTotalData = ComFormatAmount(CoinCollectionBoxBTotal) + _T("MOP");	
 	}
 	m_DataModel.DetachCoinCollectionBoxFlowTime = FirstFrame;
 	NotifyDataModelChanged();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     处理硬币回收箱数据 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageDetachCoinCollectionBoxSvc::HandleCoinCollectionBoxData()
{

	CString collectionboxID = _T("");

	long errCode = 0;						// 错误代码
	//CH_COMMON_RSP chcommonrsp;				// 通用命令响应数据
	//CH_RSP_READ_RFID result;				// 读RFID
	//COIN_DATA coindata;						// 硬币回收箱A、B枚数

	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionAinfo;					// 硬币回收箱A计数信息
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionBinfo;					// 硬币回收箱B计数信息

	coincollectionAinfo = theCOIN_COUNT.GetCollectionCoinboxAInfo();				// 获取硬币回收箱A计数信息
	coincollectionBinfo = theCOIN_COUNT.GetCollectionCoinboxBInfo();				// 获取硬币回收箱B计数信息

	try
	{
		// 检测硬币回收箱是否安装
		//errCode = COIN_HELPER->ReadStatus(chcommonrsp);

		// 硬币回收箱A未安装
		if(_T("FFFFFFFF") == coincollectionAinfo.coinBoxID.ToString())
		{
			HandleDataModel(1);
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX1_SET_STATUS)
			{
				m_DataModel.coincollectionboxAdata.CoinCollectionBoxAStatus = m_DataModel.coincollectionboxAdata.CoinBoxABeforeRemove =_opl(TXT_COINMGR_BOX_DETACHED);				// 硬币回收箱A已卸载

				m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = _T("--------");

			}
			//else
			//{
			//	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAStatus = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);		// 非法安装

			//	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = _T("--------");
			//}
			IsBoxARemoved = TRUE;
		}
		else
		{
			HandleDataModel(1);
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX1_SET_STATUS)
			//{
			//	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAStatus = _opl(DEVICESTATUS_ILLEGAL_DETACH);			// 硬币回收箱A非法卸载
			//}
			//else
			//{
			//	errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_COLLECT1, result);	
			//	USES_CONVERSION;
			//	m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = A2T(result.rfid_info.data);
			//	if(coincollectionAinfo.coinBoxID.ToString() != m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID)
			//	{
			//		m_DataModel.coincollectionboxAdata.CoinCollectionBoxAStatus = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);	// 硬币回收箱A非法安装
			//		m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = _T("--------");
			//	}
			//	else
			{
				m_DataModel.coincollectionboxAdata.CoinCollectionBoxAID = coincollectionAinfo.coinBoxID.ToString();	// (此句代码是不检测安装是否非法加的)
				m_DataModel.coincollectionboxAdata.CoinCollectionBoxAStatus =m_DataModel.coincollectionboxAdata.CoinBoxABeforeRemove = _opl(TXT_COINMGR_BOX_ATTACHED);			// 硬币回收箱A已安装
			}
			//}

		}

		// 硬币回收箱B未安装
		if(_T("FFFFFFFF") == coincollectionBinfo.coinBoxID.ToString())
		{
			HandleDataModel(2);
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX2_SET_STATUS)
			{
				m_DataModel.coincollectionboxBdata.CoinCollectionBoxBStatus = m_DataModel.coincollectionboxBdata.CoinBoxBBeforeRemove = _opl(TXT_COINMGR_BOX_DETACHED);				// 硬币回收箱A已卸载

				m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = _T("--------");

			}
			//else
			//{
			//	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBStatus = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);		// 非法安装

			//	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = _T("--------");
			//}
			IsBoxBRemoved = TRUE;
		}
		else
		{
			HandleDataModel(2);
			//if(FALSE == chcommonrsp.sensor_info.CH_BOX2_SET_STATUS)
			//{
			//	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBStatus = _opl(DEVICESTATUS_ILLEGAL_DETACH);			// 硬币回收箱B非法卸载
			//}
			//else
			//{
			//	errCode = COIN_HELPER->ReadCoinBoxRFID(CH_BOX_ID_COLLECT2, result);	
			//	USES_CONVERSION;
			//	m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = A2T(result.rfid_info.data);
			//	if(coincollectionBinfo.coinBoxID.ToString() != m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID)
			//	{
			//		m_DataModel.coincollectionboxBdata.CoinCollectionBoxBStatus = _opl(DEVICESTATUS_ILLEGAL_INSTALLATION);	// 硬币回收箱B非法安装
			//		m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = _T("--------");
			//	}
			//	else
			{
				m_DataModel.coincollectionboxBdata.CoinCollectionBoxBID = coincollectionBinfo.coinBoxID.ToString();		// (此句代码是不检测安装是否非法加的)
				m_DataModel.coincollectionboxBdata.CoinCollectionBoxBStatus = m_DataModel.coincollectionboxBdata.CoinBoxBBeforeRemove = _opl(TXT_COINMGR_BOX_ATTACHED);			// 硬币回收箱B已安装

			}
			//}

		}
		NotifyDataModelChanged();
	}
	// 自定义异常处理
	catch (CSysException& e) {
		m_DataModel.IsHasException = TRUE;
		NotifyDataModelChanged();
		theEXCEPTION_MGR.ProcessException(e);
	}
	// 自定义外的异常处理
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
LRESULT CCoinBoxManageDetachCoinCollectionBoxSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_DataModel.IsHasException){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_EXCEPTION));
		return FALSE;
	}
	int index = theAPP_SESSION.GetInputToInt();														// 获取向导栏输入内容
	// 输入内容非法
	if(index != MENU_CODE_1/* || index > MENU_CODE_3*/)
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
		return TRUE;
	}
	// 输入内容合法
	else
	{
		//HandleCoinCollectionBoxData();
		if(MENU_CODE_1 == index){
			if(IsBoxCouldBeRemoved(coin_collection_boxA)){// A未移除
				m_DataModel.BoxA.status = ST_PRE_REMOVED;
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_ALREADY));
			}			
		}
		else if(MENU_CODE_2 == index){
			if(IsBoxCouldBeRemoved(coin_collection_boxB)){// B未移除
				m_DataModel.BoxB.status = ST_PRE_REMOVED;
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_ALREADY));
			}			
		}
		else if(MENU_CODE_3 == index){
			if(IsBoxCouldBeRemoved(coin_collection_All)){
				if(IsBoxCouldBeRemoved(coin_collection_boxA)){// A未移除
					m_DataModel.BoxA.status = ST_PRE_REMOVED;
				}
				if(IsBoxCouldBeRemoved(coin_collection_boxB)){// B未移除
					m_DataModel.BoxB.status = ST_PRE_REMOVED;
				}
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
			}else{
				theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_DETACH_ALREADY));
			}
		}
		NotifyDataModelChanged();	
	}
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
LRESULT CCoinBoxManageDetachCoinCollectionBoxSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
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
LRESULT CCoinBoxManageDetachCoinCollectionBoxSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{	
	CString strTips = _T("");

	long errCode = 0;																				// 错误代码										
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO coincollectionboxInfo;								// 硬币回收箱计数信息

	// 如果有卸载操作
	bool isOperated = (m_DataModel.BoxA.status == ST_PRE_REMOVED) || (m_DataModel.BoxB.status == ST_PRE_REMOVED);

	if(!isOperated){
		return 0;
	}
	// 硬币回收箱A卸载
	if(m_DataModel.BoxA.status == ST_PRE_REMOVED)
	{

		coincollectionboxInfo = CTCoinCountInfo::COIN_COLLECTION_BOX_INFO();
		coincollectionboxInfo.ulInitCount = mEx_collect_A.ulInitCount;
		coincollectionboxInfo.operateTime = ComGetCurTime();
		coincollectionboxInfo.ulOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
		m_DataModel.BoxA.countInfo = coincollectionboxInfo;
		// 硬币回收箱AID置为0，置硬币回收箱A硬币数量为0，写入软件计数
		if(!theCOIN_COUNT.SetCollectionCoinboxAInfo(coincollectionboxInfo))	{
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱A的计数信息写入失败"));
			strTips += _opl(TXT_COINMGR_COLLECTION_BOXA_UNINSTALL_FAILED);
			errCode = 1;
		}
		else{
			try
			{
				m_DataModel.BoxA.status = ST_REMOVED;
				m_DataModel.BoxA.countInfo.ulCount_1yuan = 0;
				m_DataModel.BoxA.countInfo.ulCount_5jiao = 0;
				LOG("硬币回收箱卸载成功：%d！", mEx_collect_A.ulCount_1yuan + mEx_collect_A.ulCount_5jiao);
				strTips += _T("硬币回收箱卸载成功")/*_opl(TXT_COINMGR_COLLECTION_BOXA_UNINSTALL_SUCCESS)*/;
				theTVM_STATUS_MGR.SetCoinCollectionBox1Status(STATUS_MAGAZINE::STATUS_MAGAZINE_UNSET);
				// 数据导入数据库
				Dataintodatabase(coin_collection_boxA);
				// 上传电文
				Sendcables(coin_collection_boxA);
				// 打印
				//PrintCoinBoxExchangeReceipt(coin_collection_boxA);
				// 本机设置日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,true);
			}
			catch(...)
			{
				strTips += _opl(TXT_COINMGR_EXCEPTION);
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,false);
				errCode = 1;
			}
		}
	}
	//// 硬币回收箱B卸载
	//if(m_DataModel.BoxB.status == ST_PRE_REMOVED)
	//{
	//	coincollectionboxInfo = CTCoinCountInfo::COIN_COLLECTION_BOX_INFO();
	//	m_DataModel.BoxB.countInfo = coincollectionboxInfo;
	//	coincollectionboxInfo.ulInitCount = mEx_collect_B.ulInitCount;
	//	// 硬币回收箱AID置为0，置硬币回收箱A硬币数量为0，写入软件计数
	//	if(!theCOIN_COUNT.SetCollectionCoinboxBInfo(coincollectionboxInfo))	{
	//		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币回收箱B的计数信息写入失败"));
	//		strTips += _opl(TXT_COINMGR_COLLECTION_BOXB_UNINSTALL_FAILED);
	//		errCode = 1;
	//	}else{
	//		try
	//		{
	//			m_DataModel.BoxB.status = ST_REMOVED;
	//			m_DataModel.BoxB.countInfo.ulCount_1yuan = 0;
	//			strTips += _opl(TXT_COINMGR_COLLECTION_BOXB_UNINSTALL_SUCCESS);
	//			// 数据导入数据库
	//			Dataintodatabase(coin_collection_boxB);
	//			// 上传电文
	//			Sendcables(coin_collection_boxB);
	//			// 打印
	//			//PrintCoinBoxExchangeReceipt();
	//			// 本机设置日志
	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,true);
	//		}
	//		catch(...)
	//		{
	//			if (strTips != _opl(TXT_COINMGR_EXCEPTION))
	//			{
	//				strTips = _opl(TXT_COINMGR_EXCEPTION);
	//			}
	//			
	//			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_COINBOX_REMOVAL,false);
	//			errCode = 1;
	//		}
	//	}
	//}
	if (isOperated){
		PrintCoinBoxExchangeReceipt();
		try{
			COIN_HELPER->CH_SetCoinNum();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){

		}
	}

	mEx_change_A = theCOIN_COUNT.GetChangeCoinboxAInfo();
	mEx_change_B = theCOIN_COUNT.GetChangeCoinboxBInfo();

	mEx_hopper_A = theCOIN_COUNT.GetCoinHopperboxAInfo();	
	mEx_hopper_B = theCOIN_COUNT.GetCoinHopperboxBInfo();

	mEx_collect_A = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	mEx_collect_B = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	strTips += _opl(GUIDE_F3_ESC);

	theAPP_SESSION.ShowOfficeGuide(strTips);

	if(m_DataModel.bIsPrintFalied){
		Sleep(1000);
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));
	}
	NotifyDataModelChanged();
	return errCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上传电文

@param      int boxnumber

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageDetachCoinCollectionBoxSvc::Sendcables(int boxnumber)
{
	CASHBOX_CHANGE  coinbox;					// 硬币回收箱更换业务数据
	vector<CASH_TYPE_COUNT> vecCash;			// 硬币回收箱币种代码、数量

	// 硬币回收箱A、B--操作类型
	coinbox.handleType = UNINSTALL;
	CASH_TYPE_COUNT cash;
	
	// 硬币回收箱A--ID、位置ID
	if(coin_collection_boxA == boxnumber)
	{
		// 硬币回收箱A--ID
		coinbox.SetBoxID(mEx_collect_A.coinBoxID.bLineCode, mEx_collect_A.coinBoxID.bType,mEx_collect_A.coinBoxID.bNumInQu,mEx_collect_A.coinBoxID.bNum);
		// 硬币回收箱A位置ID
		coinbox.position = MAGAZINEG;
		// 硬币回收箱A币种代码、数量
		cash.cash_type = Coin1Yuan;
		cash.cash_count =  mEx_collect_A.ulCount_1yuan;
		vecCash.push_back(cash);

		//cash.cash_type = CoinHalfYuan;
		//cash.cash_count =  mEx_collect_A.ulCount_5jiao;
		//vecCash.push_back(cash);
	}
	// 硬币回收箱B--ID、位置ID
	else
	{
		// 硬币回收箱B--ID
		coinbox.SetBoxID(mEx_collect_B.coinBoxID.bLineCode, mEx_collect_B.coinBoxID.bType,mEx_collect_B.coinBoxID.bNumInQu,mEx_collect_B.coinBoxID.bNum);
		// 硬币回收箱位置ID
		coinbox.position = MAGAZINEG;
		// 硬币回收箱B币种代码、数量
		cash.cash_type = Coin1Yuan;
		cash.cash_count =  mEx_collect_B.ulCount_1yuan;
		vecCash.push_back(cash);

		//cash.cash_type = CoinHalfYuan;
		//cash.cash_count =  mEx_collect_B.ulCount_5jiao;
		//vecCash.push_back(cash);
	}
	// 硬币回收箱状态
	coinbox.State = VALID_DETACH;

	// 硬币回收箱币种代码、数量
	coinbox.vecCash = vecCash;

	// 硬币回收箱币种总数
	coinbox.btCount = 1;//coinbox.GetSumCurrency();

	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBanknoteBoxExchange(coinbox);

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      打包币种代码、数量

@param     int FAVOScount	50AVOS

@param     int OMOPcount	1MOP

@param     int SMOPcount	2MOP

@param     int FMOPcount	5MOP

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
vector<CASH_TYPE_COUNT> CCoinBoxManageDetachCoinCollectionBoxSvc::PackageCoinTypeAndCount(int FAVOScount,int OMOPcount,int SMOPcount,int FMOPcount)
{
	CASH_TYPE_COUNT cointypecount;
	vector<CASH_TYPE_COUNT> vecCash;

	// 5AVOS枚数不为0
	if(0 != FAVOScount)
	{
		cointypecount.cash_type = CoinHalfYuan;
		cointypecount.cash_count = FAVOScount;
		vecCash.push_back(cointypecount);
	}

	// 1MOP枚数不为0
	if(0 != OMOPcount)
	{
		cointypecount.cash_type = Coin1Yuan;
		cointypecount.cash_count = OMOPcount;
		vecCash.push_back(cointypecount);
	}

	// 2MOP枚数不为0
	//if(0 != SMOPcount)
	//{
	//	cointypecount.cash_type = Coin2MOP;
	//	cointypecount.cash_count = SMOPcount;
	//	vecCash.push_back(cointypecount);
	//}

	//// 5MOP枚数不为0
	//if(0 != FMOPcount)
	//{
	//	cointypecount.cash_type = Coin5MOP;
	//	cointypecount.cash_count = FMOPcount;
	//	vecCash.push_back(cointypecount);
	//}
	return vecCash;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币循环找零箱加币凭证打印

@param      int Box_No

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinBoxManageDetachCoinCollectionBoxSvc::PrintCoinBoxExchangeReceipt()
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

		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_UNINSTALL);

		PRINTER_HELPER->PrintReceiptByBmp(&printtext,true,true);
	}
	catch(CSysException& e) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(e);
		m_DataModel.bIsPrintFalied = true;
	}
	catch (...) {
		//theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_PRINTER_FAILURE));						// 凭证打印失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		m_DataModel.bIsPrintFalied = true;
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
void CCoinBoxManageDetachCoinCollectionBoxSvc::Dataintodatabase(int collectionboxnumber)
{
	try
	{
		CoinCollectBoxRemoveOperationRecord removecoincollectionboxrecord;
		//removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iID = 0;								// 记录编号
		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.dtTransTime = ComGetCurTime();			// 操作时间
		CoinBoxCashDetail coinboxcashdetail;
		if(coin_collection_boxA == collectionboxnumber)
		{
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtCoinCollectBoxID = coincollectioninfoA.coinBoxID.ToString();
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iTotalAmount = coincollectioninfoA.ulCount_5jiao*50 + coincollectioninfoA.ulCount_1yuan*100 /*+ coincollectioninfoA.ulCount_5yuan*500*/;
			
			coinboxcashdetail.iCoinTypeCode = Coin1Yuan;
			coinboxcashdetail.iQuantity = mEx_collect_A.ulCount_1yuan;
			removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

			coinboxcashdetail.iCoinTypeCode = CoinHalfYuan;
			coinboxcashdetail.iQuantity = mEx_collect_A.ulCount_5jiao;
			removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);
		}
		else if(coin_collection_boxB == collectionboxnumber)
		{
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtCoinCollectBoxID = coincollectioninfoB.coinBoxID.ToString();
			removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.iTotalAmount = coincollectioninfoB.ulCount_5jiao*50 + coincollectioninfoB.ulCount_1yuan*100/* + coincollectioninfoB.ulCount_2yuan*200 + coincollectioninfoB.ulCount_5yuan*500*/;
			coinboxcashdetail.iCoinTypeCode = Coin1Yuan;
			coinboxcashdetail.iQuantity = mEx_collect_B.ulCount_1yuan;
			removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);

			coinboxcashdetail.iCoinTypeCode = CoinHalfYuan;
			coinboxcashdetail.iQuantity = mEx_collect_B.ulCount_5jiao;
			removecoincollectionboxrecord.v_coinBoxCashDetail.push_back(coinboxcashdetail);
		}
		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtOperatorID = theAPP_SESSION.GetUserInfo().sUserID;														// 操作员ID
		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtComment = _T("");
		removecoincollectionboxrecord.coinCollectBoxRemoveOperationDetail.txtReserve = _T("");

		DATA_HELPER->DbInsertCoinCollectBoxRemoveOperationRecord(removecoincollectionboxrecord);
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_COINMGR_INSERT_SQL_FAILURE));						// 数据导入数据库失败
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

}

// 数据初始化
void CCoinBoxManageDetachCoinCollectionBoxSvc::InitializeData()
{
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO  infoA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO  infoB = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	m_DataModel.BoxA.countInfo = infoA;
	if (infoA.coinBoxID.ToString() == _T("FFFFFFFF")){
		m_DataModel.BoxA.status = ST_REMOVED;
	}else{
		m_DataModel.BoxA.status = ST_INSTALLED;
	}
	m_DataModel.BoxB.countInfo = infoB;
	if (infoB.coinBoxID.ToString() == _T("FFFFFFFF")){
		m_DataModel.BoxB.status = ST_REMOVED;
	}else{
		m_DataModel.BoxB.status = ST_INSTALLED;
	}

	m_DataModel.strColBoxAExID = infoA.coinBoxID.ToString();
	m_DataModel.strColBoxBExID = infoB.coinBoxID.ToString();

	m_DataModel.nColBoxAExCount = infoA.GetCurCount();
	m_DataModel.nColBoxAExCount = infoB.GetCurCount();

	if(theEXCEPTION_MGR.HasException(CCHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
		m_DataModel.IsHasException = true;
	}
}


// 判断箱子是否可被卸载
bool CCoinBoxManageDetachCoinCollectionBoxSvc::IsBoxCouldBeRemoved(COIN_COLLECTION_BOX box)
{
	bool Yes = true;
	if(box ==  coin_collection_boxA){
		if ( m_DataModel.BoxA.status != ST_REMOVED){
			Yes = true;
		}else
			Yes = false;
	}else if (box == coin_collection_boxB){
		if( m_DataModel.BoxB.status != ST_REMOVED){
			Yes = true;
		}else
			Yes = false;
	}else if ( box = coin_collection_All){
		if (IsBoxCouldBeRemoved(coin_collection_boxA) || IsBoxCouldBeRemoved(coin_collection_boxB)){
			Yes = true;
		}else
			Yes = false;
	}
	return Yes;
}