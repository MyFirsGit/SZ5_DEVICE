#pragma once
#include "BackService.h"
#include "scapi.h"
//#include "SCAuditData.h"
#include "scsvcdef.h"

/**
@brief 定期集计Service
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
	long SendAccountLock(int nOperatorID, BYTE nPosType);									  // 账户锁定
	long SendDeviceUdsnReset(UDSN_TYPE  udsnType,UDSN_RESET_REASON resetReason);			  // 设备流水重置
	long SendDeviceComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);  			  // 部件更换(包括SAM更换)	
	long SendSamComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);  			  // 部件更换（预留） 
	long SendTicketBoxCountReport();														  // 票箱库存报告
	long SendMoneyBoxCountReport();															  // 钱箱数量报告
	long SendClearBankNotesBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount);				  // 纸币箱清空事件
	long SendClearBanknotesChangeBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount);			  // 纸币找零箱清空事件
	long SendClearCoinsBoxReport(vector<CASH_TYPE_COUNT>& vecTypeCount);					  // 硬币箱清空事件

	long SendTVMAuditData(PurchaseData& purchaseData);										  // TVM审计数据？
	long SendPackageMsgTransAudit();   														  // 包传输审计数据

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