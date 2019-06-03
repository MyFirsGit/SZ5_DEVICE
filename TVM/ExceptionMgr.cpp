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
#include "RechargeRWMonitorSvc.h"
#include "CardRWMonitorSvc.h"
#include "CardModuleMonitorSvc.h"
#include "FTPHelper.h"
//#include "TOKENException.h"
//#include "TOKENHardwareException.h"
//#include "BankCardHardException.h"
//#include "PrinterHardException.h"

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
CExceptionMgr::CExceptionMgr():CExceptionProcessor()
{
	m_isBusy = false;
	ResetTimesOfRepareException();
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
/*
@brief      获取该异常的MTC码  (出票机使用)    

@param      (i)CESException& e   异常对象

@retval     unsigned long :异常的MTC码   

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionMgr::GetMTC(CSysException& e) const
{
	CExceptionInfo::EXCEPTION_DETAIL exMsg;
	theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	return exMsg.lMTC;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取该异常的EC码      

@param      (i)CESException& e   异常对象

@retval     unsigned long :异常的EC码   

@exception  none  
*/
//////////////////////////////////////////////////////////////////////////
unsigned short CExceptionMgr::GetEC(CSysException& e) const
{
	CExceptionInfo::EXCEPTION_DETAIL exMsg;
	theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	return exMsg.lEc;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取该异常的MTC码      

@param      (i)CESException& e   异常对象

@retval     unsigned long :异常的MTC码      

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
unsigned long CExceptionMgr::GetActualMTC(CSysException& e) const
{
	CExceptionInfo::EXCEPTION_DETAIL exMsg;
	theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
	SERVICE_STATE serviceState;
	e.GetBag().GetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
	return (exMsg.lMTC & 0xFF00FFFF) | ((serviceState << 16) & 0x00FF0000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取该异常对象的业务状态     

@param      (i)CESException& e   异常对象

@retval     SERVICE_STATE:异常的业务状态        

@exception  none
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
@brief      获取该异常的描述   

@param      (i)CESException& e   异常对象

@retval     CString :异常的描述       

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CExceptionMgr::GetDescription(CSysException& e) const
{
	CExceptionInfo::EXCEPTION_DETAIL exMsg;
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language){
	case LANGUAGE_ID_CHINESE:
		theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
		return exMsg.sErr;
	case LANGUAGE_ID_ENGLISH:
		theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
		return exMsg.eErr;
	case LANGUAGE_ID_PORTUGUESE:
		theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
		return exMsg.pErr;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取该异常的处理建议描述   

@param      (i)CESException& e   异常对象

@retval     CString :异常的处理建议

@exception  none 
*/
//////////////////////////////////////////////////////////////////////////
CString CExceptionMgr::GetPrompt(CSysException& e) const
{
	CExceptionInfo::EXCEPTION_DETAIL exMsg;
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentOperatorLanguageType();
	switch(language){
	case LANGUAGE_ID_CHINESE:
		theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
		return exMsg.sDeal;
	case LANGUAGE_ID_ENGLISH:
		theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
		return exMsg.eDeal;
	case LANGUAGE_ID_PORTUGUESE:
		theExceptionInfo.GetExceptionByID(exMsg, e.GetModuleID(), e.GetDetailID());
		return exMsg.pDeal;

	}
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
	// 纸币接受模块
	//hardwareExceptionModules.push_back(CBHException::MODULE_ID);
	//hardwareExceptionModules.push_back(CBHHardwareException::MODULE_ID);
	hardwareExceptionModules.push_back(CBRHException::MODULE_ID);
	hardwareExceptionModules.push_back(CBRHHardwareException::MODULE_ID);
	// 硬币模块
	hardwareExceptionModules.push_back(CCHException::MODULE_ID);
	hardwareExceptionModules.push_back(CCHHardwareException::MODULE_ID);
	// Card处理模块
	hardwareExceptionModules.push_back(CTHException::MODULE_ID);
	hardwareExceptionModules.push_back(CTHHardwareException::MODULE_ID);
	// 纸币找零模块
	hardwareExceptionModules.push_back(CBHChangeException::MODULE_ID);
	hardwareExceptionModules.push_back(CBHChangeHardwareException::MODULE_ID);
	// IO模块
	hardwareExceptionModules.push_back(CIOException::MODULE_ID);
	// 状态显示屏模块
	hardwareExceptionModules.push_back(CStatusDisplayException::MODULE_ID);	
	// Card读写器模块
	hardwareExceptionModules.push_back(CRWException::TVM_CARD_RW_MODULE_ID);
	hardwareExceptionModules.push_back(CRWHardwareException::TVM_CARD_RW_MODULE_ID);	
	// 打印机模块
	hardwareExceptionModules.push_back(CPrinterException::MODULE_ID);

	return OnlyHasException(hardwareExceptionModules);
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
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		// 记录异常发生时的业务状态
		e.GetBag().SetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
		// 写异常日志
		WriteExceptionLog(e, CExceptionLog::LEVEL_ERROR);
		AddException(e);
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

@param      bool bMenualRepare;是否为手动解除异常

@retval     bool    true:成功;  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::RepareException(bool bMenualRepare /* = false*/)
{
	try {
		// 只有硬件異常才會顯示正在忙碌中,且不为后维护业务时
		if(OnlyHasHardwareException() && MAINTENANCE != theSERVICE_MGR.GetState()){
			NOTIFY_PROCESSING_WINDOW();
		}
		theDEBUG_LOG->WriteData(_T("CExceptionMgr"),(CString)__FUNCTION__,__LINE__,_T("<"));
		CTVMForeService* pService = (CTVMForeService*)theSERVICE_MGR.GetCurService();
		theAPP_SESSION.ShowOfficeGuide(_opl(GUIDE_REMOVE_EXCEPTION_WORKING));
		int nMaxTimes = theFunction_INFO.GetTimesRepareException();// >=0
		if (nMaxTimes < 0){
			nMaxTimes = 0;
		}
		// 如果是维护业务，不限制修复次数
		if (MAINTENANCE == theSERVICE_MGR.GetState()){
			ResetTimesOfRepareException();
			bMenualRepare = true;
		}

		if (m_nTimesReparePrint <= nMaxTimes && HasException(CParamException::MODULE_ID)) {
			RemoveException(CParamException::MODULE_ID);
			// 初始化所有参数		
			try{
				thePARAM_HELPER.InitAllParam();
			}
			catch(CSysException& e){
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch(...){
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
		// 解除充值读卡器故障,如果充值读写器允许使用
		//if (m_nTimesRepareRWRecharge <= nMaxTimes && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_RECHARGE_RW) && (HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) || HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID))) {
		//	// 清除RW异常对象
		//	RemoveException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID);
		//	RemoveException(CRWException::TVM_RECHARGE_RW_MODULE_ID);
		//	try{
		//		// 修复读卡器
		//		RECHARGERW_HELPER->Repare();
		//	}
		//	catch (CSysException& e) {
		//		m_nTimesRepareRWRecharge++;
		//		//还有异常，直接加入异常队列
		//		ProcessException(e);
		//	}
		//	catch (...) {
		//		m_nTimesRepareRWRecharge++;
		//		ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		//	}
		//}

		// 解除票卡读卡器故障,如果票卡读卡器在用
		if (m_nTimesRepareRWCard < nMaxTimes && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_CARD_RW) && (HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID) || HasException(CRWException::TVM_CARD_RW_MODULE_ID))) {
			// 清除TW异常对象
			RemoveException(CRWHardwareException::TVM_CARD_RW_MODULE_ID);
			RemoveException(CRWException::TVM_CARD_RW_MODULE_ID);
			try{
				// 修复读卡器
				CARDRW_HELPER->Repare();
			}
			catch (CSysException& e) {
				m_nTimesRepareRWCard++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...) {
				m_nTimesRepareRWCard++;
				ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}

		//// 解除Token读卡器故障,如果Token读卡器允许使用
		//if (m_nTimesRepareRWToken <= nMaxTimes && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_TOKEN_RW) && (HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID) || HasException(CRWException::TVM_TOKEN_RW_MODULE_ID))) {
		//	// 清除TW异常对象
		//	RemoveException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID);
		//	RemoveException(CRWException::TVM_TOKEN_RW_MODULE_ID);
		//	try{
		//		// 修复读卡器
		//		TOKENRW_HELPER->Repare();
		//	}
		//	catch (CSysException& e) {
		//		m_nTimesRepareRWToken++;
		//		//还有异常，直接加入异常队列
		//		ProcessException(e);
		//	}
		//	catch (...) {
		//		m_nTimesRepareRWToken++;
		//		ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		//	}
		//}


		// 解除打印机故障
		if (m_nTimesReparePrint <= nMaxTimes && theTVM_INFO.IsPrinterUsed() && HasException(CPrinterException::MODULE_ID)) {
			// 清除打印机异常对象
			RemoveException(CPrinterException::MODULE_ID);
			try{
				PRINTER_HELPER->Repare();
				if(MAINTENANCE == theSERVICE_MGR.GetState())
					PRINTER_HELPER->PrintLastUnprintReceipt();// 打印因为打印机故障未打印的数据
				theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_NORMAL);
			}
			catch (CSysException& e) {
				m_nTimesReparePrint++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...) {
				m_nTimesReparePrint++;
				ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}	

		//解除时钟同步故障
		if(m_nTimesRepareSNTP <= nMaxTimes && HasException(CSNTPSException::MODULE_ID)){
			RemoveException(CSNTPSException::MODULE_ID);
			try{
				//强制同步一次(只有上位可以下发强制时钟同步)
				//theSNTP_HELPER.ForceTimeSynchronize();
				//再同步一次，如果有错误抛出异常。
				theSNTP_HELPER.TimeSynchronize();
			}
			catch (CSysException& e)	{
				m_nTimesRepareSNTP++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch(...){
				m_nTimesRepareSNTP++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		//解除FTP异常
		if(m_nTimesRepareFTP <= nMaxTimes && HasException(CFtpException::MODULE_ID)){
			RemoveException(CFtpException::MODULE_ID);
			try{
				// 暂时注释，防止解除耗时，zhengxianle
				//theFTP_HELPER.RepairException();
			}
			catch (CSysException& e)	{
				m_nTimesRepareFTP++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch(...){
				m_nTimesRepareFTP++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
		//解除银行卡模块异常
		//if(m_nTimesRepareBankcard <= nMaxTimes && theTVM_INFO.IsBankCardUsed() && (HasException(CBankCardException::MODULE_ID) || HasException(CBankHardException::MODULE_ID))){
		//	RemoveException(CBankCardException::MODULE_ID);
		//	RemoveException(CBankHardException::MODULE_ID);
		//	try{
		//		BANKCARD_HELPER->Repare();
		//	}
		//	catch (CSysException& e){
		//		m_nTimesRepareBankcard++;
		//		//还有异常，直接加入异常队列
		//		ProcessException(e);
		//	}
		//	catch(...){
		//		m_nTimesRepareBankcard++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}

		//解除纸币模块异常
		//if (m_nTimesRepareBanknote <= nMaxTimes && theTVM_INFO.IsBHUsed() &&(HasException(CBHException::MODULE_ID) || HasException(CBHHardwareException::MODULE_ID)))	{
		if (m_nTimesRepareBanknote <= nMaxTimes && theTVM_INFO.IsBHUsed() &&(HasException(CBRHException::MODULE_ID) || HasException(CBRHHardwareException::MODULE_ID)))	{
			//RemoveException(CBHException::MODULE_ID);
			//RemoveException(CBHHardwareException::MODULE_ID);
			RemoveException(CBRHException::MODULE_ID);
			RemoveException(CBRHHardwareException::MODULE_ID);
			try{
				BANKNOTE_HELPER->Repare(bMenualRepare);
			}
			catch (CSysException& e)	{
				m_nTimesRepareBanknote++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...){
				m_nTimesRepareBanknote++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		//解除纸币找零模块异常
		//if (m_nTimesRepareBanknoteChange <= nMaxTimes && theTVM_INFO.IsBHUsed() &&(HasException(CBHChangeException::MODULE_ID) || HasException(CBHChangeHardwareException::MODULE_ID)))	{
		//	if(!bMenualRepare && 
		//		(HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_BOX_EMPTY)||
		//		 HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_BOX_A_NOT_INSTALL) ||
		//		 HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_BOX_B_NOT_INSTALL) ||
		//		 HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_WASTE_BOX_NOT_INSTALL)	||
		//		 HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_IGILL_INSTALL_BOX_A)	||
		//		 HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_IGILL_INSTALL_BOX_B)	||
		//		 HasException(CBHChangeHardwareException::MODULE_ID,CBHChangeHardwareException::ERROR_BH_CHANGE_IGILL_INSTALL_BOX_INVALID)
		//		)){
		//		// 箱子空这类异常不是每次都解除
		//	}
		//	else{
		//		RemoveException(CBHChangeException::MODULE_ID);
		//		RemoveException(CBHChangeHardwareException::MODULE_ID);
		//		try{
		//			BH_CHANGE_HELPER->Repare();
		//		}
		//		catch (CSysException& e)	{
		//			m_nTimesRepareBanknoteChange++;
		//			//还有异常，直接加入异常队列
		//			ProcessException(e);
		//		}
		//		catch (...){
		//			m_nTimesRepareBanknoteChange++;
		//			theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//		}
		//	}
		//}

		////解除发卡模块异常
		//if (m_nTimesRepareCard <= nMaxTimes && theTVM_INFO.IsTHUsed() && (HasException(CTHException::MODULE_ID) || HasException(CTHHardwareException::MODULE_ID)))	{
		//	RemoveException(CTHException::MODULE_ID);
		//	RemoveException(CTHHardwareException::MODULE_ID);
		//	try{
		//		CARD_HELPER->Repare();
		//		theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_STATUS::HARDWARE_MODULE_NORMAL);
		//	}
		//	catch (CSysException& e)	{
		//		m_nTimesRepareCard++;
		//		//还有异常，直接加入异常队列
		//		ProcessException(e);
		//	}
		//	catch (...) {
		//		m_nTimesRepareCard++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}

		//解除硬币模块异常
		if (m_nTimesRepareCoin <= nMaxTimes && theTVM_INFO.IsCHUsed() && (HasException(CCHHardwareException::MODULE_ID) || HasException(CCHException::MODULE_ID))){
			RemoveException(CCHHardwareException::MODULE_ID);
			RemoveException(CCHException::MODULE_ID);
			try{
				COIN_HELPER->CH_Repare();
			}
			catch (CSysException& e)	{
				m_nTimesRepareCoin++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...){
				m_nTimesRepareCoin++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		// 解除硬币接收口异常
// 		if (m_nTimesRepareCoinShutter <= nMaxTimes && theTVM_INFO.IsCoinShutterUsed() &&(HasException(CCHShutterException::MODULE_ID) || HasException(CCHShutterHardwareException::MODULE_ID))){
// 			RemoveException(CCHShutterException::MODULE_ID);
// 			RemoveException(CCHShutterHardwareException::MODULE_ID);
// 			try{
// 				COIN_HELPER->RepareShutter();
// 			}
// 			catch(CSysException& e){
// 				m_nTimesRepareCoinShutter++;
// 				ProcessException(e);
// 			}
// 			catch(...){
// 				m_nTimesRepareCoinShutter++;
// 				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
// 			}
// 		}

		//解除IO模块异常
		if (m_nTimesRepareIO <= nMaxTimes && theTVM_INFO.IsIOUsed() && HasException(CIOException::MODULE_ID)){
			RemoveException(CIOException::MODULE_ID);
			try{
				IO_HELPER->RepareIO();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->PostMessage(SM_MAIN_IO_MONITOR,NULL,NULL);// 开启IO监控。
			}
			catch (CSysException& e)	{
				m_nTimesRepareIO++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...)	{
				m_nTimesRepareIO++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		//解除密码键盘异常
		//if (m_nTimesReparePinPad <= nMaxTimes && theTVM_INFO.IsPinPadUsed() && HasException(CTVMPinPadException::MODULE_ID)){
		//	//RemoveException(CTVMPinPadException::MODULE_ID);
		//	try{
		//		//PINPAD_HELPER->UnInitialize();
		//		//PINPAD_HELPER->Initialize();
		//	}
		//	catch (CSysException& e)	{
		//		m_nTimesReparePinPad++;
		//		//还有异常，直接加入异常队列
		//		ProcessException(e);
		//	}
		//	catch (...)
		//	{
		//		m_nTimesReparePinPad++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}

		//解除状态显示屏异常
		if (m_nTimesRepareDisplay <= nMaxTimes && theTVM_INFO.IsSdspUsed() && HasException(CStatusDisplayException::MODULE_ID)){
			RemoveException(CStatusDisplayException::MODULE_ID);
			try{
				// 暂时注释，防止解除耗时，zhengxianle
				//theSTATUS_DISPLAYER.Close();
				//theSTATUS_DISPLAYER.Connect();
				//pService->GetStatusDisplayModuleHelper()->Close();
				//pService->GetStatusDisplayModuleHelper()->Connect();
			}
			catch (CSysException& e)	{
				m_nTimesRepareDisplay++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...){
				m_nTimesRepareDisplay++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		// 解除纸币找零RFID异常
		//if(m_nTimerRepareRFID <= nMaxTimes && HasException(CBHRFIDException::MODULE_ID) && theFunction_INFO.IsBanknoteChangeBoxIDRead()){

		//	RemoveException(CBHRFIDException::MODULE_ID);

		//	try{
		//		BH_RFID_HELPER->uninitizlize();
		//		BH_RFID_HELPER->initialize();
		//	}
		//	catch(CSysException& e){
		//		m_nTimerRepareRFID++;
		//		ProcessException(e);
		//	}
		//	catch(...){
		//		m_nTimerRepareRFID++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}


		//解除Token异常
		if (m_nTimesRepareToken <= nMaxTimes && /*theTVM_INFO.IsTokenUsed() && */(HasException(CTOKENException::MODULE_ID)||HasException(CTOKENHardwareException::MODULE_ID))){
			RemoveException(CTOKENHardwareException::MODULE_ID);
			RemoveException(CTOKENException::MODULE_ID);
			try{
				TOKEN_HELPER->Repare();
			}
			catch (CSysException& e)
			{
				m_nTimesRepareToken++;
				//还有异常，直接加入异常队列
				ProcessException(e);
			}
			catch (...)
			{
				m_nTimesRepareToken++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		theDEBUG_LOG->WriteData(_T("CExceptionMgr"),(CString)__FUNCTION__ ,__LINE__,_T(">"));
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
@brief      获取异常的模块编码

@param      CSysException& e 系统异常对象

@retval     BYTE 部件类型

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE CExceptionMgr::GetDevicePartTypeOfException(CSysException& e)
{
	BYTE retValue = DEVICE_COMPOENT_UNKNOW;
	AFC_DEVICE_TYPE deviceType = theMAINTENANCE_INFO.GetDeviceType();
	DEVICE_SUBTYPE deviceSubType = theMAINTENANCE_INFO.GetDeviceSubType();
	// 从配置文件读取取得部件ID
	CString strComponentId = theTVM_INFO.GetDeviceComponentCode(deviceType,deviceSubType,e.GetModuleID());
	if(!strComponentId.IsEmpty()){
		//char* strtmp =  (LPSTR)(LPCTSTR)strComponentId;
		//ComfrHEX(strtmp, &retValue,1);
		retValue = (BYTE)_tcstoul(strComponentId,NULL,16);
	}
	return retValue;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		重置自动异常修复限制次数  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::ResetTimesOfRepareException(){
	int nTimes = 0;
	m_nTimesRepareRWRecharge = nTimes;		// 充值读卡器解除异常次数						
	m_nTimesRepareRWCard = nTimes;			// 票卡读卡器异常解除次数
	m_nTimesRepareRWToken = nTimes;			// Token读卡器异常解除次数
	m_nTimesReparePrint = nTimes;			// 打印机异常解除次数
	m_nTimesRepareSNTP = nTimes;			// NTP对时服务异常解除次数
	m_nTimesRepareFTP = nTimes;				// FTP异常解除次数
	m_nTimesRepareBankcard = nTimes;		// 银行卡异常解除次数
	m_nTimesRepareBanknote = nTimes;		// 纸币模块异常解除次数
	m_nTimesRepareBanknoteChange = nTimes;  // 纸币找零模块异常解除次数
	m_nTimesRepareCard = nTimes;			// 票卡模块异常解除次数
	m_nTimesRepareCoin = nTimes;			// 硬币模块异常解除次数
	m_nTimesRepareCoinShutter = nTimes;		// 硬币投币口异常解除次数
	m_nTimesRepareIO  = nTimes;				// IO异常解除次数
	m_nTimesReparePinPad = nTimes;			// 密码键盘异常解除次数
	m_nTimesRepareDisplay = nTimes;			// 显示屏异常解除次数
	m_nTimesRepareToken = nTimes;			// Token模块异常解除次数
}