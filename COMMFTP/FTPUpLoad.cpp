/******************************************************************************/
/**	@class CFTPUpLoad
 *	@brief the implementation of functions of upload process
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
CFTPUpLoad::CFTPUpLoad()
{
	m_sLocalPath = _T("");
	m_sRemoteDir = _T("");
}
/******************************************************************************/
/**	@brief CFTPUpLoad construction.
*  @param sLocalPath : The local file path (source)
*  @param sRemoteDir : The remote directory path (destination)
 *	@date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
CFTPUpLoad::CFTPUpLoad(CString sLocalPath, CString sRemoteDir)
{
	m_sLocalPath = sLocalPath;
    m_sRemoteDir = sRemoteDir;

    if(m_pFtpConnection!=NULL )
    {
        CString sDomainName;
        m_pFtpConnection->GetCurrentDirectory(sDomainName);
        m_sRemoteDir = sDomainName.TrimRight(_T("/")) + _T("/") + sRemoteDir.TrimLeft(_T("/"));
    }
}
/******************************************************************************/
/**	@brief CFTPUpLoad destruction.
 *	@date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
CFTPUpLoad::~CFTPUpLoad()
{
	
}
/******************************************************************************/
/** @brief Upload a file or directory on remote server
 *  @param : None
 *  @return E_NO_ERROR : if successful
 *  @return E_LOCAL_PATH_NOT_FOUND : The local file is not found on local 
 *  machine.
 *  @return E_RETMOTE_DIR_NOT_FOUND : The remote directory can not found on server.
 *  @return E_PUT_FILE : Can not put File on server
 *  @return E_OPEN_SERVER : Can not connect to FTP server
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
int CFTPUpLoad::UploadFunction()
{
	
	if(m_pFtpConnection!=NULL )
	{
		if(ComCheckFileExist(m_sLocalPath)==FALSE)
		{
			return E_LOCAL_PATH_NOT_FOUND;
		}
		else
		{
			if(m_sRemoteDir.IsEmpty())
			{
				m_bIsDirectory=TRUE;
				m_sRemoteDir = '/' + m_sRemoteDir;
			}
			else
			{
				if(m_Ftp.FtpFileExists(m_sRemoteDir)==FALSE)
				{
					return E_RETMOTE_DIR_NOT_FOUND;
				}
			}
			
			
			if(m_bIsDirectory==TRUE)
			{
				BOOL bRet;
				CString sFileName = m_sLocalPath;
				int nPos = sFileName.ReverseFind('\\');
				if (nPos != -1)
				{
					sFileName = sFileName.Mid(nPos+1);
				}
				m_pFtpConnection->SetCurrentDirectory(m_sRemoteDir);
				m_sRemoteDir = m_sRemoteDir +_T("/") + sFileName;
				if(ComCheckDirectory(m_sLocalPath)==TRUE)
				{
					bRet = UploadDirectory(m_sLocalPath, m_sRemoteDir);
			
				}
				else
				{
					bRet = UploadFile(m_sLocalPath, m_sRemoteDir);
				
				}
				if(bRet)
				{
					return E_NO_ERROR;
				}
				else
				{
					return E_PUT_FILE;
				}
			}
			else
			{
				return E_RETMOTE_DIR_NOT_FOUND;
			}
		}
					
		
	}
	else
	{
			return E_OPEN_SERVER;
	}
}

/******************************************************************************/
/** @brief Upload a directory on FTP server
 *  @param lpszDirectory : The local directory
 *  @param lpszDestination : The remote directory
 *  @return TRUE if successful; otherwise FALSE
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks
 */
/******************************************************************************/
BOOL CFTPUpLoad::UploadDirectory(LPCTSTR lpszDirectory, LPCTSTR lpszDestination) 
{
	BOOL bRet ;
	bRet = TRUE;
	CString sDestination;
	CString sDirectory = lpszDirectory;
	CString sDirectory2 = lpszDirectory;
	
	sDirectory2.TrimLeft('\\');

	int nPos = sDirectory.ReverseFind('\\');
	if (nPos != -1)
	{
		sDirectory = sDirectory.Mid(nPos+1);
	}

	ComSetCurrentDir(lpszDirectory, m_strCurrentDirectory);
	bRet = RecursiveFileList(sDirectory, sDirectory2, lpszDestination);
	return bRet;	
}
/******************************************************************************/
/** @brief Recursively walk through directory to be able to download complete
 *  directory structure
 *  @param lpszPath : The name of local directory
 *  @param lpszOrgPath : The path of local directory
 *  @param lpszDestDir : The path of remote directory
 *  @return TRUE if successful; otherwise FALSE
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
BOOL CFTPUpLoad::RecursiveFileList(LPCTSTR lpszPath, LPCTSTR lpszOrgPath, LPCTSTR lpszDestDir)
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
	sDestination.Replace(_T("\\"), _T("/"));
	m_pFtpConnection->CreateDirectory(sDestination);
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

	/* add all files in lpszPath */
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
			if(FALSE==UploadFile(sPath+_T("\\")+ sFileName, sDestination+_T("/")+sFileName))
			{
				return FALSE;
			}
		
		}
	}
	finder.Close();

	/* get contains of directories */
	for (int i = 0; i < sDirectoryArray.GetSize(); i++)
	{
		if (!RecursiveFileList(sDirectoryArray.GetAt(i),lpszOrgPath, lpszDestDir))
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
/** @brief Upload a file on FTP server
 *  @param lpszLocalFile : The local file
 *  @param lpszDestFile : The remote file
 *  @return TRUE if successful, otherwise FALSE
 *  @date 2004-10-01 (K.Khanh) : Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
BOOL CFTPUpLoad::UploadFile(LPCTSTR lpszLocalFile, LPCTSTR lpszDestFile)
{
	BOOL bRet ;
	//CString sRemoteDir = lpszDestFile;
	//int nPos = sRemoteDir.ReverseFind('/');
	//if (nPos != -1)
	//{
	//	sRemoteDir = sRemoteDir.Mid(0,nPos);
	//}
	//bRet = m_pFtpConnection->SetCurrentDirectory(sRemoteDir);
	bRet = m_pFtpConnection->PutFile(lpszLocalFile, lpszDestFile, FTP_TRANSFER_TYPE_BINARY,1);
	return bRet;
}



/******************************************************************************/
/** @brief Upload a file or directory on ftp server
*  @return 0 if successful; otherwise nonzero
*  @date 2004-10-01 (K.Khanh): Create and update comment.
*  @remarks
*/
/******************************************************************************/
long CFTPUpLoad::ExecuteCommand(void)
{
	int iRet = -1;
	m_sRemoteDir.TrimRight(_T("/"));
	m_sRemoteDir.TrimRight(_T("\\"));
	m_sLocalPath.TrimRight(_T("/"));
	m_sLocalPath.TrimRight(_T("\\"));
	m_bIsDirectory =  FALSE;

	if(( m_pFtpConnection!=NULL))
	{
        DWORD dwStartTime = ::GetTickCount(); //Starting timer

        while (true)
        {
            iRet = UploadFunction();
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
