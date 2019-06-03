//  [6/25/2016 yufuxiang]

#pragma once

#include <GdiPlus.h>
#pragma comment(lib,"gdiplus.lib")

using namespace Gdiplus;

#define BK_COLOR                          (Color(234,234,235))
#define MIN_SCALE                         (0.4)
#define CHANGE_SCALE_RATE                 (2)
//0.2
#define MIN_SCALE_RESLOVING               (600)


class CImageZoom
{
public:
	CImageZoom(CWnd* pWnd);
	CImageZoom(CWnd* pWnd,CString imgPath,BOOL isFull=FALSE);
	~CImageZoom(void);
public:
	void setImgShow();
public:
	void OnLButtonDown(POINT point);
	void OnLButtonUp(POINT point);
	void OnMouseMove(POINT point);
	void OnMouseWheel(short zDelta, CPoint pt);

public:
	void ReSetScale(){
		m_pScale = 0.4956;
		m_bIsLBtnDown=FALSE;
		m_pMouDownPnt.x=0;m_pMouDownPnt.y=0;
		m_pXoffset=0.0;
		m_pYoffset=0.0;
		m_pXstart=0.0;
		m_pYstart=0.0;
}
	double getScale(){ return m_pScale;}
	REAL getCurrentX(int x);
	void setStartX(int x){
		CRect rect;
		m_pWnd->GetClientRect(&rect);
		REAL tmp = m_pXstart;
		if (tmp + x >= 0)
		{
			m_pXstart = 0;
		}
		else if ((fabs(tmp+x)+rect.Width())>pImage->GetWidth()*m_pScale)
		{
			m_pXstart = rect.Width() - pImage->GetWidth()*m_pScale;
		}
		else
		{
			m_pXstart += x;
		}
	}
	REAL getCurrentY(int y);
	void setStartY(int y){
		CRect rect;
		m_pWnd->GetClientRect(&rect);
		REAL tmp = m_pYstart;
		if (tmp + y >= 0)
		{
			m_pYstart = 0;
		}
		else if ((fabs(tmp+y)+rect.Height())>pImage->GetHeight()*m_pScale)
		{
			m_pYstart =(rect.Height() - pImage->GetHeight()*m_pScale);
		}
		else
		{
			m_pYstart += y;
		}
	}
private:
	CWnd* m_pWnd;
	CDC* m_pDC;
	CString m_pImgPath;
	Image* pImage;
	//
	BOOL m_bIsLBtnDown;
	POINT m_pMouDownPnt;
	//
	REAL m_pXoffset;
	REAL m_pYoffset;
	REAL m_pXstart;
	REAL m_pYstart;

	double m_pScale;
protected:
	ULONG_PTR m_gdiplusToken;
};

