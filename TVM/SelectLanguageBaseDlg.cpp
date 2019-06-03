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
@brief      ���캯��

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
@brief      ��������

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
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    0:�����ɹ�    -1:����ʧ��

@exception  CSysException
*/
////////////////////////////////////////////////////////////////////////////
int CSelectLanguageBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BASE_AREA)) {
        return -1;
    }

	// ����GStatic
	m_baseInfoArea->SetLableWidth(0, 400);
	m_baseInfoArea->SetLableWidth(1, 200);
	m_baseInfoArea->SetLableAlign(0, DT_VCENTER);
	m_baseInfoArea->SetLableAlign(1, DT_VCENTER);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageBaseDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	// ����ָ������
	theAPP_SESSION.SetKeyboard(KEY_INPUT);
	theAPP_SESSION.SetMaxInputLength(1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������ģ�ͷ����仯ʱ�ĸ��½���   

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
@brief		��ʾ����   

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageBaseDlg::ShowData()
{
	m_lableGroup[0].nameLeft = _T("[1] ��������");
	m_lableGroup[1].nameLeft = _T("[2] English");
//	m_lableGroup[2].nameLeft = _T("[3] Portugu��s");
}