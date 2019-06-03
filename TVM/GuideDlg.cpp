#include "stdafx.h"
#include "TVM.h"
#include "GuideDlg.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// Guide样式定义

static CRect GUIDE_RECT				= CRect(0,0,0,0);		// 文言显示区域，构造函数中初始化
static const int GUIDE_BORDER		= 1;					// 边框宽度
static const CPoint GUIDE_ROUND		= CPoint(12,12);		// 圆角弧度
static const MARGINS GUIDE_MARGIN	= {2,0,2,0};			// 外边距
static const PADDINGS GUIDE_PADDING	= PADDINGS(8,2,2,2);	// 内边距

IMPLEMENT_DYNAMIC(CGuideDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(CGuideDlg, CBaseDlg)

END_MESSAGE_MAP()

CGuideDlg CGuideDlg::instance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CWnd* pParent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGuideDlg::CGuideDlg(CWnd* pParent)
	:CBaseDlg(CGuideDlg::IDD, pParent, OPERATOR_GUIDE_RECT, GRAY)
    ,m_msg()
    ,m_color(OPERATOR_GUIDE_FONT_COLOR/*BLACK*/) //默认颜色  从配置文件中读取
{
	GUIDE_RECT = CRect(GUIDE_MARGIN.cxLeftWidth, GUIDE_MARGIN.cyTopHeight,
		OPERATOR_GUIDE_RECT.Width() - GUIDE_MARGIN.cxLeftWidth- GUIDE_MARGIN.cxRightWidth,
		OPERATOR_GUIDE_RECT.Height() - GUIDE_MARGIN.cyTopHeight -GUIDE_MARGIN.cyBottomHeight);
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

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建

@param      (i)CWnd* pParentWnd 父窗口

@retval     BOOL  TURE:成功  FALSE:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CGuideDlg::Create(CWnd* pParentWnd)
{
	if(IsWindow(m_hWnd))
	{
		this->DestroyWindow();
	}
	__super::Create(CGuideDlg::IDD, pParentWnd);
	
	return this->ShowWindow(SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC 设备上下文

@retval     无

@exception  无
*/
////////////////////////////////////////////////////////////////////////////
void CGuideDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	GUIDE_RECT = CRect(GUIDE_MARGIN.cxLeftWidth, GUIDE_MARGIN.cyTopHeight,
		OPERATOR_GUIDE_RECT.Width() - GUIDE_MARGIN.cxLeftWidth- GUIDE_MARGIN.cxRightWidth,
		OPERATOR_GUIDE_RECT.Height() - GUIDE_MARGIN.cyTopHeight -GUIDE_MARGIN.cyBottomHeight);
    DrawRoundRect(pDC, GUIDE_RECT, GUIDE_BORDER, DARKGRAY, OPERATOR_GUIDE_BK_COLOR/*GRAY*/, GUIDE_ROUND);
	GUIDE_RECT.DeflateRect(GUIDE_PADDING.cxLeftWidth, GUIDE_PADDING.cyTopHeight, GUIDE_PADDING.cxRightWidth, GUIDE_PADDING.cyBottomHeight);
	//DrawText(pDC, GUIDE_RECT, m_msg, DEFAULT_FONT, m_color, DT_VCENTER|DT_SINGLELINE); Modified by lws :  Font&Size will be readed frome INI
	//绘制Guide文言，不要使用基类的，基类实现有问题
	CFont font;
    font.CreateFont(GUIDE_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
        OPERATORCHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
        DEFAULT_PITCH|FF_SWISS, GUIDE_FONT_NAME);

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetTextColor(m_color);
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(m_msg, GUIDE_RECT, DT_VCENTER|DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示向导信息

@param      (i)guideMsg	        向导信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGuideDlg::ShowGuideMsg(CString officeMsg, COLORREF color /* = BLACK */)
{
	m_msg = officeMsg;
	m_color = color;

	// 刷新屏幕
	if(IsWindow(m_hWnd))
	{
		InvalidateRect(&GUIDE_RECT);
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

@retval     CString 向导信息

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CGuideDlg::GetGuideMsg()
{
    return m_msg;
}

void CGuideDlg::ShowGuideMsg(DWORD officeMsg)
{
}