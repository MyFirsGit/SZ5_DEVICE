#pragma once
#include <afxinet.h>
#include "SCSvcDef.h"

using namespace std;

#define  theWEBSERVER_HELPER CWebServerHelper::GetInstance()
/**
@brief  webserver���ؽ��
*/
typedef enum _txn_code {
	TXN_CODE_OK						= 0,		// ����ɹ�
	TXN_CODE_PARAM_ERR				= 1,		// �����������
	TXN_CODE_TIMESPAN_ERR			= 2,		// ʱ����쳣
	TXN_CODE_ENCODE_ERR				= 3,		// �����㷨δ֪
	TXN_CODE_REPEAT					= 5,		// �������ط�
	TXN_CODE_EQUIPID_ERR			= 6,		// �豸��Ŵ���
	TXN_CODE_PASSWORD_ERR			= 7,		// �豸���������豸��ע��
	TXN_CODE_DATABASE_ERR			= 8,		// ���ݿ��������
	TXN_CODE_TOKEN_ERR				= 9,		// �豸��Կ��Ч�ڳ�ʱ
	TXN_CODE_LOCK_ERR				= 10,		// ��������״̬����Ԥ��Ʊ�ӿڷ��ش�����10
}TXN_CODE;

typedef enum _enum_web_operator_type{
	WEB_ORDER_REQUEST				= 1,	// �������ɶ���
	WEB_ORDER_QUERY					= 2,	// ��ѯ����
	WEB_ORDER_CANCEL				= 3,	// ȡ������
	WEB_ORDER_CONFIRM				= 4,	// ȷ�϶���
}WEB_OPERATOR_TYPE;

typedef struct _ticketInfo{
	CString ticketID;
	CString strWriteTicketTime;
	CString strIssueTime;	
	_ticketInfo(){
		ticketID = _T("");
		strWriteTicketTime = _T("");
		strIssueTime = _T("");
	}
}TicketInfo;

/*
* �̲߳���
*/
typedef struct {
	CHttpFile* pHttpFile;
	CHAR* pSendData;
	CString strHeader;
}PARM;

/**
@brief  ��������
*/

class CWebServerHelper 
{

public:
	static CWebServerHelper&  GetInstance(){
		return theInstance;
	}
	CWebServerHelper();                                                 // ���캯��
	~CWebServerHelper();                                                // ��������

public:
	RETURN_CODE Web_ConnectCheck();										// �����������Լ��
	RETURN_CODE Weixin_RequesetOrder(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Weixin_QueryOrder();
	RETURN_CODE Weixin_CancelOrder();
	RETURN_CODE Zhifubao_RequesetOrder(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Zhifubao_QueryOrder();
	RETURN_CODE Zhifubao_CancelOrder();
	RETURN_CODE Web_ConfirmOrder(int requestTicketNum, int actualTicketNum, TicketInfo* actrualTicketInfo);
	RETURN_CODE Web_IdleProcessOrder();

	void SetConnectFlag(bool bConnected);		// ����WEB��������ͨ��ʶ
	bool GetConnectFlag();						// ��ȡWEB��������ͨ��ʶ
	void EnableIdleProcess(bool bEnabled);		// �����Ƿ������ش������δ������
	bool IsEnableIdleProcess();					// ��ȡ�Ƿ������ش������δ������
	bool IsExistLocalOrder();					// �Ƿ���ڱ����δ������ 
	void ClearLocalOrder();						// �������δ������ 
	void SaveLocalOrder();						// ����δ������ 

private:
	// ����ʵʱ���Ͷ�������
	RETURN_CODE Weixin_Requeset(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Weixin_Query();
	RETURN_CODE Weixin_Cancel();
	RETURN_CODE Zhifubao_Requeset(int beginStation, int endStation, int tickePrice, int ticketCount, int discountAmount);
	RETURN_CODE Zhifubao_Query();
	RETURN_CODE Zhifubao_Cancel();
	RETURN_CODE Web_Confirm(int requestTicketNum, int actualTicketNum, TicketInfo* actrualTicketInfo);
	// ������ʱ���ͱ��ر���Ķ�����������ȷ������
	RETURN_CODE Web_IdleConfirm(CString strConfirmURL);
	RETURN_CODE Web_IdleCancel(CString strCancelURL);

private:
	// ������������Ϣ
	CString host;
	int port;
	CString strHeaders;
	CString baseURL;
	CString connectURL;
	// ����URL��Ϣ
	CString weixin_requestOrderURL;
	CString weixin_queryOrderURL;
	CString weixin_cancelOrderURL;
	CString zhifubao_requestOrderURL;
	CString zhifubao_queryOrderURL;
	CString zhifubao_cancelOrderURL;
	CString confirmOrderURL;
	// ������Ϣ
	int m_nOrderRetryTimes;
	CString m_strOrderID;

	// ���ر��涩����Ϣ
	bool m_bCurConnect;						//��ǰWEB�������Ƿ����
	bool m_bLoacalProcessEnable;			//�Ƿ������ش�����
	int m_nOrderProcessType;				//���ر������ʱ����Ķ�����������
	CString m_strOrderProcessURL;			//���ر������ʱ����Ķ�������URL

	void UTF8toANSI(CString &strUTF8);
	RETURN_CODE ServerPost(CString senddata, CString url,CString& getdata,int nTimeOut = 10000 /* ��λ��ms */);
	long StartWebThread();
	DWORD WINAPI WebCommunicateFunction(LPVOID lpParameter);
	static  CWebServerHelper theInstance;   // �����Ψһ�ӿ�

public:
	// �߳����
	HANDLE m_hPauseThread;
	HANDLE m_hWaitResult;
	HANDLE m_hExitThread;
	PARM m_Parm;
	RETURN_CODE m_EmThreadReturn;
	BOOL	m_bExitThread;
};