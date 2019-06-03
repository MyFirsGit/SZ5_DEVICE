#pragma once
#include "OperationDlg.h"
#include "CoinModuleTestSvc.h"
/**
@brief 基本画面
*/

#define _MAX_ROW 15   //定义基本显示区最多显示行数
#define _MAX_COL  3   //定义基本显示区最多显示列数

class CCoinModuleMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinModuleMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinModuleMaintainDlg(CService* pService);
	~CCoinModuleMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

public:
	void UpdateUI();              //提供给其他类的公用接口
protected:
	void InitMenuUI();            //初始化菜单界面
	void InitReturnCoinUI();      //初始化退币界面
	void InitClearCoinUI();       //初始化清币界面
	void InitSafeLockCtrlUI();    //初始化安全扣控制界面
	void InitReadIdUI();          //初始化读RFID界面
	void InitWriteIdUI();         //初始化写RFID界面
	void InitAddCoinUI();         //初始化加币界面

	void UpdateMenuData();        //更新菜单界面数据
	void UpdateReturnCoinData();  //更新退币界面数据
	void UpdateClearCoinData();   //更新清币界面数据
	void UpdateSafeLockCtrlData();//更新安全扣控制界面数据
	void UpdateReadIdData();      //更新读RFID界面数据
	void UpdateWriteIdData();     //更新写RFID界面数据	
	void UpdateAddCoinData();     //更新加币操作界面数据

	void Clear();                 //1清空显示内容
private:
	LABEL_GROUP_THREE m_labelGroupThree[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};