#pragma once
#include "BackService.h"
#include "TxnDataEnum.h"
#include "scapi.h"
#include "SCSvcDef.h"
#include "txnauditdata.h"
/**
@brief ���ڼ���Service
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
	void InsertBomRefundTVMMsg(BYTE (&bRefoundNo)[15],DWORD dwRefoundAmount);												  // 6.3.1	BOM��TVM�����˿�
	void InsertInvalidTicketRecycleMsg(INVALIDE_TICKET_RECYCLETRADE&);  // 6.3.5	��Ʊ���ս���
	void InsertBomNonBusinessIncomeMsg(CString reason,int amount,CString memo);//6.3.6BOM��Ӫҵ����֧
	void InsertTransactionAudit(TxnAuditData&);
	void SaveTxnAuditData(WORD txnLen,LPBYTE txnBuf);                                         // ���潻���������
	//��Ʊ������ݶ���(6.2.1.2)
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
		TIMER_TRANSMISSION = 1,      // �������ݴ��䶨ʱ��
		TIMER_PACKAGE = 2,					//�����ʱ��
	};

	void OnTimer(UINT nEventID);

	long SendTransactionData(DATA_TYPE_CODE dataTypeCode);

};
