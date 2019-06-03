#include "StdAfx.h"
#include "BomInfo.h"
#include "SysException.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BOM_INI_FILE_NAME		    _T("BOM.INI")

CBomInfo CBomInfo::theInstance;

CBomInfo& CBomInfo::GetInstance()
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
CBomInfo::CBomInfo()
{
    try {
        m_bomIni = NULL;

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
CBomInfo::~CBomInfo()
{
    delete m_bomIni;
    m_bomIni = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ���ڻ�

@param       �� 

@retval      int 0 �ɹ���1 ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::Initialize() 
{
    try {
        // ���ѳ�ʼ������ֱ�ӷ���
        if (m_bomIni != NULL) {
            return 0;
        }

        TCHAR szAppPath[_MAX_PATH];

        if (ComGetAppPath(szAppPath, _MAX_PATH) <= 0) {
			return 1;
        }

        CString fileName = CString(szAppPath) + BOM_INI_FILE_NAME;

        // check BOM.INI file exist
        if (FALSE == ComCheckFileExist(fileName)) {
			return 1;
        }

        // �ļ����Ը�Ϊ��д
        SetFileAttributes(fileName, FILE_ATTRIBUTE_NORMAL);

        m_bomIni = new CGetIni(fileName);
		
		Initialized.Invoke();

		//[APPLICATION]
		//m_accAuthorizeManner = m_bomIni->GetDataInt("APPLICATION", "ACCAuthorizeManner",2);			// ACC��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		//m_ectAuthorizeManner = m_bomIni->GetDataInt("APPLICATION", "ECTAuthorizeManner",2);			// ECT��֤��ʽ��0������֤��1���ڲ�ģʽ��2���ⲿģʽ��
		//m_deviceTestMode = m_bomIni->GetDataInt("APPLICATION", "DEVICE_TEST_MODE",0);				// �豸����ģʽ��0������ģʽ��1������ģʽ��
		//m_isRWUsed = (m_bomIni->GetDataInt("OUT_ICRW", "IS_USED") == 1);
		//m_rfidWriteStatus = m_bomIni->GetDataInt("APPLICATION", "RFID_WRITEABLE_STATUS",1);			// RFID��д״̬���ã�0������д�룻1����ֹд�룩

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
CString CBomInfo::AddBackslash(CString sPath)
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
CString CBomInfo::GetInitialVersion()
{
    return m_bomIni->GetDataString(_T("APPLICATION"), _T("InitialVersion"));
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
bool CBomInfo::IsTopmost()
{
    return m_bomIni->GetDataInt(_T("APPLICATION"), _T("TopMost"), 1) == 0 ? false : true;
}

bool CBomInfo::IsFunctionKeyEnabled()
{
	 return m_bomIni->GetDataInt(_T("APPLICATION"), _T("FunctionKeyEnable"), 1) != 0 ;
}
bool CBomInfo::IsTaskManagerEnabled()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("TaskManagerEnabled"), 1) != 0 ;
}

bool    CBomInfo::IsDesktopEnabled()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("DesktopEnabled"), 1) != 0 ;
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
bool CBomInfo::CanShowCursor()
{
    return m_bomIni->GetDataInt(_T("APPLICATION"), _T("ShowCursor")) == 0 ? false : true;
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
CString CBomInfo::GetDataPath()
{
    return AddBackslash(m_bomIni->GetDataString(_T("APPLICATION"), _T("DataPath")));
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
CString CBomInfo::GetBackupPath()
{
    return AddBackslash(m_bomIni->GetDataString(_T("APPLICATION"), _T("BackupPath")));
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
CString CBomInfo::GetLogPath()
{
	CString strLogPath =  m_bomIni->GetDataString(_T("APPLICATION"), _T("LogPath"),_T(""));
	if(strLogPath == _T(""))
	{
		strLogPath = GetBackupPath() + _T("\Log");
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
UINT CBomInfo::GetLimitedDaysOfUnSendData()
{
	return m_bomIni->GetDataInt(_T("TXNDATA"),_T("UnsendMaxDay"),7);
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
UINT CBomInfo::GetLimitedDaysOfSendData()
{
	return m_bomIni->GetDataInt(_T("TXNDATA"),_T("SendedMaxDay"),10);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡBOM ���Ʋ����� �ϴ�·��

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CBomInfo::GetBomExportPath()
{
	return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), _T("ExportPath")));
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
//int CBomInfo::GetMaxRecoverTimes()
//{
//    return m_bomIni->GetDataInt(_T("APPLICATION", "MaxRecoverTimes", 10);
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
//bool CBomInfo::IsLoginTimeFixed()
//{
//    return m_bomIni->GetDataInt(_T("APPLICATION", "LoginTimeFixed") == 0 ? false : true;
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
 bool CBomInfo::IsWriteDebug()
 {
    return m_bomIni->GetDataInt(_T("APPLICATION"), _T("WriteDebug")) == 0 ? false : true;
 }

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ܰ�ť��������

@param       �� 

@retval      CString

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CBomInfo::GetFuncBtnFont()
{
    return m_bomIni->GetDataString(_T("APPLICATION"),_T( "FuncBtnFont"),_T( "����"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ���ܰ�ť�����С

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetFuncBtnFontSize()
{
    return m_bomIni->GetDataInt(_T("APPLICATION"),_T("FuncBtnFontSize"), 16);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡҵ��ť��������

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CBomInfo::GetOperateBtnFont()
{
    return m_bomIni->GetDataString(_T("APPLICATION"),_T( "OperateBtnFont"),_T( "����"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡҵ��ť�����С

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetOperateBtnFontSize()
{
    return m_bomIni->GetDataInt(_T("APPLICATION"),_T( "OperateBtnFontSize"), 12);
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
bool CBomInfo::IsUseFounderStaffID()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("UseFounderStaff"),0) == 0 ? false : true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡŷķ��Ա��ID

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetFounderStaffID()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("FounderStaffID"), 10101028);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsFounderStaffID(int userID)
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
CString CBomInfo::GetFounderStaffPassword()
{
	return m_bomIni->GetDataString(_T("APPLICATION"), _T("FounderStaffPassword"), _T("888888"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        �Ƿ������Զ��峵Ʊ����

@param       �� 

@retval      bool

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsUseConfigTicketCategory()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("UseConfigTicketCategory"),0) == 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief        ȡ�����Զ��峵Ʊ����

@param       ticketTypeCode 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetConfigTicketCategory(WORD ticketTypeCode)
{
	CString strTicketCode = _T("");
	strTicketCode.Format(_T("%d"),ticketTypeCode);
	return m_bomIni->GetDataInt(_T("TICKET_CATEGORY"),strTicketCode,-1);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief        ȡ��ҵ��������ȴ�ʱ�䵥λ���룩

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

int CBomInfo::GetCloseOperationMaxWaitSeconds()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("CloseOperationMaxWaitSeconds"),10*60);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       // �Ƿ�����CPU����ʱ�˿�

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::IsEnableCPUImmidatelyRefund()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("EnableCPUImmidatelyRefund"),0) == 1;
}

bool CBomInfo::IsValidateUnSendPackage()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("ValidateUnSendPackage"),1) != 99;
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
//bool CBomInfo::IsAuthorizeSet()
//{
//	return m_bomIni->GetDataInt(_T("APPLICATION"), "IsAuthorizeSet") == 0 ? false : true;
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
//int  CBomInfo::GetACCAuthorizeManner()
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
//int  CBomInfo::GetECTAuthorizeManner()
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
//int CBomInfo::GetMCBFCount()
//{
//	return m_bomIni->GetDataInt(_T("APPLICATION"), "MCBFCount"), 3);
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
//int CBomInfo::GetUpdateFlag()
//{
//	return m_bomIni->GetDataInt(_T("APPLICATION"), "UpdateFlag"), 0);
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
//int CBomInfo::GetACCUpdateFlag()
//{
//    return m_bomIni->GetDataInt(_T("APPLICATION"), "ACCUpdateFlag"), 0);
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
//int  CBomInfo::GetRFIDWriteStatus()
//{
//	return m_bomIni->GetDataInt(_T("APPLICATION"), "RFIDWriteableStatus"), 0);
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUSB��֤��ʽ��0������֤��1����Ҫ��֤��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetUSBAuthFlag()
{
	return m_bomIni->GetDataInt(_T("APPLICATION"), _T("UsbAuthorizeManner"), 0);
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
//CString CBomInfo::GetFtpLogPath()
//{
//    return m_bomIni->GetDataString(_T("FTP"), "LogPath");
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
//CString CBomInfo::GetFtpSendFailLogPath()
//{
//    return m_bomIni->GetDataString(_T("FTP"), "SendFailLogPath");
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
//CString CBomInfo::GetTestFileDownloadFTPPath()
//{
//    return m_bomIni->GetDataString(_T("FTP"), "TestFileDownloadFTPPath");
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
//CString CBomInfo::GetTestFileDownloadLocalDir()
//{
//    return m_bomIni->GetDataString(_T("FTP"), "TestFileDownloadLocalDir");
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
//CString CBomInfo::GetTestFileUploadLocalPath()
//{
//    return m_bomIni->GetDataString(_T("FTP"), "TestFileUploadLocalPath");
//}


//////////////////////////////////////////////////////////////////////////
/*
@brief ȡ��ƾ����ӡ����     

@param ƾ������

@retval     ��ӡ������Ĭ��1��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetReceiptPrintTimes(CString receiptName)
{
	return m_bomIni->GetDataInt(_T("RECEIPT_PRINT_CONFIG"),receiptName,1);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTHͨ�ŷ�ʽ

@param       none 

@retval      int  0:���� 1:�ʲ� 2:�ܵ� 

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetTHCommuMethod()
{
	return m_bomIni->GetDataInt(_T("TH"), _T("CMU_METHOD"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡTH���ں�

@param       none 

@retval      int

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetTHCommPort()
{
    return m_bomIni->GetDataInt(_T("TH"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1BaudRate

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetTHBaudRate()
{
    return m_bomIni->GetDataInt(_T("TH"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnReceive

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetTHMaxConnReceive()
{
    return m_bomIni->GetDataInt(_T("TH"), _T("MAX_CONN_RECEIVE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect1MaxConnSend

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetTHMaxConnSend()
{
    return m_bomIni->GetDataInt(_T("TH"), _T("MAX_CONN_SEND"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsTHMonitorOpened()
{
	return (m_bomIni->GetDataInt(_T("TH"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetTHMonitorInterval()
{
	return m_bomIni->GetDataInt(_T("TH"),_T("MONITOR_INTERVAL"),5000);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ�ö�д��

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      bool true:ʹ�ö�д��  false:ʹ��ģ������

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsRWUsed(bool isRW)
{
	if(isRW){
		return (m_bomIni->GetDataInt(_T("ICRW"), _T("IS_USED")) == 1);
	}
	return (m_bomIni->GetDataInt(_T("ICTW"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool	CBomInfo::IsRWMonitorOpened(bool bIsRW)
{
	if(bIsRW){
		return m_bomIni->GetDataInt(_T("ICRW"), _T("MONITOR_SWITCH"),0)!=0;
	}
	return m_bomIni->GetDataInt(_T("ICTW"), _T("MONITOR_SWITCH"),0)!=0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int     CBomInfo::GetRWMonitorInterval(bool bIsRW)
{
	if(bIsRW){
		return m_bomIni->GetDataInt(_T("ICRW"), _T("MONITOR_INTERVAL"),30);
	}
	return m_bomIni->GetDataInt(_T("ICTW"), _T("MONITOR_INTERVAL"),30);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�ⲿ��д�����ں�

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int ���ں�

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetRWCommPort(bool isRW)
{
	if(isRW){
		return m_bomIni->GetDataInt(_T("ICRW"), _T("COMM_PORT"));
	}
    return m_bomIni->GetDataInt(_T("ICTW"), _T("COMM_PORT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�ⲿ��д�����ڲ�����

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int ���ڲ�����

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetRWBaudRate(bool isRW)
{
	if(isRW){
		return m_bomIni->GetDataInt(_T("ICRW"), _T("BAUD_RATE"));
	}
    return m_bomIni->GetDataInt(_T("ICTW"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡ�ⲿ��д��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//int CBomInfo::GetRWMaxConnReceive()
//{
//    return m_bomIni->GetDataInt(_T("OUT_ICRW"), "MAX_CONN_RECEIVE");
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡConnect2MaxConnSend

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
//int CBomInfo::GetRWMaxConnSend()
//{
//    return m_bomIni->GetDataInt(_T("OUT_ICRW"), "MAX_CONN_SEND");
//}



//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ��ģ�⿨�������ͣ�����ʹ��

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д�� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetSimCardType(bool isRW)
{
	if (isRW){
		return m_bomIni->GetDataInt(_T("ICRW"), _T("SIM_CARD_TYPE"));
	}
	return m_bomIni->GetDataInt(_T("ICTW"), _T("SIM_CARD_TYPE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ�ö���ʱ����

@param       (i)bool isRW  true:�ⲿ��д��  false:�ڲ���д��

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int	CBomInfo::GetReadCardInterval(bool isRW)
{
	if (isRW){
		return m_bomIni->GetDataInt(_T("ICRW"), _T("READ_CARD_INTERVAL"));
	}
	return m_bomIni->GetDataInt(_T("ICTW"), _T("READ_CARD_INTERVAL"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       �ж��Ƿ�ʹ�ô�ӡ��

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsPrinterUsed()
{
	return (m_bomIni->GetDataInt(_T("PRINTER"), _T("IS_USED")) == 1);
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
int CBomInfo::GetPrinterCommPort()
{
    return m_bomIni->GetDataInt(_T("PRINTER"), _T("COMM_PORT"));
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
int CBomInfo::GetPrinterBaudRate()
{
   return m_bomIni->GetDataInt(_T("PRINTER"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUpsUsed

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsUPSUsed()
{
    return (m_bomIni->GetDataInt(_T("UPS"), _T("IS_USED")) == 1);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUPS CommPort

@param       �� 

@retval      int

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetUPSCommPort()
{
	return m_bomIni->GetDataInt(_T("UPS"), _T("COMM_PORT"));
}

////////////////////////////////////////////////////////////////////////
/**
@brief       ��ȡUPS BaudRate

@param       �� 

@retval      int

@exception   ��
*/
////////////////////////////////////////////////////////////////////////
int CBomInfo::GetUPSBaudRate()
{
	return m_bomIni->GetDataInt(_T("UPS"), _T("BAUD_RATE"));
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsUPSMonitorOpened()
{
	return (m_bomIni->GetDataInt(_T("UPS"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetUPSMonitorInterval()
{
	return m_bomIni->GetDataInt(_T("UPS"),_T("MONITOR_INTERVAL"),150000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsLPTUsed()
{
		return (m_bomIni->GetDataInt(_T("LPT"),_T("IS_USED"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		�ж�TH�Ž�����Ƿ���

@param      none

@retval     bool true:����  false:�ر�

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CBomInfo::IsLPTMonitorOpened()
{
	return (m_bomIni->GetDataInt(_T("LPT"),_T("MONITOR_SWITCH"),0) == 1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetLPTMonitorInterval()
{
	return m_bomIni->GetDataInt(_T("LPT"),_T("MONITOR_INTERVAL"),1000);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ȡ���Ž��ر�ʱ��ֵ  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD CBomInfo::GetClosedDoorStatusValue()
{
	return m_bomIni->GetDataInt(_T("LPT"),_T("DOOR_STATUS_CLOSED"),127);
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
CString CBomInfo::GetBOMUSBDrive()
{
    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), _T("USBDRIVE")));
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
//CString CBomInfo::GetBOMCFDrive()
//{
//    return m_bomIni->GetDataString(_T("MOVEDISK"), "CFDRIVE");
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
CString CBomInfo::GetBomSysParaUploadPath()
{
    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), _T("BomSysParaUploadPath")));
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
//CString CBomInfo::GetBomProgramUploadPath()
//{
//    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), "BomProgramUploadPath"));
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
CString CBomInfo::GetBomMachineParaUploadPath()
{
    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), _T("BomMachineParaUploadPath")));
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
//CString CBomInfo::GetBomLogFileUploadPath()
//{
//    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), "BomLogFileUploadPath"));
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
//CString CBomInfo::GetBomDataAuditPath(){
//	return m_bomIni->GetDataString(_T("MOVEDISK"), "BomDataAudit");
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
//CString CBomInfo::GetBomDataTransactionPath(){
//	return m_bomIni->GetDataString(_T("MOVEDISK"), "BomDataTransaction");
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
//CString CBomInfo::GetBomDataEventPath(){
//	return m_bomIni->GetDataString(_T("MOVEDISK"), "BomDataEvent");
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
//CString CBomInfo::GetBomMainDBUploadPath(){
//    return m_bomIni->GetDataString(_T("MOVEDISK"), "BomMainDBPath");
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
//CString CBomInfo::GetBomBackupDBUploadPath(){
//    return m_bomIni->GetDataString(_T("MOVEDISK"), "BomBackupDBPath");
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
//CString CBomInfo::GetBomDBDownloadPath(){
//    return m_bomIni->GetDataString(_T("MOVEDISK"),"BomDBDownloadPath");
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
//CString CBomInfo::GetBomSysParaDownloadPath()
//{
//    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), "BomSysParaDownloadPath"));
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
//CString CBomInfo::GetBomProgramDownloadPath()
//{
//    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), "BomProgramDownloadPath"));
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
CString CBomInfo::GetBomMachineParaDownloadPath()
{
    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), _T("BomMachineParaDownloadPath")));
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
//CString CBomInfo::GetBomMaintentanceOutputPath()
//{
//    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), "BomMaintenance"));
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
//CString CBomInfo::GetBomMaintenceTracePath()
//{
//    return AddBackslash(m_bomIni->GetDataString(_T("MOVEDISK"), "BomMaintenanceTrace"));
//}
////////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�����ݼ�¼���������

@param      none

@retval     int

@exception  none
*/
////////////////////////////////////////////////////////////////////////////
int CBomInfo::GetStoreDay()
{
    return m_bomIni->GetDataInt(_T("LOG"), _T("StoreDay"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�����ݼ�¼ʵ�ʱ��������

@param      none

@retval     int

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetRealStoreDay()
{
	return m_bomIni->GetDataInt(_T("LOG"), _T("RealStoreDay"));
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
//int CBomInfo::GetTouchLastTime()
//{
//    return m_bomIni->GetDataInt(_T("Maintain"), "touchLastTime");
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
//int CBomInfo::GetMaxTransMsgCnt()
//{
//    return m_bomIni->GetDataInt(_T("CPS"),"MAX_TRANSACTION_MSG_CNT");
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
//int CBomInfo::GetMaxEventMsgCnt()
//{
//    return m_bomIni->GetDataInt(_T("CPS"),"MAX_EVENT_MSG_CNT");
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
//int CBomInfo::GetWaitReadCardTime()
//{
//    return m_bomIni->GetDataInt(_T("EXCHANGE_SVC"), "WAIT_READ_CARD_TIME");
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
//int CBomInfo::GetWaitQueryTime()
//{
//    return m_bomIni->GetDataInt(_T("REFUND_SVC"), "WAIT_QUERY_TIME");
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
//CString CBomInfo::GetOperationalMode(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // ���ģʽȫ����Ϊ����ģʽ����
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_bomIni->GetDataString(_T("OPERATIONAL_MODE"), sMessageID);
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
//CString CBomInfo::GetOperationalModeEn(OperationalMode_t operationalMode)
//{
//    CString sMessageID;
//    // ���ģʽȫ����Ϊ����ģʽ����
//    if( (int)operationalMode > 10){
//        operationalMode = (OperationalMode_t)10;
//    }
//    sMessageID.Format(_T("%d"), operationalMode);
//    return m_bomIni->GetDataString(_T("OPERATIONAL_MODE_EN"), sMessageID);
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
//CString CBomInfo::GetResultCodeMessage(RESULT_CODE_MSG resultCode)
//{
//    CString msg;
//    if (!m_mapResultCodeMsg.Lookup(resultCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), resultCode);
//        CString resultCodeMsg = m_bomIni->GetDataString(_T("RESULT_CODE_MSG"), sMessageID);
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
//CString CBomInfo::GetOperationMessage(OPERATION_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapOperationMsg.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_bomIni->GetDataString(_T("OPERATION_MSG"), sMessageID);
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
//CString CBomInfo::GetJourneyStatusCn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetJourneyStatusEn(JourneyStatus_t journeyStatus)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), journeyStatus);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("JOURNEY_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetTransactionTypeCn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetTransactionTypeEn(U16_t tranType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%u"), tranType);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetPassengerMessageCn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgCn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_bomIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
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
//CString CBomInfo::GetPassengerMessageEn(PASSENGER_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapPassengerMsgEn.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_bomIni->GetDataString(_T("PASSENGER_MSG"), sMessageID);
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
//CString CBomInfo::GetMaintenanceMessage(MAINTENANCE_MSG msgCode)
//{
//    CString msg;
//    if (!m_mapMaintenanceMsg.Lookup(msgCode,msg)) {
//        CString sMessageID;
//        sMessageID.Format(_T("%d"), msgCode);
//        CString resultCodeMsg = m_bomIni->GetDataString(_T("MAINTENANCE_MSG"), sMessageID);
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
//CString CBomInfo::GetPassengerTypeCn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetPassengerTypeEn(PassengerType_t type)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), type);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("PASSENGER_TYPE"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetLockStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetLockStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("LOCK_STATUS"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetCardStatusCn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("CARD_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetCardStatusEn(CardStatusCode_t status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("CARD_STATUS"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetYKTStatusCn(CARDSTATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetYKTJourneyStatusCn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetYKTJourneyStatusEn(CARD_TRANS_STATUS status)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), status);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_JOURNEY_STATUS"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetYKTSystemCodeCn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetYKTSystemCodeEn(BYTE systemCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), systemCode);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_SYSTEM_CODE"), sMessageID);
//
//    // Ӣ��
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetYKTTransTypeCn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//CString CBomInfo::GetYKTTransTypeEn(BYTE transType)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), transType);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_TRANSACTION_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgEn = _T("";
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
//CString CBomInfo::GetYKTStaffTransTypeCn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_bomIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
//
//	// ����
//	CString strMsgCn = _T("";
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
//CString CBomInfo::GetYKTStaffTransTypeEn(BYTE transType)
//{
//	CString sMessageID;
//	sMessageID.Format(_T("%d"), transType);
//	CString resultCodeMsg = m_bomIni->GetDataString(_T("YKTSTAFF_TRANSACTION_TYPE"), sMessageID);
//
//	// ����
//	CString strMsgEn = _T("";
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
//CString CBomInfo::GetYKTAgenceCodeCn(BYTE agenceCode)
//{
//    CString sMessageID;
//    sMessageID.Format(_T("%d"), agenceCode);
//    CString resultCodeMsg = m_bomIni->GetDataString(_T("YKT_AGENCECODE_TYPE"), sMessageID);
//
//    // ����
//    CString strMsgCn = _T("";
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
//void CBomInfo::SetReadMode(int num)
//{
//	CString str;
//	str.Format(_T("%d"), num);
//	m_bomIni->WriteData(_T("ICRW"), "READ_MODE"), str);
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
//void CBomInfo::SetACCAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_bomIni->WriteData(_T("APPLICATION"), "ACCAuthorizeManner"), str);
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
//void CBomInfo::SetECTAuthorizeManner(int authorizeManner)
//{
//	CString str;
//	str.Format(_T("%d"), authorizeManner);
//	m_bomIni->WriteData(_T("APPLICATION"), "ECTAuthorizeManner"), str);
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
//void CBomInfo::SetUpdateFlag(int updateFlag)
//{
//	CString str;
//	str.Format(_T("%d"), updateFlag);
//	m_bomIni->WriteData(_T("APPLICATION"), "UpdateFlag"), str);
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
//void CBomInfo::SetACCUpdateFlag(int updateFlag)
//{
//    CString str;
//    str.Format(_T("%d"), updateFlag);
//    m_bomIni->WriteData(_T("APPLICATION"), "ACCUpdateFlag"), str);
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
//void CBomInfo::SetRFIDWriteStatus(int status)
//{
//	CString str;
//	str.Format(_T("%d"), status);
//	m_bomIni ->WriteData(_T("APPLICATION"), "RFID_WRITEABLE_STATUS"), str);
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
CString CBomInfo::GetBomUsedAdapterName()
{
	return m_bomIni->GetDataString(_T("MACHINE"), _T("BOMUSED_ADAPTER_NAME"));
}


//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ������ԭ������

@param       ��

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetIncomeReasonCount()
{
	return m_bomIni->GetDataInt(_T("INCOME_REASON"),_T("COUNT"));
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ������ԭ������

@param       (i)int NO

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CBomInfo::GetIncomeReason(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	return m_bomIni->GetDataString(_T("INCOME_REASON"), strNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ��֧��ԭ������

@param       ��

@retval      int 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetExpendReasonCount()
{
	return m_bomIni->GetDataInt(_T("EXPENSE_REASON"),_T("COUNT"));
}

CString CBomInfo::GetExpendReason( int NO )
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	return m_bomIni->GetDataString(_T("EXPENSE_REASON"), strNo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief       ȡ�ò�Ʊ�������ݣ�վԱ���棩

@param       (i)int NO

@retval      CString 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
CString CBomInfo::GetUpdateSuggestOfOperator(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

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
CString CBomInfo::GetUpdateSuggestOfPassager(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

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
CString CBomInfo::GetUpdateSuggestEn(int NO)
{
	CString strNo;
	strNo.Format(_T("0x%02x"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("UPDATE_SUGGEST"), strNo);

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
CString CBomInfo::GetTransactionType(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("TRANSACTION_TYPE"), strNo);

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
CString CBomInfo::GetACCCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("ANALYSE_ACC_STATUS"), strNo);

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
CString CBomInfo::GetApplyCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("APPLY_STATE"), strNo);

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
CString CBomInfo::GetACCCardStateEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("ANALYSE_ACC_STATUS"), strNo);

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
CString CBomInfo::GetLineName(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("LINE_NAME"), strNo);

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
CString CBomInfo::GetLineNameEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("LINE_NAME"), strNo);

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
CString CBomInfo::GetECTCardState(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("ANALYSE_ECT_STATUS"), strNo);

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
CString CBomInfo::GetECTCardStateEn(int NO)
{
	CString strNo;
	strNo.Format(_T("%d"),NO);
	CString resultCodeMsg = m_bomIni->GetDataString(_T("ANALYSE_ECT_STATUS"), strNo);

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
int CBomInfo::GetTransactionPackageInterval()
{
	 return m_bomIni->GetDataInt(_T("INTERVAL"), _T("TRANSACTION_PACKAGE"),30);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ҵ�����ݴ�����ʱ�䣨��λ���룩

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetBusinessPackageInterval()
{
	return m_bomIni->GetDataInt(_T("INTERVAL"), _T("BUSINESS_PACKAGE"),30);
}
//////////////////////////////////////////////////////////////////////////
/*
@brief     ȡ�ûص�Ĭ��ҵ��ļ��ʱ�䣨��λ���룩 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetReturnDefaultServiceInterval()
{
	return m_bomIni->GetDataInt(_T("INTERVAL"), _T("RETURN_DEFAULT_SERVICE"),120);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ��ҵ���еȴ������ļ��ʱ�䣨��λ���룩

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CBomInfo::GetWaitOperationInterval()
{
	return m_bomIni->GetDataInt(_T("INTERVAL"), _T("WAIT_OPERATION"),120);
}
