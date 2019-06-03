#pragma once
#include "OperationDlg.h"
#include "BanknoteCollectingBoxInstallationSvc.h"

class CBanknoteCollectingBoxInstallationDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteCollectingBoxInstallationDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteCollectingBoxInstallationDlg(CService* pService);
	~CBanknoteCollectingBoxInstallationDlg(void);

	enum { IDD = IDD_20113_STATION_SET_END_DLG }; //?? why use this dialog??
	// 初始化格式
	void InitBaseUI();
	void InitAttachBoxUI();
	// 显示数据
	void ShowAttachBoxData();
	// 清空数据
	void Clear();

protected:
	void UpdateUI();

private:

	CBanknoteCollectingBoxInstallationSvc* pSvc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};

