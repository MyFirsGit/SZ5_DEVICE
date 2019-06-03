#include "StdAfx.h"
#include "BRHMessage.h"
//#include "SysInfo.h"
//#include "DCR4000Comm.h"
//#include "MaintenanceInfo.h"
#include "BRHException.h"
#include "BRHHardwareException.h"
bool isAccepting = false;
bool isStopAccept = false;

// ������Ϣ
map<UINT,CASH_BOX_INFO> mapCashBoxInfo;

// �����Ϣ
BRH_CMD_UPDATE_DENOMINATIONS denominationTable;
tBncSetInhibitList RechargeSetInhibitList;
//tBncDevReturn * Status;


tBncCashInfo *CashInfo;
BYTE Operation;
tBncDevReturn *Status;
static T_XfsCashOrder cashOrder;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void  BRH_SetStopAcceptFlag(bool flag)
{
	isStopAccept = flag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPBRH_RSP_OPEN     RSPOpenReader

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpen::CBRHOpen():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpen::~CBRHOpen(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHOpen::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHOpen::ExecuteCommand(void)
{
	unsigned int uiCommPort = 0;
	unsigned int uiBaudRate = 0;
	long lCompleteCode = BNC_open(uiCommPort,uiBaudRate);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHClose::CBRHClose():CCommand()
{						   
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHClose::~CBRHClose(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHClose::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHClose::ExecuteCommand(void)
{
	long lCompleteCode = BNC_close();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHReset::CBRHReset(int iMode, int iRetryTimes, tBncInitNumInfo* pNumInfo,tBncDevReturn* p_psStatus):CCommand()
{
	m_iMode = iMode;
	m_iRetryTimes = iRetryTimes;
	//m_pNumInfo = pNumInfo;
	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHReset::~CBRHReset(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReset::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReset::ExecuteCommand()
{
	long lCompleteCode = BNC_Reset(m_iMode,m_iRetryTimes,&m_pNumInfo,&m_psStatus);
	OnComplete(lCompleteCode);
	BRH_SetAcceptFlag(false);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHReboot::CBRHReboot():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHReboot::~CBRHReboot(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReboot::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReboot::ExecuteCommand(void)
{
	return SP_SUCCESS;

	//long lCompleteCode = Reboot();
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetStatus::CBRHGetStatus():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetStatus::~CBRHGetStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetStatus::ExecuteCommand()
{
	tBncDevReturn  p_psStatus;
	long lCompleteCode = BNC_GetStatus(&p_psStatus);
	// T_XfsCdrStatus�ṹ��BRH_RSP_GET_STATUS�ṹʵ�ʶ�������ͬ�ġ�
	memcpy(&m_status, &p_psStatus, sizeof(tBncDevReturn));
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)BRH_RSP_GET_STATUS* status    BRH״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHGetStatus::GetBRHResponse(tBncDevReturn* status)
{
	try {
		if(status == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(status, &m_status, sizeof(tBncDevReturn));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetSystemStatus::CBRHGetSystemStatus():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetSystemStatus::~CBRHGetSystemStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetSystemStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetSystemStatus::ExecuteCommand()
{
	long lCompleteCode = BNC_GetSysDevStatus(&m_status);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)BRH_RSP_GET_SYSTEM_STATUS* status    BRH״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHGetSystemStatus::GetBRHResponse(tBncSysDevStatus* status)
{
	try {
		if(status == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(status, &m_status, sizeof(tBncSysDevStatus));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancel::CBRHCancel():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancel::~CBRHCancel(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCancel::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCancel::ExecuteCommand()
{
	tBncDevReturn p_psStatus;
	long lCompleteCode = BNC_Cancel(&p_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetCapabilities::CBRHGetCapabilities():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetCapabilities::~CBRHGetCapabilities(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetCapabilities::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetCapabilities::ExecuteCommand(void)
{
	//T_XfsCapabilities tmp_Capabilities;
	//long lCompleteCode = GetCapabilities(&tmp_Capabilities);
	//// T_XfsCapabilities�ṹ��BRH_RSP_GET_CAPABLILITIES�ṹʵ�ʶ�������ͬ�ġ�
	//memcpy(&m_Capabilities, &tmp_Capabilities, sizeof(T_XfsCapabilities));
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)BRH_RSP_GET_CAPABLILITIES* Capabilities    BRH����     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHGetCapabilities::GetBRHResponse(BRH_RSP_GET_CAPABLILITIES* Capabilities)
{
	try {
		if(Capabilities == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(Capabilities, &m_Capabilities, sizeof(BRH_RSP_GET_CAPABLILITIES));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCapabilities::CBRHSetCapabilities(BRH_CMD_SET_CAPABLILITIES* Capabilities):CCommand()
{
	memcpy(&m_Capabilities, Capabilities, sizeof(BRH_CMD_SET_CAPABLILITIES));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCapabilities::~CBRHSetCapabilities(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetCapabilities::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetCapabilities::ExecuteCommand(void)
{
	//long lCompleteCode = SetCapabilities(&m_Capabilities);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetTransactionStatus::CBRHGetTransactionStatus():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetTransactionStatus::~CBRHGetTransactionStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetTransactionStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetTransactionStatus::ExecuteCommand(void)
{
	//T_TransactionStatus tmp_Status;
	//long lCompleteCode = GetTransactionStatus(&tmp_Status);
	//// T_XfsCapabilities�ṹ��BRH_RSP_GET_CAPABLILITIES�ṹʵ�ʶ�������ͬ�ġ�
	//memcpy(&m_Status, &tmp_Status, sizeof(T_TransactionStatus));
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)BRH_RSP_GET_CAPABLILITIES* Capabilities    BRH����     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHGetTransactionStatus::GetBRHResponse(BRH_CMD_GET_TRANSACTIONSTATUS* status)
{
	try {
		if(status == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(status, &m_Status, sizeof(BRH_RSP_GET_CAPABLILITIES));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetDateTime::CBRHSetDateTime(time_t dateTime):CCommand()
{
	memcpy(&m_Time, &dateTime, sizeof(time_t));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetDateTime::~CBRHSetDateTime(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetDateTime::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetDateTime::ExecuteCommand(void)
{
	//long lCompleteCode = SetDateTime(m_Time);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHEmpty::CBRHEmpty(char* pcuName, BOOL toFloat):CCommand()
{
	memset(m_pcuName,0, sizeof(m_pcuName));
	strcpy(m_pcuName, pcuName);
	m_toFloat = toFloat;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHEmpty::~CBRHEmpty(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEmpty::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEmpty::ExecuteCommand(void)
{
	long lCompleteCode = BNC_Empty(&m_result);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�÷������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBRHEmpty::GetResponse(tBncChangeNum* result){
	if(NULL != result){
		*result = m_result;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHReject::CBRHReject():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHReject::~CBRHReject(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReject::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReject::ExecuteCommand(void)
{
	return SP_SUCCESS;

	//long lCompleteCode = Reject(&m_amount);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH���ܳ�Ʊ���     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHReject::GetBRHResponse(UINT32* amount)
{
	try {
		if(amount == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*amount = m_amount;
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSelfTest::CBRHSelfTest():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSelfTest::~CBRHSelfTest(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSelfTest::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSelfTest::ExecuteCommand(void)
{
	return SP_SUCCESS;

	//long lCompleteCode = SelfTest();
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHPark::CBRHPark():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHPark::~CBRHPark(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHPark::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHPark::ExecuteCommand(void)
{
	return SP_SUCCESS;

	//long lCompleteCode = Park();
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancelWaitingCashTaken::CBRHCancelWaitingCashTaken():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancelWaitingCashTaken::~CBRHCancelWaitingCashTaken(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCancelWaitingCashTaken::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCancelWaitingCashTaken::ExecuteCommand(void)
{
	return SP_SUCCESS;

	//long lCompleteCode = CancelWaitingCashTaken();
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInStart::CBRHCashInStart():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInStart::~CBRHCashInStart(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInStart::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInStart::ExecuteCommand()
{
	long lCompleteCode = BNC_Start(&m_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashIn::CBRHCashIn():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashIn::~CBRHCashIn(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashIn::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashIn::ExecuteCommand()
{
	BYTE operate;
	long lCompleteCode = BNC_GetCashInfo(&m_CashInfo,operate,&m_Return);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH���ܳ�Ʊ���     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHCashIn::GetBRHResponse(tBncCashInfo *p_psCashInfo,tBncDevReturn *p_psStatus)
{
	try {
		if(p_psCashInfo == NULL || p_psStatus == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(p_psCashInfo,&m_CashInfo,sizeof(tBncCashInfo));
			memcpy(p_psStatus,&m_Return,sizeof(tBncDevReturn));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInRollback::CBRHCashInRollback():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInRollback::~CBRHCashInRollback(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInRollback::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInRollback::ExecuteCommand()
{
	tBncDevReturn p_psStatus;
	long lCompleteCode = BNC_Refund(&p_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH�˳����ν��ܵ����г�Ʊ     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHCashInRollback::GetBRHResponse(UINT32* amount)
{
	try {
		if(amount == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*amount = m_amount;
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInEnd::CBRHCashInEnd():CCommand()
{
	memset(&m_cashOrder,0x00,sizeof(m_cashOrder));
	memset(&m_cashIntoInfo,0x00,sizeof(m_cashIntoInfo));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInEnd::~CBRHCashInEnd(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInEnd::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInEnd::ExecuteCommand()
{
	tBncDevReturn  p_psStatus;

	long lCompleteCode = BNC_Stop(&m_cashIntoInfo,&p_psStatus);

	if(BRH_NO_ERROR == lCompleteCode){
		
		if(GetCashOrder(&m_cashOrder)){

			m_cashIntoInfo.dwCashIntoAmount = m_cashOrder.denomination.amount;

			for(int i = 0; i< m_cashOrder.denomination.size ; i++){
				T_XfsDenominationItem item = m_cashOrder.denomination.items[i];

				map<UINT,CASH_BOX_INFO>::iterator it = mapCashBoxInfo.find(item.unit);
				// �߼���Ŷ�Ӧ
				if(it != mapCashBoxInfo.end()){
					// ��Ǯ���뵽�ĸ�����
					T_XfsPcuName* pBoxName = &(it->second.PhysicalName);
					if(strcmp((char*)pBoxName,BNR_MODULE_CASH_BOX_NAME) == 0 ){// ������
						switch(it->second.cashType.value){
						case 100:
							m_cashIntoInfo.wCashBox_CNY1_IntoCount = item.count;
							break;
						case 500:
							m_cashIntoInfo.wCashBox_CNY5_IntoCount = item.count;
							break;
						case 1000:
							m_cashIntoInfo.wCashBox_CNY10_IntoCount = item.count;
							break;
						case 2000:
							m_cashIntoInfo.wCashBox_CNY20_IntoCount = item.count;
							break;
						case 5000:
							m_cashIntoInfo.wCashBox_CNY50_IntoCount = item.count;
							break;
						case 10000:
							m_cashIntoInfo.wCashBox_CNY100_IntoCount = item.count;
							break;
						}
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0 ){// ѭ����A
						m_cashIntoInfo.wRecyclerAIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0 ){// ѭ����B
						m_cashIntoInfo.wRecyclerBIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0 ){// ѭ����C
						m_cashIntoInfo.wRecyclerCIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0 ){// ѭ����D
						m_cashIntoInfo.wRecyclerDIntoCount = item.count;
					}
				}
			}
		}
	}

	OnComplete(lCompleteCode);
	BRH_SetAcceptFlag(false);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH�˳����ν��ܵ����г�Ʊ     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHCashInEnd::GetBRHResponse(tBncEnCashInfo* intoInfo)
{
	try {
		if(intoInfo == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*intoInfo = m_cashIntoInfo;
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHEject::CBRHEject():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHEject::~CBRHEject(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEject::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEject::ExecuteCommand()
{
	tBncDevReturn *p_psStatus;
	long lCompleteCode = BNC_Refund(p_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH�˳����ν��ܵ����г�Ʊ     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHEject::GetBRHResponse(UINT32* amount)
{
	try {
		if(amount == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*amount = m_amount;
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispenseRequest::CBRHDispenseRequest(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus):CCommand()
{
	memcpy(&m_pChangeNum, pChangeNum, sizeof(tBncChangeNum));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispenseRequest::~CBRHDispenseRequest(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHDispenseRequest::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHDispenseRequest::ExecuteCommand()
{
	tBncDevReturn *pDevStatus;
	long lCompleteCode = BNC_ChangeBill(&m_pChangeNum,pDevStatus);
	if(lCompleteCode == BRH_NO_ERROR){
		T_XfsCashOrder order;
		if(GetCashOrder(&order)){
			m_changeResult.dwOutAmount = order.denomination.amount;

			for(int i = 0; i < order.denomination.size;i++){
				T_XfsDenominationItem item = order.denomination.items[i];
				map<UINT,CASH_BOX_INFO>::iterator it = mapCashBoxInfo.find(item.unit);
				if(it != mapCashBoxInfo.end()){
					T_XfsPcuName* pBoxName = &(it->second.PhysicalName);
					if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0 ){// ѭ����A
						m_changeResult.wRecyclerAOutCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0 ){// ѭ����B
						m_changeResult.wRecyclerBOutCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0 ){// ѭ����C
						m_changeResult.wRecyclerCOutCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0 ){// ѭ����D
						m_changeResult.wRecyclerDOutCount = item.count;
					}
				}
			}
		}
	}


	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ������ϸ��Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBRHDispenseRequest::GetResponse(CHANGE_RESULT* result){
	if(NULL != result){
		*result = m_changeResult;
	}
	else{
		//throw CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR,_T(""));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHPresent::CBRHPresent():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHPresent::~CBRHPresent(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHPresent::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHPresent::ExecuteCommand(void)
{
	//long lCompleteCode = Present(&m_amount);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH���ܳ�Ʊ���     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHPresent::GetBRHResponse(UINT32* amount)
{
	try {
		if(amount == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*amount = m_amount;
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispense::CBRHDispense(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus):CCommand()
{
	memcpy(&m_pChangeNum, pChangeNum, sizeof(tBncChangeNum));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispense::~CBRHDispense(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHDispense::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHDispense::ExecuteCommand()
{
	tBncDevReturn *pDevStatus;
	long lCompleteCode = BNC_ChangeBill(&m_pChangeNum,pDevStatus);
	if (!lCompleteCode){
		return lCompleteCode;
	}
	//lCompleteCode = Present(&m_amount);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHRetract::CBRHRetract():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHRetract::~CBRHRetract(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHRetract::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHRetract::ExecuteCommand()
{
	tBncDevReturn p_psStatus;
	long lCompleteCode = BNC_Cancel(&p_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH���ܳ�Ʊ���     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHRetract::GetBRHResponse(UINT32* amount)
{
	try {
		if(amount == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*amount = m_amount;
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryCashUnit::CBRHQueryCashUnit():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryCashUnit::~CBRHQueryCashUnit(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHQueryCashUnit::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHQueryCashUnit::ExecuteCommand()
{
	tBncBillBoxInfo pBillBoxInfo;
	tBncDevReturn pDevStatus;
	//m_QueryCashUnit.logicalCashUnitList.maxSize = NBMAXLCU;
	//m_QueryCashUnit.physicalCashUnitList.maxSize =  NBMAXPCU;
	long lCompleteCode = BNC_GetBillBoxInfo(&pBillBoxInfo,&pDevStatus);
	//if(0 == lCompleteCode){
	//	for(int i = 0;i < m_QueryCashUnit.logicalCashUnitList.size;i++){
	//		T_XfsLogicalCashUnit LogicalCashUnit = m_QueryCashUnit.logicalCashUnitList.items[i];
	//		CASH_BOX_INFO boxInfo;

	//		boxInfo.cashType = LogicalCashUnit.cashType;
	//		boxInfo.logicalNumber = LogicalCashUnit.number;
	//		boxInfo.TotalCount    = LogicalCashUnit.count;
	//		memcpy(boxInfo.PhysicalName,LogicalCashUnit.physicalCashUnit->name,SIZE_OF_PHYSICAL_NAME);

	//		mapCashBoxInfo[boxInfo.logicalNumber] = boxInfo;
	//	}
	//}
	memcpy(&m_QueryCashUnit, &pBillBoxInfo, sizeof(tBncBillBoxInfo));
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH���ܳ�Ʊ���     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHQueryCashUnit::GetBRHResponse(tBncBillBoxInfo* queryCashUnit)
{
	try {
		if(queryCashUnit == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(queryCashUnit, &m_QueryCashUnit, sizeof(tBncBillBoxInfo));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateCashUnit::CBRHUpdateCashUnit(UINT uiSlotID,tBncBillBoxInfo *pBillBoxInfo):CCommand()
{
	m_uiSlotID = uiSlotID;
	m_pBillBoxInfo = pBillBoxInfo;
	//memcpy(&m_UpdateCashUnit, updateCashUnit, sizeof(BRH_CMD_UPDATE_CASHUNIT));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateCashUnit::~CBRHUpdateCashUnit(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHUpdateCashUnit::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHUpdateCashUnit::ExecuteCommand()
{

	tBncDevReturn *pDevStatus;
	long lCompleteCode = BNC_SetBillBoxInfo(m_uiSlotID, m_pBillBoxInfo, pDevStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryDenominations::CBRHQueryDenominations(tBncSetInhibitList *p_psBuyTicketSetInhibitList, 
												tBncSetInhibitList *p_psRechargeSetInhibitList):CCommand()
{
	m_psBuyTicketSetInhibitList = p_psBuyTicketSetInhibitList;
	m_psRechargeSetInhibitList = p_psRechargeSetInhibitList;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryDenominations::~CBRHQueryDenominations(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHQueryDenominations::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHQueryDenominations::ExecuteCommand()
{
	//m_QueryDenominations.maxSize = MAX_NR_OF_DENOMINATION;
	long lCompleteCode = BNC_SetPrevalue(m_psBuyTicketSetInhibitList, m_psRechargeSetInhibitList, m_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�BRH������Ϣ

@param      (o)long* amount    BRH���ܳ�Ʊ���     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CBRHQueryDenominations::GetBRHResponse(tBncSetInhibitList *BuyTicketSetInhibitList, 
											tBncSetInhibitList *RechargeSetInhibitList,
											tBncDevReturn * Status)
{
	try {
		if(RechargeSetInhibitList == NULL){
			//throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(BuyTicketSetInhibitList, &m_psBuyTicketSetInhibitList, sizeof(tBncSetInhibitList));
			memcpy(Status, &m_psStatus, sizeof(tBncDevReturn));
			memcpy(RechargeSetInhibitList, &m_psRechargeSetInhibitList, sizeof(tBncSetInhibitList));
		}
	}
	//catch(CSysException&) {
	//	throw;
	//}
	catch(...) {
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��  

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateDenominations::CBRHUpdateDenominations(tBncSetInhibitList *p_psBuyTicketSetInhibitList, tBncSetInhibitList *p_psRechargeSetInhibitList):CCommand()
{
	memcpy(&m_psBuyTicketSetInhibitList, p_psBuyTicketSetInhibitList, sizeof(tBncSetInhibitList));
	memcpy(&m_psRechargeSetInhibitList, p_psRechargeSetInhibitList, sizeof(tBncSetInhibitList));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateDenominations::~CBRHUpdateDenominations(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж������Ƿ���Ч

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHUpdateDenominations::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHUpdateDenominations::ExecuteCommand()
{
	tBncDevReturn * p_psStatus;
	long lCompleteCode = BNC_SetPrevalue(&m_psBuyTicketSetInhibitList, m_psRechargeSetInhibitList, p_psStatus);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

void  BRH_SetAcceptFlag(bool flag)
{
	isAccepting = flag;
}

bool GetCashOrder(T_XfsCashOrder* pCashOrder){
	if(NULL != pCashOrder){
		*pCashOrder = cashOrder;
		return true;
	}

	return false;
}
////////////////////////////////////////////////////////////////////////////
///**
// @brief		���캯��
//
// @param		BH_FACEVALUE_TABLE& bhTable
//
// @retval	��  
//
// @exception	��
//*/
//////////////////////////////////////////////////////////////////////////
CBRHOpenAndValidate::CBRHOpenAndValidate(tBncSetInhibitList& brhTable):CCommand()
{
	//done_phase = DONE_P()SE_READY;
	//memcpy(&m_BuyTicketSetInhibitList,&brhTable,sizeof(tBncSetInhibitList));
	m_BuyTicketSetInhibitList.iNumOfItems = brhTable.iNumOfItems;
	m_BuyTicketSetInhibitList.psBncCashInfo = new tBncSetInhibitItem[brhTable.iNumOfItems];
	memcpy(m_BuyTicketSetInhibitList.psBncCashInfo,brhTable.psBncCashInfo,m_BuyTicketSetInhibitList.iNumOfItems*sizeof(tBncSetInhibitItem));
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		���캯��

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpenAndValidate::CBRHOpenAndValidate():CCommand()
{
	//done_phase CCommandHASE_READY;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��������

 @param		��

 @retval	��  

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpenAndValidate::~CBRHOpenAndValidate()
{
	if (m_BuyTicketSetInhibitList.psBncCashInfo)
	{
		delete[] m_BuyTicketSetInhibitList.psBncCashInfo;
		m_BuyTicketSetInhibitList.psBncCashInfo = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ִ������

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHOpenAndValidate::ExecuteCommand(void)
{
	long lRet = 1;
	lRet = OpenAndValidateFunction();
	//this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		ȡ��ִ�н��

 @param		(o)BH_STATUS *res	ֽ�ҽ���ģ��״̬�����ṹ

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
////////////////////////////////////////////////////////////////////////
long CBRHOpenAndValidate::GetResponse(BRH_STATUS* rsp)
{
	try {
		*rsp = m_bhStatus;
	}
	catch (...){
		return CBRHException::ERR_OTHERS;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		MEI ȡ��ִ�н��

 @param		(o)BH_STATUS *res	ֽ�ҽ���ģ��״̬�����ṹ

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
////////////////////////////////////////////////////////////////////////
long CBRHOpenAndValidate::GetResponse(tBncCashInfo* rsp)
{
	try {
		*rsp = m_feedback;
	}
	catch (...){
		return CBRHException::ERR_OTHERS;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		��ֽ�ҽ���ģ������״̬

 @param		��

 @retval	long  0 : �ɹ�	��0 : ʧ��

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHOpenAndValidate::OpenAndValidateFunction()
{
	long errCode = 0;


	// ���¿ɽ��������Ϣ
	CBRHUpdateDenominations setDenominations(&m_BuyTicketSetInhibitList, &RechargeSetInhibitList);
	errCode = setDenominations.ExecuteCommand();
	if(errCode != BRH_NO_ERROR){
		return errCode;
	}

	// 20180120,lichao:���ڶ��̣߳���ֹֹͣ���������ٿ�ʼ��������֮ǰ������
	if(isStopAccept){
		return CBRHHardwareException::ERR_CANCELLED;
	}

	if (!isAccepting){
		BRH_SetAcceptFlag(true);
		// ��ʼ����ֽ��

		CBRHCashInStart startCmd;
		errCode = startCmd.ExecuteCommand();
		if (errCode != BRH_NO_ERROR && errCode != XFS_E_CDR_CASHIN_ACTIVE){
			BRH_SetAcceptFlag(false);
			return errCode;
		}
	}

	// ����ֽ��
	CBRHCashIn cashInCmd;
	errCode = cashInCmd.ExecuteCommand();
	if (errCode == BRH_NO_ERROR){
		cashInCmd.GetBRHResponse(&m_feedback,&m_return);
		BankNoteAndCoinType_t tmpType;
		switch (m_feedback.psBncCashInfo[m_feedback.iNumOfItems-1].iValue) {
		case 100: 
			tmpType = Banknote1Yuan;
			break;
		case 500: 
			tmpType = Banknote5Yuan;
			break;
		case 1000:
			tmpType = Banknote10Yuan;
			break;
		case 2000: 
			tmpType = Banknote20Yuan;
			break;
		case 5000: 
			tmpType = Banknote50Yuan;
			break;
		case 10000: 
			tmpType = Banknote100Yuan;
			break;
		default: 
			tmpType = VALUE_UNKNOWN;
		}
		if(TRUE/*IsAcceptFaceValue(tmpType)*/){	// �����������յ����
			m_bhStatus.bh_faceValue = tmpType;
			m_feedback.psBncCashInfo[m_feedback.iNumOfItems-1].iValue = tmpType;
			m_feedback.psBncCashInfo[m_feedback.iNumOfItems-1].iCount = 1;
			//return errCode;
		}
		else{
			CBRHCashInRollback refCmd;
			errCode = refCmd.ExecuteCommand();					// �˳���ǰ���յ�����
			if (errCode==BRH_NO_ERROR){
				errCode = BRH_EJECT_SUCCESS;
			}
		}
	}
	if(isStopAccept){
		errCode = CBRHHardwareException::ERR_CANCELLED;
	}
	OnComplete(errCode);
	return errCode;
}

////////////////////////////////////////////////////////////////////////////
///**
// @brief		��ȡ��ֵ
//
// @param		��
//
// @retval	long  0 : �ɹ�	��0 : ʧ��
//
// @exception	��
//*/
////////////////////////////////////////////////////////////////////////////
//long CBHOpenAndValidate::GetFace()
//{
//	
//	long errCode = 0;
//
//	/*
//	// 1.��ѯ��ֵ
//	isBHAbort = false;
//	while (!isBHAbort)
//	{
//		// 1.1.��ȡ״̬
//		BH_STS sts;
//		CBHGetSts stsCmd;
//		errCode = stsCmd.ExecuteCommand();
//		if (errCode != SP_SUCCESS)
//		{
//			return errCode;
//		}
//		stsCmd.GetResponse(&sts);
//
//		// 1.2.����ԭ��
//		bool need_return = false;
//		if (!sts.reject.empty())
//		{
//			need_return = true;
//		}
//
//		// 1.3.��ȡ��ֵ
//		if (!need_return && sts.IsValidFaceValue())
//		{
//			CBHGetFaceValue faceCmd(sts.valueNo);
//			errCode = faceCmd.ExecuteCommand();
//			if (errCode != SP_SUCCESS)
//			{
//				need_return = true;
//			}
//			else
//			{
//				faceCmd.GetResponse(&m_bhStatus.bh_faceValue);
//				// �ж��Ƿ���������յ�ֽ�����
//				if (IsAcceptFaceValue(m_bhStatus.bh_faceValue.faceValue)){
//					done_phase |= DONE_PHASE_ACT_GET_FACE;
//				}
//				else
//				{
//					need_return = true;
//					sts.reject.push_back(BH_REJECT::REJECT_BH_NO_PERMISSION);
//				}				
//			}
//		}
//
//		// 1.4.������ѯ
//		if (!need_return && (!sts.IsValidFaceValue() || m_bhStatus.bh_faceValue.faceValue == UNKNOWN))
//		{
//			Sleep(30);
//			continue;
//		}
//
//		// 1.5.�˳��ж�
//		if (need_return)
//		{
//			CBHReturnOne returnCmd(true);
//			errCode = returnCmd.ExecuteCommand();
//			if (errCode != SP_SUCCESS)
//			{
//				return errCode;
//			}
//			done_phase |= DONE_PHASE_ACT_RETURN_ONE;
//		}
//
//		// 1.6.�ɹ�������״̬
//		done_phase |= DONE_PHASE_SUCCESS;
//		m_bhStatus.bh_sts = sts;
//		return 0;
//	}
//
//	// 2.�ֶ���ֹ��ѯ
//	done_phase |= DONE_PHASE_ACT_ABORT;
//	*/
//	
//	return 0;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      �Ƿ��ǿɽ��ܵ���ֵ
//
//@param      BankNoteAndCoinType_t& thisFaceValue
//
//@retval     bool
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
bool CBRHOpenAndValidate::IsAcceptFaceValue(BankNoteAndCoinType_t& thisFaceValue)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      �������������Ϣ���캯��

@param      BNR_MODULE_ID boxId,UINT faceValue

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCashType::CBRHSetCashType(BNR_MODULE_ID boxId,UINT faceValue){
	m_boxId = boxId;
	// �����Ϣ
	m_cashTypeInfo.value = faceValue;
	char currency [4] = {"CNY"};
	strcpy(m_cashTypeInfo.currencyCode, currency);
	m_cashTypeInfo.variant = 'AB';
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCashType::~CBRHSetCashType(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ģ�������Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetCashType::ExecuteCommand(){
	return SP_SUCCESS;

	//long lCompleteCode = SetCashType(m_boxId,&m_cashTypeInfo);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      BNR_MODULE_ID boxId,UINT faceValue

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHEncash::CBRHEncash(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBRHEncash::~CBRHEncash(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEncash::ExecuteCommand(){
	return SP_SUCCESS;
}