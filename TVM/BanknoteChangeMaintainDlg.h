#pragma once
#include "OperationDlg.h"
#include "BanknoteChangeModuleTestSvc.h"
/**
@brief 基本画面
*/


class CBanknoteChangeMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteChangeMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteChangeMaintainDlg(CService* pService);
	~CBanknoteChangeMaintainDlg();

	enum { IDD = IDD_00501_EXCHANGE_BASE_DLG };

public:
	void UpdateUI();                    //刷新界面
protected: 
	void InitMenuUI();                  //初始化Menu界面
	void UpdateMenuData();              //设置Menu数据
	void Clear();
private:
	LABEL_GROUP_FOUR m_labelGroupFour[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};