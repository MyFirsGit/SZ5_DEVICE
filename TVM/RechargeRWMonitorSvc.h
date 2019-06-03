#pragma  once
#include "MonitorSvc.h"
/*
充值读写器监控类
*/
class CRechargeRWMonitorSvc: public CMonitorSvc
{
public:
	typedef	enum _operation_type  {
		DO_NOTHING									= 0x00,         // 无处理
		DO_START_ANALYZE_SERVICE			= 0x01,         // 启动查询业务
		DO_START_DEFAULT_SERVICE			= 0x02		    // 停止查询业务
	}OPERATION_TYPE;

	CRechargeRWMonitorSvc();
	virtual ~CRechargeRWMonitorSvc();

	void ResetStartedByMonitorStatus();						// 重置启动状态，当前不是分析业务的时候重置。
protected:
	virtual bool Monitor();

private:
	bool m_bIsStartedByMonitor;
};