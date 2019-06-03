#pragma once
#include "TVMForeService.h"
#define MAX_CO_MENU_NUM 9		//�������ý���˵��������

/**
@brief    ͨ�Ų����趨
*/
class CCommunicationSetSvc : public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CCommunicationSetSvc();
	~CCommunicationSetSvc();

	typedef enum _dialog_group {
		DIALOG_GROUP_COMMUNICATION_SET = 1,                       
	} DIALOG_GROUP;

	typedef struct _tagModel
	{
		int MenuNumber;					// �˵���
		BOOL MenuChange;				// �˵��仯��־(�˵�����1�䵽2�����ֱ仯��MenuChangeΪTRUE)
		BOOL IpChange;					// IP�仯��־(IP�ķ����˸ı䣬��IpChangeΪTRUE)
		BOOL IpLegal;					// ����IP�Ƿ�Ƿ���־
		CString myIpAddress;
		CString mySubnetAddress;
		CString myGatewayAddress;
		CString scHostAddress;
		CString scHostPort;
		CString scNtpAddress;
		CString scNtpPort;
		CString scFtpAddress;
		CString scFtpPort;
		_tagModel()
		{
			MenuNumber = 0;
			MenuChange = FALSE;
			IpChange = FALSE;
			IpLegal = TRUE;
			myIpAddress = _T("");
			mySubnetAddress = _T("");
			myGatewayAddress = _T("");
			scHostAddress = _T("");
			scHostPort = _T("");
			scNtpAddress = _T("");
			scNtpPort = _T("");
			scFtpAddress = _T("");
			scFtpPort = _T("");
		}
	}Model;

	typedef struct _IpWrite			// ��������
	{
		CString IpIndex;
		CString SubnetIndex;
		CString GatewayIndex;
		CString HostIndex;
		CString HostPortIndex;
		CString NtpIndex;
		CString NtpPortIndex;
		CString FtpIndex;
		CString FtpPortIndex;
		_IpWrite()
		{
			IpIndex = _T("");
			SubnetIndex = _T("");
			GatewayIndex = _T("");
			HostIndex = _T("");
			HostPortIndex = _T("");
			NtpIndex = _T("");
			NtpPortIndex = _T("");
			FtpIndex = _T("");
			FtpPortIndex = _T("");
		}
	}IpSetIn;

	//��������Ϣ
	typedef struct tagAdapterInfo
	{
		CString strConnName;		// �������� �磺��������
		CString strName;			// ��������������
		CString strDriverDesc;	    // ����������
		CString strIP;			    // IP��ַ
		CString strNetMask;		    // ��������
		CString strNetGate;		    // ����
		tagAdapterInfo()
		{
			strConnName = _T("");
			strName = _T("");
			strDriverDesc = _T("");
			strIP = _T("");
			strNetMask = _T("");
			strNetGate = _T("");
		}
	}ADAPTER_INFO;

	Model& GetDataModel();
	void ReadIniText();															// ��ȡMaintenance.INI�����ļ�
	void CheckMenuNumber();														// �˵����Ƿ�Ƿ��ж�
	void InputGuideMsg();														// ��GuideMsg�ֱ�ֵ
	void DoCheckInput();														// �����������Ƿ����������Ӧ���ʵ���ж�
	
	virtual void	NotifyDataModelChanged();
protected:
	void OnStart();																// Service��������
private:
	int EnterTime;
	CString GuideMsg;															// ������ʾ
	Model m_DataModel;
	IpSetIn ipsetin;

	typedef int (CALLBACK* DHCPNOTIFYPROC)(LPWSTR, LPWSTR, BOOL, DWORD, DWORD, DWORD, int);
	BOOL GetAdapterInfo();                                                                                      //�õ�������������Ϣ

	BOOL RegGetIP(ADAPTER_INFO &adptInfo, LPCTSTR lpszAdapterName, int nIndex = 0);                             //��ϵͳע����еõ�����Ĭ���������ϵ�IP��ַ,��������,������Ϣ

	BOOL RegSetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate); //�޸�ϵͳע����б���Ĭ���������ϵ�IP��ַ,��������,������Ϣ

	BOOL NotifyIPChange(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask);             //֪ͨ����Ĭ���������ϵ�IP��ַ,��������,������Ϣ�Ѿ��ı�

	BOOL SetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate);    //�޸ı���Ĭ���������ϵ�IP��ַ,��������,������Ϣ                                                                                 // SubMenu��4��Buttonλ��������ʾ������

	vector<ADAPTER_INFO> AdapterInfoVector; //��������Ϣ����
	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Clear] ��������Ϣ��ɾ����
	LRESULT OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [.] ��������Ϣ��С���㣩
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// ���� [Enter] ��������Ϣ
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F2/Alt] ��������Ϣ�����棩
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// ���� [F3/Ctrl] ��������Ϣ�����أ�
};