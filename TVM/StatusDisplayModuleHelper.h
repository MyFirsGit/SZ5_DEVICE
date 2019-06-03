#pragma once
#include "ServiceHelper.h"
#include "StatusDisplayCommand.h"

#define theSTATUS_DISPLAYER CStatusDisplayModuleHelper::GetInstance()

/**
* 定义LED显示类型
*/
typedef enum _em_show_detail_{
	MSG_IN_SERVICE = 0,
	MSG_ONLY_SALE,
	MSG_ONLY_EXCHANGE,
	MSG_ONLY_COIN,
	MSG_ONLY_BANKNOTE,
	MSG_ONLY_QR,
	MSG_ONLY_CASH,
	MSG_NO_COIN,
	MSG_NO_BANKNOTE,
	MSG_NO_CHANGE,
	MSG_OUT_OF_SVC,
	MSG_STOP_SVC,
	MSG_INTERNET_TICKETS,
	MSG_ONLY_RECHARGE,
	MSG_ONLY_QUERY,
}SHOW_DETAILE;

// 语言对应Ini配置文件
#define CN_ID				1
#define EN_ID				2
#define PN_ID				3

/**
@brief  LED 显示屏控制类
*/
class CStatusDisplayModuleHelper 
{
public:
	~CStatusDisplayModuleHelper(void);				// 析构函数
	static CStatusDisplayModuleHelper& GetInstance();
public:
	long Connect();								//打开串口
	long Init();
	long Close();								// 关闭串口
	bool Repare();								// 尝试修复条屏

	long showTVMStatus(WORD wbStatusCode);	//显示运营状态
	long showWorkMode();					//显示当前工作模式
	long showChargeMode();					//显示充值模式
	long showPaymentMode();					//显示支付模式

private:
	CStatusDisplayCommand m_StatusDisplay;
	CStatusDisplayModuleHelper();			// 构造函数
	static CStatusDisplayModuleHelper theInstance;
	void ShowCurWorkMode();					// 显示当前模式
	long ShowModeFunction(int modeValue);
	DWORD m_wCurrentStatus;
};
