#pragma once
#include "BackService.h"
#include "TxnDataEnum.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include "txnauditdata.h"
/**
@brief 定期集计Service
*/
class CTransSvc :
	public CBackService
{
	DECLARE_MESSAGE_MAP()

public:
	CTransSvc(void);
	~CTransSvc(void);

public:
	void StartTransmissionTimer();
	void EndTransmissionTimer();
	//void InsertNonImmidatelySurrenderApply(NonImmidatelySurrenderApply&);
	//void InsertNonImmidatelySurrender(NonImmidatelySurrender&);
	void InsertSignCardApply(SignCardApply&);
	void InsertSignCardModify(SignCardModify&);
	void InsertSignCardReportLoss(ReportLoss&);
	void InsertSignCardCancelReportLoss(CancelReportLoss&);
	void InsertAdministrationProcess(FinanceComHdr_t&, AdministrationProcess&);
	void InsertBomRefundTVMMsg(BYTE (&bRefoundNo)[15],DWORD dwRefoundAmount);												  // 6.3.1	BOM对TVM故障退款
	void InsertInvalidTicketRecycleMsg(INVALIDE_TICKET_RECYCLETRADE&);  // 6.3.5	废票回收交易
	void InsertBomNonBusinessIncomeMsg(CString reason,int amount,CString memo);//6.3.6BOM非营业性收支
	void InsertTransactionAudit(TxnAuditData&);
	void SaveTxnAuditData(WORD txnLen,LPBYTE txnBuf);                                         // 保存交易审核数据
	//审计报文数据定义(6.2.1.2)
	void InsertBomAR(DEVICE_AR_REASON arReason);
	long SendNonImmidatelySurrenderApply(NonImmidatelySurrenderApply&);
	long SendQueryNonImmidatelySurrenderApply(QueryNonImmidatelySurrenderApply&);
	long SendNonImmediatelySurrender(NonImmidatelySurrender&);
	long SendQuerySignCardApplyStatus(QuerySignCardApply&);
	long SendTransactionData();
protected:
	void OnStart();
	bool OnStop();

private:
	
	enum ENUM_TIMER {
		TIMER_TRANSMISSION = 1,      // 交易数据传输定时器
		TIMER_PACKAGE = 2,					//打包定时器
	};

	void OnTimer(UINT nEventID);

	long SendTransactionData(DATA_TYPE_CODE dataTypeCode);

};
