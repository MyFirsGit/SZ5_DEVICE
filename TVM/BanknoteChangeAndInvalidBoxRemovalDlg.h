#pragma once
#include "OperationDlg.h"
#include "BanknoteChangeAndInvalidBoxRemovalSvc.h"

class CBanknoteChangeAndInvalidBoxRemovalDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteChangeAndInvalidBoxRemovalDlg)
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteChangeAndInvalidBoxRemovalDlg(CService* pService);
	~CBanknoteChangeAndInvalidBoxRemovalDlg(void);
	enum { IDD = IDD_20113_STATION_SET_END_DLG };

protected:
	void UpdateUI();

private:
	LABEL_GROUP_SEVEN m_lableGroup[15];
	CBanknoteChangeAndInvalidBoxRemovalSvc *m_pSvc;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

