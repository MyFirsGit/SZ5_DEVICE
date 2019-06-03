#include "StdAfx.h"
#include "BomStatusMgr.h"
#include "Service.h"
#include "scapi.h"
#include "ServiceMgr.h"
#include "ServiceDefinition.h"
#include "scmessagemakehelper.h"
#include "scdatamsg.h"
#include "headermanager.h"
#include "businesstranslatesvc.h"
#include "serverclientsvc.h"
#include "bomdef.h"
#include "MainSvc.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数
@param      无
@retval     CBomStatusMgr&    CBomStatusMgr实例对象
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBomStatusMgr::CBomStatusMgr()
	:m_sServiceStatus(OUTOF_SERVICE)         // 服务状态
	,m_sRunStatus(RUNING)              // 运行状态
	,m_sWorkMode(ISSUE_ADJUST_MODE)          // 工作模式
	,m_sRunMode(DEVICE_MODE_NORMAL)		//系统模式
	,m_s24HourMode(0x00)												//24小时模式
	,m_sDelayTime(0x00)													//延长运营时间
	,m_sConnectStatus(DISCONNECT)            // 连接状态
	,m_sTimeStatus(VALID_TIME)               // 时钟差异
	,m_sLoginStatus(LOGOUT)                  // 登录状态
	,m_sAutorunStatus(AUTORUN_STOP)          // 设备自动运行状态
	,m_sMagazineAStatus(STATUS_MAGAZINE_SET) // 票箱1安装状态
	,m_sMagANumStatus(MAG_NUM_NORMAL)        // 票箱1数量状态
	,m_sMagazineBStatus(STATUS_MAGAZINE_SET) // 票箱2安装状态
	,m_sMagBNumStatus(MAG_NUM_NORMAL)        // 票箱2数量状态
	,m_sCycleBoxStatus(STATUS_MAGAZINE_SET)  // 废票箱安装状态
	,m_sCycleBoxNumStatus(MAG_NUM_NORMAL)    // 废票箱数量状态
	,m_sRW1Status(RW_NORMAL)                 // 读写器1状态
	,m_sRW2Status(RW_NORMAL)                 // 读写器2状态
	,m_sSam1Type(YPT_PSAM)                   // SAM1类型
	,m_sSam2Type(YKT_PSAM)                   // SAM2类型
	,m_sSam3Type(YPT_PSAM)                   // SAM3类型
	,m_sSam1Status(ISAM_UNAUTH)              // ISAM1状态
	,m_sSam2Status(ISAM_UNAUTH)              // ISAM2状态
	,m_sSam3Status(ISAM_UNAUTH)              // ISAM2状态
	,m_sDoorStatus(DOOR_NORMAL_CLOSE)        // 维修门状态
	,m_sPrintStatus(PRINTER_NORMAL)          // 打印机状态     
	,m_sThModuleStatus(TH_MODULE_NORMAL)     // TH模块错误
{
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CBomStatusMgr::OnExceptionItemNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数
@param      无
@retval     CBomStatusMgr&    CBomStatusMgr实例对象
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBomStatusMgr::~CBomStatusMgr()
{
	theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CBomStatusMgr::OnExceptionItemNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取该类的对象
@param      无
@retval     CBomStatusMgr&    CBomStatusMgr实例对象
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBomStatusMgr& CBomStatusMgr::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置服务状态 
@param      SERVICE_STATUS    status
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetServiceStatus(SERVICE_STATUS status)
{
	try{
		m_sServiceStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::SERVICE_STATUS,(BYTE)m_sServiceStatus);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgImportantStatusChange(m_sServiceStatus);
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
@param      无
@retval     SERVICE_STATUS    status
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATUS CBomStatusMgr::GetServiceStatus()
{
	return m_sServiceStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置运行状态
@param      RUN_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetRunStatus(RUN_STATUS status)
{
	try{
		m_sRunStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::RUN_STATUS,(BYTE)status);
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
@brief      获取运行状态
@param      无
@retval     RUN_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
RUN_STATUS CBomStatusMgr::GetRunStatus()
{
	return m_sRunStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置工作模式
@param      WORD_MODE
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetWorkMode(WORK_MODE mode, bool bManual)
{
	try{
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_NEW_WORK_MODE,mode,NULL);
		m_sWorkMode = mode;
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::WORK_MODE,(BYTE)mode);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		OPERATOR_LOG operatorLog;
		operatorLog.operCode = (bManual == true)?OPERATION_MODE_SET:OPERATION_MODE_NOTIC;
		operatorLog.operLevel = (bManual == true)?OPERATION_FIVE_LEVEL:OPERATION_THREE_LEVEL;
		operatorLog.result = 0x01;
		char *pDetail = (char*)(operatorLog.operDetail);
		switch(mode)
		{
			case ISSUE_MODE:
				strcpy(pDetail, "售票模式");
				break;
			case ADJUST_MODE:
				strcpy(pDetail, "补票模式");
				break;
			case ISSUE_ADJUST_MODE:
				strcpy(pDetail, "售补票模式");
				break;
			default:
				strcpy(pDetail, "未知模式");
				break;
		}
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);
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
@brief      获取工作模式
@param      无
@retval     WORD_MODE
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORK_MODE CBomStatusMgr::GetWorkMode()
{
	return m_sWorkMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置运营模式
@param      WORD
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetRunMode(OperationalMode_t mode)
{
	try{
		m_sRunMode = mode;
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::RUN_MODE,(BYTE)mode);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		
		RW_CMD_SET_MODE rw_cmd_set_mode;
		rw_cmd_set_mode.ControlCode = 0x01;
		rw_cmd_set_mode.ModeCode = mode;

		CRWHelper helper(*theSERVICE_MGR.GetService(MAIN_SVC));
		
		RW_RSP_STATUS rw_rsp_status;
		helper.SetOperationMode(rw_cmd_set_mode,rw_rsp_status);

		CTWHelper twhelper(*theSERVICE_MGR.GetService(MAIN_SVC));
		twhelper.SetOperationMode(rw_cmd_set_mode,rw_rsp_status);

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
@param      无
@retval     WORD
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
OperationalMode_t CBomStatusMgr::GetRunMode()
{
	return m_sRunMode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得运营模式名字

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CBomStatusMgr::GetRunModeName()
{
	switch(m_sRunMode)
	{
	case DEVICE_MODE_NORMAL: return _T("正常模式");
	case DEVICE_MODE_CLOSED : return _T("关闭模式");
	case DEVICE_MODE_TRAIN_FAULT: return _T("列车故障模式");
	case DEVICE_MODE_CHECK_FREE_ENTRY: return _T("进站免检");
	case DEVICE_MODE_CHECK_FREE_EXIT: return _T("出站免检");
	case DEVICE_MODE_EMERGENCY_EXIT:return _T("紧急模式");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME: return _T("时间免检");
	case DEVICE_MODE_NORMAL_CHECK_FREE_DATE: return _T("日期免检");
	case DEVICE_MODE_NORMAL_CHECK_FREE_FARE: return _T("费用免检");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE : return _T("日期+时间免检");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME_FARE: return _T("时间+费用免检");
	case DEVICE_MODE_NORMAL_CHECK_FREE_DATE_FARE: return _T("日期+费用免检");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE_FARE: return _T("日期+时间+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME:	return _T("进站+时间免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_DATE:	return	_T("进站+日期免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_FARE:	return _T("进站+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE: return _T("进站+日期+时间免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME_FARE : return _T("进站+时间+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_DATE_FARE: return  _T("进站+日期+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE_FARE: return _T("进站+时间+日期+费用免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME: return _T("出站+时间免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_DATE: return _T("出站+日期免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_FARE: return _T("出站+费用免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE: return _T("出站+时间+日期免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME_FARE: return _T("出站+时间+费用免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_DATE_FARE: return _T("出站+日期+费用免检");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE_FARE: return _T("出站+时间+日期+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME: return _T("进站+时间+出站免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE: return _T("进站+出站+日期免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_FARE: return _T("进站+出站+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE: return _T("进站+出站+时间+日期免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_FARE: return _T("进站+出站+时间+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE_FARE: return _T("进站+出站+日期+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE_FARE: return _T("进站+出站+时间+日期+费用免检");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT: return _T("进站+出站免检");
	default:
		return _T("未知模式");
	};
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置24小时运营模式
@param      byte
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::Set24HourMode(BYTE b24HourMode)
{
	try{
		m_s24HourMode = b24HourMode;
		//theMenuMgr.Set24DelayStatus((modeCode == 1) ? true : false); 
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::HOUR24MODE,(BYTE)b24HourMode);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_24HOUR_DELAY);
		RW_CMD_SET_MODE rw_cmd_set_mode;
		rw_cmd_set_mode.ControlCode = b24HourMode == 0x00 ? 0x02 : 0x01;
		//rw_cmd_set_mode.ModeCode = RUN_24HOUR_MODE;
		CRWHelper helper(*theSERVICE_MGR.GetService(MAIN_SVC));
		RW_RSP_STATUS rw_rsp_status;
		helper.SetOperationMode(rw_cmd_set_mode,rw_rsp_status);
		
		CTWHelper twhelper(*theSERVICE_MGR.GetService(MAIN_SVC));
		twhelper.SetOperationMode(rw_cmd_set_mode,rw_rsp_status);

		OPERATOR_LOG operatorLog;
		operatorLog.operCode = OPERATION_24HOUR_DELAY;
		operatorLog.operLevel = OPERATION_ONE_LEVEL;
		operatorLog.result = 0x01;
		char *pDetail = (char*)(operatorLog.operDetail);
		strcpy(pDetail, b24HourMode == 0x00 ?" 非24小时运营":"24小时运营");
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);

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
@param      无
@retval     WORD
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE CBomStatusMgr::Get24HourMode()
{
	return m_s24HourMode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     设置延长运营时间
@param      WORD
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetDelayTime(WORD delayTime)
{
	try{

			m_sDelayTime = delayTime;
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::DELAY_RUN_MODE,(delayTime==0)?0x00:0x01);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_24HOUR_DELAY);
			CRWHelper helper(*theSERVICE_MGR.GetService(MAIN_SVC));
			CTWHelper twhelper(*theSERVICE_MGR.GetService(MAIN_SVC));
		
			RW_CMD_SET_TIME cmdSetTime;
			cmdSetTime.ControlCode = 0x10;
			cmdSetTime.Time = delayTime * 60;
			RW_RSP_STATUS rspStatus;
			//if(delayTime == 0)//取消延长运营
			//{
			//	COleDateTime currentTime = COleDateTime::GetCurrentTime();
			//	CAfcDeviceTimeParam::OPRATION_TIME operationTime;
			//	theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
			//	COleDateTime defaultSwitchTime = COleDateTime::GetCurrentTime();
			//	defaultSwitchTime.SetTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);
			//	bool isEnterDelayPeriod = currentTime>defaultSwitchTime;
			//	if(isEnterDelayPeriod)//已经进入延长运营时间段，则不处理。
			//	{
			//		return;
			//	}
			//	helper.SetBusinessTime(cmdSetTime,rspStatus);
			//	twhelper.SetBusinessTime(cmdSetTime,rspStatus);
			//	ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);//处理运营日切换时间点
			//}
			//else//设置延长运营
			//{
				helper.SetBusinessTime(cmdSetTime,rspStatus);
				twhelper.SetBusinessTime(cmdSetTime,rspStatus);
				
				CAfcDeviceTimeParam::OPRATION_TIME operationTime;
				theAFC_DEV_TIME.GetOperationTimeParam(operationTime);

				COleDateTime switchTime = COleDateTime::GetCurrentTime();
				switchTime.SetTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,operationTime.oprationDateChangeTime.bSec);

				COleDateTimeSpan delayTimeSpan = COleDateTimeSpan(0,delayTime/60,delayTime%60,0);

				COleDateTime switchTimeAfterDelay = switchTime+delayTimeSpan;

				ComSetBusiDateSwitchTime(switchTimeAfterDelay.GetHour(),switchTimeAfterDelay.GetMinute(),switchTimeAfterDelay.GetSecond());

			//}
			OPERATOR_LOG operatorLog;
			operatorLog.operCode = OPERATION_24HOUR_DELAY;
			operatorLog.operLevel = OPERATION_ONE_LEVEL;
			operatorLog.result = 0x01;
			char *pDetail = (char*)(operatorLog.operDetail);
			strcpy(pDetail, delayTime == 0 ?" 取消延长运营":"设置延长运营");
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);

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
@param      无
@retval     WORD
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD CBomStatusMgr::GetDelayTime()
{
	return m_sDelayTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置通信状态
@param      COMMUNICATION
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetCommunicate(COMMUNICATION comm)
{
	try{

		if (comm != m_sConnectStatus)
		{
			m_sConnectStatus = comm;
			if(theAPP_SESSION.IsSCConnected())
			{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::CONNECT_STATUS,(BYTE)comm);
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
			}
			OPERATOR_LOG operatorLog;
			operatorLog.operCode = OPERATION_CONNECT_STATUS;
			operatorLog.operLevel = OPERATION_ONE_LEVEL;
			operatorLog.result = 0x01;
			char *pDetail = (char*)(operatorLog.operDetail);
			switch(comm)
			{
				case DISCONNECT:
					strcpy(pDetail, " 通讯中断");
					break;
				case CONNECTED:
					strcpy(pDetail, "通讯正常");
					break;
				case CONNECTING:
					strcpy(pDetail, "通讯恢复中");
					break;
				default:
					strcpy(pDetail, "未知状态");
					break;
			}
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);
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
@param      无
@retval     COMMUNICATION
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COMMUNICATION CBomStatusMgr::GetCommunicate()
{
	return m_sConnectStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置时钟同步状态
@param      TIME_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetTimeSynStatus(TIME_STATUS status)
{
	try{
		m_sTimeStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::TIME_DELAY,(BYTE)status);
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
@param      无
@retval     TIME_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TIME_STATUS CBomStatusMgr::GetTimeSynStatus()
{
	return m_sTimeStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置登录状态
@param      LOGIN_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetLoginStatus(LOGIN_STATUS status)
{
	try{
		m_sLoginStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::LOGIN_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		OPERATOR_LOG operatorLog;
		operatorLog.operCode =(m_sLoginStatus== LOGIN)?OPERATION_LOGIN:OPERATION_LOGOUT;
		operatorLog.operLevel = OPERATION_ONE_LEVEL; 
		operatorLog.result =0x01;
		memset(operatorLog.operTip, 0, sizeof(operatorLog.operTip));
		char *pDetail = (char*)(operatorLog.operDetail);
		strcpy(pDetail, (m_sLoginStatus== LOGIN)?"登录":"登出");
		operatorLog.csUserID = theAPP_SESSION.GetUserInfo().sUserID;
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operatorLog);	
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
@param      无
@retval     LOGIN_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LOGIN_STATUS CBomStatusMgr::GetLoginStatus()
{
	return m_sLoginStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取自动运行
@param      无
@retval     LOGIN_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_AUTORUN CBomStatusMgr::GetAutoRunStatus()
{
	return m_sAutorunStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置A箱Set状态
@param      STATUS_MAGAZINE
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetMagAStatus(STATUS_MAGAZINE status)
{
	try{
		m_sMagazineAStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::MAGAZINEA_STATUS,(BYTE)status);
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
@brief      获取A箱Set状态
@param      无
@retval     STATUS_MAGAZINE
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CBomStatusMgr::GetMagAStatus()
{
	return m_sMagazineAStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置A箱数量状态
@param      MAGAZINE_NUM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetMagANumStatus(MAGAZINE_NUM_STATUS status)
{
	try{
		m_sMagANumStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::MAGAZINEA_COUNT,(BYTE)status);
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
@brief      获取A箱数量状态
@param      无
@retval     MAGAZINE_NUM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CBomStatusMgr::GetMagANumStatus()
{
	return m_sMagANumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置B箱Set状态
@param      STATUS_MAGAZINE
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetMagBStatus(STATUS_MAGAZINE status)
{
	try{
		m_sMagazineBStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::MAGAZINEB_STATUS,(BYTE)status);
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
@brief      获取B箱Set状态
@param      无
@retval     STATUS_MAGAZINE
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CBomStatusMgr::GetMagBStatus()
{
	return m_sMagazineBStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置B箱数量状态
@param      MAGAZINE_NUM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetMagBNumStatus(MAGAZINE_NUM_STATUS status)
{
	try{
		m_sMagBNumStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::MAGAZINEB_COUNT,(BYTE)status);
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
@brief      获取B箱数量状态
@param      无
@retval     MAGAZINE_NUM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CBomStatusMgr::GetMagBNumStatus()
{
	return  m_sMagBNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置回收箱Set状态
@param      STATUS_MAGAZINE
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetCycleBoxStatus(STATUS_MAGAZINE status)
{
	try{
		m_sCycleBoxStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::CYCLEBOX_STATUS,(BYTE)status);
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
@brief      获取回收箱Set状态
@param      无
@retval     STATUS_MAGAZINE
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CBomStatusMgr::GetCycleBoxStatus()
{

	return m_sCycleBoxStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置回收数量状态
@param      MAGAZINE_NUM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetCycleBoxNumStatus(MAGAZINE_NUM_STATUS status)
{
	try{
		m_sCycleBoxNumStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::CYCLEBOX_COUNT,(BYTE)status);
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
@brief      获取回收箱数量状态
@param      无
@retval     MAGAZINE_NUM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CBomStatusMgr::GetCycleBoxNumStatus()
{
	return m_sCycleBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置内部RW状态
@param      STATUS_RW
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetInnerRWStatus(STATUS_RW status)
{
	m_sRW1Status = status;
	if(theAPP_SESSION.IsSCConnected())
	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::RW1_STATUS,(BYTE)status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取内部RW状态
@param      无
@retval     STATUS_RW
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CBomStatusMgr::GetInnerRWStatus()
{
	return m_sRW1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置外部RW状态
@param      STATUS_RW
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetOuterRWStatus(STATUS_RW status)
{
	m_sRW2Status = status;
	if(theAPP_SESSION.IsSCConnected())
	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::RW2_STATUS,(BYTE)status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取内部RW状态
@param      无
@retval     STATUS_RW
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CBomStatusMgr::GetOuterRWStatus()
{
	return m_sRW2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam1类型
@param      SAM_TYPE_
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam1Type(SAM_TYPE_ type)
{
	m_sSam1Type = type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Sam1类型
@param      无
@retval     SAM_TYPE_
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_TYPE_ CBomStatusMgr::GetSam1Type()
{
	return m_sSam1Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam2类型
@param      SAM_TYPE_
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam2Type(SAM_TYPE_ type)
{
	m_sSam2Type = type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Sam2类型
@param      无
@retval     SAM_TYPE_
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_TYPE_ CBomStatusMgr::GetSam2Type()
{
	return m_sSam2Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam3类型
@param      SAM_TYPE_
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam3Type(SAM_TYPE_ type)
{
	m_sSam3Type = type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Sam3类型
@param      无
@retval     SAM_TYPE_
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_TYPE_ CBomStatusMgr::GetSam3Type()
{
	return m_sSam3Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam1认证状态(外部读写器ACC ISAM)
@param      SAM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam1Status(SAM_STATUS status)
{
	try{
		m_sSam1Status = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::ISAM1_STATUS,(BYTE)status);
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
@brief      获取Sam1认证状态
@param      无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CBomStatusMgr::GetSam1Status()
{
	return m_sSam1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam2认证状态(外部读写器YKT ISAM)
@param      SAM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam2Status(SAM_STATUS status)
{
	try{
		m_sSam2Status = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::ISAM2_STATUS,(BYTE)status);
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
@brief      获取Sam2认证状态
@param      无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CBomStatusMgr::GetSam2Status()
{
	return m_sSam2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Sam3认证状态(内部读写器ACC ISAM)
@param      SAM_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam3Status(SAM_STATUS status)
{
	try{
		m_sSam3Status = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::ISAM3_STATUS,(BYTE)status);
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
@brief      获取Sam3认证状态
@param      无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CBomStatusMgr::GetSam3Status()
{
	return m_sSam3Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置维修门状态
@param      DOOR_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetDoorStatus(DOOR_STATUS status)
{
	m_sDoorStatus = status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取维修门状态
@param      无
@retval     DOOR_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DOOR_STATUS CBomStatusMgr::GetDoorStatus()
{
	return m_sDoorStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      设置打印机状态
@param      PRINTER_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetPrintStatus(PRINTER_STATUS status)
{
	try{
		m_sPrintStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::PRINTER_STATUS,(BYTE)status);
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
@param      无
@retval     PRINTER_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
PRINTER_STATUS CBomStatusMgr::GetPrintStatus()
{
	return m_sPrintStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置TH模块状态
@param      TH_MODULE_STATUS
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetTHModuleStatus(TH_MODULE_STATUS status)
{
	try{
		m_sThModuleStatus = status;
		if(theAPP_SESSION.IsSCConnected())
		{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(BOM_STATUS_IDS::TH_MODULE_STATUS,(BYTE)status);
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
@brief      获取TH模块状态
@param      无
@retval     SAM_STATUS
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
TH_MODULE_STATUS CBomStatusMgr::GetTHModuleStatus()
{
	return m_sThModuleStatus;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCDataMsg* CBomStatusMgr::MakeStatusChangeCommand(BOM_STATUS_ID statusID,BYTE bValue)
{
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
@brief      异常队列发生变化时相应函数

@param      (i)CSysException& e 异常对象
@param      (i)bool isInsert      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::OnExceptionItemNotify(CSysException& e,bool isInsert)
{
	if(isInsert){
		// 非维护状态时，发送BOM状态电文
		if(e.GetModuleID() == CPrinterException::MODULE_ID){
			SetPrintStatus((e.GetDetailID() == CPrinterException::ERROR_Printer_NO_PAPER ||	e.GetDetailID() == CPrinterException::ERROR_Printer_LESS_PAPER) ? PRINTER_NOPAPER :PRINTER_ERR	);
		}
		if(e.GetModuleID() == CRWException::MODULE_ID || e.GetModuleID() == CRWHardwareException::MODULE_ID){
			SetOuterRWStatus(RW_ERROR);
		}
		if(e.GetModuleID() == CTWException::MODULE_ID || e.GetModuleID() ==CTWHardwareException::MODULE_ID)
		{
			SetInnerRWStatus(RW_ERROR);
		}
		if(e.GetModuleID() == CTHException::MODULE_ID || e.GetModuleID() == CTWHardwareException::MODULE_ID)
		{
			SetTHModuleStatus(TH_MODULE_ERR);
		}
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_ERR,NULL,theEXCEPTION_MGR.GetActualMTC(e));
		SetRunStatus(WARNING);
		SetServiceStatus(theEXCEPTION_MGR.OnlyHasHardwareException() ? WARN_SERVICE : OUTOF_SERVICE);
		// 设备故障业务数据
		DEVICE_ERRORDATA errorData;
		unsigned long ulMtcCode = theEXCEPTION_MGR.GetActualMTC(e);
		unsigned short usEc = theEXCEPTION_MGR.GetEC(e);
		errorData.partType = theEXCEPTION_MGR.GetDevicePartTypeOfException(e);
		errorData.bDeviceType =  theEXCEPTION_MGR.GetDevicePartTypeOfException(e);                // 1 BIN 部件类型    
		if(errorData.bDeviceType!=DEVICE_COMPONENT::DEVICE_COMPOENT_UNKNOW
			)
		{
			int departIndex = 1;
			memcpy(errorData.devicePartID,&departIndex,3);
		}
		else
		{
			memset(errorData.devicePartID, 0xFF, 3);     // 3 BIN 部件ID  
		}
		errorData.deviceDeviderId = FOUNDER_ID;      // 2 BIN 设备供应商ID
		memcpy(errorData.maintanCode, &ulMtcCode, 4);// 4 1BIN+1BIN+1BIN+1BIN 故障代码   
		memcpy(errorData.ecode, &usEc, 2);           // 2 1BIN+1BIN 维修代码
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceErrorData(errorData);
	}
	else{
		if((!theEXCEPTION_MGR.HasException(CRWException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::MODULE_ID))){
			SetOuterRWStatus(RW_NORMAL);
		}
		if((!theEXCEPTION_MGR.HasException(CTWException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CTWHardwareException::MODULE_ID))){
			SetInnerRWStatus(RW_NORMAL);
		}
		if((!theEXCEPTION_MGR.HasException(CTHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID))){
			SetTHModuleStatus(TH_MODULE_NORMAL);
		}
		if(!theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID)){
			SetPrintStatus(PRINTER_NORMAL);
		}
		if(theEXCEPTION_MGR.HasException()){
			return;
		}
		SetRunStatus(RUNING);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_ERR,NULL,0);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_FULLSTATUS,STATUS_TYPE_FULL,NULL);
		bool hasLoginCommon = (theAPP_SESSION.GetUserInfo().sUserID != "") && (theAPP_SESSION.GetUserInfo().curAuthority == COMMON_AUTHORITY);
		SetServiceStatus(hasLoginCommon ? IN_SERVICE : OUTOF_SERVICE);
	}
}

CBomStatusMgr CBomStatusMgr::theInstance;
