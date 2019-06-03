#pragma once
#include "OperationDlg.h"

/**
@brief 维护业务基本画面
*/
class CMaintenanceBaseDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CMaintenanceBaseDlg)
    DECLARE_MESSAGE_MAP()

public:
	CMaintenanceBaseDlg(CService* pService);
	~CMaintenanceBaseDlg();

	enum { IDD = IDD_20000_MAINTENANCE_BASE_DLG };

	void Refresh();

protected:
	void UpdateUI();

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP m_labelGroup[15];
};
