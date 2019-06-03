
#pragma once
#include "stdafx.h"
#include "scexp.h"
#include "UtilThread.h"
#include "UtilList.h"
#include "Command.h"

class CService;
class CSCSocket;
class CSCCommand;
class CSCClientHelper;

#define theClient CSCClient::GetInstance()

/*
协议层收发命令类
*/

class SC_API CSCClient : CUtilThread
{

	DECLARE_MESSAGE_MAP()

public:
	typedef struct _tagSCClientParameter
	{
		CService* hostSvc;
		UINT sendTimeOutInterval;
		UINT receiveTimeOutInterval;
		UINT enquireLinkInterval;
		UINT nackResendMaxTimes;
		UINT timeoutResendMaxTimes;
		_tagSCClientParameter()
		{
			hostSvc = NULL;
			sendTimeOutInterval = 30;
			receiveTimeOutInterval = 30;
			enquireLinkInterval = 30;
			nackResendMaxTimes = 3;
			timeoutResendMaxTimes = 3;
		}
	} SCClientParameter;
	
	static CSCClient& GetInstance()
	{
		return instance;
	}

	virtual ~CSCClient();
	void Initialize(SCClientParameter&);
	void Shutdown();
	bool Connect(CString ipAddress,UINT port);
	bool Close();
	void Send(CSCCommand*);
	bool IsClosed();

protected:
	virtual BOOL OnIdle(LONG lCount);

private:
	static CSCClient instance;
	CSCSocket* m_Socket;
	CService*	m_HostSvc;
	SCClientParameter m_Parameter;
	CSCClientHelper* m_Helper;
	CSCCommand* m_LocalCommand;
	CUtilList  m_PendingCommand;
	UINT nackResendTimes;
	UINT timeoutResendTimes;
	volatile bool m_PauseSend;
	bool m_IsClosed;

	CSCClient();
	bool ConnectImp(CString ipAddress,UINT port);
	bool CloseImp();
	void SendImp(CSCCommand*);
	void SendNACK(NETWORK_TRANS_ID responseForNTID,SC_NACK_CODE nackCode);
	void SendACK(NETWORK_TRANS_ID responseForNTID);
	void OnRemoteCommandArravial(CSCCommand* command);
	CSCCommand* CreateCommand(VARIABLE_DATA& commandMsg);
	void Resend(bool becauseNACK);
	void OnSocketStartReceiveOneMessage(int nErrorCode);
	void OnSocketEndReceiveOneMessage(bool,VARIABLE_DATA&);
	void OnSocketStartSendOneMessage(const unsigned char*, int);
	void OnSocketEndSendOneMessage(const unsigned char*, int);
	void OnSocketClosed(int);

	void OnReceiveTimeout();
	void OnSendTimeout();
	void OnEnquirelink();
	
	void OnExecCommand(WPARAM wParam,LPARAM lParam);
	void OnPostCommand(WPARAM wParam,LPARAM lParam);
	void OnSCDataMsgCompleting(CCommand*);
	void OnSCCommandCompleted(CCommand*);

	friend class CSCClientHelper;

};