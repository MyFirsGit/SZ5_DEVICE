#pragma once
#include "OperationDlg.h"
#include "PrinterTestSvc.h"

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

public:
	void UpdateUI();                        //刷新界面（外部接口）
protected:
	void InitMenuUI();                      //初始化菜单界面
	void UpdateMenuData();                  //更新界面数据
	void Clear();                           //清空数据
private:
	LABEL_GROUP m_LableGroup[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
