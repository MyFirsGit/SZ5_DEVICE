#include "StdAfx.h"
#include "PrinterHelper.h"
#include "Xfile.h"
#include "TVMInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const PRINT_TYPE_NAME_BALANCE				= _T("结算");
static const TCHAR* const PRINT_TYPE_NAME_CH_CHANGE_INSTALL		= _T("硬币找零箱安装");
static const TCHAR* const PRINT_TYPE_NAME_CH_CHANGE_UNINSTALL	= _T("硬币找零箱卸下");
static const TCHAR* const PRINT_TYPE_NAME_CH_COLLBOX_INSTALL	= _T("硬币回收箱安装");
static const TCHAR* const PRINT_TYPE_NAME_CH_COLLBOX_UNINSTALL	= _T("硬币回收箱卸下");
static const TCHAR* const PRINT_TYPE_NAME_CH_COLLECTION			= _T("硬币回收");
static const TCHAR* const PRINT_TYPE_NAME_CH_SUPPLY				= _T("硬币补充");
static const TCHAR* const PRINT_TYPE_NAME_BH_INSTALL			= _T("纸币回收箱安装");
static const TCHAR* const PRINT_TYPE_NAME_BH_UNINSTALL			= _T("纸币回收箱卸下");
static const TCHAR* const PRINT_TYPE_NAME_BH_CHANGE_INSTALL		= _T("纸币找零箱安装");
static const TCHAR* const PRINT_TYPE_NAME_BH_CHANGE_UNINSTALL	= _T("纸币找零箱卸下");
static const TCHAR* const PRINT_TYPE_NAME_TH_INSTALL			= _T("票箱安装");
static const TCHAR* const PRINT_TYPE_NAME_TH_UNINSTALL			= _T("票箱卸下");
static const TCHAR* const PRINT_TYPE_NAME_ISSUE_ERROR			= _T("售票故障单");
static const TCHAR* const PRINT_TYPE_NAME_CHARGE_ERROR			= _T("充值故障单");

CString g_szLogoPath(_T(""));
int g_logoHeight=174;
//#define UD_LOGOPATH _T("C:\\logo.bmp")
//#define UD_LOGOHEIGHT	70


/**
@brief	CPrinterHelper类
*/

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPrinterHelper::~CPrinterHelper()
{
	
}

////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测打印机是否存在异常

@param      (i)无

@retval     bool 
			true	成功
			false	失败

@exception  无
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
@brief      连接打印机

@param      无

@retval     long
			0	成功
			1	失败

@exception  无
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
@brief      断开打印机连接

@param      (i)无

@retval     void

@exception  无
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
@brief      检测打印机状态 
			是否少纸 
			压纸棒位置 
			黑条位置切刀位置 
			有无纸 RAM存储器是否正确

@param      (i)int nPort	串口号
			(i)int nBaud	波特率

@retval     long
			0	成功
			1	失败

@exception  无
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
@brief      修复打印机

@param      (i)无

@retval     bool
			true 修复成功
			false 修复失败

@exception  无
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
@brief      以位图的方式打印凭证 支持各种语言

@param      (i)CStringArray* sArray 每行文本的文本数组
			(i)bool bIsSaveFile	是否保存成文件
			(i)bool bIsPrintLogo	是否打印logo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrintReceiptByBmp (CStringArray* sArray, bool bIsSaveFile,bool bIsPrintLogo)
{
	return PrintReceiptByText(sArray,bIsSaveFile,bIsPrintLogo);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      以文本的方式打印凭证 不支持葡萄牙文

@param      (i)CStringArray* sArray 每行文本的文本数组
			(i)bool bIsSaveFile	是否保存成文件
			(i)bool bIsPrintLogo	是否打印logo

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrintReceiptByText(CStringArray* sArray, bool bIsSaveFile,bool bIsPrintLogo)
{
	long lret=1;

	CString strPrintPath("");
	if(bIsSaveFile){
		strPrintPath = SaveToUnPrintDir(sArray);// 将需要打印的东西放到未打印目录下
	}
	else{
		strPrintPath = m_strNeedPrintPath;
	}

	// 将打印文件放在打印日志中去
	PrinterToFile(sArray);

	// 检查打印机状态
	long errCode = CheckStatus();

	// 当前支持打印
	if(theTVM_SETTING.GetPrintMode() == 0){
		tPRTDevReturn pDevReturn = {0};
		bool m_bOpenBox = false;
		lret = PRT_Print(sArray,m_bOpenBox,true,&pDevReturn);
		if(0 != lret){
			throw;// CPrinterException((CPrinterException::DETAIL_ERROR_CODE)lret,_T(__FILE__),__LINE__);
		}
		if(lret == 0){
			// 打印成功，移动打印的历史记录移动到History中
			MovePrintFileToHistory(strPrintPath);
		}
	}
	
	return lret;
}
 

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取打印机的硬件ID

@param      (i)BYTE*

@retval     long
			0 成功
			1 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::GetPrinterID(BYTE*)
{
	long lret=1;
	return lret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取打印机的硬件ID

@param      (i)no

@retval     long
			0 normal 
			1 printer in unhealth status

@exception  无
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
@brief      打印的内容保存到文件中 (用于调试代码使用)

@param      (i)CStringArray* sArray	每行文本的文本数组

@retval     无

@exception  无
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
@brief      将打印的内容保存到打印历史文件夹 (按照每天一个文件方式保存)

@param      (i)CStringArray* sArray	每行文本的文本数组

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPrinterHelper::SaveToPrintHistoryFile(CStringArray* sArray)
{
	try{
		CString unPrintFileFullPath = theTVM_INFO.GetGetReceiptLogPath() + FILTER_AUDIT_LOG;// 放到审计文件中去
		CString szHistoryFolder=ComGetCurTime().ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")).Left(8);
		unPrintFileFullPath +=szHistoryFolder;
		unPrintFileFullPath +=_T("\\");
		ComCheckAndCreateDirectory(unPrintFileFullPath);//检查路径如果不存在当天业结数据的文件夹，就创建
		unPrintFileFullPath = unPrintFileFullPath + GetNeedPrintFileName();//ComGetCurTime().ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d"))+_T(".txt");
		//这个是干什么的?
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
		CInnerException e = CInnerException(CInnerException::MODULE_ID,CInnerException::OTHER_ERR, _T(__FILE__), __LINE__, _T("写入日结文件失败！"));
		theEXCEPTION_MGR.ProcessException(e);
	}
	return ;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      将要打印的内容存到打印历史中

@param      （i）CStringArray* sArray；需要打印的文本

@retval     （o）保存文件的名称

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CPrinterHelper::SaveToUnPrintDir(CStringArray* sArray){
	try{
		CString strFilePathName(""),strPrinterType("Staff");
		UINT uServerID = theSERVICE_MGR.GetCurService()->GetServiceID();
		if(RECEIPT_REPRINT_SVC == uServerID){// 
			return strFilePathName;
		}	
		else if(ISSUE_SVC == uServerID || CHARGE_SVC == uServerID || ISSUE_PRODUCT_SVC == uServerID){// 乘客业务
			strPrinterType = FILTER_PASSENGER_PRINT_UNPRINT;
		}
		else{
			// 站员业务
			strPrinterType = FILTER_STAFF_PRINT_UNPRINT;
		}

		// D:\TVM_Data\PrintHistory\Staff\UnPrint\20150525111215.txt
		strFilePathName.Format(_T("%s%s\\"),theTVM_INFO.GetGetReceiptLogPath(),strPrinterType);
		// 检测并创建目录
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
@brief      移动打印记录到打印历史

@param      (i)CString strFilePathName;(需要移动的文件)

@retval     无

@exception  无
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
@brief      打印因打印机故障产生的未打印的凭条

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPrinterHelper::PrintLastUnprintReceipt(){
	// 打印乘客及后维护界面未打印的凭证
	if(PrintLastUnprintFromPath(FILTER_STAFF_PRINT_UNPRINT) && PrintLastUnprintFromPath(FILTER_PASSENGER_PRINT_UNPRINT)){
		return 0;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取需要生成文件的名称

@param      无

@retval     CString strFileName; 20160227180102_101028_票箱安装.TXT

@exception  无
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
@brief      根据打印生成的文件名称获取打印类型

@param      const CString& strFileName

@retval     RECEIPT_PRINT_TYPE printType;

@exception  无
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
@brief      打印指定目录下为打印的凭证文件

@param      CString strFilePath(指定目录)

@retval     BOOL TRUE(成功)；FALSE(失败)

@exception  无
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