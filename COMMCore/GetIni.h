#pragma once
#include "CoreExp.h"
#include "DebugLog.h"
#define INI_BUFF_LEN	            1024
class CORE_API CGetIni
{
public:
	CGetIni();
	CGetIni(CString sFileName);
	virtual ~CGetIni();
	int GetDataInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int iDefault = 0);
	CString GetDataString(LPCTSTR lpszSection, LPCTSTR lpszKey, CString sDefault = _T(""));
	TCHAR * GetData(char *pszSection, char *pszKey);
	BOOL WriteData(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszData);
	BOOL DeleteSection(LPCTSTR lpszSection);
	BOOL DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszKey);
private:
	CString m_sFileName;

};
