#include "stdafx.h"
#include "TVM.h"
#include "OutOfReceptionDlg.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const COLORREF CL_BRUSH_TIP = RGB(217,217,217);	//服务终止界面提示框背景色
static const COLORREF CL_BRUSH_BK = RGB(236,240,241);	//服务终止界面背景色

static const COLORREF TXT_FONT_COLOR = theGUI_INFO.GetServiceStateFontColor();	//提示字体的颜色
static const DWORD TIP_FONT_SIZE = theGUI_INFO.GetServiceStateFontSize(); // 字体大小

#define TIP_FRAME_WIDTH		900	//服务终止时 文字（文字+图片）提示框的宽度
#define TIP_FRAME_HEIGHT	500	//服务终止时 文字（文字+图片）提示框的高度
#define HEAD_MENUE_HEIGHT	100	//顶部和底部区域的高度
//#define TIP_FONT_SIZE		100	//提示字体大小
#define TIP_PIC_WIDTH		270	//提示图片宽度
#define TIP_PIC_HEIGHT		300	//提示图片高度



static const CRect WELCOME_FRAME_RECT_RATIO = CRect(WELCOME_FRAME_RECT.left*REC_RATIOWIDTH, WELCOME_FRAME_RECT.top*REC_RATIOHEIGHT,
	WELCOME_FRAME_RECT.right*REC_RATIOWIDTH, WELCOME_FRAME_RECT.bottom*REC_RATIOHEIGHT);

static CRect TEXT_RECT_CN = CRect(10*REC_RATIOWIDTH,350*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,550*REC_RATIOHEIGHT);
static CRect TEXT_RECT_EN = CRect(10*REC_RATIOWIDTH,550*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,750*REC_RATIOHEIGHT);
static CRect TEXT_RECT_PN = CRect(10*REC_RATIOWIDTH,750*REC_RATIOHEIGHT,1270*REC_RATIOWIDTH,1000*REC_RATIOHEIGHT);

static CRect VERSION_RECT = CRect(1000*REC_RATIOWIDTH,1000*REC_RATIOHEIGHT,1280*REC_RATIOWIDTH,1024*REC_RATIOHEIGHT);

IMPLEMENT_DYNAMIC(COutOfReceptionDlg, CBaseDlg)

	BEGIN_MESSAGE_MAP(COutOfReceptionDlg, CBaseDlg)
		ON_WM_ACTIVATE()
		ON_WM_CREATE()
	END_MESSAGE_MAP()

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      构造函数

	@param      (i)CWnd*     pParent

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	COutOfReceptionDlg::COutOfReceptionDlg(CService* pService)
		:CBaseDlg(COutOfReceptionDlg::IDD, NULL,RECT_OUTSVC, RECEPTION_HEAD_BK_COLOR, pService, false)
	{

	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      构造函数

	@param      (i)CWnd*     pParent

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	COutOfReceptionDlg::COutOfReceptionDlg(CService* pService, END_SVC_UI& ui)
		:CBaseDlg(COutOfReceptionDlg::IDD, NULL, RECT_OUTSVC, RECEPTION_HEAD_BK_COLOR, pService, false)
	{
		m_ui = ui;
		m_curStatues = SERVICE_OF_STATUES::SERVICE_STATUES_OUTOF_SVC;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      析构函数

	@param      无

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	COutOfReceptionDlg::~COutOfReceptionDlg()
	{
		//delete m_pHeadDLg;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面绘制函数

	@param      (i) CDC* pDC  设备上下文

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void COutOfReceptionDlg::OnDraw(CDC* pDC)
	{
		__super::OnDraw(pDC);

		CRect rect_base(RECT_OUTSVC.left,RECT_OUTSVC.top-130, RECT_OUTSVC.right,RECT_OUTSVC.bottom +130);//顶部被ReceptionHeadDlg占据 所以 重绘时的坐标要调整
		CPen *pen = new CPen();
		CBrush *pBrush_Rgn = new CBrush(CL_BRUSH_TIP);		//提示框背景
		CBrush *pBrush_base = new CBrush(CL_BRUSH_BK);		//整个界面的背景

		CBrush *pOldBrush = pDC->SelectObject(pBrush_base);						 
		pDC->FillRect(rect_base/*RECT_OUTSVC*/, pBrush_base);			//刷服务终止时的界面背景（浅灰）
		pDC->SelectObject(pOldBrush); 

		CRgn *pRgn = new CRgn;								//刷提示框的圆角矩形
		int rgn_left = (RECT_OUTSVC.right - TIP_FRAME_WIDTH)/2;
		int rgn_top = (RECT_OUTSVC.bottom - TIP_FRAME_HEIGHT + HEAD_MENUE_HEIGHT -100)/2;//
		CRect rectRgn(rgn_left,rgn_top, rgn_left+ TIP_FRAME_WIDTH, rgn_top + TIP_FRAME_HEIGHT);
		pRgn->CreateRoundRectRgn(rectRgn.left, rectRgn.top, rectRgn.right, rectRgn.bottom/*rgn_left,rgn_top, rgn_left+ rgn_width,rgn_top + rgn_height*/, 10,10);

		pOldBrush = pDC->SelectObject(pBrush_Rgn);
		pDC->FillRgn(pRgn, pBrush_Rgn);
		pDC->SelectObject(pOldBrush);

		CRect rect_txt;
		CRect rect_pic;
		CFont *pFont = new CFont();							//创建字体
		CFont *pFont_en = new CFont();						//创建字体
		CString textfont = theGUI_INFO.GetServiceStateFontName();
		int nFontSize = TIP_FONT_SIZE;						//提示字体大小
		int nFirstLine = (TIP_FRAME_HEIGHT - 2*TIP_FONT_SIZE)/2;//首行处理

		if(m_ui.isTxtOnly)
		{
			rect_txt = CRect(rgn_left,rgn_top, rgn_left+ TIP_FRAME_WIDTH, rgn_top + TIP_FRAME_HEIGHT);
		}
		else
		{
			rect_pic = CRect(rectRgn.left + 10, rectRgn.top + (TIP_FRAME_HEIGHT - TIP_PIC_HEIGHT)/2, rectRgn.left+10+TIP_PIC_WIDTH, rectRgn.top + TIP_PIC_HEIGHT + (TIP_FRAME_HEIGHT - TIP_PIC_HEIGHT)/2);//居中
			rect_txt = CRect(rect_pic.right,rgn_top, rgn_left+ TIP_FRAME_WIDTH,rgn_top + TIP_FRAME_HEIGHT);//其他坐标不变 只是left右移了
			m_picex.loadPic(m_ui.strPicPath);
			m_picex.MoveWindow(rect_pic);
			m_picex.resumeRender();
			//m_picex.renderPicture(m_ui.strPicPath,this->m_hWnd,rect_pic);
		}

		pFont->CreateFont(nFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS, textfont);

		CFont* pOldFont = pDC->SelectObject(pFont);
		if(INIT == theSERVICE_MGR.GetState()){
			pDC->SetTextColor(BLUE);
		}
		else{
			pDC->SetTextColor(TXT_FONT_COLOR);
		}
		pDC->SetBkMode(TRANSPARENT);
		

		rect_txt.top += nFirstLine;//首行下移  保证整体上下居中
		pDC->DrawText(m_ui.strTxtCN, &rect_txt, DT_WORDBREAK | DT_VCENTER | DT_CENTER);

		pFont_en->CreateFont(nFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS, _T("Arial"));
		 pDC->SelectObject(pFont_en);

		rect_txt.top += nFontSize;//行间距
		pDC->DrawText(m_ui.strTxtEN, &rect_txt, DT_WORDBREAK | DT_VCENTER | DT_CENTER);
		//rect_txt.top += nFontSize;//行间距
		//pDC->DrawText(m_ui.strTxtPN, &rect_txt, DT_WORDBREAK | DT_VCENTER | DT_CENTER);

		// 防止内存泄露
		delete pen;
		pen = NULL;
		delete pBrush_Rgn;
		pBrush_Rgn = NULL;
		delete pBrush_base;
		pBrush_base = NULL;
		delete pRgn;
		pRgn = NULL;
		delete pFont;
		pFont = NULL;
		delete pFont_en;
		pFont_en = NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      画面激活后的处理

	@param      (i)UINT nState      不使用
	@param      (i)CWnd* pWndOther  不使用
	@param      (i)BOOL bMinimized  不使用

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void COutOfReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
	{
		//// 隐藏乘客画面标题与向导
		theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_SHOW);
		__super::OnActivate(nState,pWndOther,bMinimized);

	}

	int COutOfReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDialog::OnCreate(lpCreateStruct) == -1)
			return -1;
		MoveWindow(RECT_OUTSVC.left,RECT_OUTSVC.top,RECT_OUTSVC.Width(),RECT_OUTSVC.Height());

		m_picex.createPicWnd(this->m_hWnd);
		return 0;
	}
	BOOL COutOfReceptionDlg::OnInitDialog()
	{
		CDialog::OnInitDialog();

		::MoveWindow(this->GetSafeHwnd(),RECT_OUTSVC.left,RECT_OUTSVC.top,RECT_OUTSVC.right,RECT_OUTSVC.bottom,TRUE);

		return TRUE;  // return TRUE  unless you set the focus to a control
	}
	//////////////////////////////////////////////////////////////////////////
	/**
	@brief      显示窗口

	@param      (i)CONTENT_TYPE contentType 指定内容

	@retval     无

	@exception  无
	*/
	//////////////////////////////////////////////////////////////////////////
	void COutOfReceptionDlg::Show(/*CONTENT_TYPE contentType*/)
	{
		SERVICE_OF_STATUES currentStatus;
		if(INIT == theSERVICE_MGR.GetState()){
			m_ui.strTxtCN = _cn(TXT_GUIDE_MSG_SYSTEM_INITIALIZING);
			m_ui.strTxtEN = _en(TXT_GUIDE_MSG_SYSTEM_INITIALIZING);
			//m_ui.strTxtPN = _po(TXT_GUIDE_MSG_SYSTEM_INITIALIZING);
			currentStatus = SERVICE_STATUES_INIT;
		}
		// 关机中
		else if(SHUTDOWNING == theTVM_STATUS_MGR.GetRunStatus()){
			m_ui.strTxtCN = _cn(TXT_GUIDE_MSG_SYSTEM_SHUTDOWNING);
			m_ui.strTxtEN = _en(TXT_GUIDE_MSG_SYSTEM_SHUTDOWNING);
			//m_ui.strTxtPN = _po(TXT_GUIDE_MSG_SYSTEM_SHUTDOWNING);
			currentStatus = SERVICE_STATUES_SHUTING;
		}
		// 停止服務
		else if(STOP_SERVICE == theTVM_STATUS_MGR.GetServiceStatus()){
			m_ui.strTxtCN = _cn(TXT_GUIDE_MSG_SYSTEM_STOP_SERVICE);
			m_ui.strTxtEN = _en(TXT_GUIDE_MSG_SYSTEM_STOP_SERVICE);
			//m_ui.strTxtPN = _po(TXT_GUIDE_MSG_SYSTEM_STOP_SERVICE);
			currentStatus = SERVICE_STATUES_STOP_SVC;
		}	
		// 暫停服務
		else /*if(OUTOF_SERVICE == theTVM_STATUS_MGR.GetServiceStatus())*/{
			m_ui.strTxtCN = _cn(TXT_GUIDE_MSG_SYSTEM_OUT_OF_SERVICE);
			m_ui.strTxtEN = _en(TXT_GUIDE_MSG_SYSTEM_OUT_OF_SERVICE);
		//	m_ui.strTxtPN = _po(TXT_GUIDE_MSG_SYSTEM_OUT_OF_SERVICE);
			currentStatus = SERVICE_STATUES_OUTOF_SVC;
		}

		 //当前没有显示，且不是待机中
		if(!this->IsWindowVisible() && REST != theSERVICE_MGR.GetState() || m_curStatues != currentStatus){
			// 显示头部及菜单栏			
			theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_NORMAL);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
			// 显示功能区按钮，隐藏语言选择按钮，取消按钮
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowChargeButton(false);
			// 立即执行
			theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->UpdateWindow();
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->UpdateWindow();
			m_curStatues = currentStatus;
			this->ShowWindow(SW_SHOW);
			this->Invalidate();
			this->UpdateWindow();
		}
	}
