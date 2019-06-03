#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "delegate.h"

#define theTCM_INFO CTCMInfo::GetInstance()

class COMMON_API CTCMInfo
{
public:

    static CTCMInfo&  GetInstance();

    int Initialize();                              // 初期化
	
	delegate<void()> Initialized;

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
	CString GetCompanyNameCn();                     // 获取运营商中文名
	CString GetCompanyNameEn();                     // 获取运营商英文名

	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();
	int  GetUSBAuthFlag();							    // 获取USB认证方式（0：不认证；1：需要认证）

	bool IsUseFounderStaffID();							// 是否启 用默认员工
	int  GetFounderStaffID();							// 获取默认员工ID
	bool IsFounderStaffID(int userID);
	CString GetFounderStaffPassword();					// 获取默认员工密码
	bool IsUseConfigTicketCategory();					// 是否启用自定义车票种类
	int  GetConfigTicketCategory(WORD ticketTypeCode);	//取得自定义车票种类
	int  GetCloseOperationMaxWaitSeconds();				// 获取业务结束最大等待时间
	int  IsEnableCPUImmidatelyRefund();					// 是否启用CPU卡即时退卡
	bool IsValidateUnSendPackage();
	

	CString GetTCMUsedAdapterName();

    // [TH]
	int GetTHCommuMethod();
    int GetTHCommPort();
    int GetTHBaudRate();
    int GetTHMaxConnReceive();
    int GetTHMaxConnSend();
	bool IsTHMonitorOpened();
	int GetTHMonitorInterval();

	// [ICRW]
	bool    IsRWUsed(bool bIsRW = true);
	bool	IsRWMonitorOpened(bool bIsRW = true);
	int     GetRWMonitorInterval(bool bIsRW = true);
    int     GetRWCommPort(bool bIsRW = true);
    int     GetRWBaudRate(bool bIsRW = true);
	int     GetSimCardType(bool bIsRW = true);
	int     GetReadCardInterval(bool bIsRW = true);

    // [PRINTER]
	bool    IsPrinterUsed();
    int     GetPrinterCommPort();                   // port open
    int     GetPrinterBaudRate();                   // Baud Rate (bps)

	// [CARD_PRINTER]
	bool    IsCardPrinterUsed();
	CString GetCardPrinterDriverName();            
	CString GetPrinterTempFile();
	int		GetCardType();
	int		GetTestSendCardCount();					 // 发卡测试次数

	// [CAMERA]
	bool	IsCameraUsed();

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
	CString GetTCMUSBDrive();
	CString GetTCMExportPath();
	CString GetTCMSysParaUploadPath();
    CString GetTCMMachineParaUploadPath();
	CString GetTCMSysParaDownloadPath();
	CString GetTCMMachineParaDownloadPath();

    // [LOG]
    int     GetStoreDay();
	int     GetRealStoreDay();

	//// [INCOME_REASON]
	int GetIncomeReasonCount();
	CString GetIncomeReason(int NO);

	// [EXPENSE_REASON]
	int GetExpendReasonCount();
	CString GetExpendReason(int NO);
	CString GetUpdateSuggestOfOperator(int NO);
	CString GetUpdateSuggestOfPassager(int NO);
	CString GetUpdateSuggestEn(int NO);
	CString GetACCCardState(int NO);
	CString GetACCCardStateEn(int NO);
	CString GetECTCardState(int NO);
	CString GetECTCardStateEn(int NO);
	int GetTransactionPackageInterval();
	int GetBusinessPackageInterval();
	int GetReturnDefaultServiceInterval();
	int GetWaitOperationInterval();
	CString GetLineName(int NO);
	CString GetLineNameEn(int NO);
	CString GetApplyCardState(int NO);
	CString GetTransactionType(int NO);


    
private:
    static CTCMInfo theInstance;
    CGetIni* m_TCMIni;

	CString m_strCompanyNameCN;
	CString m_strCompanyNameEN;

    // 隐藏构造函数
    CTCMInfo();
    ~CTCMInfo();
    CTCMInfo(const CTCMInfo&);
    CTCMInfo& operator=(const CTCMInfo&);
    CString AddBackslash(CString sPath);                // 在目录名后添加反斜杠
};