#pragma  once
#include "MonitorSvc.h"
/*
�ⲿ��д�������
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