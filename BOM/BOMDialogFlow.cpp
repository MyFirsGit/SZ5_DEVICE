#include "stdafx.h"
#include "BOMDialogFlow.h"
#include "InnerException.h"
#include "AppSession.h"
#include "WelcomeReceptionDlg.h"
#include "OutOfReceptionDlg.h"
#include "ReceptionDlg.h"


/**
@brief	CBOMDialogFlow 
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBOMDialogFlow::CBOMDialogFlow(CService& service):CDialogFlow(service)
{
	theSERVICE_MGR.StateChanged.AddHandler(this,&CBOMDialogFlow::OnServiceStateChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBOMDialogFlow::~CBOMDialogFlow()
{
	theSERVICE_MGR.StateChanged.RemoveHandler(this,&CBOMDialogFlow::OnServiceStateChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Dialog

@param      (i) UINT dlgID          Dialog ID
@param      (i) CBaseDlg* pDlg      Dialog

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::AddDialog(UINT dlgID, CBaseDlg* pDlg)
{
	__super::AddDialog(dlgID,pDlg);
	if (pDlg->IsOfficeDlg()) {
	    pDlg->Create(dlgID, theAPP_SESSION.GetChildView());
	}
	else {
	    pDlg->Create(dlgID, theAPP_SESSION.GetChildViewRec());
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾDialog

@param      (i) UINT dlgID          Dialog ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::ShowDialog(UINT dlgID)
{
	try {
		CBaseDlg* pDlg = (CBaseDlg*)GetDialog(dlgID);

		// ������Welcome dialog (��Out of serviceͳһ����)
		if (pDlg->IsKindOf(RUNTIME_CLASS(CWelcomeReceptionDlg))) {
			return;
		}
		if(pDlg->IsKindOf(RUNTIME_CLASS(CReceptionDlg)) && theSERVICE_MGR.GetState() == READY)
		{
			return;
		}
		// ����Dialog
		if (pDlg->GetSafeHwnd() == NULL) {
			if (pDlg->IsOfficeDlg()) {
				pDlg->Create(dlgID, theAPP_SESSION.GetChildView());
			}
			else {
				pDlg->Create(dlgID, theAPP_SESSION.GetChildViewRec());
			}
		}

		// ��ʾDialog
		pDlg->ShowWindow(SW_SHOW);
		//pDlg->UpdateWindow();
		pDlg->PostMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
		pDlg->PostMessage(WM_NOTIFY_DIALOG_UPDATE,NULL,NULL);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

CBaseDlg* CBOMDialogFlow::GetDialog(UINT dlgID)
{
	return (CBaseDlg*)__super::GetDialog(dlgID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::HideDialog(UINT dlgID)
{
	CBaseDlg* pDlg = (CBaseDlg*)GetDialog(dlgID);

	// ������Welcome dialog (��Out of serviceͳһ����)
	if (pDlg->IsKindOf(RUNTIME_CLASS(CWelcomeReceptionDlg))) {
		return;
	}
	if(IsWindow(pDlg->m_hWnd))
	{
		pDlg->ShowWindow(SW_HIDE);
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ضԻ�����

@param      (i)UINT dlgGroupID                     �Ի�����ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::HideDialogGroup(UINT dlgGroupID)
{
	__super::HideDialogGroup(dlgGroupID);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ�Ի�����

@param      (i)UINT dlgGroupID                     �Ի�����ID

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::ShowDialogGroup(UINT dlgGroupID)
{
	if (HasWelcomeReceptionDlg(dlgGroupID)) {   // ���ڡ���ӭ���١��˿ͻ���ʱ
		// ��ʾ����ӭ���١�����
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(COutOfReceptionDlg::WELCOME);
	}
	else if (HasReceptionDlg(dlgGroupID)) {     // ������ͨ�˿ͻ���ʱ
	}
	else {
		// ��ʾ����ֹ���񡱻���
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(COutOfReceptionDlg::OUT_OF_SERVICE);
	}

	CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);

	// ��ʾ�򵼱���
	theAPP_SESSION.ShowAllGuide(pDlgGroup->m_pGuideMsg, pDlgGroup->m_pRecpGuideMsg, pDlgGroup->m_pRecpGuideMsgEn);

	// ��ʾ���е�����Dialog
	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		ShowDialog(pDlgGroup->m_dlgList[i]);		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰվԱDialog

@param      ��

@retval     COperationDlg*  ��ǰվԱDialogָ�룬������ʱ����NULL

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg* CBOMDialogFlow::GetCurOperationDlg()
{
	if (m_curDlgGroupID == -1) {
		return NULL;
	}

	CDialogGroup* pDlgGroup = GetDialogGroup(m_curDlgGroupID);

	CBaseDlg* pDlg = NULL;

	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		pDlg = GetDialog(pDlgGroup->m_dlgList[i]);

		if (pDlg->IsKindOf(RUNTIME_CLASS(COperationDlg))) {
			break;
		}
	}

	return (COperationDlg*)pDlg;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ֪ͨ��ǰ���������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::NotifyCurrentDialogGroupUpdate()
{
	if (m_curDlgGroupID == -1) {
		return;
	}
	CDialogGroup* pDlgGroup = GetDialogGroup(m_curDlgGroupID);
	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		CBaseDlg* pDlg = GetDialog(pDlgGroup->m_dlgList[i]);
		pDlg->PostMessage(WM_NOTIFY_DIALOG_UPDATE,NULL,NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�����ͨ�˿ͻ��棨����ֹ���񡱡�����ӭ���١����⣩

@param      (i)UINT dlgGroupID          �Ի�����ID

@retval     bool true:�г˿ͻ���, false:�޳˿ͻ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBOMDialogFlow::HasReceptionDlg(UINT dlgGroupID)
{
	bool result = false;

	CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);

	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		CBaseDlg* pDlg = GetDialog(pDlgGroup->m_dlgList[i]);

		if (pDlg->IsKindOf(RUNTIME_CLASS(CReceptionDlg))) {
			result = true;
			break;
		}
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��С�Welcome���˿ͻ���

@param      (i)UINT dlgGroupID          �Ի�����ID

@retval     bool true:��Welcome�˿ͻ���, false:��Welcome�˿ͻ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBOMDialogFlow::HasWelcomeReceptionDlg(UINT dlgGroupID)
{
	bool result = false;

	CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);

	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		CBaseDlg* pDlg = GetDialog(pDlgGroup->m_dlgList[i]);

		if (pDlg->IsKindOf(RUNTIME_CLASS(CWelcomeReceptionDlg))) {
			result = true;
			break;
		}
	}

	return result;
}


void CBOMDialogFlow::OnServiceStateChanged(SERVICE_STATE oldState,SERVICE_STATE newState)
{
	CDialogGroup* pDlgGroup = GetDialogGroup(m_curDlgGroupID);
	// ��ʾ���е�����Dialog
	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		CBaseDlg* pDlg = (CBaseDlg*)GetDialog(pDlgGroup->m_dlgList[i]);
		if(pDlg->IsKindOf(RUNTIME_CLASS(CReceptionDlg)))
		{
			if(newState!=READY)
			{
				ShowDialog(pDlgGroup->m_dlgList[i]);
			}
			else
			{
				HideDialog(pDlgGroup->m_dlgList[i]);
			}
			break;
		}
				
	}

	if(newState == READY)
	{
		// ��ʾ����ӭ���١�����
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(COutOfReceptionDlg::WELCOME);
	}
}