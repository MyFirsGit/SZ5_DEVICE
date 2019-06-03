#include "stdafx.h"
#include "BanknoteAcceptModuleTestSvc.h"
#include "BanknoteAcceptMaintainDlg.h"
#include "Service.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; 
#endif

BEGIN_MESSAGE_MAP(CBanknoteAcceptModuleTestSvc,CTVMForeService)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_NUMBER,OnKeyboardNumber)
	//ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
	//ON_COMMAND_COMPLETE(BANKNOTE_MODULE_COMMAND_ID_INSERT,OnBanknoteInsertedEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteAcceptModuleTestSvc::CBanknoteAcceptModuleTestSvc()
	:CTVMForeService(BANKNOTE_ACCEPT_MODULE_TEST_SVC)
{
	//设置操作员画面
	GetDialogFlow()->AddDialog(IDD_20113_STATION_SET_END_DLG,new CBanknoteAcceptMaintainDlg(this));

	//设置画面组
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_BANKNOTE,_opl(GUIDE_ENTER_F3_ESC));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_BANKNOTE,CBanknoteAcceptMaintainDlg::IDD);

	//设置初始画面
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_BANKNOTE);

	m_bIsBanknotesInserted = false;
	/*bhRspType.bIsBanknote1Yuan = TRUE;
	bhRspType.bIsBanknote5Yuan = TRUE;
	bhRspType.bIsBanknote10Yuan = TRUE;
	bhRspType.bIsBanknote20Yuan = TRUE;
	bhRspType.bIsBanknote50Yuan = TRUE;
	bhRspType.bIsBanknote100Yuan = TRUE;*/

	m_bIsBanknotesInserted = false;
	m_bIsReceiving = false;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBanknoteAcceptModuleTestSvc::~CBanknoteAcceptModuleTestSvc(){
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      业务启动后需要初始化的数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteAcceptModuleTestSvc::OnStart(){
	/*bhRspType.bIsBanknote1Yuan = TRUE;
	bhRspType.bIsBanknote5Yuan = TRUE;
	bhRspType.bIsBanknote10Yuan = TRUE;
	bhRspType.bIsBanknote20Yuan = TRUE;
	bhRspType.bIsBanknote50Yuan = TRUE;
	bhRspType.bIsBanknote100Yuan = TRUE;*/
	m_bIsBanknotesInserted = false;
	m_bIsReceiving = false;
	m_Modol = Modol();
	//设置初始画面
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_BANKNOTE);

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
LRESULT CBanknoteAcceptModuleTestSvc::OnKeyboardEnter(WPARAM , LPARAM ){

	try
	{
		long errCode = 0;
		//BRH_STATUS bhstatus;lichao
		//errCode = BANKNOTE_HELPER->GetStatus(bhstatus);
		int index  = theAPP_SESSION.GetInputToInt();

		// 非超级用户只响应第一个菜单
		if(index != MENU_CODE_1 && !theAPP_SESSION.IsSuperUser() && !theFunction_INFO.IsFounderOperation()){
			theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
			return FALSE;
		}

		switch(index)
		{
		case MENU_CODE_1:
			{
				//1.重置
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_RESET;
			}
			break;
		case MENU_CODE_2:
			{
				//2.开始接收
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_START_IDENTIFY;
			}
			break;
		case MENU_CODE_3:
			{
				//3.停止接收
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_STOP_IDENTIFY;
			}
			break;
		case MENU_CODE_4:
			{
				//4.退币
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_RETURN_CASH;
			}
			break;
		/*case MENU_CODE_32:
			{
				m_OperationType = OPERATION_TYPE::OPERATION_TYPE_RETURN_FROM_BOTH;
			}
			break;*/
		//case MENU_CODE_4:
		//	{
		//		//4.压入暂存
		//		m_OperationType = OPERATION_TYPE::OPERATION_TYPE_ACCEPT;
		//	}
		//	break;
		//case MENU_CODE_5:
		//	{
		//		//5.收纳纸币
		//		m_OperationType = OPERATION_TYPE::OPERATION_TYPE_COLLECT;
		//	}
		//	break;
		default:
			{
				theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
				return TRUE;
			}
			break;
		}
		//执行操作
		DoOperation();

		//执行完成后显示执行结果
		NotifyDataModelChanged();

		return TRUE;
	}
	catch (CSysException& e) {
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_IO_OPEN_PORT_ERROR));
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_IO_OPEN_PORT_ERROR));
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}


}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteAcceptModuleTestSvc::OnKeyboardF3(WPARAM , LPARAM )
{
	LONG errCode = 0;
	try
	{ 
		if (m_bIsReceiving){
			errCode = BANKNOTE_HELPER->StopValidation();
			if (errCode == 0){
				m_bIsReceiving = false;
			}		
		}

		if (m_bIsBanknotesInserted){
			errCode = BANKNOTE_HELPER->ReturnAll(true);
			if (errCode == 0){
				m_bIsBanknotesInserted = false;
			}
		}

		theAPP_SESSION.ReturnMaintainMenu();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBanknoteAcceptModuleTestSvc::OnKeyboardEsc(WPARAM wParam, LPARAM lParam){

	LONG errCode = 0;
	try
	{
		if (m_bIsReceiving){
			errCode = BANKNOTE_HELPER->StopValidation();
			if (errCode == 0){
				m_bIsReceiving = false;
			}		
		}

		if (m_bIsBanknotesInserted){
			errCode = BANKNOTE_HELPER->ReturnAll(true);
			if (errCode == 0){
				m_bIsBanknotesInserted = false;
			}
		}

		__super::OnKeyboardEsc(0,0);

	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      刷新菜单界面显示

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteAcceptModuleTestSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
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
//LRESULT CBanknoteAcceptModuleTestSvc::OnKeyboardNumber(WPARAM wParam, LPARAM )
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
CBanknoteAcceptModuleTestSvc::Modol& CBanknoteAcceptModuleTestSvc::GetDataModol()
{
	return m_Modol;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      执行操作，返回操作结果

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBanknoteAcceptModuleTestSvc::DoOperation()
{
	int nMenuNum = 0;
	long errCode = 0;

	// 如果正在接收纸币，当前只能取消
	if(m_bIsReceiving){
		if (m_OperationType != OPERATION_TYPE::OPERATION_TYPE_STOP_IDENTIFY){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RECOVERY_STOP));
			return;
		}
	}
	// 如果已经收入纸币，当前只能压箱或者退钞
	if(m_bIsBanknotesInserted){
		if (m_OperationType != OPERATION_TYPE::OPERATION_TYPE_ACCEPT && m_OperationType != OPERATION_TYPE::OPERATION_TYPE_RETURN_CASH){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RECOVERY_BACK_NOTES));
			return;
		}
	}
	theAPP_SESSION.ShowOfficeGuide(GUIDE_BANKNOTE_TEST_TESTING);
	theSERVICE_MGR.SetForeServiceBusy(true);

	try
	{
		if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_RESET){
			nMenuNum = 0;
			errCode = BANKNOTE_HELPER->Reset();
			if (errCode == 0){
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
			}
			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
		else if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_START_IDENTIFY){
			nMenuNum = 1;
			//开始识别纸币
			m_bIsReceiving = true;
			//errCode = BANKNOTE_HELPER->OpenAndValidate(bhRspType);
			tBncSetInhibitList bhAcceptTable;
			bhAcceptTable.iNumOfItems = 6;
			bhAcceptTable.psBncCashInfo = new tBncSetInhibitItem[bhAcceptTable.iNumOfItems];
			bhAcceptTable.psBncCashInfo[0].iValue = 100;
			bhAcceptTable.psBncCashInfo[0].bAccepted= true;
			bhAcceptTable.psBncCashInfo[1].iValue = 500;
			bhAcceptTable.psBncCashInfo[1].bAccepted= true;
			bhAcceptTable.psBncCashInfo[2].iValue = 1000;
			bhAcceptTable.psBncCashInfo[2].bAccepted= true;
			bhAcceptTable.psBncCashInfo[3].iValue = 2000;
			bhAcceptTable.psBncCashInfo[3].bAccepted= true;
			bhAcceptTable.psBncCashInfo[4].iValue = 5000;
			bhAcceptTable.psBncCashInfo[4].bAccepted= true;
			bhAcceptTable.psBncCashInfo[5].iValue = 10000;
			bhAcceptTable.psBncCashInfo[5].bAccepted= true;

			errCode = 0;
			BANKNOTE_HELPER->OpenAndValidateAsync(this,bhAcceptTable);

			if (bhAcceptTable.psBncCashInfo)
			{
				delete[] bhAcceptTable.psBncCashInfo;
				bhAcceptTable.psBncCashInfo = NULL;
			}
			if(errCode == 0){
				//m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				//m_Modol.bh_face = bhRspType.bh_faceValue;
				//m_bIsBanknotesInserted = true;
				//theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RECOVERY_SUCCESS));
			}
			else{
				m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RECOVERY_FAILED));
			}
			m_bIsReceiving = false;
		}
		else if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_STOP_IDENTIFY){
			nMenuNum = 2;
			errCode = BANKNOTE_HELPER->StopValidation();
			if (errCode == 0){
				m_bIsReceiving = false;
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
				theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
			}
			else{
				theAPP_SESSION.ShowOfficeGuide(_T("停止接收失败！。"));
			}
		}
		else if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_RETURN_CASH)
		{
			nMenuNum = 3;
			errCode = BANKNOTE_HELPER->ReturnAll(true);
			if (errCode == 0){
				m_bIsBanknotesInserted = false;
				m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
			}
			theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		}
		else if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_ACCEPT)
		{
			nMenuNum = 4;
			//errCode = BANKNOTE_HELPER->Collect();
			//if (errCode == 0){
			//	m_Modol.menuInfo[nMenuNum].strMenuStatus = SUCCESS;
			//}
			//BANKNOTE_HELPER->Abort();
		}
		else if (m_OperationType == OPERATION_TYPE::OPERATION_TYPE_CHANGE)
		{
			nMenuNum = 5;
		}
	}
	catch(CSysException& e){
		//异常
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		theEXCEPTION_MGR.ProcessException(e);
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,true);
	}
	catch(...)
	{
		//异常
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		theAPP_SESSION.ShowOfficeGuide(GUIDE_ENTER_F3_ESC);
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
	}


	theSERVICE_MGR.SetForeServiceBusy(false);
}

void CBanknoteAcceptModuleTestSvc::OnBanknoteInserted(UINT resultCode,CBRHOpenAndValidate* bhReadReceiveAmount)
{
	int nMenuNum = 0;
	LONG errCode = 0;
	BRH_STATUS bhRsp;
	try
	{
		if (resultCode==0){
			tBncCashInfo feedback;
			bhReadReceiveAmount->GetResponse(&feedback);
			m_Modol.bh_face = (BankNoteAndCoinType_t)feedback.psBncCashInfo[0].iValue;
			if(feedback.iNumOfItems==0){
				m_bIsBanknotesInserted = false;
			}
			else{
				m_bIsBanknotesInserted = true;
			}
		}
		else{
			m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_BANKNOTE_RECOVERY_FAILED));
		}

		NotifyDataModelChanged();
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,true);
		return ;
	}
	catch (CSysException& e) {
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		m_Modol.menuInfo[nMenuNum].strMenuStatus = FAILED;
		// 纸币模块测试--操作日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_BANKNOTE_UNIT_TEST,false);
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}


}