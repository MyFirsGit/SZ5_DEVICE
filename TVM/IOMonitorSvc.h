#pragma once
#include "MonitorSvc.h"
/*
LPT��ط���
*/
class CIOMonitorSvc : public CMonitorSvc
{
public:
	CIOMonitorSvc();
	virtual ~CIOMonitorSvc();
	void PostAssistanceButtonActionMSG();										// ������Ԯ��ť������Ϣ
	void PostDoorActionMSG();													// ����ά���Ŷ�����Ϣ
	IO_RSP_STATUS_INFO ioStatus;
protected:
	virtual bool Monitor();
	void ResponseAssistBtn(UINT uMsg);                                          // ��Ӧ��Ԯ��ť   
private:
	DOOR_STATUS m_DoorStatus;
	int UPORDOWN;
	int OPENORCLOSE;
};