#pragma once
#include "MonitorSvc.h"
/*
Ʊ������ģ������
*/
class CCardModuleMonitorSvc:public CMonitorSvc
{
public:
	CCardModuleMonitorSvc();
	virtual ~CCardModuleMonitorSvc();
protected:
	virtual bool Monitor();
};