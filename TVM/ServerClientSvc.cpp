#include "stdafx.h"
#include "scapi.h"
#include "scconnectrequest.h"
#include "serverclientsvc.h"
#include "scclient.h"
#include "maintenanceinfo.h"
#include "scmessagemakehelper.h"
#include "SCConnectRequestReply.h"
#include "funccommand.h"
#include "requestmanager.h"
#include "hook.h"
#include "Func.h"
#include "scdatamsgreply.h"
#include "HeaderManager.h"
#include "mlcencrypt.h"
//#include "ParamHelper.h"
#include "scmessagecontentdefine.h"
#include "scmessageparsehelper.h"
#include "TxnDataStruct.h"
#include "AppSession.h"
#include "ServiceMgr.h"
#include "UpgradeHelper.h"
#include "MessageIDDefinition.h"
#include "tvmstatusmgr.h"
#include "ctissuepermit.h"
#include "ctmessageaudit.h"
#include "ConnectSequence.h"
#include "transcationtranslatesvc.h"
#include "businesstranslatesvc.h"
#include "eventdatasvc.h"
#include "CloseOperationSequence.h"
#include "SntpApi.h"
#include "tvmdef.h"
#include "CTLOGSoftVer.h"
//#include "SCAuditManager.h"
#include "DebugLog.h"
#include "scsvcresult.h"
#include "loghelper.h"
#include "FTPHelper.h"
#include "Sync.h"
#include "tvmsetting.h"
#include "ParamMsg.h"
#include "StartOperationSequence.h"

#define  __FILENAME__ ""

#define theSC_Svc_Trace CFileLog::GetInstance(FILTER_SC_SVC_LOG)


BEGIN_MESSAGE_MAP(CServerClientSvc,CBackService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SC_MSG_SEND_TO_SC,OnSendMsgToSC)
	ON_SERVICE_MESSAGE(SC_MSG_SEND_DEVICE_ERR,SendTVMError)
	ON_SERVICE_MESSAGE(SC_MSG_SEND_DEVICE_FULLSTATUS,SendTVMFullStatus)
	ON_SERVICE_MESSAGE(SC_MSG_CONNECT,OnConnect)
	ON_SERVICE_MESSAGE(SC_MSG_DATAMSG,OnScDataMsgArrival)
	ON_SERVICE_MESSAGE(SC_MSG_RECONNECT,OnReconnect)
	ON_SERVICE_MESSAGE(SC_MSG_CLOSEOPERATION,OnCloseOperation)
	ON_SERVICE_MESSAGE(SC_MSG_STARTOPERATION,OnStartOperation)
	ON_SERVICE_MESSAGE(SC_MSG_DOWNLOAD_IMMIDIATE_PARAMETER,OnRequestDownloadImmidiateParameter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CServerClientSvc::CServerClientSvc()
:CBackService(SC_SVC,ROOT_SVC,true)
{
	m_ReconnectThread = NULL;
	m_Sequence = NULL;
	m_hThreadMonitorOnline=NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CServerClientSvc::~CServerClientSvc()
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ʱ��Ӧ����
@param      ��    
@retval    bool
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnStart()
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	CSCClient::SCClientParameter clientParamter;
	clientParamter.hostSvc = this;
	clientParamter.enquireLinkInterval = 28;
	clientParamter.receiveTimeOutInterval = 32;
	clientParamter.sendTimeOutInterval = 32;
	clientParamter.nackResendMaxTimes = 3;
	clientParamter.timeoutResendMaxTimes = 3;
	theClient.Initialize(clientParamter);
	
	m_SCClientStatus = SC_CONNECTION_OFF;
	m_ReconnectThread = (CServiceThread*) CUtilThread::StartThread(RUNTIME_CLASS(CServiceThread), "CServiceThread");
	m_ReconnectThread->SetService(this);
	g_pSCControl->BeforeDoCommand.AddHandler(this,&CServerClientSvc::OnSCSendCommand);
	m_RequestExitMonitorThread = FALSE;
	do{
		DWORD workThreadID=0;
		m_hThreadMonitorOnline = CreateThread(NULL,0,&CServerClientSvc::MonitorOnline,this,0,&workThreadID);//���������߳�
		::Sleep(100);
	}while(m_hThreadMonitorOnline==NULL || m_hThreadMonitorOnline==INVALID_HANDLE_VALUE);
	m_WaitForeServiceStarting = CreateEvent(NULL,FALSE,FALSE,NULL);
	__super::OnStart();
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ֹͣʱ��Ӧ����
@param      ��    
@retval    bool
*/
//////////////////////////////////////////////////////////////////////////
bool CServerClientSvc::OnStop()
{
	try
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		m_SCClientStatus = SC_CONNECTION_OFF;
		if (m_ReconnectThread != NULL) 
		{
			CUtilThread::ShutdownThread(m_ReconnectThread,10*1000);
			m_ReconnectThread = NULL;
		}
		if(m_Sequence!=NULL)
		{
			m_Sequence->Completed.RemoveHandler(this,&CServerClientSvc::OnSequenceComplete);
			delete m_Sequence;
			m_Sequence = NULL;
		}
		g_pSCControl->BeforeDoCommand.RemoveHandler(this,&CServerClientSvc::OnSCSendCommand);
		StopTimer(RECONNECT_TIMER);
		m_RequestExitMonitorThread = TRUE;
		if(m_hThreadMonitorOnline!=NULL && m_hThreadMonitorOnline!=INVALID_HANDLE_VALUE)
		{
			DWORD exitCode;
			GetExitCodeThread(m_hThreadMonitorOnline,&exitCode);
			if(exitCode == STILL_ACTIVE)
			{
				QueueUserAPC(ExitMonitorThread,m_hThreadMonitorOnline,NULL);
				DWORD waitResult = WaitForSingleObject(m_hThreadMonitorOnline,10*1000);
				if(waitResult!=WAIT_OBJECT_0)
				{
					TerminateThread(m_hThreadMonitorOnline,NULL);
				}

			}
			CloseHandle(m_hThreadMonitorOnline);
			m_hThreadMonitorOnline = NULL;
		}
		CloseHandle(m_WaitForeServiceStarting);
		m_WaitForeServiceStarting = NULL;
		theClient.Shutdown();
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ʼ���Ϣ

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CServerClientSvc::PostMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	//�������������ӣ����ȼ���ߡ�
	if(message == SC_MSG_RECONNECT )
	{
		return m_ReconnectThread->PostThreadServiceMsg(message,wParam,lParam);
	}
	//if (message == SC_MSG_SEND_DEVICE_FULLSTATUS)
	//{
	//	return SendTVMFullStatus(STATUS_TYPE_FULL,NULL);
	//}

	//��λ��Ϣ���ȼ���֮
	if(message == SC_MSG_DATAMSG )
	{
		CServiceThread* processThread = (CServiceThread*) CUtilThread::StartThread(RUNTIME_CLASS(CServiceThread), "CServiceThread");
		processThread->SetService(this);
		m_SCProcessThreads.push_back(processThread);
		return processThread->PostThreadServiceMsg(message,wParam,lParam);
	}
	DWORD currentThreadID = GetCurrentThreadId();
	if(message == SC_MSG_SEND_TO_SC)
	{
		if(currentThreadID == GetServiceThread()->m_nThreadID)
		{
			return GetServiceThread()->SendThreadServiceMsg(message,wParam,lParam);
		}
	}
	
	for(list<CServiceThread*>::iterator iter = m_SCProcessThreads.begin();iter!=m_SCProcessThreads.end();iter++)
	{
		if((*iter)->m_nThreadID == currentThreadID)
		{
			return (*iter)->SendThreadServiceMsg(message,wParam,lParam);
		}
	}
	//�������
	return __super::PostMessage(message,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::SendMessage(UINT message, WPARAM wParam, LPARAM lParam )
{
	//�������������ӣ����ȼ���ߡ�
	if(message == SC_MSG_RECONNECT )
	{
		return m_ReconnectThread->SendThreadServiceMsg(message,wParam,lParam);
	}
	if (message == SC_MSG_SEND_DEVICE_FULLSTATUS)
	{
		return SendTVMFullStatus(wParam,lParam);
	}
	//��λ��Ϣ���ȼ���֮
	if(message == SC_MSG_DATAMSG )
	{
		CServiceThread* processThread = (CServiceThread*) CUtilThread::StartThread(RUNTIME_CLASS(CServiceThread), "CServiceThread");
		processThread->SetService(this);
		m_SCProcessThreads.push_back(processThread);
		return processThread->SendThreadServiceMsg(message,wParam,lParam);
	}
	DWORD currentThreadID = GetCurrentThreadId();
	for(list<CServiceThread*>::iterator iter = m_SCProcessThreads.begin();iter!=m_SCProcessThreads.end();iter++)
	{
		if((*iter)->m_nThreadID == currentThreadID)
		{
			return (*iter)->SendThreadServiceMsg(message,wParam,lParam);
		}
	}
	//�������
	return __super::SendMessage(message,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ͬ����������Ժ�
@param      CRequestItem* sequence ����    
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnSequenceComplete(CCommand* sequence)
{
	try
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		m_Sequence = NULL;
		if(sequence->GetResultCode()!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return;
		}
		sequence->Completed.RemoveHandler(this,&CServerClientSvc::OnSequenceComplete);
		CConnectSequence* connectSequence = dynamic_cast<CConnectSequence*>(sequence);
		if(connectSequence!=NULL)
		{
			m_SCClientStatus =  connectSequence->GetResultCode() == SP_SUCCESS ? SC_CONNECTION_ON : SC_CONNECTION_OFF;
			if(m_SCClientStatus==SC_CONNECTION_ON)
			{
				theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("SC_CONNECTION_ON"));
				PostSCConnected();
			}
		}
		// ��Ӫ��ʼʱ����Ӧ������Ϣ
		CStartOperationSequence* startSequence = dynamic_cast<CStartOperationSequence*>(sequence);

		if(NULL != startSequence){
			delete startSequence;
			startSequence = NULL;
		}
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ�߼����ӽ����ɹ���������
@param      ��    
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::PostSCConnected()
{

	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	
	theAPP_SESSION.SetSCConnected(true);
	

	// ����������λʱ��
	theTVM_SETTING.SetLastOnlineDate(ComGetCurDate());
	// ����г���������������쳣
	if(theEXCEPTION_MGR.HasException(CSCException::MODULE_ID,CSCException::NOT_CONNECT_SERVER_OUT_OF_TIME)){
		theEXCEPTION_MGR.RemoveException(CSCException::MODULE_ID,CSCException::NOT_CONNECT_SERVER_OUT_OF_TIME);
	}

	m_SCClientStatus = SC_CONNECTION_ON;
	bool sucess =	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_NET_STATUS,NULL,m_SCClientStatus);
	SendTVMFullStatus(STATUS_TYPE_FULL,NULL);
	SendTVMError(NULL,theEXCEPTION_MGR.HasException() ? theEXCEPTION_MGR.GetActualMTC(theEXCEPTION_MGR.GetLastException()) : 0);

	//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceRegisterData(AR_IN_SERVICE);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->SendBusinessData();
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendTransactionData();
	// �¼�����
	//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendEventData();
	// �ϴ��������������
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendPackageMsgTransAudit();
	// �ϴ�Ʊ���汨��
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTicketBoxCountReport();
	// �ϴ�Ǯ���汨��
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendMoneyBoxCountReport();
	// ������ʱ��
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->StartTicketAuditTimer();
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->StartPackageAuditTimer();
	theTVM_STATUS_MGR.SetCommunicate(CONNECTED);

	

	//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->StartTicketAuditTimer();
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->StartBusinessIntervalTimer();
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->StartTransmissionTimer();	
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	if(theTVM_STATUS_MGR.GetServiceStatus() == IN_SERVICE)
	{
		//this->PostMessage(SC_MSG_STARTOPERATION,NULL,NULL);
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ�߼����ӶϿ���������
@param      ��    
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::PostSCDisConnected()
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	theAPP_SESSION.SetSCConnected(false);
	theAPP_SESSION.SetPhysicalConnStatus(false);
	CService* pMainSvc = theSERVICE_MGR.GetService(MAIN_SVC);
	if(pMainSvc!=NULL)
	{
		pMainSvc->PostMessage(SM_NET_STATUS,NULL,m_SCClientStatus);
	}
	theTVM_STATUS_MGR.SetCommunicate(DISCONNECT);
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndPackageAuditTimer();
	//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndTicketAuditTimer();
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->EndBusinessIntervalTimer();
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->EndTransmissionTimer();
	CCloseOperationSequence* closeSeq = dynamic_cast<CCloseOperationSequence*>(m_Sequence);
	if(closeSeq==NULL)//û��ҵ����������У�����������ʱ
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T(""));
		//����������ʱ
		StartTimer(RECONNECT_TIMER,RECONNECT_INTERVAL);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T(""));
	}
	if(m_Sequence!=NULL)
	{
		m_Sequence->OnComplete(SC_ERR_NOT_RESPONSE);
		m_Sequence = NULL;
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����controlʱ�Ļص�����
@param      CRequestItem* command Ҫִ�еĺ���    
@retval     �ܷ�ִ��
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnSCSendCommand(CCommand* command)
{
	try
	{
		CSCCommand* sccommand = dynamic_cast<CSCCommand*>(command);
		if(sccommand==NULL)
		{
			return SP_SUCCESS;
		}
		if(m_Sequence==NULL)//����ͬ��������
		{
			if(m_SCClientStatus<SC_CONNECTION_ON)//δ����״̬
			{
				sccommand->OnComplete(SC_ERR_NOT_RESPONSE);
				return SC_ERR_NOT_RESPONSE;
			}
			return SP_SUCCESS;
		}
		else
		{
			if(m_Sequence->CanAcceptSCCommand(sccommand))
			{
				sccommand->Completed.AddHandler(this,&CServerClientSvc::OnSCSendCommandComplete);
				return SP_SUCCESS;
			}
			else//ͬ�����в����ܸ�����
			{
				sccommand->OnComplete(SC_ERR_INVALID_SEQUENCE);
				this->PostMessage(SC_MSG_RECONNECT,NULL,NULL);//���ߣ�����
				theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("SC_MSG_RECONNECT"));
				return SC_ERR_INVALID_SEQUENCE;
			}
		}
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnSCSendCommandComplete(CCommand* command)
{
	try
	{
		CSCCommand* sccommand = dynamic_cast<CSCCommand*>(command);

		if(NULL == sccommand) return;

		sccommand->Completed.RemoveHandler(this,&CServerClientSvc::OnSCSendCommandComplete);
		if(m_Sequence==NULL)
		{
			return ;
		}
		m_Sequence->OnSCCommandComplete(sccommand);
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}

}



//////////////////////////////////////////////////////////////////////////
/**
@brief      �ͻ�������������Ϣ��������
@param   WPARAM   wParam ������
@param	LPARAM	  lParam	CSCCommand*
@retval     long m_lErrorCode �������
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnSendMsgToSC(WPARAM wParam,LPARAM lParam)
{
	try
	{
		// ��Ӫ��ʼʱ����Ӧ������Ϣ
		CStartOperationSequence* startSequence = dynamic_cast<CStartOperationSequence*>(m_Sequence);

		if(NULL != startSequence){
			return SP_SUCCESS;
		}

		CSCCommand* command = (CSCCommand*)lParam;

		if(NULL == command || NULL == g_pSCControl){
			return FALSE;
		}

		if(command->GetAutoDeleted())
		{
			command->SetAutoDeleted(false);
			auto_ptr<CSCCommand> pCommand(command);
			g_pSCControl->DoCommand(pCommand.get());
			return WaitForCommandComplete(pCommand.get());
		}
		else
		{
			g_pSCControl->DoCommand(command);
			return WaitForCommandComplete(command);
		}
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����������������ʱ��
@param      WPARAM wParam
@param      LPARAM lParam    
@retval     LRESULT  �������
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnReconnect(WPARAM wParam,LPARAM lParam)
{
	try
	{
		SYNC(CONNECT,_T("CONNECT"));
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		theClient.Close();
		m_SCClientStatus = SC_CONNECTION_OFF;
		PostSCDisConnected();

		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ����Ӧ����
@param      UINT nEventID
@retval    void
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnTimer(UINT nEventID)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(nEventID == RECONNECT_TIMER && (!theAPP_SESSION.IsSCConnected()))
	{
		StopTimer(RECONNECT_TIMER);
		OnConnect(NULL,NULL);
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���SC�Ƿ������߳�
@param      LPVOID CServerClientSvc;
@retval     DWORD
*/
//////////////////////////////////////////////////////////////////////////
DWORD CServerClientSvc::MonitorOnline(LPVOID lpVoid)
{
	CServerClientSvc* pSvc = (CServerClientSvc*)lpVoid;
	if(pSvc == NULL)
	{
		return 1;
	}
	while(!pSvc->m_RequestExitMonitorThread)
	{
		::SleepEx(10*60*1000,TRUE);//��Ϣ10����,�ɻ���
		if(!pSvc->m_RequestExitMonitorThread && !theAPP_SESSION.IsSCConnected())
		{
			if(pSvc->m_Sequence == NULL)
			{
				//pSvc->PostMessage(SC_MSG_CONNECT,NULL,NULL);
				pSvc->OnConnect(NULL,NULL);
			}
		}
	}
	return 0;
}

void  CServerClientSvc::ExitMonitorThread(ULONG_PTR dwParam){

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      SLE����ʱ��
@param      WPARAM wParam������
@param      LPARAM lParam������
@retval    void
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnConnect(WPARAM wParam,LPARAM lParam)
{
	try
	{
		SYNC(CONNECT,_T("CONNECT"));
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		bool connectSuccess = theClient.Connect(theMAINTENANCE_INFO.GetCpsHostID(),theMAINTENANCE_INFO.GetCpsPort());
		if(!connectSuccess)
		{
			OnReconnect(NULL,NULL);
			return FALSE;
		}
		//�½�һ����������
		if(m_Sequence!=NULL)
		{
			delete m_Sequence;
			m_Sequence = NULL;
		}
		m_Sequence = new CConnectSequence;
		m_Sequence->SetAutoDeleted(TRUE);
		m_Sequence->Completed.AddHandler(this,&CServerClientSvc::OnSequenceComplete);
		long result = ConnectAuthenticate();

		if(SP_SUCCESS == result){
			theAPP_SESSION.SetPhysicalConnStatus(true);
		}
		else{
			theAPP_SESSION.SetPhysicalConnStatus(false);
		}
		//WaitForCommandComplete(m_Sequence);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ������֤����
@retval    ��֤���
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::ConnectAuthenticate()
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	//����������֤����
	CMD_HEADER header = CHeaderManager::AquireHeader(PROTOCAL_DATA,CMD_CONNECT_REQUEST);
	auto_ptr<CSCConnectRequest> pConnectRequest(new CSCConnectRequest);
	pConnectRequest->SetHeader(&header);
	BYTE lpContent[LEN_REQ_CONNECT_AUTH] = {0};
	theSCMessageMakeHelper.MakeConnectAuthRequest(lpContent);
	pConnectRequest->SetContent(lpContent);
	pConnectRequest->SetIsNeedReply(TRUE);
	 g_pSCControl->DoCommand(pConnectRequest.get());
	 long connectRequestExecResult =WaitForCommandComplete(pConnectRequest.get(),3*60);
	//������֤����ִ���Ƿ�ɹ�
	if(connectRequestExecResult!=SP_SUCCESS)
	{
		OnReconnect(NULL,NULL);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("-> failed"));
		return connectRequestExecResult;
	}
	//������֤����
	CSCConnectRequestReply* connectRequestReply = (CSCConnectRequestReply*)pConnectRequest->GetReplyCommand();
	if(connectRequestReply == NULL || connectRequestReply->GetContent()==NULL || connectRequestReply->GetContent()->nLen<3)
	{
		OnReconnect(NULL,NULL);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("-> failed"));
		return SP_ERR_CONNECTION_LOST;
	}
	if(connectRequestReply->GetContent()->lpData[2] != 0x01)
	{
		OnReconnect(NULL,NULL);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("-> failed"));
		return SP_ERR_CONNECTION_LOST;
	}

	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ҵ���������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnCloseOperation(WPARAM wParam,LPARAM lParam)
{
	try{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		// ֹͣ��������Ƽ�ʱ��
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndPackageAuditTimer();
		// ֹͣƱ���汨���ʱ��
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndTicketAuditTimer();
		// ֹͣҵ�������ϴ���ʱ��
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->EndBusinessIntervalTimer();
		// ֹͣ���������ϴ���ʱ��
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->EndTransmissionTimer();
		
		// ���ð��������Ϊ����״̬:zhengxianle
		//theTVM_STATUS_MGR.SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS_CLOSE);

		// ������Ӫ������������
		if(m_Sequence!=NULL){
			delete m_Sequence;
			m_Sequence = NULL;
		}
		m_Sequence = new CCloseOperationSequence;
		m_Sequence->SetAutoDeleted(TRUE);
		m_Sequence->Completed.AddHandler(this,&CServerClientSvc::OnSequenceComplete);

		// �����豸��������ҵ������
		//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceComponentInfo();
		
		// ����Ʊ���汨��
		//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTicketBoxCountReport();


		//�ϴ�������־(���ڹػ���ʱ���ϴ���������־ѹ����)
		try{			
			//theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEBUG_UPLOAD);
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		
		// ֪ͨ��λ��Ӫ����
		//SendTVMSingleStatus(TVM_STATUS_IDS::RUN_STATUS,CLOSEING);
		SendTVMFullStatus(TVM_STATUS_IDS::RUN_STATUS,CLOSEING); // zhong �ϴ�ȫ״̬
		//ǿ��ʱ��ͬ��
		try{			
			theSNTP_HELPER.ForceTimeSynchronize();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}

		// SLE�սᴦ��
		try {
			if (theAPP_SESSION.GetCloseStartFlag() == CS_CLOSE) {  
				CAuditHelper* m_AuditHelper = new CAuditHelper(*this);
				m_AuditHelper->PrintBalanceBill(theAPP_SESSION.IsManualCloseOperation());
				//theSCAudit_MGR.ClearCurrentBusinessDay();
				delete m_AuditHelper;
				m_AuditHelper = NULL;
			}
		}
		catch (CSysException& e) 
		{
			theEXCEPTION_MGR.WriteExceptionLog(e,CExceptionLog::LEVEL_ERROR);
		}
		catch (...) {
			theEXCEPTION_MGR.WriteExceptionLog(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__),CExceptionLog::LEVEL_ERROR);
		}
		// ���潻����˽������� ���������޸ý������ݣ�
		/*theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertTransactionAudit(theTxnAudit_MGR.GetCurrentTxnAudit());
		theTxnAudit_MGR.SetCurrentTxnAuditSended();*/
		
		// ����SC������� mzy
		//theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(theSCAudit_MGR.GetCurrentDayPurchase(), END_OF_DAY);
		//theSCAudit_MGR.SetCurrentDayPurchaseSended();
		
		// �����豸�Ĵ�������
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceRegisterData(AR_OUTOF_SERVICE);
		
		// �ϴ�����ҵ������
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->SendBusinessData();

		//theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertDeviceAR(AR_BUSINESS_DAY_END);

		// �ϴ����н�������
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendTransactionData();

		// �ϴ������¼�����
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendEventData();

		// ���ð�������ƽ���ʱ��
		thePEKG_AUDIT.SetEndDateTime(ComGetCurTime());
		
		//�ϴ��������������
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendPackageMsgTransAudit();

		if(m_SCClientStatus<SC_CONNECTION_ON){
			delete m_Sequence;
			m_Sequence = NULL;
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SP_SUCCESS;
		}
		WaitForCommandComplete(m_Sequence,theTVM_INFO.GetFinishToReadyTimerInterval() * 60);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ӫ��ʼ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnStartOperation(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	if(m_SCClientStatus==SC_CONNECTION_ON){
		if(m_Sequence == NULL){
			m_Sequence = new CStartOperationSequence;
			m_Sequence->SetAutoDeleted(TRUE);
			m_Sequence->Completed.AddHandler(this,&CServerClientSvc::OnSequenceComplete);
			//������Ӫ״̬Ϊҵ�������
			theTVM_STATUS_MGR.SetServiceStatus(IN_SERVICE);
			SendTVMSingleStatus(TVM_STATUS_IDS::RUN_STATUS,OPERATION_STARTING);
			try{
				// ����20170628�����п���Ӫ��ʱ��ͬ��������������޸�
				//theSNTP_HELPER.ForceTimeSynchronize();
				_DATE dateBefore = ComGetBusiDate();
				theTVM_STATUS_MGR.SetTimeSynStatus(VALID_TIME);
				theEXCEPTION_MGR.RemoveException(CSNTPSException::MODULE_ID);
				//theSNTP_HELPER.ForceTimeSynchronize();
				theSNTP_HELPER.TimeSynchronize();//��ʼ��Ӫ��Ӧ��ǿ��ͬ�� ��ʼ��ʱ����ͬ�����ϻᱻ�������
				// ����TPUʱ��
				_DATE_TIME currentDateTime = ComGetCurTime();
				_DATE_TIME datetime;
				CARDRW_HELPER->SetTime(currentDateTime,datetime);
				RECHARGERW_HELPER->SetTime(currentDateTime,datetime);
				_DATE dateAfter = ComGetBusiDate();

				// ����Ӫ��ʱ��ͬ������Ҫ��Ӫ���л����սᡣ
				if(dateBefore != dateAfter){
					theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->PostMessage(SM_MAIN_ON_SWITCH_BUSINESS_DAY_OPERATION,NULL,NULL);
				}
				else{
					// ǿ��ʱ��ͬ��ʱ��ͣ������Ҫ����Ĭ��ҵ��
					if(OUTOF_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
						theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
					}
				}
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		}
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ���������ʱ��Ӧ����
@param      WPARAM wParam
@param      LPARAM lParam    
@retval     LRESULT  �������
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnScDataMsgArrival(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	CSCCommand* scDataMsg = (CSCCommand*)lParam;
	try
	{
		//û��ͬ�������У��������н��ܸ���λ����
		if(m_Sequence==NULL || m_Sequence->CanAcceptSCCommand(scDataMsg))
		{
			OnSCDataMsg(wParam,lParam);//��λ�����
			if(m_Sequence!=NULL)
			{
				m_Sequence->OnSCCommandComplete(scDataMsg);
			}
		}
		else//��ͬ�������У����Ҹ����в�������λ��������������
		{
			PostMessage(SC_MSG_RECONNECT,NULL,NULL);//�Ͽ�����
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("SC_MSG_RECONNECT"));
		}
		scDataMsg->OnComplete(SP_SUCCESS);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		DWORD currentThreadID = GetCurrentThreadId();
		for(list<CServiceThread*>::iterator iter = m_SCProcessThreads.begin();iter!=m_SCProcessThreads.end();iter++)
		{
			if((*iter)->m_nThreadID == currentThreadID)
			{
				m_SCProcessThreads.erase(iter);
				break;
			}
		}
		BOOL success = ::PostThreadMessage (currentThreadID, WM_QUIT, 0, 0);
		while(!success)
		{
			::Sleep(30);
			success = ::PostThreadMessage (currentThreadID, WM_QUIT, 0, 0);
		}
	}
	catch (CSysException& e)
	{
		DWORD currentThreadID = GetCurrentThreadId();
		for(list<CServiceThread*>::iterator iter = m_SCProcessThreads.begin();iter!=m_SCProcessThreads.end();iter++)
		{
			if((*iter)->m_nThreadID == currentThreadID)
			{
				m_SCProcessThreads.erase(iter);
				break;
			}
		}
		BOOL success = ::PostThreadMessage (currentThreadID, WM_QUIT, 0, 0);
		while(!success)
		{
			::Sleep(30);
			success = ::PostThreadMessage (currentThreadID, WM_QUIT, 0, 0);
		}
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		DWORD currentThreadID = GetCurrentThreadId();
		for(list<CServiceThread*>::iterator iter = m_SCProcessThreads.begin();iter!=m_SCProcessThreads.end();iter++)
		{
			if((*iter)->m_nThreadID == currentThreadID)
			{
				m_SCProcessThreads.erase(iter);
				break;
			}
		}
		BOOL success = ::PostThreadMessage (currentThreadID, WM_QUIT, 0, 0);
		while(!success)
		{
			::Sleep(30);
			success = ::PostThreadMessage (currentThreadID, WM_QUIT, 0, 0);
		}
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}

	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
WORD CServerClientSvc::GetCommandMsgCode(CSCCommand* scCommand)
{
	CMD_HEADER& header =  scCommand->GetHeader();
	LPVARIABLE_DATA pContent = scCommand->GetContent();
	if(header.bDataTransType == CONTROL_DATA || header.bDataTransType == STATUS_DATA)
	{
		if(pContent!=NULL&&pContent->nLen>2)
		{
			return MAKEWORD(*(pContent->lpData),*(pContent->lpData+1));
		}
	}
	if(header.bDataTransType == TRANSACTION_DATA || header.bDataTransType == OPER_DATA)
	{
		if(pContent!=NULL && pContent->nLen>LEN_PACKAGE_HEADER)//28���ֽڰ�ͷ
		{
			BYTE packageType = *(pContent->lpData+1);
			if(packageType == 1)//acc����
			{
				return MAKEWORD(*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER+10),*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER+9));
			}
			if(packageType == 2)//afc����
			{
				return MAKEWORD(*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER + LEN_TXN_DATATYPE-1),*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER + LEN_TXN_DATATYPE-2));
			}
			if(packageType == 4 || packageType == 5) //ҵ������
			{
				return MAKEWORD(*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_BUSINESS_HEADER_DATATYPE-1),*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_BUSINESS_HEADER_DATATYPE-2));
			}
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λ���������ʱ��Ӧ����
@param      WPARAM wParam
@param      LPARAM lParam    
@retval     LRESULT  �������
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnSCDataMsg(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	CSCCommand* scDataMsg = (CSCCommand*)lParam;

	WORD msgCode =  GetCommandMsgCode(scDataMsg);
	switch(msgCode)
	{
	case MSG_SPECIFY_BLOCKNO_DATA:
		OnUploadSpecifyBlockNOData(scDataMsg);
		break;
	case MSG_SPECIFY_TIME_DATA:
		OnUploadSpecifyTimeData(scDataMsg);//Ԥ��
		break;
	case MSG_DEBUG_DATA:
		OnUploadDebugData(scDataMsg);//Ԥ��
		break;
	case MSG_OPERATION_DATA:
		OnOperationModeControl(scDataMsg);
		break;
	case MSG_DEVICE_CONTROL_COMMAND:
		OnDeviceControl(scDataMsg);
		break;
	case MSG_24HOUR_OPERATION_DATA:
		On24HourOperation(scDataMsg);
		break;
	case MSG_DELAY_OPERATION_DATA:
		OnDelayOperation(scDataMsg);
		break;
	case MSG_FORCE_TIME_SYNC:
		OnForceTimeSync(scDataMsg);
		break;
	case MSG_ISSUE_RESTRICT:
		OnIssueRestrict(scDataMsg);
		break;
	case MSG_UPLOAD_PARAMETER_ID:
		OnUploadParameterVersion(scDataMsg);
		break;
	case MSG_UPDATE_PARAMETER:
		OnUpdateParameter(scDataMsg);
		break;
	case MSG_FORCE_LOGOUT:
		OnForceLogout(scDataMsg);
		break;
	case MSG_REQUEST_DEVICE_STATUS:
		OnRequestDeviceStatus(scDataMsg);
		break;
	case MSG_AUTORUN_PARAMETER:
		OnAutorunParameter(scDataMsg);
		break;
	case MSG_ACCOUNT_UNLOCK_NOTIC:
		OnAccountUnlockNotice(scDataMsg);
		break;
	case MSG_FTP_CONFIG_NOTIC:
		OnFtpConfigNotice(scDataMsg);
		break;
	case MSG_OP_END_NOTIC:
		OnOpEndNotice(scDataMsg);
		break;
	//case MSG_NONIMMIDATELY_SURRENDER_APPLY_RESULT:
	//	OnNonImmidatelySurrenderQueryResultNotice(scDataMsg);
	//	break;
	//case MSG_SIGNCARD_APPLY_RESULT:
	//	OnSignCardApplyQueryResultNotice(scDataMsg);
		break;
	default:
		return SC_ERR_INVLID_CONTENT;
		break;
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �ϴ�ָ��������
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadSpecifyBlockNOData(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	DATA_TYPE_CODE dataType = UNKNOW_DATA;
	vector<CString> vecBlockNOs;
	theSCMessageParaseHelper.ParseUploadSpecifyBlockNOData(lpContent,dataType,vecBlockNOs);
	for(int i=0;i<vecBlockNOs.size();i++)
	{
		PEKG_DATA pekgData;
		pekgData.strPekgID = vecBlockNOs[i];
		TRANSFER_DATA_TYPE transferDataType = TRANSACTION_DATA;
		bool findPekg = false;
		switch(dataType)
		{
		case ACC_TRANS:
			findPekg = theACC_TXN_MGR.GetDataPekgById(pekgData);
			transferDataType = TRANSACTION_DATA;
			break;
		case ECT_TRANS:
			findPekg = theECT_TXN_MGR.GetDataPekgById(pekgData);
			transferDataType = TRANSACTION_DATA;
			break;
		case AFC_TRANS:
			findPekg = theAFC_TXN_MGR.GetDataPekgById(pekgData);
			transferDataType = TRANSACTION_DATA;
			break;
		case BUS_DATA:
			findPekg = theBUS_TXN_MGR.GetDataPekgById(pekgData);
			transferDataType = TRANSFER_DATA_TYPE::OPER_DATA;
			break;
		case EVEN_DATA:
			//theTVM_STATUS_MGR.SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS_WARNING);// ������ƴ���Ϊ����״̬��zhengxianle
			findPekg = theEVN_TXN_MGR.GetDataPekgById(pekgData);
			transferDataType = TRANSFER_DATA_TYPE::OPER_DATA;
			break;
		default:
			break;
		}
		
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("DataType=%d   PekgID = %s  Found=%d"),dataType,pekgData.strPekgID,findPekg);
		if(findPekg)
		{
			auto_ptr<CSCDataMsg> pUploadSpecifyBlockNO ( new CSCDataMsg);
			//pUploadSpecifyBlockNO->SetResponseNTID(scDataMsg->GetHeader().nwTransId);
			CMD_HEADER header = CHeaderManager::AquireHeader(transferDataType,CMD_DATA_TRANSFER);
			pUploadSpecifyBlockNO->SetHeader(&header);
			pUploadSpecifyBlockNO->SetContent(pekgData.lpData,pekgData.nLen);
			g_pSCControl->DoCommand(pUploadSpecifyBlockNO.get());
			WaitForCommandComplete(pUploadSpecifyBlockNO.get());
		}
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    ָ��ʱ������
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadSpecifyTimeData(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �ϴ��豸��������
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadDebugData(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData + 2;
	_DATE specifyDate;
	CString ftpUploadDir;
	theSCMessageParaseHelper.ParseUploadDebugData(lpContent,specifyDate,ftpUploadDir);
	//theSETTING.SetFileUploadFTPDir(ftpUploadDir);
	if(CLogHelper::UploadLogFile(specifyDate))
	{
		SendOpEndNotice(UPLOAD_COMPLETE);
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    ��Ӫģʽ����
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnOperationModeControl(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	WORD mode;
	theSCMessageParaseHelper.ParseOperationData(lpContent,mode);
	theTVM_STATUS_MGR.SetOperationMode((OperationalMode_t)mode);
	/*if(mode == DEVICE_MODE_EMERGENCY_EXIT)
	{
		ForceForegroundLogout(FORCE_LOGOUT_EMERGENCY_MODE,true);
	}*/
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    �豸���п���
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnDeviceControl(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	WORD mode;
	theSCMessageParaseHelper.ParseDeviceControl(lpContent,mode);
	bool needLogout = false;

	WORD wServiceMode = theTVM_STATUS_MGR.GetWorkMode();
	WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
	WORD wChangeMode = theTVM_STATUS_MGR.GetChangeMode();
	if(wServiceMode==DEVICE_CONTRL_CODE_BUSINESS_ALL && wPayment==DEVICE_CONTRL_CODE_PAYMENT_ALL && (wChangeMode==DEVICE_CONTRL_CODE_ALL_CHANGE||wChangeMode==DEVICE_CONTRL_CODE_NO_BILL_CHANGE)){//��ֽ�����㣬Ҳ��������
		if(mode==DEVICE_CONTRL_CODE_SC_RECOVER_NORMAL_MODE){
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return 0;
		}
	}
	else{
		if(mode>=DEVICE_CONTRL_CODE_SC_BANKNOTE_ONLY && mode<=DEVICE_CONTRL_CODE_SC_NO_PRINT){
			if(mode!=DEVICE_CONTRL_CODE_SC_RECOVER_NORMAL_MODE){
				theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
				return 0;
			}
		}
	}

	if (mode == DEVICE_CONTRL_CODE_SC_BANKNOTE_ONLY){
		// ֻ����ֽ�ң����ܽ���Ӳ�Һʹ�ֵ��
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)== DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_SVT) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		// 2. �޸������ļ��е�֧����ʽ
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)== DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_SVT) | DEVICE_CONTRL_CODE_PAYMENT;
		}
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_COIN_ONLY){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)== DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_SVT) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		WORD wWordMode = theTVM_STATUS_MGR.GetWorkMode();
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE)== DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
			wWordMode = (wWordMode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		theTVM_STATUS_MGR.SetWorkMode(wWordMode);
		// 2. �޸������ļ��е�֧����ʽ
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)== DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_SVT) | DEVICE_CONTRL_CODE_PAYMENT;
		}
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_NO_CHANGE){
		// ֧��ֽ�Һʹ�ֵ������֧��Ӳ�ң�Ӳ��hopper��գ�
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		// 2. �޸������ļ��е�֧����ʽ
		WORD wPaymentIni = theTVM_SETTING.GetPayMentMode();
		if ((wPaymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPaymentIni |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}
		if ((wPaymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPaymentIni |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}
		if ((wPaymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPaymentIni |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}
		theTVM_SETTING.SetPayMentMode(wPaymentIni);
		// �޸�����ģʽ
		WORD wChangeMode = theTVM_STATUS_MGR.GetChangeMode();
		if ((wChangeMode & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)== DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
			wChangeMode = (wChangeMode ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
		}	
		theTVM_STATUS_MGR.SetChangeMode(wChangeMode);

		WORD wChangeModeIni = theTVM_SETTING.GetChangeMode();
		if ((wChangeModeIni & DEVICE_CONTRL_CODE_NO_BILL_CHANGE)== DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
			wChangeModeIni = (wChangeModeIni ^ DEVICE_CONTRL_CODE_NO_BILL_CHANGE) | DEVICE_CONTRL_CODE_NO_CHANGE;
		}	
		theTVM_SETTING.SetChangeMode(wChangeModeIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_ONLY_SVT_ISSUE){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		WORD wWordMode = theTVM_STATUS_MGR.GetWorkMode();
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE)== DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
			wWordMode = (wWordMode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		theTVM_STATUS_MGR.SetWorkMode(wWordMode);
		// 2. �޸������ļ��е�֧����ʽ
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
		}
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_NOT_ISSUE){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wWordMode = theTVM_STATUS_MGR.GetWorkMode();
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_CARD)== DEVICE_CONTRL_CODE_BUSINESS_CARD){
			wWordMode = (wWordMode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		theTVM_STATUS_MGR.SetWorkMode(wWordMode);
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		// 2. �޸������ļ��е�֧����ʽ
		WORD wWordModeIni = theTVM_SETTING.GetServiceMode();
		if ((wWordModeIni & DEVICE_CONTRL_CODE_BUSINESS_CARD)== DEVICE_CONTRL_CODE_BUSINESS_CARD){
			wWordModeIni = (wWordModeIni ^ DEVICE_CONTRL_CODE_BUSINESS_CARD) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		theTVM_SETTING.SetServiceMode(wWordModeIni);
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}	
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_ONLY_ANALY){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wWordMode = theTVM_STATUS_MGR.GetWorkMode();
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_CARD)== DEVICE_CONTRL_CODE_BUSINESS_CARD){
			wWordMode = (wWordMode ^ DEVICE_CONTRL_CODE_BUSINESS_CARD) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE)== DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
			wWordMode = (wWordMode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_QUERY)!= DEVICE_CONTRL_CODE_BUSINESS_QUERY){
			wWordMode |=  DEVICE_CONTRL_CODE_BUSINESS_QUERY;
		}
		theTVM_STATUS_MGR.SetWorkMode(wWordMode);
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		// 2. �޸������ļ��е�֧����ʽ
		WORD wWordModeIni = theTVM_SETTING.GetServiceMode();
		if ((wWordModeIni & DEVICE_CONTRL_CODE_BUSINESS_CARD)== DEVICE_CONTRL_CODE_BUSINESS_CARD){
			wWordModeIni = (wWordModeIni ^ DEVICE_CONTRL_CODE_BUSINESS_CARD) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		if ((wWordModeIni & DEVICE_CONTRL_CODE_BUSINESS_CHARGE)== DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
			wWordModeIni = (wWordModeIni ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		if ((wWordModeIni & DEVICE_CONTRL_CODE_BUSINESS_QUERY)!= DEVICE_CONTRL_CODE_BUSINESS_QUERY){
			wWordModeIni |=  DEVICE_CONTRL_CODE_BUSINESS_QUERY;
		}

		theTVM_SETTING.SetServiceMode(wWordModeIni);
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_NOT_SVT){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}	
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)== DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_SVT) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		// 2. �޸������ļ��е�֧����ʽ
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)== DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_SVT) | DEVICE_CONTRL_CODE_PAYMENT;
		}
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_NOT_BANKNOTE){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		WORD wWordMode = theTVM_STATUS_MGR.GetWorkMode();
		if ((wWordMode & DEVICE_CONTRL_CODE_BUSINESS_CHARGE)== DEVICE_CONTRL_CODE_BUSINESS_CHARGE){
			wWordMode = (wWordMode ^ DEVICE_CONTRL_CODE_BUSINESS_CHARGE) | DEVICE_CONTRL_CODE_BUSINESS;
		}
		theTVM_STATUS_MGR.SetWorkMode(wWordMode);
		// 2. �޸������ļ��е�֧����ʽ
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)!= DEVICE_CONTRL_CODE_PAYMENT_COIN){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_COIN;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)== DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) | DEVICE_CONTRL_CODE_PAYMENT;
		}	
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	else if (mode == DEVICE_CONTRL_CODE_SC_NOT_COIN){
		// 1. �޸�ʵʱ��֧����ʽ
		WORD wPayment = theTVM_STATUS_MGR.GetPaymentMode();
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
			wPayment = (wPayment ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
		}		
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}	
		if ((wPayment & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			wPayment |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}	
		theTVM_STATUS_MGR.SetPaymentMode(wPayment);
		// 2. �޸������ļ��е�֧����ʽ
		WORD paymentIni = theTVM_SETTING.GetPayMentMode();
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_COIN)== DEVICE_CONTRL_CODE_PAYMENT_COIN){
			paymentIni = (paymentIni ^ DEVICE_CONTRL_CODE_PAYMENT_COIN) | DEVICE_CONTRL_CODE_PAYMENT;
		}		
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)!= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE;
		}
		if ((paymentIni & DEVICE_CONTRL_CODE_PAYMENT_SVT)!= DEVICE_CONTRL_CODE_PAYMENT_SVT){
			paymentIni |= DEVICE_CONTRL_CODE_PAYMENT_SVT;
		}
		theTVM_SETTING.SetPayMentMode(paymentIni);
	}
	// �豸��������
	else{
		theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_DEVICE_CONTROL,mode,NULL);
	}
	if(needLogout){
		ForceForegroundLogout(FORCE_LOGOUT_DEVICE_CONTROL_SHUTDOWN,true);
	}	

	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief    24Сʱ��Ӫ����
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::On24HourOperation(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	BYTE mode;
	theSCMessageParaseHelper.Parse24HourMode(lpContent,mode);
	if(mode != 0x00 && mode != 0x01)
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return SC_ERR_INVLID_CONTENT;
	}
	theTVM_STATUS_MGR.Set24HourMode((OPERATION_24HOURS_MODE)mode);
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �ӳ���Ӫʱ��
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnDelayOperation(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	WORD delayTime=0x00;
	theSCMessageParaseHelper.ParseDelayOperationData(lpContent,delayTime);
	theTVM_STATUS_MGR.SetDelayTime(delayTime);
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ǿ��ʱ��ͬ��
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnForceTimeSync(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	_DATE dateBefore = ComGetBusiDate();
	theTVM_STATUS_MGR.SetTimeSynStatus(VALID_TIME);
	theEXCEPTION_MGR.RemoveException(CSNTPSException::MODULE_ID);
	theSNTP_HELPER.ForceTimeSynchronize();
	// ����TPUʱ��
	_DATE_TIME currentDateTime = ComGetCurTime();
	_DATE_TIME datetime;
	CARDRW_HELPER->SetTime(currentDateTime,datetime);
	//TOKENRW_HELPER->SetTime(currentDateTime,datetime);
	//RECHARGERW_HELPER->SetTime(currentDateTime,datetime);

	_DATE dateAfter = ComGetBusiDate();

	// ����Ӫ��ʱ��ͬ������Ҫ��Ӫ���л����սᡣ
	if(dateBefore != dateAfter){
		theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->PostMessage(SM_MAIN_ON_SWITCH_BUSINESS_DAY_OPERATION,NULL,NULL);
	}
	else{
		// ǿ��ʱ��ͬ��ʱ��ͣ������Ҫ����Ĭ��ҵ��
		if(OUTOF_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
		}
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ʊ����
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnIssueRestrict(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	BYTE bType = 0x00;
	int iStationCode = 0;
	theSCMessageParaseHelper.ParseIssueRestrict(lpContent,bType,iStationCode);
	switch(bType)
	{
	case 0x00:
		theISSUE_PERMIT.DeleteAllIssueStation();
		break;
	case 0x01:
		theISSUE_PERMIT.DeleteIssueStation(iStationCode);
		break;
	case 0x02:
		theISSUE_PERMIT.AddIssueStationPermit(iStationCode);
		break;
	default:
		break;
	}
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ϴ������汾
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadParameterVersion(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	BYTE lpContent[2*1024] = {0};
	int contentLength = 0;
	theSCMessageMakeHelper.MakeParamAndSoftwareVersionInfo(lpContent,contentLength);
	auto_ptr<CSCDataMsgReply> pParameterVersionReply(new CSCDataMsgReply);
	pParameterVersionReply->SetContent(lpContent,contentLength);
	pParameterVersionReply->SetResponseNTID(scDataMsg->GetHeader().nwTransId);
	CMD_HEADER header= CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER_REPLY);
	pParameterVersionReply->SetHeader(&header);
	g_pSCControl->DoCommand(pParameterVersionReply.get());
	long result = WaitForCommandComplete(pParameterVersionReply.get());
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return result;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �ϴ������汾
@param      
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendParameterVersion()
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	BYTE lpContent[2*1024] = {0};
	int contentLength = 0;
	theSCMessageMakeHelper.MakeParamAndSoftwareVersionInfo(lpContent,contentLength);
	auto_ptr<CSCDataMsg> pParameterVersion(new CSCDataMsg);
	pParameterVersion->SetContent(lpContent,contentLength);
	CMD_HEADER header= CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
	pParameterVersion->SetHeader(&header);
	g_pSCControl->DoCommand(pParameterVersion.get());
	long result = WaitForCommandComplete(pParameterVersion.get());
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �����ͳ������
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUpdateParameter(CSCCommand* scDataMsg)
{
	try
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
		vector<ParameterAndSoftwareUpdate> updates;
		bool parseSuccess = theSCMessageParaseHelper.ParseParameterAndSoftwareUpdate(lpContent,updates);
		if(!parseSuccess)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_INVLID_CONTENT;
		}
		long updateImmidiateParameterResult = SP_SUCCESS;
		bool needReloadParameter = false;
		for (vector<ParameterAndSoftwareUpdate>::iterator iter = updates.begin();iter!=updates.end();iter++)
		{
			if(iter->parameterID == AFC_MODEHISTORY_ID)
			{
				this->SendMessage(SC_MSG_DOWNLOAD_IMMIDIATE_PARAMETER,NULL,iter->version);
				updates.erase(iter);
				needReloadParameter = true;
				break;
			}
		}
		if(updates.size()>0)
		{
			theUPGRADE_HELPER::DownLoadFiles(updates);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDownloadParameterAndProgram(updates);
			// ɾ��Э��ͷ��Э��β��
			if(theFunction_INFO.IsDeleteParameterHead()){
				TCHAR szRunPath[MAX_PATH];
				ComGetAppPath(szRunPath, MAX_PATH);	
				CString sUpdatePath = CString(szRunPath) + PM_UPDATE_DIR;
				thePARAM_HELPER.RemoveParamTransHead(sUpdatePath);
			}
			CParamHelper::VEC_VERSION_INFO VecVersionInfo;
			thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
			needReloadParameter = true;
		}
		if(needReloadParameter)
		{
			//ǰ̨�ǳ�
			ForceForegroundLogout(FORCE_LOGOUT_NEWPARAMETER,true);
			// ��ʼ�����в���		
			if(!thePARAM_HELPER.InitAllParam()){	
				theEXCEPTION_MGR.ProcessException(CLogException(CLogException::FILE_READ_FAIL, _T(__FILENAME__), __LINE__,_T("��ʼ�������쳣")));
			}	
			// ���������ļ���ģ���ļ�
			theUPGRADE_HELPER::UpdateVoiceAndTemplateFolder();
			// ����Ӧ�ó���GUI�ļ�
			if(theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade())
			{
				theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
				theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
				theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
				theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
				return SP_SUCCESS;
			}
		}
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}

	OnUploadParameterVersion(scDataMsg);
	
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    ǿ���˳���¼����
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnForceLogout(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	ForceForegroundLogout(FORCE_LOGOUT_SERVER_REQUEST,true);
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �豸״̬��ѯ����
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnRequestDeviceStatus(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	SendTVMFullStatus(STATUS_TYPE_FULL,NULL);
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Զ����в������ÿ���
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnAutorunParameter(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData + 2;
	// ���ÿ�����������
	lpContent++;
	// ������Ч��Χ(��ʱ)
	lpContent++;
	// ���ñ��
	theTVM_STATUS_MGR.SetAutoRunStatus((DEVICE_AUTORUN)*lpContent);
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �˻�����֪ͨ
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnAccountUnlockNotice(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	int staffID = 0;
	theSCMessageParaseHelper.ParseUnlockStaff(lpContent,staffID);
	thePWD_ERR.ClearTheStaffNoInfo(staffID);
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      FTP����֪ͨ
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnFtpConfigNotice(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	CString csIPAdress = _T("");
	csIPAdress.Format(_T("%d.%d.%d.%d"), lpContent[0],lpContent[1],lpContent[2],lpContent[3]);
	theMAINTENANCE_INFO.SetFtpHostID(csIPAdress);

	char HostName[8]={0};
	for (int i = 0; i<8; i++)
	{
		HostName[i] += lpContent[4+i];
	}
	USES_CONVERSION;
	CString csHostName = A2T(HostName);
	// ����FTP��������½�û���
	theMAINTENANCE_INFO.SetFtpUserID(csHostName);

	// ����FTP��������½������
	// BCD2PSTR()
	char ciperPassword[18]={0};
	BCD2PSTR((char*)lpContent+12,8,ciperPassword);
	char plainPassword[18] = {0};
	decrypt((unsigned char*)ciperPassword,8,plainPassword);
	decrypt((unsigned char*)ciperPassword+8,8,plainPassword+8);
	CString strPassword = A2T(plainPassword);

	theMAINTENANCE_INFO.SetFtpPwd(strPassword);

	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������֪ͨ
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnOpEndNotice(CSCCommand* scDataMsg)
{
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ǽ�ʱ�˿���ѯ���֪ͨ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnNonImmidatelySurrenderQueryResultNotice(CSCCommand* scDataMsg)
{
	//!!
	//LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	//CRefundApplyResult::REFUND_APP_RESULT refund_app_result;
	//lpContent+=LEN_TXN_SYSCOM_HDR;
	//memcpy(refund_app_result.ticketApplicationSerialNo,lpContent+2,sizeof(refund_app_result.ticketApplicationSerialNo));
	//lpContent+=(LEN_TXN_TICKETCOM_HDR);
	//refund_app_result.Balance = ComMakeLong(*(lpContent+3),*(lpContent + 2),*(lpContent + 1),*lpContent);
	//lpContent += 4;
	//refund_app_result.certificateType = (CERTIFICATE_TYPE)*lpContent;
	//lpContent+=1;
	//memcpy(refund_app_result.certificateID,lpContent,sizeof(refund_app_result.certificateID));
	//lpContent += 20;
	//memcpy(refund_app_result.ReceiptNum,lpContent,sizeof(refund_app_result.ReceiptNum));
	//lpContent += 10;
	//refund_app_result.Deposit = ComMakeLong(*(lpContent+3),*(lpContent + 2),*(lpContent + 1),*lpContent);
	//lpContent += 4;
	//refund_app_result.Status = *lpContent;
	//!!theREFUND_APPLY_RRSULT.UpdateTheApplyResult(refund_app_result);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  ��������������ѯ���֪ͨ    

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnSignCardApplyQueryResultNotice(CSCCommand* scDataMsg)
{
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	lpContent+=(LEN_TXN_SYSCOM_HDR + LEN_TXN_TICKETCOM_HDR+LEN_TXN_OPERATORCOM_HDR);
	LPBYTE lpBillNum = lpContent;
	BYTE status = *(lpBillNum + 21);
	//!!theNAMED_CARD_APPLY_RRSULT.UpdateTheApplyResult(lpBillNum,status);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ��������
@param      CSCCommand* scDataMsg ��������
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnRequestDownloadImmidiateParameter(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	BYTE lpMsg[LEN_REQ_SEND_IMMIDIATLY_PARAMETER]={0};
	theSCMessageMakeHelper.MakeImmediatelyParameterDownloadRequst(lpMsg,(WORD)lParam);
	CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);

	auto_ptr<CSCDataMsg> pDownImmidiateParameterRequest(new CSCDataMsg);
	pDownImmidiateParameterRequest->SetHeader(&header);
	pDownImmidiateParameterRequest->SetContent(lpMsg);
	pDownImmidiateParameterRequest->SetIsNeedReply(TRUE);
	g_pSCControl->DoCommand(pDownImmidiateParameterRequest.get());
	long requestExecuteResult =WaitForCommandComplete(pDownImmidiateParameterRequest.get());
	if(requestExecuteResult!=SP_SUCCESS)
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return requestExecuteResult;
	}
	DWORD dwVersion;
	memcpy(&dwVersion,pDownImmidiateParameterRequest->GetReplyCommand()->GetContent()->lpData+10,sizeof(DWORD));
	theSOFT_VERSION.SetModeHistoryVer(dwVersion);
	thePARAM_MSG.SaveMsg(pDownImmidiateParameterRequest->GetReplyCommand()->GetContent()->lpData+2);

	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ͳ������֪ͨ
@param      NOTICE_TYPE ֪ͨ����
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendOpEndNotice(NOTICE_TYPE type)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	try{
		BYTE lpContent[LEN_NOTICE_OPERATION_END]={0};
		theSCMessageMakeHelper.MakeOpEndNotice(lpContent,type);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pSCDataMsg(new CSCDataMsg);
		pSCDataMsg->SetHeader(&header);
		pSCDataMsg->SetContent(lpContent);
		g_pSCControl->DoCommand(pSCDataMsg.get());
		long result = WaitForCommandComplete(pSCDataMsg.get());
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILENAME__), __LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����TVM����״̬��Ϣ
@param     WPARAM wParam ������ 
@param     LPARAM lParam MTC�� 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::SendTVMError(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	try
	{
		if(m_SCClientStatus < SC_CONNECTION_ON)
		{
			return SCSVC_RESULT_NOT_CONNECTED;
		}
		BYTE lpContent[LEN_STATUS_HAS_ERROR]={0};
		theSCMessageMakeHelper.MakeDeviceErrorStatus(lpContent,(long)lParam);
		CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBOMErrorDataMsg(new CSCDataMsg);
		pBOMErrorDataMsg->SetHeader(&header);
		pBOMErrorDataMsg->SetContent(lpContent);
		g_pSCControl->DoCommand(pBOMErrorDataMsg.get());
		long result = WaitForCommandComplete(pBOMErrorDataMsg.get());
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return result;

	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����tvmȫ״̬
@param     WPARAM wParam ȫ״̬���ͣ����ֻ�ȫ�� 
@param     LPARAM lParam ������ 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::SendTVMFullStatus(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	try
	{
		STATUS_TYPE statusType = (STATUS_TYPE)wParam;
		UINT uiLength = 0;//LEN_STATUS_HEADER + 2;
		// TVM 48��״̬��Ҫ�ϱ� 
		uiLength = statusType == STATUS_TYPE_THREESTATUS ? LEN_STATUS_THREE:LEN_STATUS_TVM_FULL;

		BYTE lpContent[LEN_STATUS_TVM_FULL] = {0};
		theSCMessageMakeHelper.MakeTVMFullStatus(statusType,lpContent);
		CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBOMErrorDataMsg(new CSCDataMsg);
		pBOMErrorDataMsg->SetHeader(&header);
		pBOMErrorDataMsg->SetContent(lpContent,uiLength);
		g_pSCControl->DoCommand(pBOMErrorDataMsg.get());
		long result = WaitForCommandComplete(pBOMErrorDataMsg.get());
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����tvmȫ״̬
@param     WPARAM wParam ȫ״̬���ͣ����ֻ�ȫ�� 
@param     LPARAM lParam ������ 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendTVMFullStatus(TVM_STATUS_ID tvmStatusID,BYTE value)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	try
	{
		if(m_SCClientStatus < SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SCSVC_RESULT_NOT_CONNECTED;
		}

		BYTE lpContent[LEN_STATUS_TVM_FULL] = {0};
		theSCMessageMakeHelper.MakeTVMFullStatus(tvmStatusID,value,lpContent);
		CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBOMErrorDataMsg(new CSCDataMsg);
		pBOMErrorDataMsg->SetHeader(&header);
		pBOMErrorDataMsg->SetContent(lpContent);
		g_pSCControl->DoCommand(pBOMErrorDataMsg.get());
		long result = WaitForCommandComplete(pBOMErrorDataMsg.get());
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ե�һ״̬����TVMȫ״̬
@param     TVM_STATUS_ID bomStatusID ״̬ID 
@param     BYTE value ֵ 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendTVMSingleStatus(TVM_STATUS_ID bomStatusID,BYTE value)
{
	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
	try
	{
		if(m_SCClientStatus < SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SCSVC_RESULT_NOT_CONNECTED;
		}

		BYTE lpContent[LEN_STATUS_SINGLE_MODE] = {0};
		theSCMessageMakeHelper.MakeTVMFullStatus(bomStatusID,value,lpContent);
		CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBOMErrorDataMsg(new CSCDataMsg);
		pBOMErrorDataMsg->SetHeader(&header);
		pBOMErrorDataMsg->SetContent(lpContent);
		g_pSCControl->DoCommand(pBOMErrorDataMsg.get());
		long result = WaitForCommandComplete(pBOMErrorDataMsg.get());
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//bool CServerClientSvc::UploadLogFile(_DATE dateOfLog)
//{
//	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
//	CString uploadFileFullPath = "";
//	vector<_DATE> logDates;
//	logDates.push_back(dateOfLog);
//	CLogHelper::PrepareUploadLogFile(uploadFileFullPath,logDates);
//	vector<CString> uploadFiles;
//	uploadFiles.push_back(uploadFileFullPath);
//	CString sUploadFtpPath = theSETTING.GetFileUploadFTPDir();
//	bool success = theFTP_HELPER.UploadFiles(uploadFiles,sUploadFtpPath) == 1;
//	DeleteFile(uploadFileFullPath);
//	theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEBUG_UPLOAD);
//	return success;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Bom��¼�ǳ�����
@param      int nOperatorID       ����ԱID
int nPwd              ����
LOGIN_TYPE LoginType  ��������
@retval     ִ�н��
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestLoginLogoff(int nOperatorID, CString sPwd, LOGIN_TYPE LoginType,BYTE& result)
{
	try
	{

		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_LOGINLOGOFF]={0};

		char* pPassword = NULL;
		char pEncryptPwd[6+2] = {0};

		USES_CONVERSION;
		pPassword = T2A(sPwd);
		encrypt((const unsigned char*)pPassword,6,pEncryptPwd);// ���������

		theSCMessageMakeHelper.MakeLoginOrLogoutRequest(LoginType,nOperatorID,(LPBYTE)pEncryptPwd,lpMsg);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pLoginOrOutRequest(new CSCDataMsg);
		pLoginOrOutRequest->SetHeader(&header);
		pLoginOrOutRequest->SetContent(lpMsg);
		pLoginOrOutRequest->SetIsNeedReply(TRUE);
		g_pSCControl->DoCommand(pLoginOrOutRequest.get());
		long requestExecuteResult =WaitForCommandComplete(pLoginOrOutRequest.get(),3*60);
		if(requestExecuteResult!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return requestExecuteResult;
		}
		theSCMessageParaseHelper.ParseLoginOrOutReply(pLoginOrOutRequest->GetReplyCommand()->GetContent()->lpData+2,result);
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ա�����޸ģ�Ԥ����
@param      CHANGE_STAFFPASSWORD  ����Ա��Ϣ
Ա��ID
������
������
@retval     0x00���Ƿ��޸ģ��޸�ʧ�ܣ�0x01�������޸ĳɹ���0x02�������ظ����޸�ʧ�ܣ�
0x03�����޸�����Ȩ�ޣ��޸�ʧ�ܣ�0x04������Ա�����ڣ��޸�ʧ�ܣ�
0x05������Ա��ͣ�û���ֹ���޸�ʧ�ܣ� 0xFF��ͨѶ�жϣ����ܽ��������޸�
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestChangeStaffPwd(CHANGE_STAFFPASSWORD& staffInfo,BYTE& result)
{
	try
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_CHANGE_PASSWORD]={0};
		//ת���ɶ��ֽ�
		USES_CONVERSION;
		LPSTR lpOldPlainPassword = T2A((LPTSTR)staffInfo.sOldPassword.GetString());
		LPSTR lpNewPlainPassword = T2A((LPTSTR)staffInfo.sNewPassword.GetString());
		//����
		char ciperOldPassword[6+2] = {0};
		encrypt((const unsigned char*)lpOldPlainPassword,6,ciperOldPassword);
		char ciperNewPassword[6+2] = {0};
		encrypt((const unsigned char*)lpNewPlainPassword,6,ciperNewPassword);
		//ת����BCD
		PSTR2BCD(ciperOldPassword,6,(char*)staffInfo.ciperOldPwd);
		PSTR2BCD(ciperNewPassword,6,(char*)staffInfo.ciperNewPwd);

		theSCMessageMakeHelper.MakeReformPasswordRequest(staffInfo, lpMsg);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg>  pChangePasswordRequest(new CSCDataMsg);
		pChangePasswordRequest->SetHeader(&header);
		pChangePasswordRequest->SetContent(lpMsg);
		pChangePasswordRequest->SetIsNeedReply(TRUE);
		g_pSCControl->DoCommand(pChangePasswordRequest.get());
		long requestExecuteResult =WaitForCommandComplete(pChangePasswordRequest.get());
		if(requestExecuteResult!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return requestExecuteResult;
		}
		theSCMessageParaseHelper.ParseChangePasswordReply(pChangePasswordRequest->GetReplyCommand()->GetContent()->lpData+2,result);
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;

}


//////////////////////////////////////////////////////////////////////////
/*
@brief    ����Query ������ѯ���Ի���Ϣ 

@param    (in)PersonalizationQuery& query ��ѯ����
@param    (out)vector<Personalization>& result ��ѯ���

@retval     long SP_SUCCESS �ɹ�������ʧ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestPersonlization(PersonalizationQuery& query,vector<Personalization>& result)
{
	try
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_PERSONALIZATION]={0};
		theSCMessageMakeHelper.MakePersonalizationRequest(query, lpMsg);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg>  pChangePasswordRequest(new CSCDataMsg);
		pChangePasswordRequest->SetHeader(&header);
		pChangePasswordRequest->SetContent(lpMsg);
		pChangePasswordRequest->SetIsNeedReply(TRUE);
		g_pSCControl->DoCommand(pChangePasswordRequest.get());
		long requestExecuteResult =WaitForCommandComplete(pChangePasswordRequest.get());
		if(requestExecuteResult!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return requestExecuteResult;
		}
		theSCMessageParaseHelper.ParsePersonalizationReply(pChangePasswordRequest->GetReplyCommand()->GetContent()->lpData+2,result);
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ����Query ������ѯ��ʷ��Ϣ 

@param    (in)HistoryInfoQuery& query ��ѯ����
@param    (out)HistoryInfo& result ��ѯ���

@retval     long SP_SUCCESS �ɹ�������ʧ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestHistoryInfo(HistoryInfoQuery& query,vector<HistoryProductInfo>& result)
{
	try
	{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_PERSONALIZATION]={0};
		DWORD lenth = 0;
		theSCMessageMakeHelper.MakeHistoryInfoRequest(query, lpMsg,lenth);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg>  pChangePasswordRequest(new CSCDataMsg);
		pChangePasswordRequest->SetHeader(&header);
		pChangePasswordRequest->SetContent(lpMsg,lenth);
		pChangePasswordRequest->SetIsNeedReply(TRUE);
		g_pSCControl->DoCommand(pChangePasswordRequest.get());
		long requestExecuteResult =WaitForCommandComplete(pChangePasswordRequest.get());
		if(requestExecuteResult!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return requestExecuteResult;
		}
		theSCMessageParaseHelper.ParseHistoryInfoQuerryReply(pChangePasswordRequest->GetReplyCommand()->GetContent()->lpData+2,result);
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILENAME__),__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief   ǿ��ǰ̨�ǳ�   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CServerClientSvc::ForceForegroundLogout(FORCELOGOUTREASON reason,bool waitLogout)
{
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	if(pMainSvc!=NULL)
	{
		if(pMainSvc->IsServiceBusyOnPassger() || theTVM_STATUS_MGR.GetLoginStatus() == LOGIN_TYPE::LOGIN_ON)// �˿Ͳ�������վԱ�Ѿ���¼
		{
			if(waitLogout)//�����Ҫ�ȴ��ǳ�����������Ҫ�ȴ���ǰ���е�ҵ�����
			{
				//�ȴ���ǰ����ҵ�����
				pMainSvc->StartingForeService.AddHandler(this,&CServerClientSvc::HookStarttingForeService);
				WaitForSingleObject(m_WaitForeServiceStarting,INFINITE);
				::Sleep(20);
			}
		}
		if(theTVM_STATUS_MGR.GetLoginStatus() == LOGIN_TYPE::LOGIN_ON){
			if(waitLogout)
			{
				pMainSvc->SendMessage(SM_FORCE_LOGOUT,reason,NULL);
			}
			else
			{
				pMainSvc->PostMessage(SM_FORCE_LOGOUT,reason,NULL);
			}
		}
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ǰ�˷���

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::HookStarttingForeService(SERVICE_ID serviceID,bool* cancelStartting)
{
	CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
	if(pMainSvc!=NULL)
	{
		pMainSvc->StartingForeService.RemoveHandler(this,&CServerClientSvc::HookStarttingForeService);
		*cancelStartting = true;//ǰ̨ҵ������
		SetEvent(m_WaitForeServiceStarting);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ������Ԯ��Ϣ

@param      bool��true��ȡ����Ԯ��false��������Ԯ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestCallHelp(bool bIsCallHelp){
	try{
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("<-"));
		!bIsCallHelp ? theTVM_STATUS_MGR.SetAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS_PRESSED):theTVM_STATUS_MGR.SetAssistanceButtonStatus(ASSISTANCE_BUTTON_STATUS_UNPRESSED);
		if(!theAPP_SESSION.IsSCConnected()/*m_SCClientStatus<SC_CONNECTION_ON*/ /*|| bIsCallHelp*/)
		{
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_CALL_HELP];
		memset(lpMsg,0x00,sizeof(lpMsg));

		theSCMessageMakeHelper.MakeCallHelpRequest(lpMsg,false);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		

		CSCDataMsg* callHelpMsg = new CSCDataMsg; 
		callHelpMsg->SetHeader(&header);
		callHelpMsg->SetAutoDeleted(true);
		callHelpMsg->SetContent(lpMsg);
		callHelpMsg->SetIsNeedReply(TRUE);
		theSERVICE_MGR.GetService(SC_SVC)->PostMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)callHelpMsg);

		/*auto_ptr<CSCDataMsg> callHelp(new CSCDataMsg);
		callHelp->SetHeader(&header);
		callHelp->SetContent(lpMsg);
		callHelp->SetIsNeedReply(TRUE);
		g_pSCControl->DoCommand(callHelp.get());

		long result = WaitForCommandComplete(callHelp.get(),1);*/
		long result = SP_SUCCESS;

		if(result != SP_SUCCESS){
			theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
			return result;
		}	
		// ���������壬������
		theSC_Svc_Trace->WriteData(_T(__FILENAME__),_T(__FUNCTION__),__LINE__,_T("->"));
		return SP_SUCCESS;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}	
	return SP_ERR_INTERNAL_ERROR;
}