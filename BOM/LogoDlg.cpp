// CLogoDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "BOM.h"
#include "LogoDLG.h"
#include "servicemgr.h"
#include "servicedefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CLogoDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CLogoDlg, CBaseDlg)
END_MESSAGE_MAP()

static CRect  LOGO_TEXT_RECT = CRect(117,20*CYSCREENRATIO,267*CXSCREENRATIO,70*CYSCREENRATIO);
const static char* const LOGO_TEXT = "��������";
static CRect  LOGO_ENGLISH_TEXT_RECT = CRect(117,70*CYSCREENRATIO,267*CXSCREENRATIO,90*CYSCREENRATIO);
const static char* const LOGO_ENGLISH_TEXT = "Shenyang Metro";
static CRect VERSION_RECT = CRect(25*CXSCREENRATIO,0,267*CXSCREENRATIO,20*CYSCREENRATIO);
static const UINT const VERSION_FONT_SIZE = 9;

static const UINT const REC_EN_FONT_SIZE3 = 16;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogoDlg::CLogoDlg(CWnd* pParent)
: CBaseDlg(CLogoDlg::IDD, pParent, LOGO_FRAME_RECT,RECEPTION_HEAD_BK_COLOR), m_hLogoBmp(NULL)
{
    m_hLogoBmp = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, _T(".\\res\\logo.bmp"),IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	theVERSION_INI.VersionNoChanged.AddHandler(this,&CLogoDlg::OnVersionNoChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLogoDlg::~CLogoDlg()
{
	theVERSION_INI.VersionNoChanged.RemoveHandler(this,&CLogoDlg::OnVersionNoChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLogoDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	if (m_hLogoBmp != NULL) {
		// ��ȡLogo���ڴ�С
		CRect rect;
		this->GetWindowRect(&rect);
		ScreenToClient(&rect);          // ת��������ڵ�ǰOperationDlg��λ��

		// ��ͼ
		CDC srcDC;
		srcDC.CreateCompatibleDC(pDC);

		CBitmap* pLogoBmp = CBitmap::FromHandle(m_hLogoBmp);
		CBitmap* pOldBmp = srcDC.SelectObject(pLogoBmp);
		
		// LOGO
		pDC->BitBlt(LOGO_RECT.left, LOGO_RECT.top, LOGO_RECT.Width(), LOGO_RECT.Height(), &srcDC, 0, 0, SRCCOPY);	
		srcDC.SelectObject(pOldBmp);
	}
	CString strVersion = _T("");
	strVersion.Format(_T("%s BOM:V%d OT:V%d  IT:V%d"),theMAINTENANCE_INFO.GetCurrentDevice().ToString(), theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP),theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_APP));
	//DrawText(pDC,VERSION_RECT,strVersion,LOGO_ENGLISH_TEXT,VERSION_FONT_SIZE,FW_NORMAL,BLACKGRAY,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	// ����
	//DrawText(pDC,LOGO_TEXT_RECT,LOGO_TEXT,REC_CN_FONT_NAME,REC_CN_FONT_SIZE+2,FW_NORMAL,WHITE,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	// Ӣ��
	//DrawText(pDC,LOGO_ENGLISH_TEXT_RECT,LOGO_ENGLISH_TEXT,REC_CN_FONT_NAME,REC_EN_FONT_SIZE3,FW_NORMAL,WHITE,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �汾�ŷ����ı�ʱ��Ӧ����

@param      

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CLogoDlg::OnVersionNoChanged(CVersionInfo::VER_TYPE verType, CVersionInfo::PROG_TYPE progType, DWORD versionNo)
{
	this->Invalidate();
}