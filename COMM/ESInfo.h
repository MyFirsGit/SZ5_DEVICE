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
  //  int Initialize();                              // ���ڻ�
	
	//delegate<void()> Initialized;

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
	bool IsUsedGuardian();							// �Ƿ����ػ�����
	UINT GetLimitedDaysOfUnSendData();
	UINT GetLimitedDaysOfSendData();
	int GetUSBAuthFlag();							// ��ȡUSB��֤��ʽ��0������֤��1����Ҫ��֤��
	int GetStatusTimerInterval();					// ��ȡ״̬��ˢ��ʱ����
	bool IsUseFounderStaffID();							// �Ƿ�����Ĭ��Ա��
	int GetFounderStaffID();							// ��ȡĬ��Ա��ID
	bool IsFounderStaffID(int userID);
	CString GetFounderStaffPassword();					// ��ȡĬ��Ա������
	int GetCloseOperationMaxWaitSeconds();				// ��ȡҵ��������ȴ�ʱ��
	
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
	int     GetPrintType();                         //��ӡ����
	int     GetPrintDiret();                        //��ӡ����
	int     GetPrintRowDis();                       //�м��

	// [CARD_PRINTER]
	bool    IsCardPrinterUsed();
	CString GetCardPrinterDriverName();            
	CString GetPrinterTempFile();
	int		GetCardType();
	int		GetTestSendCardCount();					 // �������Դ���

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
	bool IsRfidUsed(RFID_NO rfidId);		// ��ȡRFID�Ƿ�ʹ��
	int GetRfidCommPort(RFID_NO rfidId);	// ��ȡRFID���ں�	
	int GetRfidBaudRate(RFID_NO rfidId);	// ��ȡRFID������	
	int GetRfidStopBit(RFID_NO rfidId);		// ��ȡRFIDֹͣλ	
	CString GetRfidParity(RFID_NO rfidId);	// ��ȡRFID��żУ��	
	int GetRfidByteSize(RFID_NO rfidId);	// ��ȡRFID�ֽڴ�С	
	int GetRfidInputSize(RFID_NO rfidId);	// ��ȡRFID�������뻺���С	
	int GetRfidOutputSize(RFID_NO rfidId);	// ��ȡRFID������������С	

	// ��������
	int GetTxnIntervalTime();			// ��ȡ��ʱ��ʱ��
	int GetAuditInterval();				// ��ȡ��������Ƽ�ʱ
	int GetBusinessInterval();			// ҵ�����ݵ�����ʱ��
    bool IsDeleteParameterHead();						// �Ƿ�ɾ������Э��ͷ��β��
	// ��ȡ�豸��������
	CString GetDeviceComponentCode(long);
private:
    static CESInfo theInstance;
   // CGetIni* m_DeviceIni;

    // ���ع��캯��
    CESInfo();
    ~CESInfo();
    CESInfo(const CESInfo&);
    CESInfo& operator=(const CESInfo&);
    //CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��

	CString GetRWTypeString(RW_TYPE rwType);
	CString GetRfidTypeString(RFID_NO rfidId);
};