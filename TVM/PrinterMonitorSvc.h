#pragma once
#include "MonitorSvc.h"
/*
��ӡ����ط���
*/
class CPrinterMonitorSvc : public CMonitorSvc
{
public:
	CPrinterMonitorSvc();
	virtual ~CPrinterMonitorSvc();
	void PostPrinterEmptyMSG();		// ���ʹ�ӡ��ȱֽ��Ϣ
protected:
	virtual bool Monitor();
	bool m_isError;
};