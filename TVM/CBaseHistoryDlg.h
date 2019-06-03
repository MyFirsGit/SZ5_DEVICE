#pragma once
#include "ReceptionDlg.h"
#include "AnalyzeSvc.h"
#include "resource.h"

// CBaseHistoryDlg dialog

class CBaseHistoryDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CBaseHistoryDlg)

public:
	CBaseHistoryDlg(CService* pServic);   // standard constructor
	virtual ~CBaseHistoryDlg();

// Dialog Data
	enum { IDD = IDD_10806_ANALYZE_BASEHISTORY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateUI();

	DECLARE_MESSAGE_MAP()
};
