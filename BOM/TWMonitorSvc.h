#pragma once
#include "MonitorSvc.h"
/*
ÄÚ²¿¶ÁÐ´Æ÷¼à¿ØÀà
*/
class CTWMonitorSvc:public CMonitorSvc
{
public:
	CTWMonitorSvc();
	virtual ~CTWMonitorSvc();
protected:
	virtual bool Monitor();
private:
	CTWHelper* m_TWHelper;
};