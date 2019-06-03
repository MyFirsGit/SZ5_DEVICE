#include "stdafx.h"
#include "IOModuleTestSvc.h"
#include "IOModuleTestMaintainDlg.h"

// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_IO_TEST_MENU_TO_OPERATION = _T("WM_IO_TEST_MENU_TO_OPERATION");
static const TCHAR* const WM_IO_TEST_OPERATION_TO_MENU = _T("WM_IO_TEST_OPERATION_TO_MENU");

BEGIN_MESSAGE_MAP(CIOModuleTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_F3,OnKeyboardF3)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestSvc::CIOModuleTestSvc()
	:CTVMForeService(IO_MODULE_TEST_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20113_STATION_SET_END_DLG, new CIOModuleTestMaintainDlg(this));

	//设置IO测试画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_IO_TEST, _opl(GUIDE_ENTER_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_IO_TEST, IDD_20113_STATION_SET_END_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_IO_STATUS);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_IO_STATUS,IDD_20113_STATION_SET_END_DLG);

	//设置画面扭转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_IO_TEST,WM_IO_TEST_MENU_TO_OPERATION,DIALOG_GROUP_IO_STATUS);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_IO_STATUS,WM_IO_TEST_OPERATION_TO_MENU,DIALOG_GROUP_IO_TEST);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_IO_TEST);

	m_OperationType  = OPERATION_TYPE_UNKNOW;
	m_bIsStopTesting = false;
	m_hHandle        = NULL;

	DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// 获取设备类型
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestSvc::~CIOModuleTestSvc()
{
	//按下ESC后需要退出监测线程
	m_bIsStopTesting = true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      业务启动时设置第一个画面组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestSvc::OnStart(){
	__super::OnStart();
	// 初始化值
	m_OperationType  = OPERATION_TYPE_UNKNOW;
	m_bIsStopTesting = false;
	m_hHandle        = NULL;
	m_Modol = Modol();
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();			// 获取设备类型
	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_IO_TEST);
	if(theEXCEPTION_MGR.HasException(CIOException::MODULE_ID))
		theAPP_SESSION.ShowOfficeGuide(IO_TEST_TEXT_IO_ERROR_WARNING);
	else
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
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
LRESULT CIOModuleTestSvc::OnKeyboardEnter(WPARAM , LPARAM )
{
	int index = theAPP_SESSION.GetInputToInt();

	switch(index)
	{
	case MENU_CODE_1:
		{
			//Do it and set data
			m_OperationType = OPERATION_TYPE_STATUS;
		}
		break;
	//case MENU_CODE_2:
	//	{
	//		/*if(DEVICE_TYPE_AVM == DeviceType)
	//		{
	//		m_OperationType = OPERATION_TYPE_OPEN_ALARM;
	//		break;
	//		}
	//		m_OperationType = OPERATION_TYPE_CHANGE_INDICATOR;*/
	//		m_OperationType = OPERATION_TYPE_CHANGE_INDICATOR;
	//	}
	//	break;
	//case MENU_CODE_3:
	//	{
	//		/*if(DEVICE_TYPE_AVM == DeviceType)
	//		{
	//			m_OperationType = OPERATION_TYPE_CLOSE_ALARM;
	//			break;
	//		}
	//		m_OperationType = OPERATION_TYPE_CHANGE_LIGHT;*/
	//		m_OperationType = OPERATION_TYPE_CHANGE_LIGHT;
	//	}
	//	break;
	//case MENU_CODE_4:
	//	{
	//		m_OperationType = OPERATION_TYPE_CHARGE_INDICATOR;
	//	}
	//	break;
	////case MENU_CODE_5:
	////	{
	////		m_OperationType = OPERATION_TYPE_ALARM;
	////	}
	////	break;
	//case MENU_CODE_5:
	//	{
	//		m_OperationType = OPERATION_TYPE_CH_INDICATOR;
	//	}
	//	break;
	//case MENU_CODE_6:
	//	{
	//		m_OperationType = OPERATION_TYPE_RECEIPT_INDICATOR;
	//	}
	//	break;
	//case MENU_CODE_7:
	//	{
	//		m_OperationType = OPERATION_TYPE_BAFFLE;
	//	}
	//	break;
	//case MENU_CODE_8:
	//	{
	//		m_OperationType = OPERATION_TYPE_CARDFFLE;
	//	}
	//	break;
	case MENU_CODE_2:
		{
			m_OperationType = OPERATION_TYPE_OPEN_ALL;
		}
		break;
	case MENU_CODE_3:
		{
			m_OperationType = OPERATION_TYPE_CLOSE_ALL;
		}
		break;
	default:
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
		return FALSE;
	}

	//theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
	//根据选项，做相应的操作
	DoOperation();

	//更新界面显示
	NotifyDataModelChanged();
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
//LRESULT CIOModuleTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
//{
//	theAPP_SESSION.AppendOneNumToGuide(wParam);
//	return TRUE;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      停止IO监控

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIOModuleTestSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam){
	m_bIsStopTesting = true;
	__super::OnKeyboardEsc(wParam,lParam);
	if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
		try{
			IO_HELPER->CloseAll();
		}
		catch(CSysException& e){

		}
		catch(...){

		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      F3响应函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CIOModuleTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	if (GetCurDialogGroupID() == DIALOG_GROUP_IO_STATUS)
	{
		m_bIsStopTesting = true;
		DoDialogFlow(WM_IO_TEST_OPERATION_TO_MENU);
	}
	else
	{
		theAPP_SESSION.ReturnMaintainMenu();
	}
	NotifyDataModelChanged();

	if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
		try{
			IO_HELPER->CloseAll();
		}
		catch(CSysException& e){

		}
		catch(...){

		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取数据模型数据

@param      none

@retval     (o)m_Modol

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CIOModuleTestSvc::Modol& CIOModuleTestSvc::GetDataModol()
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
void CIOModuleTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      具体操作

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CIOModuleTestSvc::DoOperation()
{
	long errCode = 0;
	int nMenuNum = 0;
	int nTryNTimes = 3;
	//bool bIsComOpen = false;
	theSERVICE_MGR.SetForeServiceBusy(true);
	try
	{
		try{
			IO_HELPER->CloseIO();
			IO_HELPER->OpenIO();
			IO_HELPER->InitializeIO();
			IO_RSP_STATUS_INFO rsp;
			IO_HELPER->GetIOStatus(rsp);
		}
		catch(CSysException& e)
		{
			//Open failed
			theAPP_SESSION.ShowOfficeGuide(GUIDE_IO_OPEN_PORT_ERROR);
			theSERVICE_MGR.SetForeServiceBusy(false);

			// IO模块测试--操作日志
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_IO_UNIT_TEST,false);
			theEXCEPTION_MGR.ProcessException(e);
			return;
		}
		//1.IO状态测试
		if (m_OperationType == OPERATION_TYPE_STATUS)
		{
			DoDialogFlow(WM_IO_TEST_MENU_TO_OPERATION);

			nMenuNum = 0;
			//每次监测都需要初始化
			m_bIsStopTesting = false;
			//theSERVICE_MGR.SetForeServiceBusy(false);
			m_hHandle = CreateThread(NULL,0,BeginTestingIOStatus,this,0,&dwThreadID);
			if (m_hHandle != NULL)
			{
				m_Modol.menuInfo[nMenuNum].strMenuStatus = COMPLETED;

				theSERVICE_MGR.SetForeServiceBusy(false);
				CloseHandle(m_hHandle);
				return;
			}
			else
			{
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			}
		}
		////2.打开取票找零指示灯
		//else if (m_OperationType == OPERATION_TYPE_CHANGE_INDICATOR)
		//{
		//	nMenuNum = 1;
		//	IO_HELPER->OpenReturnCupIndicator();
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////3.打开取票找零照明灯
		//else if (m_OperationType == OPERATION_TYPE_CHANGE_LIGHT)
		//{
		//	nMenuNum = 2;
		//	IO_HELPER->OpenReturnLight(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////4.打开充值口指示灯
		//else if (m_OperationType == OPERATION_TYPE_CHARGE_INDICATOR)
		//{
		//	nMenuNum = 3;
		//	IO_HELPER->OpenChargeIndicator();
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////5.打开蜂鸣报警
		////else if (m_OperationType == OPERATION_TYPE_ALARM)
		////{
		////	nMenuNum = 4;
		////	IO_HELPER->OpenAlarm(); 
		////	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		////}
		////6.打开硬币投币口指示灯
		//else if (m_OperationType == OPERATION_TYPE_CH_INDICATOR)
		//{
		//	nMenuNum = 4;
		//	IO_HELPER->OpenCHIndicator(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////7.打开凭条口指示灯
		//else if (m_OperationType == OPERATION_TYPE_RECEIPT_INDICATOR)
		//{
		//	nMenuNum = 5;
		//	IO_HELPER->OpenReceiptIndicator(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////8.硬币电磁铁开（允许投币）
		//else if (m_OperationType == OPERATION_TYPE_BAFFLE)
		//{
		//	nMenuNum = 6;
		//	IO_HELPER->OpenCoinShutter(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		////9.打开储值卡电磁铁（允许插卡）
		//else if (m_OperationType == OPERATION_TYPE_CARDFFLE)
		//{
		//	nMenuNum = 7;
		//	IO_HELPER->OpenCardShutter(); 
		//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		//}
		//10.打开所有IO控制
		else if (m_OperationType == OPERATION_TYPE_OPEN_ALL)
		{
			nMenuNum = 1;
			IO_HELPER->OpenAll();
			m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		}
		//11.关闭所有IO控制
		else if (m_OperationType == OPERATION_TYPE_CLOSE_ALL)
		{
			nMenuNum = 2;
			IO_HELPER->CloseAll();
			m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
		}

		// IO模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_IO_UNIT_TEST,true);
	}
	catch(CSysException& e)
	{
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;

		// IO模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_IO_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}

	//闭串口
	//if (bIsComOpen)
	//{
	//	try
	//	{
	//		IO_HELPER->CloseIO();
	//	}
	//	catch(...)
	//	{
	//		//关闭错误
	//	}
	//}

	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);

	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      动态监测IO状态线程

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CIOModuleTestSvc::BeginTestingIOStatus(LPVOID lpParameter)
{
	CIOModuleTestSvc* pSvc = (CIOModuleTestSvc*)lpParameter;
	CIOModuleTestSvc::Modol& modol = pSvc->GetDataModol();
	theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);//??
	while(!pSvc->m_bIsStopTesting)
	{
		try
		{
			IO_RSP_STATUS_INFO rsp;
			IO_HELPER->GetIOStatus(rsp);
			//设置状态-如果成功
			modol.IO_Status[0].bIsThisActive = !rsp.isFrontDoorOpen;
			modol.IO_Status[1].bIsThisActive = !rsp.isBackDoorOpen;
			modol.IO_Status[2].bIsThisActive = !rsp.isLeftPersonActive;
			modol.IO_Status[3].bIsThisActive = !rsp.isBanknoteModeReady;
			modol.IO_Status[4].bIsThisActive = !rsp.isTokenInvBoxReady;
			modol.IO_Status[5].bIsThisActive = !rsp.isTokenColBoxReady;

			//更新数据
			pSvc->NotifyDataModelChanged();
			Sleep(500);//500毫秒检测一次
		}
		catch(CSysException& e)
		{
			theEXCEPTION_MGR.ProcessException(e);
			//有异常直接跳出循环，线程结束
			break;
		}
	}

	//关闭串口连接
	//pSvc->IO_HELPER->CloseIO();
	return 0;
}
