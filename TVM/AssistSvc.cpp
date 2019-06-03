#include "StdAfx.h"
#include "AssistSvc.h"
#include "assistdlg.h"

BEGIN_MESSAGE_MAP(CAssistSvc, CTVMForeService)

END_MESSAGE_MAP()

static const TCHAR* const WM_ANALYZE_BASE_TO_ONE = _T("WM_ANALYZE_BASE_TO_ONE");
static const TCHAR* const WM_ANALYZE_ONE_TO_BASE = _T("WM_ANALYZE_ONE_TO_BASE");

CAssistSvc::CAssistSvc(void)
	:CTVMForeService(CALL_HELP_SVC)
{
	//CAssistDlg *pDlg = new CAssistDlg(this);
	//
	m_pDialogFlow->AddDialog(CAssistDlg::IDD, new CAssistDlg(this));

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10805_ANALYZE_BASEM1_DLG);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ONE);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ONE, IDD_00303_CHARGE_RESULT_DLG);


	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_ANALYZE_BASE_TO_ONE, DIALOG_GROUP_ONE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_ONE, WM_ANALYZE_ONE_TO_BASE, DIALOG_GROUP_BASE);

	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);

}


CAssistSvc::~CAssistSvc(void)
{
}


void CAssistSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}