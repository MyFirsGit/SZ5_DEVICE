#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "delegate.h"
#include "CommonDefine.h"

/**
@brief	设备INI配置文件管理父类
*/

class  COMMON_API CSYSInfo
{
public:

	int Initialize();                              // 初期化
	
	//delegate<void()> Initialized;

    // [APPLICATION]-TVM系统信息
    CString GetInitialVersion();						// 获取软件版本信息
    bool      IsTopmost();									// 是否显示在最前面
	bool     CanShowCursor();							// 是否显示鼠标光标
	bool	    IsFunctionKeyEnabled();				// 是否启用功能键
	bool	    IsTaskManagerEnabled();				// 是否启用任务管理器
	bool     IsDesktopEnabled();						// 是否显示桌面
	bool		IsUsedGuardian();							// 是否开启守护进程
	int        GetStatusTimerInterval();				// 获取状态栏刷新时间间隔
	bool     IsAutoReturnDefaultService();				// （待机时有人靠近）是否自动返回默认业务（0：不自动；1：自动）
	bool     IsEnableClickSound();						//是否啓用按鍵音

	// [DISKFREESPACE]-磁盘空间配置（单位：M）
	int GetDataDirectoryWarn();					// 主数据目录警告
	int GetDataDirectoryStop();					// 主数据目录故障
	int GetBackupDirectoryWarn();				// 备份数据目录警告
	int GetBackupDirectoryStop();		    	// 备份数据目录故障
	int GetLogDirectoryWarn();					// 日志文件目录警告
	int GetLogDirectoryStop();						// 日志文件目录故障
	int GetSysDirectoryWarn();					// 日志文件目录警告
	int GetSysDirectoryStop();						// 日志文件目录故障

	// [LOG]-日志相关配置
    bool IsWriteDebug();                         // 是否记录Debug消息
	int	GetStoreDay();							// 获取日志保留天数
	void SetStoreDay(int nDays);				// 设置日志保留天数

	// 	[MACHINE]-网卡信息
	CString GetUsedAdapterName();		// 获取系统正在使用的网卡名称

	// [APP_PATH]-应用路径设置
	CString GetDataPath();								// 获取主数据目录
	CString GetBackupPath();							// 获取备份目录
	CString GetLogPath();								// 获取日志存放路径
	CString GetUSBDrive();								// 获取USB盘符
	CString GetCFDrive();								// 获取CF卡盘符
	CString GetExportPath();							// 获取USB数据导出子目录
	CString GetFileDownloadLocalDir();                  // 获取FTP临时下载路径
	CString GetGetReceiptLogPath();                     // 获取凭证数据目录

	int GetTransactionPackageInterval();			// 获取交易数据打包时间间隔
	int GetBusinessPackageInterval();				// 获取业务数据打包时间间隔

    CGetIni* m_DeviceIni;

	 // 构造析构函数
	 CSYSInfo();
	 ~CSYSInfo();
	 CSYSInfo(const CSYSInfo&);
	 CSYSInfo& operator=(const CSYSInfo&);
	 CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠

protected:
  
	CString GetRWTypeString(RW_TYPE rwType);
    CString GetRfidTypeString(RFID_NO rfidId);

	CString m_strDataPath;						// 定义数据目录
	CString m_strDataBackpath;					// 定义数据备份目录
	CString m_strLogPath;						// 定义日志目录
	CString m_strPrintPath;						// 打印存放目录
};