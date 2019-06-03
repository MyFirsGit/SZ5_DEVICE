#include "stdafx.h"
#include "CommunicationSetMaintainDlg.h"
#include "CommunicationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"

#pragma comment ( lib, "iphlpapi.lib") 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCommunicationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CCommunicationSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


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
	m_baseInfo->detailInfo.labelGroup = m_labelGroup;// 行数		
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

@retval     int   0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CCommunicationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct,  GUIDE_AREA | BASE_AREA) == -1)
	{
		return -1;
	}
	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN | KEY_DOT);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);				// 设置最大输入位数

	m_baseInfoArea->SetLableWidth(LABEL_COL_1,200);		// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2,400);		// 设置第二列宽度

	//InitlabelCommunicationInfo();						// 初始化BASE区域LABEL里面的数据
	//SetFromReadIniText();								// 读取Maintenance.INI配置文件并显示在对应项上

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      画面激活函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelCommunicationInfo();						// 初始化BASE区域LABEL里面的数据
	SetFromReadIniText();								// 读取Maintenance.INI配置文件并显示在对应项上
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
	// 初始化车站信息区域文字
	m_labelGroup[0].nameLeft = _T("[1] ")+_opl(COMMUNICATION_TXT_MYIPADDRESS)+_opl(TXT_COLON);
	m_labelGroup[1].nameLeft = _T("[2] ")+_opl(COMMUNICATION_TXT_MYSUBNETMASK)+_opl(TXT_COLON);
	m_labelGroup[2].nameLeft = _T("[3] ")+_opl(COMMUNICATION_TXT_MYGATEWAY)+_opl(TXT_COLON);
	m_labelGroup[3].nameLeft = _T("[4] ")+_opl(COMMUNICATION_TXT_CPSHOSTADDRESS)+_opl(TXT_COLON);
	m_labelGroup[4].nameLeft = _T("[5] ")+_opl(COMMUNICATION_TXT_CPSPORT)+_opl(TXT_COLON);
	m_labelGroup[5].nameLeft = _T("[6] ")+_opl(COMMUNICATION_TXT_NTPHOSTADDRESS)+_opl(TXT_COLON);
	m_labelGroup[6].nameLeft = _T("[7] ")+_opl(COMMUNICATION_TXT_NTPPORT)+_opl(TXT_COLON);
	m_labelGroup[7].nameLeft = _T("[8] ")+_opl(COMMUNICATION_TXT_FTPHOSTADDRESS)+_opl(TXT_COLON);
	m_labelGroup[8].nameLeft = _T("[9] ")+_opl(COMMUNICATION_TXT_FTPPORT)+_opl(TXT_COLON);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      读取Maintenance.INI配置文件并显示在对应项上

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::SetFromReadIniText()
{
	CCommunicationSetSvc* pSvc = (CCommunicationSetSvc*)GetService();
	CCommunicationSetSvc::Model& model = pSvc->GetDataModel();
	m_labelGroup[0].nameRight = model.myIpAddress;
	m_labelGroup[1].nameRight = model.mySubnetAddress;
	m_labelGroup[2].nameRight = model.myGatewayAddress;
	m_labelGroup[3].nameRight = model.scHostAddress;
	m_labelGroup[4].nameRight = model.scHostPort;
	m_labelGroup[5].nameRight = model.scNtpAddress;
	m_labelGroup[6].nameRight = model.scNtpPort;
	m_labelGroup[7].nameRight = model.scFtpAddress;
	m_labelGroup[8].nameRight = model.scFtpPort;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::UpdateUI()
{
	CCommunicationSetSvc * pSvc = (CCommunicationSetSvc*)GetService();
	CCommunicationSetSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.IpChange)			//IP更新
	{
		ShowIPContent();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      基本信息区显示对应项设置的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCommunicationSetMaintainDlg::ShowIPContent()
{
	CCommunicationSetSvc* pSvc = (CCommunicationSetSvc*)GetService();
	CCommunicationSetSvc::Model& model = pSvc->GetDataModel();
	if(FALSE == model.IpLegal)		//IP内容非法
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	switch(model.MenuNumber)
	{
	case MENU_CODE_1:				//本机地址
		m_labelGroup[0].nameRight = model.myIpAddress;
		break;
	case MENU_CODE_2:				//子网掩码
		m_labelGroup[1].nameRight = model.mySubnetAddress;
		break;
	case MENU_CODE_3:				//网关
		m_labelGroup[2].nameRight = model.myGatewayAddress;
		break;
	case MENU_CODE_4:				//主机地址
		m_labelGroup[3].nameRight = model.scHostAddress;
		break;
	case MENU_CODE_5:				//主机端口
		m_labelGroup[4].nameRight = model.scHostPort;
		break;
	case MENU_CODE_6:				//NTP地址
		m_labelGroup[5].nameRight = model.scNtpAddress;
		break;
	case MENU_CODE_7:				//NTP端口
		m_labelGroup[6].nameRight = model.scNtpPort;
		break;
	case MENU_CODE_8:				//FTP地址
		m_labelGroup[7].nameRight = model.scFtpAddress;
		break;
	case MENU_CODE_9:				//FTP端口
		return;
	default:
		break;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}
