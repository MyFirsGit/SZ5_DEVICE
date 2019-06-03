#include "stdafx.h"
#include "SelectLanguageSvc.h"
#include "SelectLanguageBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSelectLanguageBaseDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CSelectLanguageBaseDlg, COperationDlg)
    ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageBaseDlg::CSelectLanguageBaseDlg(CService* pService)
	: COperationDlg(CSelectLanguageBaseDlg::IDD,pService)
{
	m_baseInfo->detailInfo.labelGroup = m_lableGroup;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageBaseDlg::~CSelectLanguageBaseDlg()
{
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:创建成功    -1:创建失败

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CSelectLanguageBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)) {
        return -1;
    }

	// 设置GStatic
	m_baseInfoArea->SetLableWidth(0, 400);
	m_baseInfoArea->SetLableWidth(1, 200);
	m_baseInfoArea->SetLableAlign(0, DT_VCENTER);
	m_baseInfoArea->SetLableAlign(1, DT_VCENTER);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageBaseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	// 开启指定按键
	theAPP_SESSION.SetKeyboard(KEY_INPUT);
	theAPP_SESSION.SetMaxInputLength(1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		服务数据模型发生变化时的更新界面   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageBaseDlg::UpdateUI()
{
	ShowData();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		显示数据   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageBaseDlg::ShowData()
{
	m_lableGroup[0].nameLeft = _T("[1] 简体中文");
	m_lableGroup[1].nameLeft = _T("[2] English");
//	m_lableGroup[2].nameLeft = _T("[3] Português");
}