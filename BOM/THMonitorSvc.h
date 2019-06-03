#pragma once
#include "MonitorSvc.h"
/*
³öÆ±»ú¼à¿ØÀà
*/
class CTHMonitorSvc:public CMonitorSvc
{
public:
	CTHMonitorSvc();
	virtual ~CTHMonitorSvc();
protected:
	virtual bool Monitor();
};