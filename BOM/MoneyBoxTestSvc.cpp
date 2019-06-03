#include "stdafx.h"
#include "MoneyBoxTestSvc.h"
#include "bominfo.h"
#include "MoneyBoxTestMaintainDlg.h"
#include "guideinfo.h"

BEGIN_MESSAGE_MAP(CMoneyBoxTestSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestSvc::CMoneyBoxTestSvc()
:CBOMForeService(MONEYBOX_TEST_SVC)
{
	GetDialogFlow()->AddDialog(IDD_20541_MONEYBOX_TEST_DLG,new CMoneyBoxTestMaintainDlg(this));
	GetDialogFlow()->AddDialogGroup(DIALOG_GROUP_MONEYBOX, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_CHOOSE_ITEM));
	GetDialogFlow()->AddDialogGroupMember(DIALOG_GROUP_MONEYBOX,IDD_20541_MONEYBOX_TEST_DLG);

	// ���ó�ʼ������
	GetDialogFlow()->SetFirstDialogGroup(DIALOG_GROUP_MONEYBOX);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMoneyBoxTestSvc::~CMoneyBoxTestSvc()
{

}

void CMoneyBoxTestSvc::OnOK()
{
	// ���öԻ���æ
	theSERVICE_MGR.SetForeServiceBusy(true);
	// ��ʾ����Ϣ��������Ǯ�����
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_CHOOSE_ITEM));
	// �رմ�ӡ��
	GetPRTHelper()->PrinterClose();
	// ���Ӵ�ӡ��
	GetPRTHelper()->PrinterConnect(theBOM_INFO.GetPrinterCommPort(), theBOM_INFO.GetPrinterBaudRate());
	// ��Ǯ��
	GetPRTHelper()->PrinterOpenBox();	

	// ȡ���Ի���æ
	theSERVICE_MGR.SetForeServiceBusy(false);
	// ��ʾ����Ϣ����Ǯ��������
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_CASH_BOX_TEST_FINISH));

	
}
