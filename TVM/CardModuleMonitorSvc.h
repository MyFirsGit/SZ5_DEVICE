#pragma once
#include "MonitorSvc.h"
/*
票卡处理模块监控类
*/
class CCardModuleMonitorSvc:public CMonitorSvc
{
public:
	CCardModuleMonitorSvc();
	virtual ~CCardModuleMonitorSvc();
protected:
	virtual bool Monitor();
};