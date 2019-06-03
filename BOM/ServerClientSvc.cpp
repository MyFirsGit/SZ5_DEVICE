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
#include "bomstatusmgr.h"
#include "ctissuepermit.h"
#include "ctmessageaudit.h"
#include "ConnectSequence.h"
#include "transcationtranslatesvc.h"
#include "businesstranslatesvc.h"
#include "eventdatasvc.h"
#include "CloseOperationSequence.h"
#include "SntpApi.h"
#include "bomdef.h"
#include "CTLOGSoftVer.h"
#include "SCAuditManager.h"
#include "DebugLog.h"
#include "scsvcresult.h"
#include "loghelper.h"
#include "FTPHelper.h"
#include "Sync.h"
#include "setting.h"
#include "ParamMsg.h"
#include "TxnAuditManager.h"

#define  __FILENAME__ _T("")

#define theSC_Svc_Trace CFileLog::GetInstance(FILTER_SC_SVC_LOG)


BEGIN_MESSAGE_MAP(CServerClientSvc,CBackService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SC_MSG_SEND_TO_SC,OnSendMsgToSC)
	ON_SERVICE_MESSAGE(SC_MSG_SEND_ERR,SendBomError)
	ON_SERVICE_MESSAGE(SC_MSG_SEND_FULLSTATUS,SendBomFullStatus)
	ON_SERVICE_MESSAGE(SC_MSG_CONNECT,OnConnect)
	ON_SERVICE_MESSAGE(SC_MSG_DATAMSG,OnScDataMsgArrival)
	ON_SERVICE_MESSAGE(SC_MSG_RECONNECT,OnReconnect)
	ON_SERVICE_MESSAGE(SC_MSG_CLOSEOPERATION,OnCloseOperation)
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
@brief      服务启动时响应函数
@param      无    
@retval    bool
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnStart()
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
		m_hThreadMonitorOnline = CreateThread(NULL,0,&CServerClientSvc::MonitorOnline,this,0,&workThreadID);//创建监视线程
		::Sleep(100);
	}while(m_hThreadMonitorOnline==NULL || m_hThreadMonitorOnline==INVALID_HANDLE_VALUE);
	m_WaitForeServiceStarting = CreateEvent(NULL,FALSE,FALSE,NULL);

	__super::OnStart();
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      服务停止时响应函数
@param      无    
@retval    bool
*/
//////////////////////////////////////////////////////////////////////////
bool CServerClientSvc::OnStop()
{
	try
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      邮寄消息

@param      (i)UINT message
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
BOOL CServerClientSvc::PostMessage(UINT message,WPARAM wParam,LPARAM lParam)
{
	//重连及建立连接，优先级最高。
	if(message == SC_MSG_RECONNECT )
	{
		return m_ReconnectThread->PostThreadServiceMsg(message,wParam,lParam);
	}
	//上位消息优先级次之
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
	//其他情况
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
	//重连及建立连接，优先级最高。
	if(message == SC_MSG_RECONNECT )
	{
		return m_ReconnectThread->SendThreadServiceMsg(message,wParam,lParam);
	}
	//上位消息优先级次之
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
	//其他情况
	return __super::SendMessage(message,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      同步序列完成以后
@param      CRequestItem* sequence 序列    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnSequenceComplete(CCommand* sequence)
{
	try
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		m_Sequence = NULL;
		if(sequence->GetResultCode()!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return;
		}
		sequence->Completed.RemoveHandler(this,&CServerClientSvc::OnSequenceComplete);
		CConnectSequence* connectSequence = dynamic_cast<CConnectSequence*>(sequence);
		if(connectSequence!=NULL)
		{
			m_SCClientStatus =  connectSequence->GetResultCode() == SP_SUCCESS ? SC_CONNECTION_ON : SC_CONNECTION_OFF;
			if(m_SCClientStatus==SC_CONNECTION_ON)
			{
				theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("SC_CONNECTION_ON"));
				PostSCConnected();
			}
		}
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上位逻辑连接建立成功后续操作
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::PostSCConnected()
{

	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	
	theAPP_SESSION.SetSCConnected(true);
	m_SCClientStatus = SC_CONNECTION_ON;
	bool sucess =	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_NET_STATUS,NULL,m_SCClientStatus);
	SendBomFullStatus(STATUS_TYPE_FULL,NULL);
	SendBomError(NULL,theEXCEPTION_MGR.HasException() ? theEXCEPTION_MGR.GetActualMTC(theEXCEPTION_MGR.GetLastException()) : 0);

	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceRegisterData(AR_IN_SERVICE);
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->SendBusinessData();
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendTransactionData();
	// 发送票箱库存报告
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTicketBoxCountReport();
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendEventData();
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendPackageMsgTransAudit();
	theBOM_STATUS_MGR.SetCommunicate(CONNECTED);

	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->StartPackageAuditTimer();
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->StartTicketAuditTimer();
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->StartBusinessIntervalTimer();
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->StartTransmissionTimer();	
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上位逻辑连接断开后续操作
@param      无    
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::PostSCDisConnected()
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	theAPP_SESSION.SetSCConnected(false);
	CService* pMainSvc = theSERVICE_MGR.GetService(MAIN_SVC);
	if(pMainSvc!=NULL)
	{
		pMainSvc->PostMessage(SM_NET_STATUS,NULL,m_SCClientStatus);
	}
	theBOM_STATUS_MGR.SetCommunicate(DISCONNECT);
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndPackageAuditTimer();
	theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndTicketAuditTimer();
	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->EndBusinessIntervalTimer();
	theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->EndTransmissionTimer();
	CCloseOperationSequence* closeSeq = dynamic_cast<CCloseOperationSequence*>(m_Sequence);
	if(closeSeq==NULL)//没在业务结束序列中，开启重连计时
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(""));
		//开启重连计时
		StartTimer(RECONNECT_TIMER,RECONNECT_INTERVAL);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T(""));
	}
	if(m_Sequence!=NULL)
	{
		m_Sequence->OnComplete(SC_ERR_NOT_RESPONSE);
		m_Sequence = NULL;
	}
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      调用control时的回调函数
@param      CRequestItem* command 要执行的函数    
@retval     能否执行
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
		if(m_Sequence==NULL)//不在同步序列中
		{
			if(m_SCClientStatus<SC_CONNECTION_ON)//未连接状态
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
			else//同步序列不接受该命令
			{
				sccommand->OnComplete(SC_ERR_INVALID_SEQUENCE);
				this->PostMessage(SC_MSG_RECONNECT,NULL,NULL);//断线，重连
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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}

}



//////////////////////////////////////////////////////////////////////////
/**
@brief      客户端主动发送消息给服务器
@param   WPARAM   wParam 无意义
@param	LPARAM	  lParam	CSCCommand*
@retval     long m_lErrorCode 结果代码
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnSendMsgToSC(WPARAM wParam,LPARAM lParam)
{
	try
	{
		CSCCommand* command = (CSCCommand*)lParam;
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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重连命令，开启重连计时器
@param      WPARAM wParam
@param      LPARAM lParam    
@retval     LRESULT  结果代码
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnReconnect(WPARAM wParam,LPARAM lParam)
{
	try
	{
		SYNC(CONNECT,_T("CONNECT"));
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		theClient.Close();
		m_SCClientStatus = SC_CONNECTION_OFF;
		PostSCDisConnected();

		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      计时器响应命令
@param      UINT nEventID
@retval    void
*/
//////////////////////////////////////////////////////////////////////////
void CServerClientSvc::OnTimer(UINT nEventID)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	if(nEventID == RECONNECT_TIMER && (!theAPP_SESSION.IsSCConnected()))
	{
		StopTimer(RECONNECT_TIMER);
		OnConnect(NULL,NULL);
	}
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      监测SC是否在线线程
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
		::SleepEx(10*60*1000,TRUE);//休息10分种,可唤醒
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
@brief      SLE连接时序
@param      WPARAM wParam无意义
@param      LPARAM lParam无意义
@retval    void
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnConnect(WPARAM wParam,LPARAM lParam)
{
	try
	{
		SYNC(CONNECT,_T("CONNECT"));
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		bool connectSuccess = theClient.Connect(theMAINTENANCE_INFO.GetCpsHostID(),theMAINTENANCE_INFO.GetCpsPort());
		if(!connectSuccess)
		{
			OnReconnect(NULL,NULL);
			return FALSE;
		}
		//新建一个开机序列
		if(m_Sequence!=NULL)
		{
			delete m_Sequence;
			m_Sequence = NULL;
		}
		m_Sequence = new CConnectSequence;
		m_Sequence->SetAutoDeleted(TRUE);
		m_Sequence->Completed.AddHandler(this,&CServerClientSvc::OnSequenceComplete);
		long result = ConnectAuthenticate();
		//WaitForCommandComplete(m_Sequence);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     连接认证请求
@retval    认证结果
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::ConnectAuthenticate()
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	//发送连接认证请求
	CMD_HEADER header = CHeaderManager::AquireHeader(PROTOCAL_DATA,CMD_CONNECT_REQUEST);
	auto_ptr<CSCConnectRequest> pConnectRequest(new CSCConnectRequest);
	pConnectRequest->SetHeader(&header);
	BYTE lpContent[LEN_REQ_CONNECT_AUTH] = {0};
	theSCMessageMakeHelper.MakeConnectAuthRequest(lpContent);
	pConnectRequest->SetContent(lpContent);
	pConnectRequest->SetIsNeedReply(TRUE);
	 g_pSCControl->DoCommand(pConnectRequest.get());
	 long connectRequestExecResult =WaitForCommandComplete(pConnectRequest.get(),3*60);
	//连接认证请求执行是否成功
	if(connectRequestExecResult!=SP_SUCCESS)
	{
		OnReconnect(NULL,NULL);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("-> failed"));
		return connectRequestExecResult;
	}
	//连接认证反馈
	CSCConnectRequestReply* connectRequestReply = (CSCConnectRequestReply*)pConnectRequest->GetReplyCommand();
	if(connectRequestReply == NULL || connectRequestReply->GetContent()==NULL || connectRequestReply->GetContent()->nLen<3)
	{
		OnReconnect(NULL,NULL);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("-> failed"));
		return SP_ERR_CONNECTION_LOST;
	}
	if(connectRequestReply->GetContent()->lpData[2] != 0x01)
	{
		OnReconnect(NULL,NULL);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("-> failed"));
		return SP_ERR_CONNECTION_LOST;
	}

	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      业务结束处理

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnCloseOperation(WPARAM wParam,LPARAM lParam)
{
	try{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		// 停止包传输审计计时器
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndPackageAuditTimer();
		// 停止票箱库存报告计时器
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->EndTicketAuditTimer();
		// 停止业务数据上传计时器
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->EndBusinessIntervalTimer();
		// 停止交易数据上传计时器
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->EndTransmissionTimer();
		
		// 创建运营结束序列命令
		if(m_Sequence!=NULL){
			delete m_Sequence;
			m_Sequence = NULL;
		}
		m_Sequence = new CCloseOperationSequence;
		m_Sequence->SetAutoDeleted(TRUE);
		m_Sequence->Completed.AddHandler(this,&CServerClientSvc::OnSequenceComplete);

		// 保存设备部件构成业务数据
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceComponentInfo();
		
		// 发送票箱库存报告
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTicketBoxCountReport();


		//上传调试日志
		try{			
			//UploadLogFile(ComGetCurDate());
			CLogHelper::UploadLogFile(ComGetBusiDate());
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		//设置运营状态为业务结束中
		theBOM_STATUS_MGR.SetServiceStatus(STOP_SERVICE);
		SendBomFullStatus(BOM_STATUS_IDS::RUN_STATUS,CLOSEING);

		//强制时钟同步
		try{			
			theSNTP_HELPER.ForceTimeSynchronize();
		}
		catch(CSysException& e){
			theEXCEPTION_MGR.ProcessException(e);
		}
		// 保存交易审核交易数据
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertTransactionAudit(theTxnAudit_MGR.GetCurrentTxnAudit());
		theTxnAudit_MGR.SetCurrentTxnAuditSended();
		
		// 发送SC审计数据
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendBomAuditData(theSCAudit_MGR.GetCurrentDayPurchase(), END_OF_DAY);
		theSCAudit_MGR.SetCurrentDayPurchaseSended();
		
		// 保存设备寄存器数据
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgDeviceRegisterData(AR_OUTOF_SERVICE);
		
		// 上传所有业务数据
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->SendBusinessData();

		//theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->InsertBomAR(BOM_AR_BUSINESS_DAY_END);

		// 上传所有交易数据
		theSERVICE_MGR.GetService<CTransSvc>(TRANSMISSION_INTERVAL_SVC)->SendTransactionData();

		// 上传所有事件数据
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendEventData();

		// 设置包传输审计结束时间
		thePEKG_AUDIT.SetEndDateTime(ComGetCurTime());
		
		//上传包传输审计数据
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendPackageMsgTransAudit();

		if(m_SCClientStatus<SC_CONNECTION_ON){
			delete m_Sequence;
			m_Sequence = NULL;
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return SP_SUCCESS;
		}
		WaitForCommandComplete(m_Sequence,theBOM_INFO.GetCloseOperationMaxWaitSeconds());
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上位请求命令到达时响应函数
@param      WPARAM wParam
@param      LPARAM lParam    
@retval     LRESULT  结果代码
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnScDataMsgArrival(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	CSCCommand* scDataMsg = (CSCCommand*)lParam;
	try
	{
		//没在同步序列中，或者序列接受该上位命令
		if(m_Sequence==NULL || m_Sequence->CanAcceptSCCommand(scDataMsg))
		{
			OnSCDataMsg(wParam,lParam);//上位命令处理
			if(m_Sequence!=NULL)
			{
				m_Sequence->OnSCCommandComplete(scDataMsg);
			}
		}
		else//在同步序列中，并且该序列不接受上位发来的请求命令
		{
			PostMessage(SC_MSG_RECONNECT,NULL,NULL);//断开连接
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("SC_MSG_RECONNECT"));
		}
		scDataMsg->OnComplete(SP_SUCCESS);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
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
		if(pContent!=NULL && pContent->nLen>LEN_PACKAGE_HEADER)//28个字节包头
		{
			BYTE packageType = *(pContent->lpData+1);
			if(packageType == 1)//acc交易
			{
				return MAKEWORD(*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER+10),*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER+9));
			}
			if(packageType == 2)//afc交易
			{
				return MAKEWORD(*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER + LEN_TXN_DATATYPE-1),*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_AFC_SYS_HEADER + LEN_TXN_DATATYPE-2));
			}
			if(packageType == 4 || packageType == 5) //业务数据
			{
				return MAKEWORD(*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_BUSINESS_HEADER_DATATYPE-1),*(pContent->lpData+LEN_PACKAGE_HEADER+LEN_BUSINESS_HEADER_DATATYPE-2));
			}
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上位请求命令到达时响应函数
@param      WPARAM wParam
@param      LPARAM lParam    
@retval     LRESULT  结果代码
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnSCDataMsg(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	CSCCommand* scDataMsg = (CSCCommand*)lParam;
	WORD msgCode =  GetCommandMsgCode(scDataMsg);
	switch(msgCode)
	{
	case MSG_SPECIFY_BLOCKNO_DATA:
		OnUploadSpecifyBlockNOData(scDataMsg);
		break;
	case MSG_SPECIFY_TIME_DATA:
		OnUploadSpecifyTimeData(scDataMsg);
		break;
	case MSG_DEBUG_DATA:
		OnUploadDebugData(scDataMsg);
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
	case MSG_NONIMMIDATELY_SURRENDER_APPLY_RESULT:
		OnNonImmidatelySurrenderQueryResultNotice(scDataMsg);
		break;
	case MSG_SIGNCARD_APPLY_RESULT:
		OnSignCardApplyQueryResultNotice(scDataMsg);
		break;
	default:
		return SC_ERR_INVLID_CONTENT;
		break;
	}
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      上传指定包数据
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadSpecifyBlockNOData(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
			findPekg = theEVN_TXN_MGR.GetDataPekgById(pekgData);
			transferDataType = TRANSFER_DATA_TYPE::OPER_DATA;
			break;
		default:
			break;
		}
		
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("DataType=%d   PekgID = %s  Found=%d"),dataType,pekgData.strPekgID,findPekg);
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
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    指定时间数据
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadSpecifyTimeData(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief    上传设备调试数据
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadDebugData(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData + 2;
	_DATE specifyDate;
	CString ftpUploadDir;
	theSCMessageParaseHelper.ParseUploadDebugData(lpContent,specifyDate,ftpUploadDir);
	//theSETTING.SetFileUploadFTPDir(ftpUploadDir);
	if(CLogHelper::UploadLogFile(specifyDate))
	{
		SendOpEndNotice(UPLOAD_COMPLETE);
	}
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    运营模式控制
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnOperationModeControl(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	WORD mode;
	theSCMessageParaseHelper.ParseOperationData(lpContent,mode);
	theBOM_STATUS_MGR.SetRunMode((OperationalMode_t)mode);
	if(mode & DEVICE_MODE_EMERGENCY_EXIT)
	{
		ForceForegroundLogout(FORCE_LOGOUT_EMERGENCY_MODE,true);
	}
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    设备运行控制
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnDeviceControl(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	WORD mode;
	theSCMessageParaseHelper.ParseDeviceControl(lpContent,mode);
	theBOM_STATUS_MGR.SetWorkMode((WORK_MODE)HIBYTE(mode));
	ForceForegroundLogout(FORCE_LOGOUT_DEVICE_CONTROL,true);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    24小时运营控制
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::On24HourOperation(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	BYTE mode;
	theSCMessageParaseHelper.Parse24HourMode(lpContent,mode);
	if(mode != 0x00 && mode != 0x01)
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return SC_ERR_INVLID_CONTENT;
	}
	theBOM_STATUS_MGR.Set24HourMode(mode);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     延长运营时间
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnDelayOperation(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	WORD delayTime=0x00;
	theSCMessageParaseHelper.ParseDelayOperationData(lpContent,delayTime);
	theBOM_STATUS_MGR.SetDelayTime(delayTime);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     强制时钟同步
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnForceTimeSync(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	theBOM_STATUS_MGR.SetTimeSynStatus(VALID_TIME);
	theEXCEPTION_MGR.RemoveException(CSNTPSException::MODULE_ID);
	theSNTP_HELPER.ForceTimeSynchronize();
	// 设置TPU时钟
	CRWHelper rwHelper(*this);
	_DATE_TIME currentDateTime = ComGetCurTime();
	_DATE_TIME datetime;
	rwHelper.SetTime(currentDateTime,datetime);
	CTWHelper twHelper(*this);
	twHelper.SetTime(currentDateTime,datetime);
	//CRWSetTime pRwCmd();
	//pRwCmd.ExecuteCommand();
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      售票限制
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnIssueRestrict(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      上传参数版本
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUploadParameterVersion(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return result;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      上传参数版本
@param      
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendParameterVersion()
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	BYTE lpContent[2*1024] = {0};
	int contentLength = 0;
	theSCMessageMakeHelper.MakeParamAndSoftwareVersionInfo(lpContent,contentLength);
	auto_ptr<CSCDataMsg> pParameterVersion(new CSCDataMsg);
	pParameterVersion->SetContent(lpContent,contentLength);
	CMD_HEADER header= CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
	pParameterVersion->SetHeader(&header);
	g_pSCControl->DoCommand(pParameterVersion.get());
	long result = WaitForCommandComplete(pParameterVersion.get());
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     参数和程序更新
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnUpdateParameter(CSCCommand* scDataMsg)
{
	try
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
		vector<ParameterAndSoftwareUpdate> updates;
		bool parseSuccess = theSCMessageParaseHelper.ParseParameterAndSoftwareUpdate(lpContent,updates);
		if(!parseSuccess)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
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
			if(theBOM_INFO.GetUSBAuthFlag() == 1){
				TCHAR szRunPath[MAX_PATH];
				ComGetAppPath(szRunPath, MAX_PATH);	
				CString sUpdatePath = CString(szRunPath) + BOM_PM_UPDATE_DIR;
				thePARAM_HELPER.RemoveParamTransHead(sUpdatePath);
			}
			CParamHelper::VEC_VERSION_INFO VecVersionInfo;
			thePARAM_HELPER.UpdateLocalVersion(VecVersionInfo);
			theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgUpdateParameterAndProgram(VecVersionInfo);
			needReloadParameter = true;
		}
		if(needReloadParameter)
		{
			ForceForegroundLogout(FORCE_LOGOUT_NEWPARAMETER,true);
			// 初始化所有参数		
			if(thePARAM_HELPER.InitAllParam()){
				theACC_LINE.Internalize(true,theBOM_INFO.GetDataPath(),theBOM_INFO.GetBackupPath());
			}
			else{			
				//theEXCEPTION_MGR.ProcessException(CLogException(BUSINESS_INTERVAL_SVC,CLogException::FILE_READ_FAIL, __FILENAME__,__LINE__));
				theEXCEPTION_MGR.ProcessException(CLogException(CLogException::FILE_READ_FAIL, __FILENAME__, __LINE__,_T("初始化参数异常")));

			}	
			if(theUPGRADE_HELPER::CheckDownloadFileNeedUpgrade())
			{
				theUPGRADE_HELPER::MoveDownloadFileToUpgradeFolder();
				theAPP_SESSION.SetCloseStartFlag(CS_RESTART);
				theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);
				theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}

	OnUploadParameterVersion(scDataMsg);
	
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief    强制退出登录命令
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnForceLogout(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	ForceForegroundLogout(FORCE_LOGOUT_SERVER_REQUEST,true);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     设备状态查询命令
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnRequestDeviceStatus(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	SendBomFullStatus(STATUS_TYPE_FULL,NULL);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      自动运行参数启用控制
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnAutorunParameter(CSCCommand* scDataMsg)
{

	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      账户解锁通知
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnAccountUnlockNotice(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	int staffID = 0;
	theSCMessageParaseHelper.ParseUnlockStaff(lpContent,staffID);
	thePWD_ERR.ClearTheStaffNoInfo(staffID);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      FTP配置通知
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnFtpConfigNotice(CSCCommand* scDataMsg)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	CString csIPAdress = _T("");
	csIPAdress.Format(_T("%d.%d.%d.%d"), lpContent[0],lpContent[1],lpContent[2],lpContent[3]);
	theMAINTENANCE_INFO.SetFtpHostID(csIPAdress);
	CString csHostName = _T("");
	for (int i = 0; i<8; i++)
	{
		csHostName += lpContent[4+i];
	}
	// 设置FTP服务器登陆用户名
	theMAINTENANCE_INFO.SetFtpUserID(csHostName);

	// 设置FTP服务器登陆用密码
	char ciperPassword[18]={0};
    BCD2PSTR((char*)lpContent+12,8,ciperPassword);
    char plainPassword[18] = {0};
    decrypt((unsigned char*)ciperPassword,8,plainPassword);
    decrypt((unsigned char*)ciperPassword+8,8,plainPassword+8);
	USES_CONVERSION;
    CString strPassword = A2T(plainPassword);

    theMAINTENANCE_INFO.SetFtpPwd(strPassword);

    theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
    return SP_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      操作结束通知
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnOpEndNotice(CSCCommand* scDataMsg)
{
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      非及时退卡查询结果通知

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::OnNonImmidatelySurrenderQueryResultNotice(CSCCommand* scDataMsg)
{
	LPBYTE lpContent = scDataMsg->GetContent()->lpData+2;
	CRefundApplyResult::REFUND_APP_RESULT refund_app_result;
	lpContent+=LEN_TXN_SYSCOM_HDR;
	memcpy(refund_app_result.ticketApplicationSerialNo,lpContent+2,sizeof(refund_app_result.ticketApplicationSerialNo));
	lpContent+=(LEN_TXN_TICKETCOM_HDR);
	refund_app_result.Balance = ComMakeLong(*(lpContent+3),*(lpContent + 2),*(lpContent + 1),*lpContent);
	lpContent += 4;
	refund_app_result.certificateType = (CERTIFICATE_TYPE)*lpContent;
	lpContent+=1;
	memcpy(refund_app_result.certificateID,lpContent,sizeof(refund_app_result.certificateID));
	lpContent += 20;
	memcpy(refund_app_result.ReceiptNum,lpContent,sizeof(refund_app_result.ReceiptNum));
	lpContent += 10;
	refund_app_result.Deposit = ComMakeLong(*(lpContent+3),*(lpContent + 2),*(lpContent + 1),*lpContent);
	lpContent += 4;
	refund_app_result.Status = *lpContent;
	theREFUND_APPLY_RRSULT.UpdateTheApplyResult(refund_app_result);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  记名卡申请结果查询结果通知    

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
	theNAMED_CARD_APPLY_RRSULT.UpdateTheApplyResult(lpBillNum,status);
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      即时参数更新
@param      CSCCommand* scDataMsg 请求命令
@retval     void
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::OnRequestDownloadImmidiateParameter(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return requestExecuteResult;
	}
	DWORD dwVersion;
	memcpy(&dwVersion,pDownImmidiateParameterRequest->GetReplyCommand()->GetContent()->lpData+10,sizeof(DWORD));
	theSOFT_VERSION.SetModeHistoryVer(dwVersion);
	thePARAM_MSG.SaveMsg(pDownImmidiateParameterRequest->GetReplyCommand()->GetContent()->lpData+2);
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送操作完成通知
@param      NOTICE_TYPE 通知类型
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendOpEndNotice(NOTICE_TYPE type)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	try{
		BYTE lpContent[LEN_NOTICE_OPERATION_END]={0};
		theSCMessageMakeHelper.MakeOpEndNotice(lpContent,type);
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pSCDataMsg(new CSCDataMsg);
		pSCDataMsg->SetHeader(&header);
		pSCDataMsg->SetContent(lpContent);
		g_pSCControl->DoCommand(pSCDataMsg.get());
		long result = WaitForCommandComplete(pSCDataMsg.get());
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, __FILENAME__, __LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送Bom错误状态信息
@param     WPARAM wParam 无意义 
@param     LPARAM lParam MTC码 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::SendBomError(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
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
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return result;

	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送Bom全状态
@param     WPARAM wParam 全状态类型，三种或全部 
@param     LPARAM lParam 无意义 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CServerClientSvc::SendBomFullStatus(WPARAM wParam,LPARAM lParam)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	try
	{
		STATUS_TYPE statusType = (STATUS_TYPE)wParam;
		UINT uiLength = statusType == STATUS_TYPE_THREESTATUS ? LEN_STATUS_THREE:LEN_STATUS_FULL;
		BYTE lpContent[LEN_STATUS_FULL] = {0};
		theSCMessageMakeHelper.MakeBomFullStatus(statusType,lpContent);
		CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBOMErrorDataMsg(new CSCDataMsg);
		pBOMErrorDataMsg->SetHeader(&header);
		pBOMErrorDataMsg->SetContent(lpContent,uiLength);
		g_pSCControl->DoCommand(pBOMErrorDataMsg.get());
		long result = WaitForCommandComplete(pBOMErrorDataMsg.get());
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      以单一状态发送Bom全状态
@param     BOM_STATUS_ID bomStatusID 状态ID 
@param     BYTE value 值 
@retval     long
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::SendBomFullStatus(BOM_STATUS_ID bomStatusID,BYTE value)
{
	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
	try
	{
		if(m_SCClientStatus < SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return SCSVC_RESULT_NOT_CONNECTED;
		}

		BYTE lpContent[LEN_STATUS_SINGLE_MODE] = {0};
		theSCMessageMakeHelper.MakeBomFullStatus(bomStatusID,value,lpContent);
		CMD_HEADER header = CHeaderManager::AquireHeader(STATUS_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pBOMErrorDataMsg(new CSCDataMsg);
		pBOMErrorDataMsg->SetHeader(&header);
		pBOMErrorDataMsg->SetContent(lpContent);
		g_pSCControl->DoCommand(pBOMErrorDataMsg.get());
		long result = WaitForCommandComplete(pBOMErrorDataMsg.get());
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return result;
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//bool CServerClientSvc::UploadLogFile(_DATE dateOfLog)
//{
//	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
//	CString uploadFileFullPath = "";
//	vector<_DATE> logDates;
//	logDates.push_back(dateOfLog);
//	CLogHelper::PrepareUploadLogFile(uploadFileFullPath,logDates);
//	vector<CString> uploadFiles;
//	uploadFiles.push_back(uploadFileFullPath);
//	CString sUploadFtpPath = theSETTING.GetFileUploadFTPDir();
//	bool success = theFTP_HELPER.UploadFiles(uploadFiles,sUploadFtpPath) == 1;
//	DeleteFile(uploadFileFullPath);
//	theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
//	theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_DEBUG_UPLOAD);
//	return success;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Bom登录登出请求
@param      int nOperatorID       操作员ID
int nPwd              密码
LOGIN_TYPE LoginType  操作类型
@retval     执行结果
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestLoginLogoff(int nOperatorID, CString sPwd, LOGIN_TYPE LoginType,BYTE& result)
{
	try
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_LOGINLOGOFF]={0};
		char* pPlainText = NULL;
		USES_CONVERSION;
		pPlainText = T2A(sPwd);

		//char* pPlainText = sPwd.GetBuffer();
		char* pEncrypt = new char[sPwd.GetLength()+1];
		char* pPassword = new char[sPwd.GetLength()+1];

		encrypt((const unsigned char*)pPlainText, sPwd.GetLength(), pEncrypt);
		//changeString2Char(pEncrypt, pPassword, sPwd.GetLength());
		PSTR2BCD(pEncrypt,sPwd.GetLength(),pPassword);

		theSCMessageMakeHelper.MakeLoginOrLogoutRequest(LoginType,nOperatorID,(LPBYTE)pPassword,lpMsg);
		delete [] pEncrypt;
		delete [] pPassword;
		CMD_HEADER header = CHeaderManager::AquireHeader(CONTROL_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pLoginOrOutRequest(new CSCDataMsg);
		pLoginOrOutRequest->SetHeader(&header);
		pLoginOrOutRequest->SetContent(lpMsg);
		pLoginOrOutRequest->SetIsNeedReply(TRUE);
		g_pSCControl->DoCommand(pLoginOrOutRequest.get());
		long requestExecuteResult =WaitForCommandComplete(pLoginOrOutRequest.get(),3*60);
		if(requestExecuteResult!=SP_SUCCESS)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return requestExecuteResult;
		}
		theSCMessageParaseHelper.ParseLoginOrOutReply(pLoginOrOutRequest->GetReplyCommand()->GetContent()->lpData+2,result);
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
		return SP_SUCCESS;
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      操作员密码修改
@param      CHANGE_STAFFPASSWORD  操作员信息
员工ID
旧密码
新密码
@retval     0x00：非法修改，修改失败；0x01：密码修改成功；0x02：密码重复，修改失败；
0x03：无修改密码权限，修改失败；0x04：操作员不存在，修改失败；
0x05：操作员已停用或终止，修改失败； 0xFF：通讯中断，不能进行密码修改
*/
//////////////////////////////////////////////////////////////////////////
long CServerClientSvc::RequestChangeStaffPwd(CHANGE_STAFFPASSWORD& staffInfo,BYTE& result)
{
	/*
	try
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_CHANGE_PASSWORD]={0};
		char ciperPassWord[8]={0};
		char* lpPassword = NULL;
		// 原始密码
		lpPassword = staffInfo.sOldPassword.GetBuffer();
		encrypt((const unsigned char*)lpPassword, 6, ciperPassWord);
		//changeString2Char(ciperPassWord, (char*)staffInfo.ciperOldPwd, 6);
		PSTR2BCD(ciperPassWord,6,(char*)staffInfo.ciperOldPwd);

		lpPassword = staffInfo.sNewPassword.GetBuffer();
		encrypt((const unsigned char*)lpPassword, 6, ciperPassWord);
		//changeString2Char(ciperPassWord, (char*)staffInfo.ciperNewPwd, 6);
		PSTR2BCD(ciperPassWord,6,(char*)staffInfo.ciperNewPwd);

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
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return requestExecuteResult;
		}
		theSCMessageParaseHelper.ParseChangePasswordReply(pChangePasswordRequest->GetReplyCommand()->GetContent()->lpData+2,result);
		return SP_SUCCESS;
	}
	*/
	try
	{
		theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("<-"));
		if(m_SCClientStatus<SC_CONNECTION_ON)
		{
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
			return SC_ERR_NOT_RESPONSE;
		}
		BYTE lpMsg[LEN_REQ_CHANGE_PASSWORD]={0};
		//转换成多字节
		USES_CONVERSION;
		LPSTR lpOldPlainPassword = T2A((LPTSTR)staffInfo.sOldPassword.GetString());
		LPSTR lpNewPlainPassword = T2A((LPTSTR)staffInfo.sNewPassword.GetString());
		//加密
		char ciperOldPassword[6+2] = {0};
		encrypt((const unsigned char*)lpOldPlainPassword,6,ciperOldPassword);
		char ciperNewPassword[6+2] = {0};
		encrypt((const unsigned char*)lpNewPlainPassword,6,ciperNewPassword);
		//转换成BCD
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
			theSC_Svc_Trace->WriteData(__FILENAME__,_T(__FUNCTION__),__LINE__,_T("->"));
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
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,__FILENAME__,__LINE__));
	}
	return SP_ERR_INTERNAL_ERROR;

}

//////////////////////////////////////////////////////////////////////////
/*
@brief   强制前台登出   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CServerClientSvc::ForceForegroundLogout(FORCELOGOUTREASON reason,bool waitLogout)
{
	if(theBOM_STATUS_MGR.GetLoginStatus() == LOGIN_TYPE::LOGIN_ON)//已经登录
	{
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		if(pMainSvc!=NULL)
		{
			if(waitLogout)
			{
				pMainSvc->StartingForeService.AddHandler(this,&CServerClientSvc::HookStarttingForeService);
				WaitForSingleObject(m_WaitForeServiceStarting,INFINITE);
				::Sleep(20);
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
@brief      拦截前端服务

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
		*cancelStartting = true;
		SetEvent(m_WaitForeServiceStarting);
	}
}

