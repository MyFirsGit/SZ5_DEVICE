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
@brief      ���캯��

@param    none

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStatusHelper::CStatusHelper()
	:m_sServiceStatus(OUTOF_SERVICE)									// ����״̬
	,m_sRunStatus(RUNING)												// ����״̬
	,m_sWorkMode(DEVICE_CONTRL_CODE_BUSINESS_ALL)						// ����ģʽ
	,m_OperationMode(NORMAL_SERVICE_MODE)								// ��Ӫģʽ
	,m_s24HourMode(OPERATION_24HOURS_OFF)								// 24Сʱģʽ
	,m_sDelayTime(0)													// �ӳ���Ӫʱ��
	,m_sConnectStatus(DISCONNECT)										// ����״̬
	,m_sTimeStatus(VALID_TIME)											// ʱ�Ӳ���
	,m_sLoginStatus(LOGOUT)												// ��¼״̬
	,m_sAutorunStatus(AUTORUN_START)									// �豸�Զ�����״̬
	,m_lDeviceErrStatus(0)												// �豸����״̬
	,m_sTransferAudit(TRANSFER_AUDIT_STATUS_NORMAL)						// �������״̬
	//,m_sPaymentMode(DEVICE_CONTRL_CODE_PAYMENT)						// ֧��ģʽ 
	,m_sPrintMode(0)													// ��ӡģʽ
	//,m_sChangeMode(DEVICE_CONTRL_CODE_ALL_CHANGE)						// ����ģʽ
	,m_sDiskSpaceStatus(DISK_SPACE_NORMAL)								// ���̿ռ�״̬
	,m_sTokenBox1Status(STATUS_MAGAZINE_SET)							// Token������1��װ״̬
	,m_sTokenBox1NumStatus(MAG_NUM_NORMAL) 								// Token������1����״̬���ݲ�ʹ�ã�
	,m_sTokenBox2Status(STATUS_MAGAZINE_SET)							// Token������2��װ״̬
	,m_sTokenBox2NumStatus(MAG_NUM_NORMAL) 								// Token������2����״̬���ݲ�ʹ�ã�
	,m_sTokenHopper1Status(STATUS_MAGAZINE_SET)							// Token Hopper��1��װ״̬���ݲ�ʹ�ã�
	,m_sTokenHopper1NumStatus(MAG_NUM_NORMAL) 							// Token Hopper��1����״̬
	,m_sTokenHopper2Status(STATUS_MAGAZINE_SET)							// Token Hopper��2��װ״̬���ݲ�ʹ�ã�
	,m_sTokenHopper2NumStatus(MAG_NUM_NORMAL) 							// Token Hopper��2����״̬
	,m_sTokenWasteBoxStatus(STATUS_MAGAZINE_SET)						// Token��Ʊ�䰲װ״̬
	,m_sTokenWasteBoxNumStatus(MAG_NUM_NORMAL) 							// Token��Ʊ������״̬
	,m_sCardBox1Status(STATUS_MAGAZINE_SET)								// Card��1��װ״̬
	,m_sCardBox1NumStatus(MAG_NUM_NORMAL) 								// Card��1����״̬
	,m_sCardBox2Status(STATUS_MAGAZINE_SET)								// Card��2��װ״̬
	,m_sCardBox2NumStatus(MAG_NUM_NORMAL) 								// Card��2����״̬
	,m_sCardWasteBoxStatus(STATUS_MAGAZINE_SET)							// Card��Ʊ�䰲װ״̬
	,m_sCardWasteBoxNumStatus(MAG_NUM_NORMAL) 							// Card��Ʊ������״̬
	,m_sCoinBox1Status(STATUS_MAGAZINE_SET)								// Coin������1��װ״̬
	,m_sCoinBox1NumStatus(MAG_NUM_NORMAL) 								// Coin������1����״̬
	,m_sCoinBox2Status(STATUS_MAGAZINE_SET)								// Coin������2��װ״̬
	,m_sCoinBox2NumStatus(MAG_NUM_NORMAL) 								// Coin������2����״̬
	,m_sCoinHopper1Status(STATUS_MAGAZINE_SET)							// Coin Hopper��1��װ״̬���ݲ�ʹ�ã�
	,m_sCoinHopper1NumStatus(MAG_NUM_NORMAL) 							// Coin Hopper��1����״̬
	,m_sCoinHopper2Status(STATUS_MAGAZINE_SET)							// Coin Hopper��2��װ״̬���ݲ�ʹ�ã�
	,m_sCoinHopper2NumStatus(MAG_NUM_NORMAL) 							// Coin Hopper��2����״̬
	,m_sCoinCircleHopperStatus(STATUS_MAGAZINE_SET)						// Coin ѭ������Hopper�䰲װ״̬���ݲ�ʹ�ã�
	,m_sCoinCircleHopperNumStatus(MAG_NUM_NORMAL) 						// Coin ѭ������Hopper������״̬
	,m_sCoinCollectionBox1Status(STATUS_MAGAZINE_SET)					// Coin������1��װ״̬
	,m_sCoinCollectionBox1NumStatus(MAG_NUM_NORMAL) 					// Coin������1����״̬
	,m_sCoinCollectionBox2Status(STATUS_MAGAZINE_SET)					// Coin������2��װ״̬
	,m_sCoinCollectionBox2NumStatus(MAG_NUM_NORMAL) 					// Coin������2����״̬
	,m_sBanknoteCollectionBoxStatus(STATUS_MAGAZINE_SET)				// ֽ�һ����䰲װ״̬
	,m_sBanknoteCollectionBoxNumStatus(MAG_NUM_NORMAL) 					// ֽ�һ���������״̬
	,m_sSecurityDoorStatus(SECURITY_DOOR_STATUS_NORMAL_CLOSE)			// �ֽ�ȫ��״̬���ݲ�ʹ�ã�
	,m_sTouchScreenStatus	(TOUCH_SCREEN_STATUS_NORMAL)				// ������״̬���ݲ�ʹ�ã�
	,m_sAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS_UNPRESSED)		// ��Ԯ��ť״̬
	,m_sRechargeRWStatus(RW_NORMAL)										// ��ֵ��д��״̬
	,m_sCardRWStatus(RW_NORMAL)											// Card��д��״̬
	,m_sTokenRWStatus(RW_NORMAL)										// Token��д��״̬
	,m_sSAM1Status(ISAM_UNAUTH)											// SAM1״̬(��ֵ��д��һƱͨ ISAM)
	,m_sSAM2Status(ISAM_UNAUTH)											// SAM2״̬(Card��д��һƱͨ ISAM)
	,m_sTokenRWYPTISAMStatus(SAM_STATUS_LRT_NORMAL)						// SAM3״̬(Token��д��һƱͨ ISAM)
	,m_sRechargeRWYKTISAMStatus(SAM_STATUS_MACAUPASS_NORMAL)			// SAM4״̬(��ֵ��д��һ��ͨISAM)(�ݲ�ʹ��)
	,m_sPrintStatus(PRINTER_NORMAL)										// ��ӡ��״̬
	,m_sUPSStatus(UPS_STATUS_CLOSE)										// UPS״̬
	,m_sMaintenanceDoorStatus(DOOR_NORMAL_CLOSE)						// ά����״̬
	,m_sBanknoteModuleStatus(HARDWARE_MODULE_NORMAL)					// ֽ��ģ��״̬
	,m_sTokenModuleStatus(HARDWARE_MODULE_NORMAL)						// Tokenģ��״̬
	,m_sCardModuleStatus(HARDWARE_MODULE_NORMAL)						// Cardģ��״̬
	,m_sCoinModuleStatus(HARDWARE_MODULE_NORMAL)						// Ӳ��ģ��״̬
	,m_sStatusDisplayStatus(HARDWARE_MODULE_NORMAL)						// ״̬��ʾ��״̬
	,m_sTiltStatus(TILT_STATUS_OFF)										// ��б������״̬(�𶯴�����״̬)
	,m_sOffLineDays(0)													// ��������
	,m_sAuthorizedAccessStatus(AUTHORIZED_STATUS_NORMAL)				// ��Ȩ����״̬ 
	,m_sDegradedMode(DEVICE_CONTRL_CODE_SC_DE_OPER_ON)					// �Զ��������п���
	,m_sBanknoteChangeBox1NumStatus(MAG_NUM_NORMAL)						// ֽ��������A����״̬
	,m_sBanknoteChangeBox2NumStatus(MAG_NUM_NORMAL)						// ֽ��������B����״̬

	,m_sBanknoteRE1NumStatus(MAG_NUM_NORMAL)							// ֽ��ѭ����1����״̬
	,m_sBanknoteRE2NumStatus(MAG_NUM_NORMAL)							// ֽ��ѭ����2����״̬
	,m_sBanknoteRE3NumStatus(MAG_NUM_NORMAL)							// ֽ��ѭ����3����״̬
	,m_sBanknoteRE4NumStatus(MAG_NUM_NORMAL)							// ֽ��ѭ����4����״̬
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
@brief      ��������

@param    none

@retval     none

@exception  ��
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
@brief      ��ȡ����Ķ���

@param   none

@retval     CStatusHelper&    CStatusHelperʵ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStatusHelper& CStatusHelper::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���÷���״̬ 

@param   (i) SERVICE_STATUS status ����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetServiceStatus(SERVICE_STATUS status)
{
	try{
		// ֹͣ����ֻ���л���������
		if(m_sServiceStatus == STOP_SERVICE && status != IN_SERVICE){
			theSTATUS_DISPLAYER.showTVMStatus(STOP_SERVICE);
			theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show();
			return;
		}
		// ���SC�����������豸״̬�������
		if(IsSCConnected()){
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::SERVICE_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}

		if(m_sServiceStatus != status){//״̬��һ��ʱ�����ã�ͨѶʱ�����
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
			
			
		// �豸��Ҫ״̬���ҵ�����ݣ�����Ԥ����
		//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgImportantStatusChange(m_sServiceStatus);

		// ������ʾ����ͣ�����ֹͣ����
		//if (OUTOF_SERVICE == status && STOP_SERVICE != m_sServiceStatus || STOP_SERVICE == status){
		//	try{
		//		theSTATUS_DISPLAYER.showTVMStatus(status);
		//	}
		//	catch(CSysException& e){
		//		theEXCEPTION_MGR.ProcessException(e);
		//	}
		//}
		//else if(MANT_SERVICE == status && m_sServiceStatus != STOP_SERVICE){// ά��ģʽҪ��ʾ��ͣ����
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
@brief      ��ȡ����״̬

@param    none

@retval     SERVICE_STATUS   ��ǰ����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATUS CStatusHelper::GetServiceStatus()
{
	return m_sServiceStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������״̬

@param    (i)RUN_STATUS status ����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetRunStatus(RUN_STATUS status)
{
	try{
		if(m_sRunStatus == status) return;

		m_sRunStatus = status;
		// ���SC�����������豸״̬�������
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RUN_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// ��ʾ���ڹػ�...
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
@brief      ��ȡ����״̬

@param   ��

@retval     RUN_STATUS ��ǰ����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RUN_STATUS CStatusHelper::GetRunStatus()
{
	return m_sRunStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ù���ģʽ

@param   (i)WORD_MODE ����ģʽ
@param   (i)bool bManual �Ƿ����ֶ�����

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetWorkMode(WORD mode, bool bManual)
{
	try{
		
		// ��λ�·���״̬��������˲�����ֱ������Ĭ�Ϸ����ж�״̬
		if(bManual){
			if(!theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->IsServiceBusyOnPassger()){
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
			}
			return;
		}
		
		if (m_sWorkMode != mode)
		{
			m_sWorkMode = mode;
			// ���SC�����������豸״̬�������
			if(IsSCConnected())	{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::WORK_MODE,GetDegreeWorkMode());
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
			}
			if ((mode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE) == DEVICE_CONTRL_CODE_BUSINESS_CHARGE)
			{
				theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
			}

			// ����״̬��ʾ����ʾ���ݼ�������ʾ����
			theSTATUS_DISPLAYER.showWorkMode();

		}
		//m_sWorkMode = mode;
		//// ��¼������־
		///*OPERATION_CODE operCode;
		//if(bManual == true){
		//operCode = OPERATION_MODE_SET;
		//}
		//else{
		//operCode = OPERATION_MODE_NOTIC;
		//}
		//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(operCode);*/

		//// ����״̬��ʾ����ʾ���ݼ�������ʾ����
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
@brief			��ȡ����ģʽ

@param		none

@retval			BYTE ����ģʽ����

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetWorkMode()
{
	return m_sWorkMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ӫģʽ

@param   (i)OperationalMode_t   ��Ӫģʽ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetOperationMode(OperationalMode_t mode)
{
	try{
		if(m_OperationMode == mode)
			return;
		if (mode != NORMAL_SERVICE_MODE && mode != EMERGENCY_MODE && mode != TRAIN_SERVICE_DISRUPTION_MODE)
		{
			//����ģʽֻ����״̬��TVM������Ӧ
			// ���SC�����������豸״̬�������
			if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::OPERATION_MODE,LOBYTE(mode));// ȡ��λ
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
				m_OperationMode = mode;
			}
			return;
		}
		m_OperationMode = mode;
		// ���½�����ʾ����Ӫģʽ
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_NEW_OPERATION_MODE,mode,NULL);
		// ���SC�����������豸״̬�������
		if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::OPERATION_MODE,LOBYTE(mode));// ȡ��λ
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// ����TPU��ǰ��Ӫģʽ		
		RW_CMD_SET_MODE rw_cmd_set_mode;
		if(NORMAL_SERVICE_MODE == mode)
			rw_cmd_set_mode.ControlCode = 0x02;// ���ģʽ
		else 
			rw_cmd_set_mode.ControlCode = 0x01;// ����ģʽ
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
@brief      ��ȡ��Ӫģʽ

@param   none

@retval     OperationalMode_t ��ǰ��Ӫģʽ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
OperationalMode_t CStatusHelper::GetOperationMode()
{
	return m_OperationMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����24Сʱ��Ӫģʽ

@param    (i)OPERATION_24HOURS_MODE b24HourMode 24Сʱ��Ӫģʽ

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::Set24HourMode(OPERATION_24HOURS_MODE b24HourMode)
{
	try{
		m_s24HourMode = b24HourMode;
		// ���SC�����������豸״̬�������
		if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::HOUR24MODE,(BYTE)b24HourMode);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// ����TPU 24Сʱ��Ӫģʽ
		RW_CMD_SET_TIME rw_cmd_set_time;
		rw_cmd_set_time.ControlCode = b24HourMode == OPERATION_24HOURS_OFF? 0x02 : 0x01;
		rw_cmd_set_time.Time = 0;
		RW_RSP_STATUS rw_rsp_status;
		CARDRW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);
///		TOKENRW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);
//		RECHARGERW_HELPER->SetBusinessTime(rw_cmd_set_time,rw_rsp_status);

		// ���������־
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
@brief      ��ȡ24Сʱ��Ӫģʽ

@param    none

@retval     OPERATION_24HOURS_MODE 24Сʱ��Ӫģʽ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
OPERATION_24HOURS_MODE CStatusHelper::Get24HourMode()
{
	return m_s24HourMode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ӳ���Ӫʱ��

@param    (i)OPERATION_DELAY_MODE delayTime  �ӳ���Ӫʱ��״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDelayTime(WORD delayTime)
{
	try{
		m_sDelayTime = delayTime;
		// ���SC�����������豸״̬�������
		if(IsSCConnected()|| theAPP_SESSION.GetPhysicalConnStatus())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DELAY_RUN_MODE,(delayTime==0)?0x00:0x01);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// ����TPU �ӳ���Ӫģʽ״̬
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
		// ���������־
		CString strOperComment = (delayTime == 0) ?_T("�ӳ���Ӫ��"):_T("�ӳ���Ӫ��");
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
@brief     ȡ���ӳ���Ӫʱ��

@param   none

@retval     OPERATION_DELAY_MODE �ӳ���Ӫʱ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetDelayTime()
{
	return m_sDelayTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ͨ��״̬

@param   (i)COMMUNICATION comm

@retval    none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCommunicate(COMMUNICATION comm)
{
	try{
		if (comm != m_sConnectStatus){
			m_sConnectStatus = comm;
			// ���SC�����������豸״̬�������
			if(IsSCConnected())	{
				CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CONNECT_STATUS,(BYTE)comm);
				theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
			}
			// ���������־
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
@brief      ��ȡͨ��״̬

@param   ��

@retval     COMMUNICATION ��ǰͨ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COMMUNICATION CStatusHelper::GetCommunicate()
{
	return m_sConnectStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         ����ʱ��ͬ��״̬

@param      TIME_STATUS

@retval        none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTimeSynStatus(TIME_STATUS status)
{
	try{
		m_sTimeStatus = status;
		// ���SC�����������豸״̬�������
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
@brief      ��ȡʱ��ͬ��״̬

@param   ��

@retval     TIME_STATUS ʱ��ͬ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
TIME_STATUS CStatusHelper::GetTimeSynStatus()
{
	return m_sTimeStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���õ�¼״̬

@param    (i)LOGIN_STATUS status ��¼״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetLoginStatus(LOGIN_STATUS status)
{
	try{
		if(m_sLoginStatus == status) return;

		m_sLoginStatus = status;
		// ���SC�����������豸״̬�������
		if(IsSCConnected())	{
			CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::LOGIN_STATUS,(BYTE)status);
			theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
		}
		// ���������־
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
@brief      ��ȡ��¼״̬

@param   ��

@retval     LOGIN_STATUS ��¼״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LOGIN_STATUS CStatusHelper::GetLoginStatus()
{
	return m_sLoginStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			�����Զ�����״̬

@param      (i)DEVICE_AUTORUN �Զ�����״̬

@retval       none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void  CStatusHelper::SetAutoRunStatus(DEVICE_AUTORUN AutorunStatus)
{
	try{
		if(m_sAutorunStatus == AutorunStatus) return;

		m_sAutorunStatus = AutorunStatus;
		// ���SC�����������豸״̬�������
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
@brief      ��ȡ�Զ�����״̬

@param   ��

@retval     DEVICE_AUTORUN �Զ�����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_AUTORUN CStatusHelper::GetAutoRunStatus()
{
	return theFunction_INFO.IsEquipmentAutomaticRunningOn()?AUTORUN_START:AUTORUN_STOP/*m_sAutorunStatus*/;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����豸����״̬

@param   (i)long lMtcCode �豸����״̬

@retval     ��

@exception  ��
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
@brief      ��ȡ�豸����״̬

@param      ��

@retval     long �豸����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CStatusHelper::GetDeviceErrStatus()
{
	return m_lDeviceErrStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����豸�������״̬

@param      TRANSFER_AUDIT_STATUS status_transfer_audit

@retval     ��

@exception  ��
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
@brief      ȡ��������豸״̬

@param      ��

@retval     TRANSFER_AUDIT_STATUS m_sTransferAudit;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
TRANSFER_AUDIT_STATUS CStatusHelper::GetDeviceTransferAudit(){
	return m_sTransferAudit;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ý�������ģʽ

@param      ��

@retval			none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDegradedMode(DEVICE_CONTRL_CODE degradedMode)
{
		m_sDegradedMode = degradedMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			�Ƿ������Զ���������

@param      none

@retval			bool true:�����Զ�����  false:�������Զ�����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusHelper::IsCanAutoDegraded()
{
	return (m_sDegradedMode == DEVICE_CONTRL_CODE_SC_DE_OPER_ON);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			����֧��ģʽ

@param       (i)WORD paymentMode ֧��ģʽ����,bool bIsDeviceControl;�Ƿ�Ϊ������

@retval			none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetPaymentMode(WORD paymentMode,bool bIsDeviceControl)
{
	try{
		if(m_sPaymentMode == paymentMode) return;

		m_sPaymentMode = paymentMode;
		// ���SC�����������豸״̬�������
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
@brief         ��ȡ֧��ģʽ

@param      ��

@retval       WORD ֧��ģʽ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetPaymentMode()
{
	return m_sPaymentMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			���ô�ӡģʽ

@param       (i)WORD printMode ��ӡģʽ����

@retval			none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetPrintMode(WORD printMode,bool bIsDeviceControl)
{
	try{
		if(m_sPrintMode == printMode)return;

		m_sPrintMode = printMode;

 		// ���SC�����������豸״̬�������
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
@brief      ��ȡ��ӡ��ģʽ���Ƿ�֧�ִ�ӡ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetPrintMode()
{
	return m_sPrintMode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief         �Ƿ����޴�ӡģʽ

@param      ��

@retval       bool true:�޴�ӡ  false:������ӡ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusHelper::IsNoPrintMode()
{
	return (m_sPrintMode == DEVICE_CONTRL_CODE_SC_NO_PRINT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief			��������ģʽ

@param       (i)WORD changeMode ����ģʽ����

@retval			none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetChangeMode(WORD changeMode,bool bIsDeviceControl)
{
	try{
		if(m_sChangeMode == changeMode)return;

		m_sChangeMode = changeMode;
		// ���SC�����������豸״̬�������
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
@brief			��ȡ����ģʽ

@param       none

@retval			WORD ����ģʽ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORD CStatusHelper::GetChangeMode()
{
	return m_sChangeMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô��̿ռ�״̬

@param    (i)DISK_SPACE_CODE DiskSpaceStatus

@retval     LOGIN_STATUS

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetDiskSpaceStatus(DISK_SPACE_CODE DiskSpaceStatus)
{
	m_sDiskSpaceStatus = DiskSpaceStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		//CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::DISC_SPACE_STATUS,(BYTE)DiskSpaceStatus);
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡ���̿ռ�״̬

@param      ��

@retval       DISK_SPACE_CODE ���̿ռ�״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DISK_SPACE_CODE CStatusHelper::GetDiskSpaceStatus()
{
	return m_sDiskSpaceStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token������1�䰲װ״̬

@param    (i)STATUS_MAGAZINE TokenBox1Status

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox1Status(STATUS_MAGAZINE TokenBox1Status)
{
	m_sTokenBox1Status = TokenBox1Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX1_STATUS,(BYTE)TokenBox1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken������1��װ״̬

@param      ��

@retval       STATUS_MAGAZINE Token������1��װ״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenAddBox1Status()
{
	return m_sTokenBox1Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token������1������״̬���ݲ�ʹ�ã�

@param    (i)MAGAZINE_NUM_STATUS TokenBox1NumStatus Token������1����״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox1NumStatus(MAGAZINE_NUM_STATUS TokenBox1NumStatus)
{
	m_sTokenBox1NumStatus = TokenBox1NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX1_COUNT,(BYTE)TokenBox1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken������1����״̬���ݲ�ʹ�ã�

@param      ��

@retval       MAGAZINE_NUM_STATUS Token������1����״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenAddBox1NumStatus()
{
	return m_sTokenBox1NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token������2��װ״̬

@param    (i)STATUS_MAGAZINE TokenBox2Status Token������2��װ״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox2Status(STATUS_MAGAZINE TokenBox2Status)
{
	m_sTokenBox2Status = TokenBox2Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX2_STATUS,(BYTE)TokenBox2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken������2�䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Token������2��װ״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenAddBox2Status()					
{
	return m_sTokenBox2Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token������2����״̬���ݲ�ʹ�ã�

@param    (i)MAGAZINE_NUM_STATUS TokenBox2NumStatus Token������2����״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenAddBox2NumStatus(MAGAZINE_NUM_STATUS TokenBox2NumStatus)
{
	m_sTokenBox2NumStatus = TokenBox2NumStatus ;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_BOX2_COUNT,(BYTE)TokenBox2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken������2����״̬���ݲ�ʹ�ã�

@param      ��

@retval       MAGAZINE_NUM_STATUS Token������2����״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenAddBox2NumStatus()
{
	return m_sTokenBox2NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token Hopper1�䰲װ״̬���ݲ�ʹ�ã�

@param    (i)STATUS_MAGAZINE TokenHopper1Status Token Hopper1�䰲װ״̬

@retval     LOGIN_STATUS

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper1Status(STATUS_MAGAZINE TokenHopper1Status)
{
	m_sTokenHopper1Status = TokenHopper1Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER1_STATUS,(BYTE)TokenHopper1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief         ��ȡToken Hopper1�䰲װ״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_MAGAZINE Token Hopper1�䰲װ״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenHopper1Status()
{
	return m_sTokenHopper1Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token Hopper1������״̬

@param    (i)MAGAZINE_NUM_STATUS TokenHopper1NumStatus Token Hopper1������״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper1NumStatus(MAGAZINE_NUM_STATUS TokenHopper1NumStatus) 
{
	m_sTokenHopper1NumStatus = TokenHopper1NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER1_COUNT,(BYTE)TokenHopper1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken Hopper1������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Token Hopper1������״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenHopper1NumStatus()
{
	return m_sTokenHopper1NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token Hopper2�䰲װ״̬���ݲ�ʹ�ã�

@param    (i)STATUS_MAGAZINE TokenHopper2Status Token Hopper2�䰲װ״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper2Status(STATUS_MAGAZINE TokenHopper2Status)
{
	m_sTokenHopper2Status = TokenHopper2Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER2_STATUS,(BYTE)TokenHopper2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken Hopper2�䰲װ״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_MAGAZINE Token Hopper2�䰲װ״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenHopper2Status()
{
	return m_sTokenHopper2Status;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token Hopper2������״̬

@param    (i)MAGAZINE_NUM_STATUS TokenHopper2NumStatus Token Hopper2������״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenHopper2NumStatus(MAGAZINE_NUM_STATUS TokenHopper2NumStatus)
{
	m_sTokenHopper2NumStatus = TokenHopper2NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_HOPPER2_COUNT,(BYTE)TokenHopper2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken Hopper2������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Token Hopper2������״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenHopper2NumStatus()
{
	return m_sTokenHopper2NumStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token��Ʊ�䰲װ״̬

@param    (i)STATUS_MAGAZINE TokenWasteBoxStatus Token��Ʊ�䰲װ״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenWasteBoxStatus(STATUS_MAGAZINE TokenWasteBoxStatus)
{
	m_sTokenWasteBoxStatus = TokenWasteBoxStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_CYCLEBOX_STATUS,(BYTE)TokenWasteBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken��Ʊ�䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Token��Ʊ�䰲װ״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenWasteBoxStatus()
{
	return m_sTokenWasteBoxStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token��Ʊ������״̬

@param    (i)MAGAZINE_NUM_STATUS TokenWasteBoxNumStatus Token��Ʊ������״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenWasteBoxNumStatus(MAGAZINE_NUM_STATUS TokenWasteBoxNumStatus)
{
	m_sTokenWasteBoxNumStatus = TokenWasteBoxNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_CYCLEBOX_COUNT,(BYTE)TokenWasteBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken��Ʊ������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Token��Ʊ������״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenWasteBoxNumStatus()
{
	return m_sTokenWasteBoxNumStatus;
}


////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token�����䰲װ״̬

@param    (i)STATUS_MAGAZINE TokenCollectedBoxStatus Token�����䰲װ״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenCollectedBoxStatus(STATUS_MAGAZINE TokenCollectedBoxStatus)
{
	m_sTokenCollectedBoxStatus = TokenCollectedBoxStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_COLLBOX_STATUS,(BYTE)TokenCollectedBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken�����䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Token�����䰲װ״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetTokenCollectedBoxStatus()
{
	return m_sTokenCollectedBoxStatus;
}

////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token����������״̬

@param    (i)MAGAZINE_NUM_STATUS TokenCollectedBoxNumStatus Token����������״̬

@retval     none

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTokenCollectedBoxNumStatus(MAGAZINE_NUM_STATUS TokenCollectedBoxNumStatus)
{
	m_sTokenCollectedBoxNumStatus = TokenCollectedBoxNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_COLLEBOX_COUNT,(BYTE)TokenCollectedBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken����������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Token����������״̬

@exception  ��
*/
////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetTokenCollectedBoxNumStatus()
{
	return m_sTokenCollectedBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card��1��װ״̬

@param    (i)STATUS_MAGAZINE CardBox1Status Card��1��װ״̬

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox1Status(STATUS_MAGAZINE CardBox1Status)
{
	// ״̬��ͬ�����ϱ�
	if(m_sCardBox1Status == CardBox1Status)return;

	m_sCardBox1Status = CardBox1Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected()){
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX1_STATUS,(BYTE)CardBox1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCard��1�䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Card��1�䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCardBox1Status()
{
	return m_sCardBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card��1����״̬

@param    (i)MAGAZINE_NUM_STATUS CardBox1NumStatus Card��1����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox1NumStatus(MAGAZINE_NUM_STATUS CardBox1NumStatus)
{
	// ״̬��ͬ����Ҫ�ٴ��ϱ�״̬
	if(m_sCardBox1NumStatus == CardBox1NumStatus) return;

	m_sCardBox1NumStatus = CardBox1NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX1_COUNT,(BYTE)CardBox1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCard��1������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Card��1������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCardBox1NumStatus()
{
	return m_sCardBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card��2�䰲װ״̬

@param    (i)STATUS_MAGAZINE CardBox2Status Card��2�䰲װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox2Status(STATUS_MAGAZINE CardBox2Status)
{
	if(m_sCardBox2Status == CardBox2Status)return;

	m_sCardBox2Status = CardBox2Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX2_STATUS,(BYTE)CardBox2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCard��2�䰲װ״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_MAGAZINE Card��2�䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCardBox2Status()					
{
	return m_sCardBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card��2������״̬

@param    (i)MAGAZINE_NUM_STATUS CardBox2NumStatus Card��2������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardBox2NumStatus(MAGAZINE_NUM_STATUS CardBox2NumStatus)
{
	if(m_sCardBox2NumStatus == CardBox2NumStatus ) return;

	m_sCardBox2NumStatus = CardBox2NumStatus ;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_BOX2_COUNT,(BYTE)CardBox2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCard��2������״̬���ݲ�ʹ�ã�

@param      ��

@retval       MAGAZINE_NUM_STATUS Card��2������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCardBox2NumStatus()
{
	return m_sCardBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card�����䰲װ״̬

@param    (i)STATUS_MAGAZINE CardWasteBoxStatus Card�����䰲װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardWasteBoxStatus(STATUS_MAGAZINE CardWasteBoxStatus)
{
	if(m_sCardWasteBoxStatus == CardWasteBoxStatus)return;
	m_sCardWasteBoxStatus = CardWasteBoxStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_CYCLEBOX_STATUS,(BYTE)CardWasteBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCard�����䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Card�����䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCardWasteBoxStatus()
{
	return m_sCardWasteBoxStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card����������״̬

@param    (i)MAGAZINE_NUM_STATUS CardWasteBoxNumStatus Card����������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCardWasteBoxNumStatus(MAGAZINE_NUM_STATUS CardWasteBoxNumStatus)
{
	if(m_sCardWasteBoxNumStatus == CardWasteBoxNumStatus)return;

	m_sCardWasteBoxNumStatus = CardWasteBoxNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_CYCLEBOX_COUNT,(BYTE)CardWasteBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCard����������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Card����������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCardWasteBoxNumStatus()
{
	return m_sCardWasteBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������1�䰲װ״̬

@param    (i)STATUS_MAGAZINE CoinBox1Status

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox1Status(STATUS_MAGAZINE CoinBox1Status)
{
	if(m_sCoinBox1Status == CoinBox1Status) return;
	m_sCoinBox1Status = CoinBox1Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX1_STATUS,(BYTE)CoinBox1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������1��װ״̬

@param      ��

@retval       STATUS_MAGAZINE Coin������1��װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinAddBox1Status()
{
	return m_sCoinBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������1������״̬���ݲ�ʹ�ã�

@param    (i)MAGAZINE_NUM_STATUS CoinBox1NumStatus Coin������1����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox1NumStatus(MAGAZINE_NUM_STATUS CoinBox1NumStatus)
{
	if(m_sCoinBox1NumStatus == CoinBox1NumStatus)return;

	m_sCoinBox1NumStatus = CoinBox1NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX1_COUNT,(BYTE)CoinBox1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������1����״̬���ݲ�ʹ�ã�

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin������1����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinAddBox1NumStatus()
{
	return m_sCoinBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������2��װ״̬

@param    (i)STATUS_MAGAZINE CoinBox2Status Coin������2��װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox2Status(STATUS_MAGAZINE CoinBox2Status)
{
	if(m_sCoinBox2Status == CoinBox2Status)return;

	m_sCoinBox2Status = CoinBox2Status;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX2_STATUS,(BYTE)CoinBox2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������2�䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Coin������2��װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinAddBox2Status()					
{
	return m_sCoinBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������2����״̬���ݲ�ʹ�ã�

@param    (i)MAGAZINE_NUM_STATUS CoinBox2NumStatus Coin������2����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinAddBox2NumStatus(MAGAZINE_NUM_STATUS CoinBox2NumStatus)
{
	if(m_sCoinBox2NumStatus == CoinBox2NumStatus)return;

	m_sCoinBox2NumStatus = CoinBox2NumStatus ;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CHANGE_BOX2_COUNT,(BYTE)CoinBox2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������2����״̬���ݲ�ʹ�ã�

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin������2����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinAddBox2NumStatus()
{
	return m_sCoinBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin Hopper1�䰲װ״̬���ݲ�ʹ�ã�

@param    (i)STATUS_MAGAZINE CoinHopper1Status Coin Hopper1�䰲װ״̬

@retval     LOGIN_STATUS

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper1Status(STATUS_MAGAZINE CoinHopper1Status)
{
	m_sCoinHopper1Status = CoinHopper1Status;
	// ���SC�����������豸״̬�������
	/*if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX1_STATUS,(BYTE)CoinHopper1Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief         ��ȡCoin Hopper1�䰲װ״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_MAGAZINE Coin Hopper1�䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinHopper1Status()
{
	return m_sCoinHopper1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin Hopper1������״̬

@param    (i)MAGAZINE_NUM_STATUS CoinHopper1NumStatus Coin Hopper1������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper1NumStatus(MAGAZINE_NUM_STATUS CoinHopper1NumStatus) 
{
	if(m_sCoinHopper1NumStatus == CoinHopper1NumStatus)return;

	m_sCoinHopper1NumStatus = CoinHopper1NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX1_COUNT,(BYTE)CoinHopper1NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin Hopper1������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin Hopper1������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinHopper1NumStatus()
{
	return m_sCoinHopper1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin Hopper2�䰲װ״̬���ݲ�ʹ�ã�

@param    (i)STATUS_MAGAZINE CoinHopper2Status Coin Hopper2�䰲װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper2Status(STATUS_MAGAZINE CoinHopper2Status)
{
	m_sCoinHopper2Status = CoinHopper2Status;
	// ���SC�����������豸״̬�������
	/*if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX2_STATUS,(BYTE)CoinHopper2Status);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin Hopper2�䰲װ״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_MAGAZINE Coin Hopper2�䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinHopper2Status()
{
	return m_sCoinHopper2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin Hopper2������״̬

@param    (i)MAGAZINE_NUM_STATUS CoinHopper2NumStatus Coin Hopper2������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinHopper2NumStatus(MAGAZINE_NUM_STATUS CoinHopper2NumStatus)
{
	if(m_sCoinHopper2NumStatus == CoinHopper2NumStatus)return;

	m_sCoinHopper2NumStatus = CoinHopper2NumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX2_COUNT,(BYTE)CoinHopper2NumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin Hopper2������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin Hopper2������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinHopper2NumStatus()
{
	return m_sCoinHopper2NumStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ѭ������Coin Hopper�䰲װ״̬���ݲ�ʹ�ã�

@param    (i)STATUS_MAGAZINE CoinHopperStatus ѭ������Coin Hopper�䰲װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCircleHopperStatus(STATUS_MAGAZINE CoinHopperStatus)
{
	m_sCoinCircleHopperStatus = CoinHopperStatus;
	// ���SC�����������豸״̬�������
	/*if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_HOPPER_BOX_STATUS,(BYTE)CoinHopperStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡѭ������Coin Hopper�䰲װ״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_MAGAZINE Coin Hopper2�䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinCircleHopperStatus()
{
	return m_sCoinCircleHopperStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ѭ������Coin Hopper������״̬

@param    (i)MAGAZINE_NUM_STATUS CoinHopper2NumStatus Coin Hopper2������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCircleHopperNumStatus(MAGAZINE_NUM_STATUS CoinHopperNumStatus)
{
	if(m_sCoinCircleHopperNumStatus == CoinHopperNumStatus)return;

	m_sCoinCircleHopperNumStatus = CoinHopperNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_HOPPER_BOX2_COUNT,(BYTE)CoinHopperNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡѭ������Coin Hopper������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin Hopper2������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinCircleHopperNumStatus()
{
	return m_sCoinCircleHopperNumStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������1��װ״̬

@param    (i)STATUS_MAGAZINE CoinCollectionBoxStatus Coin������1��װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox1Status(STATUS_MAGAZINE CoinCollectionBoxStatus)
{
	if(m_sCoinCollectionBox1Status == CoinCollectionBoxStatus)return;

	m_sCoinCollectionBox1Status = CoinCollectionBoxStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX1_STATUS,(BYTE)CoinCollectionBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������1��װ״̬

@param      ��

@retval       STATUS_MAGAZINE Coin������1��װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinCollectionBox1Status()
{
	return m_sCoinCollectionBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������1����״̬

@param    (i)MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus Coin������1����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox1NumStatus(MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus)
{
	if(m_sCoinCollectionBox1NumStatus == CoinCollectionBoxNumStatus)return;

	m_sCoinCollectionBox1NumStatus = CoinCollectionBoxNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX1_COUNT,(BYTE)CoinCollectionBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������1����״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin����������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinCollectionBox1NumStatus()
{
	return m_sCoinCollectionBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������2��װ״̬

@param    (i)STATUS_MAGAZINE CoinCollectionBoxStatus Coin������2��װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox2Status(STATUS_MAGAZINE CoinCollectionBoxStatus)
{
	if(m_sCoinCollectionBox2Status = CoinCollectionBoxStatus)return;

	m_sCoinCollectionBox2Status = CoinCollectionBoxStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX2_STATUS,(BYTE)CoinCollectionBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������2��װ״̬

@param      ��

@retval       STATUS_MAGAZINE Coin������2��װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetCoinCollectionBox2Status()
{
	return m_sCoinCollectionBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Coin������2����״̬

@param    (i)MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus Coin������2����״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetCoinCollectionBox2NumStatus(MAGAZINE_NUM_STATUS CoinCollectionBoxNumStatus)
{
	if(m_sCoinCollectionBox2NumStatus == CoinCollectionBoxNumStatus)return;

	m_sCoinCollectionBox2NumStatus = CoinCollectionBoxNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::COIN_CYCLE_BOX2_COUNT,(BYTE)CoinCollectionBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡCoin������2����״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Coin������2����״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetCoinCollectionBox2NumStatus()
{
	return m_sCoinCollectionBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��������1��װ״̬

@param      STATUS_MAGAZINE status (ֽ�������䰲ת״̬)

@retval     ��

@exception  ��
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
@brief      ��ȡֽ��������1��װ״̬

@param      ��

@retval     STATUS_MAGAZINE m_sBanknoteChangeBox1Status (��װ״̬)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetBanknoteChangeBox1Status(){
	return m_sBanknoteChangeBox1Status;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��������1����״̬

@param      MAGAZINE_NUM_STATUS numStatus(ֽ��������״̬)

@retval     ��

@exception  ��
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
@brief      ��ȡֽ��������1����״̬

@param      ��

@retval     MAGAZINE_NUM_STATUS m_sBanknoteChangeBox1NumStatus;(ֽ��������1����״̬)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteChangeBox1NumStatus(){
	return m_sBanknoteChangeBox1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��������2��װ״̬

@param      STATUS_MAGAZINE status (ֽ�������䰴ת״̬)

@retval     ��

@exception  ��
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
@brief      ��ȡֽ��������2��װ״̬

@param      ��

@retval     STATUS_MAGAZINE m_sBanknoteChangeBox2Status (��װ״̬)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetBanknoteChangeBox2Status(){
	return m_sBanknoteChangeBox2Status;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��������2����״̬

@param      MAGAZINE_NUM_STATUS numStatus(ֽ��������״̬)

@retval     ��

@exception  ��
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
@brief      ��ȡֽ��������2����״̬

@param      ��

@retval     MAGAZINE_NUM_STATUS ֽ������������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteChangeBox2NumStatus(){
	return m_sBanknoteChangeBox2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��ѭ����1����״̬

@param      MAGAZINE_NUM_STATUS numStatus(ֽ��������״̬)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE1NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE1NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ��ѭ����1����״̬

@param      ��

@retval     MAGAZINE_NUM_STATUS ֽ������������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE1NumStatus(){
	return m_sBanknoteRE1NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��ѭ����2����״̬

@param      MAGAZINE_NUM_STATUS numStatus(ֽ��������״̬)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE2NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE2NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ��ѭ����2����״̬

@param      ��

@retval     MAGAZINE_NUM_STATUS ֽ������������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE2NumStatus(){
	return m_sBanknoteRE2NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��ѭ����3����״̬

@param      MAGAZINE_NUM_STATUS numStatus(ֽ��������״̬)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE3NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE3NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ��ѭ����3����״̬

@param      ��

@retval     MAGAZINE_NUM_STATUS ֽ������������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE3NumStatus(){
	return m_sBanknoteRE3NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ��ѭ����4����״̬

@param      MAGAZINE_NUM_STATUS numStatus(ֽ��������״̬)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteRE4NumStatus(MAGAZINE_NUM_STATUS numStatus){
	m_sBanknoteRE4NumStatus = numStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ��ѭ����4����״̬

@param      ��

@retval     MAGAZINE_NUM_STATUS ֽ������������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteRE4NumStatus(){
	return m_sBanknoteRE4NumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Banknote�����䰲װ״̬

@param    (i)STATUS_MAGAZINE BanknoteCollectionBoxStatus Banknote�����䰲װ״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteCollectionBoxStatus(STATUS_MAGAZINE BanknoteCollectionBoxStatus)
{
	if(m_sBanknoteCollectionBoxStatus == BanknoteCollectionBoxStatus) return;

	m_sBanknoteCollectionBoxStatus = BanknoteCollectionBoxStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_STATUS,(BYTE)BanknoteCollectionBoxStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡBanknote�����䰲װ״̬

@param      ��

@retval       STATUS_MAGAZINE Banknote�����䰲װ״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_MAGAZINE CStatusHelper::GetBanknoteCollectionBoxStatus()
{
	return m_sBanknoteCollectionBoxStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Banknote����������״̬

@param    (i)MAGAZINE_NUM_STATUS BanknoteCollectionBoxNumStatus Banknote����������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetBanknoteCollectionBoxNumStatus(MAGAZINE_NUM_STATUS BanknoteCollectionBoxNumStatus)
{
	if(m_sBanknoteCollectionBoxNumStatus == BanknoteCollectionBoxNumStatus) return;

	m_sBanknoteCollectionBoxNumStatus = BanknoteCollectionBoxNumStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_COUNT,(BYTE)BanknoteCollectionBoxNumStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡBanknote����������״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Banknote����������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
MAGAZINE_NUM_STATUS CStatusHelper::GetBanknoteCollectionBoxNumStatus()
{
	return m_sBanknoteCollectionBoxNumStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ֽ�ȫ��״̬���ݲ�ʹ�ã�

@param    (i)SECURITY_DOOR_STATUS �ֽ�ȫ��״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetSecurityDoorStatus(SECURITY_DOOR_STATUS SecurityDoorStatus)
{
	if(m_sSecurityDoorStatus == SecurityDoorStatus)return;

	m_sSecurityDoorStatus = SecurityDoorStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::SECURITY_DOOR_STATUS,(BYTE)SecurityDoorStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡ�ֽ�ȫ��״̬���ݲ�ʹ�ã�

@param      ��

@retval       SECURITY_DOOR_STATUS  �ֽ�ȫ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SECURITY_DOOR_STATUS CStatusHelper::GetSecurityDoorStatus()
{
	return m_sSecurityDoorStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô�����״̬���ݲ�ʹ�ã�

@param    (i)TOUCH_SCREEN_STATUS ������״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetTouchDisplayStatus(TOUCH_SCREEN_STATUS TouchScreenStatus)
{
	if(m_sTouchScreenStatus == TouchScreenStatus) return;

	m_sTouchScreenStatus = TouchScreenStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOUCH_SCREEN_STATUS,(BYTE)TouchScreenStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡ������״̬���ݲ�ʹ�ã�

@param      ��

@retval       TOUCH_SCREEN_STATUS ������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
TOUCH_SCREEN_STATUS CStatusHelper::GetTouchDisplayStatus()
{
	return m_sTouchScreenStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ԯ��ť״̬

@param    (i)ASSISTANCE_BUTTON_STATUS ��Ԯ��ť״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS AssistanceButtonStatus)
{
	//if(m_sAssistanceButtonStatus == AssistanceButtonStatus)return;

	//m_sAssistanceButtonStatus = AssistanceButtonStatus;
	//// ���SC�����������豸״̬�������
	//if(IsSCConnected())	{
	//	CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::HELP_BUTTON_STATUS,(BYTE)AssistanceButtonStatus);
	//	theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡ��Ԯ��ť״̬

@param      ��

@retval       MAGAZINE_NUM_STATUS Banknote ����������״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
ASSISTANCE_BUTTON_STATUS CStatusHelper::GetAssistanceButtonStatus()
{
	return m_sAssistanceButtonStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó�ֵRW״̬

@param    (i)STATUS_RW ��ֵRW״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetRW1Status(STATUS_RW RechargeRWStatus)
{
	if(m_sCardRWStatus == RechargeRWStatus)return;

	m_sCardRWStatus = RechargeRWStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW1_STATUS,(BYTE)RechargeRWStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
	// ��ֵ��д���豸��ع���
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
@brief        ��ȡ��ֵRW״̬

@param      ��

@retval       STATUS_RW ��ֵRW״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CStatusHelper::GetRW1Status()									
{
	return m_sCardRWStatus;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó�ֵRW״̬

@param    (i)STATUS_RW ��ֵRW״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::SetRW2Status(STATUS_RW RechargeRWStatus)
{
	if(m_sRechargeRWStatus == RechargeRWStatus)return;

	m_sRechargeRWStatus = RechargeRWStatus;
	// ���SC�����������豸״̬�������
	if(IsSCConnected())	{
		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::RW2_STATUS,(BYTE)RechargeRWStatus);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
	}
	// ��ֵ��д���豸��ع���
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
@brief        ��ȡ��ֵRW״̬

@param      ��

@retval       STATUS_RW ��ֵRW״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
STATUS_RW CStatusHelper::GetRW2Status()									
{
	return m_sRechargeRWStatus;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Card RW״̬

@param    (i)STATUS_RW Card RW״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetCardRWStatus(STATUS_RW CardRWStatus)
// {
// 	m_sCardRWStatus = CardRWStatus;
// 	// ���SC�����������豸״̬�������
// 	if(IsSCConnected())	{
// 		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::CARD_RW_STATUS,(BYTE)CardRWStatus);
// 		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
// 	}
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief         ��ȡCard RW״̬

@param      ��

@retval       STATUS_RW Card RW״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// STATUS_RW CStatusHelper::GetCardRWStatus()											
// {
// 	return m_sCardRWStatus;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Token RW״̬

@param    (i)STATUS_RW Token RW״̬

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetTokenRWStatus(STATUS_RW TokenRWStatus)
// {
// 	m_sTokenRWStatus = TokenRWStatus;
// 	// ���SC�����������豸״̬�������
// 	if(IsSCConnected())	{
// 		CSCDataMsg* statusChangeCommand = MakeStatusChangeCommand(TVM_STATUS_IDS::TOKEN_RW_STATUS,(BYTE)TokenRWStatus);
// 		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)statusChangeCommand);
// 	}
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief        ��ȡToken RW״̬���ݲ�ʹ�ã�

@param      ��

@retval       STATUS_RW Token RW״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// STATUS_RW CStatusHelper::GetTokenRWStatus()
// {
// 	return m_sTokenRWStatus;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam1���ͣ��ݲ�ʹ�ã�
@param      SAM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡSam1���ͣ��ݲ�ʹ�ã�
@param      ��
@retval     SAM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CStatusHelper::GetSam1Type()
{
	return m_sSam1Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam2����
@param      SAM_STATUS
@retval     ��
@exception  ��
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
@brief      ��ȡSam2����
@param      ��
@retval     SAM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_STATUS CStatusHelper::GetSam2Type()
{
	return m_sSam2Type;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Sam3���ͣ��ݲ�ʹ�ã�
@param      SAM_STATUS
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// void CStatusHelper::SetSam3Type(SAM_STATUS type)
// {
// 	m_sSam3Type = type;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSam3���ͣ��ݲ�ʹ�ã�
@param     ��
@retval     SAM_STATUS
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// SAM_STATUS CStatusHelper::GetSam3Type()
// {
// 	return m_sSam3Type;
// }


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SAM1״̬(��ֵ��д��һƱͨ ISAM)

@param   (i)SAM_STATUS SAM��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡSAM1״̬(��ֵ��д��һƱͨ ISAM)

@param      ��

@retval     SAM_STATUS SAM��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_AUTH_STATUS CStatusHelper::GetSam1Status()
{
	return m_sSAM1Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SAM2״̬(Card��д��һƱͨ ISAM)

@param   (i)SAM_STATUS SAM��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡSAM2״̬(Card��д��һƱͨ ISAM)

@param      ��

@retval     SAM_STATUS SAM��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
SAM_AUTH_STATUS CStatusHelper::GetSam2Status()
{
	return m_sSAM2Status;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SAM3״̬(Token��д��һƱͨ ISAM)

@param   (i)SAM_STATUS SAM��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡSAM3״̬(Token��д��һƱͨ ISAM)

@param      ��

@retval     SAM_STATUS SAM��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//SAM_STATUS CStatusHelper::GetSam3Status()
//{
//	return m_sTokenRWYPTISAMStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SAM4״̬(��ֵ��д��һ��ͨISAM)(�ݲ�ʹ��)

@param    (i)SAM_STATUS SAM��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡSAM4״̬(��ֵ��д��һ��ͨISAM)(�ݲ�ʹ��)

@param      ��

@retval     SAM_STATUS SAM��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//SAM_STATUS CStatusHelper::GetSam4Status()
//{
//	return m_sRechargeRWYKTISAMStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ô�ӡ��״̬

@param    (i)PRINTER_STATUS ��ӡ��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡ��ӡ��״̬

@param   ��

@retval     PRINTER_STATUS ��ӡ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
PRINTER_STATUS CStatusHelper::GetPrintStatus()
{
	return m_sPrintStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����UPS״̬

@param    (i)UPS_STATUS UPS״̬

@retval     ��

@exception  ��
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
@brief      ��ȡUPS״̬

@param   ��

@retval     UPS_STATUS UPS״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//UPS_STATUS CStatusHelper::GetUPSStatus()
//{
//	return m_sUPSStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ά����״̬

@param      DOOR_STATUS

@retval     ��

@exception  ��
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
@brief      ��ȡά����״̬

@param      ��

@retval     DOOR_STATUS

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DOOR_STATUS CStatusHelper::GetMaintenanceDoorStatus()
{
	return m_sMaintenanceDoorStatus;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ֽ�ҽ���ģ��״̬

@param   (i)HARDWARE_MODULE_STATUS ֽ�ҽ���ģ��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡֽ�ҽ���ģ��״̬

@param      ��

@retval     HARDWARE_MODULE_STATUS

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetBanknoteModuleStatus()
{
	return m_sBanknoteModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ������ģ��״̬

@param      (HARDWARE_MODULE_STATUS status��������ģ��״̬��

@retval     ��

@exception  ��
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
@brief      ��ȡֽ������ģ��״̬

@param      ��

@retval     HARDWARE_MODULE_STATUS

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetBanknoteChangeModuleStatus()
{
	return m_sBanknoteChangeModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Tokenģ��״̬

@param    (i)HARDWARE_MODULE_STATUS Tokenģ��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡTokenģ��״̬

@param      ��

@retval     HARDWARE_MODULE_STATUS Tokenģ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//HARDWARE_MODULE_STATUS CStatusHelper::GetTokenModuleStatus()
//{
//	return m_sTokenModuleStatus;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Cardģ��״̬

@param    (i)HARDWARE_MODULE_STATUS Cardģ��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡCardģ��״̬

@param      ��

@retval     HARDWARE_MODULE_STATUS Cardģ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetCardModuleStatus()
{
	return m_sCardModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ӳ��ģ��״̬

@param    (i) HARDWARE_MODULE_STATUS Ӳ��ģ��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡӲ��ģ��״̬

@param      ��

@retval     HARDWARE_MODULE_STATUS Ӳ��ģ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetCoinModuleStatus()
{
	return m_sCoinModuleStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����״̬��ʾ��״̬

@param    (i) HARDWARE_MODULE_STATUS ״̬��ʾ��״̬

@retval     ��

@exception  ��
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
@brief      ��ȡ״̬��ʾ��״̬

@param      ��

@retval     HARDWARE_MODULE_STATUS ״̬��ʾ��״̬

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
HARDWARE_MODULE_STATUS CStatusHelper::GetStatusDisplayStatus()
{
	return m_sStatusDisplayStatus;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������б������״̬(�𶯴�����״̬)

@param    (i) TILT_STATUS ��б������״̬(�𶯴�����״̬)

@retval     ��

@exception  ��
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
@brief      ��ȡ��б������״̬(�𶯴�����״̬)

@param      ��

@retval     TILT_STATUS ��б������״̬(�𶯴�����״̬)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
// TILT_STATUS CStatusHelper::GetTiltStatus()
// {
// 	return m_sTiltStatus;
// }

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������

@param    (i) BYTE ��������

@retval     ��

@exception  ��
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
@brief      ��ȡ��������

@param      ��

@retval     TILT_STATUS ��������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BYTE CStatusHelper::GetOffLineDays()
{
	return m_sOffLineDays;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������б������״̬(�𶯴�����״̬)

@param    (i) TILT_STATUS ��б������״̬(�𶯴�����״̬)

@retval     ��

@exception  ��
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
@brief      ��ȡ��Ȩ����״̬

@param      ��

@retval     AUTHORIZED_STATUS ��Ȩ����״̬

@exception  ��
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
@brief      TVM�豸����״̬����

@param      ����ID

@retval     ������Ϣ����ָ��

@exception  ��
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
@brief      �쳣���з����仯ʱ��Ӧ����

@param      (i)CSysException& e �쳣����
@param      (i)bool isInsert          �Ƿ��ǲ����쳣

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CStatusHelper::OnExceptionItemNotify(CSysException& e,bool isInsert)
{
	// �����쳣
	if(isInsert){
		//SetDeviceErrStatus(DEVICE_ERROR);
		// ��ӡ��״̬
		if(e.GetModuleID() == CPrinterException::MODULE_ID){
			SetPrintStatus(PRINTER_STATUS::PRINTER_ERR);//SetPrintStatus((e.GetDetailID() == CPrinterException::ERROR_Printer_NO_PAPER ||	e.GetDetailID() == CPrinterException::ERROR_Printer_LESS_PAPER) ? PRINTER_NOPAPER :PRINTER_ERR	);
		}		
		// Crad��д��
		if(e.GetModuleID() == CRWException::TVM_CARD_RW_MODULE_ID || e.GetModuleID() == CRWHardwareException::TVM_CARD_RW_MODULE_ID ){
			SetRW1Status(RW_ERROR);
		}
		// ֽ��ģ��
		if(e.GetModuleID() == CBRHException::MODULE_ID || e.GetModuleID() == CBRHHardwareException::MODULE_ID){
			if (e.GetDetailID() == CBRHHardwareException::BANKNOTE_ERROR_MODULE_NOT_STANDBY){
				SetBanknoteModuleStatus(HARDWARE_MODULE_UNSET);
			}
			else{
				SetBanknoteModuleStatus(HARDWARE_MODULE_FAILURE);
			}
		}	
		// Ӳ��ģ��
		if(e.GetModuleID() == CCHException::MODULE_ID || e.GetModuleID() == CCHHardwareException::MODULE_ID){
			if (e.GetDetailID() == CCHHardwareException::ERROR_CH_MODULE_NOT_STANDBY){
				SetCoinModuleStatus(HARDWARE_MODULE_UNSET);
			}
			else{
				SetCoinModuleStatus(HARDWARE_MODULE_FAILURE);
			}
		}
		// IOģ��
		if(e.GetModuleID() == CIOException::MODULE_ID){
			//SetBanknoteModuleStatus(HARDWARE_MODULE_FAILURE);
		}	
		// ״̬��ʾ��
		if(e.GetModuleID() == CStatusDisplayException::MODULE_ID){
			SetStatusDisplayStatus(HARDWARE_MODULE_FAILURE);
		}
		// Cardģ��
		if(theEXCEPTION_MGR.HasException(CTHException::MODULE_ID) || theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID)){
			SetCardModuleStatus(HARDWARE_MODULE_FAILURE);
		}
		// �豸����״̬
		//theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_DEVICE_ERR,NULL,theEXCEPTION_MGR.GetActualMTC(e));
		SetRunStatus(WARNING);
		//SetServiceStatus(theEXCEPTION_MGR.OnlyHasHardwareException() ? WARN_SERVICE : WARN_SERVICE/*OUTOF_SERVICE*/);// zhengxianle ��ֹ��ҵ������в����쳣��ʾ��ͣ������ʱ����
		// �豸����ҵ������
		DEVICE_ERRORDATA errorData;
		unsigned long ulMtcCode = theEXCEPTION_MGR.GetActualMTC(e);
		unsigned short usEc = theEXCEPTION_MGR.GetEC(e);
		errorData.partType = theEXCEPTION_MGR.GetDevicePartTypeOfException(e);
		errorData.bDeviceType =  theEXCEPTION_MGR.GetDevicePartTypeOfException(e);                // 1 BIN ��������    
		if(errorData.bDeviceType!=DEVICE_COMPONENT::DEVICE_COMPOENT_UNKNOW){
			int departIndex = 1;
			memcpy(errorData.devicePartID,&departIndex,3);
		}
		else{
			memset(errorData.devicePartID, 0xFF, 3);     // 3 BIN ����ID  
		}
		errorData.deviceDeviderId = FOUNDER_ID;      // 2 BIN �豸��Ӧ��ID
		memcpy(errorData.maintanCode, &ulMtcCode, 4);// 4 1BIN+1BIN+1BIN+1BIN ���ϴ���   
		memcpy(errorData.ecode, &usEc, 2);           // 2 1BIN+1BIN ά�޴���
		SetDeviceErrStatus(ulMtcCode);
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceErrorData(errorData);
	}
	// ɾ���쳣
	else{
		// ��ӡ��
		/*if(!theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID)){
		SetPrintStatus(PRINTER_NORMAL);
		}*/
		//// ��ֵ��д��
		//if((!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID))){
		//	SetRechargeRWStatus(RW_NORMAL);
		//}
		//// Card��д��
		//if((!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID))){
		//	SetCardRWStatus(RW_NORMAL);
		//}
		//// Token��д��
		//if((!theEXCEPTION_MGR.HasException(CRWException::TVM_TOKEN_RW_MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID))){
		//	SetCardRWStatus(RW_NORMAL);
		//}
		// Cardģ��
		/*if((!theEXCEPTION_MGR.HasException(CTHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CTHHardwareException::MODULE_ID))){
		SetCardModuleStatus(HARDWARE_MODULE_NORMAL);
		}*/
		// ֽ��ģ��
		if((!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID))){
			SetBanknoteModuleStatus(HARDWARE_MODULE_NORMAL);
		}
		// Ӳ��ģ��
		if((!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)) && (!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID))){
			SetCoinModuleStatus(HARDWARE_MODULE_NORMAL);
		}
		// IOģ��
		if(!theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
			//SetBanknoteModuleStatus(HARDWARE_MODULE_NORMAL);
		}

		// LED��ʾ��
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
@brief      ��ȡ����ģʽ

@param      ��

@retval     WORK_MODE ����ģʽ����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
WORK_MODE CStatusHelper::GetDegreeWorkMode()
{
	if(m_sWorkMode & DEVICE_CONTRL_CODE_BUSINESS_CARD==DEVICE_CONTRL_CODE_BUSINESS_CARD){
		// ֧��Ӳ�Һ�ֽ�ң���֧�ִ�ֵ��
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)!=DEVICE_CONTRL_CODE_PAYMENT_SVT)){
			return WORK_MODE::WORK_MODE_NO_SVT;
		}
		// ֧��Ӳ�Һʹ�ֵ������֧��ֽ��
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_BANKNOTE_ACCEPT;
		}
		// ֧��ֽ�Һʹ�ֵ������֧��Ӳ�ң�Ӳ��ģ����ϣ�
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)!=DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_COIN_ACCEPT;
		}
		// ֧��ֽ�Һʹ�ֵ������֧��Ӳ�����㣨Ӳ��hopper��գ�
		if ( ((m_sChangeMode & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)!=DEVICE_CONTRL_CODE_NO_BILL_CHANGE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_CHANGE;
		}
		// ֻ֧��ֽ��
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)!=DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)!=DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_BANKNOTE_ONLY;
		}
		// ֻ֧��Ӳ��
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)==DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)!=DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_COIN_ONLY;
		}
		// ֻ֧�ִ�ֵ��
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_COIN)!=DEVICE_CONTRL_CODE_PAYMENT_COIN) 
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_SVT_ONLY;
		}

		// ֧��ȫ��֧����ʽ
		if (m_sPaymentMode == DEVICE_CONTRL_CODE_PAYMENT_ALL){
			return WORK_MODE::WORK_MODE_NORMALLY;
		}
	}
	else{
		// ֧��ֽ�Һʹ�ֵ��
		if ( ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)==DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_NO_ISSUE;
		}
		// ֻ֧�ִ�ֵ��
		if (((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!=DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
			&& ((m_sPaymentMode & DEVICE_CONTRL_CODE_PAYMENT_SVT)==DEVICE_CONTRL_CODE_PAYMENT_SVT)){
				return WORK_MODE::WORK_MODE_ANALY_ONLY;
		}
	}

	return WORK_MODE::WORK_MODE_NORMALLY;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �жϵ�ǰSC�Ƿ����ӣ��Ҳ�����Ӫ����ʱ����

@param      ��

@retval     bool true:������״̬���� false:�����Է���״̬����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CStatusHelper::IsSCConnected(){
	return theAPP_SESSION.IsSCConnected() && !theAPP_SESSION.GetCloseOperationFlag();
}


CStatusHelper CStatusHelper::theInstance;
