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
@brief      CTHApp���캯��

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CTHApp::CTHApp()
{
}


// Ψһ��һ�� CTHApp ����
CTHApp theApp;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CTHApp ��ʼ��

@param      ��         

@retval     BOOL    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CTHApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
