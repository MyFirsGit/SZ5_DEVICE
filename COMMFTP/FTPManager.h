// FTPManager.h: interface for the CFTPManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPMANAGER_H__3088AEDD_64F3_40D8_9FF7_39F8ADA1BE8F__INCLUDED_)
#define AFX_FTPMANAGER_H__3088AEDD_64F3_40D8_9FF7_39F8ADA1BE8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxinet.h"

class CFTPManager  
{
public:
	CFTPManager();
	virtual ~CFTPManager();
public:
	
	int Connect(LPCTSTR lpszSiteName, LPCTSTR lpszUserName, LPCTSTR lpszPassword, int nPort,  BOOL bPassMode );
	BOOL FtpFileExists(LPCTSTR lpszPath);
	
	void Close();
	int GetLastError(DWORD dwErrorCode);
	
	
};

#endif // !defined(AFX_FTPMANAGER_H__3088AEDD_64F3_40D8_9FF7_39F8ADA1BE8F__INCLUDED_)
