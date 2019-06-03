// BaseDlg.cpp : ʵ���ļ�
#include "StdAfx.h"
#include "LanguageDialog.h"
#include <..\src\mfc\afximpl.h>
#include "UIDefinition.h"
#include "tvmsetting.h"
#include "CommonDefine.h"
#include "ControlManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(LanguageDialog, CDialog)

BEGIN_MESSAGE_MAP(LanguageDialog, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BTN_CHINESE_CLICK(OnChinese)
	ON_BTN_ENGLISH_CLICK(OnEnglish)
	ON_BTN_PORTUGUESE_CLICK(OnPortuguese)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LanguageDialog::LanguageDialog() : CDialog(LanguageDialog::IDD)
{
	m_bkColor = OPERATOR_HEAD_BK_COLOR;
	m_DefautLanguageType = LANGUAGE_ID_UNSPECIFIED;
	
	//������ť
	//m_btnChinese = BuildOperationButton(BTN_CHINESE_RECT,STR_CHINESE,WM_BTN_CHINESE);
	//m_btnEnglish= BuildOperationButton(BTN_ENGLISH_RECT, STR_ENGLISH,WM_BTN_ENGLISH);
	//m_btnPortuguese = BuildOperationButton(BTN_PORTUGUESE_RECT,STR_PORTUGUESE,WM_BTN_PORTUGUESE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LanguageDialog::~LanguageDialog()
{
	delete m_btnChinese;
	m_btnChinese = NULL;

	delete m_btnEnglish;
	m_btnEnglish = NULL;

	delete m_btnPortuguese;
	m_btnPortuguese = NULL;

	KillTimer(TIMER_LAN);
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
int LanguageDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CDialog::OnCreate(lpCreateStruct) == -1) {
		return -1;
	}
	::SetWindowPos(this->m_hWnd,HWND_TOP, 0, 0, CXSCREEN, CYSCREEN, SWP_SHOWWINDOW );
	m_btnChinese->Create();
	m_btnEnglish->Create();
	m_btnPortuguese->Create();	

	// ����ʱ����ʾ��ʱ��
	SetTimer(TIMER_LAN, TIMER_INTERVAL, NULL);

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
void LanguageDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = CDC::FromHandle(dc);

	OnDraw(pDC);
}
//////////////////////////////////////////////////////////////////////////
/**
 @brief      ������ƺ���

 @param      (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void LanguageDialog::OnDraw(CDC* pDC)
{
	m_DefautLanguageType = theMAINTENANCE_INFO.GetLanguageMode();;
	switch(m_DefautLanguageType)
	{
	case LANGUAGE_ID_CHINESE:
		m_btnChinese->SetFocus();
		break;
	case LANGUAGE_ID_ENGLISH:
		m_btnEnglish->SetFocus();
		break;
	case LANGUAGE_ID_PORTUGUESE:
		m_btnPortuguese->SetFocus();
		break;
	default:
		m_btnChinese->SetFocus();
	}
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      �ػ�Dialog����

 @param     (i) CDC* pDC  �豸������

 @retval     ��

 @exception  ��
*/
//////////////////////////////////////////////////////////////////////////
afx_msg BOOL LanguageDialog::OnEraseBkgnd(CDC* pDC)
{
    CBrush bkBrush(m_bkColor);      // ���屳��ɫˢ��
    CRect rect;
    GetClientRect(rect);
    pDC->FillRect(&rect, &bkBrush); // �ñ���ɫ��һ������

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����������ť

@param      (i)CRect rect     ��ť����
@param      (i)CString text   ��ť����
@param      (i)CString msg    ��ť��Ϣ

@retval     CXButton* 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CXButton* LanguageDialog::BuildOperationButton(CRect rect,CString text,CString msg)
{
	UIINFO ui;
	ui.m_pParentWnd = this;
	ui.m_DockStyle = DOCKSTYLE::NONE;
	ui.m_Location= rect.TopLeft();
	ui.m_Size = rect.Size();
	ui.m_Visible = true;
	BTN_INFO btnInfo;
	btnInfo.text = text;
	btnInfo.btnMsg = msg;
	btnInfo.isVisible = true;
	btnInfo.type = BUTTON_TYPE_NORMAL;
	return new CXButton(ui,btnInfo);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  LanguageDialog::OnEnglish(WPARAM wParam,LPARAM lParam)
{
	theMAINTENANCE_INFO.SetLanguageMode(LANGUAGE_ID_ENGLISH);
	this->EndDialog(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  LanguageDialog::OnChinese(WPARAM wParam,LPARAM lParam)
{
	theMAINTENANCE_INFO.SetLanguageMode(LANGUAGE_ID_CHINESE);
	this->EndDialog(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ť��Ϣ

@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     LRESULT  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT  LanguageDialog::OnPortuguese(WPARAM wParam,LPARAM lParam)
{	
	theMAINTENANCE_INFO.SetLanguageMode(LANGUAGE_ID_PORTUGUESE);
	this->EndDialog(0);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ʱ�����

@param      UINT nIDEvent

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void LanguageDialog::OnTimer(UINT nIDEvent) 
{
	this->EndDialog(0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ

@param      (i)MSG* pMsg  ��Ϣ��Ϣ

@retval     BOOL  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BOOL LanguageDialog::PreTranslateMessage(MSG* pMsg)
{
	//����alt+f4
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) {
		return TRUE;
	}

	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN
									|| pMsg->wParam == VK_TAB 
									|| pMsg->wParam == VK_DOWN
									|| pMsg->wParam == VK_UP))
	{
		CWnd* current = this->GetFocus();
		bool isButton = current->IsKindOf(RUNTIME_CLASS(CXButton));
		//��ť�س�
		if(pMsg->wParam == VK_RETURN && isButton) {
			/*CXButton* button = dynamic_cast<CXButton*>(current);zhengxianle:��ֹ��ά������Ӱ��˿�ҵ�����
			button->SetFocus();
			button->SendMessage(WM_XBUTTON_CLICK,NULL,NULL);*/
			return true;
		}
		CWnd* pNextFocusControl = NULL;
		// TAB�����¡��Ǳ����򽹵�س�
		if((pMsg->wParam == VK_RETURN&& !isButton) || pMsg->wParam == VK_TAB || pMsg->wParam == VK_DOWN) {
			pNextFocusControl= theControl_MGR.GetNextCanFocusControl(current);
		}
		if(pMsg->wParam == VK_UP) {
			pNextFocusControl = theControl_MGR.GetPreCanFocusControl(current);
		}
		// ���°�ť����
		if(pNextFocusControl!=NULL) {
			pNextFocusControl->SetFocus();
			pNextFocusControl->Invalidate();
		}
		else {
			current->SetFocus();
			current->Invalidate();
		}
		return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������Ϣ

@param      (i)UINT message   ��ϢID
@param      (i)WPARAM wParam  ��Ϣ��Ϣ
@param      (i)LPARAM lParam  ��Ϣ��Ϣ����ʾ��ť���·��͵ĸ�����Ϣ

@retval     BOOL  0 �ɹ�����0 ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT LanguageDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( message == WM_XGROUP || message == WM_XBUTTON)
	{
		UINT type = message;
		UINT code=wParam;
		if(message == WM_XBUTTON)
		{
			CString btnMsg =CString((LPCTSTR)lParam);
			if(btnMsg == WM_BTN_CHINESE) code = 0;
			if(btnMsg == WM_BTN_ENGLISH) code = 1;
			if(btnMsg == WM_BTN_PORTUGUESE) code = 2;
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
			typedef LRESULT (AFX_MSG_CALL LanguageDialog::*PMENUCLICKFUNCTION)(WPARAM, LPARAM);
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


