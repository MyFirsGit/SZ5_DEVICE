// BaseDlg.cpp : ʵ���ļ�
#include "stdafx.h"
#include "TVM.h"
#include "BaseDlg.h"
#include "SYSInfo.h"
#include <Strsafe.h>
#include <..\src\mfc\afximpl.h>
#include "SysException.h"
#include "messageiddefinition.h"
#include "XButton.h"
#include "controlmanager.h"
#include "shortcutkeymanager.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




IMPLEMENT_DYNAMIC(CBaseDlg, CDialog)

BEGIN_MESSAGE_MAP(CBaseDlg, CDialog)
    ON_WM_CREATE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_NOTIFY_DIALOG_UPDATE,OnServiceDataModelChanged)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)UINT ID              �Ի���ID
@param      (i)CWnd* pParent        ������
@param      (i)CRect position       λ�ô�С
@param      (i)COLORREF bkColor     ����ɫ
@param      (i)CService* pService   �Ի���ʹ�õ�Service
@param      (i)bool isOfficeDlg     վԱ�ͳ˿�Dialog��־��true��վԱDialog��false���˿�Dialog

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBaseDlg::CBaseDlg(UINT ID, CWnd* pParent, CRect position, COLORREF bkColor, CService* pService, bool isOfficeDlg)
    :CDialogEx(ID, pParent)
    , m_ID(ID)
    , m_rectDlg(position)
    , m_bkColor(bkColor)
    , m_pService(pService)
    , m_isOfficeDlg(isOfficeDlg)
{
	theAPP_SESSION.ClientLanguageModeChanged.AddHandler(this,&CBaseDlg::OnClientLanguageModeChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CBaseDlg::~CBaseDlg()
{
	theAPP_SESSION.ClientLanguageModeChanged.RemoveHandler(this,&CBaseDlg::OnClientLanguageModeChanged);
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
int CBaseDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    try {
        if (CDialog::OnCreate(lpCreateStruct) == -1) {
            return -1;
        }

        MoveWindow(m_rectDlg.left, m_rectDlg.top, m_rectDlg.Width(), m_rectDlg.Height(), TRUE);
    }
    catch (CSysException& e) {
        //theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        //theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ӿؼ��ػ�ʱ����

@param      (i)CDC* pDC
@param      (i)CWnd* pWnd
@param      (i)UINT nCtlColor

@retval     HBRUSH 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
HBRUSH CBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  �ڴ˸��� DC ���κ�����
    if (nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    // TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
    return hbr;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      �ػ�Dialog����

 @param     (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
afx_msg BOOL CBaseDlg::OnEraseBkgnd(CDC* pDC)
{
    CBrush bkBrush(m_bkColor);      // ���屳��ɫˢ��
    CRect rect;
    GetClientRect(rect);
    pDC->FillRect(&rect, &bkBrush); // �ñ���ɫ��һ������

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      ������ƺ���

 @param      ��

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    CDC* pDC = CDC::FromHandle(dc);

    OnDraw(pDC);
}

BOOL CBaseDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)//����alt+f4
	{
		return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN 
		&& 
		(
		pMsg->wParam == VK_RETURN
		|| pMsg->wParam == VK_TAB 
		|| pMsg->wParam == VK_DOWN
		||pMsg->wParam == VK_UP
		)
		)
	{
		
		CWnd* current = this->GetFocus();
		bool isButton = current->IsKindOf(RUNTIME_CLASS(CXButton));
		if(pMsg->wParam == VK_RETURN && isButton)//��ť�س�
		{
			/*CXButton* button = dynamic_cast<CXButton*>(current);zhengxianle:��ֹ��ά������Ӱ��˿�ҵ�����
			button->SetFocus();
			button->SendMessage(WM_XBUTTON_CLICK,NULL,NULL);
			return true;*/
			return true;
		}
		CWnd* pNextFocusControl = NULL;
		if((pMsg->wParam == VK_RETURN&& !isButton) || pMsg->wParam == VK_TAB || pMsg->wParam == VK_DOWN)
		{
			pNextFocusControl= theControl_MGR.GetNextCanFocusControl(current);
		}
		if(pMsg->wParam == VK_UP)
		{
			pNextFocusControl = theControl_MGR.GetPreCanFocusControl(current);
		}
		if(pNextFocusControl!=NULL)
		{
			pNextFocusControl->SetFocus();
			pNextFocusControl->Invalidate();
		}
		else
		{
			current->SetFocus();
			current->Invalidate();
		}
		return TRUE;
	}
	//if(pMsg->message == WM_KEYDOWN)
	//{
	//	bool success = theShortCutKey_MGR.FireShortCutKey(pMsg->wParam);
	//	if(success)
	//	{
	//		return TRUE;
	//	}
	//}
	return __super::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////
/**
 @brief      ������ƺ���

 @param      (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::OnDraw(CDC* pDC)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���������ݶ������ı�ʱ��Ӧ����

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBaseDlg::OnServiceDataModelChanged(WPARAM,LPARAM)
{
	try{
		UpdateUI();
	}
	catch(CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::UpdateUI()
{

}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �������Է����仯

@param      

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::OnClientLanguageModeChanged(LANGUAGETYPE_T&)
{

}
//////////////////////////////////////////////////////////////////////////
/**
 @brief     ������

 @param     (i)CDC* pDC                 �豸������
 @param     (i)const CRect& rect	    λ�ô�С
 @param     (i)int nBorderWidth 	    �߿��� ��Ĭ��LINE_BORDER��
 @param     (i)COLORREF crBorderColor	�߿���ɫ ��Ĭ��BLACKGRAY��
 @param     (i)COLORREF crFillColor 	�����ɫ ��Ĭ��GRAY��

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::DrawRect(CDC* pDC, const CRect& rect, int nBorderWidth, COLORREF crBorderColor, COLORREF crFillColor)
{
    CPen pen(PS_SOLID, nBorderWidth, crBorderColor);
    CBrush brush(crFillColor);

    pDC->SelectObject(&brush);
    pDC->SelectObject(&pen);

    pDC->Rectangle(&rect);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��Բ�Ǿ���

@param     (i)CDC* pDC                  �豸������
@param     (i)const CRect& rect         λ�ô�С
@param     (i)int nBorderWidth 	        �߿��� ��Ĭ��LINE_BORDER��
@param     (i)COLORREF crBorderColor	�߿���ɫ ��Ĭ��BLACKGRAY��
@param     (i)COLORREF crFillColor 	    �����ɫ ��Ĭ��GRAY��
@param     (i)CPoint point              Բ�Ǵ�С ��Ĭ�ϣ�20, 20����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::DrawRoundRect(CDC* pDC, const CRect& rect, int nBorderWidth, COLORREF crBorderColor, COLORREF crFillColor, CPoint point)
{
    CPen pen(PS_SOLID, nBorderWidth, crBorderColor);
    CBrush brush(crFillColor);

    pDC->SelectObject(&brush);
    pDC->SelectObject(&pen);

    pDC->RoundRect(&rect, point);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ָ��λ����ʾ����

@param     (i)CDC* pDC              �豸������
@param     (i)int x		            ��ʾ����������
@param     (i)int y		            ��ʾ����������
@param     (i)CString sText	        ����
@param     (i)CString sFont  	    ����
@param     (i)int nFontSize  	    ��С
@param     (i)COLORREF crColor 	    ��ɫ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::DrawText(CDC* pDC, int x, int y, CString sText, CString sFont, int nFontSize,int nFontWeight, COLORREF crColor)
{
    //int height = -MulDiv(nFontSize, GetDeviceCaps(*pDC, LOGPIXELSY), 72);

	CFont font;
	font.CreateFont(nFontSize, 0, 0, 0, nFontWeight, FALSE, FALSE, 0, 
		CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, sFont);

    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->SetTextColor(crColor);
    pDC->SetBkMode(TRANSPARENT);

    pDC->TextOut(x, y, sText);

    pDC->SelectObject(pOldFont);
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
void CBaseDlg::DrawText(CDC* pDC, CRect& rect, CString sText, CString sFont, int nFontSize,int nFontWeight, COLORREF crColor, UINT nFormat)
{
    //int height = -MulDiv(nFontSize, GetDeviceCaps(*pDC, LOGPIXELSY), 72);

    CFont font;
    font.CreateFont(nFontSize, 0, 0, 0, nFontWeight, FALSE, FALSE, 0, 
        CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
        DEFAULT_PITCH|FF_SWISS, sFont);

    CFont* pOldFont = pDC->SelectObject(&font);

    pDC->SetTextColor(crColor);
    pDC->SetBkMode(TRANSPARENT);

    pDC->DrawText(sText, rect, nFormat);

    pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief     ��ָ��λ����ʾ����

@param     (i)CDC* pDC              �豸������
@param     (i)CPoint pos		    ��ʾ����������
@param     (i)CString upText	    ��һ������
@param     (i)CString downText	    �ڶ�������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::DrawText(CDC* pDC, CPoint pos, CString upText, CString downText)
{
    //int height = -MulDiv(theTVM_INFO.GetOperateBtnFontSize(), GetDeviceCaps(*pDC, LOGPIXELSY), 72);

    CFont font;
    font.CreateFont(NORMAL_FONT_SIZE, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0,
        CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
        DEFAULT_PITCH | FF_SWISS, NORMAL_FONT_NAME);

    CFont *pOldFont = pDC->SelectObject(&font);

    pDC->SetTextColor(BLACK);
    pDC->SetBkMode(TRANSPARENT);					// ����͸��

    int cx = pos.x;

    // �ж������Ϣ����
    if (downText != "") {  // ��������
        int dist = 4;		// �м��
        CSize szTxtLine1 = pDC->GetTextExtent(upText, lstrlen(upText));
        CSize szTxtLine2 = pDC->GetTextExtent(downText, lstrlen(downText));
        int cy1 = pos.y + NORMAL_BUTTON_HEIGHT / 2 - szTxtLine1.cy - dist / 2;
        int cy2 = pos.y + NORMAL_BUTTON_HEIGHT / 2 + dist / 2;
        pDC->DrawText(upText, CRect(CPoint(cx, cy1), CPoint(pos.x + NORMAL_BUTTON_WIDTH, pos.y + NORMAL_BUTTON_HEIGHT)), DT_WORDBREAK | DT_VCENTER | DT_CENTER);
        pDC->DrawText(downText, CRect(CPoint(cx, cy2), CPoint(pos.x + NORMAL_BUTTON_WIDTH, pos.y + NORMAL_BUTTON_HEIGHT)), DT_WORDBREAK | DT_VCENTER | DT_CENTER);
    }
    else {               	// һ������
        CSize szTxtLine = pDC->GetTextExtent(upText, lstrlen(upText));
        int cy = pos.y + NORMAL_BUTTON_HEIGHT / 2 - szTxtLine.cy / 2;
        pDC->DrawText(upText, CRect(CPoint(cx, cy), CPoint(pos.x + NORMAL_BUTTON_WIDTH, pos.y + NORMAL_BUTTON_HEIGHT)), DT_VCENTER | DT_CENTER);			
    }

    pDC->SelectObject(pOldFont);
}

void CBaseDlg::DrawText(CDC* pDC,CRect& rect,CString sText,LOGFONT logFont,COLORREF crColor ,UINT nFormat)
{
	CFont font;
	font.CreateFontIndirect(&logFont);
	CFont* pOldFont = pDC->SelectObject(&font);

	pDC->SetTextColor(crColor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(sText, rect, nFormat);
	pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡService����

@param      ��

@retval     Service����

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CService* CBaseDlg::GetService()
{
    return m_pService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ظ�Dialog�Ƿ���վԱDialog

@param      ��

@retval     bool    \n
            true��վԱDialog��false���˿�Dialog

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CBaseDlg::IsOfficeDlg()
{
    return m_isOfficeDlg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡDialog ID

@param      ��

@retval     UINT

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CBaseDlg::GetID()
{
    return m_ID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֱ�ӵ��ø�������¼���Ӧ���򣨱ܿ�MFC���쳣����

@param      (i)UINT nMsg
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     LRESULT, The return value depends on the message.

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBaseDlg::CallWndProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    return WindowProc(nMsg, wParam, lParam);
}

LRESULT CBaseDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//��WM_XGROUP��WM_XBUTTON�����ض�����
	//����ON_GROUP_BUTTON_CLICK
	//			  ON_BTN_OK_CLICK ,ON_BTN_OK_BACK_CLICK,ON_BTN_PRINT_CLICK,ON_BTN_RESET_CLICK

	if( message == WM_XGROUP || message == WM_XBUTTON|| message == WM_XLINEGROUP)
	{
		UINT type = message;
		UINT code=wParam;
		if(message == WM_XBUTTON)
		{
			CString btnMsg =CString((LPCTSTR)lParam);
			if(btnMsg == WM_BTN_OK) code = 1;
			if(btnMsg == WM_BTN_BACK) code = 2;
			if(btnMsg == WM_BTN_PRINT) code = 3;
			if(btnMsg == WM_BTN_RESET)	 code = 4;
			if(btnMsg == WM_BTN_NOPRINT) code = 5;
			if(btnMsg == WM_BTN_OkAndCancel1) code = 6;
			if(btnMsg == WM_BTN_OkAndCancel2) code = 7;
			if (btnMsg == WM_BTN_SELECT_LANGUAGE) code = 8;
			if (btnMsg == WM_BTN_CHARGE) code = 9;
			if (btnMsg == WM_BTN_CASH_PAY) code = 10;
			if (btnMsg == WM_BTN_NET_PAY) code = 11;

		}
		const AFX_MSGMAP* pMessageMap;
		const AFX_MSGMAP_ENTRY* lpEntry;
		// look through message map to see if it applies to us
#ifdef _AFXDLL
		for (pMessageMap = GetMessageMap(); pMessageMap->pfnGetBaseMap != NULL;
			pMessageMap = (*pMessageMap->pfnGetBaseMap)())
#else
		for (pMessageMap = GetMessageMap(); pMessageMap != NULL;
			pMessageMap = pMessageMap->pBaseMap)
#endif
		{
			// Note: catches BEGIN_MESSAGE_MAP(CMyClass, CMyClass)!
#ifdef _AFXDLL
			ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
#else
			ASSERT(pMessageMap != pMessageMap->pBaseMap);
#endif
			typedef LRESULT (AFX_MSG_CALL CBaseDlg::*PMENUCLICKFUNCTION)(WPARAM, LPARAM);
			lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries, type, code, 0);
			if(lpEntry!=NULL)
			{
				PMENUCLICKFUNCTION pfn = reinterpret_cast<PMENUCLICKFUNCTION>(lpEntry->pfn);
				return (this->*pfn)(wParam,lParam);
			}
		}
	}
	LRESULT result = __super::WindowProc(message,wParam,lParam);

	return result;
}