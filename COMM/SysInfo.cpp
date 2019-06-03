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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSYSInfo::CSYSInfo()
{
    try {
        m_DeviceIni = NULL;
		
        // ���ڻ�
        //Initialize();
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
CSYSInfo::~CSYSInfo()
{
    delete m_DeviceIni;
    m_DeviceIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CSYSInfo::Initialize() 
{
    try {
        // ���ѳ�ʼ������ֱ�ӷ���
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

        // �ļ����Ը�Ϊ��д
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
@brief      ��Ŀ¼������ӷ�б��

@param      (i)CString sPath    Ŀ¼

@retval     CString     �������б�ܵ�Ŀ¼��

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
@brief       ��ȡ���ڰ�װ�İ汾��Ϣ

@param       �� 

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetInitialVersion()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �Ƿ���ʾ����ǰ��

@param       ��

@retval      bool true:����ǰ�� ; false:������ǰ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::IsTopmost()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("TopMost"), 1) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �Ƿ����ù��ܼ�

@param       ��

@retval      bool true:���� ; false:������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::IsFunctionKeyEnabled()
{
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("FunctionKeyEnable"), 1) != 0 ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �Ƿ��������������

@param       ��

@retval      bool true:���� ; false:������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::IsTaskManagerEnabled()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("TaskManagerEnabled"), 1) != 0 ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �Ƿ���ʾ����

@param       ��

@retval      bool true:��ʾ ; false:����ʾ

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool    CSYSInfo::IsDesktopEnabled()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("DesktopEnabled"), 1) != 0 ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �Ƿ���ʾ�����

@param       ��

@retval      bool true:��ʾ ; false:����ʾ

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CSYSInfo::CanShowCursor()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ������Ŀ¼���棨��λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetDataDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("DataDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ������Ŀ¼���ϣ���λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetDataDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("DataDirectoryStop"));

}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������Ŀ¼���棨��λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetBackupDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("BackupDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��������Ŀ¼���ϣ���λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetBackupDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("BackupDirectoryStop"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��־�ļ�Ŀ¼���棨��λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetLogDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("LogDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��־�ļ�Ŀ¼���ϣ���λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetLogDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("LogDirectoryStop"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ϵͳĿ¼���棨��λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetSysDirectoryWarn()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("SystemDirectoryWarn"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ϵͳĿ¼���ϣ���λ��M��

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CSYSInfo::GetSysDirectoryStop()
{
	return m_DeviceIni->GetDataInt(_T("DISKFREESPACE"), _T("SystemDirectoryStop"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ������Ŀ¼

@param       �� 

@retval      CString

@exception   ��
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
@brief       ��ȡ����Ŀ¼

@param       �� 

@retval      CString

@exception   ��
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
@brief       ��ȡCF�� ʹ�õ� �̷�

@param       �� 

@retval      CString

@exception   ��
*/
////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetCFDrive()
{
    return m_DeviceIni->GetDataString(_T("APP_PATH"), _T("CFDRIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUSB ʹ�õ� �̷�

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetUSBDrive()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("APP_PATH"), _T("USBDRIVE")));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ����Ŀ¼

@param       �� 

@retval      CString

@exception   ��
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
@brief       ��ȡUSB���ݵ�����Ŀ¼

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetExportPath()
{
	return AddBackslash(m_DeviceIni->GetDataString(_T("APP_PATH"), _T("ExportPath")));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡFTP��ʱ����·��

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetFileDownloadLocalDir()
{
	return m_DeviceIni->GetDataString(_T("APP_PATH"), _T("FileDownloadLocalDir"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ȡ�ý��״����� ����λ���룩  

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
@brief      ȡ��ҵ�����ݴ�����ʱ�䣨��λ���룩

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
@brief      ȡ��ƾ֤���ݵ�·��

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
@brief       �Ƿ��¼Debug��Ϣ

@param       ��

@retval      bool true:��¼Debug��Ϣ ; false:����¼Debug��Ϣ

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
 bool CSYSInfo::IsWriteDebug()
 {
    return m_DeviceIni->GetDataInt(_T("LOG"), _T("WriteDebug")) == 0 ? false : true;
 }

 ////////////////////////////////////////////////////////////////////////////
 /**
 @brief      ȡ����־���������

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
 @brief      ������־���������

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
@brief       ��ȡϵͳ����ʹ�õ���������

@param       �� 

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CSYSInfo::GetUsedAdapterName()
{
	return m_DeviceIni->GetDataString(_T("MACHINE"), _T("USED_ADAPTER_NAME"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ȡ�ö�д���������ļ��е�section����  

@param (i)RW_TYPE rwType ��д������

@retval     CString ��д���������ļ��е�section����

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
@brief    ȡ�õ��ӱ�ǩ�������ļ��е�����  

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
 @brief       �Ƿ����ػ�����

 @param       ��

 @retval      bool true:�����ػ����� ; false:�������ػ�����

 @exception   ��
 */
 //////////////////////////////////////////////////////////////////////////
 bool CSYSInfo::IsUsedGuardian()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("IsUsedGuardian")) == 0 ? false : true;
 }

 //////////////////////////////////////////////////////////////////////////
 /**
 @brief       ��ȡ״̬��ˢ��ʱ����

 @param        

 @retval      int

 @exception   ��
 */
 //////////////////////////////////////////////////////////////////////////
 int CSYSInfo::GetStatusTimerInterval()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("StatusTimerInterval"), 1000);
 }
 
 //////////////////////////////////////////////////////////////////////////
 /**
 @brief			������ʱ���˿������Ƿ��Զ�����Ĭ��ҵ��

 @param        

 @retval      bool-> true:�Զ�����  false�����Զ�����

 @exception   ��
 */
 //////////////////////////////////////////////////////////////////////////
 bool CSYSInfo::IsAutoReturnDefaultService()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("IsAutoReturnDefaultService")) == 0 ? false : true;
 }

  //////////////////////////////////////////////////////////////////////////
 /**
 @brief			�Ƿ��ð��I��

 @param        

 @retval      bool-> true:�Զ�����  false�����Զ�����

 @exception   ��
 */
 //////////////////////////////////////////////////////////////////////////
 bool    CSYSInfo::IsEnableClickSound()
 {
	 return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("EnableClickSound")) == 0 ? false : true;
 }