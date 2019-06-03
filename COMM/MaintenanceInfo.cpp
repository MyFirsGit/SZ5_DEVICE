#include "StdAfx.h"
#include "MaintenanceInfo.h"
#include "SysException.h"
#include "Util.h"
#include "InnerException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAINTENANCE_INI_FILE_NAME	_T("Maintenance.INI")

CMaintenanceInfo CMaintenanceInfo::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceInfo::CMaintenanceInfo()
{
    try {
        m_maintenanceIni = NULL;

        m_cpsHostID     = _T("");               // CPS HOST IP
        m_cpsPort       = 0;                // CPS �˿�
        m_ntpHostID     = _T("");               // NTP HOST IP
        m_ntpPort       = 0;                // NTP �˿�
        m_ftpHostID     = _T("");               // FTP HOST IP
        m_ftpPort       = 0;                // FTP �˿�
		m_printerHostID = _T("");               // PRINTER HOST IP
		m_printerName   = _T("");                // PRINTER �˿�

        m_stationCode   = 0;                // ��վ����
        m_machineCode   = 0;                // ��������
        m_IP            = _T("");               // ������ַ
        m_subnetMask    = _T("");               // ����
        m_gateway       = _T("");               // ���ص�ַ
        m_groupNumber   = 0;                // ���
        m_equNumberInGroup  = 0;            // ���ڱ��
		m_deviceType = _T("");

        // ���ڻ�
        Initialize();
    }
    catch (...) {
        // �����쳣��Ϣ
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ʵ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceInfo&  CMaintenanceInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceInfo::~CMaintenanceInfo()
{
    delete m_maintenanceIni;
    m_maintenanceIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      bool   true:�ɹ�;  false:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::Initialize() 
	{
        // ���ѳ�ʼ������ֱ�ӷ���
		if (m_maintenanceIni != NULL) {
			return 0;
		}

        TCHAR szAppPath[_MAX_PATH];

        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }

        CString fileName = CString(szAppPath) + MAINTENANCE_INI_FILE_NAME;

        // check DEVICE.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // �ļ����Ը�Ϊ��д
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_maintenanceIni = new CGetIni(fileName);

		// ��ȡ����ģʽ
		m_LanguageMode = (LANGUAGETYPE_T)m_maintenanceIni->GetDataInt(_T("APPLICATION"),_T("LanguageMode"),0);

        // [CPS]
        m_cpsHostID       = m_maintenanceIni->GetDataString(_T("CPS"), _T("HostID"));
        m_cpsPort         = m_maintenanceIni->GetDataInt(_T("CPS"), _T("Port"));

        // [NTP]
        m_ntpHostID       = m_maintenanceIni->GetDataString(_T("NTP"), _T("HostID"));
        m_ntpPort         = m_maintenanceIni->GetDataInt(_T("NTP"), _T("Port"));

		//[CHARGE_IP]
		m_YPTHostID       = m_maintenanceIni->GetDataString(_T("CHARGE_IP"), _T("HostID"));
		m_YPTPort         = m_maintenanceIni->GetDataInt(_T("CHARGE_IP"), _T("Port"));
		m_YPTTimeOutTime  = m_maintenanceIni->GetDataInt(_T("CHARGE_IP"), _T("TimeOut"));
		
		//[ECT_CHARGE_IP]
		m_ECTHostID       = m_maintenanceIni->GetDataString(_T("ECT_CHARGE_IP"), _T("HostID"));
		m_ECTPort         = m_maintenanceIni->GetDataInt(_T("ECT_CHARGE_IP"), _T("Port"));
		m_ECTTimeOutTime  = m_maintenanceIni->GetDataInt(_T("ECT_CHARGE_IP"), _T("TimeOut"));

        // [FTP]
        m_ftpHostID       = m_maintenanceIni->GetDataString(_T("FTP"), _T("HostID"));
        m_ftpPort         = m_maintenanceIni->GetDataInt(_T("FTP"), _T("Port"));
		m_ftpUserId = m_maintenanceIni->GetDataString(_T("FTP"),_T("UserID"));
		m_ftpPwd = m_maintenanceIni->GetDataString(_T("FTP"),_T("UserPwd"));
		
		// [PRINTER]
		m_printerHostID   = m_maintenanceIni->GetDataString(_T("LASERPRINTER"),_T("HostID"));
		m_printerName     = m_maintenanceIni->GetDataString(_T("LASERPRINTER"),_T("Name"));

        //[MACHINE]
		m_deviceType      = m_maintenanceIni->GetDataString(_T("MACHINE"),_T("AFC_DEVICE_TYPE"),_T("TOM"));
		//m_lineCode		  = m_maintenanceIni->GetDataInt(_T("MACHINE"),_T("LINE_CODE"));
        m_stationCode     = m_maintenanceIni->GetDataInt(_T("MACHINE"), _T("STATION_CODE"));
        m_machineCode     = m_maintenanceIni->GetDataInt(_T("MACHINE"), _T("MACHINE_CODE"));
        m_groupNumber     = m_maintenanceIni->GetDataInt(_T("MACHINE"), _T("GROUP_NUMBER"));
        m_equNumberInGroup = m_maintenanceIni->GetDataInt(_T("MACHINE"), _T("EQU_NUMBER_IN_GROUP"));
        m_IP              = m_maintenanceIni->GetDataString(_T("MACHINE"), _T("IP"));
        m_subnetMask      = m_maintenanceIni->GetDataString(_T("MACHINE"), _T("SUBNET_MASK"));
        m_gateway         = m_maintenanceIni->GetDataString(_T("MACHINE"), _T("GATEWAY"));
      

		return 0;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ŀ¼������ӷ�б��

@param      (i)CString sPath    Ŀ¼

@retval     CString     �������б�ܵ�Ŀ¼��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::AddBackslash(CString sPath)
{
    return sPath.TrimRight(_T("\\")) + _T("\\");
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       �����豸����ģʽ   

@param       (o)int mode

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetDeviceTestMode(int mode)
{
	CString str;
	str.Format(_T("%d"),mode);
	m_maintenanceIni->WriteData(_T("APPLICATION"),_T("DeviceTestMode"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�豸����ģʽ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetDeviceTestMode()
{
	return m_maintenanceIni->GetDataInt(_T("APPLICATION"),_T("DeviceTestMode"),1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�豸����

@param       none 

@retval      AFC_DEVICE_TYPE �Զ����豸����

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
AFC_DEVICE_TYPE CMaintenanceInfo::GetDeviceType()
{
	return (AFC_DEVICE_TYPE)m_maintenanceIni->GetDataInt(_T("APPLICATION"), _T("AFC_DEVICE_TYPE"),DEVICE_TYPE_TVM);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ ��ǰ�豸����

@param       �� 

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
BYTE CMaintenanceInfo::GetDeviecTypeID()
{
	return (BYTE)m_maintenanceIni->GetDataInt(_T("APPLICATION"), _T("AFC_DEVICE_TYPE"),DEVICE_TYPE_TVM);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��·����

@param       �� 

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetTPUStationCode()
{
	return m_stationCode ;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�豸������

@param    none 

@retval     DEVICE_SUBTYPE �豸������

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_SUBTYPE CMaintenanceInfo::GetDeviceSubType()
{
	return (DEVICE_SUBTYPE)m_maintenanceIni->GetDataInt(_T("APPLICATION"), _T("AFC_SUB_DEVICE_TYPE"),DEVICE_SUBTYPE_UNKNOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�豸���ƣ��������ƺϳɣ����磺BOM��VM�����ƶ�ΪBOM��Card��Token��PM��ΪES.....

@param       none 

@retval      CString name ����

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetProgName()
{
	return m_maintenanceIni->GetDataString(_T("ProgName"),_T("TVM"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ Cps HostID

@param       �� 

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetCpsHostID()
{
    return m_cpsHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ CPS PORT

@param       �� 

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetCpsPort()
{
    return m_cpsPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ Ntp Host ID

@param       �� 

@retval      CString  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetNtpHostID()
{
    return m_ntpHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ Ntp Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetNtpPort()
{
    return m_ntpPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ YPT Host ID

@param       �� 

@retval      CString  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetYPTHostID()
{
	return m_YPTHostID;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ YPT Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetYPTPort()
{
	return m_YPTPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ Ntp Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetYPTTimeOutTime()
{
	return m_YPTTimeOutTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ ECT Host ID

@param       �� 

@retval      CString  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetECTHostID()
{
	return m_ECTHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ ECT Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetECTPort()
{
	return m_ECTPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ ECT TimeOutTime

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetECTTimeOutTime()
{
	return m_ECTTimeOutTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ Ftp HostID

@param       �� 

@retval      CString  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetFtpHostID()
{
    return m_ftpHostID;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ FTP Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetFtpPort()
{
    return m_ftpPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ FTP User ID

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetFtpUserID()
{
	return m_ftpUserId;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ FTP ����

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetFtpPwd()
{
	return m_ftpPwd;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ PRINTER Host ID

@param       �� 

@retval      CString  

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetLaserPrinterHostID()
{
	return m_printerHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ PRINTER Port

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetLaserPrinterName()
{
	return m_printerName;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��վ����

@param       �� 

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetStationCode()
{
    return GetStationCodeWithLine() % 100;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��·����

@param       �� 

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetLineCode()
{
    return GetStationCodeWithLine() / 100;
}

WORD CMaintenanceInfo::GetStationCodeWithLine()
{
	return  m_stationCode;
}

DEVICE_ID& CMaintenanceInfo::GetCurrentDevice()
{
	BYTE lineCode = 0;
	int2BCD(GetLineCode(),(char*)&lineCode,1);
	m_CurrentDevice.bLineId = lineCode;
	BYTE stationCode = 0;
	(BYTE)int2BCD(GetStationCode(),(char*)&stationCode,1);
	m_CurrentDevice.bStationId = stationCode;
	m_CurrentDevice.bAfcDeviceType = GetDeviecTypeID();
	m_CurrentDevice.bDeviceSeqNoInStation = GetMachineCode();
	return m_CurrentDevice;
}

TPU_DEVICE_ID& CMaintenanceInfo::GetCurrentTPUDevice()
{
	m_CurrentTPUDevice.bAfcDeviceType = GetDeviecTypeID();
	m_CurrentTPUDevice.bDeviceSeqNoInStation = GetMachineCode();
	int lineCode = 0;
	int2BCD(GetLineCode(),(char*)&lineCode,1);
	int stationCode = 0;
	int2BCD(GetStationCode(),(char*)&stationCode,1);
	m_CurrentTPUDevice.wAfcStationID = MAKEWORD(stationCode,lineCode);
	return m_CurrentTPUDevice;
}


AccDeviceID_t& CMaintenanceInfo::GetCurrentACCDevice()
{
	m_CurrentAccDevice.deviceType = theMAINTENANCE_INFO.GetDeviceType();
	m_CurrentAccDevice.lineCode = theMAINTENANCE_INFO.GetLineCode();
	m_CurrentAccDevice.stationCode = theMAINTENANCE_INFO.GetStationCodeWithLine();
	m_CurrentAccDevice.wSerialNo = theMAINTENANCE_INFO.GetMachineCode();
	return m_CurrentAccDevice;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��������

@param       �� 

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
unsigned char CMaintenanceInfo::GetMachineCode()
{
    return m_machineCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int  CMaintenanceInfo::GetGroupNumber()
{
    return m_groupNumber;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ڱ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int  CMaintenanceInfo::GetEquNumberInGroup()
{
    return m_equNumberInGroup;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ������ַ

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetIP()
{
    return m_IP;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ����

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetSubnetMask()
{
    return m_subnetMask;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ص�ַ

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetGateway()
{
    return m_gateway;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����CpsHostID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetCpsHostID(const CString& str)
{
    m_maintenanceIni->WriteData(_T("CPS"), _T("HostID"), str);
	m_cpsHostID = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Cps Port

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetCpsPort(const CString& str)
{
    m_maintenanceIni->WriteData(_T("CPS"), _T("Port"), str);
	m_cpsPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����NtpHostID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetNtpHostID(const CString& str)
{
    m_maintenanceIni->WriteData(_T("NTP"), _T("HostID"), str);
	m_ntpHostID = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Ntp Port

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetNtpPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("NTP"), _T("Port"), str);
	m_ntpPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����FtpHostID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpHostID(const CString& str)
{
	m_ftpHostID = str;
    m_maintenanceIni->WriteData(_T("FTP"), _T("HostID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Ftp Port

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("FTP"), _T("Port"), str);
	m_ftpPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����FTP�û�ID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpUserID(const CString& str)
{
	m_ftpUserId = str;
	m_maintenanceIni->WriteData(_T("FTP"), _T("UserID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����FTP����

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpPwd(const CString& str)
{
	m_ftpPwd = str;
	m_maintenanceIni->WriteData(_T("FTP"), _T("UserPwd"), str);
	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ����YPTHostID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetYPTHostID(const CString& str)
{
	m_YPTHostID = str;
	m_maintenanceIni->WriteData(_T("CHARGE_IP"), _T("HostID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����Ftp Port

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetYPTPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("CHARGE_IP"), _T("Port"), str);
	m_YPTPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����ECTHostID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetECTHostID(const CString& str)
{
	m_ECTHostID = str;
	m_maintenanceIni->WriteData(_T("ECT_CHARGE_IP"), _T("HostID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����ECT Port

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetECTPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("ECT_CHARGE_IP"), _T("Port"), str);
	m_ECTPort = _ttoi(str);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       ����PrinterHostID

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetPrinterHostID(const CString& str)
{
	m_maintenanceIni->WriteData(_T("LASERPRINTER"), _T("HostID"), str);
	m_printerHostID = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����PrinterName

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetPrinterName(const CString& str)
{
	m_maintenanceIni->WriteData(_T("LASERPRINTER"), _T("Name"), str);
	m_printerName = str;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       �������   

@param       (o)int num

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetGroupNumber(int num)
{
    CString str;
    str.Format(_T("%d"), num);
    m_maintenanceIni ->WriteData(_T("MACHINE"), _T("GROUP_NUMBER"), str);
	m_groupNumber = num;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       �������ڱ��   

@param       (o)int num

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetEquNumberInGroup(int num)
{
    CString str;
    str.Format(_T("%d"), num);
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("EQU_NUMBER_IN_GROUP"), str);
	m_equNumberInGroup = num;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ������·����

@param       (o)const int num

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetLineCode(int num)
{
	CString str;
	str.Format(_T("%d"), num);
	m_maintenanceIni->WriteData(_T("MACHINE"), _T("LINE_CODE"), str);
	m_lineCode = num;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ó�վ����

@param       (o)const int num

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetStationCode(int num)
{
    CString str;
    str.Format(_T("%d"), num);
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("STATION_CODE"), str);
	m_stationCode = num;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ñ�������

@param       (o)const int num

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetMachineCode(int num)
{
    CString str;
    str.Format(_T("%d"), num);
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("MACHINE_CODE"), str);
	m_machineCode = num;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ñ���IP��ַ   

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetIP(const CString& str)
{
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("IP"), str);
	m_IP = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������   

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetSubnetMask(const CString& str)
{
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("SUBNET_MASK"), str);
	m_subnetMask = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �������ص�ַ   

@param       (o)const CString& str

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetGateway(const CString& str)
{
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("GATEWAY"), str);
	m_gateway = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ñ�����������   

@param       ��

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetLinkName()
{
	return m_maintenanceIni->GetDataString(_T("MACHINE"),_T("LINK_NAME"),_T("��������"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ����ģʽ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CMaintenanceInfo::GetLanguageMode()
{
	return m_LanguageMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������ģʽ   

@param       (o)int mode

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetLanguageMode(LANGUAGETYPE_T mode)
{
	CString str;
	str.Format(_T("%d"), mode);
	m_maintenanceIni->WriteData(_T("APPLICATION"),_T("LanguageMode"),str);
	m_LanguageMode = mode;
}
