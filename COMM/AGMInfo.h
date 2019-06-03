#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"
#include "SysInfo.h"

#define theAGM_INFO CAGMInfo::GetInstance()

class COMMON_API CAGMInfo : CSYSInfo
{
public:

    static CAGMInfo&  GetInstance();

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
 //   int     GetMaxRecoverTimes();                   // 获取二重化文件最大可恢复次数
 //   bool    IsLoginTimeFixed();                     // 登录电文中的登录时间是否固定
    bool    IsWriteDebug();                         // 是否记录Debug消息

    CString GetFuncBtnFont();                       // 获取功能按钮字体名称
    int     GetFuncBtnFontSize();                   // 获取功能按钮字体大小
    CString GetOperateBtnFont();                    // 获取业务按钮字体名称
    int     GetOperateBtnFontSize();                // 获取业务按钮字体大小

	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();
	int GetUSBAuthFlag();							// 获取USB认证方式（0：不认证；1：需要认证）

	bool IsUseFounderStaffID();							// 是否启用默认员工
	int GetFounderStaffID();							// 获取默认员工ID
	bool IsFounderStaffID(int userID);
	CString GetFounderStaffPassword();					// 获取默认员工密码
	bool IsUseConfigTicketCategory();					// 是否启用自定义车票种类
	int GetConfigTicketCategory(WORD ticketTypeCode);	//取得自定义车票种类
	int GetCloseOperationMaxWaitSeconds();				// 获取业务结束最大等待时间
	int IsEnableCPUImmidatelyRefund();					// 是否启用CPU卡即时退卡
	bool IsValidateUnSendPackage();
	
	//bool IsAuthorizeSet();							// 是否可以设置认证方式

	//int     GetACCAuthorizeManner();				// 获取ACC认证方式
	//int     GetECTAuthorizeManner();				// 获取ECT认证方式

	//int GetMCBFCount();								// 获取MCBF计数(达到该次数后计入MCBF)
	//int GetUpdateFlag();							// 获取程序更新标志
 //   int GetACCUpdateFlag();							// 获取ACC更新失败标志

	

	//int GetDatabaseNeedRepair();					// 数据库需要修复


	CString GetBomUsedAdapterName();

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
    //int     GetRWMaxConnReceive();
    //int     GetRWMaxConnSend();
	//int     GetReadMode();

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
 //   // [MOVEDISK]
      CString GetBOMUSBDrive();
 //   CString GetBOMCFDrive();
	  CString GetBomExportPath();
      CString GetBomSysParaUploadPath();
 //   CString GetBomProgramUploadPath();
    CString GetBomMachineParaUploadPath();


      CString GetBomSysParaDownloadPath();
 //   CString GetBomProgramDownloadPath();
	  CString GetBomMachineParaDownloadPath();
 //   CString GetBomMaintentanceOutputPath();
 //   CString GetBomMaintenceTracePath();

 //   CString GetBomMainDBUploadPath();
 //   CString GetBomBackupDBUploadPath();
 //   CString GetBomDBDownloadPath();

    // [LOG]
    int     GetStoreDay();
	int     GetRealStoreDay();

 //   // [CPS]
 //   int GetMaxTransMsgCnt();
 //   int GetMaxEventMsgCnt();

 //   // [EXCHANGE_SVC]
 //   int GetWaitReadCardTime(); 

 //   // [REFUND_SVC]
 //   int GetWaitQueryTime(); 

 //   // [OPERATIONAL_MODE]
 //   CString GetOperationalMode(OperationalMode_t);

 //   // [OPERATIONAL_MODE_EN]
 //   CString GetOperationalModeEn(OperationalMode_t);

 //   // [JOURNEY_STATUS]
 //   CString GetJourneyStatusCn(JourneyStatus_t);
 //   CString GetJourneyStatusEn(JourneyStatus_t);

 //   // [TRANSACTION_TYPE]
 //   CString GetTransactionTypeCn(U16_t);
 //   CString GetTransactionTypeEn(U16_t);



    // [RESULT_CODE_MSG]
    //CString GetResultCodeMessage(RESULT_CODE_MSG);

    //[OPERATION_MSG]
    //CString GetOperationMessage(OPERATION_MSG);

    //[PASSENGER_MSG]
  //  CString GetPassengerMessageCn(PASSENGER_MSG);
    //CString GetPassengerMessageEn(PASSENGER_MSG);

    //[MAINTENANCE_MSG]
   // CString GetMaintenanceMessage(MAINTENANCE_MSG);

 //   //[PASSENGER_TYPE]
 //   CString GetPassengerTypeCn(PassengerType_t);
 //   CString GetPassengerTypeEn(PassengerType_t);

 //   //[LOCK_STATUS]
 //   CString GetLockStatusCn(CardStatusCode_t);
 //   CString GetLockStatusEn(CardStatusCode_t);

 //   //[CARD_STATUS]
 //   CString GetCardStatusCn(CardStatusCode_t status);
 //   CString GetCardStatusEn(CardStatusCode_t status);

 //   //[YKT_STATUS]
 //   CString GetYKTStatusCn(CARDSTATUS status);

 //   //[YKT_JOURNEY_STATUS]
 //   CString GetYKTJourneyStatusCn(CARD_TRANS_STATUS status);
 //   CString GetYKTJourneyStatusEn(CARD_TRANS_STATUS status);

 //   //[YKT_SYSTEM_CODE]
 //   CString GetYKTSystemCodeCn(BYTE systemCode);
 //   CString GetYKTSystemCodeEn(BYTE systemCode);

 //   //[YKT_TRANSACTION_TYPE]
 //   CString GetYKTTransTypeCn(BYTE transType);
 //   CString GetYKTTransTypeEn(BYTE transType);

	////[YKTSTAFF_TRANSACTION_TYPE]
	//CString GetYKTStaffTransTypeCn(BYTE transType);
	//CString GetYKTStaffTransTypeEn(BYTE transType);

 //   //[YKT_AGENCECODE_TYPE]
 //   CString GetYKTAgenceCodeCn(BYTE agenceCode);
 //  
 //   // [Maintain]
 //   int GetTouchLastTime();



	//// [INCOME_REASON]
	int GetIncomeReasonCount();
	CString GetIncomeReason(int NO);

	//// [EXPENSE_REASON]
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
    static CAGMInfo theInstance;

    // 隐藏构造函数
    CAGMInfo();
    ~CAGMInfo();
    CAGMInfo(const CAGMInfo&);
    CAGMInfo& operator=(const CAGMInfo&);

};