#pragma once
#include "StatusDlg.h"
#include "GuideDlg.h"
#include "NavigationDlg.h"
#include "OperationDlg.h"

// 前向声明
class CFunctionDlg;

/**
@brief  CChildView 界面布局视图
*/
class CChildView : public CWnd
{
	DECLARE_MESSAGE_MAP()

public:
	CChildView();
	virtual ~CChildView();

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);						// 添加 Owner Draw 属性
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 画面创建
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	LRESULT CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam);	// 窗口消息处理

public:
	CStatusDlg* m_pStatusDlg;		// 状态栏
	vector<CBaseDlg*> children;
};
