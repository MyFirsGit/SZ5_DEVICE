#pragma once
#include "MonitorSvc.h"
/*
LPT��ط���
*/
class CLPTMonitorSvc : public CMonitorSvc
{
public:
	CLPTMonitorSvc();
	virtual ~CLPTMonitorSvc();
protected:
	virtual bool Monitor();
private:
	DOORSTATUS m_DoorStatus;
};