// BaseDlg.cpp : 实现文件
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
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LanguageDialog::LanguageDialog() : CDialog(LanguageDialog::IDD)
{
	m_bkColor = OPERATOR_HEAD_BK_COLOR;
	m_DefautLanguageType = LANGUAGE_ID_UNSPECIFIED;
	
	//操作按钮
	//m_btnChinese = BuildOperationButton(BTN_CHINESE_RECT,STR_CHINESE,WM_BTN_CHINESE);
	//m_btnEnglish= BuildOperationButton(BTN_ENGLISH_RECT, STR_ENGLISH,WM_BTN_ENGLISH);
	//m_btnPortuguese = BuildOperationButton(BTN_PORTUGUESE_RECT,STR_PORTUGUESE,WM_BTN_PORTUGUESE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
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
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  无
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

	// 启动时钟显示计时器
	SetTimer(TIMER_LAN, TIMER_INTERVAL, NULL);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      画面绘制函数

 @param      (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
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
 @brief      画面绘制函数

 @param      (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
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
 @brief      重画Dialog背景

 @param     (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
afx_msg BOOL LanguageDialog::OnEraseBkgnd(CDC* pDC)
{
    CBrush bkBrush(m_bkColor);      // 定义背景色刷子
    CRect rect;
    GetClientRect(rect);
    pDC->FillRect(&rect, &bkBrush); // 用背景色画一个矩形

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      创建操作按钮

@param      (i)CRect rect     按钮区域
@param      (i)CString text   按钮标题
@param      (i)CString msg    按钮消息

@retval     CXButton* 

@exception  无
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
@brief      处理按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  0 成功，非0 失败

@exception  无
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
@brief      处理按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  0 成功，非0 失败

@exception  无
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
@brief      处理按钮消息

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  0 成功，非0 失败

@exception  无
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
@brief      时间控制

@param      UINT nIDEvent

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void LanguageDialog::OnTimer(UINT nIDEvent) 
{
	this->EndDialog(0);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理消息

@param      (i)MSG* pMsg  消息信息

@retval     BOOL  0 成功，非0 失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BOOL LanguageDialog::PreTranslateMessage(MSG* pMsg)
{
	//屏蔽alt+f4
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
		//按钮回车
		if(pMsg->wParam == VK_RETURN && isButton) {
			/*CXButton* button = dynamic_cast<CXButton*>(current);zhengxianle:防止后维护键盘影响乘客业务操作
			button->SetFocus();
			button->SendMessage(WM_XBUTTON_CLICK,NULL,NULL);*/
			return true;
		}
		CWnd* pNextFocusControl = NULL;
		// TAB、向下、非本区域焦点回车
		if((pMsg->wParam == VK_RETURN&& !isButton) || pMsg->wParam == VK_TAB || pMsg->wParam == VK_DOWN) {
			pNextFocusControl= theControl_MGR.GetNextCanFocusControl(current);
		}
		if(pMsg->wParam == VK_UP) {
			pNextFocusControl = theControl_MGR.GetPreCanFocusControl(current);
		}
		// 更新按钮焦点
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
@brief      处理消息

@param      (i)UINT message   消息ID
@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     BOOL  0 成功，非0 失败

@exception  无
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


