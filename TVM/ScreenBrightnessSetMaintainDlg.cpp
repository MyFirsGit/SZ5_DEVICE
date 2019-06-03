#include "stdafx.h"
#include "ScreenBrightnessSetSvc.h"
#include "ScreenBrightnessSetMaintainDlg.h"
#include "AppSession.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CScreenBrightnessSetDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CScreenBrightnessSetDlg,COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetDlg::CScreenBrightnessSetDlg(CService* pService)
	:COperationDlg(CScreenBrightnessSetDlg::IDD, pService)
{
	m_baseInfo->detailInfo.labelGroup = m_labelGroup;           
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      (i)无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CScreenBrightnessSetDlg::~CScreenBrightnessSetDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int  0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CScreenBrightnessSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)){
		return -1;
	}
	theAPP_SESSION.SetKeyboard(KEY_F2 | KEY_PAGE | KEY_RETURN);		// 启用指定按键
	theAPP_SESSION.SetMaxInputLength(1);							// 设置最大输入位数
	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 200);				// 设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 400);				// 设置第二列宽度

	//InitlabelScreenBrightnessInfo();											// 初始化BASE区域LABEL里面的数据
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
void CScreenBrightnessSetDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelScreenBrightnessInfo();											// 初始化BASE区域LABEL里面的数据
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CScreenBrightnessSetDlg::InitlabelScreenBrightnessInfo()
{
	CScreenBrightnessSetSvc* pSvc = (CScreenBrightnessSetSvc*)GetService();
	CScreenBrightnessSetSvc::Model& model = pSvc->GetDataModel();
	
	CString Brightness = _T("");
	Brightness.Format(_T("%d"),model.BrightnessNumber);
	m_labelGroup[0].nameLeft = _opl(GUIDE_SCREEN_BRIGHTNESS_SET);
	m_labelGroup[0].nameRight = Brightness;		// 屏幕亮度默认为5
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CScreenBrightnessSetDlg::UpdateUI()
{
	ShowCodeContent();			//基本信息区显示对应项设置的数据
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

void CScreenBrightnessSetDlg::ShowCodeContent()
{
	CScreenBrightnessSetSvc* pSvc = (CScreenBrightnessSetSvc*)GetService();
	CScreenBrightnessSetSvc::Model& model = pSvc->GetDataModel();
	
	CString TimeStr = _T("");
	TimeStr.Format(_T("%d"),model.BrightnessNumber);
	m_labelGroup[0].nameRight = TimeStr;
}
