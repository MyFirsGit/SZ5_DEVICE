// UtilThread.cpp : implementation file
//

#include "stdafx.h"
//#include "UTILSLIB.h"
#include "UtilThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CUtilThread

IMPLEMENT_DYNCREATE(CUtilThread, CWinThread)

CUtilThread::CUtilThread():CWinThread()
{
}

CUtilThread::~CUtilThread()
{
}

BOOL CUtilThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CUtilThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUtilThread, CWinThread)
END_MESSAGE_MAP()


// CUtilThread message handlers

/******************************************************************************/
/**
*	@brief This function will send message to register windows.
*	@param hWnd : The window handle which is to receive message.
*	@param uiMessage : Specifies the message to be posted.
*	@param wParam : WPARAM of message
*	@param lParam : LPARAM of message
*	@date 10/2004.
*/
/******************************************************************************/
BOOL CUtilThread::PostMessageEx(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	return ::PostMessage (hWnd, uiMessage, wParam, lParam);
}


BOOL CUtilThread::PostThreadMessageEx(DWORD dwThreadID, 
									  UINT uiMessage, 
									  WPARAM wParam, 
									  LPARAM lParam)
{
	return ::PostThreadMessage (dwThreadID, uiMessage, wParam, lParam);
}

CUtilThread *CUtilThread::StartThread(CRuntimeClass *pRunTimeThread, 
									  LPSTR sClassName)
{
	CUtilThread *pUtilThread = NULL;
	TRY
	{
		if ( strcmp(pRunTimeThread->m_lpszClassName , sClassName)) 
		{
			return pUtilThread;				
		}
		pUtilThread = (CUtilThread *)::AfxBeginThread(pRunTimeThread);
	}
	CATCH (CException, pCException) 
	{
		THROW_LAST() ;
	}
	END_CATCH
		return pUtilThread;
}

/******************************************************************************/
/**	
*	@brief Shutdown specify thread.
*	@return 
*		- 0 success to shutdown thread. 
*		- Other: fail to shutdown thread.
*	@date 023/02/2004.
*/
/******************************************************************************/
LONG CUtilThread::ShutdownThread(CUtilThread *pUtilThread,DWORD dwMilliseconds)
{
	LONG lReturn = 0;
	if (pUtilThread != NULL)
	{
		if (!::PostThreadMessage (pUtilThread->m_nThreadID, WM_QUIT, 0, 0))
		{
			lReturn = 1;
		}
	}
	else
	{
		lReturn = 1;
	}
	// if thread is safe to shutdown
	if (lReturn == 0)
	{
		// Wait until this thread completly close
		HANDLE hThread = pUtilThread->m_hThread;
		//::WaitForSingleObject (hThread, INFINITE);	//???
		int waitCode = ::WaitForSingleObject (hThread, dwMilliseconds);	//???
		if (waitCode == WAIT_TIMEOUT){
			lReturn = 2;
			::TerminateThread(hThread, NULL);
		}
	}
	return lReturn;
}

/******************************************************************************/
/**	@brief Initilize the thread.
*	@return 
*		- FALSE if error to start thread. 
*		- TRUE is success to create thread.
*	@date 023/02/2004.
*/
/******************************************************************************/
BOOL CUtilThread::StartThread(CUtilThread *pUtilThread)
{
	BOOL bSuccess = TRUE;
	if (pUtilThread != NULL)
	{
		pUtilThread->CreateThread();
	}
	else
	{
		bSuccess = FALSE;
	}

	return bSuccess;

}