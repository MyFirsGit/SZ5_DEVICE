#pragma once
#include "MonitorSvc.h"
/*
��Ʊ�������
*/
class CTHMonitorSvc:public CMonitorSvc
{
public:
	CTHMonitorSvc();
	virtual ~CTHMonitorSvc();
protected:
	virtual bool Monitor();
};