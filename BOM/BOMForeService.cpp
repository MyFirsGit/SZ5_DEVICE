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
	 m_pRWHelper = new CRWHelper(*this);                 // 读写器Helper
	 m_pTWHelper = new CTWHelper(*this);
	 m_pPRTHelper = new PRTHelper(*this);
	 m_pTHHelper = new CTHHelper(*this);                 // 出票机Helper
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
@brief      Service启动后处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnStart()
{
	try {
		theCURSOR_MGR.EndWaitCursor();
		m_pDialogFlow->ShowFirstDialogGroup();
		// 隐藏父Service的Dialog
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
@brief      Service停止前处理

@param      无

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CBOMForeService::OnStop()
{
	try {
		// 隐藏父Service的当前Dialog组
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
@brief      处理画面跳转按钮消息

@param      (i)const char* pButtonMsg 按钮按下发送的附加消息

@retval     无

@exception  无
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
@brief      获取当前Dialog组编号

@param      无

@retval     UINT    当前Dialog组编号

@exception  无
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
@brief      获取前一组Dialog组编号

@param      无

@retval     UINT    前一组Dialog组编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CBOMForeService::GetPreDialogGroupID()
{
	return m_pDialogFlow->GetPreDialogGroupID();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示当前Dialog组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::ShowCurDialogGroup()
{
	m_pDialogFlow->ShowDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏当前Dialog组

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::HideCurDialogGroup()
{
	m_pDialogFlow->HideDialogGroup(GetCurDialogGroupID());
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

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
@brief      模式变化响应函数

@param      (i)UINT mode    模式种类（SP_MODE_SYSTEM，，）

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnModeChange(UINT mode)
{
	try {
		// 把模式变动消息转发到当前站员Dialog
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
@brief      获取拼接后的文言

@param      (i)CString resultMsg     应答代码
@param      (i)CString resultGuide   操作信息代码 

@retval     CString 拼接后的文言（返回的字符串不能用于设置画面的默认文言）

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
//CString CBOMForeService::GetCompleteGuide(ResultCode_t resultCode,OPERATION_MSG operationMsg)
//{
//	CString resultMsg;
//	CString resultGuide;
//	resultMsg   = theBOM_INFO.GetResultCodeMessage(resultCode).Trim();
//	resultGuide = theBOM_INFO.GetOperationMessage(operationMsg).Trim();
//	resultGuide.Replace("ＸＸＸＸＸＸ", resultMsg);
//	return resultGuide;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得读写器Helper

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
@brief      取得内部读写器

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
@brief      取得打印机Helper

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
@brief      取得UPSHelper

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
@brief      取得THHelper

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
@brief      读卡回调函数

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
@brief   读卡实现函数   

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBOMForeService::OnReadCard(UINT, CRWReadCard *)
{

}