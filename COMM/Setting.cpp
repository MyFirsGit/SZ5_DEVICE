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
@brief      ��ȡ��ʵ������

@param      (i)void

@retval      CSetting& CSetting�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CSetting& CSetting::GetInstance()
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
CSetting::CSetting()
{
	try {
		m_settingIni = NULL;
		m_LanguageMode = MODE_UNKNOWN;
		// ��ʼ��
		Initialize();
		// ��ȡ����ģʽ
		m_LanguageMode = LANGUAGE_ID_CHINESE/*(LANGUAGETYPE_T)m_settingIni->GetDataInt(_T("APPLICATION"),_T("LanguageMode"),0)*/;
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
CSetting::~CSetting()
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
int CSetting::Initialize() 
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

	// check TCM.INI file exist
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
CString CSetting::AddBackslash(CString sPath)
{
	if(sPath.IsEmpty()){
		return sPath;
	}

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
void CSetting::SetDeviceTestMode(int mode)
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
int CSetting::GetDeviceTestMode()
{
	return m_settingIni->GetDataInt(_T("APPLICATION"),_T("DeviceTestMode"),1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������ģʽ   

@param       (o)int mode

@retval      void

@exception   ��
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
@brief       ��ȡ����ģʽ

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
LANGUAGETYPE_T CSetting::GetLanguageMode()
{
	return m_LanguageMode;
}

////////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��ӡԱ��ID

@param       �� 

@retval      CString

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
CString CSetting::GetPrintStaffID()
{
	return m_settingIni->GetDataString(_T("APPLICATION"),_T("PrintStaffID"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ��ӡ��¼ʱ��

@param       �� 

@retval      _DATE_TIME

@exception   ��
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
@brief       �����ϴ����ļ�·��

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetFileUploadFTPDir(CString scPath)
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
CString CSetting::GetFileUploadFTPDir()
{
	CString s = m_settingIni->GetDataString(_T("FTP"),_T("FileUploadFTPDir"));
	s = s.TrimRight(_T("/")) + _T("/");
	s = s + theMAINTENANCE_INFO.GetCurrentDevice().ToString() + _T("/");
	return s;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTHģʽ

@param       none 

@retval      int 0:����ģʽ; 1:����ģʽ

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CSetting::GetTHMode()
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
void CSetting::SetTHMode(int sTHMode)
{
	CString str;
	str.Format(_T("%d"), sTHMode);
	m_settingIni ->WriteData(_T("TH"), _T("TH_MODE"), str);
	THModeChanged.Invoke(sTHMode);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡRFID��д״̬

@param       none 

@retval      int 0:��ֹ��д 1:�����д

@exception   none
*/
////////////////////////////////////////////////////////////////////////////
int  CSetting::GetRFIDWriteStatus()
{
	return m_settingIni->GetDataInt(_T("RFID"), _T("RFIDWriteableStatus"), 0);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief       ����RFID��д״̬

@param       (i)bool isUsed  true:����  false:����

@retval      int 0:��ֹ��д 1:�����д

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
@brief       ��ȡ��д�����³ɹ���־

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int 0:�ɹ� 1:ʧ��

@exception   ��
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
@brief       ���ö�д���������ʧ�ܱ�־

@param       (i)int updateFlag

@retval      void

@exception   ��
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
@brief       ���ô�ӡԱ��ID

@param       (i)CString printStaffID

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetPrintStaffID(CString printStaffID)
{
	m_settingIni->WriteData(_T("APPLICATION"), _T("PrintStaffID"), printStaffID);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ô�ӡ��¼ʱ��

@param       (i)_DATE_TIME printLoginTime

@retval      void

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
void CSetting::SetPrintLoginTime(_DATE_TIME printLoginTime)
{
	CString str;
	str.Format(_T("%.4d%.2d%.2d%.2d%.2d%.2d"),printLoginTime.wYear,printLoginTime.biMonth,printLoginTime.biDay,
		printLoginTime.biHour,printLoginTime.biMinute,printLoginTime.biSecond);
	m_settingIni->WriteData(_T("APPLICATION"), _T("PrintLoginTime"), str);
}