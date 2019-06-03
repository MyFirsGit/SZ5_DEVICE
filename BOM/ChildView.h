#pragma once
#include "StatusDLG.h"
#include "LogoDLG.h" 
#include "GuideDlg.h"
#include "OperationDlg.h"
#include "keyboard.h"


/////////////////////////////////////////////////////////////////////////////
// CChildView window

class CFunctionDlg;

class CChildView : public CWnd
{
    DECLARE_MESSAGE_MAP()

// Construction
public:
	CChildView();
    virtual ~CChildView();

private:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	CStatusDlg* m_pStatusDlg;
	CLogoDlg* m_pLogoDlg;
	CFunctionDlg* m_Left1_Dlg;
	CFunctionDlg* m_Left2_Dlg;
	CFunctionDlg* m_Right1_Dlg;
	CFunctionDlg* m_Right2_Dlg;
	//CFunctionDlg* m_Right3_Dlg;
	CKeyboard* m_Keyboard;
	vector<CBaseDlg*> children;
	LRESULT CallWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
