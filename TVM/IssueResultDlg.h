#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"

class CIssueResultDlg:public CReceptionDlg
{
	DECLARE_DYNAMIC(CIssueResultDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueResultDlg(CService*);
	~CIssueResultDlg();
	// 对话框数据
	enum { IDD = IDD_00103_ISSUE_RESULT_DLG };

private:	
	void InitializeGuideArea();
	void UpdateUI();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnDraw(CDC* pDC);
	void ShowGuideArea();
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CIssueSvc* pSvc;
};