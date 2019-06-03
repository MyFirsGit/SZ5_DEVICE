#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"

class CIssueACCReceptionDlg : public CReceptionDlg
{
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CIssueACCReceptionDlg)
public:
	CIssueACCReceptionDlg(CService*);
	~CIssueACCReceptionDlg();
public:
	// 对话框数据
	enum { IDD = IDD_10101_ISSUE_DLG };
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	void UpdateUI();
	CIssueSvc* pSvc;
};