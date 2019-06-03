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

// ʵ����CExceptionMgr��Ψһ����
CExceptionMgr CExceptionMgr::theInstance;

const static TCHAR* EXCEPTION_SERVICE_STATE_KEY =_T("EXCEPTION_SERVICE_STATE");

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionMgr::CExceptionMgr()
{
	m_isBusy = false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionMgr::~CExceptionMgr()
{
	m_exceptionArr.clear();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����Ķ���

@param      ��

@retval     CExceptionMgr&

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CExceptionMgr& CExceptionMgr::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����쳣��¼

@param      (i)CSysException& e
@param      (i)bool needSendStatus   �Ƿ���Ҫ����״̬����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::AddException(CSysException& e,bool needSendStatus)
{
	try {
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		// ��¼�쳣����ʱ��ҵ��״̬
		e.GetBag().SetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
		// д�쳣��־
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
@brief      ɾ���쳣��¼

@param      (i)long m_moduleID    ģ�����
@param      (i)long detailID      ģ������ϸ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::RemoveException(long moduleID, long detailID)
{
	try {
		vector<CSysException>::iterator iter = m_exceptionArr.begin();
		while (iter != m_exceptionArr.end()) {
			// �쳣��ϸ����ΪNO_DETAIL_IDʱ����ʹ����ϸ����
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
@brief      ɾ��ָ���쳣�����ģ���������쳣��¼

@param      (i)long m_moduleID    ģ�����
@param      (i)long detailID      ���Ƴ����쳣��¼

@retval     ��

@exception  ��
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
@brief      �Ƿ����쳣

@param      ��

@retval     bool    true: ���쳣, false:���쳣

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::HasException()
{
	return (! m_exceptionArr.empty());
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ���ָ�������쳣

@param      (i)long moduleID    ģ�����

@retval     bool    true: ���쳣, false:���쳣

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
@brief      �Ƿ���ָ�����쳣

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
@brief      �Ƿ�ֻ��Ӳ���쳣

@param      (i)long moduleID    ģ�����

@retval     bool    true: ���쳣, false:���쳣

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
@brief      �Ƿ�ֻ���ض����쳣

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
@brief      �Ƿ�ֻ���ض����쳣

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
@brief      �Ƿ���ָ�����ָ����ϸ������쳣

@param      (i)long moduleID    ģ�����

@retval     bool    true: ���쳣, false:���쳣

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
@brief      �Ƿ���ά���쳣

@param      ��

@retval     bool    true: ���쳣, false:���쳣

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
@brief      ��ȡ���һ���쳣����

@param      ��

@retval     CSysException&      ���һ���쳣���������

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
@brief      ɾ�����������һ���쳣����

@param      ��

@retval     CSysException

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
CSysException CExceptionMgr::RemoveLastException()
{
	CSysException& lastException = m_exceptionArr.back();
	CSysException newException = lastException;
	// ɾ��ԭ�쳣����
	m_exceptionArr.pop_back();
	ExceptionItemChanged.Invoke(newException,false);
	return newException;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д���쳣Log

@param      (i)CSysException e

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::WriteExceptionLog(CSysException& e, CExceptionLog::EXCEPTION_LEVEL exLevel)
{
	try {		
		// �����쳣log
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
@brief      �����쳣

@param      (i)CSysException& e

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::ProcessException(CSysException& e)
{
	try {
		// ��ӵ��쳣�����С�
		AddException(e,false);
		if(!m_isBusy)
		{
			m_isBusy = true;
			// �����ȴ����
			theCURSOR_MGR.EndWaitCursor();
			switch (theSERVICE_MGR.GetState()) {
			// ��ʼ��|��Ϣ��|��ֹ��
			case INIT:
			case OUT_OF:
			case REST:
				{
					CString guideMsg = theGUIDE_INFO.GetOperationMessage(GUIDE_LOGIN_FAIL);
					guideMsg.Replace(_T("�أأأ��쳣"), GetDescription(e));
					theAPP_SESSION.ShowOfficeGuide(guideMsg);
				}
				break;
			default:
				{
					// ��ʾ�쳣��Ϣ
					CString message = _T("");
					message.Format(_T("%s%08X"), _T("������룺"), GetActualMTC(e));
					message += _T("|������Ϣ��") + GetDescription(e);
					message += _T("|��������");

					int len = CMessageBox::MAX_LINE_LENGTH - 10;    // ��ʾ��Ϣÿ�г���
					CString prompt=GetPrompt(e);
					if (prompt.GetLength() > len) {
						// ������λ���ǰ�����֣�������һ���ֽ�
						if (IsHalfHZ(prompt, len - 1)) {
							len--;
						}
						message += prompt.Left(len);
						message += _T("|����������") + prompt.Mid(len);
					}
					else {
						message += prompt;
					}
					CMessageBox::Show(_T("�쳣"), message, CMessageBox::BTN_OK, DT_LEFT,CMessageBox::DISPLAY_TIME_30);
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
@brief      ���Ͻ��(���ܽ���Ƿ�����Ʊ����쳣)

@param      ��

@retval     bool    true:�ɹ�;  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::RepareException()
{
	try {
		CBOMForeService* pService = (CBOMForeService*)theSERVICE_MGR.GetCurService();
		// ����ⲿ����������
		if (HasException(CRWHardwareException::MODULE_ID) || HasException(CRWException::MODULE_ID)) {
			// ���RW�쳣����
			RemoveException(CRWHardwareException::MODULE_ID);
			RemoveException(CRWException::MODULE_ID);
			try{
				// �޸�������
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
		// ����ڲ�����������
		if (HasException(CTWHardwareException::MODULE_ID) || HasException(CTWException::MODULE_ID)) {
			// ���TW�쳣����
			RemoveException(CTWHardwareException::MODULE_ID);
			RemoveException(CTWException::MODULE_ID);
			try{
				// �޸�������
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
		// �����Ʊ������
		if (HasException(CTHHardwareException::MODULE_ID) || HasException(CTHException::MODULE_ID)) {
			// ���TH�쳣����
			RemoveException(CTHHardwareException::MODULE_ID);
			//RemoveExceptionExcept(CTHException::MODULE_ID,CTHException::INVALID_EXCHANGE);
			try{
				// �޸���Ʊ��
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
		// �����ӡ������
		if (HasException(CPrinterException::MODULE_ID)) {
			// �����ӡ���쳣����
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
		//���ʱ��ͬ������
		if(HasException(CSNTPSException::MODULE_ID))
		{
			RemoveException(CSNTPSException::MODULE_ID);
			try
			{
				//ǿ��ͬ��һ��
				theSNTP_HELPER.ForceTimeSynchronize();
				//��ͬ��һ�Σ�����д����׳��쳣��
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
		//���FTP�쳣
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
@brief      �õ����е��쳣

@param      ��

@retval     const vector<CSysException>&

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
const vector<CSysException>& CExceptionMgr::GetAllException()
{
	return m_exceptionArr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ�쳣��ģ�����

@param      CSysException& e BOM�쳣����

@retval     DEVICE_PART_TYPE ��������

@exception  ��
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

