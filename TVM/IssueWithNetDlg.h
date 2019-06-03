#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"


class CIssueSvc;

/**
@brief 二维码售票画面
*/
class CIssueWithNetDlg : public CReceptionDlg
{
public:
	DECLARE_DYNAMIC(CIssueWithNetDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueWithNetDlg(CService*);
	~CIssueWithNetDlg();
	
	// 对话框数据
	enum { IDD = IDD_00103_ISSUE_WITH_NET_DLG };

	enum {MAX_TICKET_INFO_ROWS = 18};

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNotifyIssuing(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStartIssue(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnNotifyErweimaResult(WPARAM wParam,LPARAM lParam);

	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// 点击取消按钮

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
