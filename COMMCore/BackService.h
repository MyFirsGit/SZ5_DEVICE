#pragma once
#include "Service.h"
#include "ServiceThread.h"
#include "CoreExp.h"

/**
@brief	��̨�������
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
	virtual bool StartTimer(UINT timerID, int seconds);         // ������ʱ��
	virtual void StopTimer(UINT timerID);                       // ֹͣ��ʱ��
protected:
	 virtual bool OnStop();                                              // Serviceֹͣǰ����
    CServiceThread* m_pServiceThread;                                   // Service�߳�
private:
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnStopTimer(WPARAM,LPARAM);
};
