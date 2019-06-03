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

// 基本区域显示的TITILE NAME
static const TCHAR* const BASE_TITLE_NAME   =  _T("通信参数信息");
// 初始化字符串常量
static const TCHAR* const TXT_MYIPADDRESS				       = 	_T("本机地址：");
static const TCHAR* const TXT_MYSUBNETMASK				       = 	_T("子网掩码：");
static const TCHAR* const TXT_MYGATEWAY				           = 	_T("默认网关：");
static const TCHAR* const TXT_CPSHOSTADDRESS			       = 	_T("上位地址：");
static const TCHAR* const TXT_CPSPORT			               = 	_T("上位端口：");
static const TCHAR* const TXT_NTPHOSTADDRESS			       = 	_T(" NTP地址：");
static const TCHAR* const TXT_NTPPORT			               = 	_T(" NTP端口：");
static const TCHAR* const TXT_FTPHOSTADDRESS			       = 	_T(" FTP地址：");
static const TCHAR* const TXT_FTPPORT			               = 	_T(" FTP端口：");

// 基本区域29个EDIT框的位置
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
@brief      构造函数

@param      (i)CService*    pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetMaintainDlg::CCommunicationSetMaintainDlg(CService* pService) 
	: COperationDlg(CCommunicationSetMaintainDlg::IDD, pService)
{ 
	m_baseInfo->titleInfo.titleName = BASE_TITLE_NAME;					// 标题
	m_baseInfo->detailInfo.row = 9;                                     // 行数
	m_baseInfo->detailInfo.labelGroup = m_labelCommunicationinfo;		// 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 27;                          // 编辑框个数
	m_baseInfo->detailInfo.editGroup.editRect = editComInfo;            // 编辑框位置
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetMaintainDlg::~CCommunicationSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CCommunicationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{
		if (__super::OnCreate(lpCreateStruct,  GUIDE_AREA | BASE_AREA  | BTN_OK | BTN_RESET) == -1)
			return -1;
		// 设置对话框输入限制
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
		// 初始化BASE区域LABEL里面的数据
		InitlabelCommunicationInfo();
		// 初始化BASE区域EDIT里面的数据
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
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::InitlabelCommunicationInfo()
{
	// 设置窗口区域
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
@brief      初始化BASE区域EDIT里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::InitEditText()
{
	CStringArray myIPAddressArray,mySubnetMaskArray,myGateWayArray;
	CStringArray cpsHostAddressArray,ntpHostAddressArray,ftpHostAddressArray;
	CString cpsPort,ntpPort,ftpPort;

	// 把类似IP地址这样的数据差分成字符串数组
	// 获得IP地址
	GetSplitStrArray(theMAINTENANCE_INFO.GetIP(),_T("."),myIPAddressArray);
	// 获得子网掩码
	GetSplitStrArray(theMAINTENANCE_INFO.GetSubnetMask(),_T("."),mySubnetMaskArray);
	// 获得网关
	GetSplitStrArray(theMAINTENANCE_INFO.GetGateway(),_T("."),myGateWayArray);
	// 获得CPS主机地址
	GetSplitStrArray(theMAINTENANCE_INFO.GetCpsHostID(),_T("."),cpsHostAddressArray);
	// 获得NTP主机地址
	GetSplitStrArray(theMAINTENANCE_INFO.GetNtpHostID(),_T("."),ntpHostAddressArray);
	// 获得FTP主机地址
	GetSplitStrArray(theMAINTENANCE_INFO.GetFtpHostID(),_T("."),ftpHostAddressArray);
	// 如果IP地址数组大小为4位，将IP地址设置到控件中显示;否则，显示为空字符串
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
	// 如果子网掩码数组大小为4位，将IP地址设置到控件中显示;否则，显示为空字符串
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
	// 如果默认网关数组大小为4位，将IP地址设置到控件中显示;否则，显示为空字符串
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
	// 如果CPS主机地址大小为4位，将CPS主机地址设置到控件中显示;否则，显示为空字符串
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
	// 取得CPS端口号，并设置到控件中
	cpsPort.Format(_T("%d"),theMAINTENANCE_INFO.GetCpsPort()); 
	SetEditText(COMMUNICATION_EDIT_GROUP_CPSPort,cpsPort);
	// 如果NTP主机地址大小为4位，将NTP主机地址设置到控件中显示;否则，显示为空字符串
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

	// 取得NTP端口号，并设置到控件中
	ntpPort.Format(_T("%d"),theMAINTENANCE_INFO.GetNtpPort()); 
	SetEditText(COMMUNICATION_EDIT_GROUP_NTPPort,ntpPort);
	// 如果FTP主机地址大小为4位，将FTP主机地址设置到控件中显示;否则，显示为空字符串
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
	// 取得FTP端口号，并设置到控件中
	ftpPort.Format(_T("%d"),theMAINTENANCE_INFO.GetFtpPort()); 
	SetEditText(COMMUNICATION_EDIT_GROUP_FTPPort,ftpPort);

	// 设置编辑框焦点
	SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      把EDIT框里面数据的保存到SVC里面

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::SetCommunicateParameters()
{
	// 格式化主机IP地址字符串
	CString strLocalIP = GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4);
	// 格式化主机子网掩码字符串
	CString strSubnetMask = GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4);
	// 格式化主机网关字符串
	CString strGateway = GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3) + _T(".") 
							+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4);
	// CPS IP地址
	CString strCPSIP = GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4);
	// CPS 端口
	CString strCPSPort = GetEditText(COMMUNICATION_EDIT_GROUP_CPSPort);
	// NTP IP地址
	CString strNTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4);
	// NTP 端口
	CString strNTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_NTPPort);
	// FTP IP地址
	CString strFTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3) + _T(".") 
						+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4);
	// FTP 端口
	CString strFTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_FTPPort);

	// 修改INI文件
	// 设置IP地址
	theMAINTENANCE_INFO.SetIP(strLocalIP);
	// 设置子网掩码
	theMAINTENANCE_INFO.SetSubnetMask(strSubnetMask);
	// 设置网关
	theMAINTENANCE_INFO.SetGateway(strGateway);
	// 设置CPS主机地址
	theMAINTENANCE_INFO.SetCpsHostID(strCPSIP);
	// 设置CPS端口号
	theMAINTENANCE_INFO.SetCpsPort(strCPSPort);
	// 设置NTP主机地址
	theMAINTENANCE_INFO.SetNtpHostID(strNTPIP);
	// 设置NTP端口号
	theMAINTENANCE_INFO.SetNtpPort(strNTPPort);
	// 设置FTP主机地址
	theMAINTENANCE_INFO.SetFtpHostID(strFTPIP);
	// 设置FTP端口号
	theMAINTENANCE_INFO.SetFtpPort(strFTPPort);
	// 重新初始化维护数据
	theMAINTENANCE_INFO.Initialize();

	// 设置本机IP地址
	CString strLinkName = theMAINTENANCE_INFO.GetLinkName();
	CString strCommand = _T("netsh interface ip set address \"") + strLinkName + _T("\" static ") + strLocalIP + _T(" ") + strSubnetMask + _T(" ") + strGateway + _T(" auto");
	// 启动DOS命令
	USES_CONVERSION;
	UINT iResult = WinExec(T2A(strCommand),SW_HIDE);
	// 执行成功
	if (iResult > 31){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_FINISH));
	}
	// 执行失败
	else{
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_IP_FAIL));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查EDIT框里的数据是否合法

@param      void

@retval     bool /n
true 合法     false 非法

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CCommunicationSetMaintainDlg::CheckEditText()
{

	// 检查IP地址的4个项,是不是数字并在0-255之间
	// 检查端口号是不是数字并在0-65535只间

	// 检查IP地址第一部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1);
		return false;
	}

	// 检查IP地址第二部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2);
		return false;
	}

	// 检查IP地址第三部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3);
		return false;
	}

	// 检查IP地址第四部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4);
		return false;
	}

	// 检查子网掩码第一部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1);
		return false;
	}

	// 检查子网掩码第二部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2);
		return false;
	}

	// 检查子网掩码第三部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3);
		return false;
	}

	// 检查子网掩码第四部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4);
		return false;
	}

	// 检查本地网关第一部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1);
		return false;
	}

	// 检查本地网关第二部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2);
		return false;
	}

	// 检查本地网关第三部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3);
		return false;
	}

	// 检查本地网关第四部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4);
		return false;
	}

	// 检查主机IP第一部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1);
		return false;
	}

	// 检查主机IP第二部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2);
		return false;
	}

	// 检查主机IP第三部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3);
		return false;
	}

	// 检查主机IP第四部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4);
		return false;
	}

	// 检查主机端口号
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_CPSPort),0,65535)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_CPSPort);
		return false;
	}

	// 检查NTP主机地址第一部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1);
		return false;
	}

	// 检查NTP主机地址第二部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2);
		return false;
	}

	// 检查NTP主机地址第三部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3);
		return false;
	}

	// 检查NTP主机地址第四部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4);
		return false;
	}

	// 检查NTP端口号
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_NTPPort),0,65535)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_NTPPort);
		return false;
	}

	// 检查FTP主机地址第一部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1);
		return false;
	}

	// 检查FTP主机地址第二部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2);
		return false;
	}

	// 检查FTP主机地址第三部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3);
		return false;
	}

	// 检查FTP主机地址第四部分
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4),0,255)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4);
		return false;
	}

	// 检查FTP端口号
	if (!CheckNumText(GetEditText(COMMUNICATION_EDIT_GROUP_FTPPort),0,65535)){
		theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INPUT_ERROR));
		SetEditFocus(COMMUNICATION_EDIT_GROUP_FTPPort);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCommunicationSetMaintainDlg::OnReset(WPARAM wParam,LPARAM lParam)
{
	try{
		// 初始化编辑框控件
		InitEditText();
		// 显示用户导航栏
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
@brief      处理确认按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCommunicationSetMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	try{
		theSERVICE_MGR.SetForeServiceBusy(true);
		// 检查编辑框中数据是否符合规范
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
@brief      隐藏界面组件

@param      void

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::HideGUIModule(ULONG ulCompomentFlag)
{
	// 格式化主机IP地址字符串
	CString strLocalIP = GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyIPAddress_PART4);
	// 格式化主机子网掩码字符串
	CString strSubnetMask = GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MySubnetMask_PART4);
	// 格式化主机网关字符串
	CString strGateway = GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_MyGateWay_PART4);
	// CPS IP地址
	CString strCPSIP = GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_CPSHostAddress_PART4);
	// CPS 端口
	CString strCPSPort = GetEditText(COMMUNICATION_EDIT_GROUP_CPSPort);
	// NTP IP地址
	CString strNTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_NTPHostAddress_PART4);
	// NTP 端口
	CString strNTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_NTPPort);
	// FTP IP地址
	CString strFTPIP = GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART1) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART2) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART3) + _T(".") 
		+ GetEditText(COMMUNICATION_EDIT_GROUP_FTPHostAddress_PART4);
	// FTP 端口
	CString strFTPPort = GetEditText(COMMUNICATION_EDIT_GROUP_FTPPort);
	
	// 设置标签文字
	m_labelCommunicationinfo[0].nameRight = strLocalIP;
	m_labelCommunicationinfo[1].nameRight = strSubnetMask;
	m_labelCommunicationinfo[2].nameRight = strGateway;
	m_labelCommunicationinfo[3].nameRight = strCPSIP;
	m_labelCommunicationinfo[4].nameRight = strCPSPort;
	m_labelCommunicationinfo[5].nameRight = strNTPIP;
	m_labelCommunicationinfo[6].nameRight = strNTPPort;
	m_labelCommunicationinfo[7].nameRight = strFTPIP;
	m_labelCommunicationinfo[8].nameRight = strFTPPort;
	// 隐藏组件
	HideCompoment(ulCompomentFlag);
	// 刷新基本信息区
	m_baseInfoArea->Invalidate();
}
