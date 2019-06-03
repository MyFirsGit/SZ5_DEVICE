#include "stdafx.h"
#include "TOKENMessage.h"
#include "TOKENHardwareException.h"


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommOpen::CTOKENCommOpen(TOKEN_CMD_COMM_OPEN &command) : CTOKENCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommOpen::~CTOKENCommOpen()
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
long CTOKENCommOpen::IsValidCommand()
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
long CTOKENCommOpen::ExecuteCommand()
{
	long lCompleteCode = CommOpen(m_command.uiCommPort, m_command.uiBaudRate);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_INIT cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENInit::CTOKENInit(TOKEN_CMD_INIT command) : CTOKENCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENInit::~CTOKENInit(void)
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
long CTOKENInit::IsValidCommand()
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
long CTOKENInit::ExecuteCommand()
{
	long lCompleteCode = Init(m_command, m_ucRetractNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_INIT* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENInit::GetTOKENResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_ucRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_RESET cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENReset::CTOKENReset() : CTOKENCommands()
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
CTOKENReset::~CTOKENReset(void)
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
long CTOKENReset::IsValidCommand()
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
long CTOKENReset::ExecuteCommand()
{
	long lCompleteCode = Reset();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetVersion::CTOKENGetVersion(void) : CTOKENCommands()
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
CTOKENGetVersion::~CTOKENGetVersion(void)
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
long CTOKENGetVersion::IsValidCommand()
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
long CTOKENGetVersion::ExecuteCommand()
{
	TOKEN_NAMESPACE::ModuleInfo tmpInfo;
	long lCompleteCode = GetVersion(tmpInfo);
	if (lCompleteCode==0){
		memcpy(&m_response.ucModNum, &tmpInfo.ucModNum, sizeof(m_response.ucModNum));
		memcpy(&m_response.ucFwVer, &tmpInfo.ucFwVer, sizeof(m_response.ucFwVer));
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_GET_VERSION* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENGetVersion::GetTOKENResponse(TOKEN_RSP_GET_VERSION* response)
{
	try {
		if(response == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response->ucModNum, m_response.ucModNum, sizeof(m_response.ucModNum));
			memcpy(response->ucFwVer, m_response.ucFwVer, sizeof(m_response.ucFwVer));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetDevStatus::CTOKENGetDevStatus(void) : CTOKENCommands()
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
CTOKENGetDevStatus::~CTOKENGetDevStatus(void)
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
long CTOKENGetDevStatus::IsValidCommand()
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
long CTOKENGetDevStatus::ExecuteCommand()
{
	TOKEN_NAMESPACE::ModuleStatus tmpStatus;

	long lCompleteCode = GetDevStatus(tmpStatus);
	if (TOKEN_RSP_OK == lCompleteCode) {
		// 返回模块的状态信息
		m_response.bBoxANearEmpty = ((tmpStatus.ucSensorStatus & 1) == 1);
		m_response.bBoxAExist = ((tmpStatus.ucSensorStatus & 2) == 2);
		m_response.bBoxBNearEmpty = ((tmpStatus.ucSensorStatus & 4) == 4);
		m_response.bBoxBExist = ((tmpStatus.ucSensorStatus & 8) == 8);
		m_response.bBoxAEmpty = ((tmpStatus.ucSensorStatus & 16) == 16);
		m_response.bBoxBEmpty = ((tmpStatus.ucSensorStatus & 32) == 32);
		m_response.bCardInReadArea = ((tmpStatus.ucSensorStatus & 64) == 64);

		m_response.bHopperAException = ((tmpStatus.ucSubModuleStatus & 2) == 2);
		m_response.bHopperBException = ((tmpStatus.ucSubModuleStatus & 4) == 4);
		m_response.bHasCardAtTemporaryA = ((tmpStatus.ucSubModuleStatus & 8) == 8);
		m_response.bHasCardAtTemporaryB = ((tmpStatus.ucSubModuleStatus & 16) == 16);
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_GET_DEV_STATUS* response    

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENGetDevStatus::GetTOKENResponse(TOKEN_RSP_GET_DEV_STATUS* response)
{
	try {
		if(response == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
	       *response = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_CARD_OUT cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCardOut::CTOKENCardOut(TOKEN_BOX_NO boxNo) : CTOKENCommands()
{
	m_boxNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCardOut::~CTOKENCardOut(void)
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
long CTOKENCardOut::IsValidCommand()
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
long CTOKENCardOut::ExecuteCommand()
{
	long lCompleteCode = CardOut(m_boxNo, m_ucTransportNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_CARD_OUT* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENCardOut::GetTOKENResponse(UINT* uiTransportNum)
{
	try {
		if(uiTransportNum == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiTransportNum = m_ucTransportNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENSendCard::CTOKENSendCard(void) : CTOKENCommands()
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
CTOKENSendCard::~CTOKENSendCard(void)
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
long CTOKENSendCard::IsValidCommand()
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
long CTOKENSendCard::ExecuteCommand()
{
	long lCompleteCode = SendCard(m_ucTransportNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_SEND_CARD* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENSendCard::GetTOKENResponse(UINT* uiTransportNum)
{
	try {
		if(uiTransportNum == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiTransportNum = m_ucTransportNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENRetractCard::CTOKENRetractCard(void) : CTOKENCommands()
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
CTOKENRetractCard::~CTOKENRetractCard(void)
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
long CTOKENRetractCard::IsValidCommand()
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
long CTOKENRetractCard::ExecuteCommand()
{
	long lCompleteCode = RetractCard(m_ucRetractNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_RETRACT_CARD* response     

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENRetractCard::GetTOKENResponse(UINT* uiRetractNum)
{
	try {
		if(uiRetractNum == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*uiRetractNum = m_ucRetractNum;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCleanOut::CTOKENCleanOut(TOKEN_BOX_NO boxNo) : CTOKENCommands()
{
	m_ucBoxNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCleanOut::~CTOKENCleanOut(void)
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
long CTOKENCleanOut::IsValidCommand()
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
long CTOKENCleanOut::ExecuteCommand()
{
	long lCompleteCode = 0;
	lCompleteCode = CleanOut(m_ucBoxNo);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetSN::CTOKENGetSN(TOKEN_BOX_NO boxNo): CTOKENCommands()
{
	m_boxNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetSN::~CTOKENGetSN(void)
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
long CTOKENGetSN::IsValidCommand()
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
long CTOKENGetSN::ExecuteCommand()
{
	long lCompleteCode = RfidReadID(m_boxNo, m_boxId);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      获取返回信息

@param      (o)TOKEN_RSP_GET_SN* response 

@retval     void     

@exception  CTCMException
*/
//////////////////////////////////////////////////////////////////////////
void CTOKENGetSN::GetTOKENResponse(char* response)
{
	try {
		if(response == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(response, m_boxId, sizeof(m_boxId));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommClose::CTOKENCommClose() : CTOKENCommands()
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
CTOKENCommClose::~CTOKENCommClose()
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
long CTOKENCommClose::IsValidCommand()
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
long CTOKENCommClose::ExecuteCommand()
{
	long lCompleteCode = CommClose();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENReadRFIDInfo::CTOKENReadRFIDInfo(unsigned char boxNo){
	m_RfidInfo.ucRfidNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENReadRFIDInfo::~CTOKENReadRFIDInfo(){

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      读取票箱RFID执行命令

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENReadRFIDInfo::ExecuteCommand()
{
	long lRet = DoRead();

	this->OnComplete(lRet);
	return lRet;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      读取票箱RFID

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENReadRFIDInfo::DoRead()
{
	long lRet =0;
	TOKEN_NAMESPACE::BlockData tmpData = {0};
	lRet = RfidReadBlock(m_RfidInfo.ucRfidNo, 8, tmpData);
	if (TOKEN_RSP_OK==lRet){
		memcpy(m_RfidInfo.rfidData.uBoxNo, &tmpData, sizeof(m_RfidInfo.rfidData.uBoxNo));
	}

	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取读取RFID的反馈

@param      tTIMRFIDInfo* rspRfid

@retval     long lRet;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENReadRFIDInfo::GetTOKENResponse(TOKEN_READ_WRITE_RFID* rspRfid){
	try{
		if(rspRfid == NULL){
			throw CTOKENException(CTOKENException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(rspRfid,&m_RfidInfo,sizeof(TOKEN_READ_WRITE_RFID));
		}
			
		return 0;
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("内存空间赋值错误！"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID构造函数

@param      BYTE bTicketBoxNo 箱子类型
			tTIMRFIDInfo& pTimRfidInfo RFID信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENWriteRFIDInfo::CTOKENWriteRFIDInfo(TOKEN_READ_WRITE_RFID* pRfidInfo){
	memcpy(&m_RfidInfo, pRfidInfo, sizeof(TOKEN_READ_WRITE_RFID));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTOKENWriteRFIDInfo::~CTOKENWriteRFIDInfo()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID执行命令函数

@param      无

@retval     long 反馈结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENWriteRFIDInfo::ExecuteCommand()
{
	long lRet = DoWrite();
	this->OnComplete(lRet);
	return lRet;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      写RFID执行函数

@param      无

@retval     long 反馈结果

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENWriteRFIDInfo::DoWrite()
{
	TOKEN_NAMESPACE::BlockData tmpData = {0};
	memcpy(&tmpData, m_RfidInfo.rfidData.uBoxNo, sizeof(m_RfidInfo.rfidData.uBoxNo));
	RfidWriteBlock(m_RfidInfo.ucRfidNo, 8, tmpData);

	return 0;
}

