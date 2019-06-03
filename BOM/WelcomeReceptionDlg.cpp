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
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeReceptionDlg::CWelcomeReceptionDlg(CService* pService)
	: CBaseDlg(CWelcomeReceptionDlg::IDD, NULL, WELCOME_FRAME_RECT, RECEPTION_BK_COLOR, pService, false)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeReceptionDlg::~CWelcomeReceptionDlg()
{
}
