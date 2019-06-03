#pragma once
#include "OperationDlg.h"

/**
@brief      出票模式设定界面
*/
class CTHModeSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTHModeSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTHModeSetMaintainDlg(CService* pService);
	~CTHModeSetMaintainDlg();
	enum { IDD = IDD_20120_TH_MODE_SET_DLG };                               // 对话框数据

private:
	LABEL_GROUP m_lableTHModeInfo[LABEL_GROUP_EDIT_ROWS_2];					// 画面显示的模式切换信息

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg LRESULT OnModeItemClick(WPARAM,LPARAM);							// 按钮按下后的处理
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// 处理确认按钮消息
	void InitTHModeInfo();                                                  // 初始化基本信息

	int m_CurrentMode;
	int m_SelectedMode;
};
