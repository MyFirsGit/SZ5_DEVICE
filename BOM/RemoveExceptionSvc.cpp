#include "stdafx.h"
#include "RemoveExceptionSvc.h"
#include "RemoveExceptionDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRemoveExceptionSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::CRemoveExceptionSvc():CBOMForeService(REMOVE_EXCEPTION_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_20701_REMOVE_EXCEPTION_DLG, new CRemoveExceptionDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_REMOVE_EXCEPTION));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_REMOVE_EXCEPTION, IDD_20701_REMOVE_EXCEPTION_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_REMOVE_EXCEPTION);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRemoveExceptionSvc::~CRemoveExceptionSvc(void)
{
}