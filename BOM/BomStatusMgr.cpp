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
@brief      ���캯��
@param      ��
@retval     CBomStatusMgr&    CBomStatusMgrʵ������
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBomStatusMgr::CBomStatusMgr()
	:m_sServiceStatus(OUTOF_SERVICE)         // ����״̬
	,m_sRunStatus(RUNING)              // ����״̬
	,m_sWorkMode(ISSUE_ADJUST_MODE)          // ����ģʽ
	,m_sRunMode(DEVICE_MODE_NORMAL)		//ϵͳģʽ
	,m_s24HourMode(0x00)												//24Сʱģʽ
	,m_sDelayTime(0x00)													//�ӳ���Ӫʱ��
	,m_sConnectStatus(DISCONNECT)            // ����״̬
	,m_sTimeStatus(VALID_TIME)               // ʱ�Ӳ���
	,m_sLoginStatus(LOGOUT)                  // ��¼״̬
	,m_sAutorunStatus(AUTORUN_STOP)          // �豸�Զ�����״̬
	,m_sMagazineAStatus(STATUS_MAGAZINE_SET) // Ʊ��1��װ״̬
	,m_sMagANumStatus(MAG_NUM_NORMAL)        // Ʊ��1����״̬
	,m_sMagazineBStatus(STATUS_MAGAZINE_SET) // Ʊ��2��װ״̬
	,m_sMagBNumStatus(MAG_NUM_NORMAL)        // Ʊ��2����״̬
	,m_sCycleBoxStatus(STATUS_MAGAZINE_SET)  // ��Ʊ�䰲װ״̬
	,m_sCycleBoxNumStatus(MAG_NUM_NORMAL)    // ��Ʊ������״̬
	,m_sRW1Status(RW_NORMAL)                 // ��д��1״̬
	,m_sRW2Status(RW_NORMAL)                 // ��д��2״̬
	,m_sSam1Type(YPT_PSAM)                   // SAM1����
	,m_sSam2Type(YKT_PSAM)                   // SAM2����
	,m_sSam3Type(YPT_PSAM)                   // SAM3����
	,m_sSam1Status(ISAM_UNAUTH)              // ISAM1״̬
	,m_sSam2Status(ISAM_UNAUTH)              // ISAM2״̬
	,m_sSam3Status(ISAM_UNAUTH)              // ISAM2״̬
	,m_sDoorStatus(DOOR_NORMAL_CLOSE)        // ά����״̬
	,m_sPrintStatus(PRINTER_NORMAL)          // ��ӡ��״̬     
	,m_sThModuleStatus(TH_MODULE_NORMAL)     // THģ�����
{
	theEXCEPTION_MGR.ExceptionItemChanged.AddHandler(this,&CBomStatusMgr::OnExceptionItemNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������
@param      ��
@retval     CBomStatusMgr&    CBomStatusMgrʵ������
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBomStatusMgr::~CBomStatusMgr()
{
	theEXCEPTION_MGR.ExceptionItemChanged.RemoveHandler(this,&CBomStatusMgr::OnExceptionItemNotify);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����Ķ���
@param      ��
@retval     CBomStatusMgr&    CBomStatusMgrʵ������
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBomStatusMgr& CBomStatusMgr::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���÷���״̬ 
@param      SERVICE_STATUS    status
@retval     ��
@exception  ��
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
@brief      ��ȡ����״̬
@param      ��
@retval     SERVICE_STATUS    status
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATUS CBomStatusMgr::GetServiceStatus()
{
	return m_sServiceStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������״̬
@param      RUN_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡ����״̬
@param      ��
@retval     RUN_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RUN_STATUS CBomStatusMgr::GetRunStatus()
{
	return m_sRunStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ù���ģʽ
@param      WORD_MODE
@retval     ��
@exception  ��
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
				strcpy(pDetail, "��Ʊģʽ");
				break;
			case ADJUST_MODE:
				strcpy(pDetail, "��Ʊģʽ");
				break;
			case ISSUE_ADJUST_MODE:
				strcpy(pDetail, "�۲�Ʊģʽ");
				break;
			default:
				strcpy(pDetail, "δ֪ģʽ");
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
@brief      ��ȡ����ģʽ
@param      ��
@retval     WORD_MODE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORK_MODE CBomStatusMgr::GetWorkMode()
{
	return m_sWorkMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ӫģʽ
@param      WORD
@retval     ��
@exception  ��
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
@brief      ��ȡ��Ӫģʽ
@param      ��
@retval     WORD
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
OperationalMode_t CBomStatusMgr::GetRunMode()
{
	return m_sRunMode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ����Ӫģʽ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CBomStatusMgr::GetRunModeName()
{
	switch(m_sRunMode)
	{
	case DEVICE_MODE_NORMAL: return _T("����ģʽ");
	case DEVICE_MODE_CLOSED : return _T("�ر�ģʽ");
	case DEVICE_MODE_TRAIN_FAULT: return _T("�г�����ģʽ");
	case DEVICE_MODE_CHECK_FREE_ENTRY: return _T("��վ���");
	case DEVICE_MODE_CHECK_FREE_EXIT: return _T("��վ���");
	case DEVICE_MODE_EMERGENCY_EXIT:return _T("����ģʽ");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME: return _T("ʱ�����");
	case DEVICE_MODE_NORMAL_CHECK_FREE_DATE: return _T("�������");
	case DEVICE_MODE_NORMAL_CHECK_FREE_FARE: return _T("�������");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE : return _T("����+ʱ�����");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME_FARE: return _T("ʱ��+�������");
	case DEVICE_MODE_NORMAL_CHECK_FREE_DATE_FARE: return _T("����+�������");
	case DEVICE_MODE_NORMAL_CHECK_FREE_TIME_DATE_FARE: return _T("����+ʱ��+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME:	return _T("��վ+ʱ�����");
	case DEVICE_MODE_CHECK_FREE_ENTRY_DATE:	return	_T("��վ+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_FARE:	return _T("��վ+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE: return _T("��վ+����+ʱ�����");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME_FARE : return _T("��վ+ʱ��+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_DATE_FARE: return  _T("��վ+����+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_TIME_DATE_FARE: return _T("��վ+ʱ��+����+�������");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME: return _T("��վ+ʱ�����");
	case DEVICE_MODE_CHECK_FREE_EXIT_DATE: return _T("��վ+�������");
	case DEVICE_MODE_CHECK_FREE_EXIT_FARE: return _T("��վ+�������");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE: return _T("��վ+ʱ��+�������");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME_FARE: return _T("��վ+ʱ��+�������");
	case DEVICE_MODE_CHECK_FREE_EXIT_DATE_FARE: return _T("��վ+����+�������");
	case DEVICE_MODE_CHECK_FREE_EXIT_TIME_DATE_FARE: return _T("��վ+ʱ��+����+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME: return _T("��վ+ʱ��+��վ���");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE: return _T("��վ+��վ+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_FARE: return _T("��վ+��վ+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE: return _T("��վ+��վ+ʱ��+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_FARE: return _T("��վ+��վ+ʱ��+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_DATE_FARE: return _T("��վ+��վ+����+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT_TIME_DATE_FARE: return _T("��վ+��վ+ʱ��+����+�������");
	case DEVICE_MODE_CHECK_FREE_ENTRY_EXIT: return _T("��վ+��վ���");
	default:
		return _T("δ֪ģʽ");
	};
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����24Сʱ��Ӫģʽ
@param      byte
@retval     ��
@exception  ��
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
		strcpy(pDetail, b24HourMode == 0x00 ?" ��24Сʱ��Ӫ":"24Сʱ��Ӫ");
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
@brief      ��ȡ24Сʱ��Ӫģʽ
@param      ��
@retval     WORD
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BYTE CBomStatusMgr::Get24HourMode()
{
	return m_s24HourMode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     �����ӳ���Ӫʱ��
@param      WORD
@retval     ��
@exception  ��
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
			//if(delayTime == 0)//ȡ���ӳ���Ӫ
			//{
			//	COleDateTime currentTime = COleDateTime::GetCurrentTime();
			//	CAfcDeviceTimeParam::OPRATION_TIME operationTime;
			//	theAFC_DEV_TIME.GetOperationTimeParam(operationTime);
			//	COleDateTime defaultSwitchTime = COleDateTime::GetCurrentTime();
			//	defaultSwitchTime.SetTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);
			//	bool isEnterDelayPeriod = currentTime>defaultSwitchTime;
			//	if(isEnterDelayPeriod)//�Ѿ������ӳ���Ӫʱ��Σ��򲻴���
			//	{
			//		return;
			//	}
			//	helper.SetBusinessTime(cmdSetTime,rspStatus);
			//	twhelper.SetBusinessTime(cmdSetTime,rspStatus);
			//	ComSetBusiDateSwitchTime(operationTime.oprationDateChangeTime.bHour,operationTime.oprationDateChangeTime.bMin,0);//������Ӫ���л�ʱ���
			//}
			//else//�����ӳ���Ӫ
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
			strcpy(pDetail, delayTime == 0 ?" ȡ���ӳ���Ӫ":"�����ӳ���Ӫ");
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
@brief     ȡ���ӳ���Ӫʱ��
@param      ��
@retval     WORD
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CBomStatusMgr::GetDelayTime()
{
	return m_sDelayTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ͨ��״̬
@param      COMMUNICATION
@retval     ��
@exception  ��
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
					strcpy(pDetail, " ͨѶ�ж�");
					break;
				case CONNECTED:
					strcpy(pDetail, "ͨѶ����");
					break;
				case CONNECTING:
					strcpy(pDetail, "ͨѶ�ָ���");
					break;
				default:
					strcpy(pDetail, "δ֪״̬");
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
@brief      ��ȡͨ��״̬
@param      ��
@retval     COMMUNICATION
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COMMUNICATION CBomStatusMgr::GetCommunicate()
{
	return m_sConnectStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ʱ��ͬ��״̬
@param      TIME_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡʱ��ͬ��״̬
@param      ��
@retval     TIME_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
TIME_STATUS CBomStatusMgr::GetTimeSynStatus()
{
	return m_sTimeStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�¼״̬
@param      LOGIN_STATUS
@retval     ��
@exception  ��
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
		strcpy(pDetail, (m_sLoginStatus== LOGIN)?"��¼":"�ǳ�");
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
@brief      ��ȡ��¼״̬
@param      ��
@retval     LOGIN_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LOGIN_STATUS CBomStatusMgr::GetLoginStatus()
{
	return m_sLoginStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�Զ�����
@param      ��
@retval     LOGIN_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_AUTORUN CBomStatusMgr::GetAutoRunStatus()
{
	return m_sAutorunStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����A��Set״̬
@param      STATUS_MAGAZINE
@retval     ��
@exception  ��
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
@brief      ��ȡA��Set״̬
@param      ��
@retval     STATUS_MAGAZINE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CBomStatusMgr::GetMagAStatus()
{
	return m_sMagazineAStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����A������״̬
@param      MAGAZINE_NUM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡA������״̬
@param      ��
@retval     MAGAZINE_NUM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CBomStatusMgr::GetMagANumStatus()
{
	return m_sMagANumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����B��Set״̬
@param      STATUS_MAGAZINE
@retval     ��
@exception  ��
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
@brief      ��ȡB��Set״̬
@param      ��
@retval     STATUS_MAGAZINE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CBomStatusMgr::GetMagBStatus()
{
	return m_sMagazineBStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����B������״̬
@param      MAGAZINE_NUM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡB������״̬
@param      ��
@retval     MAGAZINE_NUM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CBomStatusMgr::GetMagBNumStatus()
{
	return  m_sMagBNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���û�����Set״̬
@param      STATUS_MAGAZINE
@retval     ��
@exception  ��
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
@brief      ��ȡ������Set״̬
@param      ��
@retval     STATUS_MAGAZINE
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CBomStatusMgr::GetCycleBoxStatus()
{

	return m_sCycleBoxStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���û�������״̬
@param      MAGAZINE_NUM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡ����������״̬
@param      ��
@retval     MAGAZINE_NUM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CBomStatusMgr::GetCycleBoxNumStatus()
{
	return m_sCycleBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ڲ�RW״̬
@param      STATUS_RW
@retval     ��
@exception  ��
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
@brief      ��ȡ�ڲ�RW״̬
@param      ��
@retval     STATUS_RW
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CBomStatusMgr::GetInnerRWStatus()
{
	return m_sRW1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ⲿRW״̬
@param      STATUS_RW
@retval     ��
@exception  ��
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
@brief      ��ȡ�ڲ�RW״̬
@param      ��
@retval     STATUS_RW
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CBomStatusMgr::GetOuterRWStatus()
{
	return m_sRW2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam1����
@param      SAM_TYPE_
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam1Type(SAM_TYPE_ type)
{
	m_sSam1Type = type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSam1����
@param      ��
@retval     SAM_TYPE_
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_TYPE_ CBomStatusMgr::GetSam1Type()
{
	return m_sSam1Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam2����
@param      SAM_TYPE_
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam2Type(SAM_TYPE_ type)
{
	m_sSam2Type = type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSam2����
@param      ��
@retval     SAM_TYPE_
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_TYPE_ CBomStatusMgr::GetSam2Type()
{
	return m_sSam2Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam3����
@param      SAM_TYPE_
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetSam3Type(SAM_TYPE_ type)
{
	m_sSam3Type = type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSam3����
@param      ��
@retval     SAM_TYPE_
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_TYPE_ CBomStatusMgr::GetSam3Type()
{
	return m_sSam3Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam1��֤״̬(�ⲿ��д��ACC ISAM)
@param      SAM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡSam1��֤״̬
@param      ��
@retval     SAM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CBomStatusMgr::GetSam1Status()
{
	return m_sSam1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam2��֤״̬(�ⲿ��д��YKT ISAM)
@param      SAM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡSam2��֤״̬
@param      ��
@retval     SAM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CBomStatusMgr::GetSam2Status()
{
	return m_sSam2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam3��֤״̬(�ڲ���д��ACC ISAM)
@param      SAM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡSam3��֤״̬
@param      ��
@retval     SAM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CBomStatusMgr::GetSam3Status()
{
	return m_sSam3Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ά����״̬
@param      DOOR_STATUS
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::SetDoorStatus(DOOR_STATUS status)
{
	m_sDoorStatus = status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡά����״̬
@param      ��
@retval     DOOR_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DOOR_STATUS CBomStatusMgr::GetDoorStatus()
{
	return m_sDoorStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô�ӡ��״̬
@param      PRINTER_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡ��ӡ��״̬
@param      ��
@retval     PRINTER_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PRINTER_STATUS CBomStatusMgr::GetPrintStatus()
{
	return m_sPrintStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����THģ��״̬
@param      TH_MODULE_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡTHģ��״̬
@param      ��
@retval     SAM_STATUS
@exception  ��
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
@brief      �쳣���з����仯ʱ��Ӧ����

@param      (i)CSysException& e �쳣����
@param      (i)bool isInsert      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBomStatusMgr::OnExceptionItemNotify(CSysException& e,bool isInsert)
{
	if(isInsert){
		// ��ά��״̬ʱ������BOM״̬����
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
		// �豸����ҵ������
		DEVICE_ERRORDATA errorData;
		unsigned long ulMtcCode = theEXCEPTION_MGR.GetActualMTC(e);
		unsigned short usEc = theEXCEPTION_MGR.GetEC(e);
		errorData.partType = theEXCEPTION_MGR.GetDevicePartTypeOfException(e);
		errorData.bDeviceType =  theEXCEPTION_MGR.GetDevicePartTypeOfException(e);                // 1 BIN ��������    
		if(errorData.bDeviceType!=DEVICE_COMPONENT::DEVICE_COMPOENT_UNKNOW
			)
		{
			int departIndex = 1;
			memcpy(errorData.devicePartID,&departIndex,3);
		}
		else
		{
			memset(errorData.devicePartID, 0xFF, 3);     // 3 BIN ����ID  
		}
		errorData.deviceDeviderId = FOUNDER_ID;      // 2 BIN �豸��Ӧ��ID
		memcpy(errorData.maintanCode, &ulMtcCode, 4);// 4 1BIN+1BIN+1BIN+1BIN ���ϴ���   
		memcpy(errorData.ecode, &usEc, 2);           // 2 1BIN+1BIN ά�޴���
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
