#pragma once
#include "MonitorSvc.h"
/*
打印机监控服务
*/
class CPrinterMonitorSvc : public CMonitorSvc
{
public:
	CPrinterMonitorSvc();
	virtual ~CPrinterMonitorSvc();
	void PostPrinterEmptyMSG();		// 发送打印机缺纸消息
protected:
	virtual bool Monitor();
	bool m_isError;
};