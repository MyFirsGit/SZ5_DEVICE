#pragma once
#include "operationdlg.h"
#include "BalanceSvc.h"
class CBalanceDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBalanceDlg)

	DECLARE_MESSAGE_MAP()

public:
	CBalanceDlg(CService* pService = NULL);
	~CBalanceDlg(void);

	enum { IDD = IDD_00002_LOGIN_DLG };

	void UpdateUI();
	void Initialize();

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CBalanceSvc *m_pSvc;
	LABEL_GROUP m_lableGroup[MAX_ROW];
};

