#include "StdAfx.h"
#include "tvmStatusMgr.h"
#include "Service.h"
#include "scapi.h"
#include "ServiceMgr.h"
#include "ServiceDefinition.h"
#include "scmessagemakehelper.h"
#include "scdatamsg.h"
#include "headermanager.h"
#include "businesstranslatesvc.h"
#include "serverclientsvc.h"
#include "tvmdef.h"
#include "MainSvc.h"
#include "RechargeRWMonitorSvc.h"

//#include "StatusDisplayModuleHelper.h"
//#include "ReceptionHeadDlg.h"

#define GUI_INI_FILE_NAME		    _T("TVM_SETTING.INI")
#define RUN_MODE_SETTINGS			_T("RUN_MODE_SETTINGS")
#define SERVICE_MODE				_T("SERVICE_MODE")
#define PAYMENT_MODE				_T("PAYMENT_MODE")	
#define CHANGE_MODE					_T("CHANGE_MODE")
#define PRINT_MODE					_T("PRINT_MODE")


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param    none

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusHelper::CStatusHelper()
	:m_sServiceStatus(OUTOF_SERVICE)									// 服务状态
	,m_sRunStatus(RUNING)												// 运行状态
	,m_sWorkMode(DEVICE_CONTRL_CODE_BUSINESS_ALL)						// 工作模式
	,m_OperationMode(NORMAL_SERVICE_MODE)								// 运营模式
	,m_s24HourMode(OPERATION_24HOURS_OFF)								// 24小时模式
	,m_sDelayTime(0)													// 延长运营时间
	,m_sConnectStatus(DISCONNECT)										// 连接状态
	,m_sTimeStatus(VALID_TIME)											// 时钟差异
	,m_sLoginStatus(LOGOUT)												// 登录状态
	,m_sAutorunStatus(AUTORUN_START)									// 设备自动运行状态
	,m_lDeviceErrStatus(0)												// 设备故障状态
	,m_sTransferAudit(TRANSFER_AUDIT_STATUS_NORMAL)						// 传输审计状态
	//,m_sPaymentMode(DEVICE_CONTRL_CODE_PAYMENT)						// 支付模式 
	,m_sPrintMode(0)													// 打印模式
	//,m_sChangeMode(DEVICE_CONTRL_CODE_ALL_CHANGE)						// 找零模式
	,m_sDiskSpaceStatus(DISK_SPACE_NORMAL)								// 磁盘空间状态
	,m_sTokenBox1Status(STATUS_MAGAZINE_SET)							// Token补充箱1安装状态
	,m_sTokenBox1NumStatus(MAG_NUM_NORMAL) 								// Token补充箱1数量状态（暂不使用）
	,m_sTokenBox2Status(STATUS_MAGAZINE_SET)							// Token补充箱2安装状态
	,m_sTokenBox2NumStatus(MAG_NUM_NORMAL) 								// Token补充箱2数量状态（暂不使用）
	,m_sTokenHopper1Status(STATUS_MAGAZINE_SET)							// Token Hopper箱1安装状态（暂不使用）
	,m_sTokenHopper1NumStatus(MAG_NUM_NORMAL) 							// Token Hopper箱1数量状态
	,m_sTokenHopper2Status(STATUS_MAGAZINE_SET)							// Token Hopper箱2安装状态（暂不使用）
	,m_sTokenHopper2NumStatus(MAG_NUM_NORMAL) 							// Token Hopper箱2数量状态
	,m_sTokenWasteBoxStatus(STATUS_MAGAZINE_SET)						// Token废票箱安装状态
	,m_sTokenWasteBoxNumStatus(MAG_NUM_NORMAL) 							// Token废票箱数量状态
	,m_sCardBox1Status(STATUS_MAGAZINE_SET)								// Card箱1安装状态
	,m_sCardBox1NumStatus(MAG_NUM_NORMAL) 								// Card箱1数量状态
	,m_sCardBox2Status(STATUS_MAGAZINE_SET)								// Card箱2安装状态
	,m_sCardBox2NumStatus(MAG_NUM_NORMAL) 								// Card箱2数量状态
	,m_sCardWasteBoxStatus(STATUS_MAGAZINE_SET)							// Card废票箱安装状态
	,m_sCardWasteBoxNumStatus(MAG_NUM_NORMAL) 							// Card废票箱数量状态
	,m_sCoinBox1Status(STATUS_MAGAZINE_SET)								// Coin补充箱1安装状态
	,m_sCoinBox1NumStatus(MAG_NUM_NORMAL) 								// Coin补充箱1数量状态
	,m_sCoinBox2Status(STATUS_MAGAZINE_SET)								// Coin补充箱2安装状态
	,m_sCoinBox2NumStatus(MAG_NUM_NORMAL) 								// Coin补充箱2数量状态
	,m_sCoinHopper1Status(STATUS_MAGAZINE_SET)							// Coin Hopper箱1安装状态（暂不使用）
	,m_sCoinHopper1NumStatus(MAG_NUM_NORMAL) 							// Coin Hopper箱1数量状态
	,m_sCoinHopper2Status(STATUS_MAGAZINE_SET)							// Coin Hopper箱2安装状态（暂不使用）
	,m_sCoinHopper2NumStatus(MAG_NUM_NORMAL) 							// Coin Hopper箱2数量状态
	,m_sCoinCircleHopperStatus(STATUS_MAGAZINE_SET)						// Coin 循环找零Hopper箱安装状态（暂不使用）
	,m_sCoinCircleHopperNumStatus(MAG_NUM_NORMAL) 						// Coin 循环找零Hopper箱数量状态
	,m_sCoinCollectionBox1Status(STATUS_MAGAZINE_SET)					// Coin回收箱1安装状态
	,m_sCoinCollectionBox1NumStatus(MAG_NUM_NORMAL) 					// Coin回收箱1数量状态
	,m_sCoinCollectionBox2Status(STATUS_MAGAZINE_SET)					// Coin回收箱2安装状态
	,m_sCoinCollectionBox2NumStatus(MAG_NUM_NORMAL) 					// Coin回收箱2数量状态
	,m_sBanknoteCollectionBoxStatus(STATUS_MAGAZINE_SET)				// 纸币回收箱安装状态
	,m_sBanknoteCollectionBoxNumStatus(MAG_NUM_NORMAL) 					// 纸币回收箱数量状态
	,m_sSecurityDoorStatus(SECURITY_DOOR_STATUS_NORMAL_CLOSE)			// 现金安全门状态（暂不使用）
	,m_sTouchScreenStatus	(TOUCH_SCREEN_STATUS_NORMAL)				// 触摸屏状态（暂不使用）
	,m_sAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS_UNPRESSED)		// 招援按钮状态
	,m_sRechargeRWStatus(RW_NORMAL)										// 充值读写器状态
	,m_sCardRWStatus(RW_NORMAL)											// Card读写器状态
	,m_sTokenRWStatus(RW_NORMAL)										// Token读写器状态
	,m_sSAM1Status(ISAM_UNAUTH)											// SAM1状态(充值读写器一票通 ISAM)
	,m_sSAM2Status(ISAM_UNAUTH)											// SAM2状态(Card读写器一票通 ISAM)
	,m_sTokenRWYPTISAMStatus(SAM_STATUS_LRT_NORMAL)						// SAM3状态(Token读写器一票通 ISAM)
	,m_sRechargeRWYKTISAMStatus(SAM_STATUS_MACAUPASS_NORMAL)			// SAM4状态(充值读写器一卡通ISAM)(暂不使用)
	,m_sPrintStatus(PRINTER_NORMAL)										// 打印机状态
	,m_sUPSStatus(UPS_STATUS_CLOSE)										// UPS状态
	,m_sMaintenanceDoorStatus(DOOR_NORMAL_CLOSE)						// 维修门状态
	,m_sBanknoteModuleStatus(HARDWARE_MODULE_NORMAL)					// 纸币模块状态
	,m_sTokenModuleStatus(HARDWARE_MODULE_NORMAL)						// Token模块状态
	,m_sCardModuleStatus(HARDWARE_MODULE_NORMAL)						// Card模块状态
	,m_sCoinModuleStatus(HARDWARE_MODULE_NORMAL)						// 硬币模块状态
	,m_sStatusDisplayStatus(HARDWARE_MODULE_NORMAL)						// 状态显示器状态
	,m_sTiltStatus(TILT_STATUS_OFF)										// 倾斜传感器状态(震动传感器状态)
	,m_sOffLineDays(0)													// 离线天数
	,m_sAuthorizedAccessStatus(AUTHORIZED_STATUS_NORMAL)				// 授权访问状态 
	,m_sDegradedMode(DEVICE_CONTRL_CODE_SC_DE_OPER_ON)					// 自动降级运行开关
	,m_sBanknoteChangeBox1NumStatus(MAG_NUM_NORMAL)						// 纸币找零箱A数量状态
	,m_sBanknoteChangeBox2NumStatus(MAG_NUM_NORMAL)						// 纸币找零箱B数量状态

	,m_sBanknoteRE1NumStatus(MAG_NUM_NORMAL)							// 纸币循环鼓1数量状态
	,m_sBanknoteRE2NumStatus(MAG_NUM_NORMAL)							// 纸币循环鼓2数量状态
	,m_sBanknoteRE3NumStatus(MAG_NUM_NORMAL)							// 纸币循环鼓3数量状态
	,m_sBanknoteRE4NumStatus(MAG_NUM_NORMAL)							// 纸币循环鼓4数量状态
{
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CStatusHelper::OnExceptionItemNotify);

	//TVMWorkChanged.AddHandler(&theSTATUS_DISPLAYER,&CStatusDisplayModuleHelper::showWorkMode);
	//TVMChargeChanged.AddHandler(&theSTATUS_DISPLAYER,&CStatusDisplayModuleHelper::showChargeMode);
	//TVMPaymentChanged.AddHandler(&theSTATUS_DISPLAYER,&CStatusDisplayModuleHelper::showPaymentMode);
	//TVMStatusChanged.AddHandler(this,&CReceptionHeadDlg::OnWorkModeChanged);

	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath, _MAX_PATH);
	CString fileName = CString(szAppPath) + GUI_INI_FILE_NAME;
	if (TRUE == ComCheckFileExist(fileName)) {
		SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);
		//pINI = new CGetIni(fileName);
		m_sWorkMode = theTVM_SETTING.GetServiceMode()/*pINI->GetDataInt(RUN_MODE_SETTINGS, SERVICE_MODE)*/;
		m_sPaymentMode = theTVM_SETTING.GetPayMentMode()/*pINI->GetDataInt(RUN_MODE_SETTINGS, PAYMENT_MODE )*/;
		m_sPrintMode = theTVM_SETTING.GetPrintMode()/*pINI->GetDataInt(RUN_MODE_SETTINGS, CHANGE_MODE  )*/;
		m_sChangeMode = theTVM_SETTING.GetChangeMode()/*pINI->GetDataInt(RUN_MODE_SETTINGS, PRINT_MODE )*/;
	}else{
		m_sWorkMode = 0;
		m_sPaymentMode = DEVICE_CONTRL_CODE_PAYMENT;
		m_sPrintMode = 0;
		m_sChangeMode = DEVICE_CONTRL_CODE_ALL_CHANGE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param    none

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusHelper::~CStatusHelper()
{
	theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CStatusHelper::OnExceptionItemNotify);
	
	//TVMWorkChanged.RemoveHandler(&theSTATUS_DISPLAYER,&CStatusDisplayModuleHelper::showWorkMode);
	//TVMChargeChanged.RemoveHandler(&theSTATUS_DISPLAYER,&CStatusDisplayModuleHelper::showChargeMode);
	//TVMPaymentChanged.RemoveHandler(&theSTATUS_DISPLAYER,&CStatusDisplayModuleHelper::showPaymentMode);
	//TVMStatusChanged.RemoveHandler(this,&CReceptionHeadDlg::OnWorkModeChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取该类的对象

@param   none

@retval     CStatusHelper&    CStatusHelper实例对象

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStatusHelper& CStatusHelper::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置服务状态 

@param   (i) SERVICE_STATUS status 服务状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetServiceStatus(SERVICE_STATUS status)
{
	try{
		// 停止服务只能切换到服务中
		if(m_sServiceStatus == STOP_SERVICE && status != IN_SERVICE){
			theSTATUS_DISPLAYER.showTVMStatus(STOP_SERVICE);
			theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show();
			return;
		}
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected()){
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::SERVICE_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}

		if(m_sServiceStatus != status){//状态不一致时才设置，通讯时间过长
			try{
				m_sServiceStatus = status;
				theSTATUS_DISPLAYER.showWorkMode();
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			//return;
		}
		else{
			//m_sServiceStatus = status;

			//try{
			//	theSTATUS_DISPLAYER.showWorkMode();
			//}
			//catch(CSysException& e){
			//	theEXCEPTION_MGR.ProcessException(e);
			//}
		}
			
			
		// 设备重要状态变更业务数据（澳门预留）
		//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgImportantStatusChange(m_sServiceStatus);

		// 条屏显示：暂停服务和停止服务
		//if (OUTOF_SERVICE == status && STOP_SERVICE != m_sServiceStatus || STOP_SERVICE == status){
		//	try{
		//		theSTATUS_DISPLAYER.showTVMStatus(status);
		//	}
		//	catch(CSysException& e){
		//		theEXCEPTION_MGR.ProcessException(e);
		//	}
		//}
		//else if(MANT_SERVICE == status && m_sServiceStatus != STOP_SERVICE){// 维护模式要显示暂停服务
		//	try{
		//		theSTATUS_DISPLAYER.showTVMStatus(OUTOF_SERVICE);
		//	}
		//	catch(CSysException& e){
		//		theEXCEPTION_MGR.ProcessException(e);
		//	}
		//}
		
		if(IN_SERVICE != status)
			theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取服务状态

@param    none

@retval     SERVICE_STATUS   当前服务状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATUS CStatusHelper::GetServiceStatus()
{
	return m_sServiceStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置运行状态

@param    (i)RUN_STATUS status 运行状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetRunStatus(RUN_STATUS status)
{
	try{
		if(m_sRunStatus == status) return;

		m_sRunStatus = status;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RUN_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// 显示正在关机...
		if(SHUTDOWNING == m_sRunStatus || START_RUNING == m_sRunStatus)theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取运行状态

@param   无

@retval     RUN_STATUS 当前运行状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
RUN_STATUS CStatusHelper::GetRunStatus()
{
	return m_sRunStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置工作模式

@param   (i)WORD_MODE 工作模式
@param   (i)bool bManual 是否是手动设置

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetWorkMode(WORD mode, bool bManual)
{
	try{
		
		// 上位下发的状态，如果无人操作，直接启动默认服务判断状态
		if(bManual){
			if(!theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->IsServiceBusyOnPassger()){
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
			}
			return;
		}
		
		if (m_sWorkMode != mode)
		{
			m_sWorkMode = mode;
			// 如果SC已连接则发生设备状态变更数据
			if(IsSCConnected())	{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::WORK_MODE,GetDegreeWorkMode());
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
			}
			if ((mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE)
			{
				theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
			}

			// 更新状态显示器显示内容及界面显示内容
			theSTATUS_DISPLAYER.showWorkMode();

		}
		//m_sWorkMode = mode;
		//// 记录操作日志
		///*OPERATION_CODE operCode;
		//if(bManual == true){
		//operCode = OPERATION_MODE_SET;
		//}
		//else{
		//operCode = OPERATION_MODE_NOTIC;
		//}
		//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operCode);*/

		//// 更新状态显示器显示内容及界面显示内容
		//theSTATUS_DISPLAYER.showWorkMode();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			获取工作模式

@param		none

@retval			BYTE 工作模式代码

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetWorkMode()
{
	return m_sWorkMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置运营模式

@param   (i)OperationalMode_t   运营模式

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetOperationMode(OperationalMode_t mode)
{
	try{
		if(m_OperationMode == mode)
			return;
		if (mode != NORMAL_SERVICE_MODE && mode != EMERGENCY_MODE && mode != TRAIN_SERVICE_DISRUPTION_MODE)
		{
			//其它模式只反馈状态，TVM不做响应
			// 如果SC已连接则发生设备状态变更数据
			if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::OPERATION_MODE,LOBYTE(mode));// 取低位
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
				m_OperationMode = mode;
			}
			return;
		}
		m_OperationMode = mode;
		// 更新界面显示的运营模式
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_NEW_OPERATION_MODE,mode,NULL);
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::OPERATION_MODE,LOBYTE(mode));// 取低位
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// 设置TPU当前运营模式		
		RW_CMD_SET_MODE rw_cmd_set_mode;
		if(NORMAL_SERVICE_MODE == mode)
			rw_cmd_set_mode.ControlCode = 0x02;// 解除模式
		else 
			rw_cmd_set_mode.ControlCode = 0x01;// 设置模式
		rw_cmd_set_mode.ModeCode = mode;
		RW_RSP_STATUS rw_rsp_status;

		CARDRW_HELPER->SetOperationMode(rw_cmd_set_mode,rw_rsp_status);

		//TOKENRW_HELPER->SetOperationMode(rw_cmd_set_mode,rw_rsp_status);

		//RECHARGERW_HELPER->SetOperationMode(rw_cmd_set_mode,rw_rsp_status);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取运营模式

@param   none

@retval     OperationalMode_t 当前运营模式

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
OperationalMode_t CStatusHelper::GetOperationMode()
{
	return m_OperationMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置24小时运营模式

@param    (i)OPERATION_24HOURS_MODE b24HourMode 24小时运营模式

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::Set24HourMode(OPERATION_24HOURS_MODE b24HourMode)
{
	try{
		m_s24HourMode = b24HourMode;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::HOUR24MODE,(BYTE)b24HourMode);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// 设置TPU 24小时运营模式
		RW_CMD_SET_TIME rw_cmd_set_time;
		rw_cmd_set_time.ControlCode = b24HourMode == OPERATION_24HOURS_OFF? 0x02 : 0x01;
		rw_cmd_set_time.Time = 0;
		RW_RSP_STATUS rw_rsp_status;
		CARDRW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);
///		TOKENRW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);
//		RECHARGERW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);

		// 保存操作日志
		CString strOperComment = (b24HourMode == OPERATION_24HOURS_OFF) ?_T("24 HOUR OFF"):_T("24 HOUR ON");
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_24HOUR_DELAY,true,strOperComment);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取24小时运营模式

@param    none

@retval     OPERATION_24HOURS_MODE 24小时运营模式状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
OPERATION_24HOURS_MODE CStatusHelper::Get24HourMode()
{
	return m_s24HourMode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置延长运营时间

@param    (i)OPERATION_DELAY_MODE delayTime  延长运营时间状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDelayTime(WORD delayTime)
{
	try{
		m_sDelayTime = delayTime;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DELAY_RUN_MODE,(delayTime==0)?0x00:0x01);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// 设置TPU 延长运营模式状态
		RW_CMD_SET_TIME rw_cmd_set_time;
		if(delayTime == 0){
			rw_cmd_set_time.ControlCode = 0x02;
			rw_cmd_set_time.Time = 0;
		}
		else{
			rw_cmd_set_time.ControlCode = 0x10;
			rw_cmd_set_time.Time = delayTime * 60;
		}
		
		RW_RSP_STATUS rw_rsp_status;
		CARDRW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);
		// 保存操作日志
		CString strOperComment = (delayTime == 0) ?_T("延长运营关"):_T("延长运营开");
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_24HOUR_DELAY,true,strOperComment);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     取得延长运营时间

@param   none

@retval     OPERATION_DELAY_MODE 延长运营时间状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetDelayTime()
{
	return m_sDelayTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置通信状态

@param   (i)COMMUNICATION comm

@retval    none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCommunicate(COMMUNICATION comm)
{
	try{
		if (comm != m_sConnectStatus){
			m_sConnectStatus = comm;
			// 如果SC已连接则发生设备状态变更数据
			if(IsSCConnected())	{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CONNECT_STATUS,(BYTE)comm);
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
			}
			// 保存操作日志
			CString strOperComment =_T("");
			switch(comm){
				case DISCONNECT:
					strOperComment = _T("OFF LINE"); 
					break;
				case CONNECTED:
					strOperComment = _T("ON LINE"); 
					break;
				case CONNECTING:
					strOperComment = _T("CONNECTING"); 
					break;
				default:
					strOperComment = _T("UNKNOW"); 
					break;
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_CONNECT_STATUS,true,strOperComment);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取通信状态

@param   无

@retval     COMMUNICATION 当前通信状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COMMUNICATION CStatusHelper::GetCommunicate()
{
	return m_sConnectStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         设置时钟同步状态

@param      TIME_STATUS

@retval        none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTimeSynStatus(TIME_STATUS status)
{
	try{
		m_sTimeStatus = status;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TIME_DELAY,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}		
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取时钟同步状态

@param   无

@retval     TIME_STATUS 时钟同步状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TIME_STATUS CStatusHelper::GetTimeSynStatus()
{
	return m_sTimeStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置登录状态

@param    (i)LOGIN_STATUS status 登录状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetLoginStatus(LOGIN_STATUS status)
{
	try{
		if(m_sLoginStatus == status) return;

		m_sLoginStatus = status;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::LOGIN_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// 保存操作日志
		OPERATION_CODE operCode = (m_sLoginStatus== LOGIN)?OPERATION_LOGIN:OPERATION_LOGOUT;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operCode,true);	
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取登录状态

@param   无

@retval     LOGIN_STATUS 登录状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LOGIN_STATUS CStatusHelper::GetLoginStatus()
{
	return m_sLoginStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			设置自动运行状态

@param      (i)DEVICE_AUTORUN 自动运行状态

@retval       none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void  CStatusHelper::SetAutoRunStatus(DEVICE_AUTORUN AutorunStatus)
{
	try{
		if(m_sAutorunStatus == AutorunStatus) return;

		m_sAutorunStatus = AutorunStatus;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DEVICE_AUTO_RUN_STATUS,(BYTE)AutorunStatus);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取自动运行状态

@param   无

@retval     DEVICE_AUTORUN 自动运行状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_AUTORUN CStatusHelper::GetAutoRunStatus()
{
	return theFunction_INFO.IsEquipmentAutomaticRunningOn()?AUTORUN_START:AUTORUN_STOP/*m_sAutorunStatus*/;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置设备故障状态

@param   (i)long lMtcCode 设备故障状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDeviceErrStatus(long lMtcCode)
{
	try{
		m_lDeviceErrStatus = lMtcCode;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeErrorStatusCommand(lMtcCode);//MakeStatusChangeCommand(TVM_STATUS_IDS::DEVICE_ERR_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取设备故障状态

@param      无

@retval     long 设备故障状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CStatusHelper::GetDeviceErrStatus()
{
	return m_lDeviceErrStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置设备传输审计状态

@param      TRANSFER_AUDIT_STATUS status_transfer_audit

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS status_transfer_audit){
	try{
		if(m_sTransferAudit == status_transfer_audit) return;

		m_sTransferAudit = status_transfer_audit;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DEVICE_TRANSFER_AUDIT,(BYTE)status_transfer_audit);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取传输审计设备状态

@param      无

@retval     TRANSFER_AUDIT_STATUS m_sTransferAudit;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TRANSFER_AUDIT_STATUS CStatusHelper::GetDeviceTransferAudit(){
	return m_sTransferAudit;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置降级运行模式

@param      无

@retval			none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDegradedMode(DEVICE_CONTRL_CODE degradedMode)
{
		m_sDegradedMode = degradedMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			是否允许自动降级运行

@param      none

@retval			bool true:允许自动降级  false:不允许自动降级

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusHelper::IsCanAutoDegraded()
{
	return (m_sDegradedMode == DEVICE_CONTRL_CODE_SC_DE_OPER_ON);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			设置支付模式

@param       (i)WORD paymentMode 支付模式代码,bool bIsDeviceControl;是否为服务器

@retval			none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetPaymentMode(WORD paymentMode,bool bIsDeviceControl)
{
	try{
		if(m_sPaymentMode == paymentMode) return;

		m_sPaymentMode = paymentMode;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected() && bIsDeviceControl){
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::WORK_MODE,GetDegreeWorkMode());
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		
		theSTATUS_DISPLAYER.showPaymentMode();
		TVMPaymentChanged.Invoke();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         获取支付模式

@param      无

@retval       WORD 支付模式代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetPaymentMode()
{
	return m_sPaymentMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			设置打印模式

@param       (i)WORD printMode 打印模式代码

@retval			none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetPrintMode(WORD printMode,bool bIsDeviceControl)
{
	try{
		if(m_sPrintMode == printMode)return;

		m_sPrintMode = printMode;

 		// 如果SC已连接则发生设备状态变更数据
 		if(IsSCConnected() && bIsDeviceControl){
			BYTE btPrintMode = printMode == DEVICE_CONTRL_CODE_SC_NO_PRINT ? 0x00 : 0x01;
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::PRINTER_STATUS,btPrintMode);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
 		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取打印机模式（是否支持打印）

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetPrintMode()
{
	return m_sPrintMode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief         是否是无打印模式

@param      无

@retval       bool true:无打印  false:正常打印

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusHelper::IsNoPrintMode()
{
	return (m_sPrintMode == DEVICE_CONTRL_CODE_SC_NO_PRINT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			设置找零模式

@param       (i)WORD changeMode 找零模式代码

@retval			none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetChangeMode(WORD changeMode,bool bIsDeviceControl)
{
	try{
		if(m_sChangeMode == changeMode)return;

		m_sChangeMode = changeMode;
		// 如果SC已连接则发生设备状态变更数据
		if(IsSCConnected() && bIsDeviceControl){
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::WORK_MODE,GetDegreeWorkMode());
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}

		theSTATUS_DISPLAYER.showPaymentMode();
		TVMPaymentChanged.Invoke();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			获取找零模式

@param       none

@retval			WORD 找零模式代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetChangeMode()
{
	return m_sChangeMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置磁盘空间状态

@param    (i)DISK_SPACE_CODE DiskSpaceStatus

@retval     LOGIN_STATUS

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDiskSpaceStatus(DISK_SPACE_CODE DiskSpaceStatus)
{
	m_sDiskSpaceStatus = DiskSpaceStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		//CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DISC_SPACE_STATUS,(BYTE)DiskSpaceStatus);
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取磁盘空间状态

@param      无

@retval       DISK_SPACE_CODE 磁盘空间状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DISK_SPACE_CODE CStatusHelper::GetDiskSpaceStatus()
{
	return m_sDiskSpaceStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token补充箱1箱安装状态

@param    (i)STATUS_MAGAZINE TokenBox1Status

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox1Status(STATUS_MAGAZINE TokenBox1Status)
{
	m_sTokenBox1Status = TokenBox1Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX1_STATUS,(BYTE)TokenBox1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token补充箱1安装状态

@param      无

@retval       STATUS_MAGAZINE Token补充箱1安装状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenAddBox1Status()
{
	return m_sTokenBox1Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token补充箱1箱数量状态（暂不使用）

@param    (i)MAGAZINE_NUM_STATUS TokenBox1NumStatus Token补充箱1数量状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS TokenBox1NumStatus)
{
	m_sTokenBox1NumStatus = TokenBox1NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX1_COUNT,(BYTE)TokenBox1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token补充箱1数量状态（暂不使用）

@param      无

@retval       MAGAZINE_NUM_STATUS Token补充箱1数量状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenAddBox1NumStatus()
{
	return m_sTokenBox1NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token补充箱2安装状态

@param    (i)STATUS_MAGAZINE TokenBox2Status Token补充箱2安装状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox2Status(STATUS_MAGAZINE TokenBox2Status)
{
	m_sTokenBox2Status = TokenBox2Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX2_STATUS,(BYTE)TokenBox2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token补充箱2箱安装状态

@param      无

@retval       STATUS_MAGAZINE Token补充箱2安装状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenAddBox2Status()					
{
	return m_sTokenBox2Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token补充箱2数量状态（暂不使用）

@param    (i)MAGAZINE_NUM_STATUS TokenBox2NumStatus Token补充箱2数量状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS TokenBox2NumStatus)
{
	m_sTokenBox2NumStatus = TokenBox2NumStatus ;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX2_COUNT,(BYTE)TokenBox2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token补充箱2数量状态（暂不使用）

@param      无

@retval       MAGAZINE_NUM_STATUS Token补充箱2数量状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenAddBox2NumStatus()
{
	return m_sTokenBox2NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token Hopper1箱安装状态（暂不使用）

@param    (i)STATUS_MAGAZINE TokenHopper1Status Token Hopper1箱安装状态

@retval     LOGIN_STATUS

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper1Status(STATUS_MAGAZINE TokenHopper1Status)
{
	m_sTokenHopper1Status = TokenHopper1Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER1_STATUS,(BYTE)TokenHopper1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief         获取Token Hopper1箱安装状态（暂不使用）

@param      无

@retval       STATUS_MAGAZINE Token Hopper1箱安装状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenHopper1Status()
{
	return m_sTokenHopper1Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token Hopper1箱数量状态

@param    (i)MAGAZINE_NUM_STATUS TokenHopper1NumStatus Token Hopper1箱数量状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS TokenHopper1NumStatus) 
{
	m_sTokenHopper1NumStatus = TokenHopper1NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER1_COUNT,(BYTE)TokenHopper1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token Hopper1箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Token Hopper1箱数量状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenHopper1NumStatus()
{
	return m_sTokenHopper1NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token Hopper2箱安装状态（暂不使用）

@param    (i)STATUS_MAGAZINE TokenHopper2Status Token Hopper2箱安装状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper2Status(STATUS_MAGAZINE TokenHopper2Status)
{
	m_sTokenHopper2Status = TokenHopper2Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER2_STATUS,(BYTE)TokenHopper2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token Hopper2箱安装状态（暂不使用）

@param      无

@retval       STATUS_MAGAZINE Token Hopper2箱安装状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenHopper2Status()
{
	return m_sTokenHopper2Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token Hopper2箱数量状态

@param    (i)MAGAZINE_NUM_STATUS TokenHopper2NumStatus Token Hopper2箱数量状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS TokenHopper2NumStatus)
{
	m_sTokenHopper2NumStatus = TokenHopper2NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER2_COUNT,(BYTE)TokenHopper2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token Hopper2箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Token Hopper2箱数量状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenHopper2NumStatus()
{
	return m_sTokenHopper2NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token废票箱安装状态

@param    (i)STATUS_MAGAZINE TokenWasteBoxStatus Token废票箱安装状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenWasteBoxStatus(STATUS_MAGAZINE TokenWasteBoxStatus)
{
	m_sTokenWasteBoxStatus = TokenWasteBoxStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_CYCLEBOX_STATUS,(BYTE)TokenWasteBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token废票箱安装状态

@param      无

@retval       STATUS_MAGAZINE Token废票箱安装状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenWasteBoxStatus()
{
	return m_sTokenWasteBoxStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token废票箱数量状态

@param    (i)MAGAZINE_NUM_STATUS TokenWasteBoxNumStatus Token废票箱数量状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS TokenWasteBoxNumStatus)
{
	m_sTokenWasteBoxNumStatus = TokenWasteBoxNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_CYCLEBOX_COUNT,(BYTE)TokenWasteBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token废票箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Token废票箱数量状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenWasteBoxNumStatus()
{
	return m_sTokenWasteBoxNumStatus;
}


////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token回收箱安装状态

@param    (i)STATUS_MAGAZINE TokenCollectedBoxStatus Token回收箱安装状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenCollectedBoxStatus(STATUS_MAGAZINE TokenCollectedBoxStatus)
{
	m_sTokenCollectedBoxStatus = TokenCollectedBoxStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_COLLBOX_STATUS,(BYTE)TokenCollectedBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token回收箱安装状态

@param      无

@retval       STATUS_MAGAZINE Token回收箱安装状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenCollectedBoxStatus()
{
	return m_sTokenCollectedBoxStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token回收箱数量状态

@param    (i)MAGAZINE_NUM_STATUS TokenCollectedBoxNumStatus Token回收箱数量状态

@retval     none

@exception  无
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenCollectedBoxNumStatus(MAGAZINE_NUM_STATUS TokenCollectedBoxNumStatus)
{
	m_sTokenCollectedBoxNumStatus = TokenCollectedBoxNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_COLLEBOX_COUNT,(BYTE)TokenCollectedBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token回收箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Token回收箱数量状态

@exception  无
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenCollectedBoxNumStatus()
{
	return m_sTokenCollectedBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card箱1安装状态

@param    (i)STATUS_MAGAZINE CardBox1Status Card箱1安装状态

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox1Status(STATUS_MAGAZINE CardBox1Status)
{
	// 状态相同不再上报
	if(m_sCardBox1Status == CardBox1Status)return;

	m_sCardBox1Status = CardBox1Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected()){
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX1_STATUS,(BYTE)CardBox1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Card箱1箱安装状态

@param      无

@retval       STATUS_MAGAZINE Card箱1箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCardBox1Status()
{
	return m_sCardBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card箱1数量状态

@param    (i)MAGAZINE_NUM_STATUS CardBox1NumStatus Card箱1数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox1NumStatus(MAGAZINE_NUM_STATUS CardBox1NumStatus)
{
	// 状态相同不需要再次上报状态
	if(m_sCardBox1NumStatus == CardBox1NumStatus) return;

	m_sCardBox1NumStatus = CardBox1NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX1_COUNT,(BYTE)CardBox1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Card箱1箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Card箱1箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCardBox1NumStatus()
{
	return m_sCardBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card箱2箱安装状态

@param    (i)STATUS_MAGAZINE CardBox2Status Card箱2箱安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox2Status(STATUS_MAGAZINE CardBox2Status)
{
	if(m_sCardBox2Status == CardBox2Status)return;

	m_sCardBox2Status = CardBox2Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX2_STATUS,(BYTE)CardBox2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Card箱2箱安装状态（暂不使用）

@param      无

@retval       STATUS_MAGAZINE Card箱2箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCardBox2Status()					
{
	return m_sCardBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card箱2箱数量状态

@param    (i)MAGAZINE_NUM_STATUS CardBox2NumStatus Card箱2箱数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox2NumStatus(MAGAZINE_NUM_STATUS CardBox2NumStatus)
{
	if(m_sCardBox2NumStatus == CardBox2NumStatus ) return;

	m_sCardBox2NumStatus = CardBox2NumStatus ;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX2_COUNT,(BYTE)CardBox2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Card箱2箱数量状态（暂不使用）

@param      无

@retval       MAGAZINE_NUM_STATUS Card箱2箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCardBox2NumStatus()
{
	return m_sCardBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card回收箱安装状态

@param    (i)STATUS_MAGAZINE CardWasteBoxStatus Card回收箱安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardWasteBoxStatus(STATUS_MAGAZINE CardWasteBoxStatus)
{
	if(m_sCardWasteBoxStatus == CardWasteBoxStatus)return;
	m_sCardWasteBoxStatus = CardWasteBoxStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_CYCLEBOX_STATUS,(BYTE)CardWasteBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Card回收箱安装状态

@param      无

@retval       STATUS_MAGAZINE Card回收箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCardWasteBoxStatus()
{
	return m_sCardWasteBoxStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card回收箱数量状态

@param    (i)MAGAZINE_NUM_STATUS CardWasteBoxNumStatus Card回收箱数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS CardWasteBoxNumStatus)
{
	if(m_sCardWasteBoxNumStatus == CardWasteBoxNumStatus)return;

	m_sCardWasteBoxNumStatus = CardWasteBoxNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_CYCLEBOX_COUNT,(BYTE)CardWasteBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Card回收箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Card回收箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCardWasteBoxNumStatus()
{
	return m_sCardWasteBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin补充箱1箱安装状态

@param    (i)STATUS_MAGAZINE CoinBox1Status

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox1Status(STATUS_MAGAZINE CoinBox1Status)
{
	if(m_sCoinBox1Status == CoinBox1Status) return;
	m_sCoinBox1Status = CoinBox1Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX1_STATUS,(BYTE)CoinBox1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin补充箱1安装状态

@param      无

@retval       STATUS_MAGAZINE Coin补充箱1安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinAddBox1Status()
{
	return m_sCoinBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin补充箱1箱数量状态（暂不使用）

@param    (i)MAGAZINE_NUM_STATUS CoinBox1NumStatus Coin补充箱1数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS CoinBox1NumStatus)
{
	if(m_sCoinBox1NumStatus == CoinBox1NumStatus)return;

	m_sCoinBox1NumStatus = CoinBox1NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX1_COUNT,(BYTE)CoinBox1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin补充箱1数量状态（暂不使用）

@param      无

@retval       MAGAZINE_NUM_STATUS Coin补充箱1数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinAddBox1NumStatus()
{
	return m_sCoinBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin补充箱2安装状态

@param    (i)STATUS_MAGAZINE CoinBox2Status Coin补充箱2安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox2Status(STATUS_MAGAZINE CoinBox2Status)
{
	if(m_sCoinBox2Status == CoinBox2Status)return;

	m_sCoinBox2Status = CoinBox2Status;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX2_STATUS,(BYTE)CoinBox2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin补充箱2箱安装状态

@param      无

@retval       STATUS_MAGAZINE Coin补充箱2安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinAddBox2Status()					
{
	return m_sCoinBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin补充箱2数量状态（暂不使用）

@param    (i)MAGAZINE_NUM_STATUS CoinBox2NumStatus Coin补充箱2数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS CoinBox2NumStatus)
{
	if(m_sCoinBox2NumStatus == CoinBox2NumStatus)return;

	m_sCoinBox2NumStatus = CoinBox2NumStatus ;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX2_COUNT,(BYTE)CoinBox2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin补充箱2数量状态（暂不使用）

@param      无

@retval       MAGAZINE_NUM_STATUS Coin补充箱2数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinAddBox2NumStatus()
{
	return m_sCoinBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin Hopper1箱安装状态（暂不使用）

@param    (i)STATUS_MAGAZINE CoinHopper1Status Coin Hopper1箱安装状态

@retval     LOGIN_STATUS

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper1Status(STATUS_MAGAZINE CoinHopper1Status)
{
	m_sCoinHopper1Status = CoinHopper1Status;
	// 如果SC已连接则发生设备状态变更数据
	/*if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX1_STATUS,(BYTE)CoinHopper1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         获取Coin Hopper1箱安装状态（暂不使用）

@param      无

@retval       STATUS_MAGAZINE Coin Hopper1箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinHopper1Status()
{
	return m_sCoinHopper1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin Hopper1箱数量状态

@param    (i)MAGAZINE_NUM_STATUS CoinHopper1NumStatus Coin Hopper1箱数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS CoinHopper1NumStatus) 
{
	if(m_sCoinHopper1NumStatus == CoinHopper1NumStatus)return;

	m_sCoinHopper1NumStatus = CoinHopper1NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX1_COUNT,(BYTE)CoinHopper1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin Hopper1箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Coin Hopper1箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinHopper1NumStatus()
{
	return m_sCoinHopper1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin Hopper2箱安装状态（暂不使用）

@param    (i)STATUS_MAGAZINE CoinHopper2Status Coin Hopper2箱安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper2Status(STATUS_MAGAZINE CoinHopper2Status)
{
	m_sCoinHopper2Status = CoinHopper2Status;
	// 如果SC已连接则发生设备状态变更数据
	/*if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX2_STATUS,(BYTE)CoinHopper2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin Hopper2箱安装状态（暂不使用）

@param      无

@retval       STATUS_MAGAZINE Coin Hopper2箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinHopper2Status()
{
	return m_sCoinHopper2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin Hopper2箱数量状态

@param    (i)MAGAZINE_NUM_STATUS CoinHopper2NumStatus Coin Hopper2箱数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS CoinHopper2NumStatus)
{
	if(m_sCoinHopper2NumStatus == CoinHopper2NumStatus)return;

	m_sCoinHopper2NumStatus = CoinHopper2NumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX2_COUNT,(BYTE)CoinHopper2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin Hopper2箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Coin Hopper2箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinHopper2NumStatus()
{
	return m_sCoinHopper2NumStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置循环找零Coin Hopper箱安装状态（暂不使用）

@param    (i)STATUS_MAGAZINE CoinHopperStatus 循环找零Coin Hopper箱安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCircleHopperStatus(STATUS_MAGAZINE CoinHopperStatus)
{
	m_sCoinCircleHopperStatus = CoinHopperStatus;
	// 如果SC已连接则发生设备状态变更数据
	/*if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_HOPPER_BOX_STATUS,(BYTE)CoinHopperStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取循环找零Coin Hopper箱安装状态（暂不使用）

@param      无

@retval       STATUS_MAGAZINE Coin Hopper2箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinCircleHopperStatus()
{
	return m_sCoinCircleHopperStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置循环找零Coin Hopper箱数量状态

@param    (i)MAGAZINE_NUM_STATUS CoinHopper2NumStatus Coin Hopper2箱数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCircleHopperNumStatus(MAGAZINE_NUM_STATUS CoinHopperNumStatus)
{
	if(m_sCoinCircleHopperNumStatus == CoinHopperNumStatus)return;

	m_sCoinCircleHopperNumStatus = CoinHopperNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX2_COUNT,(BYTE)CoinHopperNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取循环找零Coin Hopper箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Coin Hopper2箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinCircleHopperNumStatus()
{
	return m_sCoinCircleHopperNumStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin回收箱1安装状态

@param    (i)STATUS_MAGAZINE CoinCollectionBoxStatus Coin回收箱1安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox1Status(STATUS_MAGAZINE CoinCollectionBoxStatus)
{
	if(m_sCoinCollectionBox1Status == CoinCollectionBoxStatus)return;

	m_sCoinCollectionBox1Status = CoinCollectionBoxStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX1_STATUS,(BYTE)CoinCollectionBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin回收箱1安装状态

@param      无

@retval       STATUS_MAGAZINE Coin回收箱1安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinCollectionBox1Status()
{
	return m_sCoinCollectionBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin回收箱1数量状态

@param    (i)MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus Coin回收箱1数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus)
{
	if(m_sCoinCollectionBox1NumStatus == CoinCollectionBoxNumStatus)return;

	m_sCoinCollectionBox1NumStatus = CoinCollectionBoxNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX1_COUNT,(BYTE)CoinCollectionBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin回收箱1数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Coin回收箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinCollectionBox1NumStatus()
{
	return m_sCoinCollectionBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin回收箱2安装状态

@param    (i)STATUS_MAGAZINE CoinCollectionBoxStatus Coin回收箱2安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox2Status(STATUS_MAGAZINE CoinCollectionBoxStatus)
{
	if(m_sCoinCollectionBox2Status = CoinCollectionBoxStatus)return;

	m_sCoinCollectionBox2Status = CoinCollectionBoxStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX2_STATUS,(BYTE)CoinCollectionBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin回收箱2安装状态

@param      无

@retval       STATUS_MAGAZINE Coin回收箱2安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinCollectionBox2Status()
{
	return m_sCoinCollectionBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Coin回收箱2数量状态

@param    (i)MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus Coin回收箱2数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox2NumStatus(MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus)
{
	if(m_sCoinCollectionBox2NumStatus == CoinCollectionBoxNumStatus)return;

	m_sCoinCollectionBox2NumStatus = CoinCollectionBoxNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX2_COUNT,(BYTE)CoinCollectionBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Coin回收箱2数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Coin回收箱2数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinCollectionBox2NumStatus()
{
	return m_sCoinCollectionBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零箱1安装状态

@param      STATUS_MAGAZINE status (纸币找零箱安转状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteChangeBox1Status(STATUS_MAGAZINE status){
	
	if(m_sBanknoteChangeBox1Status == status)return;

	m_sBanknoteChangeBox1Status = status;
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX1_STATUS,(BYTE)status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零箱1安装状态

@param      无

@retval     STATUS_MAGAZINE m_sBanknoteChangeBox1Status (安装状态)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetBanknoteChangeBox1Status(){
	return m_sBanknoteChangeBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零箱1数量状态

@param      MAGAZINE_NUM_STATUS numStatus(纸币箱数量状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS numStatus){
	if(m_sBanknoteChangeBox1NumStatus == numStatus)return;

	m_sBanknoteChangeBox1NumStatus = numStatus;
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX1_COUNT,(BYTE)numStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零箱1数量状态

@param      无

@retval     MAGAZINE_NUM_STATUS m_sBanknoteChangeBox1NumStatus;(纸币找零箱1数量状态)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteChangeBox1NumStatus(){
	return m_sBanknoteChangeBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零箱2安装状态

@param      STATUS_MAGAZINE status (纸币找零箱按转状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteChangeBox2Status(STATUS_MAGAZINE status){
	if(m_sBanknoteChangeBox2Status == status)return;

	m_sBanknoteChangeBox2Status = status;
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX2_STATUS,(BYTE)status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零箱2安装状态

@param      无

@retval     STATUS_MAGAZINE m_sBanknoteChangeBox2Status (安装状态)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetBanknoteChangeBox2Status(){
	return m_sBanknoteChangeBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零箱2数量状态

@param      MAGAZINE_NUM_STATUS numStatus(纸币箱数量状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS numStatus){
	if(m_sBanknoteChangeBox2NumStatus == numStatus) return;

	m_sBanknoteChangeBox2NumStatus = numStatus;
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX2_COUNT,(BYTE)numStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零箱2数量状态

@param      无

@retval     MAGAZINE_NUM_STATUS 纸币找零箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteChangeBox2NumStatus(){
	return m_sBanknoteChangeBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币循环鼓1数量状态

@param      MAGAZINE_NUM_STATUS numStatus(纸币箱数量状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE1NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币循环鼓1数量状态

@param      无

@retval     MAGAZINE_NUM_STATUS 纸币找零箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE1NumStatus(){
	return m_sBanknoteRE1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币循环鼓2数量状态

@param      MAGAZINE_NUM_STATUS numStatus(纸币箱数量状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE2NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币循环鼓2数量状态

@param      无

@retval     MAGAZINE_NUM_STATUS 纸币找零箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE2NumStatus(){
	return m_sBanknoteRE2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币循环鼓3数量状态

@param      MAGAZINE_NUM_STATUS numStatus(纸币箱数量状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE3NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币循环鼓3数量状态

@param      无

@retval     MAGAZINE_NUM_STATUS 纸币找零箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE3NumStatus(){
	return m_sBanknoteRE3NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币循环鼓4数量状态

@param      MAGAZINE_NUM_STATUS numStatus(纸币箱数量状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE4NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币循环鼓4数量状态

@param      无

@retval     MAGAZINE_NUM_STATUS 纸币找零箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE4NumStatus(){
	return m_sBanknoteRE4NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Banknote回收箱安装状态

@param    (i)STATUS_MAGAZINE BanknoteCollectionBoxStatus Banknote回收箱安装状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE BanknoteCollectionBoxStatus)
{
	if(m_sBanknoteCollectionBoxStatus == BanknoteCollectionBoxStatus) return;

	m_sBanknoteCollectionBoxStatus = BanknoteCollectionBoxStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_STATUS,(BYTE)BanknoteCollectionBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Banknote回收箱安装状态

@param      无

@retval       STATUS_MAGAZINE Banknote回收箱安装状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetBanknoteCollectionBoxStatus()
{
	return m_sBanknoteCollectionBoxStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Banknote回收箱数量状态

@param    (i)MAGAZINE_NUM_STATUS BanknoteCollectionBoxNumStatus Banknote回收箱数量状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS BanknoteCollectionBoxNumStatus)
{
	if(m_sBanknoteCollectionBoxNumStatus == BanknoteCollectionBoxNumStatus) return;

	m_sBanknoteCollectionBoxNumStatus = BanknoteCollectionBoxNumStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_COUNT,(BYTE)BanknoteCollectionBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Banknote回收箱数量状态

@param      无

@retval       MAGAZINE_NUM_STATUS Banknote回收箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteCollectionBoxNumStatus()
{
	return m_sBanknoteCollectionBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置现金安全门状态（暂不使用）

@param    (i)SECURITY_DOOR_STATUS 现金安全门状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetSecurityDoorStatus(SECURITY_DOOR_STATUS SecurityDoorStatus)
{
	if(m_sSecurityDoorStatus == SecurityDoorStatus)return;

	m_sSecurityDoorStatus = SecurityDoorStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::SECURITY_DOOR_STATUS,(BYTE)SecurityDoorStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取现金安全门状态（暂不使用）

@param      无

@retval       SECURITY_DOOR_STATUS  现金安全门状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SECURITY_DOOR_STATUS CStatusHelper::GetSecurityDoorStatus()
{
	return m_sSecurityDoorStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置触摸屏状态（暂不使用）

@param    (i)TOUCH_SCREEN_STATUS 触摸屏状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTouchDisplayStatus(TOUCH_SCREEN_STATUS TouchScreenStatus)
{
	if(m_sTouchScreenStatus == TouchScreenStatus) return;

	m_sTouchScreenStatus = TouchScreenStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOUCH_SCREEN_STATUS,(BYTE)TouchScreenStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取触摸屏状态（暂不使用）

@param      无

@retval       TOUCH_SCREEN_STATUS 触摸屏状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TOUCH_SCREEN_STATUS CStatusHelper::GetTouchDisplayStatus()
{
	return m_sTouchScreenStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置招援按钮状态

@param    (i)ASSISTANCE_BUTTON_STATUS 招援按钮状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS AssistanceButtonStatus)
{
	//if(m_sAssistanceButtonStatus == AssistanceButtonStatus)return;

	//m_sAssistanceButtonStatus = AssistanceButtonStatus;
	//// 如果SC已连接则发生设备状态变更数据
	//if(IsSCConnected())	{
	//	CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::HELP_BUTTON_STATUS,(BYTE)AssistanceButtonStatus);
	//	theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取招援按钮状态

@param      无

@retval       MAGAZINE_NUM_STATUS Banknote 回收箱数量状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
ASSISTANCE_BUTTON_STATUS CStatusHelper::GetAssistanceButtonStatus()
{
	return m_sAssistanceButtonStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置充值RW状态

@param    (i)STATUS_RW 充值RW状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetRW1Status(STATUS_RW RechargeRWStatus)
{
	if(m_sCardRWStatus == RechargeRWStatus)return;

	m_sCardRWStatus = RechargeRWStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW1_STATUS,(BYTE)RechargeRWStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
	// 充值读写器设备监控管理
	//if(theTVM_INFO.IsRWMonitorOpened(TVM_RECHARGE_RW)){
	//	if(RechargeRWStatus == RW_NORMAL){
	//		theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();
	//	}
	//	else{
	//		theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Stop();
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取充值RW状态

@param      无

@retval       STATUS_RW 充值RW状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CStatusHelper::GetRW1Status()									
{
	return m_sCardRWStatus;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      设置充值RW状态

@param    (i)STATUS_RW 充值RW状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetRW2Status(STATUS_RW RechargeRWStatus)
{
	if(m_sRechargeRWStatus == RechargeRWStatus)return;

	m_sRechargeRWStatus = RechargeRWStatus;
	// 如果SC已连接则发生设备状态变更数据
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW2_STATUS,(BYTE)RechargeRWStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
	// 充值读写器设备监控管理
	//if(theTVM_INFO.IsRWMonitorOpened(TVM_RECHARGE_RW)){
	//	if(RechargeRWStatus == RW_NORMAL){
	//		theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Resume();
	//	}
	//	else{
	//		theSERVICE_MGR.GetService<CRechargeRWMonitorSvc>(MONITOR_RECHARGE_RW_SVC)->Stop();
	//	}
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取充值RW状态

@param      无

@retval       STATUS_RW 充值RW状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CStatusHelper::GetRW2Status()									
{
	return m_sRechargeRWStatus;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card RW状态

@param    (i)STATUS_RW Card RW状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetCardRWStatus(STATUS_RW CardRWStatus)
// {
// 	m_sCardRWStatus = CardRWStatus;
// 	// 如果SC已连接则发生设备状态变更数据
// 	if(IsSCConnected())	{
// 		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_RW_STATUS,(BYTE)CardRWStatus);
// 		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
// 	}
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief         获取Card RW状态

@param      无

@retval       STATUS_RW Card RW状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// STATUS_RW CStatusHelper::GetCardRWStatus()											
// {
// 	return m_sCardRWStatus;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token RW状态

@param    (i)STATUS_RW Token RW状态

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetTokenRWStatus(STATUS_RW TokenRWStatus)
// {
// 	m_sTokenRWStatus = TokenRWStatus;
// 	// 如果SC已连接则发生设备状态变更数据
// 	if(IsSCConnected())	{
// 		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_RW_STATUS,(BYTE)TokenRWStatus);
// 		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
// 	}
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief        获取Token RW状态（暂不使用）

@param      无

@retval       STATUS_RW Token RW状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// STATUS_RW CStatusHelper::GetTokenRWStatus()
// {
// 	return m_sTokenRWStatus;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam1类型（暂不使用）
@param      SAM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetSam1Type(SAM_STATUS type)
{
	if(m_sSam1Type == type)return;

	m_sSam1Type = type;
	try{
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW_SAM1_TYPE,(BYTE)type);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Sam1类型（暂不使用）
@param      无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CStatusHelper::GetSam1Type()
{
	return m_sSam1Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam2类型
@param      SAM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetSam2Type(SAM_STATUS type)
{
	if(m_sSam2Type == type) return;

	m_sSam2Type = type;
	try{
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW_SAM2_TYPE,(BYTE)type);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Sam2类型
@param      无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CStatusHelper::GetSam2Type()
{
	return m_sSam2Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam3类型（暂不使用）
@param      SAM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetSam3Type(SAM_STATUS type)
// {
// 	m_sSam3Type = type;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Sam3类型（暂不使用）
@param     无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// SAM_STATUS CStatusHelper::GetSam3Type()
// {
// 	return m_sSam3Type;
// }


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置SAM1状态(充值读写器一票通 ISAM)

@param   (i)SAM_STATUS SAM卡状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetSam1Status(SAM_AUTH_STATUS status)
{
	try{
		if(m_sSAM1Status == status){
			return;
		}
		m_sSAM1Status = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW_SAM_1_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取SAM1状态(充值读写器一票通 ISAM)

@param      无

@retval     SAM_STATUS SAM卡状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_AUTH_STATUS CStatusHelper::GetSam1Status()
{
	return m_sSAM1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置SAM2状态(Card读写器一票通 ISAM)

@param   (i)SAM_STATUS SAM卡状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetSam2Status(SAM_AUTH_STATUS status)
{
	try{
		if(m_sSAM2Status == status)return;
		m_sSAM2Status = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW_SAM_2_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取SAM2状态(Card读写器一票通 ISAM)

@param      无

@retval     SAM_STATUS SAM卡状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_AUTH_STATUS CStatusHelper::GetSam2Status()
{
	return m_sSAM2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置SAM3状态(Token读写器一票通 ISAM)

@param   (i)SAM_STATUS SAM卡状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CStatusHelper::SetSam3Status(SAM_STATUS status)
//{
//	try{
//		m_sTokenRWYPTISAMStatus = status;
//		if(IsSCConnected())	{
//			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_RW_SAM1_STATUS,(BYTE)status);
//			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
//		}
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取SAM3状态(Token读写器一票通 ISAM)

@param      无

@retval     SAM_STATUS SAM卡状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//SAM_STATUS CStatusHelper::GetSam3Status()
//{
//	return m_sTokenRWYPTISAMStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置SAM4状态(充值读写器一卡通ISAM)(暂不使用)

@param    (i)SAM_STATUS SAM卡状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CStatusHelper::SetSam4Status(SAM_STATUS status)
//{
//	try{
//		m_sRechargeRWYKTISAMStatus = status;
//		if(IsSCConnected())	{
//			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_RW_SAM1_STATUS,(BYTE)status);
//			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
//		}
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取SAM4状态(充值读写器一卡通ISAM)(暂不使用)

@param      无

@retval     SAM_STATUS SAM卡状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//SAM_STATUS CStatusHelper::GetSam4Status()
//{
//	return m_sRechargeRWYKTISAMStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置打印机状态

@param    (i)PRINTER_STATUS 打印机状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetPrintStatus(PRINTER_STATUS status)
{
	try{
		if(m_sPrintStatus == status)return;

		m_sPrintStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::PRINTER_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取打印机状态

@param   无

@retval     PRINTER_STATUS 打印机状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
PRINTER_STATUS CStatusHelper::GetPrintStatus()
{
	return m_sPrintStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置UPS状态

@param    (i)UPS_STATUS UPS状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CStatusHelper::SetUPSStatus(UPS_STATUS status)
//{
//	try{
//		m_sUPSStatus = status;
//		if(IsSCConnected())	{
//			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::UPS_STATUS,(BYTE)status);
//			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
//		}
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取UPS状态

@param   无

@retval     UPS_STATUS UPS状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//UPS_STATUS CStatusHelper::GetUPSStatus()
//{
//	return m_sUPSStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置维修门状态

@param      DOOR_STATUS

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetMaintenanceDoorStatus(DOOR_STATUS status)
{
	if(m_sMaintenanceDoorStatus == status) return;

	m_sMaintenanceDoorStatus = status;
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DOOR_STATUS,(BYTE)status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取维修门状态

@param      无

@retval     DOOR_STATUS

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DOOR_STATUS CStatusHelper::GetMaintenanceDoorStatus()
{
	return m_sMaintenanceDoorStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置纸币接收模块状态

@param   (i)HARDWARE_MODULE_STATUS 纸币接收模块状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteModuleStatus(HARDWARE_MODULE_STATUS status)
{
	try{
		if(m_sBanknoteModuleStatus == status){
			return;
		}
		m_sBanknoteModuleStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_ACCEPT_MODULE_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取纸币接收模块状态

@param      无

@retval     HARDWARE_MODULE_STATUS

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetBanknoteModuleStatus()
{
	return m_sBanknoteModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零模块状态

@param      (HARDWARE_MODULE_STATUS status；（找零模块状态）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteChangeModuleStatus(HARDWARE_MODULE_STATUS status){
	try{
		if(m_sBanknoteModuleStatus == status) return;

		m_sBanknoteModuleStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CHANGE_MODULE_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取纸币找零模块状态

@param      无

@retval     HARDWARE_MODULE_STATUS

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetBanknoteChangeModuleStatus()
{
	return m_sBanknoteChangeModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Token模块状态

@param    (i)HARDWARE_MODULE_STATUS Token模块状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CStatusHelper::SetTokenModuleStatus(HARDWARE_MODULE_STATUS status)
//{
//	try{
//		m_sTokenModuleStatus = status;
//		if(IsSCConnected())	{
//			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_MODULE_STATUS,(BYTE)status);
//			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
//		}
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Token模块状态

@param      无

@retval     HARDWARE_MODULE_STATUS Token模块状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//HARDWARE_MODULE_STATUS CStatusHelper::GetTokenModuleStatus()
//{
//	return m_sTokenModuleStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Card模块状态

@param    (i)HARDWARE_MODULE_STATUS Card模块状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardModuleStatus(HARDWARE_MODULE_STATUS status)
{
	try{
		if(m_sCardModuleStatus == status)return;

		m_sCardModuleStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_MODULE_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Card模块状态

@param      无

@retval     HARDWARE_MODULE_STATUS Card模块状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetCardModuleStatus()
{
	return m_sCardModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置硬币模块状态

@param    (i) HARDWARE_MODULE_STATUS 硬币模块状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinModuleStatus(HARDWARE_MODULE_STATUS status)
{
	try{
		if(m_sCoinModuleStatus == status)return;

		m_sCoinModuleStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_MODULE_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取硬币模块状态

@param      无

@retval     HARDWARE_MODULE_STATUS 硬币模块状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetCoinModuleStatus()
{
	return m_sCoinModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置状态显示器状态

@param    (i) HARDWARE_MODULE_STATUS 状态显示器状态

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetStatusDisplayStatus(HARDWARE_MODULE_STATUS status)
{
	try{
		if(m_sStatusDisplayStatus == status)return;

		m_sStatusDisplayStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::STATUS_DISPLAY_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取状态显示器状态

@param      无

@retval     HARDWARE_MODULE_STATUS 状态显示器状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetStatusDisplayStatus()
{
	return m_sStatusDisplayStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置倾斜传感器状态(震动传感器状态)

@param    (i) TILT_STATUS 倾斜传感器状态(震动传感器状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetTiltStatus(TILT_STATUS status)
// {
// 	try{
// 		/*m_sTiltStatus = status;
// 		if(IsSCConnected())	{
// 			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TILT_STATUS,(BYTE)status);
// 			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
// 		}*/
// 	}
// 	catch (CSysException& e) {
// 		theEXCEPTION_MGR.ProcessException(e);
// 	}
// 	catch (...) {
// 		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
// 	}
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取倾斜传感器状态(震动传感器状态)

@param      无

@retval     TILT_STATUS 倾斜传感器状态(震动传感器状态)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
// TILT_STATUS CStatusHelper::GetTiltStatus()
// {
// 	return m_sTiltStatus;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置离线天数

@param    (i) BYTE 离线天数

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetOffLineDays(BYTE days)
{
	try{
		/*m_sOffLineDays = days;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::OFFLINE_DAYS,(BYTE)days);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}*/
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取离线天数

@param      无

@retval     TILT_STATUS 离线天数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE CStatusHelper::GetOffLineDays()
{
	return m_sOffLineDays;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置倾斜传感器状态(震动传感器状态)

@param    (i) TILT_STATUS 倾斜传感器状态(震动传感器状态)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetAuthorizedAccessStatus(AUTHORIZED_STATUS status)
{
	/*try{
		m_sAuthorizedAccessStatus = status;
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::AUTHORIZED_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取授权访问状态

@param      无

@retval     AUTHORIZED_STATUS 授权访问状态

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
AUTHORIZED_STATUS CStatusHelper::GetAuthorizedAccessStatus()
{
	return m_sAuthorizedAccessStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCDataMsg* CStatusHelper::MakeStatusChangeCommand(TVM_STATUS_ID statusID,BYTE bValue)
{
	SYNC(CSCDataMsg,_T("TVM_STATUS_ID"));
	BYTE lpContent[LEN_STATUS_CHANGE] = {0};
	theSCMessageMakeHelper.MakeDeviceStatusChange(lpContent,statusID,bValue);
	CSCDataMsg* bomStatusChange = new CSCDataMsg;
	CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
	bomStatusChange->SetAutoDeleted(true);
	bomStatusChange->SetHeader(&header);
	bomStatusChange->SetContent(lpContent);
	return bomStatusChange;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TVM设备故障状态数据

@param      故障ID

@retval     发送消息对象指针

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCDataMsg* CStatusHelper::MakeErrorStatusCommand(long lMtcCode){
	BYTE lpContent[LEN_STATUS_HAS_ERROR] = {0};
	theSCMessageMakeHelper.MakeDeviceErrorStatus(lpContent,lMtcCode);
	CSCDataMsg* tvmErrorMsg = new CSCDataMsg;
	CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
	tvmErrorMsg->SetAutoDeleted(TRUE);
	tvmErrorMsg->SetHeader(&header);
	tvmErrorMsg->SetContent(lpContent);
	return tvmErrorMsg;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      异常队列发生变化时相应函数

@param      (i)CSysException& e 异常对象
@param      (i)bool isInsert          是否是插入异常

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::OnExceptionItemNotify(CSysException& e,bool isInsert)
{
	// 增加异常
	if(isInsert){
		//SetDeviceErrStatus(DEVICE_ERROR);
		// 打印机状态
		if(e.GetModuleID() == CPrinterException::MODULE_ID){
			SetPrintStatus(PRINTER_STATUS::PRINTER_ERR);//SetPrintStatus((e.GetDetailID() == CPrinterException::ERROR_Printer_NO_PAPER ||	e.GetDetailID() == CPrinterException::ERROR_Printer_LESS_PAPER) ? PRINTER_NOPAPER :PRINTER_ERR	);
		}		
		// Crad读写器
		if(e.GetModuleID() == CRWException::TVM_CARD_RW_MODULE_ID || e.GetModuleID() == CRWHardwareException::TVM_CARD_RW_MODULE_ID ){
			SetRW1Status(RW_ERROR);
		}
		// 纸币模块
		if(e.GetModuleID() == CBRHException::MODULE_ID || e.GetModuleID() == CBRHHardwareException::MODULE_ID){
			if (e.GetDetailID() == CBRHHardwareException::BANKNOTE_ERROR_MODULE_NOT_STANDBY){
				SetBanknoteModuleStatus(HARDWARE_MODULE_UNSET);
			}
			else{
				SetBanknoteModuleStatus(HARDWARE_MODULE_FAILURE);
			}
		}	
		// 硬币模块
		if(e.GetModuleID() == CCHException::MODULE_ID || e.GetModuleID() == CCHHardwareException::MODULE_ID){
			if (e.GetDetailID() == CCHHardwareException::ERROR_CH_MODULE_NOT_STANDBY){
				SetCoinModuleStatus(HARDWARE_MODULE_UNSET);
			}
			else{
				SetCoinModuleStatus(HARDWARE_MODULE_FAILURE);
			}
		}
		// IO模块
		if(e.GetModuleID() == CIOException::MODULE_ID){
			//SetBanknoteModuleStatus(HARDWARE_MODULE_FAILURE);
		}	
		// 状态显示器
		if(e.GetModuleID() == CStatusDisplayException::MODULE_ID){
			SetStatusDisplayStatus(HARDWARE_MODULE_FAILURE);
		}
		// Card模块
		if(theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID)){
			SetCardModuleStatus(HARDWARE_MODULE_FAILURE);
		}
		// 设备故障状态
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_DEVICE_ERR,NULL,theEXCEPTION_MGR.GetActualMTC(e));
		SetRunStatus(WARNING);
		//SetServiceStatus(theEXCEPTION_MGR.OnlyHasHardwareException() ? WARN_SERVICE : WARN_SERVICE/*OUTOF_SERVICE*/);// zhengxianle 防止在业务进行中产生异常显示暂停服务，暂时处理
		// 设备故障业务数据
		DEVICE_ERRORDATA errorData;
		unsigned long ulMtcCode = theEXCEPTION_MGR.GetActualMTC(e);
		unsigned short usEc = theEXCEPTION_MGR.GetEC(e);
		errorData.partType = theEXCEPTION_MGR.GetDevicePartTypeOfException(e);
		errorData.bDeviceType =  theEXCEPTION_MGR.GetDevicePartTypeOfException(e);                // 1 BIN 部件类型    
		if(errorData.bDeviceType!=DEVICE_COMPONENT::DEVICE_COMPOENT_UNKNOW){
			int departIndex = 1;
			memcpy(errorData.devicePartID,&departIndex,3);
		}
		else{
			memset(errorData.devicePartID, 0xFF, 3);     // 3 BIN 部件ID  
		}
		errorData.deviceDeviderId = FOUNDER_ID;      // 2 BIN 设备供应商ID
		memcpy(errorData.maintanCode, &ulMtcCode, 4);// 4 1BIN+1BIN+1BIN+1BIN 故障代码   
		memcpy(errorData.ecode, &usEc, 2);           // 2 1BIN+1BIN 维修代码
		SetDeviceErrStatus(ulMtcCode);
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceErrorData(errorData);
	}
	// 删除异常
	else{
		// 打印机
		/*if(!theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID)){
		SetPrintStatus(PRINTER_NORMAL);
		}*/
		//// 充值读写器
		//if((!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID))){
		//	SetRechargeRWStatus(RW_NORMAL);
		//}
		//// Card读写器
		//if((!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID))){
		//	SetCardRWStatus(RW_NORMAL);
		//}
		//// Token读写器
		//if((!theEXCEPTION_MGR.HasException(CRWException::TVM_TOKEN_RW_MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID))){
		//	SetCardRWStatus(RW_NORMAL);
		//}
		// Card模块
		/*if((!theEXCEPTION_MGR.HasException(CTHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID))){
		SetCardModuleStatus(HARDWARE_MODULE_NORMAL);
		}*/
		// 纸币模块
		if((!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID))){
			SetBanknoteModuleStatus(HARDWARE_MODULE_NORMAL);
		}
		// 硬币模块
		if((!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID))){
			SetCoinModuleStatus(HARDWARE_MODULE_NORMAL);
		}
		// IO模块
		if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			//SetBanknoteModuleStatus(HARDWARE_MODULE_NORMAL);
		}

		// LED显示器
		if(!theEXCEPTION_MGR.HasException(CStatusDisplayException::MODULE_ID)){
			SetStatusDisplayStatus(HARDWARE_MODULE_NORMAL);
		}

		//SetDeviceErrStatus(DEVICE_NO_ERROR);
		SetRunStatus(RUNING);
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_DEVICE_ERR,NULL,0);
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_DEVICE_FULLSTATUS,STATUS_TYPE_FULL,NULL);
		bool hasLoginCommon = (theAPP_SESSION.GetUserInfo().sUserID != _T("")) && (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY);
		//SetServiceStatus(hasLoginCommon ? IN_SERVICE : OUTOF_SERVICE);
		//m_sServiceStatus = hasLoginCommon ? IN_SERVICE : OUTOF_SERVICE;
		SetDeviceErrStatus(0);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取工作模式

@param      无

@retval     WORK_MODE 工作模式代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORK_MODE CStatusHelper::GetDegreeWorkMode()
{
	if(m_sWorkMode & DEVICE_CONTRL_CODE_BUSINESS_CARD==DEVICE_CONTRL_CODE_BUSINESS_CARD){
		// 支持硬币和纸币，不支持储值卡
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)!=DEVICE_CONTRL_CODE_PAYMENT_SVT)){
			return WORK_MODE::WORK_MODE_NO_SVT;
		}
		// 支持硬币和储值卡，不支持纸币
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_BANKNOTE_ACCEPT;
		}
		// 支持纸币和储值卡，不支持硬币（硬币模块故障）
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)!=DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_COIN_ACCEPT;
		}
		// 支持纸币和储值卡，不支持硬币找零（硬币hopper箱空）
		if ( ((m_sChangeMode & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)!=DEVICE_CONTRL_CODE_NO_BILL_CHANGE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_CHANGE;
		}
		// 只支持纸币
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)!=DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)!=DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_BANKNOTE_ONLY;
		}
		// 只支持硬币
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)!=DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_COIN_ONLY;
		}
		// 只支持储值卡
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)!=DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_SVT_ONLY;
		}

		// 支持全部支付方式
		if (m_sPaymentMode == DEVICE_CONTRL_CODE_PAYMENT_ALL){
			return WORK_MODE::WORK_MODE_NORMALLY;
		}
	}
	else{
		// 支持纸币和储值卡
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_ISSUE;
		}
		// 只支持储值卡
		if (((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_ANALY_ONLY;
		}
	}

	return WORK_MODE::WORK_MODE_NORMALLY;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断当前SC是否连接，且不在运营结束时序中

@param      无

@retval     bool true:可以送状态数据 false:不可以发送状态数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusHelper::IsSCConnected(){
	return theAPP_SESSION.IsSCConnected() && !theAPP_SESSION.GetCloseOperationFlag();
}


CStatusHelper CStatusHelper::theInstance;
