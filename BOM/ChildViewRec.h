#pragma once
// ChildViewRec.h : CChildViewRec 类的接口
//
#include "OutOfReceptionDlg.h"
#include "GuideReceptionDlg.h"
#include "ReceptionHeadDLG.h"

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
    CGuideReceptionDlg* m_pReceptionGuideDlg;       // 乘客向导Dialog

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    BOOL PreCreateWindow(CREATESTRUCT& cs);
};

