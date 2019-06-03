#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CRect			 LOGO_TEXT_RECT		= CRect(180,35,500,95);
const static TCHAR* const LOGO_TEXT			= _T("����IT,���������......");
static const TCHAR* const REC_CN_FONT_NAME	= _T("Arial");
static const UINT REC_CN_FONT_SIZE	= 18;

CCTLauncherDlg::CCTLauncherDlg(CWnd* pParent /*=NULL*/)	: CDialog(CCTLauncherDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CCTLauncherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


BOOL CCTLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);	
		
	return TRUE;  
}

void CCTLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

void CCTLauncherDlg::OnPaint() 
{
	CPaintDC dc(this); 
	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
	// ����LOGO
	int cxIcon = GetSystemMetrics(SM_CXICON);
	int cyIcon = GetSystemMetrics(SM_CYICON);
	CRect rect;
	GetClientRect(&rect);
	int x = cxIcon*2+80;
	int y = (rect.Height() - cyIcon + 1) / 2;
	dc.DrawIcon(x, y, m_hIcon);
	CDialog::OnPaint();
	
	// ��ʾ��˾��Ϣ
	DrawText(&dc,LOGO_TEXT_RECT,LOGO_TEXT,REC_CN_FONT_NAME,REC_CN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

HCURSOR CCTLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ָ��λ����ʾ����

@param     (i)CDC* pDC              �豸������
@param     (i)CRect& rect           ��ʾ����
@param     (i)CString sText	        ����
@param     (i)CString sFont  	    ����
@param     (i)int nFontSize  	    ��С
@param     (i)COLORREF crColor 	    ��ɫ
@param     (i)UINT nFormat 	        ��ʽ��DT_CENTER|DT_VCENTER|DT_SINGLELINE��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCTLauncherDlg::DrawText(CDC* pDC, CRect& rect, CString sText, CString sFont, int nFontSize,int nFontWeight, COLORREF crColor, UINT nFormat)
{
	int height = -MulDiv(nFontSize, GetDeviceCaps(*pDC, LOGPIXELSY), 72);

	CFont font;
	font.CreateFont(height, 0, 0, 0, nFontWeight, FALSE, FALSE, 0, 
		GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH|FF_DONTCARE, sFont);

	CFont* pOldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(crColor);
	pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(sText, rect, nFormat);

	pDC->SelectObject(pOldFont);
}