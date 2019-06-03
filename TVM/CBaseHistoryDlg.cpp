// CBaseHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TVM.h"
#include "CBaseHistoryDlg.h"
#include "afxdialogex.h"
#include "AnalyzeSvc.h"


// CBaseHistoryDlg dialog

IMPLEMENT_DYNAMIC(CBaseHistoryDlg, CReceptionDlg)
BEGIN_MESSAGE_MAP(CBaseHistoryDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

CBaseHistoryDlg::CBaseHistoryDlg(CService* pService)
	: CReceptionDlg(CBaseHistoryDlg::IDD, pService)
{

}

CBaseHistoryDlg::~CBaseHistoryDlg()
{
}

void CBaseHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面创建函数
//
//@param      (i)LPCREATESTRUCT lpCreateStruct  
//
//@retval     int    \n
//             0:创建成功    -1:创建失败
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
int CBaseHistoryDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CAnalyzeSvc* pAnalyzeSvc = (CAnalyzeSvc*)GetService();
    try{    		
		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA ) == -1) {
			return -1;
		}
    }
    // 异常处理
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
         theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////
///*
//@brief      更新画面函数
//
//@param      无
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseHistoryDlg::UpdateUI()
{
	m_baseInfoArea->Invalidate();
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面激活后的处理
//
//@param      (i)UINT nState      不使用
//@param      (i)CWnd* pWndOther  不使用
//@param      (i)BOOL bMinimized  不使用
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseHistoryDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{	
	__super::OnActivate(nState,pWndOther,bMinimized);
}

// CBaseHistoryDlg message handlers
