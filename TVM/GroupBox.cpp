#include "stdafx.h"
#include "GroupBox.h"

IMPLEMENT_DYNAMIC(CGroupBox,CPanel)



//////////////////////////////////////////////////////////////////////////
/**
@brief      GroupBox构造函数

@param      (i)UIINFO& uIInfo 本控件样式

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGroupBox::CGroupBox(UIINFO& uIInfo,CString caption)
:CPanel(uIInfo)
{
	m_UIInfo.m_Margins = DEFAULT_GROUPBOX_MARGINS;
	m_UIInfo.m_BorderWidth = 1;	
	m_UIInfo.m_BKMode = BKMODE_LAST;

	UIINFO captionUIInfo;
	captionUIInfo.m_DockStyle = DOCKSTYLE::TOP;
	captionUIInfo.m_Size.cy = 40;
	captionUIInfo.m_pParentWnd = this;
	captionUIInfo.m_Location.y = 15;
	captionUIInfo.m_Margins.cxLeftWidth = 15;
	captionUIInfo.m_Font.lfWeight = FW_HEAVY * 1.2;
	m_pLblCaption = new CLable(captionUIInfo,caption);

}


//////////////////////////////////////////////////////////////////////////
/**
@brief      CGroupBox析造函数

@param      void

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGroupBox::~CGroupBox()
{
	delete m_pLblCaption;
}


CLable* CGroupBox::GetCaption()
{
	return m_pLblCaption;
}


BOOL CGroupBox::Create()
{
	BOOL success = FALSE;
	success = __super::Create();
	if(success)
	{
		success =	m_pLblCaption->Create();
	}
	return success;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      控件绘制函数

@param      (i)LPDRAWITEMSTRUCT lpDrawItemStruct 

@retval     void

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CGroupBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rect =  lpDrawItemStruct->rcItem;
	CRect borderRect = rect;
	borderRect.left += m_UIInfo.m_Margins.cxLeftWidth;
	borderRect.right-=m_UIInfo.m_Margins.cxRightWidth;
	borderRect.top += m_UIInfo.m_Margins.cyTopHeight;
	borderRect.bottom -= m_UIInfo.m_Margins.cyBottomHeight;

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if(m_UIInfo.m_pBackgroundImage)
	{
		int nSaveDC = pDC->SaveDC();
		CDC pSrcDC;
		pSrcDC.CreateCompatibleDC(pDC);
		pSrcDC.SelectObject(m_UIInfo.m_pBackgroundImage);
		::StretchBlt(pDC->GetSafeHdc(), borderRect.left, borderRect.top,borderRect.Width(), borderRect.Height(), 
			pSrcDC.GetSafeHdc(), 0, 0,  m_UIInfo.m_pBackgroundImage->GetWidth(), m_UIInfo.m_pBackgroundImage->GetHeight(), SRCCOPY);
		pDC->TransparentBlt(borderRect.left, borderRect.top,borderRect.Width(), borderRect.Height() , &pSrcDC, 0, 0, m_UIInfo.m_Size.cx , m_UIInfo.m_Size.cy, WHITE);
		pDC->RestoreDC(nSaveDC);
	}
	else
	{
		CBrush brush(m_UIInfo.m_BackColor);
		pDC->SelectObject(&brush);
	}
	CPen pen(PS_SOLID, m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderColor);
	pDC->SelectObject(&pen);
	pDC->RoundRect(&borderRect, ROUND_POINT);
}
