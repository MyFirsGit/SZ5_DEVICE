#pragma once
#include "BackService.h"
#include "TxnDataEnum.h"
#include "scapi.h"
#include "SCSvcDef.h"

/**
@brief 定期集计Service
*/
class CTransSvc : public CBackService
{
	DECLARE_MESSAGE_MAP()

public:
	CTransSvc(void);
	~CTransSvc(void);

	void StartTransmissionTimer();
	void EndTransmissionTimer();

	long SendTransactionData();

	void InsertTxnAuditRegister();												// ACC寄存器数据
	void InsertTxnEventCDVersionStatus();										// CD版本切换
	void InsertTxnEventDeviceStatus(BYTE deviceStatus);							// 设备状态变更（预留）
	void InsertTxnEventDeviceIdentification();									// ISAM卡变更（预留）
	void InsertInvalidTicketRecycleMsg(INVALIDE_TICKET_RECYCLETRADE&);			// 废票回收交易
	void InsertDeviceAR(DEVICE_AR_REASON arReason);								// 设备审计数据

protected:
	void OnStart();
	bool OnStop();

private:
	enum ENUM_TIMER {
		TIMER_TRANSMISSION = 1,      // 交易数据传输定时器
		TIMER_PACKAGE = 2,			 // 打包定时器
	};

	void OnTimer(UINT nEventID);
	long SendTransactionData(DATA_TYPE_CODE dataTypeCode);

	void SaveUnTacMessage(LPBYTE buffer,int length,MSG_CODE_TRANS msg_code); // 保存计算Tac失败UD
};