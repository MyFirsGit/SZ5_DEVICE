#include "stdafx.h"
#include "Lable.h"

IMPLEMENT_DYNAMIC(CLable,CPanel)

BEGIN_MESSAGE_MAP(CLable,CPanel)
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      Label构造函数

@param      (i)UIINFO& uIInfo 本控件样式
@param      (i)CString text 显示的文字

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLable::CLable(UIINFO& uIInfo,CString text)
:CPanel(uIInfo)
{
	this->m_OnPaintFunc = NULL;
	this->m_Text = text;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CLable析造函数

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLable::~CLable()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建一个Label

@param      void

@retval     BOOL
						TRUE 成功
						FALSE	失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CLable::Create()
{
	this->m_UIInfo.m_Size = GetTextSize();
	return __super::Create();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      根据文字算出，文字所在区域大小

@param      void

@retval     CSize 大小

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSize CLable::GetTextSize()
{
	CPaintDC dc(m_UIInfo.m_pParentWnd);
	CFont cFont;
	cFont.CreateFontIndirect(&m_UIInfo.m_Font);
	CFont* pOldFont=dc.SelectObject(&cFont);
	CSize size = dc.GetTextExtent(m_Text);
	dc.SelectObject(pOldFont);
	size.cx += (m_UIInfo.m_Margins.cxLeftWidth+m_UIInfo.m_Margins.cxRightWidth+m_UIInfo.m_BorderWidth*2+m_UIInfo.m_Paddings.cxLeftWidth+m_UIInfo.m_Paddings.cxRightWidth);
	size.cy	+= (m_UIInfo.m_Margins.cyTopHeight+m_UIInfo.m_Margins.cyBottomHeight+m_UIInfo.m_BorderWidth*2+m_UIInfo.m_Paddings.cyTopHeight+m_UIInfo.m_Paddings.cyBottomHeight);
	return size;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      绘制函数

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLable::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = CDC::FromHandle(dc);

	CFont cFont;
	cFont.CreateFontIndirect(&m_UIInfo.m_Font);

	pDC->SetTextColor(m_UIInfo.m_ForeColor);
	CFont* pOldFont = pDC->SelectObject(&cFont);
	pDC->SetBkMode(TRANSPARENT);

	if(m_OnPaintFunc)
	{
		m_OnPaintFunc->Invoke(pDC,this);
	}
	else
	{
		CRect contentRect = GetContentRect();
		CSize textSize = dc.GetTextExtent(m_Text);

		CPoint pointStartText = CPoint(contentRect.left,contentRect.top);
		switch(m_UIInfo.m_DockStyle)
		{
		case LEFT:
			{
				break;
			}
		case TOP:
			{
				break;
			}
		case RIGHT:
			{
				pointStartText = CPoint(contentRect.right- textSize.cx,contentRect.top);
				break;
			}
		case BOTTOM:
			{
				pointStartText = CPoint(contentRect.left,contentRect.bottom - textSize.cy);
				break;
			}
		case FILL:
			{
				break;
			}
		default:
			{
				break;
			}
		}

		pDC->TextOut(pointStartText.x,pointStartText.y,m_Text);
	}

	pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得文字

@param      void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString CLable::GetText()
{
	return m_Text;
}		

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置文字

@param      void

@retval     CString

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLable::SetText(CString text)
{
	m_Text = text;
	UIINFO uiInfo = m_UIInfo;
	if(IsWindow(m_hWnd))
	{
		uiInfo.m_Size = GetTextSize();
	}
	SetUIInfo(uiInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置控件样式，重载父类函数

@param      UIINFO& uIInfo 控件样式

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLable::SetUIInfo(UIINFO& uIInfo)
{
	if(IsWindow(m_hWnd))
	{
		uIInfo.m_Size = GetTextSize();
	}
	__super::SetUIInfo(uIInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置绘制函数时的回调函数，提供一个接口给调用者自行绘制

@param      Func<void (CDC*,CLable*)> * pPaintFunc 回调函数指针

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLable::SetPaintFunc(Func<void (CDC*,CLable*)> * pPaintFunc)
{
	m_OnPaintFunc = pPaintFunc;
}