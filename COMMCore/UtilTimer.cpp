#include "StdAfx.h"
#include "utiltimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUtilTimer::CUtilTimer(void)
{
	m_dwLastTime = 0;
	m_dwTimeOut = 0;
}

CUtilTimer::~CUtilTimer(void)
{
}

CUtilTimer::CUtilTimer(const DWORD &dwTimeOut)
{
	/* Get the first timer */
	m_dwLastTime = GetTickCount();
	m_dwTimeOut = dwTimeOut;
}

/******************************************************************************/
/**
*	@brief Check the current time is time out. 
*	@return Timer is out
*	@date 10/2004
*/
/******************************************************************************/

BOOL CUtilTimer::isTimeOut()
{
	/* Get current time */
	DWORD dwCurTime = GetTickCount();
	/* Get time running */
	DWORD dwTime = 	dwCurTime - m_dwLastTime;
	/* If time is out */
	if(m_dwTimeOut < dwTime) 
	{
		return TRUE;
	}
	return FALSE;	
}

/******************************************************************************/
/**
*	@brief Set time out. 
*	@return None.
*	@date 10/2004
*/
/******************************************************************************/
void CUtilTimer::SetTimeOut(const DWORD &dwTimeOut)
{
	m_dwLastTime = GetTickCount();
	m_dwTimeOut = dwTimeOut;
}

/******************************************************************************/
/**
*	@brief Reset timer. 
*	@return None.
*	@date 11/2004
*/
/******************************************************************************/
void CUtilTimer::Reset(void)
{
	SetTimeOut(m_dwTimeOut);
}
