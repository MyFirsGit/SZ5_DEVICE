#pragma once
#include "MonitorSvc.h"
/*
ֽ�Ҵ���ģ������
*/
class CBanknoteCollectionModuleMonitorSvc:public CMonitorSvc
{
public:
	CBanknoteCollectionModuleMonitorSvc();
	virtual ~CBanknoteCollectionModuleMonitorSvc();
protected:
	virtual bool Monitor();
private:
	bool m_isAlarmOpend;
	BOOL DO_OPEN_ALARM_FOR_Banknotebox;
};