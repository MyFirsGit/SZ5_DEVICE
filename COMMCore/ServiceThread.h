#pragma once
#include "UtilThread.h"
#include "service.h"
#include "CoreExp.h"

/**
@brief	Service中使用的线程类                                                                   
*/
class CORE_API CServiceThread :
    public CUtilThread
{
    DECLARE_DYNCREATE(CServiceThread)
    DECLARE_MESSAGE_MAP()
	
	typedef struct _ThreadSendMessageStruct
	{
		UINT message;
		WPARAM wParam;
		LPARAM lParam;
		LRESULT lResult;
		HANDLE handle;
		_ThreadSendMessageStruct()
		{
			message = 0;
			wParam =0;
			lParam=0;
			lResult = SP_ERR_INTERNAL_ERROR;
			handle = INVALID_HANDLE_VALUE;
		}
	} THREADSENDMESSAGESTRUCT,*LPTHREADSENDMESSAGESTRUCT;

public:
    CServiceThread(void);                                           // 构造函数
    ~CServiceThread(void);                                          // 析构函数
	void SetService(CService* pService);
    bool IsBusy();                                                  // 是否忙碌
	int GetPendMsgCount();											// 获取未处理信息数
	BOOL PostThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam);
	LRESULT SendThreadServiceMsg(UINT msg,WPARAM wParam,LPARAM lParam);
private:
	void OnPostServiceMsg(WPARAM wParam, LPARAM lParam);	//服务消息
	void OnSendServiceMsg(WPARAM wParam,LPARAM lParam);
    void DispachCmdMsg(UINT message, WPARAM wParam, LPARAM lParam); // 调用Service命令响应函数
	
    CService* m_pService;                                           // 所属的Service
    int m_pendMsgCount;                                             // 未处理的信息数
};
