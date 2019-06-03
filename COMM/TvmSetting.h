#pragma once
#include "GetIni.h"
//#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"
#include "CommonExp.h"

#define theTVM_SETTING CTvmSetting::GetInstance()

class COMMON_API CTvmSetting
{
public:
	static CTvmSetting&  GetInstance();
	int Initialize();                              // 初期化
	delegate<void()> Initialized;

	// === TVM系统信息 ===
	int     GetDelayOperationTime();							// 获取延长运营时间（单位:秒）

	// === FTP上传路径 ===
	CString GetFileUploadFTPDir();				// 获取文件上传路径
	void SetFileUploadFTPDir(CString);			// 设置文件上传路径

	// === 标志信息 ===
	bool  IsProgFirstRun();						// 程序是否是初次安装，第一次运行
	void SetIsFirstRun(bool bFirst);

	int     GetUpdateFlag();							// 获取程序更新标志
	int     GetACCUpdateFlag();					// 获取ACC更新失败标志
	// [ICRW]
	int GetRechargeRWUpdateFlag();				// 获取读写器UpdateError
	void SetRechargeRWUpdateFlag(int);		// 设置读写器UpdateError
	// [CardICRW]
	int GetCardRWUpdateFlag();						// 获取读写器UpdateError
	void SetCardRWUpdateFlag(int);				// 设置读写器UpdateError
	// [TokenICRW]
	int GetTokenRWUpdateFlag();					// 获取读写器UpdateError
	void SetTokenRWUpdateFlag(int);				// 设置读写器UpdateError
	// === 时间设定 ===
	int GetTimeChangeValue();					// 获取时间改变标志value
	void SetTimeChangeValue(int);				// 设置时间改变标志value
	CString GetCurSystemTime();					// 获取当前系统时间
	void SetCurSystemTime(CString);				// 设置当前系统时间
	// === 屏幕亮度设定 ===
	int GetScreenBrightnessValue();				// 获取屏幕亮度
	void SetScreenBrightnessValue(int);			// 设置屏幕亮度
	// === 运行模式设定 ===
	//void SetDegradeMode(bool);					//	设置是否允许降级运行
	//bool	IsAllowDegrade();					//	判断是否允许降级	

	int	GetServiceMode();						//	获取业务模式
	void SetServiceMode(int);					//	设置业务模式

	int GetPrintMode();							//	获取打印模式
	void SetPrintMode(int);						//  设置打印模式

	int GetPayMentMode();						//	获取支付模式
	void SetPayMentMode(int);					//	设置支付模式

	int	GetChangeMode();						//	获取找零模式
	void SetChangeMode(int);					//	设置找零模式

	bool IsSCServiceOutOfService();				//  是否上位下发暂停服务状态
	void SetSCServiceStatus(int);				//  设置上位下发服务状态

	bool IsStartOpeartion();					// 是否运营开始？
	void SetStartOperation(bool);				// 设置是否运开始

	// [ICRW]/[ICTW]
	int GetRWUpdateFlag(bool bIsRW = true);		// 获取读写器UpdateError
	void SetRWUpdateFlag(int,bool bIsRW = true);// 设置读写器UpdateError

	void SetDeviceTestMode(int);				// 设置设备测试模式
	int GetDeviceTestMode();					// 获取设备测试模式

	bool IsAllowDegrade();						//  是否允许自动降级模式
	void SetDeradedMode(int);					//  设置允许自动降级模式

	int  GetSettleSettings();					// 读取一键收机配置项
	void SetSettleSettings(int);


	int GetCardBoxLastAction();
	void SetCardBoxLastAction(int);

	int GetCollectTokenResult();
	void SetCollectTokenResult(int);

	int GetCoinBoxLastAction();
	void SetCoinBoxLastAction(int);

	int GetCollectCoinResult();
	void SetCollectCoinResult(int);

	int GetBanknoteBoxLastAction();
	void SetBanknoteBoxLastAction(int);

	int GetCollectBanknoteResult();
	void SetCollectBanknoteResult(int);

	CString GetBalanceOpTime();
	void SetBalanceOpTime(CString);

	// [TH]
	int GetTHMode();							// 获取出票机模式
	void SetTHMode(int);						// 设置出票机模式

	_DATE GetLastOnlineDate();					//  获取上次连线时间（年月日）
	void SetLastOnlineDate(_DATE dtDay);		//  设置上次连线时间（年月日）

	int GetMaxOffLineDay();						//  取最大离线运营天数
	void SetMaxOffLineDay(int nDays);			//  设置最大离线运营天数
	/*
	     串口号配置
	*/
	// 读写器[TVM_CHARGE_RW]
	int GetChargeRWComPort();
	int GetChargeRWBaudRate();
	// 读写器 [TVM_CARD_RW]
	int GetRWComPort();								// 获取读写器串口号
	int GetRWBaudRate();							// 获取读写器串口波特率

	void SetRwComPort(const CString&);				// 设置读写器串口号
	void SetRwBaudRate(const CString&);				// 设置读写器波特率

	// 纸币接收模块
	int GetBHComPort();								// 获取纸币接收模块串口号
	int GetBHBaudRate();							// 获取纸币接收模块串口波特率

	void SetBHComPort(const CString&);				// 设置纸币接收模块串口号
	void SetBHBaudRate(const CString&);				// 设置纸币接收模块波特率

	// 票卡模块
	int GetTHComPort();								// 获取票卡模块串口号
	int GetTHBaudRate();							// 获取票卡模块波特率
	int GetTHUpadateBaudRate();						// 获取票卡模块更新时波特率

	void SetTHComPort(const CString&);				// 设置票卡模块串口号
	void SetTHBaudRate(const CString&);				// 设置票卡模块波特率

	// token模块
	int GetTokenComPort();							// 获取票卡模块串口号
	int GetTokenBaudRate();							// 获取票卡模块波特率
	int GetTokenUpadateBaudRate();					// 获取票卡模块更新时波特率

	void SetTokenComPort(const CString&);				// 设置票卡模块串口号
	void SetTokenBaudRate(const CString&);				// 设置票卡模块波特率

	// token回收箱模块
	int GetTokenCyclerComPort();						// 获取token回收箱模块串口号
	int GetTokenCyclerBaudRate();						// 获取token回收箱模块波特率

	void SetTokenCyclerComPort(const CString&);			// 设置token回收箱模块串口号
	void SetTokenCyclerBaudRate(const CString&);		// 设置token回收箱模块波特率


	// 硬币模块
	int GetCHComPort();								// 获取硬币模块串口号
	int GetCHBaudRate();							// 获取硬币模块波特率

	int SetCHComPort(const CString&);				// 设置硬币模块串口号
	int SetCHBaudRate(const CString&);				// 设置硬币模块波特率

	// 硬币闸门模块
	int GetCSHComPort();
	int GetCSHBaudRate();
	int SetCSHComPort(const CString&);				// 设置硬币闸门模块串口号
	int SetCSHBaudRate(const CString&);				// 设置硬币闸门模块波特率

	// 打印机模块
	int GetPrinterComPort();						// 获取打印机模块串口号
	int GetPrinterBaudRate();						// 获取打印机模块波特率

	void SetPrinterComPort(const CString&);			// 设置打印机模块串口号
	void SetPrinterBaudRate(const CString&);		// 设置打印机模块波特率

	// IO
	int GetIOComPort();
	int GetIOBaudRate();

	void SetIOComPort(const CString&);
	void SetIOBaudRate(const CString&);

	// 状态显示器
	int GetDisplayComPort();						// 获取状态显示器串口号
	int GetDisplayBaudRate();						// 获取状态显示器波特率

	void SetDisplayComPort(const CString&);			// 设置状态显示器串口号
	void SetDisplayBaudRate(const CString&);		// 设置状态显示器波特率

	// 纸币找零模块
	int  GetBanknoteChangeComPort();				// 获取纸币找零模块串口号
	int  GetBanknoteChangeBaudRate();				// 获取纸币找零模块波特率

	void SetBanknoteChangeComPort(const CString&);	// 设置纸币找零模块串口号
	void SetBanknoteChangeBaudRate(const CString&); // 设置纸币找零模块波特率

	// 纸币找零RFID
	int GetBankchangeRFIDComPort();					// 获取纸币找零RFID串口号
	int GetBankchangeRFIDBaudRate();				// 获取纸币找零RFID波特率

	void SetBanknotechangeRFIDComPort(const CString&); // 设置纸币找零RFID串口号
	void SetBanknotechangeRFIDBaudRate(const CString&);// 设置纸币找零RFID波特率

	// 纸币循环模块配置
	CString GetBNRRecycleBoxAName();					// 纸币循环鼓A识别信息
	UINT    GetBNRRecycleBoxAId();						// 纸币循环鼓A的ID
	CString GetBNRRecycleBoxAMoneyType();				// 纸币循环鼓A的存储币种类型
	UINT	GetBNRRecycleBoxAAmount();					// 纸币循环鼓A的面额
	UINT	GetBNRRecycleBoxAMaxCount();				// 纸币循环鼓A的最大数量

	CString GetBNRRecycleBoxBName();					// 纸币循环鼓B识别信息
	UINT    GetBNRRecycleBoxBId();						// 纸币循环鼓B的ID
	CString GetBNRRecycleBoxBMoneyType();				// 纸币循环鼓B的存储币种类型
	UINT	GetBNRRecycleBoxBAmount();					// 纸币循环鼓B的面额
	UINT	GetBNRRecycleBoxBMaxCount();				// 纸币循环鼓B的最大数量

	CString GetBNRRecycleBoxCName();					// 纸币循环鼓C识别信息
	UINT    GetBNRRecycleBoxCId();						// 纸币循环鼓C的ID
	CString GetBNRRecycleBoxCMoneyType();				// 纸币循环鼓C的存储币种类型
	UINT	GetBNRRecycleBoxCAmount();					// 纸币循环鼓C的面额
	UINT	GetBNRRecycleBoxCMaxCount();				// 纸币循环鼓C的最大数量

	CString GetBNRRecycleBoxDName();					// 纸币循环鼓D识别信息
	UINT    GetBNRRecycleBoxDId();						// 纸币循环鼓D的ID
	CString GetBNRRecycleBoxDMoneyType();				// 纸币循环鼓D的存储币种类型
	UINT	GetBNRRecycleBoxDAmount();					// 纸币循环鼓D的面额
	UINT	GetBNRRecycleBoxDMaxCount();				// 纸币循环鼓D的最大数量

	CString GetBNRChangeBoxName();						// 纸币找零箱识别信息
	UINT    GetBNRChangeBoxId();						// 纸币找零箱的ID
	CString GetBNRChangeBoxMoneyType();					// 纸币找零箱的存储币种类型
	UINT	GetBNRChangeBoxAmount();					// 纸币找零箱的面额
	UINT	GetBNRChangeBoxMaxCount();					// 纸币找零箱的最大数量

	UINT	GetBNRCollectBoxId();						// 纸币回收箱的ID

	UINT	GetCoinChangeBox1Id();						// 硬币找零箱A id
	UINT	GetCoinChangeBox2Id();						// 硬币找零箱B id
	UINT	GetCoinCysleBoxId();						// 硬币循环箱id
	UINT	GetCoinCollectBoxId();						// 硬币回收箱ID

	// WEB
	void SetWebIdleProcessType(CString);
	int	GetWebIdleProcessType();
	void SetWebIdleProcessURL(CString);
	CString GetWebIdleProcessURL();

	// Recharge Server
	CString GetRechargeServeYKT_IP();
	int GetRechargeServeYKT_Port();
	CString GetRechargeServeYPT_IP();
	int GetRechargeServeYPT_Port();
private:
	// 隐藏构造函数
	CTvmSetting();
	~CTvmSetting();
	CTvmSetting(const CTvmSetting&);
	CTvmSetting& operator=(const CTvmSetting&);
	CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠

	static CTvmSetting theInstance;
	CGetIni* m_settingIni;
	
};