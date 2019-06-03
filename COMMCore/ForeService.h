#pragma once
#include "DialogFlow.h"
#include "Service.h"
#include "CoreExp.h"

//#include "BR.h"


/**
@brief	前台服务基类
*/

class CORE_API CForeService :
    public CService
{
		DECLARE_MESSAGE_MAP()
public:
    CForeService(UINT serviceID, UINT parentServiceID,CDialogFlow* dialogFlow = NULL);      // 构造函数
    ~CForeService();                                                                // 析构函数

	virtual BOOL PostMessage(UINT message,WPARAM wParam, LPARAM lParam);
	virtual LRESULT SendMessage(UINT message,WPARAM wParam, LPARAM lParam);
  
    bool static IsWaitingCommand();                                                 // 是否在等待命令结束中
    void static SetWaitingCommand(bool isWaiting);                                  // 设置等待命令结束中
	void DoDialogFlow(const TCHAR* pButtonMsg);                                      // 处理画面跳转按钮消息
	CDialogFlow* GetDialogFlow();
	virtual bool StartTimer(UINT timerID, int seconds);         // 启动定时器
	virtual void StopTimer(UINT timerID);                       // 停止定时器
protected:
	virtual void OnStart();                                                         // Service启动后处理
	virtual bool OnStop();                                                          // Service停止前处理
    CDialogFlow* m_pDialogFlow;                                                     // 对话框流转控制
private:
    bool static m_isWaitingCommand;                                                 // 是否在等待命令结束中
	LRESULT OnStartTimer(WPARAM,LPARAM);
	LRESULT OnStopTimer(WPARAM,LPARAM);
};
