#pragma once
#include "MonitorSvc.h"
/*
�ڲ���д�������
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