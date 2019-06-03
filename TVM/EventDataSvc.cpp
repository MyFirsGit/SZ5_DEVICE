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
@brief      �¼����ݷ����캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CEventDataSvc::CEventDataSvc(void)
	:CBackService(EVENT_MSG_SVC,ROOT_SVC)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CEventDataSvc::~CEventDataSvc(void)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ʱ����Ӧ����

@param      ��

@retval     ��

@exception  ��
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
@brief      ����ֹͣʱ��Ӧ����

@param      ��

@retval     bool ����ֹͣ

@exception  ��
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
@brief      ��ʼ����Ƽ�ʱ
@param      UINT nTimerID  identifier of a timer
@retval     ��
@exception  ��
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
@brief      ��������Ƽ�ʱ
@param      ��
@retval     ��
@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::EndPackageAuditTimer()
{
	StopTimer(PACKAGE_AUDIT_TIMER);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     ����Ʊ���汨���ʱ�� 

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
@brief      �ر�Ʊ���汨���ʱ��

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
@brief      ��ʱ����Ӧ����
@param      UINT nTimerID  identifier of a timer
@retval     ��
@exception  ��
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
			// ����Ǯ�估Ʊ����
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
@brief      ������������Ӧ����
@param ����������
@retval     ִ�н��
*/
//////////////////////////////////////////////////////////////////////////
void CEventDataSvc::OnCounterReseted(COUNTER_TYPE counterType)
{
	this->PostMessage(SM_UDSN_RESET,counterType,MAXVALUERESET);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������������ϢSM_UDSN_RESET��Ӧ����
@param ����������
@retval     ִ�н��
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
@brief      �˻�����
@param      int		nOperatorID       ����ԱID
int		nPosType          λ������: 0x01��·���ļ�����;x02Ʊ�����ļ�����0x03ά�����ļ�����0x06��վ������
@retval     ִ�н��
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
@brief      ����UDSN��������

@param      UDSN_TYPE  udsnType UDSN����
@param	UDSN_RESET_REASON����ԭ��

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
@brief      ����AVM\TVM�������

@param   PURCHASE_DATA& purchaseData �������
@param	AUDIT_FLAG auditFlag ���ԭ��

@retval     long

@exception  ��
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
@brief     ������������� 

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CEventDataSvc::SendPackageMsgTransAudit()
{
	try
	{
		SYNC(TXNMANAGER,_T("EVENT"));

		OperComHdr_t operComHdr=theSCMessageMakeHelper.AquireOperHeader(MSG_BUSSINESS_PACKAGE_AUDIT,0);
		BYTE lpMsg[RECEIVED_BUFF_SIZE]={0};
		// ��ȡ�������������
		theSCMessageMakeHelper.MakePackageMsgTransAudit(operComHdr, lpMsg);
		// �����¼�����
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
@brief      �����¼�����

@param      ��

@retval     long SP_SUCCESS�ɹ�

@exception  ��
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
			// �����ͳɹ��������ݰ�Ϊ����
			theEVN_TXN_MGR.SetPekgSended(dataPekg.strPekgID);	
			WORD msgCode = MAKEWORD(*(dataPekg.lpData+ LEN_PACKAGE_HEADER + INX_BUSINESS_DATATYPE+1),*(dataPekg.lpData+LEN_PACKAGE_HEADER+INX_BUSINESS_DATATYPE));
			if(msgCode!=MSG_BUSSINESS_PACKAGE_AUDIT)
			{
				CTMessageAudit::PEKG_INFO PekgInfo;
				memcpy(PekgInfo.sPekgId, dataPekg.lpData+1, 14);
				memcpy(&PekgInfo.iRecordCnt,dataPekg.lpData+19, 2);		
				thePEKG_AUDIT.AddWriteFile(PekgInfo);
			}
			// ���ô������״̬Ϊ����:zhengxianle
			//theTVM_STATUS_MGR.SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS_NORMAL);
		}
		else
		{
			// ���ô������״̬Ϊ����:zhengxianle
			//theTVM_STATUS_MGR.SetDeviceTransferAudit(TRANSFER_AUDIT_STATUS_WARNING);
			return executeResult;
		}
	}

	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief     ����������Ԥ����

@param     (i)BYTE     bType      ��������
@param     (i)LPBYTE   lpNewID    �²�������
@param     (i)LPBYTE   lpOldID    �ɲ�������

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
@brief     SAM����

@param     (i)BYTE     bType      SAM����
@param     (i)LPBYTE   lpNewID    ��SAM����
@param     (i)LPBYTE   lpOldID    ��SAM����

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
@brief      ����Ʊ���汨��

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
		operComHdr.msgLen = LEN_BUSINESS_HEADER + dataLen;//ʵ�ʳ���
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
@brief      ����Ǯ���汨��

@param      ��

@retval     ��

@exception  ��
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
@brief      TVMֽ��������¼�

@param      ��

@retval     long �������

@exception  ��
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
@brief      ����ֽ������������¼�

@param      vector<CASH_TYPE_COUNT>& vecTypeCount;��յ�ֽ������ּ�������

@retval     long lRet���������

@exception  ��
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
@brief      ����TVMӲ��������¼�����

@param      ��

@retval     long ���ش���

@exception  ��
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