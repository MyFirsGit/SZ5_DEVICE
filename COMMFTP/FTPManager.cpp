/******************************************************************************/
/**	@class CFTPManager
 *	@brief The implementation of CFTPManager class
 *	@version  
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
#include "stdafx.h"
#include "FTP.h"
#include "FTPManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/******************************************************************************/
/**	@brief CFTPManager construction.
 *	@date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/

CFTPManager::CFTPManager()
{

}
/******************************************************************************/
/**	@brief CFTPManager destruction.
 *	@date 2004-10-01 (K.Khanh): Create and update comment.
 */
/******************************************************************************/
CFTPManager::~CFTPManager()
{

}
/******************************************************************************/
/** @brief Connect to FTP server
 *  @param lpszSiteName : FTP Server Name 
 *	@param lpszUserName : User Name is used to login FTP server
 *	@param lpszPassword : Password is used to login FTP server
 *	@param iPort : The TCP/IP port to use on the server
 *	@param bPassMode : Specifies passive or active mode for this FTP session
 *  @return 0 if successful; otherwise nonzero
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
int CFTPManager::Connect(LPCTSTR lpszSiteName, LPCTSTR lpszUserName, LPCTSTR lpszPassword, int iPort,  BOOL bPassMode )
{
	
	CString  sServerName;
	CString  sAddress;
	CString  sUserName;
	CString  sPassword;
	CString  sObject;
	BOOL     bMode;
	INTERNET_PORT nPort;
	DWORD dwServiceType;
	sAddress = lpszSiteName;
	sUserName = lpszUserName;
	sPassword = lpszPassword;	
	bMode = bPassMode;
	
	/* check to see if this is a reasonable URL */

	if (!AfxParseURL(sAddress, dwServiceType, sServerName, sObject, nPort))
	{
		/* try adding the _T("ftp://") protocol */
		CString sFtpURL = _T("ftp://");
		sFtpURL += sAddress;

		if (!AfxParseURL(sFtpURL, dwServiceType, sServerName, sObject, nPort))
		{
			return E_INVALID_URL;
		}
	}
	/* Now open a FTP connection to the server */
	if ((dwServiceType == INTERNET_SERVICE_FTP) && !sServerName.IsEmpty())
	{
			try
			{
				m_pintsession = new CInternetSession (NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
				int nTimeout = AfxGetApp()->GetProfileInt(_T("Settings"), _T("ConnectionTimeout"), 30);
				m_pintsession->SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeout * 1000);
				m_pintsession->SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, nTimeout * 1000);
				m_pintsession->SetOption(INTERNET_OPTION_SEND_TIMEOUT, nTimeout * 1000);
				m_pFtpConnection = m_pintsession->GetFtpConnection(sServerName, sUserName, sPassword, iPort,bMode);
			}
			catch (CInternetException* pEx)
			{
				int nErr = GetLastError(pEx->m_dwError);
                pEx->Delete();
				return nErr;
			}
	}
	else
	{
			return E_INVALID_URL;
	}
	return E_NO_ERROR;
}

/******************************************************************************/
/** @brief Determine if the remote path exists
 *  @param lpszPath : The remote path
 *  @return TRUE if successful; otherwise FALSE
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
BOOL CFTPManager::FtpFileExists(LPCTSTR lpszPath)
{
	
	CString sFilePath = lpszPath;
	if(sFilePath.IsEmpty())
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	CFtpFileFind finder(m_pFtpConnection);
	int iPos = sFilePath.ReverseFind ( '/' ) ;
	
	CString sPath = sFilePath.Left ( iPos + 1 ) ;
	CString sFirst = sFilePath.Left(1);
	if(sFirst!='/')
	{
		return FALSE;
	}
	m_strCurrentDirectory = sPath;
	/* Lets check for the files existing first using a standard FindFile call */
	BOOL bWorking = finder.FindFile ( sPath + _T("*.*") );
	while (bWorking)
	{
		bWorking = finder.FindNextFile( ) ;
		CString sFname = finder.GetFilePath( ) ;
		CString sFileName = finder.GetFileName();
		if ( finder.IsDots( ) ) 
		continue ; 
		if ( sFname == sFilePath)
		{ 
			bRet = TRUE;
			if(finder.IsDirectory())
			{
				m_bIsDirectory = TRUE; 
			}
			break;
		}
	}	
	finder.Close();
return bRet;
}


/******************************************************************************/
/** @brief Get defined error code
 *  @param dwErrorCode : Error code of system
 *  @return E_INTERNET_OUT_OF_HANDLES : No more handles could be generated
 *  @return E_INTERNET_TIMEOUT : The request has timed out
 *  @return E_INTERNET_INVALID_URL : The URL is invalid
 *  @return E_INTERNET_UNRECOGNIZED_SCHEME : The URL scheme could not be
 *  recognized or is not supported.
 *  @return E_INTERNET_NAME_NOT_RESOLVED : The server name could not be
 *  resolved.
 *  @return E_INTERNET_PROTOCOL_NOT_FOUND : The requested protocol could 
 *  not be located.
 *  @return E_INTERNET_INVALID_OPTION : A request to InternetQueryOption
 *  or InternetSetOption specified an invalid option value.
 *  @return E_INTERNET_BAD_OPTION_LENGTH : InternetSetOption is incorrect
 *  for the type of option specified.
 *  @return E_INTERNET_OPTION_NOT_SETTABLE : The request option cannot
 *  be set.
 *  @return E_INTERNET_SHUTDOWN : The Win32 Internet function support
 *  is being shut down or unloaded.
 *  @return E_INTERNET_INCORRECT_USER_NAME : The request to connect and
 *  log on to an FTP server could not be completed because the supplied
 *  user name is incorrect.
 *  @return E_INTERNET_INCORRECT_PASSWORD: The request to connect and
 *  log on to an FTP server could not be completed because the supplied
 *  password is incorrect.
 *  @return E_INTERNET_LOGIN_FAILURE : The request to connect to
 *  and log on to an FTP server failed.
 *  @return E_INTERNET_INVALID_OPERATION : The requested operation is 
 *  invalid.
 *  @return E_INTERNET_OPERATION_CANCELLED : The operation was canceled.
 *  @return E_INTERNET_INCORRECT_HANDLE_TYPE : The type of handle 
 *  supplied is incorrect for this operation.
 *  @return E_INTERNET_INCORRECT_HANDLE_STATE : The requested operation
 *  cannot be carried out because the handle supplied is not in the
 *  correct state.
 *  @return E_INTERNET_NOT_PROXY_REQUEST : The request cannot be made
 *  via a proxy.
 *  @return E_INTERNET_REGISTRY_VALUE_NOT_FOUND : A required registry
 *  value could not be located.
 *  @return E_INTERNET_BAD_REGISTRY_PARAMETER : A required registry 
 *  value was located but is an incorrect type or has an invalid value.
 *  @return E_INTERNET_NO_DIRECT_ACCESS : Direct network access cannot
 *  be made at this time.
 *  @return E_INTERNET_NO_CONTEXT : An asynchronous request could not
 *  be made because a zero context value was supplied.
 *  @return E_INTERNET_NO_CALLBACK : An asynchronous request could not
 *  be made because a callback function has not been set.
 *  @return E_INTERNET_REQUEST_PENDING : The required operation could
 *  not be completed because one or more requests are pending.
 *  @return E_INTERNET_INCORRECT_FORMAT : The format of the request
 *  is invalid.
 *  @return E_INTERNET_ITEM_NOT_FOUND : The requested item could not
 *  be located.
 *  @return E_INTERNET_CANNOT_CONNECT : The attempt to connect to the
 *  server failed.
 *  @return E_INTERNET_CONNECTION_ABORTED : The connection with the server
 *  has been terminated.
 *  @return E_INTERNET_CONNECTION_RESET : The connection with the server
 *  has been reset.
 *  @return E_INTERNET_FORCE_RETRY : Calls for the Win32 Internet function
 *  to redo the request.
 *  @return E_INTERNET_INVALID_PROXY_REQUEST : The request to the proxy 
 *  was invalid.
 *  @return E_INTERNET_HANDLE_EXISTS : The request failed because the 
 *  handle already exists.
 *  @return E_FTP_TRANSFER_IN_PROGRESS : The requested operation cannot
 *  be made on the FTP session handle because an operation is already
 *  in progress.
 *  @return E_FTP_DROPPED : The FTP operation was not completed because
 *  the session was aborted.
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
int CFTPManager::GetLastError(DWORD dwErrorCode)
{
	int nRet;
	switch(dwErrorCode)
	{
		case ERROR_INTERNET_OUT_OF_HANDLES:
			nRet = E_INTERNET_OUT_OF_HANDLES;
			break;
		case ERROR_INTERNET_TIMEOUT:
			nRet = E_INTERNET_TIMEOUT;
			break;
		case ERROR_INTERNET_INVALID_URL:
			nRet = E_INTERNET_INVALID_URL;
			break;
		case ERROR_INTERNET_UNRECOGNIZED_SCHEME:
			nRet = E_INTERNET_UNRECOGNIZED_SCHEME;
			break;
		case ERROR_INTERNET_NAME_NOT_RESOLVED:
			nRet = E_INTERNET_NAME_NOT_RESOLVED;
			break;
		case ERROR_INTERNET_PROTOCOL_NOT_FOUND:
			nRet = E_INTERNET_PROTOCOL_NOT_FOUND;
			break;
		case ERROR_INTERNET_INVALID_OPTION:
			nRet = E_INTERNET_INVALID_OPTION;
			break;
		case ERROR_INTERNET_BAD_OPTION_LENGTH:
			nRet = E_INTERNET_BAD_OPTION_LENGTH;
			break;
		case ERROR_INTERNET_OPTION_NOT_SETTABLE:
			nRet = E_INTERNET_OPTION_NOT_SETTABLE;
			break;
		case ERROR_INTERNET_SHUTDOWN:
			nRet = E_INTERNET_SHUTDOWN;
			break;
		case ERROR_INTERNET_INCORRECT_USER_NAME:
			nRet = E_INTERNET_INCORRECT_USER_NAME;
			break;
		case ERROR_INTERNET_INCORRECT_PASSWORD:
			nRet = E_INTERNET_INCORRECT_PASSWORD;
			break;
		case ERROR_INTERNET_LOGIN_FAILURE:
			nRet = E_INTERNET_LOGIN_FAILURE;
			break;
		case ERROR_INTERNET_INVALID_OPERATION:
            nRet = E_INTERNET_INVALID_OPERATION;
			break;
		case ERROR_INTERNET_OPERATION_CANCELLED:
            nRet = E_INTERNET_OPERATION_CANCELLED;
			break;
		case ERROR_INTERNET_INCORRECT_HANDLE_TYPE:
            nRet = E_INTERNET_INCORRECT_HANDLE_TYPE;
			break;
		case ERROR_INTERNET_INCORRECT_HANDLE_STATE:
            nRet = E_INTERNET_INCORRECT_HANDLE_STATE;
			break;
		case ERROR_INTERNET_NOT_PROXY_REQUEST:
            nRet = E_INTERNET_NOT_PROXY_REQUEST;
			break;
		case ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND:
            nRet = E_INTERNET_REGISTRY_VALUE_NOT_FOUND;
			break;
		case ERROR_INTERNET_BAD_REGISTRY_PARAMETER:
            nRet = E_INTERNET_BAD_REGISTRY_PARAMETER;
			break;
		case ERROR_INTERNET_NO_DIRECT_ACCESS:
            nRet = E_INTERNET_NO_DIRECT_ACCESS;
			break;
		case ERROR_INTERNET_NO_CONTEXT:
            nRet = E_INTERNET_NO_CONTEXT;
			break;
		case ERROR_INTERNET_NO_CALLBACK:
            nRet = E_INTERNET_NO_CALLBACK;
			break;
		case ERROR_INTERNET_REQUEST_PENDING:
            nRet = E_INTERNET_REQUEST_PENDING;
			break;
		case ERROR_INTERNET_INCORRECT_FORMAT:
            nRet = E_INTERNET_INCORRECT_FORMAT;
			break;
		case ERROR_INTERNET_ITEM_NOT_FOUND:
            nRet = E_INTERNET_ITEM_NOT_FOUND;
			break;
		case ERROR_INTERNET_CANNOT_CONNECT:
            nRet = E_INTERNET_CANNOT_CONNECT;
			break;
		case ERROR_INTERNET_CONNECTION_ABORTED:
            nRet = E_INTERNET_CONNECTION_ABORTED;
			break;
		case ERROR_INTERNET_CONNECTION_RESET:
            nRet = E_INTERNET_CONNECTION_RESET;
			break;
		case ERROR_INTERNET_FORCE_RETRY:
            nRet = E_INTERNET_FORCE_RETRY;
			break;
		case ERROR_INTERNET_INVALID_PROXY_REQUEST:
            nRet = E_INTERNET_INVALID_PROXY_REQUEST;
			break;
		case ERROR_INTERNET_HANDLE_EXISTS:
            nRet = E_INTERNET_HANDLE_EXISTS;
			break;
		case ERROR_FTP_TRANSFER_IN_PROGRESS:
            nRet = E_FTP_TRANSFER_IN_PROGRESS;
			break;
		case ERROR_FTP_DROPPED:
            nRet = E_FTP_DROPPED;
			break;
		default:
			nRet = dwErrorCode;
			break;
	}
	return nRet;
}
/******************************************************************************/
/** @brief End the download/ upload process, and release all resource
 *  @param : None
 *  @return None
 *  @date 2004-10-01 (K.Khanh): Create and update comment.
 *  @remarks None
 */
/******************************************************************************/
void CFTPManager::Close()
{
	if(m_pFtpConnection!= NULL)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
		m_pFtpConnection = NULL;
	}
	if(m_pintsession!= NULL)
	{
		m_pintsession->Close();
		delete m_pintsession;
		m_pintsession = NULL;
	}
}

