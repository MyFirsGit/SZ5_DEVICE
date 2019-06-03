#pragma once
#include "BackService.h"
#include "TxnDataEnum.h"
#include "scapi.h"
#include "SCSvcDef.h"

/**
@brief ���ڼ���Service
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

	void InsertTxnAuditRegister();												// ACC�Ĵ�������
	void InsertTxnEventCDVersionStatus();										// CD�汾�л�
	void InsertTxnEventDeviceStatus(BYTE deviceStatus);							// �豸״̬�����Ԥ����
	void InsertTxnEventDeviceIdentification();									// ISAM�������Ԥ����
	void InsertInvalidTicketRecycleMsg(INVALIDE_TICKET_RECYCLETRADE&);			// ��Ʊ���ս���
	void InsertDeviceAR(DEVICE_AR_REASON arReason);								// �豸�������

protected:
	void OnStart();
	bool OnStop();

private:
	enum ENUM_TIMER {
		TIMER_TRANSMISSION = 1,      // �������ݴ��䶨ʱ��
		TIMER_PACKAGE = 2,			 // �����ʱ��
	};

	void OnTimer(UINT nEventID);
	long SendTransactionData(DATA_TYPE_CODE dataTypeCode);

	void SaveUnTacMessage(LPBYTE buffer,int length,MSG_CODE_TRANS msg_code); // �������Tacʧ��UD
};