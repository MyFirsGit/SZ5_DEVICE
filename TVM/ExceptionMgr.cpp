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
CExceptionMgr::CExceptionMgr():CExceptionProcessor()
{
	m_isBusy = false;
	ResetTimesOfRepareException();
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
/*
@brief      ��ȡ���쳣��MTC��  (��Ʊ��ʹ��)    

@param      (i)CESException& e   �쳣����

@retval     unsigned long :�쳣��MTC��   

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
@brief      ��ȡ���쳣��EC��      

@param      (i)CESException& e   �쳣����

@retval     unsigned long :�쳣��EC��   

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
@brief      ��ȡ���쳣��MTC��      

@param      (i)CESException& e   �쳣����

@retval     unsigned long :�쳣��MTC��      

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
@brief      ��ȡ���쳣�����ҵ��״̬     

@param      (i)CESException& e   �쳣����

@retval     SERVICE_STATE:�쳣��ҵ��״̬        

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
@brief      ��ȡ���쳣������   

@param      (i)CESException& e   �쳣����

@retval     CString :�쳣������       

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
@brief      ��ȡ���쳣�Ĵ���������   

@param      (i)CESException& e   �쳣����

@retval     CString :�쳣�Ĵ�����

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
@brief      �Ƿ�ֻ��Ӳ���쳣

@param      (i)long moduleID    ģ�����

@retval     bool    true: ���쳣, false:���쳣

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::OnlyHasHardwareException()
{
	vector<long> hardwareExceptionModules;
	// ֽ�ҽ���ģ��
	//hardwareExceptionModules.push_back(CBHException::MODULE_ID);
	//hardwareExceptionModules.push_back(CBHHardwareException::MODULE_ID);
	hardwareExceptionModules.push_back(CBRHException::MODULE_ID);
	hardwareExceptionModules.push_back(CBRHHardwareException::MODULE_ID);
	// Ӳ��ģ��
	hardwareExceptionModules.push_back(CCHException::MODULE_ID);
	hardwareExceptionModules.push_back(CCHHardwareException::MODULE_ID);
	// Card����ģ��
	hardwareExceptionModules.push_back(CTHException::MODULE_ID);
	hardwareExceptionModules.push_back(CTHHardwareException::MODULE_ID);
	// ֽ������ģ��
	hardwareExceptionModules.push_back(CBHChangeException::MODULE_ID);
	hardwareExceptionModules.push_back(CBHChangeHardwareException::MODULE_ID);
	// IOģ��
	hardwareExceptionModules.push_back(CIOException::MODULE_ID);
	// ״̬��ʾ��ģ��
	hardwareExceptionModules.push_back(CStatusDisplayException::MODULE_ID);	
	// Card��д��ģ��
	hardwareExceptionModules.push_back(CRWException::TVM_CARD_RW_MODULE_ID);
	hardwareExceptionModules.push_back(CRWHardwareException::TVM_CARD_RW_MODULE_ID);	
	// ��ӡ��ģ��
	hardwareExceptionModules.push_back(CPrinterException::MODULE_ID);

	return OnlyHasException(hardwareExceptionModules);
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
		SERVICE_STATE serviceState = theSERVICE_MGR.GetState();
		// ��¼�쳣����ʱ��ҵ��״̬
		e.GetBag().SetValue(EXCEPTION_SERVICE_STATE_KEY,serviceState);
		// д�쳣��־
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
@brief      ���Ͻ��(���ܽ���Ƿ�����Ʊ����쳣)

@param      bool bMenualRepare;�Ƿ�Ϊ�ֶ�����쳣

@retval     bool    true:�ɹ�;  false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CExceptionMgr::RepareException(bool bMenualRepare /* = false*/)
{
	try {
		// ֻ��Ӳ�������ŕ��@ʾ����æµ��,�Ҳ�Ϊ��ά��ҵ��ʱ
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
		// �����ά��ҵ�񣬲������޸�����
		if (MAINTENANCE == theSERVICE_MGR.GetState()){
			ResetTimesOfRepareException();
			bMenualRepare = true;
		}

		if (m_nTimesReparePrint <= nMaxTimes && HasException(CParamException::MODULE_ID)) {
			RemoveException(CParamException::MODULE_ID);
			// ��ʼ�����в���		
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
		// �����ֵ����������,�����ֵ��д������ʹ��
		//if (m_nTimesRepareRWRecharge <= nMaxTimes && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_RECHARGE_RW) && (HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID) || HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID))) {
		//	// ���RW�쳣����
		//	RemoveException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID);
		//	RemoveException(CRWException::TVM_RECHARGE_RW_MODULE_ID);
		//	try{
		//		// �޸�������
		//		RECHARGERW_HELPER->Repare();
		//	}
		//	catch (CSysException& e) {
		//		m_nTimesRepareRWRecharge++;
		//		//�����쳣��ֱ�Ӽ����쳣����
		//		ProcessException(e);
		//	}
		//	catch (...) {
		//		m_nTimesRepareRWRecharge++;
		//		ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		//	}
		//}

		// ���Ʊ������������,���Ʊ������������
		if (m_nTimesRepareRWCard < nMaxTimes && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_CARD_RW) && (HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID) || HasException(CRWException::TVM_CARD_RW_MODULE_ID))) {
			// ���TW�쳣����
			RemoveException(CRWHardwareException::TVM_CARD_RW_MODULE_ID);
			RemoveException(CRWException::TVM_CARD_RW_MODULE_ID);
			try{
				// �޸�������
				CARDRW_HELPER->Repare();
			}
			catch (CSysException& e) {
				m_nTimesRepareRWCard++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch (...) {
				m_nTimesRepareRWCard++;
				ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}

		//// ���Token����������,���Token����������ʹ��
		//if (m_nTimesRepareRWToken <= nMaxTimes && theTVM_INFO.IsRWUsed(RW_TYPE::TVM_TOKEN_RW) && (HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID) || HasException(CRWException::TVM_TOKEN_RW_MODULE_ID))) {
		//	// ���TW�쳣����
		//	RemoveException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID);
		//	RemoveException(CRWException::TVM_TOKEN_RW_MODULE_ID);
		//	try{
		//		// �޸�������
		//		TOKENRW_HELPER->Repare();
		//	}
		//	catch (CSysException& e) {
		//		m_nTimesRepareRWToken++;
		//		//�����쳣��ֱ�Ӽ����쳣����
		//		ProcessException(e);
		//	}
		//	catch (...) {
		//		m_nTimesRepareRWToken++;
		//		ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		//	}
		//}


		// �����ӡ������
		if (m_nTimesReparePrint <= nMaxTimes && theTVM_INFO.IsPrinterUsed() && HasException(CPrinterException::MODULE_ID)) {
			// �����ӡ���쳣����
			RemoveException(CPrinterException::MODULE_ID);
			try{
				PRINTER_HELPER->Repare();
				if(MAINTENANCE == theSERVICE_MGR.GetState())
					PRINTER_HELPER->PrintLastUnprintReceipt();// ��ӡ��Ϊ��ӡ������δ��ӡ������
				theTVM_STATUS_MGR.SetPrintStatus(PRINTER_STATUS::PRINTER_NORMAL);
			}
			catch (CSysException& e) {
				m_nTimesReparePrint++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch (...) {
				m_nTimesReparePrint++;
				ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
		}	

		//���ʱ��ͬ������
		if(m_nTimesRepareSNTP <= nMaxTimes && HasException(CSNTPSException::MODULE_ID)){
			RemoveException(CSNTPSException::MODULE_ID);
			try{
				//ǿ��ͬ��һ��(ֻ����λ�����·�ǿ��ʱ��ͬ��)
				//theSNTP_HELPER.ForceTimeSynchronize();
				//��ͬ��һ�Σ�����д����׳��쳣��
				theSNTP_HELPER.TimeSynchronize();
			}
			catch (CSysException& e)	{
				m_nTimesRepareSNTP++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch(...){
				m_nTimesRepareSNTP++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		//���FTP�쳣
		if(m_nTimesRepareFTP <= nMaxTimes && HasException(CFtpException::MODULE_ID)){
			RemoveException(CFtpException::MODULE_ID);
			try{
				// ��ʱע�ͣ���ֹ�����ʱ��zhengxianle
				//theFTP_HELPER.RepairException();
			}
			catch (CSysException& e)	{
				m_nTimesRepareFTP++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch(...){
				m_nTimesRepareFTP++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}
		//������п�ģ���쳣
		//if(m_nTimesRepareBankcard <= nMaxTimes && theTVM_INFO.IsBankCardUsed() && (HasException(CBankCardException::MODULE_ID) || HasException(CBankHardException::MODULE_ID))){
		//	RemoveException(CBankCardException::MODULE_ID);
		//	RemoveException(CBankHardException::MODULE_ID);
		//	try{
		//		BANKCARD_HELPER->Repare();
		//	}
		//	catch (CSysException& e){
		//		m_nTimesRepareBankcard++;
		//		//�����쳣��ֱ�Ӽ����쳣����
		//		ProcessException(e);
		//	}
		//	catch(...){
		//		m_nTimesRepareBankcard++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}

		//���ֽ��ģ���쳣
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
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch (...){
				m_nTimesRepareBanknote++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		//���ֽ������ģ���쳣
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
		//		// ���ӿ������쳣����ÿ�ζ����
		//	}
		//	else{
		//		RemoveException(CBHChangeException::MODULE_ID);
		//		RemoveException(CBHChangeHardwareException::MODULE_ID);
		//		try{
		//			BH_CHANGE_HELPER->Repare();
		//		}
		//		catch (CSysException& e)	{
		//			m_nTimesRepareBanknoteChange++;
		//			//�����쳣��ֱ�Ӽ����쳣����
		//			ProcessException(e);
		//		}
		//		catch (...){
		//			m_nTimesRepareBanknoteChange++;
		//			theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//		}
		//	}
		//}

		////�������ģ���쳣
		//if (m_nTimesRepareCard <= nMaxTimes && theTVM_INFO.IsTHUsed() && (HasException(CTHException::MODULE_ID) || HasException(CTHHardwareException::MODULE_ID)))	{
		//	RemoveException(CTHException::MODULE_ID);
		//	RemoveException(CTHHardwareException::MODULE_ID);
		//	try{
		//		CARD_HELPER->Repare();
		//		theTVM_STATUS_MGR.SetCardModuleStatus(HARDWARE_MODULE_STATUS::HARDWARE_MODULE_NORMAL);
		//	}
		//	catch (CSysException& e)	{
		//		m_nTimesRepareCard++;
		//		//�����쳣��ֱ�Ӽ����쳣����
		//		ProcessException(e);
		//	}
		//	catch (...) {
		//		m_nTimesRepareCard++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}

		//���Ӳ��ģ���쳣
		if (m_nTimesRepareCoin <= nMaxTimes && theTVM_INFO.IsCHUsed() && (HasException(CCHHardwareException::MODULE_ID) || HasException(CCHException::MODULE_ID))){
			RemoveException(CCHHardwareException::MODULE_ID);
			RemoveException(CCHException::MODULE_ID);
			try{
				COIN_HELPER->CH_Repare();
			}
			catch (CSysException& e)	{
				m_nTimesRepareCoin++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch (...){
				m_nTimesRepareCoin++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		// ���Ӳ�ҽ��տ��쳣
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

		//���IOģ���쳣
		if (m_nTimesRepareIO <= nMaxTimes && theTVM_INFO.IsIOUsed() && HasException(CIOException::MODULE_ID)){
			RemoveException(CIOException::MODULE_ID);
			try{
				IO_HELPER->RepareIO();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->PostMessage(SM_MAIN_IO_MONITOR,NULL,NULL);// ����IO��ء�
			}
			catch (CSysException& e)	{
				m_nTimesRepareIO++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch (...)	{
				m_nTimesRepareIO++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		//�����������쳣
		//if (m_nTimesReparePinPad <= nMaxTimes && theTVM_INFO.IsPinPadUsed() && HasException(CTVMPinPadException::MODULE_ID)){
		//	//RemoveException(CTVMPinPadException::MODULE_ID);
		//	try{
		//		//PINPAD_HELPER->UnInitialize();
		//		//PINPAD_HELPER->Initialize();
		//	}
		//	catch (CSysException& e)	{
		//		m_nTimesReparePinPad++;
		//		//�����쳣��ֱ�Ӽ����쳣����
		//		ProcessException(e);
		//	}
		//	catch (...)
		//	{
		//		m_nTimesReparePinPad++;
		//		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
		//	}
		//}

		//���״̬��ʾ���쳣
		if (m_nTimesRepareDisplay <= nMaxTimes && theTVM_INFO.IsSdspUsed() && HasException(CStatusDisplayException::MODULE_ID)){
			RemoveException(CStatusDisplayException::MODULE_ID);
			try{
				// ��ʱע�ͣ���ֹ�����ʱ��zhengxianle
				//theSTATUS_DISPLAYER.Close();
				//theSTATUS_DISPLAYER.Connect();
				//pService->GetStatusDisplayModuleHelper()->Close();
				//pService->GetStatusDisplayModuleHelper()->Connect();
			}
			catch (CSysException& e)	{
				m_nTimesRepareDisplay++;
				//�����쳣��ֱ�Ӽ����쳣����
				ProcessException(e);
			}
			catch (...){
				m_nTimesRepareDisplay++;
				theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
			}
		}

		// ���ֽ������RFID�쳣
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


		//���Token�쳣
		if (m_nTimesRepareToken <= nMaxTimes && /*theTVM_INFO.IsTokenUsed() && */(HasException(CTOKENException::MODULE_ID)||HasException(CTOKENHardwareException::MODULE_ID))){
			RemoveException(CTOKENHardwareException::MODULE_ID);
			RemoveException(CTOKENException::MODULE_ID);
			try{
				TOKEN_HELPER->Repare();
			}
			catch (CSysException& e)
			{
				m_nTimesRepareToken++;
				//�����쳣��ֱ�Ӽ����쳣����
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
@brief      ��ȡ�쳣��ģ�����

@param      CSysException& e ϵͳ�쳣����

@retval     BYTE ��������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BYTE CExceptionMgr::GetDevicePartTypeOfException(CSysException& e)
{
	BYTE retValue = DEVICE_COMPOENT_UNKNOW;
	AFC_DEVICE_TYPE deviceType = theMAINTENANCE_INFO.GetDeviceType();
	DEVICE_SUBTYPE deviceSubType = theMAINTENANCE_INFO.GetDeviceSubType();
	// �������ļ���ȡȡ�ò���ID
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
@brief		�����Զ��쳣�޸����ƴ���  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CExceptionMgr::ResetTimesOfRepareException(){
	int nTimes = 0;
	m_nTimesRepareRWRecharge = nTimes;		// ��ֵ����������쳣����						
	m_nTimesRepareRWCard = nTimes;			// Ʊ���������쳣�������
	m_nTimesRepareRWToken = nTimes;			// Token�������쳣�������
	m_nTimesReparePrint = nTimes;			// ��ӡ���쳣�������
	m_nTimesRepareSNTP = nTimes;			// NTP��ʱ�����쳣�������
	m_nTimesRepareFTP = nTimes;				// FTP�쳣�������
	m_nTimesRepareBankcard = nTimes;		// ���п��쳣�������
	m_nTimesRepareBanknote = nTimes;		// ֽ��ģ���쳣�������
	m_nTimesRepareBanknoteChange = nTimes;  // ֽ������ģ���쳣�������
	m_nTimesRepareCard = nTimes;			// Ʊ��ģ���쳣�������
	m_nTimesRepareCoin = nTimes;			// Ӳ��ģ���쳣�������
	m_nTimesRepareCoinShutter = nTimes;		// Ӳ��Ͷ�ҿ��쳣�������
	m_nTimesRepareIO  = nTimes;				// IO�쳣�������
	m_nTimesReparePinPad = nTimes;			// ��������쳣�������
	m_nTimesRepareDisplay = nTimes;			// ��ʾ���쳣�������
	m_nTimesRepareToken = nTimes;			// Tokenģ���쳣�������
}