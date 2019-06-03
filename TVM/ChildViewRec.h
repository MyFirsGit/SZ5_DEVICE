#pragma once
// ChildViewRec.h : CChildViewRec 类的接口
//
#include "OutOfReceptionDlg.h"
#include "GuideReceptionDlg.h"
#include "ReceptionHeadDLG.h"
#include "MenuDlg.h"

// CChildViewRec 窗口

class CChildViewRec : public CWnd
{
    DECLARE_MESSAGE_MAP()

// 构造
public:
	CChildViewRec();
    ~CChildViewRec();

// 实现
public:
    COutOfReceptionDlg* m_pOutOfReceptionDlg;       // 暂停服务对话框
    CReceptionHeadDlg* m_pReceptionHeadDlg;         // 接客画面标题对话框
	CMenuDlg* m_pMenuDlg;
	LRESULT CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

