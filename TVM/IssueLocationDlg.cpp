// IssueLocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TVM.h"
#include "IssueLocationDlg.h"
#include "IssueAreaMapDlg.h"
// 检查内存泄漏
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CRect  RECT_BASE = CRect(CPoint(REC_LINE_MAP_RECT.left, 0),CSize(REC_LINE_MAP_RECT.Width()-2,REC_LINE_MAP_RECT.Height()));

static const CRect  RECT_LEFT = CRect(CPoint(0,RECT_BASE.Height()/2-100/2),CSize(50,100));
static const CRect  RECT_TOP  = CRect(CPoint(RECT_BASE.Width()/2-100/2,0),CSize(100,50));
static const CRect  RECT_RIGHT = CRect(CPoint(RECT_BASE.Width()-50,RECT_BASE.Height()/2-100/2),CSize(50,100));
static const CRect  RECT_BOTTOM = CRect(CPoint(RECT_BASE.Width()/2-100/2,RECT_BASE.Height()-50),CSize(100,50));

// CIssueLocationDlg dialog
IMPLEMENT_DYNAMIC(CIssueLocationDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(CIssueLocationDlg, CBaseDlg)
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CIssueLocationDlg::CIssueLocationDlg(CWnd* wnd,CService* pService)
	:CBaseDlg(CIssueLocationDlg::IDD,NULL, RECT_BASE , RECEPTION_BK_COLOR,NULL,false)
{
	pMainWnd = wnd;
	pSvc = (CIssueSvc*)pService;
	m_lineGroupArea = NULL;
	m_mousepressed = false;  
	InitializeLineGroup();
	pStatic = new CPictureEx;
	m_pImageZoom = NULL;
	m_btnLeft = NULL;
	m_btnTop = NULL;
	m_btnRight = NULL;
	m_btnBottom = NULL;
	CString apppath;
	GetAppPath(apppath);
	m_leftImage = new Image(apppath + TVM_GUI_BTN_LEFT);
	m_topImage = new Image(apppath + TVM_GUI_BTN_TOP);
	m_rightImage = new Image(apppath+ TVM_GUI_BTN_RIGHT);
	m_bottomImage = new Image(apppath+ TVM_GUI_BTN_BOTTOM);
}

CIssueLocationDlg::~CIssueLocationDlg()
{

}
void CIssueLocationDlg::ReFrush()
{
	m_mousepressed = false;
	if (m_pImageZoom != NULL)
	{
		delete m_pImageZoom;
		m_pImageZoom = NULL;
	}
	if (m_btnLeft != NULL)
	{
		delete m_btnLeft;
		m_btnLeft = NULL;
	}
	if (m_btnTop != NULL)
	{
		delete m_btnTop;
		m_btnTop = NULL;
	}
	if (m_btnRight != NULL)
	{
		delete m_btnRight;
		m_btnRight = NULL;
	}
	if (m_btnBottom != NULL)
	{
		delete m_btnBottom;
		m_btnBottom = NULL;
	}
	InitializeLineGroup();
	Invalidate();
}
bool CIssueLocationDlg::InitializeLineGroup(DWORD layoutID /* = DEFAULT_LAYOUT */)
{
	m_vec_BtnUIInfo.clear();
	m_vec_buttonInfos.clear();
	m_vec_textInfos.clear();
	m_vec_hotLocation.clear();
	LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentClientLanguageType();

	// 获取布局
	//AREA_LAYOUT layout;
	MAP_LAYOUT layout;

	theGUIMAP_MGR.GetLineMapInfo(layoutID,layout);

	// 判断是否启用布局
	if (!layout.bEnable) return false;

	// 初始化线路面板信息
	m_LineAreaUIInfo.m_pParentWnd			= this;
	m_LineAreaUIInfo.m_DockStyle			= DOCKSTYLE::NONE;
	m_LineAreaUIInfo.m_Location				= CPoint(0,0);
	m_LineAreaUIInfo.m_Size					= layout.mapSize;
	m_LineAreaUIInfo.m_BKMode				= TRANSPARENT;
	m_LineAreaUIInfo.m_BorderWidth			= 0;
	m_LineAreaUIInfo.m_Margins				= DEFAULT_GROUPBOX_MARGINS;
	//m_LineAreaUIInfo.m_pBackgroundImage		= layout.pImageBackupground;
	m_LineAreaUIInfo.m_strBackgroundImage	= layout.szBackupground;
	// 获取布局按钮
	//vector<AREA_BTN_INFO> vecLayoutButton;
	//theBitmap_MGR.m_guiparam.getAreaBtnInfo(layout.layOutId, language, vecLayoutButton);
	vector<MAP_BTN_INFO> vecBtnInfo;
	theGUIMAP_MGR.GetLayoutBtnInfo(layout.dwLineId,vecBtnInfo);

	// 初始化线路按钮信息
	vector<MAP_BTN_INFO>::iterator iter = vecBtnInfo.begin();
	vector<MAP_BTN_INFO>::iterator end = vecBtnInfo.end();
	for(; iter < end; ++iter)
	{
		MAP_BTN_INFO& element = *iter;

		// 是否跳转
		//bool isSkipSheet = element.nextPage != 0;
		// 获取当前车站编号
		DWORD locationNum = element.dwBtnId;//isSkipSheet ? 0 : element.btnID;
		// 判断是否当前车站
		bool isCurrentStation = false;//isSkipSheet ? false : (element.btnID == theMAINTENANCE_INFO.GetStationCodeWithLine());
		// 选择状态图（必须为.bmp格式）
		UIINFO uiInfo;
		uiInfo.m_pBackgroundImage = element.pBtnImage;

		// 按钮是否启用判断
		bool enable_button = element.bEnable ? (pSvc->SupportSJT() ? true : false) : false;

		// 创建面板信息

		uiInfo.m_pParentWnd			= this;
		uiInfo.m_DockStyle			= DOCKSTYLE::NONE;
		uiInfo.m_Location			= element.startBntPiont;
		uiInfo.m_Size				= element.sizeOfBtn;
		uiInfo.m_BKMode				= TRANSPARENT;
		uiInfo.m_BorderWidth		= 0;
		uiInfo.m_Margins			= DEFAULT_GROUPBOX_MARGINS;
		uiInfo.m_Visible			= true;//element.isVisiable;
		uiInfo.m_Enabled			= enable_button;/*element.isEnable;*/
		//获取按钮区域，用于判断当前是否选择车站
		uiInfo.m_Area				= CRect(uiInfo.m_Location.x,uiInfo.m_Location.y,uiInfo.m_Size.cx+uiInfo.m_Location.x,uiInfo.m_Size.cy+uiInfo.m_Location.y);
		m_vec_BtnUIInfo.push_back(uiInfo);
		// 创建按钮信息
		BTN_INFO btnInfo;
		btnInfo.btnID = element.dwBtnId;
		btnInfo.btnMsg.Format(_T("%d"), 0/*element.nextPage*/);
		btnInfo.text = _T("");//isSkipSheet ? element.text : _T("");//theACC_LOCATION.GetLocationName(locationNum, language);
		btnInfo.isEnable = enable_button;/*element.isEnable;*/
		btnInfo.type = BUTTON_TYPE_LINE_AREA;
		m_vec_buttonInfos.push_back(btnInfo);

		// 字体信息
		TEXT_INFO textinfo;
		m_vec_textInfos.push_back(textinfo);
	}

	return true;
}
//根据鼠标左键坐标判断当前站点
void CIssueLocationDlg::CheckLFBtnSelect(CPoint point)
{
	CPoint SrcPoint = CPoint(m_pImageZoom->getCurrentX(point.x),m_pImageZoom->getCurrentY(point.y));
	vector<UIINFO>::iterator iter = m_vec_BtnUIInfo.begin();
	vector<BTN_INFO>::iterator iters = m_vec_buttonInfos.begin();
	for (;iter != m_vec_BtnUIInfo.end(),iters != m_vec_buttonInfos.end();iter++,iters++)
	{
		if (iter->m_Area.PtInRect(SrcPoint))
		{
			pMainWnd->PostMessage(WM_XLINEGROUP,0,(LPARAM)&iters->btnID);
			break;
		}
		else
		{
			continue;
		}
	}
}
BOOL CIssueLocationDlg::RectTrans(CPoint SrcPoint)
{

	CPoint DescPoint = SrcPoint;
	ScreenToClient(&DescPoint);
	if (RECT_LEFT.PtInRect(DescPoint))
	{
		m_pImageZoom->setStartX(100);
		m_pImageZoom->OnMouseMove(SrcPoint);
		return TRUE;
	}
	if (RECT_TOP.PtInRect(DescPoint))
	{
		m_pImageZoom->setStartY(100);
		m_pImageZoom->OnMouseMove(SrcPoint);
		return TRUE;
	}
	if (RECT_RIGHT.PtInRect(DescPoint))
	{
		m_pImageZoom->setStartX(-100);
		m_pImageZoom->OnMouseMove(SrcPoint);
		return TRUE;
	}
	if (RECT_BOTTOM.PtInRect(DescPoint))
	{
		m_pImageZoom->setStartY(-100);
		m_pImageZoom->OnMouseMove(SrcPoint);
		return TRUE;
	}
	return FALSE;
}
void CIssueLocationDlg::Invalidate(){
	if (m_pImageZoom == NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		pStatic->Create(_T(""),WS_CHILD |WS_VISIBLE |WS_TABSTOP ,rect,this,IDC_PIC);
		m_pWnd = CWnd::FromHandle(pStatic->GetSafeHwnd());
		m_pDC=m_pWnd->GetDC();
		m_pImageZoom = new CImageZoom(m_pWnd,m_LineAreaUIInfo.m_strBackgroundImage);
	}
	m_pImageZoom->setImgShow();
}
BOOL CIssueLocationDlg::PreTranslateMessage(MSG* pMsg)
{	
	if (pMsg->message==WM_LBUTTONDOWN)
	{
		if (!m_mousepressed)
		{
			m_pImageZoom->OnLButtonDown(pMsg->pt);
			m_mousepressed = true;
			CreateButton();

		}
		else
		{
			if(RectTrans(pMsg->pt))
			{
				CreateButton();
			}
			else
			{CheckLFBtnSelect(pMsg->pt);}
		}
		
	}

	else if (pMsg->message==WM_LBUTTONUP)
	{
		//m_pImageZoom->OnLButtonUp(pMsg->pt);
	}

	else if (pMsg->message==WM_MOUSEMOVE && m_mousepressed)
	{
		//m_pImageZoom->OnMouseMove(pMsg->pt);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
void  CIssueLocationDlg::RecalculateBtnInfo()
{
	vector<UIINFO> vecBtnUIInfo = m_vec_BtnUIInfo;
	m_vec_BtnUIInfo.clear();
	vector<UIINFO>::iterator iter = vecBtnUIInfo.begin();
	UIINFO btnUIInfo;
	for(;iter != vecBtnUIInfo.end();iter++)
	{
		btnUIInfo = *iter;
		{
			btnUIInfo.m_Area = CRect(CPoint(m_pImageZoom->getCurrentX(iter->m_Area.left),m_pImageZoom->getCurrentY(iter->m_Area.top)),CSize(iter->m_Area.Width()*m_pImageZoom->getScale(),iter->m_Area.Height()*m_pImageZoom->getScale()));
		}
		m_vec_BtnUIInfo.push_back(btnUIInfo);
	}
}
void CIssueLocationDlg::CreateButton()
{
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_BITMAP;
	Graphics graphics(this->GetWindowDC()->m_hDC);
	if (m_btnLeft == NULL)
	{
		//m_btnLeft = new CStatic;
		//m_btnLeft->Create(_T(""),dwStyle,RECT_LEFT_BTN,this,IDC_BTN_LEFT);
		//m_btnLeft->GetWindowDC()->SetBkMode(TRANSPARENT);
		//Graphics graphics1(m_leftImage/*m_btnLeft->GetWindowDC()->m_hDC*/);
		graphics.DrawImage(m_leftImage,RECT_LEFT.left,RECT_LEFT.top);
	}
	if (m_btnTop == NULL)
	{
		//m_btnTop = new CStatic;
		//m_btnTop->Create(_T(""),dwStyle,RECT_TOP_BTN,this,IDC_BTN_TOP);
		//Graphics graphics2(m_btnTop->GetWindowDC()->m_hDC);
		graphics.DrawImage(m_topImage,RECT_TOP.left,RECT_TOP.top);

	}
	if (m_btnRight == NULL)
	{
		//m_btnRight = new CStatic;
		//m_btnRight->Create(_T(""),dwStyle,RECT_RIGHT_BTN,this,IDC_BTN_RIGHT);
		//Graphics graphics3(m_btnRight->GetWindowDC()->m_hDC);
		graphics.DrawImage(m_rightImage,RECT_RIGHT.left,RECT_RIGHT.top);

	}
	if (m_btnBottom == NULL)
	{
		//m_btnBottom = new CStatic;
		//CRect rect(CPoint(550,950),CSize(100,50));
		//m_btnBottom->Create(_T(""),dwStyle,rect/*RECT_BOTTOM_BTN*/,this,IDC_BTN_BOTTOM);
		//Graphics graphics4(m_btnBottom->GetWindowDC()->m_hDC);
		graphics.DrawImage(m_bottomImage,RECT_BOTTOM.left,RECT_BOTTOM.top);

	}

}

void CIssueLocationDlg::OnDraw(CDC* pDC)
{
	Invalidate();
}
