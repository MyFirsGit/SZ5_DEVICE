#pragma once
#include "OperationDlg.h"
#include "SmartCardTestSvc.h"
/**
@brief      Smart-Card测试画面
*/
class CSmartCardTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CSmartCardTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSmartCardTestMaintainDlg(CService* pService);
	~CSmartCardTestMaintainDlg();

	enum { IDD = IDD_TH_TEST_DLG };			// 对话框数据

public:
	void UpdateUI();                        //刷新界面（外部接口）
protected:
	void InitMenuUI();                      //初始化菜单界面
	void InitMotorTestMenuUI();             //初始化菜单界面
	void InitReadRFIDTestMenuUI();
	void InitWriteRFIDTestMenuUI();
	void InitSensorTestMenuUI();
	void UpdateMenuData();                  //更新界面数据
	void UpdateMotorTestMenuData();         //更新界面数据
	void UpdateReadRFIDTestMenuData();      //更新界面数据
	void UpdateWriteRFIDMenuData();         //更新界面数据
	void UpdateSensorTestMenuData();        //更新界面数据
	void Clear();                           //清空数据
private:
	LABEL_GROUP_FIVE  m_lableGroupFive[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
