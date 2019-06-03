#pragma once
#include "BackService.h"
#include "scapi.h"
#include "SCAuditData.h"
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
	long SendAccountLock(int nOperatorID, BYTE nPosType);									  // by xxj, 6.5.1	账户锁定
	long SendDeviceUdsnReset(UDSN_TYPE  udsnType,UDSN_RESET_REASON resetReason);						  // 6.5.2	设备流水重置
	long SendDeviceComponentExchange(BYTE bType, LPBYTE lpNewID, LPBYTE lpOldID);  			  // 6.5.3	部件更换				 
	long SendTicketBoxCountReport();															 // 6.5.4	票箱库存报告

	long SendBomAuditData(PurchaseData& purchaseData,AUDIT_FLAG auditFlag);					      // 6.5.8	BOM审计数据-登出/结账，发送审计电文
	//long SendBomCloseData();																  // 6.5.8	BOM审计数据-业务结束，发送BOM日结审计电文
	long SendPackageMsgTransAudit();   														  // 6.5.10	包传输审计数据

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