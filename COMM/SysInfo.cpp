#include "StdAfx.h"
#include "SysInfo.h"
#include "SysException.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSYSInfo::CSYSInfo()
{
    try {
        m_DeviceIni = NULL;
		
        // 初期化
        //Initialize();
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
CSYSInfo::~CSYSInfo()
{
    delete m_DeviceIni;
    m_DeviceIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初期化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CSYSInfo::Initialize() 
{
    try {
        // 若已初始化，则直接返回
        if (m_DeviceIni != NULL) {
            return 0;
        }

        TCHAR szAppPath[_MAX_PATH];
        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }
		CString strMaintenanceFileName = CString(szAppPath)  + MAINTENANCE_INI_FILE_NAME;
		CString strExeFileName = GetIniDataString(strMaintenanceFileName,_T("APPLICATION"),_T("ProgName"),_T("TVM"));

        CString fileName = CString(szAppPath) + strExeFileName+_T(".INI");

        // check DEVICE.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // 文件属性改为可写
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_DeviceIni = new CGetIni(fileName);
		
		//Initialized.Invoke();
		m_strLogPath = _T("");
		m_strDataPath = _T("");
		m_strDataBackpath = _T("");
		m_strPrintPath = _T("");
		return 0;
   }
    catch(CSysException&) {
        throw;
    }
    catch (...) {
        //throw CInnerException(ROOT_SVC, CInnerException::OTHER_ERR, __FILE__, __LINE__);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      在目录名后添加反斜杠

@param      (i)CString sPath    目录

@retval     CString     右面带反斜杠的目录名

@exception  none
*/
////////////////////////////////////////////////////////////////////////
CString CSYSInfo::AddBackslash(CString sPath)
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
CString CSYSInfo::GetInitialVersion()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       是否显示在最前面

@param       无

@retval      bool true:在最前面 ; false:不在最前面

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::IsTopmost()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("TopMost"), 1) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       是否启用功能键

@param       无

@retval      bool true:启用 ; false:不启用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::IsFunctionKeyEnabled()
{
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("FunctionKeyEnable"), 1) != 0 ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       是否启用任务管理器

@param       无

@retval      bool true:启用 ; false:不启用

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::IsTaskManagerEnabled()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("TaskManagerEnabled"), 1) != 0 ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       是否显示桌面

@param       无

@retval      bool true:显示 ; false:不显示

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool    CSYSInfo::IsDesktopEnabled()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("DesktopEnabled"), 1) != 0 ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       是否显示鼠标光标

@param       无

@retval      bool true:显示 ; false:不显示

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::CanShowCursor()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       主数据目录警告（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetDataDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("DataDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       主数据目录故障（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetDataDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("DataDirectoryStop"));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief       备份数据目录警告（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetBackupDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("BackupDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       备份数据目录故障（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetBackupDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("BackupDirectoryStop"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       日志文件目录警告（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetLogDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("LogDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       日志文件目录故障（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetLogDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("LogDirectoryStop"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       系统目录警告（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetSysDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("SystemDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       系统目录故障（单位：M）

@param       无 

@retval      int

@exception   无
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetSysDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("SystemDirectoryStop"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取主数据目录

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetDataPath()
{
	if(m_strDataPath.IsEmpty()){
		m_strDataPath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("DataPath"),_T("D:\\TVM_Data\\"));
	}
	return m_strDataPath;
    //return AddBackslash(m_DeviceIni->GetDataString(_T("APP_PATH"), _T("DataPath")));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取备份目录

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetBackupPath()
{
	if(m_strDataBackpath.IsEmpty()){
		m_strDataBackpath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("BackupPath"),_T("E:\\TVM_Backup\\"));
	}
	return m_strDataBackpath;
    //return AddBackslash(m_DeviceIni->GetDataString(_T("APP_PATH"), _T("BackupPath")));
}

////////////////////////////////////////////////////////////////////
/**
@brief       获取CF卡 使用的 盘符

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetCFDrive()
{
    return m_DeviceIni->GetDataString(_T("APP_PATH"), _T("CFDRIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取USB 使用的 盘符

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetUSBDrive()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("APP_PATH"), _T("USBDRIVE")));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取备份目录

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetLogPath()
{
	if(m_strLogPath.IsEmpty()){
		m_strLogPath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("LogPath"),_T("E:\\Log\\"));
	}
	return m_strLogPath;

	/*CString strLogPath =  m_DeviceIni->GetDataString(_T("APP_PATH"), _T("LogPath"),_T(""));
	if(strLogPath == _T(""))
	{
	strLogPath = GetBackupPath() + _T("\\Log");
	}
	else
	{
	strLogPath = AddBackslash(strLogPath);
	}
	return strLogPath;*/
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取USB数据导出子目录

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetExportPath()
{
	return AddBackslash(m_DeviceIni->GetDataString(_T("APP_PATH"), _T("ExportPath")));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取FTP临时下载路径

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetFileDownloadLocalDir()
{
	return m_DeviceIni->GetDataString(_T("APP_PATH"), _T("FileDownloadLocalDir"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   取得交易打包间隔 （单位：秒）  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetTransactionPackageInterval()
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
int CSYSInfo::GetBusinessPackageInterval()
{
	return m_DeviceIni->GetDataInt(_T("INTERVAL"), _T("BUSINESS_PACKAGE"),30);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得凭证数据的路径

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetGetReceiptLogPath()
{
	if(m_strPrintPath.IsEmpty()){
		m_strPrintPath = m_DeviceIni->GetDataString(_T("APP_PATH"),_T("ReceiptPath"),_T("D:\\TVM_Data\\PrintHistory\\"));
	}
	return m_strPrintPath;//m_DeviceIni->GetDataString(_T("APP_PATH"),_T("ReceiptPath"),_T(""));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       是否记录Debug消息

@param       无

@retval      bool true:记录Debug消息 ; false:不记录Debug消息

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
 bool CSYSInfo::IsWriteDebug()
 {
    return m_DeviceIni->GetDataInt(_T("LOG"), _T("WriteDebug")) == 0 ? false : true;
 }

 ////////////////////////////////////////////////////////////////////////////
 /**
 @brief      取得日志保存的天数

 @param      none

 @retval     int

 @exception  none
 */
 ////////////////////////////////////////////////////////////////////////////
 int CSYSInfo::GetStoreDay()
 {
	 return m_DeviceIni->GetDataInt(_T("LOG"), _T("StoreDay"));
 } 

 ////////////////////////////////////////////////////////////////////////////
 /**
 @brief      设置日志保存的天数

 @param      none

 @retval     int

 @exception  none
 */
 ////////////////////////////////////////////////////////////////////////////
 void CSYSInfo::SetStoreDay(int nDays)
 {
	 CString strTemp;
	 strTemp.Format(_T("%d"),nDays);
	 m_DeviceIni->WriteData(_T("LOG"), _T("StoreDay"),strTemp);
 }

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取系统正在使用的网卡名称

@param       无 

@retval      CString 

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetUsedAdapterName()
{
	return m_DeviceIni->GetDataString(_T("MACHINE"), _T("USED_ADAPTER_NAME"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得读写器在配置文件中的section名称  

@param (i)RW_TYPE rwType 读写器类型

@retval     CString 读写器在配置文件中的section名称

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetRWTypeString(RW_TYPE rwType)
{
	switch(rwType)	{
	case BOM_DESKTOP_RW : return _T("BOM_DESKTOP_RW");
	case BOM_CARD_RW : return _T("BOM_CARD_RW");
	case BOM_TOKEN_RW : return _T("BOM_TOKEN_RW");
	case TVM_RECHARGE_RW   : return _T("TVM_RECHARGE_RW");
	case TVM_CARD_RW     : return _T("TVM_CARD_RW");
	case TVM_TOKEN_RW : return _T("TVM_TOKEN_RW");
	case AGM_CARD_RECYCLE_RW : return _T("AGM_CARD_RECYCLE_RW");
	case AGM_TOKEN_RECYCLE_RW : return _T("AGM_TOKEN_RECYCLE_RW");
	case AGM_ENTRY_RW   : return _T("AGM_ENTRY_RW");
	case AGM_EXIT_RW     : return _T("AGM_EXIT_RW");
	case ES_TOKEN_RW1 : return _T("ES_TOKEN_RW1");
	case ES_TOKEN_RW2 : return _T("ES_TOKEN_RW2");
	case ES_TOKEN_RW3 : return _T("ES_TOKEN_RW3");
	case ES_CARD_RW   : return _T("RW_ES_CARD");
	case PM_RW     : return _T("PM_RW");
	case TCM_RW     : return _T("TCM_RW");
	default:
		return _T("RW");
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得电子标签在配置文件中的名称  

@param

@retval   CString  

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetRfidTypeString(RFID_NO rfidId)
{	
	switch(rfidId)	{
	case RFID_1 : return _T("RFID_ONE");
	case RFID_2 : return _T("RFID_TWO");
	case RFID_3 : return _T("RFID_THREE");
	case RFID_4 : return _T("RFID_FOUR");
	default:
		return _T("RFID");
	}
}

 //////////////////////////////////////////////////////////////////////////
 /**
 @brief       是否开启守护进程

 @param       无

 @retval      bool true:开启守护进程 ; false:不开启守护进程

 @exception   无
 */
 //////////////////////////////////////////////////////////////////////////
 bool CSYSInfo::IsUsedGuardian()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("IsUsedGuardian")) == 0 ? false : true;
 }

 //////////////////////////////////////////////////////////////////////////
 /**
 @brief       获取状态栏刷新时间间隔

 @param        

 @retval      int

 @exception   无
 */
 //////////////////////////////////////////////////////////////////////////
 int CSYSInfo::GetStatusTimerInterval()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("StatusTimerInterval"), 1000);
 }
 
 //////////////////////////////////////////////////////////////////////////
 /**
 @brief			（待机时有人靠近）是否自动返回默认业务

 @param        

 @retval      bool-> true:自动返回  false：不自动返回

 @exception   无
 */
 //////////////////////////////////////////////////////////////////////////
 bool CSYSInfo::IsAutoReturnDefaultService()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("IsAutoReturnDefaultService")) == 0 ? false : true;
 }

  //////////////////////////////////////////////////////////////////////////
 /**
 @brief			是否啓用按鍵音

 @param        

 @retval      bool-> true:自动返回  false：不自动返回

 @exception   无
 */
 //////////////////////////////////////////////////////////////////////////
 bool    CSYSInfo::IsEnableClickSound()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("EnableClickSound")) == 0 ? false : true;
 }