#pragma once
#include "OperationDlg.h"

/**
@brief      出票机测试显示画面
*/
class CTHTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTHTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTHTestMaintainDlg(CService* pService);
	~CTHTestMaintainDlg();

	enum { IDD = IDD_TH_TEST_DLG };										// 对话框数据

private:
	LABEL_GROUP m_labelTHTestInfo[11];				// 画面显示的测试信息

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// 处理确认按钮消息
	LRESULT OnReset(WPARAM wParam,LPARAM lParam);						// 处理重置按钮信息
	LRESULT OnSingleTestTypeBtnClick(WPARAM wParam,LPARAM lParam);		// 单体测试按钮选择
	LRESULT OnDetailTestTypeBtnClick(WPARAM wParam,LPARAM lParam);		// 详细测试按钮选择
	void ShowSensorInfo();

	BOOL OnInitDialog();												// 对话框初始化
	void InitialButtons();												// 初始化按键
	void InitialBaseInfo();												// 初始化基本信息
	void ShowRFIDInfo();												// 显示RFID信息
	void DoRW();														// 写入
	bool CheckInputText();												// 检查EDIT框里的数据是否合法

	WORD        m_wUnitTestFirstNo;										// 单体测试类型编号
	WORD        m_wUnitTestSecondNo;									// 详细测试类型编号
};
