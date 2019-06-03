#pragma once
#include "OperationDlg.h"

/**
@brief 加币箱卸载
*/
class CCoinBoxManageDetachAddingBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageDetachAddingBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachAddingBoxDlg(CService* pService);
	~CCoinBoxManageDetachAddingBoxDlg();

	enum { IDD = IDD_20111_MACHINE_PARAMETER_SET_DLG };

	void InitDetachCoinAddingBoxInfo();
	void ShowDetachCoinAddingBoxInfo();							// 显示硬币补充箱状态
	void ShowDoDetachCoinAddingBoxInfo();						// 显示硬币补充箱卸载状态
	void SetColor(int row,CString status);						// 设置状态显示的颜色
protected:
	virtual void UpdateUI();									// 更新显示完成回调函数

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_FOUR m_lableGroup[15];
}; 