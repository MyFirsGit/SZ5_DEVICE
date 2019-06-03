#pragma once
#include "TVMForeService.h"
#define MAX_CO_MENU_NUM 9		//网络设置界面菜单的最大编号

/**
@brief    通信参数设定
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
		int MenuNumber;					// 菜单号
		BOOL MenuChange;				// 菜单变化标志(菜单号由1变到2，这种变化置MenuChange为TRUE)
		BOOL IpChange;					// IP变化标志(IP的发生了改变，置IpChange为TRUE)
		BOOL IpLegal;					// 输入IP是否非法标志
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

	typedef struct _IpWrite			// 保存设置
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

	//适配器信息
	typedef struct tagAdapterInfo
	{
		CString strConnName;		// 连接名称 如：本地连接
		CString strName;			// 适配器物理名称
		CString strDriverDesc;	    // 适配器描述
		CString strIP;			    // IP地址
		CString strNetMask;		    // 子网掩码
		CString strNetGate;		    // 网关
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
	void ReadIniText();															// 读取Maintenance.INI配置文件
	void CheckMenuNumber();														// 菜单号是否非法判断
	void InputGuideMsg();														// 对GuideMsg分别赋值
	void DoCheckInput();														// 对输入数据是否合理做出相应项的实际判断
	
	virtual void	NotifyDataModelChanged();
protected:
	void OnStart();																// Service启动后处理
private:
	int EnterTime;
	CString GuideMsg;															// 向导栏提示
	Model m_DataModel;
	IpSetIn ipsetin;

	typedef int (CALLBACK* DHCPNOTIFYPROC)(LPWSTR, LPWSTR, BOOL, DWORD, DWORD, DWORD, int);
	BOOL GetAdapterInfo();                                                                                      //得到本机适配器信息

	BOOL RegGetIP(ADAPTER_INFO &adptInfo, LPCTSTR lpszAdapterName, int nIndex = 0);                             //从系统注册表中得到本机默认适配器上的IP地址,子网掩码,网关信息

	BOOL RegSetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate); //修改系统注册表中本机默认适配器上的IP地址,子网掩码,网关信息

	BOOL NotifyIPChange(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask);             //通知本机默认适配器上的IP地址,子网掩码,网关信息已经改变

	BOOL SetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate);    //修改本机默认适配器上的IP地址,子网掩码,网关信息                                                                                 // SubMenu第4个Button位置区域显示的数据

	vector<ADAPTER_INFO> AdapterInfoVector; //适配器信息容器
	LRESULT OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Clear] 键按下消息（删除）
	LRESULT OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [.] 键按下消息（小数点）
	LRESULT OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/);				// 处理 [Enter] 键按下消息
	LRESULT OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F2/Alt] 键按下消息（保存）
	LRESULT OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/);					// 处理 [F3/Ctrl] 键按下消息（返回）
};