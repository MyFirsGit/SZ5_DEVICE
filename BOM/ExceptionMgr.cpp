#include "StdAfx.h"
#include "exceptionmgr.h"
#include "SysException.h"
#include "InnerException.h"
#include "exceptionlog.h"
#include "ServiceMgr.h"
#include "MessageBox.h"
#include "commondefine.h"
#include "guideinfo.h"
#include <algorithm>
#include "RWMonitorSvc.h"
#include "TWMonitorSvc.h"
#include "THMonitorSvc.h"
#include "ftphelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

// 实例化CExceptionMgr的唯一对象
CExceptionMgr CExceptionMgr::theInstance;

const static TCHAR* EXCEPTION_SERVICE_STATE_KEY =_T("EXCEPTION_SERVICE_STATE");

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionMgr::CExceptionMgr()
{
	m_isBusy = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionMgr::~CExceptionMgr()
{
	m_exceptionArr.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取该类的对象

@param      无

@retval     CExceptionMgr&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExceptionMgr& CExceptionMgr::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      添加异常记录

@param      (i)CSysException& e
@param      (i)bool needSendStatus   是否需要发送状态电文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::AddException(CSysException& e,bool needSendStatus)
{
	try {
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		// 记录异常发生时的业务状态
		e.GetBag().SetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
		// 写异常日志
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
		m_exceptionArr.push_back(e);	
		ExceptionItemChanged.Invoke(e,true);
	}
	catch (CSysException& e) {
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		WriteExceptionLog(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__), CExceptionLog::LEVEL_ERROR);
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
unsigned long CExceptionMgr::GetMTC(CSysException& e) const
{
	CGlobalParameter::EXCEPTION_DETAIL exMsg;
	theGlobalParameter.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	return exMsg.lMTC;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
unsigned short CExceptionMgr::GetEC(CSysException& e) const
{
	CGlobalParameter::EXCEPTION_DETAIL exMsg;
	theGlobalParameter.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	return exMsg.lEc;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionMgr::GetActualMTC(CSysException& e) const
{
	CGlobalParameter::EXCEPTION_DETAIL exMsg;
	theGlobalParameter.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	SERVICE_STATE serviceState;
	e.GetBag().GetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
	return (exMsg.lMTC & 0xFF00FFFF) | ((serviceState << 16) & 0x00FF0000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
SERVICE_STATE CExceptionMgr::GetServiceState(CSysException& e) const
{
	SERVICE_STATE serviceState;
	e.GetBag().GetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
	return serviceState;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CExceptionMgr::GetDescription(CSysException& e) const
{
	CGlobalParameter::EXCEPTION_DETAIL exMsg;
	theGlobalParameter.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
    USES_CONVERSION;
	return A2T(exMsg.sErr);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CExceptionMgr::GetPrompt(CSysException& e) const
{
    CGlobalParameter::EXCEPTION_DETAIL exMsg;
	theGlobalParameter.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	USES_CONVERSION;
	return A2T(exMsg.sDeal);
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除异常记录

@param      (i)long m_moduleID    模块编码
@param      (i)long detailID      模块内详细编码

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::RemoveException(long moduleID, long detailID)
{
	try {
		vector<CSysException>::iterator iter = m_exceptionArr.begin();
		while (iter != m_exceptionArr.end()) {
			// 异常详细编码为NO_DETAIL_ID时，不使用详细编码
			if ((*iter).GetModuleID() == moduleID && (detailID == NO_DETAIL_ID
				|| detailID != NO_DETAIL_ID && (*iter).GetDetailID() == detailID)) {
					CSysException newException = *iter;
					iter = m_exceptionArr.erase(iter);
					ExceptionItemChanged.Invoke(newException,false);
			}
			else {
				iter++;
			}
		}
	}
	catch (CSysException& e) {
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		WriteExceptionLog(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__), CExceptionLog::LEVEL_ERROR);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除指定异常以外的模块内所有异常记录

@param      (i)long m_moduleID    模块编码
@param      (i)long detailID      不移除的异常记录

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::RemoveExceptionExcept(long moduleID, long detailID)
{
	try {
		vector<CSysException>::iterator iter = m_exceptionArr.begin();
		while (iter != m_exceptionArr.end()) {
			if ((*iter).GetModuleID() == moduleID && (*iter).GetDetailID() != detailID){
					CSysException newException = *iter;
					iter = m_exceptionArr.erase(iter);
					ExceptionItemChanged.Invoke(newException,false);
			}
			else {
				iter++;
			}
		}
	}
	catch (CSysException& e) {
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		WriteExceptionLog(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__), CExceptionLog::LEVEL_ERROR);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有异常

@param      无

@retval     bool    true: 有异常, false:无异常

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::HasException()
{
	return (! m_exceptionArr.empty());
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有指定类别的异常

@param      (i)long moduleID    模块编码

@retval     bool    true: 有异常, false:无异常

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::HasException(long moduleID)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		if(iter->GetModuleID() == moduleID)
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否有指定的异常

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::HasException(vector<long>& moduleIDs)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		vector<long>::iterator position = find(moduleIDs.begin(),moduleIDs.end(),iter->GetModuleID());
		if(position != moduleIDs.end())
		{
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否只有硬件异常

@param      (i)long moduleID    模块编码

@retval     bool    true: 有异常, false:无异常

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::OnlyHasHardwareException()
{
	vector<long> hardwareExceptionModules;
	hardwareExceptionModules.push_back(CRWException::MODULE_ID);
	hardwareExceptionModules.push_back(CRWHardwareException::MODULE_ID);
	hardwareExceptionModules.push_back(CTWException::MODULE_ID);
	hardwareExceptionModules.push_back(CTWHardwareException::MODULE_ID);
	hardwareExceptionModules.push_back(CTHHardwareException::MODULE_ID);
	hardwareExceptionModules.push_back(CTHException::MODULE_ID);
	hardwareExceptionModules.push_back(CPrinterException::MODULE_ID);
	hardwareExceptionModules.push_back(CUPSException::MODULE_ID);
	return OnlyHasException(hardwareExceptionModules);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否只有特定的异常

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::OnlyHasException(long moduleID)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		if(iter->GetModuleID() != moduleID)
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      是否只有特定的异常

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::OnlyHasException(vector<long>& moduleIDs)
{
	for(vector<CSysException>::iterator iter =m_exceptionArr.begin();iter!=m_exceptionArr.end();iter++)
	{
		vector<long>::iterator position = find(moduleIDs.begin(),moduleIDs.end(),iter->GetModuleID());
		if(position == moduleIDs.end())
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有指定类别指定详细错误的异常

@param      (i)long moduleID    模块编码

@retval     bool    true: 有异常, false:无异常

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::HasException(long moduleID, long detailID)
{
	bool found = false;
	vector<CSysException>::iterator iter = m_exceptionArr.begin();
	while (iter != m_exceptionArr.end()) {
		if ((*iter).GetModuleID() == moduleID && (*iter).GetDetailID() == detailID) {
			found = true;
			break;
		}
		iter++;
	}
	return found;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否有维护异常

@param      无

@retval     bool    true: 有异常, false:无异常

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::HasMaintainException()
{
	bool found = false;
	vector<CSysException>::iterator iter = m_exceptionArr.begin();
	while (iter != m_exceptionArr.end()) {
		SERVICE_STATE serviceStatus;
		iter->GetBag().GetValue(EXCEPTION_SERVICE_STATE_KEY,serviceStatus);
		if (serviceStatus== MAINTENANCE) {
			found = true;
			break;
		}
		iter++;
	}
	return found;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取最后一个异常对象

@param      无

@retval     CSysException&      最后一个异常对象的引用

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CSysException& CExceptionMgr::GetLastException()
{
	if (HasException()) {
		CSysException& lastException = m_exceptionArr.back();
		return lastException;
	}
	else {
		throw CInnerException(CInnerException::MODULE_ID,CInnerException::FUNC_ERR_RETURN, _T(__FILE__), __LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      删除并返回最后一个异常对象

@param      无

@retval     CSysException

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CSysException CExceptionMgr::RemoveLastException()
{
	CSysException& lastException = m_exceptionArr.back();
	CSysException newException = lastException;
	// 删除原异常对象
	m_exceptionArr.pop_back();
	ExceptionItemChanged.Invoke(newException,false);
	return newException;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写入异常Log

@param      (i)CSysException e

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::WriteExceptionLog(CSysException& e, CExceptionLog::EXCEPTION_LEVEL exLevel)
{
	try {		
		// 记入异常log
		CString errMsg=GetDescription(e);
		if (e.GetMessage() != _T("")) {
			errMsg += _T("(") + e.GetMessage() + _T(")");
		}
		theEXCEPTION_LOG.WriteData(exLevel, GetActualMTC(e), e.GetFileName(), e.GetLineNo(), errMsg);
	}
	catch (...) {
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理异常

@param      (i)CSysException& e

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::ProcessException(CSysException& e)
{
	try {
		// 添加到异常队列中。
		AddException(e,false);
		if(!m_isBusy)
		{
			m_isBusy = true;
			// 结束等待光标
			theCURSOR_MGR.EndWaitCursor();
			switch (theSERVICE_MGR.GetState()) {
			// 初始中|休息中|中止中
			case INIT:
			case OUT_OF:
			case REST:
				{
					CString guideMsg = theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_FAIL);
					guideMsg.Replace(_T("ＸＸＸＸ异常"), GetDescription(e));
					theAPP_SESSION.ShowOfficeGuide(guideMsg);
				}
				break;
			default:
				{
					// 显示异常信息
					CString message = _T("");
					message.Format(_T("%s%08X"), _T("错误编码："), GetActualMTC(e));
					message += _T("|错误信息：") + GetDescription(e);
					message += _T("|处理方法：");

					int len = CMessageBox::MAX_LINE_LENGTH - 10;    // 提示信息每行长度
					CString prompt=GetPrompt(e);
					if (prompt.GetLength() > len) {
						// 若换行位置是半个汉字，则左移一个字节
						if (IsHalfHZ(prompt, len - 1)) {
							len--;
						}
						message += prompt.Left(len);
						message += _T("|　　　　　") + prompt.Mid(len);
					}
					else {
						message += prompt;
					}
					CMessageBox::Show(_T("异常"), message, CMessageBox::BTN_OK, DT_LEFT,CMessageBox::DISPLAY_TIME_30);
				}
				break;
			}
			m_isBusy = false;
		}
	}
	catch (CSysException& e) {
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
		m_isBusy = false;
	}
	catch (...) {
		WriteExceptionLog(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__), CExceptionLog::LEVEL_ERROR);
		m_isBusy = false;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      故障解除(不能解除非法更换票箱的异常)

@param      无

@retval     bool    true:成功;  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::RepareException()
{
	try {
		CBOMForeService* pService = (CBOMForeService*)theSERVICE_MGR.GetCurService();
		// 解除外部读卡器故障
		if (HasException(CRWHardwareException::MODULE_ID) || HasException(CRWException::MODULE_ID)) {
			// 清除RW异常对象
			RemoveException(CRWHardwareException::MODULE_ID);
			RemoveException(CRWException::MODULE_ID);
			try{
				// 修复读卡器
				pService->GetRWHelper()->Repare();
				if(theBOM_INFO.IsRWMonitorOpened(true)){
					theSERVICE_MGR.GetService<CRWMonitorSvc>(MONITOR_RW_SVC)->Resume();
				}
			}
			catch (CSysException& e) {
				ProcessException(e);
			}
			catch (...) {
				ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}
		// 解除内部读卡器故障
		if (HasException(CTWHardwareException::MODULE_ID) || HasException(CTWException::MODULE_ID)) {
			// 清除TW异常对象
			RemoveException(CTWHardwareException::MODULE_ID);
			RemoveException(CTWException::MODULE_ID);
			try{
				// 修复读卡器
				pService->GetTWHelper()->Repare();
				if(theBOM_INFO.IsRWMonitorOpened(false)){
					theSERVICE_MGR.GetService<CTWMonitorSvc>(MONITOR_TW_SVC)->Resume();
				}
			}
			catch (CSysException& e) {
				ProcessException(e);
			}
			catch (...) {
				ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}
		// 解除出票机故障
		if (HasException(CTHHardwareException::MODULE_ID) || HasException(CTHException::MODULE_ID)) {
			// 清除TH异常对象
			RemoveException(CTHHardwareException::MODULE_ID);
			//RemoveExceptionExcept(CTHException::MODULE_ID,CTHException::INVALID_EXCHANGE);
			try{
				// 修复出票机
				pService->GetTHHelper()->Repare();
				if(theBOM_INFO.IsTHMonitorOpened()){
					theSERVICE_MGR.GetService<CTHMonitorSvc>(MONITOR_TH_SVC)->Resume();
				}
			}
			catch (CSysException& e) {
				ProcessException(e);
			}
			catch (...) {
				ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}
		// 解除打印机故障
		if (HasException(CPrinterException::MODULE_ID)) {
			// 清除打印机异常对象
			RemoveException(CPrinterException::MODULE_ID);
			try{
				pService->GetPRTHelper()->Repare();
			}
			catch (CSysException& e) {
				ProcessException(e);
			}
			catch (...) {
				ProcessException(CInnerException(pService->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}
		//解除时钟同步故障
		if(HasException(CSNTPSException::MODULE_ID))
		{
			RemoveException(CSNTPSException::MODULE_ID);
			try
			{
				//强制同步一次
				theSNTP_HELPER.ForceTimeSynchronize();
				//再同步一次，如果有错误抛出异常。
				theSNTP_HELPER.TimeSynchronize();
			}
			catch (CSysException& e)
			{
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...)
			{
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
		//解除FTP异常
		if(HasException(CFtpException::MODULE_ID))
		{
			RemoveException(CFtpException::MODULE_ID);
			try
			{
				theFTP_HELPER.RepairException();
			}
			catch (CSysException& e)
			{
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...)
			{
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
	}
	catch (CSysException& e) {
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
	}
	catch (...) {
		WriteExceptionLog(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__), CExceptionLog::LEVEL_ERROR);
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      得到所有的异常

@param      无

@retval     const vector<CSysException>&

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
const vector<CSysException>& CExceptionMgr::GetAllException()
{
	return m_exceptionArr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取异常的模块编码

@param      CSysException& e BOM异常对象

@retval     DEVICE_PART_TYPE 部件类型

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_COMPONENT CExceptionMgr::GetDevicePartTypeOfException(CSysException& e)
{
	DEVICE_COMPONENT type = BOM_DEVICE_APPLI;
	switch(e.GetModuleID()){
		case CPrinterException::MODULE_ID:
			type = BOM_DEVICE_PRINTER;
			break;
		case CRWException::MODULE_ID:
		case CRWHardwareException::MODULE_ID:
			type = BOM_DEVICE_OUT_TPU;
			break;
		case CTWException::MODULE_ID:
		case CTWHardwareException::MODULE_ID:
			type = BOM_DEVICE_INNER_TPU;
			break;
		case CTHException::MODULE_ID:
		case CTHHardwareException::MODULE_ID:
			type = BOM_DEVICE_TICKET_MODULE;
			break;
		default:
			type = DEVICE_COMPOENT_UNKNOW;
	}
	return type;
}

