#pragma once
#include "BanknoteCollectingBoxRemovalSvc.h"

class CBanknoteCollectingBoxRemovalDlg  : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteCollectingBoxRemovalDlg)
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteCollectingBoxRemovalDlg(CService* pService);
	~CBanknoteCollectingBoxRemovalDlg(void);

	enum { IDD = IDD_20113_STATION_SET_END_DLG }; //?? why use this dialog??

	// 初始化格式
	//void InitBaseUI();
	//void InitAttachBoxUI();
	void InitDetachBoxUI();

	// 显示数据
	//void ShowBaseData();
	//void ShowAttachBoxData();
	void ShowDetachBoxData();

	// 清空数据
	void Clear();

protected:
	void UpdateUI();

private:

	CBanknoteCollectingBoxRemovalSvc* pSvc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};

