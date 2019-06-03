// ReceptionStaticInfo.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "BOM.h"
#include "ReceptionStaticInfo.h"
#include "UIDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CReceptionStaticInfo

IMPLEMENT_DYNAMIC(CReceptionStaticInfo, CStatic)

BEGIN_MESSAGE_MAP(CReceptionStaticInfo, CStatic)
    ON_WM_CREATE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
  @brief      ���캯��

  @param      (i)CWnd*          pParentWnd      ������ָ��
  @param      (i)PMSG_INFO     kindInfo        ��������          

  @retval     ��     

  @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionStaticInfo::CReceptionStaticInfo(CWnd* pParentWnd, PMSG_INFO msgInfo)
{
    CStatic::CStatic();
    m_msgInfo = msgInfo;
	CRect rect = REC_BODY_RECT;
	rect.MoveToXY(0,0);
    if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, rect, pParentWnd, 0)) {
        throw _T("�����ı���ʧ��");
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��       

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionStaticInfo::~CReceptionStaticInfo()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��Ϊ��ǰ��ʱ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionStaticInfo::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW);
    CStatic::PreSubclassWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����仯ʱˢ�»���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionStaticInfo::OnPaint()
{
    CPaintDC dc(this); 
    CRect rect;
    GetClientRect(&rect);

	CPen pen(PS_SOLID, 1, DARKGRAY);
	CBrush brush(GRAY);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);
	CRect roundRect1 = rect;
	roundRect1.right = roundRect1.right /2;
	roundRect1.DeflateRect(5,5,5,5);
	dc.RoundRect(roundRect1,ROUND_POINT);
	
	CRect roundRect2 = rect;
	roundRect2.left = rect.right /2;
	roundRect2.DeflateRect(5,5,5,5);
	dc.RoundRect(roundRect2,ROUND_POINT);


    int left = MSG_FIRST_DETAIL_X;
    int right = left + MSG_ROUND_WIDTH;
    int top = MSG_FIRST_DETAIL_Y;
    int bottom = top + MSG_ROUND_HEIGHT;

    for (int i = 0; i <m_msgInfo->detailList.size(); i++) {
        top += MSG_DISTANCE;
        bottom += MSG_DISTANCE;
    }

    // ��ʾ�������
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(m_msgInfo->titleName.color);
    CFont titleCNFont;
    int height = -MulDiv(REC_CN_FONT_SIZE, GetDeviceCaps(dc, LOGPIXELSY), 72);
    titleCNFont.CreateFont(height, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, 
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH|FF_DONTCARE, REC_CN_FONT_NAME);
    CFont *poldFont = dc.SelectObject(&titleCNFont);
    dc.DrawText(m_msgInfo->titleName.nameChinese,CRect(MSG_TITLE_X, MSG_TITLE_Y,MSG_TITLE_X+100,MSG_TITLE_Y + 50),DT_CENTER);
	dc.DrawText(m_msgInfo->titleName.nameChinese,CRect(MSG_TITLE_X+CXSCREENREC, MSG_TITLE_Y,MSG_TITLE_X+100+CXSCREENREC,MSG_TITLE_Y + 50),DT_CENTER);
	
    CFont titleENFont;
    height = -MulDiv(REC_EN_FONT_SIZE, GetDeviceCaps(dc, LOGPIXELSY), 72);
    titleENFont.CreateFont(height, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, 
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH|FF_DONTCARE, REC_CN_FONT_NAME);
    dc.SelectObject(titleENFont);
    dc.DrawText(m_msgInfo->titleName.nameEnglish,CRect(MSG_TITLE_X, MSG_TITLE_Y + 50,MSG_TITLE_X+100,MSG_TITLE_Y + 100), DT_CENTER);
	dc.DrawText(m_msgInfo->titleName.nameEnglish,CRect(MSG_TITLE_X+CXSCREENREC, MSG_TITLE_Y + 50,MSG_TITLE_X+100+CXSCREENREC,MSG_TITLE_Y + 100), DT_CENTER);


    // ��ʾ��ϸ����
    CFont detailCNFont;
    height = -MulDiv(REC_CN_FONT_SIZE3, GetDeviceCaps(dc, LOGPIXELSY), 72);
    detailCNFont.CreateFont(height, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, 
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
        DEFAULT_PITCH|FF_DONTCARE, REC_CN_FONT_NAME);

	CFont detailENFont;
	height = -MulDiv(REC_EN_FONT_SIZE2, GetDeviceCaps(dc, LOGPIXELSY), 72);
	detailENFont.CreateFont(height, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, 
		GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_DONTCARE, REC_CN_FONT_NAME);
    top = 16;

    for (int i = 0; i < m_msgInfo->detailList.size(); i++) {
        dc.SelectObject(detailCNFont);
        dc.SetTextColor(m_msgInfo->detailList[i].nameLeft.color);
        dc.DrawText(m_msgInfo->detailList[i].nameLeft.nameChinese, CRect(MSG_DETAIL_LEFT_X_LEFT, top, MSG_DETAIL_LEFT_X_RIGHT, top + MSG_CHINESE_HEIGHT), DT_RIGHT|DT_TOP|DT_SINGLELINE);
		dc.DrawText(m_msgInfo->detailList[i].nameLeft.nameChinese, CRect(MSG_DETAIL_LEFT_X_LEFT+CXSCREENREC, top, MSG_DETAIL_LEFT_X_RIGHT+CXSCREENREC, top + MSG_CHINESE_HEIGHT), DT_RIGHT|DT_TOP|DT_SINGLELINE);
        dc.SelectObject(detailENFont);
        dc.DrawText(m_msgInfo->detailList[i].nameLeft.nameEnglish, CRect(MSG_DETAIL_LEFT_X_LEFT, top + MSG_CHINESE_HEIGHT, MSG_DETAIL_LEFT_X_RIGHT, top + MSG_CHINESE_HEIGHT + 20), DT_RIGHT|DT_TOP|DT_SINGLELINE);
		dc.DrawText(m_msgInfo->detailList[i].nameLeft.nameEnglish, CRect(MSG_DETAIL_LEFT_X_LEFT+CXSCREENREC, top + MSG_CHINESE_HEIGHT, MSG_DETAIL_LEFT_X_RIGHT+CXSCREENREC, top + MSG_CHINESE_HEIGHT + 20), DT_RIGHT|DT_TOP|DT_SINGLELINE);
        dc.SelectObject(detailCNFont);
        dc.SetTextColor(m_msgInfo->detailList[i].nameRight.color);
        dc.DrawText(m_msgInfo->detailList[i].nameRight.nameChinese, CRect(MSG_DETAIL_RIGHT_X_LEFT, top, MSG_DETAIL_RIGHT_X_RIGHT, top + MSG_CHINESE_HEIGHT), DT_LEFT|DT_TOP|DT_SINGLELINE);
		dc.DrawText(m_msgInfo->detailList[i].nameRight.nameChinese, CRect(MSG_DETAIL_RIGHT_X_LEFT+CXSCREENREC, top, MSG_DETAIL_RIGHT_X_RIGHT+CXSCREENREC, top + MSG_CHINESE_HEIGHT), DT_LEFT|DT_TOP|DT_SINGLELINE);
        dc.SelectObject(detailENFont);
        dc.DrawText(m_msgInfo->detailList[i].nameRight.nameEnglish, CRect(MSG_DETAIL_RIGHT_X_LEFT, top + MSG_CHINESE_HEIGHT, MSG_DETAIL_RIGHT_X_RIGHT, top + MSG_CHINESE_HEIGHT + 20), DT_LEFT|DT_TOP|DT_SINGLELINE);
		dc.DrawText(m_msgInfo->detailList[i].nameRight.nameEnglish, CRect(MSG_DETAIL_RIGHT_X_LEFT+CXSCREENREC, top + MSG_CHINESE_HEIGHT, MSG_DETAIL_RIGHT_X_RIGHT+CXSCREENREC, top + MSG_CHINESE_HEIGHT + 20), DT_LEFT|DT_TOP|DT_SINGLELINE);
        top += MSG_DISTANCE;
    }

    dc.SelectObject(poldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ı������ʾ����

@param      (i)int           row      �к�, ��1��ʼ
@param      (i)int           col      �к�, ��1��ʼ
@param      (i)MSG_NAME      txt      �ı�������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionStaticInfo::SetText(UINT row, UINT col, MSG_NAME txt)
{
    if ( row < m_msgInfo->detailList.size() && col <=2) {

		MSG_NAME  msgName = col == 1 ? m_msgInfo->detailList[row].nameLeft : m_msgInfo->detailList[row].nameRight;
		msgName.color = txt.color;
		msgName.nameChinese = txt.nameChinese;
		msgName.nameEnglish = txt.nameEnglish;

        InvalidateRect(NULL);
    }
}
