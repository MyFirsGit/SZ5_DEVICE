#pragma once
#include "OperationDlg.h"
#include "GStatic.h"

// CPrinterTestMaintainDlg 对话框

/**
@brief      打印机测试画面
*/
class CPrinterTestMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CPrinterTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestMaintainDlg(CService* pService);				// 标准构造函数
	~CPrinterTestMaintainDlg();

	// 对话框数据
	enum { IDD = IDD_20531_PRINTER_TEST_DLG };
private:

public:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);						// 处理确认按钮消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// 窗体创建函数
};
