#include "stdafx.h"
#include "TVM.h"
#include "GuideDlg.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// Guide��ʽ����

static CRect GUIDE_RECT				= CRect(0,0,0,0);		// ������ʾ���򣬹��캯���г�ʼ��
static const int GUIDE_BORDER		= 1;					// �߿���
static const CPoint GUIDE_ROUND		= CPoint(12,12);		// Բ�ǻ���
static const MARGINS GUIDE_MARGIN	= {2,0,2,0};			// ��߾�
static const PADDINGS GUIDE_PADDING	= PADDINGS(8,2,2,2);	// �ڱ߾�

IMPLEMENT_DYNAMIC(CGuideDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(CGuideDlg, CBaseDlg)

END_MESSAGE_MAP()

CGuideDlg CGuideDlg::instance;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd* pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGuideDlg::CGuideDlg(CWnd* pParent)
	:CBaseDlg(CGuideDlg::IDD, pParent, OPERATOR_GUIDE_RECT, GRAY)
    ,m_msg()
    ,m_color(OPERATOR_GUIDE_FONT_COLOR/*BLACK*/) //Ĭ����ɫ  �������ļ��ж�ȡ
{
	GUIDE_RECT = CRect(GUIDE_MARGIN.cxLeftWidth, GUIDE_MARGIN.cyTopHeight,
		OPERATOR_GUIDE_RECT.Width() - GUIDE_MARGIN.cxLeftWidth- GUIDE_MARGIN.cxRightWidth,
		OPERATOR_GUIDE_RECT.Height() - GUIDE_MARGIN.cyTopHeight -GUIDE_MARGIN.cyBottomHeight);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGuideDlg::~CGuideDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴��

@param      (i)CWnd* pParentWnd ������

@retval     BOOL  TURE:�ɹ�  FALSE:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CGuideDlg::Create(CWnd* pParentWnd)
{
	if(IsWindow(m_hWnd))
	{
		this->DestroyWindow();
	}
	__super::Create(CGuideDlg::IDD, pParentWnd);
	
	return this->ShowWindow(SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC �豸������

@retval     ��

@exception  ��
*/
////////////////////////////////////////////////////////////////////////////
void CGuideDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	GUIDE_RECT = CRect(GUIDE_MARGIN.cxLeftWidth, GUIDE_MARGIN.cyTopHeight,
		OPERATOR_GUIDE_RECT.Width() - GUIDE_MARGIN.cxLeftWidth- GUIDE_MARGIN.cxRightWidth,
		OPERATOR_GUIDE_RECT.Height() - GUIDE_MARGIN.cyTopHeight -GUIDE_MARGIN.cyBottomHeight);
    DrawRoundRect(pDC, GUIDE_RECT, GUIDE_BORDER, DARKGRAY, OPERATOR_GUIDE_BK_COLOR/*GRAY*/, GUIDE_ROUND);
	GUIDE_RECT.DeflateRect(GUIDE_PADDING.cxLeftWidth, GUIDE_PADDING.cyTopHeight, GUIDE_PADDING.cxRightWidth, GUIDE_PADDING.cyBottomHeight);
	//DrawText(pDC, GUIDE_RECT, m_msg, DEFAULT_FONT, m_color, DT_VCENTER|DT_SINGLELINE); Modified by lws :  Font&Size will be readed frome INI
	//����Guide���ԣ���Ҫʹ�û���ģ�����ʵ��������
	CFont font;
    font.CreateFont(GUIDE_FONT_SIZE, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
        OPERATORCHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
        DEFAULT_PITCH|FF_SWISS, GUIDE_FONT_NAME);

    CFont* pOldFont = pDC->SelectObject(&font);
    pDC->SetTextColor(m_color);
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(m_msg, GUIDE_RECT, DT_VCENTER|DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ����Ϣ

@param      (i)guideMsg	        ����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGuideDlg::ShowGuideMsg(CString officeMsg, COLORREF color /* = BLACK */)
{
	m_msg = officeMsg;
	m_color = color;

	// ˢ����Ļ
	if(IsWindow(m_hWnd))
	{
		InvalidateRect(&GUIDE_RECT);
		if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		{
			UpdateWindow();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����Ϣ

@param      ��

@retval     CString ����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CGuideDlg::GetGuideMsg()
{
    return m_msg;
}

void CGuideDlg::ShowGuideMsg(DWORD officeMsg)
{
}