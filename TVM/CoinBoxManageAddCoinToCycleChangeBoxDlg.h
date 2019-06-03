#pragma once
#include "OperationDlg.h"

/**
@brief 基本画面
*/
class CCoinBoxManageAddCoinToCycleChangeBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageAddCoinToCycleChangeBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAddCoinToCycleChangeBoxDlg(CService* pService);
	~CCoinBoxManageAddCoinToCycleChangeBoxDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

	void InitCycleChangeBoxInfo();
	void ShowCycleChangeBoxData();												// 显示硬币循环找零箱ID、数量
	void ShowCycleChangeBoxAddingCoin();										// 显示硬币循环找零箱加币中界面
	void ShowFinishCycleChangeBoxAddingCoin();									// 显示硬币循环找零箱加币完成界面
protected:
	virtual void UpdateUI();													// 更新显示完成回调函数
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};
