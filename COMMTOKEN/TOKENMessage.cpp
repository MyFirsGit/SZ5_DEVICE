#include "stdafx.h"
#include "TOKENMessage.h"
#include "TOKENHardwareException.h"


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommOpen::CTOKENCommOpen(TOKEN_CMD_COMM_OPEN &command) : CTOKENCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommOpen::~CTOKENCommOpen()
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
long CTOKENCommOpen::IsValidCommand()
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
long CTOKENCommOpen::ExecuteCommand()
{
	long lCompleteCode = CommOpen(m_command.uiCommPort, m_command.uiBaudRate);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_INIT cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENInit::CTOKENInit(TOKEN_CMD_INIT command) : CTOKENCommands()
{
	m_command = command;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENInit::~CTOKENInit(void)
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
long CTOKENInit::IsValidCommand()
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
long CTOKENInit::ExecuteCommand()
{
	long lCompleteCode = Init(m_command, m_ucRetractNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_RESET cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENReset::CTOKENReset() : CTOKENCommands()
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
CTOKENReset::~CTOKENReset(void)
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
long CTOKENReset::IsValidCommand()
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
long CTOKENReset::ExecuteCommand()
{
	long lCompleteCode = Reset();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetVersion::CTOKENGetVersion(void) : CTOKENCommands()
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
CTOKENGetVersion::~CTOKENGetVersion(void)
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
long CTOKENGetVersion::IsValidCommand()
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
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetDevStatus::CTOKENGetDevStatus(void) : CTOKENCommands()
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
CTOKENGetDevStatus::~CTOKENGetDevStatus(void)
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
long CTOKENGetDevStatus::IsValidCommand()
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
long CTOKENGetDevStatus::ExecuteCommand()
{
	TOKEN_NAMESPACE::ModuleStatus tmpStatus;

	long lCompleteCode = GetDevStatus(tmpStatus);
	if (TOKEN_RSP_OK == lCompleteCode) {
		// ����ģ���״̬��Ϣ
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
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_CARD_OUT cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCardOut::CTOKENCardOut(TOKEN_BOX_NO boxNo) : CTOKENCommands()
{
	m_boxNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCardOut::~CTOKENCardOut(void)
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
long CTOKENCardOut::IsValidCommand()
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
long CTOKENCardOut::ExecuteCommand()
{
	long lCompleteCode = CardOut(m_boxNo, m_ucTransportNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENSendCard::CTOKENSendCard(void) : CTOKENCommands()
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
CTOKENSendCard::~CTOKENSendCard(void)
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
long CTOKENSendCard::IsValidCommand()
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
long CTOKENSendCard::ExecuteCommand()
{
	long lCompleteCode = SendCard(m_ucTransportNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENRetractCard::CTOKENRetractCard(void) : CTOKENCommands()
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
CTOKENRetractCard::~CTOKENRetractCard(void)
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
long CTOKENRetractCard::IsValidCommand()
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
long CTOKENRetractCard::ExecuteCommand()
{
	long lCompleteCode = RetractCard(m_ucRetractNum);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCleanOut::CTOKENCleanOut(TOKEN_BOX_NO boxNo) : CTOKENCommands()
{
	m_ucBoxNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCleanOut::~CTOKENCleanOut(void)
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
long CTOKENCleanOut::IsValidCommand()
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
long CTOKENCleanOut::ExecuteCommand()
{
	long lCompleteCode = 0;
	lCompleteCode = CleanOut(m_ucBoxNo);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetSN::CTOKENGetSN(TOKEN_BOX_NO boxNo): CTOKENCommands()
{
	m_boxNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENGetSN::~CTOKENGetSN(void)
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
long CTOKENGetSN::IsValidCommand()
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
long CTOKENGetSN::ExecuteCommand()
{
	long lCompleteCode = RfidReadID(m_boxNo, m_boxId);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ������Ϣ

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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)TOKEN_CMD_COMM_OPEN cmdCommOpen

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENCommClose::CTOKENCommClose() : CTOKENCommands()
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
CTOKENCommClose::~CTOKENCommClose()
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
long CTOKENCommClose::IsValidCommand()
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
long CTOKENCommClose::ExecuteCommand()
{
	long lCompleteCode = CommClose();
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENReadRFIDInfo::CTOKENReadRFIDInfo(unsigned char boxNo){
	m_RfidInfo.ucRfidNo = boxNo;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENReadRFIDInfo::~CTOKENReadRFIDInfo(){

}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��RFIDִ������

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ȡƱ��RFID

@param      ��

@retval     ��

@exception  ��
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
@brief      ȡ��ȡRFID�ķ���

@param      tTIMRFIDInfo* rspRfid

@retval     long lRet;

@exception  ��
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
		throw CTOKENException(CTOKENException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дRFID���캯��

@param      BYTE bTicketBoxNo ��������
			tTIMRFIDInfo& pTimRfidInfo RFID��Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENWriteRFIDInfo::CTOKENWriteRFIDInfo(TOKEN_READ_WRITE_RFID* pRfidInfo){
	memcpy(&m_RfidInfo, pRfidInfo, sizeof(TOKEN_READ_WRITE_RFID));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTOKENWriteRFIDInfo::~CTOKENWriteRFIDInfo()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      дRFIDִ�������

@param      ��

@retval     long �������

@exception  ��
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
@brief      дRFIDִ�к���

@param      ��

@retval     long �������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CTOKENWriteRFIDInfo::DoWrite()
{
	TOKEN_NAMESPACE::BlockData tmpData = {0};
	memcpy(&tmpData, m_RfidInfo.rfidData.uBoxNo, sizeof(m_RfidInfo.rfidData.uBoxNo));
	RfidWriteBlock(m_RfidInfo.ucRfidNo, 8, tmpData);

	return 0;
}

