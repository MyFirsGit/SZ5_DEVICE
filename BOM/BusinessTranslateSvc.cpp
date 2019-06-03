#include "StdAfx.h"
#include "BusinessTranslateSvc.h"
#include "CAfcDeviceCommonParam.h"
#include "headermanager.h"
#include "ctmessageaudit.h"
#include "scdatamsg.h"
#include "serverclientsvc.h"
#include "scmessagemakehelper.h"

#include "ServerClientSvc.h"
#include "bomdef.h"
#include "CTLOGCountInfo.h"
#include "ACCAuditManager.h"
#include "scsvcresult.h"
#include "Sync.h"
/**
@brief ���ڼ���Service
*/

BEGIN_MESSAGE_MAP(CBusinessTranslateSvc, CBackService)
	ON_SERVICE_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBusinessTranslateSvc::CBusinessTranslateSvc()
:CBackService(BUSINESS_INTERVAL_SVC,ROOT_SVC)
{
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBusinessTranslateSvc::~CBusinessTranslateSvc(void)
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ���ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::StartBusinessIntervalTimer()
{
	// ����24Сʱ��������
	CAfcDeviceCommonParam::COMMUNICATION Communication;
	theAFC_COMMON.GetCommunication(Communication);
	DWORD registerTime = Communication.businessInterval*60;
	theDEBUG_LOG->WriteData(_T("CBusinessSvc"), _T("StartBusinessIntervalTimer"), __LINE__, _T("CBusinessSvc = 0x%x"),this);
	theDEBUG_LOG->WriteData(_T("CBusinessSvc"), _T("StartBusinessIntervalTimer"), __LINE__, _T("TxnIntervalTime = %d"),registerTime);
	if (registerTime == 0 || registerTime > 24*3600) {
		StartTimer(TIMER_REGISTER, 900);
	}
	else {
		StartTimer(TIMER_REGISTER, registerTime);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ر�ҵ���ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::EndBusinessIntervalTimer()
{
	StopTimer(TIMER_REGISTER);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ����Ӧ����

@param      (i)UINT nTimerID  identifier of a timer

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::OnTimer(UINT nTimerID)
{
	if(nTimerID == TIMER_PACKAGE)
	{
		//���
		theBUS_TXN_MGR.MakePekg(false);
	}
	if(nTimerID == TIMER_REGISTER)
	{
		EndBusinessIntervalTimer();
		try {
			theDEBUG_LOG->WriteData(_T("CBusinessSvc"), _T("OnTimer"), __LINE__, _T("UINT nTimerID = %x <-"),nTimerID);
			// �����豸�Ĵ�������
			InsertMsgDeviceRegisterData(AR_TIME_ON);
			SendBusinessData();
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...) {
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		StartBusinessIntervalTimer();
	}
}

void CBusinessTranslateSvc::OnStart()
{
	__super::OnStart();
	StartTimer(TIMER_PACKAGE,theBOM_INFO.GetBusinessPackageInterval());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֹͣʱ��Ӧ����

@param      ��

@retval     bool �Ƿ�����ֹͣ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBusinessTranslateSvc::OnStop()
{
	StopTimer(TIMER_PACKAGE);
	StopTimer(TIMER_REGISTER);
	return __super::OnStop();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ҵ������
@param      ��
@retval     int    ����
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBusinessTranslateSvc::SendBusinessData()
{

	theBUS_TXN_MGR.MakePekg();
	if(!theAPP_SESSION.IsSCConnected())
	{
		return SCSVC_RESULT_NOT_CONNECTED;
	}
	SYNC(SENDDATA,_T("BUSINESS"));
	// ��δ����
	while(theBUS_TXN_MGR.HasUnsendData() > 0)
	{
		// ��ȡ�����е����ݰ�
		PEKG_DATA dataPekg;		
		bool aquirePekgSuccess = theBUS_TXN_MGR.GetDataPekg(dataPekg);	
		if(!aquirePekgSuccess)
		{
			continue;
		}
		CMD_HEADER header = CHeaderManager::AquireHeader(OPER_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBusDataMsg(new CSCDataMsg);
		pBusDataMsg->SetHeader(&header);
		pBusDataMsg->SetContent(dataPekg.lpData,dataPekg.nLen);
		long executeResult = theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)pBusDataMsg.get());
		if (executeResult == SP_SUCCESS)
		{
			// �����ͳɹ��������ݰ�Ϊ����
			theBUS_TXN_MGR.SetPekgSended(dataPekg.strPekgID);
			CTMessageAudit::PEKG_INFO PekgInfo;
			memcpy(PekgInfo.sPekgId, dataPekg.lpData+1, 14);
			memcpy(&PekgInfo.iRecordCnt,dataPekg.lpData+19, 2);				
			thePEKG_AUDIT.AddWriteFile(PekgInfo);
		}
		else
		{
			return executeResult;
		}
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����ͳ������
@param      
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgUpdateParameterAndProgram(CParamHelper::VEC_VERSION_INFO& Vec_VersionInfo)   
{
	try
	{
		int iCount = Vec_VersionInfo.size();
		long lMsgLen = LEN_BUS_PARAMETER_SOFT_UPDATE_BASE + iCount*23;
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_SOFTANDPROGRAM_UPDATE,lMsgLen);
		LPBYTE lpMsg =  new BYTE[lMsgLen];
		theSCMessageMakeHelper.MakeUpdateParameterAndProgram(operComHdr, Vec_VersionInfo, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(lMsgLen, lpMsg);
		delete[] lpMsg;
		lpMsg = NULL;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����ͳ�������ҵ������
@param    list<_parameter_download> list_parameter  ���ز������ļ��б�
LPBYTE					lpByte           
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDownloadParameterAndProgram(vector<ParameterAndSoftwareUpdate>& downloads)     
{
	try
	{
		int iCount = downloads.size();
		long lMsgLen = LEN_BUS_PARAMETER_SOFT_DOWNLOAD_BASE + 51*iCount;
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_SOFTANDPROGRAM_DOWNLOAD,lMsgLen);
		LPBYTE lpMsg = new BYTE[lMsgLen];
		theSCMessageMakeHelper.MakeDownloadParameterAndProgram(operComHdr, downloads,lpMsg);
		theBUS_TXN_MGR.SaveTxnData(lMsgLen, lpMsg);
		delete[] lpMsg;
		lpMsg = NULL;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �豸��Ҫ״̬�ı�
@param    SERVICE_STATUS	status   ����״̬
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgImportantStatusChange(SERVICE_STATUS status)  
{
	try
	{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_IMPORTANT_STATUS_CHANGE,LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG);
		BYTE lpMsg[LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG]={0};
		theSCMessageMakeHelper.MakeImportantStatusChange(operComHdr,lpMsg,status);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_DEVICE_IMPORTANT_STATUS_CHANG, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief   ISAMǩ��/ǩ��
@param   ISAM_AUTH  isam_auth  ǩ����ʽ
BYTE       result     ���
@retval  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CBusinessTranslateSvc::InsertMsgIsamCheckIn(ISAM_AUTH isam_auth, BYTE result)		 
//{
//	try
//	{
//		OperComHdr_t operComHdr=theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_ISAM_LOGIN_LOGOUT,MSG_BUSSINESS_ISAM_LOGIN_LOGOUT_LEN);
//		BYTE lpMsg[MSG_BUSSINESS_ISAM_LOGIN_LOGOUT_LEN]={0};
//		//theSCMessageMakeHelper.MakeIsamCheckIn(operComHdr,lpMsg,isam_auth, result);
//		theBUS_TXN_MGR.SaveTxnData(MSG_BUSSINESS_ISAM_LOGIN_LOGOUT_LEN, lpMsg);
//	}
//	catch (CSysException& e) {
//		theEXCEPTION_MGR.ProcessException(e);
//	}
//	catch (...) {
//		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief    ������־
@param    OPERATOR_LOG  ������־����
@retval   ��
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgOperationLog(OPERATOR_LOG operLog)					 
{
	try
	{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_OPERATING_LOG,LEN_BUS_OPERATING_LOG);
		BYTE lpMsg[LEN_BUS_OPERATING_LOG]={0};
		theSCMessageMakeHelper.MakeOperationLog(operComHdr, operLog, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_OPERATING_LOG, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    ������־
@param    OPERATOR_LOG  ������־����
@retval   ��
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgOperationLog(OPERATION_CODE operCode)					 
{
	try
	{
		OPERATOR_LOG operatorLog;
		operatorLog.operCode = operCode;
		operatorLog.operLevel = OPERATION_FIVE_LEVEL;
		operatorLog.result = 0x01;
		operatorLog.csUserID=theAPP_SESSION.GetUserInfo().sUserID;
		char *pDetail = (char*)(operatorLog.operDetail);
		switch(operCode)
		{
		case OPERATION_LOGIN:
			strcpy(pDetail, "��¼");
			break;
		case OPERATION_LOGOUT:
			strcpy(pDetail, "�ǳ�");
			break;
		case OPERATION_INPUT_PARAM:
			strcpy(pDetail, "��������ͳ���");
			break;
		case OPERATION_OUTPUT_PARAM:
			strcpy(pDetail, "���������ͳ���");
			break;
		case OPERATION_INPUT_DEAL:
			strcpy(pDetail, "���뽻������");
			break;
		case OPERATION_OUTPUT_DEAL:
			strcpy(pDetail, "������������");
			break;
		case OPERATION_INPUT_OPER:
			strcpy(pDetail, "����ҵ������");
			break;
		case OPERATION_OUTPUT_OPER:
			strcpy(pDetail, "����ҵ������");
			break;
		case OPERATION_INPUT_DEBUG:
			strcpy(pDetail, "�����������");
			break;
		case OPERATION_OUTPUT_DEBUG:
			strcpy(pDetail, "������������");
			break;
		case OPERATION_FORCE_TIME:
			strcpy(pDetail, "ǿ��ʱ��ͬ��");
			break;
		case OPERATION_MANUAL_TIME:
			strcpy(pDetail, "�ֹ�ʱ��ͬ��");
			break;
		case OPERATION_24HOUR_DELAY:
			strcpy(pDetail, "24Сʱ��Ӫ���ӳ���Ӫ����");
			break;
		case OPERATION_PARAM_EDIT:
			strcpy(pDetail, "�����༭���༭���������·���");
			break;
		case OPERATION_MODE_SET:
			strcpy(pDetail, "ģʽ�趨���˹������豸ģʽ��");
			break;
		case OPERATION_MODE_NOTIC:
			strcpy(pDetail, "ģʽ֪ͨ�趨");
			break;
		case OPERATION_TIME_WARNING:
			strcpy(pDetail, "ʱ��ͬ������");
			break;
		case OPERATION_TIME_ERROR:
			strcpy(pDetail, "ʱ��ͬ���쳣");
			break;
		case OPERATION_CONNECT_STATUS:
			strcpy(pDetail, "����״̬");
			break;
		default:
			strcpy(pDetail, "δ֪����");
			break;
		}
		InsertMsgOperationLog(operatorLog);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �豸��������

@param    (i)DEVICE_ERRORDATA& DeviceErrData

@retval   none
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDeviceErrorData(DEVICE_ERRORDATA& DeviceErrData)					 
{
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_ERROR,LEN_BUS_DEVICE_ERROR);
		BYTE lpMsg[LEN_BUS_DEVICE_ERROR] ={0};
		theSCMessageMakeHelper.MakeDeviceErrorData(operComHdr, DeviceErrData, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_DEVICE_ERROR, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �豸�Ĵ�������	
@param     PURCHASE_ACC_DATA PurchaseAccData
AR_RESON arReson
@retval     
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDeviceRegisterData(AR_REASON arReson)                                    
{
	try
	{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_REGISTER,LEN_BUS_DEVICE_REGISTER);
		BYTE lpMsg[LEN_BUS_DEVICE_REGISTER]={0};
		DWORD dataLen = 0;
		theSCMessageMakeHelper.MakeDeviceRegisterData(operComHdr, theACCAUDIT_MGR.GetCurrentAR(), arReson, lpMsg,dataLen);
		theBUS_TXN_MGR.SaveTxnData(dataLen, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     Ʊ�����  	
@param     MAGAZINE_CHANGE magazine_change Ʊ�������Ϣ
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgMagazineExchange(MAGAZINE_CHANGE& magazine_change)                                  
{

	try
	{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_TICKETBOX_CHANGE,LEN_BUS_TICKETBOX_CHANGE);
		BYTE lpMsg[LEN_BUS_TICKETBOX_CHANGE]={0};
		theSCMessageMakeHelper.MakeMagazineExchange(operComHdr, magazine_change, lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_TICKETBOX_CHANGE, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����豸������������  

@param    none  

@retval   none

*/
//////////////////////////////////////////////////////////////////////////
void CBusinessTranslateSvc::InsertMsgDeviceComponentInfo()                                   
{
	try{
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_COMPONENT,LEN_BUS_DEVICE_COMPONENT);
		BYTE lpMsg[LEN_BUS_DEVICE_COMPONENT]={0};
		theSCMessageMakeHelper.MakeDeviceComponentInfo(operComHdr,lpMsg);
		theBUS_TXN_MGR.SaveTxnData(LEN_BUS_DEVICE_COMPONENT, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(BUSINESS_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
