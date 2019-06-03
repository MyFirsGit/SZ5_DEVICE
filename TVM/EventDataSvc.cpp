#include "stdafx.h"
#include "EventDataSvc.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include "headermanager.h"
#include "scdatamsg.h"
#include "scmessagemakehelper.h"
#include "ctmessageaudit.h"
#include "serverclientsvc.h"
#include "CTLOGCountInfo.h"
#include "CAfcDeviceCommonParam.h"

#include "ServerClientSvc.h"
#include "tvmdef.h"
#include "scsvcresult.h"
#include "Sync.h"

#define SM_UDSN_RESET (SP_USER + 1100)

BEGIN_MESSAGE_MAP(CEventDataSvc, CBackService)
	ON_SERVICE_TIMER()
	ON_SERVICE_MESSAGE(SM_UDSN_RESET,OnUDSNReset)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      事件数据服务构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CEventDataSvc::CEventDataSvc(void)
	:CBackService(EVENT_MSG_SVC,ROOT_SVC)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CEventDataSvc::~CEventDataSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动时的响应函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::OnStart()
{
	theCOUNT_INFO.CounterReseted.AddHandler(this,&CEventDataSvc::OnCounterReseted);
	StartTimer(PACKAGE_TIMER,300);
	__super::OnStart();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务停止时响应函数

@param      无

@retval     bool 允许停止

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CEventDataSvc::OnStop()
{
	//StopTimer(TICKET_AUDIT_TIMER);
	StopTimer(PACKAGE_TIMER);
	StopTimer(PACKAGE_AUDIT_TIMER);
	theCOUNT_INFO.CounterReseted.RemoveHandler(this,&CEventDataSvc::OnCounterReseted);
	return __super::OnStop();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      开始包审计计时
@param      UINT nTimerID  identifier of a timer
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::StartPackageAuditTimer()
{
	CAfcDeviceCommonParam::COMMUNICATION communication;
	theAFC_COMMON.GetCommunication(communication);
	int seconds = communication.auditInterval*60;
	StartTimer(PACKAGE_AUDIT_TIMER, seconds !=  0 ? seconds: 15*60);
	if (thePEKG_AUDIT.HasStartDateTime() == false)
	{
		_DATE_TIME CurDateTime = ComGetCurTime();
		thePEKG_AUDIT.SetStartDateTime(CurDateTime);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      结束包审计计时
@param      无
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::EndPackageAuditTimer()
{
	StopTimer(PACKAGE_AUDIT_TIMER);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     开启票箱库存报告计时器 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::StartTicketAuditTimer()
{
	CAfcDeviceCommonParam::COMMUNICATION communication;
	theAFC_COMMON.GetCommunication(communication);
	int seconds = communication.magazineInterval*60;
	StartTimer(TICKET_AUDIT_TIMER, seconds!=0 ? seconds :30*60 );

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      关闭票箱库存报告计时器

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::EndTicketAuditTimer()
{
	StopTimer(TICKET_AUDIT_TIMER);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      定时器响应处理
@param      UINT nTimerID  identifier of a timer
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::OnTimer(UINT nTimerID)
{
	if(PACKAGE_AUDIT_TIMER == nTimerID)
	{
		EndPackageAuditTimer();
		try {
			theDEBUG_LOG->WriteData(_T(""), _T(__FUNCTION__), __LINE__, _T("UINT nTimerID = %x <-"),nTimerID);
			SendPackageMsgTransAudit();
			theDEBUG_LOG->WriteData(_T(""), _T(__FUNCTION__), __LINE__, _T("UINT nTimerID = %x ->"),nTimerID);
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...) {
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		StartPackageAuditTimer();
	}
	else if(TICKET_AUDIT_TIMER == nTimerID )
	{
		EndTicketAuditTimer();
		try {
			theDEBUG_LOG->WriteData(_T(""), _T(__FUNCTION__), __LINE__, _T("UINT nTimerID = %x <-"),nTimerID);
			// 发送钱箱及票箱库存
			SendTicketBoxCountReport();
			SendMoneyBoxCountReport();
			theDEBUG_LOG->WriteData(_T(""), _T(__FUNCTION__), __LINE__, _T("UINT nTimerID = %x ->"),nTimerID);
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...) {
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		StartTicketAuditTimer();
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      记数器重置响应函数
@param 记数器类型
@retval     执行结果
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::OnCounterReseted(COUNTER_TYPE counterType)
{
	this->PostMessage(SM_UDSN_RESET,counterType,MAXVALUERESET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      记数器重置消息SM_UDSN_RESET响应函数
@param 记数器类型
@retval     执行结果
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CEventDataSvc::OnUDSNReset(WPARAM wParam,LPARAM lParam)
{
	UDSN_TYPE type = ACC_UDSN;
	switch((COUNTER_TYPE)wParam)
	{
	case COUNTER_TYPE_TRANS_ACC:
		type = ACC_UDSN;
		break;
	case COUNTER_TYPE_TRANS_ECT:
		type = ECT_UDSN;
		break;
	case COUNTER_TYPE_TRANS_AFC:
		type = AFC_UDSN;
		break;
	}
	return SendDeviceUdsnReset(type,(UDSN_RESET_REASON)lParam);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      账户锁定
@param      int		nOperatorID       操作员ID
int		nPosType          位置类型: 0x01线路中心级管理;x02票务中心级管理；0x03维修中心级管理；0x06车站级管理
@retval     执行结果
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendAccountLock(int nOperatorID, BYTE nPosType) 
{
	try{

		SYNC(TXNMANAGER,_T("EVENT"));

		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_ACCOUNT_LOCK,LEN_EVN_ACCOUNT_LOCK);
		int2BCD(nOperatorID, (char *)operComHdr.operatorID, sizeof(operComHdr.operatorID));
		BYTE lpMsg[LEN_EVN_ACCOUNT_LOCK]={0};
		theSCMessageMakeHelper.MakeAccountLock(operComHdr,lpMsg);
		theEVN_TXN_MGR.SaveTxnData(LEN_EVN_ACCOUNT_LOCK, lpMsg);
		theEVN_TXN_MGR.MakePekg();
		return SendEventData();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SCSVC_RESULT_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送UDSN重置命令

@param      UDSN_TYPE  udsnType UDSN类型
@param	UDSN_RESET_REASON重置原因

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendDeviceUdsnReset(UDSN_TYPE  udsnType,UDSN_RESET_REASON resetReason)
{
	try
	{
		SYNC(TXNMANAGER,_T("EVENT"));

		UDSN_RESET udsnReset;
		udsnReset.udsn_type =udsnType;
		udsnReset.beforeResetNo = 0XFFFFFFFF;
		udsnReset.afterResetNo = 0x01;

		BYTE lpMsg[LEN_EVN_UDSN_RESET] = {0};
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_UDSN_RESET,LEN_EVN_UDSN_RESET);
		theSCMessageMakeHelper.MakeDeviceUdsnNoReset(operComHdr, udsnReset, resetReason, lpMsg);
		theEVN_TXN_MGR.SaveTxnData(LEN_EVN_UDSN_RESET, lpMsg);
		theEVN_TXN_MGR.MakePekg();
		return SendEventData();
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
	return SCSVC_RESULT_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送AVM\TVM审计数据

@param   PURCHASE_DATA& purchaseData 审计数据
@param	AUDIT_FLAG auditFlag 审计原因

@retval     long

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendTVMAuditData(PurchaseData& purchaseData)
{
	try
	{
		SYNC(TXNMANAGER,_T("EVENT"));
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_TVM_AUDIT,LEN_EVN_AVM_TVM_AUDIT);
		BYTE lpByte[LEN_EVN_AVM_TVM_AUDIT] = {0};
		theSCMessageMakeHelper.MakeTVMAuditData(operComHdr,purchaseData,lpByte);
		theEVN_TXN_MGR.SaveTxnData(LEN_EVN_AVM_TVM_AUDIT,lpByte);
		return SendEventData();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     包传输审计数据 

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendPackageMsgTransAudit()
{
	try
	{
		SYNC(TXNMANAGER,_T("EVENT"));

		OperComHdr_t operComHdr=theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_PACKAGE_AUDIT,0);
		BYTE lpMsg[RECEIVED_BUFF_SIZE]={0};
		// 获取包传输审计内容
		theSCMessageMakeHelper.MakePackageMsgTransAudit(operComHdr, lpMsg);
		// 保存事件数据
		theEVN_TXN_MGR.SaveTxnData(operComHdr.msgLen, lpMsg);
		//theEVN_TXN_MGR.MakePekg();
		return SendEventData();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送事件数据

@param      无

@retval     long SP_SUCCESS成功

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendEventData()
{
	SYNC(TXNMANAGER,_T("EVENT"));
	theEVN_TXN_MGR.MakePekg();
	if(!theAPP_SESSION.IsSCConnected())
	{
		return SCSVC_RESULT_NOT_CONNECTED;
	}
	while(theEVN_TXN_MGR.HasUnsendData()>0)
	{
		PEKG_DATA dataPekg;		
		bool aquirePekgSuccess = theEVN_TXN_MGR.GetDataPekg(dataPekg);	
		if(!aquirePekgSuccess)
		{
			continue;
		}
		CMD_HEADER header = CHeaderManager::AquireHeader(OPER_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pTickAuditDataMsg(new CSCDataMsg);
		pTickAuditDataMsg->SetHeader(&header);
		pTickAuditDataMsg->SetContent(dataPekg.lpData,dataPekg.nLen);

		long executeResult = theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)pTickAuditDataMsg.get());
		if (executeResult == SP_SUCCESS)
		{
			// 若发送成功设置数据包为已送
			theEVN_TXN_MGR.SetPekgSended(dataPekg.strPekgID);	
			WORD msgCode = MAKEWORD(*(dataPekg.lpData+ LEN_PACKAGE_HEADER + INX_BUSINESS_DATATYPE+1),*(dataPekg.lpData+LEN_PACKAGE_HEADER+INX_BUSINESS_DATATYPE));
			if(msgCode!=MSG_BUSSINESS_PACKAGE_AUDIT)
			{
				CTMessageAudit::PEKG_INFO PekgInfo;
				memcpy(PekgInfo.sPekgId, dataPekg.lpData+1, 14);
				memcpy(&PekgInfo.iRecordCnt,dataPekg.lpData+19, 2);		
				thePEKG_AUDIT.AddWriteFile(PekgInfo);
			}
			// 设置传输审计状态为正常:zhengxianle
			//theTVM_STATUS_MGR.SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS_NORMAL);
		}
		else
		{
			// 设置传输审计状态为警告:zhengxianle
			//theTVM_STATUS_MGR.SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS_WARNING);
			return executeResult;
		}
	}

	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     部件更换（预留）

@param     (i)BYTE     bType      部件类型
@param     (i)LPBYTE   lpNewID    新部件号码
@param     (i)LPBYTE   lpOldID    旧部件号码

@retval     
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendDeviceComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID)                                  
{
	try{
		SYNC(TXNMANAGER,_T("EVENT"));
		BYTE lpMsg[LEN_EVN_DEVICE_COMPONENT_CHANGE] = {0};
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_DEVICE_COMPONENT_CHANGE,LEN_EVN_DEVICE_COMPONENT_CHANGE);
		theSCMessageMakeHelper.MakeDeviceComponentExchange(operComHdr,lpMsg,bType,lpNewID,lpOldID);
		theEVN_TXN_MGR.SaveTxnData(LEN_EVN_DEVICE_COMPONENT_CHANGE, lpMsg);
		theEVN_TXN_MGR.MakePekg();
		return SendEventData();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SCSVC_RESULT_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     SAM更换

@param     (i)BYTE     bType      SAM类型
@param     (i)LPBYTE   lpNewID    新SAM号码
@param     (i)LPBYTE   lpOldID    旧SAM号码

@retval     
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendSamComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID)                                  
{
	try{
		//SYNC(TXNMANAGER,_T("EVENT"));
		//BYTE lpMsg[LEN_EVN_DEVICE_COMPONENT_CHANGE] = {0};
		//OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_SAM_CHANGE,LEN_EVN_DEVICE_COMPONENT_CHANGE);
		//theSCMessageMakeHelper.MakeDeviceComponentExchange(operComHdr,lpMsg,bType,lpNewID,lpOldID);
		//theEVN_TXN_MGR.SaveTxnData(LEN_EVN_DEVICE_COMPONENT_CHANGE, lpMsg);
		//theEVN_TXN_MGR.MakePekg();
		//return SendEventData();
		return 0;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SCSVC_RESULT_INTERNAL_ERROR;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送票箱库存报告

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendTicketBoxCountReport()
{
	try{
		SYNC(TXNMANAGER,_T("EVENT"));
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MAGAZINE_INFO,LEN_EVN_TICKETBOX_INFO);
		BYTE lpMsg[LEN_EVN_TICKETBOX_INFO] = {0};
		DWORD dataLen = 0;
		theSCMessageMakeHelper.MakeReportMagazineCount(operComHdr,lpMsg+LEN_BUSINESS_HEADER,dataLen);
		operComHdr.msgLen = LEN_BUSINESS_HEADER + dataLen;//实际长度
		theSCMessageMakeHelper.MakeOperComHdr(operComHdr, lpMsg);
		theEVN_TXN_MGR.SaveTxnData(operComHdr.msgLen, lpMsg);
		return SendEventData();
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SCSVC_RESULT_INTERNAL_ERROR;	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送钱箱库存报告

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendMoneyBoxCountReport(){
	try{
		//SYNC(TXNMANAGER,_T("EVENT"));
		// TVM
		//if(theMAINTENANCE_INFO.GetDeviceType() == DEVICE_TYPE_TVM){
			BYTE lpMsg[LEN_EVN_TVM_MONEY_BOX] = {0};
			OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MONEYBOX_INFO,LEN_EVN_TVM_MONEY_BOX);
			DWORD dwLen = 0;
			theSCMessageMakeHelper.MakeReportMoneyBoxCount(operComHdr,lpMsg,dwLen);
			memcpy(lpMsg + INX_BUSINESS_HEADER_LENGTH,&dwLen,2);
			theEVN_TXN_MGR.SaveTxnData(dwLen,lpMsg);
		/*}
		else{
			BYTE lpAvmMsg[LEN_EVN_AVM_MONEY_BOX] = {0};
			OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MONEYBOX_INFO,LEN_EVN_AVM_MONEY_BOX);
			theSCMessageMakeHelper.MakeReportMoneyBoxCount(operComHdr,lpAvmMsg);
			theEVN_TXN_MGR.SaveTxnData(LEN_EVN_AVM_MONEY_BOX,lpAvmMsg);
		}	*/
		//theEVN_TXN_MGR.MakePekg();
		return SendEventData();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      TVM纸币箱清空事件

@param      无

@retval     long 错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendClearBankNotesBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount){
	try{
		SYNC(TXNMANAGER,_T("EVENT"));
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MONEYBOX_EMPTY_REPORT,LEN_EVN_CLEAR_BANKNOTES_INFO);
		BYTE lpMsg[LEN_EVN_CLEAR_BANKNOTES_INFO];
		memset(lpMsg,0x00,LEN_EVN_CLEAR_BANKNOTES_INFO);
		WORD wMsgLen = 0;
		theSCMessageMakeHelper.MakeClearBanknotesOrCoinsBoxEvt(operComHdr,lpMsg,vecTypeCount,wMsgLen);
		memcpy(lpMsg + 27,&wMsgLen,2);
		theEVN_TXN_MGR.SaveTxnData(wMsgLen,lpMsg);
		return SendEventData();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送纸币找零箱清空事件

@param      vector<CASH_TYPE_COUNT>& vecTypeCount;清空的纸币箱币种及数量集

@retval     long lRet；错误代码

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendClearBanknotesChangeBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount){
	try{
		SYNC(TXNMANAGER,_T("EVENT"));
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MONEYBOX_EMPTY_REPORT,LEN_EVN_CLEAR_BANKNOTES_CHANGE_BOX_INFO);
		BYTE lpMsg[LEN_EVN_CLEAR_BANKNOTES_CHANGE_BOX_INFO];
		memset(lpMsg,0x00,LEN_EVN_CLEAR_BANKNOTES_CHANGE_BOX_INFO);
		WORD wMsgLen = 0;
		theSCMessageMakeHelper.MakeClearBanknotesOrCoinsBoxEvt(operComHdr,lpMsg,vecTypeCount,wMsgLen);
		memcpy(lpMsg + 27,&wMsgLen,2);
		theEVN_TXN_MGR.SaveTxnData(wMsgLen,lpMsg);
		return SendEventData();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送TVM硬币箱清空事件数据

@param      无

@retval     long 返回错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendClearCoinsBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount){
	try{
		SYNC(TXNMANAGER,_T("EVENT"));
		OperComHdr_t operComHdr = theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_MONEYBOX_EMPTY_REPORT,LEN_EVN_CLEAR_COINS_INFO);
		BYTE lpMsg[LEN_EVN_CLEAR_COINS_INFO];
		memset(lpMsg,0x00,LEN_EVN_CLEAR_COINS_INFO);
		WORD wMsgLen = 0;
		theSCMessageMakeHelper.MakeClearBanknotesOrCoinsBoxEvt(operComHdr,lpMsg,vecTypeCount,wMsgLen);
		memcpy(lpMsg + 27,&wMsgLen,2);
		theEVN_TXN_MGR.SaveTxnData(wMsgLen,lpMsg);
		return SendEventData();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theEXCEPTION_MGR.ProcessException(CInnerException(EVENT_MSG_SVC,CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}