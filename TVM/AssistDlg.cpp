#include "StdAfx.h"
#include "AssistDlg.h"
#include "AssistSvc.h"
#include "GUIInfo.h"
#include "ReceptionDlg.h"

#define    TIP_HEIGTH           500                  //矩形区域高度
#define    TIP_WIDTH            900                  //矩形区域宽度
#define    TIP_BK_COLOR         RGB(179,179,179)     //矩形区域背景色
#define    TIP_TXT_SIZE         80                   //提示文字的大小
#define    TXT_FIRST_LINE       120                  //文字首行与矩形区域上边框的距离


IMPLEMENT_DYNAMIC(CAssistDlg, CReceptionDlg)
BEGIN_MESSAGE_MAP(CAssistDlg, CReceptionDlg)
	ON_WM_CREATE()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_BTN_BACK_CLICK(OnCancel)

END_MESSAGE_MAP()

CAssistDlg::CAssistDlg(CService *pSvc)
	: CReceptionDlg(CAssistDlg::IDD, pSvc )
{
	// DrawTipDlg();
	m_baseInfo->titleInfo.titleName = _opl(IO_TEST_TEXT_ASSISTANT);
	m_baseInfo->stytle = PASSAGE_WIDE_STYLE;
	m_baseInfo->detailInfo.row = 20;
	//m_baseInfo->detailInfo.labelGroup = baseInfoGroup;
}


CAssistDlg::~CAssistDlg(void)
{
	ShowCompoment(m_Right_1_Group);                           //隐藏无关区域
	ShowCompoment(m_baseInfoArea);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(true);//lichao: 暂时屏蔽语言切换按钮
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
	if (m_btnCancel != NULL)
	{
		delete m_btnCancel;
		m_btnCancel = NULL;
	}

}


void CAssistDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
}

int CAssistDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try{    		
		DrawRoundRect();
		if (__super::OnCreate(lpCreateStruct, GUIDE_AREA| BASE_AREA ) == -1) {
			return -1;
		}
	}
	// 异常处理
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		//theEXCEPTION_MGR.ProcessException(CInnerException(pAnalyzeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	return 0;
} 

void CAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void CAssistDlg::UpdateUI()
{
	CMessagePictureBox::End();
	m_baseInfoArea->Invalidate();
	DrawRoundRect();
}

VOID CAssistDlg::DrawRoundRect()
{
	HideCompoment(m_Right_1_Group);                           //隐藏无关区域
	HideCompoment(m_baseInfoArea);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(true);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);

	CPaintDC dc(this);                                        //创建当前对话框的设备上下文 
	CRect rect;
	rect.left   = (REC_FRMAE_RECT.Width()  - TIP_WIDTH) / 2;  //计算一个居中的圆角矩形提示框区域
	rect.top    = (REC_FRMAE_RECT.Height() - TIP_HEIGTH) / 2;
	rect.bottom = rect.top + TIP_HEIGTH;
	rect.right  = rect.left + TIP_WIDTH;

	CBrush *pColorBrush = new CBrush(TIP_BK_COLOR);           //提示区域的背景色,深灰

	CPen   pen(PS_SOLID, BORDER_MAX_SIZE, DARKGRAY);          //用于绘制圆角矩形边框的CPEN
	CBrush brush(GRAY);
	CPen*   pOldPen = dc.SelectObject(&pen);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	
	CRect roundRect1 = rect;
	roundRect1.DeflateRect(0,5,5,5);                         //以圆角矩形边框向中心缩小边框单位的矩形区域
	dc.RoundRect(roundRect1,ROUND_POINT);                    //画圆角矩形

	CRgn *pRgn = new CRgn;
	pRgn->CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, ROUND_POINT.x, ROUND_POINT.y);
	dc.FillRgn(pRgn,pColorBrush);                            //用dc以TIP_BK_COLOR来填充圆角矩形
	dc.SelectObject(pOldPen);                                //将pen和brush选入设备描述表
	dc.SelectObject(pOldBrush);

	CFont *pFont = new CFont();                             //创建字体
	CString textfont = theGUI_INFO.GetTitleFontName();
	pFont->CreateFont(TIP_TXT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
		CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, textfont);

	CFont* pOldFont = dc.SelectObject(pFont);

	dc.SetTextColor(RGB(255,0,0));
	dc.SetBkMode(TRANSPARENT);

	CString strTip_CN = _T("呼叫站员中，请稍候！");
	CString strTip_EN = _T("Calling for help,Please Wait!");

	rect.top += TXT_FIRST_LINE;
	dc.DrawText(strTip_CN, &rect, DT_WORDBREAK | DT_VCENTER | DT_CENTER);
	rect.top += TIP_TXT_SIZE;//行间距
	dc.DrawText(strTip_EN, &rect, DT_WORDBREAK | DT_VCENTER | DT_CENTER);
	rect.top += TIP_TXT_SIZE;//行间距
	
	//UIINFO buttonUI;
	//buttonUI.m_Location = CPoint(rect.left + 20,rect.top + 20);
	//buttonUI.m_pParentWnd = this;
	//buttonUI.m_Size = DEFAULT_SIZE_MENU_BUTTON;
	//BTN_INFO buttonInfo;
	//buttonInfo.btnMsg.Format(_T("%d"),IDCANCEL);
	//buttonInfo.text = _tl(STR_CANCEL);
	//buttonInfo.type = BUTTON_TYPE_NOSELECT;//对应修改图片底色为蓝色
	//buttonInfo.btnID = 1;
	//m_btnCancel = new CXButton(buttonUI,buttonInfo);
	//m_btnCancel->Create();

}

void CAssistDlg::OnPaint()
{
	DrawRoundRect();
}

LRESULT  CAssistDlg::OnCancel(WPARAM wParam,LPARAM lParam)
{
	//启动默认业务
	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->ResponseAssistBtn(SM_ASSITANCE_BUTTON_UP);
	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->ReSetHelpStatus();
	return NULL;
}