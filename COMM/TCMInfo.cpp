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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTCMInfo::CTCMInfo()
{
    try {
        m_TCMIni = NULL;

		//m_accAuthorizeManner = 0;			// ACC��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		//m_ectAuthorizeManner = 0;			// ECT��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		
		//m_rfidWriteStatus = 0;				// RFID��д״̬��=0����д�룻=1��ֹд�룩

        // ���ڻ�
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
CTCMInfo::~CTCMInfo()
{
    delete m_TCMIni;
    m_TCMIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::Initialize() 
{
    try {
        // ���ѳ�ʼ������ֱ�ӷ���
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

        // �ļ����Ը�Ϊ��д
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_TCMIni = new CGetIni(fileName);
		
		Initialized.Invoke();

		//[APPLICATION]
		//m_accAuthorizeManner = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ACCAuthorizeManner"),2);			// ACC��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		//m_ectAuthorizeManner = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ECTAuthorizeManner"),2);			// ECT��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		//m_deviceTestMode = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("DEVICE_TEST_MODE"),0);				// �豸����ģʽ��0������ģʽ��1������ģʽ��
		//m_isRWUsed = (m_TCMIni->GetDataInt(_T("OUT_ICRW"), _T("IS_USED")) == 1);
		//m_rfidWriteStatus = m_TCMIni->GetDataInt(_T("APPLICATION"), _T("RFID_WRITEABLE_STATUS"),1);			// RFID��д״̬���ã�0������д�룻1����ֹд�룩
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
//@brief      ��Ŀ¼������ӷ�б��
//
//@param      (i)CString sPath    Ŀ¼
//
//@retval     CString     �������б�ܵ�Ŀ¼��
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
@brief       ��ȡ���ڰ�װ�İ汾��Ϣ

@param       �� 

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetInitialVersion()
{
    return m_TCMIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       �Ƿ���ʾ����ǰ��
//
//@param       ��
//
//@retval      bool true:����ǰ�� ; false:������ǰ��
//
//@exception   ��
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
//@brief       �Ƿ���ʾ�����
//
//@param       ��
//
//@retval      bool true:��ʾ ; false:����ʾ
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CTCMInfo::CanShowCursor()
{
    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ������Ŀ¼
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetDataPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("APPLICATION"), _T("DataPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ����Ŀ¼
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetBackupPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("APPLICATION"), _T("BackupPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ����Ŀ¼
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
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
//@brief       ��ȡ��Ӫ��������
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetCompanyNameCn()
{
	return  m_strCompanyNameCN;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ��Ӫ��Ӣ����
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetCompanyNameEn()
{
	return m_strCompanyNameEN;
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ�������δ����������
//
//@param       �� 
//
//@retval      UINT
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
UINT CTCMInfo::GetLimitedDaysOfUnSendData()
{
	return m_TCMIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ�������ݱ�������
//
//@param       �� 
//
//@retval      UINT
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
UINT CTCMInfo::GetLimitedDaysOfSendData()
{
	return m_TCMIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTCM ���Ʋ����� �ϴ�·��

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMExportPath()
{
	return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("ExportPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ���ػ��ļ����ɻָ�����
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetMaxRecoverTimes()
//{
//    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("MaxRecoverTimes"), 10);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��¼�����еĵ�¼ʱ���Ƿ�̶�
//
//@param       ��
//
//@retval      bool true:�̶� ; false:���̶�
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTCMInfo::IsLoginTimeFixed()
//{
//    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("LoginTimeFixed")) == 0 ? false : true;
//}
//
//////////////////////////////////////////////////////////////////////////
/**
@brief       �Ƿ��¼Debug��Ϣ

@param       ��

@retval      bool true:��¼Debug��Ϣ ; false:����¼Debug��Ϣ

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
 bool CTCMInfo::IsWriteDebug()
 {
    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("WriteDebug")) == 0 ? false : true;
 }

////////////////////////////////////////////////////////////////////////////
///**
//@brief       �Ƿ����÷���Ա��
//
//@param       ��
//
//@retval      bool true:���� ; false:������
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUseFounderStaffID()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡŷķ��Ա��ID

@param       �� 

@retval      int

@exception   ��
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
@brief       ��ȡŷķ��Ա������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetFounderStaffPassword()
{
	return m_TCMIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        �Ƿ������Զ��峵Ʊ����

@param       �� 

@retval      bool

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUseConfigTicketCategory()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UseConfigTicketCategory"),0) == 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ȡ�����Զ��峵Ʊ����

@param       ticketTypeCode 

@retval      int

@exception   ��
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
@brief        ȡ��ҵ��������ȴ�ʱ�䵥λ���룩

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CTCMInfo::GetCloseOperationMaxWaitSeconds()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("CloseOperationMaxWaitSeconds"),10*60);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       // �Ƿ�����CPU����ʱ�˿�

@param       �� 

@retval      int

@exception   ��
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
//@brief       �Ƿ����������֤��ʽ
//
//@param       ��
//
//@retval      bool true:���� ; false:������
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CTCMInfo::IsAuthorizeSet()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("IsAuthorizeSet")) == 0 ? false : true;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡACC��֤��ʽ
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int  CTCMInfo::GetACCAuthorizeManner()
//{
//	return m_accAuthorizeManner;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡECT��֤��ʽ
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int  CTCMInfo::GetECTAuthorizeManner()
//{
//	return m_ectAuthorizeManner;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡMCBF����(�ﵽ�ô��������MCBF)
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetMCBFCount()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("MCBFCount"), 3);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ������±�־
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetUpdateFlag()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UpdateFlag"), 0);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡACC��������ʧ�ܱ�־
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetACCUpdateFlag()
//{
//    return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("ACCUpdateFlag"), 0);
//}


////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡRFID��д״̬
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//int  CTCMInfo::GetRFIDWriteStatus()
//{
//	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("RFIDWriteableStatus"), 0);
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUSB��֤��ʽ��0������֤��1����Ҫ��֤��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetUSBAuthFlag()
{
	return m_TCMIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"), 0);
}


////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡFTP��־·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetFtpLogPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("LogPath"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡFtpSendFailLogPath
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetFtpSendFailLogPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("SendFailLogPath"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡFTP ���صĲ����ļ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTestFileDownloadFTPPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("TestFileDownloadFTPPath"));
//}
//


////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ���ز����ļ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTestFileDownloadLocalDir()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("TestFileDownloadLocalDir"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ���ز����ļ��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTestFileUploadLocalPath()
//{
//    return m_TCMIni->GetDataString(_T("FTP"), _T("TestFileUploadLocalPath"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTHͨ�ŷ�ʽ

@param       none 

@retval      int  0:���� 1:�ʲ� 2:�ܵ� 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHCommuMethod()
{
	return m_TCMIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTH���ں�

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
@brief       ��ȡConnect1BaudRate

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHBaudRate()
{
    return m_TCMIni->GetDataInt(_T("TH"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnReceive

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTHMaxConnReceive()
{
    return m_TCMIni->GetDataInt(_T("TH"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnSend

@param       �� 

@retval      int

@exception   ��
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
@brief       �ж��Ƿ�ʹ�ö�д��

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      bool true:ʹ�ö�д��  false:ʹ��ģ������

@exception   ��
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
@brief       ��ȡ�ⲿ��д�����ں�

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int ���ں�

@exception   ��
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
@brief       ��ȡ�ⲿ��д�����ڲ�����

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int ���ڲ�����

@exception   ��
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
@brief       ��ȡ�ⲿ��д��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetRWMaxConnReceive()
//{
//    return m_TCMIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_RECEIVE"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect2MaxConnSend

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//int CTCMInfo::GetRWMaxConnSend()
//{
//    return m_TCMIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_SEND"));
//}



//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ��ģ�⿨�������ͣ�����ʹ��

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д�� 

@retval      int

@exception   ��
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
@brief       ȡ�ö���ʱ����

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int

@exception   ��
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
@brief       �ж��Ƿ�ʹ�ô�ӡ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsPrinterUsed()
{
	return (m_TCMIni->GetDataInt(_T("PRINTER"), _T("IS_USED")) == 1);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡConnect3CommPort
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetPrinterCommPort()
{
    return m_TCMIni->GetDataInt(_T("PRINTER"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡConnect3BaudRate
//
//@param       �� 
//
//@retval      int
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetPrinterBaudRate()
{
   return m_TCMIni->GetDataInt(_T("PRINTER"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��Ʊ����ӡ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsCardPrinterUsed()
{
	return (m_TCMIni->GetDataInt(_T("CARD_PRINTER"), _T("IS_USED")) == 1);
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       ��ȡ��ӡ������

@param       �� 

@retval      CString

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetCardPrinterDriverName()
{
	return m_TCMIni->GetDataString(_T("CARD_PRINTER"), _T("DRIVER_NAME"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       ��ȡƱ����ӡ����ӡ��ʱ�ļ���

@param       �� 

@retval      CString

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetPrinterTempFile()
{
	return m_TCMIni->GetDataString(_T("CARD_PRINTER"), _T("PRT_TEMP"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       ��ȡƱ����ӡ����������

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetCardType()
{
	return m_TCMIni->GetDataInt(_T("CARD_PRINTER"), _T("CARD_TYPE"));	
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       �������Դ���

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetTestSendCardCount()
{
	return m_TCMIni->GetDataInt(_T("CARD_PRINTER"), _T("TEST_SEND_CARD_COUNT "));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ������ͷ

@param       �� 

@retval      bool

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsCameraUsed()
{
	return (m_TCMIni->GetDataInt(_T("CAMERA"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUpsUsed

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CTCMInfo::IsUPSUsed()
{
    return (m_TCMIni->GetDataInt(_T("UPS"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUPS CommPort

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetUPSCommPort()
{
	return m_TCMIni->GetDataInt(_T("UPS"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUPS BaudRate

@param       �� 

@retval      int

@exception   ��
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
@brief		�ж�TH�Ž�����Ƿ���

@param      none

@retval     bool true:����  false:�ر�

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
@brief    ȡ���Ž��ر�ʱ��ֵ  

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
//@brief       ��ȡUSB ʹ�õ� �̷�
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMUSBDrive()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("USBDRIVE")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡCF�� ʹ�õ� �̷�
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMCFDrive()
//{
//    return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("CFDRIVE"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM ϵͳ���� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMSysParaUploadPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMSysParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM ���Ʋ����� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMProgramUploadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMProgramUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �������� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMMachineParaUploadPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMachineParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �������� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMLogFileUploadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMLogFileUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ������� Data Audit �ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDataAuditPath(){
//	return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMDataAudit"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� Data Transaction �ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDataTransactionPath(){
//	return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMDataTransaction"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� Data Event �ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDataEventPath(){
//	return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMDataEvent"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� �����ݿ��ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMMainDBUploadPath(){
//    return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMainDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� �������ݿ��ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMBackupDBUploadPath(){
//    return m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMBackupDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� ���ݿ� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMDBDownloadPath(){
//    return m_TCMIni->GetDataString(_T("MOVEDISK"),_T("TCMDBDownloadPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� ϵͳ���� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMSysParaDownloadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMSysParaDownloadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� ���Ʋ����� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMProgramDownloadPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMProgramDownloadPath")));
//}
//
//////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM �ƶ����� �������� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
//////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMMachineParaDownloadPath()
{
    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMachineParaDownloadPath")));
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM ������־�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMMaintentanceOutputPath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMaintenance")));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡTCM ά��Trace�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTCMMaintenceTracePath()
//{
//    return AddBackslash(m_TCMIni->GetDataString(_T("MOVEDISK"), _T("TCMMaintenanceTrace")));
//}
////////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�����ݼ�¼���������

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
@brief      ȡ�����ݼ�¼ʵ�ʱ��������

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
//@brief      ȡ�����ݼ�¼ʵ�ʱ��������
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
//@brief      ȡ��һ�η���Transaction���ĵ��������
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
//@brief      ȡ��һ�η���Event���ĵ��������
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
//@brief      ȡ�ø���ҵ���ȡ�¿��ȴ�ʱ��
//
//@param      none
//
//@retval     int �ȴ�ʱ��
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
//@brief      ȡ�õȴ�һƱͨ�Ǽ�ʱ�˿��ѯ������Ӧ��ʱ��
//
//@param      none
//
//@retval     int �ȴ�ʱ��
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
//@brief      ȡ��Ӧ������Ӧ��Ϣ
//
//@param      (i)OperationalMode_t operationalMode     Ӧ�����
//
//@retval     CString
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetOperationalMode(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // ���ģʽȫ����Ϊ����ģʽ����
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_TCMIni->GetDataString(_T("OPERATIONAL_MODE"), sMessageID);
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��Ӧ������Ӧ��Ϣ
//
//@param      (i)OperationalMode_t operationalMode     Ӧ�����
//
//@retval     CString
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetOperationalModeEn(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // ���ģʽȫ����Ϊ����ģʽ����
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_TCMIni->GetDataString(_T("OPERATIONAL_MODE_EN"), sMessageID);
//}
//
//////////////////////////////////////////////////////////////////////////

//brief      ȡ��Ӧ������Ӧ��Ϣ
//
//param      (i)RESULT_CODE_MSG resultCode     Ӧ�����
//
//retval     CString  �����������Ϣ
//
//exception  ��

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

//@brief      ȡ�ó�վģʽ����
//
//@param      (i)OPERATION_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

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
////@brief      ȡ���ó�״̬����(��)
////
////@param      (i)JourneyStatus_t journeyStatus   ���Դ���
////
////@retval     CString  �����������Ϣ(��)
////
////@exception  ��
//
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetJourneyStatusCn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;    
//}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ���ó�״̬����(Ӣ)
//
//@param      (i)JourneyStatus_t journeyStatus   ���Դ���
//
//@retval     CString  �����������Ϣ(Ӣ)
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetJourneyStatusEn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // ��������
//    return strMsgEn;    
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ�ý�����������(��)
//
//@param      (i)U16_t tranType   
//
//@retval     CString  �����������Ϣ(��)
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTransactionTypeCn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ�ý�����������(Ӣ)
//
//@param      (i)U16_t tranType   
//
//@retval     CString  �����������Ϣ(Ӣ)
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetTransactionTypeEn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // ��������
//    return strMsgEn;
//}
//
//////////////////////////////////////////////////////////////////////////

//@brief      ȡ��ͨ��ҵ��˿ͻ�������
//
//@param      (i)PASSENGER_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerMessageCn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgCn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
//
//        // ����
//        CString strMsgCn;
//        AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//        m_mapPassengerMsgCn.SetAt(msgCode,strMsgCn);
//
//        // ��������
//        msg = strMsgCn;
//    }
//    return msg;
//}

//////////////////////////////////////////////////////////////////////////
//
//@brief      ȡ��ͨ��ҵ��˿ͻ�������
//
//@param      (i)PASSENGER_MSG msgCode   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��

//////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerMessageEn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgEn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
//
//        // Ӣ��
//        CString strMsgEn;
//        AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//        m_mapPassengerMsgEn.SetAt(msgCode,strMsgEn);
//
//        // ����Ӣ��
//        msg = strMsgEn;
//    }
//    return msg;
//}
//
////////////////////////////////////////////////////////////////////////////

//brief      ȡ��ά��ҵ�����Ա��������
//
//param      (i)MAINTENANCE_MSG msgCode   ���Դ���
//
//retval     CString  �����������Ϣ
//
//exception  ��

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
//@brief      ȡ�ó˿���������(��)
//
//@param      (i)PassengerType_t type   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerTypeCn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ�ó˿���������(Ӣ)
//
//@param      (i)PassengerType_t type   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetPassengerTypeEn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//    
//    // ����Ӣ��
//    return strMsgEn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ������״̬����(��)
//
//@param      (i)PassengerType_t type   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetLockStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
// 
//    // ��������
//    return strMsgCn;    
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ������״̬����(Ӣ)
//
//@param      (i)PassengerType_t type   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetLockStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // ����Ӣ��
//    return strMsgEn;    
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ�ó�Ʊ״̬����(��)
//
//@param      (i)CardStatusCode_t type   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetCardStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("CARD_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;    
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ�ó�Ʊ״̬����(Ӣ)
//
//@param      (i)CardStatusCode_t type   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetCardStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("CARD_STATUS"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':'); 
//
//    // ����Ӣ��
//    return strMsgEn;    
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��һ��ͨ��״̬(��)
//
//@param      (i)CARDSTATUS status   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTStatusCn(CARDSTATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��һ��ͨ�ó�״̬(��)
//
//@param      (i)CARDSTATUS status   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTJourneyStatusCn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��һ��ͨ�ó�״̬(Ӣ)
//
//@param      (i)CARDSTATUS status   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTJourneyStatusEn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // ����Ӣ��
//    return strMsgEn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����(��)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTSystemCodeCn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����(Ӣ)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTSystemCodeEn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // ����Ӣ��
//    return strMsgEn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����(��)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTTransTypeCn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����(Ӣ)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTTransTypeEn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgEn = _T("");
//    AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//    // ��������
//    return strMsgEn;
//}
//
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����(��)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTStaffTransTypeCn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
//
//	// ����
//	CString strMsgCn = _T("");
//	AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//	// ��������
//	return strMsgCn;
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����(Ӣ)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����Ӣ����Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTStaffTransTypeEn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
//
//	// ����
//	CString strMsgEn = _T("");
//	AfxExtractSubString(strMsgEn,resultCodeMsg,1,':');
//
//	// ��������
//	return strMsgEn;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief      ȡ��ϵͳ����������(��)
//
//@param      (i)BYTE systemCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CTCMInfo::GetYKTAgenceCodeCn(BYTE agenceCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), agenceCode);
//    CString resultCodeMsg = m_TCMIni->GetDataString(_T("YKT_AGENCECODE_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("");
//    AfxExtractSubString(strMsgCn,resultCodeMsg,0,':');
//
//    // ��������
//    return strMsgCn;
//}
//
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ���ö�ģʽ
//
//@param       (o)const int num
//
//@retval      void
//
//@exception   ��
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
//@brief       ����ACC��֤��ʽ
//
//@param       (i)int authorizeManner
//
//@retval      void
//
//@exception   ��
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
//@brief       ����ECT��֤��ʽ
//
//@param       (i)int authorizeManner
//
//@retval      void
//
//@exception   ��
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
//@brief       ���ó�����±�־
//
//@param       (i)int updateFlag
//
//@retval      void
//
//@exception   ��
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
//@brief       ����ACC��������ʧ�ܱ�־
//
//@param       (i)int updateFlag
//
//@retval      void
//
//@exception   ��
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
//@brief       ����RFID��д״̬
//
//@param       (o)int status
//
//@retval      void
//
//@exception   ��
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
//@brief       ��ȡ TCMϵͳ����ʹ�õ���������
//
//@param       �� 
//
//@retval      CString 
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CTCMInfo::GetTCMUsedAdapterName()
{
	return m_TCMIni->GetDataString(_T("MACHINE"), _T("USED_ADAPTER_NAME"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ������ԭ������

@param       ��

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetIncomeReasonCount()
{
	return m_TCMIni->GetDataInt(_T("INCOME_REASON"),_T("COUNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ������ԭ������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ��֧��ԭ������

@param       ��

@retval      int 

@exception   ��
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
@brief       ȡ�ò�Ʊ�������ݣ�վԱ���棩

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ�ò�Ʊ�������ݣ��˿ͽ��棩

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ���˿������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ�ý�������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ���˿������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ�ü���������������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ���˿������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ����·����

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ����·Ӣ������

@param       (i)int NO

@retval      CString 

@exception   ��
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

@exception   ��
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

@exception   ��
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
@brief   ȡ�ý��״����� ����λ���룩  

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
@brief      ȡ��ҵ�����ݴ�����ʱ�䣨��λ���룩

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
@brief     ȡ�ûص�Ĭ��ҵ��ļ��ʱ�䣨��λ���룩 

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
@brief      ȡ��ҵ���еȴ������ļ��ʱ�䣨��λ���룩

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CTCMInfo::GetWaitOperationInterval()
{
	return m_TCMIni->GetDataInt(_T("INTERVAL"), _T("WAIT_OPERATION"),30);
}

