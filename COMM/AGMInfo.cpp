#include "StdAfx.h"
#include "AGMInfo.h"
#include "SysException.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAGMInfo CAGMInfo::theInstance;

CAGMInfo& CAGMInfo::GetInstance()
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
CAGMInfo::CAGMInfo()
{
    try {
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
CAGMInfo::~CAGMInfo()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取初期安装的版本信息

@param       无 

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetInitialVersion()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
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
bool CAGMInfo::IsTopmost()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("TopMost"), 1) == 0 ? false : true;
}

bool CAGMInfo::IsFunctionKeyEnabled()
{
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("FunctionKeyEnable"), 1) != 0 ;
}
bool CAGMInfo::IsTaskManagerEnabled()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("TaskManagerEnabled"), 1) != 0 ;
}

bool    CAGMInfo::IsDesktopEnabled()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("DesktopEnabled"), 1) != 0 ;
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
bool CAGMInfo::CanShowCursor()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
}
//
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
CString CAGMInfo::GetDataPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("APPLICATION"), _T("DataPath")));
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
CString CAGMInfo::GetBackupPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("APPLICATION"), _T("BackupPath")));
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
CString CAGMInfo::GetLogPath()
{
	CString strLogPath =  m_DeviceIni->GetDataString(_T("APPLICATION"), _T("LogPath"),_T(""));
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
//@brief       获取允许最大未送数据天数
//
//@param       无 
//
//@retval      UINT
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
UINT CAGMInfo::GetLimitedDaysOfUnSendData()
{
	return m_DeviceIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
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
UINT CAGMInfo::GetLimitedDaysOfSendData()
{
	return m_DeviceIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取BOM 控制部程序 上传路径

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomExportPath()
{
	return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("ExportPath")));
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
//int CAGMInfo::GetMaxRecoverTimes()
//{
//    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("MaxRecoverTimes"), 10);
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
//bool CAGMInfo::IsLoginTimeFixed()
//{
//    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("LoginTimeFixed")) == 0 ? false : true;
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
 bool CAGMInfo::IsWriteDebug()
 {
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("WriteDebug")) == 0 ? false : true;
 }

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取功能按钮字体名称

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetFuncBtnFont()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("FuncBtnFont"),_T("宋体"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取功能按钮字体大小

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetFuncBtnFontSize()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"),_T("FuncBtnFontSize"), 16);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取业务按钮字体名称

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetOperateBtnFont()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("OperateBtnFont"),_T("宋体"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取业务按钮字体大小

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetOperateBtnFontSize()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"),_T("OperateBtnFontSize"), 12);
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
bool CAGMInfo::IsUseFounderStaffID()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取欧姆龙员工ID

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetFounderStaffID()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("FounderStaffID"), 10101028);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsFounderStaffID(int userID)
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
CString CAGMInfo::GetFounderStaffPassword()
{
	return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        是否启用自定义车票种类

@param       无 

@retval      bool

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsUseConfigTicketCategory()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UseConfigTicketCategory"),0) == 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        取启用自定义车票种类

@param       ticketTypeCode 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetConfigTicketCategory(WORD ticketTypeCode)
{
	CString strTicketCode = _T("");
	strTicketCode.Format(_T("%d"),ticketTypeCode);
	return m_DeviceIni->GetDataInt(_T("TICKET_CATEGORY"),strTicketCode,-1);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief        取得业务结束最大等待时间单位（秒）

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

int CAGMInfo::GetCloseOperationMaxWaitSeconds()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("CloseOperationMaxWaitSeconds"),10*60);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       // 是否启用CPU卡即时退卡

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::IsEnableCPUImmidatelyRefund()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("EnableCPUImmidatelyRefund"),0) == 1;
}

bool CAGMInfo::IsValidateUnSendPackage()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ValidateUnSendPackage"),1) != 99;
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
//bool CAGMInfo::IsAuthorizeSet()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("IsAuthorizeSet")) == 0 ? false : true;
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
//int  CAGMInfo::GetACCAuthorizeManner()
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
//int  CAGMInfo::GetECTAuthorizeManner()
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
//int CAGMInfo::GetMCBFCount()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("MCBFCount"), 3);
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
//int CAGMInfo::GetUpdateFlag()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UpdateFlag"), 0);
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
//int CAGMInfo::GetACCUpdateFlag()
//{
//    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ACCUpdateFlag"), 0);
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
//int  CAGMInfo::GetRFIDWriteStatus()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("RFIDWriteableStatus"), 0);
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取USB认证方式（0：不认证；1：需要认证）

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetUSBAuthFlag()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"), 0);
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
//CString CAGMInfo::GetFtpLogPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("LogPath"));
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
//CString CAGMInfo::GetFtpSendFailLogPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("SendFailLogPath"));
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
//CString CAGMInfo::GetTestFileDownloadFTPPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("TestFileDownloadFTPPath"));
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
//CString CAGMInfo::GetTestFileDownloadLocalDir()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("TestFileDownloadLocalDir"));
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
//CString CAGMInfo::GetTestFileUploadLocalPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("TestFileUploadLocalPath"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH通信方式

@param       none 

@retval      int  0:串口 1:邮槽 2:管道 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHCommuMethod()
{
	return m_DeviceIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH串口号

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHCommPort()
{
    return m_DeviceIni->GetDataInt(_T("TH"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1BaudRate

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHBaudRate()
{
    return m_DeviceIni->GetDataInt(_T("TH"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnReceive

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHMaxConnReceive()
{
    return m_DeviceIni->GetDataInt(_T("TH"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect1MaxConnSend

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHMaxConnSend()
{
    return m_DeviceIni->GetDataInt(_T("TH"), _T("MAX_CONN_SEND"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsTHMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("TH"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("TH"),_T("MONITOR_INTERVAL"),5000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用读写器

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      bool true:使用读写器  false:使用模拟数据

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsRWUsed(bool isRW)
{
	if(isRW){
		return (m_DeviceIni->GetDataInt(_T("ICRW"), _T("IS_USED")) == 1);
	}
	return (m_DeviceIni->GetDataInt(_T("ICTW"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool	CAGMInfo::IsRWMonitorOpened(bool bIsRW)
{
	if(bIsRW){
		return m_DeviceIni->GetDataInt(_T("ICRW"), _T("MONITOR_SWITCH"),0)!=0;
	}
	return m_DeviceIni->GetDataInt(_T("ICTW"), _T("MONITOR_SWITCH"),0)!=0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int     CAGMInfo::GetRWMonitorInterval(bool bIsRW)
{
	if(bIsRW){
		return m_DeviceIni->GetDataInt(_T("ICRW"), _T("MONITOR_INTERVAL"),30);
	}
	return m_DeviceIni->GetDataInt(_T("ICTW"), _T("MONITOR_INTERVAL"),30);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取外部读写器串口号

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int 串口号

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetRWCommPort(bool isRW)
{
	if(isRW){
		return m_DeviceIni->GetDataInt(_T("ICRW"), _T("COMM_PORT"));
	}
    return m_DeviceIni->GetDataInt(_T("ICTW"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取外部读写器串口波特率

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int 串口波特率

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetRWBaudRate(bool isRW)
{
	if(isRW){
		return m_DeviceIni->GetDataInt(_T("ICRW"), _T("BAUD_RATE"));
	}
    return m_DeviceIni->GetDataInt(_T("ICTW"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取外部读写器

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//int CAGMInfo::GetRWMaxConnReceive()
//{
//    return m_DeviceIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_RECEIVE"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取Connect2MaxConnSend

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
//int CAGMInfo::GetRWMaxConnSend()
//{
//    return m_DeviceIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_SEND"));
//}



//////////////////////////////////////////////////////////////////////////
/**
@brief       取得模拟卡物理类型，读卡使用

@param       (i)bool isRW  true:外部读写器  false:内部读写器 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetSimCardType(bool isRW)
{
	if (isRW){
		return m_DeviceIni->GetDataInt(_T("ICRW"), _T("SIM_CARD_TYPE"));
	}
	return m_DeviceIni->GetDataInt(_T("ICTW"), _T("SIM_CARD_TYPE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得读卡时间间隔

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CAGMInfo::GetReadCardInterval(bool isRW)
{
	if (isRW){
		return m_DeviceIni->GetDataInt(_T("ICRW"), _T("READ_CARD_INTERVAL"));
	}
	return m_DeviceIni->GetDataInt(_T("ICTW"), _T("READ_CARD_INTERVAL"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用打印机

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsPrinterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("PRINTER"), _T("IS_USED")) == 1);
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
int CAGMInfo::GetPrinterCommPort()
{
    return m_DeviceIni->GetDataInt(_T("PRINTER"), _T("COMM_PORT"));
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
int CAGMInfo::GetPrinterBaudRate()
{
   return m_DeviceIni->GetDataInt(_T("PRINTER"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用票卡打印机

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsCardPrinterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_PRINTER"), _T("IS_USED")) == 1);
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       获取打印机名称

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetCardPrinterDriverName()
{
	return m_DeviceIni->GetDataString(_T("CARD_PRINTER"), _T("DRIVER_NAME"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       获取票卡打印机打印临时文件名

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetPrinterTempFile()
{
	return m_DeviceIni->GetDataString(_T("CARD_PRINTER"), _T("PRT_TEMP"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       获取票卡打印机驱动名称

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetCardType()
{
	return m_DeviceIni->GetDataInt(_T("CARD_PRINTER"), _T("CARD_TYPE"));	
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       发卡测试次数

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTestSendCardCount()
{
	return m_DeviceIni->GetDataInt(_T("CARD_PRINTER"), _T("TEST_SEND_CARD_COUNT "));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       判断是否使用摄像头

@param       无 

@retval      bool

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsCameraUsed()
{
	return (m_DeviceIni->GetDataInt(_T("CAMERA"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取UpsUsed

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsUPSUsed()
{
    return (m_DeviceIni->GetDataInt(_T("UPS"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取UPS CommPort

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetUPSCommPort()
{
	return m_DeviceIni->GetDataInt(_T("UPS"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////
/**
@brief       获取UPS BaudRate

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetUPSBaudRate()
{
	return m_DeviceIni->GetDataInt(_T("UPS"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsUPSMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("UPS"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetUPSMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("UPS"),_T("MONITOR_INTERVAL"),150000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsLPTUsed()
{
		return (m_DeviceIni->GetDataInt(_T("LPT"),_T("IS_USED"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		判断TH门禁监控是否开启

@param      none

@retval     bool true:开启  false:关闭

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsLPTMonitorOpened()
{
	return (m_DeviceIni->GetDataInt(_T("LPT"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetLPTMonitorInterval()
{
	return m_DeviceIni->GetDataInt(_T("LPT"),_T("MONITOR_INTERVAL"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得门禁关闭时的值  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD CAGMInfo::GetClosedDoorStatusValue()
{
	return m_DeviceIni->GetDataInt(_T("LPT"),_T("DOOR_STATUS_CLOSED"),127);
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
CString CAGMInfo::GetBOMUSBDrive()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("USBDRIVE")));
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
//CString CAGMInfo::GetBOMCFDrive()
//{
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("CFDRIVE"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取BOM 系统参数 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomSysParaUploadPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomSysParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取BOM 控制部程序 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomProgramUploadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomProgramUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取BOM 机器参数 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomMachineParaUploadPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMachineParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取BOM 机器参数 上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomLogFileUploadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomLogFileUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取BOM 移动介质中 Data Audit 文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDataAuditPath(){
//	return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomDataAudit"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 Data Transaction 文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDataTransactionPath(){
//	return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomDataTransaction"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 Data Event 文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDataEventPath(){
//	return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomDataEvent"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 主数据库文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomMainDBUploadPath(){
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMainDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 备份数据库文件夹的上传路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomBackupDBUploadPath(){
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomBackupDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 数据库 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDBDownloadPath(){
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"),_T("BomDBDownloadPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 系统参数 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomSysParaDownloadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomSysParaDownloadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 控制部程序 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomProgramDownloadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomProgramDownloadPath")));
//}
//
//////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 移动介质 机器参数 文件夹的下载路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomMachineParaDownloadPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMachineParaDownloadPath")));
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 工作日志输出的路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomMaintentanceOutputPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMaintenance")));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取Bom 维护Trace的输出路径
//
//@param       无 
//
//@retval      CString
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomMaintenceTracePath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMaintenanceTrace")));
//}
////////////////////////////////////////////////////////////////////////////
/**
@brief      取得数据纪录保存的天数

@param      none

@retval     int

@exception  none
*/
////////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetStoreDay()
{
    return m_DeviceIni->GetDataInt(_T("LOG"), _T("StoreDay"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得数据纪录实际保存的天数

@param      none

@retval     int

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetRealStoreDay()
{
	return m_DeviceIni->GetDataInt(_T("LOG"), _T("RealStoreDay"));
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
//int CAGMInfo::GetTouchLastTime()
//{
//    return m_DeviceIni->GetDataInt(_T("Maintain"), _T("touchLastTime"));
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
//int CAGMInfo::GetMaxTransMsgCnt()
//{
//    return m_DeviceIni->GetDataInt(_T("CPS"),_T("MAX_TRANSACTION_MSG_CNT"));
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
//int CAGMInfo::GetMaxEventMsgCnt()
//{
//    return m_DeviceIni->GetDataInt(_T("CPS"),_T("MAX_EVENT_MSG_CNT"));
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
//int CAGMInfo::GetWaitReadCardTime()
//{
//    return m_DeviceIni->GetDataInt(_T("EXCHANGE_SVC"), _T("WAIT_READ_CARD_TIME"));
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
//int CAGMInfo::GetWaitQueryTime()
//{
//    return m_DeviceIni->GetDataInt(_T("REFUND_SVC"), _T("WAIT_QUERY_TIME"));
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
//CString CAGMInfo::GetOperationalMode(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // 组合模式全部作为降级模式处理
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_DeviceIni->GetDataString(_T("OPERATIONAL_MODE"), sMessageID);
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
//CString CAGMInfo::GetOperationalModeEn(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // 组合模式全部作为降级模式处理
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_DeviceIni->GetDataString(_T("OPERATIONAL_MODE_EN"), sMessageID);
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
//CString CAGMInfo::GetResultCodeMessage(RESULT_CODE_MSG resultCode)
//{
//    CString msg;
//    if (!m_mapResultCodeMsg.Lookup(resultCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), resultCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("RESULT_CODE_MSG"), sMessageID);
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
//CString CAGMInfo::GetOperationMessage(OPERATION_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapOperationMsg.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("OPERATION_MSG"), sMessageID);
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
//CString CAGMInfo::GetJourneyStatusCn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetJourneyStatusEn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetTransactionTypeCn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetTransactionTypeEn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetPassengerMessageCn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgCn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
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
//CString CAGMInfo::GetPassengerMessageEn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgEn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
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
//CString CAGMInfo::GetMaintenanceMessage(MAINTENANCE_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapMaintenanceMsg.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("MAINTENANCE_MSG"), sMessageID);
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
//CString CAGMInfo::GetPassengerTypeCn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
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
//CString CAGMInfo::GetPassengerTypeEn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
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
//CString CAGMInfo::GetLockStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
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
//CString CAGMInfo::GetLockStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
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
//CString CAGMInfo::GetCardStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("CARD_STATUS"), sMessageID);
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
//CString CAGMInfo::GetCardStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("CARD_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTStatusCn(CARDSTATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTJourneyStatusCn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTJourneyStatusEn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTSystemCodeCn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
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
//CString CAGMInfo::GetYKTSystemCodeEn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
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
//CString CAGMInfo::GetYKTTransTypeCn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTTransTypeEn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTStaffTransTypeCn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTStaffTransTypeEn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTAgenceCodeCn(BYTE agenceCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), agenceCode);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_AGENCECODE_TYPE"), sMessageID);
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
//void CAGMInfo::SetReadMode(int num)
//{
//	CString str;
//	str.Format(_T("%d"), num);
//	m_DeviceIni->WriteData(_T("ICRW"), _T("READ_MODE"), str);
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
//void CAGMInfo::SetACCAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_DeviceIni->WriteData(_T("APPLICATION"), _T("ACCAuthorizeManner"), str);
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
//void CAGMInfo::SetECTAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_DeviceIni->WriteData(_T("APPLICATION"), _T("ECTAuthorizeManner"), str);
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
//void CAGMInfo::SetUpdateFlag(int updateFlag)
//{
//	CString str;
//	str.Format(_T("%d"), updateFlag);
//	m_DeviceIni->WriteData(_T("APPLICATION"), _T("UpdateFlag"), str);
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
//void CAGMInfo::SetACCUpdateFlag(int updateFlag)
//{
//    CString str;
//    str.Format(_T("%d"), updateFlag);
//    m_DeviceIni->WriteData(_T("APPLICATION"), _T("ACCUpdateFlag"), str);
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
//void CAGMInfo::SetRFIDWriteStatus(int status)
//{
//	CString str;
//	str.Format(_T("%d"), status);
//	m_DeviceIni ->WriteData(_T("APPLICATION"), _T("RFID_WRITEABLE_STATUS"), str);
//}
//



////////////////////////////////////////////////////////////////////////////
///**
//@brief       获取 BOM系统正在使用的网卡名称
//
//@param       无 
//
//@retval      CString 
//
//@exception   无
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomUsedAdapterName()
{
	return m_DeviceIni->GetDataString(_T("MACHINE"), _T("BOMUSED_ADAPTER_NAME"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       取得收入原因数量

@param       无

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetIncomeReasonCount()
{
	return m_DeviceIni->GetDataInt(_T("INCOME_REASON"),_T("COUNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得收入原因内容

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetIncomeReason(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	return m_DeviceIni->GetDataString(_T("INCOME_REASON"), strNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得支出原因数量

@param       无

@retval      int 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetExpendReasonCount()
{
	return m_DeviceIni->GetDataInt(_T("EXPENSE_REASON"),_T("COUNT"));
}

CString CAGMInfo::GetExpendReason( int NO )
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	return m_DeviceIni->GetDataString(_T("EXPENSE_REASON"), strNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       取得补票建议内容（站员界面）

@param       (i)int NO

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetUpdateSuggestOfOperator(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

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
CString CAGMInfo::GetUpdateSuggestOfPassager(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

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
CString CAGMInfo::GetUpdateSuggestEn(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

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
CString CAGMInfo::GetTransactionType(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TRANSACTION_TYPE"), strNo);

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
CString CAGMInfo::GetACCCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("ANALYSE_ACC_STATUS"), strNo);

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
CString CAGMInfo::GetApplyCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("APPLY_STATE"), strNo);

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
CString CAGMInfo::GetACCCardStateEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("ANALYSE_ACC_STATUS"), strNo);

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
CString CAGMInfo::GetLineName(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("LINE_NAME"), strNo);

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
CString CAGMInfo::GetLineNameEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("LINE_NAME"), strNo);

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
CString CAGMInfo::GetECTCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("ANALYSE_ECT_STATUS"), strNo);

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
CString CAGMInfo::GetECTCardStateEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("ANALYSE_ECT_STATUS"), strNo);

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
int CAGMInfo::GetTransactionPackageInterval()
{
	 return m_DeviceIni->GetDataInt(_T("INTERVAL"), _T("TRANSACTION_PACKAGE"),30);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      取得业务数据打包间隔时间（单位：秒）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetBusinessPackageInterval()
{
	return m_DeviceIni->GetDataInt(_T("INTERVAL"), _T("BUSINESS_PACKAGE"),30);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     取得回到默认业务的间隔时间（单位：秒） 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetReturnDefaultServiceInterval()
{
	return m_DeviceIni->GetDataInt(_T("INTERVAL"), _T("RETURN_DEFAULT_SERVICE"),120);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得业务中等待操作的间隔时间（单位：秒）

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetWaitOperationInterval()
{
	return m_DeviceIni->GetDataInt(_T("INTERVAL"), _T("WAIT_OPERATION"),120);
}