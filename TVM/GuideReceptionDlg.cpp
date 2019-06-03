#include "stdafx.h"
#include "TVM.h"
#include "GuideReceptionDlg.h"
#include "UIDefinition.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Guide�Ի���λ�ô�С
#define GUD_L	REC_BODY_L
#define GUD_R	REC_BODY_R
#define GUD_U	70
#define GUD_D	REC_BODY_U + 4

// ����˿ͻ���ֱ��ʱ�ֵ
static const CRect REC_GUIDE_RECT_RATIO = CRect(REC_GUIDE_RECT.left*REC_RATIOWIDTH, REC_GUIDE_RECT.top*REC_RATIOHEIGHT,
												REC_GUIDE_RECT.right*REC_RATIOWIDTH, REC_GUIDE_RECT.bottom*REC_RATIOHEIGHT);

static CRect VERSION_RECT = CRect(730*REC_RATIOWIDTH,748*REC_RATIOHEIGHT,1024*REC_RATIOWIDTH,768*REC_RATIOHEIGHT);

IMPLEMENT_DYNAMIC(CGuideReceptionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CGuideReceptionDlg, CBaseDlg)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGuideReceptionDlg::CGuideReceptionDlg(CWnd* pParent)
    :CBaseDlg(CGuideReceptionDlg::IDD, pParent, REC_GUIDE_RECT_RATIO, RECEPTION_GUIDE_BK_COLOR, false)
	,m_guideMsgID(0)
	,m_guideChineseMsg(_T(""))
	,m_guideEnglishMsg(_T(""))
	//,m_guidePortugueseMsg(_T(""))
    ,m_color(WHITE)
{ 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGuideReceptionDlg::~CGuideReceptionDlg()
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
void CGuideReceptionDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	DrawText(pDC, 26*REC_RATIOWIDTH, 9*REC_RATIOHEIGHT, m_guideChineseMsg, REC_GUIDEINFO_FONT_NAME_CN, REC_GUIDEINFO_FONT_SIZE_CN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	DrawText(pDC, 26*REC_RATIOWIDTH, 35*REC_RATIOHEIGHT, m_guideEnglishMsg, REC_GUIDEINFO_FONT_NAME_EN, REC_GUIDEINFO_FONT_SIZE_EN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	//DrawText(pDC, 26*REC_RATIOWIDTH, 50*REC_RATIOHEIGHT, m_guidePortugueseMsg, REC_GUIDEINFO_FONT_NAME_PN, REC_GUIDEINFO_FONT_SIZE_PN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	//DrawText(pDC, (26+CXSCREEN)*REC_RATIOWIDTH, 9*REC_RATIOHEIGHT, m_guideChineseMsg, REC_GUIDEINFO_FONT_NAME_CN, REC_GUIDEINFO_FONT_SIZE_CN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	//DrawText(pDC, (26+CXSCREEN)*REC_RATIOWIDTH, 35*REC_RATIOHEIGHT, m_guideEnglishMsg, REC_GUIDEINFO_FONT_NAME_EN, REC_GUIDEINFO_FONT_SIZE_EN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	//DrawText(pDC, (26+CXSCREEN)*REC_RATIOWIDTH, 50*REC_RATIOHEIGHT, m_guidePortugueseMsg, REC_GUIDEINFO_FONT_NAME_PN, REC_GUIDEINFO_FONT_SIZE_PN,FW_NORMAL, m_color== BLACK? WHITE:m_color);

	CString strVersion = _T("");
	//strVersion.Format(_T("TVM:V%d OT:V%d IT:V%d TH:V%d"), theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),
	//	theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::RW_APP),
	//	theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TW_APP),
	//	theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::TH_APP));
	strVersion.Format(_T("TVM:V%d OT:V%d"), theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::MC),
		theVERSION_INI.GetVersionNo(CVersionInfo::CURRENT,CVersionInfo::CHARGE_RW_APP));
	DrawText(pDC,VERSION_RECT,strVersion,LOGO_FONT_NAME_EN,VERSION_FONT_SIZE,FW_NORMAL,BLACKGRAY,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ʾ����Ϣ

@param      (i)guideMsg	            ��������Ϣ
@param      (i)guideEnglishMsg	    Ӣ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGuideReceptionDlg::ShowGuideMsg(DWORD guideMsgID)
{
	m_guideChineseMsg = _cn(guideMsgID);
	m_guideEnglishMsg = _en(guideMsgID);
	//m_guidePortugueseMsg = _po(guideMsgID);
    // ��������Ϣ�����ģ�
	theAPP_SESSION.ParseGuideMsg(m_guideChineseMsg, m_guideChineseMsg, m_color );
	theAPP_SESSION.ParseGuideMsg(m_guideEnglishMsg, m_guideEnglishMsg, m_color );
//	theAPP_SESSION.ParseGuideMsg(m_guidePortugueseMsg, m_guidePortugueseMsg, m_color );

	this->Invalidate();
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		UpdateWindow();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ʾ����Ϣ

@param      (i)guideMsg	            ��������Ϣ
@param      (i)guideEnglishMsg	    Ӣ������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGuideReceptionDlg::ShowGuideMsg(CString guideChineseMsg,CString guideEnglishMsg,CString guidePortugueseMsg)
{
	m_guideChineseMsg = guideChineseMsg;
	m_guideEnglishMsg = guideEnglishMsg;
	//m_guidePortugueseMsg = guidePortugueseMsg;
	// ��������Ϣ�����ģ�
	theAPP_SESSION.ParseGuideMsg(m_guideChineseMsg, m_guideChineseMsg, m_color );
	theAPP_SESSION.ParseGuideMsg(m_guideEnglishMsg, m_guideEnglishMsg, m_color );
	//theAPP_SESSION.ParseGuideMsg(m_guidePortugueseMsg, m_guidePortugueseMsg, m_color );

	this->Invalidate();
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		UpdateWindow();
	}
}