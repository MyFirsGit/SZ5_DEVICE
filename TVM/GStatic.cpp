//GStaic.cpp : ʵ���ļ�

#include "stdafx.h"
#include "TVM.h"
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
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CRect CGStatic::GetDrawRect( CPoint point, int rows )
{
	//GSTATIC_PADDING.cyTopHeight + GSTATIC_TITLE_HEIGHT;
	return CRect(
		point.x + m_margin.cxLeftWidth
		, point.y + m_margin.cyTopHeight
		, point.x + m_width - m_margin.cxLeftWidth - m_margin.cxRightWidth 
		, point.y + m_title_height + m_lable_height * rows + m_margin.cyTopHeight + m_margin.cyBottomHeight
		);
}
CGStatic::CGStatic(CWnd* pParentWnd,CRect rect)
{
	CStatic::CStatic();
	m_labelRow = 0;
	m_gstaticInfo = NULL;
	m_col = 0;
	m_isUsedForAuditSvc = false;
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
	m_szContent = _T("0.0Ԫ");
	//m_width = GSTATIC_STYLE_1_WIDTH;
	m_border = GSTATIC_STYLE_1_BORDER;
	m_round = GSTATIC_STYLE_1_ROUND;
	m_margin = GSTATIC_STYLE_1_MARGIN;
	m_padding = GSTATIC_STYLE_1_PADDING;
	m_title_height =GSTATIC_STYLE_1_TITLE_HEIGHT;

	m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;
	m_pNoticeImgBk = new CImage;
	CString szPath = _T("");
	GetAppPath(szPath);
	szPath += _T("\\res\\TVM_cash_in.png");
	m_pNoticeImgBk->Load(szPath);
	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, rect, pParentWnd, 0xffff)) {
		throw "�����ı���ʧ��";
	}

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
CGStatic::CGStatic(CWnd* pParentWnd, PGSTATIC_INFO gstaticInfo, bool isUsedForAuditSvc,CImage* pImge)
{
	CStatic::CStatic();
	m_labelRow = gstaticInfo->detailInfo.row;
	m_gstaticInfo = gstaticInfo;
	m_col = 2;
	m_isUsedForAuditSvc = isUsedForAuditSvc;
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
	m_pBkImg = NULL;
	m_pBkImg = pImge;
	SetStyle();

	if (!CStatic::Create(_T(""), WS_CHILD|WS_VISIBLE, gstaticInfo->size == CSize(0,0)?GetDrawRect(m_gstaticInfo->point,gstaticInfo->detailInfo.row):CRect(gstaticInfo->point,gstaticInfo->size), pParentWnd, 0xffff)) {
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
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
	SetStyle();
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
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
	SetStyle();
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
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
	SetStyle();
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
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
    SetStyle();
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
	m_pErweimaImg = NULL;
	m_pPaymentTipsImg = NULL;
	m_pUseTipImg = NULL;
	SetStyle();

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
		if (m_pBkImg)
		{
			delete m_pBkImg;
			m_pBkImg = NULL;
		}
		if(m_pErweimaImg){
			delete m_pErweimaImg;
			m_pErweimaImg = NULL;
		}
		if(m_pPaymentTipsImg){
			delete m_pPaymentTipsImg;
			m_pPaymentTipsImg = NULL;
		}
		if(m_pUseTipImg){
			delete m_pUseTipImg;
			m_pUseTipImg = NULL;
		}
		if (m_pNoticeImgBk)
		{
			delete m_pNoticeImgBk;
			m_pNoticeImgBk = NULL;
		}
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
	if (m_gstaticInfo)
	{
		if(m_gstaticInfo->stytle==IMAGE_CENTER_STYLE){
			CString bmppath;
			CString apppath;
			GetAppPath(apppath);
			// ��ά��ɨ����ʾ��̬ͼƬ
			bmppath = apppath + _T("\\res\\TVM_ErWeiMa_Use_tips.gif");
			m_picex.createPicWnd(this->m_hWnd);
			m_picex.loadPic(bmppath);
		}


		// ������ɫ����
		//for (int i = 0; i < MAX_ROW; i++) {
		//	for (int j = 0; j< MAX_COL; j++) {
		//		m_colorArray[i][j] = NORMAL_FONT_COLOR/*m_gstaticInfo->detailInfo.color*/;
		//	}
		//}

		// ��ʼ�����뷽ʽ
		for (int i = 0; i < MAX_ROW; ++i) {
			for (int j = 0; j < MAX_COL; j++) {
				m_alignArray[i][j] = DT_LEFT;
			}
		}

		// ��ʼ�����
		for (int i = 0; i < MAX_ROW; ++i) {
			for (int j = 0; j < MAX_COL; j++) {
				m_widthArray[i][j] = 300;
			}
		}

		CPaintDC dc(this); 

		// �����༭��
		for (int i = 0; i < m_gstaticInfo->detailInfo.editGroup.cnt; i++) {
			m_editArray[i] = new CGEdit(this, *(m_gstaticInfo->detailInfo.editGroup.editRect[i]), 
				m_gstaticInfo->detailInfo.editGroup.isLeftAlign);

			m_editArray[i]->SetFont(&m_font);
		}

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

	CRect rect;
	if (m_gstaticInfo)
	{
		rect=GetDrawRect(m_gstaticInfo->point,m_gstaticInfo->detailInfo.row);
		if (m_gstaticInfo->size != CSize(0,0))
		{
			rect = CRect(CPoint(0,0),m_gstaticInfo->size);
		}
	}
	else{
		rect = REC_CASHIN_NOTICE_RECT;
	}
	rect.MoveToXY(0,0);
	//GetClientRect(&rect);

	// ʹ��˫�����ͼ
	CDC MemDc;
	CBitmap MemBitMap;

	MemDc.CreateCompatibleDC(NULL);// �ڴ���׼��ͼ��

	MemBitMap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());// �����ڴ�λͼ

	CBitmap* pOldBitmap = MemDc.SelectObject(&MemBitMap);// ѡ��λͼ

	
	// ������ʹ���ڴ�λͼ��ͼ
	// ��̬��������������
	if (m_gstaticInfo)
	{
		//lws  : ���ڳ˿ͽ���ͺ�ά��������BaseinfoArea������Gsatic�ؼ� ����GStatic���Ƴ˿ͽ���ͺ�ά������ʱҪ�����ж�
		if (m_gstaticInfo->stytle == OPERATOR_STATIC_STYLE){
			// ����վԱ����ͼzxl
			CBrush brush(OPERATOR_BASE_BK_COLOR);
			MemDc.FillRect(rect,&brush);
		}
		// �˿ͻ�����Ϣ������
		else if(m_gstaticInfo->stytle == PASSAGE_NARROW_STYLE || m_gstaticInfo->stytle == PASSAGE_WIDE_STYLE || m_gstaticInfo->stytle == SIGLE_ELEMENT_STYLE 
			|| m_gstaticInfo->stytle == ANYLE_INFO_STYLE || m_gstaticInfo->stytle == ISSUE_INFO_STYLE|| m_gstaticInfo->stytle == IMAGE_CENTER_STYLE){
				 //���Ƴ˿ͱ���ͼzxl
				CBrush brush(RECEPTION_BK_COLOR);
				MemDc.FillRect(rect,&brush);
		}

		//// ��������Ϣ���߿�
		//CBrush operaArea;// ����߿�ˢ
		//CRgn rgn;				   // ָ��Բ�Ǿ���
		//rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,m_round.x,m_round.y);
		//MemDc.FrameRgn(&rgn,&operaArea,1,1/*m_border,m_border*/);// ��ʼ���߿�
		//CRgn rgn;				   // ָ��Բ�Ǿ���
		//rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,m_round.x,m_round.y);
		//CBrush *pColorBrush = new CBrush(/*OPERATOR_BASE_BK_COLOR*/);
		//MemDc.FillRgn(&rgn,pColorBrush);

		// ���򱳾�
		if (m_col == 2 && m_pBkImg != NULL)
		{
			//CRect contentRect = rect;
			//contentRect.DeflateRect(0,0,0,0);
			m_pBkImg->Draw(MemDc,rect);
		}
		CFont *poldFont = NULL;
		MemDc.SetBkMode(TRANSPARENT);
		// ��ʾ�������
		if (m_title_height!=0){
			MemDc.SetTextColor(RED/*m_Title_Font_Color*/);
			//MemDc.SetBkMode(TRANSPARENT);

			poldFont = (CFont*)MemDc.SelectObject(&m_titlefont);

			MemDc.TextOut(m_gstaticInfo->type == 0?m_padding.cxLeftWidth+25:m_padding.cxLeftWidth+170, m_padding.cyTopHeight+60, m_gstaticInfo->titleInfo.titleName);
			MemDc.SelectObject(poldFont );
		}
		// ��ʾ��������
		poldFont = MemDc.SelectObject(&m_font);
		if (!m_gstaticInfo->szContent.IsEmpty())
		{
			CString szContent = m_gstaticInfo->szContent;
			int nIndex = szContent.Find(_T("^"));
			CString szCon1 = _T(""),szCon2 = _T("");
			if (nIndex > 0)
			{
				szCon1 = szContent.Left(nIndex);
				szCon2 = szContent.Mid(nIndex+1);
			}
			else
			{
				szCon1 = szContent;
			}
			poldFont = MemDc.SelectObject(&m_titlefont);
			CRect rt2;  
			int startX = m_padding.cxLeftWidth+220;
			int startY = m_padding.cyTopHeight +60 + m_title_height + 50 + 200;
			if (nIndex > 0)
			{
				startY -= 60;
			}
			rt2.SetRect(startX,startY,startX+m_lable_left_with+270, startY+m_lable_height+50);		
			// ���
			MemDc.SetTextColor(RGB(0,0,0));
			MemDc.DrawText(szCon1,rt2, DT_LEFT| DT_VCENTER | DT_SINGLELINE);
			startY = rt2.bottom+50;
			rt2.SetRect(startX,startY,startX+m_lable_left_with+270, startY+m_lable_height+50);		
			MemDc.SetTextColor(RGB(0,0,0));
			MemDc.DrawText(szCon2,rt2, DT_LEFT| DT_VCENTER | DT_SINGLELINE);

			MemDc.SelectObject(poldFont);

		}
		else{
			PLABEL_GROUP tmp = m_gstaticInfo->detailInfo.labelGroup;
			PLABEL_GROUP_THREE groupThree = m_labelGroupThree;
			PLABEL_GROUP_FOUR groupFour = m_labelGroupFour;
			PLABEL_GROUP_FIVE groupFive = m_labelGroupFive;
			PLABEL_GROUP_SIX  groupSix  = m_labelGroupSix;
			PLABEL_GROUP_SEVEN groupSeven  = m_labelGroupSeven;

			int labelY = m_padding.cyTopHeight + m_title_height;

			CRect rt2;  
			int startX = m_padding.cxLeftWidth+60+40;
			int startY = m_padding.cyTopHeight +60 + m_title_height + 60;
			if (m_gstaticInfo->stytle == OPERATOR_STATIC_STYLE)
			{
				if (m_gstaticInfo->type == 2)
				{
					startY -= 85;
				}
				else
				{
					startY -= 120;
					startX -= 60;
				}
			}
			for (int i = 0; i < m_gstaticInfo->detailInfo.row; i++) {
				if (i > 9 && m_gstaticInfo->stytle != OPERATOR_STATIC_STYLE)
				{
					poldFont = MemDc.SelectObject(&m_fontR);
				}
				//else 
				if (m_col == 2 && m_gstaticInfo->stytle != OPERATOR_STATIC_STYLE) {
					if (m_gstaticInfo->stytle == CHARGE_INFO_STYLE)
					{
						rt2.SetRect(startX+100,startY+200,startX+m_lable_left_with+50, startY+m_lable_height);		
						// ���
						MemDc.SetTextColor(m_colorArray[i][0]);
						MemDc.DrawText(tmp->nameLeft,rt2, DT_RIGHT| DT_VCENTER | DT_SINGLELINE);

						// �ұ�
						rt2.left += rt2.right/*+m_lable_mid_with*/;
						rt2.right += rt2.left+m_lable_right_with;
						MemDc.SetTextColor(m_colorArray[i][1]);
						MemDc.DrawText(tmp->nameRight,rt2, DT_LEFT|DT_VCENTER | DT_SINGLELINE);

						startY+=(m_lable_height+20);
						tmp++;
					}
					else if(m_gstaticInfo->stytle != IMAGE_CENTER_STYLE){
						rt2.SetRect(startX,startY,startX+m_lable_left_with, startY+m_lable_height);		
						// ���
						MemDc.SetTextColor(m_colorArray[i][0]);
						MemDc.DrawText(tmp->nameLeft,rt2, DT_LEFT| DT_VCENTER | DT_SINGLELINE);

						// �ұ�
						rt2.left += rt2.right/*+m_lable_mid_with*/;
						rt2.right += rt2.left+m_lable_right_with;
						MemDc.SetTextColor(m_colorArray[i][1]);
						MemDc.DrawText(tmp->nameRight,rt2, DT_LEFT|DT_VCENTER | DT_SINGLELINE);

						startY+=m_lable_height;
						tmp++;
					}
					else{		
						// ����֧����ʽ��ʾͼƬ
						if (!m_pPaymentTipsImg->IsNull()){
							int border = 2;
							CRect imageRect = CRect(140, 100, 140+m_pPaymentTipsImg->GetWidth(), 100+m_pPaymentTipsImg->GetHeight());
							imageRect.DeflateRect(4, 4, 4, 4);
							m_pPaymentTipsImg->Draw(MemDc,imageRect);
						}
						// �����ά��ͼƬ���ڣ���ʾ��ά��ͼƬ��������ʾɨ����ʾͼƬ
						//CImage* pShowImg = m_pUseTipImg;
						//CString bmppath;
						//CString apppath;
						//GetAppPath(apppath);
						//bmppath = apppath + _T("QR_ORDER_INFO_0.bmp");
						//if(ComCheckFileExist(bmppath)){
						//	if(m_pErweimaImg==NULL){
						//		m_pErweimaImg = new CImage;
						//		m_pErweimaImg->Load(bmppath);
						//		if(m_pErweimaImg->IsNull()){
						//			pShowImg = m_pErweimaImg;
						//		}
						//	}
						//	else{
						//		pShowImg = m_pErweimaImg;
						//	}
						//}
						//if (!pShowImg->IsNull()){
						//	CRect imageRect = CRect(784, 100, 784+225, 100+225);
						//	imageRect.DeflateRect(4, 4, 4, 4);
						//	pShowImg->Draw(MemDc,imageRect);
						//}

						// �����ά��ͼƬ���ڣ���ʾ��ά��ͼƬ��������ʾɨ����ʾͼƬ
						CString bmppath;
						CString apppath;
						GetAppPath(apppath);
						bmppath = apppath + _T("QR_ORDER_INFO_0.bmp");
						if(ComCheckFileExist(bmppath)){
							if(m_pErweimaImg==NULL){
								KillTimer(10001);
								m_picex.ShowWindow(FALSE);
								m_pErweimaImg = new CImage;
								m_pErweimaImg->Load(bmppath);
							}
							if(!m_pErweimaImg->IsNull()){
								CRect imageRect = CRect(784, 100, 784+225, 100+225);
								imageRect.DeflateRect(4, 4, 4, 4);
								m_pErweimaImg->Draw(MemDc,imageRect);				
							}
						}
						else{
							CRect imageRect = CRect(784, 100, 784+225, 100+225);
							m_picex.MoveWindow(&imageRect);
							m_picex.ShowWindow(TRUE);
							m_picex.resumeRender();
						}
					}
				}
				else if (m_col == 2) {
					rt2.SetRect(startX, startY, startX + m_widthArray[i][0], startY + m_lable_height);
					MemDc.SetTextColor(m_colorArray[i][0]);
					MemDc.DrawText(tmp->nameLeft, rt2, m_alignArray[i][0] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][1];
					MemDc.SetTextColor(m_colorArray[i][1]);
					MemDc.DrawText(tmp->nameRight, rt2, m_alignArray[i][1] | DT_SINGLELINE);

					startY += m_lable_height;
					tmp++;
				}
				else if (m_col == 3) {
					rt2.SetRect(startX, startY, startX + m_widthArray[i][0], startY + m_lable_height);
					MemDc.SetTextColor(m_colorArray[i][0]);
					MemDc.DrawText(groupThree->nameLeft, rt2, m_alignArray[i][0] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][1];
					MemDc.SetTextColor(m_colorArray[i][1]);
					MemDc.DrawText(groupThree->nameCenter, rt2, m_alignArray[i][1] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][2];
					MemDc.SetTextColor(m_colorArray[i][2]);
					MemDc.DrawText(groupThree->nameRight, rt2, m_alignArray[i][2] | DT_SINGLELINE);

					startY += m_lable_height;
					groupThree++;
				}
				else if (m_col == 4) {
					rt2.SetRect(startX, startY, startX + m_widthArray[i][0], startY + m_lable_height);
					MemDc.SetTextColor(m_colorArray[i][0]);
					MemDc.DrawText(groupFour->name1, rt2, m_alignArray[i][0] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][1];
					MemDc.SetTextColor(m_colorArray[i][1]);
					MemDc.DrawText(groupFour->name2, rt2, m_alignArray[i][1] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][2];
					MemDc.SetTextColor(m_colorArray[i][2]);
					MemDc.DrawText(groupFour->name3, rt2, m_alignArray[i][2] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][3];
					MemDc.SetTextColor(m_colorArray[i][3]);
					MemDc.DrawText(groupFour->name4, rt2, m_alignArray[i][3] | DT_SINGLELINE);

					startY += m_lable_height;
					groupFour++;
				}
				else if (m_col == 5) {
					rt2.SetRect(startX, startY, startX + m_widthArray[i][0], startY + m_lable_height);
					MemDc.SetTextColor(m_colorArray[i][0]);
					MemDc.DrawText(groupFive->name1, rt2, m_alignArray[i][0] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][1];
					MemDc.SetTextColor(m_colorArray[i][1]);
					MemDc.DrawText(groupFive->name2, rt2, m_alignArray[i][1] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][2];
					MemDc.SetTextColor(m_colorArray[i][2]);
					MemDc.DrawText(groupFive->name3, rt2, m_alignArray[i][2] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][3];
					MemDc.SetTextColor(m_colorArray[i][3]);
					MemDc.DrawText(groupFive->name4, rt2, m_alignArray[i][3] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][4];
					MemDc.SetTextColor(m_colorArray[i][4]);
					MemDc.DrawText(groupFive->name5, rt2, m_alignArray[i][4] | DT_SINGLELINE);

					startY += m_lable_height;
					groupFive++;
				}
				else if (m_col == 6) {
					rt2.SetRect(startX, startY, startX + m_widthArray[i][0], startY + m_lable_height);
					MemDc.SetTextColor(m_colorArray[i][0]);
					MemDc.DrawText(groupSix->name1, rt2, m_alignArray[i][0] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][1];
					MemDc.SetTextColor(m_colorArray[i][1]);
					MemDc.DrawText(groupSix->name2, rt2, m_alignArray[i][1] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][2];
					MemDc.SetTextColor(m_colorArray[i][2]);
					MemDc.DrawText(groupSix->name3, rt2, m_alignArray[i][2] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][3];
					MemDc.SetTextColor(m_colorArray[i][3]);
					MemDc.DrawText(groupSix->name4, rt2, m_alignArray[i][3] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][4];
					MemDc.SetTextColor(m_colorArray[i][4]);
					MemDc.DrawText(groupSix->name5, rt2, m_alignArray[i][4] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][5];
					MemDc.SetTextColor(m_colorArray[i][5]);
					MemDc.DrawText(groupSix->name6, rt2, m_alignArray[i][5] | DT_SINGLELINE);

					startY += m_lable_height;
					groupSix++;
				}
				else if (m_col == 7) {
					rt2.SetRect(startX, startY, startX + m_widthArray[i][0], startY + m_lable_height);
					MemDc.SetTextColor(m_colorArray[i][0]);
					MemDc.DrawText(groupSeven->name1, rt2, m_alignArray[i][0] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][1];
					MemDc.SetTextColor(m_colorArray[i][1]);
					MemDc.DrawText(groupSeven->name2, rt2, m_alignArray[i][1] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][2];
					MemDc.SetTextColor(m_colorArray[i][2]);
					MemDc.DrawText(groupSeven->name3, rt2, m_alignArray[i][2] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][3];
					MemDc.SetTextColor(m_colorArray[i][3]);
					MemDc.DrawText(groupSeven->name4, rt2, m_alignArray[i][3] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][4];
					MemDc.SetTextColor(m_colorArray[i][4]);
					MemDc.DrawText(groupSeven->name5, rt2, m_alignArray[i][4] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][5];
					MemDc.SetTextColor(m_colorArray[i][5]);
					MemDc.DrawText(groupSeven->name6, rt2, m_alignArray[i][5] | DT_SINGLELINE);

					rt2.left = rt2.right + m_lable_mid_with;
					rt2.right = rt2.left + m_widthArray[i][6];
					MemDc.SetTextColor(m_colorArray[i][6]);
					MemDc.DrawText(groupSeven->name7, rt2, m_alignArray[i][6] | DT_SINGLELINE);

					startY += m_lable_height;
					groupSeven++;
				}
				else if (m_col == 1) {
					if(tmp->nameLeft.Find(TXT_COLOR_RED)>=0){
						tmp->nameLeft.Replace(TXT_COLOR_RED, _T(""));
						MemDc.SetTextColor(RED);
					}
					else{
						MemDc.SetTextColor(m_colorArray[i][0]);
					}
					MemDc.DrawText(tmp->nameLeft, rect, DT_CENTER |DT_VCENTER | DT_SINGLELINE);

					startY += m_lable_height;
					tmp++;
				}		
				labelY += m_lable_height;
			}
			
			MemDc.SelectObject(poldFont);
			if (m_gstaticInfo->stytle == CHARGE_INFO_STYLE)
			{
				CFont font;
				TCHAR tcInfoFontName[32];
				_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);

				font.CreateFont(TVM_PAS_BASE_INFO_FONT_SIZE+5, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
					CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
					DEFAULT_PITCH|FF_SWISS, tcInfoFontName);

				MemDc.SelectObject(&font);
				CRect rt2;
				rt2.SetRect(startX+160,startY+350,startX+m_lable_left_with+360, startY+m_lable_height+50);		
				// ���
				MemDc.SetTextColor(RGB(0,0,0));
				MemDc.DrawText(_T("����ֽ�Һ󣬰���ֵ��ť"),rt2, DT_LEFT| DT_VCENTER | DT_SINGLELINE);

			}

		}
		
	}
	// ��̬����ֻ��ʾ�ı�
	else{
		CBrush brush(OPERATOR_BASE_BK_COLOR);
		MemDc.FillRect(rect,&brush);
		// ��������Ϣ���߿�
		//CBrush operaArea(DARKGRAY);// ����߿�ˢ
		CRgn rgn;				   // ָ��Բ�Ǿ���
		rgn.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,m_round.x,m_round.y);
		//MemDc.FrameRgn(&rgn,&operaArea,m_border,m_border);// ��ʼ���߿�
		CBrush *pColorBrush = new CBrush(OPERATOR_BASE_BK_COLOR);           //��ʾ����ı���ɫ,���

		MemDc.FillRgn(&rgn,pColorBrush);
		CRect imageRect = rect;
		//imageRect.DeflateRect(4, 4, 4, 4);
		m_pNoticeImgBk->Draw(MemDc,imageRect);				

		CFont *poldFont = NULL;
		MemDc.SetBkMode(TRANSPARENT);
		// ��ʾ�������
		if (1/*m_title_height!=0*/){
			MemDc.SetTextColor(RED);
			CFont font;
			font.CreateFont(50, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
				CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
				DEFAULT_PITCH|FF_SWISS, _T("����"));

			poldFont = (CFont*)MemDc.SelectObject(&font);

			MemDc.TextOut(m_padding.cxLeftWidth+160, m_padding.cyTopHeight+30, m_szContent);
			MemDc.SelectObject(poldFont );
		}
	}
	// ���ڴ��е�λͼ�浽DC��ȥ
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDc,0,0,SRCCOPY);

	// �ڴ�ָ�
	MemDc.SelectObject(pOldBitmap);

	// ��ͼ����
	MemBitMap.DeleteObject();
	MemDc.DeleteDC();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CGStatic::OnEraseBkgnd(CDC*){
	return FALSE;
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
@brief      �����ı������ֵĶ��뷽ʽ

@param      (i)int         row      �ı����к�
@param      (i)int         col      �ı����к�
@param      (i)DWORD       align    �ı����ı����뷽ʽ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetLableAlign(int row, int col, DWORD align)
{
	m_alignArray[row][col] = align;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ı������ֵĶ��뷽ʽ��һ�У�

@param      (i)int         col      �ı����к�
@param      (i)DWORD       align    �ı����ı����뷽ʽ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetLableAlign(int col, DWORD align)
{
	for (int i = 0; i < MAX_ROW; ++i)
		m_alignArray[i][col] = align;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ı������ֵĿ��

@param      (i)int         row      �ı����к�
@param      (i)int         col      �ı����к�
@param      (i)int         width    �ı�����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetLableWidth(int row, int col, int width)
{
	m_widthArray[row][col] = width;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ı������ֵĿ�ȣ�һ�У�

@param      (i)int         col      �ı����к�
@param      (i)int         width    �ı�����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CGStatic::SetLableWidth(int col, int width)
{
	for (int i = 0; i < MAX_ROW; ++i)
		m_widthArray[i][col] = width;
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

void CGStatic::SetStyle()
{
	DWORD textSize;
	CString textfont;
	if (m_fontR.GetSafeHandle() != NULL)
	{
		m_fontR.DeleteObject();
	}
	if(m_font.GetSafeHandle()!=NULL)
	{
		m_font.DeleteObject();
	}
	if(m_titlefont.GetSafeHandle()!=NULL)
	{
		m_titlefont.DeleteObject();
	}

	DWORD dwTitleFontSize=1;
	DWORD dwTitleCharSet=CHARSET;
	TCHAR tcTitleFontName[32];
	DWORD dwInfoFontSize=1;
	DWORD dwInfoFontSizeEx = 1;
	DWORD dwInfoCharSet=CHARSET;
	TCHAR tcInfoFontName[32];

	switch (m_gstaticInfo->stytle)
	{
	// �˿Ϳ���Ϣ����
	case PASSAGE_WIDE_STYLE:
		m_width = GSTATIC_STYLE_1_WIDTH;
		m_border = GSTATIC_STYLE_1_BORDER;
		m_round = GSTATIC_STYLE_1_ROUND;
		m_margin = GSTATIC_STYLE_1_MARGIN;
		m_padding = GSTATIC_STYLE_1_PADDING;
		m_lable_height = GSTATIC_STYLE_1_LABEL_HEIGHT;
		m_title_height = m_gstaticInfo->titleInfo.titleName==_T("--")?0:GSTATIC_STYLE_1_TITLE_HEIGHT;
		m_lable_left_with = LABEL_1_LEFT_WITH;    
		m_lable_right_with = LABEL_1_RIGHT_WITH;   
		m_lable_mid_with = LABEL_1_MID_WITH;     

		m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;
		
		dwInfoFontSize=TVM_PAS_BASE_INFO_FONT_SIZE;
		_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);
		
		dwTitleFontSize=TVM_PAS_BASE_TITLE_FONT_SIZE;
		_tcscpy_s(tcTitleFontName,32,TVM_PAS_BASE_TITLE_FONT_NAME );

		dwInfoCharSet=CHARSET;
		dwTitleCharSet=CHARSET;

		for (int i = 0; i < MAX_ROW; i++) {
			for (int j = 0; j< MAX_COL; j++) {
				m_colorArray[i][j] = TVM_PAS_BASE_INFO_FONT_COLOR;
			}
		}
		break;
		// �˿���խ����
	case PASSAGE_NARROW_STYLE:
		{
			bool bIsShowPaymentArea = theFunction_INFO.IsShowPaymentMethod();

			m_width = GSTATIC_STYLE_3_WIDTH;
			m_border = GSTATIC_STYLE_3_BORDER;
			m_round = GSTATIC_STYLE_3_ROUND;
			m_margin = GSTATIC_STYLE_3_MARGIN;
			m_padding = GSTATIC_STYLE_3_PADDING;
			m_lable_height =bIsShowPaymentArea ? GSTATIC_STYLE_3_LABEL_HEIGHT : GSTATIC_STYLE_3_LABEL_PADDING;
			m_title_height = GSTATIC_STYLE_3_TITLE_HEIGHT;
			m_lable_left_with = LABEL_3_LEFT_WITH;    
			m_lable_right_with = LABEL_3_RIGHT_WITH;   
			m_lable_mid_with = LABEL_3_MID_WITH ;

			m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;

			dwInfoFontSize=TVM_PAS_BASE_INFO_FONT_SIZE;
			_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);

			dwTitleFontSize=TVM_PAS_BASE_TITLE_FONT_SIZE;
			_tcscpy_s(tcTitleFontName,32,TVM_PAS_BASE_TITLE_FONT_NAME );

			dwInfoCharSet=CHARSET;
			dwTitleCharSet=CHARSET;

			for (int i = 0; i < MAX_ROW; i++) {
				for (int j = 0; j< MAX_COL; j++) {
					m_colorArray[i][j] = TVM_PAS_BASE_INFO_FONT_COLOR;
				}
			}
		}
		
		break;
	case SIGLE_ELEMENT_STYLE:
		m_col = 1;
		m_width = GSTATIC_STYLE_5_WIDTH;
		m_border = GSTATIC_STYLE_1_BORDER;
		m_round = GSTATIC_STYLE_1_ROUND;
		m_margin = GSTATIC_STYLE_1_MARGIN;
		m_padding = GSTATIC_STYLE_1_PADDING;
		m_lable_height = GSTATIC_STYLE_1_LABEL_HEIGHT;
		m_title_height = m_gstaticInfo->titleInfo.titleName==_T("--")?0:GSTATIC_STYLE_1_TITLE_HEIGHT;
		m_lable_left_with = LABEL_1_LEFT_WITH;    
		m_lable_right_with = LABEL_1_RIGHT_WITH;   
		m_lable_mid_with = LABEL_1_MID_WITH;  

		m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;

		dwInfoFontSize=TVM_PAS_BASE_INFO_FONT_SIZE;
		_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);

		dwTitleFontSize=TVM_PAS_BASE_TITLE_FONT_SIZE;
		_tcscpy_s(tcTitleFontName,32,TVM_PAS_BASE_TITLE_FONT_NAME );

		dwInfoCharSet=CHARSET;
		dwTitleCharSet=CHARSET;

		for (int i = 0; i < MAX_ROW; i++) {
			for (int j = 0; j< MAX_COL; j++) {
				m_colorArray[i][j] = TVM_PAS_BASE_INFO_FONT_COLOR;
			}
		}
		break;
	case ANYLE_INFO_STYLE:
		m_width = GSTATIC_STYLE_4_WIDTH;
		m_border = GSTATIC_STYLE_4_BORDER;
		m_round = GSTATIC_STYLE_4_ROUND;
		m_margin = GSTATIC_STYLE_4_MARGIN;
		m_padding = GSTATIC_STYLE_4_PADDING;
		m_lable_height = GSTATIC_STYLE_4_LABEL_HEIGHT;
		m_title_height = m_gstaticInfo->titleInfo.titleName==_T("--")?0:GSTATIC_STYLE_4_TITLE_HEIGHT;
		m_lable_left_with = LABEL_4_LEFT_WITH;    
		m_lable_right_with = LABEL_4_RIGHT_WITH;   
		m_lable_mid_with = LABEL_4_MID_WITH;     

		m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;

		dwInfoFontSize=TVM_PAS_BASE_INFO_FONT_SIZE;
		_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);

		dwTitleFontSize=TVM_PAS_BASE_TITLE_FONT_SIZE;
		_tcscpy_s(tcTitleFontName,32,TVM_PAS_BASE_TITLE_FONT_NAME );

		dwInfoCharSet=CHARSET;
		dwTitleCharSet=CHARSET;

		for (int i = 0; i < MAX_ROW; i++) {
			for (int j = 0; j< MAX_COL; j++) {
				m_colorArray[i][j] = TVM_PAS_BASE_INFO_FONT_COLOR;
			}
		}
		break;
	case IMAGE_CENTER_STYLE:
		{
			CString bmppath;
			CString apppath;
			GetAppPath(apppath);
			// ��ά��ɨ����ʾ��̬ͼƬ
			SetTimer(1001,1000,NULL);
			// ֧����ʽ��ʾͼƬ
			bmppath = apppath + _T("\\res\\TVM_ErWeiMa_PayMent_tips.bmp");
			m_pPaymentTipsImg = new CImage;
			m_pPaymentTipsImg->Load(bmppath);
			// ����ά��ͼƬ�Ƿ���ڣ�������ɾ��
			if(m_pErweimaImg){
				delete m_pErweimaImg;
				m_pErweimaImg = NULL;
			}
			bmppath = apppath + _T("QR_ORDER_INFO_0.bmp");
			if(ComCheckFileExist(bmppath)){
				DeleteFile(bmppath);
			}
		}
	case ISSUE_INFO_STYLE:
		m_width = GSTATIC_STYLE_5_WIDTH;
		m_border = GSTATIC_STYLE_5_BORDER;
		m_round = GSTATIC_STYLE_5_ROUND;
		m_margin = GSTATIC_STYLE_5_MARGIN;
		m_padding = GSTATIC_STYLE_5_PADDING;
		m_lable_height = GSTATIC_STYLE_5_LABEL_HEIGHT;
		m_title_height = m_gstaticInfo->titleInfo.titleName==_T("--")?0:GSTATIC_STYLE_5_TITLE_HEIGHT;
		m_lable_left_with = LABEL_5_LEFT_WITH;    
		m_lable_right_with = LABEL_5_RIGHT_WITH;   
		m_lable_mid_with = LABEL_5_MID_WITH;     

		m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;

		dwInfoFontSize=TVM_PAS_BASE_INFO_FONT_SIZE;
		_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);
		dwInfoFontSizeEx = TVM_PAS_BASE_INFO_FONT_SIZE+3;

		dwTitleFontSize=TVM_PAS_BASE_TITLE_FONT_SIZE;
		_tcscpy_s(tcTitleFontName,32,TVM_PAS_BASE_TITLE_FONT_NAME );

		dwInfoCharSet=CHARSET;
		dwTitleCharSet=CHARSET;

		m_colorArray[0][0] = RGB(0,0,0);
		m_colorArray[2][0] = RGB(0,0,0);
		m_colorArray[4][0] = RGB(0,0,0);
		m_colorArray[6][0] = RGB(0,0,0);
		m_colorArray[8][0] = RGB(0,0,0);
		m_colorArray[10][0] = RGB(255,128,255);
		m_colorArray[12][0] = RGB(255,0,0);
		m_colorArray[14][0] = RGB(255,0,0);

		m_colorArray[0][1] = RGB(255,0,0);
		m_colorArray[2][1] = RGB(0,0,255);
		m_colorArray[4][1] = RGB(0,0,255);
		m_colorArray[6][1] = RGB(255,0,0);
		m_colorArray[8][1] = RGB(255,0,0);
		m_colorArray[10][1] = RGB(255,0,0);
		m_colorArray[12][1] = RGB(255,0,0);
		m_colorArray[14][1] = RGB(255,0,0);

		//for (int i = 0; i < MAX_ROW; i++) {
		//	for (int j = 0; j< MAX_COL; j++) 
		//	{
		//		m_colorArray[i][1] = TVM_PAS_BASE_INFO_FONT_COLOR;
		//	}
		//}

		break;
	case CHARGE_INFO_STYLE:
		m_width = GSTATIC_STYLE_5_WIDTH;
		m_border = GSTATIC_STYLE_5_BORDER;
		m_round = GSTATIC_STYLE_5_ROUND;
		m_margin = GSTATIC_STYLE_5_MARGIN;
		m_padding = GSTATIC_STYLE_5_PADDING;
		m_lable_height = GSTATIC_STYLE_5_LABEL_HEIGHT;
		m_title_height = m_gstaticInfo->titleInfo.titleName==_T("--")?0:GSTATIC_STYLE_5_TITLE_HEIGHT;
		m_lable_left_with = LABEL_5_LEFT_WITH;    
		m_lable_right_with = LABEL_5_RIGHT_WITH;   
		m_lable_mid_with = LABEL_5_MID_WITH;     

		m_Title_Font_Color = TVM_PAS_BASE_TITLE_FONT_COLOR;

		dwInfoFontSize=TVM_PAS_BASE_INFO_FONT_SIZE+10;
		_tcscpy_s(tcInfoFontName,32,TVM_PAS_BASE_INFO_FONT_NAME);
		dwInfoFontSizeEx = TVM_PAS_BASE_INFO_FONT_SIZE+10;

		dwTitleFontSize=TVM_PAS_BASE_TITLE_FONT_SIZE;
		_tcscpy_s(tcTitleFontName,32,TVM_PAS_BASE_TITLE_FONT_NAME );

		dwInfoCharSet=CHARSET;
		dwTitleCharSet=CHARSET;

		m_colorArray[0][0] = RGB(255,0,0);
		m_colorArray[0][1] = RGB(255,0,0);
		m_colorArray[2][0] = RGB(255,0,0);
		m_colorArray[2][1] = RGB(255,0,0);

		break;
	// ά������
	case OPERATOR_STATIC_STYLE:
	default:	
		m_width = GSTATIC_STYLE_2_WIDTH;
		m_border = GSTATIC_STYLE_2_BORDER;
		m_round = GSTATIC_STYLE_2_ROUND;
		m_margin = GSTATIC_STYLE_2_MARGIN;
		m_padding = GSTATIC_STYLE_2_PADDING;
		m_lable_height = GSTATIC_STYLE_2_LABEL_HEIGHT;
		m_title_height = GSTATIC_STYLE_2_TITLE_HEIGHT;
		m_lable_left_with = LABEL_2_LEFT_WITH;    
		m_lable_right_with = LABEL_2_RIGHT_WITH;   
		m_lable_mid_with = LABEL_2_MID_WITH;     

		m_Title_Font_Color = TITLE_FONT_COLOR;

		dwInfoFontSize=NORMAL_FONT_SIZE;
		_tcscpy_s(tcInfoFontName,32,NORMAL_FONT_NAME);
		
		dwTitleFontSize=TITLE_FONT_SIZE;
		_tcscpy_s(tcTitleFontName,32,TITLE_FONT_NAME );

		dwInfoCharSet=OPERATORCHARSET;
		dwTitleCharSet=OPERATORCHARSET;
		
		for (int i = 0; i < MAX_ROW; i++) {
			for (int j = 0; j< MAX_COL; j++) {
				m_colorArray[i][j] = NORMAL_FONT_COLOR;
			}
		}
		break;
	}
	m_font.CreateFont(dwInfoFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			dwInfoCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS, tcInfoFontName);
	m_fontR.CreateFont(dwInfoFontSizeEx, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
		dwInfoCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, tcInfoFontName);

		m_titlefont.CreateFont(dwTitleFontSize, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, 
			dwTitleCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
			DEFAULT_PITCH|FF_SWISS,tcTitleFontName);
}

void CGStatic::OnTimer(UINT nIDEvent)
{
	if(this->IsWindowVisible()){
		CRect imageRect = CRect(784, 100, 784+225, 100+225);
		InvalidateRect(&imageRect);
	}
}
void CGStatic::SetContent(CString szNew)
{
	if ((m_szContent != szNew) && this->IsWindowVisible())
	{
		m_szContent = szNew;
		this->Invalidate();
	}
}