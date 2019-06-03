#pragma once
#include "OperationDlg.h"
#include "IssueSvc.h"

class CIssueResultDlg:public COperationDlg
{
	DECLARE_DYNAMIC(CIssueResultDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueResultDlg(CService*);
	~CIssueResultDlg();
	// 对话框数据
	enum { IDD = IDD_00103_ISSUE_RESULT_DLG };
private:
	void UpdateUI();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LABEL_GROUP m_TicketInfoGroup[10];                          // 车票信息区域数据
	CIssueSvc* pSvc;
};