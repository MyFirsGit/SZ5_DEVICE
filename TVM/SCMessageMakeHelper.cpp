#include "StdAfx.h"
#include "SCMessageMakeHelper.h"
#include "AppSession.h"
#include "tvmstatusmgr.h"
#include "CTLOGCountInfo.h"
#include "versioninfo.h"
#include "cparametermaster.h"
#include "CAfcStaffPwdParam.h"
#include "CAfcOperatorCodeTableParam.h"
#include "CAfcDeviceTimeParam.h"
#include "CAfcBOMOperationParam.h"
#include "CAfcAccessLevelParam.h"
#include "CAfcDeviceCommonParam.h"
//#include "CAccBusinessParam.h"
#include "CTLOGSoftVer.h"
#include "TicketHelper.h"
#include "DeviceAR.h"
#include "CTBanknoteCountInfo.h"

CSCMessageMakeHelper CSCMessageMakeHelper::instance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CSCMessageHelper���캯��
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageMakeHelper::CSCMessageMakeHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CSCMessageHelper��������
@param      ��
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageMakeHelper::~CSCMessageMakeHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��AFCϵͳר��ͷ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
AfcSysHdr_t	CSCMessageMakeHelper::AquireAfcSysHeader(WORD wDataLen)
{
	AfcSysHdr_t header;
	int nOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	int2BCD(nOperatorID, (char *)header.OperatorID, sizeof header.OperatorID);
	header.DateTime = ComGetBusiDate();
	header.DataLen = wDataLen;
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ϵͳ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
SysComHdr_t CSCMessageMakeHelper::AquireSysComHeader(WORD msgType)
{
	SysComHdr_t header;
	header.transTime  = ComGetCurTime();
	header.resourceProviderID = char2BCD(0x02);//��������2������Ӫ��˾
	header.transType = HIBYTE(msgType);
	header.transSubType = LOBYTE(msgType);
	header.cityCode = 1100;//���д���
	header.industryCode = 0003;//��ҵ����
	header.lineCode =theMAINTENANCE_INFO.GetCurrentACCDevice().lineCode;
	header.stationCode = theMAINTENANCE_INFO.GetCurrentACCDevice().stationCode;
	header.deviceType = theMAINTENANCE_INFO.GetCurrentACCDevice().deviceType;
	header.deviceID = theMAINTENANCE_INFO.GetCurrentACCDevice();
	header.transSerialNo = theCOUNT_INFO.GetUDSN();
	if(theTVM_SETTING.GetDeviceTestMode() == 0){
		header.dataType = 0x00;			//��������
	}
	else{
		header.dataType = 0x01;			//��������
	}	
	header.parameterVersion = 0;
	//memcpy(header.samID,theAPP_SESSION.GetSAMInfo().rw_acc_psam_id,sizeof header.samID);
	header.samTransSerialNo = 0;
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��Ʊ��������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
SysCardCom_t CSCMessageMakeHelper::AquireSysCardComHeader()
{
	SysCardCom_t header;
	header.cardIssuerId = 0;
	header.cardSerialNumber = 0;
	header.cardType = 0;
	header.cardLifeCycleCount = 0;
	header.cardActionSequenceNumber = 0;
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�óֿ��˹�����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
SysCardholderCom_t CSCMessageMakeHelper::AquireSysCardholderComHeader()
{
	SysCardholderCom_t header;
	header.cardholderSerialNum = 0;
	header.cardholderIssuerId = 0;
	header.companyId = 0;
	header.classificationLevel = 0;
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ���ڲ��������ݹ�����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
TransComHdr_t CSCMessageMakeHelper::AquireTransComHeader(WORD msgType)
{
	TransComHdr_t transComHeader;
	transComHeader.deviceid = theMAINTENANCE_INFO.GetCurrentDevice();
	transComHeader.dwAfcSerialNo = theCOUNT_INFO.GetTransactionNum();
	transComHeader.dtTransDataTime = ComGetCurTime();
	transComHeader.bMsgType = HIBYTE(msgType);
	transComHeader.bMsgSubType = LOBYTE(msgType);
	return transComHeader;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ҵ������ͷ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
OperComHdr_t CSCMessageMakeHelper::AquireOperHeader(WORD msgType,WORD wDataLen)
{
	OperComHdr_t operComHdr;
	int nOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	int2BCD(nOperatorID, (char *)operComHdr.operatorID, sizeof(operComHdr.operatorID));
	operComHdr.dataTime = ComGetBusiDate();
	operComHdr.localType = 0x06;
	operComHdr.localAreaID = theMAINTENANCE_INFO.GetStationCodeWithLine();
	operComHdr.deviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	operComHdr.afcSeqNo = theCOUNT_INFO.GetTransactionNum();
	operComHdr.operTime = ComGetCurTime();
	operComHdr.msgType = HIBYTE(msgType);
	operComHdr.msgSubType = LOBYTE(msgType);
	operComHdr.msgLen = wDataLen;
	return operComHdr;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ȡ��״̬����ͷ   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
StatusComHdr_t CSCMessageMakeHelper::AquireStatusComHeader(WORD msgType)
{
	StatusComHdr_t header;
	header.wStatusCode = msgType;
	header.wStationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
	header.deviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	int nOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	int2BCD(nOperatorID, header.operatorID, sizeof header.operatorID);
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����AFCϵͳ����ͷ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeAfcSysHdr(AfcSysHdr_t& afcSysHder, LPBYTE lpMsg)
{
	// 3 BIN ����ԱID
	memcpy(lpMsg, afcSysHder.OperatorID,sizeof(afcSysHder.OperatorID));
	lpMsg += sizeof(afcSysHder.OperatorID);
	// 4 BIN ��������
	Date2BCD(afcSysHder.DateTime,lpMsg);
	lpMsg += 4;
	// 2 BIN ��Ϣ����
	memcpy(lpMsg,&afcSysHder.DataLen,2);
	lpMsg+=2;
	// 1 BIN Ԥ��
	*lpMsg = 0xFF;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ϵͳ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysComHdr(SysComHdr_t& sysComHdr,LPBYTE lpMsg)
{
	sysComHdr.transTime.Serialize(lpMsg);
	lpMsg+=7;
	int2BCD(sysComHdr.resourceProviderID,(char*)lpMsg,2);
	lpMsg+=2;
	*lpMsg = sysComHdr.transType;
	lpMsg+=1;
	*lpMsg = sysComHdr.transSubType;
	lpMsg+=1;
	int2BCD(sysComHdr.cityCode,(char*)lpMsg,2);
	lpMsg+=2;
	int2BCD(sysComHdr.industryCode,(char*)lpMsg,2);
	lpMsg+=2;
	*lpMsg = sysComHdr.lineCode;
	lpMsg+=1;
	*lpMsg = HIBYTE(sysComHdr.stationCode);
	lpMsg++;
	*lpMsg = LOBYTE(sysComHdr.stationCode);
	lpMsg++;
	*lpMsg = sysComHdr.deviceType;
	lpMsg+=1;
	sysComHdr.deviceID.Serialize(lpMsg);
	lpMsg+=6;
	ComRevLong(sysComHdr.transSerialNo,lpMsg);
	lpMsg+=4;
	*lpMsg = sysComHdr.dataType;
	lpMsg+=1;
	int2BCD(sysComHdr.parameterVersion,(char*)lpMsg,2);
	lpMsg+=2;
	memcpy(lpMsg,sysComHdr.samID,6);
	lpMsg+=6;
	ComRevLong(sysComHdr.samTransSerialNo,lpMsg);
	lpMsg+=4;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ����Ʊ�������� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysCardCom(SysCardCom_t& sysCardCom,LPBYTE lpMsg)
{
	// 4 BIN ����Ʊ���ķ����˵�ΨһID
	ComRevLong(sysCardCom.cardIssuerId, lpMsg);
	lpMsg += 4;
	// 4 BIN Ʊ�������
	ComRevLong(sysCardCom.cardSerialNumber, lpMsg);
	lpMsg += 4;
	// 4 BIN Ʊ��������
	ComRevLong(sysCardCom.cardType, lpMsg);
	lpMsg += 4;
	// 4 BIN Ʊ���ĵ�ǰʹ�����ڼ���
	ComRevLong(sysCardCom.cardLifeCycleCount, lpMsg);
	lpMsg += 4;
	// 4 BIN ���������к�
	ComRevLong(sysCardCom.cardActionSequenceNumber, lpMsg);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ���ɳֿ��˹����� 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysCardholderCom(SysCardholderCom_t& sysCardholderCom,LPBYTE lpMsg)
{    
	// 4 BIN ���ض������˷�Χ��ʶ��ֿ��˵�ר�ú���
	ComRevLong(sysCardholderCom.cardholderSerialNum, lpMsg);
	lpMsg += 4;
	// 4 BIN �ֿ��˵ķ�����
	ComRevLong(sysCardholderCom.cardholderIssuerId, lpMsg);
	lpMsg += 4;
	// 4 BIN ACC ���幫˾��ʶ����
	ComRevLong(sysCardholderCom.companyId, lpMsg);
	lpMsg += 4;
	// 4 BIN �˿ͼ���
	ComRevLong(sysCardholderCom.classificationLevel, lpMsg);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   �����ڲ��������ݹ�����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg)
{
	// 4 1BIN+2BCD+1BIN �豸ID
	transComHdr.deviceid.Serialize(lpMsg);
	lpMsg+=4;
	// 4 BIN AFC������ˮ��
	memcpy(lpMsg, &transComHdr.dwAfcSerialNo, 4);
	lpMsg += 4;
	// 7 BCD ���ײ���ʱ��
	Time2BCD(transComHdr.dtTransDataTime,lpMsg);
	lpMsg+=7;
	// 1 BIN ��������
	*lpMsg = transComHdr.bMsgType;
	lpMsg++;
	// 1 BIN ����������
	*lpMsg = transComHdr.bMsgSubType;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ����ҵ������ר��ͷ

@param     OperComHdr_t      ҵ������ר��ͷ

@retval    ��

@exception 
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperComHdr(OperComHdr_t& OperComHdr, LPBYTE lpMsg)
{
	// 3 BCD ����ԱID
	memcpy(lpMsg, OperComHdr.operatorID, sizeof(OperComHdr.operatorID));
	lpMsg += 3;
	// 4 BCD ��������
	Date2BCD(OperComHdr.dataTime, lpMsg);
	lpMsg += 4;
	// 1 BIN λ������
	*lpMsg = OperComHdr.localType;
	lpMsg++;
	// 2 BCD λ��ID
	BYTE btArrLocal[2];
	memset(btArrLocal,0x00,2);
	int2BCD(OperComHdr.localAreaID,(char*)btArrLocal,2);

	*lpMsg = btArrLocal[0];//HIBYTE(OperComHdr.localAreaID);
	lpMsg++;
	*lpMsg = btArrLocal[1];//LOBYTE(OperComHdr.localAreaID);
	lpMsg++;
	// 4 1BIN+2BCD+1BIN �豸ID
	OperComHdr.deviceID.Serialize(lpMsg);
	lpMsg += 4;
	// 4 BIN AFC������ˮ��
	memcpy(lpMsg, &OperComHdr.afcSeqNo, 4);
	lpMsg += 4;
	// 7 BCD ҵ�����ݲ���ʱ��
	Time2BCD(OperComHdr.operTime, lpMsg);
	lpMsg += 7;
	// 1 BIN ��������
	*lpMsg = OperComHdr.msgType;
	lpMsg++;
	// 1 BIN ����������
	*lpMsg = OperComHdr.msgSubType;
	lpMsg++;
	// 2 BIN ��Ϣ����
	memcpy(lpMsg, &OperComHdr.msgLen, 2);
	lpMsg += 2;   
	// 3 Ԥ��
	memset(lpMsg, 0xFF, 3);
	lpMsg += 3;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����״̬����ͷ

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg)
{
	// 2 BIN ����������
	memcpy(lpMsg, &statusComHdr.wStatusCode, 2);
	lpMsg+=2;
	// 2 BCD ��վID
	char chStationID[2] = {0};
	int2BCD(statusComHdr.wStationCode,&chStationID[0],2);
	*lpMsg = chStationID[0];//HIBYTE(statusComHdr.wStationCode);
	lpMsg++;
	*lpMsg = chStationID[1];//LOBYTE(statusComHdr.wStationCode);
	lpMsg++;
	// 4 1BIN+2BCD+1BIN �豸ID
	statusComHdr.deviceID.Serialize(lpMsg);
	lpMsg += 4;
	// 3 BCD ����ԱID
	memcpy(lpMsg, statusComHdr.operatorID, sizeof(statusComHdr.operatorID));
	lpMsg += sizeof(statusComHdr.operatorID);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ɰ�ȫ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysSecurityHdr(SysSecurityHdr_t& sysSecurityHdr,LPBYTE lpMsg)
{
	// 8 BIN MAC
	memcpy(lpMsg, sysSecurityHdr.txnMac, 8);
	lpMsg += 8;
	// 4 BIN ������������MAC ����Կ�汾
	ComRevLong(sysSecurityHdr.keyVersion, lpMsg);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		����ACC�豸�Ĵ�������

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnAuditRegister(TXN_AUDIT_REGISTER& TxnAuditRegister, ACCAR& arData, LPBYTE lpMsg, LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFCר��ͷ
	MakeAfcSysHdr(TxnAuditRegister.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// ϵͳ����ͷ
	MakeSysComHdr(TxnAuditRegister.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;

	// �����м�/ֵ�Ե����ֺ�
	DWORD dataNumber = 0;
	LPBYTE lpNumberPos = lpData;
	lpData += 4;

	// ����������ݵĿ���ԭ��
	ComRevLong(1,lpData);
	lpData += 4;

	// ��¼����
	LPBYTE lpValue = lpData;
	LPBYTE lpId = lpData + 1200;
	for (ACCAR::iterator itemIter = arData.begin();itemIter != arData.end();itemIter++)
	{
		if(itemIter->first > 512 || itemIter->second == 0)
		{
			continue;
		}
		// ��¼ֵ
		ComRevLong(itemIter->second,lpValue);
		lpValue += 4;
		// ��¼ID
		ComRevLong(itemIter->first,lpId);
		lpId += 4;

		dataNumber++;
	}

	// �����м�/ֵ�Ե����ֺ�
	ComRevLong(dataNumber,lpNumberPos);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		����CD�汾�л��������ݣ�Ԥ����

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnEventCDVersionStatus(TXN_EVENT_CD_VERSION_STATUS& TxnEventCDVersionStatus,LPBYTE lpMsg,LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFCר��ͷ
	MakeAfcSysHdr(TxnEventCDVersionStatus.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// ϵͳ����ͷ
	MakeSysComHdr(TxnEventCDVersionStatus.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;

	// cdGenerationId
	ComRevLong(TxnEventCDVersionStatus.cdGenerationId, lpData);
	lpData += 4;
	// numberOfEntries
	ComRevLong(TxnEventCDVersionStatus.numberOfEntries, lpData);
	lpData += 4;
	// cdPayloadIdList 200 BIN
	ComRevLong(CD_PAYLOAD_ID_SYSTEM_PARAMS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_SERVICE_PROVIDER_PARAMS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_CARD_ACTIONLIST, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_PRODUCT_ACTIONLIST, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_SAM_HOTLIST, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_CALENDARS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_FARE_TABLES, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_FARE_PATTERNS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_FARE_CODES, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_DAY_TYPES, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_LOCATION_NAMES, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_PRODUCT_TYPE_PARAMS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_PARTICIPANT_PARAMS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_MIFARE_PARAMS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_HIGH_SECURITY_BLACKLIST, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_LOCATIONS, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_BUSINESS_RULES, lpData);
	lpData += 4;
	ComRevLong(CD_PAYLOAD_ID_CARD_BATCH_WITHDRAW_LIST, lpData);
	lpData += 4;
	lpData += 128;

	// cdVersionList 200 BIN
	ComRevLong(TxnEventCDVersionStatus.system_params, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.service_provider_params, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.card_actionlist, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.product_actionlist, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.sam_hotlist, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.calendars, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.fare_tables, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.fare_patterns, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.fare_codes, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.day_types, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.location_names, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.product_type_params, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.participant_params, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.mifare_params, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.high_security_blacklist, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.locations, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.business_rules, lpData);
	lpData += 4;
	ComRevLong(TxnEventCDVersionStatus.card_batch_withdraw_list, lpData);
	lpData += 4;
	lpData += 128;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�����豸״̬�������ݣ�Ԥ����

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnEventDeviceStatus(TXN_EVENT_DEVICE_STATUS& TxnEventDeviceStatus,LPBYTE lpMsg,LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFCר��ͷ
	MakeAfcSysHdr(TxnEventDeviceStatus.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// ϵͳ����ͷ
	MakeSysComHdr(TxnEventDeviceStatus.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;

	// �豸״̬
	ComRevLong(TxnEventDeviceStatus.deviceStatus, lpData);
	lpData += 4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		�����豸��ʶ�������ݣ�Ԥ����

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnEventDeviceIdentification(TXN_EVENT_DEVICE_IDENTIFICATION& TxnEventDeviceIdentification,LPBYTE lpMsg,LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFCר��ͷ
	MakeAfcSysHdr(TxnEventDeviceIdentification.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// ϵͳ����ͷ
	MakeSysComHdr(TxnEventDeviceIdentification.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TVM��AVM���ϵ�ҵ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBusinessFaliedData(OperComHdr_t& operComHdr,BUSINESS_FAILED_DATA& inBusinessFailedData,LPBYTE lpByte){
	// AFCר��ͷ
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	
	// �豸���
	inBusinessFailedData.DeviceID.Serialize(lpMsg);
	lpMsg +=4;
	// ����ʱ��
	inBusinessFailedData.DateTime.Serialize(lpMsg);
	lpMsg += 7;
	// �豸AFC��ˮ��
	memcpy(lpMsg,&inBusinessFailedData.DeviceAfcNum,4);
	lpMsg += 4;
	// ��Ӧ������ˮ��
	memcpy(lpMsg,&inBusinessFailedData.TransactionNum,4);
	lpMsg += 4;
	// Ӧ�Կ���
	memcpy(lpMsg,&inBusinessFailedData.RefoundAmount,4);
	lpMsg += 4;
	// ����ԭ��
	memcpy(lpMsg,&inBusinessFailedData.FailedReason,12);
	lpMsg += 12;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ͳ�������ҵ������
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDownloadParameterAndProgram(OperComHdr_t& operComHdr,vector<ParameterAndSoftwareUpdate>& updates, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 1BIN ��������
	char count = updates.size();
	memcpy(lpMsg, &count, 1);
	lpMsg += 1;
	for (vector<ParameterAndSoftwareUpdate>::iterator it = updates.begin(); it != updates.end(); it++)
	{
		// 2 BCD �����ͳ���ID
		//int2BCD(it->parameterID, (char*)lpMsg, 2);
		ComRevWORD(it->parameterID, lpMsg);
		lpMsg += 2;
		// 2 BIN �豸��Ӧ��ID
		memcpy(lpMsg,&(it->providerID),2 );
		lpMsg += 2;
		// 2 BIN ������κ�
		memcpy(lpMsg,&(it->softwareBatchNO),2 );
		lpMsg += 2;
		// 1 BIN �汾����
		*lpMsg = it->versionType;
		lpMsg++;
		// 4 BIN �汾��
		memcpy(lpMsg,&(it->version),4 );
		lpMsg += 4;
		// 7 BCD ����ʱ��
		_DATE_TIME currentTime = ComGetCurTime();
		Time2BCD(currentTime, lpMsg);
		lpMsg += 7;
		// 32 ASCII �ļ�����
		CString& downLoadPath = it->downLoadPath;
		memcpy(lpMsg, downLoadPath.GetBuffer(), downLoadPath.GetLength()>32 ? 32 :downLoadPath.GetLength());
		lpMsg += 32;		
		// 1 BIN ���ؽ��
		*lpMsg = 0x00;
		lpMsg++;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ͳ���
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
long CSCMessageMakeHelper::MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;

	long lDataLen = 34;
	// �л�����
	WORD wCount = 0;
	wCount = VecVersionInfo.size();
	operComHdr.msgLen = lDataLen + wCount*23;
	// ҵ��ͷ����
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 2 BIN �л�����
	memcpy(lpMsg, &wCount, 2);
	lpMsg += 2;
	CParamHelper::VEC_VERSION_INFO::iterator itor;
	for (itor = VecVersionInfo.begin(); itor != VecVersionInfo.end(); itor++)
	{
		// 2 BCD �����ͳ���ID
		int2BCD((*itor).paramId, (char*)lpMsg, 2);
		lpMsg += 2;
		// 2 BIN �豸��Ӧ��ID
		memset(lpMsg, 0x00, 2);
		lpMsg += 2;
		// 2 BIN ������κ�
		memset(lpMsg, 0x00, 2);
		lpMsg += 2;
		// 1 BIN �汾����
		*lpMsg = (*itor).versionType;
		lpMsg++;
		// 4 BIN �ɰ汾��
		memcpy(lpMsg, &(*itor).oldVersion, 4);
		lpMsg += 4;
		// 4 BCD ��Ч����
		Date2BCD((*itor).oldStartDate, lpMsg);
		lpMsg += 4;
		// 4 BIN �°汾��
		memcpy(lpMsg, &(*itor).newVersion, 4);
		lpMsg += 4;			
		// 4 BCD ��Ч����
		Date2BCD((*itor).newStartDate, lpMsg);
		lpMsg += 4;
	}
	lDataLen += 23*wCount;
	return lDataLen;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BOM��Ҫ״̬���
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeImportantStatusChange(OperComHdr_t& operComHdr, LPBYTE lpByte, SERVICE_STATUS status)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 3 1BIN+1BIN+1BIN ״̬ID 
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x01;
	lpMsg++;
	// 1 BIN ״ֵ̬ 
	*lpMsg = status;
	lpMsg++;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ISAMǩ��ǩ�ˣ�Ԥ����
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeIsamCheckIn(OperComHdr_t operComHdr, LPBYTE lpByte, ISAM_AUTH isam_auth, BYTE result)
{
	LPBYTE lpMsg = lpByte;

	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// 6 BCD ISAM����
	memcpy(lpMsg,isam_auth.samid,6);
	lpMsg+=6;
	// 4 BIN ���뷽ID
	memcpy(lpMsg, &isam_auth.participantId, 4);
	lpMsg += 4;
	// 1 BIN ����״̬
	*lpMsg = isam_auth.auth_type;
	lpMsg++;
	// 1 BIN ��֤���
	*lpMsg = isam_auth.authResult;
	lpMsg++;
	// 4 BIN ��Ȩ���
	memcpy(lpMsg, &isam_auth.amount, 4);
	lpMsg += 4;
	// 7 BCD ��ȨʧЧʱ��
	byte specialTime[7] = {0xFF};
	if(memcmp(&isam_auth.datetime,&specialTime,7)==0)//ǩ��ʱ��ֵΪ7��FF�����ý���BCD�仯��
	{
		memcpy(lpMsg, &isam_auth.datetime, 7);
	}
	else
	{
		Time2BCD(isam_auth.datetime, lpMsg);
	}
	lpMsg += 7;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �豸������־
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceErrorData(OperComHdr_t& operComHdr,DEVICE_ERRORDATA& ErrorData, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 1 BIN �豸��������
	*lpMsg = ErrorData.partType;
	lpMsg++;
	// 1 1BIN+3BIN ����ID
	*lpMsg = ErrorData.bDeviceType;
	lpMsg++;
	memcpy(lpMsg, ErrorData.devicePartID, 3);
	lpMsg += 3;
	// 2 BIN �豸��Ӧ��ID
	memcpy(lpMsg, &ErrorData.deviceDeviderId, 2);
	lpMsg += 2;
	// 4 1BIN+1BIN+1BIN+1BIN ���ϴ���
	memcpy(lpMsg, ErrorData.maintanCode, 4);
	lpMsg += 4;
	// 2 1BIN+1BIN ά�޴���
	memcpy(lpMsg, ErrorData.ecode, 2);
	lpMsg += 2;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������־
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 2 1BIN+1BIN ��������
	ComRevWORD(operLog.operCode, lpMsg);
	lpMsg += 2;
	// 1 BIN ��������
	*lpMsg = operLog.operLevel;
	lpMsg++;
	// 1 BIN �������
	*lpMsg = operLog.result;
	lpMsg++;
	// 32 ASCII ��������
	memcpy(lpMsg, operLog.operDetail, sizeof (operLog.operDetail));
	lpMsg += 32;
	// 32 ASCII ��ע
	memcpy(lpMsg, operLog.operTip, sizeof (operLog.operTip));
	lpMsg += 32;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      �豸�Ĵ�������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceRegisterData(OperComHdr_t& operComHdr, DEVICEREG_AR& purchaseAccData,AR_REASON arReson, LPBYTE lpByte,DWORD& dataLen)
{
	LPBYTE lpMsg = lpByte;

	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// �����Ĵ�������״̬����
	*lpMsg = arReson;
	lpMsg++;

	// �豸�Ĵ������ݼ�¼����
	unsigned short arlen = 0;
	LPBYTE lpLengthPos = lpMsg;
	lpMsg+=2;

	// �豸�Ĵ�������
	for (DEVICEREG_AR::iterator itemIter = purchaseAccData.begin();itemIter!=purchaseAccData.end();itemIter++)
	{
		/*if(itemIter->first <= 512)
		{
			continue;
		}*/
		*lpMsg = LOBYTE((short)itemIter->first);
		lpMsg++;
		*lpMsg = HIBYTE((short)itemIter->first);
		lpMsg++;
		memcpy(lpMsg,&(itemIter->second),4);
		lpMsg+=4;
		arlen ++;
	}

	// �豸�Ĵ������ݼ�¼����
	*lpLengthPos= LOBYTE(arlen);
	lpLengthPos++;
	*lpLengthPos = HIBYTE(arlen);

	// ���ݳ���
	dataLen = lpMsg - lpByte;
	WORD wLen = dataLen;
	memcpy(lpByte + INX_BUSINESS_HEADER_LENGTH,&wLen,2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ֽ��������ĵ���

@param      OperComHdr_t& operComHdr  Э��ͷ

@param      CASHBOX_CHANGE& cash_change Ǯ���������

@param      [out] LPBYTE lpByte   ��������

*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeCashBoxExchange(OperComHdr_t& operComHdr, CASHBOX_CHANGE& cash_change,LPBYTE  lpData )
{
	try{
		MakeOperComHdr(operComHdr, lpData); //���챨��Э��ͷ
		lpData += LEN_BUSINESS_HEADER;      //Э��ͷƫ����

		memcpy(lpData, &cash_change.handleType,1);// �������� 1BIN 
		lpData ++;
		
		//memcpy(lpData, &cash_change.COIN_BANKNOTE_BOX_ID, 4);    //1BCD+1BIN+2BIIN Ǯ��ID
		//lpData += 4;
		//for(int i = 0;i < 4;++i){
		//	*lpData = cash_change.COIN_BANKNOTE_BOX_ID[i];	// BCD 4λ
		//	lpData++;
		//}
		cash_change.Serialze(lpData);
		lpData+=4;
		memcpy(lpData, &cash_change.position, 1); //1BIN  Ǯ��λ��
		lpData ++;
		
		memcpy(lpData, &cash_change.State, 1);    //1BIN  Ǯ��״̬  
		lpData++;
		
		memcpy(lpData, &cash_change.btCount, 1);  //1BIN  ��������
		lpData++;

		//1BIN+2BIN ���ִ���+Ǯ������
		vector<CASH_TYPE_COUNT> ::iterator  iter = cash_change.vecCash.begin();
		while (iter != cash_change.vecCash.end() )
		{
			memcpy(lpData, &(*iter).cash_type, 1); //1BIN ���ִ��� 
			lpData++;

			memcpy(lpData, &(*iter).cash_count, 2);//2BIN Ǯ������
			lpData += 2;

			iter++;
		}

	}
	catch(...)
	{

	}

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �˻�����
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeAccountLock(OperComHdr_t& operComHdr, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ˮ������
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceUdsnNoReset(OperComHdr_t& operComHdr, UDSN_RESET& udsnRest, UDSN_RESET_REASON& resetReason, LPBYTE lpMsg)
{
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 1 BIN ����ԭ��
	*lpMsg = resetReason;
	lpMsg++;
	// 1 BIN �豸��ˮ������
	*lpMsg = 1;
	lpMsg++;
	// 1 BIN �豸��ˮ������
	*lpMsg = udsnRest.udsn_type;
	lpMsg++;
	// 4 BIN ����ǰ�豸��ˮ��
	memcpy(lpMsg, &udsnRest.beforeResetNo, 4);
	lpMsg += 4;
	// 4 BIN ���ú��豸��ˮ��
	memcpy(lpMsg, &udsnRest.afterResetNo, 4);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������
@param      OperComHdr_t operComHdr 
LPBYTE		 lpByte
BYTE		 bType     ��������
LPBYTE		 lpNewID   �²������
LPBYTE		 lpOldID   �ɲ������
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceComponentExchange(OperComHdr_t& operComHdr, LPBYTE lpMsg, BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID)
{
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	*lpMsg = bType;
	lpMsg++;
	for(int i=0;i<6;i++)
	{
		memcpy(lpMsg+i,lpNewID+5-i,1);
	}
	lpMsg += 6;
	for(int i=0;i<6;i++)
	{
		memcpy(lpMsg+i,lpOldID+5-i,1);
	}
	//memcpy(lpMsg, lpOldID, 6);
	lpMsg += 6;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      TVM�������
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTVMAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData, LPBYTE lpdata/*, DWORD& datalen*/)
{
	LPBYTE lpMsg = lpdata;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// ��ʼʱ��
	purchaseData.auditTimeFrom.Serialize(lpMsg);
	lpMsg += 7;
	// ����ʱ��
	purchaseData.auditTimeTo.Serialize(lpMsg);
	lpMsg += 7;

	// ҵ���������
	purchaseData.settlementData.Serialize(lpMsg);
	lpMsg += 78;
	//typedef struct _tagTransKey
	//{
	//	int provider;
	//	char transType;
	//	char transSubType;
	//	void Serialize(LPBYTE lpMsg) const
	//	{
	//		memcpy(lpMsg,&provider,4);
	//		memcpy(lpMsg+4,&transType,1);
	//		memcpy(lpMsg+5,&transSubType,1);
	//	}
	//	
	//} TransKey;

	//typedef struct _tagTransKeyCompare
	//{
	//	bool operator()(const TransKey& one,const TransKey& another) const 
	//	{
	//		if(one.provider<another.provider) return true;
	//		if(one.provider>another.provider) return false;
	//		if(one.transType<another.transType) return true;
	//		if(one.transType>another.transType) return false;
	//		if(one.transSubType<another.transSubType) return true;
	//		if(one.transSubType>another.transSubType) return false;
	//		return false;
	//	}
	//} TransKeyCompare;

	//IncomeItem allIncome;
	//for ( IncomeByPayTypeItemMap::iterator it = purchaseData.incomeByPayTypeItems.begin();it!=purchaseData.incomeByPayTypeItems.end();it++)
	//{
	//	allIncome += it->second;
	//}

	//map<TransKey,vector<SCAuditKey>,TransKeyCompare> trans;	
	//for ( PurchaseItemMap::iterator it = purchaseData.purchaseItems.begin();it!=purchaseData.purchaseItems.end();it++)
	//{
	//	TransKey key;
	//	CString strKey;
	//	ProductType_t ticketType = it->first.ticketType;
	//	ProductCategory_t cardCategory;
	//	cardCategory = (ProductCategory_t)theACC_PRODUCT.GetProductCategory(ticketType);
	//	key.provider = 0x01;
	//	if(it->first.serviceID == ISSUE_SVC && cardCategory == PURSE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x03;
	//	}
	//	else if(it->first.serviceID == ISSUE_SVC && cardCategory == PERIOD)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x02;
	//	}
	//	else if(it->first.serviceID == ISSUE_SVC && cardCategory == MULTIRIDE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x01;
	//	}
	//	if(it->first.serviceID == CHARGE_SVC && cardCategory == MULTIRIDE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x12;
	//	}
	//	else if(it->first.serviceID == CHARGE_SVC && cardCategory == PERIOD)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x11;
	//	}
	//	else if(it->first.serviceID == CHARGE_SVC && cardCategory == PURSE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x10;
	//	}
	//	if(it->first.serviceID == ADJUST_SVC && cardCategory == MULTIRIDE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x120;
	//	}
	//	else if(it->first.serviceID == ADJUST_SVC && cardCategory == PERIOD)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x119;
	//	}
	//	else if(it->first.serviceID == ADJUST_SVC && cardCategory == PURSE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x118;
	//	}
	//	if(it->first.serviceID == REFUND_SVC && cardCategory == MULTIRIDE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x28;
	//	}
	//	else if(it->first.serviceID == REFUND_SVC && cardCategory == PERIOD)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x27;
	//	}
	//	else if(it->first.serviceID == REFUND_SVC && cardCategory == PURSE)
	//	{
	//		key.transType = 0x03;
	//		key.transSubType = 0x26;
	//	}		
	//	//if(it->first.serviceID == TVMFAILURE_REFUND_SVC)
	//	//{
	//	//	key.transType = 0x50;
	//	//	key.transSubType = 0x01;
	//	//}
	//	//if(it->first.serviceID == PERSONALIZE_SVC)
	//	//{
	//	//	key.transType = 0x01;
	//	//	key.transSubType = 0x04;
	//	//}
	//	
	//	if(key.transType!=0 && key.transSubType!=0)
	//	{
	//		trans[key].push_back(it->first);
	//	}

	//}

	//LPBYTE lpMsg = lpdata;
	//MakeOperComHdr(operComHdr,lpMsg);
	//lpMsg+=LEN_BUSINESS_HEADER;
	////datalen = lpMsg - lpdata;
	//// ͳ��ʱ��
	//Date2BCD(purchaseData.businessDate,lpMsg);
	//lpMsg+=4;
	//// ��������ԭ��
	//*lpMsg = auditflag;
	//lpMsg+=1;
	////������ʼʱ��
	//Time2BCD(purchaseData.auditTimeFrom,lpMsg);
	//lpMsg+=7;
	////���ݽ���ʱ��
	//Time2BCD(purchaseData.auditTimeTo,lpMsg);
	//lpMsg+=7;
	////�����ܽ��
	//memcpy(lpMsg,&(allIncome.receivableAmount),4);
	//lpMsg+=4;	
	////֧���ܽ��
	//int refundAmount = abs(allIncome.refundAmount);//??֧���������ֵ
	//memcpy(lpMsg,&refundAmount,4);
	//lpMsg+=4;
	////ʵ���ܽ��
	//memcpy(lpMsg,&(allIncome.incomeAmount),4);
	//lpMsg+=4;
	////���ճ�Ʊ������
	//int recyleTicketCount = allIncome.recyleTicketCount;
	//memcpy(lpMsg,&recyleTicketCount,4);
	//lpMsg+=4;

	////֧����ʽ����
	//int payTypeCount = purchaseData.incomeByPayTypeItems.size();
	//memcpy(lpMsg,&payTypeCount,4);
	//lpMsg+=4;
	//for( map<char,IncomeItem>::iterator iterIncome = purchaseData.incomeByPayTypeItems.begin();iterIncome!=purchaseData.incomeByPayTypeItems.end();iterIncome++)
	//{
	//	//֧����ʽ
	//	*lpMsg = iterIncome->first;
	//	lpMsg+=1;
	//	//�����ܽ��
	//	memcpy(lpMsg,&(iterIncome->second.receivableAmount),4);
	//	lpMsg+=4;	
	//	//֧���ܽ��
	//	int refundAmount = abs(iterIncome->second.refundAmount);//??֧���������ֵ
	//	memcpy(lpMsg,&refundAmount,4);
	//	lpMsg+=4;
	//	//ʵ���ܽ��
	//	memcpy(lpMsg,&(iterIncome->second.incomeAmount),4);
	//	lpMsg+=4;
	//	//���ճ�Ʊ������
	//	int recyleTicketCount = iterIncome->second.recyleTicketCount;
	//	memcpy(lpMsg,&recyleTicketCount,4);
	//	lpMsg+=4;
	//}
	////������������
	//short transCount = trans.size();
	//memcpy(lpMsg,&transCount,2);
	//lpMsg+=2;

	//for(map<TransKey,vector<SCAuditKey>,TransKeyCompare>::iterator iterTrans = trans.begin();iterTrans!=trans.end();iterTrans++)
	//{
	//	//Ʊ��������ID���������ͣ�����������
	//	const TransKey & key = iterTrans->first;
	//	key.Serialize(lpMsg);
	//	lpMsg+=6;
	//	//��Ʊ��Ʒ��������
	//	const vector<SCAuditKey>& transItems = iterTrans->second;
	//	char transItemCount = transItems.size();
	//	memcpy(lpMsg,&transItemCount,1);
	//	lpMsg+=1;
	//	for(vector<SCAuditKey>::const_iterator iterTranItem = transItems.begin();iterTranItem!=transItems.end();iterTranItem++)
	//	{
	//		//��Ʊ��Ʒ����
	//		memcpy(lpMsg,&(iterTranItem->ticketType),2);
	//		lpMsg+=2;
	//		const SCAuditValue& iterTranItemValue = purchaseData.purchaseItems[*iterTranItem];
	//		//����������
	//		memcpy(lpMsg,&(iterTranItemValue.transCount),4);
	//		lpMsg+=4;
	//		int depositAmount = iterTranItemValue.depositAmount;
	//		int transAmount = iterTranItemValue.transAmount;
	//		if(iterTranItem->serviceID == REFUND_SVC)
	//		{
	//			depositAmount = abs(depositAmount);	//�˿���˿�����ʱѺ������ֵ
	//			transAmount = abs(transAmount);
	//		}
	//		//Ѻ���ܽ��
	//		memcpy(lpMsg,&depositAmount,4);
	//		lpMsg+=4;
	//		//�����ܽ��
	//		memcpy(lpMsg,&transAmount,4);
	//		lpMsg+=4;
	//		//�����������ܽ��
	//		memcpy(lpMsg,&(iterTranItemValue.serviceFeeAmount),4);
	//		lpMsg+=4;
	//		//���ɱ���
	//		memcpy(lpMsg,&(iterTranItemValue.cardCostAmount),4);
	//		lpMsg+=4;
	//		//���տ�������
	//		short recyleCardNum = iterTranItemValue.recyleCardNum;
	//		memcpy(lpMsg,&recyleCardNum,2);
	//		lpMsg+=2;
	//	}
	//}
	//datalen = lpMsg - lpdata;
	//WORD wLen = lpMsg - lpdata;
	//memcpy(lpdata + INX_BUSINESS_HEADER_LENGTH,&wLen,2);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������
@param      OperComHdr_t&	operComHdr
LPBYTE			lpByte      
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakePackageMsgTransAudit(OperComHdr_t& operComHdr, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;

	VARIABLE_DATA binFileData;
	thePEKG_AUDIT.GetPekgAuditMessage(binFileData);

	//int iMsgNo = 0;
	//memcpy(&iMsgNo, binFileData.lpData + 11, 4);
	operComHdr.msgLen = LEN_BUSINESS_HEADER + binFileData.nLen;

	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	memcpy(lpMsg, binFileData.lpData, binFileData.nLen);
	lpMsg += binFileData.nLen;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ����TVM״̬����
@param     MessageType      ״̬�������� 
0   ��Ӫģʽ
1   24Сʱ��Ӫģʽ
2   �ӳ���Ӫģʽ״̬
3   ��Ӫģʽ + 24Сʱ��Ӫ + �ӳ���Ӫģʽ
4   TVM����״̬
@param     LPBYTE lpByte    ƴװ����
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTVMFullStatus(STATUS_TYPE statusType, LPBYTE lpData)
{
	LPBYTE lpMsg = lpData;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_ALL_STATUS_DATA);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;
		
	*lpMsg = statusType == STATUS_TYPE_THREESTATUS  ? 3 : 39;
	lpMsg+=2;
	//ģʽ״̬��24Сʱ��Ӫ״̬���ӳ���Ӫ״̬��
	if(statusType == STATUS_TYPE_THREESTATUS )
	{
		memcpy(lpMsg, &TVM_STATUS_IDS::OPERATION_MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetOperationMode();
		lpMsg++;
		memcpy(lpMsg, &TVM_STATUS_IDS::HOUR24MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.Get24HourMode();
		lpMsg++;
		memcpy(lpMsg, &TVM_STATUS_IDS::DELAY_RUN_MODE, 3);
		lpMsg += 3;
		*lpMsg = (theTVM_STATUS_MGR.GetDelayTime()==0)?0x00:0x01;
		lpMsg++;
	}
	else
	{
		//1 ����״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::SERVICE_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetServiceStatus();
		lpMsg++;
		//2 ����״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::RUN_STATUS, 3);
		lpMsg += 3;	
		*lpMsg = theTVM_STATUS_MGR.GetRunStatus();
		lpMsg++;
		//3 ����ģʽ
		memcpy(lpMsg, &TVM_STATUS_IDS::WORK_MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetServerWorkMode();
		lpMsg++;
		//4 ��Ӫģʽ
		memcpy(lpMsg, &TVM_STATUS_IDS::OPERATION_MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetOperationMode();
		lpMsg++;
		//5 24Сʱ��Ӫ
		memcpy(lpMsg, &TVM_STATUS_IDS::HOUR24MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.Get24HourMode();
		lpMsg++;
		//6 �ӳ���Ӫʱ��
		memcpy(lpMsg, &TVM_STATUS_IDS::DELAY_RUN_MODE, 3);
		lpMsg += 3;
		*lpMsg = (theTVM_STATUS_MGR.GetDelayTime()==0)?0x00:0x01;
		lpMsg++;
		//7 ����״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::CONNECT_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCommunicate();
		lpMsg++;
		//8 ʱ��״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::TIME_DELAY, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetTimeSynStatus();
		lpMsg++;
		//9 ��½״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::LOGIN_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetLoginStatus();
		lpMsg++;		

		/*------------------------Card---------------------------*/
		//10 Card��1��װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox1Status();
		lpMsg++;
		//11 Card��1����
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox1NumStatus();
		lpMsg++;
		//12 Card��2��װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX2_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox2Status();
		lpMsg++;
		//13 Card��2����
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX2_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox2NumStatus();
		lpMsg++;
		
		//14 Card ��Ʊ��״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_CYCLEBOX_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardWasteBoxStatus();
		lpMsg++;
		//15 Card ��Ʊ������״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_CYCLEBOX_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardWasteBoxNumStatus();
		lpMsg++;
		/*----------------------Coin------------------------------*/
		//16 Ӳ��������1��װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox1Status();
		lpMsg++;

		//17 ����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox1NumStatus();
		lpMsg++;
		//18 Ӳ��������2��װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX2_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox2Status();
		lpMsg++;
		//19 ����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX2_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox2NumStatus();
		lpMsg++;
		 //20 Ӳ�һ�����1��װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CYCLE_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinCollectionBox1Status();
		lpMsg++;
		//21 ����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CYCLE_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinCollectionBox1NumStatus();
		lpMsg++;
		// 22 Ӳ��Hopper1����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_HOPPER_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinHopper1NumStatus();
		lpMsg++;
		// 23 Ӳ��Hopper2����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_HOPPER_BOX2_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinHopper2NumStatus();
		lpMsg++;
		
		// 24 ֽ��������1��װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteChangeBox1Status();
		lpMsg++;
		// 25 ֽ��������1����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus();
		lpMsg++;
		// 26 ֽ�һ����䰲װ״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteCollectionBoxStatus();
		lpMsg++;
		// 27 ֽ�һ���������״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteCollectionBoxNumStatus();
		lpMsg++;
		///---------------------������--------------------------------
		// 28 ������1״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::RW1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetRW1Status();
		lpMsg++;
		// 29 ������1״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::RW2_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetRW2Status();
		lpMsg++;

		//30SAM��1����
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM1_TYPE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam1Type();// ��д��SAM1������
		lpMsg++;
		
		//31 SAM��2����
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM2_TYPE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam2Type();// ��д��SAM2������
		lpMsg++;

		//32 SAM��1״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM_1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam1Status();// SAM1��״̬
		lpMsg++;

		//33 SAM��2״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM_1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam2Status();// SAM2��״̬
		lpMsg++;

		// 34 ��ӡ��
		memcpy(lpMsg, &TVM_STATUS_IDS::PRINTER_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetPrintStatus();
		lpMsg++;

		// 35 ά����״̬
		memcpy(lpMsg,&TVM_STATUS_IDS::DOOR_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetMaintenanceDoorStatus();
		lpMsg++;
		// 36 ֽ��ģ��
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_ACCEPT_MODULE_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteModuleStatus();
		lpMsg++;
		
		// 37 ����ģ��
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_MODULE_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardModuleStatus();
		lpMsg++;
		// 38 Ӳ��ģ��
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_MODULE_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinModuleStatus();
		lpMsg++;
		
		// 39 LEDģ��
		memcpy(lpMsg,&TVM_STATUS_IDS::STATUS_DISPLAY_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetStatusDisplayStatus();
		lpMsg++;
		/*
		// Ʊ��������
		memcpy(lpMsg, &TVM_STATUS_IDS::CARD_RW_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardRWStatus();
		lpMsg++;
		// SAM��2״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::CARD_RW_SAM1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam2Status();// Ʊ����д��SAM��״̬
		lpMsg++;
		// token������
		memcpy(lpMsg, &TVM_STATUS_IDS::TOKEN_RW_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetTokenRWStatus();
		lpMsg++;
		// SAM��3״̬
		memcpy(lpMsg, &TVM_STATUS_IDS::TOKEN_RW_SAM1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam3Status();// Token������SAM��״̬
		lpMsg++;*/
		
		////41 �ֽ�ȫ��
		//memcpy(lpMsg,&TVM_STATUS_IDS::SECURITY_DOOR_STATUS,3);
		//lpMsg += 3;
		//*lpMsg = theTVM_STATUS_MGR.GetSecurityDoorStatus();
		//lpMsg++;


	
		// �𶯴�����
		/*memcpy(lpMsg,&TVM_STATUS_IDS::TILT_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetTiltStatus();
		lpMsg++;*/
		// ��������
		/*memcpy(lpMsg,&TVM_STATUS_IDS::OFFLINE_DAYS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetOffLineDays();
		lpMsg++;*/
		// ��Ȩ����
		/*memcpy(lpMsg,&TVM_STATUS_IDS::AUTHORIZED_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetAuthorizedAccessStatus();
		lpMsg++;*/
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ɵ�״̬��ȫ״̬����
@param     LPBYTE lpByte    ƴװ����
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTVMFullStatus(TVM_STATUS_ID bomStatusID, BYTE bValue,LPBYTE lpByte)
{
	LPBYTE lpMsg= lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_ALL_STATUS_DATA);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;
	//״̬����
	*lpMsg = 0x01;
	lpMsg+=2;
	// 3 1BIN+1BIN+1BIN ���״̬ID
	memcpy(lpMsg, &bomStatusID, 3);
	lpMsg += 3;
	// 1 BIN ���״ֵ̬
	*lpMsg = bValue;
	lpMsg++;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ����BOM״̬�������
@param     LPBYTE lpByte    ƴװ����
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceStatusChange(LPBYTE lpByte, TVM_STATUS_ID bomStatusID, BYTE bValue)
{
	LPBYTE lpMsg = lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_STATUS_CHANGE_DATA);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;
	// 3 1BIN+1BIN+1BIN ���״̬ID
	memcpy(lpMsg, &bomStatusID, 3);
	lpMsg += 3;
	// 1 BIN ���״ֵ̬
	*lpMsg = bValue;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ����TVM�쳣״̬����
@param     LPBYTE lpByte    ƴװ����
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode)
{
	LPBYTE lpMsg = lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_ERROR_NOTIC);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;

	// 3 BIN + BIN + BIN ״̬ID
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x0B; 
	lpMsg++;
	// 4 1BIN+1BIN+1BIN+1BIN ����ID
	// MTC Code
	ComRevLong(mtcCode, lpMsg);
	lpMsg += 4;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ǿ��ʱ��ͬ��(Ԥ��)
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeForceTimerSynchronize(LPBYTE lpDateTime,  LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	*lpMsg = LOBYTE(MSG_FORCE_TIME_SYNC);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_FORCE_TIME_SYNC);
	lpMsg++;
	memcpy(lpMsg, lpDateTime, 7);
	lpMsg += 7;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief    ��¼�ǳ�����  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeLoginOrLogoutRequest(LOGIN_TYPE LoginType, int nOperatorID, LPBYTE lpPwd, LPBYTE lpByte)
{
	LPBYTE pData = lpByte;

	//���ƴ��룺0x5101������ʱת����INTEL��, 2���ֽڡ� 
	// MSG_REQUEST_LOGIN_OR_LOGOUT = 0x5101;
	*pData = LOBYTE(MSG_REQUEST_LOGIN_OR_LOGOUT);
	pData++;
	*pData++ = HIBYTE(MSG_REQUEST_LOGIN_OR_LOGOUT);

	//�����¼�Ĳ���Ա, 3 BCD
	int2BCD(nOperatorID, (char*)pData, 3);
	pData += 3;

	//�����¼�Ĳ���Ա����, 3 BCD
	PSTR2BCD((PSTR)lpPwd,6,(char*)pData);
	pData += 3;

	// 4 2BIN+1BIN+1BIN �豸ID
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(pData);
	pData+=4;

	//����ĵ�¼���͡�0x00����¼��0x01���ǳ�
	*pData++ = static_cast<int>(LoginType);

	//Ԥ��ʹ�ã����0xFF
	*pData = 0xFF;
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakePersonalizationRequest(PersonalizationQuery& query,LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	// 2 BIN �������
	*lpMsg = LOBYTE(MSG_REQUEST_PERSONALIZATION);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_PERSONALIZATION);
	lpMsg++;
	// 61	ASCII	Name of Cardholder �ֿ�������
	memcpy(lpMsg, query.cardHolderName, 61);
	lpMsg+=61;
	// 4	BCD		Date of birth of cardholder
	Date2BCD(query.dateOfBirth, lpMsg);
	lpMsg+=4;
	// 21	ASCII	�ֿ����ռ�绰
	memcpy(lpMsg, query.cardholderPhoneDay, 21);
	lpMsg+=21;
	// 21	ASCII	�ֿ��˼�ͥ�绰
	memcpy(lpMsg, query.cardholderPhoneHome, 21);
	lpMsg+=21;
	// 16	ASCII	�ֿ�������
	memcpy(lpMsg, query.cardholderPassword, 16);
	lpMsg+=16;
	// 30	ASCII	���ڳֿ��˵�Ա��ID
	memcpy(lpMsg, query.staffID, 30);
	lpMsg+=30;
	// 19	ASCII	�ֿ����ṩ�ĸ������
	memcpy(lpMsg, query.cardholderNumber, 19);
	lpMsg+=19;
	// 1	BIN		����ֿ��˱����ʹ�õĸ����������
	*lpMsg = query.cardholderNumberType;
	lpMsg++;
	// 4	BIN		Defines the card logic serial number for the passenger.
	memcpy(lpMsg, &query.cardSerialNumber, 4);
	lpMsg+=4;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʷ��Ϣ��ѯ

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeHistoryInfoRequest(HistoryInfoQuery& query,LPBYTE lpByte,DWORD& lenth)
{
	LPBYTE lpMsg = lpByte;
	lenth =0;
	// 2 BIN �������
	*lpMsg = LOBYTE(MSG_REQUEST_HISTORYINFO);
	lpMsg++;
	lenth++;
	*lpMsg = HIBYTE(MSG_REQUEST_HISTORYINFO);
	lpMsg++;
	lenth++;
	memcpy(lpMsg,&query.cardIssuerId,4);
	lpMsg+=4;
	lenth+=4;
	memcpy(lpMsg,&query.cardSerialNumber,4);
	lpMsg+=4;
	lenth+=4;
	memcpy(lpMsg,&query.StartDate,7);
	lpMsg+=7;
	lenth+=7;
	memcpy(lpMsg,&query.EndDate,7);
	lpMsg+=7;
	lenth+=7;
	memcpy(lpMsg,&query.ProdutCount,4);
	lpMsg+=4;
	lenth+=4;
	for(int i=0;i<query.ProdutCount;i++){
		memcpy(lpMsg,&query.ProductInfo[i].ProductType,2);
		lpMsg+=2;
		lenth+=2;
		memcpy(lpMsg,&query.ProductInfo[i].ProductVariantType,2);
		lpMsg+=2;
		lenth+=2;
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʱ��Ч�汾����
@param      LPBYTE lpByte    
WORD wVersionID  �汾����
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeImmediatelyParameterDownloadRequst(LPBYTE lpData, WORD wVersionID)
{
	LPBYTE lpMsg = lpData;
	*lpMsg = LOBYTE(MSG_REQUEST_SEND_IMIDIATLY_PARAMETER);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_SEND_IMIDIATLY_PARAMETER);;
	lpMsg++;
	*lpMsg = 0x10;
	lpMsg++;
	*lpMsg = 0x90;
	lpMsg++;
	memcpy(lpMsg, &wVersionID, 4);
	lpMsg += 4;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������֤�������
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeConnectAuthRequest(LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	*lpMsg = LOBYTE(MSG_REQUEST_CONNECT_AUTH);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_CONNECT_AUTH);
	lpMsg++;
	// 4 1HEX+2BCD+1HEX �豸ID
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
	lpMsg+=4;
	// 4 BIN IP��ַ
	CString cIP = theMAINTENANCE_INFO.GetIP();
	int first, second, third;
	first = cIP.Find('.',0);
	*lpMsg = _ttoi(cIP.Mid(0, first));
	lpMsg++;
	second = cIP.Find('.', first+1);
	*lpMsg = _ttoi(cIP.Mid(first+1, second-first-1));
	lpMsg++;
	third = cIP.Find('.', second+1);
	*lpMsg = _ttoi(cIP.Mid(second+1,third-second-1));
	lpMsg++;
	*lpMsg = _ttoi(cIP.Mid(third+1, cIP.GetLength()-third-1));
	lpMsg++;
	// Ԥ��
	memset(lpMsg, 0xFF, 3);
	lpMsg += 3;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     TVM/AVM ��Ԯ��ť��������
@param     LPBYTE lpByte    ƴװ����
@param     bool bIsCallOrCancel false:������Ԯ 
								true :ȡ����Ԯ
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeCallHelpRequest(LPBYTE lpByte,bool bIsCallOrCancel){
	LPBYTE lpMsg = lpByte;
	// �������
	*lpMsg = LOBYTE(MSG_REQUEST_TVM_HELP);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_TVM_HELP);
	lpMsg++;
	
	// ����ȷ������
	//bIsCallOrCancel ? *lpMsg = 0x01: *lpMsg = 0x00;
	memset(lpMsg,0x00,2);
	lpMsg += 2;
	
	// ��Ԯ��ʶ��ʱ��
	_DATE_TIME curTime = ComGetCurTime();
	curTime.Serialize(lpMsg);
	lpMsg += 7;

	// Ԥ��
	//BYTE btReserve[5];
	memset(lpMsg,0xFF,5);
	lpMsg += 5;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ����������汾��Ϣ
@param     LPBYTE lpByte    ƴװ����
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeParamAndSoftwareVersionInfo(LPBYTE lpByte,int &length)
{
	LPBYTE lpMsg;
	lpMsg = lpByte;
	// ��������	2	BIN
	*lpMsg = LOBYTE((short)MSG_PARAMETER_ID_DATA);
	lpMsg++;
	*lpMsg = HIBYTE((short)MSG_PARAMETER_ID_DATA);
	lpMsg++;
	// ��վID
	WORD stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
	char chStationCode[2] = {0};
	int2BCD(stationCode,&chStationCode[0],2);
	*lpMsg = chStationCode[0];//HIBYTE(stationCode);
	lpMsg++;
	*lpMsg = chStationCode[1];//LOBYTE(stationCode);
	lpMsg++;
	// �豸ID
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
	lpMsg+=4;

	// �����ļ�����	2 BIN
	WORD wProviderID = 0x001D;
	unsigned short usVersionNum = 0;
	LPBYTE lpVersionNum = lpMsg;
	lpMsg += 2;

	// ����
	std::map<PARAMETER_ID,CParameterMaster*> mapVersionProvider = thePARAM_HELPER.GetVersionProvider();

	typedef std::map<PARAMETER_ID,CParameterMaster*>::iterator iterator;
	for(iterator iter = mapVersionProvider.begin();iter!=mapVersionProvider.end();iter++)
	{
		WORD parameID = iter->first;
		// ----------------��Ч�汾--------------------
		// ����ID
		*lpMsg = HIBYTE(parameID);
		lpMsg++;
		*lpMsg= LOBYTE(parameID);
		lpMsg++;
		// ��Ӧ��ID:0x0011������
		memcpy(lpMsg, &wProviderID, 2);
		lpMsg += 2;
		// ������κ�
		memset(lpMsg, 0, 2);
		lpMsg += 2;
		// �汾����
		*lpMsg=iter->second->GetCurVerType();
		lpMsg++;
		// �汾��
		int iVersion = iter->second->GetCurMasterVer();
		memcpy(lpMsg, &iVersion, 4);
		lpMsg += 4;
		// ��Ч����
		Date2BCD(iter->second->GetCurStartDate(), lpMsg);
		lpMsg += 4;
		usVersionNum++;// �ļ�����+1

		// --------------  ���ð汾--------------------
		// ��������ͬ��
		*lpMsg = HIBYTE(parameID);
		lpMsg++;
		*lpMsg= LOBYTE(parameID);
		lpMsg++;
		memcpy(lpMsg, &wProviderID, 2);
		lpMsg += 2;
		memset(lpMsg, 0, 2);
		lpMsg += 2;
		*lpMsg=iter->second->GetFutVerType();
		lpMsg++;
		iVersion = iter->second->GetFutMasterVer();
		memcpy(lpMsg, &iVersion, 4);
		lpMsg += 4;
		Date2BCD(iter->second->GetFutStartDate(), lpMsg);
		lpMsg += 4;
		usVersionNum++;
	}
	// ����
	PARAMETER_ID programIDs[]={AFC_TVMPROGRAM_ID,	// TVM ����
								AFC_TPUPROGRAM_ID,	// TPU����
								//AFC_TVMGUI_ID,		// GUI�ļ�����ʵΪ0x9700��
								//ACC_STATION_MAP_ID, // ��·վ�����������������
								AFC_TVMTHPROG_ID,	// THӦ�ó���
	};
	CVersionInfo::_PROG_TYPE programsTypes[]={
		CVersionInfo::MC,				// TVM����
		CVersionInfo::CARD_RW_APP,		// TPU����
		//CVersionInfo::CHARGE_RW_MAIN,	// TPU������
		//CVersionInfo::VOICE,			// �����ļ�
		//CVersionInfo::GUI,				// GUI�ļ�
		//CVersionInfo::STATION_MAP,		// ��·վ�����������������
		//CVersionInfo::TEMPLATE			// ģ���ļ�
		CVersionInfo::TH_APP,			// THӦ�ó���
	};
	//bool isChargeRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CHARGE_RW_APP) == 0;
	//bool isCardRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CARD_RW_APP) == 0;
	//bool isTokenRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_RW_APP) == 0;
	/*if(!isChargeRWAPPFailed && !isCardRWAPPFailed && !isTokenRWAPPFailed)
	{
		if(theSETTING.GetCardRWUpdateFlag() == 1)//�ڲ�����ʧ�����ڲ���
		{
			programsTypes[1] = CVersionInfo::TOKEN_RW_APP;
			programsTypes[2] = CVersionInfo::TOKEN_RW_MAIN;
		}
	}
	else if(!isCardRWAPPFailed)
	{
		programsTypes[1] = CVersionInfo::CARD_RW_APP;
		programsTypes[2] = CVersionInfo::CARD_RW_MAIN;
	}
	else if(!isTokenRWAPPFailed)
	{
		programsTypes[1] = CVersionInfo::CHARGE_RW_APP;
		programsTypes[2] = CVersionInfo::CHARGE_RW_MAIN;
	}*/

	CVersionInfo::_VER_TYPE versionTypes[] = {CVersionInfo::CURRENT,CVersionInfo::FUTURE};
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<2;j++)
		{
			// ID
			*lpMsg = HIBYTE(programIDs[i]);
			lpMsg++;
			*lpMsg = LOBYTE(programIDs[i]);
			lpMsg++;
			WORD bomProviderID = theVERSION_INI.GetCompanyID(versionTypes[j],programsTypes[i]);
			memcpy(lpMsg, &bomProviderID, 2);
			lpMsg += 2;
			WORD bomBatchNo = theVERSION_INI.GetBatchNo(versionTypes[j],programsTypes[i]);
			memcpy(lpMsg, &bomBatchNo, 2);
			lpMsg += 2;
			*lpMsg = theVERSION_INI.GetVersionType(versionTypes[j],programsTypes[i]);
			lpMsg++;
			int iVersion = theVERSION_INI.GetVersionNo(versionTypes[j],programsTypes[i]);
			memcpy(lpMsg, &iVersion, 4);
			lpMsg += 4;
			Date2BCD(theVERSION_INI.GetStartDate(versionTypes[j],programsTypes[i]), lpMsg);
			lpMsg += 4;
			usVersionNum++;
		}
	}
	memcpy(lpVersionNum, &usVersionNum, 2);
	length = lpMsg - lpByte;
}


/*-------------------------------------���±���-------------------------------------------*/
//////////////////////////////////////////////////////////////////////////
/**
@brief      �������֪ͨ
@param      LPBYTE lpByte  
@param      NOTICE_TYPE type ֪ͨ���� 
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOpEndNotice(LPBYTE lpByte,NOTICE_TYPE type)
{
	*lpByte = LOBYTE(MSG_OP_END_NOTIC);
	lpByte++;
	*lpByte = HIBYTE(MSG_OP_END_NOTIC);
	lpByte++;
	*lpByte = LOBYTE(type);
	lpByte++;
	*lpByte = HIBYTE(type);
	lpByte++;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      AR

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceAR(DEVICE_AR_REASON arReason,VARIABLE_DATA& msg)
{
	VARIABLE_DATA content;
	theDeviceAR.ReadProcessStat(arReason,content);
	msg.nLen = content.nLen + LEN_AFC_SYS_HEADER + 4;
	msg.lpData = new BYTE[msg.nLen];
	LPBYTE lpMsg = msg.lpData;
	AfcSysHdr_t sysHeader = AquireAfcSysHeader(msg.nLen);
	MakeAfcSysHdr(sysHeader,lpMsg);
	lpMsg+=LEN_AFC_SYS_HEADER;
	//DWORD type = MSG_TRANS_BOM_AR;
	//memcpy(lpMsg,&type,4);
	//lpMsg+=4;
	*lpMsg = 0x00;
	lpMsg+=1;
	*lpMsg=0x00;
	lpMsg+=1;
	*lpMsg = HIBYTE(MSG_TRANS_AUDIT_REGISTER);
	lpMsg+=1;
	*lpMsg = LOBYTE(MSG_TRANS_AUDIT_REGISTER);
	lpMsg+=1;
	memcpy(lpMsg,content.lpData,content.nLen);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ��Ʊ���ս��ף�Ԥ����
@param     BOM_REFUND_TVM   BOM��TVM�����˿���Ϣ�ṹ��
@param     LPBYTE lpByte    ƴװ����
@retval    ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeRecycleInvalidTicket(AfcSysHdr_t & afcHeader,INVALIDE_TICKET_RECYCLETRADE& recycleTrade, LPBYTE lpMsg)
{
	// AFCϵͳ����ͷ
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;
	MakeTransComHdr(recycleTrade.transComHdr,lpMsg);
	lpMsg += LEN_TXN_DATATYPE;
	// 4 BIN �����к�
	memcpy(lpMsg, &recycleTrade.cardSerialNo, 8);
	lpMsg += 8;
	// 1 BIN ����������
	*lpMsg = recycleTrade.cardPhysicalType;
	lpMsg++;
	// 4 BIN ��������
	memcpy(lpMsg, &recycleTrade.issueId, 4);
	lpMsg += 4;
	// 2 BIN ��Ʊ��Ʒ����
	memcpy(lpMsg, &recycleTrade.productType, 2);
	lpMsg += 2;
	// 1 �Ͽ�ԭ��
	*lpMsg = recycleTrade.reson;
	lpMsg++;
	// ��Ʊ��ID
	recycleTrade.magazineId.Serialze(lpMsg);
	lpMsg+=4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Ʊ�佻��
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeMagazineExchange(OperComHdr_t& operComHdr, MAGAZINE_CHANGE& magazine_change,LPBYTE lpByte)
{
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
		MakeOperComHdr(operComHdr, lpMsg);
		lpMsg += LEN_BUSINESS_HEADER;
		// 1 BIN ��������
		*lpMsg = magazine_change.operType;
		lpMsg++;
		// 4 1BCD+1BIN+2BIN Ʊ��ID
		magazine_change.magazineId.Serialze(lpMsg);
		lpMsg+=4;
		// 1 BIN Ʊ��λ��ID
		*lpMsg = magazine_change.magazne;
		lpMsg++;
		// 1 BIN Ʊ����������
		*lpMsg = magazine_change.cardType;
		lpMsg++;
		// 4 BIN Ʊ��������ID
		memcpy(lpMsg, &magazine_change.providerId, 4);
		lpMsg += 4;
		// 2 BIN ��Ʊ��Ʒ����
		*lpMsg++ = HIBYTE(magazine_change.cardProductType);
		*lpMsg++ = LOBYTE(magazine_change.cardProductType);
		//memcpy(lpMsg, &magazine_change.cardProductType, 2);
		//lpMsg += 2;
		// 1 BIN Ԥ��ֵ����
		*lpMsg = magazine_change.preProperty;
		lpMsg++;
		// 1 BIN ������ƷID
		*lpMsg = magazine_change.childproductId;
		lpMsg++;
		// 2 BIN Ʊ������
		memcpy(lpMsg, &magazine_change.CardCount, 2);
		lpMsg += 2;
		// 1 BIN Ʊ��״̬
		*lpMsg = magazine_change.magazineStatus;
		lpMsg++;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ƴװ�豸������������

@param      (i)OperComHdr_t& operComHdr  ҵ������ר��ͷ�ṹ
@param      (o)LPBYTE lpByte             ������������ָ��

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceComponentInfo(OperComHdr_t& operComHdr, LPBYTE lpByte)
{
	try{
		// ȡ�ø�����ID
		CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
		theDeviceInfo.GetDeviceInfo(deviceInfo);
		LPBYTE lpMsg = NULL;
		lpMsg = lpByte;
		// ƴװҵ������ͷ
		MakeOperComHdr(operComHdr, lpMsg);
		lpMsg += LEN_BUSINESS_HEADER;
		// �豸����������
		*lpMsg = COUNT_DEVICE_COMPONET;
		lpMsg+=2;

		// ��Ʊģ��
		//*lpMsg = DEVICE_TICKET_MODULE;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.TH_DEVICE_ID, 6);
		//lpMsg += 6;

		// �ⲿ��д��
		//*lpMsg = DEVICE_OUT_TPU;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.RW_DEVICE_ID, 6);
		//lpMsg += 6;

		//// �ڲ���д��
		//*lpMsg = DEVICE_INNER_TPU;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.TW_DEVICE_ID, 6);
		//lpMsg += 6;

		// ��ӡ��
		*lpMsg = 0x01;//DEVICE_PRINTER;zhengxianle
		lpMsg++;
		memcpy(lpMsg, deviceInfo.PRT_DEVICE_ID, 6);
		lpMsg += 6;

		// �ⲿ��д��һƱͨISAM
		//*lpMsg = ACC_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ACC_RW_ISAM, 6);
		//lpMsg += 6;

		// �ⲿ��д��һƱͨPSAM	
		*lpMsg = ACC_PSAM_RW;
		lpMsg++;
		memcpy(lpMsg, deviceInfo.ACC_RW1_PSAM, 6);
		lpMsg += 6;

		// �ⲿ��д��һ��ͨISAM	
		//*lpMsg = ECT_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_RW_ISAM, 6);
		//lpMsg += 6;

		// �ⲿ��д��һ��ͨPSAM	
		//*lpMsg = ECT_PSAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_RW_PSAM, 6);
		//lpMsg += 6;

		// �ڲ���д��һƱͨISAM
		//*lpMsg = ACC_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ACC_TH_ISAM, 6);
		//lpMsg += 6;

		// �ڲ���д��һƱͨPSAM	
		//*lpMsg = ACC_PSAM_TW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ACC_TH_PSAM, 6);
		//lpMsg += 6;

		// �ڲ���д��һ��ͨISAM	
		//*lpMsg = ECT_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_TH_ISAM, 6);
		//lpMsg += 6;

		//// �ڲ���д��һ��ͨPSAM	
		//*lpMsg = ECT_PSAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_TH_PSAM, 6);
		//lpMsg += 6;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Ʊ����ͳ��
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen)
{
	// ����������λ����λ������Ҫ���棬ж��״̬IDΪFFFFFFFF,����Ϊ0
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// Card
	CTCardCountInfo::CARD_BOX_INFO cardBoxA		  = theCARD_COUNT.GetCardboxAInfo();
	CTCardCountInfo::CARD_BOX_INFO cardBoxB		  = theCARD_COUNT.GetCardboxBInfo();
	CTCardCountInfo::CARD_BOX_INFO cardWasted	  = theCARD_COUNT.GetCardboxWastedInfo();

	// �豸��Ʊ����
	WORD totalTicketCount = cardBoxA.ulCurCount + cardBoxB.ulCurCount + cardWasted.ulCurCount;
	memcpy(lpMsg,&totalTicketCount,2);
	lpMsg += 2;

	// Ʊ������
	*lpMsg = 0x03;
	lpMsg += 1;

	WORD count = 0;
	// Ʊ��A
	cardBoxA.ticketBoxID.Serialze(lpMsg,true);
	lpMsg += 4;

	*lpMsg = MAGAZINEB;
	lpMsg += 1;

	count = cardBoxA.ulCurCount;
	memcpy(lpMsg,&count,2);
	lpMsg += 2;

	// Ʊ��B
	cardBoxB.ticketBoxID.Serialze(lpMsg,true);
	lpMsg += 4;

	*lpMsg = MAGAZINEA;
	lpMsg += 1;

	count = cardBoxB.ulCurCount;
	memcpy(lpMsg,&count,2);
	lpMsg += 2;

	// ��Ʊ��
	cardWasted.ticketBoxID.Serialze(lpMsg,true);
	lpMsg += 4;

	*lpMsg++ = MAGAZINEC;

	count = cardWasted.ulCurCount;
	memcpy(lpMsg,&count,2);
	lpMsg += 2;

	// ���ݳ���
	dataLen = lpMsg - lpByte;
	//// ����������λ����λ������Ҫ���棬ж��״̬IDΪFFFFFFFF,����Ϊ0
	//LPBYTE lpMsg = lpByte;

	//CTTokenCountInfo::TOKEN_BOX_INFO token_boxA_info = theTOKEN_COUNT.GetTokenboxAInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO token_boxB_info = theTOKEN_COUNT.GetTokenboxBInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO token_hopperA_info = theTOKEN_COUNT.GetHopperAInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO token_hopperB_info = theTOKEN_COUNT.GetHopperBInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO invalidTokenBoxInfo = theTOKEN_COUNT.GetTokenboxWastedInfo();
	//CTTokenCountInfo::TOKEN_BOX_INFO collectTokenBoxInfo = theTOKEN_COUNT.GetTokenboxCollectedInfo();
	//bool bBoxAExist = false;
	//bool bBoxBExist = false;
	//bool bHopperAExist = false;
	//bool bHopperBExist = false;
	//bool bInvalidBoxExist = false;
	//bool bCollectedBoxExist = false;

	//BYTE boxCount = 0;
	//WORD totalTicketCount = 0;
	//if(token_boxA_info.tokenBoxID.ToString() != INVALID_ID){
	//	bBoxAExist = true;
	//	boxCount++;
	//	totalTicketCount += token_boxA_info.ulCurCount;
	//}
	//if(token_boxB_info.tokenBoxID.ToString() != INVALID_ID){
	//	bBoxBExist = true;
	//	boxCount++;
	//	totalTicketCount += token_boxB_info.ulCurCount;
	//}
	//if(token_hopperA_info.tokenBoxID.ToString() != INVALID_ID){
	//	bHopperAExist = true;
	//	boxCount++;
	//	totalTicketCount += token_hopperA_info.ulCurCount;
	//}
	//if(token_hopperB_info.tokenBoxID.ToString() != INVALID_ID){
	//	bHopperBExist = true;
	//	boxCount++;
	//	totalTicketCount += token_hopperB_info.ulCurCount;
	//}
	//if(invalidTokenBoxInfo.tokenBoxID.ToString() != INVALID_ID){
	//	bInvalidBoxExist = true;
	//	boxCount++;
	//	totalTicketCount += invalidTokenBoxInfo.ulCurCount;
	//}
	//if(collectTokenBoxInfo.tokenBoxID.ToString() != INVALID_ID){
	//	bCollectedBoxExist = true;
	//	boxCount++;
	//	totalTicketCount += collectTokenBoxInfo.ulCurCount;
	//}

	//// �豸��Ʊ����
	//memcpy(lpMsg,&totalTicketCount,2);
	//lpMsg += 2;

	//// Ʊ������
	//*lpMsg = boxCount;
	//lpMsg += 1;

	//WORD count = 0;
	//if(bCollectedBoxExist){
	//	// ������
	//	collectTokenBoxInfo.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEF;
	//	lpMsg += 1;
	//	count = collectTokenBoxInfo.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//if(bInvalidBoxExist){
	//	// ��Ʊ��
	//	invalidTokenBoxInfo.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEA;
	//	lpMsg += 1;
	//	count = invalidTokenBoxInfo.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//if(bBoxAExist){
	//	// Ʊ��A
	//	token_boxA_info.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEC;
	//	lpMsg += 1;
	//	count = token_boxA_info.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//if(bBoxBExist){
	//	// Ʊ��B
	//	token_boxB_info.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEB;
	//	lpMsg += 1;
	//	count = token_boxB_info.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//// hopper A
	//if(bHopperAExist){
	//	token_hopperA_info.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEE;
	//	lpMsg += 1;
	//	count = token_hopperA_info.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//// hopper B
	//if(bHopperBExist){
	//	token_hopperB_info.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINED;
	//	lpMsg += 1;
	//	count = token_hopperB_info.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//// ���ݳ���
	//dataLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����޸�����(Ԥ��)
@param      LPBYTE lpByte        
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReformPasswordRequest(CHANGE_STAFFPASSWORD& staffPasswd, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	// 2 BIN �������
	*lpMsg = LOBYTE(MSG_REQUEST_REFORM_PASSWORD);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_REFORM_PASSWORD);
	lpMsg++;
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
	lpMsg+=4;
	// 3 BCD ����ԱID
	int2BCD(_ttoi(theAPP_SESSION.GetUserInfo().sUserID), (char*)lpMsg, 3);
	lpMsg += 3;
	// 3 BCD �޸Ĳ���ԱID
	int2BCD(staffPasswd.StaffID, (char*)lpMsg, 3);
	lpMsg += 3;
	// 3 BCD ������
	PSTR2BCD((PSTR)staffPasswd.ciperOldPwd,6,(char*)lpMsg);
	lpMsg += 3;
	// 3 BCD ������
	PSTR2BCD((PSTR)staffPasswd.ciperNewPwd,6,(char*)lpMsg);
	lpMsg += 3;
	// 1 BIN Ԥ��
	*lpMsg = 0xFF;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ǯ���汨��

@param      OperComHdr_t& operComHdr (�¼�����ͷ��)
            LPBYTE lpByte			�������壩

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReportMoneyBoxCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dwMsgLen){
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	//----------------------------------ֻ������λ������״̬--------------------------------

	CTCoinCountInfo::COIN_CHANGE_BOX_INFO addBox1 = theCOIN_COUNT.GetChangeCoinboxAInfo();
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO addBox2 = theCOIN_COUNT.GetChangeCoinboxBInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collBox1 = theCOIN_COUNT.GetCollectionCoinboxAInfo();
	CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collBox2 = theCOIN_COUNT.GetCollectionCoinboxBInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopper1  = theCOIN_COUNT.GetCoinHopperboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopper2  = theCOIN_COUNT.GetCoinHopperboxBInfo();
	bool bCoinBoxAExist = false;
	bool bCoinBoxBExist = false;
	bool bCoinHopperAExist = false;
	bool bCoinHopperBExist = false;
	bool bCoinMoenyBoxExist = false;

	BYTE boxCount = 0;
	DWORD totalMoneyAmount = 0;	
	if(hopper1.coinBoxID.ToString() != INVALID_ID){
		bCoinHopperAExist = true;
		boxCount++;
		totalMoneyAmount += hopper1.ulCurCount * 100;;
	}
	if(hopper2.coinBoxID.ToString() != INVALID_ID){
		bCoinHopperBExist = true;
		boxCount++;
		totalMoneyAmount += hopper2.ulCurCount * 100;;
	}
	if(collBox1.coinBoxID.ToString() != INVALID_ID){
		bCoinMoenyBoxExist = true;
		boxCount++;
		totalMoneyAmount += collBox1.GetCurCount() * 100;
	}


	CTBanknoteCountInfo::BANKNOTE_BOX_INFO bankBox = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();
	bool bMoneyBoxExist = false;
	bool bChangeBoxExist = false;
	if(bankBox.banknoteBoxID.ToString() != INVALID_ID){
		bMoneyBoxExist = true;
		boxCount++;
		totalMoneyAmount += bankBox.BankNotesCountAvos();
	}
	if(changeBox.banknoteBoxID.ToString() != INVALID_ID){
		bChangeBoxExist = true;
		boxCount++;
		totalMoneyAmount += changeBox.BankNotesCountAvos();
	}	
	if(1){
		boxCount++;
		totalMoneyAmount += recycleBoxA.BankNotesCountAvos() + recycleBoxB.BankNotesCountAvos() + recycleBoxC.BankNotesCountAvos() + recycleBoxD.BankNotesCountAvos();
	}

	// �豸����ܽ��
	memcpy(lpMsg,&totalMoneyAmount,4);
	lpMsg += 4;
	// Ǯ�����
	*lpMsg = boxCount;
	lpMsg += 1;

	//-----------------Ǯ��һ��ѭ��---------------------------------------------------------
	WORD wdMoney = 0;
	// *************************************ֽ����******************************************  Ǯ����� 1
	if(bMoneyBoxExist){
		// Ǯ��ID
		bankBox.banknoteBoxID.Serialze(lpMsg);
		lpMsg  += 4;
		// Ǯ��λ��
		*lpMsg = MAGAZINEA;
		lpMsg += 1;

		// ��������
		*lpMsg = 0x06;
		lpMsg += 1;
		//---------------  ���ֶ���ѭ��------------------------ ������
		// ���ִ��뼰����
		*lpMsg = Banknote1Yuan;
		lpMsg += 1;
		wdMoney = bankBox.ulCount_CNY1;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;

		*lpMsg = Banknote5Yuan;
		lpMsg += 1;
		wdMoney = bankBox.ulCount_CNY5;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;

		*lpMsg = Banknote10Yuan;
		lpMsg += 1;
		wdMoney = bankBox.ulCount_CNY10;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;

		*lpMsg = Banknote20Yuan;
		lpMsg += 1;
		wdMoney = bankBox.ulCount_CNY20;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;

		*lpMsg = Banknote50Yuan;
		lpMsg += 1;
		wdMoney = bankBox.ulCount_CNY50;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;

		*lpMsg = Banknote100Yuan;
		lpMsg += 1;
		wdMoney = bankBox.ulCount_CNY100;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}

	// *************************************ֽ����������************************************** Ǯ����� 5
	// �ĸ�ѭ���䵱��һ�����ӣ�ID������· + �������� + վ����� + ��������
	// Ǯ��λ��
	{
		// Ǯ��ID
		recycleBoxA.banknoteBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// Ǯ��λ��
		*lpMsg = MAGAZINEI;
		lpMsg += 1;
		// ��������
		*lpMsg = 0x02;
		lpMsg += 1;
		//---------------  ���ֶ���ѭ��------------------------ ������
		// ���ִ��뼰����
		*lpMsg = Banknote10Yuan;
		lpMsg += 1;
		wdMoney = recycleBoxA.ulCurCount + recycleBoxD.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;

		*lpMsg = Banknote5Yuan;
		lpMsg += 1;
		wdMoney = recycleBoxB.ulCurCount + recycleBoxC.ulCurCount;;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}

	// ---------------ֽ��������-------------
	if(changeBox.banknoteBoxID.ToString() != INVALID_ID){
		// Ǯ��ID
		changeBox.banknoteBoxID.Serialze(lpMsg);
		lpMsg += 4;

		// Ǯ��λ��
		*lpMsg = MAGAZINEH;
		lpMsg += 1;

		// ��������
		*lpMsg = 0x01;
		lpMsg += 1;

		*lpMsg = TranBanknoteToType(theTVM_SETTING.GetBNRChangeBoxAmount());
		lpMsg += 1;

		wdMoney = changeBox.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}
	// ---------------ֽ���ҷϱ���-------------
	//if(bankInvalid.banknoteBoxID.ToString() != INVALID_ID){
	//	// Ǯ��ID
	//	bankInvalid.banknoteBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// Ǯ��λ��
	//	*lpMsg = BANKNOTE_INVALID_POS;
	//	lpMsg += 1;

	//	// ��������
	//	*lpMsg = 0x01;
	//	lpMsg += 1;

	//	*lpMsg = TranBanknoteToType(theTVM_INFO.GetBHChangeBoxBType());
	//	lpMsg += 1;

	//	wdMoney = bankInvalid.ulCurCount;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;
	//}

	////*************************************Ӳ����****************************************** Ǯ����� 4��
	//// ---------------������1-------------
	//if(addBox1.coinBoxID.ToString() != INVALID_ID){
	//	// Ǯ��ID
	//	addBox1.coinBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// Ǯ��λ��
	//	*lpMsg = COIN_CHANGE_BOX_A_POS;
	//	lpMsg += 1;

	//	// ��������
	//	*lpMsg = 0x01;
	//	lpMsg += 1;

	//	// ���ִ���
	//	*lpMsg = TranCoinType(theTVM_INFO.GetCHChangeBoxAType());
	//	lpMsg += 1;

	//	// ��������
	//	wdMoney = addBox1.ulCount;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;
	//}
	//

	//// ---------------������2-------------
	//if(addBox2.coinBoxID.ToString() != INVALID_ID){
	//	// Ǯ��ID
	//	addBox2.coinBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// Ǯ��λ��
	//	*lpMsg = COIN_CHANGE_BOX_B_POS;
	//	lpMsg += 1;

	//	// ��������
	//	*lpMsg = 0x01;
	//	lpMsg += 1;

	//	// ���ִ���
	//	*lpMsg = TranCoinType(theTVM_INFO.GetCHChangeBoxBType());
	//	lpMsg += 1;

	//	// ��������
	//	wdMoney = addBox2.ulCount;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;
	//}
	

	// ---------------������1-------------
	if(collBox1.coinBoxID.ToString() != INVALID_ID){
		// Ǯ��ID
		collBox1.coinBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// Ǯ��λ��
		*lpMsg = MAGAZINEG;
		lpMsg += 1;
		// ��������
		*lpMsg = 0x01;
		lpMsg += 1;
		// Ӳ������1
		*lpMsg = Coin1Yuan;
		lpMsg += 1;
		// Ӳ������1
		wdMoney = collBox1.ulCount_1yuan;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
		/*
		// Ӳ������2
		*lpMsg = CoinHalfYuan;
		lpMsg += 1;

		// Ӳ������2
		wdMoney = collBox1.ulCount_5jiao;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;*/
	}
	

	//// ---------------������2-------------
	//if(collBox2.coinBoxID.ToString() != INVALID_ID){
	//	// Ǯ��ID
	//	collBox2.coinBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// Ǯ��λ��
	//	*lpMsg = COIN_COLL_BOX_POS;
	//	lpMsg += 1;

	//	// ��������
	//	*lpMsg = 0x01;//0x02;
	//	lpMsg += 1;

	//	// Ӳ������1
	//	*lpMsg = Coin1Yuan;
	//	lpMsg += 1;

	//	// Ӳ������1
	//	wdMoney = collBox2.ulCount_1yuan;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;

	//	/*
	//	// Ӳ������2
	//	*lpMsg = CoinHalfYuan;
	//	lpMsg += 1;

	//	// Ӳ������2
	//	wdMoney = collBox2.ulCount_5jiao;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;*/
	//}
	
	// ---------------����Hopper1---------
	if(hopper1.coinBoxID.ToString() != INVALID_ID){
		// Ǯ��ID
		hopper1.coinBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// Ǯ��λ��
		*lpMsg = MAGAZINEE;
		lpMsg += 1;
		// ��������
		*lpMsg = 0x01;
		lpMsg += 1;

		// ���ִ���
		*lpMsg = TranCoinType(theTVM_INFO.GetCHHopper1Type());
		lpMsg += 1;
		// ��������
		wdMoney = hopper1.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}

		// ---------------����Hopper2---------
	if(hopper2.coinBoxID.ToString() != INVALID_ID){
		// Ǯ��ID
		hopper2.coinBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// Ǯ��λ��
		*lpMsg = MAGAZINED;
		lpMsg += 1;
		// ��������
		*lpMsg = 0x01;
		lpMsg += 1;
		// ���ִ���
		*lpMsg = TranCoinType(theTVM_INFO.GetCHHopper2Type());
		lpMsg += 1;
		// ��������
		wdMoney = hopper2.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}


	// ��������������
	*lpMsg = 0;
	lpMsg += 1;

	// ����
	dwMsgLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ����ֵ���� BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DWORD CSCMessageMakeHelper::TranCoinTypeToValue(int nType){
	switch (nType) {
	case 1: return 50;
	case 2: return 100;
	//case 3: return 200;
	//case 4: return 500;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�������Ϊ��Ч������countInfo��¼��������"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ����ֵ���� BIN	0����Ч��1��0.5MOP��2��1MOP��3��2MOP��4��5MOP

@param      ��

@retval     ֽ�ҽ�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CSCMessageMakeHelper::TranCoinType(int nType){
	switch (nType) {
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	//case 3: return Coin2MOP;
	//case 4: return Coin5MOP;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�������Ϊ��Ч������countInfo��¼��������"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����ִ���תΪʵ�ʵĽ���λ���֣�

@param      BankNoteAndCoinType_t type Ҫת��������

@retval     DWORD ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DWORD CSCMessageMakeHelper::TranTypeToValue(BankNoteAndCoinType_t type){
	switch(type){
	case CoinHalfYuan:		return 50;
	case Coin1Yuan:			return 100;
	case Banknote1Yuan:		return 100;
	case Banknote5Yuan:		return 500;
	case Banknote10Yuan:	return 1000;
	case Banknote20Yuan:	return 2000;
	case Banknote50Yuan:	return 5000;
	case Banknote100Yuan:	return 10000;
	default:
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����ִ���תΪʵ�ʵĽ���λ���֣�

@param      int type Ҫת��������

@retval     DWORD ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DWORD CSCMessageMakeHelper::TranBanknoteToValue(int type){
	switch(type){
	case 1:			return 100;
	case 5:			return 500;
	case 10:		return 1000;
	case 20:		return 2000;
	case 50:		return 5000;
	case 100:		return 10000;
	default:
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����������ת��Ϊֽ�Ҵ���

@param      int nType;ֽ����������

@retval     BankNoteAndCoinType_t ֽ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CSCMessageMakeHelper::TranBanknoteToType(int nType){
	switch(nType){
	case 1  :	return  Banknote1Yuan;
	case 5  :	return  Banknote5Yuan;
	case 10 :	return  Banknote10Yuan;
	case 20 :	return  Banknote20Yuan;
	case 50 :	return  Banknote50Yuan;
	case 100:	return  Banknote100Yuan;
	default:
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ӳ����\ֽ��������¼�

@param      OperComHdr_t& operComHdr (����ͷ��)
@param      LPBYTE lpByte�������壩
@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeClearBanknotesOrCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount,WORD& wMsgLen){
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// �ܽ��
	DWORD dwMoneyCount = 0;
	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
		dwMoneyCount += TranTypeToValue(typeCount->cash_type) * typeCount->cash_count;
	}
	memcpy(lpMsg,&dwMoneyCount,4);
	lpMsg += 4;

	// �����������
	*lpMsg = vecTypeCount.size();
	lpMsg += 1;

	// ���ִ��뼰����
	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
		*lpMsg = typeCount->cash_type;
		lpMsg += 1;
		memcpy(lpMsg,&typeCount->cash_count,2);
		lpMsg += 2;
	}



	CTBanknoteCountInfo::BANKNOTE_BOX_INFO bankBox = theBANKNOTE_COUNT.GetBanknoteboxInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycleBoxD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
	CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

	CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
	CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBoxA = theCOIN_COUNT.GetChangeCoinboxAInfo();
	CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeBoxB = theCOIN_COUNT.GetChangeCoinboxBInfo();
	BankNoteAndCoinType_t changeBoxAType,changeBoxBType;
	//// ������ı���
	//changeBoxAType = TranCoinType(theTVM_INFO.GetCHChangeBoxAType());
	//changeBoxBType = TranCoinType(theTVM_INFO.GetCHChangeBoxBType());
	// ������
	//dwReleseMometCount += changeBoxA.ulCount * TranCoinTypeToValue(theTVM_INFO.GetCHChangeBoxAType());
	//dwReleseMometCount += changeBoxB.ulCount * TranCoinTypeToValue(theTVM_INFO.GetCHChangeBoxBType());

	DWORD dwReleseMoneyAmount = 0;
	BYTE  bReleaseMoneyType = 0;
	// Ӳ�Ҳд�(1Ԫ��
	if(hopperA.ulCurCount + hopperB.ulCurCount>0){
		bReleaseMoneyType += 1;
		dwReleseMoneyAmount += hopperA.ulCurCount* 100 + hopperB.ulCurCount * 100;
	}
	// ֽ�Ҳд棨10Ԫ��
	if (recycleBoxA.ulCurCount + recycleBoxD.ulCurCount>0){
		bReleaseMoneyType += 1;
		dwReleseMoneyAmount += recycleBoxA.BankNotesCountAvos() + recycleBoxD.BankNotesCountAvos();
	}
	// ֽ�Ҳд棨5Ԫ��
	if (recycleBoxB.ulCurCount + recycleBoxC.ulCurCount + changeBox.ulCurCount>0){
		bReleaseMoneyType += 1;
		dwReleseMoneyAmount += recycleBoxB.BankNotesCountAvos() + recycleBoxC.BankNotesCountAvos() + changeBox.BankNotesCountAvos();
	}
	// �д��ܽ��
	memcpy(lpMsg,&dwReleseMoneyAmount,4);
	lpMsg += 4;
	// �޲д�
	if(dwReleseMoneyAmount == 0){
		// �д��������
		*lpMsg = 0x00;
		lpMsg += 1;
	}
	else{
		// �д��������
		*lpMsg = bReleaseMoneyType;
		lpMsg += 1;

		// һԪӲ���вд�
		if(hopperA.ulCurCount + hopperB.ulCurCount>0){
			WORD num = hopperA.ulCurCount + hopperB.ulCurCount;
			// ���ִ���
			*lpMsg = Coin1Yuan;
			lpMsg++;
			// ��������
			memcpy(lpMsg,&num,2);
			lpMsg += 2;

			// �д�λ������
			*lpMsg = hopperA.ulCurCount>0?1:0 + hopperB.ulCurCount>0?1:0;
			lpMsg++;

			// �д沿������
			if(hopperA.ulCurCount > 0){
				// �д沿������
				*lpMsg = 0x04;	// Ӳ��ģ��
				lpMsg++;
				// �д沿��λ��
				*lpMsg = MAGAZINEE;
				lpMsg++;
			}
			if(hopperB.ulCurCount > 0){
				// �д沿������
				*lpMsg = 0x04;	// Ӳ��ģ��
				lpMsg++;
				// �д沿��λ��
				*lpMsg = MAGAZINED;
				lpMsg++;
			}
		}

		// ��Ԫֽ���вд�
		if(recycleBoxB.ulCurCount + recycleBoxC.ulCurCount>0){
			WORD num = recycleBoxB.ulCurCount + recycleBoxC.ulCurCount + changeBox.ulCurCount;
			// ���ִ���
			*lpMsg = Banknote5Yuan;
			lpMsg++;
			// ��������
			memcpy(lpMsg,&num,2);
			lpMsg += 2;
			// �д�λ������
			*lpMsg = (recycleBoxB.ulCurCount+recycleBoxC.ulCurCount>0)?1:0 + changeBox.ulCurCount>0?1:0;
			lpMsg++;
			// �д沿������
			if(recycleBoxB.ulCurCount + recycleBoxC.ulCurCount > 0){
				// �д沿������
				*lpMsg = 0x05;	// ֽ��ģ��
				lpMsg++;
				// �д沿��λ��
				*lpMsg = MAGAZINEI;
				lpMsg++;
			}
			if(changeBox.ulCurCount > 0){
				// �д沿������
				*lpMsg = 0x05;	// ֽ��ģ��
				lpMsg++;
				// �д沿��λ��
				*lpMsg = MAGAZINEH;
				lpMsg++;
			}
		}

		// ʮԪֽ���вд�
		if(recycleBoxA.ulCurCount + recycleBoxD.ulCurCount>0){
			WORD num = recycleBoxA.ulCurCount + recycleBoxD.ulCurCount;
			// ���ִ���
			*lpMsg = Banknote10Yuan;
			lpMsg++;
			// ��������
			memcpy(lpMsg,&num,2);
			lpMsg += 2;
			// �д�λ������
			*lpMsg = (recycleBoxA.ulCurCount+recycleBoxD.ulCurCount>0)?1:0;
			lpMsg++;
			// �д沿������
			if(recycleBoxA.ulCurCount + recycleBoxD.ulCurCount > 0){
				// �д沿������
				*lpMsg = 0x05;	// ֽ��ģ��
				lpMsg++;
				// �д沿��λ��
				*lpMsg = MAGAZINEI;
				lpMsg++;
			}
		}
	}
	
	// ��Ϣ����
	wMsgLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ӳ��������¼�

@param      OperComHdr_t& operComHdr ������ͷ����
@param      LPBYTE lpByte�������壩

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CSCMessageMakeHelper::MakeClearCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount){
//	LPBYTE lpMsg = lpByte;
//	MakeOperComHdr(operComHdr,lpMsg);
//	lpMsg += LEN_BUSINESS_HEADER;
//
//	// �ܽ��
//	DWORD dwMoneyCount = 0;
//	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
//		dwMoneyCount += TranTypeToValue(typeCount->cash_type) * typeCount->cash_count;
//	}
//	*lpMsg = dwMoneyCount;
//	lpMsg += 4;
//
//	// �����������
//	*lpMsg = vecTypeCount.size();
//	lpMsg += 1;
//
//	// ���ִ��뼰����
//	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
//		*lpMsg = typeCount->cash_type;
//		lpMsg += 1;
//		*lpMsg = typeCount->cash_count;
//		lpMsg += 2;
//	}
//}