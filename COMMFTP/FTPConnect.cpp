/******************************************************************************/
/**	@class CFTPConnect
*	@brief implementation file of CFTPConnect class.
*	@version 0.01 (2004-10-01): The first version that generate by VC .Net
*/
/******************************************************************************/
#include "StdAfx.h"
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
CFTPConnect::CFTPConnect(void)
{
	m_sHostID = "";
	m_sUserID = "";
	m_sPassword = "";
	m_nPort = 0;
}

/******************************************************************************/
/** @brief construction
*  @param sHostID : FTP Server Name 
*  @param sUserID : User Name is used to login FTP server
*  @param sPassword : Password is used to login FTP server
*  @param nPort : The TCP/IP port to use on the server
*  @date 2004-10-01 (K.Khanh): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
CFTPConnect::CFTPConnect(CString sHostID,CString sUserID,CString sPassword,int nPort)
{
	m_sHostID = sHostID;
	m_sUserID = sUserID;
	m_sPassword = sPassword;
	m_nPort = nPort;
}

/******************************************************************************/
/**	@brief CFTPConnect destruction.
*	@date 2004-10-01 (K.Khanh): Create and update comment.
*/
/******************************************************************************/
CFTPConnect::~CFTPConnect(void)
{
}

/******************************************************************************/
/** @brief Connect to FTP server
*  @return 0 if successful; otherwise nonzero
*  @date 2004-10-01 (K.Khanh): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
long CFTPConnect::ExecuteCommand(void)
{
	int iRet;
	BOOL bPasvMode = TRUE;
	iRet = m_Ftp.Connect(m_sHostID,m_sUserID,m_sPassword,m_nPort,bPasvMode);
	OnComplete(iRet);

    if (iRet != 0)
    {
        m_Ftp.Close();
    }

	return iRet;
}
