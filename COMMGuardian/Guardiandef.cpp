/******************************************************************************/
/**	@class CTGuardiandef
*	@brief The implementation of CTGuardiandef class
*	@version  
*	@date 2004-10-08 (K.Khanh): Create and update comment.
*/
/******************************************************************************/

#include "stdafx.h"
#include "Guardiandef.h"
#include "shlwapi.h"
#include <Tlhelp32.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString m_strCurrentDirectory;

/******************************************************************************/
/** @brief Copies an existing directory to a new directory
*  @param lpszSourceDir : The path of existing directory
*  @param lpszDestDir : The path of new directory
*  @return TRUE if successful; otherwise FALSE
*  @date 2004-10-08 (K.Khanh): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
BOOL CopyDirectory(LPCTSTR lpszSourceDir, LPCTSTR lpszDestDir)
{
	BOOL bRet = TRUE;
	CString sDestination;

	if(FALSE == ComCheckDirectory(lpszSourceDir))
	{
		return FALSE;
	}

	if(FALSE == ComCheckDirectory(lpszDestDir))
	{
		return FALSE;
	}

	CString sDirectory = lpszSourceDir;
	CString sDirectory2 = lpszSourceDir;
	sDirectory2.TrimLeft('\\');

	int nPos = sDirectory.ReverseFind('\\');
	if (nPos != -1)
	{
		sDirectory = sDirectory.Mid(nPos+1);
	}

	ComSetCurrentDir(lpszSourceDir,m_strCurrentDirectory);
	bRet = RecursiveFileList(sDirectory,sDirectory2, lpszDestDir, TRUE);
	return bRet;	
}

/******************************************************************************/
/** @brief Recursively walk through directory to be able to copy complete
*  directory structure
*  @param lpszPath : The name of existing directory
*  @param lpszOrgPath : The path of existing directory
*  @param lpszDestDir : The path of new directory
*  @param bIsFirst : FALSE If the existing directory is a sub folder; 
*  otherwise TRUE
*  @return TRUE if successful; otherwise FALSE
*  @date 2004-10-08 (K.Khanh): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
BOOL RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir, BOOL bIsFirst)
{
	
	CFileFind finder;
	CStringArray sDirectoryArray;
	CString sDestination;
	CString sPrevDirectory;

	/* remember current directory */
	sPrevDirectory = m_strCurrentDirectory;
	
	/* go to requested directory */
	m_strCurrentDirectory = m_strCurrentDirectory + _T("\\") + lpszPath;

	/* add directory (without filename), so we can create the directory structure later */
	CString strPath = sPrevDirectory;
	
	ComGetDestDir(strPath + _T("\\") + lpszPath,lpszOrgPath,lpszDestDir,sDestination);
	if(bIsFirst==FALSE)
	{
		CreateDirectory(sDestination,0);
	}
	CString sFileName, sPath;
	BOOL bContinue = finder.FindFile(m_strCurrentDirectory + _T("\\*.*"));
	if (!bContinue)
	{
		/* the directory is empty; just close up and return. */
		finder.Close();
		
		/* go back to parent directory */
		m_strCurrentDirectory = sPrevDirectory;
		return TRUE;
	}

	while (bContinue)
	{
		bContinue = finder.FindNextFile();

		if (finder.IsDots())
			continue;
		
		/* is selected item a directory ? */
		if (finder.IsDirectory())
		{
			sDirectoryArray.Add(finder.GetFileName());
		}
		else
		{
			sFileName = finder.GetFileName();
			sPath = sPrevDirectory + _T("\\") + lpszPath;
			CString sLocalFileName = RenServerName2LocalName(sFileName);

			if(sLocalFileName != _T(""))
			{
                SetFileAttributes(sDestination+_T("\\")+ sLocalFileName,FILE_ATTRIBUTE_NORMAL);
				if(FALSE == CopyFile(sPath+_T("\\")+ sFileName, sDestination+_T("\\")+ sLocalFileName, FALSE))
				{
					return FALSE;
				}
			}
		}
	}
	finder.Close();

	/* get contains of directories */
	for (int i = 0; i < sDirectoryArray.GetSize(); i++)
	{
		if (FALSE == RecursiveFileList(sDirectoryArray.GetAt(i), lpszOrgPath, lpszDestDir, FALSE))
		{
			m_strCurrentDirectory = sPrevDirectory;
			return FALSE;
		}
	}

	/* go back to parent directory */
	m_strCurrentDirectory = sPrevDirectory;
	
	return TRUE;
}

/******************************************************************************/
/**
*  @brief Rename server file name to local file name.
*  @param sServerFileName: file name on server (ex: CSTI_OU_01_01_01.exe)
*  @return corespondent local name. 
*  @date 2005-14-03 (K.Thu): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
CString RenServerName2LocalName(CString sServerFileName){

	CString sLocalName = _T("");
	
	//get unit modle type
	sLocalName = sServerFileName;

	return sLocalName;
}

/******************************************************************************/
/** @brief Check file is exist
*  @param lpszFileName : Buffer for file need to check
*  @return TRUE if file exist else return FALSE
* @date 2004-09-17 (K.Tuyen): Create and update comment.
*  @remarks
*/
/******************************************************************************/
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
/******************************************************************************/
/** @brief Determine if the local path is a directory
*  @param lpszDirName : The local path
*  @return TRUE if successful; otherwise FALSE
*  @date 2004-10-01 (K.Khanh): Create and update comment.
*  @date 2006-04-04 (nijun): update
*  @remarks None
*/
/******************************************************************************/
BOOL ComCheckDirectory(LPCTSTR lpszDirName)
{
    if (PathIsDirectory(lpszDirName))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/**	@brief Run an application.
*	@param lpszAppPath: Path of application
*	@param lpszAppParameter: Parameter of application
*	@return true: if run success.
*	@return false: fail.
*	@date 2004-10-08 (K. Tuyen): Create.
*/
/******************************************************************************/
BOOL ComExecuteApp(LPCTSTR lpszAppPath, LPCTSTR lpszAppParameter)
{

	_ASSERTE(lpszAppPath != NULL);
	_ASSERTE(lpszAppParameter != NULL);

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
		CloseHandle(shi.hProcess);
	}

	return bSuccess;
}

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
		if(_tcscmp(pe.szExeFile,name)   ==   0)   
		{   
			id   =   pe.th32ProcessID;   
			break;   
		}   

	}   while(1);   
	CloseHandle(hSnapshot);   
	return   id;   
}  

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

/******************************************************************************/
/** @brief Get destinating path
*  @param sPath : The root path
*  @param sOrgPath : The original path
*  @param sDestDir : The destinating directory
*  @param sDest : The destinating path
*  @return None
*  @date 2004-10-08 (K.Khanh): Create and update comment.
*  @remarks
*/
/******************************************************************************/
void ComGetDestDir(CString sPath, CString sOrgPath, CString sDestDir, CString &sDest)
{
	sDest = sPath;
	sDest.TrimLeft('/');
	sDest.TrimLeft('\\');
	int nPos = sOrgPath.GetLength();
	sDest =  sDest.Mid(nPos);
	sDest = sDestDir + sDest;
}
/******************************************************************************/
/** @brief Keep current directory on local machine
*  @param lpszDirName : The current directory
*  @param sCurrentDir : The variable which keeps current directory path
*  @return None
*  @date 2004-10-08 (K.Khanh): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
void ComSetCurrentDir(LPCTSTR lpszDirName, CString &sCurrentDir)
{
	CString sDir = lpszDirName;
	int iPos = sDir.ReverseFind ( '\\' ) ;
	sCurrentDir = sDir.Left ( iPos + 1 ) ;
	sCurrentDir.TrimRight('\\');
}
//-------------------------------------------------------------------------
/**
 * @brief Read data from ini file
 * @param lpszSection: section of read data
 * @param lpszKey: key of read data
 * @param CString sDefault: default value
 * @return value of the key as an CString
 * @since 2004-03-26 (Khang Luu)
 */
//-------------------------------------------------------------------------
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