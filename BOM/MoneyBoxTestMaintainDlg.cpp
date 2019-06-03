#include "stdafx.h"
#include "BOM.h"
#include "MoneyBoxTestMaintainDlg.h"
#include "guideinfo.h"
#include "MoneyBoxTestSvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMoneyBoxTestMaintainDlg,COperationDlg)

BEGIN_MESSAGE_MAP(CMoneyBoxTestMaintainDlg,COperationDlg)
	ON_WM_CREATE()
	ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService*    pService    �Ի���ʹ�õ�Service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestMaintainDlg::CMoneyBoxTestMaintainDlg(CService* pService)
: COperationDlg(CMoneyBoxTestMaintainDlg::IDD, pService)
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestMaintainDlg::~CMoneyBoxTestMaintainDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      OnCreate

@param      (i)LPCREATESTRUCT lpCreateStruct ���崴��������

@retval     int		\n
0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CMoneyBoxTestMaintainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// ��ʾBOM���ԣ��밴ȷ�ϼ���ʼ����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_CHOOSE_ITEM));

	return __super::OnCreate(lpCreateStruct,   GUIDE_AREA | BTN_OK);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ�ϰ�ť��Ϣ����

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  \n

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CMoneyBoxTestMaintainDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	//// ��ȡService
	CMoneyBoxTestSvc* pMoneyBoxTestSvc = (CMoneyBoxTestSvc*)GetService();
	pMoneyBoxTestSvc->OnOK();

	HideCompoment(BTN_OK);

	return 0;
}