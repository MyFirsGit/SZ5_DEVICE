#pragma once
#include "OperationDlg.h"

/**
@brief 硬币回收箱卸载
*/
class CCoinBoxManageDetachCoinCollectionBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageDetachCoinCollectionBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageDetachCoinCollectionBoxDlg(CService* pService);
	~CCoinBoxManageDetachCoinCollectionBoxDlg();

	enum { IDD = IDD_20101_SYSTEM_SET_DLG };

	void InitDetachCoinCollectionBoxInfo();
	void ShowDetachCoinCollectionBoxInfo();					// 显示硬币回收箱状态
	void ShowDoDetachCoinCollectionBoxInfo();				// 显示硬币回收箱卸载状态
	void SetColor(int row,CString status);					// 设置状态显示的颜色
protected:
	virtual void UpdateUI();								// 更新显示完成回调函数

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SIX m_lableGroup[15];
};