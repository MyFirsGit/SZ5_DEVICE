#include "StdAfx.h"
#include "tvmsetting.h"
#include "Util.h"
#include "MaintenanceInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SETTING_FILE_NAME _T("TVM_SETTING.INI")

CTvmSetting CTvmSetting::theInstance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取单实例对象

@param      (i)void

@retval      CSetting& CSetting类对象

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CTvmSetting& CTvmSetting::GetInstance()
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
CTvmSetting::CTvmSetting()
{
	try {
		m_settingIni = NULL;
		// 初始化
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
CTvmSetting::~CTvmSetting()
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
int CTvmSetting::Initialize() 
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

	// check DEVICE.INI file exist
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
CString CTvmSetting::AddBackslash(CString sPath)
{
	if(sPath.IsEmpty()){
		return sPath;
	}

	return sPath.TrimRight(_T("\\")) + _T("\\");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取延长运营时间（单位:秒）

@param    无 

@retval      int 延长运营时间（单位:秒）

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDelayOperationTime()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"), _T("DelayOperationTime"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置上传的文件路径

@param       无 

@retval      CString

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetFileUploadFTPDir(CString scPath)
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
CString CTvmSetting::GetFileUploadFTPDir()
{
	CString s = m_settingIni->GetDataString(_T("FTP"),_T("FileUploadFTPDir"));
	s = s.TrimRight(_T("/")) + _T("/");
	s = s + theMAINTENANCE_INFO.GetCurrentDevice().ToString() + _T("/");
	return s;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器更新成功标志

@param    none

@retval      int 0:成功 1:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetRechargeRWUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("FLAG"),_T("RechargeUpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置读写器程序更新失败标志

@param      (i)int updateFlag

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetRechargeRWUpdateFlag(int updateFlag)
{
	CString str;
	str.Format(_T("%d"),updateFlag);
	m_settingIni->WriteData(_T("FLAG"),_T("RechargeUpdateError"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器更新成功标志

@param     none

@retval      int 0:成功 1:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetCardRWUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("FLAG"),_T("CardUpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置读写器程序更新失败标志

@param       (i)int updateFlag

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetCardRWUpdateFlag(int updateFlag)
{
	CString str;
	str.Format(_T("%d"),updateFlag);
	m_settingIni->WriteData(_T("FLAG"),_T("CardUpdateError"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器更新成功标志

@param    none

@retval      int 0:成功 1:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetTokenRWUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("FLAG"),_T("TokenUpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置读写器程序更新失败标志

@param       (i)int updateFlag

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenRWUpdateFlag(int updateFlag)
{
	CString str;
	str.Format(_T("%d"),updateFlag);
	m_settingIni->WriteData(_T("FLAG"),_T("CardUpdateError"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取程序更新标志

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"), _T("UpdateFlag"), 0);
}

bool CTvmSetting::IsProgFirstRun()
{
	return m_settingIni->GetDataInt(_T("FLAG"), _T("ProgFirstRunFlag"), 0)==1;
}

void CTvmSetting::SetIsFirstRun(bool bFirst)
{
	CString str;
	str.Format(_T("%d"),bFirst?1:0);
	m_settingIni->WriteData(_T("FLAG"),_T("ProgFirstRunFlag"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取ACC参数更新失败标志

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetACCUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"), _T("ACCUpdateFlag"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取时间改变标志value

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTimeChangeValue()
{
	return m_settingIni->GetDataInt(_T("TIME"), _T("TimeChange"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置时间改变标志value

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTimeChangeValue(int timechange)
{
	CString str;
	str.Format(_T("%d"),timechange);
	m_settingIni->WriteData(_T("TIME"),_T("TimeChange"),str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取当前系统时间

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
CString CTvmSetting::GetCurSystemTime()
{
	return m_settingIni->GetDataString(_T("TIME"), _T("CurSystemTime"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置当前系统时间

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetCurSystemTime(CString time)
{
	m_settingIni->WriteData(_T("TIME"),_T("CurSystemTime"),time);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取屏幕亮度

@param       无 

@retval      int

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetScreenBrightnessValue()
{
	return m_settingIni->GetDataInt(_T("SCREEN_BRIGHTNESS"), _T("BrightnessValue"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       设置屏幕亮度

@param       (i)int brightnessvalue

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetScreenBrightnessValue(int brightnessvalue)
{
	CString str;
	str.Format(_T("%d"),brightnessvalue);
	m_settingIni->WriteData(_T("SCREEN_BRIGHTNESS"),_T("BrightnessValue"),str);
}




//////////////////////////////////////////////////////////////////////////
/**
@brief       设置业务模式

@param       (i)int serviceMode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetServiceMode(int serviceMode)
{
	CString  strService;
	strService.Format(_T("%d"), serviceMode);
	m_settingIni->WriteData(_T("RUN_MODE_SETTINGS"), _T("SERVICE_MODE"),strService);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取业务模式

@param       (i)int serviceMode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetServiceMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("SERVICE_MODE"), 0);
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       设置打印模式

@param       (i)int printMode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetPrintMode(int printMode)
{
	CString  strSet;
	strSet.Format(_T("%d"), printMode);
	m_settingIni->WriteData(_T("RUN_MODE_SETTINGS"), _T("PRINT_MODE"),strSet);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取打印模式

@param       

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetPrintMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("PRINT_MODE"), 0);
}





//////////////////////////////////////////////////////////////////////////
/**
@brief       设置支付模式

@param       (i)int printMode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetPayMentMode(int payment)
{
	CString  strSet;
	strSet.Format(_T("%d"), payment);
	m_settingIni->WriteData(_T("RUN_MODE_SETTINGS"), _T("PAYMENT_MODE"),strSet);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取支付模式

@param       

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetPayMentMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("PAYMENT_MODE"), 0);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       设置找零模式

@param       (i)int change

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetChangeMode(int change)
{
	CString  strSet;
	strSet.Format(_T("%d"), change);
	m_settingIni->WriteData(_T("RUN_MODE_SETTINGS"), _T("CHANGE_MODE"),strSet);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取找零模式

@param       

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetChangeMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("CHANGE_MODE"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		设置上位下发服务状态  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetSCServiceStatus(int nStatus){
	CString  strSet;
	strSet.Format(_T("%d"), nStatus);
	m_settingIni->WriteData(_T("RUN_MODE_SETTINGS"), _T("IS_OUT_OF_SERVICE"),strSet);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		获取上位下发服务状态  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTvmSetting::IsSCServiceOutOfService(){
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("IS_OUT_OF_SERVICE"), 0) == 1 ? true : false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      判断是否运营开始

@param      无

@retval     bool true:运营开始; false:自动运营开始

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CTvmSetting::IsStartOpeartion(){
	return m_settingIni->GetDataInt(_T("SERVICE_MODE_CONTROL"),_T("START_OPERATION_OPEN"),0) == 1 ? true : false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置是否运营开始标志

@param      (i)bool true运营开始 false正常运营

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetStartOperation(bool bSet){
	m_settingIni->WriteData(_T("SERVICE_MODE_CONTROL"),_T("START_OPERATION_OPEN"),bSet ? _T("1") : _T("0"));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		设置允许降级模式  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetDeradedMode(int nMode){
	CString strMode;
	strMode.Format(_T("%d"),nMode);
	m_settingIni->WriteData(_T("RUN_MODE_SETTINGS"), _T("IS_DERADED_ON_OFF"),strMode);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取TH模式

@param       none 

@retval      int 0:正常模式; 1:缩退模式

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHMode()
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
void CTvmSetting::SetTHMode(int sTHMode)
{
	CString str;
	str.Format(_T("%d"), sTHMode);
	m_settingIni ->WriteData(_T("TH"), _T("TH_MODE"), str);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		是否允许降级模式  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CTvmSetting::IsAllowDegrade(){
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"),_T("IS_DERADED_ON_OFF"),0) == 1 ?  true : false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       获取读写器更新成功标志

@param       (i)bool isRW  true:外部读写器  false:内部读写器

@retval      int 0:成功 1:失败

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetRWUpdateFlag(bool isRW)
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
void CTvmSetting::SetRWUpdateFlag(int updateFlag,bool isRW)
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
@brief       设置设备测试模式   

@param       (o)int mode

@retval      void

@exception   无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetDeviceTestMode(int mode)
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
int CTvmSetting::GetDeviceTestMode()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"),_T("DeviceTestMode"),1);
}

/*
@brief      设置最后一次连上位时间

@param      _DATE dtDay(年月日)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetLastOnlineDate(_DATE dtDay){
	m_settingIni->WriteData(_T("APPLICATION"),_T("LastOnlineTime"),dtDay.ToString(_T("%04d%02d%02d")));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取最后一次连接上位时间

@param      无

@retval     _DATE dtDay(年月日)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE CTvmSetting::GetLastOnlineDate(){
	_DATE dtDay = ComGetCurDate();
	CString strDate = m_settingIni->GetDataString(_T("APPLICATION"),_T("LastOnlineTime"));
	dtDay.TryParse(strDate);
	return dtDay;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取TVM最大离线运营天数

@param      无

@retval     int day（天）

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetMaxOffLineDay(){
	return m_settingIni->GetDataInt(_T("APPLICATION"),_T("MaxOffLineDay"),30);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置最大离线运营天数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetMaxOffLineDay(int nDays){
	CString strTemp;
	strTemp.Format(_T("%d"),nDays);
	 
	m_settingIni->WriteData(_T("APPLICATION"),_T("MaxOffLineDay"),strTemp);
}

int CTvmSetting::GetSettleSettings()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("SETTLE_SETTINGS"),0);
}

void CTvmSetting::SetSettleSettings(int nSettings)
{
	CString strSettings;
	strSettings.Format(_T("%d"),nSettings);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("SETTLE_SETTINGS"),strSettings);
}


int CTvmSetting::GetCardBoxLastAction()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("CARD_BOX_OPTIME"),0);
}

void CTvmSetting::SetCardBoxLastAction(int nAction)
{
	CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("CARD_BOX_OPTIME"),strAction);
}

int CTvmSetting::GetCoinBoxLastAction()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("COIN_BOX_OPTIME"),0);
}

void CTvmSetting::SetCoinBoxLastAction(int nAction)
{
	CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("COIN_BOX_OPTIME"),strAction);
}
int CTvmSetting::GetBanknoteBoxLastAction()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("BANKNOTE_BOX_OPTIME"),0);
}
void CTvmSetting::SetBanknoteBoxLastAction(int nAction)
{
	CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("BANKNOTE_BOX_OPTIME"),strAction);

}

CString CTvmSetting::GetBalanceOpTime()
{
	return m_settingIni->GetDataString(_T("SETTLEMENT"),_T("BALANCE_OPTIME"),0);
}

void CTvmSetting::SetBalanceOpTime(CString strTime)
{
	/*CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);*/
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("BALANCE_OPTIME"),strTime);
}

int CTvmSetting::GetCollectTokenResult()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("TOKEN_COLLECTION"),0);
}
void CTvmSetting::SetCollectTokenResult(int nAction)
{
	CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("TOKEN_COLLECTION"),strAction);
}

int CTvmSetting::GetCollectCoinResult()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("COIN_COLLECTION"),0);
}
void CTvmSetting::SetCollectCoinResult(int nAction)
{
	CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("COIN_COLLECTION"),strAction);
}

int CTvmSetting::GetCollectBanknoteResult()
{
	return m_settingIni->GetDataInt(_T("SETTLEMENT"),_T("BANKNOTE_COLLECTION"),0);
}
void CTvmSetting::SetCollectBanknoteResult(int nAction)
{
	CString strAction = _T("0");
	strAction.Format(_T("%d"),nAction);
	m_settingIni->WriteData(_T("SETTLEMENT"),_T("BANKNOTE_COLLECTION"),strAction);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取读写器串口号

@param      无

@retval     int 串口号。

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetRWComPort(){
	return m_settingIni->GetDataInt(_T("TVM_CARD_RW"),_T("COM_PORT"),5);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取读写器波特率

@param      无

@retval     int 波特率号。

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetRWBaudRate(){
	return m_settingIni->GetDataInt(_T("TVM_CARD_RW"),_T("BAUD_RATE"),115200);
}
int CTvmSetting::GetChargeRWComPort(){
	return m_settingIni->GetDataInt(_T("TVM_CHARGE_RW"),_T("COM_PORT"),8);
}
int CTvmSetting::GetChargeRWBaudRate(){
	return m_settingIni->GetDataInt(_T("TVM_CHARGE_RW"),_T("BAUD_RATE"),115200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置读写器串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetRwComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("TVM_CARD_RW"),_T("COM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置读写器波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetRwBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("TVM_CARD_RW"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币模块接收串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBHComPort(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"),_T("COMM_PORT"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币接收模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBHBaudRate(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"),_T("BAUD_RATE"),4800);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币接收模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBHComPort(const CString& sComPort){
	 m_settingIni->WriteData(_T("BANKNOTE_ACCEPT_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币接收模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBHBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("BANKNOTE_ACCEPT_MODULE"),_T("BAUD_RATE"),sBaudRate);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票卡模块串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHComPort(){
		return m_settingIni->GetDataInt(_T("CARD_MODULE"),_T("COMM_PORT"),3);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票卡模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHBaudRate(){
	return m_settingIni->GetDataInt(_T("CARD_MODULE"),_T("BAUD_RATE"),38400);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票卡模块更新时波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHUpadateBaudRate(){
	return m_settingIni->GetDataInt(_T("CARD_MODULE"),_T("BAUD_RATE_UPDATE"),57600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置票卡模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTHComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("CARD_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置票卡模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTHBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("CARD_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票卡模块串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenComPort(){
	return m_settingIni->GetDataInt(_T("TOKEN_MODULE"),_T("COMM_PORT"),3);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票卡模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenBaudRate(){
	return m_settingIni->GetDataInt(_T("TOKEN_MODULE"),_T("BAUD_RATE"),38400);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取票卡模块更新时波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenUpadateBaudRate(){
	return m_settingIni->GetDataInt(_T("TOKEN_MODULE"),_T("BAUD_RATE_UPDATE"),57600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置票卡模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("TOKEN_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置票卡模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("TOKEN_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取token回收箱模块串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenCyclerComPort(){
	return m_settingIni->GetDataInt(_T("TOKEN_CYCLER_MODULE"),_T("COMM_PORT"),3);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取token回收箱模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenCyclerBaudRate(){
	return m_settingIni->GetDataInt(_T("TOKEN_CYCLER_MODULE"),_T("BAUD_RATE"),38400);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置token回收箱模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenCyclerComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("TOKEN_CYCLER_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置token回收箱模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenCyclerBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("TOKEN_CYCLER_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取硬币模块接收串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetCHComPort(){
	//TCHAR szAppPath[_MAX_PATH];

	//if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
	//	return 1;
	//}

	//CString fileName = CString(szAppPath) + _T("GRGDTATM_CommCfg.ini");

	//// check DEVICE.INI file exist
	//if (FALSE == ComCheckFileExist(fileName)) {
	//	return 1;
	//}

	//// 文件属性改为可写
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//int ret = m_setChIni->GetDataInt(_T("COINSYSTEM"),_T("ComID"),6);
	//delete m_setChIni;

	//return ret;
	return m_settingIni->GetDataInt(_T("COIN_MODULE"),_T("COMM_PORT"),2);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置硬币模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::SetCHComPort(const CString& sComPort){
	//TCHAR szAppPath[_MAX_PATH];

	//if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
	//	return 1;
	//}

	//CString fileName = CString(szAppPath) + _T("GRGDTATM_CommCfg.ini");

	//// check DEVICE.INI file exist
	//if (FALSE == ComCheckFileExist(fileName)) {
	//	return 1;
	//}

	//// 文件属性改为可写
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//int ret = m_setChIni->WriteData(_T("COINSYSTEM"),_T("ComID"),sComPort);
	//delete m_setChIni;

	//return ret!=TRUE;
	return m_settingIni->WriteData(_T("COIN_MODULE"),_T("COMM_PORT"),sComPort);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取硬币模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetCHBaudRate(){
	//TCHAR szAppPath[_MAX_PATH];

	//if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
	//	return 1;
	//}

	//CString fileName = CString(szAppPath) + _T("GRGDTATM_CommCfg.ini");

	//// check DEVICE.INI file exist
	//if (FALSE == ComCheckFileExist(fileName)) {
	//	return 1;
	//}

	//// 文件属性改为可写
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//int ret = m_setChIni->GetDataInt(_T("COINSYSTEM"),_T("ComBaud"),57600);
	//delete m_setChIni;

	//return ret;
	return m_settingIni->GetDataInt(_T("COIN_MODULE"),_T("BAUD_RATE"),38400);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置硬币模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::SetCHBaudRate(const CString& sBaudRate){
	//TCHAR szAppPath[_MAX_PATH];

	//if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
	//	return 1;
	//}

	//CString fileName = CString(szAppPath) + _T("GRGDTATM_CommCfg.ini");

	//// check DEVICE.INI file exist
	//if (FALSE == ComCheckFileExist(fileName)) {
	//	return 1;
	//}

	//// 文件属性改为可写
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//BOOL ret = m_setChIni->WriteData(_T("COINSYSTEM"),_T("ComBaud"),sBaudRate);
	//delete m_setChIni;

	//return ret!=TRUE;
	return m_settingIni->WriteData(_T("COIN_MODULE"),_T("BAUD_RATE"),sBaudRate);

}
int CTvmSetting::GetCSHComPort()
{
	return m_settingIni->GetDataInt(_T("COIN_SHUTTER_MODULE"),_T("COMM_PORT"),7);
}
int CTvmSetting::GetCSHBaudRate()
{
	return m_settingIni->GetDataInt(_T("COIN_SHUTTER_MODULE"),_T("BAUD_RATE"),9600);
}
int CTvmSetting::SetCSHComPort(const CString& port)
{
	return m_settingIni->WriteData(_T("COIN_SHUTTER_MODULE"),_T("COMM_PORT"),port);

}
int CTvmSetting::SetCSHBaudRate(const CString& sBaudRate)
{
	return m_settingIni->WriteData(_T("COIN_SHUTTER_MODULE"),_T("BAUD_RATE"),sBaudRate);

}


//////////////////////////////////////////////////////////////////////////
/*
@brief      获取打印机模块接收串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetPrinterComPort(){
		return m_settingIni->GetDataInt(_T("RP_COMMUNICATE"),_T("COMM_PORT"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取打印机模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetPrinterBaudRate(){
	return m_settingIni->GetDataInt(_T("RP_COMMUNICATE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置打印机模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetPrinterComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("RP_COMMUNICATE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置打印机模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetPrinterBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("RP_COMMUNICATE"),_T("BAUD_RATE"),sBaudRate);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      获取IO串口号

@param      无

@retval     int 串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetIOComPort(){
	return m_settingIni->GetDataInt(_T("IO_COMMUNICATE"),_T("COMM_PORT"),8);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取IO波特率

@param      无

@retval     int 波特率

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetIOBaudRate(){
	return m_settingIni->GetDataInt(_T("IO_COMMUNICATE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置IO串口号

@param      const CString& sComPort

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetIOComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("IO_COMMUNICATE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置IO波特率

@param      const CString& sBaudRate

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetIOBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("IO_COMMUNICATE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取状态显示器串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDisplayComPort(){
		return m_settingIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("COMM_PORT"),4);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取状态显示器波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDisplayBaudRate(){
	return m_settingIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("BAUD_RATE"),19200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      状态显示器

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetDisplayComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("STATUS_DISPLAY_COMMUNICATE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置状态显示器波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetDisplayBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("STATUS_DISPLAY_COMMUNICATE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零模块串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBanknoteChangeComPort(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("COMM_PORT"),4);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBanknoteChangeBaudRate(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零模块串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknoteChangeComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零模块波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknoteChangeBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零模块RFID串口号

@param      无

@retval     int nComPort;串口号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBankchangeRFIDComPort(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("COMM_PORT"),11);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币找零模块波特率

@param      无

@retval     ini nBaudRate;

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBankchangeRFIDBaudRate(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零模块RFID串口号

@param      const CString& sComPort;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknotechangeRFIDComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置纸币找零模块RFID波特率

@param      const CString& sBaudRate;

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknotechangeRFIDBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币循环鼓A识别信息

@param      无

@retval     Cstring name_of_recycler

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTvmSetting::GetBNRRecycleBoxAName(){
	return m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_CASH_UNIT_INFO"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币循环鼓A的ID

@param      无

@retval     UINT id_of_recycler

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CTvmSetting::GetBNRRecycleBoxAId(){
	return m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_CASH_UNIT_ID"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币循环鼓A的存储币种类型

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CTvmSetting::GetBNRRecycleBoxAMoneyType(){
	return m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_CURRENCY_ID"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币循环鼓A的面额

@param      无

@retval     UINT (面额)

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CTvmSetting::GetBNRRecycleBoxAAmount(){
	return m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_VALUE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币循环鼓A的最大数量

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CTvmSetting::GetBNRRecycleBoxAMaxCount(){
	return m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_MAXINUM"));
}

// 纸币循环鼓B识别信息
CString CTvmSetting::GetBNRRecycleBoxBName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_CASH_UNIT_INFO"));}
// 纸币循环鼓B的ID
UINT    CTvmSetting::GetBNRRecycleBoxBId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_CASH_UNIT_ID"));}
// 纸币循环鼓B的存储币种类型
CString CTvmSetting::GetBNRRecycleBoxBMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_CURRENCY_ID"));}
// 纸币循环鼓B的面额
UINT	CTvmSetting::GetBNRRecycleBoxBAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_VALUE"));}
// 纸币循环鼓B的最大数量
UINT	CTvmSetting::GetBNRRecycleBoxBMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_MAXINUM"));}


// 纸币循环鼓C识别信息
CString CTvmSetting::GetBNRRecycleBoxCName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_CASH_UNIT_INFO"));}
// 纸币循环鼓C的ID
UINT    CTvmSetting::GetBNRRecycleBoxCId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_CASH_UNIT_ID"));}
// 纸币循环鼓C的存储币种类型
CString CTvmSetting::GetBNRRecycleBoxCMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_CURRENCY_ID"));}
// 纸币循环鼓C的面额
UINT	CTvmSetting::GetBNRRecycleBoxCAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_VALUE"));}
// 纸币循环鼓C的最大数量
UINT	CTvmSetting::GetBNRRecycleBoxCMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_MAXINUM"));}


// 纸币循环鼓D识别信息
CString CTvmSetting::GetBNRRecycleBoxDName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_CASH_UNIT_INFO"));}
// 纸币循环鼓D的ID
UINT    CTvmSetting::GetBNRRecycleBoxDId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_CASH_UNIT_ID"));}
// 纸币循环鼓D的存储币种类型
CString CTvmSetting::GetBNRRecycleBoxDMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_CURRENCY_ID"));}
// 纸币循环鼓D的面额
UINT	CTvmSetting::GetBNRRecycleBoxDAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_VALUE"));}
// 纸币循环鼓D的最大数量
UINT	CTvmSetting::GetBNRRecycleBoxDMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_MAXINUM"));}

// 纸币找零箱识别信息
CString CTvmSetting::GetBNRChangeBoxName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_CASH_UNIT_INFO"));}
// 纸币找零箱的ID
UINT    CTvmSetting::GetBNRChangeBoxId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_CASH_UNIT_ID"));}
// 纸币找零箱的存储币种类型
CString CTvmSetting::GetBNRChangeBoxMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_CURRENCY_ID"));}
// 纸币找零箱的面额
UINT	CTvmSetting::GetBNRChangeBoxAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_VALUE"));}
// 纸币找零箱的最大数量
UINT	CTvmSetting::GetBNRChangeBoxMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_MAXINUM"));}
// 纸币回收箱的ID
UINT	CTvmSetting::GetBNRCollectBoxId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("COLLECT_BOX_CAHS_UNIT_ID"));}

UINT	CTvmSetting::GetCoinChangeBox1Id(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_CHANGE_BOX1_UNIT_ID"));};	// 硬币找零箱A id
UINT	CTvmSetting::GetCoinChangeBox2Id(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_CHANGE_BOX2_UNIT_ID"));};	// 硬币找零箱B id
UINT	CTvmSetting::GetCoinCysleBoxId(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_CYCLE_UNIT_ID"));};	// 硬币循环箱id
UINT	CTvmSetting::GetCoinCollectBoxId(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_COLLECT_UNIT_ID"));};	// 硬币回收箱ID




void CTvmSetting::SetWebIdleProcessType(CString sType)
{
	m_settingIni->WriteData(_T("WEB_MODULE_LOCAL_SAVE"),_T("WEB_IDLE_ORDER_TYPE"),sType);
}

int CTvmSetting::GetWebIdleProcessType()
{
	return m_settingIni->GetDataInt(_T("WEB_MODULE_LOCAL_SAVE"),_T("WEB_IDLE_ORDER_TYPE"));
}


void CTvmSetting::SetWebIdleProcessURL(CString sURL)
{
	m_settingIni->WriteData(_T("WEB_MODULE_LOCAL_SAVE"),_T("WEB_IDLE_ORDER_URL"),sURL);
}

CString CTvmSetting::GetWebIdleProcessURL()
{
	return m_settingIni->GetDataString(_T("WEB_MODULE_LOCAL_SAVE"),_T("WEB_IDLE_ORDER_URL"));
}



CString CTvmSetting::GetRechargeServeYKT_IP()
{
	TCHAR szAppPath[_MAX_PATH];
	if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
		return _T("");
	}

	CString fileName = CString(szAppPath) + _T("DCR4000Comm.ini");
	// check DEVICE.INI file exist
	if (FALSE == ComCheckFileExist(fileName)) {
		return _T("");
	}

	CGetIni* m_setChIni = new CGetIni(fileName);
	CString ret = m_setChIni->GetDataString(_T("ServerInfo"),_T("ykthost"),_T("10.99.0.102"));
	delete m_setChIni;

	return ret;
}

int CTvmSetting::GetRechargeServeYKT_Port()
{
	TCHAR szAppPath[_MAX_PATH];
	if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
		return 0;
	}

	CString fileName = CString(szAppPath) + _T("DCR4000Comm.ini");
	// check DEVICE.INI file exist
	if (FALSE == ComCheckFileExist(fileName)) {
		return 0;
	}

	CGetIni* m_setChIni = new CGetIni(fileName);
	int ret = m_setChIni->GetDataInt(_T("ServerInfo"),_T("yktport"),6666);
	delete m_setChIni;

	return ret;
}

CString CTvmSetting::GetRechargeServeYPT_IP()
{
	TCHAR szAppPath[_MAX_PATH];
	if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
		return _T("");
	}

	CString fileName = CString(szAppPath) + _T("DCR4000Comm.ini");
	// check DEVICE.INI file exist
	if (FALSE == ComCheckFileExist(fileName)) {
		return _T("");
	}

	CGetIni* m_setChIni = new CGetIni(fileName);
	CString ret = m_setChIni->GetDataString(_T("ServerInfo"),_T("acchost"),_T("10.99.0.6"));
	delete m_setChIni;

	return ret;
}

int CTvmSetting::GetRechargeServeYPT_Port()
{
	TCHAR szAppPath[_MAX_PATH];
	if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
		return 0;
	}

	CString fileName = CString(szAppPath) + _T("DCR4000Comm.ini");
	// check DEVICE.INI file exist
	if (FALSE == ComCheckFileExist(fileName)) {
		return 0;
	}

	CGetIni* m_setChIni = new CGetIni(fileName);
	int ret = m_setChIni->GetDataInt(_T("ServerInfo"),_T("accport"),50031);
	delete m_setChIni;

	return ret;
}