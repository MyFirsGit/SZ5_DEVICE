#pragma once
#include "OperationDlg.h"
#include "RWTestSvc.h"

#define _MAX_ROW 15   //定义基本显示区最多显示行数
#define _MAX_COL  3   //定义基本显示区最多显示列数


class CCardReadWriteMachineTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCardReadWriteMachineTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCardReadWriteMachineTestMaintainDlg(CService* pService);
	virtual ~CCardReadWriteMachineTestMaintainDlg();

	enum {IDD = IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG };

public:
	//初始化界面
	void InitMainMenuUI();         //初始化主菜单栏
	void InitBaseMenuUI();         //初始化基本菜单选项（功能）
	void InitParameterUI();        //初始化Parameter信息显示
	void InitRWInfoUI();           //初始化RW info信息显示界面
	//设置数据
	void ShowMainMenuData();       //设置主菜单选项值
	void ShowBaseMenuData();       //显示基本菜单选项
	void ShowParameterData();      //显示Parameter值
	void ShowRWInfoData();         //显示RWInfo值

	void InitParameterBaseData(); //初始化Parameter的静态值

	void ClearData();             //Clear All Data

protected:
	void UpdataUI();
private:
	int DeviceType;
	LABEL_GROUP_THREE m_LabelGroupThree[20];

	//BaseMenu_Info m_BaseMenuInfo[4];//菜单子选项
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	friend class CRWTestSvc;
};