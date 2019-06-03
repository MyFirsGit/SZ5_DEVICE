#pragma once
#include "FtpExp.h"
#include "Command.h"


class FTP_API CFTPConnect :	public CCommand
{
public:
	CFTPConnect(void);
	CFTPConnect::CFTPConnect(CString sHostID,CString sUserID,CString sPassword,int nPort);
	~CFTPConnect(void);
public:
	CString m_sHostID;
	CString m_sUserID;
	CString m_sPassword;
	int m_nPort;
	long ExecuteCommand(void);
};

class FTP_API CFTPClose :	public CCommand
{
public:
	CFTPClose(void);
	~CFTPClose(void);
	long ExecuteCommand(void);
};

class FTP_API CFTPDownLoad : public CCommand
{
public:
	CFTPDownLoad();
	CFTPDownLoad(CString sRemotePath, CString sLocalDir);
	virtual ~CFTPDownLoad();
public:

	BOOL RecursiveFileList(LPCTSTR lpszPath,LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir);
	BOOL DownloadDirectory(LPCTSTR lpszDirectory, LPCTSTR lpszDestination) ;
	BOOL DownLoadFile(LPCTSTR lpszRemoteFile, LPCTSTR lpszDestFile);
	int DownLoadFunction();
public:	
	CString m_sRemotePath;
	CString m_sLocalDir;
	long ExecuteCommand(void);
};

class FTP_API CFTPUpLoad : public CCommand 
{
public:
	CFTPUpLoad();
	CFTPUpLoad(CString sLocalPath, CString sRemoteDir);
	virtual ~CFTPUpLoad();
public:
	BOOL RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir);
	BOOL UploadDirectory(LPCTSTR lpszDirectory, LPCTSTR lpszDestination) ;
	BOOL UploadFile(LPCTSTR lpszLocalFile, LPCTSTR lpszDestFile);
	int UploadFunction();
public:
	CString m_sLocalPath;
	CString m_sRemoteDir;
	long ExecuteCommand(void);
};
