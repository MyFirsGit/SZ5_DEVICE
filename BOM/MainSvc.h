#pragma once
#include "MenuDlg.h"
#include "BOMForeService.h"
#include "audithelper.h"

class CMainSvc :
    public CBOMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	typedef enum _tagTimers
	{
		TIMER_NTP = 1,			//NTP对时计时器
		TIMER_DEFAULT_SERVICE=2,	//默认业务定时器
		TIMER_AUTOMATIC_LOGOFF=3, //自动登出定时器
		TIMER_BUSIDAY_SWITH=4//运营日切换定时器
	}TIMERS;

    CMainSvc(void);
    ~CMainSvc(void);

    virtual void OnModeChange(UINT mode);                                   // 模式变化响应函数

    void CloseOperation(void);                                              // 业务结束
    void Shutdown(void);                                                    // 关机
	void RestartBOM();														// 重新启动
	void ExitBOM();
	void RepairInitException();
	void StartDefaultService();

	delegate<void (SERVICE_ID,bool*)> StartingForeService;

protected:
	void OnStart();
private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
    } DIALOG_GROUP;
    void ReleaseController();                                               // 释放Controller
	LRESULT OnForceLogout(WPARAM wParam,LPARAM lParam);
	LRESULT OnNetStatusChange(WPARAM wParam,LPARAM lParam);
	LRESULT OnShutDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseOperation(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartNtpTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartSwithBusinessDayTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnNewWorkMode(WPARAM wParam,LPARAM lParam);
	void  OnButtonClicking(CXButton*,bool*);
	void OnEditChanged(CGEdit*,CString);
	void OnTimer(UINT nEventID);
	_DATE currentBusiDate;
};
