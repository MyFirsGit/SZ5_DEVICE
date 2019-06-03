#pragma once
#include "MonitorSvc.h"
/*
Card¶ÁÐ´Æ÷¼à¿ØÀà
*/
class CCardRWMonitorSvc:public CMonitorSvc
{
public:
	CCardRWMonitorSvc();
	virtual ~CCardRWMonitorSvc();
protected:
	virtual bool Monitor();
};