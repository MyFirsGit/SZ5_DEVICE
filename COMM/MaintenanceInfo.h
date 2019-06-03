#include "GetIni.h"
#include "CommonExp.h"
#include "CommonDefine.h"
#pragma once

#define theMAINTENANCE_INFO CMaintenanceInfo::GetInstance()       // CMaintenanceInfo的唯一接口

/**
@brief Maintenance.INI的信息管理

@attention
该文件中可以通过上位或保守进行修改的项目需提供相应的Get和Set方法，
并定义相应的成员变量。一般Maintenance.INI中项目的修改不需要即时生效，
所以Set方法只要修改文件，而无需修改对应的成员变量。
*/
class COMMON_API CMaintenanceInfo
{
public:

    static CMaintenanceInfo&  GetInstance();

    int Initialize();                              // 初期化

    // 以下是取得数据的方法
	AFC_DEVICE_TYPE GetDeviceType();					// 获取设备类型
	DEVICE_SUBTYPE GetDeviceSubType();				// 获取设备子类型
	CString GetProgName();									// 获取程序名称，用于名称合成，例如：BOM和VM的名称都为BOM，Card、Token、PM都为ES.....

	void SetDeviceTestMode(int);			// 设置设备测试模式
	int GetDeviceTestMode();					// 获取设备测试模式

    // [CPS]
    CString GetCpsHostID();                         // 获取 Cps HostID
    int     GetCpsPort();                           // 获取 CPS PORT

    // [NTP]
    CString GetNtpHostID();                         // 获取 Ntp HostID
    int     GetNtpPort();                           // 获取 Ntp Port

	// [CHARGE_IP]
	CString GetYPTHostID();                         // 获取 YPT HostID
	int     GetYPTPort();                           // 获取 YPT Port
	int     GetYPTTimeOutTime();                       // 获取 YPT Port

	// [ECT_CHARGE_IP]
	CString GetECTHostID();                         // 获取 ECT HostID
	int     GetECTPort();                           // 获取 ECT Port
	int     GetECTTimeOutTime();                       // 获取 ECT Port

	// [FTP]
    CString GetFtpHostID();                         // 获取 Ftp HostID
    int     GetFtpPort();                           // 获取 FTP Port
	CString GetFtpUserID();
	CString GetFtpPwd();

	// [镭射打印机]
	CString GetLaserPrinterHostID();                         // 获取 镭射打印机 HostID
	CString GetLaserPrinterName();                           // 获取 镭射打印机 名称

    // [MACHINE]
	BYTE GetDeviecTypeID();
    int     GetStationCode();
    int     GetLineCode();
	int     GetTPUStationCode();
	WORD	GetStationCodeWithLine();
    unsigned char GetMachineCode();
	DEVICE_ID& GetCurrentDevice();
	AccDeviceID_t& GetCurrentACCDevice();
	TPU_DEVICE_ID& GetCurrentTPUDevice();
    CString GetIP();
    CString GetSubnetMask();
    CString GetGateway();
    int     GetGroupNumber();
    int     GetEquNumberInGroup();
	void SetLanguageMode(LANGUAGETYPE_T);					// 设置语言模式
	LANGUAGETYPE_T GetLanguageMode();			            // 获取语言模式


    // 以下是设置数据的方法
    // [CPS]
    void SetCpsHostID(const CString&);
    void SetCpsPort(const CString&);

    // [NTP]
    void SetNtpHostID(const CString&);
	void SetNtpPort(const CString&);

    // [FTP]
    void SetFtpHostID(const CString&);
	void SetFtpPort(const CString&);
	void SetFtpUserID(const CString&);
	void SetFtpPwd(const CString&);
	// [YPT]
	void SetYPTHostID(const CString&);
	void SetYPTPort(const CString&);
	// [ECT]
	void SetECTHostID(const CString&);
	void SetECTPort(const CString&);

	// [PRINTER]
	void SetPrinterHostID(const CString&);
	void SetPrinterName(const CString&);

    // [MACHINE]
	void SetLineCode(int);
    void SetStationCode(int);
    void SetMachineCode(int);
    void SetGroupNumber(int);
    void SetEquNumberInGroup(int);
    void SetIP(const CString&);
    void SetSubnetMask(const CString&);
    void SetGateway(const CString&);
	CString GetLinkName();


private:
    static CMaintenanceInfo theInstance;
    CGetIni* m_maintenanceIni;

    // 重启动后生效的参数
    // [CPS]
    CString m_cpsHostID;                // CPS HOST IP
    int     m_cpsPort;                  // CPS 端口

    // [NTP]
    CString m_ntpHostID;                // NTP HOST IP
    int     m_ntpPort;                  // NTP 端口
	
	// [CHARGE_IP]
	CString m_YPTHostID;                // YPT HOST IP
	int     m_YPTPort;                  // YPT 端口
	int     m_YPTTimeOutTime;              // YPT 端口

	// [ECT_CHARGE_IP]
	CString m_ECTHostID;                // ECT HOST IP
	int     m_ECTPort;                  // ECT 端口
	int     m_ECTTimeOutTime;              // ECT 端口

	// [FTP]
    CString m_ftpHostID;                // FTP HOST IP
    int     m_ftpPort;                  // FTP 端口
	CString m_ftpUserId;				// FTP用户ID
	CString m_ftpPwd;					// FTP密码

	// [PRINTER]
	CString m_printerHostID;                // PRINTER HOST IP
	CString m_printerName;                  // PRINTER 名称

    // [BMAC]
    CString m_BMACHostID;               // BMAC HOST IP
    int     m_BMACPort;                 // BMAC 端口

    // [MACHINE]
	int		m_lineCode;					//线路代码
    int     m_stationCode;              // 车站代码
    int     m_machineCode;              // 本机代码
    int     m_groupNumber;              // 组号
    int     m_equNumberInGroup;         // 组内编号

	CString m_deviceType;
	BYTE m_deviceTypeID;
	LANGUAGETYPE_T m_LanguageMode;
    CString m_IP;                       // 本机地址
    CString m_subnetMask;               // 掩码
    CString m_gateway;                  // 网关地址
    CString m_stuffID;                  // 员工ID
    CString m_stuffPassword;            // 员工密码
    CString m_RWBootVersion;            // 外部读写器BOOT版本
    CString m_THRWBootVersion;          // 内部读写器BOOT版本

	DEVICE_ID m_CurrentDevice;
	TPU_DEVICE_ID m_CurrentTPUDevice;
	AccDeviceID_t m_CurrentAccDevice;
    // 隐藏构造函数
    CMaintenanceInfo();
    ~CMaintenanceInfo();
    CMaintenanceInfo(const CMaintenanceInfo&);
    CMaintenanceInfo& operator=(const CMaintenanceInfo&);
    CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠
};