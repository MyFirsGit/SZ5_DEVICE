#pragma once
#include "MonitorSvc.h"
/*
UPS��ط���
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

	// UPS������ʱ����
	void BatteryLowAlert();
	void BatteryNeedClose();
};