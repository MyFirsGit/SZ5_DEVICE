#pragma once
#include "OperationDlg.h"
#include "TokenModuleTestSvc.h"
/**
@brief 基本画面
*/

#define _MAX_ROW 15   //定义基本显示区最多显示行数
#define _MAX_COL  4   //定义基本显示区最多显示列数

class CTokenModuleMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTokenModuleMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTokenModuleMaintainDlg(CService* pService);
	~CTokenModuleMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

public:
	void UpdateUI();              //刷新界面（外部接口）
protected:
	//界面
	void InitMenuUI();            //初始化菜单界面
	void InitReadRfidUI();        //初始化读RFID界面
	void InitWriteRfidUI();       //初始化写RFID界面
	//数据
	void UpdateMenuData();        //更新菜单显示结果
	void UpdateReadRfidData();    //更新读RFID数据
	void UpdateWriteRfidData();   //更新写RFID数据
	void Clear();                 //清空显示内容
private:
	LABEL_GROUP_FOUR  m_labelGroupFour[_MAX_ROW];  //基本数据显示区

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                       //画面创建
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);    //画面激活
};