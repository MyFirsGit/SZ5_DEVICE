#include "stdafx.h"
#include "SystemSetSvc.h"
#include "AppSession.h"
#include "service.h"
#include "shlwapi.h"
#include "bominfo.h"
#include "WelcomeDlg.h"
#include "guideinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSystemSetSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSystemSetSvc::CSystemSetSvc()
:CBOMForeService(SYSTEM_SET_SVC)
{

	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_WELCOME_DLG, new CWelcomeDlg(this));

	// ���û������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_SYSTEM_SET, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_SYSTEM_SET));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_SYSTEM_SET, IDD_WELCOME_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_SYSTEM_SET);

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      void

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSystemSetSvc::~CSystemSetSvc()
{
}









