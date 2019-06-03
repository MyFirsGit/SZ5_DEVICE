/******************************************************************************/
/**	@class CFTPClose
*	@brief implementation file of CFTPClose class.
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
CFTPClose::CFTPClose(void)
{
}

/******************************************************************************/
/**	@brief CFTPClose destruction.
*	@date 2004-10-01 (K.Khanh): Create and update comment.
*/
/******************************************************************************/
CFTPClose::~CFTPClose(void)
{
}

/******************************************************************************/
/** @brief End the download/ upload process, and release all resource
*  @param : None
*  @return None
*  @date 2004-10-01 (K.Khanh): Create and update comment.
*  @remarks None
*/
/******************************************************************************/
long CFTPClose::ExecuteCommand(void)
{
    m_Ftp.Close();
	OnComplete(E_NO_ERROR);
	return 0;
}
