#include "stdafx.h"
#include "CoinAddingBoxSetMaintainDlg.h"
#include "CoinAddingBoxSetSvc.h"
#include "AppSession.h"
#include "GuideInfo.h"
#include "LanguageInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCoinAddingBoxMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CCoinAddingBoxMaintainDlg,COperationDlg)
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
CCoinAddingBoxMaintainDlg :: CCoinAddingBoxMaintainDlg(CService* pService) 
	: COperationDlg(CCoinAddingBoxMaintainDlg::IDD, pService)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      void

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CCoinAddingBoxMaintainDlg::~CCoinAddingBoxMaintainDlg()
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
int CCoinAddingBoxMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (__super::OnCreate(lpCreateStruct,GUIDE_AREA) == -1){
		return -1;
	}
	// 设置GStatic
	m_baseInfoArea = new CGStatic(this, m_baseInfo, m_labelGroup);

	m_baseInfoArea->SetLableWidth(LABEL_COL_1, 150);//设置第一列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_2, 100);//设置第二列宽度
	m_baseInfoArea->SetLableWidth(LABEL_COL_3, 350);//设置第三列宽度
	
	m_baseInfoArea->SetLableAlign(LABEL_COL_3, DT_LEFT | DT_VCENTER);

	//InitlabelAddingBoxInfo();//初始化基本区域的label信息
	//SetFromReadIniText();//读取TVM.INI配置文件并显示在对应项上

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
void CCoinAddingBoxMaintainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){
	__super::OnActivate(nState,pWndOther,bMinimized);
	InitlabelAddingBoxInfo();//初始化基本区域的label信息
	SetFromReadIniText();//读取TVM.INI配置文件并显示在对应项上
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化BASE区域LABEL里面的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::InitlabelAddingBoxInfo()
{
	// 初始化信息区域文字
	m_labelGroup[0].nameLeft = _T("[1]")+_opl(GUIDE_ADDBOX_SET_A)+_opl(TXT_COLON) ;	//补充箱A
	m_labelGroup[0].nameRight = _opl(GUIDE_ADDBOX_SET_MENU);
		
	m_labelGroup[1].nameLeft = _T("[2]")+_opl(GUIDE_ADDBOX_SET_B)+_opl(TXT_COLON);	//补充箱B
	m_labelGroup[1].nameRight = _opl(GUIDE_ADDBOX_SET_MENU);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示从TVM.INI读取的配置文件在对应项上

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::SetFromReadIniText()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();
	m_labelGroup[0].nameCenter = model.hoponecurrency;
	m_labelGroup[1].nameCenter = model.hoptwocurrency;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::UpdateUI()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();

	if(TRUE == model.CodeChange)//代码更新
	{
		ShowCodeContent();
	}
	if(TRUE == model.MenuChange)//菜单更新
	{
		ShowGuideDisplay();
	}
	m_baseInfoArea->Invalidate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      向导栏显示提示数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CCoinAddingBoxMaintainDlg::ShowGuideDisplay()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();
	switch(model.MenuNumber)
	{
	case MENU_CODE_1:		//补充箱A
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ADDBOX_SET_A)+_opl(TXT_COLON) );
		break;
	case MENU_CODE_2:		//补充箱B
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ADDBOX_SET_B)+_opl(TXT_COLON));
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      基本信息区显示对应项设置的数据

@param      void

@retval     none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

void CCoinAddingBoxMaintainDlg::ShowCodeContent()
{
	CCoinAddingBoxSetSvc* pSvc = (CCoinAddingBoxSetSvc*)GetService();
	CCoinAddingBoxSetSvc::Model& model = pSvc->GetDataModel();	
	if(FALSE == model.CodeContent )		//代码内容非法
	{
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_STATION_SET_INPUT_ERROR));
		return;
	}
	switch (model.MenuNumber)
	{
	case MENU_CODE_1:		//补充箱A
		m_labelGroup[0].nameCenter = model.hoponecurrency;
		break;
	case MENU_CODE_2:		//补充箱B
		m_labelGroup[1].nameCenter = model.hoptwocurrency;
		break;
	default:
		break;
	}
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_ENTER_F2_F3_ESC));
}