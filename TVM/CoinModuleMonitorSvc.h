#pragma once
#include "MonitorSvc.h"
/*
Ӳ��ģ���ط���
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