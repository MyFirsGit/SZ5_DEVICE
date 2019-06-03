#include "stdafx.h"
#include "CommunicationSetMaintainDlg.h"
#include "CommunicationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#pragma comment ( lib, "iphlpapi.lib" ) 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCommunicationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CCommunicationSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
	ON_BTN_RESET_CLICK(OnReset)
END_MESSAGE_MAP()

// ����������ʾ��TITILE NAME
static const TCHAR* const BASE_TITLE_NAME   =  _T("ͨ�Ų�����Ϣ");
// ��ʼ���ַ�������
static const TCHAR* const TXT_MYIPADDRESS				       = 	_T("������ַ��");
static const TCHAR* const TXT_MYSUBNETMASK				       = 	_T("�������룺");
static const TCHAR* const TXT_MYGATEWAY				           = 	_T("Ĭ�����أ�");
static const TCHAR* const TXT_CPSHOSTADDRESS			       = 	_T("��λ��ַ��");
static const TCHAR* const TXT_CPSPORT			               = 	_T("��λ�˿ڣ�");
static const TCHAR* const TXT_NTPHOSTADDRESS			       = 	_T(" NTP��ַ��");
static const TCHAR* const TXT_NTPPORT			               = 	_T(" NTP�˿ڣ�");
static const TCHAR* const TXT_FTPHOSTADDRESS			       = 	_T(" FTP��ַ��");
static const TCHAR* const TXT_FTPPORT			               = 	_T(" FTP�˿ڣ�");

// ��������29��EDIT���λ��
static CRect editComInfo[] = {
	BASE_INFO_RECT(1,3),
	BASE_INFO_RECT_SP(1,3,6),
	BASE_INFO_RECT_SP(1,3,12),
	BASE_INFO_RECT_SP(1,3,18),
	BASE_INFO_RECT(2,3),
	BASE_INFO_RECT_SP(2,3,6),
	BASE_INFO_RECT_SP(2,3,12),
	BASE_INFO_RECT_SP(2,3,18),
	BASE_INFO_RECT(3,3),
	BASE_INFO_RECT_SP(3,3,6),
	BASE_INFO_RECT_SP(3,3,12),
	BASE_INFO_RECT_SP(3,3,18),
	BASE_INFO_RECT(4,3),
	BASE_INFO_RECT_SP(4,3,6),
	BASE_INFO_RECT_SP(4,3,12),
	BASE_INFO_RECT_SP(4,3,18),
	BASE_INFO_RECT(5,5),
	BASE_INFO_RECT(6,3),
	BASE_INFO_RECT_SP(6,3,6),
	BASE_INFO_RECT_SP(6,3,12),
	BASE_INFO_RECT_SP(6,3,18),
	BASE_INFO_RECT(7,5),
	BASE_INFO_RECT(8,3),
	BASE_INFO_RECT_SP(8,3,6),
	BASE_INFO_RECT_SP(8,3,12),
	BASE_INFO_RECT_SP(8,3,18),
	BASE_INFO_RECT(9,5)
};

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetMaintainDlg::CCommunicationSetMaintainDlg(CService* pService) 
	: COperationDlg(CCommunicationSetMaintainDlg::IDD, pService)
{ 
	m_baseInfo->titleInfo.titleName = BASE_TITLE_NAME;					// ����
	m_baseInfo->detailInfo.row = 9;                                     // ����
	m_baseInfo->detailInfo.labelGroup = m_labelCommunicationinfo;		// ��ǩ���֣�2�У�
	m_baseInfo->detailInfo.editGroup.cnt = 27;                          // �༭�����
	m_baseInfo->detailInfo.editGroup.editRect = editComInfo;            // �༭��λ��
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetMaintainDlg::~CCommunicationSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CCommunicationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		if (__super::OnCreate(lpCreateStruct,  GUIDE_AREA | BASE_AREA  | BTN_OK | BTN_RESET) == -1)
			return -1;
		// ���öԻ�����������
		GetEdit(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_CPSPort)->SetLimitText(5);
		GetEdit(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_NTPPort)->SetLimitText(5);
		GetEdit(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4)->SetLimitText(3);
		GetEdit(COMMUNICATION_EDIT_GROUP_FTPPort)->SetLimitText(5);
		// ��ʼ��BASE����LABEL���������
		InitlabelCommunicationInfo();
		// ��ʼ��BASE����EDIT���������
		InitEditText();
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����LABEL���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::InitlabelCommunicationInfo()
{
	// ���ô�������
	m_labelCommunicationinfo[0].nameLeft  = add_wsp(TXT_MYIPADDRESS,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[0].nameRight = "     .      .      .";
	m_labelCommunicationinfo[1].nameLeft  = add_wsp(TXT_MYSUBNETMASK,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[1].nameRight = "     .      .      .";
	m_labelCommunicationinfo[2].nameLeft  = add_wsp(TXT_MYGATEWAY,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[2].nameRight = "     .      .      .";
	m_labelCommunicationinfo[3].nameLeft  = add_wsp(TXT_CPSHOSTADDRESS,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[3].nameRight = "     .      .      .";
	m_labelCommunicationinfo[4].nameLeft  = add_wsp(TXT_CPSPORT,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[5].nameLeft  = add_wsp(TXT_NTPHOSTADDRESS,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[5].nameRight = "     .      .      .";
	m_labelCommunicationinfo[6].nameLeft  = add_wsp(TXT_NTPPORT,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[7].nameLeft  = add_wsp(TXT_FTPHOSTADDRESS,MAX_SBC_CASE_LEN);
	m_labelCommunicationinfo[7].nameRight = "     .      .      .";
	m_labelCommunicationinfo[8].nameLeft  = add_wsp(TXT_FTPPORT,MAX_SBC_CASE_LEN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��BASE����EDIT���������

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::InitEditText()
{
	CStringArray myIPAddressArray,mySubnetMaskArray,myGateWayArray;
	CStringArray cpsHostAddressArray,ntpHostAddressArray,ftpHostAddressArray;
	CString cpsPort,ntpPort,ftpPort;

	// ������IP��ַ���������ݲ�ֳ��ַ�������
	// ���IP��ַ
	GetSplitStrArray(theMAINTENANCE_INFO.GetIP(),_T("."),myIPAddressArray);
	// �����������
	GetSplitStrArray(theMAINTENANCE_INFO.GetSubnetMask(),_T("."),mySubnetMaskArray);
	// �������
	GetSplitStrArray(theMAINTENANCE_INFO.GetGateway(),_T("."),myGateWayArray);
	// ���CPS������ַ
	GetSplitStrArray(theMAINTENANCE_INFO.GetCpsHostID(),_T("."),cpsHostAddressArray);
	// ���NTP������ַ
	GetSplitStrArray(theMAINTENANCE_INFO.GetNtpHostID(),_T("."),ntpHostAddressArray);
	// ���FTP������ַ
	GetSplitStrArray(theMAINTENANCE_INFO.GetFtpHostID(),_T("."),ftpHostAddressArray);
	// ���IP��ַ�����СΪ4λ����IP��ַ���õ��ؼ�����ʾ;������ʾΪ���ַ���
	if (4 == myIPAddressArray.GetCount()){
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1,myIPAddressArray[0]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2,myIPAddressArray[1]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3,myIPAddressArray[2]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4,myIPAddressArray[3]);
	}
	else{
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4,_T(""));
	}
	// ����������������СΪ4λ����IP��ַ���õ��ؼ�����ʾ;������ʾΪ���ַ���
	if (4 == mySubnetMaskArray.GetCount()){
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1,mySubnetMaskArray[0]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2,mySubnetMaskArray[1]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3,mySubnetMaskArray[2]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4,mySubnetMaskArray[3]);
	}
	else{
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4,_T(""));
	}
	// ���Ĭ�����������СΪ4λ����IP��ַ���õ��ؼ�����ʾ;������ʾΪ���ַ���
	if (4 == myGateWayArray.GetCount()){
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1,myGateWayArray[0]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2,myGateWayArray[1]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3,myGateWayArray[2]);
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4,myGateWayArray[3]);
	}
	else{
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4,_T(""));
	}
	// ���CPS������ַ��СΪ4λ����CPS������ַ���õ��ؼ�����ʾ;������ʾΪ���ַ���
	if (4 == cpsHostAddressArray.GetCount()){
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1,cpsHostAddressArray[0]);
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2,cpsHostAddressArray[1]);
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3,cpsHostAddressArray[2]);
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4,cpsHostAddressArray[3]);
	}
	else{
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4,_T(""));
	}
	// ȡ��CPS�˿ںţ������õ��ؼ���
	cpsPort.Format(_T("%d"),theMAINTENANCE_INFO.GetCpsPort()); 
	SetEditText(COMMUNICATION_EDIT_GROUP_CPSPort,cpsPort);
	// ���NTP������ַ��СΪ4λ����NTP������ַ���õ��ؼ�����ʾ;������ʾΪ���ַ���
	if (4 == ntpHostAddressArray.GetCount()){
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1,ntpHostAddressArray[0]);
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2,ntpHostAddressArray[1]);
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3,ntpHostAddressArray[2]);
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4,ntpHostAddressArray[3]);
	}
	else{
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4,_T(""));
	}

	// ȡ��NTP�˿ںţ������õ��ؼ���
	ntpPort.Format(_T("%d"),theMAINTENANCE_INFO.GetNtpPort()); 
	SetEditText(COMMUNICATION_EDIT_GROUP_NTPPort,ntpPort);
	// ���FTP������ַ��СΪ4λ����FTP������ַ���õ��ؼ�����ʾ;������ʾΪ���ַ���
	if (4 == ftpHostAddressArray.GetCount()){
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1,ftpHostAddressArray[0]);
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2,ftpHostAddressArray[1]);
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3,ftpHostAddressArray[2]);
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4,ftpHostAddressArray[3]);
	}
	else {
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3,_T(""));
		SetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4,_T(""));
	}
	// ȡ��FTP�˿ںţ������õ��ؼ���
	ftpPort.Format(_T("%d"),theMAINTENANCE_INFO.GetFtpPort()); 
	SetEditText(COMMUNICATION_EDIT_GROUP_FTPPort,ftpPort);

	// ���ñ༭�򽹵�
	SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��EDIT���������ݵı��浽SVC����

@param      void

@retval     none

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::SetCommunicateParameters()
{
	// ��ʽ������IP��ַ�ַ���
	CString strLocalIP = GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4);
	// ��ʽ���������������ַ���
	CString strSubnetMask = GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4);
	// ��ʽ�����������ַ���
	CString strGateway = GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4);
	// CPS IP��ַ
	CString strCPSIP = GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4);
	// CPS �˿�
	CString strCPSPort = GetEditText(COMMUNICATION_EDIT_GROUP_CPSPort);
	// NTP IP��ַ
	CString strNTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4);
	// NTP �˿�
	CString strNTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_NTPPort);
	// FTP IP��ַ
	CString strFTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4);
	// FTP �˿�
	CString strFTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_FTPPort);

	// �޸�INI�ļ�
	// ����IP��ַ
	theMAINTENANCE_INFO.SetIP(strLocalIP);
	// ������������
	theMAINTENANCE_INFO.SetSubnetMask(strSubnetMask);
	// ��������
	theMAINTENANCE_INFO.SetGateway(strGateway);
	// ����CPS������ַ
	theMAINTENANCE_INFO.SetCpsHostID(strCPSIP);
	// ����CPS�˿ں�
	theMAINTENANCE_INFO.SetCpsPort(strCPSPort);
	// ����NTP������ַ
	theMAINTENANCE_INFO.SetNtpHostID(strNTPIP);
	// ����NTP�˿ں�
	theMAINTENANCE_INFO.SetNtpPort(strNTPPort);
	// ����FTP������ַ
	theMAINTENANCE_INFO.SetFtpHostID(strFTPIP);
	// ����FTP�˿ں�
	theMAINTENANCE_INFO.SetFtpPort(strFTPPort);
	// ���³�ʼ��ά������
	theMAINTENANCE_INFO.Initialize();

	// ���ñ���IP��ַ
	CString strLinkName = theMAINTENANCE_INFO.GetLinkName();
	CString strCommand = _T("netsh interface ip set address \"") + strLinkName + _T("\" static ") + strLocalIP + _T(" ") + strSubnetMask + _T(" ") + strGateway + _T(" auto");
	// ����DOS����
	USES_CONVERSION;
	UINT iResult = WinExec(T2A(strCommand),SW_HIDE);
	// ִ�гɹ�
	if (iResult > 31){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_FINISH));
	}
	// ִ��ʧ��
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_IP_FAIL));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���EDIT����������Ƿ�Ϸ�

@param      void

@retval     bool /n
true �Ϸ�     false �Ƿ�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CCommunicationSetMaintainDlg::CheckEditText()
{

	// ���IP��ַ��4����,�ǲ������ֲ���0-255֮��
	// ���˿ں��ǲ������ֲ���0-65535ֻ��

	// ���IP��ַ��һ����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1);
		return false;
	}

	// ���IP��ַ�ڶ�����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2);
		return false;
	}

	// ���IP��ַ��������
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3);
		return false;
	}

	// ���IP��ַ���Ĳ���
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4);
		return false;
	}

	// ������������һ����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1);
		return false;
	}

	// �����������ڶ�����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2);
		return false;
	}

	// ������������������
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3);
		return false;
	}

	// �������������Ĳ���
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4);
		return false;
	}

	// ��鱾�����ص�һ����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1);
		return false;
	}

	// ��鱾�����صڶ�����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2);
		return false;
	}

	// ��鱾�����ص�������
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3);
		return false;
	}

	// ��鱾�����ص��Ĳ���
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4);
		return false;
	}

	// �������IP��һ����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1);
		return false;
	}

	// �������IP�ڶ�����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2);
		return false;
	}

	// �������IP��������
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3);
		return false;
	}

	// �������IP���Ĳ���
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4);
		return false;
	}

	// ��������˿ں�
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSPort),0,65535)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSPort);
		return false;
	}

	// ���NTP������ַ��һ����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1);
		return false;
	}

	// ���NTP������ַ�ڶ�����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2);
		return false;
	}

	// ���NTP������ַ��������
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3);
		return false;
	}

	// ���NTP������ַ���Ĳ���
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4);
		return false;
	}

	// ���NTP�˿ں�
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPPort),0,65535)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPPort);
		return false;
	}

	// ���FTP������ַ��һ����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1);
		return false;
	}

	// ���FTP������ַ�ڶ�����
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2);
		return false;
	}

	// ���FTP������ַ��������
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3);
		return false;
	}

	// ���FTP������ַ���Ĳ���
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4);
		return false;
	}

	// ���FTP�˿ں�
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPPort),0,65535)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPPort);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ð�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCommunicationSetMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	try{
		// ��ʼ���༭��ؼ�
		InitEditText();
		// ��ʾ�û�������
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INFO_INPUT));
	}
	catch (CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCommunicationSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// ���༭���������Ƿ���Ϲ淶
		if(CheckEditText()){
			SetCommunicateParameters();
			HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		}
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	catch (CSysException& e){
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e = CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		theAPP_SESSION.ShowOfficeGuide(theEXCEPTION_MGR.GetDescription(e));
		HideGUIModule(BTN_OK | BTN_RESET | EDIT_BOX | KEYBOARD_AREA);
		theSERVICE_MGR.SetForeServiceBusy(false);
		theEXCEPTION_MGR.ProcessException(e);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ؽ������

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// ��ʽ������IP��ַ�ַ���
	CString strLocalIP = GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4);
	// ��ʽ���������������ַ���
	CString strSubnetMask = GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4);
	// ��ʽ�����������ַ���
	CString strGateway = GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4);
	// CPS IP��ַ
	CString strCPSIP = GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4);
	// CPS �˿�
	CString strCPSPort = GetEditText(COMMUNICATION_EDIT_GROUP_CPSPort);
	// NTP IP��ַ
	CString strNTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4);
	// NTP �˿�
	CString strNTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_NTPPort);
	// FTP IP��ַ
	CString strFTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4);
	// FTP �˿�
	CString strFTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_FTPPort);
	
	// ���ñ�ǩ����
	m_labelCommunicationinfo[0].nameRight = strLocalIP;
	m_labelCommunicationinfo[1].nameRight = strSubnetMask;
	m_labelCommunicationinfo[2].nameRight = strGateway;
	m_labelCommunicationinfo[3].nameRight = strCPSIP;
	m_labelCommunicationinfo[4].nameRight = strCPSPort;
	m_labelCommunicationinfo[5].nameRight = strNTPIP;
	m_labelCommunicationinfo[6].nameRight = strNTPPort;
	m_labelCommunicationinfo[7].nameRight = strFTPIP;
	m_labelCommunicationinfo[8].nameRight = strFTPPort;
	// �������
	HideCompoment(ulCompomentFlag);
	// ˢ�»�����Ϣ��
	m_baseInfoArea->Invalidate();
}
