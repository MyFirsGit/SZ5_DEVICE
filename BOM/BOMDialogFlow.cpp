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
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBOMDialogFlow::CBOMDialogFlow(CService& service):CDialogFlow(service)
{
	theSERVICE_MGR.StateChanged.AddHandler(this,&CBOMDialogFlow::OnServiceStateChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBOMDialogFlow::~CBOMDialogFlow()
{
	theSERVICE_MGR.StateChanged.RemoveHandler(this,&CBOMDialogFlow::OnServiceStateChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加Dialog

@param      (i) UINT dlgID          Dialog ID
@param      (i) CBaseDlg* pDlg      Dialog

@retval     无

@exception  无
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
@brief      显示Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::ShowDialog(UINT dlgID)
{
	try {
		CBaseDlg* pDlg = (CBaseDlg*)GetDialog(dlgID);

		// 不处理Welcome dialog (与Out of service统一管理)
		if (pDlg->IsKindOf(RUNTIME_CLASS(CWelcomeReceptionDlg))) {
			return;
		}
		if(pDlg->IsKindOf(RUNTIME_CLASS(CReceptionDlg)) && theSERVICE_MGR.GetState() == READY)
		{
			return;
		}
		// 创建Dialog
		if (pDlg->GetSafeHwnd() == NULL) {
			if (pDlg->IsOfficeDlg()) {
				pDlg->Create(dlgID, theAPP_SESSION.GetChildView());
			}
			else {
				pDlg->Create(dlgID, theAPP_SESSION.GetChildViewRec());
			}
		}

		// 显示Dialog
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
@brief      隐藏Dialog

@param      (i) UINT dlgID          Dialog ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::HideDialog(UINT dlgID)
{
	CBaseDlg* pDlg = (CBaseDlg*)GetDialog(dlgID);

	// 不处理Welcome dialog (与Out of service统一管理)
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
@brief      隐藏对话框组

@param      (i)UINT dlgGroupID                     对话框组ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::HideDialogGroup(UINT dlgGroupID)
{
	__super::HideDialogGroup(dlgGroupID);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      显示对话框组

@param      (i)UINT dlgGroupID                     对话框组ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBOMDialogFlow::ShowDialogGroup(UINT dlgGroupID)
{
	if (HasWelcomeReceptionDlg(dlgGroupID)) {   // 存在“欢迎光临”乘客画面时
		// 显示“欢迎光临”画面
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(COutOfReceptionDlg::WELCOME);
	}
	else if (HasReceptionDlg(dlgGroupID)) {     // 存在普通乘客画面时
	}
	else {
		// 显示“终止服务”画面
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(COutOfReceptionDlg::OUT_OF_SERVICE);
	}

	CDialogGroup* pDlgGroup = GetDialogGroup(dlgGroupID);

	// 显示向导标题
	theAPP_SESSION.ShowAllGuide(pDlgGroup->m_pGuideMsg, pDlgGroup->m_pRecpGuideMsg, pDlgGroup->m_pRecpGuideMsgEn);

	// 显示组中的所有Dialog
	for (int i = 0; i < pDlgGroup->m_dlgList.size(); i++) {
		ShowDialog(pDlgGroup->m_dlgList[i]);		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前站员Dialog

@param      无

@retval     COperationDlg*  当前站员Dialog指针，不存在时返回NULL

@exception  无
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
@brief      通知当前画面组更新

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
@brief      是否有普通乘客画面（“终止服务”、“欢迎光临”除外）

@param      (i)UINT dlgGroupID          对话框组ID

@retval     bool true:有乘客画面, false:无乘客画面

@exception  无
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
@brief      是否有“Welcome”乘客画面

@param      (i)UINT dlgGroupID          对话框组ID

@retval     bool true:有Welcome乘客画面, false:无Welcome乘客画面

@exception  无
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
	// 显示组中的所有Dialog
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
		// 显示“欢迎光临”画面
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Show(COutOfReceptionDlg::WELCOME);
	}
}