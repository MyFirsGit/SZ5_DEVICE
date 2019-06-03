#pragma once
#include "BackService.h"

class  CMonitorSvc : public CBackService
{
	DECLARE_MESSAGE_MAP()
public:

	CMonitorSvc(UINT serviceID, UINT parentServiceID, int interval,bool puase=true);
	virtual ~CMonitorSvc();
	void Pause();
	void Resume();
	bool IsPause();

	typedef enum _TIMER_ID
	{
		MONITOR_TIMER = 1,
	} TIMER_ID;
protected:
	virtual bool Monitor() = 0;
	virtual void OnStart();
	virtual bool OnStop();
	virtual void OnResume();
private:	
	bool m_PauseFlag;
	int m_Interval;
	LRESULT StartMonitorTimer(WPARAM wParam,LPARAM lParam);
	void OnTimer(UINT);
};
