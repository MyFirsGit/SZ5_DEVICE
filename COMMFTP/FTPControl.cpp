/******************************************************************************/
/**	@class CFTPControl.
*	@brief 
*	See FTPControl.h for the header of this class.
*	@version 1.00 (10-15-2004): The first version that generate by VS.NET 2003
*/
/******************************************************************************/
#include "StdAfx.h"
#include ".\ftpcontrol.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************/
/**	@brief Construction. 
*	@date 10-15-2004 (K.Thu): Create and update this method
*/
/******************************************************************************/
CFTPControl::CFTPControl(void)
{
}

/******************************************************************************/
/**	@brief Destruction. 
*	@date 10-15-2004 (K.Thu): Create and update this method
*/
/******************************************************************************/
CFTPControl::~CFTPControl(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初始化

@param      (i) bool bWithSense
(i) int iIdleTimer

@retval     long      

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CFTPControl::Initialize()
{
	//CControl::Initialize();
	//m_pRWLibControl->Initialize();
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭控制

@param      无

@retval     long      

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CFTPControl::Shutdown(void)
{
	//CControl::Shutdown();
	//m_pRWLibControl->Shutdown();
	return 0;
}