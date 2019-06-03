#pragma once
#include "OperationDlg.h"

/**
@brief      读写器测试画面
*/
class CCardReadWriteMachineTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCardReadWriteMachineTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCardReadWriteMachineTestMaintainDlg(CService* pService);			// 标准构造函数
	~CCardReadWriteMachineTestMaintainDlg();

	// 对话框数据
	enum { IDD = IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG };

	LABEL_GROUP m_CardReadWriteInfo[LABEL_GROUP_EDIT_ROWS_10];			// 读写器测试信息
private:
	BOOL OnInitDialog();												// 对话框初始化
	void InitialButtons();												// 初始化按键

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// 确定按钮的响应函数
	LRESULT OnRWTestBtnClick(WPARAM wParam,LPARAM lParam);				// 读写器类型按钮选择
	LRESULT OnRWDetailTestBtnClick(WPARAM wParam,LPARAM lParam);		// 读写器详细测试按钮选择

	WORD m_RWType;
	WORD m_RWDetailType;
};
