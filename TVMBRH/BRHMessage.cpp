#include "StdAfx.h"
#include "BRHMessage.h"
//#include "SysInfo.h"
//#include "DCR4000Comm.h"
//#include "MaintenanceInfo.h"
#include "BRHException.h"
#include "BRHHardwareException.h"
bool isAccepting = false;
bool isStopAccept = false;

// 箱子信息
map<UINT,CASH_BOX_INFO> mapCashBoxInfo;

// 面额信息
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
@brief      构造函数

@param      (i)LPBRH_RSP_OPEN     RSPOpenReader

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpen::CBRHOpen():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpen::~CBRHOpen(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHOpen::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHClose::CBRHClose():CCommand()
{						   
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHClose::~CBRHClose(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHClose::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
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
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHReset::~CBRHReset(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReset::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHReboot::CBRHReboot():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHReboot::~CBRHReboot(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReboot::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetStatus::CBRHGetStatus():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetStatus::~CBRHGetStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetStatus::ExecuteCommand()
{
	tBncDevReturn  p_psStatus;
	long lCompleteCode = BNC_GetStatus(&p_psStatus);
	// T_XfsCdrStatus结构与BRH_RSP_GET_STATUS结构实际定义是相同的。
	memcpy(&m_status, &p_psStatus, sizeof(tBncDevReturn));
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      得到BRH返回信息

@param      (o)BRH_RSP_GET_STATUS* status    BRH状态     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetSystemStatus::CBRHGetSystemStatus():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetSystemStatus::~CBRHGetSystemStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetSystemStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)BRH_RSP_GET_SYSTEM_STATUS* status    BRH状态     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancel::CBRHCancel():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancel::~CBRHCancel(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCancel::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetCapabilities::CBRHGetCapabilities():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetCapabilities::~CBRHGetCapabilities(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetCapabilities::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetCapabilities::ExecuteCommand(void)
{
	//T_XfsCapabilities tmp_Capabilities;
	//long lCompleteCode = GetCapabilities(&tmp_Capabilities);
	//// T_XfsCapabilities结构与BRH_RSP_GET_CAPABLILITIES结构实际定义是相同的。
	//memcpy(&m_Capabilities, &tmp_Capabilities, sizeof(T_XfsCapabilities));
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      得到BRH返回信息

@param      (o)BRH_RSP_GET_CAPABLILITIES* Capabilities    BRH性能     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCapabilities::CBRHSetCapabilities(BRH_CMD_SET_CAPABLILITIES* Capabilities):CCommand()
{
	memcpy(&m_Capabilities, Capabilities, sizeof(BRH_CMD_SET_CAPABLILITIES));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCapabilities::~CBRHSetCapabilities(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetCapabilities::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetTransactionStatus::CBRHGetTransactionStatus():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHGetTransactionStatus::~CBRHGetTransactionStatus(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetTransactionStatus::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHGetTransactionStatus::ExecuteCommand(void)
{
	//T_TransactionStatus tmp_Status;
	//long lCompleteCode = GetTransactionStatus(&tmp_Status);
	//// T_XfsCapabilities结构与BRH_RSP_GET_CAPABLILITIES结构实际定义是相同的。
	//memcpy(&m_Status, &tmp_Status, sizeof(T_TransactionStatus));
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return SP_SUCCESS;

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      得到BRH返回信息

@param      (o)BRH_RSP_GET_CAPABLILITIES* Capabilities    BRH性能     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetDateTime::CBRHSetDateTime(time_t dateTime):CCommand()
{
	memcpy(&m_Time, &dateTime, sizeof(time_t));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetDateTime::~CBRHSetDateTime(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSetDateTime::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
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
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHEmpty::~CBRHEmpty(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEmpty::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      取得反馈结果

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBRHEmpty::GetResponse(tBncChangeNum* result){
	if(NULL != result){
		*result = m_result;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHReject::CBRHReject():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHReject::~CBRHReject(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHReject::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH接受钞票金额     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSelfTest::CBRHSelfTest():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSelfTest::~CBRHSelfTest(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHSelfTest::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHPark::CBRHPark():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHPark::~CBRHPark(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHPark::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancelWaitingCashTaken::CBRHCancelWaitingCashTaken():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCancelWaitingCashTaken::~CBRHCancelWaitingCashTaken(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCancelWaitingCashTaken::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInStart::CBRHCashInStart():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInStart::~CBRHCashInStart(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInStart::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashIn::CBRHCashIn():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashIn::~CBRHCashIn(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashIn::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH接受钞票金额     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInRollback::CBRHCashInRollback():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInRollback::~CBRHCashInRollback(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInRollback::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH退出本次接受的所有钞票     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInEnd::CBRHCashInEnd():CCommand()
{
	memset(&m_cashOrder,0x00,sizeof(m_cashOrder));
	memset(&m_cashIntoInfo,0x00,sizeof(m_cashIntoInfo));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHCashInEnd::~CBRHCashInEnd(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHCashInEnd::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
				// 逻辑编号对应
				if(it != mapCashBoxInfo.end()){
					// 看钱进入到哪个箱子
					T_XfsPcuName* pBoxName = &(it->second.PhysicalName);
					if(strcmp((char*)pBoxName,BNR_MODULE_CASH_BOX_NAME) == 0 ){// 回收箱
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
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0 ){// 循环鼓A
						m_cashIntoInfo.wRecyclerAIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0 ){// 循环鼓B
						m_cashIntoInfo.wRecyclerBIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0 ){// 循环鼓C
						m_cashIntoInfo.wRecyclerCIntoCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0 ){// 循环鼓D
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH退出本次接受的所有钞票     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHEject::CBRHEject():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHEject::~CBRHEject(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEject::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH退出本次接受的所有钞票     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispenseRequest::CBRHDispenseRequest(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus):CCommand()
{
	memcpy(&m_pChangeNum, pChangeNum, sizeof(tBncChangeNum));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispenseRequest::~CBRHDispenseRequest(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHDispenseRequest::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
					if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_A_NAME) == 0 ){// 循环鼓A
						m_changeResult.wRecyclerAOutCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_B_NAME) == 0 ){// 循环鼓B
						m_changeResult.wRecyclerBOutCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_C_NAME) == 0 ){// 循环鼓C
						m_changeResult.wRecyclerCOutCount = item.count;
					}
					else if(strcmp((char*)pBoxName,BNR_MODULE_RECYCLER_BOX_D_NAME) == 0 ){// 循环鼓D
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
@brief      获取找零详细信息

@param      无

@retval     无

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHPresent::CBRHPresent():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHPresent::~CBRHPresent(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHPresent::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH接受钞票金额     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispense::CBRHDispense(tBncChangeNum *pChangeNum, tBncDevReturn *pDevStatus):CCommand()
{
	memcpy(&m_pChangeNum, pChangeNum, sizeof(tBncChangeNum));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHDispense::~CBRHDispense(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHDispense::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHRetract::CBRHRetract():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHRetract::~CBRHRetract(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHRetract::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH接受钞票金额     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryCashUnit::CBRHQueryCashUnit():CCommand()
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryCashUnit::~CBRHQueryCashUnit(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHQueryCashUnit::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH接受钞票金额     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
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
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateCashUnit::~CBRHUpdateCashUnit(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHUpdateCashUnit::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      构造函数

@param      无  

@retval     无     

@exception  无
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
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHQueryDenominations::~CBRHQueryDenominations(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHQueryDenominations::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
@brief      得到BRH返回信息

@param      (o)long* amount    BRH接受钞票金额     

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
		//throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无  

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateDenominations::CBRHUpdateDenominations(tBncSetInhibitList *p_psBuyTicketSetInhibitList, tBncSetInhibitList *p_psRechargeSetInhibitList):CCommand()
{
	memcpy(&m_psBuyTicketSetInhibitList, p_psBuyTicketSetInhibitList, sizeof(tBncSetInhibitList));
	memcpy(&m_psRechargeSetInhibitList, p_psRechargeSetInhibitList, sizeof(tBncSetInhibitList));

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHUpdateDenominations::~CBRHUpdateDenominations(void)
{
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      判断命令是否有效

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHUpdateDenominations::IsValidCommand()
{
	return SP_SUCCESS;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令

@param      无         

@retval     无     

@exception  无
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
// @brief		构造函数
//
// @param		BH_FACEVALUE_TABLE& bhTable
//
// @retval	无  
//
// @exception	无
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
 @brief		构造函数

 @param		无

 @retval	无  

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
CBRHOpenAndValidate::CBRHOpenAndValidate():CCommand()
{
	//done_phase CCommandHASE_READY;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief		析构函数

 @param		无

 @retval	无  

 @exception	无
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
 @brief		执行命令

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
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
 @brief		取得执行结果

 @param		(o)BH_STATUS *res	纸币接收模块状态解析结构

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
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
 @brief		MEI 取得执行结果

 @param		(o)BH_STATUS *res	纸币接收模块状态解析结构

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
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
 @brief		向纸币接收模块请求状态

 @param		无

 @retval	long  0 : 成功	非0 : 失败

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHOpenAndValidate::OpenAndValidateFunction()
{
	long errCode = 0;


	// 更新可接收面额信息
	CBRHUpdateDenominations setDenominations(&m_BuyTicketSetInhibitList, &RechargeSetInhibitList);
	errCode = setDenominations.ExecuteCommand();
	if(errCode != BRH_NO_ERROR){
		return errCode;
	}

	// 20180120,lichao:由于多线程，防止停止接收命令再开始接收命令之前被处理。
	if(isStopAccept){
		return CBRHHardwareException::ERR_CANCELLED;
	}

	if (!isAccepting){
		BRH_SetAcceptFlag(true);
		// 开始接收纸币

		CBRHCashInStart startCmd;
		errCode = startCmd.ExecuteCommand();
		if (errCode != BRH_NO_ERROR && errCode != XFS_E_CDR_CASHIN_ACTIVE){
			BRH_SetAcceptFlag(false);
			return errCode;
		}
	}

	// 接收纸币
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
		if(TRUE/*IsAcceptFaceValue(tmpType)*/){	// 如果是允许接收的面额
			m_bhStatus.bh_faceValue = tmpType;
			m_feedback.psBncCashInfo[m_feedback.iNumOfItems-1].iValue = tmpType;
			m_feedback.psBncCashInfo[m_feedback.iNumOfItems-1].iCount = 1;
			//return errCode;
		}
		else{
			CBRHCashInRollback refCmd;
			errCode = refCmd.ExecuteCommand();					// 退出当前接收的所有
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
// @brief		获取面值
//
// @param		无
//
// @retval	long  0 : 成功	非0 : 失败
//
// @exception	无
//*/
////////////////////////////////////////////////////////////////////////////
//long CBHOpenAndValidate::GetFace()
//{
//	
//	long errCode = 0;
//
//	/*
//	// 1.轮询面值
//	isBHAbort = false;
//	while (!isBHAbort)
//	{
//		// 1.1.获取状态
//		BH_STS sts;
//		CBHGetSts stsCmd;
//		errCode = stsCmd.ExecuteCommand();
//		if (errCode != SP_SUCCESS)
//		{
//			return errCode;
//		}
//		stsCmd.GetResponse(&sts);
//
//		// 1.2.拒收原因
//		bool need_return = false;
//		if (!sts.reject.empty())
//		{
//			need_return = true;
//		}
//
//		// 1.3.获取面值
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
//				// 判断是否是允许接收的纸币面额
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
//		// 1.4.继续轮询
//		if (!need_return && (!sts.IsValidFaceValue() || m_bhStatus.bh_faceValue.faceValue == UNKNOWN))
//		{
//			Sleep(30);
//			continue;
//		}
//
//		// 1.5.退钞判断
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
//		// 1.6.成功，返回状态
//		done_phase |= DONE_PHASE_SUCCESS;
//		m_bhStatus.bh_sts = sts;
//		return 0;
//	}
//
//	// 2.手动中止轮询
//	done_phase |= DONE_PHASE_ACT_ABORT;
//	*/
//	
//	return 0;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      是否是可接受的面值
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
@brief      设置箱子面额信息构造函数

@param      BNR_MODULE_ID boxId,UINT faceValue

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCashType::CBRHSetCashType(BNR_MODULE_ID boxId,UINT faceValue){
	m_boxId = boxId;
	// 面额信息
	m_cashTypeInfo.value = faceValue;
	char currency [4] = {"CNY"};
	strcpy(m_cashTypeInfo.currencyCode, currency);
	m_cashTypeInfo.variant = 'AB';
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHSetCashType::~CBRHSetCashType(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置模块面额信息

@param      无

@retval     无

@exception  无
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

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHEncash::CBRHEncash(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBRHEncash::~CBRHEncash(){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CBRHEncash::ExecuteCommand(){
	return SP_SUCCESS;
}