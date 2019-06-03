#pragma once
#include "MonitorSvc.h"
/*
UPS监控服务
*/
class CUPSMonitorSvc:public CMonitorSvc
{
public:
	CUPSMonitorSvc();
	virtual ~CUPSMonitorSvc();
protected:
	virtual bool Monitor();
private:
	CUPSHelper* m_UPSHelper;
	bool m_Alerted;

	// UPS电量低时调用
	void BatteryLowAlert();
	void BatteryNeedClose();
};