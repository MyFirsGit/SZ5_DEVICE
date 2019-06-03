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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceInfo::CMaintenanceInfo()
{
    try {
        m_maintenanceIni = NULL;

        m_cpsHostID     = _T("");               // CPS HOST IP
        m_cpsPort       = 0;                // CPS 端口
        m_ntpHostID     = _T("");               // NTP HOST IP
        m_ntpPort       = 0;                // NTP 端口
        m_ftpHostID     = _T("");               // FTP HOST IP
        m_ftpPort       = 0;                // FTP 端口
		m_printerHostID = _T("");               // PRINTER HOST IP
		m_printerName   = _T("");                // PRINTER 端口

        m_stationCode   = 0;                // 车站代码
        m_machineCode   = 0;                // 本机代码
        m_IP            = _T("");               // 本机地址
        m_subnetMask    = _T("");               // 掩码
        m_gateway       = _T("");               // 网关地址
        m_groupNumber   = 0;                // 组号
        m_equNumberInGroup  = 0;            // 组内编号
		m_deviceType = _T("");

        // 初期化
        Initialize();
    }
    catch (...) {
        // 忽略异常信息
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回实例

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceInfo&  CMaintenanceInfo::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CMaintenanceInfo::~CMaintenanceInfo()
{
    delete m_maintenanceIni;
    m_maintenanceIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      bool   true:成功;  false:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::Initialize() 
	{
        // 若已初始化，则直接返回
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

        // 文件属性改为可写
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_maintenanceIni = new CGetIni(fileName);

		// 获取语言模式
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
@brief      在目录名后添加反斜杠

@param      (i)CString sPath    目录

@retval     CString     右面带反斜杠的目录名

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::AddBackslash(CString sPath)
{
    return sPath.TrimRight(_T("\\")) + _T("\\");
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       设置设备测试模式   

@param       (o)int mode

@retval      void

@exception   无
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
@brief       获取设备测试模式

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetDeviceTestMode()
{
	return m_maintenanceIni->GetDataInt(_T("APPLICATION"),_T("DeviceTestMode"),1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取设备类型

@param       none 

@retval      AFC_DEVICE_TYPE 自定义设备类型

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
AFC_DEVICE_TYPE CMaintenanceInfo::GetDeviceType()
{
	return (AFC_DEVICE_TYPE)m_maintenanceIni->GetDataInt(_T("APPLICATION"), _T("AFC_DEVICE_TYPE"),DEVICE_TYPE_TVM);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 当前设备类型

@param       无 

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
BYTE CMaintenanceInfo::GetDeviecTypeID()
{
	return (BYTE)m_maintenanceIni->GetDataInt(_T("APPLICATION"), _T("AFC_DEVICE_TYPE"),DEVICE_TYPE_TVM);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取线路代码

@param       无 

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetTPUStationCode()
{
	return m_stationCode ;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取设备子类型

@param    none 

@retval     DEVICE_SUBTYPE 设备子类型

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
DEVICE_SUBTYPE CMaintenanceInfo::GetDeviceSubType()
{
	return (DEVICE_SUBTYPE)m_maintenanceIni->GetDataInt(_T("APPLICATION"), _T("AFC_SUB_DEVICE_TYPE"),DEVICE_SUBTYPE_UNKNOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取设备名称，用于名称合成，例如：BOM和VM的名称都为BOM，Card、Token、PM都为ES.....

@param       none 

@retval      CString name 名称

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetProgName()
{
	return m_maintenanceIni->GetDataString(_T("ProgName"),_T("TVM"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 Cps HostID

@param       无 

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetCpsHostID()
{
    return m_cpsHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 CPS PORT

@param       无 

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetCpsPort()
{
    return m_cpsPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 Ntp Host ID

@param       无 

@retval      CString  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetNtpHostID()
{
    return m_ntpHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 Ntp Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetNtpPort()
{
    return m_ntpPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 YPT Host ID

@param       无 

@retval      CString  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetYPTHostID()
{
	return m_YPTHostID;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 YPT Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetYPTPort()
{
	return m_YPTPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 Ntp Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetYPTTimeOutTime()
{
	return m_YPTTimeOutTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 ECT Host ID

@param       无 

@retval      CString  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetECTHostID()
{
	return m_ECTHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 ECT Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetECTPort()
{
	return m_ECTPort;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 ECT TimeOutTime

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetECTTimeOutTime()
{
	return m_ECTTimeOutTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 Ftp HostID

@param       无 

@retval      CString  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetFtpHostID()
{
    return m_ftpHostID;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 FTP Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetFtpPort()
{
    return m_ftpPort;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 FTP User ID

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetFtpUserID()
{
	return m_ftpUserId;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 FTP 密码

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetFtpPwd()
{
	return m_ftpPwd;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 PRINTER Host ID

@param       无 

@retval      CString  

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetLaserPrinterHostID()
{
	return m_printerHostID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取 PRINTER Port

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetLaserPrinterName()
{
	return m_printerName;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取车站代码

@param       无 

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CMaintenanceInfo::GetStationCode()
{
    return GetStationCodeWithLine() % 100;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取线路代码

@param       无 

@retval      int 

@exception   无
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
@brief       获取本机代码

@param       无 

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
unsigned char CMaintenanceInfo::GetMachineCode()
{
    return m_machineCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取组号

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int  CMaintenanceInfo::GetGroupNumber()
{
    return m_groupNumber;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取组内编号

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int  CMaintenanceInfo::GetEquNumberInGroup()
{
    return m_equNumberInGroup;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       获取本机地址

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetIP()
{
    return m_IP;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取掩码

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetSubnetMask()
{
    return m_subnetMask;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取网关地址

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetGateway()
{
    return m_gateway;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置CpsHostID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetCpsHostID(const CString& str)
{
    m_maintenanceIni->WriteData(_T("CPS"), _T("HostID"), str);
	m_cpsHostID = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置Cps Port

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetCpsPort(const CString& str)
{
    m_maintenanceIni->WriteData(_T("CPS"), _T("Port"), str);
	m_cpsPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置NtpHostID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetNtpHostID(const CString& str)
{
    m_maintenanceIni->WriteData(_T("NTP"), _T("HostID"), str);
	m_ntpHostID = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置Ntp Port

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetNtpPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("NTP"), _T("Port"), str);
	m_ntpPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置FtpHostID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpHostID(const CString& str)
{
	m_ftpHostID = str;
    m_maintenanceIni->WriteData(_T("FTP"), _T("HostID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置Ftp Port

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("FTP"), _T("Port"), str);
	m_ftpPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置FTP用户ID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpUserID(const CString& str)
{
	m_ftpUserId = str;
	m_maintenanceIni->WriteData(_T("FTP"), _T("UserID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置FTP密码

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetFtpPwd(const CString& str)
{
	m_ftpPwd = str;
	m_maintenanceIni->WriteData(_T("FTP"), _T("UserPwd"), str);
	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       设置YPTHostID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetYPTHostID(const CString& str)
{
	m_YPTHostID = str;
	m_maintenanceIni->WriteData(_T("CHARGE_IP"), _T("HostID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置Ftp Port

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetYPTPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("CHARGE_IP"), _T("Port"), str);
	m_YPTPort = _ttoi(str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置ECTHostID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetECTHostID(const CString& str)
{
	m_ECTHostID = str;
	m_maintenanceIni->WriteData(_T("ECT_CHARGE_IP"), _T("HostID"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置ECT Port

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetECTPort(const CString& str)
{
	m_maintenanceIni->WriteData(_T("ECT_CHARGE_IP"), _T("Port"), str);
	m_ECTPort = _ttoi(str);
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       设置PrinterHostID

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetPrinterHostID(const CString& str)
{
	m_maintenanceIni->WriteData(_T("LASERPRINTER"), _T("HostID"), str);
	m_printerHostID = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置PrinterName

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetPrinterName(const CString& str)
{
	m_maintenanceIni->WriteData(_T("LASERPRINTER"), _T("Name"), str);
	m_printerName = str;
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       设置组号   

@param       (o)int num

@retval      void

@exception   无
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
@brief       设置组内编号   

@param       (o)int num

@retval      void

@exception   无
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
@brief       设置线路代码

@param       (o)const int num

@retval      void

@exception   无
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
@brief       设置车站代码

@param       (o)const int num

@retval      void

@exception   无
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
@brief       设置本机号码

@param       (o)const int num

@retval      void

@exception   无
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
@brief       设置本机IP地址   

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetIP(const CString& str)
{
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("IP"), str);
	m_IP = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置掩码   

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetSubnetMask(const CString& str)
{
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("SUBNET_MASK"), str);
	m_subnetMask = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置网关地址   

@param       (o)const CString& str

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetGateway(const CString& str)
{
    m_maintenanceIni->WriteData(_T("MACHINE"), _T("GATEWAY"), str);
	m_gateway = str;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获得本地连接名称   

@param       无

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CMaintenanceInfo::GetLinkName()
{
	return m_maintenanceIni->GetDataString(_T("MACHINE"),_T("LINK_NAME"),_T("本地连接"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取语言模式

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CMaintenanceInfo::GetLanguageMode()
{
	return m_LanguageMode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置语言模式   

@param       (o)int mode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CMaintenanceInfo::SetLanguageMode(LANGUAGETYPE_T mode)
{
	CString str;
	str.Format(_T("%d"), mode);
	m_maintenanceIni->WriteData(_T("APPLICATION"),_T("LanguageMode"),str);
	m_LanguageMode = mode;
}
