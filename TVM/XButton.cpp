// XButton.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "XButton.h"
#include "resource.h"
#include <Strsafe.h>
#include "InnerException.h"
#include "exceptionmgr.h"
#include <mmsystem.h>
#include "BitmapManager.h"
#include "shortcutkeymanager.h"
#include "controlmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXButton, CButton)

BEGIN_MESSAGE_MAP(CXButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_XBUTTON_CLICK, OnXButtonClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CXButton::CXButton(UIINFO& uIInfo,BTN_INFO & buttonInfo)
{
	m_pNormal=NULL;
	m_pPressed=NULL;
	m_pHover=NULL;
	m_pDisabled=NULL;
	m_UIInfo = uIInfo;
	m_ButtonInfo = buttonInfo;
	if (buttonInfo.type == BUTTON_TYPE_SERVICE /*|| buttonInfo.type == BUTTON_TYPE_LANGUAGE*/)
	{
		m_UIInfo.m_Font = GUEST_MENUE_BTN_FONT;
		m_TextInfo.color = GUEST_MENUE_BTN_FONT_COLOR/*WHITE*/;
	}
	else
	{
		m_UIInfo.m_Font = GUEST_FUNC_BTN_FONT;
		m_TextInfo.color = GUEST_FUNC_BTN_FONT_COLOR;
	}
	Initialize(uIInfo,buttonInfo);
}

CXButton::CXButton(UIINFO& uIInfo,BTN_INFO& buttonInfo,TEXT_INFO& textinfo)
{
	m_pNormal=NULL;
	m_pPressed=NULL;
	m_pHover=NULL;
	m_pDisabled=NULL;
	//uIInfo.m_Font = theGUI_INFO.GetBtnFont();
	m_UIInfo = uIInfo;
	m_ButtonInfo = buttonInfo;

	m_TextInfo = textinfo;
	m_UIInfo.m_Font.lfHeight = -textinfo.size;
	m_UIInfo.m_Font.lfWeight = textinfo.bold ? FW_BOLD : FW_NORMAL;
	_tcsncpy(m_UIInfo.m_Font.lfFaceName, textinfo.face.GetBuffer(), LF_FACESIZE);
	textinfo.face.ReleaseBuffer();

	if (buttonInfo.type == BUTTON_TYPE_SERVICE /*|| buttonInfo.type == BUTTON_TYPE_LANGUAGE*/){
		m_UIInfo.m_Font = GUEST_MENUE_BTN_FONT;
		m_TextInfo.color = GUEST_MENUE_BTN_FONT_COLOR;
	} 
	else if(buttonInfo.type == BUTTON_TYPE_LINE_AREA){
		// Ĭ�ϸ�ֵ
	}
	else{
		m_UIInfo.m_Font = GUEST_FUNC_BTN_FONT;
		m_TextInfo.color = GUEST_FUNC_BTN_FONT_COLOR;
	}

	Initialize(uIInfo,buttonInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��̬��ť�Ĺ��캯��

@param      (i)CPoint        point      ��ť����
@param      (i)CWnd*         pParentWnd ������ָ��
@param      (i)UINT          ID         ��ť��
@param      (i)CString       msg        ��ť������Ϣ
@param      (i)CString       fontFace   ��������
@param      (i)UINT          fontSize   ����ߴ�
@param      (i)CString       text1      ��ť�ĵ�һ������   
@param      (i)CString       text2      ��ť�ĵڶ�������
@param      (i)BUTTON_TYPE   type       ��ť����
@param      (i)bool          isSelected ��ťѡ�б�ʶ
@param      (i)bool          isEnable   ��ťʹ�ñ�ʶ
@param      (i)bool          isVisible  ��ť�ɼ���ʶ

@retval     ��

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::Initialize(UIINFO& uIInfo,BTN_INFO& buttonInfo)
{
	m_isDown = false;           // ��ʼʱ������Ϊ����״̬
	m_isProcessing = false;     // ����Click�¼���Ӧ��
	switch (buttonInfo.type) {
	case BUTTON_TYPE::BUTTON_TYPE_NORMAL:       // ��������
		m_pNormal= &theBitmap_MGR.m_pNormalNormal;
		m_pPressed = &theBitmap_MGR.m_pNormalPressed;
		m_pHover= &theBitmap_MGR.m_pNormalHover;
		m_pDisabled= &theBitmap_MGR.m_pNormalDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_NOSELECT:     // ��ѡ��״̬�İ�ť
		m_pNormal= &theBitmap_MGR.m_pOkNormal;
		m_pPressed = &theBitmap_MGR.m_pOkPressed;
		m_pHover= &theBitmap_MGR.m_pOkNormal;
		m_pDisabled= &theBitmap_MGR.m_pOkHover;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_OK:          // �_�J��ť
		m_pNormal= &theBitmap_MGR.m_pOkNormal;
		m_pPressed = &theBitmap_MGR.m_pOkPressed;
		m_pHover= &theBitmap_MGR.m_pOkHover;
		m_pDisabled= &theBitmap_MGR.m_pOkHover;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_PRINT:     // ��ӡ�x��ť
		m_pNormal= &theBitmap_MGR.m_pOkNormal;
		m_pPressed = &theBitmap_MGR.m_pOkPressed;
		m_pHover= &theBitmap_MGR.m_pOkHover;
		m_pDisabled= &theBitmap_MGR.m_pOkHover;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_SERVICE:       // ����״̬��Ŧ
		m_pNormal= &theBitmap_MGR.m_pServiceNormal;
		m_pPressed = &theBitmap_MGR.m_pServicePressed;
		m_pHover= &theBitmap_MGR.m_pOkHover;
		m_pDisabled= &theBitmap_MGR.m_pOkDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_LANGUAGE:     // ����ѡ��Ŧ
		m_pNormal= &theBitmap_MGR.m_pLanguageNormal;
		m_pPressed = &theBitmap_MGR.m_pLanguagePressed;
		m_pHover= &theBitmap_MGR.m_pLanguageHover;
		m_pDisabled= &theBitmap_MGR.m_pLanguageDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_BUTTENGROUP:       // ��Ŧ��
		m_pNormal= &theBitmap_MGR.m_pGroupNormal;
		m_pPressed = &theBitmap_MGR.m_pGroupPressed;
		m_pHover= &theBitmap_MGR.m_pGroupHover;
		m_pDisabled= &theBitmap_MGR.m_pGroupDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_CANCLE:       // ��Ŧ��
		m_pNormal= &theBitmap_MGR.m_pBackNormal;
		m_pPressed = &theBitmap_MGR.m_pBackPressed;
		m_pHover= &theBitmap_MGR.m_pBackHover;
		m_pDisabled= &theBitmap_MGR.m_pBackDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_LINE_AREA:       // ��վ����Ŧ
		m_pNormal= &theBitmap_MGR.m_pLineAreaNormal;
		m_pPressed = &theBitmap_MGR.m_pLineAreaPressed;
		m_pHover= &theBitmap_MGR.m_pLineAreaHover;
		m_pDisabled= &theBitmap_MGR.m_pLineAreaDisabled;
		break;
		// ����Ϊ����人���棬�����ӵķ��
	case BUTTON_TYPE::BUTTON_TYPE_BUTTONGROUP_BLUE:
		m_pNormal = &theBitmap_MGR.m_pGroupBlueNormal;
		m_pPressed = &theBitmap_MGR.m_pGroupBluePressed;
		m_pHover= &theBitmap_MGR.m_pGroupBlueHover;
		m_pDisabled= &theBitmap_MGR.m_pGroupBlueDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_BUTTONGROUP_BROWN:
		m_pNormal = &theBitmap_MGR.m_pGroupBrownNormal;
		m_pPressed = &theBitmap_MGR.m_pGroupBrownPressed;
		m_pHover= &theBitmap_MGR.m_pGroupBrownHover;
		m_pDisabled= &theBitmap_MGR.m_pGroupBrownDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_BUTTONGROUP_BLUE_SPEC:
		m_pNormal = &theBitmap_MGR.m_pGroupBlueNormal;
		m_pPressed = &theBitmap_MGR.m_pGroupBluePressed;
		m_pHover= &theBitmap_MGR.m_pGroupBlueHover;
		m_pDisabled= &theBitmap_MGR.m_pGroupBlueDisabled;
		break;
	case BUTTON_TYPE::BUTTON_TYPE_BUTTONGROUP_HOT:
		m_pNormal = &theBitmap_MGR.m_pGroupBtnHot;
		m_pPressed = &theBitmap_MGR.m_pGroupBtnHot;
		m_pHover= &theBitmap_MGR.m_pGroupBtnHot;
		m_pDisabled= &theBitmap_MGR.m_pGroupBtnHot;
		break;
	}

	if(m_UIInfo.m_Size == CSize(0,0))//Ĭ��ֵ��ȡͼƬ��С
	{
		m_UIInfo.m_Size.cx =  m_pNormal->GetWidth();
		m_UIInfo.m_Size.cy = m_pNormal->GetHeight();
	}
	m_Style = WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW | BS_BITMAP|WS_TABSTOP;
	if(!m_UIInfo.m_Enabled){
		m_Style|=WS_DISABLED;
	}
	if(m_UIInfo.m_Visible)
	{
		m_Style|=WS_VISIBLE;
	}
	if(m_ButtonInfo.shortCutKey!=0)
	{
		theShortCutKey_MGR.RegisterShortCutKey(m_ButtonInfo.shortCutKey,this);
	}
	m_Bag = new CBag();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CXButton::~CXButton()
{
	delete m_Bag;
	m_Bag = NULL;
	if(m_ButtonInfo.shortCutKey!=0)
	{
		theShortCutKey_MGR.UnRegisterShortCutKey(this);
	}
	theControl_MGR.UnRegisterControl(this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CXButton::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)	
{
	return CButton::Create(lpszCaption,dwStyle,rect,pParentWnd,nID);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
BOOL CXButton::Create()
{
	CalculateDrawRect(m_UIInfo.m_pParentWnd,m_UIInfo.m_DockStyle,m_UIInfo.m_Location);

	return CButton::Create(_T(""), m_Style , GetFrameRect(), m_UIInfo.m_pParentWnd, m_ButtonInfo.btnID);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
UIINFO& CXButton::GetUIInfo()
{
	return m_UIInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetUIInfo(UIINFO& uiInfo)
{
	m_UIInfo = uiInfo;
	if(!IsWindow(m_hWnd))
	{
		return;
	}
	this->DestroyWindow();
	this->Create();
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ư�ť

@param      (i)LPDRAWITEMSTRUCT lpDrawItemStruct

@retval     ��

@exception  �� 
*/
//////////////////////////////////////////////////////////////////////////

void CXButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	try {
		CRect rect =  GetContentRect();
		CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		if(NULL!=pDC)
		{
			int nSaveDC = pDC->SaveDC();
			UINT state = lpDrawItemStruct->itemState;
			bool isFocus = GetFocus() == this;

			CImage* pCurrent= NULL;
			if (m_UIInfo.m_pBackgroundImage){
				pCurrent = m_UIInfo.m_pBackgroundImage;
			}
			else{
				if (_tl(TXT_ISSUE_HOME)==m_ButtonInfo.text){
					pCurrent = &theBitmap_MGR.m_pGroupBtnOrange;
				}
				else if (_tl(TXT_PAGE_UP)==m_ButtonInfo.text && BUTTON_TYPE_BUTTONGROUP_BLUE_SPEC==m_ButtonInfo.type){
					pCurrent = &theBitmap_MGR.m_pGroupBtnYellow;
				}
				else if (_tl(TXT_PAGE_DOWN)==m_ButtonInfo.text && BUTTON_TYPE_BUTTONGROUP_BLUE_SPEC==m_ButtonInfo.type){
					pCurrent = &theBitmap_MGR.m_pGroupBtnYellow;
				}
				else if (_tl(TXT_ALL_LINES)==m_ButtonInfo.text){
					pCurrent = &theBitmap_MGR.m_pGroupBtnOrange;
				}
				else if (_tl(TXT_ID_LANGUAGE_SWITCH)==m_ButtonInfo.text){
					pCurrent = &theBitmap_MGR.m_pGroupBtnBlue;
				}
				// �������ȵ㳵վͼ������⴦��
				else if(BUTTON_TYPE_BUTTONGROUP_HOT==m_ButtonInfo.type){
					if(theBitmap_MGR.map_HotStation.count(m_ButtonInfo.text)>0){
						pCurrent = theBitmap_MGR.map_HotStation[m_ButtonInfo.text];
					}				
				}		
				else if(BUTTON_TYPE_BLUE==m_ButtonInfo.type){
					pCurrent = &theBitmap_MGR.m_pGroupBlueNormal;
				}
				else if(BUTTON_TYPE_CLOSE==m_ButtonInfo.type){
					pCurrent = &theBitmap_MGR.m_pGroupBtnYellow;
				}
				else{
					if (m_isDown) {
						pCurrent= m_pPressed;
						m_isDown = false;
					}
					else if (m_ButtonInfo.isSelected) {
						pCurrent= m_pPressed;
					}
					else if (!m_ButtonInfo.isEnable){
						pCurrent= m_pDisabled;
					}
					else if(isFocus)
					{
						pCurrent= m_pHover;
					}
					else{
						pCurrent= m_pNormal;
					}
				}
			}
			if(NULL!=pCurrent)
			{
				bool bNull=pCurrent->IsNull();
				if(!bNull)
				{
					pCurrent->Draw(pDC->m_hDC,rect);
				}
			}

			//��ʾ��ť���ı�
			//CDC* parentCDC = CDC::FromHandle(::GetDC(::GetParent(m_hWnd)));
			/*CRect textrect;
			GetWindowRect(&textrect);
			GetParent()->ScreenToClient(&textrect);
			textrect.left += m_TextInfo.offset.x;
			textrect.top += m_TextInfo.offset.y;
			textrect.right += m_TextInfo.offset.x;
			textrect.bottom += m_TextInfo.offset.y;
			ShowButtonText(parentCDC, textrect);*/
			if(BUTTON_TYPE_BUTTONGROUP_HOT!=m_ButtonInfo.type){
				ShowButtonText(pDC, rect);
			}

			pDC->RestoreDC(nSaveDC);
		}
		return;
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	m_isProcessing = false;     // Click�¼���Ӧ����
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���Owner Draw����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_isProcessing)
	{
		m_isDown = true;
		Invalidate(NULL);
		UpdateWindow();
		PostMessage(WM_XBUTTON_CLICK, NULL, NULL);
	}
	m_isProcessing = true;
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	//if(!m_isProcessing)
	//{
	//	m_isDown = true;
	//	Invalidate(NULL);
	//	UpdateWindow();
	//	PostMessage(WM_XBUTTON_CLICK, NULL, NULL);
	//}
	//m_isProcessing = true;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Childview������ɫ(��ֹ���Ƶ�ɫ)

@param      (i)CDC* pDC

@retval     BOOL    TRUE:��ֹ���Ƶ�ɫ; FALSE:������Ƶ�ɫ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL CXButton::OnEraseBkgnd(CDC* pDC)
{
	// ��ֹ���Ƶ�ɫ
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ʾ��ť����

@param     (i)CDC*    pDC      �豸ָ��
@param     (i)CRECT   rect     �Ի�����Ϣ

@retval     ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::ShowButtonText(CDC* pDC, CRect rect)
{
	//COLORREF clf_btn_font_color;
	//if ( m_ButtonInfo.type == BUTTON_TYPE_LANGUAGE || m_ButtonInfo.type == BUTTON_TYPE_SERVICE )
	//{
	//	// �˵��ఴť
	//	clf_btn_font_color = GUEST_MENUE_BTN_FONT_COLOR;
	//}
	//else
	//	clf_btn_font_color = GUEST_FUNC_BTN_FONT_COLOR;
	if(m_ButtonInfo.text != STR_PORTUGUESE)
		FormatBtnName();
	CStringArray arrText;
	CString splite = _T("^");
	GetSplitStrArray(m_ButtonInfo.text,splite,arrText);
	CString text1 = _T("");
	CString text2 = _T("");
	CString text3 = _T("");

	if(arrText.GetSize()>0)
	{
		text1 = arrText.ElementAt(0);
	}
	if(arrText.GetSize()>1)
	{
		text2 = arrText.ElementAt(1);
		if (!(text1.ReverseFind(' ')==text1.GetLength()-1||text2.Find(_T(' '))==0))
		{
			text1+=_T("-");
		}
	}
	if(arrText.GetSize()>2)
	{
		text3 = arrText.ElementAt(2);
		if (!(text2.ReverseFind(_T(' '))==text2.GetLength()-1||text3.Find(_T(' '))==0))
		{
			text2+=_T("-");
		}
	}
	// ��ť����1Ϊ��ʱ����Ϊ��ťû�����֣���̬��ť��Ԥ�����ֲ���ʾ��Ҫ��ʾ�����ֶ����ⲿ����
	if (text1 == _T("")) {
		return;     // ȡ������ť���ֵ���Ϣ
	}
	CFont font;
	font.CreateFontIndirect(&(theGUI_INFO.GetBtnFont()));
	//font.CreateFontIndirect(&(m_UIInfo.m_Font));

	CFont* hOldFont = pDC->SelectObject(&font);
	int nMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(  m_TextInfo.color/*WHITE*/);

	if(text2==_T(""))
	{
		if (text1 != STR_PORTUGUESE)
		{
			CSize szExtent = pDC->GetTextExtent(text1, lstrlen(text1));
			CPoint pt( rect.CenterPoint().x - szExtent.cx / 2, rect.CenterPoint().y - szExtent.cy / 2);
			CPoint ptL( 60, rect.CenterPoint().y - szExtent.cy / 2);

			if (BUTTON_TYPE_BUTTONGROUP_HOT==m_ButtonInfo.type){
				pt = ptL;
			}

			if (m_isDown) {          // ��ťΪ����״̬ 
				pt.Offset(1, 1);
			}

			int nMode = pDC->SetBkMode(TRANSPARENT);

			if (IsEnabled()) {  // ��ťΪʹ��״̬
				pDC->DrawState(pt, szExtent, text1, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
			}
			else {
				pDC->DrawState(pt, szExtent,text1, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
			}
			pDC->SelectObject(hOldFont);

		}
		else
		{
			CFont vertFont;
			//����
			vertFont.CreateFont(35, 0, -900, 900, FW_BOLD,
				0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, _T("Arial"));
			CStringArray* vecText = SplitCString(text1,_T(" "));
			CRect texRect = rect;
			texRect.top += 5;
			texRect.left = rect.Width()/2 - 20;
			CFont *pOldFont = pDC->SelectObject(&vertFont);
			int nWeight = 0;
			int it = 0;
			while(it < vecText->GetSize())
			{
				CString szCnt = vecText->GetAt(it);
				TextOut(pDC->m_hDC,texRect.left+nWeight, texRect.top, szCnt,szCnt.GetLength()); //�ı���ʼλ��
				it++;
				nWeight += 50;
			}
			pDC->SelectObject(pOldFont); 

		}
	}
	else if(text3==_T("")){  // ��ť�ڶ������ֲ�Ϊ��
		CSize szExtent1= pDC->GetTextExtent(text1, lstrlen(text1));
		CSize szExtent2= pDC->GetTextExtent(text2, lstrlen(text2));

		CPoint pt1( rect.CenterPoint().x - szExtent1.cx / 2, rect.CenterPoint().y - szExtent1.cy -1);
		CPoint pt2( rect.CenterPoint().x - szExtent2.cx / 2, rect.CenterPoint().y + 1);

		if (m_isDown) {
			pt1.Offset(1, 1);
			pt2.Offset(1, 1);
		}

		if (IsEnabled()) {
			pDC->DrawState(pt1, szExtent1, text1, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
			pDC->DrawState(pt2, szExtent2, text2, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		}
		else {
			pDC->DrawState(pt1, szExtent1, text1, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
			pDC->DrawState(pt2, szExtent2, text2, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
		pDC->SelectObject(hOldFont);

	}
	else {  // ��ť���������ֲ�Ϊ��

		// ��С�ֺ�
		LOGFONT logfont = m_UIInfo.m_Font;
		logfont.lfHeight -= 5;
		CFont font;
		font.CreateFontIndirect(&(logfont));
		pDC->SelectObject(&font);

		CSize szExtent1= pDC->GetTextExtent(text1, lstrlen(text1));
		CSize szExtent2= pDC->GetTextExtent(text2, lstrlen(text2));
		CSize szExtent3= pDC->GetTextExtent(text3, lstrlen(text3));

		CPoint pt1( rect.CenterPoint().x - szExtent1.cx / 2, rect.CenterPoint().y - szExtent1.cy -szExtent2.cy +2);
		CPoint pt2( rect.CenterPoint().x - szExtent2.cx / 2, rect.CenterPoint().y - szExtent2.cy  +4);
		CPoint pt3( rect.CenterPoint().x - szExtent3.cx / 2, rect.CenterPoint().y + 5);

		if (m_isDown) {
			pt1.Offset(1, 1);
			pt2.Offset(1, 1);
			pt3.Offset(1, 1);
		}

		if (IsEnabled()) {
			pDC->DrawState(pt1, szExtent1, text1, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
			pDC->DrawState(pt2, szExtent2, text2, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
			pDC->DrawState(pt3, szExtent3, text3, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		}
		else {
			pDC->DrawState(pt1, szExtent1, text1, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
			pDC->DrawState(pt2, szExtent2, text2, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
			pDC->DrawState(pt3, szExtent3, text3, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
		pDC->SelectObject(hOldFont);

	}
	pDC->SetBkMode(nMode);
	font.DeleteObject();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ���ð�ť�Ƿ�ѡ��

@param     (i)bool isSelected   true:ѡ��;  false:δѡ��

@retval     ��    

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetSelect(bool isSelected) 
{
	m_ButtonInfo.isSelected = isSelected;
	if(IsWindow(m_hWnd))
	{
		this->Invalidate();
		this->UpdateWindow();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ť�Ƿ�ѡ��

@param      ��

@retval     bool isSelected   true:ѡ��;  false:δѡ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CXButton::IsSelected() 
{
	return m_ButtonInfo.isSelected;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ð�ť�Ƿ����

@param      (i)bool    true:����, false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetVisible(bool isVisible) 
{
	m_ButtonInfo.isVisible = isVisible;
	if(IsWindow(m_hWnd))
	{
		ShowWindow(isVisible ? SW_SHOW : SW_HIDE);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ť�Ƿ����

@param      ��

@retval     bool    true:����, false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CXButton::IsVisible() 
{
	if(!IsWindow(m_hWnd))
	{
		return m_ButtonInfo.isVisible;
	}
	return IsWindowVisible();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ð�ť�Ƿ����

@param      (i)bool    true:����, false:������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetEnabled(bool isEnabled)
{
	m_ButtonInfo.isEnable = isEnabled;
	if(IsWindow(m_hWnd))
	{
		EnableWindow(isEnabled ? TRUE : FALSE);
		Invalidate();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ť�Ƿ����

@param      ��

@retval     bool    true:����, false:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CXButton::IsEnabled()
{
	if(!IsWindow(m_hWnd))
	{
		return m_ButtonInfo.isEnable;
	}
	return IsWindowEnabled();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetShortCutKey(UINT shortCutKey)
{
	if(m_ButtonInfo.shortCutKey!=0)
	{
		theShortCutKey_MGR.UnRegisterShortCutKey(this);
	}
	m_ButtonInfo.shortCutKey = shortCutKey;
	theShortCutKey_MGR.RegisterShortCutKey(shortCutKey,this);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
UINT CXButton::GetShortCutKey()
{
	return m_ButtonInfo.shortCutKey;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ťID

@param      ��

@retval     UINT    ��ťID

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CXButton::GetID() 
{
	return m_ButtonInfo.btnID;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetID(UINT ID)
{
	m_ButtonInfo.btnID = ID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ð�ť������Ϣ

@param      (i)CString  ��ť������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetMsg(CString msg) 
{
	m_ButtonInfo.btnMsg = msg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ť������Ϣ

@param      ��

@retval     CString    ��ť������Ϣ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CXButton::GetMsg() 
{
	return m_ButtonInfo.btnMsg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ð�ť����

@param      (i)CString text1    ��һ������
@param      (i)CString text2    �ڶ�������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::SetText(CString text) 
{
	m_ButtonInfo.text = text;
	if(IsWindow(m_hWnd))
	{
		InvalidateRect(NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ť����

@param      (i)int index    �кţ�1��2��

@retval     ָ��������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CString CXButton::GetText() 
{
	return m_ButtonInfo.text;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CBag* CXButton::GetBag()
{
	return m_Bag;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������ť��Ч����

@param      (i)LPCREATESTRUCT lpCreateStruct

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CXButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CButton::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	theControl_MGR.RegisterControl(this);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void	CXButton::OnDestroy()
{
	theControl_MGR.UnRegisterControl(this);
	__super::OnDestroy();
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ť�������¼�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CXButton::OnXButtonClick(WPARAM wParam, LPARAM lParam)
{
	// ��תѡ��״̬, ˢ����Ļ
	bool cancel = false;
	Clicking.Invoke(this,&cancel);
	if(cancel)
	{
		m_isProcessing = false;
		InvalidateRect(NULL);
		return FALSE;
	}
	ProcessButtonEvent();
	m_isProcessing = false;     // Click�¼���Ӧ����
	Clicked.Invoke(this);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �����ť�������¼��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::ProcessButtonEvent()
{
	try {
		m_ButtonInfo.isSelected = !m_ButtonInfo.isSelected;
		InvalidateRect(NULL);
		// ��������
		//if (IsEnableSound()) 
		if(theTVM_INFO.IsEnableClickSound())
		{
			PlayClickSound();
		}
		CWnd* pParent = GetParent();
		if (pParent!=NULL && pParent->IsKindOf(RUNTIME_CLASS(CBaseDlg))) {
			((CBaseDlg*)pParent)->CallWndProc(WM_XBUTTON, m_ButtonInfo.btnID, (LPARAM)(LPCTSTR)m_ButtonInfo.btnMsg);
		}
		else if(pParent->IsKindOf(RUNTIME_CLASS(CPanel)))
		{
			((CPanel*)pParent)->CallWndProc(WM_XBUTTON, m_ButtonInfo.btnID, (LPARAM)(LPCTSTR)m_ButtonInfo.btnMsg);
		}
		else
		{
			pParent->SendMessage(WM_XBUTTON, m_ButtonInfo.btnID, (LPARAM)(LPCTSTR)m_ButtonInfo.btnMsg);
		}
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �����Ƿ�������������ť����

@param      bool enable

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::EnableSound(bool enable)
{
	s_enableSound = enable;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �Ƿ�������������ť����

@param      ��

@retval     bool: true ������������ť����; false ��������������ť����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CXButton::IsEnableSound()
{
	return s_enableSound;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ť����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::PlayClickSound()
{
	// ��������
	sndPlaySound(_T(".\\res\\click.wav"), SND_ASYNC);

	// ˯��100���룬����ʾ���»���
	Sleep(100);

	EnableSound(false);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CRect CXButton::GetFrameRect()
{
	return CRect(m_UIInfo.m_Location,m_UIInfo.m_Size);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CXButton::CalculateDrawRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint location)
{
	if(m_UIInfo.m_DockStyle == DOCKSTYLE::NONE)
	{
		return;
	}
	CRect parentRectSrc;
	CRect parentContentRect;
	if(m_UIInfo.m_pParentWnd->IsKindOf(RUNTIME_CLASS(CPanel)))
	{
		CPanel* panel = reinterpret_cast<CPanel*>(m_UIInfo.m_pParentWnd);
		parentContentRect = panel->GetContentRect();
		parentContentRect = CRect(CPoint(parentContentRect.left,parentContentRect.top),CSize(button_size));
	}
	else{
		m_UIInfo.m_pParentWnd->GetWindowRect(&parentRectSrc);
		parentContentRect.left = 0;
		parentContentRect.top = 0;
		parentContentRect.right = parentRectSrc.Width();
		parentContentRect.bottom = parentRectSrc.Height();
	}
	m_UIInfo.m_Location = location;
	switch(m_UIInfo.m_DockStyle)
	{
	case LEFT:
		{
			m_UIInfo.m_Location.x = parentContentRect.left + location.x;
			m_UIInfo.m_Location.y = parentContentRect.top;
			m_UIInfo.m_Size.cy = parentContentRect.Height();
			break;
		}
	case TOP:
		{
			m_UIInfo.m_Location.y = parentContentRect.top + location.y;
			m_UIInfo.m_Location.x = parentContentRect.left;
			m_UIInfo.m_Size.cx = parentContentRect.Width();
			break;
		}
	case RIGHT:
		{
			m_UIInfo.m_Location.x = parentContentRect.right - location.x - m_UIInfo.m_Size.cx;
			m_UIInfo.m_Location.y = parentContentRect.top;
			m_UIInfo.m_Size.cy = parentContentRect.Height();
			break;
		}
	case BOTTOM:
		{
			m_UIInfo.m_Location.y = parentContentRect.bottom - location.y - m_UIInfo.m_Size.cy;
			m_UIInfo.m_Location.x = parentContentRect.left;
			m_UIInfo.m_Size.cx = parentContentRect.Width();
			break;
		}
	case FILL:
		{
			m_UIInfo.m_Location = CPoint(parentContentRect.left ,parentContentRect.top);
			m_UIInfo.m_Size = CSize(parentContentRect.Width(),parentContentRect.Height());
			break;
		}
	case LEFTRIGHT:
		{
			//m_UIInfo.m_Location = CPoint((left_number_size.cx - parentContentRect.Size().cx)/2 ,parentContentRect.top);
			m_UIInfo.m_Size = CSize(parentContentRect.Width(),parentContentRect.Height());
			break;
		}
	case TOPBOTTOM:
		{
			//m_UIInfo.m_Location = CPoint(parentContentRect.left+100 ,(top_line_size.cy - parentContentRect.Size().cy)/2);
			m_UIInfo.m_Size = CSize(parentContentRect.Width(),parentContentRect.Height());
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief    ���ݿؼ�ʵ�ʻ������ݵ����򣨳����ⲹ�ף��ڲ��ף����߿�  

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CRect CXButton::GetContentRect()
{
	CRect contentRect = CRect(CPoint(0,0),m_UIInfo.m_Size);

	contentRect.left	+= (m_UIInfo.m_Margins.cxLeftWidth + m_UIInfo.m_Paddings.cxLeftWidth + m_UIInfo.m_BorderWidth);  
	contentRect.right	-= (m_UIInfo.m_Margins.cxRightWidth + m_UIInfo.m_Paddings.cxRightWidth + m_UIInfo.m_BorderWidth);  
	contentRect.top += (m_UIInfo.m_Margins.cyTopHeight + m_UIInfo.m_Paddings.cyTopHeight + m_UIInfo.m_BorderWidth);
	contentRect.bottom -= (m_UIInfo.m_Margins.cyBottomHeight + m_UIInfo.m_Paddings.cyBottomHeight + m_UIInfo.m_BorderWidth);
	return contentRect;
}

bool CXButton::s_enableSound = false;              // �Ƿ�������������ť����
delegate<void(CXButton*,bool*)> CXButton::Clicking;
delegate<void(CXButton*)> CXButton::Clicked;


void CXButton::FormatBtnName()
{
	CString btn_name = m_ButtonInfo.text;
	TCHAR splite = '^';

	btn_name.Remove(splite);
	int nameLen = btn_name.GetLength();
	int maxSizeOfline = 0;
	if(m_UIInfo.m_Size.cx-DEFAULT_SIZE_BUTTON.cx>0){
		int nMax = m_UIInfo.m_Size.cx / DEFAULT_SIZE_BUTTON.cx;
		maxSizeOfline = nMax == 1 ? 14 : 8 * nMax;
	}
	else{
		maxSizeOfline = 10;	
	}

	if (nameLen<GetAactualCharacterWidth(btn_name)){
		maxSizeOfline = maxSizeOfline/2; 
	}

	int countOfline = (nameLen/maxSizeOfline)+1;
	for (int i=1; i<countOfline; i++){
		btn_name.Insert((nameLen/countOfline)*i,splite);
	}
	m_ButtonInfo.text = btn_name;
}
