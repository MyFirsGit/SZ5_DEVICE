#pragma once
#include "OperationDlg.h"

/**
@brief 维护业务基本画面
*/
class CSelectLanguageBaseDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CSelectLanguageBaseDlg)
    DECLARE_MESSAGE_MAP()

public:
	CSelectLanguageBaseDlg(CService* pService);
	~CSelectLanguageBaseDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };

	void ShowData();

protected:
	void UpdateUI();

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP m_lableGroup[15];
};
