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

    // ������ȡ�����ݵķ���

    // [APPLICATION]
    CString GetInitialVersion();                    // ��ȡ���ڰ�װ�İ汾��Ϣ
    bool    IsTopmost();                            // �Ƿ���ʾ����ǰ��
	bool	IsFunctionKeyEnabled();					// ���ܼ��Ƿ����
	bool	IsTaskManagerEnabled();					// ����������Ƿ����
	bool    IsDesktopEnabled();						// �����Ƿ����
    bool    CanShowCursor();                        // �Ƿ���ʾ�����
	CString GetLogPath();							// ��ȡ��־���·��
    CString GetDataPath();                          // ��ȡ������Ŀ¼
    CString GetBackupPath();                        // ��ȡ����Ŀ¼
 //   int     GetMaxRecoverTimes();                   // ��ȡ���ػ��ļ����ɻָ�����
 //   bool    IsLoginTimeFixed();                     // ��¼�����еĵ�¼ʱ���Ƿ�̶�
    bool    IsWriteDebug();                         // �Ƿ��¼Debug��Ϣ

    CString GetFuncBtnFont();                       // ��ȡ���ܰ�ť��������
    int     GetFuncBtnFontSize();                   // ��ȡ���ܰ�ť�����С
    CString GetOperateBtnFont();                    // ��ȡҵ��ť��������
    int     GetOperateBtnFontSize();                // ��ȡҵ��ť�����С

	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();
	int GetUSBAuthFlag();							// ��ȡUSB��֤��ʽ��0������֤��1����Ҫ��֤��

	bool IsUseFounderStaffID();							// �Ƿ�����Ĭ��Ա��
	int GetFounderStaffID();							// ��ȡĬ��Ա��ID
	bool IsFounderStaffID(int userID);
	CString GetFounderStaffPassword();					// ��ȡĬ��Ա������
	bool IsUseConfigTicketCategory();					// �Ƿ������Զ��峵Ʊ����
	int GetConfigTicketCategory(WORD ticketTypeCode);	//ȡ���Զ��峵Ʊ����
	int GetCloseOperationMaxWaitSeconds();				// ��ȡҵ��������ȴ�ʱ��
	int IsEnableCPUImmidatelyRefund();					// �Ƿ�����CPU����ʱ�˿�
	bool IsValidateUnSendPackage();
	
	//bool IsAuthorizeSet();							// �Ƿ����������֤��ʽ

	//int     GetACCAuthorizeManner();				// ��ȡACC��֤��ʽ
	//int     GetECTAuthorizeManner();				// ��ȡECT��֤��ʽ

	//int GetMCBFCount();								// ��ȡMCBF����(�ﵽ�ô��������MCBF)
	//int GetUpdateFlag();							// ��ȡ������±�־
 //   int GetACCUpdateFlag();							// ��ȡACC����ʧ�ܱ�־

	

	//int GetDatabaseNeedRepair();					// ���ݿ���Ҫ�޸�


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
	int		GetTestSendCardCount();					 // �������Դ���

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

    // ���ع��캯��
    CAGMInfo();
    ~CAGMInfo();
    CAGMInfo(const CAGMInfo&);
    CAGMInfo& operator=(const CAGMInfo&);

};