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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::CCommunicationSetSvc() : CTVMForeService(COMMUNICATION_SET_SVC)
{
	//��ӶԻ������
	m_pDialogFlow->AddDialog(IDD_20114_COMMUNICATION_SET_DLG,new CCommunicationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COMMUNICATION_SET,_opl(GUIDE_ENTER_F2_F3_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COMMUNICATION_SET, IDD_20114_COMMUNICATION_SET_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);	// ���ó�ʼ������
	
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::~CCommunicationSetSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::OnStart()
{
	__super::OnStart();

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN | KEY_DOT);	// ����ָ������

	EnterTime = 1;
	GuideMsg = _T("");

	ReadIniText();
	try{
		GetAdapterInfo();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);	// ���ó�ʼ������
	m_pDialogFlow->ShowDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);
	// �����������ʾ����������
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ȡMaintenance.INI�����ļ�

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::ReadIniText()
{
	m_DataModel.myIpAddress = theMAINTENANCE_INFO.GetIP();					//���ص�ַ
	m_DataModel.mySubnetAddress = theMAINTENANCE_INFO.GetSubnetMask();		//��������
	m_DataModel.myGatewayAddress = theMAINTENANCE_INFO.GetGateway();		//����
	m_DataModel.scHostAddress = theMAINTENANCE_INFO.GetCpsHostID();			//������ַ
	int index = theMAINTENANCE_INFO.GetCpsPort();							//�����˿�
	m_DataModel.scHostPort.Format(_T("%d"),index);
	m_DataModel.scNtpAddress = theMAINTENANCE_INFO.GetNtpHostID();			//NTP��ַ
	index = theMAINTENANCE_INFO.GetNtpPort();								//NTP�˿�
	m_DataModel.scNtpPort.Format(_T("%d"),index);
	m_DataModel.scFtpAddress = theMAINTENANCE_INFO.GetFtpHostID();			//FTP��ַ
	index = theMAINTENANCE_INFO.GetFtpPort();								//FTP�˿�
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
@brief      ��ȡSvcģ��

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
@brief      ���� [Enter] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
		DoCheckInput();							// �����������Ƿ����������Ӧ���ʵ���ж�
		theAPP_SESSION.SetMaxInputLength(1);	// �����������λ��
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [.] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      �˵����Ƿ�Ƿ��ж�

@param      ��
@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::CheckMenuNumber()
{
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		m_DataModel.MenuNumber = theAPP_SESSION.GetInputToInt();
		if(m_DataModel.MenuNumber<1 || m_DataModel.MenuNumber>MAX_CO_MENU_NUM)////���˵���ų�1-9֮���Ϊ�Ƿ��˵���
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
@brief     ��GuideMsg�ֱ�ֵ

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::InputGuideMsg()
{
	switch(m_DataModel.MenuNumber)
	{
	case MENU_CODE_1:			// ���ص�ַ
		GuideMsg = _opl(COMMUNICATION_TXT_MYIPADDRESS)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_2:			// ��������
		GuideMsg = _opl(COMMUNICATION_TXT_MYSUBNETMASK)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_3:			// ����
		GuideMsg = _opl(COMMUNICATION_TXT_MYGATEWAY)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_4:			// ������ַ
		GuideMsg = _opl(COMMUNICATION_TXT_CPSHOSTADDRESS)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	case MENU_CODE_5:			// �����˿�
		GuideMsg = _opl(COMMUNICATION_TXT_CPSPORT)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(4);
		break;
	case MENU_CODE_6:			// NTP��ַ
		GuideMsg = _opl(COMMUNICATION_TXT_NTPHOSTADDRESS)+_opl(TXT_COLON);
		theAPP_SESSION.SetMaxInputLength(15);
		break;
	//case MENU_CODE_7:			// NTP�˿�
	//	GuideMsg = _opl(COMMUNICATION_TXT_NTPPORT)+_opl(TXT_COLON);
	//	theAPP_SESSION.SetMaxInputLength(5);
	//	break;
	//case MENU_CODE_8:			// FTP��ַ
	//	GuideMsg = _opl(COMMUNICATION_TXT_FTPHOSTADDRESS)+_opl(TXT_COLON);
	//	theAPP_SESSION.SetMaxInputLength(15);
	//	break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     �����������Ƿ����������Ӧ���ʵ���ж�

@param      void

@retval     none

@exception  ��
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
	case MENU_CODE_1:			// ���ص�ַ
		 if(INADDR_NONE == addrreturn || 0!= IsEqual)//IP�Ƿ�
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
	case MENU_CODE_2:			// ��������
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
	case MENU_CODE_3:			// ����
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
	case MENU_CODE_4:			// ������ַ
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
	case MENU_CODE_5:			// �����˿�
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
	case MENU_CODE_6:			// NTP��ַ
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
	//case MENU_CODE_7:			// NTP�˿�
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
	//case MENU_CODE_8:			// FTP��ַ
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
	m_DataModel.IpChange = TRUE;	// ���뷢���仯��־
	NotifyDataModelChanged();
	EnterTime = 1;					// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ֪ͨ����ģ�͸���

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F2/Alt] ��������Ϣ

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardF2(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// �����������ʾ����������
	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
		return FALSE;
	}
	else{
		__super::OnKeyboardF2(0,0);
		// ���ñ���IP��ַ
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
		theMAINTENANCE_INFO.SetIP(ipsetin.IpIndex);					// ���汾�ص�ַ
		theMAINTENANCE_INFO.SetSubnetMask(ipsetin.SubnetIndex);		// ������������
		theMAINTENANCE_INFO.SetGateway(ipsetin.GatewayIndex);		// ��������
		theMAINTENANCE_INFO.SetCpsHostID(ipsetin.HostIndex);		// ����������ַ
		theMAINTENANCE_INFO.SetCpsPort(ipsetin.HostPortIndex);		// ���������˿�
		theMAINTENANCE_INFO.SetNtpHostID(ipsetin.NtpIndex);			// ����NTP��ַ
		theMAINTENANCE_INFO.SetNtpPort(ipsetin.NtpPortIndex);		// ����NTP�˿�
		theMAINTENANCE_INFO.SetFtpHostID(ipsetin.FtpIndex);			// ����FTP��ַ
		// ����������־
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgOperationLog(OPERATION_NETWORK_SETTINGS,true);
		return TRUE;	
	}			
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [Clear] ��������Ϣ��ɾ����

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CCommunicationSetSvc::OnKeyboardClear(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardClear(0,0);
	if(true == theAPP_SESSION.IsEmptyPrefix())
	{
		EnterTime = 1;							// �ð�������Ϊ1�����´�ѡ��˵���ſ�ʼ
		theAPP_SESSION.SetMaxInputLength(1);	// �����������λ��
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
@brief      �õ�������������Ϣ

@param      void

@retval     BOOL    \n
            TRUE:�ɹ�    FALSE:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::GetAdapterInfo()
{
    try
    {
	    // ����Ĵ����ʺ�WINDOWS2000������NT��Ҫ��ȡHKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards
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
					    if(strcmp((char*)szData, (char*)(_T("ethernet"))) == 0)		//	�ж��ǲ�����̫����
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
									// ȡ����ʱ���Ƶ�ֵ
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
							    AdapterInfoVector.push_back(adptInfo);	// ���뵽������
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
@brief      ��ϵͳע����еõ�����Ĭ���������ϵ�IP��ַ,��������,������Ϣ

@param      (i)  ADAPTER_INFO*    pAI                 ��������Ϣ����ָ��
@param      (i)  LPCTSTR          lpszAdapterName     ����������
@param		(i)  int              nIndex                       

@retval     BOOL    \n
            TRUE:�ɹ�    FALSE:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::RegGetIP(ADAPTER_INFO &adptInfo, LPCTSTR lpszAdapterName, int nIndex/* =0 */)
{
    try
    {
	    HKEY hKey;
	    CString strKeyName = _T("SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\");
	    strKeyName += lpszAdapterName;
        //��ע���
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
@brief      �޸�ϵͳע����б���Ĭ���������ϵ�IP��ַ,��������,������Ϣ

@param      (i)  LPCTSTR          lpszAdapterName     ����������
@param      (i)  int              nIndex              
@param      (i)  LPCTSTR          pIPAddress          IP��ַ
@param		(i)  LPCTSTR          pNetMask            ��������
@param		(i)  LPCTSTR          pNetGate            ����


@retval     BOOL    \n
            TRUE:�ɹ�    FALSE:ʧ��

@exception  ��
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

        //дע���
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
@brief      �޸ı���Ĭ���������ϵ�IP��ַ,��������,������Ϣ

@param      (i)  LPCTSTR          lpszAdapterName     ����������
@param      (i)  int              nIndex              
@param      (i)  LPCTSTR          pIPAddress          IP��ַ
@param      (i)  LPCTSTR          pNetMask            ��������

@retval     BOOL    \n
            TRUE:�ɹ�    FALSE:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CCommunicationSetSvc::SetIP(LPCTSTR lpszAdapterName, int nIndex, LPCTSTR pIPAddress, LPCTSTR pNetMask, LPCTSTR pNetGate)
{
    try
    {
       	//2017-11-9 v16 ���޸�IP���������ڰ�����ʱ����
		/*//LOG("SetIP----0");
        if(!RegSetIP(lpszAdapterName, nIndex, pIPAddress, pNetMask, pNetGate)){
			LOG("SetIP----1");
		    return FALSE;
        }

	    if(!NotifyIPChange(lpszAdapterName, nIndex, pIPAddress, pNetMask)){
			LOG("SetIP----2");
	    	return FALSE;
		}*/
		// ���ñ���IP��ַ
		CString strLinkName = theMAINTENANCE_INFO.GetLinkName();
		CString strCommand = _T("netsh interface ip set address \"") + strLinkName + _T("\" static ") + pIPAddress + _T(" ") + pNetMask + _T(" ") + pNetGate + _T(" auto");
		// ����DOS����
		USES_CONVERSION;
		UINT iResult = WinExec(T2A(strCommand),SW_HIDE);
		// ִ�гɹ�
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
@brief      ֪ͨ����Ĭ���������ϵ�IP��ַ,��������,������Ϣ�Ѿ��ı�

@param      (i)  LPCTSTR          lpszAdapterName     ����������
@param      (i)  int              nIndex              
@param      (i)  LPCTSTR          pIPAddress          IP��ַ
@param      (i)  LPCTSTR          pNetMask            ��������

@retval     BOOL    \n
            TRUE:�ɹ�    FALSE:ʧ��

@exception  ��
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