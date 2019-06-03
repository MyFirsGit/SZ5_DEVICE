#pragma once
#include "BackService.h"
#include "scapi.h"
//#include "SCAuditData.h"
#include "scsvcdef.h"

/**
@brief ���ڼ���Service
*/
class CEventDataSvc :
	public CBackService
{
	DECLARE_MESSAGE_MAP()

public:
	CEventDataSvc(void);
	~CEventDataSvc(void);
	void OnStart();
	bool OnStop();
	void StartPackageAuditTimer();
	void EndPackageAuditTimer();
	void StartTicketAuditTimer();
	void EndTicketAuditTimer();
	long SendAccountLock(int nOperatorID, BYTE nPosType);									  // �˻�����
	long SendDeviceUdsnReset(UDSN_TYPE  udsnType,UDSN_RESET_REASON resetReason);			  // �豸��ˮ����
	long SendDeviceComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);  			  // ��������(����SAM����)	
	long SendSamComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);  			  // ����������Ԥ���� 
	long SendTicketBoxCountReport();														  // Ʊ���汨��
	long SendMoneyBoxCountReport();															  // Ǯ����������
	long SendClearBankNotesBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount);				  // ֽ��������¼�
	long SendClearBanknotesChangeBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount);			  // ֽ������������¼�
	long SendClearCoinsBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount);					  // Ӳ��������¼�

	long SendTVMAuditData(PurchaseData& purchaseData);										  // TVM������ݣ�
	long SendPackageMsgTransAudit();   														  // �������������

	long SendEventData();

private:
	enum ENUM_TIMER {
		TICKET_AUDIT_TIMER = 1, 
		PACKAGE_AUDIT_TIMER = 2,
		PACKAGE_TIMER = 3,
	};
	void OnTimer(UINT nEventID);
	void OnCounterReseted(COUNTER_TYPE);
	LRESULT OnUDSNReset(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnEndTimer(WPARAM,LPARAM);
};