#pragma once
#include "OperationDlg.h"
#include "IOModuleTestSvc.h"
/**
@brief      IO测试画面
*/
class CIOModuleTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CIOModuleTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIOModuleTestMaintainDlg(CService* pService);
	~CIOModuleTestMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };			// 对话框数据

public:
	void UpdateUI();                        //刷新界面（外部接口）
protected:
	void InitMenuUI();                      //初始化菜单界面
	void InitStatusUI();                    //初始化传感器状态界面

	void UpdateMenuData();                  //更新界面数据
	void UpdateStatusData();                //更新传感器数据
	void Clear();                           //清空数据
private:
	int DeviceType;
	LABEL_GROUP   m_LableGroup[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
