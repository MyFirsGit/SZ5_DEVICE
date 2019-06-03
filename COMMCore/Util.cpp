/******************************************************************************/
/**	@class CCTIMLIBApp
*	@brief Main implementation file for the CCTIMLIBApp application.
*	See CTIMLIBApp.cpp for the implementation of this class
*	@version 0.01 (2004-10-01): The first version that generate by VS6
*/
/******************************************************************************/

#include "stdafx.h"
#include "Util.h"

#include "winspool.h"
#include "string.h"
#include <fstream>
#include <Limits.h>
#include "cverify.h"
#include <shlwapi.h>
#include <vector>
#include <Tlhelp32.h>
#include "BR.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCTIMLIBApp



// CCTIMLIBApp construction

// 打印用纸一行最多输出的半角字符数
static const short PRINT_MAX_WIDTH = 50;

//////////////////////////////////////////////////////////////////////////
/*
@brief   十六进制转十进制   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Hex2Dec(int hexNum)
{
	CString s;
	s.Format(_T("%x"),hexNum);
	return wcstol(s.GetBuffer(),NULL,10);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    十进制转十六进制  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Dec2Hex(int decNum)
{
	CString s;
	s.Format(_T("%d"),decNum);
	return  _tcstol(s.GetBuffer(),NULL,16);

}

/******************************************************************************/
/**	@brief Kill Application
*	@date 2004-10-02 (K.Tuyen): Create and update comment.
*/
/******************************************************************************/

void ComKillApp()
{
	/*If terminate abnormal, then don't call IDF_ATOSYO */
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0,0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取当前应用程序的路径

@param      (o)  LPTSTR lpszBuf            路径名的空间
@param      (i)  DWORD ucchMax             最大的路径长度

@retval     INT   \n
            E_INVALID_PARAMS:参数无效, E_WIN32_ERROR:系统错误, 其他:路径的实际长度

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
INT ComGetAppPath(IN OUT LPTSTR lpszBuf, IN DWORD ucchMax)
{
	DWORD dwLen;
	TCHAR szStore[MAX_PATH + 1];   /* + 1 for zero character */

	/* Check the sanity of all input parameters */
	if (lpszBuf != NULL)
	{
		_ASSERTE(ucchMax > 0);
		ASSERT_ISWRITEPOINTER(lpszBuf, ucchMax * sizeof(TCHAR));
		if ((0 == ucchMax) || IsBadWritePtr(lpszBuf, ucchMax * sizeof(TCHAR)))
		{
			return E_INVALID_PARAMS;
		}
	}

	WIN32CHECK(dwLen = GetModuleFileName(NULL, szStore, MAX_PATH));
	if (0 == dwLen)
	{
		return E_WIN32_ERROR;
	}
	szStore[MAX_PATH] = _T('\0');

	WIN32CHECK(PathRemoveFileSpec(szStore));
	WIN32CHECK(PathAddBackslash(szStore) != NULL);

	/* The request buffer size return is len of path and +1 for null character */
	dwLen = lstrlen(szStore) + 1;
	if (lpszBuf != NULL)
		lstrcpyn(lpszBuf, szStore, ucchMax);

	return dwLen;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查文件是否存在 CREATED BY NIJUN

@param      (i)  LPCTSTR lpszFileName        文件名

@retval     BOOL   \n
            TRUE:存在, FALSE:不存在

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckFileExist(LPCTSTR lpszFileName)
{
	if( NULL == lpszFileName)
	{
		return FALSE;
	}

	if (PathFileExists(lpszFileName))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查路径是否存在 CREATED BY NIJUN

@param      (i)  LPCTSTR lpszDirName        路径名

@retval     bool   \n
            true:存在, false:不存在

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckDirectory(LPCTSTR lpszDirName)
{
    if (PathIsDirectory(lpszDirName)) {
        return true;
    }
    else {
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查并创建路径

@param      (i) LPCTSTR lpszDirName        路径名

@retval     bool   true:成功, false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckAndCreateDirectory(LPCTSTR lpszDirName)
{
    if (PathIsDirectory(lpszDirName)) {
        return true;
    }
    else {
		int nRet = SHCreateDirectoryEx(NULL, lpszDirName, NULL);
        if ( nRet == ERROR_SUCCESS || nRet == ERROR_ALREADY_EXISTS) {
            return true;
        }
        else {
            return false;
        }
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查字符串是否只有字母和数字

@param      (i)  LPCTSTR lpszText        字符串指针
@param      (i)  USHORT usTextLen        字符串长度

@retval     BOOL   \n
            TRUE:只有字母和数字, FALSE:不是只有字母和数字

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckAlphaNumericText(IN LPCTSTR lpszText, IN USHORT usTextLen)
{
	_ASSERTE(lpszText != NULL);
	ASSERT_ISREADPOINTER(lpszText, usTextLen * sizeof(TCHAR));

	TCHAR ch;
	int cbLen = 0;          /* Len in bytes of a character */
	UINT uiPos = 0;

	while (uiPos < usTextLen)
	{
		cbLen = (int)_tclen(lpszText);
		ch = *lpszText;

		if ((2 == cbLen) ||
			(((_T('a') > ch) || (_T('z') < ch)) &&
			((_T('A') > ch) || (_T('Z') < ch)) &&
			((_T('0') > ch) || (_T('9') < ch))))
			return FALSE;

		uiPos = uiPos + cbLen;
		lpszText = (LPCTSTR) ((LPBYTE) lpszText + cbLen);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      改变路径

@param      (i)  CString sPath           原来的全路径
@param      (i)  CString sOrgPath        变更前的前面部分路径
@param      (i)  CString sDestDir        变更后的前面部分路径
@param      (o)  CString &sDest          变更后的全路径

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void ComGetDestDir(CString sPath, CString sOrgPath, CString sDestDir, CString &sDest)
{
	sDest = sPath;
	sDest.TrimLeft('/');
	sDest.TrimLeft('\\');
	int nPos = sOrgPath.GetLength();
	sDest =  sDest.Mid(nPos);
	sDest = sDestDir + sDest;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得当前路径的上级路径

@param      (i)  LPCTSTR lpszDirName          当前的路径
@param      (o)  CString &sCurrentDir         当前路径的前一级路经

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void ComSetCurrentDir(LPCTSTR lpszDirName, CString &sCurrentDir)
{
	CString sDir = lpszDirName;
	int iPos = sDir.ReverseFind ( '\\' ) ;
	sCurrentDir = sDir.Left ( iPos + 1 ) ;
	sCurrentDir.TrimRight('\\');
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否是闰年

@param      (i)  int iYear       年

@retval     bool   \n
            true:闰年,  false:不是闰年

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool IsLeapYear(int iYear)
{
	return ((iYear % 4 == 0 && iYear % 100 != 0) || iYear % 400 == 0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获得某月的实际天数

@param      (i)  int month       月
@param      (i)  int year        年

@retval     int  当月的天数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int DaysInMonth(int month, int year)
{
	int numOfDays;
	if ((month==1)  ||(month==3) || (month==5) || (month==7) ||
		(month==8)|| (month==10) || (month==12))
		numOfDays=31;
	else if ((month==4)||(month==6)||(month==9)||(month==11))
		numOfDays=30;
	else{
		if (IsLeapYear(year))
			numOfDays=29;
		else
			numOfDays=28;
	}
	return numOfDays;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测当前时间是否有效

@param      (i)  CString sYear        年
@param      (i)  CString sMonth       月
@param      (i)  CString sDay         日
@param      (i)  CString sHour        时
@param      (i)  CString sMinute      分
@param      (i)  CString sSecond      秒

@retval     BYTE   \n
            0：没有错误  1：年有错误   2：月有错误 3：日有错误
            4：时有错误  5：分有错误   6：秒有错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDate(CString sYear, CString sMonth, CString sDay,CString sHour, CString sMinute, CString sSecond)
{
	int iRet;
	int iYear;
	int iMonth;
	int iDay;
	int iHour;
	int iMinute;
	int iSecond;
    BYTE errorCode = 0;
	CString sDate;

	/*check whether year is valid or not*/
	if(sYear.Trim().CompareNoCase(_T("")) == 0){
		return 1;
	}

	if(sMonth.Trim().CompareNoCase(_T("")) == 0){
		return 2;
	}

	/*check whether day is valid or not */
	if(sDay.Trim().CompareNoCase(_T("")) == 0){
		return 3;
	}

	/*check whether hour is valid or not */
	if(sHour.Trim().CompareNoCase(_T("")) == 0){
		return 4;
	}

	/*check whether minute is valid or not */
	if(sMinute.Trim().CompareNoCase(_T("")) == 0){
		return 5;
	}
	/*check whether second is valid or not */
	if(sSecond.Trim().CompareNoCase(_T("")) == 0){
		return 6;
	}

	sDate.Format(_T("%s/%s/%s %s:%s:%s"),sYear,sMonth,sDay,sHour,sMinute,sSecond);

	_stscanf(sDate, _T("%d/%d/%d %d:%d:%d"), &iYear,&iMonth,&iDay, &iHour, &iMinute, &iSecond);

	/*check whether year is valid or not*/
	if(iYear < 1970){
		return 1;
	}

	if(iYear > 2141){
		return 1;
	}

	COleDateTime dDateTime;
	iRet = dDateTime.ParseDateTime(sDate);
	if (dDateTime.GetStatus()==COleDateTime::valid)
	{
		return 0;
	}
	else
	{
		/*check whether year is valid or not*/
		if(iYear <= 0){
			return 1;
		}
		if(iMonth <= 0 || iMonth > 12){
			return 2;
		}
		/*check whether day is valid or not */
		if((iDay <= 0) || (iDay > DaysInMonth(iMonth, iYear))){
			return 3;
		}
		/*check whether hour is valid or not */
		if (iHour < 0 || iHour > 23)
		{
			return 4;
		}
		/*check whether minute is valid or not */
		if (iMinute < 0 || iMinute > 59)
		{
			return 5;
		}
		/*check whether second is valid or not */
		if (iSecond < 0 || iSecond > 59)
		{
			return 6;
		}
        return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测当前时间是否有效

@param      (i)  CString sYear        年
@param      (i)  CString sMonth       月
@param      (i)  CString sDay         日
@param      (i)  CString sHour        时
@param      (i)  CString sMinute      分
@param      (i)  CString sSecond      秒

@retval     BYTE   \n
0：没有错误  1：年有错误   2：月有错误 3：日有错误
4：时有错误  5：分有错误   6：秒有错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateTimeValidSub(int iYear, int iMonth, int iDay,int iHour, int iMinute, int iSecond)
{
	int iRet;
	BYTE errorCode = 0;
	CString sDate;

	sDate.Format(_T("%d/%d/%d %d:%d:%d"),iYear,iMonth,iDay,iHour,iMinute,iSecond);

	/*check whether year is valid or not*/
	if(iYear < 1970){
		return 1;
	}

	if(iYear > 2141){
		return 1;
	}

	COleDateTime dDateTime;
	iRet = dDateTime.ParseDateTime(sDate);
	if (dDateTime.GetStatus()==COleDateTime::valid)
	{
		return 0;
	}
	else
	{
		/*check whether year is valid or not*/
		if(iYear <= 0){
			return 1;
		}
		if(iMonth <= 0 || iMonth > 12){
			return 2;
		}
		/*check whether day is valid or not */
		if((iDay <= 0) || (iDay > DaysInMonth(iMonth, iYear))){
			return 3;
		}
		/*check whether hour is valid or not */
		if (iHour < 0 || iHour > 23)
		{
			return 4;
		}
		/*check whether minute is valid or not */
		if (iMinute < 0 || iMinute > 59)
		{
			return 5;
		}
		/*check whether second is valid or not */
		if (iSecond < 0 || iSecond > 59)
		{
			return 6;
		}
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测当前时间是否有效

@param      (i)  _DATE_TIME 

@retval     BYTE   \n
0：没有错误  1：年有错误   2：月有错误 3：日有错误
4：时有错误  5：分有错误   6：秒有错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateTimeValid(_DATE_TIME dateTime)
{
    return ComCheckDateTimeValidSub(dateTime.wYear,dateTime.biMonth,dateTime.biDay,dateTime.biHour,dateTime.biMinute,dateTime.biSecond);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测当前日期是否有效

@param      (i)  _DATE 

@retval     BYTE   \n
0：没有错误  1：年有错误   2：月有错误 3：日有错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateValid(_DATE dateTime)
{
	return ComCheckDateValidSub(dateTime.wYear,dateTime.biMonth,dateTime.biDay);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      检测当前日期是否有效

@param      (i)  int sYear        年
@param      (i)  int sMonth       月
@param      (i)  int sDay         日

@retval     BYTE   \n
0：没有错误  1：年有错误   2：月有错误 3：日有错误
4：时有错误  5：分有错误   6：秒有错误

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE ComCheckDateValidSub(int iYear, int iMonth, int iDay)
{
	int iRet;
	BYTE errorCode = 0;
	CString sDate;

	sDate.Format(_T("%d/%d/%d 00:00:00"),iYear,iMonth,iDay);

	/*check whether year is valid or not*/
	if(iYear < 1970){
		return 1;
	}

	if(iYear > 2141){
		return 1;
	}

	COleDateTime dDateTime;
	iRet = dDateTime.ParseDateTime(sDate);
	if (dDateTime.GetStatus()==COleDateTime::valid)
	{
		return 0;
	}
	else
	{
		/*check whether year is valid or not*/
		if(iYear <= 0){
			return 1;
		}
		if(iMonth <= 0 || iMonth > 12){
			return 2;
		}
		/*check whether day is valid or not */
		if((iDay <= 0) || (iDay > DaysInMonth(iMonth, iYear))){
			return 3;
		}
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测IP地址是否为空

@param      (i)  CString sIP1         IP地址1
@param      (i)  CString sIP2         IP地址2
@param      (i)  CString sIP3         IP地址3
@param      (i)  CString sIP4         IP地址4
@param      (o)  int *iErr            错误代码

@retval     BOOL   \n
            TRUE:全部IP不为空,  FALSE:至少有一个为空

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComCheckIPEmpty(CString sIP1, CString sIP2, 
					 CString sIP3, CString sIP4, int *iErr)
{
	if(0 == sIP1.Compare(_T(""))){
		*iErr = 1;
		return FALSE;
	}

	if(0 == sIP2.Compare(_T(""))){
		*iErr = 2;
		return FALSE;
	}

	if(0 == sIP3.Compare(_T(""))){
		*iErr = 3;
		return FALSE;
	}

	if(0 == sIP4.Compare(_T(""))){
		*iErr = 4;
		return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      运行一个应用程序

@param      (i)  LPCTSTR lpszAppPath           应用程序路径和名称
@param      (i)  LPCTSTR lpszAppParameter      应用程序参数

@retval     BOOL   \n
            TRUE:运行成功,  FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComExecuteApp(LPCTSTR lpszAppPath, LPCTSTR lpszAppParameter)
{

	_ASSERTE(lpszAppPath != NULL);

	SHELLEXECUTEINFO shi;
	memset(&shi, 0, sizeof(shi));

	shi.cbSize       = sizeof(shi);
	shi.fMask        = SEE_MASK_NOCLOSEPROCESS;                
	shi.hwnd         = 0;                
	shi.lpVerb       = _T("open");                  /* Operation to perform */
	shi.lpFile       = lpszAppPath;				/* Application name */
	shi.lpParameters = lpszAppParameter;			/* Additional parameters */
	shi.lpDirectory  = 0;                       /* Default directory */
	shi.nShow        = SW_SHOWNORMAL;
	shi.hInstApp     = 0;
	BOOL bSuccess = ShellExecuteEx(&shi);

	if (TRUE == bSuccess)
	{
#ifdef Khanh	
		WaitForInputIdle(shi.hProcess, INFINITE);
		/* Check the process created successed */
		DWORD dwExitCode = STILL_ACTIVE;
		GetExitCodeProcess(shi.hProcess, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
		{
			return  FALSE;
		}
#endif	
		/* Close the process handle of process to avoid memory leak */
		WIN32CHECK(CloseHandle(shi.hProcess));
	}

	return bSuccess;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   根据进程名字获得进程ID   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
DWORD   GetProcessIdFromName(LPCTSTR   name)
{
	PROCESSENTRY32   pe;   
	DWORD   id   =   0;   
	HANDLE   hSnapshot   =   CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);   
	pe.dwSize   =   sizeof(PROCESSENTRY32);   
	if(   !Process32First(hSnapshot,&pe)   )  
	{
		CloseHandle(hSnapshot);
		return   0;  
	} 
	do   
	{   
		pe.dwSize   =   sizeof(PROCESSENTRY32);   
		if(Process32Next(hSnapshot,&pe)==FALSE   )   
			break;   
		if(wcscmp(pe.szExeFile,name)   ==   0)   
		{   
			id   =   pe.th32ProcessID;   
			break;   
		}   

	}   while(1);   
	CloseHandle(hSnapshot);   
	return   id;  
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  杀死指定名字的进程    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComKillAppEx(LPCTSTR lpszBuf)
{
	DWORD processID = GetProcessIdFromName(lpszBuf);
	if(processID<=0)
	{
		return FALSE;
	}
	HANDLE processHandle = OpenProcess(PROCESS_TERMINATE,FALSE,processID);
	TerminateProcess(processHandle,1);
	CloseHandle(processHandle);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断某位是否为1

@param      (i)  WORD value        目标
@param      (i)  int iBitNumber    查询的位数，0-15

@retval     bool \n
            false:该位为0,  true:该位为1

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool ComIsBitSet(WORD value, int iBitNumber)
{
    if ((value & (1 << iBitNumber)) == 0) {
        return false;
    }
    else {
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到文件的后缀名

@param      (i)  LPCTSTR lpszFileName       文件名

@retval     CString   后缀名，如果没有，则返回空

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString ComGetFileExtention(LPCTSTR lpszFileName)
{
	CString sRet;
	CString sFileName = lpszFileName;
	if(sFileName.IsEmpty())
	{
		return _T("");
	}
	int ipos = sFileName.ReverseFind ( '.' ) ;
	if(ipos == -1)
	{
		sRet =_T("");
	}
	else
	{
		sRet = sFileName.Mid( ipos + 1 ) ;
	}
	
	return sRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将字符串装换为16进制的字符串

@param      (i) LPTSTR lpszValue       需转换的字符串
@param      (i) LPTSTR lpszHex         转换后的内容

@retval     BOOL   \n
            TRUE:成功,  FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL ComStringToHex(LPTSTR lpszValue, LPTSTR lpszHex)

{

	BOOL bRet = TRUE;

	CString strTemp =_T("");

	CString strHex = _T("");

	CString strValue = lpszValue;

	LPTSTR lpszTemp = NULL;

	INT nLen = 0;

	_ASSERTE(lpszValue != NULL);

	_ASSERTE(lpszHex != NULL);

	if (lpszValue == NULL || lpszHex == NULL)

	{

		return FALSE;

	}
	
	/* reset value	*/
	lpszHex[0] = _T('\0');

	nLen = strValue.GetLength();

	if (nLen <= 0)

	{

		return FALSE;

	}

	for (int i = 0; i < nLen; i++)

	{

		strTemp.Format(_T("%02X"), strValue[i]);

		strHex = strHex + strTemp;

	}

	_tcscpy(lpszHex, strHex);

	lpszHex[strHex.GetLength() * sizeof(TCHAR)] = _T('\0');

	return bRet;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将时间转换为time_t型

@param      (i) int iYear            年
@param      (i) int iMonth           月
@param      (i) int iDay             日
@param      (i) int iHour            时
@param      (i) int iMinute          分
@param      (i) int iSecond          秒

@retval     time_t  时间

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
time_t CreateTime(int iYear, int iMonth, int iDay, int iHour, int iMinute, int iSecond )
{
	CString sYear;
	CString sMonth;
	CString sDay;
	CString sHour;
	CString sMinute;
	CString sSecond;

	sYear.Format(_T("%d"),iYear);
	sMonth.Format(_T("%d"),iMonth);
	sDay.Format(_T("%d"),iDay);
	sHour.Format(_T("%d"),iHour);
	sMinute.Format(_T("%d"),iMinute);
	sSecond.Format(_T("%d"),iSecond);

	if(0 == ComCheckDate(sYear,sMonth,sDay,sHour,sMinute,sSecond))
	{
		CTime t(iYear,iMonth,iDay,iHour,iMinute,iSecond);
		return (time_t)t.GetTime();
	}
	return 0;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将16进制的数字字符串转换为数字数组

@param      (i) const char* szData        16进制的数字字符串
@param      (o) BYTE* pBuff               数组地址
@param      (i) int iBufSize              数组长度

@retval     int   被转换的数字个数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int ComfrHEX(const char* szData, BYTE* pBuff, int iBufSize)
{
	int n, c, ret;
	char ahex[3];
	BYTE* p;
	char *szText;
	char ch;

	ret = 0;
	memset(pBuff,0, iBufSize);
	n = (int)strlen(szData);
	p = pBuff;
	c = 0;
	szText = strdup(szData);
	szText = strupr(szText);

	for (int i=0; i<n; i++)
	{
		ch = szText[i];
		if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') )
		{
			ahex[c++] = ch;
			if (c == 2)
			{
				ahex[2] = 0;
				c = 0;
				*p++ = (BYTE)strtol(ahex,NULL,16);
				ret++;
			}
		}
	}
	free(szText);
	return ret;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将一个字符串转换为8位的数组

@param      (i) const char* szData        字符串
@param      (o) BYTE* pBuff               输出的数组

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void ComConvertStrToArrEight(const TCHAR* szData, BYTE* pBuff)
{
	unsigned __int64 iVal;
	unsigned __int64 iConvert = 0xFF;

	iVal = _ttoi64(szData);

	for(int i = 0; i <= 7 ; i++)
	{
		pBuff[7 - i] = (BYTE)(iVal & iConvert);
		iVal = iVal >> 8;
	}

	return;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      将8位的数组转换为字符串

@param      (i) BYTE* pBuff     数组首地址

@retval     CString    转换好的字符串

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString ComConvertArrEightToStr(BYTE* pBuff)
{
	CString sResult = _T("");
	unsigned __int64 iLongLong;
	unsigned __int64 iLongLongResult;


	iLongLongResult = 0x00;
	for(int i = 7; i > 0; i--)
	{
		iLongLong = 0x00;
		iLongLong = iLongLong | pBuff[i];
		for(int j = 7; j > i; j--)
		{
			iLongLong = iLongLong << 8;
		}
		iLongLongResult = iLongLongResult | iLongLong;
	}

	sResult.Format(_T("%d"),iLongLongResult);

	return sResult;

}

//////////////////////////////////////////////////////////////////////////
/*
@brief     将UTF8格式的文字转化成UTF16格式 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComUtf8ToUtf16(CHAR* pUtf8)
{
	WCHAR* pUtf16 = NULL;
	int utf16Len = MultiByteToWideChar(CP_UTF8,NULL,pUtf8,-1,pUtf16,0);
	pUtf16 = new WCHAR[utf16Len+1];
	memset(pUtf16,0,(utf16Len+1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8,NULL,pUtf8,-1,pUtf16,utf16Len);
	CString strUtf16;
	strUtf16.Format(_T("%s"),pUtf16);
	delete [] pUtf16;
	pUtf16 = NULL;
	return strUtf16;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CHAR* ComUtf16ToUtf8(CString strUtf16)
{
	CHAR* pUtf8 = NULL;
	//算出转换后的长度
	int utf8Len=WideCharToMultiByte(CP_UTF8,NULL,strUtf16,-1,pUtf8,0,NULL,NULL); 
	pUtf8 = new CHAR[utf8Len+1];
	memset(pUtf8,0,utf8Len+1);
	WideCharToMultiByte(CP_UTF8,NULL,strUtf16,-1,pUtf8,utf8Len,NULL,NULL);
	return pUtf8;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief    将UTF16字符转换成ANSI  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//CHAR* ComUtf16ToAnsi(CString strUtf16)
//{
//	CHAR* pAnsi = NULL;
//	//算出转换后的长度
//	int ansiLen=WideCharToMultiByte(CP_ACP,NULL,strUtf16,-1,pAnsi,0,NULL,NULL); 
//	pAnsi = new CHAR[ansiLen+1];
//	memset(pAnsi,0,ansiLen+1);
//	WideCharToMultiByte(CP_ACP,NULL,strUtf16,-1,pAnsi,ansiLen,NULL,NULL);
//	return pAnsi;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将4个BYTE转换为long

@param      (i) BYTE bHiByte1     第一位数字
@param      (i) BYTE bLoByte1     第二位数字
@param      (i) BYTE bHiByte2     第三位数字
@param      (i) BYTE bLoByte2     第四位数字

@retval     long    转换好的long型数字

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long ComMakeLong(BYTE bHiByte1, BYTE bLoByte1, BYTE bHiByte2, BYTE bLoByte2)
{
	long lLogdate = 0;
	WORD wHiWord;
	WORD wLoWord;

	wHiWord = MAKEWORD(bLoByte1,bHiByte1);
	wLoWord = MAKEWORD(bLoByte2,bHiByte2);
	lLogdate = MAKELONG(wLoWord,wHiWord);

	return lLogdate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将2个BYTE转换为WORD

@param      (i) BYTE bHiByte     第一位数字
@param      (i) BYTE bLoByte     第二位数字

@retval     WORD    转换好的WORD型数字

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
WORD ComMakeWORD(BYTE bHiByte, BYTE bLoByte)
{
    WORD lLogdate = 0;

    lLogdate = MAKEWORD(bLoByte,bHiByte);

    return lLogdate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      填充字符

@param      (i) unsigned long ulDataLen     数据长度

@retval     BYTE*    需填充的内容

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BYTE* ComPad(unsigned long ulDataLen)
{
	unsigned long ulNo = 8 - (ulDataLen % 8);
	BYTE *p = new BYTE[ulNo + 1];
	memset(p, 0, ulNo+1);
	unsigned long i = 0;
	for (; i < ulNo; i++)
		p[i] = (BYTE)ulNo;
	p[i] = '\0';
	return p;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      恢复long到BYTE类型

@param      (i) long lValue     需恢复的long
@param      (o) BYTE* p         变更后的BYTE

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void ComRevLong(long lValue, BYTE* p)
{
	WORD wHiWord;
	WORD wLoWord;
	BYTE* p2 = p;

	wHiWord = HIWORD(lValue);
	wLoWord = LOWORD(lValue);

	*p2 = HIBYTE(wHiWord);
	p2++;
	*p2 = LOBYTE(wHiWord);
	p2++;
	*p2 = HIBYTE(wLoWord);
	p2++;
	*p2 = LOBYTE(wLoWord);
	p2++;
}	

//////////////////////////////////////////////////////////////////////////
/**
@brief      恢复WORD到BYTE类型

@param      (i) WORD wValue     需恢复的WORD
@param      (o) BYTE* p         变更后的BYTE

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void ComRevWORD(WORD wValue, BYTE* p)
{
    BYTE* p2 = p;

    *p2 = HIBYTE(wValue);
    p2++;
    *p2 = LOBYTE(wValue);
    p2++;
}	

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭系统

@param      bool bReboot 是否重启动

@retval     bool    true:关闭成功,  false:关闭失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool ComShutDownSystem(bool bReboot)
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

	// Get a token for this process. 

	if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
		return false ; 
	}

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 

	if (GetLastError() != ERROR_SUCCESS) {
		return false; 
	}

    // Set shutdown type
    UINT uFlags = EWX_FORCE;
    if (bReboot) {
        uFlags |= EWX_REBOOT;
    }
    else {
        uFlags |= EWX_SHUTDOWN;
    }

    // Shut down or reboot the system and force all applications to close. 
	if (FALSE == ExitWindowsEx(uFlags, 0)) {
		return false; 
	}

	return true;
}

CString ComConvToHexStr(LPBYTE lpData, int iLen)
{
	CString strRet = _T("");
	for (int i=0;i<iLen;i++){
		CString strTmp = _T("");
		strTmp.Format(_T("%02x"), lpData[i]);
		strRet += strTmp;
	}
	return strRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      将long型转换为_DATE_TIME型

@param      (i) long lDateTime     long型时间

@retval     _DATE_TIME    _DATE_TIME型时间

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME ComConvLongToDate(long lDateTime)
{
	_DATE_TIME Date_Time;

	COleDateTime dt(lDateTime);
	if (dt.GetStatus()==COleDateTime::invalid)
	{
		return Date_Time;
	}
	else
	{
		Date_Time.wYear = dt.GetYear();
		Date_Time.biMonth = dt.GetMonth();
		Date_Time.biDay = dt.GetDay();
		Date_Time.biHour = dt.GetHour();
		Date_Time.biMinute = dt.GetMinute();
		Date_Time.biSecond = dt.GetSecond();
	}

	return Date_Time;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到打印机的工作作业

@param      (i) HANDLE hPrinter             打印机的句柄
@param      (i) JOB_INFO_2 **ppJobInfo      作业信息
@param      (i) int *pcJobs                 作业数
@param      (o) DWORD *pStatus              打印队列状态

@retval     BOOL      TRUE:获取成功     FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL GetJobs(HANDLE hPrinter,JOB_INFO_2 **ppJobInfo,int *pcJobs, DWORD *pStatus)        
{

	DWORD               cByteNeeded;
	DWORD				nReturned;
	DWORD				cByteUsed;
	JOB_INFO_2          *pJobStorage = NULL;
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	/* Get the buffer size needed. */
	if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return FALSE;
	}

	pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
	if (!(pPrinterInfo))
		/* Failure to allocate memory. */
		return FALSE;

	/* Get the printer information. */
	if (!GetPrinter(hPrinter,
		2,
		(LPBYTE)pPrinterInfo,
		cByteNeeded,
		&cByteUsed))
	{
		/* Failure to access the printer. */
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return FALSE;
	}

	/* Get job storage space. */
	if (!EnumJobs(hPrinter,
		0,
		pPrinterInfo->cJobs,
		2,
		NULL,
		0,
		(LPDWORD)&cByteNeeded,
		(LPDWORD)&nReturned))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			free(pPrinterInfo);
			pPrinterInfo = NULL;
			return FALSE;
		}
	}

	pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
	if (!pJobStorage)
	{
		/* Failure to allocate Job storage space. */
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return FALSE;
	}

	ZeroMemory(pJobStorage, cByteNeeded);

	/* Get the list of jobs. */
	if (!EnumJobs(hPrinter,
		0,
		pPrinterInfo->cJobs,
		2,
		(LPBYTE)pJobStorage,
		cByteNeeded,
		(LPDWORD)&cByteUsed,
		(LPDWORD)&nReturned))
	{
		free(pPrinterInfo);
		free(pJobStorage);
		pJobStorage = NULL;
		pPrinterInfo = NULL;
		return FALSE;
	}

	/*
	*  Return the information.
	*/
	*pcJobs = nReturned;
	*pStatus = pPrinterInfo->Status;
	*ppJobInfo = pJobStorage;
	free(pPrinterInfo);

	return TRUE;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查打印机是否有错误

@param      无

@retval     BOOL      TRUE:有错     FALSE:无错

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//BOOL ComIsPrinterError()
//{
//
//	JOB_INFO_2  *pJobs;
//	int         cJobs;
//	int			i;
//	DWORD       dwPrinterStatus;
//
//	char *pName = new char[255];
//	unsigned long lSize = 255;
//
//	//=======Get default printer name===================
//	if (GetDefaultPrinter(pName, &lSize)==0)
//	{// in case fail
//		//release memory
//		delete pName;
//		pName = NULL;
//		return TRUE;
//	}
//	//===============Open printer========================
//	HANDLE phPrinter;
//	PRINTER_DEFAULTS pDefault;
//	pDefault.pDatatype = NULL;
//	pDefault.pDevMode = NULL;
//	pDefault.DesiredAccess = PRINTER_ACCESS_USE;
//
//	if (OpenPrinter(pName, &phPrinter,  &pDefault ) == 0)
//	{
//		// in case open fail
//		delete pName;
//		pName = NULL;
//		return TRUE;
//	}
//
//	/*
//	*  Get the state information for the Printer Queue and
//	*  the jobs in the Printer Queue.
//	*/
//	if (FALSE == GetJobs(phPrinter, &pJobs, &cJobs, &dwPrinterStatus))
//	{
//		ClosePrinter(phPrinter);
//		delete pName;
//		pName = NULL;
//		return TRUE;
//	}
//
//	/*
//	*  If the Printer reports an error, believe it.
//	*/
//	if (dwPrinterStatus &
//		(PRINTER_STATUS_ERROR |
//		PRINTER_STATUS_PAPER_JAM |
//		PRINTER_STATUS_PAPER_OUT |
//		PRINTER_STATUS_PAPER_PROBLEM |
//		PRINTER_STATUS_OUTPUT_BIN_FULL |
//		PRINTER_STATUS_NOT_AVAILABLE |
//		PRINTER_STATUS_NO_TONER |
//		PRINTER_STATUS_OUT_OF_MEMORY |
//		PRINTER_STATUS_OFFLINE |
//		PRINTER_STATUS_DOOR_OPEN))
//	{
//		//ClosePrinter(phPrinter);
//		//delete pName;
//		free( pJobs );
//		return TRUE;
//	}
//
//	/*
//	*  Find the Job in the Queue that is printing.
//	*/
//	for (i=0; i < cJobs; i++)
//	{
//		if (pJobs[i].Status & JOB_STATUS_PRINTING)
//		{
//			/*
//			*  If the job is in an error state,
//			*  report an error for the printer.
//			*  Code could be inserted here to
//			*  attempt an interpretation of the
//			*  pStatus member as well.
//			*/
//			if (pJobs[i].Status &
//				(JOB_STATUS_ERROR |
//				JOB_STATUS_OFFLINE |
//				JOB_STATUS_PAPEROUT |
//				JOB_STATUS_BLOCKED_DEVQ))
//			{
//				ClosePrinter(phPrinter);
//				delete pName;
//				pName = NULL;
//				free( pJobs );
//				return TRUE;
//			}
//		}
//	}
//
//	/*
//	*  No error condition.
//	*/
//	ClosePrinter(phPrinter);
//	delete pName;
//	pName = NULL;
//	free( pJobs );
//	return FALSE;
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief     取得系统当前时间

@param     format	时间格式	/n
YYMMDD:年月日

@retval    返回系统当前时间字符串(格式)

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString ComGetSysTimeText(TIME_FORMAT format)
{
    CString result;

	CTime curTime(CTime::GetCurrentTime());

    switch (format) {
    case YYYYMMDD:
        result = curTime.Format(_T("%Y-%m-%d"));
        break;

    case YYYYMMDDHHMM_TYPE1:
		result = curTime.Format(_T("%Y-%m-%d  %H:%M"));
        break;
	case YYYYMMDDHHMMSS_TYPE1:
		result = curTime.Format(_T("%Y-%m-%d  %H:%M:%S"));
		break;
    case YYYYMMDDHHMM_TYPE2:
        result = curTime.Format(_T("%Y-%m-%d %H:%M"));
        break;
	case YYYYMMDDHHMMSS_TYPE2:
		result = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
    case YYYYMMDDHHMM_TYPE3:
        result = curTime.Format(_T("%Y-%m-%d %H:%M"));
        break;
	case YYYYMMDDHHMMSS_TYPE3:
		result = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
    case YYYYMMDDHHMM_TYPE4:
        result = curTime.Format(_T("%H:%M %Y-%m-%d"));
        break;
	case YYYYMMDDHHMMSS_TYPE4:
		result = curTime.Format(_T("%H:%M:%S %Y-%m-%d"));
		break;
	case YYYYMMDDHHMMSS_TYPE5:
		result = curTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		break;
	case DDMMYYYYHHMM_TYPE6:
		result = curTime.Format(_T("%d-%m-%Y %H:%M"));
		break;
    case WEEKDAY_CN:
        result = ComGetWeekdayNameCN(curTime.GetDayOfWeek());
        break;

    case WEEKDAY_EN:
        result = curTime.Format(_T("%A"));
        break;

    default:
        break;
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得系统当前时间

@param      void

@retval     _DATE_TIME   返回系统当前时间

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME ComGetCurTime()
{
    SYSTEMTIME sysTime;
    ::GetLocalTime(&sysTime);
    
    _DATE_TIME curTime;

    curTime.wYear    = sysTime.wYear;
    curTime.biMonth  = (BYTE)sysTime.wMonth;
    curTime.biDay    = (BYTE)sysTime.wDay;
    curTime.biHour   = (BYTE)sysTime.wHour;
    curTime.biMinute = (BYTE)sysTime.wMinute;
    curTime.biSecond = (BYTE)sysTime.wSecond;

    return curTime;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      取得系统当前日期

@param      void

@retval     _DATE_TIME   返回系统当前时间

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
_DATE ComGetCurDate()
{
    _DATE date;
    _DATE_TIME time = ComGetCurTime();
    date.wYear = time.wYear;
    date.biMonth = time.biMonth;
    date.biDay = time.biDay;
    return date;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      在字符串左边追加全角空格

@param      (i)const char* text  要变换的字符串
@param      (i)int strLen        变换后的字符串长度（全角）

@retval     CString              变换后的字符串

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString add_wsp(const TCHAR* text,int strLen)
{

    CString strRet(text);
	int actualCharacterWidth = GetAactualCharacterWidth(strRet)/2 ;	
    // 做成所需追加的空格字符串
    while (actualCharacterWidth< strLen)
    {
        // 在字符串左边追加全角空格
		strRet = W_SPACE + strRet;
        actualCharacterWidth++;
    }

    return strRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      在字符串左边追加半角空格

@param      (i)const char* text  要变换的字符串
@param      (i)int strLen        变换后的字符串长度（半角）

@retval     CString              变换后的字符串

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString add_hsp(const TCHAR* text,int strLen)
{
    CString strRet(text);

    int actualCharacterWidth = GetAactualCharacterWidth(strRet);	
	
    // 做成所需追加的空格字符串
    while (actualCharacterWidth< strLen) {
        // 在字符串左边追加半角空格
        strRet = H_SPACE + strRet;
        actualCharacterWidth++;
    }

    return strRet;

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      在字符串右边追加半角空格

@param      (i)const char* text  要变换的字符串
@param      (i)int strLen        变换后的字符串长度（半角）

@retval     CString              变换后的字符串

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString add_hsp_right(const TCHAR* text,int strLen)
{
    CString strRet(text);

    int size = GetAactualCharacterWidth(strRet);

    // 做成所需追加的空格字符串
    while (size< strLen) {
        // 在字符串右边追加半角空格
        strRet = strRet + H_SPACE;
        size++;
    }

    return strRet;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断字符串指定位置是否是半个汉字

@param      (i)CString text     被检查的字符串
@param      (i)int pos          指定位置（从0开始）

@retval     bool    true:指定位置是半个汉字 ; false:指定位置不是半个汉字

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool IsHalfHZ(CString text, int pos)
{
    bool result = false;

    if (pos < 0 || pos >= text.GetLength()) {
        return false;
    }
	int i = pos;
    for (; i >= 0 && !isascii(text.GetAt(i)); i--);

    if ((pos - i) % 2 == 1) {
        result = true;
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断字符串实际占位数（全角占两位）

@param      (i)CString text     被检查的字符串

@retval     int pos      字符串实际占位数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int GetAactualCharacterWidth(CString text)
{
	int actualCharacterWidth=0;
	for(int i=0;i<text.GetLength();i++){
		TCHAR c = text[i];
		if(c>0x0100){	//高位如果是1，则认为是全角		
			actualCharacterWidth+=2;
		}
		else{
			actualCharacterWidth+=1;
		}
	}
	return actualCharacterWidth;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断字符串实际字节数

@param      (i)CString text     被检查的字符串

@retval     int pos      字符串实际字节数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int GetAactualCharacterMemarySize(CString text)
{
	int actualCharacterWidth=0;
	for(int i=0;i<text.GetLength();i++){
		TCHAR c = text[i];
		if(c>0x0100){	//高位如果是1	
			actualCharacterWidth+=2;
		}
		else{
			actualCharacterWidth+=1;
		}
	}
	return actualCharacterWidth;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		打印时字符串的格式化处理（一行）   

@param      (i)const CString& PrintStringLeft		打印行左边的字符串
@param      (i)const CString& PrintStringRight		打印行右边的字符串

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString ComPrintLineFormat(const CString& PrintStringLeft,const CString& PrintStringRight)
{
    return PrintStringLeft + add_hsp(PrintStringRight, PRINT_MAX_WIDTH - PrintStringLeft.GetLength());
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		打印时字符串的格式化处理（模板读取数据）   

@param      (i)const CString PrintLine		打印行字符串
@param      (o)CStringArray 格式化后的字符串

@retval     无  

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void ComPrintLineFormatForTemplate(CString PrintLine,CStringArray& printLines)
{
	CString strLeft(_T(""));
	CString strRight(_T(""));

	int lineLengh = PRINT_MAX_WIDTH;
	int iPosCN = PrintLine.Find(_T("[CN]"));
	int iPosEN = PrintLine.Find(_T("[EN]"));
	int iPosPN = PrintLine.Find(_T("[PN]"));
	if (iPosCN != -1){
		strLeft = PrintLine.Left(iPosCN);
		strRight = PrintLine.Mid(iPosCN+4);
		PrintLine =strLeft + strRight;
	}
	else if (iPosEN != -1){
		strLeft = PrintLine.Left(iPosEN);
		strRight = PrintLine.Mid(iPosEN+4);
		PrintLine =strLeft + strRight;
	}
	if (iPosPN != -1){
		strLeft = PrintLine.Left(iPosPN);
		strRight = PrintLine.Mid(iPosPN+4);
		PrintLine =strLeft + strRight;
	}

	int iPos = PrintLine.FindOneOf(_T(":"));
	// 正常数据项
	if(0/*iPos != -1*/){
		// 字符数小于单行最大字符数
		if(GetAactualCharacterWidth(PrintLine)<lineLengh) {
			strLeft = PrintLine.Left(iPos);
			strRight = PrintLine.Mid(iPos+1);
			PrintLine = strLeft + add_hsp(strRight, lineLengh - GetAactualCharacterWidth(strLeft)- 2);
			printLines.Add(PrintLine);
		}
		// 字符数大于单行最大字符数
		else {
			strLeft = PrintLine.Left(iPos);
			printLines.Add(strLeft);
			if (strLeft.Find(_T("[CN]"))!=-1){
				strRight = /*_T("[CN]") + */PrintLine.Mid(iPos+1);
			} 
			else if (strLeft.Find(_T("[EN]"))!=-1){
				strRight = /* _T("[EN]") +*/ PrintLine.Mid(iPos+1);
			}
			else if (strLeft.Find(_T("[PN]"))!=-1){
				 strRight = /*_T("[PN]") +*/ PrintLine.Mid(iPos+1);
			}
			else {
				strRight = /*_T("[CN]") +*/ PrintLine.Mid(iPos+1);
			}
			PrintLine = add_wsp(strRight, lineLengh - GetAactualCharacterWidth(strRight)-1);
			printLines.Add(PrintLine);
		}		
	}
	// 标题|空行|分割线
	else{
		printLines.Add(PrintLine);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      格式化金额

@param      (i)int 金额，以分为单位

@retval     CString  格式化好的金额，
            如 ComFormatAmount(999999),返回"9,999.99"

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString ComFormatAmount(long iAmount)
{
	CString strAmount;
	if (iAmount == INT_MIN) {
		strAmount = _T("-21,474,836.48");
	}
	else {
		bool lessThanZero = false;
		if (iAmount<0) {
			lessThanZero = true;
			iAmount = 0 - iAmount;
		}
		if( iAmount<100 ) {
			strAmount.Format(_T("0.%.1d"),iAmount);
		}
		else {
			strAmount.Format(_T(".%d"),iAmount%100);
			iAmount = iAmount/100;
			CString str;
			str.Format(_T("%d"),iAmount);
			int i = str.GetLength();
			while (i-3>0) {
				i = i - 3;
				str.Insert(i,',');
			}
			strAmount = str + strAmount;
		}

		if(lessThanZero) {
			strAmount = _T("-") + strAmount;
		}
	}
	// 需要添加空格
	strAmount += _T(" ");

	return strAmount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      从格式化的金额字符串中取得金额

@param      (i)CString金额字符串

@retval     long返回的金额，以分为单位
如 ComFormatAmount("9,999.99"),返回999999

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long ComGetAmount(CString sAmount)
{
	sAmount = sAmount.Trim();
	if(sAmount ==_T("")) return 0;
	bool isMinus = sAmount[0]==_T('-');
	if(isMinus) sAmount = sAmount.Right(sAmount.GetLength()-1);
	int dotPos = sAmount.Find(_T("."));
	long ret =0;
	if(dotPos == -1)
	{
		ret = _ttoi(sAmount)*100;
	}
	else
	{
		CString sPart1 = sAmount.Left(dotPos);
		CString sPart2 = sAmount.Mid(dotPos+1);
		CString sPart2Correct=_T("");
		for(int i=0;i<2;i++)
		{
			if(sPart2.GetLength()<i+1)
			{
				sPart2Correct+="0";
			}
			else
			{
				sPart2Correct +=sPart2.Mid(i,1);
			}
		}		
		ret = _ttoi(sPart1)*100 + _ttoi(sPart2Correct);
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      格式化数量

@param      (i)int 数量

@retval     CString  格式化好的数量，
            如 ComFormatCount(9999999),返回"9,999,999"

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString ComFormatCount(int iCount)
{
	int nCount = iCount;
    CString strCount;
    strCount.Format(_T("%d"),nCount);
    int i = strCount.GetLength();
    while (i-3>0) {
        i = i - 3;
		if (strCount.GetAt(i-1) != '-') {
			strCount.Insert(i,',');
		}
    }
    return strCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断字符串中是否有字母

@param      (i)const CString& str  要进行判断的年份

@retval     bool                   true:有字母  false:没有字母

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool ComIsAlphaInString(const CString& str)
//{
//    //输入是否含有字母
//    int size = str.GetLength();
//    for (int i=0;i<size;i++){
//        if (IsCharAlpha(str.GetAt(i))){
//            return true;
//        }
//    }
//    return false;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否全是数字

@param      (i)const CString& str  要进行判断的字符串

@retval     bool                    true:全是数字  false:并不全是

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool ComIsAllNumericInString(const CString& str)
{
    int size = str.GetLength();
    for (int i=0;i<size;i++){
        if (str.GetAt(i) < '0' || str.GetAt(i) > '9'){
            return false;
        }
    }
    return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      检查EDIT框里的字符串是不是数字,并是否在规定的范围内 created by nijun

@param      (i) srcData     CString      输入待检验的字符串
(i) min         int          最小值
(i) max         int          最大值

@retval     bool /n
True 合法     False 非法

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckNumText(CString srcData,int min,int max)
{
    int num ;
    //	// 输入值包括字母
    if (!ComIsAllNumericInString(srcData)) {
        return false;
    }

    // 判断输入内容是否为空
    if ( _T("") == srcData ) {
        return false;
    }

    num = _ttoi(srcData.GetBuffer(srcData.GetLength())) ;

    if ((num < min) || (num >max)) {
        return false;
    }
    return true ;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到当前系统的路径 created by nijun

@param      (i) CString&   sAppPath   当前系统的路径
@retval     bool     \n
            true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool GetAppPath(CString& sAppPath)
{
    TCHAR szAppPath[_MAX_PATH];

    if (ComGetAppPath(szAppPath,_MAX_PATH) <= 0) {
        return false;
    }

    sAppPath = szAppPath;
    sAppPath = sAppPath.TrimRight(_T("\\"))+_T("\\");

    return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      字符串差分函数 created by nijun

@param      (i)CString srcData        被差分的字符串 \n 
            (i)CString sSplitFlag     差分字符串 \n
            (i)CStringArray &array    差分以后保存到的字符串数组 \n

@retval      none

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void GetSplitStrArray(CString sData,CString sSplitFlag,CStringArray &array)
{
    CString sTemp;
    int pos =-1;
    if (sData.Right(1) != sSplitFlag) {
        sData +=sSplitFlag;
    }

    while ((pos=sData.Find(sSplitFlag,0)) != -1){
        sTemp = sData.Left(pos);
        array.Add(sTemp); 
        sData = sData.Right(sData.GetLength() - pos - 1);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取基于基准日指定偏移天数后的日期(一票通计算车票有效期用)

@param      (i)WORD publishDay,			初始化日期数
@param		(i)WORD expirationDay		有效截止日期数

@retval     _DATE   变化后的日期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayAfterTypicalDay(WORD publishDay,WORD expirationDay)
{
	_DATE baseTime;				// 一票通计算偏移的基准日期
	baseTime.wYear = 2005;
	baseTime.biMonth = 1;
	baseTime.biDay = 1;
	DWORD offSetDay = publishDay+expirationDay;	// 计算基于基准日期的偏移量

	return GetDayAfter(baseTime,offSetDay);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定日期若干年后的日期

@param      (i) _DATE& date          指定日期
@param      (i) unsigned short day   相差的年数

@retval     _DATE   变化后的日期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetYearAfter(const _DATE& date, unsigned short year)
{
    _DATE newDate;
    newDate.wYear = date.wYear + year;
    newDate.biMonth = date.biMonth;
    if(date.biMonth == 2 && date.biDay == 29 && (!IsLeapYear(newDate.wYear))) {
        newDate.biDay = 28;
    }
    else{
        newDate.biDay = date.biDay;
    }
    return newDate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定日期若干天后的日期

@param      (i) const _DATE& date           指定日期
@param      (i) DWORD                       相差的天数

@retval     _DATE   变化后的日期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayAfter(const _DATE& date, DWORD day)
{
    CTime time(date.wYear, date.biMonth, date.biDay, 0, 0, 0);
    time += day*24*3600;
    _DATE newDate;
    newDate.wYear = time.GetYear();
    newDate.biMonth = time.GetMonth();
    newDate.biDay = time.GetDay();
    return newDate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定日期若干天前的日期

@param      (i) const _DATE& date           指定日期
@param      (i) DWORD day                   相差的天数

@retval     _DATE   变化后的日期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE GetDayBefore(const _DATE& date, DWORD day)
{
    CTime time(date.wYear, date.biMonth, date.biDay, 0, 0, 0);
    time -= day*24*3600;
    _DATE newDate;
    if (time < 0 ) {
        newDate.wYear = 1970;
        newDate.biMonth = 1;
        newDate.biDay = 1; 
    }
    else {
        newDate.wYear = time.GetYear();
        newDate.biMonth = time.GetMonth();
        newDate.biDay = time.GetDay();
    }

    return newDate;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取2个指定日期之间相差的秒数

@param      (i) const _DATE_TIME& offDate           指定日期
@param      (i) const _DATE_TIME& baseDate          基准日期

@retval     DWORD 相差的秒数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD GetOffTimeSecond(const _DATE_TIME& offDate,const _DATE_TIME& baseDate)
{
	DWORD offTimeSecond = 0;
	CTime offTime(offDate.wYear,offDate.biMonth,offDate.biDay,offDate.biHour,offDate.biMinute,offDate.biSecond);
	CTime baseTime(baseDate.wYear,baseDate.biMonth,baseDate.biDay,baseDate.biHour,baseDate.biMinute,baseDate.biSecond);
	if (offTime>= baseTime) {
		CTimeSpan offTimeSpan = offTime - baseTime;
		offTimeSecond = (DWORD)offTimeSpan.GetTotalSeconds();
	}
	else{
		CTimeSpan offTimeSpan = baseTime - offTime;
		offTimeSecond = (DWORD)offTimeSpan.GetTotalSeconds();
	}
	return offTimeSecond;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      获取2个指定日期之间相差的天数

@param      (i) const _DATE& offDate           指定日期
@param      (i) const _DATE& baseDate          基准日期

@retval     DWORD 相差的天数

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD GetOffTimeDay(const _DATE& offDate,const _DATE& baseDate)
{
	DWORD offTimeDay = 0;
	CTime offTime(offDate.wYear,offDate.biMonth,offDate.biDay,0,0,0);
	CTime baseTime(baseDate.wYear,baseDate.biMonth,baseDate.biDay,0,0,0);
	if (offTime>= baseTime) {
		CTimeSpan offTimeSpan = offTime - baseTime;
		offTimeDay = (DWORD)offTimeSpan.GetDays();
	}
	else{
		CTimeSpan offTimeSpan = baseTime - offTime;
		offTimeDay = (DWORD)offTimeSpan.GetDays();
	}
	return offTimeDay;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取指定日期后指定秒数的日期

@param      (i) const _DATE_TIME& baseDate          指定日期
@param      (i) unsigned long offSeconds			指定秒数的秒数

@retval     _DATE_TIME   变化后的日期

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetDateTimeAfterBySecond(const _DATE_TIME& baseDate, unsigned long offSeconds)
{
	CTime time(baseDate.wYear,baseDate.biMonth,baseDate.biDay,baseDate.biHour,baseDate.biMinute,baseDate.biSecond);
	time += offSeconds;
	_DATE_TIME newDate;
	newDate.wYear = time.GetYear();
	newDate.biMonth = time.GetMonth();
	newDate.biDay = time.GetDay();
    newDate.biHour = time.GetHour();
	newDate.biMinute = time.GetMinute();
	newDate.biSecond = time.GetSecond();

	return newDate;
}

_DATE_TIME GetAfterCurTime(const Duration_t &duration)
{
	_DATE curDate = ComGetCurDate();
	DurationUnit_t dateUnit = GetDurationType(duration);
	DurationValue_t dateValue = GetDurationValue(duration);

	_DATE_TIME timeAfter;
	
	CBR br;
	DateC20_t time = br.DateStructToDateC20(curDate);
	DateC20_t newTime = br.CalculateValidityEndDate(time, dateValue, dateUnit);
	_DATE date = br.DateC20ToDateStruct(newTime);

	timeAfter.wYear = date.wYear;
	timeAfter.biMonth = date.biMonth;
	timeAfter.biDay = date.biDay;

	if(timeAfter.biHour == 0 && timeAfter.biMinute == 0){
		timeAfter.biHour = 23;
		timeAfter.biMinute = 59;
	}
	return timeAfter;
}

//////////////////////////////////////////////////////////////////////////
//**
//@brief      返回延长有效期类型－天，分钟，年
//@param      （i）_DATE_TIME dtSetTime  起始时间
//@param      （i）Duration_t duration   持续时间
//
//@retval     _DATE_TIME   延长后有效时间 
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetAfterSetTime(_DATE_TIME dtSetTime,const Duration_t &duration)
{
	_DATE curDate;
	curDate.wYear = dtSetTime.wYear;
	curDate.biMonth = dtSetTime.biMonth;
	curDate.biDay = dtSetTime.biDay;

	DurationUnit_t dateUnit = GetDurationType(duration);
	DurationValue_t dateValue = GetDurationValue(duration);

	_DATE_TIME timeAfter;

	CBR br;
	DateC20_t time = br.DateStructToDateC20(curDate);
	DateC20_t newTime = br.CalculateValidityEndDate(time, dateValue, dateUnit);
	_DATE date = br.DateC20ToDateStruct(newTime);

	timeAfter.wYear    = date.wYear;
	timeAfter.biMonth  = date.biMonth;
	timeAfter.biDay    = date.biDay;
	timeAfter.biHour   = dtSetTime.biHour;
	timeAfter.biMinute = dtSetTime.biMinute;
	timeAfter.biSecond = dtSetTime.biSecond;

	if(timeAfter.biHour == 0 && timeAfter.biMinute == 0){
		timeAfter.biHour = 23;
		timeAfter.biMinute = 59;
	}
	return timeAfter;
}
//////////////////////////////////////////////////////////////////////////
//**
//@brief      返回有效期类型－天，分钟，年
//
//@param      （i）Duration_t duration   持续时间
//
//@retval     DurationUnit_t   有效持续时间类型 
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
DurationUnit_t GetDurationType(Duration_t duration)
{
	return (DurationUnit_t)(duration >> 12);
}

//////////////////////////////////////////////////////////////////////////
//**
//@brief      返回有效期的值—1 天，1 分钟，1 年
//
//@param      （i）Duration_t duration   持续时间
//
//@retval     DurationValue_t   有效持续时间 
//
//@exception  无
//*/
//////////////////////////////////////////////////////////////////////////
DurationValue_t GetDurationValue(Duration_t duration)
{
	return (DurationValue_t)(duration & 0xFFF);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME GetDateTimeAfter(const _DATE_TIME & datetime,DWORD day)
{
	CTime time(datetime.wYear, datetime.biMonth, datetime.biDay, datetime.biHour, datetime.biMinute, datetime.biSecond);
	time += day*24*3600;
	_DATE_TIME newDateTime;
	newDateTime.wYear = time.GetYear();
	newDateTime.biMonth = time.GetMonth();
	newDateTime.biDay = time.GetDay();
	newDateTime.biHour = time.GetHour();
	newDateTime.biMinute = time.GetMinute();
	newDateTime.biSecond = time.GetSecond();
	return newDateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief   　 读取二进制文件内容 created by nijun

@param      (i) const char* sPathFileName        二进制文件全路径及名称
@param      (o) VARIABLE_DATA &bufferStruct


@retval     bool  \n
            true:成功　false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool ReadBinaryFile(const TCHAR* sPathFileName,VARIABLE_DATA &bufferStruct)
{
    ifstream binFile;
    bool bRet;

    // 打开文件并读入内存
    binFile.open(sPathFileName, ios::binary | ios::in, ios::_Openprot);
    if(binFile)
    {
        binFile.seekg(0, ios_base::end);
        bufferStruct.nLen = binFile.tellg();
        if(bufferStruct.nLen > 0 )
        {
            binFile.seekg(0, ios_base::beg);
            bufferStruct.lpData = new BYTE[bufferStruct.nLen];
            ::memset(bufferStruct.lpData, 0, bufferStruct.nLen);
            binFile.read((char*)bufferStruct.lpData, bufferStruct.nLen);
            binFile.close();
            bRet = true;
        }
        else
        {
            // 文件为空
            bRet = false;
        }
    }
    else
    {
        // 文件不存在
        bRet = false;
    }
    return bRet;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief   　 创建目录

@param      (i) CString sPathName  目录名称

@retval     bool  \n
               true:成功　false: 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CreateFullDirectory(CString sPathName){
	UINT iPos = 0;
	CString sTmp = _T(""),sSubPath = _T("");
    
	sPathName = sPathName.TrimRight(_T("\\")) + _T("\\") ;
	sTmp = sPathName.Right(sPathName.GetLength() - 3);
	do {
		iPos = sTmp.Find(_T("\\"),iPos + 1);
		sSubPath = sPathName.Left(iPos + 3);
		if ( false == PathIsDirectory(sSubPath)) {
			if ( false == CreateDirectory(sSubPath,NULL)) {
				return false;
			}
		}
	} while(iPos != sTmp.GetLength() -1);
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief   　 比较2个日期大小

@param      (i) _DATE_TIME date1	日期1
@param      (i) _DATE_TIME date2	日期2

@retval     int \n
			-1  日期1小于日期2	\n
            0   日期1等于日期2  \n
			1   日期1大于日期2  \n

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CmpTwoDateTime(const _DATE_TIME& date1, const _DATE_TIME& date2)
{
	CTime time1(date1.wYear, date1.biMonth, date1.biDay, date1.biHour, date1.biMinute, date1.biSecond);
	CTime time2(date2.wYear, date2.biMonth, date2.biDay, date2.biHour, date2.biMinute, date2.biSecond);
	if (time1<time2) {	// 时间比较
		return -1;
	}
	else if (time1==time2) {
		return 0;
	}
	else{
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief   　 比较2个日期大小

@param      (i) _DATE date1	日期1
@param      (i) _DATE date2	日期2

@retval     int \n
			-1  日期1小于日期2	\n
			0   日期1等于日期2  \n
			1   日期1大于日期2  \n

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CmpTwoDate(const _DATE& date1, const _DATE& date2)
{
	CTime time1(date1.wYear, date1.biMonth, date1.biDay, 0, 0, 0);
	CTime time2(date2.wYear, date2.biMonth, date2.biDay, 0, 0, 0);
	if (time1<time2) {	// 时间比较
		return -1;
	}
	else if (time1==time2) {
		return 0;
	}
	else{
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取中文星期天号名称

@param      (i)int weekday  星期天号（1--7）

@retval     CString     中文星期天号名称

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString ComGetWeekdayNameCN(int weekday)
{
    UINT DayOfWeek[] = {
        LOCALE_SDAYNAME7,   // Sunday
        LOCALE_SDAYNAME1,   
        LOCALE_SDAYNAME2,
        LOCALE_SDAYNAME3,
        LOCALE_SDAYNAME4, 
        LOCALE_SDAYNAME5, 
        LOCALE_SDAYNAME6   // Saturday
    };

    TCHAR strWeekday[256];

	// Get string for day of the week from system
    ::GetLocaleInfo(LOCALE_USER_DEFAULT,DayOfWeek[weekday - 1],strWeekday, sizeof(strWeekday));

    return CString(strWeekday);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断数值是否超过DWROD类型的最大值（0xFFFFFFFF）

@param      (i)CString value

@retval     bool    true:超过DWROD类型的最大值; false:未超过DWROD类型的最大值

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool ComIsExceedDWROD(CString value)
{
    return (_ttoi64(value) > 0xFFFFFFFF);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得系统当前时间(当前时区秒数)

@param      void

@retval     TimeT   返回系统当前时间

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
TimeT ComGetCurTimeT()
{
    TimeT time = 0;
	tm stm;
	CTime curTime(CTime::GetCurrentTime());
	curTime.GetLocalTm(&stm);
	time = MkAbsTime(stm);
    return time;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查是否是BCD数据

@param      (i)char s   数据

@retval     bool true:是;false:不是

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckCharIsBCD(char s)
{
    char c = (s>>4) & 0x0F;
    if (c < 0 || c > 9) {
        return false;
    }

    c = s & 0x0F;
    if (c < 0 || c > 9) {
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检查是否是BCD数据

@param      (i)char *s   数据
@param      (i)int  len  数据长度

@retval     bool true:是;false:不是

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool ComCheckIsBCD(char *s, int len)
{
    for (int i = 0; i<len; i++) {
        if (!ComCheckCharIsBCD(*(s+i)) ) {
            return false;
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      TimeT转换成_DATE_TIME结构

@param      (i)TimeT offSeconds  偏移秒数

@retval     _DATE_TIME 日期时间结构体

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
_DATE_TIME ComConvertTimeTToDateTime(TimeT offSeconds)
{
	_DATE_TIME dateTime;
	CTime time(offSeconds);
	tm t1;
	time.GetGmtTm(&t1);
	dateTime.wYear = t1.tm_year + 1900;
	dateTime.biMonth = t1.tm_mon + 1;
	dateTime.biDay = t1.tm_mday;
	dateTime.biHour = t1.tm_hour;
	dateTime.biMinute = t1.tm_min;
	dateTime.biSecond = t1.tm_sec;
	return dateTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      tm转换成偏移秒数

@param      (i)tm stm	时间结构体

@retval     __int64		偏移秒数(相对1970年1月1日0时0分0秒) \n
			-1 转换失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
__int64 MkAbsTime(tm stm)
{
    // initialize the return value 
    __int64 offSeconds = -1;
    // set class COleDateTime    
    COleDateTime dt1(1970,1,1,0,0,0);
    COleDateTime dt2(stm.tm_year + 1900,stm.tm_mon + 1,stm.tm_mday,  stm.tm_hour,stm.tm_min,stm.tm_sec);
    // get COleDateTimeSpan from dt1 to dt2
    COleDateTimeSpan ts = dt2 - dt1;
    // check ts status
    if (ts.GetStatus() == COleDateTimeSpan::valid) {
        // calculate the seconds
        offSeconds = ts.GetTotalDays()*24*3600 + stm.tm_hour*3600 + stm.tm_min*60 + stm.tm_sec;
    }
    return offSeconds;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      本地时间秒数转换成utc秒数

@param      (i) __int64 localTime	本地时间秒数

@retval     __int64		utc秒数 \n
			-1 转换失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
__int64 ComConvertLocalTimeToUtcTime(__int64 localTime)
{
	// initialize the return value 
	__int64 utcTime = -1;
	CTime time(localTime);
	tm t2;
	time.GetGmtTm(&t2);
	utcTime = _mktime64(&t2);
	if (utcTime < 0) {
		return 0;
	}
	return utcTime;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      utc秒数转换成本地时间秒数

@param      (i) __int64 localTime	utc秒数

@retval     __int64		本地时间秒数 \n
			-1 转换失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
__int64 ComConvertUtcTimeToLocalTime(__int64 utcTime)
{
	// initialize the return value 
	__int64 localTime = -1;
	CTime time(utcTime);
	tm t1;
	time.GetLocalTm(&t1);
	localTime = MkAbsTime(t1);
	return localTime;
}
void CheckHeapHealth()
{
	const int HEAP_NUMBER = 1024;
	HANDLE hHeaps[HEAP_NUMBER];
	int nHeaps = GetProcessHeaps(HEAP_NUMBER, hHeaps);
	if (nHeaps > 0)
	{
		for (int i = 0; i < nHeaps; i++)
		{
			if (HeapValidate(hHeaps[i], 0, NULL) == FALSE)
			{
				throw std::bad_exception();
			}
		}
	}
};

const unsigned long crc32_tab[] = { 
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
	0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
	0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
	0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
	0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
	0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
	0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
	0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
	0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
	0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
	0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
	0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
	0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
	0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
	0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
	0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
	0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
	0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
	0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
	0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
	0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
	0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

unsigned long Crc32(void *pStart, unsigned long length, unsigned long crc32)
{
	register unsigned char  *pS= (unsigned char *)pStart;
	register const  unsigned long *p_crc32_tab    = crc32_tab;

	length++;
	while (--length)
	{
		crc32 = p_crc32_tab[(*pS++ ^ (unsigned char) crc32)& 0xFF] ^ (crc32 >> 8);
	}

	return (crc32);
}

void encrypt( const unsigned char * plaintext, int len, char * ciphertext )
{
	int i;
	for ( i = 0; i < len; i++ ) {
		ciphertext[ i ] = ((plaintext[ i ] - '0') + i ) % 10 + '0';
	}
	ciphertext[ len ] = '\0';
}

void decrypt( const unsigned char * ciphertext, int len, char * plaintext)
{
	int i;
	for ( i = 0; i < len; i++ ) {
		if( ciphertext[ i ]  < ( i + 0x30 ) ) {
			plaintext[ i ] = 10 + ciphertext[ i ] - i;
		} else {
			plaintext[ i ] = ciphertext[ i ] - i;
		}
	}	
	plaintext[ len ] = '\0';
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CString版本的Split函数

@param      (i)CString str		源字符串
@param      (i)CString delim    分隔符

@retval     CStringArray*   字符串数组

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CStringArray* SplitCString(CString str,CString delim)
{
	CString strTemp;
	CStringArray * urlarr = new CStringArray;
	int nIndex=0; 
	int nLast=0; 
	while(nIndex != -1)   
	{   
		nIndex = str.Find(delim,nLast);   
		if(nIndex != -1)   
		{   
			strTemp = str.Mid(nLast,nIndex - nLast);   
			nLast   = nIndex + delim.GetLength();   
		}   
		else   
		{   
			strTemp = str.Mid(nLast,str.GetLength() - nLast);   
		}         
		urlarr->Add(strTemp);
	} 
	return urlarr;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      根据条件查找文件名

@param      (i)CString searchCondition 搜索条件

@retval     vector<CString> 找到的文件名集合


@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void SearchFile(CString searchCondition,std::vector<CString>& result)
{
	CFileFind finder;

	BOOL bContinue = finder.FindFile(searchCondition);
	while (bContinue) {
		bContinue = finder.FindNextFile();

		if (finder.IsDots()||finder.IsDirectory()) {
			continue;
		}
		else {   
			CString sFileName = finder.GetFilePath();
			result.push_back(sFileName);

		}
	}
	finder.Close();

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      根据条件查找一个文件名

@param      (i)CString searchCondition 搜索条件

@retval     CString 找到的文件名，如果没有找到返回""。


@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CString SearchOneFile(CString searchCondition)
{
	CString fileName;

	CFileFind finder;
	BOOL bContinue = finder.FindFile(searchCondition);
	while (bContinue) {
		bContinue = finder.FindNextFile();

		if (finder.IsDots()||finder.IsDirectory()) {
			continue;
		}
		else {   
			fileName = finder.GetFilePath();
			break;
		}
	}
	finder.Close();
	return fileName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      根据条件删除文件夹

@param      (i)CString sDirPath 文件夹路径

@retval     vector<CString> 找到的文件名集合


@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool DeleteFolder(CString sDirPath)
{


	SHFILEOPSTRUCT fos ;
	ZeroMemory( &fos, sizeof( fos)) ;
	fos.hwnd = NULL;
	fos.wFunc = FO_DELETE ;
	fos.fFlags = FOF_SILENT  | FOF_NOCONFIRMATION;

	TCHAR   DelFileSource[MAX_PATH];
	memset(DelFileSource, 0x00, sizeof(DelFileSource));
	wcscat(DelFileSource, sDirPath + _T("*.*"));

	fos.pFrom = DelFileSource;
	// 删除文件夹及其内容
	SHFileOperation( &fos);

	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      验证身份证号码

@param      (i)CString number 身份证号码

@retval     bool

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCard(CString number)
{
	number.MakeUpper();
	if(number.GetLength() == 18){
		for(int i = 0; i < 18; i++)
		{
			if(i != 17 && (number[i] < '0' || number[i] > '9')){
				return false;
			}
			else if(i == 17 && (number[i] < '0' || number[i] > '9') && number[i] != 'X'){
				return false;
			}
		}

		COleDateTime dDateTime;
		if(!dDateTime.ParseDateTime(number.Mid(6,4) + _T("-") + number.Mid(10,2) + _T("-") + number.Mid(12,2))){
			return false;
		}

		int no[] = {7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2};
		char id[] = {'1','0','X','9','8','7','6','5','4','3','2'};
		int sum = 0;
		for(int i = 0; i < 17; i++)
		{
			sum += (number[i]-'0')*no[i];
		}
		int n = sum%11;
		if(id[n] != number[17]){
			return false;
		}
	}
	else if(number.GetLength() == 15){
		for(int i = 0; i < 15; i++)
		{
			if(number[i] < '0' || number[i] > '9'){
				return false;
			}
		}

		COleDateTime dDateTime;
		if(!dDateTime.ParseDateTime(CString(_T("19")) + number.Mid(6,2) + _T("-") + number.Mid(8,2) + _T("-") + number.Mid(10,2))){
			return false;
		}
	}
	else{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证身份证号码（澳门）

@param      无

@retval     true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCardMACAO(CString number)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证身份证号码（香港）

@param      无

@retval     true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckIDCardHongKong(CString number)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      验证出生日期

@param      无

@retval     true:成功  false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CheckBirthday(CString birthday)
{
	// 验证格式YYYYMMDD
	if (birthday.GetLength() != 8) {
		return false;
	}
	// 验证有效期
	_DATE_TIME currentDateTime = ComGetCurTime();
	int year = _ttoi(birthday.Left(4));
	int month = _ttoi(birthday.Mid(4, 2));
	int day = _ttoi(birthday.Mid(6, 2));
	// 年
	if (year < 1900 || year > currentDateTime.wYear) {
		return false;
	}
	// 月
	if (month < 1 || month > 12) {
		return false;
	}
	// 日
	if (day < 1 || day > 31) {
		return false;
	}
	if (month == 4 || month == 6 || month == 9 || month == 11) {
		if (day > 30) {
			return false;
		}
	}
	else if (month == 2) {
		// 闰年
		if ((year%4 == 0 && year%100 != 0) || year%400 == 0) {
			if (day > 29) {
				return false;
			}
		}
		else {
			if (day > 28) {
				return false;
			}
		}
	}
	// 今年
	if (year == currentDateTime.wYear) {
		if (month > currentDateTime.biMonth || day > currentDateTime.biDay) {
			return false;
		}
	}

	return true;
}

#define INI_BUFF_LEN	            1024
CString GetIniDataString(CString sFileName,LPCTSTR lpszSection, LPCTSTR lpszKey, CString sDefault)
{
	CString sValue = sDefault;
	try {
		DWORD dw = 0;
		TCHAR szBuf[INI_BUFF_LEN];

		dw = GetPrivateProfileString (lpszSection, lpszKey, _T(""), (TCHAR *)szBuf, INI_BUFF_LEN, sFileName);
		if (dw > 0)
		{
			AfxExtractSubString(sValue,szBuf,0,';');
			if (sValue.GetAt(0) == _T('|'))
			{
				sValue = sValue.Right(sValue.GetLength() - 1);
			}
		}
	}
	catch(...) {
	}

	return sValue;
}