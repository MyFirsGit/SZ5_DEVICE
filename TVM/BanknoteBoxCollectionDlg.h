#pragma once
#include "OperationDlg.h"

/**
@brief 纸币模块回收界面
*/
class CBanknoteBoxCollectionDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteBoxCollectionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteBoxCollectionDlg(CService* pService);
	~CBanknoteBoxCollectionDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG }; 

	// 初始化格式
	void InitBaseUI();
	/*void InitAttachBoxUI();
	void InitDetachBoxUI();*/

	// 显示数据
	void ShowBaseData();
	/*void ShowAttachBoxData();
	void ShowDetachBoxData();*/

	// 清空数据
	void Clear();

protected:
	void UpdateUI();

private:
	CBanknoteBoxCollectionSvc* pSvc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};