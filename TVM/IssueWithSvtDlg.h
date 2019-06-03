#pragma once
#include "ReceptionDlg.h"
#include "IssueSvc.h"


class CIssueSvc;

/**
@brief ��ֵ����Ʊ����
*/
class CIssueWithSvtDlg : public CReceptionDlg
{
public:
	DECLARE_DYNAMIC(CIssueWithSvtDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIssueWithSvtDlg(CService*);
	~CIssueWithSvtDlg();
	
	// �Ի�������
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


	LRESULT OnTicketNumClick(WPARAM wParam,LPARAM lParam);		// ��Ʊ����ѡ��
	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// ���ȡ����ť
	LRESULT OnOkAndCancel1(WPARAM wParam,LPARAM lParam);		// ���ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��
	LRESULT OnOkAndCancel2(WPARAM wParam,LPARAM lParam);		// ���ȡ����ť(ȷ����ȡ����ť�е�ȡ����ť��

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
