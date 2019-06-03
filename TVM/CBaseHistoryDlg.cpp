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
//@brief      ���洴������
//
//@param      (i)LPCREATESTRUCT lpCreateStruct  
//
//@retval     int    \n
//             0:�����ɹ�    -1:����ʧ��
//
//@exception  ��
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
    // �쳣����
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
//@brief      ���»��溯��
//
//@param      ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseHistoryDlg::UpdateUI()
{
	m_baseInfoArea->Invalidate();
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ���漤���Ĵ���
//
//@param      (i)UINT nState      ��ʹ��
//@param      (i)CWnd* pWndOther  ��ʹ��
//@param      (i)BOOL bMinimized  ��ʹ��
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CBaseHistoryDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{	
	__super::OnActivate(nState,pWndOther,bMinimized);
}

// CBaseHistoryDlg message handlers
