#pragma once
#include "OperationDlg.h"
#include "BanknoteAcceptModuleTestSvc.h"
/**
@brief 基本画面
*/


class CBanknoteAcceptMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteAcceptMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteAcceptMaintainDlg(CService* pService);
	~CBanknoteAcceptMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

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