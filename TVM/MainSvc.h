#pragma once
#include "MenuDlg.h"
#include "TVMForeService.h"
#include "audithelper.h"
#include "TVMRWHelper.h"
//#include "BanknoteModuleHelper.h"
#include "BHChangeHelper.h"
#include "BRHModuleHelper.h"
#include "CoinModuleHelper.h"
#include "PrinterHelper.h"
#include "TokenHelper.h"
#include "THHelper.h"
#include "IOModuleHelper.h"
#include "AudioHelper.h"
#include "StatusDisplayModuleHelper.h"
#include "DataHelper.h"
//#include "BHRfidHelper.h"
#include "GuiParaMgr.h"
#include "WebServerHelper.h"

class CMainSvc :
    public CTVMForeService
{

	DECLARE_MESSAGE_MAP()

public:
	typedef enum _tagTimers
	{
		TIMER_NTP				= 1,	// NTP对时计时器
		TIMER_DEFAULT_SERVICE	= 2,	// 默认业务定时器
		TIMER_AUTOMATIC_LOGOFF	= 3,    // 自动登出定时器
		TIMER_BUSIDAY_SWITH		= 4,    // 运营日切换定时器
		TIMER_IO_MONITOR		= 5,	// IO监控计时器
		TIMER_RETURN_INDICATOR	= 6,	// 找零指示灯计时器 
		TIMER_RETURN_LIGHT		= 7,	// 照明等计时器
		TIMER_READY_TO_REST		= 8,	// 等待中到待机中计时器
		TIMER_OUT_OF_TIME_LOGIN = 9,	// 等待登陆超时（后维护门开）
		TIMER_REPEAT_EVENT_AUDIO= 10,	// 语音重复播放计时器
		TIMER_BLIND_OPERATION_OUT_OF_TIME = 11,//盲人购票操作超时计时器
		TIMER_WEB_LOCAL_PROCESS	= 12,	// 二维码订单本地处理定时器
		TIMER_PARAMETER_UPDATE	= 13,	// SC下载参数加载定时器
	}TIMERS;

    CMainSvc(void);
    ~CMainSvc(void);

	void StartOperation();										// 运营开始
    void CloseOperation(void);                                  // 运营结束

	void ResumeService();										// 开始服务
	void PauseService();										// 暂停服务

    void Shutdown(void);                                        // 关机
	void RestartTVM();											// 重新启动
	void ExitTVM();												// 退出TVM程序
	void RepairInitException();									// 修复初始化异常
	void StartDefaultService();									// 启动默认服务
	void SetDegradedMode(bool bOnOrOff);						// 设置降级运行：开/关

	// IO Module
	void PostAssistanceButtonActionMSG();						// 发送招援按钮动作消息
	void PostDoorActionMSG();									// 发送维护门动作消息
	void FeelSomeCloseDevice();									// 距离传感器响应函数
	void WhenShockActive();										// 震动传感器响应
	void ReSetHelpStatus(){m_bIsCallHelpRunning = false;UPORDOWN ^= 1;};
	// 业务切换
	bool IsServiceBusyOnPassger();								// 判断当前service状态是否有乘客操作

	// 盲人购票
	void StartManagedBlind(BLIND_OPERATION_STATUS status);		// 托管售票和充值剩余的工作
	bool OnClickButtonWithoutPinPad(bool isStop = false);		// 当购票还在有MainSvc管理时点击了界面的按钮的处理。
	delegate<void (SERVICE_ID,bool*)> StartingForeService;
	delegate<void()> busidaySwitch;
	delegate<void()> hideAllCompoment;							// 托管返回默认业务时按钮的隐藏
	bool ResponseAssistBtn(UINT uMsg);                                       // 响应招援按钮

protected:
	void OnStart();
	// IO Module Rsp
	void CheckSetModuleStatus();											 // 判断切离设定状态
private:
    typedef enum _dialog_group {
        DIALOG_GROUP_BASE = 1,
		DIALOG_GROUP_NAVI = 2,
    } DIALOG_GROUP;
    void ReleaseController();                                               // 释放Controller
	LRESULT OnForceLogout(WPARAM wParam,LPARAM lParam);
	LRESULT OnNetStatusChange(WPARAM wParam,LPARAM lParam);
	LRESULT OnShutDown(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseOperation(WPARAM wParam,LPARAM lParam);
	LRESULT OnDeviceControl(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartNtpTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnStartSwithBusinessDayTimer(WPARAM wParam,LPARAM lParam);
	LRESULT OnNewWorkMode(WPARAM wParam,LPARAM lParam);
	LRESULT OnNewOperationMode(WPARAM wParam,LPARAM lParam);				// 响应运营模式控制
	LRESULT OnStartIOMonitor(WPARAM wParam,LPARAM lParam);					// 响应IO监控消息
	LRESULT OnStartReturnIndicator(WPARAM wParam,LPARAM lParam);			// 响应找零指示灯监控消息
	LRESULT OnStartReturnLight(WPARAM wParam,LPARAM lParam);				// 响应找零照明灯监控消息
	LRESULT OnStartReadyToRest(WPARAM wParam,LPARAM lParam);				// 响应等待中到待机中监控消息
	LRESULT OnFrontDoorClose(WPARAM wParam,LPARAM lParam);					// 响应关门操作消息
	LRESULT OnFrontDoorOpenWithoutLogin(WPARAM wParam,LPARAM lParam);		// 启动门开未登陆计时器
	LRESULT OnFrontDoorOpenTimeOutOperation(WPARAM wParam,LPARAM lParam);	// 启动操作员操作超时计时器
	LRESULT OnPinPadMonitorReturn(WPARAM wParam,LPARAM lParam);				// 密码键盘监控返回
	LRESULT OnSwitchBusinessDayAndCloseOperation(WPARAM wParam,LPARAM lParam);// 执行运营日切换及日结
	LRESULT OnServiceTimerControl(WPARAM wParam,LPARAM lParam);				// 服务控制定时器
	LRESULT OnWebIdleProcess(WPARAM wParam,LPARAM lParam);					// 二维码订单本地处理
	LRESULT OnRechargeServerMonitor(WPARAM wParam,LPARAM lParam);			// 充值服务器检测
	LRESULT OnParameterUpdateProcess(WPARAM wParam,LPARAM lParam);			// SC下载参数加载

	void	OnButtonClicking(CXButton*,bool*);
	void	OnEditChanged(CGEdit*,CString);
	void	OnTimer(UINT nEventID);
	void	GetPaymentInfoFromFuncIni(WORD& device_pay_status);				// 在Function.ini中获取支付方式
	void    StartOutOfService();											// 启动暂停服务

	void    JudgeIsOperatorLogin();											// 判断站员是否登陆
	bool    IsAutoDownGradeNeedWarn();										// 判断是否有自动降级提示
	void	PlayEventAudio(DWORD eventID,bool bStopRepeat = false);			// 播放语音接口
	_DATE m_CurrentBusiDate;												// 当前运营日
	int UPORDOWN;															// 按钮当前状态
	int OPENORCLOSE;														// 维护门当前状态
	int SOMEONECLOSE;														// 有人靠近TVM
	int WHENSHOCKACTIVE;													// 震动传感器响应
	IO_RSP_STATUS_INFO m_IOStatusRsp;										// IO当前状态
	bool m_bIsAramOpen;														// 报警器是否响应？
	bool m_bIsBeginRunning;													// 判断当前是否开始运营
	bool m_bIsEndRunning;													// 判断当前是否结束运营
	bool m_bIsStartingDefaultServic;										// 是否正在启动默认业务？
	bool m_bIsCallHelpRunning;												// 招援是否正在进行?

	bool m_bIsMenualRepare;													// 是否手动解除异常
	/*************************上位控制判断*******************/
	// 上位发送命令后状态
	bool CheckSvcStatus();													// 对应上位下发状态
	bool m_bIsSvcMsgComeSystemBusy;											// 上位下发命令，下位忙碌？
	DEVICE_CONTRL_CODE m_device_contrl_code;								// 上位下发命令代码
	// 模式控制
	OperationalMode_t m_operational_mode;									// 上位下发的模式控制代码
	bool m_bIsOperaArryBusy;												// 上位下发模式控制代码时，设备忙碌

	bool m_bIsServerMsgArrial;												// 上位有消息下发，在停止服务时
	bool m_bBeforeRunCommandArrial;											// 运营开始前上位下发消息
	bool m_bEndRuningCommandArrial;											// 运营结束后上位下发消息

	// 运营切换
	void OnSwitchBussinessDay();											// 响应运营切换
	bool m_bIsSwitchBussinessDay;											// 切换运营日

	// 运营控制
	bool m_bIsStartOperation;												// 是否已经开始运营？（非运营时间内上位下发运营开始需要记录在配置文件）
	bool m_bIsReadyStartOperation;											// 运营时间到了或者在运营时间中开机是否运营开始了？
	bool m_bIsStartOperationWhenOpen;										// 开机走过时序没有？

	bool m_bPuseCheckOperation;												// 是否暂停运营日切换检测

private:
	int m_timerInterval;
private:
	CTVMRWHelper* m_pRWHelper;
	CTVMRWHelper* m_pRechargeHelper;
	CPrinterHelper* m_pPrinterHelper;
	CTHHelper* m_pCardModuleHelper;
	CTOKENHelper* m_pTokenHelper;
	CIOModuleHelper* m_pIOHelper;
	//CBanknoteModuleHelper* m_pBanknoteModuleHelper;
	CBRHModuleHelper* m_pBanknoteModuleHelper;
	//CBHChangeHelper *m_pBHChangeHelper;
	CBRHModuleHelper* m_pBHChangeHelper;
	CCoinModuleHelper* m_pCoinModuleHelper;
	//CStatusDisplayModuleHelper* m_pStatusDisplayModuleHelper;
	CDataHelper* m_pDataHelper;
	CAudioHelper* m_pAudioHelper;
	void CheckIniValue(WORD& wService_Status,WORD& wPayment_Status,WORD& wChange_Status);// 检测配置文件配置合法性
public:
	CTVMRWHelper* GetRWHelper();

	CPrinterHelper*	GetPrinterHelper();
	CTHHelper* GetCardModuleHelper();
	CIOModuleHelper* GetIOModuleHelper();
	//CBanknoteModuleHelper* GetBanknoteModuleHelper();
	CBRHModuleHelper* GetBanknoteModuleHelper();
	//CBHChangeHelper* GetBHChangeHelper(); 
	CBRHModuleHelper* GetBHChangeHelper(); 
	CCoinModuleHelper* GetCoinModuleHelper();
	//CStatusDisplayModuleHelper* GetStatusDisplayModuleHelper();
	CDataHelper* GetDataHelper();
	CAudioHelper* GetAudioHelper();

	// 日志控制
public:
	void DeleteLog();														// 删除所有过期日志
	void DeleteLog(int days);												// 删除指定天数前的FTP上传的日志
	void DeleteTpuLog(int days);											// 删除指定天数前的日志文件
	void DeletePrintHistory(int days);										// 删除打印历史记录
private:
	void GetLimitTpuLogDate(int days,vector<CString>& logdate);				// 获取指定天数的Tpulog日志文件日期
	void GetLimitLogDate(int days,vector<int>& logdate);					// 获取指定天数的log日志文件日期
	void GetLimitPrintLogDate(int days,CString strPath,vector<CString>& vecNeedDel,bool bIsDir = false);// 取得指定目录下指定类型应该删除天数之外的文件
};

extern CTVMRWHelper* RECHARGERW_HELPER;
extern CTVMRWHelper* CARDRW_HELPER;

extern CPrinterHelper* PRINTER_HELPER;
extern CTHHelper* CARD_HELPER;
extern CTOKENHelper* TOKEN_HELPER;
extern CIOModuleHelper* IO_HELPER;
//extern CBanknoteModuleHelper* BANKNOTE_HELPER;
extern CBRHModuleHelper* BANKNOTE_HELPER;
//extern CBHChangeHelper * BH_CHANGE_HELPER;
extern CBRHModuleHelper * BH_CHANGE_HELPER;
extern CCoinModuleHelper* COIN_HELPER;
extern CDataHelper* DATA_HELPER;
extern CAudioHelper* AUDIO_HELPER;
//extern CBHRfidHelper* BH_RFID_HELPER;