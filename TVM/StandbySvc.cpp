#include "stdafx.h"
#include "StandbySvc.h"
#include "StandbyDlg.h"


#include <map>

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CStandbySvc, CTVMForeService)
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStandbySvc::CStandbySvc():CTVMForeService(STANDBY_SVC)
{
	m_pDialogFlow->AddDialog(IDD_20901_SYSTEM_OUT_DLG,new CStandbyDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_STANDBY);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_STANDBY, IDD_20901_SYSTEM_OUT_DLG);
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STANDBY);	// ���ó�ʼ������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CStandbySvc::~CStandbySvc()
{
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CStandbySvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ӷ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CStandbySvc::OnStart(){
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_STANDBY);
	m_pDialogFlow->ShowFirstDialogGroup();
}

