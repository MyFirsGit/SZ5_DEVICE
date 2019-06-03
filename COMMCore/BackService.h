#pragma once
#include "Service.h"
#include "ServiceThread.h"
#include "CoreExp.h"

/**
@brief	后台服务基类
*/


class CORE_API CBackService :
    public CService
{
	DECLARE_MESSAGE_MAP()
public:
    CBackService(UINT serviceID, UINT parentServiceID, bool hasThread = true);
    ~CBackService();
	virtual BOOL PostMessage(UINT message,WPARAM wParam, LPARAM lParam);
	virtual LRESULT SendMessage(UINT Msg, WPARAM wParam, LPARAM lParam );
	CServiceThread* GetServiceThread();
	virtual bool StartTimer(UINT timerID, int seconds);         // 启动定时器
	virtual void StopTimer(UINT timerID);                       // 停止定时器
protected:
	 virtual bool OnStop();                                              // Service停止前处理
    CServiceThread* m_pServiceThread;                                   // Service线程
private:
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnStopTimer(WPARAM,LPARAM);
};
