#include "StdAfx.h"
#include "RWMessage.h"
#include "SysInfo.h"
#include "DCR4000Comm.h"
#include "MaintenanceInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

bool isAborted = false;	// �Ƿ���ABORT����

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPRW_CMD_OPEN     cmdOpenReader

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWOpen::CRWOpen(RW_CMD_OPEN cmdOpenReader,RWID& rwid):CRWCommands(rwid)
{
	m_cmdOpen = cmdOpenReader;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWOpen::~CRWOpen(void)
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
long CRWOpen::IsValidCommand()
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
long CRWOpen::ExecuteCommand(void)
{
	long lCompleteCode = Open(m_cmdOpen.port,m_cmdOpen.baud,m_cmdOpen.isUsb);
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
CRWClose::CRWClose(RWID& rwid):CRWCommands(rwid)
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
CRWClose::~CRWClose(void)
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
long CRWClose::IsValidCommand()
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
long CRWClose::ExecuteCommand(void)
{
	long  lCompleteCode = Close();
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
CRWReset::CRWReset(RWID& rwid):CRWCommands(rwid)
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
CRWReset::~CRWReset(void)
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
long CRWReset::IsValidCommand()
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
long CRWReset::ExecuteCommand(void)
{
	long lCompleteCode = Reset(m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_RSP_RESET* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWReset::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWGetStatus::CRWGetStatus(RWID& rwid):CRWCommands(rwid)
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
CRWGetStatus::~CRWGetStatus(void)
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
long CRWGetStatus::IsValidCommand()
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
long CRWGetStatus::ExecuteCommand(void)
{
	long lCompleteCode = GetStatus(m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetStatus::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWAbort::CRWAbort(RWID& rwid):CRWCommands(rwid)
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
CRWAbort::~CRWAbort(void)
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
long CRWAbort::IsValidCommand()
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
long CRWAbort::ExecuteCommand(void)
{
	DisableRead();
	long  lCompleteCode = Abort(m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWAbort::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

RW_API void EnableRead()
{
	isAborted=false;
}

RW_API void DisableRead()
{
	isAborted=true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      �� 

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWStopReadCard::CRWStopReadCard(RWID& rwid):CRWCommands(rwid)
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
CRWStopReadCard::~CRWStopReadCard(void)
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
long CRWStopReadCard::IsValidCommand()
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
long CRWStopReadCard::ExecuteCommand(void)
{
	DisableRead();
	long  lCompleteCode = RW_RSP_OK;
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
CRWStop::CRWStop(RWID& rwid):CRWCommands(rwid)
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
CRWStop::~CRWStop(void)
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
long CRWStop::IsValidCommand()
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
long CRWStop::ExecuteCommand(void)
{
	long lCompleteCode = Stop(m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWStop::GetRWResponse(BYTE* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWSetTime::CRWSetTime(_DATE_TIME datetime,RWID& rwid):CRWCommands(rwid)
{
	m_Input = datetime;
	memset(m_response,0,7);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetTime::~CRWSetTime(void)
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
long CRWSetTime::IsValidCommand()
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
long CRWSetTime::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_Input.Serialize(tranDateTime);
	long lCompleteCode  = SetTime(tranDateTime,m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWSetTime::GetRWResponse(_DATE_TIME* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			pResponse->Deserialize(m_response);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWGetTime::CRWGetTime(RWID& rwid):CRWCommands(rwid)
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
CRWGetTime::~CRWGetTime(void)
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
long CRWGetTime::IsValidCommand()
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
long CRWGetTime::ExecuteCommand(void)
{
	long  lCompleteCode = GetTime(m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetTime::GetRWResponse(_DATE_TIME* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			pResponse->Deserialize(m_response);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWGetMac::CRWGetMac(WORD keyVesion,BYTE* hashData,RWID& rwid):CRWCommands(rwid)
{
	m_keyVersion = keyVesion;
	memcpy(m_hashData,hashData,20);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetMac::~CRWGetMac(void)
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
long CRWGetMac::IsValidCommand()
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
long CRWGetMac::ExecuteCommand(void)
{
	long lCompleteCode = GetMac((BYTE*)&m_keyVersion,m_hashData,m_resMac);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetMac::GetRWResponse(BYTE* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(pResponse,m_resMac,4);
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWGetErrCode::CRWGetErrCode(int retCode,RWID& rwid):CRWCommands(rwid)
{
	m_ErrorCode = retCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetErrCode::~CRWGetErrCode(void)
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
long CRWGetErrCode::IsValidCommand()
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
long CRWGetErrCode::ExecuteCommand(void)
{
	long lCompleteCode = GetErrCode(m_ErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetErrCode::GetRWResponse(int* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_ErrorCode;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
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
CRWGetTpuInfo::CRWGetTpuInfo(RWID& rwid):CRWCommands(rwid)
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
CRWGetTpuInfo::~CRWGetTpuInfo(void)
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
long CRWGetTpuInfo::IsValidCommand()
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
long CRWGetTpuInfo::ExecuteCommand(void)
{
	byte DeviceCode[4];
	byte AppSoftVer[2];
	byte MainSoftVer[2];
	byte HardwareVer[2];
	byte BMACPSAMID[6];
	byte BMACISAMID[6];
	byte ACCPSAMID[6];
	byte ACCISAMID[6];

	// ��ʼ��ֵ
	m_TpuInfo = _rw_rsp_tpu_info();
	DAT::TPUINFO tpuInfo;
	long lCompleteCode = GetTPUInfo(tpuInfo);
	if(lCompleteCode == RW_RSP_OK){
		memcpy(m_TpuInfo.DeviceCode,tpuInfo.DeviceCode,4);
		memcpy(m_TpuInfo.hardwareVersion,tpuInfo.HardwareVer,2);
		memcpy(m_TpuInfo.softwareVersionApp,tpuInfo.AppSoftVer,6);
		//memcpy(m_TpuInfo.softwareVersionMain,tpuInfo.MainSoftVer,2);
		memcpy(m_TpuInfo.accISAMid,tpuInfo.ACCISAMID,sizeof(m_TpuInfo.accISAMid));
		memcpy(m_TpuInfo.accPSAMid,tpuInfo.ACCPSAMID,sizeof(m_TpuInfo.accPSAMid));
		memcpy(m_TpuInfo.bmacISAMid,tpuInfo.BMACISAMID,sizeof(m_TpuInfo.bmacISAMid));
		memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.BMACPSAMID,sizeof(m_TpuInfo.bmacPSAMid));
		//// SAM 1
		//switch(tpuInfo.SAM1appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM1ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM1ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM1ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM1ID,6);
		//	break;
		//default:
		//	break;
		//}
		//
		//// SAM 2
		//switch(tpuInfo.SAM2appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM2ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM2ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM2ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM2ID,6);
		//	break;
		//default:
		//	break;
		//}

		//// SAM 3
		//switch(tpuInfo.SAM3appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM3ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM3ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM3ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM3ID,6);
		//	break;
		//default:
		//	break;
		//}

		//// SAM 4
		//switch(tpuInfo.SAM4appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM4ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM4ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM4ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM4ID,6);
		//	break;
		//default:
		//	break;
		//}

		//// SAM 5
		//switch(tpuInfo.SAM5appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM5ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM5ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM5ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM5ID,6);
		//	break;
		//default:
		//	break;
		//}

		//// SAM 6
		//switch(tpuInfo.SAM6appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM6ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM6ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM6ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM6ID,6);
		//	break;
		//default:
		//	break;
		//}

		//// SAM 7
		//switch(tpuInfo.SAM7appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM7ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM7ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM7ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM7ID,6);
		//	break;
		//default:
		//	break;
		//}

		//// SAM 8
		//switch(tpuInfo.SAM8appType){
		//case SAM_TYPE_YPT_ISAM:
		//	memcpy(m_TpuInfo.accISAMid,tpuInfo.SAM8ID,6);
		//	break;
		//case SAM_TYPE_YPT_PASM:
		//	memcpy(m_TpuInfo.accPSAMid,tpuInfo.SAM8ID,6);
		//	break;
		//case SAM_TYPE_YKT_ISAM:
		//	memcpy(m_TpuInfo.bmacISAMid,tpuInfo.SAM8ID,6);
		//	break;
		//case SAM_TYPE_YKT_PSAM:
		//	memcpy(m_TpuInfo.bmacPSAMid,tpuInfo.SAM8ID,6);
		//	break;
		//default:
		//	break;
		//}
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetTpuInfo::GetRWResponse(RW_RSP_TPU_INFO *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_TpuInfo;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPRW_CMD_FIRMWARE input

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWFirmwareDownload::CRWFirmwareDownload(LPRW_CMD_FIRMWARE input,RWID& rwid):CRWCommands(rwid)
{
	m_firmwareDownload = input;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWFirmwareDownload::~CRWFirmwareDownload(void)
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
long CRWFirmwareDownload::IsValidCommand()
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
long CRWFirmwareDownload::ExecuteCommand(void)
{
	// �ָ����
	int blockCnt = m_firmwareDownload->size/1024;
	if (0 != m_firmwareDownload->size%1024) {
		blockCnt++;
	}

	// ���ݿ����ֱ���
	WORD currentBlockNo = 0;
	long lCompleteCode = -1;
	// ȡ���������Դ���
	CString devicename = theMAINTENANCE_INFO.GetProgName();
	TCHAR szAppPath[_MAX_PATH];
	ComGetAppPath(szAppPath,_MAX_PATH);
	CString szBOMINIFName = CString(szAppPath) + devicename + INI_FILE;
	CGetIni getini(szBOMINIFName);
	int downloadCnt = getini.GetDataInt(_T("OUT_ICRW"),_T("FIRMWARE_DOWNLOAD_CNT"));
	// ���Դ�����������1��
	if (downloadCnt < 1) {
		downloadCnt = 1;
	}
	for (int cnt=0;cnt<downloadCnt;cnt++) {
		for (int i=0;i<blockCnt;i++) {
			// �ָ���
			if ( i == blockCnt-1) {
				currentBlockNo = 0xFFFF;
			}
			else {
				currentBlockNo = i;
			}
			// Firmware����
			BYTE firmwareData[1024] = {0};
			if ( i == blockCnt-1) {
				memcpy(firmwareData,m_firmwareDownload->fileData+1024*i,m_firmwareDownload->size-1024*i);
			}
			else {
				memcpy(firmwareData,m_firmwareDownload->fileData+1024*i,1024);
			}

			if(m_firmwareDownload->isMain) {	
				lCompleteCode = ProgramDownload_Main(currentBlockNo,(uchar*)firmwareData);							
			}
			else {
				lCompleteCode = ProgramDownload_App(currentBlockNo,(uchar*)firmwareData);
			}

			// �м����ʧ����ֹͣ����
			if(RW_RSP_OK != lCompleteCode){
				break;
			}
		}
		// �ɹ�������
		if(RW_RSP_OK == lCompleteCode){
			break;
		}
	}
	if (lCompleteCode == RW_RSP_OK) {
		// ���ز�
		if (m_firmwareDownload->isMain) {			
			lCompleteCode = ProgramUpdate_Main();		
		}
		// Ӧ�ò�
		else {			
			lCompleteCode = ProgramUpdate_App();			
		}
		::Sleep(1*60*1000);
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPRW_CMD_FIRMWARE input

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWInit::CRWInit(RW_CMD_INIT input,RWID& rwid):CRWCommands(rwid)
{
	m_initRequest = input;
	m_response = 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWInit::~CRWInit(void)
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
long CRWInit::IsValidCommand()
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
long CRWInit::ExecuteCommand(void)
{
	DAT::INITSTRUCT initInput;
	initInput.DelayTime = m_initRequest.ReadTimeout; 
	memcpy(initInput.DeviceCode,&m_initRequest.DeviceId,sizeof(m_initRequest.DeviceId));
	initInput.DeviceHdType = m_initRequest.DeviceType;
	memcpy(initInput.StationPLInfo,&m_initRequest.StationId,sizeof(m_initRequest.StationId));
	initInput.AgentCode = m_initRequest.AgentCode;
	Date2BCD(m_initRequest.OperationDate,initInput.OperationDate);
	Date2BCD(m_initRequest.CurrentDate,initInput.CurrentDate);
	initInput.DeviceTestMode = m_initRequest.TestMode; 
	initInput.StationOperationMode = m_initRequest.OperationMode;
	int2BCD(m_initRequest.OperaterID,(char*)initInput.OperaterID,sizeof(initInput.OperaterID));
	initInput.AntennaConfig = m_initRequest.AntennaConfig;
	initInput.ErrFareCtrMax = m_initRequest.ErrFareCtrMax;		
	initInput.WaitTimeForCardWR = m_initRequest.WaitTimeForCardWR;	
	initInput.RetryTimesForCardWR = m_initRequest.RetryTimesForCardWR;	
	memset(initInput.Reserver,0,sizeof(initInput.Reserver));

	long lCompleteCode =  InitRW(initInput,m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWInit::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPRW_CMD_FIRMWARE input

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetTicketPrice::CRWGetTicketPrice(RW_CMD_TICKET_PRICE input,RWID& rwid):CRWCommands(rwid)
{
	m_ticketInput = input;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetTicketPrice::~CRWGetTicketPrice(void)
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
long CRWGetTicketPrice::IsValidCommand()
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
long CRWGetTicketPrice::ExecuteCommand(void)
{
	DAT::TICKETINFO TicketInput;
	TicketInput.CardType = m_ticketInput.CardType;
	TicketInput.ProductType = m_ticketInput.ProductCategory;
	TicketInput.ProductCategory = m_ticketInput.ProductType;
	TicketInput.ProductSubType =m_ticketInput.ProductSubType;
	TicketInput.PriceType = m_ticketInput.PriceType;		//����Ʊ�۱�
	TicketInput.PassengerType = m_ticketInput.PassengerType;
	m_ticketInput.Time.Serialize(TicketInput.Time);
	//memcpy(TicketInput.Time,&m_ticketInput.Time,sizeof(TicketInput.Time));
	memcpy(TicketInput.BegainStation,m_ticketInput.BegainStation,sizeof(m_ticketInput.BegainStation));
	memcpy(TicketInput.EndStation,m_ticketInput.EndStation,sizeof(m_ticketInput.EndStation));
	long lCompleteCode = GetTicketPrice(TicketInput,m_response.TicketPrice,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetTicketPrice::GetRWResponse(RW_CMD_TICKET_PRICE_RES* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPRW_CMD_FIRMWARE input

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetTicketTax::CRWGetTicketTax(RW_CMD_TICKET_TAX input,RWID& rwid):CRWCommands(rwid)
{
	m_ticketInput = input;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetTicketTax::~CRWGetTicketTax(void)
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
long CRWGetTicketTax::IsValidCommand()
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
long CRWGetTicketTax::ExecuteCommand(void)
{
	//DAT::TAXINFO TicketInput;
	//TicketInput.CardType = m_ticketInput.CardType;
	//TicketInput.SaleOrAddAmount = m_ticketInput.SaleOrAddAmount;
	//long lCompleteCode = GetTicketTax(TicketInput,m_response.TicketTax,m_response.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	OnComplete(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetTicketTax::GetRWResponse(RW_CMD_TICKET_TAX_RES* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetParameterInfo::CRWGetParameterInfo(RWID& rwid):CRWCommands(rwid)
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
CRWGetParameterInfo::~CRWGetParameterInfo()
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
long CRWGetParameterInfo::IsValidCommand()
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
long CRWGetParameterInfo::ExecuteCommand(void)
{
	long lCompleteCode = GetParameterInfo(m_response.ParaCount ,m_response.ParamData);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetParameterInfo::GetRWResponse(RW_RSP_PARAM_INFO *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWParamFirmware::CRWParamFirmware(RW_PARAM_DOWNLOAD pParamInfo,RWID& rwid):CRWCommands(rwid)
{
	m_downloadInput.paramId = pParamInfo.paramId;
	m_downloadInput.size = pParamInfo.size;
	m_downloadInput.fileData = pParamInfo.fileData;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWParamFirmware::~CRWParamFirmware()
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
long CRWParamFirmware::IsValidCommand()
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
long CRWParamFirmware::ExecuteCommand(void)
{
	// �ָ����
	int blockCnt = m_downloadInput.size/1024;
	if (0 != m_downloadInput.size%1024) {
		blockCnt++;
	}

	// ���ݿ����ֱ���
	WORD currentBlockNo = 0;
	long lCompleteCode = -1;

	int downloadCnt = m_downloadInput.downloadRetryCnt;
	// ����������������1��
	if (downloadCnt < 1) {
		downloadCnt = 1;
	}

	uchar bContinue = 1;
	ushort PacketLen = 1024;
	ushort ReceivePacketNumber = 0;
	for (int cnt=0;cnt<downloadCnt;cnt++) {
		for (int i=0;i<blockCnt;i++) {
			// �ָ���
			if ((blockCnt > 1) && (i == blockCnt-1)) {
				currentBlockNo = 0xFFFF;
			}
			else {
				currentBlockNo = i;
			}

			// Firmware����
			BYTE firmwareData[1024] = {0};

			if ( i == blockCnt-1) {
				bContinue = 0;
				PacketLen = m_downloadInput.size-1024*i;
				memcpy(firmwareData,m_downloadInput.fileData+1024*i,m_downloadInput.size-1024*i);
			}
			else {
				bContinue = 1;
				PacketLen = 1024;
				memcpy(firmwareData,m_downloadInput.fileData+1024*i,1024);
			}

			lCompleteCode = DownloadParamter(m_downloadInput.paramId,bContinue,currentBlockNo,PacketLen,firmwareData,ReceivePacketNumber);

			// �м����ʧ����ֹͣ����
			if ( RW_RSP_OK != lCompleteCode) {
				break;
			}
		}
		// �ɹ�������
		if ( RW_RSP_OK == lCompleteCode) {
			break;
		}
	}

	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetOperationMode::CRWSetOperationMode(RW_CMD_SET_MODE setMode,RWID& rwid):CRWCommands(rwid)
{
	m_setModeInput.ControlCode = setMode.ControlCode;
	m_setModeInput.ModeCode = setMode.ModeCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetOperationMode::~CRWSetOperationMode()
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
long CRWSetOperationMode::IsValidCommand()
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
long CRWSetOperationMode::ExecuteCommand(void)
{
	long lCompleteCode = SetOperationMode(m_setModeInput.ControlCode,(BYTE*)&m_setModeInput.ModeCode,m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWSetOperationMode::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetBusinessTime::CRWSetBusinessTime(RW_CMD_SET_TIME setMode,RWID& rwid):CRWCommands(rwid)
{
	m_setModeInput.ControlCode = setMode.ControlCode;
	m_setModeInput.Time = setMode.Time;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetBusinessTime::~CRWSetBusinessTime()
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
long CRWSetBusinessTime::IsValidCommand()
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
long CRWSetBusinessTime::ExecuteCommand(void)
{
	long lCompleteCode = SetBusinessTime(m_setModeInput.ControlCode,m_setModeInput.Time,m_response);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWSetBusinessTime::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWGetUDData::CRWGetUDData(RWID& rwid):CRWCommands(rwid)
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
CRWGetUDData::~CRWGetUDData()
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
long CRWGetUDData::IsValidCommand()
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
long CRWGetUDData::ExecuteCommand(void)
{
	long  lCompleteCode= GetUDData(m_response->SerialAddnum,m_response->TranResult,m_response->UDlen,m_response->UDData,m_response->ARLen,m_response->ARData);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWGetUDData::GetRWResponse(LPRW_BUSSINESS_RESPONSE pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_READ_CARD_REQUEST readCardRequest    ��������ṹ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReadCard::CRWReadCard(RW_READ_CARD_REQUEST readCardRequest,RWID& rwid):CRWCommands(rwid)
{
	m_readCardRequest.readCardInterval = readCardRequest.readCardInterval;
	m_readCardRequest.IsCheck = readCardRequest.IsCheck;
	m_readCardRequest.FunctionCode = readCardRequest.FunctionCode;			
	m_readCardRequest.IsPaidArea = readCardRequest.IsPaidArea;	
	m_readCardRequest.IsReadRecords = readCardRequest.IsReadRecords;
	m_readCardRequest.PollCardMethod = readCardRequest.PollCardMethod;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReadCard::~CRWReadCard(void)
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
long CRWReadCard::IsValidCommand()
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
long CRWReadCard::ExecuteCommand(void)
{
	DAT::CardReadMisc_t readCardInput;
	DAT::CARDINFO readCardOutput;
	memset(&readCardOutput,0,sizeof(DAT::CARDINFO));
	//memset(readCardOutput.RechargeProduct,0x00,sizeof(readCardOutput.RechargeProduct));// ��ʼ������Ϊ0
	readCardInput.ValidCheckMark = (m_readCardRequest.IsCheck)?0x01:0x02;
	readCardInput.TransCode = m_readCardRequest.FunctionCode;
	readCardInput.AreaPaidMark = (m_readCardRequest.IsPaidArea)?0x01:0x02;
	ComGetCurTime().Serialize(readCardInput.TransTime);
	//memcpy(readCardInput.TransTime,&,7);
	readCardInput.AntennaMark = 0x01;//m_readCardRequest.PollCardMethod;//0x01;
	readCardInput.ucReadFlag = 0x02;
	memset(readCardInput.ucStation,0x00,sizeof(readCardInput.ucStation));
	readCardInput.ucGetTransLog = (m_readCardRequest.IsReadRecords)?0x01:0x00;
	long lCompleteCode = RW_RSP_OK;
	int detailErrCode = 0;
	isAborted = false;
	while(!isAborted){
		lCompleteCode = ReadCard(&readCardInput,readCardOutput,m_cardInfo.DetailErrorCode);
		if(lCompleteCode != RW_RSP_OK){
			// �޿�����������
			if(lCompleteCode == RW_ERROR_CARD_NO_CARD){
				Sleep(m_readCardRequest.readCardInterval);
			}
			else{
				break;
			}
		}
		else{
			break;
		}
	}
	if(lCompleteCode == RW_RSP_OK){
		m_cardInfo.CardPHType = readCardOutput.CardPHType;									//��Ʊ�������� 
		m_cardInfo.CardInfoLen = readCardOutput.CardInfoLen;								//��Ƭ��Ϣ�ĳ���
		m_cardInfo.PaidAreaAdjustCode = readCardOutput.PayAreaReticketCode;			//�쳣���������
		m_cardInfo.UnPaidAreaAdjustCode = readCardOutput.NoPayAreaReticketCode;				//�Ƿ�������Ʊ�������
		//m_cardInfo.payAdjustMethod = readCardOutput.NoPayAreaReticketCode;				//�Ƿ�������Ʊ�������
		m_cardInfo.AdjustAmount = readCardOutput.RepayTicketAmount;						//�����쳣������
		m_cardInfo.ReturnCardFee = readCardOutput.ReturnCardFee;							//������Ʊ�����ѽ��
		//m_cardInfo.DeferCardFee = readCardOutput.DeferCardFee;                              //���������ѽ��
		//m_cardInfo.BlackListUnkockCardFee = readCardOutput.BlackListUnkockCardFee;          //���������������ѽ��
		m_cardInfo.isBigMoneyCard = readCardOutput.isBigMoneyCard;							//�Ƿ�Ϊ�� true:�� false:��
		memcpy(m_cardInfo.CardInfo,readCardOutput.CardInfo,RW_MSG_CARD_INFO_LEN);			//����Ϣ
		m_cardInfo.LogCount = readCardOutput.ucLogNum;										//���׼�¼����
		memcpy(&m_cardInfo.LogInfo,readCardOutput.LogInfo,readCardOutput.ucLogNum*20/*RW_MSG_LOG_INFO_LEN*/);//���׼�¼����

		//memcpy(&m_cardInfo.LogInfo[0],readCardOutput.LogInfo,sizeof(readCardOutput.LogInfo)/*RW_MSG_LOG_INFO_LEN*/);//���׼�¼����
		//memset(&m_cardInfo.LogInfo[sizeof(readCardOutput.LogInfo)],0,sizeof(m_cardInfo.LogInfo)-sizeof(readCardOutput.LogInfo));
		// ������ֵ
		m_cardInfo.ProductCount = 1;														// ��Ʒ����
		m_cardInfo.RechargeProduct[0].ProductType = readCardOutput.CardInfo[16];			// ��Ʒ����
		m_cardInfo.RechargeProduct[0].ProductSubType = readCardOutput.CardInfo[38];			// ��Ʒ������
		m_cardInfo.RechargeProduct[0].RefoundMoney = readCardOutput.ReturnTicketAmount;	    // ���˽�� �������˿���Ѻ��
		m_cardInfo.RechargeProduct[0].RechargeMark = 1;										// �Ƿ�ɳ�ֵ
		m_cardInfo.SalableProductType = 0x07;												// ���۲�Ʒ����		��BITλ�������ӵ͵���λ���δ���Ǯ�����ƴΣ����ڡ�Ǯ����00000001 =0x01���ƴΣ�00000010 = 0x02��Ǯ��+�ƴ� = 00000011 = 0x03�����ڣ�00000100 = 0x04��Ǯ��+���� = 00000101 = 0x05��Ǯ��+�ƴ�+���� = 00000111 = 0x07
		m_cardInfo.SalableProductCount = 1;													// ���۲�Ʒ����

	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����Ϣ

@param      (o)LPRW_RSP_READ_CARD pResponse  ����Ϣ      

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWReadCard::GetRWResponse(RW_READ_CARD_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_cardInfo;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_READ_CARD_REQUEST readCardRequest    ��������ṹ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWTryReadCard::CRWTryReadCard(RW_READ_CARD_REQUEST readCardRequest,RWID& rwid):CRWCommands(rwid)
{
	m_readCardRequest.IsCheck = readCardRequest.IsCheck;
	m_readCardRequest.FunctionCode = readCardRequest.FunctionCode;			
	m_readCardRequest.IsPaidArea = readCardRequest.IsPaidArea;	
	m_readCardRequest.IsReadRecords = readCardRequest.IsReadRecords;
	m_readCardRequest.PollCardMethod = readCardRequest.PollCardMethod;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWTryReadCard::~CRWTryReadCard(void)
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
long CRWTryReadCard::IsValidCommand()
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
long CRWTryReadCard::ExecuteCommand(void)
{
	DAT::CardReadMisc_t readCardInput;
	DAT::CARDINFO readCardOutput;
	readCardInput.ValidCheckMark = (m_readCardRequest.IsCheck)?0x01:0x02;
	readCardInput.TransCode = m_readCardRequest.FunctionCode;
	readCardInput.AreaPaidMark = (m_readCardRequest.IsPaidArea)?0x01:0x02;
	ComGetCurTime().Serialize(readCardInput.TransTime);
	readCardInput.AntennaMark = m_readCardRequest.PollCardMethod;;
	readCardInput.ucReadFlag = 0;
	memset(readCardInput.ucStation,0x00,sizeof(readCardInput.ucStation));
	readCardInput.ucGetTransLog = (m_readCardRequest.IsReadRecords)?0x01:0x00;
	long lCompleteCode = RW_RSP_OK;
	int detailErrCode = 0;
	lCompleteCode = ReadCard(&readCardInput,readCardOutput,m_cardInfo.DetailErrorCode);
	if(lCompleteCode == RW_RSP_OK){
		m_cardInfo.CardPHType = readCardOutput.CardPHType;							//��Ʊ�������� 
		m_cardInfo.CardInfoLen = readCardOutput.CardInfoLen;						//��Ƭ��Ϣ�ĳ���
		m_cardInfo.PaidAreaAdjustCode = readCardOutput.PayAreaReticketCode;	//��������Ʊ�������
		m_cardInfo.UnPaidAreaAdjustCode = readCardOutput.NoPayAreaReticketCode;		//�Ƿ�������Ʊ�������
		m_cardInfo.AdjustAmount = readCardOutput.RepayTicketAmount;				//���鲹Ʊ���
		//m_cardInfo.ProductCount = readCardOutput.ProductCount;
		//memcpy(m_cardInfo.RechargeProduct,readCardOutput.RechargeProduct,sizeof(RECHARGEPRODUCT)*10);
		//m_cardInfo.SalableProductType = readCardOutput.SalableProductType;		//���۲�Ʒ����
		//m_cardInfo.SalableProductCount = readCardOutput.SalableProductCount;    //���۲�Ʒ����
		m_cardInfo.ReturnCardFee = readCardOutput.ReturnCardFee;                //������Ʊ�����ѽ��
		m_cardInfo.isBigMoneyCard = readCardOutput.isBigMoneyCard;                //�Ƿ�Ϊ�� true:�� false:��
		memcpy(m_cardInfo.CardInfo,readCardOutput.CardInfo,RW_MSG_CARD_INFO_LEN);	//����Ϣ
		m_cardInfo.LogCount = readCardOutput.ucLogNum;								//���׼�¼����
		memcpy(m_cardInfo.LogInfo,readCardOutput.LogInfo,sizeof(readCardOutput.LogInfo));	//���׼�¼����

		// ������ֵ
		m_cardInfo.ProductCount = 1;														// ��Ʒ����
		m_cardInfo.RechargeProduct[0].ProductType = readCardOutput.CardInfo[16];			// ��Ʒ����
		m_cardInfo.RechargeProduct[0].ProductSubType = readCardOutput.CardInfo[38];			// ��Ʒ������
		m_cardInfo.RechargeProduct[0].RefoundMoney = readCardOutput.ReturnTicketAmount;	    // ���˽�� �������˿���Ѻ��
		m_cardInfo.RechargeProduct[0].RechargeMark = 1;										// �Ƿ�ɳ�ֵ
		m_cardInfo.SalableProductType = 0x07;												// ���۲�Ʒ����		��BITλ�������ӵ͵���λ���δ���Ǯ�����ƴΣ����ڡ�Ǯ����00000001 =0x01���ƴΣ�00000010 = 0x02��Ǯ��+�ƴ� = 00000011 = 0x03�����ڣ�00000100 = 0x04��Ǯ��+���� = 00000101 = 0x05��Ǯ��+�ƴ�+���� = 00000111 = 0x07
		m_cardInfo.SalableProductCount = 1;													// ���۲�Ʒ����
	}
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�����Ϣ

@param      (o)LPRW_RSP_READ_CARD pResponse  ����Ϣ      

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWTryReadCard::GetRWResponse(RW_READ_CARD_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			memcpy(pResponse,&m_cardInfo,sizeof(RW_READ_CARD_RESPONSE));
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////����Ϊҵ���������/////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      none

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWIssue::CRWIssue(RW_ISSUE_REQUEST issueInput,RWID& rwid):CRWCommands(rwid)
{
	memset(&m_issueInput,0x00,sizeof(RW_ISSUE_REQUEST));
	m_issueInput.CardType = issueInput.CardType;
	m_issueInput.SerialNo = issueInput.SerialNo;
	m_issueInput.TranDateTime = issueInput.TranDateTime;
	m_issueInput.CardType = issueInput.CardType;
	m_issueInput.ProductyCategory = issueInput.ProductyCategory;
	m_issueInput.ProductType = issueInput.ProductType;
	m_issueInput.ProductSubType = issueInput.ProductSubType;
	m_issueInput.SaleMode = issueInput.SaleMode;
	m_issueInput.PassengerType = issueInput.PassengerType;
	m_issueInput.SaleAmount = issueInput.SaleAmount;
	m_issueInput.BegainStation = issueInput.BegainStation;	
	m_issueInput.EndStation = issueInput.EndStation;
	m_issueInput.MultiRideTicketCount = issueInput.MultiRideTicketCount;
	m_issueInput.PaymentMethod = issueInput.PaymentMethod;
	m_issueInput.TranDateTime = issueInput.TranDateTime;
	m_issueInput.ValidityPeriod = issueInput.ValidityPeriod;
	memcpy(m_issueInput.BankCardNum, issueInput.BankCardNum, sizeof(m_issueInput.BankCardNum));
	memcpy(m_issueInput.TicketNum, issueInput.TicketNum, sizeof(m_issueInput.TicketNum));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWIssue::~CRWIssue()
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
long CRWIssue::IsValidCommand()
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
long CRWIssue::ExecuteCommand(void)
{
	DAT::SALEINFO issueInfo;
	memset(&issueInfo,0x00,sizeof(DAT::SALEINFO));
	issueInfo.SaleMode			= m_issueInput.SaleMode;
	issueInfo.CardApplicationType			= m_issueInput.CardType;
	issueInfo.ProductyType			= m_issueInput.ProductyCategory;
	issueInfo.ProductSubType			= m_issueInput.ProductSubType;
	issueInfo.ProductyCategory			= m_issueInput.ProductType;
	issueInfo.TicketAmount		= m_issueInput.SaleAmount;
	issueInfo.byPayType	= m_issueInput.PaymentMethod;
	issueInfo.PassengerType = m_issueInput.PassengerType;
	issueInfo.byGroupSize = 0;
	issueInfo.CountTicketUseCount = m_issueInput.MultiRideTicketCount;
	issueInfo.ValidityPeriod = m_issueInput.ValidityPeriod;
	memcpy(issueInfo.PayCardSerialNumber,m_issueInput.TicketNum,sizeof(m_issueInput.TicketNum));
	// ��վ����ΪBCD��ʽ
	int2BCD(m_issueInput.BegainStation,(char*)issueInfo.BegainStation,4);
	int2BCD(m_issueInput.EndStation,(char*)issueInfo.EndStation,4);

	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_issueInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = Issue(m_issueInput.SerialNo, tranDateTime, issueInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    Ʊ��   

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWIssue::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_CHARGE chargeInput ��ֵ��Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetCheck::CRWSetCheck(RW_CMD_SETCHECK chargeInput,RWID& rwid):CRWCommands(rwid)
{
	m_srtCheck.TranDateTime = chargeInput.TranDateTime ;
	m_srtCheck.byDo = chargeInput.byDo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWSetCheck::~CRWSetCheck(void)
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
long CRWSetCheck::IsValidCommand()
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
long CRWSetCheck::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_srtCheck.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = SetCheck(tranDateTime, m_srtCheck.byDo,m_response.byTpuState,m_response.DetailErrCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_RSP_SV_ADDVALUE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWSetCheck::GetRWResponse(RW_CMD_RESP_SETCHECK *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_CHARGE chargeInput ��ֵ��Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWChargeOverTime::CRWChargeOverTime(RW_CMD_CHARGEOVERTIME chargeInput,RWID& rwid):CRWCommands(rwid)
{
	m_chargeOverTimeInput.SerialNo = chargeInput.SerialNo;		
	m_chargeOverTimeInput.TranDateTime = chargeInput.TranDateTime ;
	m_chargeOverTimeInput.byDo = chargeInput.byDo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWChargeOverTime::~CRWChargeOverTime(void)
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
long CRWChargeOverTime::IsValidCommand()
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
long CRWChargeOverTime::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_chargeOverTimeInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = ChargeOverTime(m_chargeOverTimeInput.SerialNo, tranDateTime, m_chargeOverTimeInput.byDo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_RSP_SV_ADDVALUE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWChargeOverTime::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_CHARGE chargeInput ��ֵ��Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWCharge::CRWCharge(RW_CMD_CHARGE chargeInput,RWID& rwid):CRWCommands(rwid)
{
	m_chargeInput.SerialNo = chargeInput.SerialNo;		
	m_chargeInput.TranDateTime = chargeInput.TranDateTime;
	m_chargeInput.PaymentMethod = chargeInput.PaymentMethod;
	m_chargeInput.ChargeType = chargeInput.ChargeType;
	m_chargeInput.ChargeProductType = chargeInput.ChargeProductType;
	m_chargeInput.ChargeSubProductType = chargeInput.ChargeSubProductType;
	m_chargeInput.ChargeAmount = chargeInput.ChargeAmount;
	m_chargeInput.ChargeCount = chargeInput.ChargeCount;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWCharge::~CRWCharge(void)
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
long CRWCharge::IsValidCommand()
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
long CRWCharge::ExecuteCommand(void)
{
	DAT::REVALUEINFO chargeInfo;
	memset(&chargeInfo, 0, sizeof(DAT::REVALUEINFO));
	//chargeInfo.PayMethed = m_chargeInput.PaymentMethod;
	//chargeInfo.RevalueType = m_chargeInput.ChargeType;
	//chargeInfo.MainProductType = m_chargeInput.ChargeProductType;
	//chargeInfo.AMT_YPT_Type = m_chargeInput.ChargeSubProductType;
	//chargeInfo.Amount = m_chargeInput.ChargeAmount;
	//chargeInfo.Count = m_chargeInput.ChargeCount;
	//chargeInfo.Period = 0;
	//memcpy(chargeInfo.BankCardNo,chargeInfo.BankCardNo,10);
	chargeInfo.CardType = m_chargeInput.CardType;
	chargeInfo.RevalueType = m_chargeInput.ChargeType;
	chargeInfo.PayMethed = m_chargeInput.PaymentMethod;
	chargeInfo.Amount = m_chargeInput.ChargeAmount;
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_chargeInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = Charge(m_chargeInput.SerialNo, tranDateTime,(char*)m_chargeInput.SerialNo,chargeInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_RSP_SV_ADDVALUE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWCharge::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)LPRW_CMD_SV_ADJUST	SVAddValue

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWAdjust::CRWAdjust(RW_CMD_ADJUST adjustInput,RWID& rwid):CRWCommands(rwid)
{
	m_adjustInput.SerialNo = adjustInput.SerialNo;					// ������ˮ��
	m_adjustInput.TranDateTime= adjustInput.TranDateTime;			// ����ʱ��
	m_adjustInput.CardType= adjustInput.CardType;					// ��Ƭ���� 0x01:һ��ͨS50/S70��Ƭ;0x02:һ��ͨCPU��;0x11:һƱͨS50/S70��Ƭ;0x12:һƱͨUL��
	m_adjustInput.ProductType= adjustInput.ProductType;				// ��Ʊ��Ʒ����
	m_adjustInput.AdjustMethod = adjustInput.AdjustMethod;
	m_adjustInput.AdjustAmount= adjustInput.AdjustAmount;			// ��Ʊ���   	
	m_adjustInput.AdjustArea= adjustInput.AdjustArea;				// ��Ʊ���� 0x01:��������Ʊ;0x02:�Ǹ�������Ʊ
	m_adjustInput.AdjustOperType= adjustInput.AdjustOperType;		// ��Ʊ��������
	m_adjustInput.BegainStation= adjustInput.BegainStation;			// ���վվ��	
	m_adjustInput.EndStation= adjustInput.EndStation;				// �յ�վվ��
	m_adjustInput.PaymentMethod= adjustInput.PaymentMethod;			// ֧����ʽ
	memcpy(m_adjustInput.BankCardNum, adjustInput.BankCardNum, 10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWAdjust::~CRWAdjust(void)
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
long CRWAdjust::IsValidCommand()
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
long CRWAdjust::ExecuteCommand(void)
{
	DAT::COMPENSATIONPAID adjustInfo;
	adjustInfo.CardTrantype = m_adjustInput.CardType;
	adjustInfo.ProductyCategory = m_adjustInput.ProductType;
	adjustInfo.PayMethed = m_adjustInput.AdjustMethod;
	adjustInfo.TicketAmount = m_adjustInput.AdjustAmount;
	adjustInfo.PayAttribute = m_adjustInput.AdjustArea;
	adjustInfo.OperationType = m_adjustInput.AdjustOperType;
	memcpy(adjustInfo.BegainStation,&m_adjustInput.BegainStation,4);
	memcpy(adjustInfo.EndStation,&m_adjustInput.EndStation,4);
//	adjustInfo.byPayType = m_adjustInput.PaymentMethod;
	//memset(adjustInfo.PayCardLogicID,0,sizeof(adjustInfo.PayCardLogicID));
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_adjustInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = Adjust(m_adjustInput.SerialNo, tranDateTime, adjustInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_RSP_SV_ADJUST pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWAdjust::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_REFUND refundInput �˿���Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWRefund::CRWRefund(RW_CMD_REFUND refundInput,RWID& rwid):CRWCommands(rwid)
{
	m_refundInput.SerialNo = refundInput.SerialNo;
	m_refundInput.TranDateTime = refundInput.TranDateTime;
	m_refundInput.CardPHType = refundInput.CardPHType;
	m_refundInput.CardAppType = refundInput.CardAppType;
	m_refundInput.ReasonCode = refundInput.ReasonCode;
	m_refundInput.RefundAmount = refundInput.RefundAmount;
	m_refundInput.DepositAmount = refundInput.DepositAmount;
	memcpy(m_refundInput.ReceptionNum,refundInput.ReceptionNum,sizeof(m_refundInput.ReceptionNum));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWRefund::~CRWRefund(void)
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
long CRWRefund::IsValidCommand()
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
long CRWRefund::ExecuteCommand(void)
{
	DAT::SURRENDERINFO refundInfo;
	refundInfo.PayMethed = 1;	//֧����ʽ ���ֽ�֧����01�����п���02����ֵ����03��
	refundInfo.CardApplicationType = m_refundInput.CardPHType;
	refundInfo.ProductyCategory = m_refundInput.CardAppType;
	refundInfo.SurrenderFlag = 0;
	refundInfo.YPTSurrenderOperateType = 0;
	refundInfo.YKTSurrenderOperateType = 0;
	refundInfo.ReasoneCode = m_refundInput.ReasonCode;	//00 �ÿ�ԭ��	01 ����ԭ��
	memset(refundInfo.BadCardNo,0,sizeof(refundInfo.BadCardNo));
	memset(refundInfo.CardPhyID,0,sizeof(refundInfo.CardPhyID));
	refundInfo.RefundMoney = m_refundInput.RefundAmount;
	refundInfo.CardBalance = m_refundInput.DepositAmount;
	refundInfo.CardTransCtr = 0;
	refundInfo.FareProductType = 0;
	refundInfo.FareProductyCategory = 0;
	refundInfo.CardDeposit = 0;
	refundInfo.ServiceFee = 0;
	refundInfo.CardCost = 0;
	memcpy(refundInfo.mReceiptNumber,m_refundInput.ReceptionNum,sizeof(refundInfo.mReceiptNumber));
	refundInfo.IsLimitSurrender = 0;
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_refundInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = Refund(m_refundInput.SerialNo, tranDateTime, refundInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_RSP_SV_UPDATE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWRefund::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_COUNTERACT OTBlock   counteractInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWCounteract::CRWCounteract(RW_CMD_COUNTERACT counteractInput,RWID& rwid):CRWCommands(rwid)
{
	m_counteractInput.SerialNo = counteractInput.SerialNo;
	m_counteractInput.TranDateTime = counteractInput.TranDateTime;
	m_counteractInput.CardType = counteractInput.CardType;
	m_counteractInput.ProductyType = counteractInput.ProductyType;
	m_counteractInput.ReasonCode = counteractInput.ReasonCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWCounteract::~CRWCounteract(void)
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
long CRWCounteract::IsValidCommand()
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
long CRWCounteract::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_counteractInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = 0 ;
	//lCompleteCode = Counteract(m_counteractInput.SerialNo, tranDateTime, m_counteractInput.CardType,m_counteractInput.ProductyType,(BYTE*)&m_counteractInput.ReasonCode,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_RSP_SV_BLOCK pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWCounteract::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_BLOCK blockInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWBlock::CRWBlock(RW_CMD_BLOCK blockInput,RWID& rwid):CRWCommands(rwid)
{
	m_blockInput.SerialNo = blockInput.SerialNo;
	m_blockInput.TranDateTime = blockInput.TranDateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWBlock::~CRWBlock(void)
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
long CRWBlock::IsValidCommand()
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
long CRWBlock::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_blockInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = Block(m_blockInput.SerialNo,tranDateTime, m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_BUSSINESS_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWBlock::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_BLOCK blockInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWRefresh::CRWRefresh(RW_CMD_ACTIVE refreshInput,RWID& rwid):CRWCommands(rwid)
{
	m_refreshInput.TranDateTime = refreshInput.TranDateTime;
	m_refreshInput.Category = refreshInput.Category;
	m_refreshInput.ProductyCategory = refreshInput.ProductyCategory;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWRefresh::~CRWRefresh(void)
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
long CRWRefresh::IsValidCommand()
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
long CRWRefresh::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_refreshInput.TranDateTime.Serialize(tranDateTime);
	DWORD dwSerilNo = 0;
	long lCompleteCode = Active(dwSerilNo,tranDateTime, m_refreshInput.Category,m_refreshInput.ProductyCategory,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_BUSSINESS_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWRefresh::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}



//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_BLOCK blockInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReplaceNoCard::CRWReplaceNoCard(RW_CMD_NO_CARD_REPLACE replaceInput,RWID& rwid):CRWCommands(rwid)
{
	m_replaceInput = replaceInput;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReplaceNoCard::~CRWReplaceNoCard(void)
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
long CRWReplaceNoCard::IsValidCommand()
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
long CRWReplaceNoCard::ExecuteCommand(void)
{
	/*BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_replaceInput.TranDateTime.Serialize(tranDateTime);

	DAT::STRUCT_NocardOrBadcardTransData transInfo;
	memcpy(&transInfo, &m_replaceInput.transData, sizeof(transInfo));

	long lCompleteCode = ReplaceNoCard(m_replaceInput.Serialnum,tranDateTime,transInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;*/
	//zxl
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_BUSSINESS_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWReplaceNoCard::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_BLOCK blockInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReplaceOldCard::CRWReplaceOldCard(RW_CMD_REPLACE_CARD replaceInput,RWID& rwid):CRWCommands(rwid)
{
	m_replaceOldInput.Serialnum = replaceInput.Serialnum;
	m_replaceOldInput.TranDateTime = replaceInput.TranDateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReplaceOldCard::~CRWReplaceOldCard(void)
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
long CRWReplaceOldCard::IsValidCommand()
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
long CRWReplaceOldCard::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_replaceOldInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = ReplaceOldCard(m_replaceOldInput.Serialnum,tranDateTime,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_BUSSINESS_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWReplaceOldCard::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_BLOCK blockInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReplaceNewCard::CRWReplaceNewCard(RW_CMD_REPLACE_CARD replaceInput,RWID& rwid):CRWCommands(rwid)
{
	m_replaceNewInput.Serialnum = replaceInput.Serialnum;
	m_replaceNewInput.TranDateTime = replaceInput.TranDateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReplaceNewCard::~CRWReplaceNewCard(void)
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
long CRWReplaceNewCard::IsValidCommand()
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
long CRWReplaceNewCard::ExecuteCommand(void)
{
	BYTE tranDateTime[7];
	memset(tranDateTime,0,7);
	m_replaceNewInput.TranDateTime.Serialize(tranDateTime);
	long lCompleteCode = ReplaceNewCard(m_replaceNewInput.Serialnum,tranDateTime,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode );
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)LPRW_BUSSINESS_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWReplaceNewCard::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_DEFER deferInput ������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWDefer::CRWDefer(RW_CMD_DEFER deferInput,RWID& rwid):CRWCommands(rwid)
{
	m_deferInput = deferInput;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWDefer::~CRWDefer(void)
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
long CRWDefer::IsValidCommand()
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
long CRWDefer::ExecuteCommand(void)
{
	DAT::STT_DEFERCARD sttDefer;
	//sttDefer.uUdsn = m_deferInput.SerialNo;
	//m_deferInput.TranDateTime.Serialize(sttDefer.mDataTime);
	//sttDefer.uCommissionCharge = m_deferInput.ServiceFee;
	//sttDefer.wDeferTime = m_deferInput.DeferTime;
	long lCompleteCode = Defer(sttDefer,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWDefer::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo ���µļ�������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWUpdateSignCardInfo::CRWUpdateSignCardInfo(RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo,RWID& rwid):CRWCommands(rwid)
{
	m_updateSignCardInfo.SerialNo = updateSignCardInfo.SerialNo;
	m_updateSignCardInfo.TranDateTime = updateSignCardInfo.TranDateTime;
	m_updateSignCardInfo.CardholderType = updateSignCardInfo.CardholderType;
	m_updateSignCardInfo.StaffType = updateSignCardInfo.StaffType;
	memcpy(m_updateSignCardInfo.Name,updateSignCardInfo.Name,sizeof(m_updateSignCardInfo.Name));
	memcpy(m_updateSignCardInfo.CertificateNo,updateSignCardInfo.CertificateNo,sizeof(m_updateSignCardInfo.CertificateNo));
	m_updateSignCardInfo.CertificateType = updateSignCardInfo.CertificateType;
	m_updateSignCardInfo.Deposit = updateSignCardInfo.Deposit;
	m_updateSignCardInfo.SexType = updateSignCardInfo.SexType;
	m_updateSignCardInfo.AppVer = updateSignCardInfo.AppVer;
	m_updateSignCardInfo.CertificateValidDate = updateSignCardInfo.CertificateValidDate;
	memcpy(m_updateSignCardInfo.Tel,updateSignCardInfo.Tel,sizeof(m_updateSignCardInfo.Tel));
	memcpy(m_updateSignCardInfo.Email,updateSignCardInfo.Email,sizeof(m_updateSignCardInfo.Email));
	memcpy(m_updateSignCardInfo.Addr1,updateSignCardInfo.Addr1,sizeof(m_updateSignCardInfo.Addr1));
	memcpy(m_updateSignCardInfo.Addr2,updateSignCardInfo.Addr2,sizeof(m_updateSignCardInfo.Addr2));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWUpdateSignCardInfo::~CRWUpdateSignCardInfo(void)
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
long CRWUpdateSignCardInfo::IsValidCommand()
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
long CRWUpdateSignCardInfo::ExecuteCommand(void)
{
	DAT::STT_RENMCD_CHG sttSignCardInfo;
	sttSignCardInfo.uUdsn = m_updateSignCardInfo.SerialNo;
	m_updateSignCardInfo.TranDateTime.Serialize(sttSignCardInfo.mDateTime);
	sttSignCardInfo.byTypeDsg = m_updateSignCardInfo.CardholderType;
	sttSignCardInfo.byStaffDsg = m_updateSignCardInfo.StaffType;
	memcpy(sttSignCardInfo.szName,m_updateSignCardInfo.Name,sizeof(sttSignCardInfo.szName));
	memcpy(sttSignCardInfo.szCreditNo,m_updateSignCardInfo.CertificateNo,sizeof(sttSignCardInfo.szCreditNo));
	sttSignCardInfo.byCreditType = m_updateSignCardInfo.CertificateType;
	sttSignCardInfo.wLfAmt = m_updateSignCardInfo.Deposit;
	sttSignCardInfo.bySex = m_updateSignCardInfo.SexType;
	sttSignCardInfo.byAppVer = m_updateSignCardInfo.AppVer;
	Time2BCD(m_updateSignCardInfo.CertificateValidDate,sttSignCardInfo.mCertificateValidity);
	memcpy(sttSignCardInfo.mTel,m_updateSignCardInfo.Tel,sizeof(sttSignCardInfo.mTel));
	memcpy(sttSignCardInfo.mEmail,m_updateSignCardInfo.Email,sizeof(sttSignCardInfo.mEmail));
	memcpy(sttSignCardInfo.mAddr1,m_updateSignCardInfo.Addr1,sizeof(sttSignCardInfo.mAddr1));
	memcpy(sttSignCardInfo.mAddr2,m_updateSignCardInfo.Addr2,sizeof(sttSignCardInfo.mAddr2));
	long lCompleteCode = UpdateSignCardInfo(sttSignCardInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	OnComplete(lCompleteCode);
	return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWUpdateSignCardInfo::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo ���µļ�������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReleaseEs::CRWReleaseEs(RW_CMD_ES_PM_WRITE_CARD_INFO structReleaseEs,RWID& rwid):CRWCommands(rwid)
{
	m_structReleaseEs.DealNo = structReleaseEs.DealNo;
	m_structReleaseEs.DealTime = structReleaseEs.DealTime;
	m_structReleaseEs.OperitionMode = structReleaseEs.OperitionMode;
	m_structReleaseEs.FormVisionNO = structReleaseEs.FormVisionNO;
	m_structReleaseEs.FormVisionNO = structReleaseEs.FormVisionNO;
	m_structReleaseEs.KeyEdition = structReleaseEs.KeyEdition;
	m_structReleaseEs.CityCode = structReleaseEs.CityCode;
	m_structReleaseEs.TradeCode = structReleaseEs.TradeCode;
	memcpy(m_structReleaseEs.SupplyTime , structReleaseEs.SupplyTime,3);
	m_structReleaseEs.SupplyDatch = structReleaseEs.SupplyDatch;
	m_structReleaseEs.CardLogicalNumber = structReleaseEs.CardLogicalNumber;
	memcpy(m_structReleaseEs.Reserve1, structReleaseEs.Reserve1,8);
	m_structReleaseEs.ReleaseID = structReleaseEs.ReleaseID;
	m_structReleaseEs.CardType = structReleaseEs.CardType;
	m_structReleaseEs.ProductCategory = structReleaseEs.ProductCategory;
	m_structReleaseEs.ProductType = structReleaseEs.ProductType;
	m_structReleaseEs.ProductSubtype = structReleaseEs.ProductSubtype;
	m_structReleaseEs.PassengerType = structReleaseEs.PassengerType;
	m_structReleaseEs.CardAmount = structReleaseEs.CardAmount;
	m_structReleaseEs.CardTimes = structReleaseEs.CardTimes;
	m_structReleaseEs.ValidityPeriod = structReleaseEs.ValidityPeriod;
	m_structReleaseEs.ValidityPeriodUnit = structReleaseEs.ValidityPeriodUnit;
	m_structReleaseEs.CardDeposit = structReleaseEs.CardDeposit;
	m_structReleaseEs.BookingAmount = structReleaseEs.BookingAmount;
	m_structReleaseEs.VolidStartTime = structReleaseEs.VolidStartTime;
	memcpy(m_structReleaseEs.Reserve2, structReleaseEs.Reserve2,8);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWReleaseEs::~CRWReleaseEs(void)
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
long CRWReleaseEs::IsValidCommand()
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
long CRWReleaseEs::ExecuteCommand(void)
{
	return 0;
	//DAT::STRUCT_ReleaseES struct_ReleaseEs;
	//struct_ReleaseEs.OperitionMode = m_structReleaseEs.OperitionMode;
	//struct_ReleaseEs.FormVisionNO = m_structReleaseEs.FormVisionNO;
	//struct_ReleaseEs.ManufID = 1;
	//struct_ReleaseEs.KeyEdition = m_structReleaseEs.KeyEdition;
	//struct_ReleaseEs.CityCode = 0x0082;
	//struct_ReleaseEs.TradeCode = 0x0300;
	//memcpy(struct_ReleaseEs.SupplyTime,m_structReleaseEs.SupplyTime,3);
	//struct_ReleaseEs.SupplyDatch = m_structReleaseEs.SupplyDatch;
	//struct_ReleaseEs.CardLogNo = m_structReleaseEs.CardLogicalNumber;
	//memcpy(struct_ReleaseEs.Reserve1, m_structReleaseEs.Reserve1,4);
	//struct_ReleaseEs.ReleaseID = m_structReleaseEs.ReleaseID;
	//struct_ReleaseEs.CardType = m_structReleaseEs.CardType;
	//struct_ReleaseEs.ProductCategory = m_structReleaseEs.ProductCategory;
	//struct_ReleaseEs.ProductType = m_structReleaseEs.ProductType;
	//struct_ReleaseEs.ProductSubtype = m_structReleaseEs.ProductSubtype;
	//struct_ReleaseEs.PassengerType = m_structReleaseEs.PassengerType;
	//struct_ReleaseEs.CardAmount = m_structReleaseEs.CardAmount;
	//struct_ReleaseEs.CardTimes = m_structReleaseEs.CardTimes;
	//struct_ReleaseEs.ValidityPeriod = m_structReleaseEs.ValidityPeriod;
	//struct_ReleaseEs.ValidityPeriodUnit = m_structReleaseEs.ValidityPeriodUnit;
	//struct_ReleaseEs.CardDeposit = m_structReleaseEs.CardDeposit;
	//struct_ReleaseEs.BookingAmount = m_structReleaseEs.BookingAmount;
	//Time2BCD(m_structReleaseEs.VolidStartTime,struct_ReleaseEs.VolidStartTime);
	//memcpy(struct_ReleaseEs.Reserve2, m_structReleaseEs.Reserve2,8);
	//uchar TranDateTime[8] = {0};
	//m_structReleaseEs.DealTime.Serialize(TranDateTime);
	////	m_structReleaseEs.DealTime.Serialize(struct_ReleaseEs.VolidStartTime);	
	//long lCompleteCode =ReleaseEs(m_structReleaseEs.DealNo,
	//	TranDateTime,
	//	struct_ReleaseEs,
	//	m_pResponse.SerialAddnum,
	//	m_pResponse.TranResult,
	//	m_pResponse.UDlen,
	//	m_pResponse.UDData,
	//	m_pResponse.ARLen,
	//	m_pResponse.ARData,
	//	m_pResponse.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CESException
*/
//////////////////////////////////////////////////////////////////////////
void CRWReleaseEs::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_pResponse;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo ���µļ�������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWESPersonalize::CRWESPersonalize(RW_CMD_ES_PM_WRITE_CARD_INFO individuationInfo,RWID& rwid):CRWCommands(rwid)
{
	m_individuationInfo.DealNo = individuationInfo.DealNo;
	m_individuationInfo.DealTime = individuationInfo.DealTime;
	m_individuationInfo.OperitionMode = individuationInfo.OperitionMode;
	m_individuationInfo.CorporationCode = individuationInfo.CorporationCode;
	m_individuationInfo.PersonalNO = individuationInfo.PersonalNO;
	memcpy(m_individuationInfo.CardHolderName, individuationInfo.CardHolderName,sizeof(m_individuationInfo.CardHolderName));
	m_individuationInfo.CredentialsNOType = individuationInfo.CredentialsNOType;
	memcpy(m_individuationInfo.CredentialsNO, individuationInfo.CredentialsNO,sizeof(m_individuationInfo.CredentialsNO));
	m_individuationInfo.Birthday = individuationInfo.Birthday;
	m_individuationInfo.PassengerLevel = individuationInfo.PassengerLevel;
	memcpy(m_individuationInfo.Reserve, individuationInfo.Reserve,sizeof(m_individuationInfo.Reserve));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWESPersonalize::~CRWESPersonalize(void)
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
long CRWESPersonalize::IsValidCommand()
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
long CRWESPersonalize::ExecuteCommand(void)
{
	return 0;
	//DAT::Struct_Individuation struct_individuation;
	//struct_individuation.OperitionMode = m_individuationInfo.OperitionMode;
	//memcpy(&struct_individuation.CorporationCode, &m_individuationInfo.CorporationCode, 2);
	//memcpy(&struct_individuation.PersonalNO, &m_individuationInfo.PersonalNO, 4);
	//memcpy(struct_individuation.CardHolderName, m_individuationInfo.CardHolderName,sizeof(m_individuationInfo.CardHolderName));
	//struct_individuation.CredentialsNOType = m_individuationInfo.CredentialsNOType;
	//memcpy(struct_individuation.CredentialsNO, m_individuationInfo.CredentialsNO,sizeof(m_individuationInfo.CredentialsNO));
	//memcpy(&struct_individuation.Birthday, &m_individuationInfo.Birthday, 4);
	//struct_individuation.PassengerLevel = m_individuationInfo.PassengerLevel;
	//memcpy(struct_individuation.Reserve, m_individuationInfo.Reserve,sizeof(m_individuationInfo.Reserve));
	//long lCompleteCode =IndividuationPm(m_individuationInfo.DealNo,
	//	(uchar*)(m_individuationInfo.DealTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")).GetBuffer()),
	//	struct_individuation,
	//	m_pResponse.SerialAddnum,
	//	m_pResponse.TranResult,
	//	m_pResponse.UDlen,
	//	m_pResponse.UDData,
	//	m_pResponse.ARLen,
	//	m_pResponse.ARData,
	//	m_pResponse.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CESException
*/
//////////////////////////////////////////////////////////////////////////
void CRWESPersonalize::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_pResponse;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo ���Ի���Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWPersonalize::CRWPersonalize(RW_CMD_PERSONALIZE_INFO personalizeInfo,RWID& rwid):CRWCommands(rwid)
{
	m_personalizeInfo.SerialNo = personalizeInfo.SerialNo;
	m_personalizeInfo.TranDateTime = personalizeInfo.TranDateTime;
	m_personalizeInfo.OperationType = personalizeInfo.OperationType;
	memcpy(m_personalizeInfo.CorporationCode, personalizeInfo.CorporationCode, sizeof(m_personalizeInfo.CorporationCode));
	memcpy(m_personalizeInfo.StaffNumber, personalizeInfo.StaffNumber, sizeof(m_personalizeInfo.StaffNumber));
	memcpy(m_personalizeInfo.CardHolderName,personalizeInfo.CardHolderName,sizeof(m_personalizeInfo.CardHolderName));
	m_personalizeInfo.CertificateType = personalizeInfo.CertificateType;
	memcpy(m_personalizeInfo.CertificateID,personalizeInfo.CertificateID,sizeof(m_personalizeInfo.CertificateID));
	m_personalizeInfo.Birthday = personalizeInfo.Birthday;
	m_personalizeInfo.PassengerLevel = personalizeInfo.PassengerLevel;
	m_personalizeInfo.PaymentAmount = personalizeInfo.PaymentAmount;
	m_personalizeInfo.PaymentMethod = personalizeInfo.PaymentMethod;
	memcpy(m_personalizeInfo.BankCarkNumber,personalizeInfo.BankCarkNumber,sizeof(m_personalizeInfo.BankCarkNumber));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWPersonalize::~CRWPersonalize(void)
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
long CRWPersonalize::IsValidCommand()
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
long CRWPersonalize::ExecuteCommand(void)
{
	return 0;
	//DAT::Struct_Individuation individuationInfo;
	//individuationInfo.OperitionMode = m_personalizeInfo.OperationType;
	////memcpy(individuationInfo.CorporationCode, m_personalizeInfo.CorporationCode, 2);
	////memcpy(individuationInfo.PersonalNO, m_personalizeInfo.StaffNumber, 4);
	////memcpy(individuationInfo.CardHolderName,m_personalizeInfo.CardHolderName,sizeof(individuationInfo.CardHolderName));
	//individuationInfo.CredentialsNOType = m_personalizeInfo.CertificateType;
	//memcpy(individuationInfo.CredentialsNO,m_personalizeInfo.CertificateID,sizeof(individuationInfo.CredentialsNO));
	////m_personalizeInfo.Birthday.Serialize(individuationInfo.Birthday);//ת��BCD��ʽ
	//individuationInfo.PassengerLevel = m_personalizeInfo.PassengerLevel;
	//individuationInfo.PayAmount = m_personalizeInfo.PaymentAmount;
	//individuationInfo.PayMethed = m_personalizeInfo.PaymentMethod;
	//memcpy(individuationInfo.BankCardNo,m_personalizeInfo.BankCarkNumber,sizeof(individuationInfo.BankCardNo));

	//BYTE tranDateTime[7] = {0};
	//m_personalizeInfo.TranDateTime.Serialize(tranDateTime);
	//long lCompleteCode = Personalize(m_personalizeInfo.SerialNo, tranDateTime, individuationInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWPersonalize::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo ���Ի���Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWPersonalizeUpdateOldCard::CRWPersonalizeUpdateOldCard(RW_CMD_PERSONALIZE_INFO personalizeInfo,RWID& rwid):CRWCommands(rwid)
{
	m_personalizeInfo.SerialNo = personalizeInfo.SerialNo;
	m_personalizeInfo.TranDateTime = personalizeInfo.TranDateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWPersonalizeUpdateOldCard::~CRWPersonalizeUpdateOldCard(void)
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
long CRWPersonalizeUpdateOldCard::IsValidCommand()
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
long CRWPersonalizeUpdateOldCard::ExecuteCommand(void)
{
	return 0;
	//BYTE tranDateTime[7] = {0};
	//m_personalizeInfo.TranDateTime.Serialize(tranDateTime);
	//long lCompleteCode = PersonalizeUpdateOldCard(m_personalizeInfo.SerialNo,tranDateTime,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWPersonalizeUpdateOldCard::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_PERSONALIZE_INFO personalizeInfo ���Ի���Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWPersonalizeUpdateNewCard::CRWPersonalizeUpdateNewCard(RW_CMD_PERSONALIZE_INFO personalizeInfo,RWID& rwid):CRWCommands(rwid)
{
	//m_personalizeInfo.SerialNo = personalizeInfo.SerialNo;
	//m_personalizeInfo.TranDateTime = personalizeInfo.TranDateTime;
	//m_personalizeInfo.OperationType = personalizeInfo.OperationType;
	//m_personalizeInfo.CorporationCode = personalizeInfo.CorporationCode;
	//m_personalizeInfo.StaffNumber = personalizeInfo.StaffNumber;
	//memcpy(m_personalizeInfo.CardHolderName,personalizeInfo.CardHolderName,sizeof(m_personalizeInfo.CardHolderName));
	//m_personalizeInfo.CertificateType = personalizeInfo.CertificateType;
	//memcpy(m_personalizeInfo.CertificateID,personalizeInfo.CertificateID,sizeof(m_personalizeInfo.CertificateID));
	//m_personalizeInfo.Birthday = personalizeInfo.Birthday;
	//m_personalizeInfo.PassengerLevel = personalizeInfo.PassengerLevel;
	//m_personalizeInfo.PaymentAmount = personalizeInfo.PaymentAmount;
	//m_personalizeInfo.PaymentMethod = personalizeInfo.PaymentMethod;
	//memcpy(m_personalizeInfo.BankCarkNumber,personalizeInfo.BankCarkNumber,sizeof(m_personalizeInfo.BankCarkNumber));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWPersonalizeUpdateNewCard::~CRWPersonalizeUpdateNewCard(void)
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
long CRWPersonalizeUpdateNewCard::IsValidCommand()
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
long CRWPersonalizeUpdateNewCard::ExecuteCommand(void)
{
	//DAT::Struct_IndivRenewNewCard individuationInfo;
	//individuationInfo.OperitionMode = m_personalizeInfo.OperationType;
	//individuationInfo.CorporationCode = m_personalizeInfo.CorporationCode;
	//individuationInfo.PersonalNO = m_personalizeInfo.StaffNumber;
	//memcpy(individuationInfo.CardHolderName,m_personalizeInfo.CardHolderName,sizeof(individuationInfo.CardHolderName));
	//individuationInfo.CredentialsNOType = m_personalizeInfo.CertificateType;
	//memcpy(individuationInfo.CredentialsNO,m_personalizeInfo.CertificateID,sizeof(individuationInfo.CredentialsNO));
	//BYTE birthday[4] = {0};
	//m_personalizeInfo.Birthday.Serialize(birthday);
	//individuationInfo.Birthday = ComMakeLong(birthday[0], birthday[1], birthday[2], birthday[3]);
	//individuationInfo.PassengerLevel = m_personalizeInfo.PassengerLevel;
	//individuationInfo.PayAmount = m_personalizeInfo.PaymentAmount;
	//individuationInfo.PayMethed = m_personalizeInfo.PaymentMethod;
	//memcpy(individuationInfo.BankCardNo,m_personalizeInfo.BankCarkNumber,sizeof(individuationInfo.BankCardNo));

	//BYTE tranDateTime[7] = {0};
	//m_personalizeInfo.TranDateTime.Serialize(tranDateTime);
	//long lCompleteCode = PersonalizeUpdateNewCard(m_personalizeInfo.SerialNo, tranDateTime, individuationInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)RW_CMD_DEFER_RESPONSE pResponse       

@retval     void     

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CRWPersonalizeUpdateNewCard::GetRWResponse(RW_BUSSINESS_RESPONSE *pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)RW_CMD_UPDATE_SIGNCARD_INFO updateSignCardInfo ���µļ�������Ϣ

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWAuthIssueCardMasterCard::CRWAuthIssueCardMasterCard(AuthIssueCardMasterCardParam authIssueCardMasterCard,RWID& rwid):CRWCommands(rwid)
{
	m_authIssueCardMasterCard.ICMC_Slot = authIssueCardMasterCard.ICMC_Slot;
	m_authIssueCardMasterCard.ICMCT_Slot = authIssueCardMasterCard.ICMCT_Slot;
	m_authIssueCardMasterCard.ICMCT_PIN_Len = authIssueCardMasterCard.ICMCT_PIN_Len;
	memcpy(m_authIssueCardMasterCard.ICMCT_PIN_buff,authIssueCardMasterCard.ICMCT_PIN_buff,authIssueCardMasterCard.ICMCT_PIN_Len);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWAuthIssueCardMasterCard::~CRWAuthIssueCardMasterCard(void)
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
long CRWAuthIssueCardMasterCard::IsValidCommand()
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
long CRWAuthIssueCardMasterCard::ExecuteCommand(void)
{
	return 0;
	//DAT::Struct_AuthIssueCardMasterCard struct_authIssueCardMasterCard;
	//struct_authIssueCardMasterCard.ICMC_Slot = m_authIssueCardMasterCard.ICMC_Slot;
	//struct_authIssueCardMasterCard.ICMCT_Slot = m_authIssueCardMasterCard.ICMCT_Slot;
	//struct_authIssueCardMasterCard.ICMCT_PIN_Len = m_authIssueCardMasterCard.ICMCT_PIN_Len;
	//memset(struct_authIssueCardMasterCard.ICMCT_PIN_buff,0,32);
	//memcpy(struct_authIssueCardMasterCard.ICMCT_PIN_buff,m_authIssueCardMasterCard.ICMCT_PIN_buff,2);
	//long lCompleteCode =AuthIssueCardMasterCard(struct_authIssueCardMasterCard,m_response);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �õ�RW������Ϣ

@param      (o)BYTE* pResponse    ��д��״̬     

@retval     void     

@exception  CESException
*/
//////////////////////////////////////////////////////////////////////////
void CRWAuthIssueCardMasterCard::GetRWResponse(RW_RSP_STATUS* pResponse)
{
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���캯��

@param      (i)RW_CMD_CONSUME cmd_consume ���Ѳ���

@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWConsumeEP::CRWConsumeEP(RW_CMD_CONSUME cmd_consume,RWID& rwID):CRWCommands(rwID){
	m_Cmd_Consume.SerialNo = cmd_consume.SerialNo;
	m_Cmd_Consume.TranDateTime = cmd_consume.TranDateTime;
	m_Cmd_Consume.ConsumeAmount = cmd_consume.ConsumeAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWConsumeEP::~CRWConsumeEP(void){

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ִ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWConsumeEP::ExecuteCommand(){
	return 0;
	//DAT::STRUCT_consume consumeInfo;	
	//consumeInfo.ConsumeAmount = m_Cmd_Consume.ConsumeAmount;			// ���׽��
	//BYTE tranDateTime[7];
	//memset(tranDateTime,0,7);
	//m_Cmd_Consume.TranDateTime.Serialize(tranDateTime);
	//long lCompleteCode = ConsumeEPPay(m_Cmd_Consume.SerialNo,tranDateTime,consumeInfo,m_response.SerialAddnum,m_response.TranResult,m_response.UDlen,m_response.UDData,m_response.ARLen,m_response.ARData,m_response.DetailErrorCode);
	//OnComplete(lCompleteCode);
	//return lCompleteCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ�����Ч����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CRWConsumeEP::IsValidCommand(){
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡִ�н��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CRWConsumeEP::GetRWResponse(RW_BUSSINESS_RESPONSE* pResponse){
	try {
		if(pResponse == NULL){
			throw CRWException(m_RWID.rwtype,CRWException::FUNC_PARAM_ERR,_T(__FILE__),__LINE__);
		}
		else{
			*pResponse = m_response;
		}
	}
	catch(CSysException&) {
		throw;
	}
	catch(...) {
		throw CRWException(m_RWID.rwtype,CRWException::OTHER_ERR,_T(__FILE__),__LINE__,_T("�ڴ�ռ丳ֵ����"));
	}
}