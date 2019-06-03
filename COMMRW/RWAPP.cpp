#include "stdafx.h"
#include "RWAPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRWApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      CRWApp构造函数

@param      无         

@retval     无     

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWApp::CRWApp()
{
}


// 唯一的一个 CRWApp 对象
CRWApp theApp;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CRWApp 初始化

@param      无         

@retval     BOOL    

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRWApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
