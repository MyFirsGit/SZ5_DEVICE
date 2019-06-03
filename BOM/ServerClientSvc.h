#pragma once
#include "SCSvcDef.h"
#include "BackService.h"
#include "SCCommand.h"
#include "control.h"
#include "scdatamsg.h"
#include "servicethread.h"
#include "SCSequence.h"



class CServerClientSvc:public CBackService
{
	DECLARE_MESSAGE_MAP()
public:
	typedef enum _Timers
	{
		RECONNECT_TIMER = 0x01,
	}TIMERS;
	CServerClientSvc();
	virtual ~CServerClientSvc();

	void OnStart();
	bool OnStop();
	
	virtual BOOL PostMessage(UINT message,WPARAM wParam,LPARAM lParam);
	virtual LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam );

	long RequestLoginLogoff(int nOperatorID, CString sPwd, LOGIN_TYPE LoginType,BYTE& result);// 6.7.2.1 ��¼�ǳ�����
	long RequestChangeStaffPwd(CHANGE_STAFFPASSWORD& staffInfo,BYTE& result);// 6.7.2.3 �����޸�����
	long SendParameterVersion();
	long SendBomFullStatus(BOM_STATUS_ID,BYTE);
	WORD GetCommandMsgCode(CSCCommand*);
private:
	LRESULT OnSendMsgToSC(WPARAM wParam,LPARAM lParam);
	LRESULT OnConnect(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseOperation(WPARAM wParam,LPARAM lParam);
	LRESULT OnScDataMsgArrival(WPARAM wParam,LPARAM lParam);
	LRESULT OnSCDataMsg(WPARAM wParam,LPARAM lParam);
	LRESULT OnReconnect(WPARAM wParam,LPARAM lParam);
	LRESULT OnRequestDownloadImmidiateParameter(WPARAM wParam,LPARAM lParam);
	LRESULT SendBomFullStatus(WPARAM wParam,LPARAM lParam);
	LRESULT SendBomError(WPARAM wParam,LPARAM lParam);

	void OnTimer(UINT nEventID);
	long ConnectAuthenticate();

	bool ForceForegroundLogout(FORCELOGOUTREASON reason,bool waitLogout);
	void HookStarttingForeService(SERVICE_ID,bool*);

	long OnUploadSpecifyBlockNOData(CSCCommand* scDataMsg);
	long OnUploadSpecifyTimeData(CSCCommand* scDataMsg);
	long OnUploadDebugData(CSCCommand* scDataMsg);
	long OnOperationModeControl(CSCCommand* scDataMsg);
	long OnDeviceControl(CSCCommand* scDataMsg);
	long On24HourOperation(CSCCommand* scDataMsg);
	long OnDelayOperation(CSCCommand* scDataMsg);
	long OnForceTimeSync(CSCCommand* scDataMsg);
	long OnIssueRestrict(CSCCommand* scDataMsg);
	long OnUploadParameterVersion(CSCCommand* scDataMsg);
	long OnUpdateParameter(CSCCommand* scDataMsg);
	long OnForceLogout(CSCCommand* scDataMsg);
	long OnRequestDeviceStatus(CSCCommand* scDataMsg);
	long OnAutorunParameter(CSCCommand* scDataMsg);
	
	long OnAccountUnlockNotice(CSCCommand* scDataMsg);
	long OnFtpConfigNotice(CSCCommand* scDataMsg);

	long OnOpEndNotice(CSCCommand* scDataMsg);
	long OnNonImmidatelySurrenderQueryResultNotice(CSCCommand* scDataMsg);
	long OnSignCardApplyQueryResultNotice(CSCCommand* scDataMsg);
	
	long SendOpEndNotice(NOTICE_TYPE type);


	//�������ʱ
	void OnSequenceComplete(CCommand*);
	long OnSCSendCommand(CCommand*);
	void OnSCSendCommandComplete(CCommand*);

	//bool UploadLogFile(_DATE dateOfLog);
	//�߼����ӽ����ɹ��Ժ���еĲ�����
	long PostSCConnected();
	//�������ӶϿ��Ժ���еĺ���������
	long PostSCDisConnected();


	CServiceThread* m_ReconnectThread;
	list<CServiceThread*> m_SCProcessThreads;
	long m_SCClientStatus;
	CSCSequence* m_Sequence;
	
	HANDLE m_hThreadMonitorOnline;
	BOOL   m_RequestExitMonitorThread;
	HANDLE m_WaitForeServiceStarting;
	static DWORD WINAPI MonitorOnline(LPVOID lpVoid);
	static void APIENTRY  ExitMonitorThread(ULONG_PTR dwParam);
};