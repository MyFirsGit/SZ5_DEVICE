#include "stdafx.h"
#include "THTestSvc.h"
#include "THTestMaintainDlg.h"
#include "GuideInfo.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSvc::CTHTestSvc()
	:CBOMForeService(TH_TEST_SVC)
{
	// ���ò���Ա����
	m_pDialogFlow->AddDialog(IDD_TH_TEST_DLG, new CTHTestMaintainDlg(this));

	//����TH���Ի�����
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_TH_TEST, theGUIDE_INFO.GetMaintenanceMessage(GUIDE_TH_TEST_CHOOSE_ITEM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_TH_TEST, IDD_TH_TEST_DLG);

	//���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_TH_TEST);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      none

@retval     void

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSvc::~CTHTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ���������ģ��

@param      none

@retval     Model	��������ģ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CTHTestSvc::Model& CTHTestSvc::GetDataModel()
{
	return m_DataModel;
}