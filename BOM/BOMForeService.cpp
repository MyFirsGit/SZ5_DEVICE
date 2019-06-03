#include "stdafx.h"
#include "BOMForeService.h"
#include "MessageIDDefinition.h"
#include "ExceptionMgr.h"
#include "InnerException.h"
#include "CursorMgr.h"

BEGIN_MESSAGE_MAP(CBOMForeService,CForeService)
	ON_COMMAND_COMPLETE(RW_COMMAND_ID_READ_CARD,OnReadCardEx)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CBOMForeService::CBOMForeService(SERVICE_ID serviceID, SERVICE_ID parentServiceID)
:CForeService(serviceID,parentServiceID)
{
	m_pDialogFlow = new CBOMDialogFlow(*this);
	 m_pRWHelper = new CRWHelper(*this);                 // ��д��Helper
	 m_pTWHelper = new CTWHelper(*this);
	 m_pPRTHelper = new PRTHelper(*this);
	 m_pTHHelper = new CTHHelper(*this);                 // ��Ʊ��Helper
	 m_LPTHelper = new CLPTHelper(*this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CBOMForeService::~CBOMForeService()
{
	delete m_pDialogFlow;
	delete m_pRWHelper;
	delete m_pPRTHelper;
	delete m_pTHHelper;
	delete m_LPTHelper;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Service��������

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnStart()
{
	try {
		theCURSOR_MGR.EndWaitCursor();
		m_pDialogFlow->ShowFirstDialogGroup();
		// ���ظ�Service��Dialog
		if (GetServiceID() != MAIN_SVC && GetParentServiceID() != MAIN_SVC) {
			((CBOMForeService*)GetParent())->HideCurDialogGroup();
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Serviceֹͣǰ����

@param      ��

@retval     ��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CBOMForeService::OnStop()
{
	try {
		// ���ظ�Service�ĵ�ǰDialog��
		if (GetServiceID() != MAIN_SVC && GetParentServiceID() != MAIN_SVC) {
			((CBOMForeService*)GetParent())->ShowCurDialogGroup();
		}
		return true;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CBOMDialogFlow* CBOMForeService::GetDialogFlow()
{
	return (CBOMDialogFlow*)m_pDialogFlow;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������ת��ť��Ϣ

@param      (i)const char* pButtonMsg ��ť���·��͵ĸ�����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::DoDialogFlow(const TCHAR* pButtonMsg)
{
	if (m_pDialogFlow->IsFlowButton(pButtonMsg)) {
		m_pDialogFlow->ShowNextDialogGroup(pButtonMsg);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ǰDialog����

@param      ��

@retval     UINT    ��ǰDialog����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CBOMForeService::GetCurDialogGroupID()
{
	return m_pDialogFlow->GetCurDialogGroupID();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg* CBOMForeService::GetCurOperationDlg()
{
	return GetDialogFlow()->GetCurOperationDlg();
}												 

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡǰһ��Dialog����

@param      ��

@retval     UINT    ǰһ��Dialog����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CBOMForeService::GetPreDialogGroupID()
{
	return m_pDialogFlow->GetPreDialogGroupID();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ��ǰDialog��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::ShowCurDialogGroup()
{
	m_pDialogFlow->ShowDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ص�ǰDialog��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::HideCurDialogGroup()
{
	m_pDialogFlow->HideDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ���������ݶ������仯��֪ͨ������� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::NotifyDataModelChanged()
{

	((CBOMDialogFlow*)m_pDialogFlow)->NotifyCurrentDialogGroupUpdate();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ģʽ�仯��Ӧ����

@param      (i)UINT mode    ģʽ���ࣨSP_MODE_SYSTEM������

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnModeChange(UINT mode)
{
	try {
		// ��ģʽ�䶯��Ϣת������ǰվԱDialog
		COperationDlg* pCurOperationDlg = GetDialogFlow()->GetCurOperationDlg();
		if (pCurOperationDlg != NULL) {
			pCurOperationDlg->SendMessage(SP_MODE_CHANGE, mode, NULL);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnBusy(bool busy)
{
	if(busy) {
		theCURSOR_MGR.BeginWaitCursor();
	}else
	{
		theCURSOR_MGR.EndWaitCursor();
	}

	COperationDlg* pOperationDlg = GetDialogFlow()->GetCurOperationDlg();
	if (pOperationDlg != NULL) {
		pOperationDlg->OnServiceBusy(busy);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡƴ�Ӻ������

@param      (i)CString resultMsg     Ӧ�����
@param      (i)CString resultGuide   ������Ϣ���� 

@retval     CString ƴ�Ӻ�����ԣ����ص��ַ��������������û����Ĭ�����ԣ�

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//CString CBOMForeService::GetCompleteGuide(ResultCode_t resultCode,OPERATION_MSG operationMsg)
//{
//	CString resultMsg;
//	CString resultGuide;
//	resultMsg   = theBOM_INFO.GetResultCodeMessage(resultCode).Trim();
//	resultGuide = theBOM_INFO.GetOperationMessage(operationMsg).Trim();
//	resultGuide.Replace("�أأأأأ�", resultMsg);
//	return resultGuide;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ö�д��Helper

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CRWHelper* CBOMForeService::GetRWHelper()
{
	return m_pRWHelper;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ���ڲ���д��

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTWHelper* CBOMForeService::GetTWHelper()
{
	return m_pTWHelper;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�ô�ӡ��Helper

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
PRTHelper* CBOMForeService::GetPRTHelper()
{
	return m_pPRTHelper;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��UPSHelper

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CUPSHelper* CBOMForeService::GetUPSHelper()
{
	return m_UPSHelper;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��THHelper

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTHHelper* CBOMForeService::GetTHHelper()
{
	return m_pTHHelper;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����ص�����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnReadCardEx(UINT result, CCommand * command)
{
	if(theSERVICE_MGR.GetState()!=MAINTENANCE)
	{
		theSERVICE_MGR.SetState(RECEPT);	
	}else
	{
		theSERVICE_MGR.SetForeServiceBusy(false);
	}
	CRWReadCard* rwReadCard = dynamic_cast<CRWReadCard*>(command);
	if(m_pRWHelper->IsExceptionCode(result))
	{
		theEXCEPTION_MGR.ProcessException(CRWHardwareException(result, _T(__FILE__), __LINE__));
		return;
	}
	if(rwReadCard!=NULL)
	{	
		OnReadCard(result,rwReadCard);		
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ����ʵ�ֺ���   

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnReadCard(UINT, CRWReadCard *)
{

}