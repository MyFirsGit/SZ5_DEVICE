#pragma  once
#include "MonitorSvc.h"
/*
��ֵ��д�������
*/
class CRechargeRWMonitorSvc: public CMonitorSvc
{
public:
	typedef	enum _operation_type  {
		DO_NOTHING									= 0x00,         // �޴���
		DO_START_ANALYZE_SERVICE			= 0x01,         // ������ѯҵ��
		DO_START_DEFAULT_SERVICE			= 0x02		    // ֹͣ��ѯҵ��
	}OPERATION_TYPE;

	CRechargeRWMonitorSvc();
	virtual ~CRechargeRWMonitorSvc();

	void ResetStartedByMonitorStatus();						// ��������״̬����ǰ���Ƿ���ҵ���ʱ�����á�
protected:
	virtual bool Monitor();

private:
	bool m_bIsStartedByMonitor;
};