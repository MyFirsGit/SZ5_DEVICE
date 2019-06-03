#pragma once
#include "OperationDlg.h"

/**
@brief      设备模式切换画面
*/
class CDeviceModeSwitchDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CDeviceModeSwitchDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDeviceModeSwitchDlg(CService* pService);                               // 标准构造函数
	~CDeviceModeSwitchDlg();
	enum { IDD = IDD_20116_MODE_SWITCH_DLG };                               // 对话框数据

private:
	LABEL_GROUP m_modeSwitchInfo[LABEL_GROUP_EDIT_ROWS_2];                       // 画面显示的模式切换信息

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// 处理确认按钮消息
	afx_msg LRESULT OnModeItemClick(WPARAM,LPARAM);							// 按钮按下后的处理
	void InitModeInfo();                                                    // 初始化基本信息

	int m_CurrentMode;
	int m_SelectedMode;
};
