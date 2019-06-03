#if !defined(AFX_COMMON_H__B959A317_B089_4978_8207_9B9D63D2CB1C__INCLUDED_)
#define AFX_COMMON_H__B959A317_B089_4978_8207_9B9D63D2CB1C__INCLUDED_

#include <afx.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
//#include "MIMUtils.h"
//#define WIN32CHECK(x) \
//{ \
//    BOOL __bReturn__ = (x); \
//    if (FALSE == __bReturn__) \
//    { \
//        DWORD __dwErrCode__ = GetLastError(); \
//        _RPTF1(_CRT_ASSERT, "Call the Win32 API function " #x " failed. GetLastError() return %d.", __dwErrCode__); \
//        SetLastError(__dwErrCode__); \
//    } \
//}

//#define WIN32CHECK(x)               ((void) (x))

extern CString m_strCurrentDirectory;
BOOL CopyDirectory(LPCTSTR lpszSourceDir, LPCTSTR lpszDestDir);
BOOL RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir, BOOL b_IsFirst);
CString RenServerName2LocalName(CString sServerFileName);
BOOL ComCheckFileExist(LPCTSTR lpszFileName);
BOOL ComCheckDirectory(LPCTSTR lpszDirName);
BOOL ComExecuteApp(LPCTSTR lpszAppPath, LPCTSTR lpszAppParameter);
void ComGetDestDir(CString sPath, CString sOrgPath, CString sDestDir, CString &sDest);
void ComSetCurrentDir(LPCTSTR lpszDirName, CString &sCurrentDir);
DWORD   GetProcessIdFromName(LPCTSTR   name);
BOOL ComKillAppEx(LPCTSTR lpszBuf);
CString GetIniDataString(CString sFileName,LPCTSTR lpszSection, LPCTSTR lpszKey, CString sDefault);
#endif