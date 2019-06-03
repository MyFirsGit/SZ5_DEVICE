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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAGMInfo::CAGMInfo()
{
    try {
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
CAGMInfo::~CAGMInfo()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ڰ�װ�İ汾��Ϣ

@param       �� 

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetInitialVersion()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
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
//@brief       �Ƿ���ʾ�����
//
//@param       ��
//
//@retval      bool true:��ʾ ; false:����ʾ
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
bool CAGMInfo::CanShowCursor()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
}
//
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
CString CAGMInfo::GetDataPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("APPLICATION"), _T("DataPath")));
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
CString CAGMInfo::GetBackupPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("APPLICATION"), _T("BackupPath")));
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
//@brief       ��ȡ�������δ����������
//
//@param       �� 
//
//@retval      UINT
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
UINT CAGMInfo::GetLimitedDaysOfUnSendData()
{
	return m_DeviceIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
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
UINT CAGMInfo::GetLimitedDaysOfSendData()
{
	return m_DeviceIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡBOM ���Ʋ����� �ϴ�·��

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomExportPath()
{
	return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("ExportPath")));
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
//int CAGMInfo::GetMaxRecoverTimes()
//{
//    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("MaxRecoverTimes"), 10);
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
//bool CAGMInfo::IsLoginTimeFixed()
//{
//    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("LoginTimeFixed")) == 0 ? false : true;
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
 bool CAGMInfo::IsWriteDebug()
 {
    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("WriteDebug")) == 0 ? false : true;
 }

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ܰ�ť��������

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetFuncBtnFont()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("FuncBtnFont"),_T("����"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ܰ�ť�����С

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetFuncBtnFontSize()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"),_T("FuncBtnFontSize"), 16);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡҵ��ť��������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetOperateBtnFont()
{
    return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("OperateBtnFont"),_T("����"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡҵ��ť�����С

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetOperateBtnFontSize()
{
    return m_DeviceIni->GetDataInt(_T("APPLICATION"),_T("OperateBtnFontSize"), 12);
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
bool CAGMInfo::IsUseFounderStaffID()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡŷķ��Ա��ID

@param       �� 

@retval      int

@exception   ��
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
@brief       ��ȡŷķ��Ա������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetFounderStaffPassword()
{
	return m_DeviceIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        �Ƿ������Զ��峵Ʊ����

@param       �� 

@retval      bool

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsUseConfigTicketCategory()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UseConfigTicketCategory"),0) == 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ȡ�����Զ��峵Ʊ����

@param       ticketTypeCode 

@retval      int

@exception   ��
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
@brief        ȡ��ҵ��������ȴ�ʱ�䵥λ���룩

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CAGMInfo::GetCloseOperationMaxWaitSeconds()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("CloseOperationMaxWaitSeconds"),10*60);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       // �Ƿ�����CPU����ʱ�˿�

@param       �� 

@retval      int

@exception   ��
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
//@brief       �Ƿ����������֤��ʽ
//
//@param       ��
//
//@retval      bool true:���� ; false:������
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//bool CAGMInfo::IsAuthorizeSet()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("IsAuthorizeSet")) == 0 ? false : true;
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
//int  CAGMInfo::GetACCAuthorizeManner()
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
//int  CAGMInfo::GetECTAuthorizeManner()
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
//int CAGMInfo::GetMCBFCount()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("MCBFCount"), 3);
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
//int CAGMInfo::GetUpdateFlag()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UpdateFlag"), 0);
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
//int CAGMInfo::GetACCUpdateFlag()
//{
//    return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("ACCUpdateFlag"), 0);
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
//int  CAGMInfo::GetRFIDWriteStatus()
//{
//	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("RFIDWriteableStatus"), 0);
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUSB��֤��ʽ��0������֤��1����Ҫ��֤��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetUSBAuthFlag()
{
	return m_DeviceIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"), 0);
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
//CString CAGMInfo::GetFtpLogPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("LogPath"));
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
//CString CAGMInfo::GetFtpSendFailLogPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("SendFailLogPath"));
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
//CString CAGMInfo::GetTestFileDownloadFTPPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("TestFileDownloadFTPPath"));
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
//CString CAGMInfo::GetTestFileDownloadLocalDir()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("TestFileDownloadLocalDir"));
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
//CString CAGMInfo::GetTestFileUploadLocalPath()
//{
//    return m_DeviceIni->GetDataString(_T("FTP"), _T("TestFileUploadLocalPath"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTHͨ�ŷ�ʽ

@param       none 

@retval      int  0:���� 1:�ʲ� 2:�ܵ� 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHCommuMethod()
{
	return m_DeviceIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTH���ں�

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
@brief       ��ȡConnect1BaudRate

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHBaudRate()
{
    return m_DeviceIni->GetDataInt(_T("TH"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnReceive

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTHMaxConnReceive()
{
    return m_DeviceIni->GetDataInt(_T("TH"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnSend

@param       �� 

@retval      int

@exception   ��
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
@brief       �ж��Ƿ�ʹ�ö�д��

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      bool true:ʹ�ö�д��  false:ʹ��ģ������

@exception   ��
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
@brief       ��ȡ�ⲿ��д�����ں�

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int ���ں�

@exception   ��
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
@brief       ��ȡ�ⲿ��д�����ڲ�����

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int ���ڲ�����

@exception   ��
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
@brief       ��ȡ�ⲿ��д��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//int CAGMInfo::GetRWMaxConnReceive()
//{
//    return m_DeviceIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_RECEIVE"));
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect2MaxConnSend

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//int CAGMInfo::GetRWMaxConnSend()
//{
//    return m_DeviceIni->GetDataInt(_T("OUT_ICRW"), _T("MAX_CONN_SEND"));
//}



//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ��ģ�⿨�������ͣ�����ʹ��

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д�� 

@retval      int

@exception   ��
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
@brief       ȡ�ö���ʱ����

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int

@exception   ��
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
@brief       �ж��Ƿ�ʹ�ô�ӡ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsPrinterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("PRINTER"), _T("IS_USED")) == 1);
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
int CAGMInfo::GetPrinterCommPort()
{
    return m_DeviceIni->GetDataInt(_T("PRINTER"), _T("COMM_PORT"));
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
int CAGMInfo::GetPrinterBaudRate()
{
   return m_DeviceIni->GetDataInt(_T("PRINTER"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ��Ʊ����ӡ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsCardPrinterUsed()
{
	return (m_DeviceIni->GetDataInt(_T("CARD_PRINTER"), _T("IS_USED")) == 1);
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       ��ȡ��ӡ������

@param       �� 

@retval      CString

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetCardPrinterDriverName()
{
	return m_DeviceIni->GetDataString(_T("CARD_PRINTER"), _T("DRIVER_NAME"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       ��ȡƱ����ӡ����ӡ��ʱ�ļ���

@param       �� 

@retval      CString

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetPrinterTempFile()
{
	return m_DeviceIni->GetDataString(_T("CARD_PRINTER"), _T("PRT_TEMP"));
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       ��ȡƱ����ӡ����������

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetCardType()
{
	return m_DeviceIni->GetDataInt(_T("CARD_PRINTER"), _T("CARD_TYPE"));	
}

////////////////////////////////////////////////////////////////////////////
/*
@brief       �������Դ���

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetTestSendCardCount()
{
	return m_DeviceIni->GetDataInt(_T("CARD_PRINTER"), _T("TEST_SEND_CARD_COUNT "));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ������ͷ

@param       �� 

@retval      bool

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsCameraUsed()
{
	return (m_DeviceIni->GetDataInt(_T("CAMERA"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUpsUsed

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CAGMInfo::IsUPSUsed()
{
    return (m_DeviceIni->GetDataInt(_T("UPS"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUPS CommPort

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetUPSCommPort()
{
	return m_DeviceIni->GetDataInt(_T("UPS"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUPS BaudRate

@param       �� 

@retval      int

@exception   ��
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
@brief		�ж�TH�Ž�����Ƿ���

@param      none

@retval     bool true:����  false:�ر�

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
@brief    ȡ���Ž��ر�ʱ��ֵ  

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
//@brief       ��ȡUSB ʹ�õ� �̷�
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBOMUSBDrive()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("USBDRIVE")));
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
//CString CAGMInfo::GetBOMCFDrive()
//{
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("CFDRIVE"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBOM ϵͳ���� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomSysParaUploadPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomSysParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBOM ���Ʋ����� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomProgramUploadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomProgramUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBOM �������� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomMachineParaUploadPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMachineParaUploadPath")));
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBOM �������� �ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomLogFileUploadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomLogFileUploadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBOM �ƶ������� Data Audit �ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDataAuditPath(){
//	return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomDataAudit"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� Data Transaction �ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDataTransactionPath(){
//	return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomDataTransaction"));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� Data Event �ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDataEventPath(){
//	return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomDataEvent"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� �����ݿ��ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomMainDBUploadPath(){
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMainDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� �������ݿ��ļ��е��ϴ�·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomBackupDBUploadPath(){
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomBackupDBPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� ���ݿ� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomDBDownloadPath(){
//    return m_DeviceIni->GetDataString(_T("MOVEDISK"),_T("BomDBDownloadPath"));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� ϵͳ���� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomSysParaDownloadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomSysParaDownloadPath")));
//}
//
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� ���Ʋ����� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomProgramDownloadPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomProgramDownloadPath")));
//}
//
//////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom �ƶ����� �������� �ļ��е�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
//////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomMachineParaDownloadPath()
{
    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMachineParaDownloadPath")));
}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom ������־�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomMaintentanceOutputPath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMaintenance")));
//}
////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡBom ά��Trace�����·��
//
//@param       �� 
//
//@retval      CString
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetBomMaintenceTracePath()
//{
//    return AddBackslash(m_DeviceIni->GetDataString(_T("MOVEDISK"), _T("BomMaintenanceTrace")));
//}
////////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�����ݼ�¼���������

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
@brief      ȡ�����ݼ�¼ʵ�ʱ��������

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
//@brief      ȡ�����ݼ�¼ʵ�ʱ��������
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
//@brief      ȡ��һ�η���Transaction���ĵ��������
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
//@brief      ȡ��һ�η���Event���ĵ��������
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
//@brief      ȡ�ø���ҵ���ȡ�¿��ȴ�ʱ��
//
//@param      none
//
//@retval     int �ȴ�ʱ��
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
//@brief      ȡ�õȴ�һƱͨ�Ǽ�ʱ�˿��ѯ������Ӧ��ʱ��
//
//@param      none
//
//@retval     int �ȴ�ʱ��
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
//@brief      ȡ��Ӧ������Ӧ��Ϣ
//
//@param      (i)OperationalMode_t operationalMode     Ӧ�����
//
//@retval     CString
//
//@exception  
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetOperationalMode(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // ���ģʽȫ����Ϊ����ģʽ����
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_DeviceIni->GetDataString(_T("OPERATIONAL_MODE"), sMessageID);
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
//CString CAGMInfo::GetOperationalModeEn(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // ���ģʽȫ����Ϊ����ģʽ����
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_DeviceIni->GetDataString(_T("OPERATIONAL_MODE_EN"), sMessageID);
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

//@brief      ȡ�ó�վģʽ����
//
//@param      (i)OPERATION_MSG msgCode   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��

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
////@brief      ȡ���ó�״̬����(��)
////
////@param      (i)JourneyStatus_t journeyStatus   ���Դ���
////
////@retval     CString  �����������Ϣ(��)
////
////@exception  ��
//
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetJourneyStatusCn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetJourneyStatusEn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetTransactionTypeCn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetTransactionTypeEn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetPassengerMessageCn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgCn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
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
//CString CAGMInfo::GetPassengerMessageEn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgEn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
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
//@brief      ȡ�ó˿���������(��)
//
//@param      (i)PassengerType_t type   ���Դ���
//
//@retval     CString  �����������Ϣ
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
//CString CAGMInfo::GetPassengerTypeCn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
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
//CString CAGMInfo::GetPassengerTypeEn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
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
//CString CAGMInfo::GetLockStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
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
//CString CAGMInfo::GetLockStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
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
//CString CAGMInfo::GetCardStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("CARD_STATUS"), sMessageID);
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
//CString CAGMInfo::GetCardStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("CARD_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTStatusCn(CARDSTATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTJourneyStatusCn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTJourneyStatusEn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
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
//CString CAGMInfo::GetYKTSystemCodeCn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
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
//CString CAGMInfo::GetYKTSystemCodeEn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
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
//CString CAGMInfo::GetYKTTransTypeCn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTTransTypeEn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTStaffTransTypeCn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTStaffTransTypeEn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
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
//CString CAGMInfo::GetYKTAgenceCodeCn(BYTE agenceCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), agenceCode);
//    CString resultCodeMsg = m_DeviceIni->GetDataString(_T("YKT_AGENCECODE_TYPE"), sMessageID);
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
//void CAGMInfo::SetReadMode(int num)
//{
//	CString str;
//	str.Format(_T("%d"), num);
//	m_DeviceIni->WriteData(_T("ICRW"), _T("READ_MODE"), str);
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
//void CAGMInfo::SetACCAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_DeviceIni->WriteData(_T("APPLICATION"), _T("ACCAuthorizeManner"), str);
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
//void CAGMInfo::SetECTAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_DeviceIni->WriteData(_T("APPLICATION"), _T("ECTAuthorizeManner"), str);
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
//void CAGMInfo::SetUpdateFlag(int updateFlag)
//{
//	CString str;
//	str.Format(_T("%d"), updateFlag);
//	m_DeviceIni->WriteData(_T("APPLICATION"), _T("UpdateFlag"), str);
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
//void CAGMInfo::SetACCUpdateFlag(int updateFlag)
//{
//    CString str;
//    str.Format(_T("%d"), updateFlag);
//    m_DeviceIni->WriteData(_T("APPLICATION"), _T("ACCUpdateFlag"), str);
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
//void CAGMInfo::SetRFIDWriteStatus(int status)
//{
//	CString str;
//	str.Format(_T("%d"), status);
//	m_DeviceIni ->WriteData(_T("APPLICATION"), _T("RFID_WRITEABLE_STATUS"), str);
//}
//



////////////////////////////////////////////////////////////////////////////
///**
//@brief       ��ȡ BOMϵͳ����ʹ�õ���������
//
//@param       �� 
//
//@retval      CString 
//
//@exception   ��
//*/
////////////////////////////////////////////////////////////////////////////
CString CAGMInfo::GetBomUsedAdapterName()
{
	return m_DeviceIni->GetDataString(_T("MACHINE"), _T("BOMUSED_ADAPTER_NAME"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ������ԭ������

@param       ��

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetIncomeReasonCount()
{
	return m_DeviceIni->GetDataInt(_T("INCOME_REASON"),_T("COUNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ������ԭ������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ��֧��ԭ������

@param       ��

@retval      int 

@exception   ��
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
@brief       ȡ�ò�Ʊ�������ݣ�վԱ���棩

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ�ò�Ʊ�������ݣ��˿ͽ��棩

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ���˿������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ�ý�������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ���˿������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ�ü���������������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ���˿������

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ����·����

@param       (i)int NO

@retval      CString 

@exception   ��
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
@brief       ȡ����·Ӣ������

@param       (i)int NO

@retval      CString 

@exception   ��
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

@exception   ��
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

@exception   ��
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
@brief   ȡ�ý��״����� ����λ���룩  

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
@brief      ȡ��ҵ�����ݴ�����ʱ�䣨��λ���룩

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
@brief     ȡ�ûص�Ĭ��ҵ��ļ��ʱ�䣨��λ���룩 

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
@brief      ȡ��ҵ���еȴ������ļ��ʱ�䣨��λ���룩

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAGMInfo::GetWaitOperationInterval()
{
	return m_DeviceIni->GetDataInt(_T("INTERVAL"), _T("WAIT_OPERATION"),120);
}