#pragma once
#include "MonitorSvc.h"
/*
Card��д�������
*/
class CCardRWMonitorSvc:public CMonitorSvc
{
public:
	CCardRWMonitorSvc();
	virtual ~CCardRWMonitorSvc();
protected:
	virtual bool Monitor();
};