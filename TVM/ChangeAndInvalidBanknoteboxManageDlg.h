#pragma once
#include "OperationDlg.h"
#include "ChangeAndInvalidBanknoteboxManageSvc.h"
/*
	找零箱与废币箱管理的界面
*/

class CChangeAndInvalidBanknoteboxManageDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CChangeAndInvalidBanknoteboxManageDlg)
	DECLARE_MESSAGE_MAP()

public:
	CChangeAndInvalidBanknoteboxManageDlg(CService* pService);
	~CChangeAndInvalidBanknoteboxManageDlg(void);
	enum { IDD = IDD_20113_STATION_SET_END_DLG };

protected:
	void UpdateUI();

private:
	LABEL_GROUP_SEVEN m_lableGroup[15];
	CChangeAndInvalidBanknoteboxManageSvc *m_pSvc;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

