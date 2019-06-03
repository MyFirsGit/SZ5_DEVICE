#include "StdAfx.h"
#include "TranscationTranslateSvc.h"
#include "CAfcDeviceCommonParam.h"
#include "headermanager.h"
#include "ctmessageaudit.h"
#include "scdatamsg.h"
#include "serverclientsvc.h"
#include "scmessagemakehelper.h"
#include "ServerClientSvc.h"
#include "tvmdef.h"
#include "scsvcresult.h"
#include "Sync.h"
#include "sha-1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTransSvc, CBackService)
	ON_SERVICE_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTransSvc::CTransSvc(void):CBackService(TRANSMISSION_INTERVAL_SVC,ROOT_SVC)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CTransSvc::~CTransSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::OnStart()
{
	__super::OnStart();
	StartTimer(TIMER_PACKAGE,theTVM_INFO.GetTransactionPackageInterval());
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
/**
@brief       生成ACC设备寄存器数据

@param       

@retval      

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertTxnAuditRegister()
{
	try
	{
		BYTE bMsg[LEN_TXN_AUDIT_REGISTER] = {0};
		LPBYTE lpMsg = bMsg;

		// AFC公共头和消息正文
		BYTE bAfcMsg[LEN_AFC_SYS_HEADER] = {0};
		BYTE bContentMsg[LEN_TXN_SYSCOM_HDR + 2408] = {0};
		TXN_AUDIT_REGISTER auditRegister;
		auditRegister.afcSysHdr = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_AUDIT_REGISTER);
		auditRegister.sysComHdr = theSCMessageMakeHelper.AquireSysComHeader(0x0501);
		theSCMessageMakeHelper.MakeTxnAuditRegister(auditRegister,theACCAUDIT_MGR.GetCurrentAR(),bContentMsg,bAfcMsg);

		// 计算MAC
		bool isTacCalFailed = false;
		SysSecurityHdr_t sysSecurityHdr;
		RW_CMD_CALCULATE_MAC cmdCalMac;
		// wf 重庆ACC参数中没有与MAC相关项，此处暂时不做赋值
		//cmdCalMac.keyVersion = theACC_SYSTEM.GetudKeyVersion();
		// reconciliationDate	4	BIN	数据生成日期(非MAC)（DateC20_t）
		int lenMsgMAC = LEN_TXN_SYSCOM_HDR - 4 + 2408;
		BYTE bContentMsgMAC[LEN_TXN_SYSCOM_HDR - 4 + 2408] = {0};
		memcpy(bContentMsgMAC,bContentMsg,40);
		memcpy(bContentMsgMAC + 40,bContentMsg + 44,lenMsgMAC - 40);
		SHA1(bContentMsgMAC,lenMsgMAC,cmdCalMac.macCalculate);
		if(theTVM_STATUS_MGR.GetSam1Status() == ISAM_UNAUTH){
			memset(sysSecurityHdr.txnMac,0xFF,8);
		}
		else{
			long macErrCode = -1;
			BYTE macResponse[4] = {0};
			try{
				//macErrCode = CRechargeRWHelper::CalculateMac(cmdCalMac,macResponse);
			}
			catch (CSysException& e) {
				isTacCalFailed = true;
				theEXCEPTION_MGR.ProcessException(e);
			}
			catch (...) {
				isTacCalFailed = true;
				theEXCEPTION_MGR.ProcessException(CInnerException(TRANSMISSION_INTERVAL_SVC,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
			}
			if(RW_RSP_OK == macErrCode) {
				memcpy(sysSecurityHdr.txnMac,macResponse,4);
				memset(sysSecurityHdr.txnMac + 4,0x00,4); 
			}
			else {
				isTacCalFailed = true;
				memset(sysSecurityHdr.txnMac,0xFF,8);
			}
		}
		sysSecurityHdr.keyVersion = cmdCalMac.keyVersion;
		BYTE bSecurityMsg[LEN_TXN_SYSSECURITY_HDR] = {0};
		theSCMessageMakeHelper.MakeSysSecurityHdr(sysSecurityHdr,bSecurityMsg);

		// ACC设备寄存器交易数据
		memcpy(lpMsg,bAfcMsg,LEN_AFC_SYS_HEADER);
		lpMsg += LEN_AFC_SYS_HEADER;
		memcpy(lpMsg,bContentMsg,LEN_TXN_SYSCOM_HDR + 2408);
		lpMsg += (LEN_TXN_SYSCOM_HDR + 2408);
		memcpy(lpMsg,bSecurityMsg,LEN_TXN_SYSSECURITY_HDR);
		lpMsg += LEN_TXN_SYSSECURITY_HDR;

		// 保存交易数据
		if (/*theAPP_SESSION.GetISAMYPT_RW() == UNAUTHENTICATED || */isTacCalFailed == true) {
			//SaveUnTacMessage(bMsg,LEN_TXN_AUDIT_REGISTER,MSG_TXNAUDIT_REGISTER);
		}
		else{
			theAFC_TXN_MGR.SaveTxnData(LEN_TXN_AUDIT_REGISTER,bMsg);
		}
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
void CTransSvc::InsertTxnEventCDVersionStatus()
{
	try
	{
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
void CTransSvc::InsertTxnEventDeviceStatus(BYTE deviceStatus)
{
	try
	{
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
void CTransSvc::InsertTxnEventDeviceIdentification()
{
	try
	{
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
void CTransSvc::InsertDeviceAR(DEVICE_AR_REASON arReason)
{
	try{
		VARIABLE_DATA msg;
		theSCMessageMakeHelper.MakeDeviceAR(arReason,msg);
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
@brief      废票回收交易

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::InsertInvalidTicketRecycleMsg(INVALIDE_TICKET_RECYCLETRADE& invalidTicketRecycle)
{
	try{
		BYTE lpMsg[LEN_TXN_INVALID_TICKET_RECYCLE]={0};
		AfcSysHdr_t afcHeader  = theSCMessageMakeHelper.AquireAfcSysHeader(LEN_TXN_INVALID_TICKET_RECYCLE);
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
/**
@brief       保存未算Tac交易数据

@param       LPBYTE buffer         交易数据
@param       int length            长度
@param       MSG_CODE msgCode      消息代码

@retval      无

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTransSvc::SaveUnTacMessage(LPBYTE buffer,int length,MSG_CODE_TRANS msg_code)
{

	SYSTEMTIME dtCurTime;
	::GetLocalTime(&dtCurTime);

	CString fileName = _T("");
	fileName.Format(_T("%.4x%.4d%.2d%.2d%.2d%.2d%.2d%.3d"),msg_code,dtCurTime.wYear,dtCurTime.wMonth,dtCurTime.wDay,dtCurTime.wHour,dtCurTime.wMinute,dtCurTime.wSecond,dtCurTime.wMilliseconds);
	CString folder = theTVM_INFO.GetDataPath() + ACC_UNTAC_FOLDER;
	ComCheckAndCreateDirectory(folder);
	fileName = folder + fileName;

	CFile cFile;
	CFileException cfException;
	if(!cFile.Open( fileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary,&cfException)){
		TCHAR   strCause[ERROR_MSG_MAX_LEN]; 
		CString strException; 
		cfException.GetErrorMessage(strCause, sizeof(strCause), NULL); 
		strException = "异常："; 
		strException += strCause; 
		throw CTxnDataException(CTxnDataException::FILE_OPEN_FAIL, _T(__FILE__), __LINE__, strException); 
	}
	SYNC(FILE,fileName);
	try{
		cFile.Write(buffer,length);
		cFile.Flush();
		cFile.Close();
	}
	catch(CSysException &)
	{
		cFile.Close();
		throw;
	}
	catch(...){
		cFile.Close();
		throw CTxnDataException(CTxnDataException::FILE_WRITE_FAIL, _T(__FILE__), __LINE__); 
	}
}