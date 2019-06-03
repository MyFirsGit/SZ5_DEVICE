#include "stdafx.h"
#include "TVM.h"
#include "WelcomeReceptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const TXT_WELCOME_CN         = _T("��ӭ����");
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
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CWelcomeReceptionDlg::CWelcomeReceptionDlg(CService* pService)
	: CBaseDlg(CWelcomeReceptionDlg::IDD, NULL, WELCOME_FRAME_RECT_RATIO, RECEPTION_BK_COLOR, pService, false)
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
//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CWelcomeReceptionDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);
	// ��ʾ����
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
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CWelcomeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	//// ���س˿ͻ����������
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ����

@param      (i)CONTENT_TYPE contentType ָ������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CWelcomeReceptionDlg::Show()
{
	// ���س˿ͻ������
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	this->ShowWindow(SW_SHOW);
	this->Invalidate();
	this->UpdateWindow();
}
