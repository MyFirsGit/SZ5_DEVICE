// CParamterAPIApp.cpp : Defines the initialization routines for the DLL.

#include "stdafx.h"
#include "CParameterAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CParamterAPIApp

BEGIN_MESSAGE_MAP(CParamterAPIApp, CWinApp)
END_MESSAGE_MAP()


// CParamterAPIApp construction

CParamterAPIApp::CParamterAPIApp()
{
}


// The one and only CParamterAPIApp object

CParamterAPIApp theApp;


// CParamterAPIApp initialization

BOOL CParamterAPIApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
