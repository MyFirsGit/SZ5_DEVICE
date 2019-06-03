#include "stdafx.h"
#include "PassengerOperationLog.h"
#include "ServiceMgr.h"
#include "FunctionDlg.h"
#include "MenuDlg.h"
#include "MessageBox.h"


CPassengerOperationLog CPassengerOperationLog::theInstance;

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPassengerOperationLog::CPassengerOperationLog(void): CLog(FILTER_STAFFOPERATION_LOG)
{	
	CXButton::Clicking.AddHandler(this,&CPassengerOperationLog::OnButtonClicking);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPassengerOperationLog::~CPassengerOperationLog(void)
{	
	CXButton::Clicking.RemoveHandler(this,&CPassengerOperationLog::OnButtonClicking);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief		按钮点击事件处理函数      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerOperationLog::OnButtonClicking(CXButton* pButton,bool* pCancel)
{
	CString log(_T(""));
	CWnd* parent = pButton->GetParent();
	while(parent!=NULL && !parent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
	{
		parent = parent->GetParent();	
	}
	if(parent!=NULL)
	{
		CBaseDlg* baseDlg = (CBaseDlg*)parent;
		CTVMForeService*  pForeService = NULL;
		if(baseDlg->IsKindOf(RUNTIME_CLASS(CFunctionDlg)) || baseDlg->IsKindOf(RUNTIME_CLASS(CMenuDlg)) || baseDlg->IsKindOf(RUNTIME_CLASS(CMessageBox)) ||baseDlg->IsKindOf(RUNTIME_CLASS(CKeyboard)) ||baseDlg->IsKindOf(RUNTIME_CLASS(CMessagePictureBox)))
		{
			pForeService = (CTVMForeService*)theSERVICE_MGR.GetCurService();
		}
		else
		{
			pForeService = (CTVMForeService*)baseDlg->GetService();
			
		}
		UINT DialogGroupID = pForeService->GetCurDialogGroupID();
		log += _T("Service Name:") + theAPP_SESSION.GetServiceName((SERVICE_ID)pForeService->GetServiceID()) + _T(",");
		CString strCurDialogGroupID(_T(""));
		strCurDialogGroupID.Format(_T("%d"),pForeService->GetCurDialogGroupID());
		log += _T("Dialog Group ID:") + strCurDialogGroupID + _T(",");
		CString strDialogID(_T(""));
		strDialogID.Format(_T("%d"),baseDlg->GetID());
		log += _T("Dialog ID:") +strDialogID + _T(",");
		
	}
	UINT ID = pButton->GetID();
	CString strID;
	strID.Format(_T("%d"),ID);
	log+=_T("Button ID:") + strID + _T(",");
	log+=_T("Button Name:")+pButton->GetText();
	log.Replace(_T("\n"),_T(""));
	__super::WriteData(log);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      关闭乘客点击界面按钮日志

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerOperationLog::ReleasePassengerOperationLog(){
	theInstance.CloseLog();
};