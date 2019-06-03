// BaseDlg.cpp : 实现文件
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
@brief      构造函数

@param      (i)UINT ID              对话框ID
@param      (i)CWnd* pParent        父窗口
@param      (i)CRect position       位置大小
@param      (i)COLORREF bkColor     背景色
@param      (i)CService* pService   对话框使用的Service
@param      (i)bool isOfficeDlg     站员和乘客Dialog标志，true：站员Dialog；false：乘客Dialog

@retval     无

@exception  无
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
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CBaseDlg::~CBaseDlg()
{
	theAPP_SESSION.ClientLanguageModeChanged.RemoveHandler(this,&CBaseDlg::OnClientLanguageModeChanged);
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
@brief      子控件重画时调用

@param      (i)CDC* pDC
@param      (i)CWnd* pWnd
@param      (i)UINT nCtlColor

@retval     HBRUSH 

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
HBRUSH CBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何属性
    if (nCtlColor == CTLCOLOR_STATIC) {
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      重画Dialog背景

 @param     (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
afx_msg BOOL CBaseDlg::OnEraseBkgnd(CDC* pDC)
{
    CBrush bkBrush(m_bkColor);      // 定义背景色刷子
    CRect rect;
    GetClientRect(rect);
    pDC->FillRect(&rect, &bkBrush); // 用背景色画一个矩形

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      画面绘制函数

 @param      无

 @retval     无

 @exception  无
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
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)//屏蔽alt+f4
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
		if(pMsg->wParam == VK_RETURN && isButton)//按钮回车
		{
			/*CXButton* button = dynamic_cast<CXButton*>(current);zhengxianle:防止后维护键盘影响乘客业务操作
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
 @brief      画面绘制函数

 @param      (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::OnDraw(CDC* pDC)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务中数据对象发生改变时响应函数

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
@brief      界面语言发生变化

@param      

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CBaseDlg::OnClientLanguageModeChanged(LANGUAGETYPE_T&)
{

}
//////////////////////////////////////////////////////////////////////////
/**
 @brief     画矩形

 @param     (i)CDC* pDC                 设备上下文
 @param     (i)const CRect& rect	    位置大小
 @param     (i)int nBorderWidth 	    边框宽度 （默认LINE_BORDER）
 @param     (i)COLORREF crBorderColor	边框颜色 （默认BLACKGRAY）
 @param     (i)COLORREF crFillColor 	填充颜色 （默认GRAY）

 @retval     无

 @exception  无
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
@brief     画圆角矩形

@param     (i)CDC* pDC                  设备上下文
@param     (i)const CRect& rect         位置大小
@param     (i)int nBorderWidth 	        边框宽度 （默认LINE_BORDER）
@param     (i)COLORREF crBorderColor	边框颜色 （默认BLACKGRAY）
@param     (i)COLORREF crFillColor 	    填充颜色 （默认GRAY）
@param     (i)CPoint point              圆角大小 （默认（20, 20））

@retval     无

@exception  无
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
@brief     在指定位置显示文字

@param     (i)CDC* pDC              设备上下文
@param     (i)int x		            显示区左上坐标
@param     (i)int y		            显示区左上坐标
@param     (i)CString sText	        文字
@param     (i)CString sFont  	    字体
@param     (i)int nFontSize  	    大小
@param     (i)COLORREF crColor 	    颜色

@retval     无

@exception  无
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
@brief     在指定位置显示文字

@param     (i)CDC* pDC              设备上下文
@param     (i)CRect& rect           显示区域
@param     (i)CString sText	        文字
@param     (i)CString sFont  	    字体
@param     (i)int nFontSize  	    大小
@param     (i)COLORREF crColor 	    颜色
@param     (i)UINT nFormat 	        格式（DT_CENTER|DT_VCENTER|DT_SINGLELINE）

@retval     无

@exception  无
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
@brief     在指定位置显示文字

@param     (i)CDC* pDC              设备上下文
@param     (i)CPoint pos		    显示区左上坐标
@param     (i)CString upText	    第一行文字
@param     (i)CString downText	    第二行文字

@retval     无

@exception  无
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
    pDC->SetBkMode(TRANSPARENT);					// 背景透明

    int cx = pos.x;

    // 判断输出信息行数
    if (downText != "") {  // 两行文字
        int dist = 4;		// 行间距
        CSize szTxtLine1 = pDC->GetTextExtent(upText, lstrlen(upText));
        CSize szTxtLine2 = pDC->GetTextExtent(downText, lstrlen(downText));
        int cy1 = pos.y + NORMAL_BUTTON_HEIGHT / 2 - szTxtLine1.cy - dist / 2;
        int cy2 = pos.y + NORMAL_BUTTON_HEIGHT / 2 + dist / 2;
        pDC->DrawText(upText, CRect(CPoint(cx, cy1), CPoint(pos.x + NORMAL_BUTTON_WIDTH, pos.y + NORMAL_BUTTON_HEIGHT)), DT_WORDBREAK | DT_VCENTER | DT_CENTER);
        pDC->DrawText(downText, CRect(CPoint(cx, cy2), CPoint(pos.x + NORMAL_BUTTON_WIDTH, pos.y + NORMAL_BUTTON_HEIGHT)), DT_WORDBREAK | DT_VCENTER | DT_CENTER);
    }
    else {               	// 一行文字
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
@brief      获取Service对象

@param      无

@retval     Service对象

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CService* CBaseDlg::GetService()
{
    return m_pService;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      返回该Dialog是否是站员Dialog

@param      无

@retval     bool    \n
            true：站员Dialog；false：乘客Dialog

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CBaseDlg::IsOfficeDlg()
{
    return m_isOfficeDlg;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Dialog ID

@param      无

@retval     UINT

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
UINT CBaseDlg::GetID()
{
    return m_ID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      直接调用父对象的事件响应程序（避开MFC的异常处理）

@param      (i)UINT nMsg
@param      (i)WPARAM wParam
@param      (i)LPARAM lParam

@retval     LRESULT, The return value depends on the message.

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CBaseDlg::CallWndProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
    return WindowProc(nMsg, wParam, lParam);
}

LRESULT CBaseDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//对WM_XGROUP和WM_XBUTTON进行特定处理，
	//调用ON_GROUP_BUTTON_CLICK
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