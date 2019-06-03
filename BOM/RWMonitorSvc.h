#pragma  once
#include "MonitorSvc.h"
/*
Íâ²¿¶ÁÐ´Æ÷¼à¿ØÀà
*/
class CRWMonitorSvc: public CMonitorSvc
{
public:
	CRWMonitorSvc();
	virtual ~CRWMonitorSvc();
protected:
	virtual bool Monitor();
private:
	CRWHelper* m_RWHelper;
};