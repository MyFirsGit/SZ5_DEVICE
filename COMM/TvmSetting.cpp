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
@brief      ��ȡ��ʵ������

@param      (i)void

@retval      CSetting& CSetting�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CTvmSetting& CTvmSetting::GetInstance()
{
	return theInstance;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTvmSetting::CTvmSetting()
{
	try {
		m_settingIni = NULL;
		// ��ʼ��
		Initialize();
	}
	catch (...) {
		// �����쳣��Ϣ
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTvmSetting::~CTvmSetting()
{
	delete m_settingIni;
	m_settingIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ʼ��

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::Initialize() 
{
	// ���ѳ�ʼ������ֱ�ӷ���
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

	// �ļ����Ը�Ϊ��д
	SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	m_settingIni = new CGetIni(fileName);

	Initialized.Invoke();

	return 0;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ŀ¼������ӷ�б��

@param      (i)CString sPath    Ŀ¼

@retval     CString     �������б�ܵ�Ŀ¼��

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
@brief       ��ȡ�ӳ���Ӫʱ�䣨��λ:�룩

@param    �� 

@retval      int �ӳ���Ӫʱ�䣨��λ:�룩

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDelayOperationTime()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"), _T("DelayOperationTime"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �����ϴ����ļ�·��

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetFileUploadFTPDir(CString scPath)
{
	m_settingIni->WriteData(_T("FTP"),_T("FileUploadFTPDir"),scPath);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡFTP�ϴ����ļ�·��

@param       �� 

@retval      CString

@exception   ��
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
@brief       ��ȡ��д�����³ɹ���־

@param    none

@retval      int 0:�ɹ� 1:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetRechargeRWUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("FLAG"),_T("RechargeUpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ö�д���������ʧ�ܱ�־

@param      (i)int updateFlag

@retval      void

@exception   ��
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
@brief       ��ȡ��д�����³ɹ���־

@param     none

@retval      int 0:�ɹ� 1:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetCardRWUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("FLAG"),_T("CardUpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ö�д���������ʧ�ܱ�־

@param       (i)int updateFlag

@retval      void

@exception   ��
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
@brief       ��ȡ��д�����³ɹ���־

@param    none

@retval      int 0:�ɹ� 1:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int	CTvmSetting::GetTokenRWUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("FLAG"),_T("TokenUpdateError"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ö�д���������ʧ�ܱ�־

@param       (i)int updateFlag

@retval      void

@exception   ��
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
@brief       ��ȡ������±�־

@param       �� 

@retval      int

@exception   ��
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
@brief       ��ȡACC��������ʧ�ܱ�־

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetACCUpdateFlag()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"), _T("ACCUpdateFlag"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡʱ��ı��־value

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTimeChangeValue()
{
	return m_settingIni->GetDataInt(_T("TIME"), _T("TimeChange"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����ʱ��ı��־value

@param       �� 

@retval      int

@exception   ��
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
@brief       ��ȡ��ǰϵͳʱ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTvmSetting::GetCurSystemTime()
{
	return m_settingIni->GetDataString(_T("TIME"), _T("CurSystemTime"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���õ�ǰϵͳʱ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetCurSystemTime(CString time)
{
	m_settingIni->WriteData(_T("TIME"),_T("CurSystemTime"),time);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��Ļ����

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetScreenBrightnessValue()
{
	return m_settingIni->GetDataInt(_T("SCREEN_BRIGHTNESS"), _T("BrightnessValue"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ������Ļ����

@param       (i)int brightnessvalue

@retval      void

@exception   ��
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
@brief       ����ҵ��ģʽ

@param       (i)int serviceMode

@retval      void

@exception   ��
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
@brief       ��ȡҵ��ģʽ

@param       (i)int serviceMode

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetServiceMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("SERVICE_MODE"), 0);
}



//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ô�ӡģʽ

@param       (i)int printMode

@retval      void

@exception   ��
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
@brief       ��ȡ��ӡģʽ

@param       

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetPrintMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("PRINT_MODE"), 0);
}





//////////////////////////////////////////////////////////////////////////
/**
@brief       ����֧��ģʽ

@param       (i)int printMode

@retval      void

@exception   ��
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
@brief       ��ȡ֧��ģʽ

@param       

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetPayMentMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("PAYMENT_MODE"), 0);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������ģʽ

@param       (i)int change

@retval      void

@exception   ��
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
@brief       ��ȡ����ģʽ

@param       

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CTvmSetting::GetChangeMode()
{
	return m_settingIni->GetDataInt(_T("RUN_MODE_SETTINGS"), _T("CHANGE_MODE"), 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		������λ�·�����״̬  

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
@brief		��ȡ��λ�·�����״̬  

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
@brief      �ж��Ƿ���Ӫ��ʼ

@param      ��

@retval     bool true:��Ӫ��ʼ; false:�Զ���Ӫ��ʼ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTvmSetting::IsStartOpeartion(){
	return m_settingIni->GetDataInt(_T("SERVICE_MODE_CONTROL"),_T("START_OPERATION_OPEN"),0) == 1 ? true : false;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �����Ƿ���Ӫ��ʼ��־

@param      (i)bool true��Ӫ��ʼ false������Ӫ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetStartOperation(bool bSet){
	m_settingIni->WriteData(_T("SERVICE_MODE_CONTROL"),_T("START_OPERATION_OPEN"),bSet ? _T("1") : _T("0"));
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		����������ģʽ  

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
@brief       ��ȡTHģʽ

@param       none 

@retval      int 0:����ģʽ; 1:����ģʽ

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHMode()
{
	return m_settingIni->GetDataInt(_T("TH"), _T("TH_MODE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ����THģʽ

@param       (i)int  sTHMode 0:����ģʽ; 1:����ģʽ 

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
@brief		�Ƿ�������ģʽ  

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
@brief       ��ȡ��д�����³ɹ���־

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int 0:�ɹ� 1:ʧ��

@exception   ��
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
@brief       ���ö�д���������ʧ�ܱ�־

@param       (i)int updateFlag

@retval      void

@exception   ��
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
@brief       �����豸����ģʽ   

@param       (o)int mode

@retval      void

@exception   ��
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
@brief       ��ȡ�豸����ģʽ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDeviceTestMode()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"),_T("DeviceTestMode"),1);
}

/*
@brief      �������һ������λʱ��

@param      _DATE dtDay(������)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetLastOnlineDate(_DATE dtDay){
	m_settingIni->WriteData(_T("APPLICATION"),_T("LastOnlineTime"),dtDay.ToString(_T("%04d%02d%02d")));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ���һ��������λʱ��

@param      ��

@retval     _DATE dtDay(������)

@exception  ��
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
@brief      ȡTVM���������Ӫ����

@param      ��

@retval     int day���죩

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetMaxOffLineDay(){
	return m_settingIni->GetDataInt(_T("APPLICATION"),_T("MaxOffLineDay"),30);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������������Ӫ����

@param      ��

@retval     ��

@exception  ��
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
@brief      ��ȡ��д�����ں�

@param      ��

@retval     int ���ںš�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetRWComPort(){
	return m_settingIni->GetDataInt(_T("TVM_CARD_RW"),_T("COM_PORT"),5);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��д��������

@param      ��

@retval     int �����ʺš�

@exception  ��
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
@brief      ���ö�д�����ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetRwComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("TVM_CARD_RW"),_T("COM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ö�д��������

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetRwBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("TVM_CARD_RW"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ��ģ����մ��ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBHComPort(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"),_T("COMM_PORT"),1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ�ҽ���ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBHBaudRate(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_ACCEPT_MODULE"),_T("BAUD_RATE"),4800);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ�ҽ���ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBHComPort(const CString& sComPort){
	 m_settingIni->WriteData(_T("BANKNOTE_ACCEPT_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ�ҽ���ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBHBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("BANKNOTE_ACCEPT_MODULE"),_T("BAUD_RATE"),sBaudRate);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��ģ�鴮�ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHComPort(){
		return m_settingIni->GetDataInt(_T("CARD_MODULE"),_T("COMM_PORT"),3);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHBaudRate(){
	return m_settingIni->GetDataInt(_T("CARD_MODULE"),_T("BAUD_RATE"),38400);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��ģ�����ʱ������

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTHUpadateBaudRate(){
	return m_settingIni->GetDataInt(_T("CARD_MODULE"),_T("BAUD_RATE_UPDATE"),57600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ʊ��ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTHComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("CARD_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ʊ��ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTHBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("CARD_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��ģ�鴮�ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenComPort(){
	return m_settingIni->GetDataInt(_T("TOKEN_MODULE"),_T("COMM_PORT"),3);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenBaudRate(){
	return m_settingIni->GetDataInt(_T("TOKEN_MODULE"),_T("BAUD_RATE"),38400);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡƱ��ģ�����ʱ������

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenUpadateBaudRate(){
	return m_settingIni->GetDataInt(_T("TOKEN_MODULE"),_T("BAUD_RATE_UPDATE"),57600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ʊ��ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("TOKEN_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ʊ��ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("TOKEN_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡtoken������ģ�鴮�ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenCyclerComPort(){
	return m_settingIni->GetDataInt(_T("TOKEN_CYCLER_MODULE"),_T("COMM_PORT"),3);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡtoken������ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetTokenCyclerBaudRate(){
	return m_settingIni->GetDataInt(_T("TOKEN_CYCLER_MODULE"),_T("BAUD_RATE"),38400);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����token������ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenCyclerComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("TOKEN_CYCLER_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����token������ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetTokenCyclerBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("TOKEN_CYCLER_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡӲ��ģ����մ��ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
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

	//// �ļ����Ը�Ϊ��д
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//int ret = m_setChIni->GetDataInt(_T("COINSYSTEM"),_T("ComID"),6);
	//delete m_setChIni;

	//return ret;
	return m_settingIni->GetDataInt(_T("COIN_MODULE"),_T("COMM_PORT"),2);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ��ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
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

	//// �ļ����Ը�Ϊ��д
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//int ret = m_setChIni->WriteData(_T("COINSYSTEM"),_T("ComID"),sComPort);
	//delete m_setChIni;

	//return ret!=TRUE;
	return m_settingIni->WriteData(_T("COIN_MODULE"),_T("COMM_PORT"),sComPort);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡӲ��ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
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

	//// �ļ����Ը�Ϊ��д
	//SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

	//CGetIni* m_setChIni = new CGetIni(fileName);
	//int ret = m_setChIni->GetDataInt(_T("COINSYSTEM"),_T("ComBaud"),57600);
	//delete m_setChIni;

	//return ret;
	return m_settingIni->GetDataInt(_T("COIN_MODULE"),_T("BAUD_RATE"),38400);

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ��ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
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

	//// �ļ����Ը�Ϊ��д
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
@brief      ��ȡ��ӡ��ģ����մ��ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetPrinterComPort(){
		return m_settingIni->GetDataInt(_T("RP_COMMUNICATE"),_T("COMM_PORT"),10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��ӡ��ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetPrinterBaudRate(){
	return m_settingIni->GetDataInt(_T("RP_COMMUNICATE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ô�ӡ��ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetPrinterComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("RP_COMMUNICATE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ô�ӡ��ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetPrinterBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("RP_COMMUNICATE"),_T("BAUD_RATE"),sBaudRate);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡIO���ں�

@param      ��

@retval     int ���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetIOComPort(){
	return m_settingIni->GetDataInt(_T("IO_COMMUNICATE"),_T("COMM_PORT"),8);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡIO������

@param      ��

@retval     int ������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetIOBaudRate(){
	return m_settingIni->GetDataInt(_T("IO_COMMUNICATE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����IO���ں�

@param      const CString& sComPort

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetIOComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("IO_COMMUNICATE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����IO������

@param      const CString& sBaudRate

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetIOBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("IO_COMMUNICATE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ״̬��ʾ�����ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDisplayComPort(){
		return m_settingIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("COMM_PORT"),4);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ״̬��ʾ��������

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetDisplayBaudRate(){
	return m_settingIni->GetDataInt(_T("STATUS_DISPLAY_COMMUNICATE"),_T("BAUD_RATE"),19200);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ״̬��ʾ��

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetDisplayComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("STATUS_DISPLAY_COMMUNICATE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����״̬��ʾ��������

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetDisplayBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("STATUS_DISPLAY_COMMUNICATE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ������ģ�鴮�ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBanknoteChangeComPort(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("COMM_PORT"),4);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ������ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBanknoteChangeBaudRate(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_MODULE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ������ģ�鴮�ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknoteChangeComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ������ģ�鲨����

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknoteChangeBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ������ģ��RFID���ں�

@param      ��

@retval     int nComPort;���ں�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBankchangeRFIDComPort(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("COMM_PORT"),11);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ������ģ�鲨����

@param      ��

@retval     ini nBaudRate;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CTvmSetting::GetBankchangeRFIDBaudRate(){
	return m_settingIni->GetDataInt(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("BAUD_RATE"),9600);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ������ģ��RFID���ں�

@param      const CString& sComPort;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknotechangeRFIDComPort(const CString& sComPort){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("COMM_PORT"),sComPort);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ������ģ��RFID������

@param      const CString& sBaudRate;

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CTvmSetting::SetBanknotechangeRFIDBaudRate(const CString& sBaudRate){
	m_settingIni->WriteData(_T("BANKNOTE_CHANGE_RFID_MODULE"),_T("BAUD_RATE"),sBaudRate);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ѭ����Aʶ����Ϣ

@param      ��

@retval     Cstring name_of_recycler

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTvmSetting::GetBNRRecycleBoxAName(){
	return m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_CASH_UNIT_INFO"));
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ѭ����A��ID

@param      ��

@retval     UINT id_of_recycler

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CTvmSetting::GetBNRRecycleBoxAId(){
	return m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_CASH_UNIT_ID"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ѭ����A�Ĵ洢��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTvmSetting::GetBNRRecycleBoxAMoneyType(){
	return m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_CURRENCY_ID"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ѭ����A�����

@param      ��

@retval     UINT (���)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CTvmSetting::GetBNRRecycleBoxAAmount(){
	return m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_VALUE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ѭ����A���������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CTvmSetting::GetBNRRecycleBoxAMaxCount(){
	return m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE1_MAXINUM"));
}

// ֽ��ѭ����Bʶ����Ϣ
CString CTvmSetting::GetBNRRecycleBoxBName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_CASH_UNIT_INFO"));}
// ֽ��ѭ����B��ID
UINT    CTvmSetting::GetBNRRecycleBoxBId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_CASH_UNIT_ID"));}
// ֽ��ѭ����B�Ĵ洢��������
CString CTvmSetting::GetBNRRecycleBoxBMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_CURRENCY_ID"));}
// ֽ��ѭ����B�����
UINT	CTvmSetting::GetBNRRecycleBoxBAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_VALUE"));}
// ֽ��ѭ����B���������
UINT	CTvmSetting::GetBNRRecycleBoxBMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE2_MAXINUM"));}


// ֽ��ѭ����Cʶ����Ϣ
CString CTvmSetting::GetBNRRecycleBoxCName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_CASH_UNIT_INFO"));}
// ֽ��ѭ����C��ID
UINT    CTvmSetting::GetBNRRecycleBoxCId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_CASH_UNIT_ID"));}
// ֽ��ѭ����C�Ĵ洢��������
CString CTvmSetting::GetBNRRecycleBoxCMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_CURRENCY_ID"));}
// ֽ��ѭ����C�����
UINT	CTvmSetting::GetBNRRecycleBoxCAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_VALUE"));}
// ֽ��ѭ����C���������
UINT	CTvmSetting::GetBNRRecycleBoxCMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE3_MAXINUM"));}


// ֽ��ѭ����Dʶ����Ϣ
CString CTvmSetting::GetBNRRecycleBoxDName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_CASH_UNIT_INFO"));}
// ֽ��ѭ����D��ID
UINT    CTvmSetting::GetBNRRecycleBoxDId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_CASH_UNIT_ID"));}
// ֽ��ѭ����D�Ĵ洢��������
CString CTvmSetting::GetBNRRecycleBoxDMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_CURRENCY_ID"));}
// ֽ��ѭ����D�����
UINT	CTvmSetting::GetBNRRecycleBoxDAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_VALUE"));}
// ֽ��ѭ����D���������
UINT	CTvmSetting::GetBNRRecycleBoxDMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CYCLE4_MAXINUM"));}

// ֽ��������ʶ����Ϣ
CString CTvmSetting::GetBNRChangeBoxName(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_CASH_UNIT_INFO"));}
// ֽ���������ID
UINT    CTvmSetting::GetBNRChangeBoxId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_CASH_UNIT_ID"));}
// ֽ��������Ĵ洢��������
CString CTvmSetting::GetBNRChangeBoxMoneyType(){ return  m_settingIni->GetDataString(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_CURRENCY_ID"));}
// ֽ������������
UINT	CTvmSetting::GetBNRChangeBoxAmount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_VALUE"));}
// ֽ����������������
UINT	CTvmSetting::GetBNRChangeBoxMaxCount(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("CHANGE_BOX_MAXINUM"));}
// ֽ�һ������ID
UINT	CTvmSetting::GetBNRCollectBoxId(){ return  m_settingIni->GetDataInt(_T("TVM_BNR_MODULE_SET"),_T("COLLECT_BOX_CAHS_UNIT_ID"));}

UINT	CTvmSetting::GetCoinChangeBox1Id(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_CHANGE_BOX1_UNIT_ID"));};	// Ӳ��������A id
UINT	CTvmSetting::GetCoinChangeBox2Id(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_CHANGE_BOX2_UNIT_ID"));};	// Ӳ��������B id
UINT	CTvmSetting::GetCoinCysleBoxId(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_CYCLE_UNIT_ID"));};	// Ӳ��ѭ����id
UINT	CTvmSetting::GetCoinCollectBoxId(){return  m_settingIni->GetDataInt(_T("TVM_COIN_MODULE_SET"),_T("COIN_COLLECT_UNIT_ID"));};	// Ӳ�һ�����ID




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