#pragma once
#include "tvmforeservice.h"
#include "FTPCommand.h"

/**
@brief    保守业务 - 硬件自检业务
*/
class CNetworkTestSvc :
	public CTVMForeService
{
	DECLARE_MESSAGE_MAP()

public:
	CNetworkTestSvc();
	virtual ~CNetworkTestSvc();

	BOOL PingSC(char* pBuff);										// ping上位服务器
	static const int MAX_PING_RESULT_LLENGTH = 2000;				// 结果信息最大长度
public:
	virtual void NotifyDataModelChanged();
private:
	typedef enum _dialog_group 
	{
		DIALOG_GROUP_NETWORK   =  1,                                // 网络测试画面组
	} DIALOG_GROUP;

private:
	//消息声明
	LRESULT OnKeyboardEnter(WPARAM , LPARAM );              //处理 [Enter] 键按下消息（执行）
	//virtual LRESULT OnKeyboardNumber(WPARAM wParam, LPARAM );       //处理 [0~9]   键按下消息（数字）
	LRESULT OnKeyboardF3(WPARAM , LPARAM );                 //处理 [F3/Ctrl]    键按下消息（后退）
	void OnStart();
};