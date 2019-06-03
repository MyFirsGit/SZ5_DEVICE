#include "StdAfx.h"
#include "PrinterHelper.h"
#include "Xfile.h"
#include "TVMInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const PRINT_TYPE_NAME_BALANCE				= _T("����");
static const TCHAR* const PRINT_TYPE_NAME_CH_CHANGE_INSTALL		= _T("Ӳ�������䰲װ");
static const TCHAR* const PRINT_TYPE_NAME_CH_CHANGE_UNINSTALL	= _T("Ӳ��������ж��");
static const TCHAR* const PRINT_TYPE_NAME_CH_COLLBOX_INSTALL	= _T("Ӳ�һ����䰲װ");
static const TCHAR* const PRINT_TYPE_NAME_CH_COLLBOX_UNINSTALL	= _T("Ӳ�һ�����ж��");
static const TCHAR* const PRINT_TYPE_NAME_CH_COLLECTION			= _T("Ӳ�һ���");
static const TCHAR* const PRINT_TYPE_NAME_CH_SUPPLY				= _T("Ӳ�Ҳ���");
static const TCHAR* const PRINT_TYPE_NAME_BH_INSTALL			= _T("ֽ�һ����䰲װ");
static const TCHAR* const PRINT_TYPE_NAME_BH_UNINSTALL			= _T("ֽ�һ�����ж��");
static const TCHAR* const PRINT_TYPE_NAME_BH_CHANGE_INSTALL		= _T("ֽ�������䰲װ");
static const TCHAR* const PRINT_TYPE_NAME_BH_CHANGE_UNINSTALL	= _T("ֽ��������ж��");
static const TCHAR* const PRINT_TYPE_NAME_TH_INSTALL			= _T("Ʊ�䰲װ");
static const TCHAR* const PRINT_TYPE_NAME_TH_UNINSTALL			= _T("Ʊ��ж��");
static const TCHAR* const PRINT_TYPE_NAME_ISSUE_ERROR			= _T("��Ʊ���ϵ�");
static const TCHAR* const PRINT_TYPE_NAME_CHARGE_ERROR			= _T("��ֵ���ϵ�");

CString g_szLogoPath(_T(""));
int g_logoHeight=174;
//#define UD_LOGOPATH _T("C:\\logo.bmp")
//#define UD_LOGOHEIGHT	70


/**
@brief	CPrinterHelper��
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterHelper::CPrinterHelper()
{
	CString szAppPath(_T(""));
	GetAppPath(szAppPath);
	szAppPath+=_T("\\res\\");
	g_szLogoPath=szAppPath+_T("logo_receipt.bmp");
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CPrinterHelper::~CPrinterHelper()
{
	
}

////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ӡ���Ƿ�����쳣

@param      (i)��

@retval     bool 
			true	�ɹ�
			false	ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPrinterHelper::IsPrinterHasException()
{
	bool bret=false;
	long lStatus = GetPrinterStatus();
	if(lStatus!=0)
	{
		bret=TRUE;
	}
	return bret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Ӵ�ӡ��

@param      ��

@retval     long
			0	�ɹ�
			1	ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrinterConnect()
{
	long l_lret=1;
	tPRTDevReturn pDevStatus = {};
	//TODO
	UINT port = theTVM_SETTING.GetPrinterComPort();
	UINT BaudRate = theTVM_SETTING.GetPrinterBaudRate();
	CString path = theTVM_INFO.GetLogPath();
	CString filename= CString(_T("logg.txt"));
	l_lret = PRT_CommOpen(port,BaudRate,path,filename);
	if(0 != l_lret){
		//throw CPrinterException((CPrinterException::DETAIL_ERROR_CODE)l_lret,_T(__FILE__),__LINE__);
		throw;
	}
	return l_lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ͽ���ӡ������

@param      (i)��

@retval     void

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long  CPrinterHelper::PrinterClose()
{
	long l_lret=1;
	l_lret = PRT_CommClose();
	if(0 != l_lret){
		throw; //CPrinterException((CPrinterException::DETAIL_ERROR_CODE)l_lret,_T(__FILE__),__LINE__);
	}
	return l_lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ӡ��״̬ 
			�Ƿ���ֽ 
			ѹֽ��λ�� 
			����λ���е�λ�� 
			����ֽ RAM�洢���Ƿ���ȷ

@param      (i)int nPort	���ں�
			(i)int nBaud	������

@retval     long
			0	�ɹ�
			1	ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::CheckStatus()
{
	long l_lret = 1;
	tPRTModuleStatus stu ={0};
	tPRTDevReturn devreturn = {0};
	l_lret = PRT_GetDevStatus(&stu,&devreturn);
	//if(0 != l_lret && l_lret != CPrinterException::ERROR_Printer_LESS_PAPER){
		//throw CPrinterException((CPrinterException::DETAIL_ERROR_CODE)l_lret,_T(__FILE__),__LINE__);
	if(0!=l_lret){
		throw;
	}
	return l_lret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �޸���ӡ��

@param      (i)��

@retval     bool
			true �޸��ɹ�
			false �޸�ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPrinterHelper::Repare()
{
	bool bret=true;
	try{
		PrinterClose();
		PrinterConnect();
		CheckStatus();
	}
	catch(CSysException){
		throw;
	}
	catch(...){
		throw;
	}
	return bret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��λͼ�ķ�ʽ��ӡƾ֤ ֧�ָ�������

@param      (i)CStringArray* sArray ÿ���ı����ı�����
			(i)bool bIsSaveFile	�Ƿ񱣴���ļ�
			(i)bool bIsPrintLogo	�Ƿ��ӡlogo

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrintReceiptByBmp (CStringArray* sArray, bool bIsSaveFile,bool bIsPrintLogo)
{
	return PrintReceiptByText(sArray,bIsSaveFile,bIsPrintLogo);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ı��ķ�ʽ��ӡƾ֤ ��֧����������

@param      (i)CStringArray* sArray ÿ���ı����ı�����
			(i)bool bIsSaveFile	�Ƿ񱣴���ļ�
			(i)bool bIsPrintLogo	�Ƿ��ӡlogo

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrintReceiptByText(CStringArray* sArray, bool bIsSaveFile,bool bIsPrintLogo)
{
	long lret=1;

	CString strPrintPath("");
	if(bIsSaveFile){
		strPrintPath = SaveToUnPrintDir(sArray);// ����Ҫ��ӡ�Ķ����ŵ�δ��ӡĿ¼��
	}
	else{
		strPrintPath = m_strNeedPrintPath;
	}

	// ����ӡ�ļ����ڴ�ӡ��־��ȥ
	PrinterToFile(sArray);

	// ����ӡ��״̬
	long errCode = CheckStatus();

	// ��ǰ֧�ִ�ӡ
	if(theTVM_SETTING.GetPrintMode() == 0){
		tPRTDevReturn pDevReturn = {0};
		bool m_bOpenBox = false;
		lret = PRT_Print(sArray,m_bOpenBox,true,&pDevReturn);
		if(0 != lret){
			throw;// CPrinterException((CPrinterException::DETAIL_ERROR_CODE)lret,_T(__FILE__),__LINE__);
		}
		if(lret == 0){
			// ��ӡ�ɹ����ƶ���ӡ����ʷ��¼�ƶ���History��
			MovePrintFileToHistory(strPrintPath);
		}
	}
	
	return lret;
}
 

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ӡ����Ӳ��ID

@param      (i)BYTE*

@retval     long
			0 �ɹ�
			1 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::GetPrinterID(BYTE*)
{
	long lret=1;
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ӡ����Ӳ��ID

@param      (i)no

@retval     long
			0 normal 
			1 printer in unhealth status

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::GetPrinterStatus()
{
	long l_lret = 1;
	l_lret = CheckStatus();
	if(0 != l_lret){// && l_lret != CPrinterException::ERROR_Printer_LESS_PAPER){
		throw ;//CPrinterException((CPrinterException::DETAIL_ERROR_CODE)l_lret,_T(__FILE__),__LINE__);
	}
	return l_lret;
}

BOOL CPrinterHelper::isNoPaper()
{
	BOOL bret=FALSE;
	
	long l_lret = CheckStatus();
	if(l_lret==216)
	{
		bret=TRUE;
	}

	return bret;
}

BOOL CPrinterHelper::isLessPaper()
{
	BOOL bret=FALSE;

	long l_lret = CheckStatus();
	if(l_lret==217)//CPrinterException::ERROR_Printer_LESS_PAPER)
	{
		bret=TRUE;
	}

	return bret;
}
BOOL CPrinterHelper::isRamError()
{
	BOOL bret=FALSE;

	return bret;
}
BOOL CPrinterHelper::isKnifePosError()
{
	BOOL bret=FALSE;

	long l_lret = CheckStatus();//.PRT_GetStatus();
	if(l_lret==213)//CPrinterException::ERROR_Printer_KNIFE_ERROR)
	{
		bret=TRUE;
	}
	return bret;
}
BOOL CPrinterHelper::isBlackSplitPosNotUnderKnife()
{
	BOOL bret=FALSE;

	return bret;
}
BOOL CPrinterHelper::isStickPosError()
{
	BOOL bret=FALSE;

	return bret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ�����ݱ��浽�ļ��� (���ڵ��Դ���ʹ��)

@param      (i)CStringArray* sArray	ÿ���ı����ı�����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterHelper::PrinterToFile(CStringArray* sArray)
{
	try{
		CString strFileName;

		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		strFileName.Format(_T("%sPrintHistory_%04d%02d%02d.log"),theTVM_INFO.GetLogPath(), sysTime.wYear, sysTime.wMonth, sysTime.wDay);

		CXFile sFile(strFileName);
		CString strLine;
		CString sYear = _T(""), sMonth = _T(""), sDate = _T(""), sHour= _T(""), sMinute= _T(""), sSecond=_T(""), sMilliSec=_T("");
		sYear.Format(_T("%d"), sysTime.wYear);
		sMonth.Format(_T("%d"), sysTime.wMonth);
		sDate.Format(_T("%d"), sysTime.wDay);
		sHour.Format(_T("%d"), sysTime.wHour);
		sMinute.Format(_T("%d"), sysTime.wMinute);
		sSecond.Format(_T("%d"), sysTime.wSecond);
		sMilliSec.Format(_T("%d"), sysTime.wMilliseconds);

		sFile.SeekToEnd();
		strLine = _T("Start at ") + sYear + sMonth + sDate + _T(" ") + sHour + sMinute + sSecond + _T(".") + sMilliSec + _T("\n");
		sFile.WriteFile(strLine);

		for(int i=0;i<sArray->GetCount();i++) 
		{
			sFile.SeekToEnd();
			strLine = _T("[") + sArray->GetAt(i) + _T("]") + _T("\n");
			sFile.WriteFile(strLine);
		}

		::GetLocalTime(&sysTime);
		sYear.Format(_T("%d"), sysTime.wYear);
		sMonth.Format(_T("%d"), sysTime.wMonth);
		sDate.Format(_T("%d"), sysTime.wDay);
		sHour.Format(_T("%d"), sysTime.wHour);
		sMinute.Format(_T("%d"), sysTime.wMinute);
		sSecond.Format(_T("%d"), sysTime.wSecond);
		sMilliSec.Format(_T("%d"), sysTime.wMilliseconds);

		sFile.SeekToEnd();
		strLine = _T("Finish at ") + sYear + sMonth + sDate + _T(" ") + sHour + sMinute + sSecond + _T(".") + sMilliSec + _T("\n\n");
		sFile.WriteFile(strLine);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		// 
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ӡ�����ݱ��浽��ӡ��ʷ�ļ��� (����ÿ��һ���ļ���ʽ����)

@param      (i)CStringArray* sArray	ÿ���ı����ı�����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterHelper::SaveToPrintHistoryFile(CStringArray* sArray)
{
	try{
		CString unPrintFileFullPath = theTVM_INFO.GetGetReceiptLogPath() + FILTER_AUDIT_LOG;// �ŵ�����ļ���ȥ
		CString szHistoryFolder=ComGetCurTime().ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")).Left(8);
		unPrintFileFullPath +=szHistoryFolder;
		unPrintFileFullPath +=_T("\\");
		ComCheckAndCreateDirectory(unPrintFileFullPath);//���·����������ڵ���ҵ�����ݵ��ļ��У��ʹ���
		unPrintFileFullPath = unPrintFileFullPath + GetNeedPrintFileName();//ComGetCurTime().ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d"))+_T(".txt");
		//����Ǹ�ʲô��?
		//SYNC(FILE,unPrintFileFullPath);
		CXFile cFile(unPrintFileFullPath);
		//if(cFile.Open(unPrintFileFullPath,openFlages)){
		//	cFile.SeekToEnd();
		for(int i=0;i<sArray->GetCount();i++) 
		{
			CString strLine =  sArray->GetAt(i);
			cFile.WriteFile(strLine);
		}
		//	cFile.Close();
		//}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("д���ս��ļ�ʧ�ܣ�"));
		theEXCEPTION_MGR.ProcessException(e);
	}
	return ;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��Ҫ��ӡ�����ݴ浽��ӡ��ʷ��

@param      ��i��CStringArray* sArray����Ҫ��ӡ���ı�

@retval     ��o�������ļ�������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CPrinterHelper::SaveToUnPrintDir(CStringArray* sArray){
	try{
		CString strFilePathName(""),strPrinterType("Staff");
		UINT uServerID = theSERVICE_MGR.GetCurService()->GetServiceID();
		if(RECEIPT_REPRINT_SVC == uServerID){// 
			return strFilePathName;
		}	
		else if(ISSUE_SVC == uServerID || CHARGE_SVC == uServerID || ISSUE_PRODUCT_SVC == uServerID){// �˿�ҵ��
			strPrinterType = FILTER_PASSENGER_PRINT_UNPRINT;
		}
		else{
			// վԱҵ��
			strPrinterType = FILTER_STAFF_PRINT_UNPRINT;
		}

		// D:\TVM_Data\PrintHistory\Staff\UnPrint\20150525111215.txt
		strFilePathName.Format(_T("%s%s\\"),theTVM_INFO.GetGetReceiptLogPath(),strPrinterType);
		// ��Ⲣ����Ŀ¼
		if(ComCheckAndCreateDirectory(strFilePathName)){
			strFilePathName += GetNeedPrintFileName();//ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d.txt"));

			CXFile cxFile(strFilePathName);
			for(int i=0;i<sArray->GetCount();i++) 
			{
				CString strLine =  sArray->GetAt(i);
				cxFile.WriteFile(strLine);
			}
			cxFile.Close();
		}

		return strFilePathName;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ƶ���ӡ��¼����ӡ��ʷ

@param      (i)CString strFilePathName;(��Ҫ�ƶ����ļ�)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterHelper::MovePrintFileToHistory(CString strFilePathName){
	CString strPathName,strFileName;
	if(!strFilePathName.IsEmpty()){
		//D:\TVM_Data\PrintHistory\Staff\ 
		strPathName = strFilePathName.Left(strFilePathName.ReverseFind(_T('\\')));
		//strPathName += _T("History\\");
		strPathName.Replace(_T("UnPrint"),_T("History\\"));

		strFileName = strFilePathName.Right(strFilePathName.GetLength() - strFilePathName.ReverseFind(_T('\\')) - 1);
		// Check directory existing and create it.
		if(ComCheckAndCreateDirectory(strPathName)){
			// No matter it existing,replace it .
			::MoveFileEx(strFilePathName,strPathName + strFileName,MOVEFILE_REPLACE_EXISTING);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ӡ���ӡ�����ϲ�����δ��ӡ��ƾ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrintLastUnprintReceipt(){
	// ��ӡ�˿ͼ���ά������δ��ӡ��ƾ֤
	if(PrintLastUnprintFromPath(FILTER_STAFF_PRINT_UNPRINT) && PrintLastUnprintFromPath(FILTER_PASSENGER_PRINT_UNPRINT)){
		return 0;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡ��Ҫ�����ļ�������

@param      ��

@retval     CString strFileName; 20160227180102_101028_Ʊ�䰲װ.TXT

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CPrinterHelper::GetNeedPrintFileName(){
	CString strFileName(_T("")),strOperaName(_T(""));

	switch(theAPP_SESSION.GetReceiptPrintType()){
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_BALANCE:
		{
			strOperaName = PRINT_TYPE_NAME_BALANCE;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_INSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_CH_CHANGE_INSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_UNINSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_CH_CHANGE_UNINSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_INSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_CH_COLLBOX_INSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_UNINSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_CH_COLLBOX_UNINSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLECTION:
		{
			strOperaName = PRINT_TYPE_NAME_CH_COLLECTION ;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_SUPPLY:
		{
			strOperaName = PRINT_TYPE_NAME_CH_SUPPLY;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_INSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_BH_INSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_UNINSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_BH_UNINSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_INSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_BH_CHANGE_INSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_UNINSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_BH_CHANGE_UNINSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_INSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_TH_INSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_UNINSTALL:
		{
			strOperaName = PRINT_TYPE_NAME_TH_UNINSTALL;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_ISSUE_ERROR:
		{
			strOperaName = PRINT_TYPE_NAME_ISSUE_ERROR;
		}
		break;
	case RECEIPT_PRINT_TYPE::PRINT_TYPE_CHARGE_ERROR:
		{
			strOperaName = PRINT_TYPE_NAME_CHARGE_ERROR;
		}
		break;
	default:
		break;
	}

	strFileName.Format(_T("%s_%06s_%s.TXT"),ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d")),theAPP_SESSION.GetUserInfo().sUserID,strOperaName);
	
	return strFileName;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���ݴ�ӡ���ɵ��ļ����ƻ�ȡ��ӡ����

@param      const CString& strFileName

@retval     RECEIPT_PRINT_TYPE printType;

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
RECEIPT_PRINT_TYPE CPrinterHelper::GetPrintTypeFromFileName(const CString& strFileName){

	if(strFileName.Find(PRINT_TYPE_NAME_BALANCE) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_BALANCE;
	if(strFileName.Find(PRINT_TYPE_NAME_BH_CHANGE_INSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_INSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_BH_CHANGE_UNINSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_CHANGE_UNINSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_BH_INSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_INSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_BH_UNINSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_BH_UNINSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_CH_CHANGE_INSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_INSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_CH_CHANGE_UNINSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_CHANGE_UNINSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_CH_COLLBOX_INSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_INSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_CH_COLLBOX_UNINSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLBOX_UNINSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_CH_COLLECTION) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_COLLECTION;
	if(strFileName.Find(PRINT_TYPE_NAME_CH_SUPPLY) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CH_SUPPLY;
	if(strFileName.Find(PRINT_TYPE_NAME_CHARGE_ERROR) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_CHARGE_ERROR;
	if(strFileName.Find(PRINT_TYPE_NAME_ISSUE_ERROR) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_ISSUE_ERROR;
	if(strFileName.Find(PRINT_TYPE_NAME_TH_INSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_INSTALL;
	if(strFileName.Find(PRINT_TYPE_NAME_TH_UNINSTALL) != -1)
		return RECEIPT_PRINT_TYPE::PRINT_TYPE_TH_UNINSTALL;
	
	return RECEIPT_PRINT_TYPE::PRINT_TYPE_NONE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ӡָ��Ŀ¼��Ϊ��ӡ��ƾ֤�ļ�

@param      CString strFilePath(ָ��Ŀ¼)

@retval     BOOL TRUE(�ɹ�)��FALSE(ʧ��)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPrinterHelper::PrintLastUnprintFromPath(CString strPath){
	// Get Staff UnPrint Log 
	CString strFilePath = theTVM_INFO.GetGetReceiptLogPath() + strPath;
	vector<CString>		vecData;
	map<CString,CString> mapFile;
	vecData.clear();
	mapFile.clear();

	CFileFind finder;
	bool bIsFind = finder.FindFile(strFilePath + _T("\\*.txt"));
	while(bIsFind){
		bIsFind = finder.FindNextFile();
		if(finder.IsDots() || finder.IsDirectory()){
			continue;
		}
		else {
			CString strFile = finder.GetFilePath();
			CString strFileName = finder.GetFileName();

			vecData.push_back(strFileName.Right(18).Left(14));//20150526101122
			mapFile[strFileName.Right(18).Left(14)] = strFile;
		}
	}
	finder.Close();

	if(vecData.size() == 0){
		return TRUE;
	}

	int nCount = theFunction_INFO.GetPrintLastReceiptCount();
	if(vecData.size() <= nCount){
		// Do Print 
		for(auto index = begin(mapFile);index != end(mapFile);index++){
			m_strNeedPrintPath = index->second;

			if(!m_strNeedPrintPath.IsEmpty()){

				CXFile cxFile(m_strNeedPrintPath);
				CStringArray csArr;
				CString strLine;

				while(cxFile.ReadString(strLine)){
					LPTSTR lStr = strLine.GetBuffer(0);
					int nSize   = strLine.GetLength();
					if(nSize != 0 && lStr[nSize -1] == '\r'){
						strLine.ReleaseBufferSetLength(nSize - 1);
					}
					ComPrintLineFormatForTemplate(strLine,csArr);
				}
				cxFile.Close();
				// To Print
				PrintReceiptByBmp(&csArr,false,true);
				csArr.RemoveAll();
				strLine.ReleaseBuffer();
			}
		}
	}
	else{
		// Do copy other to History and print last Max count of receipt .
		// Sequence(From Big to Small)
		sort(vecData.begin(),vecData.end(),greater<CString>());

		// Move file to history Dir
		for(int i = 0; i < vecData.size();i++){

			if(i < nCount){
				// Jump out the last receipt file.
			}
			else{
				// Do move file 
				auto thisFile = mapFile.find(vecData[i]);
				if(thisFile != mapFile.end())	{
					MovePrintFileToHistory(thisFile->second);
					thisFile->second = _T("");// Set it Empty
				}
			}
		}
		// Do Print 
		for(auto fileName = begin(mapFile); fileName != end(mapFile); fileName++){
			if(!fileName->second.IsEmpty()){
				m_strNeedPrintPath = fileName->second;

				CXFile cxFile(m_strNeedPrintPath);
				CStringArray csArr;
				CString strLine;

				while(cxFile.ReadString(strLine)){
					LPTSTR lStr = strLine.GetBuffer(0);
					int nSize   = strLine.GetLength();
					if(nSize != 0 && lStr[nSize -1] == '\r'){
						strLine.ReleaseBufferSetLength(nSize - 1);
					}
					ComPrintLineFormatForTemplate(strLine,csArr);
				}
				cxFile.Close();
				// To Print
				PrintReceiptByText(&csArr,false,true);
				csArr.RemoveAll();
				strLine.ReleaseBuffer();
			}
		}
	}
	return TRUE;
}