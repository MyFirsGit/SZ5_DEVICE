#pragma once
#include "OperationDlg.h"

/**
@brief  RFID写入设定
*/
class CRFIDSetBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRFIDSetBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRFIDSetBaseOfficeDlg(CService* pService);
	~CRFIDSetBaseOfficeDlg();
	enum { IDD = IDD_20122_RFID_MODE_SET_DLG };                               // 对话框数据

private:
	LABEL_GROUP m_lableRFIDModeInfo[LABEL_GROUP_EDIT_ROWS_2];					// 画面显示的模式切换信息

	void InitRFIDModeInfo();                                                  // 初始化基本信息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// 画面创建函数
	afx_msg LRESULT OnModeItemClick(WPARAM,LPARAM);							// 按钮按下后的处理
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// 处理确认按钮消息
};
