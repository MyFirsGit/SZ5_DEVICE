#include "stdafx.h"
#include "Panel.h"
#include "basedlg.h"
#include "Bag.h"

IMPLEMENT_DYNAMIC(CPanel,CStatic)
BEGIN_MESSAGE_MAP(CPanel,CStatic)
	 ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////
/**
@brief      Panel构造函数

@param      (i)UIINFO& uIInfo 本控件样式

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPanel::CPanel(UIINFO& uIInfo)
:CStatic()
{
	ASSERT( uIInfo.m_pParentWnd!=NULL);
	m_pBag = new CBag();
	this->m_UIInfo= uIInfo;
	calculatedFrameRect = false;
	m_ActualSize = DEFAULT_SIZE_EDIT;
	m_ActualLocation = CPoint(0,0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      Panel析造函数

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CPanel::~CPanel()
{
	delete m_pBag;
	m_pBag = NULL;
}

CBag* CPanel::GetBag()
{
	return m_pBag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建一个Panel控件

@param      (i)Func<void (CPanel*)> * creating 提供给调用者的一个回调函数，用于在调用Create之间更改Panel的属性等。

@retval     BOOL
				TRUE	成功
				FALSE	失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPanel::Create(Func<void (CPanel*)> * creating)
{
	if(creating!=NULL){
		creating->Invoke(this);
	}
	return Create();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      创建一个Panel控件

@param      void

@retval     BOOL
			TRUE	成功
			FALSE	失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPanel::Create()
{
	CalculateFrameRect(m_UIInfo.m_pParentWnd,m_UIInfo.m_DockStyle,m_UIInfo.m_Location);
	DWORD dwStyle = WS_CHILD  | ES_UPPERCASE | SS_OWNERDRAW;
	if(m_UIInfo.m_Visible)
	{
		dwStyle |= WS_VISIBLE;
	}
	if(!m_UIInfo.m_Enabled)
	{
		dwStyle |= WS_DISABLED;
	}
	return CStatic::Create(_T(""),dwStyle,GetFrameRect(),m_UIInfo.m_pParentWnd);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Childview背景颜色(禁止绘制底色)

@param      (i)CDC* pDC

@retval     BOOL    TRUE:禁止绘制底色; FALSE:允许绘制底色

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL CPanel::OnEraseBkgnd(CDC* pDC)
{
	CRect  borderRect = GetContentRect();
	pDC->SetBkMode( m_UIInfo.m_BKMode);
	if(m_UIInfo.m_BKMode!=TRANSPARENT && m_UIInfo.m_pBackgroundImage)
	{
		CRect contentRect = borderRect;
		contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
		int nSaveDC = pDC->SaveDC();
		CDC pSrcDC;
		pSrcDC.CreateCompatibleDC(pDC);
		pSrcDC.SelectObject(m_UIInfo.m_pBackgroundImage);
		BITMAP  bitMapInfo;
		m_UIInfo.m_pBackgroundImage->GetBitmap(&bitMapInfo);
		::StretchBlt(pDC->GetSafeHdc(), contentRect.left, contentRect.top,contentRect.Width(), contentRect.Height(), 
			pSrcDC.GetSafeHdc(), 0, 0,  bitMapInfo.bmWidth, bitMapInfo.bmHeight, SRCCOPY);
		pDC->RestoreDC(nSaveDC);
		return TRUE;
	}
	else if(m_UIInfo.m_BKMode!=TRANSPARENT)
	{
		pDC->SelectStockObject(NULL_PEN);
		CBrush brush(m_UIInfo.m_BackColor);
		pDC->SelectObject(&brush);
		pDC->Rectangle(borderRect);
		return TRUE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      得到Panel的样式

@param      void

@retval     UIINFO 样式

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UIINFO CPanel::GetUIInfo()
{
	return m_UIInfo;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置Panel的样式

@param      (i)UIINFO& uIInfo 设置新样式

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::SetUIInfo(UIINFO& uIInfo)
{
	if(m_UIInfo == uIInfo)
	{
		return;
	}
	calculatedFrameRect = false;
	m_UIInfo = uIInfo;
	if(!IsWindow(m_hWnd))
	{
		return;
	}

	this->DestroyWindow();
	this->Create();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      控件绘制函数

@param      (i)LPDRAWITEMSTRUCT lpDrawItemStruct 

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect =  lpDrawItemStruct->rcItem;
	CRect borderRect = rect;
	borderRect.left += m_UIInfo.m_Margins.cxLeftWidth;
	borderRect.right-=m_UIInfo.m_Margins.cxRightWidth;
	borderRect.top += m_UIInfo.m_Margins.cyTopHeight;
	borderRect.bottom -= m_UIInfo.m_Margins.cyBottomHeight;
	
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SetBkMode( m_UIInfo.m_BKMode);
	pDC->SetTextColor(m_UIInfo.m_ForeColor);
	CFont cFont;
	cFont.CreatePointFontIndirect(&m_UIInfo.m_Font);
	pDC->SelectObject(cFont);
	CPen pen(PS_SOLID, m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderColor);
	if(m_UIInfo.m_BorderWidth>0)
	{
		pDC->SelectObject(&pen);
	}
	else
	{
		pDC->SelectObject(GetStockObject(NULL_PEN));
	}
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(borderRect);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得控件的框架区域

@param      void

@retval     CRect 框架区域

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRect CPanel::GetFrameRect()
{
	return CRect(m_ActualLocation,m_ActualSize);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      根据DOCKSTYLE算出，控件的实际大小与位置

@param      void

@retval     CRect 框架区域

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPanel::CalculateFrameRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint location)
{
	if(calculatedFrameRect)
	{
		return;
	}

	calculatedFrameRect = true;
	m_ActualSize = m_UIInfo.m_Size;
	m_ActualLocation = location;
	CRect parentRectSrc;
	CRect parentContentRect;

	if(m_UIInfo.m_pParentWnd->IsKindOf(RUNTIME_CLASS(CPanel)))
	{
		CPanel* panel = reinterpret_cast<CPanel*>(m_UIInfo.m_pParentWnd);
		parentContentRect = panel->GetContentRect();
	}
	else{
		m_UIInfo.m_pParentWnd->GetWindowRect(&parentRectSrc);
		parentContentRect.left = 0;
		parentContentRect.top = 0;
		parentContentRect.right = parentRectSrc.Width();
		parentContentRect.bottom = parentRectSrc.Height();
	}
	
	
	
	switch(m_UIInfo.m_DockStyle)
	{
	case LEFT:
		{
			m_ActualLocation.x = parentContentRect.left + location.x;
			m_ActualLocation.y = parentContentRect.top;
			m_ActualSize.cy = parentContentRect.Height();
			m_ActualSize.cx > parentContentRect.Width() ? parentContentRect.Width() : m_ActualSize.cx;
			break;
		}
	case TOP:
		{
			m_ActualLocation.y = parentContentRect.top + location.y;
			m_ActualLocation.x = parentContentRect.left;
			m_ActualSize.cx = parentContentRect.Width();
			m_ActualSize.cy > parentContentRect.Height() ? parentContentRect.Height() : m_ActualSize.cx;
			break;
		}
	case RIGHT:
		{
			m_ActualLocation.x = parentContentRect.right - location.x - m_UIInfo.m_Size.cx;
			m_ActualLocation.y = parentContentRect.top;
			m_ActualSize.cy = parentContentRect.Height();
			m_ActualSize.cx > parentContentRect.Width() ? parentContentRect.Width() : m_ActualSize.cx;
			break;
		}
	case BOTTOM:
		{
			m_ActualLocation.y = parentContentRect.bottom - location.y - m_UIInfo.m_Size.cy;
			m_ActualLocation.x = parentContentRect.left;
			m_ActualSize.cx = parentContentRect.Width();
			m_ActualSize.cy > parentContentRect.Height() ? parentContentRect.Height() : m_ActualSize.cx;
			break;
		}
	case FILL:
		{
			m_ActualLocation = CPoint(parentContentRect.left ,parentContentRect.top);
			m_ActualSize = CSize(parentContentRect.Width(),parentContentRect.Height());
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      根据控件实际绘制内容的区域（除掉外补白，内补白，及边框）

@param      void

@retval     CRect 内容区域

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRect CPanel::GetContentRect()
{
	CRect contentRect = CRect(CPoint(0,0),m_ActualSize);
	
	contentRect.left	+= (m_UIInfo.m_Margins.cxLeftWidth + m_UIInfo.m_Paddings.cxLeftWidth + m_UIInfo.m_BorderWidth);  
	contentRect.right	-= (m_UIInfo.m_Margins.cxRightWidth + m_UIInfo.m_Paddings.cxRightWidth + m_UIInfo.m_BorderWidth);  
	contentRect.top += (m_UIInfo.m_Margins.cyTopHeight + m_UIInfo.m_Paddings.cyTopHeight + m_UIInfo.m_BorderWidth);
	contentRect.bottom -= (m_UIInfo.m_Margins.cyBottomHeight + m_UIInfo.m_Paddings.cyBottomHeight + m_UIInfo.m_BorderWidth);
	return contentRect;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      重写消息处理函数（如果当前消息处理函数返回值为-1时，消息不需要路由到父窗体，其他情况将消息路由到父窗体。

@param      (i)UINT message 消息ID
@param      (i)WPARAM wParam 消息参数
@param      (i)LPARAM lParam 消息参数

@retval     LRESULT

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = __super::WindowProc(message,wParam,lParam);
	if(message>WM_USER && result>=0 &&  IsWindow(m_hWnd) && GetParent()!=NULL)
	{
		CWnd *pParent = GetParent();

		if(pParent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
		{
			((CBaseDlg*)pParent)->CallWndProc(message,wParam,lParam);
		}
		else if(pParent->IsKindOf(RUNTIME_CLASS(CPanel)))
		{
			((CPanel*)pParent)->CallWndProc(message,wParam,lParam);
		}
		else
		{
			pParent->SendMessage(message,wParam,lParam);
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得父窗体（CBaseDlg类型）

@param      void

@retval     CBaseDlg* 父窗体

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBaseDlg* CPanel::GetParentDlg()
{
	if(IsWindow(this->m_hWnd))
	{
		CWnd* parent = GetParent();
		while(parent!=NULL && !parent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
		{
			parent = parent->GetParent();
		}
		return (CBaseDlg*)parent;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      调用窗口处理函数，绕过Windows消息队列

@param      (i)UINT message 消息ID
@param      (i)WPARAM wParam 消息参数
@param      (i)LPARAM lParam 消息参数

@retval     LRESULT

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CPanel::CallWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return WindowProc(message,wParam,lParam);
}