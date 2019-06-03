#include "stdafx.h"
#include "CommunicationSetSvc.h"
#include "CommunicationSetMaintainDlg.h"
#include "GuideInfo.h"

BEGIN_MESSAGE_MAP(CCommunicationSetSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::CCommunicationSetSvc() : CBOMForeService(COMMUNICATION_SET_SVC)
{
	//��ӶԻ������
	m_pDialogFlow->AddDialog(IDD_20114_COMMUNICATION_SET_DLG,new CCommunicationSetMaintainDlg(this));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_COMMUNICATION_SET,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_COMMUNICATION_SET_INFO_INPUT));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_COMMUNICATION_SET, IDD_20114_COMMUNICATION_SET_DLG);

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_COMMUNICATION_SET);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCommunicationSetSvc::~CCommunicationSetSvc()
{
}