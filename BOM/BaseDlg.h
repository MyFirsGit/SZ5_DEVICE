#pragma once
#include "resource.h"
#include "UIDefinition.h"
class CService;     // 向前引用

/**
@brief      所有对话框父类
*/
class CBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CBaseDlg)
    DECLARE_MESSAGE_MAP()

public:
    // 对话框数据
    enum { IDD = IDD_BASE_DLG };

    CBaseDlg(UINT ID, CWnd* pParent = NULL, CRect position = CRect(0,0,1920,1080)/*CRect(0,0,1024,768)*/, 
        COLORREF bkColor = GRAY, CService* pService = NULL, bool isOfficeDlg = true);                   // 构造函数
	virtual ~CBaseDlg();                                                                                // 析构函数
    virtual void OnOK() {};                                                                             // 屏蔽Eenter键
    virtual void OnCancel() {};                                                                         // 屏蔽Esc键
    CService* GetService();                                                                             // 获取Dialog的Service

	template<class T>
	T* GetService()
	{
		return (T*)m_pService;		
	};

    UINT GetID();                                                                                       // 获取Dialog ID
	
	void DrawRect(CDC* pDC, const CRect& rect, int nBorderWidth = ROUNDRECT_BORDER,
        COLORREF crBorderColor = DARKGRAY, COLORREF crFillColor = GRAY);                                // 画矩形
    void DrawRoundRect(CDC* pDC, const CRect& rect, int nBorderWidth = ROUNDRECT_BORDER,
        COLORREF crBorderColor = DARKGRAY, COLORREF crFillColor = GRAY, CPoint point = ROUND_POINT);    // 画圆角矩形
    void DrawText(CDC* pDC, int x, int y, CString sText, CString sFont = TITLE_FONT,
        int nFontSize = TITLE_FONT_SIZE, int nFontWeight = FW_BOLD, COLORREF crColor = BLACK);                                     // 在指定位置显示文字
    void DrawText(CDC* pDC, CRect& rect, CString sText, CString sFont = TITLE_FONT,
        int nFontSize = TITLE_FONT_SIZE, int nFontWeight = FW_BOLD, COLORREF crColor = BLACK, UINT nFormat = DT_CENTER);           // 在指定位置显示文字
    void DrawText(CDC* pDC, CPoint pos, CString upText, CString downText);                              // 在指定位置显示文字
	void DrawText(CDC* pDC,CRect& rect,CString sText,LOGFONT logFont = DEFAULT_FONT,COLORREF crColor = BLACK,UINT nFormat = DT_CENTER|DT_SINGLELINE|DT_VCENTER);

    bool IsOfficeDlg();	                                                                                // 返回该Dialog是否是站员Dialog

    LRESULT CallWndProc(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0);                               // 直接调用父对象的事件响应程序（避开MFC的异常处理）

protected:
    static const int BORDER_MAX_SIZE = 3;                                                               // 边框的最大宽度

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                                                // 画面创建函数
    virtual void OnDraw(CDC* pDC);                                                                      // 画面绘制函数
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void UpdateUI();
	
private:
    afx_msg void OnPaint();                                                                             // 画面绘制函数
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);                                                                // 重画Dialog背景
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);                                    // 子控件重画时调用
    LRESULT OnServiceDataModelChanged(WPARAM,LPARAM);

private:
    CRect       m_rectDlg;                              // 对话框位置
    COLORREF    m_bkColor;                              // 背景颜色
    bool        m_isOfficeDlg;                          // 站员和乘客Dialog标志，true：站员Dialog；false：乘客Dialog
    UINT        m_ID;                                   // Dialog ID
    CService*   m_pService;                             // 对话框关联的Server对象
};

#define ON_GROUP_BUTTON_CLICK(groupID, memberFxn)\
{ WM_XGROUP, groupID, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_OK_CLICK(memberFxn)\
{ WM_XBUTTON, 1, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_BACK_CLICK(memberFxn)\
{ WM_XBUTTON, 2, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_PRINT_CLICK(memberFxn)\
{ WM_XBUTTON, 3, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_RESET_CLICK(memberFxn)\
{ WM_XBUTTON, 4, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

