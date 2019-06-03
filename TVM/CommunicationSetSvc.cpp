#include "stdafx.h"
#include "CommunicationSetSvc.h"
#include "CommunicationSetMaintainDlg.h"
#include "GuideInfo.h"
#include <WinSock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCommunicationSetSvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::CCommunicationSetSvc() : CTVMForeService(COMMUNICATION_SET_SVC)
{
	//添加对话框界面
	m_pDialogFlow->AddDialog(IDD_20114_COMMUNICATION_SET_DLG,new CCommunicationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COMMUNICATION_SET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COMMUNICATION_SET, IDD_20114_COMMUNICATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);	// 设置初始画面组
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::~CCommunicationSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN | KEY_DOT);	// 启用指定按键

	EnterTime = 1;
	GuideMsg = _T("");

	ReadIniText();
	try{
		GetAdapterInfo();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);	// 设置初始画面组
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);
	// 如果在线则提示不允许设置
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief     读取Maintenance.INI配置文件

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::ReadIniText()
{
	m_DataModel.myIpAddress = theMAINTENANCE_INFO.GetIP();					//本地地址
	m_DataModel.mySubnetAddress = theMAINTENANCE_INFO.GetSubnetMask();		//子网掩码
	m_DataModel.myGatewayAddress = theMAINTENANCE_INFO.GetGateway();		//网关
	m_DataModel.scHostAddress = theMAINTENANCE_INFO.GetCpsHostID();			//主机地址
	int index = theMAINTENANCE_INFO.GetCpsPort();							//主机端口
	m_DataModel.scHostPort.Format(_T("%d"),index);
	m_DataModel.scNtpAddress = theMAINTENANCE_INFO.GetNtpHostID();			//NTP地址
	index = theMAINTENANCE_INFO.GetNtpPort();								//NTP端口
	m_DataModel.scNtpPort.Format(_T("%d"),index);
	m_DataModel.scFtpAddress = theMAINTENANCE_INFO.GetFtpHostID();			//FTP地址
	index = theMAINTENANCE_INFO.GetFtpPort();								//FTP端口
	m_DataModel.scFtpPort.Format(_T("%d"),index);

	ipsetin.IpIndex = m_DataModel.myIpAddress;
	ipsetin.SubnetIndex = m_DataModel.mySubnetAddress;
	ipsetin.GatewayIndex = m_DataModel.myGatewayAddress;
	ipsetin.HostIndex = m_DataModel.scHostAddress;
	ipsetin.HostPortIndex = m_DataModel.scHostPort;
	ipsetin.NtpIndex = m_DataModel.scNtpAddress;
	ipsetin.NtpPortIndex = m_DataModel.scNtpPort;
	ipsetin.FtpIndex = m_DataModel.scFtpAddress;
	ipsetin.FtpPortIndex = m_DataModel.scFtpPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     IPStrPut

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::Model& CCommunicationSetSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardEnter(0,0);
	if(1 == EnterTime)
	{
		CheckMenuNumber();
	}
	else if(2 == EnterTime)
	{
		DoCheckInput();							// 对输入数据是否合理做出相应项的实际判断
		theAPP_SESSION.SetMaxInputLength(1);	// 设置最大输入位数
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [.] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardDot(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(2 == EnterTime)
	{
		theAPP_SESSION.AppendOneDotToGuide();
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      菜单号是否非法判断

@param      无
@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::CheckMenuNumber()
{
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
		if(m_DataModel.MenuNumber<1 || m_DataModel.MenuNumber>MAX_CO_MENU_NUM)////主菜单编号除1-9之外的为非法菜单号
		{
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_MENU_CODE_ILLEGAL));
			return;
		}
		else
		{
			if(9 == m_DataModel.MenuNumber || 8 == m_DataModel.MenuNumber || 7 == m_DataModel.MenuNumber)
			{
				theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COMMUNICATION_SET_FTPPORT_ERROR));
			}
			else
			{
				InputGuideMsg();
				theAPP_SESSION.DeleteAllInput();
				theAPP_SESSION.SetPrefixToGuide(GuideMsg);
				EnterTime = 2;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对GuideMsg分别赋值

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::InputGuideMsg()
{
	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:			// 本地地址
		GuideMsg = _opl(COMMUNICATION_TXT_MYIPADDRESS)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_2:			// 子网掩码
		GuideMsg = _opl(COMMUNICATION_TXT_MYSUBNETMASK)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_3:			// 网关
		GuideMsg = _opl(COMMUNICATION_TXT_MYGATEWAY)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_4:			// 主机地址
		GuideMsg = _opl(COMMUNICATION_TXT_CPSHOSTADDRESS)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_5:			// 主机端口
		GuideMsg = _opl(COMMUNICATION_TXT_CPSPORT)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(4);
		break;
	case MENU_CODE_6:			// NTP地址
		GuideMsg = _opl(COMMUNICATION_TXT_NTPHOSTADDRESS)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	//case MENU_CODE_7:			// NTP端口
	//	GuideMsg = _opl(COMMUNICATION_TXT_NTPPORT)+_opl(TXT_COLON);
	//	theAPP_SESSION.SetMaxInputLength(5);
	//	break;
	//case MENU_CODE_8:			// FTP地址
	//	GuideMsg = _opl(COMMUNICATION_TXT_FTPHOSTADDRESS)+_opl(TXT_COLON);
	//	theAPP_SESSION.SetMaxInputLength(15);
	//	break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     对输入数据是否合理做出相应项的实际判断

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::DoCheckInput()
{
	int index= -1;
	int IsEqual = -1;
	unsigned long addrreturn = 0;
	CString ip = theAPP_SESSION.GetInputToString();
	if(_T("") == ip)
	{
		return;
	}
	const wchar_t* wip = (LPCTSTR)ip;
	char buf[200] = {0};
	wcstombs(buf,wip,wcslen(wip));
	const char* sip = buf;

	struct sockaddr_in addrIP;
	memset(&addrIP,0,sizeof(sockaddr_in));

	addrreturn = inet_addr(sip);
	addrIP.sin_addr.s_addr = addrreturn;
	IsEqual = strcmp(inet_ntoa(addrIP.sin_addr),sip);

	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:			// 本地地址
		 if(INADDR_NONE == addrreturn || 0!= IsEqual)//IP非法
		{
			m_DataModel.IpLegal = FALSE;
		}
		
		else
		{
			m_DataModel.myIpAddress = ip;
			ipsetin.IpIndex = ip;
			m_DataModel.IpLegal = TRUE;
		}
		break;
	case MENU_CODE_2:			// 子网掩码
		if(INADDR_NONE == addrreturn || 0!= IsEqual)
		{
			m_DataModel.IpLegal = FALSE;
		}
		else
		{
			m_DataModel.mySubnetAddress = ip;
			ipsetin.SubnetIndex = ip;
			m_DataModel.IpLegal = TRUE;
		}
		break;
	case MENU_CODE_3:			// 网关
		if(INADDR_NONE == addrreturn || 0!= IsEqual)
		{
			m_DataModel.IpLegal = FALSE;
		}
		else
		{
			m_DataModel.myGatewayAddress = ip;
			ipsetin.GatewayIndex = ip;
			m_DataModel.IpLegal = TRUE;
		}
		break;
	case MENU_CODE_4:			// 主机地址
		if(INADDR_NONE == addrreturn || 0!= IsEqual)
		{
			m_DataModel.IpLegal = FALSE;
		}
		else
		{
			m_DataModel.scHostAddress = ip;
			ipsetin.HostIndex = ip;
			m_DataModel.IpLegal = TRUE;
		}
		break;
	case MENU_CODE_5:			// 主机端口
		index = theAPP_SESSION.GetInputToInt();
		if(index<0 || index>9999)
		{
			m_DataModel.IpLegal = FALSE;
		}
		else
		{
			m_DataModel.scHostPort.Format(_T("%d"),index);
			ipsetin.HostPortIndex = m_DataModel.scHostPort;
			m_DataModel.IpLegal = TRUE;
		}
		break;
	case MENU_CODE_6:			// NTP地址
		if(INADDR_NONE == addrreturn || 0!= IsEqual)
		{
			m_DataModel.IpLegal = FALSE;
		}
		else
		{
			m_DataModel.scNtpAddress = ip;
			ipsetin.NtpIndex = ip;
			m_DataModel.IpLegal = TRUE;
		}
		break;
	//case MENU_CODE_7:			// NTP端口
	//	index = theAPP_SESSION.GetInputToInt();
	//	if(index<0 || index>9999)
	//	{
	//		m_DataModel.IpLegal = FALSE;
	//	}
	//	else
	//	{
	//		m_DataModel.scNtpPort.Format(_T("%d"),index);
	//		ipsetin.NtpPortIndex = m_DataModel.scNtpPort;
	//		m_DataModel.IpLegal = TRUE;
	//	}
	//	break;
	//case MENU_CODE_8:			// FTP地址
	//	if(INADDR_NONE == addrreturn || 0!= IsEqual)
	//	{
	//		m_DataModel.IpLegal = FALSE;
	//	}
	//	else
	//	{
	//		m_DataModel.scFtpAddress = ip;
	//		ipsetin.FtpIndex = ip;
	//		m_DataModel.IpLegal = TRUE;
	//	}
	//	break;
	default:
		break;
	}
	m_DataModel.IpChange = TRUE;	// 代码发生变化标志
	NotifyDataModelChanged();
	EnterTime = 1;					// 置按键次数为1，重新从选择菜单编号开始
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     通知数据模型更新

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F2/Alt] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 如果在线则提示不允许设置
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardF2(0,0);
		// 设置本机IP地址
		CString strLinkName =_T("");
		for(int i = 0; i < AdapterInfoVector.size();i++){
			if(AdapterInfoVector[i].strConnName ==  theMAINTENANCE_INFO.GetLinkName()){
				strLinkName = AdapterInfoVector[i].strName;
				break;
			}
		}
		if(strLinkName.IsEmpty()){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COMMUNICATION_SET_IP_FAIL));
			return FALSE;
		}
		if(!SetIP(strLinkName,0,ipsetin.IpIndex,ipsetin.SubnetIndex,ipsetin.GatewayIndex)){
			theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_COMMUNICATION_SET_IP_FAIL));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(_opl(STATIONSET_DATA_SAVE));
		}
		theMAINTENANCE_INFO.SetIP(ipsetin.IpIndex);					// 保存本地地址
		theMAINTENANCE_INFO.SetSubnetMask(ipsetin.SubnetIndex);		// 保存子网掩码
		theMAINTENANCE_INFO.SetGateway(ipsetin.GatewayIndex);		// 保存网关
		theMAINTENANCE_INFO.SetCpsHostID(ipsetin.HostIndex);		// 保存主机地址
		theMAINTENANCE_INFO.SetCpsPort(ipsetin.HostPortIndex);		// 保存主机端口
		theMAINTENANCE_INFO.SetNtpHostID(ipsetin.NtpIndex);			// 保存NTP地址
		theMAINTENANCE_INFO.SetNtpPort(ipsetin.NtpPortIndex);		// 保存NTP端口
		theMAINTENANCE_INFO.SetFtpHostID(ipsetin.FtpIndex);			// 保存FTP地址
		// 本机设置日志
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_NETWORK_SETTINGS,true);
		return TRUE;	
	}			
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Clear] 键按下消息（删除）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;							// 置按键次数为1，重新从选择菜单编号开始
		theAPP_SESSION.SetMaxInputLength(1);	// 设置最大输入位数
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到本机适配器信息

@param      void

@retval     BOOL    \n
            TRUE:成功    FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::GetAdapterInfo()
{
    try
    {
	    // 这里的代码适合WINDOWS2000，对于NT需要读取HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards
	    HKEY hKey, hSubKey, hNdiIntKey,hConnKey;

	    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		    _T("System\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}"),
		    0,
		    KEY_READ,
		    &hKey) != ERROR_SUCCESS)
		    return FALSE;

	    DWORD dwIndex = 0;
	    DWORD dwBufSize = 256;
	    DWORD dwDataType;
	    char szSubKey[256];
	    unsigned char szData[256];

        AdapterInfoVector.clear();

	    while(RegEnumKeyEx(hKey, dwIndex++, (LPWSTR)szSubKey, &dwBufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	    {
			dwDataType = REG_DWORD;
		    if(RegOpenKeyEx(hKey, (LPWSTR)szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		    {		
			    if(RegOpenKeyEx(hSubKey, _T("Ndi\\Interfaces"), 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)
			    {
				    dwBufSize = 256;
				    if(RegQueryValueEx(hNdiIntKey, _T("LowerRange"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
				    {
					    if(strcmp((char*)szData, (char*)(_T("ethernet"))) == 0)		//	判断是不是以太网卡
					    {
						    dwBufSize = 256;
						    if(RegQueryValueEx(hSubKey, _T("DriverDesc"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
						    {
							    ADAPTER_INFO adptInfo;
							    adptInfo.strDriverDesc = (LPCTSTR)szData;
							    dwBufSize = 256;
							    if(RegQueryValueEx(hSubKey, _T("NetCfgInstanceID"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
							    {
								    adptInfo.strName = (LPCTSTR)szData;
								    RegGetIP(adptInfo, (LPCTSTR)szData);
									// 取连接时名称的值
									if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
										_T("System\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}"),
										0,
										KEY_READ,
										&hConnKey) == ERROR_SUCCESS)
									{
										CString keyName = adptInfo.strName + _T("\\Connection");
										HKEY hConnSubKey;
										if(RegOpenKeyEx(hConnKey,keyName,0,KEY_READ,&hConnSubKey) == ERROR_SUCCESS){
											dwDataType = REG_SZ;
											if(RegQueryValueEx(hConnSubKey,_T("Name"),0,&dwDataType,szData,&dwBufSize) == ERROR_SUCCESS){
												adptInfo.strConnName = (LPCTSTR)szData;
											}
											RegCloseKey(hConnSubKey);
										}
										RegCloseKey(hConnKey);
									}

							    }
							    AdapterInfoVector.push_back(adptInfo);	// 加入到容器中
						    }
					    }
				    }
				    RegCloseKey(hNdiIntKey);
			    }
			    RegCloseKey(hSubKey);
		    }
		    dwBufSize = 256;
	    }	/* end of while */
	    RegCloseKey(hKey);
    }
    catch (CSysException) {
        throw;
    }
    catch (...) {
        throw ;//CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__);
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      从系统注册表中得到本机默认适配器上的IP地址,子网掩码,网关信息

@param      (i)  ADAPTER_INFO*    pAI                 适配器信息容器指针
@param      (i)  LPCTSTR          lpszAdapterName     适配器名称
@param		(i)  int              nIndex                       

@retval     BOOL    \n
            TRUE:成功    FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::RegGetIP(ADAPTER_INFO &adptInfo, LPCTSTR lpszAdapterName, int nIndex/* =0 */)
{
    try
    {
	    HKEY hKey;
	    CString strKeyName = _T("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\");
	    strKeyName += lpszAdapterName;
        //打开注册表
	    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKeyName,0,KEY_READ,&hKey) != ERROR_SUCCESS){
		    return FALSE;
	    }
	    unsigned char szData[256];
	    DWORD dwDataType, dwBufSize;
	    dwBufSize = 256;
	    if(RegQueryValueEx(hKey, _T("IPAddress"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS){
		    adptInfo.strIP = (LPCTSTR)szData;
	    }

	    dwBufSize = 256;
	    if(RegQueryValueEx(hKey, _T("SubnetMask"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS){
		    adptInfo.strNetMask = (LPCTSTR)szData;
	    }

	    dwBufSize = 256;
	    if(RegQueryValueEx(hKey, _T("DefaultGateway"), 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS){
		    adptInfo.strNetGate = (LPCTSTR)szData;
	    }
	    RegCloseKey(hKey);
    }
    catch (CSysException) {
        throw;
    }
    catch (...) {
        throw ;//CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__);
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      修改系统注册表中本机默认适配器上的IP地址,子网掩码,网关信息

@param      (i)  LPCTSTR          lpszAdapterName     适配器名称
@param      (i)  int              nIndex              
@param      (i)  LPCTSTR          pIPAddress          IP地址
@param		(i)  LPCTSTR          pNetMask            子网掩码
@param		(i)  LPCTSTR          pNetGate            网关


@retval     BOOL    \n
            TRUE:成功    FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::RegSetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate)
{
    try
    {
	    HKEY hKey;

	    CString strKeyName = _T("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\");

	    strKeyName += lpszAdapterName;

	    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKeyName,0,KEY_WRITE,&hKey) != ERROR_SUCCESS){
		    return FALSE;
	    }

        //写注册表
	    RegSetValueEx(hKey, _T("IPAddress"), 0, REG_MULTI_SZ, (CONST BYTE*)pIPAddress, (_tcslen(pIPAddress)+1)*sizeof(TCHAR));
	    RegSetValueEx(hKey, _T("SubnetMask"), 0, REG_MULTI_SZ, (CONST BYTE*)pNetMask, (_tcslen(pNetMask)+1)*sizeof(TCHAR));
	    RegSetValueEx(hKey, _T("DefaultGateway"), 0, REG_MULTI_SZ, (CONST BYTE*)pNetGate, (_tcslen(pNetGate)+1)*sizeof(TCHAR));

	    RegCloseKey(hKey);
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        //GetService()->ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__));
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      修改本机默认适配器上的IP地址,子网掩码,网关信息

@param      (i)  LPCTSTR          lpszAdapterName     适配器名称
@param      (i)  int              nIndex              
@param      (i)  LPCTSTR          pIPAddress          IP地址
@param      (i)  LPCTSTR          pNetMask            子网掩码

@retval     BOOL    \n
            TRUE:成功    FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::SetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate)
{
    try
    {
       	//2017-11-9 v16 此修改IP方法不能在拔网线时操作
		/*//LOG("SetIP----0");
        if(!RegSetIP(lpszAdapterName, nIndex, pIPAddress, pNetMask, pNetGate)){
			LOG("SetIP----1");
		    return FALSE;
        }

	    if(!NotifyIPChange(lpszAdapterName, nIndex, pIPAddress, pNetMask)){
			LOG("SetIP----2");
	    	return FALSE;
		}*/
		// 设置本机IP地址
		CString strLinkName = theMAINTENANCE_INFO.GetLinkName();
		CString strCommand = _T("netsh interface ip set address \"") + strLinkName + _T("\" static ") + pIPAddress + _T(" ") + pNetMask + _T(" ") + pNetGate + _T(" auto");
		// 启动DOS命令
		USES_CONVERSION;
		UINT iResult = WinExec(T2A(strCommand),SW_HIDE);
		// 执行成功
		if (iResult > 31){
		}
		else{
			return FALSE;
		}
    }
    catch (CSysException) {
        throw;
    }
    catch (...) {
        throw ;//CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__);
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      通知本机默认适配器上的IP地址,子网掩码,网关信息已经改变

@param      (i)  LPCTSTR          lpszAdapterName     适配器名称
@param      (i)  int              nIndex              
@param      (i)  LPCTSTR          pIPAddress          IP地址
@param      (i)  LPCTSTR          pNetMask            子网掩码

@retval     BOOL    \n
            TRUE:成功    FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::NotifyIPChange(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask)
{
    BOOL bResult = FALSE;
    try
    {
	    HINSTANCE		hDhcpDll;
	    DHCPNOTIFYPROC	pDhcpNotifyProc;
	   // WCHAR wcAdapterName[256];
		//memset(wcAdapterName,0x00,256);

	   // MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszAdapterName, -1, wcAdapterName,256);

	    if((hDhcpDll = LoadLibrary(_T("dhcpcsvc"))) == NULL)
		    return FALSE;

	    if((pDhcpNotifyProc = (DHCPNOTIFYPROC)GetProcAddress(hDhcpDll, "DhcpNotifyConfigChange")) != NULL)
		    if((pDhcpNotifyProc)(NULL, (LPTSTR)lpszAdapterName, TRUE, nIndex, inet_addr((const char*)pIPAddress), inet_addr((const char*)pNetMask), 0) == ERROR_SUCCESS)
			    bResult = TRUE;
	
		FreeLibrary(hDhcpDll);
    }
    catch (CSysException) {
        throw;
    }
    catch (...) {
        //throw CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__);
    }
	return bResult;
}