#include "stdafx.h"
#include "TVM.h"
#include "WelcomeReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const TXT_WELCOME_CN         = _T("欢迎光临");
static const TCHAR* const TXT_WELCOME_EN         = _T("Welcome");
static const TCHAR* const TXT_WELCOME_PN         = _T("Welcome");

static const CRect WELCOME_FRAME_RECT_RATIO = CRect(WELCOME_FRAME_RECT.left*REC_RATIOWIDTH, WELCOME_FRAME_RECT.top*REC_RATIOHEIGHT,
													WELCOME_FRAME_RECT.right*REC_RATIOWIDTH, WELCOME_FRAME_RECT.bottom*REC_RATIOHEIGHT);

static CRect TEXT_RECT_CN = CRect(10*REC_RATIOWIDTH,350*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,550*REC_RATIOHEIGHT);
static CRect TEXT_RECT_EN = CRect(10*REC_RATIOWIDTH,550*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,750*REC_RATIOHEIGHT);
static CRect TEXT_RECT_PN = CRect(10*REC_RATIOWIDTH,750*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,1000*REC_RATIOHEIGHT);

static CRect VERSION_RECT = CRect(1000*REC_RATIOWIDTH,1000*REC_RATIOHEIGHT,1280*REC_RATIOWIDTH,1024*REC_RATIOHEIGHT);

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
	: CBaseDlg(CWelcomeReceptionDlg::IDD, NULL, WELCOME_FRAME_RECT_RATIO, RECEPTION_BK_COLOR, pService, false)
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
//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWelcomeReceptionDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
	// 显示文字
	DrawText(pDC,TEXT_RECT_CN, TXT_WELCOME_CN, REC_TITLE_FONT_NAME_EN, 80,FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER);	
	DrawText(pDC,TEXT_RECT_EN, TXT_WELCOME_EN, REC_TITLE_FONT_NAME_EN, 80,FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER);		
	DrawText(pDC,TEXT_RECT_PN, TXT_WELCOME_PN, REC_TITLE_FONT_NAME_EN, 80,FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER);
	CString strVersion = _T("");
	strVersion.Format(_T("TVM:V%d OT:V%d"), theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),
		theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CHARGE_RW_APP));
	DrawText(pDC,VERSION_RECT,strVersion,LOGO_FONT_NAME_EN,VERSION_FONT_SIZE,FW_NORMAL,BLACKGRAY,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWelcomeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//// 隐藏乘客画面标题与向导
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示窗口

@param      (i)CONTENT_TYPE contentType 指定内容

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWelcomeReceptionDlg::Show()
{
	// 隐藏乘客画面标题
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	this->ShowWindow(SW_SHOW);
	this->Invalidate();
	this->UpdateWindow();
}
