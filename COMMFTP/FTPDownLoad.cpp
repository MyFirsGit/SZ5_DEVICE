/******************************************************************************/
/**	@class CFTPDownLoad
 *	@brief the implementation of functions of download process
 *	@version  
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
#include "stdafx.h"
#include "FTP.h"
#include "afxinet.h"
#include "FTPManager.h"
#include "FTPCommand.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************/
/**	@brief default construction.
*	@date 2004-10-01 (K.Khanh): Create and update comment.
*/
/******************************************************************************/
CFTPDownLoad::CFTPDownLoad()
{
	m_sRemotePath = _T("");
	m_sLocalDir = _T("");
}
/******************************************************************************/
/**	@brief CFTPDownLoad construction.
*  @param sRemotePath : The remote file path (source)
*  @param sLocalDir : The local directory path (destination)
 *	@date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
CFTPDownLoad::CFTPDownLoad(CString sRemotePath, CString sLocalDir)
{
	m_sRemotePath = sRemotePath;
	m_sLocalDir = sLocalDir;

    if(m_pFtpConnection!=NULL )
    {
        if (sRemotePath.Left(1) != _T("/")) {
            CString sDomainName;
            m_pFtpConnection->GetCurrentDirectory(sDomainName);
            m_sRemotePath = sDomainName.TrimRight(_T("/")) + _T("/") + sRemotePath.TrimLeft(_T("/"));
        }
    }
}
/******************************************************************************/
/**	@brief CFTPDownLoad destruction.
 *	@date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
CFTPDownLoad::~CFTPDownLoad()
{
	
}

/******************************************************************************/
/** @brief Download a file or directory from remote server
 *  @return E_NO_ERROR if successful
 *  @return E_DIR_NOT_FOUND : The local directory is not found on local 
 *	machine.
 *  @return E_FILE_NOT_FOUND : The remote file is not found on ftp server.
 *  @return E_OPEN_FOLDER : An error occurred opening current folder on 
 *  ftp server.
 *  @return E_GET_FILE : Can not get file on server.
 *  @return E_OPEN_SERVER : Can not connect to FTP server.
 *  @date 2004-09-30 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
int CFTPDownLoad::DownLoadFunction()
{
	
	if(m_pFtpConnection!=NULL )
	{
		if(ComCheckDirectory(m_sLocalDir)==FALSE)
		{
			return E_DIR_NOT_FOUND;
		}
		else
		{
			if(m_Ftp.FtpFileExists(m_sRemotePath)==FALSE)
			{
				return E_FILE_NOT_FOUND;
			}
			else
			{
				BOOL bRet;
				CString sFileName = m_sRemotePath;
				int nPos = sFileName.ReverseFind('/');
				if (nPos != -1)
				{
					sFileName = sFileName.Mid(nPos+1);
				}
				m_sLocalDir = m_sLocalDir + _T("\\") + sFileName;
				if(m_bIsDirectory==TRUE)
				{
					if (!m_pFtpConnection->SetCurrentDirectory(m_strCurrentDirectory))
					{
						return E_OPEN_FOLDER;
					}
					bRet = DownloadDirectory(m_sRemotePath, m_sLocalDir);
			
				}
				else
				{
					bRet = DownLoadFile(m_sRemotePath, m_sLocalDir	);
				
				}
				if(bRet)
				{
					return E_NO_ERROR;
				}
				else
				{
					return E_GET_FILE;
				}
			}
		}
		
		
	}
	else
	{
			return E_OPEN_SERVER;
	}
}

/******************************************************************************/
/** @brief Download a directory from FTP server and store it on local machine
 *  @param lpszDirectory : The remote directory
 *  @param lpszDestination : The local directory
 *  @return TRUE if successful; otherwise FALSE
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
BOOL CFTPDownLoad::DownloadDirectory(LPCTSTR lpszDirectory, LPCTSTR lpszDestination) 
{
	BOOL bRet ;
	bRet = TRUE;
	CString sDestination;
	CString sDirectory = lpszDirectory;
	CString sDirectory2 = lpszDirectory;
	
	sDirectory2.TrimLeft('/');

	int nPos = sDirectory.ReverseFind('/');
	if (nPos != -1)
	{
		sDirectory = sDirectory.Mid(nPos+1);
	}

	bRet = RecursiveFileList(sDirectory ,sDirectory2, lpszDestination);
	return bRet;	
}
/******************************************************************************/
/** @brief Recursively walk through directory to be able to download complete
 *  directory structure
 *  @param lpszPath : The name of remote directory
 *  @param lpszOrgPath : The path of remote directory
 *  @param lpszDestDir : The path of local directory
 *  @return TRUE if successful; otherwise FALSE
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
BOOL CFTPDownLoad::RecursiveFileList(LPCTSTR lpszPath,LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir)
{
	CFtpFileFind ftpFind(m_pFtpConnection);
	CStringArray sDirectoryArray;
	CString sDestination;
	CString sPrevDirectory;
	/* remember current directory */
	if (!m_pFtpConnection->GetCurrentDirectory(sPrevDirectory))
	{
		return FALSE;
	}
	
	/* go to requested directory */
	if (!m_pFtpConnection->SetCurrentDirectory(lpszPath))
	{
		return FALSE;
	}

	
	/* add directory (without filename), so we can create the directory structure later */
	CString strPath = sPrevDirectory;
	strPath.TrimLeft('/');
	ComGetDestDir(strPath + _T("/") + lpszPath,lpszOrgPath,lpszDestDir,sDestination);
	sDestination.Replace(_T("/"), _T("\\"));
	CreateDirectory(sDestination,0);

	CString sFileName, sPath;	
	BOOL bContinue = ftpFind.FindFile(_T(""));
	if (!bContinue)
	{
		/* the directory is empty; just close up and return. */
		ftpFind.Close();

		/* go back to parent directory */
		m_pFtpConnection->SetCurrentDirectory(!sPrevDirectory.IsEmpty() ? sPrevDirectory : _T("/"));
		return TRUE;
	}

	/* add all files in lpszPath */
	while (bContinue)
	{
		bContinue = ftpFind.FindNextFile();

		if (ftpFind.IsDots())
			continue;
		
		/* is selected item a directory ? */
		if (ftpFind.IsDirectory())
		{
			CString tmp = ftpFind.GetFileName();
			sDirectoryArray.Add(ftpFind.GetFileName());
		}
		else
		{
			
			sFileName = ftpFind.GetFileName();
			sPath = sPrevDirectory + _T("/") + lpszPath;
			
			if(FALSE==DownLoadFile(sPath+_T("/")+ sFileName, sDestination+_T("\\")+sFileName))
			{
				return FALSE;
			}
		}
	}
	ftpFind.Close();

	/* get contains of directories */
	for (int i = 0; i < sDirectoryArray.GetSize(); i++)
	{
		if (!RecursiveFileList(sDirectoryArray.GetAt(i), lpszOrgPath, lpszDestDir))
		{
			m_pFtpConnection->SetCurrentDirectory(!sPrevDirectory.IsEmpty() ? sPrevDirectory : _T("/"));
			return FALSE;
		}
	}

	/* go back to parent directory */
	m_pFtpConnection->SetCurrentDirectory(!sPrevDirectory.IsEmpty() ? sPrevDirectory : _T("/"));
	
	return TRUE;
}
/******************************************************************************/
/** @brief Download a file from FTP server and store it on local machine
 *  @param lpszRemoteFile : The remote file
 *  @param lpszDestFile : The local file
 *  @return TRUE if successful; otherwise FALSE
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
BOOL CFTPDownLoad::DownLoadFile(LPCTSTR lpszRemoteFile, LPCTSTR lpszDestFile)
{
	if(NULL == m_pFtpConnection) return FALSE;

	BOOL bRet ;
	bRet = m_pFtpConnection->GetFile(lpszRemoteFile, lpszDestFile, FALSE);
	return bRet;
}



/******************************************************************************/
/** @brief Download a file or directory from ftp server
*  @return 0 if successful; otherwise nonzero
*  @date 2004-10-01 (K.Khanh): Create and update comment.
*  @remarks
*/
/******************************************************************************/
long CFTPDownLoad::ExecuteCommand(void)
{
	int iRet = -1;
	m_sRemotePath.TrimRight(_T("/"));
	m_sRemotePath.TrimRight(_T("\\"));
	m_sLocalDir.TrimRight(_T("/"));
	m_sLocalDir.TrimRight(_T("\\"));
	m_bIsDirectory =  FALSE;

	if(( m_pFtpConnection!=NULL))
	{
        DWORD dwStartTime = ::GetTickCount(); //Starting timer

        while (true)
        {
            iRet = DownLoadFunction();
            if (E_NO_ERROR == iRet)
            {
                break;
            }

            //Check timeout
            if (IsTimeOut(dwStartTime))
            {
                iRet = E_INTERNET_TIMEOUT;
                break;
            }
        }
	}
	else
	{
		iRet = E_CONNECTION_LOST;
	}

	OnComplete(iRet);

    if (iRet != 0)
    {
        m_Ftp.Close();
    }

	return iRet;
}
