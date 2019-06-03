#pragma once
#include "OperationDlg.h"

/**
  @brief  行政处理-基本画面
*/
class  CNoBusinessIncomeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CNoBusinessIncomeBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()
public:
   CNoBusinessIncomeBaseOfficeDlg(CService* pService);
   ~CNoBusinessIncomeBaseOfficeDlg();

   enum { IDD = IDD_02701_NOBUSINESS_INCOME_BASE_DLG };
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);			// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);							// 处理确认按钮消息
	LRESULT OnIncomeBtnClick(WPARAM wParam,LPARAM lParam);		// 处理收入原因按钮消息
	LRESULT OnExpendBtnClick(WPARAM wParam,LPARAM lParam);		// 处理支出原因按钮消息

	virtual BOOL OnInitDialog();									// 对话框初始化
	void InitButtons();												// 初始化按键
	bool CheckInputValid();											// 检测编辑框输入是否正确

	bool m_BtnChecked;
};