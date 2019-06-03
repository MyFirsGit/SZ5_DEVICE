#pragma once
#include "OperationDlg.h"

/**
@brief 硬币补充
*/
class CCoinBoxManageAddCoinToDedicatedChangeBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAddCoinToDedicatedChangeBoxDlg(CService* pService);
	~CCoinBoxManageAddCoinToDedicatedChangeBoxDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };

	void InitDedicatedChangeBoxInfo();
	void ShowDedicatedChangeBoxData();						// 显示硬币专用找零箱ID、数量
	void ShowAddingCoinToDedicatedChangeBox();				// 显示硬币专用找零箱加币中界面
	void ShowFinishAddingCoinToDedicatedChangeBox();		// 显示硬币专用找零箱加币完成界面
protected:
	virtual void UpdateUI();								// 更新显示完成回调函数

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
	CString CashValueToString(BankNoteAndCoinType_t type);
	void ShowTips();										// 显示加币提示
};