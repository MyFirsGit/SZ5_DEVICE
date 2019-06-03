#include "stdafx.h"
#include "CounteractConfirmOfficeDlg.h"
#include "CounteractSvc.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CCounteractConfirmOfficeDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CCounteractConfirmOfficeDlg, COperationDlg)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCounteractConfirmOfficeDlg::CCounteractConfirmOfficeDlg(CService* pService) :
	COperationDlg(CCounteractConfirmOfficeDlg::IDD, pService)
{
	CCounteractSvc* pCounteractSvc = (CCounteractSvc*)GetService();	
	m_baseInfo->titleInfo.titleName = TITLE_TICKET_INFO;								// ����
	m_baseInfo->detailInfo.row = pCounteractSvc->GetCounteractConfirmInfoSize();							// ����
	m_baseInfo->detailInfo.labelGroup = pCounteractSvc->GetCounteractConfirmInfo();		// ��ǩ���֣�2�У�
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCounteractConfirmOfficeDlg::~CCounteractConfirmOfficeDlg()
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
int CCounteractConfirmOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ����������Ϣ���򣬷��ؼ���ȷ�ϼ�
	if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA|BASE_AREA  | BTN_OK)){
		return -1;
	}
    return 0;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϡ�����,��ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n
			TRUE:����ɹ�  FALSE:����ʧ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  CCounteractConfirmOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CCounteractSvc* pCounteractSvc = (CCounteractSvc*)GetService();
	pCounteractSvc->OnOK();
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractConfirmOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CCounteractSvc* pCounteractSvc = (CCounteractSvc*)GetService();
	if (pCounteractSvc->GetCurDialogGroupID()==CCounteractSvc::DIALOG_GROUP_ERROR) {
		HideCompoment(BASE_AREA);
		HideCompoment(BTN_OK);
	}
	else if (pCounteractSvc->GetCurDialogGroupID()==CCounteractSvc::DIALOG_GROUP_CONFIRM) {
		ShowCompoment(BASE_AREA);
		ShowCompoment(BTN_OK);
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ػ������������

@param      ��

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//void CCounteractConfirmOfficeDlg::HideAllComponent()
//{
//    HideCompoment(BTN_BACK | BTN_OK | BTN_PRINT | BASE_AREA | CASH_AREA | EDIT_BOX);
//}
