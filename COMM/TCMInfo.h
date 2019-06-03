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

    int Initialize();                              // ���ڻ�
	
	delegate<void()> Initialized;

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
    bool    IsWriteDebug();                         // �Ƿ��¼Debug��Ϣ
	CString GetCompanyNameCn();                     // ��ȡ��Ӫ��������
	CString GetCompanyNameEn();                     // ��ȡ��Ӫ��Ӣ����

	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();
	int  GetUSBAuthFlag();							    // ��ȡUSB��֤��ʽ��0������֤��1����Ҫ��֤��

	bool IsUseFounderStaffID();							// �Ƿ��� ��Ĭ��Ա��
	int  GetFounderStaffID();							// ��ȡĬ��Ա��ID
	bool IsFounderStaffID(int userID);
	CString GetFounderStaffPassword();					// ��ȡĬ��Ա������
	bool IsUseConfigTicketCategory();					// �Ƿ������Զ��峵Ʊ����
	int  GetConfigTicketCategory(WORD ticketTypeCode);	//ȡ���Զ��峵Ʊ����
	int  GetCloseOperationMaxWaitSeconds();				// ��ȡҵ��������ȴ�ʱ��
	int  IsEnableCPUImmidatelyRefund();					// �Ƿ�����CPU����ʱ�˿�
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

    // ���ع��캯��
    CTCMInfo();
    ~CTCMInfo();
    CTCMInfo(const CTCMInfo&);
    CTCMInfo& operator=(const CTCMInfo&);
    CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��
};