#include "GetIni.h"
#include "CommonExp.h"
#include "CommonDefine.h"
#pragma once

#define theMAINTENANCE_INFO CMaintenanceInfo::GetInstance()       // CMaintenanceInfo��Ψһ�ӿ�

/**
@brief Maintenance.INI����Ϣ����

@attention
���ļ��п���ͨ����λ���ؽ����޸ĵ���Ŀ���ṩ��Ӧ��Get��Set������
��������Ӧ�ĳ�Ա������һ��Maintenance.INI����Ŀ���޸Ĳ���Ҫ��ʱ��Ч��
����Set����ֻҪ�޸��ļ����������޸Ķ�Ӧ�ĳ�Ա������
*/
class COMMON_API CMaintenanceInfo
{
public:

    static CMaintenanceInfo&  GetInstance();

    int Initialize();                              // ���ڻ�

    // ������ȡ�����ݵķ���
	AFC_DEVICE_TYPE GetDeviceType();					// ��ȡ�豸����
	DEVICE_SUBTYPE GetDeviceSubType();				// ��ȡ�豸������
	CString GetProgName();									// ��ȡ�������ƣ��������ƺϳɣ����磺BOM��VM�����ƶ�ΪBOM��Card��Token��PM��ΪES.....

	void SetDeviceTestMode(int);			// �����豸����ģʽ
	int GetDeviceTestMode();					// ��ȡ�豸����ģʽ

    // [CPS]
    CString GetCpsHostID();                         // ��ȡ Cps HostID
    int     GetCpsPort();                           // ��ȡ CPS PORT

    // [NTP]
    CString GetNtpHostID();                         // ��ȡ Ntp HostID
    int     GetNtpPort();                           // ��ȡ Ntp Port

	// [CHARGE_IP]
	CString GetYPTHostID();                         // ��ȡ YPT HostID
	int     GetYPTPort();                           // ��ȡ YPT Port
	int     GetYPTTimeOutTime();                       // ��ȡ YPT Port

	// [ECT_CHARGE_IP]
	CString GetECTHostID();                         // ��ȡ ECT HostID
	int     GetECTPort();                           // ��ȡ ECT Port
	int     GetECTTimeOutTime();                       // ��ȡ ECT Port

	// [FTP]
    CString GetFtpHostID();                         // ��ȡ Ftp HostID
    int     GetFtpPort();                           // ��ȡ FTP Port
	CString GetFtpUserID();
	CString GetFtpPwd();

	// [�����ӡ��]
	CString GetLaserPrinterHostID();                         // ��ȡ �����ӡ�� HostID
	CString GetLaserPrinterName();                           // ��ȡ �����ӡ�� ����

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
	void SetLanguageMode(LANGUAGETYPE_T);					// ��������ģʽ
	LANGUAGETYPE_T GetLanguageMode();			            // ��ȡ����ģʽ


    // �������������ݵķ���
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

    // ����������Ч�Ĳ���
    // [CPS]
    CString m_cpsHostID;                // CPS HOST IP
    int     m_cpsPort;                  // CPS �˿�

    // [NTP]
    CString m_ntpHostID;                // NTP HOST IP
    int     m_ntpPort;                  // NTP �˿�
	
	// [CHARGE_IP]
	CString m_YPTHostID;                // YPT HOST IP
	int     m_YPTPort;                  // YPT �˿�
	int     m_YPTTimeOutTime;              // YPT �˿�

	// [ECT_CHARGE_IP]
	CString m_ECTHostID;                // ECT HOST IP
	int     m_ECTPort;                  // ECT �˿�
	int     m_ECTTimeOutTime;              // ECT �˿�

	// [FTP]
    CString m_ftpHostID;                // FTP HOST IP
    int     m_ftpPort;                  // FTP �˿�
	CString m_ftpUserId;				// FTP�û�ID
	CString m_ftpPwd;					// FTP����

	// [PRINTER]
	CString m_printerHostID;                // PRINTER HOST IP
	CString m_printerName;                  // PRINTER ����

    // [BMAC]
    CString m_BMACHostID;               // BMAC HOST IP
    int     m_BMACPort;                 // BMAC �˿�

    // [MACHINE]
	int		m_lineCode;					//��·����
    int     m_stationCode;              // ��վ����
    int     m_machineCode;              // ��������
    int     m_groupNumber;              // ���
    int     m_equNumberInGroup;         // ���ڱ��

	CString m_deviceType;
	BYTE m_deviceTypeID;
	LANGUAGETYPE_T m_LanguageMode;
    CString m_IP;                       // ������ַ
    CString m_subnetMask;               // ����
    CString m_gateway;                  // ���ص�ַ
    CString m_stuffID;                  // Ա��ID
    CString m_stuffPassword;            // Ա������
    CString m_RWBootVersion;            // �ⲿ��д��BOOT�汾
    CString m_THRWBootVersion;          // �ڲ���д��BOOT�汾

	DEVICE_ID m_CurrentDevice;
	TPU_DEVICE_ID m_CurrentTPUDevice;
	AccDeviceID_t m_CurrentAccDevice;
    // ���ع��캯��
    CMaintenanceInfo();
    ~CMaintenanceInfo();
    CMaintenanceInfo(const CMaintenanceInfo&);
    CMaintenanceInfo& operator=(const CMaintenanceInfo&);
    CString AddBackslash(CString sPath);                // ��Ŀ¼������ӷ�б��
};