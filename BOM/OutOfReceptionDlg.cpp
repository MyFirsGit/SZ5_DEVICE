#include "stdafx.h"
#include "BOM.h"
#include "OutOfReceptionDlg.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static TCHAR* const TXT_OUT_OF_SERVICE_CN  = _T("��ͣ����");
const static TCHAR* const TXT_OUT_OF_SERVICE_EN  = _T("Out Of Service");

static const TCHAR* const TXT_WELCOME_CN         = _T("��ӭ������������");
static const TCHAR* const TXT_WELCOME_EN         = _T("Welcome to Shenyang Metro Network");


IMPLEMENT_DYNAMIC(COutOfReceptionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(COutOfReceptionDlg, CBaseDlg)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COutOfReceptionDlg::COutOfReceptionDlg(CService* pService)
	:CBaseDlg(COutOfReceptionDlg::IDD, NULL, WELCOME_FRAME_RECT, RECEPTION_HEAD_BK_COLOR, pService, false)
    ,m_contentType(OUT_OF_SERVICE)
{
	m_hLogoBmp = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, _T(".\\res\\logo.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
COutOfReceptionDlg::~COutOfReceptionDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COutOfReceptionDlg::OnDraw(CDC* pDC)
{
	__super::OnDraw(pDC);

	// ��ʾLOGO
	if (m_hLogoBmp != NULL){
		CRect rect;
		this->GetWindowRect(&rect);
		ScreenToClient(&rect);

		CDC srcDC;
		srcDC.CreateCompatibleDC(pDC);
		CBitmap* pLogoBmp = CBitmap::FromHandle(m_hLogoBmp);
		CBitmap* pOldBmp = srcDC.SelectObject(pLogoBmp);


		if (m_contentType == OUT_OF_SERVICE){
			
		}
		else{
			pDC->StretchBlt(WELCOME_LOGO_RECT.left,WELCOME_LOGO_RECT.top,WELCOME_LOGO_RECT.Width(),WELCOME_LOGO_RECT.Height(),&srcDC,0,0,WELCOME_LOGO_RECT.Width()*3/4,WELCOME_LOGO_RECT.Height()*3/4,SRCCOPY);
			pDC->StretchBlt(WELCOME_LOGO_RECT2.left,WELCOME_LOGO_RECT2.top,WELCOME_LOGO_RECT2.Width(),WELCOME_LOGO_RECT2.Height(),&srcDC,0,0,WELCOME_LOGO_RECT2.Width()*3/4,WELCOME_LOGO_RECT2.Height()*3/4,SRCCOPY);
		}
		
		srcDC.SelectObject(pOldBmp);
	}
	

	
    // ��ʾ����
	// ��ͣ�������
    if (m_contentType == OUT_OF_SERVICE) {
		DrawText(pDC, CRect(10,200,1024,400), TXT_OUT_OF_SERVICE_CN, REC_CN_FONT_NAME, 110,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);
		DrawText(pDC, CRect(10,399,1024,600), TXT_OUT_OF_SERVICE_EN, REC_CN_FONT_NAME, 60,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);
		
		DrawText(pDC, CRect(10+CXSCREENREC,200,1024+CXSCREENREC,400), TXT_OUT_OF_SERVICE_CN, REC_CN_FONT_NAME, 110,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);
		DrawText(pDC, CRect(10+CXSCREENREC,399,1024+CXSCREENREC,600), TXT_OUT_OF_SERVICE_EN, REC_CN_FONT_NAME, 60,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);

    }
	// ��ӭ���ٽ���
    else {
		DrawText(pDC, CRect(120,200,1024,300), TXT_WELCOME_CN, REC_CN_FONT_NAME, 80,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);
		DrawText(pDC, CRect(60,399,964,668), TXT_WELCOME_EN, REC_CN_FONT_NAME, 40,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);		
		DrawText(pDC, CRect(120+CXSCREENREC,200,1024+CXSCREENREC,300), TXT_WELCOME_CN, REC_CN_FONT_NAME, 80,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);
		DrawText(pDC, CRect(60+CXSCREENREC,399,964+CXSCREENREC,668), TXT_WELCOME_EN, REC_CN_FONT_NAME, 40,FW_NORMAL, WHITE, DT_CENTER|DT_TOP);		

    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���漤���Ĵ���

@param      (i)UINT nState      ��ʹ��
@param      (i)CWnd* pWndOther  ��ʹ��
@param      (i)BOOL bMinimized  ��ʹ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COutOfReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    //// ���س˿ͻ����������
    theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
    theAPP_SESSION.GetChildViewRec()->m_pReceptionGuideDlg->ShowWindow(SW_HIDE);
	__super::OnActivate(nState,pWndOther,bMinimized);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ����

@param      (i)CONTENT_TYPE contentType ָ������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void COutOfReceptionDlg::Show(CONTENT_TYPE contentType)
{
    // ���س˿ͻ����������
	theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_HIDE);
	theAPP_SESSION.GetChildViewRec()->m_pReceptionGuideDlg->ShowWindow(SW_HIDE);
	m_contentType = contentType;
	this->ShowWindow(SW_SHOW);
	this->Invalidate();
	this->UpdateWindow();
}
