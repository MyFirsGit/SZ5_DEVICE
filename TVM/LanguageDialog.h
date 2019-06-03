#pragma once
#include "resource.h"
#include "UtilTimer.h"

// LanguageDialog 对话框
#define TIMER_LAN 1001						// 定时器ID
const static UINT TIMER_INTERVAL = 5*1000;	// 时间刷新间隔(5 second)

/**
@brief      语言选择画面
*/
class LanguageDialog : public CDialog
{
	DECLARE_DYNAMIC(LanguageDialog)
	DECLARE_MESSAGE_MAP()

public:
	LanguageDialog();   // 标准构造函数
	~LanguageDialog();

	// 对话框数据
	enum { IDD = IDD_LANGUAGE_DLG };

protected:
	void OnDraw(CDC* pDC);					// 画面绘制函数
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	afx_msg void OnPaint();					// 画面绘制函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnChinese(WPARAM,LPARAM);
	afx_msg LRESULT OnEnglish(WPARAM,LPARAM);
	afx_msg LRESULT OnPortuguese(WPARAM,LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);	// 时间控制

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CXButton* BuildOperationButton(CRect rect,CString text,CString msg);

	COLORREF        m_bkColor;				// 背景颜色
	CXButton*       m_btnChinese;			// 繁体中文按钮
	CXButton*       m_btnEnglish;			// 英文按钮
	CXButton*       m_btnPortuguese;		// 葡萄牙文按钮
	 
	LANGUAGETYPE_T m_DefautLanguageType;
};

#define ON_BTN_CHINESE_CLICK(memberFxn)\
{ WM_XBUTTON, 0, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL LanguageDialog::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_ENGLISH_CLICK(memberFxn)\
{ WM_XBUTTON, 1, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL LanguageDialog::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_PORTUGUESE_CLICK(memberFxn)\
{ WM_XBUTTON, 2, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL LanguageDialog::*)(WPARAM, LPARAM) > \
	(memberFxn)) },