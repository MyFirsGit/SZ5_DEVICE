#include "stdafx.h"
#include "BOM.h"
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
    :CBaseDlg(CGuideReceptionDlg::IDD, pParent, REC_GUIDE_RECT, RECEPTION_HEAD_BK_COLOR, false)
    ,m_guideMsg()
    ,m_guideEnglishMsg()
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

    DrawText(pDC, 26, 6, m_guideMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_CN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
    DrawText(pDC, 26, 45, m_guideEnglishMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_EN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	DrawText(pDC, 26+CXSCREENREC, 6, m_guideMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_CN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
	DrawText(pDC, 26+CXSCREENREC, 45, m_guideEnglishMsg, REC_CN_FONT_NAME, GUIDE_FONT_SIZE_EN,FW_NORMAL, m_color== BLACK? WHITE:m_color);
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
void CGuideReceptionDlg::ShowGuideMsg(CString guideMsg, CString guideEnglishMsg)
{
    // ��������Ϣ�����ģ�
	theAPP_SESSION.ParseGuideMsg(guideMsg, m_guideMsg, m_color );
    m_guideEnglishMsg = guideEnglishMsg;

	this->Invalidate();
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		UpdateWindow();
	}
}