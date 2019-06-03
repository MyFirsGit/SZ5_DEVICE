#include "StdAfx.h"
#include "TranscationTranslateSvc.h"
#include "CAfcDeviceCommonParam.h"
#include "headermanager.h"
#include "ctmessageaudit.h"
#include "scdatamsg.h"
#include "serverclientsvc.h"
#include "scmessagemakehelper.h"

#include "ServerClientSvc.h"
#include "bomdef.h"
#include "scsvcresult.h"
#include "TxnAuditManager.h"
#include "Sync.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/**
@brief 定期传送Service
*/

BEGIN_MESSAGE_MAP(CTransSvc, CBackService)
	ON_SERVICE_TIMER()
END_MESSAGE_MAP()

CTransSvc::CTransSvc(void)
:CBackService(TRANSMISSION_INTERVAL_SVC,ROOT_SVC)
{
}

CTransSvc::~CTransSvc(void)
{
	
}

void CTransSvc::OnStart()
{
	__super::OnStart();
	StartTimer(TIMER_PACKAGE,theBOM_INFO.GetTransactionPackageInterval());
	theACC_TXN_MGR.TxnDataSaved.AddHandler(this,&CTransSvc::SaveTxnAuditData);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTransSvc::OnStop()
{
	theACC_TXN_MGR.TxnDataSaved.RemoveHandler(this,&CTransSvc::SaveTxnAuditData);
	StopTimer(TIMER_PACKAGE);
	StopTimer(TIMER_TRANSMISSION);
	return __super::OnStop();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      定时器响应开启
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::StartTransmissionTimer()
{
	CAfcDeviceCommonParam::COMMUNICATION communication;
 	theAFC_COMMON.GetCommunication(communication);
	DWORD TxnIntervalTime = communication.txnInterval*60;
	theDEBUG_LOG->WriteData(_T("CTransSvc"), _T("StartTransmissionTimer"), __LINE__, _T("CTransSvc = 0x%x"),this);
	theDEBUG_LOG->WriteData(_T("CTransSvc"), _T("StartTransmissionTimer"), __LINE__, _T("TxnIntervalTime = %d"),TxnIntervalTime);
	StartTimer(TIMER_TRANSMISSION, TxnIntervalTime != 0 ? TxnIntervalTime : 60);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      定时器响应处理
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::EndTransmissionTimer()
{
	StopTimer(TIMER_TRANSMISSION);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      定时器响应处理
@param      UINT nTimerID  identifier of a timer
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::OnTimer(UINT nTimerID)
{
	if(nTimerID == TIMER_PACKAGE)
	{
		theACC_TXN_MGR.MakePekg(false);
		theECT_TXN_MGR.MakePekg(false);
		theAFC_TXN_MGR.MakePekg(false);
	}
	if(nTimerID == TIMER_TRANSMISSION)
	{
		EndTransmissionTimer();
		try {
			theDEBUG_LOG->WriteData(_T("CTransSvc"), _T("OnTimer"), __LINE__, _T("UINT nTimerID = %x ->"),nTimerID);
			SendTransactionData();
		}
		catch (CSysException& e) {
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch (...) {
			theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
		}
		StartTransmissionTimer();
	}

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      发送交易数据
@param      无
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTransSvc::SendTransactionData()
{
	try{
		SYNC(SENDDATA,_T("TRANSACTION"));
		long sendAccResult = SendTransactionData(ACC_TRANS);
		if(sendAccResult!=SP_SUCCESS)
		{
			return sendAccResult;
		}
		long sendECTResult = SendTransactionData(ECT_TRANS);
		if(sendECTResult!=SP_SUCCESS)
		{
			return sendECTResult;
		}
		long sendAfcResult = SendTransactionData(AFC_TRANS);
		if(sendAfcResult!=SP_SUCCESS)
		{
			return sendAfcResult;
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(SC_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      发送特定种类的交易数据
@param      DATA_TYPE_CODE 交易类型
@retval     无
@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CTransSvc::SendTransactionData(DATA_TYPE_CODE dataTypeCode)
{
	CTxnDataMgr* txnDataMgr = NULL;
	switch(dataTypeCode)
	{
	case ACC_TRANS:
		txnDataMgr = &(theACC_TXN_MGR);
		break;
	case ECT_TRANS:
		txnDataMgr = &(theECT_TXN_MGR);
		break;
	case AFC_TRANS:
		txnDataMgr = &(theAFC_TXN_MGR);
		break;
	default:
		return SP_ERR_INTERNAL_ERROR;
	}
	txnDataMgr->MakePekg();
	if(!theAPP_SESSION.IsSCConnected())
	{
		return SCSVC_RESULT_NOT_CONNECTED;
	}
	// 有未发送
	while(txnDataMgr->HasUnsendData() > 0)
	{
		// 获取队列中的数据包
		PEKG_DATA dataPekg;		
		bool aquirePekgSuccess = txnDataMgr->GetDataPekg(dataPekg);	
		if(!aquirePekgSuccess)
		{
			continue;
		}
		CMD_HEADER header = CHeaderManager::AquireHeader(TRANSACTION_DATA,CMD_DATA_TRANSFER);
		auto_ptr<CSCDataMsg> pTxnDataMsg(new CSCDataMsg);
		pTxnDataMsg->SetHeader(&header);
		pTxnDataMsg->SetContent(dataPekg.lpData,dataPekg.nLen);
		long executeResult = theSERVICE_MGR.GetService(SC_SVC)->SendMessage(SC_MSG_SEND_TO_SC,NULL,(LPARAM)pTxnDataMsg.get());
		if (executeResult == SP_SUCCESS)
		{
			// 若发送成功设置数据包为已送
			txnDataMgr->SetPekgSended(dataPekg.strPekgID);
			CTMessageAudit::PEKG_INFO PekgInfo;
			memcpy(PekgInfo.sPekgId, dataPekg.lpData+1, 14);
			memcpy(&PekgInfo.iRecordCnt,dataPekg.lpData+19, 2);				
			thePEKG_AUDIT.AddWriteFile(PekgInfo);
		}
		else
		{
			return executeResult;
		}
	}
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertBomAR(DEVICE_AR_REASON arReason)
{
	try
	{
		VARIABLE_DATA msg;
		theSCMessageMakeHelper.MakeBOMAR(arReason,msg);
		theACC_TXN_MGR.SaveTxnData(msg.nLen, msg.lpData);

	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     发送TVM退款电文 

@param    (i)BYTE (&bRefoundNo)[15] 退款凭条号
		  (i)DWORD dwRefoundAmount	退款金额

@retval	   无    

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertBomRefundTVMMsg(BYTE (&bRefoundNo)[15],DWORD dwRefoundAmount)
{
	try
	{
		BYTE lpMsg[LEN_TXN_BOM_REFUND_TVM]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_BOM_REFUND_TVM);
		TransComHdr_t transHeader = theSCMessageMakeHelper.AquireTransComHeader(MSG_TRANS_BOM_REFOUND_TVM);
		BOM_REFUND_TVM refundTvm;
		refundTvm.transComHdr = transHeader;
		refundTvm.dwRefoundAmount = dwRefoundAmount;
		memcpy(refundTvm.bRefoundNo,bRefoundNo,15);
		theSCMessageMakeHelper.MakeBOMRefundTVM(afcHeader,refundTvm,lpMsg);
		theAFC_TXN_MGR.SaveTxnData(LEN_TXN_BOM_REFUND_TVM, lpMsg); 
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
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
void CTransSvc::InsertInvalidTicketRecycleMsg(INVALIDE_TICKET_RECYCLETRADE& invalidTicketRecycle)
{
	try{
		BYTE lpMsg[LEN_TXN_INVALID_TICKET_RECYCLE]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_BOM_REFUND_TVM);
		invalidTicketRecycle.transComHdr = theSCMessageMakeHelper.AquireTransComHeader(MSG_TRANS_RECYLE_INVALID_CARD);
		theSCMessageMakeHelper.MakeRecycleInvalidTicket(afcHeader,invalidTicketRecycle,lpMsg);
		theAFC_TXN_MGR.SaveTxnData(LEN_TXN_INVALID_TICKET_RECYCLE, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    发送非营业性收支电文  

@param   (i)CString reason	原因
		 (i)int amount		金额
		 (i)CString memo	备注

@retval		无    

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertBomNonBusinessIncomeMsg(CString reason,int amount,CString memo)
{
	try
	{
		BYTE lpMsg[LEN_TXN_BOM_NONBUSINESS_INCOME]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_BOM_NONBUSINESS_INCOME);
		TransComHdr_t transHeader = theSCMessageMakeHelper.AquireTransComHeader(MSG_TRANS_BOM_NONBUSINESS_INCOME);
		theSCMessageMakeHelper.MakeBomNonBusinessIncome(afcHeader,transHeader,reason,amount,memo,lpMsg);
		theAFC_TXN_MGR.SaveTxnData(LEN_TXN_BOM_NONBUSINESS_INCOME, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
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
//void CTransSvc::InsertNonImmidatelySurrenderApply(NonImmidatelySurrenderApply&)
//{
//	
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CTransSvc::InsertNonImmidatelySurrender(NonImmidatelySurrender&)
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      记名卡申请

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertSignCardApply(SignCardApply& signCardApply)
{
	try
	{
		BYTE lpMsg[LEN_TXN_SIGNCARD_APPLY]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_SIGNCARD_APPLY);
		theSCMessageMakeHelper.MakeSignCardApply(afcHeader,signCardApply,lpMsg);
		theACC_TXN_MGR.SaveTxnData(LEN_TXN_SIGNCARD_APPLY, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
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
void CTransSvc::InsertSignCardModify(SignCardModify& signCardApply)
{
	try
	{
		BYTE lpMsg[LEN_TXN_SIGNCARD_MODIFY]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_SIGNCARD_MODIFY);
		theSCMessageMakeHelper.MakeSignCardModify(afcHeader,signCardApply,lpMsg);
		theACC_TXN_MGR.SaveTxnData(LEN_TXN_SIGNCARD_MODIFY, lpMsg);
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
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
void CTransSvc::InsertSignCardReportLoss(ReportLoss& reportLoss)
{
	BYTE lpMsg[LEN_TXN_REPORT_LOSS]={0};
	AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_REPORT_LOSS);
	theSCMessageMakeHelper.MakeReportLoss(afcHeader,reportLoss,lpMsg);
	theACC_TXN_MGR.SaveTxnData(LEN_TXN_REPORT_LOSS, lpMsg);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解挂

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertSignCardCancelReportLoss(CancelReportLoss& cancelReportLoss)
{
	BYTE lpMsg[LEN_TXN_CANCEL_REPORT_LOSS]={0};
	AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_CANCEL_REPORT_LOSS);
	theSCMessageMakeHelper.MakeCancelReportLoss(afcHeader,cancelReportLoss,lpMsg);
	theACC_TXN_MGR.SaveTxnData(LEN_TXN_CANCEL_REPORT_LOSS, lpMsg);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   行政处理   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertAdministrationProcess(FinanceComHdr_t& finance,AdministrationProcess& process)
{
	BYTE lpMsg[LEN_TXN_ADMINISTRATION_PROCESS]={0};
	AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_ADMINISTRATION_PROCESS);
	theSCMessageMakeHelper.MakeAdministrationProcess(afcHeader,finance,process,lpMsg);
	theACC_TXN_MGR.SaveTxnData(LEN_TXN_ADMINISTRATION_PROCESS, lpMsg);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     交易审核数据 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertTransactionAudit(TxnAuditData& txnAuditData)
{
	VARIABLE_DATA transactionAuditData;
	AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(0);
	theSCMessageMakeHelper.MakeTransactionAudit(afcHeader,txnAuditData,transactionAuditData);
	theACC_TXN_MGR.SaveTxnData(transactionAuditData.nLen,transactionAuditData.lpData);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CTransSvc::SendNonImmidatelySurrenderApply(NonImmidatelySurrenderApply& apply)
{
	BYTE lpMsg[LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY]  = {0};
	AfcSysHdr_t afcHeader = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY);
	theSCMessageMakeHelper.MakeNonImmidatelySurrenderApply(afcHeader,apply,lpMsg);
	theACC_TXN_MGR.SaveTxnData(LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY,lpMsg);
	theACC_TXN_MGR.MakePekg(false);
	return SendTransactionData(ACC_TRANS);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CTransSvc::SendQueryNonImmidatelySurrenderApply(QueryNonImmidatelySurrenderApply& apply)
{
	BYTE lpMsg[LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY]  = {0};
	AfcSysHdr_t afcHeader = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY);
	theSCMessageMakeHelper.MakeQueryNonImmidatelySurrenderApply(afcHeader,apply,lpMsg);
	theACC_TXN_MGR.SaveTxnData(LEN_TXN_NONIMMIDATELY_SURRENDER_APPLY_QUERY,lpMsg);
	theACC_TXN_MGR.MakePekg(false);
	return SendTransactionData(ACC_TRANS);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CTransSvc::SendNonImmediatelySurrender(NonImmidatelySurrender& refundInfo)
{
	BYTE lpMsg[LEN_TXN_NONIMMIDATELY_SURRENDER]  = {0};
	AfcSysHdr_t afcHeader = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_NONIMMIDATELY_SURRENDER);
	theSCMessageMakeHelper.MakeNonImmediatelySurrender(afcHeader,refundInfo,lpMsg);
	theACC_TXN_MGR.SaveTxnData(LEN_TXN_NONIMMIDATELY_SURRENDER,lpMsg);
	theACC_TXN_MGR.MakePekg(false);
	return SendTransactionData(ACC_TRANS);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CTransSvc::SendQuerySignCardApplyStatus(QuerySignCardApply& apply)
{
	try
	{
		BYTE lpMsg[LEN_TXN_SIGNCARD_APPLY_QUERY]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_SIGNCARD_APPLY_QUERY);
		theSCMessageMakeHelper.MakeQuerySignCardApplyStatus(afcHeader,apply,lpMsg);
		// 保存事件数据
		theACC_TXN_MGR.SaveTxnData(LEN_TXN_SIGNCARD_APPLY_QUERY,lpMsg);
		theACC_TXN_MGR.MakePekg(false);
		return SendTransactionData(ACC_TRANS);
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
@brief      保存交易审核数据

@param      WORD txnLen		UD数据长度（包含AFC系统专用头的10个字节）
@param      LPBYTE txnBuf   UD数据体（包含AFC系统专用头的10个字节）

@retval     无  

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::SaveTxnAuditData(WORD txnLen,LPBYTE txnBuf)
{
	if(txnLen <= 0 || txnBuf == NULL){
		throw CTxnDataException(CTxnDataException::FUNC_PARAM_ERR, _T(__FILE__), __LINE__);
	}
	TXNAuditKey txnAuditKey;
	TXNAuditValue txnAuditValue;
	LPBYTE lpBuf = txnBuf;
	lpBuf += 19;
	txnAuditKey.txnType = *lpBuf;
	lpBuf++;
	txnAuditKey.txnSubType = *lpBuf;
	lpBuf+=43;
	txnAuditKey.ticketType = *lpBuf;
	lpBuf+=15;
	txnAuditValue.transAmount=ComMakeLong(*lpBuf,*(lpBuf+1),*(lpBuf+2),*(lpBuf+3));
	txnAuditValue.transCount = 1;
	if(ComMakeWORD(txnAuditKey.txnType,txnAuditKey.txnSubType) == MSG_TRANS_ADMINISTRATION_PROCESS)
	{
		lpBuf+=9;
		if(*lpBuf == BUSINESS_TYPE_EXPENSE)
		{
			txnAuditValue.transAmount = 0-txnAuditValue.transAmount;
		}
	}
	if (ComMakeWORD(txnAuditKey.txnType,txnAuditKey.txnSubType)==MSG_TRANS_AUDIT)
	{
		return;
	}
	theTxnAudit_MGR.AddTxnAudit(txnAuditKey,txnAuditValue);
}
