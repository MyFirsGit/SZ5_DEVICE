#include "stdafx.h"
#include "MessageIDDefinition.h"
#include "LineGroup.h"
//#include <GdiPlus.h>
//#pragma comment(lib,"gdiplus.lib")
//using namespace Gdiplus;
//static Image* m_pImage = NULL;


IMPLEMENT_DYNAMIC(CLineGroup,CBaseDlg)
	
BEGIN_MESSAGE_MAP(CLineGroup,CBaseDlg)
	//ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数，从Table继承

@param      (i)UIINFO& uIInfo     当前控件的样式
@param      (i)UINT rows          需要创建Table的行数
@param      (i)UINT cols          需要创建Table的列数
@param      (i)UINT ID            需要创建按钮组的ID（用于XGroup事件中）
@param      (i)SELECTTYPE& selectType 按钮组中按钮的选择类型（多选，单选，不可选中三种）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLineGroup::CLineGroup(UIINFO areaUIInfo)
	:CBaseDlg(CLineGroup::IDD,NULL, RECT_BASE , RECEPTION_BK_COLOR,NULL,false)
{
	m_UIInfo = areaUIInfo;
	m_pParent = areaUIInfo.m_pParentWnd;
	if (areaUIInfo.m_nParentType==1){
		//// 初始化GDI+  
		//Gdiplus::GdiplusStartupInput gdiplusStartupInput;  
		//Gdiplus::GdiplusStartup( &m_gdiplusToken, &gdiplusStartupInput, NULL );
		m_fXstart = 0;
		m_fYstart = 0;
		m_fXOffset = 0;
		m_fYOffset = 0;
		m_fScale = 0;
		//m_pImage = new Image(m_UIInfo.m_strBackgroundImage);

		m_bEnlarge = false;
		m_bPushDown = false;
		m_bDragMap = false;
		bool isFull = false;

		//m_fScale=0.7796;//06缩放比例
		m_fScale = 1.0;
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CLineGroup::~CLineGroup()
{
	DeleteButton();
	//Gdiplus::GdiplusShutdown( m_gdiplusToken );
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLineGroup::BuildButton(vector<UIINFO>& buttonUIInfos,vector<BTN_INFO>& buttonInfos)
{
	DeleteButton();
	m_vecButtonUIInfo = buttonUIInfos;
	m_vecButtonInfo = buttonInfos;
	//for(int i=0;i<m_vecButtonUIInfo.size();i++)
	//{
	//	m_vecButtonUIInfo[i].m_pParentWnd = this;
	//	CXButton* pButton = new CXButton(m_vecButtonUIInfo[i],m_vecButtonInfo[i]);
	//	m_vecButton.push_back(pButton);
	//}
	//this->Create();
}

void CLineGroup::BuildButton(vector<UIINFO>& buttonUIInfos,vector<BTN_INFO>& buttonInfos,vector<TEXT_INFO>& textinfos)
{
	DeleteButton();
	m_vecButtonUIInfo = buttonUIInfos;
	m_vecButtonInfo = buttonInfos;
	//for(int i=0;i<m_vecButtonUIInfo.size();i++)
	//{
	//	m_vecButtonUIInfo[i].m_pParentWnd = this;
	//	CXButton* pButton = new CXButton(m_vecButtonUIInfo[i],m_vecButtonInfo[i],textinfos[i]);
	//	m_vecButton.push_back(pButton);
	//}
	//this->Create();
}

vector<CXButton*>& CLineGroup::getBtns()
{
	return m_vecButton;
}

vector<BTN_INFO>& CLineGroup::getBtnsInfo()
{
	return m_vecButtonInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      删除固定按钮
	
@param		无     

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CLineGroup::DeleteButton()
{
	//for (int i=0;i<m_vecButton.size();i++)
	//{
	//	CXButton* pButton = m_vecButton[i];
	//	delete pButton;
	//	pButton = NULL;
	//}
	m_vecButton.clear();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLineGroup::SetButtonUIInfos(vector<UIINFO>& buttonUIInfos)
{
	m_vecButtonUIInfo = buttonUIInfos;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CLineGroup::SetButtonInfos(vector<BTN_INFO>& buttonInfos)
{
	m_vecButtonInfo = buttonInfos;
}


//////////////////////////////////////////////////////////////////////////
/**
  @brief      按钮点击事件

  @param      (i)WPARAM   wParam     消息设置参数          
  @param      (i)LPARAM   lParam     消息设置参数

  @retval     LRESULT   \n

  @exception 无 
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CLineGroup::XButtonClick(WPARAM wParam, LPARAM lParam)
{
	UINT ID = static_cast<UINT>(wParam);
	CString msg = (LPCTSTR)lParam;	

	for(int i=0;i<m_vecButtonInfo.size();i++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[i];
		if(btnInfo.isSelected)
		{
			btnInfo.isSelected = false;
		}
		if(btnInfo.btnID == ID && btnInfo.btnMsg == msg)
		{
			btnInfo.isSelected = true;
		}
	}

	/*std::vector<CXButton*>::iterator itBtn=m_vecButton.begin();
	for(;itBtn!=m_vecButton.end();itBtn++)
	{
		if(ID==(*itBtn)->GetID())
		{
			(*itBtn)->updateParentText();
			break;
		}
	}*/
	
	//BTN_INFO& btnInfo = FindBtnInfo(ID,msg);
	//GetParentDlg()->PostMessage(WM_XLINEGROUP,m_ID,(LPARAM)&btnInfo);
	return -1;//阻止冒泡，不传到上一层。
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BTN_INFO& CLineGroup::FindBtnInfo(UINT ID,CString msg)
{
	for (int i=0;i<m_vecButtonInfo.size();i++)
	{
		BTN_INFO& btnInfo = m_vecButtonInfo[i];
		if(btnInfo.btnID == ID && btnInfo.btnMsg == msg)
		{
			return btnInfo;
		}
	}
	throw CInnerException(CInnerException::MODULE_ID,CInnerException::FUNC_ERR_RETURN,_T(__FILE__),__LINE__);
}


void CLineGroup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_UIInfo.m_nParentType==1){
		theSERVICE_MGR.SetState(RECEPT);
		if (!m_bEnlarge){
			// 放大总图
			CRect rect;
			GetWindowRect(&rect);
			m_fScale=(double)rect.Width()/(double)m_UIInfo.m_pBackgroundImage->GetWidth();

			double oldScale = m_fScale;
			m_fScale = 1.5;
			double tmpOffsetX = (m_fScale-oldScale) * (double)(point.x-m_fXstart)/oldScale;
			double tmpOffsetY = (m_fScale-oldScale) * (double)(point.y-m_fYstart)/oldScale;
			m_fXstart -= tmpOffsetX;
			m_fYstart -= tmpOffsetY;
			m_bEnlarge = true;

			CRect  borderRect = CRect(CPoint(0,0),RECT_BASE.Size());
			CRect contentRect = borderRect;
			contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
			InvalidateRect(&contentRect,FALSE);
		}
		else{
			m_curPoint = point;
			m_bPushDown = true;
		}
	}
	else{
		m_bPushDown = true;
	}
}


void CLineGroup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bPushDown){
		return;
	}
	m_bPushDown = false;
	if (m_UIInfo.m_nParentType==1){
		if (m_bEnlarge){
			if (!m_bDragMap){
				// 判断是否点击了车站按钮
				CheckLFBtnSelect(point);
			}
			m_bDragMap = false;
		}
	}
	else{
		CheckLFBtnSelect(point);
	}
}

void CLineGroup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_UIInfo.m_nParentType==1){
		if (m_bEnlarge){
			if(m_bPushDown){
				m_bDragMap = true;
				// 拖拽总图
				CRect clientRect;
				GetClientRect(&clientRect);
				CPoint offset;
				offset.x = point.x - m_curPoint.x;
				offset.y = point.y - m_curPoint.y;
				float tmp_x = m_fXstart + offset.x;
				float tmp_y = m_fYstart + offset.y;
				if ( (tmp_x>=0) 
					|| (tmp_x<(clientRect.Width()-m_UIInfo.m_pBackgroundImage->GetWidth()*m_fScale)) 
					|| (tmp_y>=0) 
					|| (tmp_y<(clientRect.Height()-m_UIInfo.m_pBackgroundImage->GetHeight()*m_fScale))){
					return;
				}

				m_curPoint = point;
				m_fXstart = tmp_x;
				m_fYstart = tmp_y;
/*				Invalidate(FALSE);	*/
				CRect  borderRect = CRect(CPoint(0,0),RECT_BASE.Size());
				CRect contentRect = borderRect;
				contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
				InvalidateRect(&contentRect,FALSE);
			}
		}
	}
}

//根据鼠标左键坐标判断当前站点
void CLineGroup::CheckLFBtnSelect(CPoint point)
{
	CPoint SrcPoint = point;
	if(m_UIInfo.m_nParentType==1){
		SrcPoint = CPoint(getCurrentX(point.x),getCurrentY(point.y));
	}

	vector<UIINFO>::iterator iter = m_vecButtonUIInfo.begin();
	vector<BTN_INFO>::iterator iters = m_vecButtonInfo.begin();
	for (;iter != m_vecButtonUIInfo.end(),iters != m_vecButtonInfo.end();iter++,iters++)
	{
		if (iter->m_Area.PtInRect(SrcPoint))
		{
			m_pParent->PostMessage(WM_XLINEGROUP,0,(LPARAM)&iters->btnID);
			break;
		}
		else
		{
			continue;
		}
	}
}


double CLineGroup::getCurrentX(int x)
{
	return (x-m_fXstart)/m_fScale;
}

double CLineGroup::getCurrentY(int y)
{
	return (y-m_fYstart)/m_fScale;
}




void CLineGroup::ShowFunc(CDC* pDc)
{	
	CRect  roundRect = CRect(CPoint(0,0),RECT_BASE.Size());
	//CRect roundRect;
	//GetClientRect(roundRect);
	// 创建内存DC
	CDC MemDc;
	MemDc.CreateCompatibleDC(NULL);// 内存中准备图像
	// 创建内存位图
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDc,roundRect.Width(),roundRect.Height());
	// 内存位图与绘图DC关联
	MemDc.SelectObject(MemBitmap);
	// 设置背景色
	MemDc.FillSolidRect(0,0,roundRect.Width(),roundRect.Height(),RGB(255,255,255));

	CRgn rgn;
	rgn.CreateRoundRectRgn(roundRect.left+2,roundRect.top+2,roundRect.right-2,roundRect.bottom-2,30,30);
	SetWindowRgn(rgn,FALSE);

	// 使用内存DC绘图
	CDC* pTmpDc = &MemDc;

	// 绘图中。。。
	// 绘制边框
	CPen pen(PS_SOLID, 2, RGB(0,0,0));
	CPen* pOldPen = (CPen*)pTmpDc->SelectObject(&pen);
	pTmpDc->Rectangle(roundRect);
	pTmpDc->SelectObject(pOldPen);	
	// 绘制内容
	pTmpDc->SetBkMode( m_UIInfo.m_BKMode);
	//pDc->Rectangle(&borderRect);
	//pDc->SelectObject(pOldPen);	
	if(m_UIInfo.m_BKMode!=TRANSPARENT && m_UIInfo.m_pBackgroundImage)
	{
		if(m_UIInfo.m_nParentType==1){//总图窗口
			if (!m_bEnlarge){
				CRect contentRect = roundRect;
				contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
				m_UIInfo.m_pBackgroundImage->Draw(pTmpDc->m_hDC,contentRect);
			}
			else{
				//CRect contentRect = CRect(CPoint(m_fXstart,m_fYstart), CSize(m_pImage->GetWidth()*m_fScale,m_pImage->GetHeight()*m_fScale));
				//m_UIInfo.m_pBackgroundImage->Draw(pDc->m_hDC,contentRect);

				CRect contentRect = roundRect;
				contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
				CRect orgImg((-1)*m_fXstart/m_fScale, (-1)*m_fYstart/m_fScale, ((-1)*m_fXstart + contentRect.Width())/m_fScale, ((-1)*m_fYstart + contentRect.Height())/m_fScale);
				m_UIInfo.m_pBackgroundImage->StretchBlt(pTmpDc->m_hDC,contentRect,orgImg,SRCCOPY);
			}
		}
		else{// 线路图窗口
			CRect contentRect = roundRect;
			contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
			m_UIInfo.m_pBackgroundImage->Draw(pTmpDc->m_hDC,contentRect);
		}
	}

	// 将缓冲区的图形拷贝到绘图DC
	pDc->BitBlt(0,0,roundRect.Width(),roundRect.Height(),pTmpDc,0,0,SRCCOPY);
	// 绘图清理
	MemBitmap.DeleteObject();
	MemDc.DeleteDC();
	// 绘图结束	
	
	return;
	

	CRect  borderRect = CRect(CPoint(0,0),RECT_BASE.Size());
	// 绘制边框
	//CPen pen(PS_SOLID, 2, RGB(0,0,0));
	//CPen* pOldPen = (CPen*)pDc->SelectObject(&pen);
	pDc->SetBkMode( m_UIInfo.m_BKMode);
	//pDc->Rectangle(&borderRect);
	//pDc->SelectObject(pOldPen);	
	if(m_UIInfo.m_BKMode!=TRANSPARENT && m_UIInfo.m_pBackgroundImage)
	{
		if(m_UIInfo.m_nParentType==1){//总图窗口
			if (!m_bEnlarge){
				CRect contentRect = borderRect;
				contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
				m_UIInfo.m_pBackgroundImage->Draw(pDc->m_hDC,contentRect);
			}
			else{
				//CRect contentRect = CRect(CPoint(m_fXstart,m_fYstart), CSize(m_pImage->GetWidth()*m_fScale,m_pImage->GetHeight()*m_fScale));
				//m_UIInfo.m_pBackgroundImage->Draw(pDc->m_hDC,contentRect);

				CRect contentRect = borderRect;
				contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
				CRect orgImg((-1)*m_fXstart/m_fScale, (-1)*m_fYstart/m_fScale, ((-1)*m_fXstart + contentRect.Width())/m_fScale, ((-1)*m_fYstart + contentRect.Height())/m_fScale);
				m_UIInfo.m_pBackgroundImage->StretchBlt(pDc->m_hDC,contentRect,orgImg,SRCCOPY);
			}
		}
		else{// 线路图窗口
			CRect contentRect = borderRect;
			contentRect.DeflateRect(m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth,m_UIInfo.m_BorderWidth);
			m_UIInfo.m_pBackgroundImage->Draw(pDc->m_hDC,contentRect);
		}
	}

	//Graphics graphics(pDc->m_hDC);
	//graphics.Clear(BK_COLOR);
	//graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
	//graphics.DrawImage(m_pImage,(REAL)(m_fXstart),m_fYstart,m_pImage->GetWidth()*scale,m_pImage->GetHeight()*m_fScale);
}


void CLineGroup::OnDraw(CDC* pDC)
{
	ShowFunc(pDC);
}


int CLineGroup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
}