//GStaic.cpp : ʵ���ļ�

#include "stdafx.h"
#include "BOM.h"
#include "GStatic.h"
#include <mmsystem.h>
#include "uidefinition.h"
#include "messageiddefinition.h"
#include "Panel.h"
#include "BaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CGStatic, CStatic)

BEGIN_MESSAGE_MAP(CGStatic, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()	
END_MESSAGE_MAP()

CRect CGStatic::GetDrawRect( CPoint point, int rows )
{
	//GSTATIC_PADDING.cyTopHeight + GSTATIC_TITLE_HEIGHT;
	return CRect(
		point.x + GSTATIC_MARGINS.cxLeftWidth
		, point.y + GSTATIC_MARGINS.cyTopHeight
		, point.x + GSTATIC_WIDTH - GSTATIC_MARGINS.cxLeftWidth - GSTATIC_MARGINS.cxRightWidth 
		, point.y + GSTATIC_TITLE_HEIGHT + GSTATIC_LABEL_HEIGHT * rows + GSTATIC_MARGINS.cyTopHeight + GSTATIC_MARGINS.cyBottomHeight
		);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*          pParentWnd      ������ָ��
@param      (i)PGSTATIC_INFO  gstaticInfo     ��������          

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, bool isUsedForAuditSvc)
{
	CStatic::CStatic();

	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 2;
	m_isUsedForAuditSvc = isUsedForAuditSvc;

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row), pParentWnd, 0)) {
		throw "�����ı���ʧ��";
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*          pParentWnd      ������ָ��
@param      (i)PGSTATIC_INFO  gstaticInfo     ��������          
@param      (i)PLABEL_GROUP_THREE lableGroup  �ı������          

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, PLABEL_GROUP_THREE lableGroup)
{
	CStatic::CStatic();

	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 3;
	m_labelGroupThree = lableGroup;

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row), pParentWnd, 0)) {
		throw "�����ı���ʧ��";
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*          pParentWnd      ������ָ��
@param      (i)PGSTATIC_INFO  gstaticInfo     ��������          
@param      (i)PLABEL_GROUP_FOUR  lableGroup  �ı������          

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, PLABEL_GROUP_FOUR lableGroup)
{
	CStatic::CStatic();

	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 4;
	m_labelGroupFour = lableGroup;

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row), pParentWnd, 0)) {
		throw "�����ı���ʧ��";
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*          pParentWnd      ������ָ��
@param      (i)PGSTATIC_INFO  gstaticInfo     ��������          
@param      (i)PLABEL_GROUP_FIVE  lableGroup  �ı������          

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, PLABEL_GROUP_FIVE lableGroup)
{
	CStatic::CStatic();

	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 5;
	m_labelGroupFive = lableGroup;

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row), pParentWnd, 0)) {
		throw "�����ı���ʧ��";
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*            pParentWnd      ������ָ��
@param      (i)PGSTATIC_INFO    gstaticInfo     ��������          
@param      (i)PLABEL_GROUP_SIX lableGroup      �ı������          

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, PLABEL_GROUP_SIX lableGroup)
{
	CStatic::CStatic();

	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 6;
	m_labelGroupSix = lableGroup;

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row), pParentWnd, 0)) {
		throw "�����ı���ʧ��";
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CWnd*            pParentWnd      ������ָ��
@param      (i)PGSTATIC_INFO    gstaticInfo     ��������          
@param      (i)PLABEL_GROUP_SIX lableGroup      �ı������          

@retval     ��     

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, PLABEL_GROUP_SEVEN lableGroup)
{
	CStatic::CStatic();

	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 7;
	m_labelGroupSeven = lableGroup;

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row), pParentWnd, 0)) {
		throw "�����ı���ʧ��";
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
CGStatic::~CGStatic()
{
	try {
		for (int i = 0; i < m_gstaticInfo->detailInfo.editGroup.cnt; i++) {
			delete m_editArray[i];
			m_editArray[i] = NULL;
		}

		delete m_font;
		m_font = NULL;
	}
	catch(...){
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ת��Ϊ��ǰ��ʱ����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CGStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}

	// ������ɫ����
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j< MAX_COL; j++) {
			m_colorArray[i][j] = m_gstaticInfo->detailInfo.color;
		}
	}

	CPaintDC dc(this); 
	m_font = new CFont();
	int height = -MulDiv(BUTTON_FONT_SIZE, GetDeviceCaps(dc, LOGPIXELSY), 72);

	m_font->CreateFont(height, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
		GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH|FF_DONTCARE, REC_CN_FONT_NAME);


	// �����༭��
	for (int i = 0; i < m_gstaticInfo->detailInfo.editGroup.cnt; i++) {
		m_editArray[i] = new CGEdit(this, *(m_gstaticInfo->detailInfo.editGroup.editRect[i]), 
			m_gstaticInfo->detailInfo.editGroup.isLeftAlign);

		m_editArray[i]->SetFont(m_font);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����仯ʱˢ�»���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect=GetDrawRect(m_gstaticInfo->point,m_gstaticInfo->detailInfo.row);
	rect.MoveToXY(0,0);
	//GetClientRect(&rect);

	CBrush roundbrush(GRAY);
	dc.SelectObject(roundbrush);   

	// ���������
	CPen pen(PS_SOLID, ROUNDRECT_BORDER, DARKGRAY);
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.RoundRect(&rect, ROUND_POINT);
	dc.SelectObject(pOldPen); 
	dc.SelectStockObject(NULL_BRUSH);

	// ��ʾ�������
	dc.SetTextColor(m_gstaticInfo->titleInfo.color);
	dc.SetBkMode(TRANSPARENT);
	CFont titleFont;
	int height = -MulDiv(TITLE_FONT_SIZE, GetDeviceCaps(dc, LOGPIXELSY), 72);
	titleFont.CreateFont(height, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, 
		GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH|FF_DONTCARE, REC_CN_FONT_NAME);

	CFont *poldFont = dc.SelectObject(&titleFont);

	dc.TextOut(GSTATIC_PADDING.cxLeftWidth, GSTATIC_PADDING.cyTopHeight, m_gstaticInfo->titleInfo.titleName);

	// ��ʾ��������
	dc.SelectObject(m_font);

	PLABEL_GROUP tmp = m_gstaticInfo->detailInfo.labelGroup;
	PLABEL_GROUP_THREE groupThree = m_labelGroupThree;
	PLABEL_GROUP_FOUR groupFour = m_labelGroupFour;
	PLABEL_GROUP_FIVE groupFive = m_labelGroupFive;
	PLABEL_GROUP_SIX  groupSix  = m_labelGroupSix;
	PLABEL_GROUP_SEVEN groupSeven  = m_labelGroupSeven;

	int labelY = GSTATIC_PADDING.cyTopHeight + GSTATIC_TITLE_HEIGHT;

	for (int i = 0; i < m_gstaticInfo->detailInfo.row; i++) {
		if (m_col == 2) {
			int right = LABEL_RIGHT_X;

			// ���������ͳ��ҵ��ʹ�ã����������ʾλ��������
			if (m_isUsedForAuditSvc) {
				right += 17*4*CXSCREENRATIO;
			}
			dc.SetTextColor(m_colorArray[i][0]);
			dc.TextOut(GSTATIC_PADDING.cxLeftWidth*CXSCREENRATIO + 90, labelY, tmp->nameLeft);
			dc.SetTextColor(m_colorArray[i][1]);

			CString upStr, downStr;
			AfxExtractSubString(upStr, tmp->nameRight, 0);
			AfxExtractSubString(downStr, tmp->nameRight, 1);

			if (downStr == "") {
				dc.TextOut(right, labelY, upStr);
			}
			else {
				dc.TextOut(right, labelY, upStr);
				dc.TextOut(right, labelY + 19, downStr);
			}

			tmp++;
		}

		if (m_col == 3) {
			dc.SetTextColor(m_colorArray[i][0]);
			dc.TextOut(GSTATIC_PADDING.cxLeftWidth*CXSCREENRATIO, labelY, groupThree->nameLeft);

			dc.SetTextColor(m_colorArray[i][1]);
			dc.TextOut(180*CXSCREENRATIO, labelY, groupThree->nameCenter);

			dc.SetTextColor(m_colorArray[i][2]);
			dc.TextOut(320*CXSCREENRATIO, labelY, groupThree->nameRight);

			groupThree++;
		}

		if (m_col == 4) {
			dc.SetTextColor(m_colorArray[i][0]);
			dc.TextOut(GSTATIC_PADDING.cxLeftWidth*CXSCREENRATIO, labelY, groupFour->name1);

			dc.SetTextColor(m_colorArray[i][1]);
			dc.TextOut(144*CXSCREENRATIO, labelY, groupFour->name2);

			dc.SetTextColor(m_colorArray[i][2]);
			dc.TextOut(270*CXSCREENRATIO, labelY, groupFour->name3);

			dc.SetTextColor(m_colorArray[i][3]);
			dc.TextOut(400*CXSCREENRATIO, labelY, groupFour->name4);

			groupFour++;
		}

		if (m_col == 5) {
			dc.SetTextColor(m_colorArray[i][0]);
			dc.TextOut(72*CXSCREENRATIO, labelY, groupFive->name1);

			dc.SetTextColor(m_colorArray[i][1]);
			dc.TextOut(276*CXSCREENRATIO, labelY, groupFive->name2);

			dc.SetTextColor(m_colorArray[i][2]);
			dc.TextOut(336*CXSCREENRATIO, labelY, groupFive->name3);

			dc.SetTextColor(m_colorArray[i][3]);
			dc.TextOut(449*CXSCREENRATIO, labelY, groupFive->name4);

			dc.SetTextColor(m_colorArray[i][4]);
			dc.TextOut(581*CXSCREENRATIO, labelY, groupFive->name5);

			groupFive++;
		}

		if (m_col == 6) {
			if (groupSix->isForECT) {
				dc.SetTextColor(m_colorArray[i][0]);
				dc.TextOut(33*CXSCREENRATIO, labelY, groupSix->name1);

				dc.SetTextColor(m_colorArray[i][1]);
				dc.TextOut(175*CXSCREENRATIO, labelY, groupSix->name2);

				dc.SetTextColor(m_colorArray[i][2]);
				dc.TextOut(258*CXSCREENRATIO, labelY, groupSix->name3);

				dc.SetTextColor(m_colorArray[i][3]);
				dc.TextOut(555*CXSCREENRATIO, labelY, groupSix->name4);

				dc.SetTextColor(m_colorArray[i][4]);
				dc.TextOut(638*CXSCREENRATIO, labelY, groupSix->name5);

				dc.SetTextColor(m_colorArray[i][5]);
				dc.TextOut(729*CXSCREENRATIO, labelY, groupSix->name6);
			}
			else {
				dc.SetTextColor(m_colorArray[i][0]);
				dc.TextOut(33*CXSCREENRATIO, labelY, groupSix->name1);

				dc.SetTextColor(m_colorArray[i][1]);
				dc.TextOut(228*CXSCREENRATIO, labelY, groupSix->name2);

				dc.SetTextColor(m_colorArray[i][2]);
				dc.TextOut(383*CXSCREENRATIO, labelY, groupSix->name3);

				dc.SetTextColor(m_colorArray[i][3]);
				dc.TextOut(534*CXSCREENRATIO, labelY, groupSix->name4);

				dc.SetTextColor(m_colorArray[i][4]);
				dc.TextOut(619*CXSCREENRATIO, labelY, groupSix->name5);

				dc.SetTextColor(m_colorArray[i][5]);
				dc.TextOut(729*CXSCREENRATIO, labelY, groupSix->name6);
			}
			groupSix++;
		}
		if (m_col == 7) {
			dc.SetTextColor(m_colorArray[i][0]);
			dc.TextOut(29*CXSCREENRATIO, labelY, groupSeven->name1);

			dc.SetTextColor(m_colorArray[i][1]);
			dc.TextOut(179*CXSCREENRATIO, labelY, groupSeven->name2);

			dc.SetTextColor(m_colorArray[i][2]);
			dc.TextOut(269*CXSCREENRATIO, labelY, groupSeven->name3);

			dc.SetTextColor(m_colorArray[i][3]);
			dc.TextOut(439*CXSCREENRATIO, labelY, groupSeven->name4);

			dc.SetTextColor(m_colorArray[i][4]);
			dc.TextOut(509*CXSCREENRATIO, labelY, groupSeven->name5);

			dc.SetTextColor(m_colorArray[i][5]);
			dc.TextOut(619*CXSCREENRATIO, labelY, groupSeven->name6);

			dc.SetTextColor(m_colorArray[i][6]);
			dc.TextOut(729*CXSCREENRATIO, labelY, groupSeven->name7);
			groupSeven++;
		}
		labelY += GSTATIC_LABEL_HEIGHT;
	}

	dc.SelectObject(poldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ı������ʾ����

@param      (i)int        row      �ı��к�
@param      (i)int        col      �ı��к�
@param      (i)CString    txt      �ı�������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetLabelText(int row, int col, CString txt)
{
	if (2 == m_col) {
		if (row > 0 && col > 0 && row <= m_labelRow) {
			if (1 == col) {
				((m_gstaticInfo->detailInfo.labelGroup) + row - 1)->nameLeft = txt;
			}
			if (2 == col) {
				((m_gstaticInfo->detailInfo.labelGroup) + row- 1)->nameRight = txt;
			}
		}
	}

	InvalidateRect(NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ñ��������

@param      (i)int        index    �༭����
@param      (i)CString    txt      �༭������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetTitleText(CString txt)
{
	m_gstaticInfo->titleInfo.titleName = txt;
	InvalidateRect(NULL);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ñ༭��ĵ�ַ

@param      (i)int        index    �༭����

@retval     CGEdit*    �༭���ַ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CGEdit* CGStatic::GetEdit(int index)
{
	return m_editArray[index];
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ı������ֵ���ɫ

@param      (i)int         row      �ı����к�
@param      (i)int         col      �ı����к�
@param      (i)COLORREF    color    �ı�����ɫ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetLabelColor(int row, int col, COLORREF color)
{
	m_colorArray[row][col] = color;
	InvalidateRect(NULL);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��д��Ϣ�������������ǰ��Ϣ����������ֵΪ-1ʱ����Ϣ����Ҫ·�ɵ������壬�����������Ϣ·�ɵ������塣

@param      (i)UINT message ��ϢID
@param      (i)WPARAM wParam ��Ϣ����
@param      (i)LPARAM lParam ��Ϣ����

@retval     LRESULT

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CGStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = __super::WindowProc(message,wParam,lParam);
	if(message>WM_USER && result>=0 &&  IsWindow(m_hWnd) && GetParent()!=NULL)
	{
		CWnd *pParent = GetParent();

		if(pParent->IsKindOf(RUNTIME_CLASS(CBaseDlg)))
		{
			((CBaseDlg*)pParent)->CallWndProc(message,wParam,lParam);
		}
		else if(pParent->IsKindOf(RUNTIME_CLASS(CPanel)))
		{
			((CPanel*)pParent)->CallWndProc(message,wParam,lParam);
		}
		else
		{
			pParent->SendMessage(message,wParam,lParam);
		}
	}
	return result;
}
