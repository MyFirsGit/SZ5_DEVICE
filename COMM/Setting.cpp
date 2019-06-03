#include "StdAfx.h"
#include "Setting.h"
#include "Util.h"
#include "MaintenanceInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SETTING_FILE_NAME _T("SETTING.INI")

CSetting CSetting::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取单实例对象

@param      (i)void

@retval      CSetting& CSetting类对象

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CSetting& CSetting::GetInstance()
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
CSetting::CSetting()
{
	try {
		m_settingIni = NULL;
		m_LanguageMode = MODE_UNKNOWN;
		// 初始化
		Initialize();
		// 获取语言模式
		m_LanguageMode = LANGUAGE_ID_CHINESE/*(LANGUAGETYPE_T)m_settingIni->GetDataInt(_T("APPLICATION"),_T("LanguageMode"),0)*/;
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
CSetting::~CSetting()
{
	delete m_settingIni;
	m_settingIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       初始化

@param       无 

@retval      int 0 成功，1 失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CSetting::Initialize() 
{
	// 若已初始化，则直接返回
	if (m_settingIni != NULL) {
		return 0;
	}

	TCHAR szAppPath[_MAX_PATH];

	if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
		return 1;
	}

	CString fileName = CString(szAppPath) + SETTING_FILE_NAME;

	// check TCM.INI file exist
	if (FALSE == ComCheckFileExist(fileName)) {
		return 1;
	}

	// 文件属性改为可写
	SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	m_settingIni = new CGetIni(fileName);

	Initialized.Invoke();

	return 0;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      在目录名后添加反斜杠

@param      (i)CString sPath    目录

@retval     CString     右面带反斜杠的目录名

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString CSetting::AddBackslash(CString sPath)
{
	if(sPath.IsEmpty()){
		return sPath;
	}

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
void CSetting::SetDeviceTestMode(int mode)
{
	CString str;
	str.Format(_T("%d"),mode);
	m_settingIni->WriteData(_T("APPLICATION"),_T("DeviceTestMode"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取设备测试模式

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CSetting::GetDeviceTestMode()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"),_T("DeviceTestMode"),1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置语言模式   

@param       (o)int mode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetLanguageMode(LANGUAGETYPE_T mode)
{
	LANGUAGETYPE_T preMode = m_LanguageMode;
	if(mode!=preMode){
		CString str;
		str.Format(_T("%d"), mode);
		m_settingIni->WriteData(_T("APPLICATION"),_T("LanguageMode"),str);
		m_LanguageMode = mode;
		LanguageModeChanged.Invoke(preMode,mode);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取语言模式

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CSetting::GetLanguageMode()
{
	return m_LanguageMode;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief       获取打印员工ID

@param       无 

@retval      CString

@exception   无
*/
////////////////////////////////////////////////////////////////////////////
CString CSetting::GetPrintStaffID()
{
	return m_settingIni->GetDataString(_T("APPLICATION"),_T("PrintStaffID"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取打印登录时间

@param       无 

@retval      _DATE_TIME

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME CSetting::GetPrintLoginTime()
{
	_DATE_TIME printLoginTime;

	CString strLoginTime = m_settingIni->GetDataString(_T("APPLICATION"), _T("PrintLoginTime"));

	printLoginTime.wYear = _ttoi(strLoginTime.Left(4));
	printLoginTime.biMonth = _ttoi(strLoginTime.Mid(4,2));
	printLoginTime.biDay = _ttoi(strLoginTime.Mid(6,2));
	printLoginTime.biHour = _ttoi(strLoginTime.Mid(8,2));
	printLoginTime.biMinute = _ttoi(strLoginTime.Mid(10,2));
	printLoginTime.biSecond = _ttoi(strLoginTime.Right(2));

	return printLoginTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置上传的文件路径

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetFileUploadFTPDir(CString scPath)
{
	m_settingIni->WriteData(_T("FTP"),_T("FileUploadFTPDir"),scPath);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取FTP上传的文件路径

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CSetting::GetFileUploadFTPDir()
{
	CString s = m_settingIni->GetDataString(_T("FTP"),_T("FileUploadFTPDir"));
	s = s.TrimRight(_T("/")) + _T("/");
	s = s + theMAINTENANCE_INFO.GetCurrentDevice().ToString() + _T("/");
	return s;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH模式

@param       none 

@retval      int 0:正常模式; 1:缩退模式

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CSetting::GetTHMode()
{
	return m_settingIni->GetDataInt(_T("TH"), _T("TH_MODE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置TH模式

@param       (i)int  sTHMode 0:正常模式; 1:缩退模式 

@retval      none

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetTHMode(int sTHMode)
{
	CString str;
	str.Format(_T("%d"), sTHMode);
	m_settingIni ->WriteData(_T("TH"), _T("TH_MODE"), str);
	THModeChanged.Invoke(sTHMode);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief       获取RFID可写状态

@param       none 

@retval      int 0:禁止读写 1:允许读写

@exception   none
*/
////////////////////////////////////////////////////////////////////////////
int  CSetting::GetRFIDWriteStatus()
{
	return m_settingIni->GetDataInt(_T("RFID"), _T("RFIDWriteableStatus"), 0);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief       设置RFID可写状态

@param       (i)bool isUsed  true:可用  false:禁用

@retval      int 0:禁止读写 1:允许读写

@exception   none
*/
////////////////////////////////////////////////////////////////////////////
void  CSetting::SetRFIDWriteStatus(bool isUsed)
{
	CString str;
	str.Format(_T("%d"), (int)isUsed);
	m_settingIni->WriteData(_T("RFID"),_T("RFIDWriteableStatus"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器更新成功标志

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int 0:成功 1:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CSetting::GetRWUpdateFlag(bool isRW)
{
	if(isRW){
		return m_settingIni->GetDataInt(_T("ICRW"),_T("UpdateError"));
	}
	return m_settingIni->GetDataInt(_T("ICTW"),_T("UpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置读写器程序更新失败标志

@param       (i)int updateFlag

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetRWUpdateFlag(int updateFlag,bool isRW)
{
	CString str;
	str.Format(_T("%d"),updateFlag);
	if(isRW){
		m_settingIni->WriteData(_T("ICRW"),_T("UpdateError"),str);
	}
	else{
		m_settingIni->WriteData(_T("ICTW"),_T("UpdateError"),str);
	}	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置打印员工ID

@param       (i)CString printStaffID

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetPrintStaffID(CString printStaffID)
{
	m_settingIni->WriteData(_T("APPLICATION"), _T("PrintStaffID"), printStaffID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置打印登录时间

@param       (i)_DATE_TIME printLoginTime

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetPrintLoginTime(_DATE_TIME printLoginTime)
{
	CString str;
	str.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),printLoginTime.wYear,printLoginTime.biMonth,printLoginTime.biDay,
		printLoginTime.biHour,printLoginTime.biMinute,printLoginTime.biSecond);
	m_settingIni->WriteData(_T("APPLICATION"), _T("PrintLoginTime"), str);
}