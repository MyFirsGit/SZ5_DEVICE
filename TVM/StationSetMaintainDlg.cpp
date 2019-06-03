#include "stdafx.h"
#include "StationSetMaintainDlg.h"
#include "StationSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStationSetMaintainDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CStationSetMaintainDlg, COperationDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*pService    对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::CStationSetMaintainDlg(CService* pService) 
	: COperationDlg(CStationSetMaintainDlg::IDD, pService)
{
	m_baseInfo->detailInfo.labelGroup = m_lableGroup;               // 行数
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStationSetMaintainDlg::~CStationSetMaintainDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval      int 0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CStationSetMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct,  BASE_AREA | GUIDE_AREA) == -1){
		return -1;
	}

	theAPP_SESSION.SetKeyboard(KEY_INPUT | KEY_F2 | KEY_RETURN);	// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 200);				// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 400);				// 设置第二列宽度

	//InitlabelStationInfo();											// 初始化基本区域的label信息
	//SetFromReadIniText();											// 读取Maintenance.INI配置文件并显示在对应项上

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      画面激活

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelStationInfo();											// 初始化基本区域的label信息
	SetFromReadIniText();											// 读取Maintenance.INI配置文件并显示在对应项上
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::InitlabelStationInfo()
{
	// 初始化车站信息区域文字
	m_lableGroup[0].nameLeft = _T("[1] ")+_opl(STATIONSET_LINE_CODE)+_opl(TXT_COLON);
	m_lableGroup[1].nameLeft = _T("[2] ")+ _opl(STATIONSET_STATION_CODE)+_opl(TXT_COLON);
	m_lableGroup[2].nameLeft = _T("[3] ")+ _opl(STATIONSET_MACHINE_CODE)+_opl(TXT_COLON);
	m_lableGroup[3].nameLeft = _T("[4] ")+_opl(STATIONSET_GROUP_NUMBER)+_opl(TXT_COLON);
	m_lableGroup[4].nameLeft = _T("[5] ")+_opl(STATIONSET_EQUNOINGROUP)+_opl(TXT_COLON);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::UpdateUI()
{
	CStationSetSvc* pSvc = (CStationSetSvc*)GetService();
	CStationSetSvc::Model& model = pSvc->GetDataModel();
	if(TRUE == model.CodeChange)										// 代码更新
	{
		ShowCodeContent();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示从Maintenance.INI读取的配置文件在对应项上

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::SetFromReadIniText()
{
	CStationSetSvc* pSvc = (CStationSetSvc*)GetService();
	CStationSetSvc::Model& model = pSvc->GetDataModel();

	// 将车站代码，本机代码，组号，组内编号设置到界面
	m_lableGroup[0].nameRight = model.sLineCode;
	m_lableGroup[1].nameRight = model.sStationCode;
	m_lableGroup[2].nameRight = model.sMachineCode;
	m_lableGroup[3].nameRight = model.sGroupNumber;
	m_lableGroup[4].nameRight = model.sEquNumberInGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      基本信息区显示对应项设置的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CStationSetMaintainDlg::ShowCodeContent()
{
	CStationSetSvc* pSvc = (CStationSetSvc*)GetService();
	CStationSetSvc::Model& model = pSvc->GetDataModel();	
	if(FALSE == model.CodeLegal )									// 代码内容非法
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	switch (model.MenuNumber)
	{
	case MENU_CODE_1:		// 线路代码
		m_lableGroup[0].nameRight = model.sLineCode;
		break;
	case MENU_CODE_2:		// 车站代码
		m_lableGroup[1].nameRight = model.sStationCode;
		break;
	case MENU_CODE_3:		// 本机代码
		m_lableGroup[2].nameRight = model.sMachineCode;
		break;
	case MENU_CODE_4:		// 组编号
		m_lableGroup[3].nameRight = model.sGroupNumber;
		break;
	case MENU_CODE_5:		// 组内编号
		m_lableGroup[4].nameRight = model.sEquNumberInGroup;
		break;
	default:
		break;
	}

	if(theTVM_STATUS_MGR.GetCommunicate() == CONNECTED){
		theAPP_SESSION.ShowOfficeGuide(_opl(TXT_WHEN_SC_CONNECTION_CANNOT_USE_THIS_FUNC));
	}
	else{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
	}
}