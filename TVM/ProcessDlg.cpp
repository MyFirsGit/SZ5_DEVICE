#include "stdafx.h"
#include "ProcessDlg.h"
const static int PAUSE_TIME = 10;           // �ȴ�������Ӧʱ����ͣʱ�䣨���룩
#define PROCESSING_WINDOW_POSPONE_DESTROY_TIMER 1000
#define PROCESSING_WINDOW_POSPONE_DESTROY_TIMER_INTERVAL 500

static const COLORREF TXT_FONT_COLOR = theGUI_INFO.GetServiceStateFontColor();	//��ʾ�������ɫ
static const DWORD TIP_FONT_SIZE = theGUI_INFO.GetServiceStateFontSize(); // �����С

static const COLORREF CL_BRUSH_TIP = RGB(217,217,217);	//��ʾ�򱳾�ɫ
static const COLORREF CL_BRUSH_BK = RGB(236,240,241);	//���汳��ɫ

static const CRect RECT_MSG(CPoint(0,130),CSize(1920,1080-110-130));	// �˿ͅ^��
#define TIP_FRAME_WIDTH		900				// ��ʾ��Ŀ��
#define TIP_FRAME_HEIGHT	500				// ��ʾ��ĸ߶�
#define FONT_ZISE			50				// ��ʾ�������С
//
//#define WM_CLOSE_PROCESSING_WINDOW

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
{

}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	//ON_MESSAGE(PROCESSING_WINDOW_POSPONE_DESTROY_TIMER,CloseWindow)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


//LRESULT CProcessDlg::CloseWindow(WPARAM wParam,LPARAM lParam){
//	this->DestroyWindow();
//}

void CProcessDlg::OnTimer(UINT_PTR nIDEvent){
	if(nIDEvent == PROCESSING_WINDOW_POSPONE_DESTROY_TIMER){

		this->KillTimer(nIDEvent);
		this->DestroyWindow();

	}
}

BOOL CProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//����WS_EX_LAYERED��չ����
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
	//	GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	//this->SetLayeredWindowAttributes(/*0,128,2*/CL_BRUSH_BK,255,LWA_ALPHA);

	// ��ʾͷ�����˵���			
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_NORMAL);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowWindow(SW_NORMAL);
	// ��ʾ��������ť����������ѡ��ť��ȡ����ť
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowServiceButtonGroup(false);
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowLanguageButtonGroup(false);
	// ����ִ��
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->UpdateWindow();
	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->UpdateWindow();

	::MoveWindow(this->GetSafeHwnd(),RECT_MSG.left,RECT_MSG.top,RECT_MSG.right,RECT_MSG.bottom,TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// ���ƽ���
void CProcessDlg::OnPaint(){
	CPaintDC dc(this);
	//// �ı�����ɫ
	CBrush* pNewBGBrush = new CBrush(CL_BRUSH_BK);
	CBrush* pOldBGBrush = dc.SelectObject(pNewBGBrush);
	CRect backRect(RECT_MSG.left,RECT_MSG.top,RECT_MSG.right,RECT_MSG.bottom);
	dc.FillRect(backRect,pNewBGBrush);

	dc.SelectObject(pOldBGBrush);
	pNewBGBrush->DeleteObject();
	if(NULL != pNewBGBrush){
		delete pNewBGBrush;
		pNewBGBrush = NULL;
	}
	CRgn* pNewRgn = new CRgn();
	CBrush* pNewBursh = new CBrush(CL_BRUSH_TIP);

	int msg_left  = (RECT_MSG.right - TIP_FRAME_WIDTH)/2;
	int msg_top   = (RECT_MSG.bottom - TIP_FRAME_HEIGHT)/2;
	CRect rectMsg(msg_left,msg_top,msg_left+TIP_FRAME_WIDTH,msg_top+TIP_FRAME_HEIGHT);
	pNewRgn->CreateRoundRectRgn(rectMsg.left,rectMsg.top,rectMsg.right,rectMsg.bottom,10,10);

	CBrush* pOldBrush = dc.SelectObject(pNewBursh);
	dc.FillRgn(pNewRgn,pNewBursh);

	dc.SelectObject(pOldBrush);

	pNewRgn->DeleteObject();
	pNewBursh->DeleteObject();
	if(NULL != pNewBursh){
		delete pNewBursh;
		pNewBursh = NULL;
	}
	if(NULL != pNewRgn){
		delete pNewRgn;
		pNewRgn = NULL;
	}

	// ��������
	CPen pNewPen(PS_SOLID,2,RED);
	CPen *pOldPen = dc.SelectObject(&pNewPen);
	if(NULL != pOldPen){
		int leftDestens  = (RECT_MSG.right - TIP_FRAME_WIDTH)/2;
		int topDestens   = (rectMsg.bottom + rectMsg.top)/2 - FONT_ZISE;//(FONT_ZISE * 3 /2);
		CRect rect(leftDestens,topDestens,leftDestens + TIP_FRAME_WIDTH,rectMsg.bottom);
		int nFontSize = FONT_ZISE;
		//int nFirstLine = (TIP_FRAME_HEIGHT - 3*nFontSize)/2;
		// ��������
		CFont* pNewFont = new CFont();
		pNewFont->CreateFont(nFontSize,0,0,0,FW_NORMAL,FALSE,FALSE,0,CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("����"));
		CFont* pOldFont = dc.SelectObject(pNewFont);
		// ������ɫ
		dc.SetTextColor(BLUE);
		dc.SetBkMode(TRANSPARENT);

		//rect.top += nFirstLine;
		dc.DrawText(m_show_msg.strCN,&rect,DT_VCENTER|DT_CENTER);
		rect.top += nFontSize;
		dc.DrawText(m_show_msg.strEN,&rect,DT_VCENTER|DT_CENTER);
		//rect.top += nFontSize;
		//dc.DrawText(m_show_msg.strPN,&rect,DT_VCENTER|DT_CENTER);

		dc.SelectObject(pOldFont);
		pNewFont->DeleteObject();
		if(NULL != pNewFont){
			delete pNewFont;
			pNewFont = NULL;
		}
		dc.SelectObject(pOldPen);
		pNewPen.DeleteObject();
	}	
}

CProcessDlg CProcessDlgHelper::dlg;
int  CProcessDlgHelper::refernceCount=0;

CProcessDlgHelper::CProcessDlgHelper(){

	if(false/*theBOM_INFO.GetScreenIsAvailableInWorking()*/){
		return;
	}

	if(refernceCount<0){
		refernceCount = 0;
	}
	if(refernceCount==0){
		//theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->Hide();
		dlg.Create(CProcessDlg::IDD);
		dlg.ShowWindow(SW_SHOW);
		dlg.UpdateWindow();
		refernceCount++;
	}
	if(refernceCount>=0){
		AfxGetApp()->BeginWaitCursor();
		dlg.BringWindowToTop();
	}	
}

CProcessDlgHelper::~CProcessDlgHelper(){
	if(false/*theBOM_INFO.GetScreenIsAvailableInWorking()*/){
		return;
	}
	//HideProcessDlg(false);
}

void CProcessDlgHelper::WaitForProcessWindowClose(){

	bool closed = !dlg.IsWindowVisible();
	while(!closed){
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if(msg.hwnd == dlg.m_hWnd && msg.message == WM_TIMER && msg.wParam == PROCESSING_WINDOW_POSPONE_DESTROY_TIMER)
			{
				closed = true;
			}
			::DispatchMessage(&msg);
			if(closed){
				return;
			}
		}
		Sleep(10);
	}
}

//bool CProcessDlgHelper::IsWindowVisible()
//{
//	return dlg.IsWindowVisible();
//}

void CProcessDlgHelper::HideProcessDlg(bool isDestroyNow){
	if(false/*theTVM_INFO.GetScreenIsAvailableInWorking()*/){
		return;
	}
	refernceCount--;
	if(refernceCount==0){
		AfxGetApp()->EndWaitCursor();
		if(isDestroyNow){
			dlg.ShowWindow(SW_HIDE);
			dlg.DestroyWindow();
		}else{
			//dlg.PostMessage(WM_CLOSE);
			//WaitForProcessWindowClose();
			UINT_PTR nIDEvent = 0;
			do{
				//dlg.ShowWindow(SW_HIDE);
				nIDEvent = dlg.SetTimer(PROCESSING_WINDOW_POSPONE_DESTROY_TIMER,PROCESSING_WINDOW_POSPONE_DESTROY_TIMER_INTERVAL,NULL);
			}while(nIDEvent==0);
		}
	}
}
