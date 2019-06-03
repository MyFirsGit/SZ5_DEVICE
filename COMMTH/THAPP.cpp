#include "stdafx.h"
#include "THAPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CTHApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHApp构造函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CTHApp::CTHApp()
{
}


// 唯一的一个 CTHApp 对象
CTHApp theApp;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHApp 初始化

@param      无         

@retval     BOOL    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTHApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
