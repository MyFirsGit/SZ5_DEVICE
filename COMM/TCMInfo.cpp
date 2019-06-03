#include "StdAfx.h"
#include "TCMInfo.h"
#include "SysException.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TCM_INI_FILE_NAME		    _T("TCM.INI")

CTCMInfo CTCMInfo::theInstance;

CTCMInfo& CTCMInfo::GetInstance()
{
		return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCMInfo::CTCMInfo()
{
    try {
        m_TCMIni = NULL;

		//m_accAuthorizeManner = 0;			// ACC认证方式（0：不认证；1：内部模式；2：外部模式）
		//m_ectAuthorizeManner = 0;			// ECT认证方式（0：不认证；1：内部模式；2：外部模式）
		
		//m_rfidWriteStatus = 0;				// RFID可写状态（=0允许写入；=1禁止写入）

        // 初期化
        Initialize();
    }
    catch (...) {
        // 忽略异常信息
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTCMInfo::~CTCMInfo()
{
    delete m_TCMIni;
    m_TCMIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::Initialize() 
{
    try {
        // 若已初始化，则直接返回
        if (m_TCMIni != NULL) {
            return 0;
        }

        TCHAR szAppPath[_MAX_PATH];

        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }

        CString fileName = CString(szAppPath) + TCM_INI_FILE_NAME;

        // check TCM.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // 文件属性改为可写
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_TCMIni = new CGetIni(fileName);
		
		Initialized.Invoke();

		//[APPLICATION]
		//m_accAuthorizeManner = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ACCAuthorizeManner"),2);			// ACC认证方式（0：不认证；1：内部模式；2：外部模式）
		//m_ectAuthorizeManner = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ECTAuthorizeManner"),2);			// ECT认证方式（0：不认证；1：内部模式；2：外部模式）
		//m_deviceTestMode = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("DEVICE_TEST_MODE"),0);				// 设备测试模式（0：正常模式；1：测试模式）
		//m_isRWUsed = (m_TCMIni->GetDataInt(_T("OUT_ICRW"), _T("IS_USED")) == 1);
		//m_rfidWriteStatus = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("RFID_WRITEABLE_STATUS"),1);			// RFID可写状态设置（0：允许写入；1：禁止写入）
        m_strCompanyNameCN = m_TCMIni->GetDataString(_T("COMPANY"), _T("COMPANY_NAME_CN "));
		m_strCompanyNameEN = m_TCMIni->GetDataString(_T("COMPANY"), _T("COMPANY_NAME_EN "));
		return 0;
   }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        //throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, __FILE__, __LINE__);
    }
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      在目录名后添加反斜杠
//
//@param      (i)CString sPath    目录
//
//@retval     CString     右面带反斜杠的目录名
//
//@exception  none
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::AddBackslash(CString sPath)
{
	if(sPath.IsEmpty()){
		return sPath;
	}

    return sPath.TrimRight(_T("\\")) + _T("\\");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取初期安装的版本信息

@param       无 

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetInitialVersion()
{
    return m_TCMIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       是否显示在最前面
//
//@param       无
//
//@retval      bool true:在最前面 ; false:不在最前面
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsTopmost()
{
    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("TopMost"), 1) == 0 ? false : true;
}

bool CTCMInfo::IsFunctionKeyEnabled()
{
	 return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("FunctionKeyEnable"), 1) != 0 ;
}
bool CTCMInfo::IsTaskManagerEnabled()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("TaskManagerEnabled"), 1) != 0 ;
}

bool    CTCMInfo::IsDesktopEnabled()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("DesktopEnabled"), 1) != 0 ;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       是否显示鼠标光标
//
//@param       无
//
//@retval      bool true:显示 ; false:不显示
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
bool CTCMInfo::CanShowCursor()
{
    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取主数据目录
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetDataPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("APPLICATION"), _T("DataPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取备份目录
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetBackupPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("APPLICATION"), _T("BackupPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取备份目录
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetLogPath()
{
	CString strLogPath =  m_TCMIni->GetDataString(_T("APPLICATION"), _T("LogPath"),_T(""));
	if(strLogPath == _T(""))
	{
		strLogPath = GetBackupPath() + _T("\\Log");
	}
	else
	{
		strLogPath = AddBackslash(strLogPath);
	}
	return strLogPath;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取运营商中文名
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetCompanyNameCn()
{
	return  m_strCompanyNameCN;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取运营商英文名
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetCompanyNameEn()
{
	return m_strCompanyNameEN;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取允许最大未送数据天数
//
//@param       无 
//
//@retval      UINT
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
UINT CTCMInfo::GetLimitedDaysOfUnSendData()
{
	return m_TCMIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取已送数据保留天数
//
//@param       无 
//
//@retval      UINT
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
UINT CTCMInfo::GetLimitedDaysOfSendData()
{
	return m_TCMIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TCM 控制部程序 上传路径

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMExportPath()
{
	return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("ExportPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取二重化文件最大可恢复次数
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetMaxRecoverTimes()
//{
//    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("MaxRecoverTimes"), 10);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       登录电文中的登录时间是否固定
//
//@param       无
//
//@retval      bool true:固定 ; false:不固定
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTCMInfo::IsLoginTimeFixed()
//{
//    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("LoginTimeFixed")) == 0 ? false : true;
//}
//
//////////////////////////////////////////////////////////////////////////
/**
@brief       是否记录Debug消息

@param       无

@retval      bool true:记录Debug消息 ; false:不记录Debug消息

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
 bool CTCMInfo::IsWriteDebug()
 {
    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("WriteDebug")) == 0 ? false : true;
 }

////////////////////////////////////////////////////////////////////////////
///**
//@brief       是否启用方正员工
//
//@param       无
//
//@retval      bool true:启用 ; false:不启用
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUseFounderStaffID()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取欧姆龙员工ID

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetFounderStaffID()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("FounderStaffID"), 10101028);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsFounderStaffID(int userID)
{
	return IsUseFounderStaffID() && (GetFounderStaffID() == userID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取欧姆龙员工密码

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetFounderStaffPassword()
{
	return m_TCMIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        是否启用自定义车票种类

@param       无 

@retval      bool

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUseConfigTicketCategory()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UseConfigTicketCategory"),0) == 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        取启用自定义车票种类

@param       ticketTypeCode 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetConfigTicketCategory(WORD ticketTypeCode)
{
	CString strTicketCode = _T("");
	strTicketCode.Format(_T("%d"),ticketTypeCode);
	return m_TCMIni->GetDataInt(_T("TICKET_CATEGORY"),strTicketCode,-1);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief        取得业务结束最大等待时间单位（秒）

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

int CTCMInfo::GetCloseOperationMaxWaitSeconds()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("CloseOperationMaxWaitSeconds"),10*60);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       // 是否启用CPU卡即时退卡

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::IsEnableCPUImmidatelyRefund()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("EnableCPUImmidatelyRefund"),0) == 1;
}

bool CTCMInfo::IsValidateUnSendPackage()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ValidateUnSendPackage"),1) != 99;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       是否可以设置认证方式
//
//@param       无
//
//@retval      bool true:启用 ; false:不启用
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTCMInfo::IsAuthorizeSet()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("IsAuthorizeSet")) == 0 ? false : true;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取ACC认证方式
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int  CTCMInfo::GetACCAuthorizeManner()
//{
//	return m_accAuthorizeManner;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取ECT认证方式
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int  CTCMInfo::GetECTAuthorizeManner()
//{
//	return m_ectAuthorizeManner;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取MCBF计数(达到该次数后计入MCBF)
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetMCBFCount()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("MCBFCount"), 3);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取程序更新标志
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetUpdateFlag()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UpdateFlag"), 0);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取ACC参数更新失败标志
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetACCUpdateFlag()
//{
//    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ACCUpdateFlag"), 0);
//}


////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取RFID可写状态
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//int  CTCMInfo::GetRFIDWriteStatus()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("RFIDWriteableStatus"), 0);
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取USB认证方式（0：不认证；1：需要认证）

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetUSBAuthFlag()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"), 0);
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取FTP日志路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetFtpLogPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("LogPath"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取FtpSendFailLogPath
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetFtpSendFailLogPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("SendFailLogPath"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取FTP 下载的测试文件路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTestFileDownloadFTPPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("TestFileDownloadFTPPath"));
//}
//


////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取本地测试文件路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTestFileDownloadLocalDir()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("TestFileDownloadLocalDir"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取本地测试文件上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTestFileUploadLocalPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("TestFileUploadLocalPath"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH通信方式

@param       none 

@retval      int  0:串口 1:邮槽 2:管道 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHCommuMethod()
{
	return m_TCMIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHCommPort()
{
    return m_TCMIni->GetDataInt(_T("TH"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1BaudRate

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHBaudRate()
{
    return m_TCMIni->GetDataInt(_T("TH"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnReceive

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHMaxConnReceive()
{
    return m_TCMIni->GetDataInt(_T("TH"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnSend

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHMaxConnSend()
{
    return m_TCMIni->GetDataInt(_T("TH"), _T("MAX_CONN_SEND"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsTHMonitorOpened()
{
	return (m_TCMIni->GetDataInt(_T("TH"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHMonitorInterval()
{
	return m_TCMIni->GetDataInt(_T("TH"),_T("MONITOR_INTERVAL"),5000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用读写器

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      bool true:使用读写器  false:使用模拟数据

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsRWUsed(bool isRW)
{
	if(isRW){
		return (m_TCMIni->GetDataInt(_T("ICRW"), _T("IS_USED")) == 1);
	}
	return (m_TCMIni->GetDataInt(_T("ICTW"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool	CTCMInfo::IsRWMonitorOpened(bool bIsRW)
{
	if(bIsRW){
		return m_TCMIni->GetDataInt(_T("ICRW"), _T("MONITOR_SWITCH"),0)!=0;
	}
	return m_TCMIni->GetDataInt(_T("ICTW"), _T("MONITOR_SWITCH"),0)!=0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int     CTCMInfo::GetRWMonitorInterval(bool bIsRW)
{
	if(bIsRW){
		return m_TCMIni->GetDataInt(_T("ICRW"), _T("MONITOR_INTERVAL"),30);
	}
	return m_TCMIni->GetDataInt(_T("ICTW"), _T("MONITOR_INTERVAL"),30);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取外部读写器串口号

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int 串口号

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetRWCommPort(bool isRW)
{
	if(isRW){
		return m_TCMIni->GetDataInt(_T("ICRW"), _T("COMM_PORT"));
	}
    return m_TCMIni->GetDataInt(_T("ICTW"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取外部读写器串口波特率

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int 串口波特率

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetRWBaudRate(bool isRW)
{
	if(isRW){
		return m_TCMIni->GetDataInt(_T("ICRW"), _T("BAUD_RATE"));
	}
    return m_TCMIni->GetDataInt(_T("ICTW"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取外部读写器

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetRWMaxConnReceive()
//{
//    return m_TCMIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_RECEIVE"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect2MaxConnSend

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetRWMaxConnSend()
//{
//    return m_TCMIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_SEND"));
//}



//////////////////////////////////////////////////////////////////////////
/**
@brief       取得模拟卡物理类型，读卡使用

@param       (i)bool isRW  true:外部读写器  false:内部读写器 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetSimCardType(bool isRW)
{
	if (isRW){
		return m_TCMIni->GetDataInt(_T("ICRW"), _T("SIM_CARD_TYPE"));
	}
	return m_TCMIni->GetDataInt(_T("ICTW"), _T("SIM_CARD_TYPE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得读卡时间间隔

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CTCMInfo::GetReadCardInterval(bool isRW)
{
	if (isRW){
		return m_TCMIni->GetDataInt(_T("ICRW"), _T("READ_CARD_INTERVAL"));
	}
	return m_TCMIni->GetDataInt(_T("ICTW"), _T("READ_CARD_INTERVAL"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用打印机

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsPrinterUsed()
{
	return (m_TCMIni->GetDataInt(_T("PRINTER"), _T("IS_USED")) == 1);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Connect3CommPort
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetPrinterCommPort()
{
    return m_TCMIni->GetDataInt(_T("PRINTER"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Connect3BaudRate
//
//@param       无 
//
//@retval      int
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetPrinterBaudRate()
{
   return m_TCMIni->GetDataInt(_T("PRINTER"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用票卡打印机

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsCardPrinterUsed()
{
	return (m_TCMIni->GetDataInt(_T("CARD_PRINTER"), _T("IS_USED")) == 1);
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       获取打印机名称

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetCardPrinterDriverName()
{
	return m_TCMIni->GetDataString(_T("CARD_PRINTER"), _T("DRIVER_NAME"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       获取票卡打印机打印临时文件名

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetPrinterTempFile()
{
	return m_TCMIni->GetDataString(_T("CARD_PRINTER"), _T("PRT_TEMP"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       获取票卡打印机驱动名称

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetCardType()
{
	return m_TCMIni->GetDataInt(_T("CARD_PRINTER"), _T("CARD_TYPE"));	
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       发卡测试次数

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTestSendCardCount()
{
	return m_TCMIni->GetDataInt(_T("CARD_PRINTER"), _T("TEST_SEND_CARD_COUNT "));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用摄像头

@param       无 

@retval      bool

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsCameraUsed()
{
	return (m_TCMIni->GetDataInt(_T("CAMERA"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取UpsUsed

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUPSUsed()
{
    return (m_TCMIni->GetDataInt(_T("UPS"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取UPS CommPort

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetUPSCommPort()
{
	return m_TCMIni->GetDataInt(_T("UPS"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////
/**
@brief       获取UPS BaudRate

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetUPSBaudRate()
{
	return m_TCMIni->GetDataInt(_T("UPS"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUPSMonitorOpened()
{
	return (m_TCMIni->GetDataInt(_T("UPS"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetUPSMonitorInterval()
{
	return m_TCMIni->GetDataInt(_T("UPS"),_T("MONITOR_INTERVAL"),150000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsLPTUsed()
{
		return (m_TCMIni->GetDataInt(_T("LPT"),_T("IS_USED"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		判断TH门禁监控是否开启

@param      none

@retval     bool true:开启  false:关闭

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsLPTMonitorOpened()
{
	return (m_TCMIni->GetDataInt(_T("LPT"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetLPTMonitorInterval()
{
	return m_TCMIni->GetDataInt(_T("LPT"),_T("MONITOR_INTERVAL"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得门禁关闭时的值  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD CTCMInfo::GetClosedDoorStatusValue()
{
	return m_TCMIni->GetDataInt(_T("LPT"),_T("DOOR_STATUS_CLOSED"),127);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取USB 使用的 盘符
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMUSBDrive()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("USBDRIVE")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取CF卡 使用的 盘符
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMCFDrive()
//{
//    return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("CFDRIVE"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 系统参数 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMSysParaUploadPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMSysParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 控制部程序 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMProgramUploadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMProgramUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 机器参数 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMMachineParaUploadPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMachineParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 机器参数 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMLogFileUploadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMLogFileUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质中 Data Audit 文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDataAuditPath(){
//	return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMDataAudit"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 Data Transaction 文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDataTransactionPath(){
//	return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMDataTransaction"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 Data Event 文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDataEventPath(){
//	return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMDataEvent"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 主数据库文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMMainDBUploadPath(){
//    return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMainDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 备份数据库文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMBackupDBUploadPath(){
//    return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMBackupDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 数据库 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDBDownloadPath(){
//    return m_TCMIni->GetDataString(_T("MOVEDISK"),_T("TCMDBDownloadPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 系统参数 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMSysParaDownloadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMSysParaDownloadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 控制部程序 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMProgramDownloadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMProgramDownloadPath")));
//}
//
//////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 移动介质 机器参数 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMMachineParaDownloadPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMachineParaDownloadPath")));
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 工作日志输出的路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMMaintentanceOutputPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMaintenance")));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取TCM 维护Trace的输出路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMMaintenceTracePath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMaintenanceTrace")));
//}
////////////////////////////////////////////////////////////////////////////
/**
@brief      取得数据纪录保存的天数

@param      none

@retval     int

@exception  none
*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetStoreDay()
{
    return m_TCMIni->GetDataInt(_T("LOG"), _T("StoreDay"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得数据纪录实际保存的天数

@param      none

@retval     int

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetRealStoreDay()
{
	return m_TCMIni->GetDataInt(_T("LOG"), _T("RealStoreDay"));
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得数据纪录实际保存的天数
//
//@param      none
//
//@retval     int
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetTouchLastTime()
//{
//    return m_TCMIni->GetDataInt(_T("Maintain"), _T("touchLastTime"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得一次发送Transaction电文的最大条数
//
//@param      none
//
//@retval     int
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetMaxTransMsgCnt()
//{
//    return m_TCMIni->GetDataInt(_T("CPS"),_T("MAX_TRANSACTION_MSG_CNT"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得一次发送Event电文的最大条数
//
//@param      none
//
//@retval     int
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetMaxEventMsgCnt()
//{
//    return m_TCMIni->GetDataInt(_T("CPS"),_T("MAX_EVENT_MSG_CNT"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得更换业务读取新卡等待时间
//
//@param      none
//
//@retval     int 等待时间
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetWaitReadCardTime()
//{
//    return m_TCMIni->GetDataInt(_T("EXCHANGE_SVC"), _T("WAIT_READ_CARD_TIME"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得等待一票通非即时退款查询电文响应的时间
//
//@param      none
//
//@retval     int 等待时间
//
//@exception  none
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetWaitQueryTime()
//{
//    return m_TCMIni->GetDataInt(_T("REFUND_SVC"), _T("WAIT_QUERY_TIME"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得应答代码对应信息
//
//@param      (i)OperationalMode_t operationalMode     应答代码
//
//@retval     CString
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetOperationalMode(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // 组合模式全部作为降级模式处理
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_TCMIni->GetDataString(_T("OPERATIONAL_MODE"), sMessageID);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得应答代码对应信息
//
//@param      (i)OperationalMode_t operationalMode     应答代码
//
//@retval     CString
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetOperationalModeEn(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // 组合模式全部作为降级模式处理
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_TCMIni->GetDataString(_T("OPERATIONAL_MODE_EN"), sMessageID);
//}
//
//////////////////////////////////////////////////////////////////////////

//brief      取得应答代码对应信息
//
//param      (i)RESULT_CODE_MSG resultCode     应答代码
//
//retval     CString  代码的中文信息
//
//exception  无

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetResultCodeMessage(RESULT_CODE_MSG resultCode)
//{
//    CString msg;
//    if (!m_mapResultCodeMsg.Lookup(resultCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), resultCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("RESULT_CODE_MSG"), sMessageID);
//        m_mapResultCodeMsg.SetAt(resultCode,resultCodeMsg);
//        msg = resultCodeMsg;
//    }
//    return msg;
//}

//////////////////////////////////////////////////////////////////////////

//@brief      取得车站模式文言
//
//@param      (i)OPERATION_MSG msgCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetOperationMessage(OPERATION_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapOperationMsg.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("OPERATION_MSG"), sMessageID);
//        m_mapOperationMsg.SetAt(msgCode,resultCodeMsg);
//        msg = resultCodeMsg;
//    }
//    return msg;
//}

////////////////////////////////////////////////////////////////////////////
//
////@brief      取得旅程状态文言(中)
////
////@param      (i)JourneyStatus_t journeyStatus   文言代码
////
////@retval     CString  代码的中文信息(中)
////
////@exception  无
//
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetJourneyStatusCn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;    
//}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得旅程状态文言(英)
//
//@param      (i)JourneyStatus_t journeyStatus   文言代码
//
//@retval     CString  代码的中文信息(英)
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetJourneyStatusEn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
//
//    // 中文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // 返回中文
//    return strMsgEn;    
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得交易类型文言(中)
//
//@param      (i)U16_t tranType   
//
//@retval     CString  代码的中文信息(中)
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTransactionTypeCn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得交易类型文言(英)
//
//@param      (i)U16_t tranType   
//
//@retval     CString  代码的中文信息(英)
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTransactionTypeEn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
//
//    // 中文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // 返回中文
//    return strMsgEn;
//}
//
//////////////////////////////////////////////////////////////////////////

//@brief      取得通常业务乘客画面文言
//
//@param      (i)PASSENGER_MSG msgCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerMessageCn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgCn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
//
//        // 中文
//        CString strMsgCn;
//        AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//        m_mapPassengerMsgCn.SetAt(msgCode,strMsgCn);
//
//        // 返回中文
//        msg = strMsgCn;
//    }
//    return msg;
//}

//////////////////////////////////////////////////////////////////////////
//
//@brief      取得通常业务乘客画面文言
//
//@param      (i)PASSENGER_MSG msgCode   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerMessageEn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgEn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
//
//        // 英文
//        CString strMsgEn;
//        AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//        m_mapPassengerMsgEn.SetAt(msgCode,strMsgEn);
//
//        // 返回英文
//        msg = strMsgEn;
//    }
//    return msg;
//}
//
////////////////////////////////////////////////////////////////////////////

//brief      取得维护业务操作员画面文言
//
//param      (i)MAINTENANCE_MSG msgCode   文言代码
//
//retval     CString  代码的中文信息
//
//exception  无

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetMaintenanceMessage(MAINTENANCE_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapMaintenanceMsg.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("MAINTENANCE_MSG"), sMessageID);
//        m_mapMaintenanceMsg.SetAt(msgCode,resultCodeMsg);
//        msg = resultCodeMsg;
//    }
//    return msg;
//}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得乘客类型文言(中)
//
//@param      (i)PassengerType_t type   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerTypeCn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得乘客类型文言(英)
//
//@param      (i)PassengerType_t type   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerTypeEn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
//
//    // 英文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//    
//    // 返回英文
//    return strMsgEn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得锁定状态文言(中)
//
//@param      (i)PassengerType_t type   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetLockStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
// 
//    // 返回中文
//    return strMsgCn;    
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得锁定状态文言(英)
//
//@param      (i)PassengerType_t type   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetLockStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
//
//    // 英文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // 返回英文
//    return strMsgEn;    
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得车票状态文言(中)
//
//@param      (i)CardStatusCode_t type   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetCardStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("CARD_STATUS"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;    
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得车票状态文言(英)
//
//@param      (i)CardStatusCode_t type   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetCardStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("CARD_STATUS"), sMessageID);
//
//    // 英文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':'); 
//
//    // 返回英文
//    return strMsgEn;    
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得一卡通卡状态(中)
//
//@param      (i)CARDSTATUS status   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTStatusCn(CARDSTATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_STATUS"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得一卡通旅程状态(中)
//
//@param      (i)CARDSTATUS status   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTJourneyStatusCn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得一卡通旅程状态(英)
//
//@param      (i)CARDSTATUS status   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTJourneyStatusEn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
//
//    // 英文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // 返回英文
//    return strMsgEn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代码(中)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTSystemCodeCn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代码(英)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTSystemCodeEn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
//
//    // 英文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // 返回英文
//    return strMsgEn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代码(中)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTTransTypeCn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代码(英)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTTransTypeEn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
//
//    // 中文
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // 返回中文
//    return strMsgEn;
//}
//
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代码(中)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTStaffTransTypeCn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
//
//	// 中文
//	CString strMsgCn = _T("");
//	AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//	// 返回中文
//	return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代码(英)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的英文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTStaffTransTypeEn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
//
//	// 中文
//	CString strMsgEn = _T("");
//	AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//	// 返回中文
//	return strMsgEn;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      取得系统代理商文言(中)
//
//@param      (i)BYTE systemCode   文言代码
//
//@retval     CString  代码的中文信息
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTAgenceCodeCn(BYTE agenceCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), agenceCode);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_AGENCECODE_TYPE"), sMessageID);
//
//    // 中文
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // 返回中文
//    return strMsgCn;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置读模式
//
//@param       (o)const int num
//
//@retval      void
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTCMInfo::SetReadMode(int num)
//{
//	CString str;
//	str.Format(_T("%d"), num);
//	m_TCMIni->WriteData(_T("ICRW"), _T("READ_MODE"), str);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置ACC认证方式
//
//@param       (i)int authorizeManner
//
//@retval      void
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTCMInfo::SetACCAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_TCMIni->WriteData(_T("APPLICATION"), _T("ACCAuthorizeManner"), str);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置ECT认证方式
//
//@param       (i)int authorizeManner
//
//@retval      void
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTCMInfo::SetECTAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_TCMIni->WriteData(_T("APPLICATION"), _T("ECTAuthorizeManner"), str);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置程序更新标志
//
//@param       (i)int updateFlag
//
//@retval      void
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTCMInfo::SetUpdateFlag(int updateFlag)
//{
//	CString str;
//	str.Format(_T("%d"), updateFlag);
//	m_TCMIni->WriteData(_T("APPLICATION"), _T("UpdateFlag"), str);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置ACC参数更新失败标志
//
//@param       (i)int updateFlag
//
//@retval      void
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTCMInfo::SetACCUpdateFlag(int updateFlag)
//{
//    CString str;
//    str.Format(_T("%d"), updateFlag);
//    m_TCMIni->WriteData(_T("APPLICATION"), _T("ACCUpdateFlag"), str);
//}
//

////////////////////////////////////////////////////////////////////////////
///**
//@brief       设置RFID可写状态
//
//@param       (o)int status
//
//@retval      void
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//void CTCMInfo::SetRFIDWriteStatus(int status)
//{
//	CString str;
//	str.Format(_T("%d"), status);
//	m_TCMIni ->WriteData(_T("APPLICATION"), _T("RFID_WRITEABLE_STATUS"), str);
//}
//



////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取 TCM系统正在使用的网卡名称
//
//@param       无 
//
//@retval      CString 
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMUsedAdapterName()
{
	return m_TCMIni->GetDataString(_T("MACHINE"), _T("USED_ADAPTER_NAME"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       取得收入原因数量

@param       无

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetIncomeReasonCount()
{
	return m_TCMIni->GetDataInt(_T("INCOME_REASON"),_T("COUNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得收入原因内容

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetIncomeReason(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	return m_TCMIni->GetDataString(_T("INCOME_REASON"), strNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得支出原因数量

@param       无

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetExpendReasonCount()
{
	return m_TCMIni->GetDataInt(_T("EXPENSE_REASON"),_T("COUNT"));
}

CString CTCMInfo::GetExpendReason( int NO )
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	return m_TCMIni->GetDataString(_T("EXPENSE_REASON"), strNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得补票建议内容（站员界面）

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetUpdateSuggestOfOperator(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,0,':');

	return strMsgEn;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       取得补票建议内容（乘客界面）

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetUpdateSuggestOfPassager(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,2,':');

	return strMsgEn;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       取得退款建议内容

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetUpdateSuggestEn(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');

	return strMsgEn;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得交易类型

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTransactionType(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("TRANSACTION_TYPE"), strNo);

	CString strMsg = _T("");
	AfxExtractSubString(strMsg,resultCodeMsg,0,':');

	return strMsg;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       取得退款建议内容

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetACCCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("ANALYSE_ACC_STATUS"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,0,':');

	return strMsgEn;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得记名卡申请结果内容

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetApplyCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("APPLY_STATE"), strNo);

	return resultCodeMsg;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       取得退款建议内容

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetACCCardStateEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("ANALYSE_ACC_STATUS"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');

	return strMsgEn;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得线路名称

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetLineName(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("LINE_NAME"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,0,':');

	return strMsgEn;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       取得线路英文名称

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetLineNameEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("LINE_NAME"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');

	return strMsgEn;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       
@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetECTCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("ANALYSE_ECT_STATUS"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,0,':');

	return strMsgEn;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief       

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetECTCardStateEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_TCMIni->GetDataString(_T("ANALYSE_ECT_STATUS"), strNo);

	CString strMsgEn = _T("");
	AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');

	return strMsgEn;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief   取得交易打包间隔 （单位：秒）  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTransactionPackageInterval()
{
	 return m_TCMIni->GetDataInt(_T("INTERVAL"), _T("TRANSACTION_PACKAGE"),30);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得业务数据打包间隔时间（单位：秒）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetBusinessPackageInterval()
{
	return m_TCMIni->GetDataInt(_T("INTERVAL"), _T("BUSINESS_PACKAGE"),30);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得回到默认业务的间隔时间（单位：秒） 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetReturnDefaultServiceInterval()
{
	return m_TCMIni->GetDataInt(_T("INTERVAL"), _T("RETURN_DEFAULT_SERVICE"),0);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得业务中等待操作的间隔时间（单位：秒）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetWaitOperationInterval()
{
	return m_TCMIni->GetDataInt(_T("INTERVAL"), _T("WAIT_OPERATION"),30);
}

