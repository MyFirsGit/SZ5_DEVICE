#include "StdAfx.h"
#include "ImageZoom.h"
#include "afxdialogex.h"

//  [6/26/2016 yufuxiang]
CImageZoom::CImageZoom(CWnd* pWnd)
{
	// 初始化GDI+  
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;  
	Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL ); 

	m_pWnd=pWnd;
	m_pDC=pWnd->GetDC();

	m_bIsLBtnDown=FALSE;
	m_pMouDownPnt.x=0;m_pMouDownPnt.y=0;

	m_pXoffset=0.0;
	m_pYoffset=0.0;
	m_pXstart=0.0;
	m_pYstart=0.0;

	m_pScale=1.0;
}
//
CImageZoom::CImageZoom(CWnd* pWnd,CString imgPath,BOOL isFull)
{
	// 初始化GDI+  
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;  
	Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL ); 

	m_pWnd=pWnd;
	m_pDC=pWnd->GetDC();
	m_pImgPath=imgPath;
	m_bIsLBtnDown=FALSE;
	m_pMouDownPnt.x=0;m_pMouDownPnt.y=0;
	pImage = new Image(m_pImgPath);
	m_pXoffset=0.0;
	m_pYoffset=0.0;
	m_pXstart=0.0;
	m_pYstart=0.0;
	
	if (!isFull)
	{
		m_pScale=0.7796;
	}
	else
	{
		Image tmpImg(m_pImgPath);

		CRect rect;
		m_pWnd->GetWindowRect(&rect);

		m_pScale=(double)rect.Width()/(double)pImage->GetWidth();
	}
}

CImageZoom::~CImageZoom(void)
{
	Gdiplus::GdiplusShutdown( m_gdiplusToken );
}
//
void CImageZoom::OnLButtonDown(POINT pt)
{	
	Image img(m_pImgPath);

	CRect rect;
	m_pWnd->GetWindowRect(&rect);
	//m_pWnd->ScreenToClient(&rect);
	if (rect.PtInRect(pt))
	{
		m_bIsLBtnDown=TRUE;
		m_pMouDownPnt=pt;
	}
	double tmpOffsetX=0;double tmpOffsetY=0;

	POINT point;point.x=pt.x;point.y=pt.y;

	HDC hdc = ::GetDC(m_pWnd->GetSafeHwnd());  
	CDC dc;  
	dc.Attach(hdc);  
	CRect rect1;
	m_pWnd->GetClientRect(&rect1);
	// 绘制边框
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&pen);
	dc.Rectangle(rect1);
	dc.SelectObject(pOldPen);	
	// 使用双缓冲绘图
	CDC MemDc;
	CBitmap MemBitMap;
	int border = 2;
	CRect imageRect;
	imageRect.left = rect1.left + border;
	imageRect.right = rect1.right - border;
	imageRect.top = rect1.top + border;
	imageRect.bottom = rect1.bottom - border;
	MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
	MemBitMap.CreateCompatibleBitmap(&dc,imageRect.Width(),imageRect.Height());// 创建内存位图
	MemDc.SelectObject(MemBitMap);// 选入位图
	Graphics graphics(MemDc.m_hDC);

	if (rect.PtInRect(point))
	{
		{
			double tmpscale=0.4/*zDelta/(double)MIN_SCALE_RESLOVING*/;

			tmpOffsetX=pImage->GetWidth()*tmpscale*((double)(point.x-rect.left-m_pXstart)/(double)(pImage->GetWidth()*(m_pScale)));
			tmpOffsetY=pImage->GetHeight()*tmpscale*((double)(point.y-rect.top-m_pYstart)/(double)(pImage->GetHeight()*(m_pScale)));

			graphics.Clear(BK_COLOR);
			graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

			graphics.DrawImage(pImage,(REAL)(m_pXstart-tmpOffsetX),m_pYstart-tmpOffsetY,pImage->GetWidth()*(m_pScale+tmpscale),pImage->GetHeight()*(m_pScale+tmpscale));
			// 将内存中的位图绘到DC中去
			dc.BitBlt(border,border,imageRect.Width(),imageRect.Height(),&MemDc,0,0,SRCCOPY);

			m_pScale+=tmpscale;
			m_pXstart-=tmpOffsetX;
			m_pYstart-=tmpOffsetY;
		}
	}

	// 绘图清理
	MemBitMap.DeleteObject();
	MemDc.DeleteDC();
	dc.Detach();
	::ReleaseDC(m_pWnd->GetSafeHwnd(),hdc);

}
//
void CImageZoom::OnLButtonUp(POINT point)
{
	m_pMouDownPnt=point;

	m_pXstart+=m_pXoffset;
	m_pYstart+=m_pYoffset;

	m_pXoffset=0.0;
	m_pYoffset=0.0;
}
//
void CImageZoom::OnMouseMove(POINT point)
{
	//if (m_bIsLBtnDown)
	{
		Image tmpImg(m_pImgPath);
		CRect rect;
		m_pWnd->GetClientRect(&rect);
		HDC hdc = ::GetDC(m_pWnd->GetSafeHwnd());  
		CDC dc;  
		dc.Attach(hdc);  
		// 使用双缓冲绘图
		CDC MemDc;
		CBitmap MemBitMap;
		MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
		MemBitMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());// 创建内存位图
		MemDc.SelectObject(MemBitMap);// 选入位图
		//CBrush brush(OPERATOR_BASE_BK_COLOR);
		//MemDc.FillRect(rect,&brush);
		Graphics graphics(MemDc.m_hDC);
		graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
		graphics.DrawImage(&tmpImg,m_pXstart+m_pXoffset,m_pYstart+m_pYoffset,tmpImg.GetWidth()*m_pScale,tmpImg.GetHeight()*m_pScale);

		// 将内存中的位图绘到DC中去
		dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDc,0,0,SRCCOPY);

		// 绘图清理
		MemBitMap.DeleteObject();
		MemDc.DeleteDC();
		dc.Detach();
		::ReleaseDC(m_pWnd->GetSafeHwnd(),hdc);

	}
}
//
void CImageZoom::OnMouseWheel(short zDelta, CPoint pt)
{
	Image img(m_pImgPath);

	if(zDelta>120)  zDelta=120;
	if(zDelta<-120) zDelta=-120;

	double tmpOffsetX=0;double tmpOffsetY=0;

	CRect rect;
	m_pWnd->GetWindowRect(&rect);
	POINT point;point.x=pt.x;point.y=pt.y;

	CMemDC memDC(*m_pDC,m_pWnd);
	CDC* pDC=&memDC.GetDC();
	Graphics graphics(pDC->m_hDC);

	if (rect.PtInRect(point))
	{
		if(m_pScale>MIN_SCALE || zDelta/120>0)
		{
			double tmpscale=zDelta/(double)MIN_SCALE_RESLOVING;
			if(m_pScale>CHANGE_SCALE_RATE) tmpscale*=CHANGE_SCALE_RATE;

			tmpOffsetX=img.GetWidth()*tmpscale*((double)(point.x-rect.left-m_pXstart)/(double)(img.GetWidth()*(m_pScale)));
			tmpOffsetY=img.GetHeight()*tmpscale*((double)(point.y-rect.top-m_pYstart)/(double)(img.GetHeight()*(m_pScale)));

			graphics.Clear(BK_COLOR);
			graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
			graphics.DrawImage(&img,(REAL)(m_pXstart-tmpOffsetX),m_pYstart-tmpOffsetY,img.GetWidth()*(m_pScale+tmpscale),img.GetHeight()*(m_pScale+tmpscale));
			m_pScale+=tmpscale;
			m_pXstart-=tmpOffsetX;
			m_pYstart-=tmpOffsetY;
		}
	}
}
//
void CImageZoom::setImgShow()
{
	//Image tmpImg(m_pImgPath);

	//CDC* pDC=m_pWnd->GetDC();

	//Graphics graphics(pDC->m_hDC);

	//graphics.Clear(BK_COLOR); 

	//graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	//graphics.DrawImage(pImage,0,0,(double)pImage->GetWidth()*m_pScale,pImage->GetHeight()*m_pScale);
	
	HDC hdc = ::GetDC(m_pWnd->GetSafeHwnd());  
	CDC dc;  
	dc.Attach(hdc);  
	CRect rect;
	m_pWnd->GetClientRect(&rect);
	// 绘制边框
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&pen);
	dc.Rectangle(rect);
	dc.SelectObject(pOldPen);		;
	// 使用双缓冲绘图
	int border = 2;
	CRect imageRect;
	imageRect.left = rect.left + border;
	imageRect.right = rect.right - border;
	imageRect.top = rect.top + border;
	imageRect.bottom = rect.bottom - border;
	CDC MemDc;
	CBitmap MemBitMap;
	MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
	MemBitMap.CreateCompatibleBitmap(&dc,imageRect.Width(),imageRect.Height());// 创建内存位图
	MemDc.SelectObject(MemBitMap);// 选入位图
	//CBrush brush(OPERATOR_BASE_BK_COLOR);
	//MemDc.FillRect(rect,&brush);
	Graphics graphics(MemDc.m_hDC);
	graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	graphics.DrawImage(pImage,0,0,(double)pImage->GetWidth()*m_pScale,pImage->GetHeight()*m_pScale);

	// 将内存中的位图绘到DC中去
	dc.BitBlt(border,border,imageRect.Width(),imageRect.Height(),&MemDc,0,0,SRCCOPY);

	// 绘图清理
	MemBitMap.DeleteObject();
	MemDc.DeleteDC();
	dc.Detach();
	::ReleaseDC(m_pWnd->GetSafeHwnd(),hdc);

}
//
REAL CImageZoom::getCurrentX(int x)
{
	CRect rect;
	m_pWnd->GetWindowRect(&rect);

	return (x-rect.left-m_pXstart)/m_pScale;
}
//
REAL CImageZoom::getCurrentY(int y)
{
	CRect rect;
	m_pWnd->GetWindowRect(&rect);

	return (y-rect.top-m_pYstart)/m_pScale;
}