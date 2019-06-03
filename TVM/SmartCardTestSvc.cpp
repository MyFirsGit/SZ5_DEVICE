#include "stdafx.h"
#include "SmartCardTestSvc.h"
#include "SmartCardTestMaintainDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_COINTEST_MENU_TO_OPERATION = _T("WM_COINTEST_MENU_TO_OPERATION");
static const TCHAR* const WM_COINTEST_OPERATION_TO_MENU = _T("WM_COINTEST_OPERATION_TO_MENU");

static const TCHAR* const WM_COINTEST_MENU_TO_SENSOR = _T("WM_COINTEST_MENU_TO_SENSOR");
static const TCHAR* const WM_COINTEST_SENSOR_TO_MENU = _T("WM_COINTEST_SENSOR_TO_MENU");

static const TCHAR* const WM_COINTEST_MENU_TO_READ_RFID = _T("WM_COINTEST_MENU_TO_READ_RFID");
static const TCHAR* const WM_COINTEST_READ_RFID_TO_MENU = _T("WM_COINTEST_READ_RFID_TO_MENU");

static const TCHAR* const WM_COINTEST_MENU_TO_WRITE_RFID = _T("WM_COINTEST_MENU_TO_WRITE_RFID");
static const TCHAR* const WM_COINTEST_WRITE_RFID_TO_MENU = _T("WM_COINTEST_WRITE_RFID_TO_MENU");
BEGIN_MESSAGE_MAP(CSmardCardTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSmardCardTestSvc::CSmardCardTestSvc() :CTVMForeService(CARD_MODULE_TEST_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_TH_TEST_DLG, new CSmartCardTestMaintainDlg(this));

	//设置TH测试画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TH_TEST, _opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TH_TEST, IDD_TH_TEST_DLG);

	//设置电机测试画面组
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_MOTOR_TEST, _opl(GUIDE_ENTER_F3_ESC));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_MOTOR_TEST, IDD_TH_TEST_DLG);

	//设置读RFID测试画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_READ_RFID_TEST, _opl(GUIDE_F3_F6_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_READ_RFID_TEST, IDD_TH_TEST_DLG);

	//设置写RFID测试画面组
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WRITE_RFID_TEST, _opl(GUIDE_ENTER_F3_ESC));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WRITE_RFID_TEST, IDD_TH_TEST_DLG);

	//设置传感器测试画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SENSOR_TEST, _opl(GUIDE_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SENSOR_TEST, IDD_TH_TEST_DLG);

	//设置画面扭转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_OPERATION,DIALOG_GROUP_MOTOR_TEST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_MOTOR_TEST,WM_COINTEST_OPERATION_TO_MENU,DIALOG_GROUP_TH_TEST);

	//设置画面扭转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_READ_RFID,DIALOG_GROUP_READ_RFID_TEST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_READ_RFID_TEST,WM_COINTEST_READ_RFID_TO_MENU ,DIALOG_GROUP_TH_TEST);

	//设置画面扭转
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_WRITE_RFID ,DIALOG_GROUP_WRITE_RFID_TEST);
	//m_pDialogFlow->AddFlow(DIALOG_GROUP_WRITE_RFID_TEST,WM_COINTEST_WRITE_RFID_TO_MENU ,DIALOG_GROUP_TH_TEST);

	//设置画面扭转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_TH_TEST,WM_COINTEST_MENU_TO_SENSOR ,DIALOG_GROUP_SENSOR_TEST);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_SENSOR_TEST,WM_COINTEST_SENSOR_TO_MENU ,DIALOG_GROUP_TH_TEST);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_TEST);

	//初始化
	//m_wSmartCardFirstCommond  = TEST_NUM_UNKNOWN;
	//m_wSmartCardSecondCommand = TEST_NUM_UNKNOWN;
	////m_eTransferSpeedSet       = TEST_SPEED::NORMAL_SPEED;//传送速度
	//memset(m_bIssueData,0,sizeof(m_bIssueData));         //初始化
	//m_bIssueData[0]           = 1;                       //只从A票箱发票
	//m_bIssueData[1]           = theTVM_INFO.GetTHTestRelease();//3; //连续发票张数
	//m_bIssueData[6]           = 1;
	//isConnect = FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSmardCardTestSvc::~CSmardCardTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      业务启动后的操作

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::OnStart()
{
	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_TEST);

	//初始化
	m_Modol = Modol();
	m_OperationType = OPERATION_TYPE_UNKNOW;
	//m_wSmartCardFirstCommond  = TEST_NUM_UNKNOWN;
	m_wSmartCardSecondCommand = -1;
	testType = -1;
	type_op = NULL_OP;
	////m_eTransferSpeedSet       = TEST_SPEED::NORMAL_SPEED;//传送速度
	//memset(m_bIssueData,0,sizeof(m_bIssueData));         //初始化
	//m_bIssueData[0]           = 1;                       //只从A票箱发票
	//m_bIssueData[1]           = theTVM_INFO.GetTHTestRelease();//3;//连续发票张数
	//m_bIssueData[6]           = 1;

	// 设置模块为维修模式
	try{
		CARD_HELPER->SetWorkMode(CMD_WORK_MODE_MAINTENANCE);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
	isMotorStart = FALSE;

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [ENTER]按键处理消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSmardCardTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	// 发卡模块测试主界面
	if (GetCurDialogGroupID() == DIALOG_GROUP_TH_TEST){
		int index = theAPP_SESSION.GetInputToInt();
		// 非超级用户只响应重置测试
		if(index != MENU_CODE_1 && !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return FALSE;
		}
		switch (index){
		// [1] 重置
		case MENU_CODE_1: 
			m_OperationType = OPERATION_TYPE_RETSET;
			break;
		// [2] 抓取票卡  [1]A 
		case MENU_CODE_21:   
			m_OperationType = OPERATION_TYPE_CATCH_CARD;
			m_wSmartCardSecondCommand = 1;
			break;
		//     抓取票卡   [2]B
		case MENU_CODE_22:
			m_OperationType = OPERATION_TYPE_CATCH_CARD;
			m_wSmartCardSecondCommand = 2;
			break;
		//	[3] 放出票卡
		case MENU_CODE_3:
			m_OperationType = OPERATION_TYPE_ISSUE_CARD;
			break;
		// 票箱A上升  
		case MENU_CODE_41: 
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 1;
			m_bActionType = 1;
			break;
		// 票箱A下降
		case MENU_CODE_42:
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 1;
			m_bActionType = 2;
			break;
		// 票箱B上升
		case MENU_CODE_43:
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 2;
			m_bActionType = 1;
			break;
		// 票箱B下降
		case MENU_CODE_44:
			m_OperationType = OPERATION_TYPE_MOVE_BOX;
			m_wSmartCardSecondCommand = 2;
			m_bActionType = 2;
			break;
		//读票箱RFID
		case MENU_CODE_5:
			m_OperationType = OPERATION_TYPE_READ_RFID;
			DoReadRFIDTestOperation();
			DoDialogFlow(WM_COINTEST_MENU_TO_READ_RFID);
			break;
		//写票箱RFID
			/*case MENU_CODE_6:
			m_OperationType = OPERATION_TYPE_WRITE_RFID;
			ReadRFID();
			DoDialogFlow(WM_COINTEST_MENU_TO_WRITE_RFID);
			break;*/
		//票箱A -> 出票口
		case MENU_CODE_61: 
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 1;m_bIsCardSendOut = true;
			break;
		// 票箱A -> 废票箱
		case MENU_CODE_62:
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 1;m_bIsCardSendOut = false;
			break;
		//票箱B -> 出票口 
		case MENU_CODE_63:
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 2;m_bIsCardSendOut = true;
			break;
		// 票箱B -> 废票箱
		case MENU_CODE_64:
			m_OperationType = OPERATION_TYPE_ISSUE_TEST;
			m_wSmartCardSecondCommand = 2;m_bIsCardSendOut = false;
			break;
		//票箱电机测试[1]A  
		/*case MENU_CODE_81:
			m_OperationType = OPERATION_TYPE_MOVE_MOTOR_TEST;
			m_wSmartCardSecondCommand = 1;
			break;
		//票箱电机测试 [2]B
		case MENU_CODE_82:
			m_OperationType = OPERATION_TYPE_MOVE_MOTOR_TEST;
			m_wSmartCardSecondCommand = 2;
			break;*/
		//多种电机测试    【跳转页面】
		//case MENU_CODE_7:
		//	m_OperationType = OPERATION_TYPE_MOTOR_TEST;
		//	DoDialogFlow(WM_COINTEST_MENU_TO_OPERATION);
		//	break;
		 //传感器测试
		case MENU_CODE_7:
			m_OperationType = OPERATION_TYPE_SENSOR_TEST;
			DoSensorTestOperation();
			DoDialogFlow(WM_COINTEST_MENU_TO_SENSOR);
			break;
		/*case MENU_CODE_11:
			m_OperationType = OPERATION_TYPE_ELECTROMAGNET_TEST;
			break;
		case MENU_CODE_12:
			m_OperationType = OPERATION_TYPE_CYCLE_TEMP_TICKS;
			break;*/
		case MENU_CODE_8:
			m_OperationType = OPERATION_TYPE_CYCLE_TEST;
			break;
		default:
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				return FALSE;
			}
			break;
		}
		//设置文言
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		//根据选项，做相应的操作
		DoOperation();
	}
	// 
	else if (GetCurDialogGroupID() == DIALOG_GROUP_MOTOR_TEST){
		int index = theAPP_SESSION.GetInputToInt();
		DoMotorTestOperation();
		//设置文言
		if(index <10){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
	}
	//else if (GetCurDialogGroupID() == DIALOG_GROUP_READ_RFID_TEST)
	//{
	//	DoReadRFIDTestOperation();
	//	//设置文言
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//}
	// RFID测试界面
	/*else if (GetCurDialogGroupID() == DIALOG_GROUP_WRITE_RFID_TEST)	{
		DoWriteRFIDTestOperation();
	}*/
	else{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	}
	//更新界面显示
	NotifyDataModelChanged();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSmardCardTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MOTOR_TEST)
	{
		//如果电机未停止，将其停止
		if(isMotorStart)
		{
			try
			{
				TH_MOTORTEST_DATA motor;
				isMotorStart = false;
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			catch(CSysException& e)
			{
				// Card模块测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...)
			{
				// Card模块测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
			}
		}
		DoDialogFlow(WM_COINTEST_OPERATION_TO_MENU);
	}
	else if(GetCurDialogGroupID() == DIALOG_GROUP_READ_RFID_TEST)
	{
		DoDialogFlow(WM_COINTEST_READ_RFID_TO_MENU);
	}
	/*else if(GetCurDialogGroupID() == DIALOG_GROUP_WRITE_RFID_TEST)
	{
		DoDialogFlow(WM_COINTEST_WRITE_RFID_TO_MENU);
	}*/
	else if(GetCurDialogGroupID() == DIALOG_GROUP_SENSOR_TEST)
	{
		DoDialogFlow(WM_COINTEST_SENSOR_TO_MENU);
	}
	else
	{
		// 恢复模式为正常模式
		try{
			CARD_HELPER->SetWorkMode(CMD_WORK_MODE_NORMAL);
		}
		catch(...){

		}
		theAPP_SESSION.ReturnMaintainMenu();
	}
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
LRESULT CSmardCardTestSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam)
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_MOTOR_TEST)
	{
		//如果电机未停止，将其停止
		if(isMotorStart)
		{
			try
			{
				TH_MOTORTEST_DATA motor;
				isMotorStart = false;
				motor.Switch = 2;
				motor.testType = testType;
//				CARD_HELPER->MotorTest(motor);
			}
			catch(CSysException& e)
			{
				// Card模块测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...)
			{
				// Card模块测试--操作日志
				theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
			}
		}
	}
	__super::OnKeyboardEsc(wParam,lParam);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      [0~9]按键处理消息

@param      (i)WPARAM wParam  数字键值，例如：数字键7，值为7
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//LRESULT CSmardCardTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型数据

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSmardCardTestSvc::Modol& CSmardCardTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      数据改变后，通知窗体更新数据

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      测试处理函数

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoOperation()
{
	theSERVICE_MGR.SetForeServiceBusy(true);
	bool bIsStart = false;
	long errCode = 0;
	int nMenuNum = 0;
	theAPP_SESSION.ShowOfficeGuide(_T("正在测试中..."));
	bool bSuccess = true;
	try{		
		//1.重置测试
		if (m_OperationType == OPERATION_TYPE_RETSET){
			nMenuNum = 0;
			//先复位，复位失败断开重新连接
			try{
				UINT uiRecycleNum = 0;
				CARD_HELPER->Init(CMD_INIT_FAREBOX,&uiRecycleNum);
				//CARD_HELPER->EnableWork(TH_WORK_FLAG::TH_BEGIN_WORK);
			}
			catch(CSysException& e){
				bSuccess = false;
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				bSuccess = false;
			}
			
			m_Modol.menuInfo[nMenuNum].strMenuStatus = bSuccess ? SUCCESS : FAILED;
			
			if(!bSuccess)
				theAPP_SESSION.ShowOfficeGuide(_T("（红）发卡模块重置失败!"));
		}
		else if (m_OperationType == OPERATION_TYPE_CATCH_CARD) // TH抓取票卡命令
		{
			nMenuNum = 1;
			bSuccess = false;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			// 先判断读卡区是否有票
			if(statusRsp.bCardInReadArea){
				theAPP_SESSION.ShowOfficeGuide(_T("读卡区有票，此时无法抓取票卡。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;m_bIsCardCatch = true;
			}
			else if(m_wSmartCardSecondCommand == 1 && statusRsp.bBoxAEmpty){// 抓取票箱1，但是没有卡
				theAPP_SESSION.ShowOfficeGuide(_T("票箱A已空，无法抓取票卡。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;m_bIsCardCatch = false;
			}
			else if(m_wSmartCardSecondCommand == 2 && statusRsp.bBoxBEmpty){
				theAPP_SESSION.ShowOfficeGuide(_T("票箱B已空，无法抓取票卡。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;m_bIsCardCatch = false;
			}
			else{
				//CARD_HELPER->SetWorkMode(TH_CMD_SET_WORK_MODE::CMD_WORK_MODE_NORMAL);
				TH_CMD_CARD_OUT OutCardBox;// = (TH_CMD_CARD_OUT)m_wSmartCardSecondCommand;
				if(m_wSmartCardSecondCommand == 1){
					OutCardBox = CMD_CARD_OUT_BOX_A;
				}
				else{
					OutCardBox = CMD_CARD_OUT_BOX_B;
				}
				
				UINT uiSendOutCount = 0;
				CARD_HELPER->CardOut(OutCardBox,&uiSendOutCount);m_bIsCardCatch = true;
				// 更新计数
				CTCardCountInfo::CARD_BOX_INFO cardAInfo = theCARD_COUNT.GetCardboxAInfo();
				CTCardCountInfo::CARD_BOX_INFO cardBInfo = theCARD_COUNT.GetCardboxBInfo();
				if(m_wSmartCardSecondCommand == 1 && cardAInfo.ticketBoxID.ToString() != INVALID_ID && cardAInfo.ulCurCount >0){
					cardAInfo.ulCurCount -= uiSendOutCount;
					theCARD_COUNT.SetCardboxAInfo(cardAInfo);
				}
				else if(m_wSmartCardSecondCommand == 2  && cardBInfo.ticketBoxID.ToString() != INVALID_ID && cardBInfo.ulCurCount >0){
					cardBInfo.ulCurCount -= uiSendOutCount;
					theCARD_COUNT.SetCardboxBInfo(cardBInfo);
				}
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				bSuccess = true;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_ISSUE_CARD)// TH放出票卡命令/回收票卡命令
		{
			nMenuNum = 2;
			bSuccess = false;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			// 先判断读卡区是否有票
			if(statusRsp.bCardInReadArea && m_bIsCardCatch){
				UINT uiTransportNum = 0;
				CARD_HELPER->SendCard(&uiTransportNum);
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				bSuccess = true;
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_T("读卡区无票或者未先抓取票，无法放出车票。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_MOVE_BOX)// TH升降票箱
		{
			nMenuNum = 3;
			bSuccess = false;
			//theAPP_SESSION.ShowOfficeGuide(_T("当前设备不支持票箱升降操作。"));
			/*SC_MOVE_BOX_CMD_INFO move;
			move.boxId = m_wSmartCardSecondCommand;
			move.actionType = m_bActionType;*/
			TH_CMD_TEST_SETTING testType;
			if(m_wSmartCardSecondCommand == 1 && m_bActionType == 1){
				testType = CMD_TEST_A_TRAY_UP;
			}
			else if(m_wSmartCardSecondCommand == 1 && m_bActionType == 2){
				testType = CMD_TEST_A_TRAY_DOWN;
			}
			else if(m_wSmartCardSecondCommand == 2 && m_bActionType == 1){
				testType = CMD_TEST_B_TRAY_UP;
			}
			else if(m_wSmartCardSecondCommand == 2 && m_bActionType == 2){
				testType = CMD_TEST_B_TRAY_DOWN;
			}
			else{
				testType = CMD_TEST_STOP;
			}
			// 判断上升时，盖板需要打开，否则提示错误
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			if(!statusRsp.bBoxACoverArrive && m_wSmartCardSecondCommand == 1 && m_bActionType == 1){
				theAPP_SESSION.ShowOfficeGuide(_T("票箱A盖板未打开，不能进行上升动作。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else if(!statusRsp.bBoxBCoverArrive && m_wSmartCardSecondCommand == 2 && m_bActionType == 1){
				theAPP_SESSION.ShowOfficeGuide(_T("票箱B盖板未打开，不能进行上升动作。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else{
				CARD_HELPER->TestSetting(testType);m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				bSuccess = true;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_ISSUE_TEST)// TH发票测试命令
		{
			nMenuNum = 7;
			bSuccess = false;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);

			if(statusRsp.bBoxAEmpty && m_wSmartCardSecondCommand == 1){
				theAPP_SESSION.ShowOfficeGuide(_T("票箱A已空，无法发票。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else if(statusRsp.bBoxBEmpty && m_wSmartCardSecondCommand == 2){
				theAPP_SESSION.ShowOfficeGuide(_T("票箱B已空，无法发票。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
			else{
				//CARD_HELPER->EnableWork(TH_WORK_FLAG::TH_BEGIN_WORK);
				// 先抓取一张卡
				TH_CMD_CARD_OUT OutCardBox ;//= (TH_CMD_CARD_OUT)m_wSmartCardSecondCommand;
				if(m_wSmartCardSecondCommand == 1){
					OutCardBox = CMD_CARD_OUT_BOX_A;
				}
				else{
					OutCardBox = CMD_CARD_OUT_BOX_B;
				}
				
				UINT uiTransportNum = 0;
				long lRet = CARD_HELPER->CardOut(OutCardBox,&uiTransportNum);;

				// 更新计数
				CTCardCountInfo::CARD_BOX_INFO cardAInfo = theCARD_COUNT.GetCardboxAInfo();
				CTCardCountInfo::CARD_BOX_INFO cardBInfo = theCARD_COUNT.GetCardboxBInfo();
				if(m_wSmartCardSecondCommand == 1 && cardAInfo.ticketBoxID.ToString() != INVALID_ID && cardAInfo.ulCurCount >0){
					cardAInfo.ulCurCount -= uiTransportNum;
					theCARD_COUNT.SetCardboxAInfo(cardAInfo);
				}
				else if(m_wSmartCardSecondCommand == 2  && cardBInfo.ticketBoxID.ToString() != INVALID_ID && cardBInfo.ulCurCount >0){
					cardBInfo.ulCurCount -= uiTransportNum;
					theCARD_COUNT.SetCardboxBInfo(cardBInfo);
				}

				// 放出票卡
				if(0 == lRet){
					UINT uiTransportNum = 0;
					lRet = m_bIsCardSendOut ? CARD_HELPER->SendCard(&uiTransportNum) : CARD_HELPER->RetractCard(&uiTransportNum);

					// 回收票卡需要增加计数
					if(!m_bIsCardSendOut){
						// 最后回收的票需要计入废票箱中
						CTCardCountInfo::CARD_BOX_INFO cardInfo = theCARD_COUNT.GetCardboxWastedInfo();
						cardInfo.ulCurCount += uiTransportNum;
						theCARD_COUNT.SetCardboxWastedInfo(cardInfo);

						// 需要生成废票交易
						INVALIDE_TICKET_RECYCLETRADE cardInvalid;
						// 保存废卡交易
						cardInvalid.reson = 0x02;// 正常票卡
						cardInvalid.cardPhysicalType = ULTRALIGHT;
						// 卡号
						//memset(cardInvalid.cardSerialNo, 0, sizeof(cardInvalid.cardSerialNo));
						cardInvalid.cardSerialNo = 0;
						// 发行商
						cardInvalid.issueId		 = 0x0002;//ACC
						// 废票箱ID
						//cardInvalid.magazineId.bDeviceType = cardInfo.ticketBoxID.bDeviceType;
						//cardInvalid.magazineId.bType = cardInfo.ticketBoxID.bType;
						//cardInvalid.magazineId.bNum = cardInfo.ticketBoxID.bNum;
						//cardInvalid.magazineId.bStationID = cardInfo.ticketBoxID.bStationID;

						cardInvalid.productType		= 0x6200;

						theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(cardInvalid);
					}
					m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
					bSuccess = true;
				}
				else m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		else if (m_OperationType == OPERATION_TYPE_MOVE_MOTOR_TEST)// 升降电机测试命令
		{
			nMenuNum = 7;
			//CARD_HELPER->MoveMotorTest(m_wSmartCardSecondCommand);
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		}
		else if (m_OperationType == OPERATION_TYPE_ELECTROMAGNET_TEST)// 电磁铁命令
		{
			nMenuNum = 10;
			//CARD_HELPER->ElectromagnetTest();
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		}
		else if(m_OperationType == OPERATION_TYPE_CYCLE_TEMP_TICKS){	// 缓冲区票卡回收
			nMenuNum = 9;
			TH_RSP_GET_DEV_STATUS statusRsp;
			CARD_HELPER->GetDevStatus(&statusRsp);
			// 判断读卡区是否有票
			if(statusRsp.bCardInReadArea){
				UINT uiTransportNum = 0;
				CARD_HELPER->RetractCard(&uiTransportNum);;
			

				// 最后回收的票需要计入废票箱中
				CTCardCountInfo::CARD_BOX_INFO cardInfo = theCARD_COUNT.GetCardboxWastedInfo();
				cardInfo.ulCurCount += uiTransportNum;
				theCARD_COUNT.SetCardboxWastedInfo(cardInfo);

				// 需要生成废票交易
				INVALIDE_TICKET_RECYCLETRADE cardInvalid;
				// 保存废卡交易
				cardInvalid.reson = 0x02;// 正常票卡
				cardInvalid.cardPhysicalType = ULTRALIGHT;
				// 卡号
				//memset(cardInvalid.cardSerialNo, 0, sizeof(cardInvalid.cardSerialNo));
				cardInvalid.cardSerialNo = 0;
				// 发行商
				cardInvalid.issueId		 = 0x0002;//ACC
				// 废票箱ID
				//cardInvalid.magazineId.bDeviceType = cardInfo.ticketBoxID.bDeviceType;
				//cardInvalid.magazineId.bType = cardInfo.ticketBoxID.bType;
				//cardInvalid.magazineId.bNum = cardInfo.ticketBoxID.bNum;
				//cardInvalid.magazineId.bStationID = cardInfo.ticketBoxID.bStationID;
				//cardInvalid.productType		= 0x6200;

				theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertInvalidTicketRecycleMsg(cardInvalid);
			}
			else{
				bSuccess = false;
				theAPP_SESSION.ShowOfficeGuide(_T("缓冲区无票卡，不能进行回收操作。"));
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		// 票卡回收测试
		else if(m_OperationType == OPERATION_TYPE_CYCLE_TEST){
			CString strMsg;
			bool bSuccessBoxA = true,bSuccessBoxB = true;
			// 先从A发送一张车票，然后进入废票箱
			try{
				// 抓取票卡
				long lRet = 0;
				UINT uiTransportNum = 0;

				lRet = CARD_HELPER->CardOut(CMD_CARD_OUT_BOX_A,&uiTransportNum);	// 尝试抓票，如果空了继续抓取另一个票箱
				
				if(lRet == 0){
					UINT uiRetractBoxNum = 0;
					CARD_HELPER->RetractCard(&uiRetractBoxNum);// 进废票箱
				}
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
				bSuccessBoxA = false;
				bSuccess = false;
			}
			catch(...){
				bSuccessBoxA = false;
				bSuccess = false;
			}

			// 从B发送一张车票，然后进入废票箱
			try{
				// 抓取票卡
				long lRet = 0;
				UINT uiTransportNum = 0;

				lRet = CARD_HELPER->CardOut(CMD_CARD_OUT_BOX_B,&uiTransportNum);	// 尝试抓票，如果空了继续抓取另一个票箱
				
				if(lRet == 0){
					UINT uiRetractBoxNum = 0;
					CARD_HELPER->RetractCard(&uiRetractBoxNum);// 进废票箱
				}
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
				bSuccessBoxB = false;
				bSuccess = false;
			}
			catch(...){
				bSuccessBoxB = false;
				bSuccess = false;
			}


			if(bSuccessBoxA && bSuccessBoxB){
				strMsg = _T("票卡回收测试成功！");
				m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			}
			else if(bSuccessBoxA && !bSuccessBoxB){
				strMsg = _T("票箱A票卡回收测试失败！");
				m_Modol.menuInfo[9].strMenuStatus = FAILED;
			}
			else if(!bSuccessBoxA && bSuccessBoxB){
				strMsg = _T("票箱B票卡回收测试失败！");
				m_Modol.menuInfo[9].strMenuStatus = FAILED;
			}
			else{
				strMsg = _T("票卡回收测试失败！");
				m_Modol.menuInfo[9].strMenuStatus = FAILED;
			}

			theAPP_SESSION.ShowOfficeGuide(strMsg);
			
		}

		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_SUCCESS_NEW);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,true);
	}
	catch(CSysException& e)
	{
		bSuccess = false;
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
		theAPP_SESSION.ShowOfficeGuide(_T("发卡模块测试失败，请检查设备状态。"));
	}
	catch(...){
		bSuccess = false;
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		theAPP_SESSION.ShowOfficeGuide(_T("发卡模块测试失败，请检查设备状态。"));
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
	if (bIsStart)
	{
		try
		{
			//CARD_HELPER->EndMaintenanceModeCMD();
		}
		catch (CSysException& e)
		{
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			// Card模块测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
			theEXCEPTION_MGR.ProcessException(e);
		}
	}
	theSERVICE_MGR.SetForeServiceBusy(false);

	if(bSuccess)
		theAPP_SESSION.ShowOfficeGuide(_T("测试完成，请检查测试结果。"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      电机测试处理函数

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoMotorTestOperation()
{
	int index = theAPP_SESSION.GetInputToInt();
	int nMenuNum = 0;
	try
	{
		TH_MOTORTEST_DATA motor;
		switch (index)
		{
		case 1://停止当前电机
			nMenuNum = 0;
			isMotorStart = false;
			/*motor.Switch = 2;
			motor.testType = testType;
			//CARD_HELPER->MotorTest(motor);*/
			CARD_HELPER->TestSetting(CMD_TEST_STOP);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 2://1——票箱1 升降电机正转
			if (isMotorStart)
			{
//				motor.Switch = 2;
//				motor.testType = testType;
//				CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 1; /*motor.testType = testType =*/;
			CARD_HELPER->TestSetting(CMD_TEST_A_TRAY_UP);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 3:// 2——票箱1 升降电机反转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 2;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 2;
			CARD_HELPER->TestSetting(CMD_TEST_A_TRAY_DOWN);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 4:// 3——票箱2 升降电机正转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 3;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 3;
			CARD_HELPER->TestSetting(CMD_TEST_B_TRAY_UP);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 5:// 4——票箱2 升降电机反转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 4;
			//CARD_HELPER->MotorTest(motor);

			nMenuNum = 4;
			CARD_HELPER->TestSetting(CMD_TEST_B_TRAY_DOWN);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 6:// 5——票箱1 刮票电机正转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 5;
			//CARD_HELPER->MotorTest(motor);

			nMenuNum = 5;
			CARD_HELPER->TestSetting(CMD_TEST_TICKSCRUB_MOTOR_BOXA);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 7:// 6——票箱1 刮票电机反转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 6;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 6;
			CARD_HELPER->TestSetting(CMD_TEST_TICKSCRUB_MOTOR_BOXB);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 8:// 7——票箱2 刮票电机正转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 7;
			//CARD_HELPER->MotorTest(motor);
			nMenuNum = 7;
			CARD_HELPER->TestSetting(CMD_TEST_TRANSTICK_MOTOR);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
		case 9:// 8——票箱2 刮票电机反转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			//isMotorStart = true;
			//motor.Switch = 1;
			//nMenuNum = motor.testType = testType = 8;
			//CARD_HELPER->MotorTest(motor);

			nMenuNum = 8;
			CARD_HELPER->TestSetting(CMD_TEST_CHANNEL_CLEAN);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[9].strMenuStatus = SUCCESS;
			break;
/*		case 10:// 9——暂存电机正转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 9;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;
		case 11:// 10——暂存电机反转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 10;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;
		case 12:// 11——读卡区电机正转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 11;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;
		case 13:// 12——读卡区电机反转
			if (isMotorStart)
			{
				//motor.Switch = 2;
				//motor.testType = testType;
				//CARD_HELPER->MotorTest(motor);
				CARD_HELPER->TestSetting(CMD_TEST_STOP);
			}
			isMotorStart = true;
			motor.Switch = 1;
			nMenuNum = motor.testType = testType = 12;
//			CARD_HELPER->MotorTest(motor);
			m_Modol.menuInfo[nMenuNum].motorStatus = SUCCESS;
			m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
			break;*/
		default:
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			break;
		}
	}
	catch(CSysException& e)
	{
		m_Modol.menuInfo[nMenuNum].motorStatus = FAILED;
		m_Modol.menuInfo[7].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		m_Modol.menuInfo[nMenuNum].motorStatus = FAILED;
		m_Modol.menuInfo[7].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     读RFDI测试处理函数

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoReadRFIDTestOperation()
{
	int nMenuNum = 0;
	TH_BOX_TYPE boxType;
	tTIMRFIDInfo rfidInfo;
	int indexMenu = 5;
	try{
		nMenuNum = 0;
		boxType = TH_BOX_TYPE::TH_BOX_A;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = boxID.ToString();//.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0],res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}

	try{
		nMenuNum = 1;
		boxType = TH_BOX_TYPE::TH_BOX_B;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = boxID.ToString();//.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0],res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		//m_Modol.menuInfo[nMenuNum].read_rfidStatus.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}

	try{
		nMenuNum = 2;
		boxType = TH_BOX_TYPE::TH_BOX_C;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = boxID.ToString();//.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0],res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		//m_Modol.menuInfo[nMenuNum].read_rfidStatus.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;	
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].read_rfidStatus = FAILED;
		m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读RFID函数

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::ReadRFID()
{
	int nMenuNum = 0;
	try{
		TH_BOX_TYPE boxType;
		tTIMRFIDInfo rfidInfo;

		boxType = TH_BOX_TYPE::TH_BOX_A;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].writeID = boxID.ToString();
		m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),((rfidInfo.ucCardNum[1] << 8 ) | rfidInfo.ucCardNum[0]));
		//m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);

		nMenuNum = 1;
		boxType = TH_BOX_TYPE::TH_BOX_B;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		//CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].writeID = boxID.ToString();
		m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),((rfidInfo.ucCardNum[1] << 8 ) | rfidInfo.ucCardNum[0]));
		//m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);

		nMenuNum = 2;
		boxType = TH_BOX_TYPE::TH_BOX_C;
		CARD_HELPER->GetStaticBoxId(boxType,rfidInfo);
		//CTCardCountInfo::CARD_BOX_ID boxID;
		boxID.Deserialize(rfidInfo.ucBoxID);
		m_Modol.menuInfo[nMenuNum].writeID = boxID.ToString();
		m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),((rfidInfo.ucCardNum[1] << 8 ) | rfidInfo.ucCardNum[0]));
		//m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%02x%02x"),res.RFIDData.byDeviceID[0]
		//,res.RFIDData.byDeviceID[1],res.RFIDData.byDeviceID[2],res.RFIDData.byDeviceID[3]);
		/*nMenuNum = 3;
		res.boxType = RFID_MAGAZINE_A_B;
		CARD_HELPER->ReadTicketBoxRFID(res);
		m_Modol.menuInfo[nMenuNum].writeID.Format(_T("0x%02x%02x%04x"),res.RFIDData.byBoxID[0]
		,res.RFIDData.byBoxID[1],(res.RFIDData.byBoxID[2]<<8) + res.RFIDData.byBoxID[3]);*/
	}
	catch(CSysException& e){
		m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      写RFID函数

@param    none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSmardCardTestSvc::DoWriteRFIDTestOperation()
{
	//int index = theAPP_SESSION.GetInputToInt();
	//int nMenuNum = 0;
	////SC_CMD_SET_RFID cmd;
	//int indexMenu = 6;
	//SC_RES_SET_RFID res;
	//WORD wNum = 0;
	//WORD wInput = 0;
	//CString str;
	//try{
	//	switch(type_op)	{
	//	case NULL_OP: {
	//			switch (index){
	//			case 1://票箱A
	//				type_op = MenuA;
	//				theAPP_SESSION.SetPrefixToGuide(_opl(GUIDE_TH_TEST_TEXT_INPUT_ID) + m_Modol.strPreBoxID);
	//				theAPP_SESSION.SetMaxInputLength(4);
	//				break;
	//			case 2://票箱B
	//				type_op = MenuB;
	//				theAPP_SESSION.SetPrefixToGuide(_opl(GUIDE_TH_TEST_TEXT_INPUT_ID) + m_Modol.strPreBoxID);
	//				theAPP_SESSION.SetMaxInputLength(4);
	//				break;
	//			case 3:// 票箱C
	//				type_op = MenuC;
	//				theAPP_SESSION.SetPrefixToGuide(_opl(GUIDE_TH_TEST_TEXT_INPUT_ID) + m_Modol.strPreInvalidID);
	//				theAPP_SESSION.SetMaxInputLength(4);
	//				break;
	//			default:
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//				break;
	//			}
	//		}
	//		break;
	//	 // 输入票箱A的RFID后四位
	//	case MenuA:
	//		// 输入ID不合法
	//		if(index == 0){
	//			theAPP_SESSION.SetPrefixToGuide(_T("输入票箱ID不合法，请重新输入:") + m_Modol.strPreBoxID);
	//			break;
	//		}
	//		// 输入票箱ID后四位合法
	//		else{
	//			nMenuNum = 0;
	//			wInput = (index % 10000) & 0xFFFF;
	//			wNum = wInput;
	//			//int2BCD(wInput,(char*)&wNum,2);// BCD: 1234 -> 0x1234 | HEX: 1234 -> 0x3412
	//			m_cmd.magazine = RFID_MAGAZINE_A;
	//			int2BCD(theMAINTENANCE_INFO.GetLineCode(),(char*)m_rfidInfo.ucBoxID,1);	// BCD 1
	//			m_rfidInfo.ucBoxID[1] = 0x01;								// BCD 1
	//			m_rfidInfo.ucBoxID[2] = HIBYTE(wNum);						// HEX 2
	//			m_rfidInfo.ucBoxID[3] = LOBYTE(wNum);
	//			str.Format(_T("%02d01%04d"),theMAINTENANCE_INFO.GetLineCode(),wInput);
	//			//保证3种票箱的RFID不同
	//			if(str == m_Modol.menuInfo[1].writeID || str == m_Modol.menuInfo[2].writeID ){
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_ID_REPEAT);
	//				type_op = NULL_OP;
	//				break;
	//			}
	//			m_Modol.menuInfo[nMenuNum].writeID = str;
	//			type_op = NumA;
	//			theAPP_SESSION.SetPrefixToGuide(_T("请输入票箱A的数量:"));
	//			theAPP_SESSION.SetMaxInputLength(3);
	//		}			
	//		break;
	//	// 输入票箱B的RFID后四位
	//	case MenuB:
	//		// 输入ID不合法
	//		if(index == 0){
	//			theAPP_SESSION.SetPrefixToGuide(_T("输入票箱ID不合法，请重新输入:") + m_Modol.strPreBoxID);
	//			break;
	//		}
	//		else{
	//			nMenuNum = 1;
	//			wInput = (index % 10000) & 0xFFFF;
	//			wNum = wInput;
	//			//int2BCD(wInput,(char*)&wNum,2);// BCD: 1234 -> 0x1234 | HEX: 1234 -> 0x3412
	//			m_cmd.magazine = RFID_MAGAZINE_B;
	//			int2BCD(theMAINTENANCE_INFO.GetLineCode(),(char*)m_rfidInfo.ucBoxID,1);	// BCD 1
	//			m_rfidInfo.ucBoxID[1] = 0x01;								// BCD 1
	//			m_rfidInfo.ucBoxID[2] = HIBYTE(wNum);						// HEX 2
	//			m_rfidInfo.ucBoxID[3] = LOBYTE(wNum);
	//			str.Format(_T("%02d01%04d"),theMAINTENANCE_INFO.GetLineCode(),wInput);
	//			if(str == m_Modol.menuInfo[0].writeID || str == m_Modol.menuInfo[2].writeID ){
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_ID_REPEAT);
	//				type_op = NULL_OP;
	//				break;
	//			}
	//			m_Modol.menuInfo[nMenuNum].writeID = str;
	//			type_op = NumB;
	//			theAPP_SESSION.SetPrefixToGuide(_T("请输入票箱B的数量:"));
	//			theAPP_SESSION.SetMaxInputLength(3);				
	//		}
	//		break;
	//	// 输入废票箱的RFID后四位
	//	case MenuC:
	//		// ID合法性
	//		if(index == 0){
	//			theAPP_SESSION.SetPrefixToGuide(_T("输入票箱ID不合法，请重新输入:") + m_Modol.strPreInvalidID);
	//			break;
	//		}
	//		nMenuNum = 2;
	//		wInput = (index % 10000) & 0xFFFF;
	//		wNum = wInput;
	//		//int2BCD(wInput,(char*)&wNum,2);// BCD: 1234 -> 0x1234 | HEX: 1234 -> 0x3412
	//		TH_BOX_TYPE boxType;
	//		tTIMRFIDInfo rfidInfo;
	//		boxType = TH_BOX_TYPE::TH_BOX_C;
	//		int2BCD(theMAINTENANCE_INFO.GetLineCode(),(char*)rfidInfo.ucBoxID,1);	// BCD 1
	//		rfidInfo.ucBoxID[1] = 0x02;									// BCD 1
	//		rfidInfo.ucBoxID[2] = HIBYTE(wNum);							// HEX 2
	//		rfidInfo.ucBoxID[3] = LOBYTE(wNum);
	//		str.Format(_T("%02d02%04d"),theMAINTENANCE_INFO.GetLineCode(),wInput);
	//		if(str == m_Modol.menuInfo[0].writeID || str == m_Modol.menuInfo[1].writeID){
	//			theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_ID_REPEAT);
	//			type_op = NULL_OP;
	//			break;
	//		}
	//		// 数量为0
	//		memset(rfidInfo.ucCardNum,0x00,sizeof(rfidInfo.ucCardNum));			
	//		CARD_HELPER->WriteTicketBoxRFID(boxType,&rfidInfo);
	//		m_Modol.menuInfo[nMenuNum].writeID = str;
	//		m_Modol.menuInfo[nMenuNum].write_rfidStatus = SUCCESS;
	//		type_op = NULL_OP;
	//		m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_SUCCESS);
	//		break;
	//	// 输入票箱A的数量	
	//	case NumA:	
	//		{
	//			WORD wTickCount = (WORD)index;
	//			// 输入票箱A的数量
	//			if(wTickCount > 1000 || wTickCount <= 0){
	//				theAPP_SESSION.SetPrefixToGuide(_T("输入的数量不合法，请重新输入:"));
	//				break;
	//			}
	//			else{
	//				nMenuNum = 0;
	//				//m_cmd.RFIDData.sTicketCount = wTickCount;
	//				TH_BOX_TYPE boxType;
	//				boxType = TH_BOX_TYPE::TH_BOX_A;
	//				//tTIMRFIDInfo rfidInfo;
	//				memcpy(m_rfidInfo.ucCardNum,&wTickCount,2);
	//				CARD_HELPER->WriteTicketBoxRFID(boxType,&m_rfidInfo);
	//				//m_Modol.menuInfo[nMenuNum].writeID = str;
	//				m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),wTickCount);
	//				m_Modol.menuInfo[nMenuNum].write_rfidStatus = SUCCESS;
	//				type_op = NULL_OP;
	//				m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_SUCCESS);
	//			}			
	//			break;
	//		}
	//	case NumB:	
	//		{
	//			WORD wTickCount = (WORD)index;
	//			// 输入票箱B的数量
	//			if(wTickCount > 1000 || wTickCount <= 0){
	//				theAPP_SESSION.SetPrefixToGuide(_T("输入的数量不合法，请重新输入:"));
	//			}
	//			else{
	//				nMenuNum = 1;
	//				m_cmd.RFIDData.sTicketCount = wTickCount;
	//				TH_BOX_TYPE boxType;
	//				boxType = TH_BOX_TYPE::TH_BOX_B;
	//				//tTIMRFIDInfo rfidInfo;
	//				memcpy(m_rfidInfo.ucCardNum,&wTickCount,2);
	//				CARD_HELPER->WriteTicketBoxRFID(boxType,&m_rfidInfo);
	//				//m_Modol.menuInfo[nMenuNum].writeID = str;
	//				m_Modol.menuInfo[nMenuNum].writeCount.Format(_T("%d"),wTickCount);
	//				m_Modol.menuInfo[nMenuNum].write_rfidStatus = SUCCESS;
	//				type_op = NULL_OP;
	//				m_Modol.menuInfo[indexMenu].strMenuStatus = SUCCESS;
	//				theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_SUCCESS);
	//			}	
	//			break;
	//		}						
	//	default:
	//		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	//		break;
	//	}
	//}
	//catch(CSysException& e){
	//	m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
	//	m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_FAIL);
	//	// Card模块测试--操作日志
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	//	theEXCEPTION_MGR.ProcessException(e);
	//}
	//catch(...){
	//	m_Modol.menuInfo[nMenuNum].write_rfidStatus = FAILED;
	//	m_Modol.menuInfo[indexMenu].strMenuStatus = FAILED;
	//	theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_TEXT_WRITE_FAIL);
	//	// Card模块测试--操作日志
	//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	//}
}

void CSmardCardTestSvc::DoSensorTestOperation()
{
	try
	{
		//SC_RSP_SENSOR_SWITCH_TEST rsp;
		//CARD_HELPER->SensorTest(rsp);
		m_Modol.cardSenserStatus = _th_rsp_get_dev_status();
		CARD_HELPER->GetDevStatus(&m_Modol.cardSenserStatus);
		m_Modol.bCardSenserTestSuccess = true;
		m_Modol.menuInfo[8].strMenuStatus = SUCCESS;
		/*m_Modol.INFS1Covered = rsp.INFS1Covered;
		m_Modol.INFS2Covered = rsp.INFS2Covered;
		m_Modol.INFS3Covered = rsp.INFS3Covered;
		m_Modol.INFS4Covered = rsp.INFS4Covered;
		m_Modol.INFS5Covered = rsp.INFS5Covered;
		m_Modol.INFS6Covered = rsp.INFS6Covered;*/
	}
	catch(CSysException& e)
	{
		m_Modol.menuInfo[8].strMenuStatus = FAILED;
		m_Modol.bCardSenserTestSuccess = false;
		/*m_Modol.INFS1Covered = -1;
		m_Modol.INFS2Covered = -1;
		m_Modol.INFS3Covered = -1;
		m_Modol.INFS4Covered = -1;
		m_Modol.INFS5Covered = -1;
		m_Modol.INFS6Covered = -1;*/
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		m_Modol.bCardSenserTestSuccess = false;
		m_Modol.menuInfo[8].strMenuStatus = FAILED;
		/*m_Modol.INFS1Covered = -1;
		m_Modol.INFS2Covered = -1;
		m_Modol.INFS3Covered = -1;
		m_Modol.INFS4Covered = -1;
		m_Modol.INFS5Covered = -1;
		m_Modol.INFS6Covered = -1;*/
		//theAPP_SESSION.ShowOfficeGuide(GUIDE_TH_TEST_FAIL);
		// Card模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CARD_UNIT_TEST,false);
	}
}