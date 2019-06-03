#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"

class CIssueSvc;

/**
@brief 现金售票画面
*/
class CIssueWithCashDlg : public CReceptionDlg
{
public:
	DECLARE_DYNAMIC(CIssueWithCashDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueWithCashDlg(CService*);
	~CIssueWithCashDlg();
	
	// 对话框数据
	enum { IDD = IDD_00101_ISSUE_WITH_CASH_DLG };

	enum {MAX_TICKET_INFO_ROWS = 16};
	void showPaymethodGuide();
	LRESULT OnCancel(WPARAM wParam,LPARAM lParam);					// 点击取消按钮

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNotifyStartReceive(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnNotifyIssuing(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnStartIssue(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnNotifyAccepted(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnNotifyErweimaResult(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateGuide(WPARAM wParam,LPARAM lParam);

	void UpdateUI();

	virtual void preShow();
	virtual void showComplete();

	LRESULT OnPriceTypeClick(WPARAM wParam,LPARAM lParam);		// 票价选择
	LRESULT OnTicketNumClick(WPARAM wParam,LPARAM lParam);		// 车票数量选择
	LRESULT OnFunctionClick(WPARAM wParam,LPARAM lParam);		// 功能选择
	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// 点击取消按钮
	LRESULT OnOkAndCancel1(WPARAM wParam,LPARAM lParam);		// 点击确定按钮(确定与取消按钮中的确定按钮）
	LRESULT OnOkAndCancel2(WPARAM wParam,LPARAM lParam);		// 点击取消按钮(确定与取消按钮中的取消按钮）

	void InitializeBaseInfo();
	void InitializeGuideArea();									

private:
	void ClearLabelData();
private:
	CIssueSvc* pSvc;
	vector<UIINFO> m_vec_BtnUIInfo;
	vector<BTN_INFO> m_vec_buttonInfos;
	vector<TEXT_INFO> m_vec_textInfos;
	vector<LocationNumber_t> m_vec_hotLocation;
	WORD m_wCurrClickStation;
	CImage m_bkgImage;
};
