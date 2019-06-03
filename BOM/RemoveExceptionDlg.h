#pragma once
#include "OperationDlg.h"

/**
@brief      外设管理选择画面
*/
class CRemoveExceptionDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRemoveExceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionDlg(CService* pService);				// 标准构造函数
	~CRemoveExceptionDlg();

	// 对话框数据
	enum { IDD = IDD_20701_REMOVE_EXCEPTION_DLG };

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);					// 处理确认按钮消息
	afx_msg LRESULT XButtonClick(WPARAM,LPARAM);
	void InitExceptionInfo();
	void RefreshExceptionInfo();							// 刷新异常列表

	CXButton* m_btnPageUp;
	CXButton* m_btnPageDown;
	LABEL_GROUP_FOUR m_LabelExceptionInfo[11];
	vector<CSysException> m_ExceptionList;
	vector<CSysException>::size_type m_ExceptionNum;
	int m_TotalPageNum;
	int m_CurrentPageNum;
};
