#pragma once
#include "GetIni.h"
#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"
#include "SysInfo.h"

#define theTVM_INFO CTVMInfo::GetInstance()

/** 
@brief   TVM.INI文件控制类
*/
class COMMON_API CTVMInfo : public CSYSInfo
{
public:

    static CTVMInfo&  GetInstance();

	//[计时器信息（单位：秒）]
	int GetFinishToReadyTimerInterval();	    				// [结束]->[等待中]定时器时间间隔
	int GetReceptToReadyTimerInterval();						// [操作中]->[等待中]定时器时间间隔
	int GetCountingToReturnTimerInterval(); 					// [计数中]->[返回中]定时器时间间隔
	int GetReadyToRestTimerInterval();							// [等待中]->[待机中]定时器时间间隔
	int GetReturnIndicatorTimerInterval();						// [开]->[关]找零指示灯
	int GetReturnLightTimerInterval();							// [开]->[关]找零照明灯

	// [读写器通讯参数]
	bool   IsRWUsed(RW_TYPE rwType);							// 读写器是否启用
	bool   IsUSBRW(RW_TYPE rwType);							// 读写器是否采用USB通信
	int     GetRWCommPort(RW_TYPE rwType);				// 获取读写器串口号
	int     GetRWBaudRate(RW_TYPE rwType);					// 获取读写器串口波特率
	int     GetRWDownLoadCnt(RW_TYPE rwType);			// 获取读写器数据下载重试次数
	int     GetRWTimeout(RW_TYPE rwType);					// 获取读写器读卡超时时间
	bool	 IsRWMonitorOpened(RW_TYPE rwType);			// 是否开启读写器监控
	int     GetRWMonitorInterval(RW_TYPE rwType);		// 读写器监控时间间隔

	//[周立功RFID通讯参数]
	bool IsZLGRfidUsed();														// 纸币接收模块是否启用
	int GetZLGRfidCommPort();												// 获取纸币接收模块串口号
	int GetZLGRfidBaudRate();												// 获取纸币接收模块串口波特率
	bool IsZLGRfidMonitorOpened();										// 是否开启纸币接收模块监控
	int GetZLGRfidMonitorInterval();										// 纸币接收模块监控时间间隔

	// [纸币接收模块通讯参数]
	bool IsBHUsed();														// 纸币接收模块是否启用
	int GetBHCommPort();												// 获取纸币接收模块串口号
	int GetBHBaudRate();												// 获取纸币接收模块串口波特率
	bool IsBHMonitorOpened();										// 是否开启纸币接收模块监控
	int GetBHMonitorInterval();										// 纸币接收模块监控时间间隔

	// [票卡模块通信参数]
	int GetTHCommuMethod();
	bool IsTHUsed();
	int GetTHCommPort();
	int GetTHBaudRate();
	int GetTHMaxConnReceive();
	int GetTHMaxConnSend();
	bool IsTHMonitorOpened();
	int GetTHMonitorInterval();
	int GetTHTestRelease();
	int GetTHTestRecycle();
	int GetTHBoxMediumType();

	// [TOKEN模块通信参数]
	bool IsTokenUsed();
	int GetTokenCommPort();
	int GetTokenBaudRate();
	int GetTokenMaxConnReceive();
	int GetTokenMaxConnSend();
	bool IsTokenTHMonitorOpened();
	int GetTokenMonitorInterval();
	int GetTokenTestRelease();
	int GetTokenTestRecycle();
	int GetTokenBoxMediumType();
	int GetTokenBoxAMaxCount();									// Token箱A最大安装票箱库存
	int GetTokenBoxBMaxCount();									// Tolen箱B最大安装票箱库存
	int GetTokenHopperAMaxCount();								// Token Hopper A最大库存容量
	int GetTokenHopperBMaxCount();								// Token Hopper B最大库存容量

	// [硬币模块参数]
	bool IsCHUsed();
	int GetCHCommPort();
	int GetCHBaudRate();
	bool IsCHMonitorOpened();
	int GetCHMonitorInterval();
	// Hopper1相关
	int GetCHHopper1Type();      // Hopper B类型
	int GetCHHopper1MaxCount();
	int GetCHHopper1NearEmpty();
	int GetCHHopper1Empty();
	
	// Hopper2相关
	int GetCHHopper2Type();		 // Hopper A类型
	int GetCHHopper2MaxCount();
	int GetCHHopper2NearEmpty();
	int GetCHHopper2Empty();
	 
	// 暂存
	int GetCHTemporary1Max();
	int GetCHTemporary2Max();

	// 找零限制
	int GetLimitOneCount();
	int GetLimitHalfCount();

	// 找零箱A
	int GetCHChangeBoxAType();	
	int GetCHChangeBoxAMax();
	int GetCHChangeBoxANearEmpty();
	int GetCHChangeBoxAEmpty();

	// 找零箱B
	int GetCHChangeBoxBType();
	int GetCHChangeBoxBMax();
	int GetCHChangeBoxBNearEmpty();
	int GetCHChangeBoxBEmpty();
	
	// 不适用郑州，删除
	//int GetCHStockMoneyType();
	//int GetCHStock1Max();
	//int GetCHTemporary1Max();
	//int GetCHTemporary2Max();
	//int GetCHStockEmpty();
	//int GetCHStockNearEmpty();
	//int GetCHHopper1Type();
	//int GetCHHopper1Max();
	//int GetCHHopper1AddMax();
	//int GetCHHopper1Empty();
	//int GetCHHopper1NearEmpty();
	//int GetCHHopper2Type();
	//int GetCHHopper2Max();
	//int GetCHHopper2AddMax();
	//int GetCHHopper2Empty();
	//int GetCHHopper2NearEmpty();
	//int GetCollectCoinPriorityforHopper1();			// 获取硬币专用找零箱A硬币回收优先级顺序
	//int GetCollectDirectionforHopper1();			// 获取硬币专用找零箱A硬币回收进入硬币回收箱编号
	//int GetCollectCoinPriorityforHopper2();			// 获取硬币专用找零箱B硬币回收优先级顺序
	//int GetCollectDirectionforHopper2();			// 获取硬币专用找零箱B硬币回收进入硬币回收箱编号
	//int GetCollectCoinPriorityforStock();			// 获取硬币循环找零箱硬币回收优先级顺序
	//int GetCollectDirectionforStock();				// 获取硬币循环找零箱硬币回收进入硬币回收箱编号

    // [凭证打印机通讯参数]
	bool    IsPrinterUsed();
    int     GetPrinterCommPort();                 // port open
    int     GetPrinterBaudRate();                   // Baud Rate (bps)
	bool	IsPrinterMonitorOpened();
	int		GetPrinterMonitorInternal();		
	// [IO通讯参数]
	bool IsIOUsed();										// IO是否启用
	int GetIOCommPort();								// 获取IO串口号
	int GetIOBaudRate();									// 获取IO波特率
	bool IsIOMonitorOpened();						// 是否启用IO监控
	int GetIOMonitorInterval();						// 获取IO监控时间间隔

	// [硬币口开关通讯参数]
	bool IsCoinShutterUsed();
	int  GetCoinShutterCommPort();
	int  GetCoinShutterBaudRate();
	bool IsCoinShutterMonitorOpened();
	int  GetCoinShutterMonitorInterval();

	// [密码键盘通讯参数]
	bool IsPinPadUsed();
	int  GetPinPadCommPort();
	int  GetPinPadBaudRate();
	bool IsPinPadMonitorOpened();
	int  GetPinPadMonitorInterval();

	// [银行卡模块通讯参数]
	bool IsBankCardUsed();
	int  GetBankCardCommPort();
	int  GetBankCardBaudRate();
	bool IsBankCardMonitorOpened();
	int  GetBankCardMonitorInterval();

	// [状态显示器通讯参数]
	bool IsSdspUsed();
	int GetSdspCommPort();
	int GetSdspBaudRate();
	bool IsSdspMonitorOpened();
	int GetSdspMonitorInterval();
	int GetSdspMonitorBrightness();
	int GetSdspMonitorMoveSpeed();
	int GetSdspMonitorStayStyle();
	int GetSdspMonitorStayTime();
	int GetSdspMonitorMoveIn();
	int GetSdspMonitorMoveOut();

	// 纸币找零模块通讯参数
	bool IsBDHUsed();
	int  GetBDHCommPort();
	int  GetBDHBaudRate();
	bool IsBDHMonitorOpened();
	int  GetBDHMonitorInterval();
	int GetBHChangeBoxAType();
	int GetBHChangeBoxBType();
	int GetBHChangeBoxNearFull();
	int GetBHChangeBoxFull();

	// [日志]
	CString GetLogPath();
	// 获取设备部件编码
	CString GetDeviceComponentCode(AFC_DEVICE_TYPE deviceType,DEVICE_SUBTYPE deviceSubType,long moudleID);

	// [程序目录]
	CString GetDataPath();
	CString GetBackupPath();

	// 以下是设置数据的方法
	// [TVM_RECHARGE_RW]
	void SetRechargeIsRWUsed(const CString&);   //充值读写器是否启用

	//	[TVM_CARD_RW]
	void SetSmartCardIsRWUsed(const CString&);  //Smart-Card读写器是否启用

	//	[TVM_TOKEN_RW]
	void SetTokenIsRWUsed(const CString&);		//Token读写器是否启用

	//	[BANKNOTE_ACCEPT_MODULE]]
	void SetBankNoteIsUsed(const CString&);		//纸币处理模块是否启用

	//	[BANKNOTE_CHANGE_MODULE]]
	void SetBankNoteChangeIsUsed(const CString&);//纸币找零模块是否启用

	//	[CARD_MODULE]
	void SetSmartCardIsUsed(const CString&);	//Smart-Card模块是否启用

	//	[TOKEN_MODULE]
	void SetTokenIsUsed(const CString&);		//Token模块是否启用

	//	[COIN_MODULE]
	void SetCoinIsUsed(const CString&);			//硬币模块是否启用

	//	[RP_COMMUNICATE]
	void SetReceiptIsUsed(const CString&);		//凭证打印机是否启用

	//	[PIN_PAD_COMMUNICATE]
	void SetPinPadIsUsed(const CString&);		//密码键盘模块

	//	[BANK_CARD_COMMUNICATE]
	void SetBankCardIsUsed(const CString&);		//银行卡模块

	int GetParameterUpdateBeginHour() const;
	int GetParameterUpdateBeginMin() const;
	int GetParameterUpdateEndHour() const;
	int GetParameterUpdateEndMin() const;
private:
    static CTVMInfo theInstance;

    // 隐藏构造函数
    CTVMInfo();
    ~CTVMInfo();
    CTVMInfo(const CTVMInfo&);
    CTVMInfo& operator=(const CTVMInfo&);

	CString m_strDataPath;						// 定义数据目录
	CString m_strDataBackpath;					// 定义数据备份目录
	CString m_strLogPath;						// 定义日志目录
};