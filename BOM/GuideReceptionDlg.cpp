#include "stdafx.h"
#include "BOM.h"
#include "GuideReceptionDlg.h"
#include "UIDefinition.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Guide对话框位置大小
#define GUD_L	REC_BODY_L
#define GUD_R	REC_BODY_R
#define GUD_U	70
#define GUD_D	REC_BODY_U + 4

IMPLEMENT_DYNAMIC(CGuideReceptionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CGuideReceptionDlg, CBaseDlg)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGuideReceptionDlg::CGuideReceptionDlg(CWnd* pParent)
    :CBaseDlg(CGuideReceptionDlg::IDD, pParent, REC_GUIDE_RECT, RECEPTION_HEAD_BK_COLOR, false)
    ,m_guideMsg()
    ,m_guideEnglishMsg()
    ,m_color(WHITE)
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
CGuideReceptionDlg::~CGuideReceptionDlg()
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
void CGuideReceptionDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);

    DrawText(pDC, 26, 6, m_guideMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_CN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
    DrawText(pDC, 26, 45, m_guideEnglishMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_EN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	DrawText(pDC, 26+CXSCREENREC, 6, m_guideMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_CN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	DrawText(pDC, 26+CXSCREENREC, 45, m_guideEnglishMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_EN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     显示向导信息

@param      (i)guideMsg	            中文向导信息
@param      (i)guideEnglishMsg	    英文向导信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuideReceptionDlg::ShowGuideMsg(CString guideMsg, CString guideEnglishMsg)
{
    // 解析向导信息（中文）
	theAPP_SESSION.ParseGuideMsg(guideMsg, m_guideMsg, m_color );
    m_guideEnglishMsg = guideEnglishMsg;

	this->Invalidate();
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		UpdateWindow();
	}
}