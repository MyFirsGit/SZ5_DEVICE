#pragma once
#include "OperationDlg.h"
#include "GStatic.h"

// CMoneyBoxTestMaintainDlg 对话框

/**
@brief      钱箱测试画面
*/
class CMoneyBoxTestMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CMoneyBoxTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMoneyBoxTestMaintainDlg(CService* pService);			// 标准构造函数
	~CMoneyBoxTestMaintainDlg();

	// 对话框数据
	enum { IDD = IDD_20541_MONEYBOX_TEST_DLG };
private:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);					// 确定按钮响应函数	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 窗体创建函数
};
