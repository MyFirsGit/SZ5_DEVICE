#pragma once
#include "MonitorSvc.h"
/*
LPT监控服务
*/
class CIOMonitorSvc : public CMonitorSvc
{
public:
	CIOMonitorSvc();
	virtual ~CIOMonitorSvc();
	void PostAssistanceButtonActionMSG();										// 发送招援按钮动作消息
	void PostDoorActionMSG();													// 发送维护门动作消息
	IO_RSP_STATUS_INFO ioStatus;
protected:
	virtual bool Monitor();
	void ResponseAssistBtn(UINT uMsg);                                          // 响应招援按钮   
private:
	DOOR_STATUS m_DoorStatus;
	int UPORDOWN;
	int OPENORCLOSE;
};