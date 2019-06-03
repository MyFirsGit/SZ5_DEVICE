#include "stdafx.h"
#include "BOM.h"
#include "GuideDlg.h"
#include "appsession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGuideDlg CGuideDlg::instance;

static const PADDINGS GUIDE_PADDINGS		  =  PADDINGS(5,5,5,5);
static const MARGINS GUIDE_MARGINS			= {5,5,5,5};
static CRect  GUIDE_MSG_RECT            =  CRect(0,0,0,0);//���캯���г�ʼ��


IMPLEMENT_DYNAMIC(CGuideDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CGuideDlg, CBaseDlg)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGuideDlg::CGuideDlg(CWnd* pParent)
	:CBaseDlg(CGuideDlg::IDD, pParent, GUIDE_FRAME_RECT, GRAY)
    ,m_guideMsg()
    ,m_msg()
    ,m_color(BLACK)
{
	GUIDE_MSG_RECT = CRect(GUIDE_MARGINS.cxLeftWidth,GUIDE_MARGINS.cyTopHeight,GUIDE_FRAME_RECT.Width() - GUIDE_MARGINS.cxLeftWidth ,GUIDE_FRAME_RECT.Height() - GUIDE_MARGINS.cyTopHeight );
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

BOOL CGuideDlg::Create(CWnd* pParentWnd )
{
	if(IsWindow(m_hWnd)){
		this->DestroyWindow();
	}
	__super::Create(CGuideDlg::IDD,pParentWnd);
	
	return this->ShowWindow(SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////
///**
//@brief      ������ƺ���
//
//@param      (i) CDC* pDC  �豸������
//
//@retval     ��
//
//@exception  ��
//*/
////////////////////////////////////////////////////////////////////////////
void CGuideDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	GUIDE_MSG_RECT = CRect(GUIDE_MARGINS.cxLeftWidth,GUIDE_MARGINS.cyTopHeight,GUIDE_FRAME_RECT.Width() - GUIDE_MARGINS.cxLeftWidth- GUIDE_MARGINS.cxRightWidth ,GUIDE_FRAME_RECT.Height() - GUIDE_MARGINS.cyTopHeight -GUIDE_MARGINS.cyBottomHeight  );
    DrawRoundRect(pDC, GUIDE_MSG_RECT);
    DrawText(pDC, GUIDE_MSG_RECT.left + GUIDE_PADDINGS.cxLeftWidth, GUIDE_MSG_RECT.top + GUIDE_PADDINGS.cyTopHeight+12, m_msg, TITLE_FONT, 16,FW_HEAVY, m_color);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʾ����Ϣ

@param      (i)guideMsg	        ����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGuideDlg::ShowGuideMsg(CString guideMsg)
{
    m_guideMsg = guideMsg;

    // ��������Ϣ
    theAPP_SESSION.ParseGuideMsg(guideMsg, m_msg, m_color);
	if(IsWindow(m_hWnd)){
    // ˢ����Ļ
		InvalidateRect(&GUIDE_MSG_RECT);
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

@retval     CString	        ����Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CGuideDlg::GetGuideMsg()
{
    return m_guideMsg;
}
