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
@brief      CRWApp���캯��

@param      ��         

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CRWApp::CRWApp()
{
}


// Ψһ��һ�� CRWApp ����
CRWApp theApp;

//////////////////////////////////////////////////////////////////////////
/**
@brief      CRWApp ��ʼ��

@param      ��         

@retval     BOOL    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CRWApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
