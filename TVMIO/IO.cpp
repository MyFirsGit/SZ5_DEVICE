#include "stdafx.h"
#include "IO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
@brief  TVM IO接口应用
*/

BEGIN_MESSAGE_MAP(CIOApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param    none

@retval     none

@exception   none
*/
//////////////////////////////////////////////////////////////////////////
CIOApp::CIOApp()
{
}

BOOL CIOApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}