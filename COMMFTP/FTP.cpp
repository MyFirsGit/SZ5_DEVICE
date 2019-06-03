/******************************************************************************/
/**	@class CCTFTPLIBApp
*	@brief Main implementation file for the CCTFTPLIBApp application.
*	See CTFTPLIBApp.cpp for the implementation of this class
*	@version 0.01 (2004-10-01): The first version that generate by VS6
*/
/******************************************************************************/

#include "stdafx.h"
#include "FTP.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCTFTPLIBApp

BEGIN_MESSAGE_MAP(CCTFTPLIBApp, CWinApp)
END_MESSAGE_MAP()


// CCTFTPLIBApp construction

/******************************************************************************/
/**	@brief Construction. 
*	@date 10-15-2004 (K.Thu): Create and update this method
*/
/******************************************************************************/
CCTFTPLIBApp::CCTFTPLIBApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCTFTPLIBApp object

CCTFTPLIBApp theApp;


// CCTFTPLIBApp initialization

/******************************************************************************/
/**	@brief The standard initialization of CCTFTPLIBApp.
*	@return TRUE if initialization is successful; otherwise FALSE.
*	@date 2004-10-01 (K. Tuyen): Create and update comment.
*	@remarks
*/
/******************************************************************************/
BOOL CCTFTPLIBApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
/******************************************************************************/
/**	@brief Get Created Time of specified file
*   @param lpszPath : A pointer to a string containing the path of file
*   @param lpszFileName : A pointer to a string containing the name of file
*	@return The created time of file
*	@date 2004-11-08 (K. Khanh): Create.
*/
/******************************************************************************/
CTime GetCreatedTimeOfFile(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	CTime tRet=0;
	CString sPath = lpszPath;

	if(m_pFtpConnection!=NULL)
	{
		CFtpFileFind finder(m_pFtpConnection);
		BOOL bWorking = finder.FindFile ( sPath + _T("/*.*") ) ;
		CString sFName = lpszFileName;
		while (bWorking)
		{
			bWorking = finder.FindNextFile( ) ;
			CString sFileName = finder.GetFileName();
			if ( finder.IsDots( ) ) 
				continue ; 
			if ( sFileName == sFName)
			{ 
				finder.GetCreationTime(tRet);
				break;
			}
		}	
		finder.Close();
	}
	return tRet;
}
