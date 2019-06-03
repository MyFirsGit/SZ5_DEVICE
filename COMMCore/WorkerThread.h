#pragma once
#include "UtilThread.h"
#include "CoreExp.h"

/**
@brief	Service中使用的线程类                                                                   
*/

class CCommand;

class CORE_API CWorkerThread:
	public CUtilThread
{
	DECLARE_DYNCREATE(CWorkerThread)
	DECLARE_MESSAGE_MAP()


public:
	CWorkerThread(void);                                           // 构造函数
	~CWorkerThread(void);                                          // 析构函数
	void SetWorker(CCommand* worker);
	void SetInterval(UINT interval);

protected:
	virtual BOOL OnIdle(LONG lCount);

private:

	CCommand* m_worker;
	UINT m_Interval;
	
};
