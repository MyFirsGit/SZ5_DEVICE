#include "stdafx.h"
#include "BOM.h"
#include "WelcomeReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CWelcomeReceptionDlg, CBaseDlg)

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeReceptionDlg::CWelcomeReceptionDlg(CService* pService)
	: CBaseDlg(CWelcomeReceptionDlg::IDD, NULL, WELCOME_FRAME_RECT, RECEPTION_BK_COLOR, pService, false)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeReceptionDlg::~CWelcomeReceptionDlg()
{
}
