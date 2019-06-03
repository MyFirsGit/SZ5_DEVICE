#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"


class CIssueSvc;

/**
@brief 储值卡售票画面
*/
class CIssueWithSvtDlg : public CReceptionDlg
{
public:
	DECLARE_DYNAMIC(CIssueWithSvtDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueWithSvtDlg(CService*);
	~CIssueWithSvtDlg();
	
	// 对话框数据
	enum { IDD = IDD_00102_ISSUE_WITH_SVT_DLG };

	enum {MAX_TICKET_INFO_ROWS = 18};

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNotifyStartReadCard(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStartIssue(WPARAM wParam,LPARAM lParam);

	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();


	LRESULT OnTicketNumClick(WPARAM wParam,LPARAM lParam);		// 车票数量选择
	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// 点击取消按钮
	LRESULT OnOkAndCancel1(WPARAM wParam,LPARAM lParam);		// 点击确定按钮(确定与取消按钮中的确定按钮）
	LRESULT OnOkAndCancel2(WPARAM wParam,LPARAM lParam);		// 点击取消按钮(确定与取消按钮中的取消按钮）

	void InitializeBaseInfo();

private:
	void ClearLabelData();
private:
	CIssueSvc* pSvc;
	vector<UIINFO> m_vec_BtnUIInfo;
	vector<BTN_INFO> m_vec_buttonInfos;
	vector<TEXT_INFO> m_vec_textInfos;
	vector<LocationNumber_t> m_vec_hotLocation;
};
