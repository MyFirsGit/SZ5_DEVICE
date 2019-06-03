#pragma once
#include "operationdlg.h"
#include "ModeSettingsSvc.h"
class CModeSettingsDlg :
	public COperationDlg
{
	DECLARE_DYNAMIC(CModeSettingsDlg)
	DECLARE_MESSAGE_MAP()

public:
	CModeSettingsDlg(CService *pSvc);
	~CModeSettingsDlg(void);

	enum{IDD = IDD_20112_STATION_SET_DLG};

	void UpdateUI();
	void DialogClearUp();
	void ShowDialog();
	void InitDialog();
	void InitOfficeGuide();

private:
	//CModeSettingsSvc *m_pSVC;
	CModeSettingsSvc::SVC_DATA m_data;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LABEL_GROUP_FOUR m_lableGroup[MAX_ROW];
	
};

