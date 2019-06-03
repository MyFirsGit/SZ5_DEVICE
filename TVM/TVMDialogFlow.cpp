#include "stdafx.h"
#include "TVMDialogFlow.h"
#include "InnerException.h"
#include "AppSession.h"
#include "WelcomeReceptionDlg.h"
#include "OutOfReceptionDlg.h"
#include "ReceptionDlg.h"


/**
@brief	CTVMDialogFlow 
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTVMDialogFlow::CTVMDialogFlow(CService& service):CDialogFlow(service)
{
	theSERVICE_MGR.StateChanged.AddHandler(this,&CTVMDialogFlow::OnServiceStateChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTVMDialogFlow::~CTVMDialogFlow()
{
	theSERVICE_MGR.StateChanged.RemoveHandler(this,&CTVMDialogFlow::OnServiceStateChanged);
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
void CTVMDialogFlow::AddDialog(UINT dlgID, CBaseDlg* pDlg)
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
void CTVMDialogFlow::ShowDialog(UINT dlgID)
{
	try {
		CBaseDlg* pDlg = (CBaseDlg*)GetDialog(dlgID);
		// ����Dialog
		if (pDlg->GetSafeHwnd() == NULL) {
			if (pDlg->IsOfficeDlg()) {
				pDlg->Create(dlgID, theAPP_SESSION.GetChildView());
			}
			else {
				pDlg->Create(dlgID, theAPP_SESSION.GetChildViewRec());
			}
		}

		pDlg->preShow();
		__super::ShowDialog(dlgID);
		//pDlg->ShowWindow(SW_SHOW);
		//pDlg->SendMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
		//pDlg->PostMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
		pDlg->PostMessage(WM_NOTIFY_DIALOG_UPDATE,NULL,NULL);
		pDlg->showComplete();

		// ��ʾDialog

		//pDlg->ShowWindow(SW_SHOW);
		//pDlg->PostMessage(WM_ACTIVATE, WA_ACTIVE, NULL);
		//pDlg->PostMessage(WM_NOTIFY_DIALOG_UPDATE,NULL,NULL);

		//pDlg->SendMessage(WM_ACTIVATE, WA_ACTIVE, NULL);	//��ܵ�֮�� ��ʾ������Ӧ����
		//pDlg->ShowWindow(SW_SHOW);
		//pDlg->PostMessage(WM_NOTIFY_DIALOG_UPDATE,NULL,NULL);
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(m_service.GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

CBaseDlg* CTVMDialogFlow::GetDialog(UINT dlgID)
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
void CTVMDialogFlow::HideDialog(UINT dlgID)
{
	CBaseDlg* pDlg = (CBaseDlg*)GetDialog(dlgID);

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
void CTVMDialogFlow::HideDialogGroup(UINT dlgGroupID)
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
void CTVMDialogFlow::ShowDialogGroup(UINT dlgGroupID)
{ 
	// û�д�����ͨ�˿ͻ���ʱ
	if (!HasReceptionDlg(dlgGroupID)) {    
		// ��ʾ����ֹ���񡱻���
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(/*COutOfReceptionDlg::OUT_OF_SERVICE*/);
	}

	CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);

	// ��ʾ�򵼱���
	theAPP_SESSION.ShowAllGuide(pDlgGroup->m_pGuideMsg, pDlgGroup->m_pRecpGuideMsg, pDlgGroup->m_pRecpGuideMsgEn, pDlgGroup->m_pRecpGuideMsgPn);

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
COperationDlg* CTVMDialogFlow::GetCurOperationDlg()
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
		pDlg = NULL;
	}

	return (COperationDlg*)pDlg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰ�˿�Dialog

@param      ��

@retval     CReceptionDlg*  ��ǰ�˿�Dialogָ�룬������ʱ����NULL

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg* CTVMDialogFlow::GetCurReceptionDlg()
{
	if (m_curDlgGroupID == -1) {
		return NULL;
	}

	CDialogGroup* pDlgGroup = GetDialogGroup(m_curDlgGroupID);

	CBaseDlg* pDlg = NULL;

	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		pDlg = GetDialog(pDlgGroup->m_dlgList[i]);

		if (pDlg->IsKindOf(RUNTIME_CLASS(CReceptionDlg))) {
			break;
		}
		pDlg = NULL;
	}

	return (CReceptionDlg*)pDlg;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ֪ͨ��ǰ���������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTVMDialogFlow::NotifyCurrentDialogGroupUpdate()
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
bool CTVMDialogFlow::HasReceptionDlg(UINT dlgGroupID)
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
bool CTVMDialogFlow::HasWelcomeReceptionDlg(UINT dlgGroupID)
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


void CTVMDialogFlow::OnServiceStateChanged(SERVICE_STATE oldState,SERVICE_STATE newState)
{
	//CDialogGroup* pDlgGroup = GetDialogGroup(m_curDlgGroupID);
	//// ��ʾ���е�����Dialog
	//for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
	//	CBaseDlg* pDlg = (CBaseDlg*)GetDialog(pDlgGroup->m_dlgList[i]);
	//	if(pDlg->IsKindOf(RUNTIME_CLASS(CMenuDlg)))
	//	{
	//		if(newState!=READY)
	//		{
	//			HideDialog(pDlgGroup->m_dlgList[i]);
	//		}
	//		else
	//		{
	//			ShowDialog(pDlgGroup->m_dlgList[i]);
	//		}
	//		break;
	//	}				
	//}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ��С�Welcome���˿ͻ���

@param      (i)UINT dlgGroupID          �Ի�����ID

@retval     bool true:��Welcome�˿ͻ���, false:��Welcome�˿ͻ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const TCHAR* CTVMDialogFlow::GetCurrentGuideMsg()
{
	UINT id = GetCurDialogGroupID();
	CDialogGroup* group = GetDialogGroup(id);
	return group->m_pGuideMsg;
}