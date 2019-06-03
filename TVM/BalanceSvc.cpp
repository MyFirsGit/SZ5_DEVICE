#include "StdAfx.h"
#include "BalanceSvc.h"
#include "BalanceDlg.h"

BEGIN_MESSAGE_MAP(CBalanceSvc,CTVMForeService)

END_MESSAGE_MAP()


CBalanceSvc::CBalanceSvc(void)
	:CTVMForeService(OPERATION_CLOSE_SVC)
{
	m_pDialogFlow->AddDialog(CBalanceDlg::IDD, new CBalanceDlg(this));

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER_F6_ESC));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CBalanceDlg::IDD);

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


CBalanceSvc::~CBalanceSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBalanceSvc::OnStart()
{
	__super::OnStart();
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
	theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_F2_F6_ESC_REPARE));
	//theAPP_SESSION.ShowOfficeGuide(TXT_BALANCE_IN_PROCESS);
	//CBalanceDlg* pDlg = dynamic_cast<CBalanceDlg*>(m_pDialogFlow->GetDialog(CBalanceDlg::IDD));
	//pDlg->Initialize();
	//NotifyDataModelChanged();
	//CAuditHelper *audit = new CAuditHelper(*this);
	//try
	//{
		//if(0 == audit->PrintBalanceBill())
		//{
		//	ShutDown(); // �ػ�����
		//}
		//audit->PrintBalanceBill();
		// �ػ�ʱ����ӡƾ֤
	//	ShutDown();
	//}
	//catch(...)
	//{
	//	ShutDown();
	//}

	//delete audit;
	//audit = NULL;
}


void CBalanceSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//��ȡ����ģ��
CBalanceSvc::DATA_MODEL CBalanceSvc::GetDataModel()
{
	return model;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief		�ػ�������  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBalanceSvc::ShutDown()
{
	theSERVICE_MGR.SetForeServiceBusy(true);
	/*theAPP_SESSION.SetCloseStartFlag(CS_SHUTDOWN);
	theTVM_STATUS_MGR.SetRunStatus(SHUTDOWNING);
	theSERVICE_MGR.GetService(MAIN_SVC)->PostMessage(SM_MAIN_SHUT_DOWN,NULL,NULL);*/
	theAPP_SESSION.SetManualCloseOperation(true);
	// �߹ػ�ʱ��
	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->CloseOperation();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ҵ�����ѡ��

@param      WPARAM , LPARAM����ʹ�ã�

@retval     LRESULT ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBalanceSvc::OnKeyboardF2(WPARAM , LPARAM){
	theAPP_SESSION.ShowOfficeGuide(TXT_BALANCE_IN_PROCESS);
	ShutDown();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ص����˵�

@param      WPARAM , LPARAM����ʹ�ã�

@retval     LRESULT ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBalanceSvc::OnKeyboardF3(WPARAM , LPARAM){
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}