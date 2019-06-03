#pragma once
#include "OperationDlg.h"
#include "StationSetSvc.h"

/**
@brief      车站信息输入画面
*/
class CStationSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CStationSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStationSetMaintainDlg(CService* pService);                             // 标准构造函数
	~CStationSetMaintainDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };                               // 对话框数据

	void InitlabelStationInfo();											// 初始化BASE区域LABEL里面的数据
	void SetFromReadIniText();												// 显示从Maintenance.INI读取的配置文件在对应项上
	void ShowCodeContent();													// 基本信息区显示对应项设置的数据
protected:
	virtual void UpdateUI();												// 更新显示完成回调函数
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	// 画面激活
	LABEL_GROUP m_lableGroup[15];											// 画面显示的车站信息
};
