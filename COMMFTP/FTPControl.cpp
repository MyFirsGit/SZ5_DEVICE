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
@brief      ��ʼ��

@param      (i) bool bWithSense
(i) int iIdleTimer

@retval     long      

@exception  ��
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
@brief      �رտ���

@param      ��

@retval     long      

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CFTPControl::Shutdown(void)
{
	//CControl::Shutdown();
	//m_pRWLibControl->Shutdown();
	return 0;
}