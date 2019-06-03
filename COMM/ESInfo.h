#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "CommonDefine.h"
#include "SysInfo.h"

#define theES_INFO CESInfo::GetInstance()

class COMMON_API CESInfo  : public CSYSInfo
{
public:
    static CESInfo&  GetInstance();
  //  int Initialize();                              // 初期化
	
	//delegate<void()> Initialized;

    // 以下是取得数据的方法

    // [APPLICATION]
    CString GetInitialVersion();                    // 获取初期安装的版本信息
    bool    IsTopmost();                            // 是否显示在最前面
	bool	IsFunctionKeyEnabled();					// 功能键是否可用
	bool	IsTaskManagerEnabled();					// 任务管理器是否可用
	bool    IsDesktopEnabled();						// 桌面是否可用
    bool    CanShowCursor();                        // 是否显示鼠标光标
	CString GetLogPath();							// 获取日志存放路径
    CString GetDataPath();                          // 获取主数据目录
    CString GetBackupPath();                        // 获取备份目录
    bool    IsWriteDebug();                         // 是否记录Debug消息
	bool IsUsedGuardian();							// 是否开启守护进程
	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();
	int GetUSBAuthFlag();							// 获取USB认证方式（0：不认证；1：需要认证）
	int GetStatusTimerInterval();					// 获取状态栏刷新时间间隔
	bool IsUseFounderStaffID();							// 是否启用默认员工
	int GetFounderStaffID();							// 获取默认员工ID
	bool IsFounderStaffID(int userID);
	CString GetFounderStaffPassword();					// 获取默认员工密码
	int GetCloseOperationMaxWaitSeconds();				// 获取业务结束最大等待时间
	
	bool IsValidateUnSendPackage();
	CString GetUsedAdapterName();

    // [MAGAZINE]
    int GetMagazineCommPort();
    int GetMagazineBaudRate();
	bool IsMagazineMonitorOpened();
	int GetMagazineMonitorInterval();
	int GetPressCardMaxCount();
	int GetMagazineMaxCapacity();

	// [ICRW]
	bool IsRWUsed(RW_TYPE rwType);
	bool IsRWMonitorOpened(RW_TYPE rwType);
	int GetRWMonitorInterval(RW_TYPE rwType);
    int GetRWCommPort(RW_TYPE rwType);
    int GetRWBaudRate(RW_TYPE rwType);
	int GetSimCardType(RW_TYPE rwType);
	int GetReadCardInterval(RW_TYPE rwType);
	int	GetMasterCardSlot(RW_TYPE rwType);
	int	GetTrandferCardSlot(RW_TYPE rwType);
	int GetPinLen(RW_TYPE rwType);
	int GetPinBuffer(RW_TYPE rwType);


    // [PRINTER]
	bool    IsPrinterUsed();
    int     GetPrinterCommPort();                   // port open
	int     GetPrinterBaudRate();                   // Baud Rate (bps)
	int     GetPrintType();                         //打印字体
	int     GetPrintDiret();                        //打印方向
	int     GetPrintRowDis();                       //行间距

	// [CARD_PRINTER]
	bool    IsCardPrinterUsed();
	CString GetCardPrinterDriverName();            
	CString GetPrinterTempFile();
	int		GetCardType();
	int		GetTestSendCardCount();					 // 发卡测试次数

	// [ES_TOKEN]
	int		GetTOKEN_ESCommPort();
	bool	IsTOKEN_ESUsed();
	bool	IsModuleUsed(ES_MODULE_NUMBER moduleID);

	// [ES_CARD]
	bool	IsCARD_ESUsed();
	int		GetCARD_ESCommPort();
	int		GetCARD_ESBaudRate();

    // [UPS]
	bool IsUPSUsed();
    int GetUPSCommPort();
    int GetUPSBaudRate();
	bool IsUPSMonitorOpened();
	int GetUPSMonitorInterval();

	// [LPT]
	bool IsLPTUsed();
	bool IsLPTMonitorOpened();
	int GetLPTMonitorInterval();
	DWORD GetClosedDoorStatusValue();
   
	// [MOVEDISK]
    CString GetBOMUSBDrive();
	CString GetBomExportPath();  

    // [LOG]
    int     GetStoreDay();
	int     GetRealStoreDay();

	int GetTransactionPackageInterval();
	int GetBusinessPackageInterval();
	
	// RFID
	bool IsRfidUsed(RFID_NO rfidId);		// 获取RFID是否使用
	int GetRfidCommPort(RFID_NO rfidId);	// 获取RFID串口号	
	int GetRfidBaudRate(RFID_NO rfidId);	// 获取RFID波特率	
	int GetRfidStopBit(RFID_NO rfidId);		// 获取RFID停止位	
	CString GetRfidParity(RFID_NO rfidId);	// 获取RFID奇偶校验	
	int GetRfidByteSize(RFID_NO rfidId);	// 获取RFID字节大小	
	int GetRfidInputSize(RFID_NO rfidId);	// 获取RFID串口输入缓冲大小	
	int GetRfidOutputSize(RFID_NO rfidId);	// 获取RFID串口输出缓冲大小	

	// 上送数据
	int GetTxnIntervalTime();			// 获取计时器时间
	int GetAuditInterval();				// 获取包传输审计计时
	int GetBusinessInterval();			// 业务数据的上送时间
    bool IsDeleteParameterHead();						// 是否删除参数协议头和尾部
	// 获取设备部件编码
	CString GetDeviceComponentCode(long);
private:
    static CESInfo theInstance;
   // CGetIni* m_DeviceIni;

    // 隐藏构造函数
    CESInfo();
    ~CESInfo();
    CESInfo(const CESInfo&);
    CESInfo& operator=(const CESInfo&);
    //CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠

	CString GetRWTypeString(RW_TYPE rwType);
	CString GetRfidTypeString(RFID_NO rfidId);
};