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
@brief      CSCMessageHelper构造函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageMakeHelper::CSCMessageMakeHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CSCMessageHelper析构函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CSCMessageMakeHelper::~CSCMessageMakeHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得AFC系统专用头

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
@brief      取得系统公共域

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
SysComHdr_t CSCMessageMakeHelper::AquireSysComHeader(WORD msgType)
{
	SysComHdr_t header;
	header.transTime  = ComGetCurTime();
	header.resourceProviderID = char2BCD(0x02);//沈阳地铁2号线运营公司
	header.transType = HIBYTE(msgType);
	header.transSubType = LOBYTE(msgType);
	header.cityCode = 1100;//城市代码
	header.industryCode = 0003;//行业代码
	header.lineCode =theMAINTENANCE_INFO.GetCurrentACCDevice().lineCode;
	header.stationCode = theMAINTENANCE_INFO.GetCurrentACCDevice().stationCode;
	header.deviceType = theMAINTENANCE_INFO.GetCurrentACCDevice().deviceType;
	header.deviceID = theMAINTENANCE_INFO.GetCurrentACCDevice();
	header.transSerialNo = theCOUNT_INFO.GetUDSN();
	if(theTVM_SETTING.GetDeviceTestMode() == 0){
		header.dataType = 0x00;			//正常数据
	}
	else{
		header.dataType = 0x01;			//测试数据
	}	
	header.parameterVersion = 0;
	//memcpy(header.samID,theAPP_SESSION.GetSAMInfo().rw_acc_psam_id,sizeof header.samID);
	header.samTransSerialNo = 0;
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得票卡公共域

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
@brief      取得持卡人公共域

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
@brief      取得内部交易数据公共域

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
@brief      取得业务数据头

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
@brief   取得状态数据头   

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
@brief      生成AFC系统公共头

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeAfcSysHdr(AfcSysHdr_t& afcSysHder, LPBYTE lpMsg)
{
	// 3 BIN 操作员ID
	memcpy(lpMsg, afcSysHder.OperatorID,sizeof(afcSysHder.OperatorID));
	lpMsg += sizeof(afcSysHder.OperatorID);
	// 4 BIN 发生日期
	Date2BCD(afcSysHder.DateTime,lpMsg);
	lpMsg += 4;
	// 2 BIN 消息长度
	memcpy(lpMsg,&afcSysHder.DataLen,2);
	lpMsg+=2;
	// 1 BIN 预留
	*lpMsg = 0xFF;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      生成系统公共域

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
@brief     生成票卡公共域 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysCardCom(SysCardCom_t& sysCardCom,LPBYTE lpMsg)
{
	// 4 BIN 发行票卡的发行人的唯一ID
	ComRevLong(sysCardCom.cardIssuerId, lpMsg);
	lpMsg += 4;
	// 4 BIN 票卡的序号
	ComRevLong(sysCardCom.cardSerialNumber, lpMsg);
	lpMsg += 4;
	// 4 BIN 票卡的类型
	ComRevLong(sysCardCom.cardType, lpMsg);
	lpMsg += 4;
	// 4 BIN 票卡的当前使用周期计数
	ComRevLong(sysCardCom.cardLifeCycleCount, lpMsg);
	lpMsg += 4;
	// 4 BIN 操作表序列号
	ComRevLong(sysCardCom.cardActionSequenceNumber, lpMsg);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    生成持卡人公共域 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysCardholderCom(SysCardholderCom_t& sysCardholderCom,LPBYTE lpMsg)
{    
	// 4 BIN 在特定发行人范围内识别持卡人的专用号码
	ComRevLong(sysCardholderCom.cardholderSerialNum, lpMsg);
	lpMsg += 4;
	// 4 BIN 持卡人的发行人
	ComRevLong(sysCardholderCom.cardholderIssuerId, lpMsg);
	lpMsg += 4;
	// 4 BIN ACC 定义公司标识符。
	ComRevLong(sysCardholderCom.companyId, lpMsg);
	lpMsg += 4;
	// 4 BIN 乘客级别
	ComRevLong(sysCardholderCom.classificationLevel, lpMsg);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   生成内部交易数据公共域

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg)
{
	// 4 1BIN+2BCD+1BIN 设备ID
	transComHdr.deviceid.Serialize(lpMsg);
	lpMsg+=4;
	// 4 BIN AFC数据流水号
	memcpy(lpMsg, &transComHdr.dwAfcSerialNo, 4);
	lpMsg += 4;
	// 7 BCD 交易产生时间
	Time2BCD(transComHdr.dtTransDataTime,lpMsg);
	lpMsg+=7;
	// 1 BIN 数据类型
	*lpMsg = transComHdr.bMsgType;
	lpMsg++;
	// 1 BIN 数据子类型
	*lpMsg = transComHdr.bMsgSubType;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     生成业务数据专用头

@param     OperComHdr_t      业务数据专用头

@retval    无

@exception 
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperComHdr(OperComHdr_t& OperComHdr, LPBYTE lpMsg)
{
	// 3 BCD 操作员ID
	memcpy(lpMsg, OperComHdr.operatorID, sizeof(OperComHdr.operatorID));
	lpMsg += 3;
	// 4 BCD 发生日期
	Date2BCD(OperComHdr.dataTime, lpMsg);
	lpMsg += 4;
	// 1 BIN 位置类型
	*lpMsg = OperComHdr.localType;
	lpMsg++;
	// 2 BCD 位置ID
	BYTE btArrLocal[2];
	memset(btArrLocal,0x00,2);
	int2BCD(OperComHdr.localAreaID,(char*)btArrLocal,2);

	*lpMsg = btArrLocal[0];//HIBYTE(OperComHdr.localAreaID);
	lpMsg++;
	*lpMsg = btArrLocal[1];//LOBYTE(OperComHdr.localAreaID);
	lpMsg++;
	// 4 1BIN+2BCD+1BIN 设备ID
	OperComHdr.deviceID.Serialize(lpMsg);
	lpMsg += 4;
	// 4 BIN AFC数据流水号
	memcpy(lpMsg, &OperComHdr.afcSeqNo, 4);
	lpMsg += 4;
	// 7 BCD 业务数据产生时间
	Time2BCD(OperComHdr.operTime, lpMsg);
	lpMsg += 7;
	// 1 BIN 数据类型
	*lpMsg = OperComHdr.msgType;
	lpMsg++;
	// 1 BIN 数据子类型
	*lpMsg = OperComHdr.msgSubType;
	lpMsg++;
	// 2 BIN 消息长度
	memcpy(lpMsg, &OperComHdr.msgLen, 2);
	lpMsg += 2;   
	// 3 预留
	memset(lpMsg, 0xFF, 3);
	lpMsg += 3;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      生成状态数据头

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg)
{
	// 2 BIN 数据类别代码
	memcpy(lpMsg, &statusComHdr.wStatusCode, 2);
	lpMsg+=2;
	// 2 BCD 车站ID
	char chStationID[2] = {0};
	int2BCD(statusComHdr.wStationCode,&chStationID[0],2);
	*lpMsg = chStationID[0];//HIBYTE(statusComHdr.wStationCode);
	lpMsg++;
	*lpMsg = chStationID[1];//LOBYTE(statusComHdr.wStationCode);
	lpMsg++;
	// 4 1BIN+2BCD+1BIN 设备ID
	statusComHdr.deviceID.Serialize(lpMsg);
	lpMsg += 4;
	// 3 BCD 操作员ID
	memcpy(lpMsg, statusComHdr.operatorID, sizeof(statusComHdr.operatorID));
	lpMsg += sizeof(statusComHdr.operatorID);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      生成安全公共域

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
	// 4 BIN 用来产生交易MAC 的密钥版本
	ComRevLong(sysSecurityHdr.keyVersion, lpMsg);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		生成ACC设备寄存器数据

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnAuditRegister(TXN_AUDIT_REGISTER& TxnAuditRegister, ACCAR& arData, LPBYTE lpMsg, LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFC专用头
	MakeAfcSysHdr(TxnAuditRegister.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// 系统公共头
	MakeSysComHdr(TxnAuditRegister.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;

	// 交易中键/值对的数字号
	DWORD dataNumber = 0;
	LPBYTE lpNumberPos = lpData;
	lpData += 4;

	// 产生审核数据的快照原因
	ComRevLong(1,lpData);
	lpData += 4;

	// 记录内容
	LPBYTE lpValue = lpData;
	LPBYTE lpId = lpData + 1200;
	for (ACCAR::iterator itemIter = arData.begin();itemIter != arData.end();itemIter++)
	{
		if(itemIter->first > 512 || itemIter->second == 0)
		{
			continue;
		}
		// 记录值
		ComRevLong(itemIter->second,lpValue);
		lpValue += 4;
		// 记录ID
		ComRevLong(itemIter->first,lpId);
		lpId += 4;

		dataNumber++;
	}

	// 交易中键/值对的数字号
	ComRevLong(dataNumber,lpNumberPos);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		生成CD版本切换交易数据（预留）

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnEventCDVersionStatus(TXN_EVENT_CD_VERSION_STATUS& TxnEventCDVersionStatus,LPBYTE lpMsg,LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFC专用头
	MakeAfcSysHdr(TxnEventCDVersionStatus.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// 系统公共头
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
@brief		生成设备状态交易数据（预留）

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnEventDeviceStatus(TXN_EVENT_DEVICE_STATUS& TxnEventDeviceStatus,LPBYTE lpMsg,LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFC专用头
	MakeAfcSysHdr(TxnEventDeviceStatus.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// 系统公共头
	MakeSysComHdr(TxnEventDeviceStatus.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;

	// 设备状态
	ComRevLong(TxnEventDeviceStatus.deviceStatus, lpData);
	lpData += 4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		生成设备标识交易数据（预留）

@param		

@retval		

@exception	
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTxnEventDeviceIdentification(TXN_EVENT_DEVICE_IDENTIFICATION& TxnEventDeviceIdentification,LPBYTE lpMsg,LPBYTE lpAfcHead)
{
	LPBYTE lpData = lpMsg;
	LPBYTE lpAfcData = lpAfcHead;

	// AFC专用头
	MakeAfcSysHdr(TxnEventDeviceIdentification.afcSysHdr, lpAfcData);
	lpAfcData += LEN_AFC_SYS_HEADER;

	// 系统公共头
	MakeSysComHdr(TxnEventDeviceIdentification.sysComHdr, lpData);
	lpData += LEN_TXN_SYSCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TVM、AVM故障单业务数据

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBusinessFaliedData(OperComHdr_t& operComHdr,BUSINESS_FAILED_DATA& inBusinessFailedData,LPBYTE lpByte){
	// AFC专用头
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	
	// 设备编号
	inBusinessFailedData.DeviceID.Serialize(lpMsg);
	lpMsg +=4;
	// 故障时间
	inBusinessFailedData.DateTime.Serialize(lpMsg);
	lpMsg += 7;
	// 设备AFC流水号
	memcpy(lpMsg,&inBusinessFailedData.DeviceAfcNum,4);
	lpMsg += 4;
	// 对应交易流水号
	memcpy(lpMsg,&inBusinessFailedData.TransactionNum,4);
	lpMsg += 4;
	// 应对款金额
	memcpy(lpMsg,&inBusinessFailedData.RefoundAmount,4);
	lpMsg += 4;
	// 故障原因
	memcpy(lpMsg,&inBusinessFailedData.FailedReason,12);
	lpMsg += 12;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      参数和程序下载业务数据
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDownloadParameterAndProgram(OperComHdr_t& operComHdr,vector<ParameterAndSoftwareUpdate>& updates, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 1BIN 下载数量
	char count = updates.size();
	memcpy(lpMsg, &count, 1);
	lpMsg += 1;
	for (vector<ParameterAndSoftwareUpdate>::iterator it = updates.begin(); it != updates.end(); it++)
	{
		// 2 BCD 参数和程序ID
		//int2BCD(it->parameterID, (char*)lpMsg, 2);
		ComRevWORD(it->parameterID, lpMsg);
		lpMsg += 2;
		// 2 BIN 设备供应商ID
		memcpy(lpMsg,&(it->providerID),2 );
		lpMsg += 2;
		// 2 BIN 软件批次号
		memcpy(lpMsg,&(it->softwareBatchNO),2 );
		lpMsg += 2;
		// 1 BIN 版本类型
		*lpMsg = it->versionType;
		lpMsg++;
		// 4 BIN 版本号
		memcpy(lpMsg,&(it->version),4 );
		lpMsg += 4;
		// 7 BCD 下载时间
		_DATE_TIME currentTime = ComGetCurTime();
		Time2BCD(currentTime, lpMsg);
		lpMsg += 7;
		// 32 ASCII 文件名称
		CString& downLoadPath = it->downLoadPath;
		memcpy(lpMsg, downLoadPath.GetBuffer(), downLoadPath.GetLength()>32 ? 32 :downLoadPath.GetLength());
		lpMsg += 32;		
		// 1 BIN 下载结果
		*lpMsg = 0x00;
		lpMsg++;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      参数和程序
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
long CSCMessageMakeHelper::MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;

	long lDataLen = 34;
	// 切换数量
	WORD wCount = 0;
	wCount = VecVersionInfo.size();
	operComHdr.msgLen = lDataLen + wCount*23;
	// 业务头生成
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 2 BIN 切换数量
	memcpy(lpMsg, &wCount, 2);
	lpMsg += 2;
	CParamHelper::VEC_VERSION_INFO::iterator itor;
	for (itor = VecVersionInfo.begin(); itor != VecVersionInfo.end(); itor++)
	{
		// 2 BCD 参数和程序ID
		int2BCD((*itor).paramId, (char*)lpMsg, 2);
		lpMsg += 2;
		// 2 BIN 设备供应商ID
		memset(lpMsg, 0x00, 2);
		lpMsg += 2;
		// 2 BIN 软件批次号
		memset(lpMsg, 0x00, 2);
		lpMsg += 2;
		// 1 BIN 版本类型
		*lpMsg = (*itor).versionType;
		lpMsg++;
		// 4 BIN 旧版本号
		memcpy(lpMsg, &(*itor).oldVersion, 4);
		lpMsg += 4;
		// 4 BCD 生效日期
		Date2BCD((*itor).oldStartDate, lpMsg);
		lpMsg += 4;
		// 4 BIN 新版本号
		memcpy(lpMsg, &(*itor).newVersion, 4);
		lpMsg += 4;			
		// 4 BCD 生效日期
		Date2BCD((*itor).newStartDate, lpMsg);
		lpMsg += 4;
	}
	lDataLen += 23*wCount;
	return lDataLen;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BOM重要状态变更
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeImportantStatusChange(OperComHdr_t& operComHdr, LPBYTE lpByte, SERVICE_STATUS status)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 3 1BIN+1BIN+1BIN 状态ID 
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x01;
	lpMsg++;
	// 1 BIN 状态值 
	*lpMsg = status;
	lpMsg++;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ISAM签到签退（预留）
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeIsamCheckIn(OperComHdr_t operComHdr, LPBYTE lpByte, ISAM_AUTH isam_auth, BYTE result)
{
	LPBYTE lpMsg = lpByte;

	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// 6 BCD ISAM卡号
	memcpy(lpMsg,isam_auth.samid,6);
	lpMsg+=6;
	// 4 BIN 参与方ID
	memcpy(lpMsg, &isam_auth.participantId, 4);
	lpMsg += 4;
	// 1 BIN 动作状态
	*lpMsg = isam_auth.auth_type;
	lpMsg++;
	// 1 BIN 认证结果
	*lpMsg = isam_auth.authResult;
	lpMsg++;
	// 4 BIN 授权额度
	memcpy(lpMsg, &isam_auth.amount, 4);
	lpMsg += 4;
	// 7 BCD 授权失效时间
	byte specialTime[7] = {0xFF};
	if(memcmp(&isam_auth.datetime,&specialTime,7)==0)//签退时，值为7个FF，不用进行BCD变化。
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
@brief      设备故障日志
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceErrorData(OperComHdr_t& operComHdr,DEVICE_ERRORDATA& ErrorData, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 1 BIN 设备部件类型
	*lpMsg = ErrorData.partType;
	lpMsg++;
	// 1 1BIN+3BIN 部件ID
	*lpMsg = ErrorData.bDeviceType;
	lpMsg++;
	memcpy(lpMsg, ErrorData.devicePartID, 3);
	lpMsg += 3;
	// 2 BIN 设备供应商ID
	memcpy(lpMsg, &ErrorData.deviceDeviderId, 2);
	lpMsg += 2;
	// 4 1BIN+1BIN+1BIN+1BIN 故障代码
	memcpy(lpMsg, ErrorData.maintanCode, 4);
	lpMsg += 4;
	// 2 1BIN+1BIN 维修代码
	memcpy(lpMsg, ErrorData.ecode, 2);
	lpMsg += 2;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      操作日志
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 2 1BIN+1BIN 操作代码
	ComRevWORD(operLog.operCode, lpMsg);
	lpMsg += 2;
	// 1 BIN 操作级别
	*lpMsg = operLog.operLevel;
	lpMsg++;
	// 1 BIN 操作结果
	*lpMsg = operLog.result;
	lpMsg++;
	// 32 ASCII 操作内容
	memcpy(lpMsg, operLog.operDetail, sizeof (operLog.operDetail));
	lpMsg += 32;
	// 32 ASCII 备注
	memcpy(lpMsg, operLog.operTip, sizeof (operLog.operTip));
	lpMsg += 32;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      设备寄存器数据

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

	// 产生寄存器数据状态代码
	*lpMsg = arReson;
	lpMsg++;

	// 设备寄存器数据记录总数
	unsigned short arlen = 0;
	LPBYTE lpLengthPos = lpMsg;
	lpMsg+=2;

	// 设备寄存器数据
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

	// 设备寄存器数据记录总数
	*lpLengthPos= LOBYTE(arlen);
	lpLengthPos++;
	*lpLengthPos = HIBYTE(arlen);

	// 数据长度
	dataLen = lpMsg - lpByte;
	WORD wLen = dataLen;
	memcpy(lpByte + INX_BUSINESS_HEADER_LENGTH,&wLen,2);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      组成纸币箱更换的电文

@param      OperComHdr_t& operComHdr  协议头

@param      CASHBOX_CHANGE& cash_change 钱箱更换数据

@param      [out] LPBYTE lpByte   电文数据

*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeCashBoxExchange(OperComHdr_t& operComHdr, CASHBOX_CHANGE& cash_change,LPBYTE  lpData )
{
	try{
		MakeOperComHdr(operComHdr, lpData); //构造报文协议头
		lpData += LEN_BUSINESS_HEADER;      //协议头偏移量

		memcpy(lpData, &cash_change.handleType,1);// 操作类型 1BIN 
		lpData ++;
		
		//memcpy(lpData, &cash_change.COIN_BANKNOTE_BOX_ID, 4);    //1BCD+1BIN+2BIIN 钱箱ID
		//lpData += 4;
		//for(int i = 0;i < 4;++i){
		//	*lpData = cash_change.COIN_BANKNOTE_BOX_ID[i];	// BCD 4位
		//	lpData++;
		//}
		cash_change.Serialze(lpData);
		lpData+=4;
		memcpy(lpData, &cash_change.position, 1); //1BIN  钱箱位置
		lpData ++;
		
		memcpy(lpData, &cash_change.State, 1);    //1BIN  钱箱状态  
		lpData++;
		
		memcpy(lpData, &cash_change.btCount, 1);  //1BIN  币种总数
		lpData++;

		//1BIN+2BIN 币种代码+钱币数量
		vector<CASH_TYPE_COUNT> ::iterator  iter = cash_change.vecCash.begin();
		while (iter != cash_change.vecCash.end() )
		{
			memcpy(lpData, &(*iter).cash_type, 1); //1BIN 币种代码 
			lpData++;

			memcpy(lpData, &(*iter).cash_count, 2);//2BIN 钱币数量
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
@brief      账户锁定
@param      LPBYTE lpByte        
@retval     无
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
@brief      交易流水号重置
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceUdsnNoReset(OperComHdr_t& operComHdr, UDSN_RESET& udsnRest, UDSN_RESET_REASON& resetReason, LPBYTE lpMsg)
{
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 1 BIN 重置原因
	*lpMsg = resetReason;
	lpMsg++;
	// 1 BIN 设备流水类型数
	*lpMsg = 1;
	lpMsg++;
	// 1 BIN 设备流水号类型
	*lpMsg = udsnRest.udsn_type;
	lpMsg++;
	// 4 BIN 重置前设备流水号
	memcpy(lpMsg, &udsnRest.beforeResetNo, 4);
	lpMsg += 4;
	// 4 BIN 重置后设备流水号
	memcpy(lpMsg, &udsnRest.afterResetNo, 4);
	lpMsg += 4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      部件更换
@param      OperComHdr_t operComHdr 
LPBYTE		 lpByte
BYTE		 bType     部件类型
LPBYTE		 lpNewID   新部件编号
LPBYTE		 lpOldID   旧部件编号
@retval     无
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
@brief      TVM审计数据
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTVMAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData, LPBYTE lpdata/*, DWORD& datalen*/)
{
	LPBYTE lpMsg = lpdata;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// 开始时间
	purchaseData.auditTimeFrom.Serialize(lpMsg);
	lpMsg += 7;
	// 结束时间
	purchaseData.auditTimeTo.Serialize(lpMsg);
	lpMsg += 7;

	// 业务数据填充
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
	//// 统计时间
	//Date2BCD(purchaseData.businessDate,lpMsg);
	//lpMsg+=4;
	//// 产生数据原因
	//*lpMsg = auditflag;
	//lpMsg+=1;
	////数据起始时间
	//Time2BCD(purchaseData.auditTimeFrom,lpMsg);
	//lpMsg+=7;
	////数据结束时间
	//Time2BCD(purchaseData.auditTimeTo,lpMsg);
	//lpMsg+=7;
	////收入总金额
	//memcpy(lpMsg,&(allIncome.receivableAmount),4);
	//lpMsg+=4;	
	////支出总金额
	//int refundAmount = abs(allIncome.refundAmount);//??支出金额填正值
	//memcpy(lpMsg,&refundAmount,4);
	//lpMsg+=4;
	////实收总金额
	//memcpy(lpMsg,&(allIncome.incomeAmount),4);
	//lpMsg+=4;
	////回收车票总数量
	//int recyleTicketCount = allIncome.recyleTicketCount;
	//memcpy(lpMsg,&recyleTicketCount,4);
	//lpMsg+=4;

	////支付方式总数
	//int payTypeCount = purchaseData.incomeByPayTypeItems.size();
	//memcpy(lpMsg,&payTypeCount,4);
	//lpMsg+=4;
	//for( map<char,IncomeItem>::iterator iterIncome = purchaseData.incomeByPayTypeItems.begin();iterIncome!=purchaseData.incomeByPayTypeItems.end();iterIncome++)
	//{
	//	//支付方式
	//	*lpMsg = iterIncome->first;
	//	lpMsg+=1;
	//	//收入总金额
	//	memcpy(lpMsg,&(iterIncome->second.receivableAmount),4);
	//	lpMsg+=4;	
	//	//支出总金额
	//	int refundAmount = abs(iterIncome->second.refundAmount);//??支出金额填正值
	//	memcpy(lpMsg,&refundAmount,4);
	//	lpMsg+=4;
	//	//实收总金额
	//	memcpy(lpMsg,&(iterIncome->second.incomeAmount),4);
	//	lpMsg+=4;
	//	//回收车票总数量
	//	int recyleTicketCount = iterIncome->second.recyleTicketCount;
	//	memcpy(lpMsg,&recyleTicketCount,4);
	//	lpMsg+=4;
	//}
	////交易类型总数
	//short transCount = trans.size();
	//memcpy(lpMsg,&transCount,2);
	//lpMsg+=2;

	//for(map<TransKey,vector<SCAuditKey>,TransKeyCompare>::iterator iterTrans = trans.begin();iterTrans!=trans.end();iterTrans++)
	//{
	//	//票卡发行商ID，交易类型，交易子类型
	//	const TransKey & key = iterTrans->first;
	//	key.Serialize(lpMsg);
	//	lpMsg+=6;
	//	//车票产品种类总数
	//	const vector<SCAuditKey>& transItems = iterTrans->second;
	//	char transItemCount = transItems.size();
	//	memcpy(lpMsg,&transItemCount,1);
	//	lpMsg+=1;
	//	for(vector<SCAuditKey>::const_iterator iterTranItem = transItems.begin();iterTranItem!=transItems.end();iterTranItem++)
	//	{
	//		//车票产品种类
	//		memcpy(lpMsg,&(iterTranItem->ticketType),2);
	//		lpMsg+=2;
	//		const SCAuditValue& iterTranItemValue = purchaseData.purchaseItems[*iterTranItem];
	//		//交易总数量
	//		memcpy(lpMsg,&(iterTranItemValue.transCount),4);
	//		lpMsg+=4;
	//		int depositAmount = iterTranItemValue.depositAmount;
	//		int transAmount = iterTranItemValue.transAmount;
	//		if(iterTranItem->serviceID == REFUND_SVC)
	//		{
	//			depositAmount = abs(depositAmount);	//退款和退款申请时押金填正值
	//			transAmount = abs(transAmount);
	//		}
	//		//押金总金额
	//		memcpy(lpMsg,&depositAmount,4);
	//		lpMsg+=4;
	//		//交易总金额
	//		memcpy(lpMsg,&transAmount,4);
	//		lpMsg+=4;
	//		//交易手续费总金额
	//		memcpy(lpMsg,&(iterTranItemValue.serviceFeeAmount),4);
	//		lpMsg+=4;
	//		//卡成本费
	//		memcpy(lpMsg,&(iterTranItemValue.cardCostAmount),4);
	//		lpMsg+=4;
	//		//回收卡总数量
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
@brief      包传输审计
@param      OperComHdr_t&	operComHdr
LPBYTE			lpByte      
@retval     无
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
@brief     生成TVM状态电文
@param     MessageType      状态电文类型 
0   运营模式
1   24小时运营模式
2   延长运营模式状态
3   运营模式 + 24小时运营 + 延长运营模式
4   TVM完整状态
@param     LPBYTE lpByte    拼装电文
@retval    无
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
	//模式状态、24小时运营状态、延长运营状态。
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
		//1 服务状态
		memcpy(lpMsg, &TVM_STATUS_IDS::SERVICE_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetServiceStatus();
		lpMsg++;
		//2 运行状态
		memcpy(lpMsg, &TVM_STATUS_IDS::RUN_STATUS, 3);
		lpMsg += 3;	
		*lpMsg = theTVM_STATUS_MGR.GetRunStatus();
		lpMsg++;
		//3 工作模式
		memcpy(lpMsg, &TVM_STATUS_IDS::WORK_MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetServerWorkMode();
		lpMsg++;
		//4 运营模式
		memcpy(lpMsg, &TVM_STATUS_IDS::OPERATION_MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetOperationMode();
		lpMsg++;
		//5 24小时运营
		memcpy(lpMsg, &TVM_STATUS_IDS::HOUR24MODE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.Get24HourMode();
		lpMsg++;
		//6 延长运营时间
		memcpy(lpMsg, &TVM_STATUS_IDS::DELAY_RUN_MODE, 3);
		lpMsg += 3;
		*lpMsg = (theTVM_STATUS_MGR.GetDelayTime()==0)?0x00:0x01;
		lpMsg++;
		//7 连接状态
		memcpy(lpMsg, &TVM_STATUS_IDS::CONNECT_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCommunicate();
		lpMsg++;
		//8 时钟状态
		memcpy(lpMsg, &TVM_STATUS_IDS::TIME_DELAY, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetTimeSynStatus();
		lpMsg++;
		//9 登陆状态
		memcpy(lpMsg, &TVM_STATUS_IDS::LOGIN_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetLoginStatus();
		lpMsg++;		

		/*------------------------Card---------------------------*/
		//10 Card箱1安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox1Status();
		lpMsg++;
		//11 Card箱1数量
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox1NumStatus();
		lpMsg++;
		//12 Card箱2安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX2_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox2Status();
		lpMsg++;
		//13 Card箱2数量
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_BOX2_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardBox2NumStatus();
		lpMsg++;
		
		//14 Card 废票箱状态
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_CYCLEBOX_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardWasteBoxStatus();
		lpMsg++;
		//15 Card 废票箱数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_CYCLEBOX_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardWasteBoxNumStatus();
		lpMsg++;
		/*----------------------Coin------------------------------*/
		//16 硬币找零箱1安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox1Status();
		lpMsg++;

		//17 数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox1NumStatus();
		lpMsg++;
		//18 硬币找零箱2安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX2_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox2Status();
		lpMsg++;
		//19 数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CHANGE_BOX2_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinAddBox2NumStatus();
		lpMsg++;
		 //20 硬币回收箱1安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CYCLE_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinCollectionBox1Status();
		lpMsg++;
		//21 数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_CYCLE_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinCollectionBox1NumStatus();
		lpMsg++;
		// 22 硬币Hopper1数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_HOPPER_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinHopper1NumStatus();
		lpMsg++;
		// 23 硬币Hopper2数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_HOPPER_BOX2_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinHopper2NumStatus();
		lpMsg++;
		
		// 24 纸币找零箱1安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX1_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteChangeBox1Status();
		lpMsg++;
		// 25 纸币找零箱1数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CHANGE_BOX1_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus();
		lpMsg++;
		// 26 纸币回收箱安装状态
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteCollectionBoxStatus();
		lpMsg++;
		// 27 纸币回收箱数量状态
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_CYCLE_BOX_COUNT,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteCollectionBoxNumStatus();
		lpMsg++;
		///---------------------读卡器--------------------------------
		// 28 读卡器1状态
		memcpy(lpMsg, &TVM_STATUS_IDS::RW1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetRW1Status();
		lpMsg++;
		// 29 读卡器1状态
		memcpy(lpMsg, &TVM_STATUS_IDS::RW2_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetRW2Status();
		lpMsg++;

		//30SAM卡1类型
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM1_TYPE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam1Type();// 读写器SAM1卡类型
		lpMsg++;
		
		//31 SAM卡2类型
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM2_TYPE, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam2Type();// 读写器SAM2卡类型
		lpMsg++;

		//32 SAM卡1状态
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM_1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam1Status();// SAM1卡状态
		lpMsg++;

		//33 SAM卡2状态
		memcpy(lpMsg, &TVM_STATUS_IDS::RW_SAM_1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam2Status();// SAM2卡状态
		lpMsg++;

		// 34 打印机
		memcpy(lpMsg, &TVM_STATUS_IDS::PRINTER_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetPrintStatus();
		lpMsg++;

		// 35 维修门状态
		memcpy(lpMsg,&TVM_STATUS_IDS::DOOR_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetMaintenanceDoorStatus();
		lpMsg++;
		// 36 纸币模块
		memcpy(lpMsg,&TVM_STATUS_IDS::BANKNOTE_ACCEPT_MODULE_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetBanknoteModuleStatus();
		lpMsg++;
		
		// 37 发卡模块
		memcpy(lpMsg,&TVM_STATUS_IDS::CARD_MODULE_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardModuleStatus();
		lpMsg++;
		// 38 硬币模块
		memcpy(lpMsg,&TVM_STATUS_IDS::COIN_MODULE_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCoinModuleStatus();
		lpMsg++;
		
		// 39 LED模块
		memcpy(lpMsg,&TVM_STATUS_IDS::STATUS_DISPLAY_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetStatusDisplayStatus();
		lpMsg++;
		/*
		// 票卡读卡器
		memcpy(lpMsg, &TVM_STATUS_IDS::CARD_RW_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetCardRWStatus();
		lpMsg++;
		// SAM卡2状态
		memcpy(lpMsg, &TVM_STATUS_IDS::CARD_RW_SAM1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam2Status();// 票卡读写器SAM卡状态
		lpMsg++;
		// token读卡器
		memcpy(lpMsg, &TVM_STATUS_IDS::TOKEN_RW_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetTokenRWStatus();
		lpMsg++;
		// SAM卡3状态
		memcpy(lpMsg, &TVM_STATUS_IDS::TOKEN_RW_SAM1_STATUS, 3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetSam3Status();// Token读卡器SAM卡状态
		lpMsg++;*/
		
		////41 现金安全门
		//memcpy(lpMsg,&TVM_STATUS_IDS::SECURITY_DOOR_STATUS,3);
		//lpMsg += 3;
		//*lpMsg = theTVM_STATUS_MGR.GetSecurityDoorStatus();
		//lpMsg++;


	
		// 震动传感器
		/*memcpy(lpMsg,&TVM_STATUS_IDS::TILT_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetTiltStatus();
		lpMsg++;*/
		// 断线天数
		/*memcpy(lpMsg,&TVM_STATUS_IDS::OFFLINE_DAYS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetOffLineDays();
		lpMsg++;*/
		// 授权访问
		/*memcpy(lpMsg,&TVM_STATUS_IDS::AUTHORIZED_STATUS,3);
		lpMsg += 3;
		*lpMsg = theTVM_STATUS_MGR.GetAuthorizedAccessStatus();
		lpMsg++;*/
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     生成单状态的全状态电文
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTVMFullStatus(TVM_STATUS_ID bomStatusID, BYTE bValue,LPBYTE lpByte)
{
	LPBYTE lpMsg= lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_ALL_STATUS_DATA);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;
	//状态数量
	*lpMsg = 0x01;
	lpMsg+=2;
	// 3 1BIN+1BIN+1BIN 变更状态ID
	memcpy(lpMsg, &bomStatusID, 3);
	lpMsg += 3;
	// 1 BIN 变更状态值
	*lpMsg = bValue;
	lpMsg++;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     生成BOM状态变更电文
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceStatusChange(LPBYTE lpByte, TVM_STATUS_ID bomStatusID, BYTE bValue)
{
	LPBYTE lpMsg = lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_STATUS_CHANGE_DATA);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;
	// 3 1BIN+1BIN+1BIN 变更状态ID
	memcpy(lpMsg, &bomStatusID, 3);
	lpMsg += 3;
	// 1 BIN 变更状态值
	*lpMsg = bValue;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     生成TVM异常状态电文
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode)
{
	LPBYTE lpMsg = lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_DEVICE_ERROR_NOTIC);
	MakeStatusComHdr(comHeader,lpMsg);
	lpMsg+=LEN_STATUS_HEADER;

	// 3 BIN + BIN + BIN 状态ID
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x01;
	lpMsg++;
	*lpMsg = 0x0B; 
	lpMsg++;
	// 4 1BIN+1BIN+1BIN+1BIN 故障ID
	// MTC Code
	ComRevLong(mtcCode, lpMsg);
	lpMsg += 4;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      强制时钟同步(预留)
@param      LPBYTE lpByte        
@retval     无
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
@brief    登录登出请求  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeLoginOrLogoutRequest(LOGIN_TYPE LoginType, int nOperatorID, LPBYTE lpPwd, LPBYTE lpByte)
{
	LPBYTE pData = lpByte;

	//控制代码：0x5101。传输时转换成INTEL序, 2个字节。 
	// MSG_REQUEST_LOGIN_OR_LOGOUT = 0x5101;
	*pData = LOBYTE(MSG_REQUEST_LOGIN_OR_LOGOUT);
	pData++;
	*pData++ = HIBYTE(MSG_REQUEST_LOGIN_OR_LOGOUT);

	//请求登录的操作员, 3 BCD
	int2BCD(nOperatorID, (char*)pData, 3);
	pData += 3;

	//请求登录的操作员密码, 3 BCD
	PSTR2BCD((PSTR)lpPwd,6,(char*)pData);
	pData += 3;

	// 4 2BIN+1BIN+1BIN 设备ID
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(pData);
	pData+=4;

	//请求的登录类型。0x00：登录；0x01：登出
	*pData++ = static_cast<int>(LoginType);

	//预留使用，填充0xFF
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
	// 2 BIN 请求代码
	*lpMsg = LOBYTE(MSG_REQUEST_PERSONALIZATION);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_PERSONALIZATION);
	lpMsg++;
	// 61	ASCII	Name of Cardholder 持卡人姓名
	memcpy(lpMsg, query.cardHolderName, 61);
	lpMsg+=61;
	// 4	BCD		Date of birth of cardholder
	Date2BCD(query.dateOfBirth, lpMsg);
	lpMsg+=4;
	// 21	ASCII	持卡人日间电话
	memcpy(lpMsg, query.cardholderPhoneDay, 21);
	lpMsg+=21;
	// 21	ASCII	持卡人家庭电话
	memcpy(lpMsg, query.cardholderPhoneHome, 21);
	lpMsg+=21;
	// 16	ASCII	持卡人密码
	memcpy(lpMsg, query.cardholderPassword, 16);
	lpMsg+=16;
	// 30	ASCII	关于持卡人的员工ID
	memcpy(lpMsg, query.staffID, 30);
	lpMsg+=30;
	// 19	ASCII	持卡人提供的个人身份
	memcpy(lpMsg, query.cardholderNumber, 19);
	lpMsg+=19;
	// 1	BIN		定义持卡人编号中使用的个人身份类型
	*lpMsg = query.cardholderNumberType;
	lpMsg++;
	// 4	BIN		Defines the card logic serial number for the passenger.
	memcpy(lpMsg, &query.cardSerialNumber, 4);
	lpMsg+=4;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      历史信息查询

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeHistoryInfoRequest(HistoryInfoQuery& query,LPBYTE lpByte,DWORD& lenth)
{
	LPBYTE lpMsg = lpByte;
	lenth =0;
	// 2 BIN 请求代码
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
@brief      即时生效版本请求
@param      LPBYTE lpByte    
WORD wVersionID  版本类型
@retval     无
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
@brief      组成连接认证请求电文
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeConnectAuthRequest(LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	*lpMsg = LOBYTE(MSG_REQUEST_CONNECT_AUTH);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_CONNECT_AUTH);
	lpMsg++;
	// 4 1HEX+2BCD+1HEX 设备ID
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
	lpMsg+=4;
	// 4 BIN IP地址
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
	// 预留
	memset(lpMsg, 0xFF, 3);
	lpMsg += 3;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     TVM/AVM 招援按钮请求数据
@param     LPBYTE lpByte    拼装电文
@param     bool bIsCallOrCancel false:请求招援 
								true :取消招援
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeCallHelpRequest(LPBYTE lpByte,bool bIsCallOrCancel){
	LPBYTE lpMsg = lpByte;
	// 请求代码
	*lpMsg = LOBYTE(MSG_REQUEST_TVM_HELP);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_TVM_HELP);
	lpMsg++;
	
	// 反馈确认类型
	//bIsCallOrCancel ? *lpMsg = 0x01: *lpMsg = 0x00;
	memset(lpMsg,0x00,2);
	lpMsg += 2;
	
	// 招援标识：时间
	_DATE_TIME curTime = ComGetCurTime();
	curTime.Serialize(lpMsg);
	lpMsg += 7;

	// 预留
	//BYTE btReserve[5];
	memset(lpMsg,0xFF,5);
	lpMsg += 5;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     参数和软件版本信息
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeParamAndSoftwareVersionInfo(LPBYTE lpByte,int &length)
{
	LPBYTE lpMsg;
	lpMsg = lpByte;
	// 反馈代码	2	BIN
	*lpMsg = LOBYTE((short)MSG_PARAMETER_ID_DATA);
	lpMsg++;
	*lpMsg = HIBYTE((short)MSG_PARAMETER_ID_DATA);
	lpMsg++;
	// 车站ID
	WORD stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
	char chStationCode[2] = {0};
	int2BCD(stationCode,&chStationCode[0],2);
	*lpMsg = chStationCode[0];//HIBYTE(stationCode);
	lpMsg++;
	*lpMsg = chStationCode[1];//LOBYTE(stationCode);
	lpMsg++;
	// 设备ID
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
	lpMsg+=4;

	// 参数文件数量	2 BIN
	WORD wProviderID = 0x001D;
	unsigned short usVersionNum = 0;
	LPBYTE lpVersionNum = lpMsg;
	lpMsg += 2;

	// 参数
	std::map<PARAMETER_ID,CParameterMaster*> mapVersionProvider = thePARAM_HELPER.GetVersionProvider();

	typedef std::map<PARAMETER_ID,CParameterMaster*>::iterator iterator;
	for(iterator iter = mapVersionProvider.begin();iter!=mapVersionProvider.end();iter++)
	{
		WORD parameID = iter->first;
		// ----------------生效版本--------------------
		// 参数ID
		*lpMsg = HIBYTE(parameID);
		lpMsg++;
		*lpMsg= LOBYTE(parameID);
		lpMsg++;
		// 供应商ID:0x0011代表方正
		memcpy(lpMsg, &wProviderID, 2);
		lpMsg += 2;
		// 软件批次号
		memset(lpMsg, 0, 2);
		lpMsg += 2;
		// 版本类型
		*lpMsg=iter->second->GetCurVerType();
		lpMsg++;
		// 版本号
		int iVersion = iter->second->GetCurMasterVer();
		memcpy(lpMsg, &iVersion, 4);
		lpMsg += 4;
		// 生效日期
		Date2BCD(iter->second->GetCurStartDate(), lpMsg);
		lpMsg += 4;
		usVersionNum++;// 文件数量+1

		// --------------  待用版本--------------------
		// 数据内容同上
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
	// 程序
	PARAMETER_ID programIDs[]={AFC_TVMPROGRAM_ID,	// TVM 程序
								AFC_TPUPROGRAM_ID,	// TPU程序
								//AFC_TVMGUI_ID,		// GUI文件（其实为0x9700）
								//ACC_STATION_MAP_ID, // 线路站点参数（当作程序处理）
								AFC_TVMTHPROG_ID,	// TH应用程序
	};
	CVersionInfo::_PROG_TYPE programsTypes[]={
		CVersionInfo::MC,				// TVM程序
		CVersionInfo::CARD_RW_APP,		// TPU程序
		//CVersionInfo::CHARGE_RW_MAIN,	// TPU主程序
		//CVersionInfo::VOICE,			// 声音文件
		//CVersionInfo::GUI,				// GUI文件
		//CVersionInfo::STATION_MAP,		// 线路站点参数（当作程序处理）
		//CVersionInfo::TEMPLATE			// 模板文件
		CVersionInfo::TH_APP,			// TH应用程序
	};
	//bool isChargeRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CHARGE_RW_APP) == 0;
	//bool isCardRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CARD_RW_APP) == 0;
	//bool isTokenRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TOKEN_RW_APP) == 0;
	/*if(!isChargeRWAPPFailed && !isCardRWAPPFailed && !isTokenRWAPPFailed)
	{
		if(theSETTING.GetCardRWUpdateFlag() == 1)//内部更新失败上内部。
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


/*-------------------------------------以下保留-------------------------------------------*/
//////////////////////////////////////////////////////////////////////////
/**
@brief      操作完成通知
@param      LPBYTE lpByte  
@param      NOTICE_TYPE type 通知类型 
@retval     无
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
@brief     废票回收交易（预留）
@param     BOM_REFUND_TVM   BOM对TVM故障退款消息结构体
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeRecycleInvalidTicket(AfcSysHdr_t & afcHeader,INVALIDE_TICKET_RECYCLETRADE& recycleTrade, LPBYTE lpMsg)
{
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;
	MakeTransComHdr(recycleTrade.transComHdr,lpMsg);
	lpMsg += LEN_TXN_DATATYPE;
	// 4 BIN 卡序列号
	memcpy(lpMsg, &recycleTrade.cardSerialNo, 8);
	lpMsg += 8;
	// 1 BIN 卡物理类型
	*lpMsg = recycleTrade.cardPhysicalType;
	lpMsg++;
	// 4 BIN 卡发行商
	memcpy(lpMsg, &recycleTrade.issueId, 4);
	lpMsg += 4;
	// 2 BIN 车票产品类型
	memcpy(lpMsg, &recycleTrade.productType, 2);
	lpMsg += 2;
	// 1 废卡原因
	*lpMsg = recycleTrade.reson;
	lpMsg++;
	// 废票箱ID
	recycleTrade.magazineId.Serialze(lpMsg);
	lpMsg+=4;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      票箱交换
@param      LPBYTE lpByte        
@retval     无
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
		// 1 BIN 操作类型
		*lpMsg = magazine_change.operType;
		lpMsg++;
		// 4 1BCD+1BIN+2BIN 票箱ID
		magazine_change.magazineId.Serialze(lpMsg);
		lpMsg+=4;
		// 1 BIN 票箱位置ID
		*lpMsg = magazine_change.magazne;
		lpMsg++;
		// 1 BIN 票卡物理类型
		*lpMsg = magazine_change.cardType;
		lpMsg++;
		// 4 BIN 票卡发行商ID
		memcpy(lpMsg, &magazine_change.providerId, 4);
		lpMsg += 4;
		// 2 BIN 车票产品种类
		*lpMsg++ = HIBYTE(magazine_change.cardProductType);
		*lpMsg++ = LOBYTE(magazine_change.cardProductType);
		//memcpy(lpMsg, &magazine_change.cardProductType, 2);
		//lpMsg += 2;
		// 1 BIN 预赋值属性
		*lpMsg = magazine_change.preProperty;
		lpMsg++;
		// 1 BIN 衍生产品ID
		*lpMsg = magazine_change.childproductId;
		lpMsg++;
		// 2 BIN 票卡数量
		memcpy(lpMsg, &magazine_change.CardCount, 2);
		lpMsg += 2;
		// 1 BIN 票箱状态
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
@brief      拼装设备部件构成数据

@param      (i)OperComHdr_t& operComHdr  业务数据专用头结构
@param      (o)LPBYTE lpByte             部件构成数据指针

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceComponentInfo(OperComHdr_t& operComHdr, LPBYTE lpByte)
{
	try{
		// 取得各部件ID
		CCTLOGDeviceInfo::DEVICE_INFO deviceInfo;
		theDeviceInfo.GetDeviceInfo(deviceInfo);
		LPBYTE lpMsg = NULL;
		lpMsg = lpByte;
		// 拼装业务数据头
		MakeOperComHdr(operComHdr, lpMsg);
		lpMsg += LEN_BUSINESS_HEADER;
		// 设备部件总数量
		*lpMsg = COUNT_DEVICE_COMPONET;
		lpMsg+=2;

		// 发票模块
		//*lpMsg = DEVICE_TICKET_MODULE;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.TH_DEVICE_ID, 6);
		//lpMsg += 6;

		// 外部读写器
		//*lpMsg = DEVICE_OUT_TPU;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.RW_DEVICE_ID, 6);
		//lpMsg += 6;

		//// 内部读写器
		//*lpMsg = DEVICE_INNER_TPU;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.TW_DEVICE_ID, 6);
		//lpMsg += 6;

		// 打印机
		*lpMsg = 0x01;//DEVICE_PRINTER;zhengxianle
		lpMsg++;
		memcpy(lpMsg, deviceInfo.PRT_DEVICE_ID, 6);
		lpMsg += 6;

		// 外部读写器一票通ISAM
		//*lpMsg = ACC_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ACC_RW_ISAM, 6);
		//lpMsg += 6;

		// 外部读写器一票通PSAM	
		*lpMsg = ACC_PSAM_RW;
		lpMsg++;
		memcpy(lpMsg, deviceInfo.ACC_RW1_PSAM, 6);
		lpMsg += 6;

		// 外部读写器一卡通ISAM	
		//*lpMsg = ECT_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_RW_ISAM, 6);
		//lpMsg += 6;

		// 外部读写器一卡通PSAM	
		//*lpMsg = ECT_PSAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_RW_PSAM, 6);
		//lpMsg += 6;

		// 内部读写器一票通ISAM
		//*lpMsg = ACC_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ACC_TH_ISAM, 6);
		//lpMsg += 6;

		// 内部读写器一票通PSAM	
		//*lpMsg = ACC_PSAM_TW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ACC_TH_PSAM, 6);
		//lpMsg += 6;

		// 内部读写器一卡通ISAM	
		//*lpMsg = ECT_ISAM_RW;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.ECT_TH_ISAM, 6);
		//lpMsg += 6;

		//// 内部读写器一卡通PSAM	
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
@brief      票箱库存统计
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen)
{
	// 不管箱子在位不在位，都需要报告，卸载状态ID为FFFFFFFF,数量为0
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// Card
	CTCardCountInfo::CARD_BOX_INFO cardBoxA		  = theCARD_COUNT.GetCardboxAInfo();
	CTCardCountInfo::CARD_BOX_INFO cardBoxB		  = theCARD_COUNT.GetCardboxBInfo();
	CTCardCountInfo::CARD_BOX_INFO cardWasted	  = theCARD_COUNT.GetCardboxWastedInfo();

	// 设备存票总量
	WORD totalTicketCount = cardBoxA.ulCurCount + cardBoxB.ulCurCount + cardWasted.ulCurCount;
	memcpy(lpMsg,&totalTicketCount,2);
	lpMsg += 2;

	// 票箱数量
	*lpMsg = 0x03;
	lpMsg += 1;

	WORD count = 0;
	// 票箱A
	cardBoxA.ticketBoxID.Serialze(lpMsg,true);
	lpMsg += 4;

	*lpMsg = MAGAZINEB;
	lpMsg += 1;

	count = cardBoxA.ulCurCount;
	memcpy(lpMsg,&count,2);
	lpMsg += 2;

	// 票箱B
	cardBoxB.ticketBoxID.Serialze(lpMsg,true);
	lpMsg += 4;

	*lpMsg = MAGAZINEA;
	lpMsg += 1;

	count = cardBoxB.ulCurCount;
	memcpy(lpMsg,&count,2);
	lpMsg += 2;

	// 废票箱
	cardWasted.ticketBoxID.Serialze(lpMsg,true);
	lpMsg += 4;

	*lpMsg++ = MAGAZINEC;

	count = cardWasted.ulCurCount;
	memcpy(lpMsg,&count,2);
	lpMsg += 2;

	// 数据长度
	dataLen = lpMsg - lpByte;
	//// 不管箱子在位不在位，都需要报告，卸载状态ID为FFFFFFFF,数量为0
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

	//// 设备存票总量
	//memcpy(lpMsg,&totalTicketCount,2);
	//lpMsg += 2;

	//// 票箱数量
	//*lpMsg = boxCount;
	//lpMsg += 1;

	//WORD count = 0;
	//if(bCollectedBoxExist){
	//	// 回收箱
	//	collectTokenBoxInfo.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEF;
	//	lpMsg += 1;
	//	count = collectTokenBoxInfo.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//if(bInvalidBoxExist){
	//	// 废票箱
	//	invalidTokenBoxInfo.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEA;
	//	lpMsg += 1;
	//	count = invalidTokenBoxInfo.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//if(bBoxAExist){
	//	// 票箱A
	//	token_boxA_info.tokenBoxID.Serialze(lpMsg);
	//	lpMsg += 4;
	//	*lpMsg = MAGAZINEC;
	//	lpMsg += 1;
	//	count = token_boxA_info.ulCurCount;
	//	memcpy(lpMsg,&count,2);
	//	lpMsg += 2;
	//}

	//if(bBoxBExist){
	//	// 票箱B
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

	//// 数据长度
	//dataLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      密码修改请求(预留)
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReformPasswordRequest(CHANGE_STAFFPASSWORD& staffPasswd, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	// 2 BIN 请求代码
	*lpMsg = LOBYTE(MSG_REQUEST_REFORM_PASSWORD);
	lpMsg++;
	*lpMsg = HIBYTE(MSG_REQUEST_REFORM_PASSWORD);
	lpMsg++;
	theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
	lpMsg+=4;
	// 3 BCD 操作员ID
	int2BCD(_ttoi(theAPP_SESSION.GetUserInfo().sUserID), (char*)lpMsg, 3);
	lpMsg += 3;
	// 3 BCD 修改操作员ID
	int2BCD(staffPasswd.StaffID, (char*)lpMsg, 3);
	lpMsg += 3;
	// 3 BCD 旧密码
	PSTR2BCD((PSTR)staffPasswd.ciperOldPwd,6,(char*)lpMsg);
	lpMsg += 3;
	// 3 BCD 新密码
	PSTR2BCD((PSTR)staffPasswd.ciperNewPwd,6,(char*)lpMsg);
	lpMsg += 3;
	// 1 BIN 预留
	*lpMsg = 0xFF;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      钱箱库存报告

@param      OperComHdr_t& operComHdr (事件数据头部)
            LPBYTE lpByte			（数据体）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReportMoneyBoxCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dwMsgLen){
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	//----------------------------------只报告在位的箱子状态--------------------------------

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

	// 设备存币总金额
	memcpy(lpMsg,&totalMoneyAmount,4);
	lpMsg += 4;
	// 钱箱个数
	*lpMsg = boxCount;
	lpMsg += 1;

	//-----------------钱箱一级循环---------------------------------------------------------
	WORD wdMoney = 0;
	// *************************************纸币箱******************************************  钱箱个数 1
	if(bMoneyBoxExist){
		// 钱箱ID
		bankBox.banknoteBoxID.Serialze(lpMsg);
		lpMsg  += 4;
		// 钱箱位置
		*lpMsg = MAGAZINEA;
		lpMsg += 1;

		// 币种总数
		*lpMsg = 0x06;
		lpMsg += 1;
		//---------------  币种二级循环------------------------ 共六种
		// 币种代码及数量
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

	// *************************************纸币箱找零箱************************************** 钱箱个数 5
	// 四个循环箱当做一个箱子，ID规则：线路 + 箱子类型 + 站点编码 + 机器编码
	// 钱箱位置
	{
		// 钱箱ID
		recycleBoxA.banknoteBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// 钱箱位置
		*lpMsg = MAGAZINEI;
		lpMsg += 1;
		// 币种总数
		*lpMsg = 0x02;
		lpMsg += 1;
		//---------------  币种二级循环------------------------ 共六种
		// 币种代码及数量
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

	// ---------------纸币找零箱-------------
	if(changeBox.banknoteBoxID.ToString() != INVALID_ID){
		// 钱箱ID
		changeBox.banknoteBoxID.Serialze(lpMsg);
		lpMsg += 4;

		// 钱箱位置
		*lpMsg = MAGAZINEH;
		lpMsg += 1;

		// 币种总数
		*lpMsg = 0x01;
		lpMsg += 1;

		*lpMsg = TranBanknoteToType(theTVM_SETTING.GetBNRChangeBoxAmount());
		lpMsg += 1;

		wdMoney = changeBox.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}
	// ---------------纸币找废币箱-------------
	//if(bankInvalid.banknoteBoxID.ToString() != INVALID_ID){
	//	// 钱箱ID
	//	bankInvalid.banknoteBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// 钱箱位置
	//	*lpMsg = BANKNOTE_INVALID_POS;
	//	lpMsg += 1;

	//	// 币种总数
	//	*lpMsg = 0x01;
	//	lpMsg += 1;

	//	*lpMsg = TranBanknoteToType(theTVM_INFO.GetBHChangeBoxBType());
	//	lpMsg += 1;

	//	wdMoney = bankInvalid.ulCurCount;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;
	//}

	////*************************************硬币箱****************************************** 钱箱个数 4个
	//// ---------------找零箱1-------------
	//if(addBox1.coinBoxID.ToString() != INVALID_ID){
	//	// 钱箱ID
	//	addBox1.coinBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// 钱箱位置
	//	*lpMsg = COIN_CHANGE_BOX_A_POS;
	//	lpMsg += 1;

	//	// 币种数量
	//	*lpMsg = 0x01;
	//	lpMsg += 1;

	//	// 币种代码
	//	*lpMsg = TranCoinType(theTVM_INFO.GetCHChangeBoxAType());
	//	lpMsg += 1;

	//	// 币种数量
	//	wdMoney = addBox1.ulCount;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;
	//}
	//

	//// ---------------找零箱2-------------
	//if(addBox2.coinBoxID.ToString() != INVALID_ID){
	//	// 钱箱ID
	//	addBox2.coinBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// 钱箱位置
	//	*lpMsg = COIN_CHANGE_BOX_B_POS;
	//	lpMsg += 1;

	//	// 币种数量
	//	*lpMsg = 0x01;
	//	lpMsg += 1;

	//	// 币种代码
	//	*lpMsg = TranCoinType(theTVM_INFO.GetCHChangeBoxBType());
	//	lpMsg += 1;

	//	// 币种数量
	//	wdMoney = addBox2.ulCount;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;
	//}
	

	// ---------------回收箱1-------------
	if(collBox1.coinBoxID.ToString() != INVALID_ID){
		// 钱箱ID
		collBox1.coinBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// 钱箱位置
		*lpMsg = MAGAZINEG;
		lpMsg += 1;
		// 币种数量
		*lpMsg = 0x01;
		lpMsg += 1;
		// 硬币类型1
		*lpMsg = Coin1Yuan;
		lpMsg += 1;
		// 硬币数量1
		wdMoney = collBox1.ulCount_1yuan;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
		/*
		// 硬币类型2
		*lpMsg = CoinHalfYuan;
		lpMsg += 1;

		// 硬币数量2
		wdMoney = collBox1.ulCount_5jiao;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;*/
	}
	

	//// ---------------回收箱2-------------
	//if(collBox2.coinBoxID.ToString() != INVALID_ID){
	//	// 钱箱ID
	//	collBox2.coinBoxID.Serialze(lpMsg);
	//	lpMsg += 4;

	//	// 钱箱位置
	//	*lpMsg = COIN_COLL_BOX_POS;
	//	lpMsg += 1;

	//	// 币种数量
	//	*lpMsg = 0x01;//0x02;
	//	lpMsg += 1;

	//	// 硬币类型1
	//	*lpMsg = Coin1Yuan;
	//	lpMsg += 1;

	//	// 硬币数量1
	//	wdMoney = collBox2.ulCount_1yuan;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;

	//	/*
	//	// 硬币类型2
	//	*lpMsg = CoinHalfYuan;
	//	lpMsg += 1;

	//	// 硬币数量2
	//	wdMoney = collBox2.ulCount_5jiao;
	//	memcpy(lpMsg,&wdMoney,2);
	//	lpMsg += 2;*/
	//}
	
	// ---------------找零Hopper1---------
	if(hopper1.coinBoxID.ToString() != INVALID_ID){
		// 钱箱ID
		hopper1.coinBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// 钱箱位置
		*lpMsg = MAGAZINEE;
		lpMsg += 1;
		// 币种数量
		*lpMsg = 0x01;
		lpMsg += 1;

		// 币种代码
		*lpMsg = TranCoinType(theTVM_INFO.GetCHHopper1Type());
		lpMsg += 1;
		// 币种数量
		wdMoney = hopper1.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}

		// ---------------找零Hopper2---------
	if(hopper2.coinBoxID.ToString() != INVALID_ID){
		// 钱箱ID
		hopper2.coinBoxID.Serialze(lpMsg);
		lpMsg += 4;
		// 钱箱位置
		*lpMsg = MAGAZINED;
		lpMsg += 1;
		// 币种数量
		*lpMsg = 0x01;
		lpMsg += 1;
		// 币种代码
		*lpMsg = TranCoinType(theTVM_INFO.GetCHHopper2Type());
		lpMsg += 1;
		// 币种数量
		wdMoney = hopper2.ulCurCount;
		memcpy(lpMsg,&wdMoney,2);
		lpMsg += 2;
	}


	// 缓存找零器总数
	*lpMsg = 0;
	lpMsg += 1;

	// 长度
	dwMsgLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析硬币面值代码 BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CSCMessageMakeHelper::TranCoinTypeToValue(int nType){
	switch (nType) {
	case 1: return 50;
	case 2: return 100;
	//case 3: return 200;
	//case 4: return 500;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币箱币种为无效，但是countInfo记录数量非零"));
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析硬币面值代码 BIN	0：无效，1：0.5MOP，2：1MOP，3：2MOP，4：5MOP

@param      无

@retval     纸币接受类型

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CSCMessageMakeHelper::TranCoinType(int nType){
	switch (nType) {
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	//case 3: return Coin2MOP;
	//case 4: return Coin5MOP;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      将币种代码转为实际的金额（单位：分）

@param      BankNoteAndCoinType_t type 要转换的类型

@retval     DWORD 金额

@exception  无
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
@brief      将币种代码转为实际的金额（单位：分）

@param      int type 要转换的类型

@retval     DWORD 金额

@exception  无
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
@brief      将配置类型转换为纸币代码

@param      int nType;纸币配置类型

@retval     BankNoteAndCoinType_t 纸币类型

@exception  无
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
@brief      硬币箱\纸币箱清空事件

@param      OperComHdr_t& operComHdr (数据头部)
@param      LPBYTE lpByte（数据体）
@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeClearBanknotesOrCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount,WORD& wMsgLen){
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;

	// 总金额
	DWORD dwMoneyCount = 0;
	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
		dwMoneyCount += TranTypeToValue(typeCount->cash_type) * typeCount->cash_count;
	}
	memcpy(lpMsg,&dwMoneyCount,4);
	lpMsg += 4;

	// 清除币种总数
	*lpMsg = vecTypeCount.size();
	lpMsg += 1;

	// 币种代码及数量
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
	//// 找零箱的币种
	//changeBoxAType = TranCoinType(theTVM_INFO.GetCHChangeBoxAType());
	//changeBoxBType = TranCoinType(theTVM_INFO.GetCHChangeBoxBType());
	// 找零箱
	//dwReleseMometCount += changeBoxA.ulCount * TranCoinTypeToValue(theTVM_INFO.GetCHChangeBoxAType());
	//dwReleseMometCount += changeBoxB.ulCount * TranCoinTypeToValue(theTVM_INFO.GetCHChangeBoxBType());

	DWORD dwReleseMoneyAmount = 0;
	BYTE  bReleaseMoneyType = 0;
	// 硬币残存(1元）
	if(hopperA.ulCurCount + hopperB.ulCurCount>0){
		bReleaseMoneyType += 1;
		dwReleseMoneyAmount += hopperA.ulCurCount* 100 + hopperB.ulCurCount * 100;
	}
	// 纸币残存（10元）
	if (recycleBoxA.ulCurCount + recycleBoxD.ulCurCount>0){
		bReleaseMoneyType += 1;
		dwReleseMoneyAmount += recycleBoxA.BankNotesCountAvos() + recycleBoxD.BankNotesCountAvos();
	}
	// 纸币残存（5元）
	if (recycleBoxB.ulCurCount + recycleBoxC.ulCurCount + changeBox.ulCurCount>0){
		bReleaseMoneyType += 1;
		dwReleseMoneyAmount += recycleBoxB.BankNotesCountAvos() + recycleBoxC.BankNotesCountAvos() + changeBox.BankNotesCountAvos();
	}
	// 残存总金额
	memcpy(lpMsg,&dwReleseMoneyAmount,4);
	lpMsg += 4;
	// 无残存
	if(dwReleseMoneyAmount == 0){
		// 残存币种总数
		*lpMsg = 0x00;
		lpMsg += 1;
	}
	else{
		// 残存币种总数
		*lpMsg = bReleaseMoneyType;
		lpMsg += 1;

		// 一元硬币有残存
		if(hopperA.ulCurCount + hopperB.ulCurCount>0){
			WORD num = hopperA.ulCurCount + hopperB.ulCurCount;
			// 币种代码
			*lpMsg = Coin1Yuan;
			lpMsg++;
			// 币种数量
			memcpy(lpMsg,&num,2);
			lpMsg += 2;

			// 残存位置总数
			*lpMsg = hopperA.ulCurCount>0?1:0 + hopperB.ulCurCount>0?1:0;
			lpMsg++;

			// 残存部件类型
			if(hopperA.ulCurCount > 0){
				// 残存部件类型
				*lpMsg = 0x04;	// 硬币模块
				lpMsg++;
				// 残存部件位置
				*lpMsg = MAGAZINEE;
				lpMsg++;
			}
			if(hopperB.ulCurCount > 0){
				// 残存部件类型
				*lpMsg = 0x04;	// 硬币模块
				lpMsg++;
				// 残存部件位置
				*lpMsg = MAGAZINED;
				lpMsg++;
			}
		}

		// 五元纸币有残存
		if(recycleBoxB.ulCurCount + recycleBoxC.ulCurCount>0){
			WORD num = recycleBoxB.ulCurCount + recycleBoxC.ulCurCount + changeBox.ulCurCount;
			// 币种代码
			*lpMsg = Banknote5Yuan;
			lpMsg++;
			// 币种数量
			memcpy(lpMsg,&num,2);
			lpMsg += 2;
			// 残存位置总数
			*lpMsg = (recycleBoxB.ulCurCount+recycleBoxC.ulCurCount>0)?1:0 + changeBox.ulCurCount>0?1:0;
			lpMsg++;
			// 残存部件类型
			if(recycleBoxB.ulCurCount + recycleBoxC.ulCurCount > 0){
				// 残存部件类型
				*lpMsg = 0x05;	// 纸币模块
				lpMsg++;
				// 残存部件位置
				*lpMsg = MAGAZINEI;
				lpMsg++;
			}
			if(changeBox.ulCurCount > 0){
				// 残存部件类型
				*lpMsg = 0x05;	// 纸币模块
				lpMsg++;
				// 残存部件位置
				*lpMsg = MAGAZINEH;
				lpMsg++;
			}
		}

		// 十元纸币有残存
		if(recycleBoxA.ulCurCount + recycleBoxD.ulCurCount>0){
			WORD num = recycleBoxA.ulCurCount + recycleBoxD.ulCurCount;
			// 币种代码
			*lpMsg = Banknote10Yuan;
			lpMsg++;
			// 币种数量
			memcpy(lpMsg,&num,2);
			lpMsg += 2;
			// 残存位置总数
			*lpMsg = (recycleBoxA.ulCurCount+recycleBoxD.ulCurCount>0)?1:0;
			lpMsg++;
			// 残存部件类型
			if(recycleBoxA.ulCurCount + recycleBoxD.ulCurCount > 0){
				// 残存部件类型
				*lpMsg = 0x05;	// 纸币模块
				lpMsg++;
				// 残存部件位置
				*lpMsg = MAGAZINEI;
				lpMsg++;
			}
		}
	}
	
	// 消息长度
	wMsgLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币箱清空事件

@param      OperComHdr_t& operComHdr （数据头部）
@param      LPBYTE lpByte（数据体）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CSCMessageMakeHelper::MakeClearCoinsBoxEvt(OperComHdr_t& operComHdr,LPBYTE lpByte,vector<CASH_TYPE_COUNT>& vecTypeCount){
//	LPBYTE lpMsg = lpByte;
//	MakeOperComHdr(operComHdr,lpMsg);
//	lpMsg += LEN_BUSINESS_HEADER;
//
//	// 总金额
//	DWORD dwMoneyCount = 0;
//	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
//		dwMoneyCount += TranTypeToValue(typeCount->cash_type) * typeCount->cash_count;
//	}
//	*lpMsg = dwMoneyCount;
//	lpMsg += 4;
//
//	// 清除币种总数
//	*lpMsg = vecTypeCount.size();
//	lpMsg += 1;
//
//	// 币种代码及数量
//	for(auto typeCount = vecTypeCount.begin();typeCount != vecTypeCount.end();typeCount++){
//		*lpMsg = typeCount->cash_type;
//		lpMsg += 1;
//		*lpMsg = typeCount->cash_count;
//		lpMsg += 2;
//	}
//}