#include "stdafx.h"
#include "BOM.h"
#include "GuideDlg.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGuideDlg CGuideDlg::instance;

static const PADDINGS GUIDE_PADDINGS		  =  PADDINGS(5,5,5,5);
static const MARGINS GUIDE_MARGINS			= {5,5,5,5};
static CRect  GUIDE_MSG_RECT            =  CRect(0,0,0,0);//构造函数中初始化


IMPLEMENT_DYNAMIC(CGuideDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CGuideDlg, CBaseDlg)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd*     pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGuideDlg::CGuideDlg(CWnd* pParent)
	:CBaseDlg(CGuideDlg::IDD, pParent, GUIDE_FRAME_RECT, GRAY)
    ,m_guideMsg()
    ,m_msg()
    ,m_color(BLACK)
{
	GUIDE_MSG_RECT = CRect(GUIDE_MARGINS.cxLeftWidth,GUIDE_MARGINS.cyTopHeight,GUIDE_FRAME_RECT.Width() - GUIDE_MARGINS.cxLeftWidth ,GUIDE_FRAME_RECT.Height() - GUIDE_MARGINS.cyTopHeight );
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGuideDlg::~CGuideDlg()
{
}

BOOL CGuideDlg::Create(CWnd* pParentWnd )
{
	if(IsWindow(m_hWnd)){
		this->DestroyWindow();
	}
	__super::Create(CGuideDlg::IDD,pParentWnd);
	
	return this->ShowWindow(SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      画面绘制函数
//
//@param      (i) CDC* pDC  设备上下文
//
//@retval     无
//
//@exception  无
//*/
////////////////////////////////////////////////////////////////////////////
void CGuideDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	GUIDE_MSG_RECT = CRect(GUIDE_MARGINS.cxLeftWidth,GUIDE_MARGINS.cyTopHeight,GUIDE_FRAME_RECT.Width() - GUIDE_MARGINS.cxLeftWidth- GUIDE_MARGINS.cxRightWidth ,GUIDE_FRAME_RECT.Height() - GUIDE_MARGINS.cyTopHeight -GUIDE_MARGINS.cyBottomHeight  );
    DrawRoundRect(pDC, GUIDE_MSG_RECT);
    DrawText(pDC, GUIDE_MSG_RECT.left + GUIDE_PADDINGS.cxLeftWidth, GUIDE_MSG_RECT.top + GUIDE_PADDINGS.cyTopHeight+12, m_msg, TITLE_FONT, 16,FW_HEAVY, m_color);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      显示向导信息

@param      (i)guideMsg	        向导信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuideDlg::ShowGuideMsg(CString guideMsg)
{
    m_guideMsg = guideMsg;

    // 解析向导信息
    theAPP_SESSION.ParseGuideMsg(guideMsg, m_msg, m_color);
	if(IsWindow(m_hWnd)){
    // 刷新屏幕
		InvalidateRect(&GUIDE_MSG_RECT);
		if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		{
				UpdateWindow();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取向导信息

@param      无

@retval     CString	        向导信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CGuideDlg::GetGuideMsg()
{
    return m_guideMsg;
}
