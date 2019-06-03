#pragma once
#include "UtilThread.h"
#include "CoreExp.h"

/**
@brief	Service��ʹ�õ��߳���                                                                   
*/

class CCommand;

class CORE_API CWorkerThread:
	public CUtilThread
{
	DECLARE_DYNCREATE(CWorkerThread)
	DECLARE_MESSAGE_MAP()


public:
	CWorkerThread(void);                                           // ���캯��
	~CWorkerThread(void);                                          // ��������
	void SetWorker(CCommand* worker);
	void SetInterval(UINT interval);

protected:
	virtual BOOL OnIdle(LONG lCount);

private:

	CCommand* m_worker;
	UINT m_Interval;
	
};
