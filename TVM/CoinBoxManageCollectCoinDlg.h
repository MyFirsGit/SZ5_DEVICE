#pragma once
#include "OperationDlg.h"

/**
@brief 基本画面
*/
class CCoinBoxManageCollectCoinDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageCollectCoinDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageCollectCoinDlg(CService* pService);
	~CCoinBoxManageCollectCoinDlg();

	enum { IDD = IDD_20114_COMMUNICATION_SET_DLG };

	void InitCollectCoinInfo();
	void ShowCollectCoinInfo();								// 显示硬币回收箱面值、ID、数量
	void ShowCollectingCoin();								// 显示硬币回收箱回收硬币时面值、ID、数量			
	void ShowFinishCollectCoin();							// 显示完成硬币回收

protected:
	virtual void UpdateUI();								// 更新显示完成回调函数

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};