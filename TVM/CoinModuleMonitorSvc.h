#pragma once
#include "MonitorSvc.h"
/*
Ó²±ÒÄ£¿é¼à¿Ø·þÎñ
*/
class CCoinModuleMonitorSvc : public CMonitorSvc
{
public:
	CCoinModuleMonitorSvc();
	virtual ~CCoinModuleMonitorSvc();
protected:
	virtual bool Monitor();
private:
	bool m_isAlarmOpend;
	BOOL DO_OPEN_ALARM_FOR_A;
	BOOL DO_OPEN_ALARM_FOR_B;
};