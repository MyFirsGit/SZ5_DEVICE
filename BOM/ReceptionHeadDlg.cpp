// CReceptionHeadDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "BOM.h"
#include "ReceptionHeadDLG.h"
#include <sstream>
#include <iomanip>
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CReceptionHeadDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CReceptionHeadDlg, CBaseDlg)
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


static CRect  COMPANY_NAME_RECT=CRect(110,15,400,75);
static CRect  COMPANY_NAME_RECT2 = CRect(110+CXSCREENREC,15,400+CXSCREENREC,75);
static CRect  STATION_NAME_CN_RECT = CRect(450,10,700,70);
static CRect  STATION_NAME_CN_RECT2 =  CRect(450+CXSCREENREC,10,700+CXSCREENREC,70);
static CRect  STATION_NAME_EN_RECT = CRect(450,65,700,85);
static CRect  STATION_NAME_EN_RECT2 = CRect(450+CXSCREENREC,65,700+CXSCREENREC,85);
static CRect  DATETIME_RECT = CRect(850,5,1024,20);
static CRect  DATETIME_RECT2 = CRect(850+CXSCREENREC,5,1024+CXSCREENREC,20);
static CRect  WEEKDAY_CN_RECT = CRect(850,20,1024,75);
static CRect  WEEKDAY_CN_RECT2 = CRect(850+CXSCREENREC,20,1024+CXSCREENREC,75);
static CRect  WEEKDAY_EN_RECT = CRect(850,75,1024,95);
static CRect  WEEKDAY_EN_RECT2 = CRect(850+CXSCREENREC,75,1024+CXSCREENREC,95);

static const TCHAR*  const COMPANY_NAME_CN = _T("��������");
static const TCHAR*  const COMPANY_NAME_EN = _T("Shenyang Metro");

static CRect  LOGO_ENGLISH_TEXT_RECT = CRect(95,105,257,40);
static CRect  LOGO_ENGLISH_TEXT_RECT2 = CRect(95+CXSCREENREC,105,257+CXSCREENREC,40);

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*     pParent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CReceptionHeadDlg::CReceptionHeadDlg(CWnd* pParent)
    :CBaseDlg(CReceptionHeadDlg::IDD, pParent, REC_HEAD_RECT, RECEPTION_HEAD_BK_COLOR)
    ,m_stationNameCH("������")
    ,m_stationNameEN("---")
	,m_hLogoBmp(NULL)
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
CReceptionHeadDlg::~CReceptionHeadDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
              0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CReceptionHeadDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (__super::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

	// ����ʱ����ʾ��ʱ��
    this->SetTimer(TIMER_STS, TIMER_INTERVAL, NULL);

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ƺ���

@param      (i) CDC* pDC  �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);

    // ��ʾLogo
    if (m_hLogoBmp != NULL) {
		CRect rect;
		this->GetWindowRect(&rect);
		ScreenToClient(&rect);

        CDC srcDC;
        srcDC.CreateCompatibleDC(pDC);

        CBitmap* pLogoBmp = CBitmap::FromHandle(m_hLogoBmp);
        CBitmap* pOldBmp = srcDC.SelectObject(pLogoBmp);

		pDC->BitBlt(10, 20, RECEPTION_HEAD_LOGO_RECT.Width(), RECEPTION_HEAD_LOGO_RECT.Height(), &srcDC, 0, 0, SRCCOPY);
		pDC->BitBlt(10+CXSCREENREC, 20, RECEPTION_HEAD_LOGO_RECT.Width(), RECEPTION_HEAD_LOGO_RECT.Height(), &srcDC, 0, 0, SRCCOPY);
        srcDC.SelectObject(pOldBmp);
    }
	DrawText(pDC,LOGO_ENGLISH_TEXT_RECT,COMPANY_NAME_EN,REC_CN_FONT_NAME,REC_EN_FONT_SIZE,FW_NORMAL,WHITE,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DrawText(pDC,LOGO_ENGLISH_TEXT_RECT2,COMPANY_NAME_EN,REC_CN_FONT_NAME,REC_EN_FONT_SIZE,FW_NORMAL,WHITE,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	

	// ��ʾ��Ӫ������
	DrawText(pDC,COMPANY_NAME_RECT,COMPANY_NAME_CN,REC_CN_FONT_NAME,REC_CN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT);
	DrawText(pDC,COMPANY_NAME_RECT2,COMPANY_NAME_CN,REC_CN_FONT_NAME,REC_CN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT);

	// ��ʾ��ǰ��վ
	DrawText(pDC,STATION_NAME_CN_RECT,m_stationNameCH,REC_CN_FONT_NAME,REC_CN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT);
	DrawText(pDC,STATION_NAME_CN_RECT2,m_stationNameCH,REC_CN_FONT_NAME,REC_CN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT);
	DrawText(pDC,STATION_NAME_EN_RECT,m_stationNameEN,REC_CN_FONT_NAME,REC_EN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT);
	DrawText(pDC,STATION_NAME_EN_RECT2,m_stationNameEN,REC_CN_FONT_NAME,REC_EN_FONT_SIZE,FW_NORMAL,WHITE,DT_LEFT);

    // ��ʾ��ǰʱ��
    DrawText(pDC, DATETIME_RECT, ComGetSysTimeText(YYYYMMDDHHMM_TYPE2), REC_CN_FONT_NAME,REC_EN_FONT_SIZE, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DrawText(pDC, DATETIME_RECT2, ComGetSysTimeText(YYYYMMDDHHMM_TYPE2), REC_CN_FONT_NAME,REC_EN_FONT_SIZE, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    DrawText(pDC, WEEKDAY_CN_RECT, ComGetSysTimeText(WEEKDAY_CN), REC_CN_FONT_NAME,REC_CN_FONT_SIZE2, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DrawText(pDC, WEEKDAY_CN_RECT2, ComGetSysTimeText(WEEKDAY_CN), REC_CN_FONT_NAME,REC_CN_FONT_SIZE2, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    DrawText(pDC, WEEKDAY_EN_RECT, ComGetSysTimeText(WEEKDAY_EN), REC_CN_FONT_NAME,REC_EN_FONT_SIZE, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DrawText(pDC, WEEKDAY_EN_RECT2, ComGetSysTimeText(WEEKDAY_EN), REC_CN_FONT_NAME,REC_EN_FONT_SIZE, FW_NORMAL, WHITE, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ٺ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnDestroy()
{
    // �ر�ʱ����ʾ��ʱ��
    this->KillTimer(TIMER_STS);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʱ�����

@param      (i)UINT nIDEvent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::OnTimer(UINT nIDEvent) 
{
    _DATE_TIME curTime = ComGetCurTime();
    // ��ʼ����Ӹı�ʱ��ˢ��ʱ����ʾ
    if (m_preTime.biMinute != curTime.biMinute || m_preTime.wYear == 0) {
        this->InvalidateRect(DATETIME_RECT, TRUE);
		this->InvalidateRect(WEEKDAY_CN_RECT, TRUE);
		this->InvalidateRect(WEEKDAY_EN_RECT, TRUE);
		this->InvalidateRect(DATETIME_RECT2, TRUE);
		this->InvalidateRect(WEEKDAY_CN_RECT2, TRUE);
		this->InvalidateRect(WEEKDAY_EN_RECT2, TRUE);
        m_preTime = curTime;
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ó�վ����

@param      (i)CString serviceNameCH   ҵ�����ƣ����ģ�
@param      (i)CString serviceNameEN   ҵ�����ƣ�Ӣ�ģ�

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionHeadDlg::SetStationName(CString stationNameCN, CString stationNameEN)
{
    m_stationNameCH = stationNameCN;
    m_stationNameEN = stationNameEN;

    this->InvalidateRect(STATION_NAME_CN_RECT, FALSE);
	this->InvalidateRect(STATION_NAME_EN_RECT, FALSE);
	this->InvalidateRect(STATION_NAME_CN_RECT2, FALSE);
	this->InvalidateRect(STATION_NAME_EN_RECT2, FALSE);
}