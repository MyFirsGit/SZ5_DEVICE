#include "StdAfx.h"
#include "SCMessageMakeHelper.h"
#include "AppSession.h"
#include "bomstatusmgr.h"
#include "CTLOGCountInfo.h"
#include "versioninfo.h"
#include "cparametermaster.h"
#include "cectblacklistparam.h"
#include "CAfcStaffPwdParam.h"
#include "CAfcOperatorCodeTableParam.h"
#include "CAfcDeviceTimeParam.h"
#include "CAfcBomOperationParam.h"
#include "CAfcAccessLevelParam.h"
#include "CAfcDeviceCommonParam.h"
#include "caccbusinessparam.h"
#include "CAccServiceFeeParam.h"
#include "CAccLineInfoParam.h"
#include "CAccTicketParam.h"
#include "CAccNotServiceStation.h"
#include "cacctollstationparam.h"
#include "caccsingleblacklistparam.h"
#include "caccsectionblacklistparam.h"
#include "caccbasepriceparam.h"
#include "caccpricelevelparam.h"
#include "caccpriceadjuest.h"
#include "cacctimetypeparam.h"
#include "cacctimedetailparam.h"
#include "cacctimefloatparam.h"
#include "CAccDiscountParam.h"
#include "caccwritelistparam.h"
#include "caccothercitycard.h"
#include "caccovertimeadjuestparam.h"
#include "CTLOGSoftVer.h"
#include "TicketHelper.h"
#include "BomAR.h"
#include "caccdevicecontrolparam.h"
#include "SignCardApplySvc.h"

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
	int2BCD(nOperatorID, (char*)operComHdr.operatorID, sizeof(operComHdr.operatorID));
	operComHdr.dataTime = ComGetBusiDate();
	operComHdr.localType = 0x06;
	WORD StationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
	operComHdr.localAreaID = StationCode;
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
@brief      取得交易数据头

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
TransComHdr_t CSCMessageMakeHelper::AquireTransComHeader(WORD msgType)
{
	TransComHdr_t transComHeader;
	transComHeader.bMsgType = HIBYTE(msgType);
	transComHeader.bMsgSubType = LOBYTE(msgType);
	transComHeader.dtTransDataTime = ComGetCurTime();
	transComHeader.deviceid = theMAINTENANCE_INFO.GetCurrentDevice();
	transComHeader.dwAfcSerialNo = theCOUNT_INFO.GetTransactionNum();
	return transComHeader;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      AFC公共头

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
AfcSysHdr_t	CSCMessageMakeHelper::AquireAfcSysHeader(WORD wDataLen)
{
	AfcSysHdr_t header;
	int nOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	int2BCD(nOperatorID, (char*)header.OperatorID, sizeof header.OperatorID);
	header.DateTime = ComGetBusiDate();
	header.DataLen = wDataLen;
	header.Reserve = 0xFF;
	return header;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief   状态数据头   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
StatusComHdr_t CSCMessageMakeHelper::AquireStatusComHeader(WORD msgType)
{
	StatusComHdr_t header;
	header.deviceID = theMAINTENANCE_INFO.GetCurrentDevice();
	header.wStationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
	int nOperatorID = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	int2BCD(nOperatorID, header.operatorID, sizeof header.operatorID);
	header.wStatusCode = msgType;
	return header;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

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
	if(theSETTING.GetDeviceTestMode() == 0){
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
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
OperatorComHdr_t CSCMessageMakeHelper::AquireOperatorComHeader()
{
	OperatorComHdr_t header;
	int userId = _ttoi(theAPP_SESSION.GetUserInfo().sUserID);
	int2BCD(userId,(char*)header.operatorID,3);
	header.bomShiftID = theCOUNT_INFO.GetLoginCount();
	return header;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeAfcSysHdr(AfcSysHdr_t &afcSysHder, LPBYTE lpMsg)
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
	*lpMsg = afcSysHder.Reserve;
	lpMsg++;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

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
@brief     票卡公共域 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTicketComHdr(TicketComHdr_t& ticketComHdr,LPBYTE lpMsg)
{
	int2BCD(ticketComHdr.issuerID,(char*)lpMsg,2);
	lpMsg+=2;
	memcpy(lpMsg,ticketComHdr.ticketApplicationSerialNo,sizeof ticketComHdr.ticketApplicationSerialNo);
	lpMsg+=sizeof ticketComHdr.ticketApplicationSerialNo;
	*lpMsg = char2BCD(ticketComHdr.ticketType);
	lpMsg +=1;
	*lpMsg = char2BCD(ticketComHdr.ticketSubType);
	lpMsg +=1;
	*lpMsg = char2BCD(ticketComHdr.ticketStatus);
	lpMsg+=1;
	ComRevWORD(ticketComHdr.ticketChargeTransSerialNo,lpMsg);
	lpMsg+=2;
	ComRevWORD(ticketComHdr.ticketConsumeTransSerialNo,lpMsg);
	lpMsg+=2;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      操作员公共域

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperatorComHdr(OperatorComHdr_t& operatorComHdr,LPBYTE lpMsg)
{
	memcpy(lpMsg,operatorComHdr.operatorID,sizeof operatorComHdr.operatorID);
	lpMsg+= sizeof operatorComHdr.operatorID;
	*lpMsg = char2BCD(operatorComHdr.bomShiftID);
	lpMsg+=1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      安全公共域

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSysSecurityHdr(SysSecurityHdr_t& sysSecurityHdr,LPBYTE lpMsg)
{
	memcpy(lpMsg,&sysSecurityHdr.txnMac,4);
	lpMsg+=4;
	*lpMsg=sysSecurityHdr.keyVersion;
	lpMsg+=1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     生成业务数据专用头
@param     OperComHdr_t      业务数据专用头
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperComHdr(OperComHdr_t &OperComHdr, LPBYTE lpMsg)
{
	// 3 BCD 操作员ID
	memcpy(lpMsg, OperComHdr.operatorID, sizeof(OperComHdr.operatorID));
	lpMsg += sizeof(OperComHdr.operatorID);
	// 4 BCD 发生日期
	Date2BCD(OperComHdr.dataTime, lpMsg);
	lpMsg += 4;
	// 1 BIN 位置类型
	*lpMsg = OperComHdr.localType;
	lpMsg++;
	// 2 BIN 位置ID
	*lpMsg = HIBYTE(OperComHdr.localAreaID);
	lpMsg++;
	*lpMsg = LOBYTE(OperComHdr.localAreaID);
	lpMsg++;
	// 4 2BIN+1BIN+1BIN 设备ID
	OperComHdr.deviceID.Serialize(lpMsg);
	lpMsg+=4;
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
@brief   拼接交易数据头   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTransComHdr(TransComHdr_t& transComHdr,LPBYTE lpMsg)
{
	// 4 2BIN+1BIN+1BIN 设备ID
	// 2 BIN 位置ID
	transComHdr.deviceid.Serialize(lpMsg);
	lpMsg+=4;
	// 4 BIN AFC数据流水号
	memcpy(lpMsg, &transComHdr.dwAfcSerialNo, 4);
	lpMsg += 4;
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
/*
@brief      拼接状态头

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeStatusComHdr(StatusComHdr_t& statusComHdr,LPBYTE lpMsg)
{
	memcpy(lpMsg,&statusComHdr.wStatusCode,2);
	lpMsg+=2;
	*lpMsg = HIBYTE(statusComHdr.wStationCode);
	lpMsg++;
	*lpMsg = LOBYTE(statusComHdr.wStationCode);
	lpMsg++;
	statusComHdr.deviceID.Serialize(lpMsg);
	lpMsg+=4;
	memcpy(lpMsg, statusComHdr.operatorID, sizeof(statusComHdr.operatorID));
	lpMsg+=sizeof(statusComHdr.operatorID);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBOMAR(DEVICE_AR_REASON arReason,VARIABLE_DATA& msg)
{
	VARIABLE_DATA content;
	theBomAR.ReadBOMProcessStat(arReason,content);
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
	*lpMsg = HIBYTE(MSG_TRANS_BOM_AR);
	lpMsg+=1;
	*lpMsg = LOBYTE(MSG_TRANS_BOM_AR);
	lpMsg+=1;
	memcpy(lpMsg,content.lpData,content.nLen);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     生成BOM对TVM故障退款
@param     BOM_REFUND_TVM   BOM对TVM故障退款消息结构体
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBOMRefundTVM(AfcSysHdr_t & afcHeader,BOM_REFUND_TVM& bom_refund_tvm, LPBYTE lpMsg)
{
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;
	MakeTransComHdr(bom_refund_tvm.transComHdr,lpMsg);
	lpMsg += LEN_TXN_DATATYPE;
	// 15 1BIN+2BCD+1BIN+7BCD+4BIN 退款凭证编号
	memcpy(lpMsg, bom_refund_tvm.bRefoundNo, 15);
	lpMsg += 15;
	// 4 BIN BOM退款金额
	memcpy(lpMsg,&bom_refund_tvm.dwRefoundAmount,4);          
	lpMsg += 4;     
}
 
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBomNonBusinessIncome(AfcSysHdr_t & afcHeader,TransComHdr_t& transHeader,CString& reason,int amount,CString& memo, LPBYTE lpMsg)
{
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;
	MakeTransComHdr(transHeader,lpMsg);
	lpMsg += LEN_TXN_DATATYPE;
	memcpy(lpMsg,reason.GetBuffer(),reason.GetLength()>20?20:reason.GetLength());
	lpMsg += 20;
	// 4 BIN BOM退款金额
	ComRevLong(amount, lpMsg);          
	lpMsg += 4;     
	memcpy(lpMsg,memo.GetBuffer(),memo.GetLength()>60?60:memo.GetLength());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     非即时退卡申请
@param     BOM_REFUND_TVM   BOM对TVM故障退款消息结构体
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
//void CSCMessageMakeHelper::MakeDelayRefound(DELAY_REFUND delay_refund, LPBYTE lpMsg)
//{
//	try
//	{
//		memset(lpMsg, 0, LEN_TXN_DELAY_REFUND);
//		// AFC系统公共头
//		MakeAfcSysHdr(delay_refund.afcSysHdr, lpMsg);
//		lpMsg += LEN_AFC_SYS_HEADER;
//		// 1BIN+2BCD+1BIN 设备ID
//		*lpMsg = delay_refund.deviceid.bAfcDeviceType;
//		lpMsg++;
//		int2BCD(delay_refund.deviceid.wAfcStationID,(char*)lpMsg,2);
//		lpMsg += 2;
//		*lpMsg = delay_refund.deviceid.bDeviceSeqNoInStation;
//		lpMsg++;
//		// 4 BIN AFC交易流水号
//		ComRevLong(mltol(delay_refund.dwAfcSerialNo), lpMsg);          
//		lpMsg += 4;
//		// 7 BCD 交易产生时间
//		Time2BCD(delay_refund.dateTime, lpMsg);
//		lpMsg += 7;
//		// 1 BIN 数据类型
//		*lpMsg = delay_refund.bMsgType;
//		lpMsg += 1;
//		// 1 BIN 数据子类型
//		*lpMsg = delay_refund.bMsgSubType;
//		lpMsg += 1;  
//		// 4 BIN 卡发行商
//		ComRevLong(mltol(delay_refund.issueId), lpMsg);
//		lpMsg += 4;
//		// 1 BIN 票卡类型
//		*lpMsg = delay_refund.cardType;
//		lpMsg++;
//		// 4 BIN 卡序列号
//		ComRevLong(mltol(delay_refund.cardSerialNo), lpMsg);
//		lpMsg += 4;
//		// 2 BIN 车票产品类型
//		ComRevWORD(mltol(delay_refund.productType), lpMsg);
//		lpMsg += 2;
//		// 20 ASCII 持卡人姓名
//		memcpy(lpMsg, delay_refund.cardHolder, sizeof delay_refund.cardHolder);
//		lpMsg += 20;
//		// 20 ASCII 持卡人电话
//		memcpy(lpMsg, delay_refund.cardHolderPhone, sizeof delay_refund.cardHolderPhone);
//		lpMsg += 20;
//		// 1 BIN 持卡人证件类型
//		*lpMsg = delay_refund.identifyType;
//		lpMsg++;
//		// 20 ASCII 证件号码
//		memcpy(lpMsg, delay_refund.identifyNo, sizeof delay_refund.identifyNo);
//		lpMsg += 20;
//		// 4 BIN 押金
//		ComRevLong(mltol(delay_refund.deposit), lpMsg);
//		lpMsg += 4;
//		// 4 BIN 卡内余额
//		ComRevLong(mltol(delay_refund.remain), lpMsg);
//		lpMsg += 4;
//		// 1 BIN 退卡原因类型
//		*lpMsg = delay_refund.refundReson;
//		lpMsg++;
//		// 4 BIN 退卡收据号码
//		ComRevLong(mltol(delay_refund.receiptNo), lpMsg);
//		lpMsg += 4;
//		// 4 BCD 退卡日期
//		Date2BCD(delay_refund.date, lpMsg);
//		lpMsg += 4;
//	}
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief     废票回收交易
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
		memcpy(lpMsg, &recycleTrade.cardSerialNo, 4);
		lpMsg += 4;
		// 1 BIN 卡物理类型
		*lpMsg = recycleTrade.cardPhysicalType;
		lpMsg++;
		// 4 BIN 卡发行商
		memcpy(lpMsg, &recycleTrade.issueId, 4);
		lpMsg += 4;
		// 2 BIN 车票产品类型
		memcpy(lpMsg, &recycleTrade.productType, 2);
		lpMsg += 2;
		recycleTrade.magazineId.Serialze(lpMsg);
		lpMsg+=4;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      交易审核

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeTransactionAudit(AfcSysHdr_t & afcHeader,TxnAuditData& txnAuditData,VARIABLE_DATA& msg)
{
	int txnAduditContentLength = txnAuditData.txnAuditItems.size() * (11);
	afcHeader.DataLen = LEN_AFC_SYS_HEADER+LEN_TXN_SYSCOM_HDR+1+2+ txnAduditContentLength +LEN_TXN_SERCOM_HDR;	
	msg.lpData = new BYTE[afcHeader.DataLen];
	msg.nLen = afcHeader.DataLen;
	memset(msg.lpData,0,msg.nLen);
	LPBYTE lpMsg = msg.lpData;
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;
	// 系统公共头
	SysComHdr_t sysHdr = AquireSysComHeader(MSG_TRANS_AUDIT);
	MakeSysComHdr(sysHdr,lpMsg);
	lpMsg += LEN_TXN_SYSCOM_HDR;
	// 审计设备类型
	*lpMsg = theMAINTENANCE_INFO.GetCurrentACCDevice().deviceType;
	lpMsg += 1;
	// 审计类型数
	WORD auditItemsCount = txnAuditData.txnAuditItems.size();
	ComRevWORD(auditItemsCount,lpMsg);
	lpMsg += sizeof(auditItemsCount);

	for (TXNAuditItemMap::iterator it = txnAuditData.txnAuditItems.begin();it!=txnAuditData.txnAuditItems.end();it++)
	{
		*lpMsg = it->first.txnType;
		lpMsg += sizeof(it->first.txnType);
		*lpMsg = it->first.txnSubType;
		lpMsg += sizeof(it->first.txnSubType);
		*lpMsg = it->first.ticketType;
		lpMsg += sizeof(it->first.ticketType);
		ComRevLong(it->second.transCount,lpMsg);
		lpMsg += sizeof(it->second.transCount);
		ComRevLong(it->second.transAmount,lpMsg);
		//memcpy(lpMsg,&it->second.transAmount,sizeof(it->second.transAmount));
		lpMsg += sizeof(it->second.transAmount);
	}
	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      记名卡申请

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSignCardApply(AfcSysHdr_t & afcHeader,SignCardApply& apply,LPBYTE lpMsg)
{
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t sysHdr = AquireSysComHeader(MSG_TRANS_SIGNCARD_APPLY);
	MakeSysComHdr(sysHdr,lpMsg);
	lpMsg+=LEN_TXN_SYSCOM_HDR;

	TicketComHdr_t ticketComHdr;
	ticketComHdr.issuerID = 0001;
	MakeTicketComHdr(ticketComHdr,lpMsg);
	lpMsg+=LEN_TXN_TICKETCOM_HDR;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;
	
	*lpMsg = char2BCD(apply.gender);
	lpMsg+=1;
	memcpy(lpMsg,apply.name,sizeof apply.name);
	lpMsg+=sizeof  apply.name;
	*lpMsg = char2BCD(apply.certificateType);
	lpMsg+=1;
	memcpy(lpMsg,apply.certificateID,sizeof apply.certificateID);
	lpMsg+=sizeof apply.certificateID;
	apply.cardValidity.Serialize(lpMsg);
	lpMsg+=7;
	memcpy(lpMsg,apply.phoneNumber,sizeof apply.phoneNumber);
	lpMsg+=sizeof apply.phoneNumber;
	memcpy(lpMsg,apply.email,sizeof apply.email);
	lpMsg+=sizeof apply.email;
	memcpy(lpMsg,apply.address1,sizeof apply.address1);
	lpMsg+=sizeof apply.address1;
	memcpy(lpMsg,apply.address2,sizeof apply.address2);
	lpMsg+=sizeof apply.address2;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      记名卡修改

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeSignCardModify(AfcSysHdr_t & afcHeader,SignCardModify& modify,LPBYTE lpMsg)
{
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;
	SysComHdr_t sysHdr = AquireSysComHeader(MSG_TRANS_SIGNCARD_MODIFY);
	MakeSysComHdr(sysHdr,lpMsg);
	lpMsg+=LEN_TXN_SYSCOM_HDR;
	lpMsg+=LEN_TXN_TICKETCOM_HDR;
	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(modify.gender);
	lpMsg+=1;
	memcpy(lpMsg,modify.name,sizeof modify.name);
	lpMsg+=sizeof  modify.name;
	*lpMsg = char2BCD(modify.certificateType);
	lpMsg+=1;
	memcpy(lpMsg,modify.certificateID,sizeof modify.certificateID);
	lpMsg+=sizeof modify.certificateID;
	modify.cardValidity.Serialize(lpMsg);
	lpMsg+=7;
	memcpy(lpMsg,modify.phoneNumber,sizeof modify.phoneNumber);
	lpMsg+=sizeof modify.phoneNumber;
	memcpy(lpMsg,modify.email,sizeof modify.email);
	lpMsg+=sizeof modify.email;
	memcpy(lpMsg,modify.address1,sizeof modify.address1);
	lpMsg+=sizeof modify.address1;
	memcpy(lpMsg,modify.address2,sizeof modify.address2);
	lpMsg+=sizeof modify.address2;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      挂失

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReportLoss(AfcSysHdr_t& afcHeader,ReportLoss& reportLoss,LPBYTE lpMsg)
{
	// AFC系统公共头
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t sysHdr = AquireSysComHeader(MSG_TRANS_REPORT_LOSS);
	MakeSysComHdr(sysHdr,lpMsg);
	lpMsg+=LEN_TXN_SYSCOM_HDR;

	TicketComHdr_t ticketComHdr;
	ticketComHdr.issuerID = 0001;
	MakeTicketComHdr(ticketComHdr,lpMsg);
	lpMsg+=LEN_TXN_TICKETCOM_HDR;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(reportLoss.gender);
	lpMsg++;
	memcpy(lpMsg,reportLoss.name,sizeof reportLoss.name);
	lpMsg+=20;
	*lpMsg = char2BCD(reportLoss.certificateType);
	lpMsg++;
	memcpy(lpMsg,reportLoss.certificateID,sizeof reportLoss.certificateID);
	lpMsg+=20;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     解挂 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void  CSCMessageMakeHelper::MakeCancelReportLoss(AfcSysHdr_t& afcHeader,CancelReportLoss& cancelReportLoss,LPBYTE lpMsg)
{
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t sysHdr = AquireSysComHeader(MSG_TRANS_CANCEL_REPORT_LOSS);
	MakeSysComHdr(sysHdr,lpMsg);
	lpMsg+=LEN_TXN_SYSCOM_HDR;

	TicketComHdr_t ticketComHdr;
	ticketComHdr.issuerID = 0001;
	MakeTicketComHdr(ticketComHdr,lpMsg);
	lpMsg+=LEN_TXN_TICKETCOM_HDR;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(cancelReportLoss.gender);
	lpMsg++;
	memcpy(lpMsg,cancelReportLoss.name,sizeof cancelReportLoss.name);
	lpMsg+=20;
	*lpMsg = char2BCD(cancelReportLoss.certificateType);
	lpMsg++;
	memcpy(lpMsg,cancelReportLoss.certificateID,sizeof cancelReportLoss.certificateID);
	lpMsg+=20;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      行政处理

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeAdministrationProcess(AfcSysHdr_t& afcHeader,FinanceComHdr_t & finance,AdministrationProcess& process,LPBYTE lpMsg)
{
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t sysHdr = AquireSysComHeader(MSG_TRANS_ADMINISTRATION_PROCESS);
	MakeSysComHdr(sysHdr,lpMsg);
	lpMsg+=LEN_TXN_SYSCOM_HDR;

	TicketComHdr_t ticketComHdr;
	ticketComHdr.issuerID = 0001;
	MakeTicketComHdr(ticketComHdr,lpMsg);
	lpMsg+=LEN_TXN_TICKETCOM_HDR;
	
	*lpMsg=char2BCD(finance.payMethod);
	lpMsg++;
	memcpy(lpMsg,finance.ticketNum,sizeof finance.ticketNum);
	lpMsg+=8;
	ComRevLong(finance.amount,lpMsg);
	lpMsg+=4;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;
	
	*lpMsg = char2BCD(process.type);
	lpMsg++;
	*lpMsg = char2BCD(process.incomeOrOutcome);
	lpMsg++;
	memcpy(lpMsg,process.memo,sizeof process.memo);
	lpMsg+=20;
	
	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeNonImmidatelySurrenderApply(AfcSysHdr_t& afcHeader,NonImmidatelySurrenderApply& apply, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t comHdr = AquireSysComHeader(MSG_BUSSINESS_NONIMMIDATELY_SURRENDER_APPLY);
	comHdr.cityCode = apply.cityCode;
	comHdr.industryCode = apply.industryCode;
	MakeSysComHdr(comHdr,lpMsg);
	lpMsg += LEN_TXN_SYSCOM_HDR;

	MakeTicketComHdr(apply.ticketComHdr,lpMsg);
	lpMsg+=LEN_TXN_TICKETCOM_HDR;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(apply.gender);
	lpMsg++;
	memcpy(lpMsg,apply.name,sizeof apply.name);
	lpMsg += 20;
	*lpMsg = char2BCD(apply.certificateType);
	lpMsg++;
	memcpy(lpMsg,apply.certificateID,sizeof apply.certificateID);
	lpMsg += 20;
	memcpy(lpMsg,apply.billNum,sizeof apply.billNum);
	lpMsg += 10;
	*lpMsg = char2BCD(apply.nouseReason);
	lpMsg++;
	*lpMsg = char2BCD(apply.applyType);
	lpMsg++;
	*lpMsg = char2BCD(apply.reason);
	lpMsg++;
	memcpy(lpMsg,apply.suspendBillNum,sizeof apply.suspendBillNum);
	lpMsg += 50;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeQueryNonImmidatelySurrenderApply(AfcSysHdr_t& afcHeader,QueryNonImmidatelySurrenderApply& queryInfo, LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t comHdr = AquireSysComHeader(MSG_BUSSINESS_NONIMMIDATELY_SURRENDER_APPLY_QUERY);
	comHdr.cityCode = queryInfo.cityCode;
	comHdr.industryCode = queryInfo.industryCode;
	MakeSysComHdr(comHdr,lpMsg);
	lpMsg += LEN_TXN_SYSCOM_HDR;

	MakeTicketComHdr(queryInfo.ticketComHdr,lpMsg);
	lpMsg += LEN_TXN_TICKETCOM_HDR;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg += LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(queryInfo.certificateType);
	lpMsg++;
	memcpy(lpMsg,queryInfo.certificateID,sizeof(queryInfo.certificateID));
	lpMsg += 20;
	memcpy(lpMsg,queryInfo.billNum,sizeof(queryInfo.billNum));
	lpMsg += 10;

	lpMsg += LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeNonImmediatelySurrender(AfcSysHdr_t& afcHeader,NonImmidatelySurrender& refundInfo,LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t comHdr = AquireSysComHeader(MSG_BUSSINESS_NONIMMIDATELY_SURRENDER);
	MakeSysComHdr(comHdr,lpMsg);
	lpMsg += LEN_TXN_SYSCOM_HDR;

	TicketComHdr_t ticketComHdr;
	ticketComHdr.issuerID = 0001;
	memcpy(ticketComHdr.ticketApplicationSerialNo,refundInfo.cardNum,sizeof refundInfo.cardNum);
	MakeTicketComHdr(ticketComHdr,lpMsg);
	lpMsg += LEN_TXN_TICKETCOM_HDR;

	// LEN_TXN_FINCOM_HDR;
	lpMsg += 9;
	ComRevLong(refundInfo.transAmount,lpMsg);
	lpMsg += 4;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg += LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(refundInfo.refundReason);
	lpMsg += 1;
	ComRevLong(refundInfo.depositAmount,lpMsg);
	lpMsg += 4;
	memcpy(lpMsg,refundInfo.billNum,sizeof refundInfo.billNum);
	lpMsg += 10;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeQuerySignCardApplyStatus(AfcSysHdr_t& afcHeader,QuerySignCardApply& apply,LPBYTE lpByte)
{
	LPBYTE lpMsg = lpByte;
	MakeAfcSysHdr(afcHeader, lpMsg);
	lpMsg += LEN_AFC_SYS_HEADER;

	SysComHdr_t sysComHdr = AquireSysComHeader(MSG_BUSSINESS_SIGNCARD_APPLY_QUERY);
	MakeSysComHdr(sysComHdr,lpMsg);
	lpMsg+=LEN_TXN_SYSCOM_HDR;

	TicketComHdr_t ticketComHdr;
	ticketComHdr.issuerID = 0001;
	MakeTicketComHdr(ticketComHdr,lpMsg);
	lpMsg += LEN_TXN_TICKETCOM_HDR;

	OperatorComHdr_t operatorHdr = AquireOperatorComHeader();
	MakeOperatorComHdr(operatorHdr,lpMsg);
	lpMsg+=LEN_TXN_OPERATORCOM_HDR;

	*lpMsg = char2BCD(apply.certificateType);
	lpMsg++;
	memcpy(lpMsg,apply.certificateID,sizeof apply.certificateID);
	lpMsg+=sizeof apply.certificateID;

	lpMsg+=LEN_TXN_SERCOM_HDR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     生成BOM状态电文
@param     MessageType      状态电文类型 
           0   运营模式
		   1   24小时运营模式
		   2   延长运营模式状态
		   3   运营模式 + 24小时运营 + 延长运营模式
		   4   BOM完整状态
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBomFullStatus(STATUS_TYPE statusType, LPBYTE lpData)
{
	try
	{
		LPBYTE lpMsg = lpData;
		StatusComHdr_t comHeader = AquireStatusComHeader(MSG_BOM_ALL_STATUS_DATA);
		MakeStatusComHdr(comHeader,lpMsg);
		lpMsg+=LEN_STATUS_HEADER;
		
		*lpMsg = statusType == STATUS_TYPE_THREESTATUS  ? 3:24;
		lpMsg+=2;
		if(statusType ==STATUS_TYPE_THREESTATUS )
		{
			memcpy(lpMsg, &BOM_STATUS_IDS::RUN_MODE, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetRunMode();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::HOUR24MODE, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.Get24HourMode();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::DELAY_RUN_MODE, 3);
			lpMsg += 3;
			*lpMsg = (theBOM_STATUS_MGR.GetDelayTime()==0)?0x00:0x01;
			lpMsg++;
		}
		else
		{
			memcpy(lpMsg, &BOM_STATUS_IDS::SERVICE_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetServiceStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::RUN_STATUS, 3);
			lpMsg += 3;	
			*lpMsg = theBOM_STATUS_MGR.GetRunStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::WORK_MODE, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetWorkMode();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::RUN_MODE, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetRunMode();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::HOUR24MODE, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.Get24HourMode();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::DELAY_RUN_MODE, 3);
			lpMsg += 3;
			*lpMsg = (theBOM_STATUS_MGR.GetDelayTime()==0)?0x00:0x01;
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::CONNECT_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetCommunicate();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::TIME_DELAY, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetTimeSynStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::LOGIN_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetLoginStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::DEVICE_AUTO_RUN_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetAutoRunStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::MAGAZINEA_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetMagAStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::MAGAZINEA_COUNT, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetMagANumStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::MAGAZINEB_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetMagBStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::MAGAZINEB_COUNT, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetMagBNumStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::CYCLEBOX_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetCycleBoxStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::CYCLEBOX_COUNT, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetCycleBoxNumStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::RW1_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetInnerRWStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::RW2_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetOuterRWStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::SAM1_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetSam1Type();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::SAM2_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetSam2Type();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::SAM3_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetSam3Type();
			lpMsg++;
			//memcpy(lpMsg, &BOM_STATUS_IDS::ISAM1_STATUS, 3);
			//lpMsg += 3;
			//*lpMsg = theBOM_STATUS_MGR.GetSam1Status();
			//lpMsg++;
			//memcpy(lpMsg, &BOM_STATUS_IDS::ISAM2_STATUS, 3);
			//lpMsg += 3;
			//*lpMsg = theBOM_STATUS_MGR.GetSam2Status();
			//lpMsg++;	
			//memcpy(lpMsg, &BOM_STATUS_IDS::ISAM3_STATUS, 3);
			//lpMsg += 3;
			//*lpMsg = theBOM_STATUS_MGR.GetSam3Status();
			//lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::PRINTER_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetPrintStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::DOOR_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetDoorStatus();
			lpMsg++;
			memcpy(lpMsg, &BOM_STATUS_IDS::TH_MODULE_STATUS, 3);
			lpMsg += 3;
			*lpMsg = theBOM_STATUS_MGR.GetTHModuleStatus();
			lpMsg++;
		}
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
@brief     生成单状态的全状态电文
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBomFullStatus(BOM_STATUS_ID bomStatusID, BYTE bValue,LPBYTE lpByte)
{
	LPBYTE lpMsg= lpByte;
	StatusComHdr_t comHeader = AquireStatusComHeader(MSG_BOM_ALL_STATUS_DATA);
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
void CSCMessageMakeHelper::MakeDeviceStatusChange(LPBYTE lpByte, BOM_STATUS_ID bomStatusID, BYTE bValue)
{
	LPBYTE lpMsg = NULL;
	try{
		lpMsg = lpByte;
		StatusComHdr_t comHeader = AquireStatusComHeader(MSG_BOM_STATUS_CHANGE_DATA);
		MakeStatusComHdr(comHeader,lpMsg);
		lpMsg+=LEN_STATUS_HEADER;
       // 3 1BIN+1BIN+1BIN 变更状态ID
	   memcpy(lpMsg, &bomStatusID, 3);
	   lpMsg += 3;
	   // 1 BIN 变更状态值
	   *lpMsg = bValue;
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
@brief     生成BOM异常状态电文
@param     LPBYTE lpByte    拼装电文
@retval    无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeDeviceErrorStatus(LPBYTE lpByte,long mtcCode)
{
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
		StatusComHdr_t comHeader = AquireStatusComHeader(MSG_BOM_ERROR_NOTIC);
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
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
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}

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
/**
@brief      强制时钟同步
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeForceTimerSynchronize(LPBYTE lpDateTime,  LPBYTE lpByte)
{
	try{
		LPBYTE lpMsg = lpByte;
		*lpMsg = LOBYTE(MSG_FORCE_TIME_SYNC);
		lpMsg++;
		*lpMsg = HIBYTE(MSG_FORCE_TIME_SYNC);
		lpMsg++;
		memcpy(lpMsg, lpDateTime, 7);
		lpMsg += 7;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
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

	//请求登录的操作员, 4 BCD
	int2BCD(nOperatorID, (char*)pData, 3);
	pData += 3;

	memcpy(pData, lpPwd, 3);
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
/**
@brief      密码修改请求
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReformPasswordRequest(CHANGE_STAFFPASSWORD& staffPasswd, LPBYTE lpByte)
{
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
		// 2 BIN 请求代码
		*lpMsg = LOBYTE(MSG_REQUEST_REFORM_PASSWORD);
		lpMsg++;
		*lpMsg = HIBYTE(MSG_REQUEST_REFORM_PASSWORD);
		lpMsg++;
		theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
		lpMsg+=4;
		// 4 BCD 操作员ID
		int2BCD(_ttoi(theAPP_SESSION.GetUserInfo().sUserID), (char*)lpMsg, 3);
		lpMsg += 3;
		// 4 BCD 修改操作员ID
		int2BCD(staffPasswd.StaffID, (char*)lpMsg, 3);
		lpMsg += 3;
		// 3 BCD 旧密码
		memcpy(lpMsg, staffPasswd.ciperOldPwd, 3);
		lpMsg += 3;
		// 3 BCD 新密码
		memcpy(lpMsg, staffPasswd.ciperNewPwd, 3);
		lpMsg += 3;
		// 1 BIN 预留
		*lpMsg = 0xFF;
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
@brief      参数和程序
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
long CSCMessageMakeHelper::MakeUpdateParameterAndProgram(OperComHdr_t& operComHdr, CParamHelper::VEC_VERSION_INFO& VecVersionInfo, LPBYTE lpByte)
{
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;

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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
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
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
		MakeOperComHdr(operComHdr, lpMsg);
		lpMsg += LEN_BUSINESS_HEADER;
		// 1BIN 下载数量
		 char count = updates.size();
		memcpy(lpMsg, &count, 1);
		lpMsg += 1;
		for (vector<ParameterAndSoftwareUpdate>::iterator it = updates.begin(); it != updates.end(); it++)
		{
			// 2 BCD 参数和程序ID
			int2BCD(it->parameterID, (char*)lpMsg, 2);
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
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
	LPBYTE lpMsg = NULL;
	try
	{
		int count = 0;
		lpMsg = lpByte;
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ISAM签到签退
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
//void CSCMessageHelper::MakeIsamCheckIn(OperComHdr_t operComHdr, LPBYTE lpByte, ISAM_AUTH isam_auth, BYTE result)
//{
//	LPBYTE lpMsg = NULL;
//	try
//	{
//		lpMsg = lpByte;
//		MakeOperComHdr(operComHdr, lpMsg);
//		lpMsg += LEN_BUSINESS_HEADER;
//		memcpy(lpMsg,isam_auth.samid,6);
//		lpMsg+=6;
//
//		// 4 BIN 参与方ID
//		memcpy(lpMsg, &isam_auth.participantId, 4);
//		lpMsg += 4;
//		// 1 BIN 动作状态
//		*lpMsg = isam_auth.auth_type;
//		lpMsg++;
//		// 1 BIN 认证结果
//		*lpMsg = isam_auth.authResult;
//		lpMsg++;
//		// 4 BIN 授权额度
//		memcpy(lpMsg, &isam_auth.amount, 4);
//		lpMsg += 4;
//		// 7 BCD 授权失效时间
//		byte specialTime[7] = {0xFF};
//		if(memcmp(&isam_auth.datetime,&specialTime,7)==0)//签退时，值为7个FF，不用进行BCD变化。
//		{
//			memcpy(lpMsg, &isam_auth.datetime, 7);
//		}
//		else
//		{
//			Time2BCD(isam_auth.datetime, lpMsg);
//		}
//		lpMsg += 7;
//	}
//	catch(CSysException&) {
//		throw;
//	}
//	catch (...) {
//		throw CInnerException(CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
//	}
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      操作日志
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeOperationLog(OperComHdr_t& operComHdr, OPERATOR_LOG& operLog , LPBYTE lpByte)
{
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
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
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
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
		memcpy(lpMsg, &magazine_change.cardProductType, 2);
		lpMsg += 2;
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
@brief      账户锁定
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeAccountLock(OperComHdr_t& operComHdr, LPBYTE lpByte)
{
	LPBYTE lpMsg = NULL;
	try
	{
		lpMsg = lpByte;
		MakeOperComHdr(operComHdr, lpMsg);
		lpMsg += LEN_BUSINESS_HEADER;
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
@brief      票箱库存统计
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeReportMagazineCount(OperComHdr_t& operComHdr, LPBYTE lpByte,DWORD& dataLen)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	CCTLOGCountInfo::TICKETBOX_INFO & ticketBoxAInfo = theCOUNT_INFO.GetTicketboxAInfo();
	CCTLOGCountInfo::TICKETBOX_INFO & ticketBoxBInfo = theCOUNT_INFO.GetTicketboxBInfo();
	WORD totalTicketCount = ticketBoxAInfo.ulCurCount + ticketBoxBInfo.ulCurCount;
	// 2 BIN 设备存票总量
	memcpy(lpMsg, &totalTicketCount, 2);
	lpMsg += 2;
	bool isTicketBoxAUnSet = ticketBoxAInfo.ticketBoxID == CCTLOGCountInfo::TICKETBOX_ID();
	bool isTicketBoxBUnSet = ticketBoxBInfo.ticketBoxID == CCTLOGCountInfo::TICKETBOX_ID();
	BYTE totalTicketBoxCount = 0;
	totalTicketBoxCount += isTicketBoxAUnSet ? 0:1;
	totalTicketBoxCount += isTicketBoxBUnSet ? 0:1;
	// 1 BIN 票箱数量
	*lpMsg = totalTicketBoxCount;
	lpMsg++;
	if(!isTicketBoxAUnSet)
	{
		ticketBoxAInfo.ticketBoxID.Serialze(lpMsg);
		lpMsg+=4;
		*lpMsg = MAGAZINEA;
		lpMsg+=1;
		WORD count = ticketBoxAInfo.ulCurCount;
		memcpy(lpMsg,&count,2);
		lpMsg+=2;
	}
	if(!isTicketBoxBUnSet)
	{
		ticketBoxBInfo.ticketBoxID.Serialze(lpMsg);
		lpMsg+=4;
		*lpMsg = MAGAZINEA;
		lpMsg+=1;
		WORD count = ticketBoxBInfo.ulCurCount;
		memcpy(lpMsg,&count,2);
		lpMsg+=2;
	}
	dataLen = lpMsg - lpByte;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      BOM审计数据
@param      LPBYTE lpByte        
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCMessageMakeHelper::MakeBomAuditData(OperComHdr_t& operComHdr, PurchaseData& purchaseData,AUDIT_FLAG auditflag, LPBYTE lpdata, DWORD& datalen)
{

	typedef struct _tagTransKey
	{
		int provider;
		char transType;
		char transSubType;
		void Serialize(LPBYTE lpMsg) const
		{
			memcpy(lpMsg,&provider,4);
			memcpy(lpMsg+4,&transType,1);
			memcpy(lpMsg+5,&transSubType,1);
		}
	} TransKey;

	typedef struct _tagTransKeyCompare
	{
		bool operator()(const TransKey& one,const TransKey& another) const 
		{
			if(one.provider<another.provider) return true;
			if(one.provider>another.provider) return false;
			if(one.transType<another.transType) return true;
			if(one.transType>another.transType) return false;
			if(one.transSubType<another.transSubType) return true;
			if(one.transSubType>another.transSubType) return false;
			return false;
		}
	} TransKeyCompare;

	map<TransKey,vector<SCAuditKey>,TransKeyCompare> trans;
	for (PurchaseItemMap::iterator it = purchaseData.purchaseItems.begin();it!=purchaseData.purchaseItems.end();it++)
	{
		TransKey key;
		CString strKey;
		TicketType_t ticketType = it->first.ticketType;
		ProductCategory_t cardCategory;
		theACC_TICKET.GetTicketCategry(ticketType,cardCategory);
		key.provider = 0x01;
		if(it->first.serviceID == ISSUE_SVC && it->first.ticketType!=TICKET_YKT_STORE_VALUE)
		{
			key.transType = 0x01;
			key.transSubType = 0x11;
		}
		else if(it->first.serviceID == ISSUE_SVC && it->first.ticketType == TICKET_YKT_STORE_VALUE)
		{
			key.transType = 0x01;
			key.transSubType = 0x12;
		}
		if(it->first.serviceID == CHARGE_SVC && cardCategory == MULTIRIDE)
		{
			key.transType = 0x01;
			key.transSubType = 0x14;
		}
		else if(it->first.serviceID == CHARGE_SVC && cardCategory != MULTIRIDE)
		{
			key.transType = 0x01;
			key.transSubType = 0x02;
		}
		if(it->first.serviceID == ADJUST_SVC && it->first.flag == ADJUST_KIND)
		{
			key.transType = 0x01;
			key.transSubType = 0x15;
		}
		if(it->first.serviceID == ADJUST_SVC && it->first.flag == UPDATE_KIND)
		{
			key.transType = 0x01;
			key.transSubType = 0x16;
		}
		if(it->first.serviceID == DEFER_SVC)
		{
			key.transType = 0x01;
			key.transSubType = 0x18;
		}
		if(it->first.serviceID == SIGNCARD_APPLY_SVC)
		{
			SERVE_TYPE  serType = (SERVE_TYPE)it->first.flag;
			if(serType == FIRST_APPLY)
			{
				key.transType = 0x02;
				key.transSubType = 0x37;
			}
			else if(serType == CHECK_APPLY)
			{
				key.transType = 0x02;
				key.transSubType = 0x46;
			}
			else if(serType == UPDATA_APPLY)
			{
				key.transType = 0x02;
				key.transSubType = 0x38;
			}
		}
		if(it->first.serviceID == NOBUSINESS_INCOME_SVC)
		{
			key.transType = 0x02;
			key.transSubType = 0x41;
		}
		if(it->first.serviceID == REFUND_SVC)
		{
			key.transType = 0x02;
			key.transSubType = 0x35;
		}
		if(it->first.serviceID == REFUND_APPLY_SVC)
		{
			if(it->first.flag == 0)//申请
			{
				key.transType = 0x02;
				key.transSubType = 0x34;
			}
			else if(it->first.flag == 1)//查询
			{
				key.transType = 0x02;
				key.transSubType = 0x44;
			}
			else if(it->first.flag == 2)
			{
				key.transType = 0x02;
				key.transSubType = 0x35;
			}
		}
		if(key.transType!=0 && key.transSubType!=0)
		{
			trans[key].push_back(it->first);
		}
		
	}

	LPBYTE lpMsg = lpdata;
	MakeOperComHdr(operComHdr,lpMsg);
	lpMsg+=LEN_BUSINESS_HEADER;
	datalen = lpMsg - lpdata;
	// 统计时间
	Date2BCD(purchaseData.businessDate,lpMsg);
	lpMsg+=4;
	// 产生数据原因
	*lpMsg = auditflag;
	lpMsg+=1;
	//数据起始时间
	Time2BCD(purchaseData.auditTimeFrom,lpMsg);
	lpMsg+=7;
	//数据结束时间
	Time2BCD(purchaseData.auditTimeTo,lpMsg);
	lpMsg+=7;
	//收入现金总金额
	memcpy(lpMsg,&(purchaseData.receivableAmount),4);
	lpMsg+=4;
	//实收现金总金额
	memcpy(lpMsg,&(purchaseData.incomeAmount),4);
	lpMsg+=4;
	//支出现金总金额
	int refundAmount = abs(purchaseData.refundAmount);//支出金额填正值
	memcpy(lpMsg,&refundAmount,4);
	lpMsg+=4;
	//回收车票总数量
	short recyleTicketCount = purchaseData.recyleTicketCount;
	memcpy(lpMsg,&recyleTicketCount,2);
	lpMsg+=2;
	//一票通回收已售车票总数量
	short accRecyleTicketCount = purchaseData.accRecyleTicketCount;
	memcpy(lpMsg,&accRecyleTicketCount,2);
	lpMsg+=2;
	//一票通回收已售车票总金额
	int accRecyleTicketAmount = abs(purchaseData.accRecyleTicketAmount);//支出金额填正值
	memcpy(lpMsg,&accRecyleTicketAmount,4);
	lpMsg+=4;
	//一票通故障退款总数量
	short accRefundFaultCount = purchaseData.accRefundFaultCount;
	memcpy(lpMsg,&accRefundFaultCount,2);
	lpMsg+=2;
	//一票通故障退款总金额
	int accRefundFaultAmount = abs(purchaseData.accRefundFaultAmount);	//TVM故障退款正值
	memcpy(lpMsg,&(purchaseData.accRefundFaultAmount),4);
	lpMsg+=4;
	//预留
	memset(lpMsg,0xFF,20);
	lpMsg+=20;
	//交易类型总数
	short transCount = trans.size();
	memcpy(lpMsg,&transCount,2);
	lpMsg+=2;
	
	for(map<TransKey,vector<SCAuditKey>,TransKeyCompare>::iterator iterTrans = trans.begin();iterTrans!=trans.end();iterTrans++)
	{
		//票卡发行商ID，交易类型，交易子类型
		const TransKey & key = iterTrans->first;
		key.Serialize(lpMsg);
		lpMsg+=6;
		//车票产品种类总数
		const vector<SCAuditKey>& transItems = iterTrans->second;
		char transItemCount = transItems.size();
		memcpy(lpMsg,&transItemCount,1);
		lpMsg+=1;
		for(vector<SCAuditKey>::const_iterator iterTranItem = transItems.begin();iterTranItem!=transItems.end();iterTranItem++)
		{
			//车票产品种类
			memcpy(lpMsg,&(iterTranItem->ticketType),2);
			lpMsg+=2;
			const SCAuditValue& iterTranItemValue = purchaseData.purchaseItems[*iterTranItem];
			//交易总数量
			memcpy(lpMsg,&(iterTranItemValue.transCount),4);
			lpMsg+=4;
			int depositAmount = iterTranItemValue.depositAmount;
			int transAmount = iterTranItemValue.transAmount;
			if(iterTranItem->serviceID == REFUND_SVC)
			{
				depositAmount = abs(depositAmount);	//退款和退款申请时押金填正值
				transAmount = abs(transAmount);
			}
			//押金总金额
			memcpy(lpMsg,&depositAmount,4);
			lpMsg+=4;
			//交易总金额
			memcpy(lpMsg,&transAmount,4);
			lpMsg+=4;
			//交易手续费总金额
			memcpy(lpMsg,&(iterTranItemValue.serviceFeeAmount),4);
			lpMsg+=4;
			//卡成本费
			memcpy(lpMsg,&(iterTranItemValue.cardCostAmount),4);
			lpMsg+=4;
			//回收卡总数量
			short recyleCardNum = iterTranItemValue.recyleCardNum;
			memcpy(lpMsg,&recyleCardNum,2);
			lpMsg+=2;
		}
	}
	datalen = lpMsg - lpdata;
	WORD wLen = datalen;
	memcpy(lpdata + INX_BUSINESS_HEADER_LENGTH,&wLen,2);
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
	try{
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
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
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
void CSCMessageMakeHelper::MakeDeviceRegisterData(OperComHdr_t& operComHdr, ACCAR& purchaseAccData,AR_REASON arReson, LPBYTE lpByte,DWORD& dataLen)
{
	LPBYTE lpMsg = lpByte;
	MakeOperComHdr(operComHdr, lpMsg);
	lpMsg += LEN_BUSINESS_HEADER;
	// 产生寄存器数据状态代码
	*lpMsg = arReson;
	lpMsg++;
	unsigned short arlen = 0;
	LPBYTE lpLengthPos = lpMsg;
	// 设备寄存器数据记录总数
	
	lpMsg+=2;

	for (ACCAR::iterator itemIter = purchaseAccData.begin();itemIter!=purchaseAccData.end();itemIter++)
	{
		if(itemIter->first<=500)
		{
			continue;
		}
		*lpMsg = LOBYTE((short)itemIter->first);
		lpMsg++;
		*lpMsg=HIBYTE((short)itemIter->first);
		lpMsg++;
		memcpy(lpMsg,&(itemIter->second),4);
		lpMsg+=4;
		arlen ++;
	}
	*lpLengthPos= LOBYTE(arlen);
	lpLengthPos++;
	*lpLengthPos = HIBYTE(arlen);
	dataLen = lpMsg - lpByte;
	WORD wLen = dataLen;
	memcpy(lpByte + INX_BUSINESS_HEADER_LENGTH,&wLen,2);
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
		//*lpMsg = BOM_DEVICE_TICKET_MODULE;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.TH_DEVICE_ID, 6);
		//lpMsg += 6;

		// 外部读写器
		//*lpMsg = BOM_DEVICE_OUT_TPU;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.RW_DEVICE_ID, 6);
		//lpMsg += 6;

		//// 内部读写器
		//*lpMsg = BOM_DEVICE_INNER_TPU;
		//lpMsg++;
		//memcpy(lpMsg, deviceInfo.TW_DEVICE_ID, 6);
		//lpMsg += 6;

		// 打印机
		*lpMsg = BOM_DEVICE_PRINTER;
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
		*lpMsg = ACC_PSAM_RW;
		lpMsg++;
		memcpy(lpMsg, deviceInfo.ACC_RW2_PSAM, 6);
		lpMsg += 6;

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
		WORD stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
		*lpMsg = HIBYTE(stationCode);
		lpMsg++;
		*lpMsg = LOBYTE(stationCode);
		lpMsg++;
		theMAINTENANCE_INFO.GetCurrentDevice().Serialize(lpMsg);
		lpMsg+=4;

		unsigned short usVersionNum = 0;
		LPBYTE lpVersionNum = lpMsg;
		lpMsg += 2;
		// 参数文件数量 n	2	BIN

		WORD wProviderID = 0x0011;

		

		std::map<PARAMETER_ID,CParameterMaster*> mapVersionProvider;
		//mapVersionProvider[AFC_
		mapVersionProvider[ACC_BUSINESS_ID]=&(theACC_BUSINESS);								// ACC运营参数
		mapVersionProvider[ACC_SERVICEFEE_ID]=&(theACC_SERVICEFEE);							// ACC行政处理手续费参数
		mapVersionProvider[ACC_OVERTIME_UPDATE_ID]=&(theACC_OVERTIMEADJUEST);					// ACC超时补交费用参数
		mapVersionProvider[ACC_DEVICE_ID]=&(theACC_DEVICECONTROL);					// ACC设备控制参数
		mapVersionProvider[ACC_STATION_ID]=&(theACC_LINE);					// ACC车站位置参数
		mapVersionProvider[ACC_CHARGE_STATION_ID]=&(theACC_TOLLSTATION);					// ACC计费站点参数
		mapVersionProvider[ACC_SECTION_ID]=&(theACC_SECTION);					// ACC区段参数
		mapVersionProvider[ACC_CARDATTRIBUTE_ID]=&(theACC_TICKET);					// ACC票卡属性参数
		mapVersionProvider[ACC_BASE_PRICE_TABLE_ID]=&(theACC_BASEPRICE);					// ACC基础票价表参数
		mapVersionProvider[ACC_PRICE_LEVEL_TABLE_ID]=&(theACC_PRICELEVEL);					// ACC票价级别表参数
		mapVersionProvider[ACC_PRICE_ADJUEST_TABLE_ID]=&(theACC_PRICEADJUEST);					// ACC票价调整策略表参数
		mapVersionProvider[ACC_TIME_FLOATING_TABLE_ID]=&(theACC_TIMEFLOAT);					// ACC时间浮动表参数
		mapVersionProvider[ACC_TIME_TYPE_TABLE_ID]=&(theACC_TIMETYPE);						// ACC时间类型表参数
		mapVersionProvider[ACC_TTIME_TYPE_DETAIL_ID]=&(theACC_TIMEDETAIL);					// ACC时间类型明细参数
		mapVersionProvider[ACC_DISSCOUNT_TABLE_ID]=&(theACC_DISCOUNT);					// ACC折扣率表参数
		mapVersionProvider[ACC_NOT_SERVICE_STATION_ID]=&(theACC_NOTSERVSTATION);					// ACC停运车站表
		mapVersionProvider[ACC_WRITE_LIST_ID]=&(theACC_WRITELIST);					// ACC白名单参数
		mapVersionProvider[ACC_OTHER_CITY_CARD_ID]=&(theACC_OTHERCITYCARD);					// ACC异地卡/城市代码对照参数
		mapVersionProvider[ACC_SINGLE_BLACK_LIST_ID]=&(theACC_SINGLEBLACKLIST);					// ACC单条黑名单参数
		mapVersionProvider[ACC_SECTION_BLACK_LIST_ID]=&(theACC_SECTIONBLACKLIST);					// ACC区段黑名单参数
		mapVersionProvider[ACC_ISSUE_COMPANY_ID]=&(theACC_ISSUECOMPANY);					// ACC票卡发行商参数

		//mapVersionProvider[ECT_SHOPBLACKLIST_ID] =&(theECT_BLACKLIST);
		mapVersionProvider[AFC_MODEHISTORY_ID] = &(theAFC_MODERECORD);						//模式履历
		mapVersionProvider[AFC_DEVICECOMMONPARAM_ID] = &(theAFC_COMMON);				// 设备公共参数
		mapVersionProvider[AFC_STAFFPWDPARAM_ID] = &(theAFC_STAFF);						// 设备操作员信息
		mapVersionProvider[AFC_ACCESSLEVELPARAM_ID] = &(theAFC_ACCESS_LEVEL);			// 设备权限参数
		mapVersionProvider[AFC_BOMOPERATIONPARAM_ID] = &(theAFC_BOM_OPERATION);			// BOM运营参数
		mapVersionProvider[AFC_DEVICERUNTIMEPARAM_ID] = &(theAFC_DEV_TIME);				// 设备运行时间参数


		typedef std::map<PARAMETER_ID,CParameterMaster*>::iterator iterator;
		for(iterator iter = mapVersionProvider.begin();iter!=mapVersionProvider.end();iter++)
		{
			WORD parameID = iter->first;
			*lpMsg = HIBYTE(parameID);
			lpMsg++;
			*lpMsg= LOBYTE(parameID);
			lpMsg++;
			memcpy(lpMsg, &wProviderID, 2);
			lpMsg += 2;
			memset(lpMsg, 0, 2);
			lpMsg += 2;
			*lpMsg=iter->second->GetCurVerType();
			lpMsg++;
			int iVersion = iter->second->GetCurMasterVer();
			memcpy(lpMsg, &iVersion, 4);
			lpMsg += 4;
			Date2BCD(iter->second->GetCurStartDate(), lpMsg);
			lpMsg += 4;
			usVersionNum++;
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
		
		PARAMETER_ID programIDs[]={AFC_BOMPROGRAM_ID,AFC_TPUPROGRAM_ID/*,AFC_TPUMAINPROGRAM_ID*/};
		CVersionInfo::_PROG_TYPE programsTypes[]={CVersionInfo::MC,CVersionInfo::RW_APP,CVersionInfo::RW_MAIN};
		bool isRWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP) == 0;
		bool isTWAPPFailed = theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_APP) == 0;
		if(!isRWAPPFailed && !isTWAPPFailed)
		{
 			if(theSETTING.GetRWUpdateFlag(false) == 1)//内部更新失败上内部。
 			{
 				programsTypes[1] = CVersionInfo::TW_APP;
 				programsTypes[2] = CVersionInfo::TW_MAIN;
 			}
		}
		else if(!isTWAPPFailed)
		{
			programsTypes[1] = CVersionInfo::TW_APP;
			programsTypes[2] = CVersionInfo::TW_MAIN;
		}
		CVersionInfo::_VER_TYPE versionTypes[] = {CVersionInfo::CURRENT,CVersionInfo::FUTURE};
		for(int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
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
	try{
		LPBYTE lpMsg = lpData;
		*lpMsg = LOBYTE(MSG_REQUEST_SEND_IMIDIATLY_PARAMETER);
		lpMsg++;
		*lpMsg = HIBYTE(MSG_REQUEST_SEND_IMIDIATLY_PARAMETER);;
		lpMsg++;
		*lpMsg = 0x10;
		lpMsg++;
		*lpMsg = 0x02;
		lpMsg++;
		memcpy(lpMsg, &wVersionID, 4);
		lpMsg += 4;
	}
	catch(CSysException&) {
		throw;
	}
	catch (...) {
		throw CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
}
