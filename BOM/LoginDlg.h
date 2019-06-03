#pragma once
#include "OperationDlg.h"
#include "GStatic.h"
#include "GroupBox.h"
#include "Table.h"
#include "CompositGroupBox.h"

/**
@brief      Login画面
*/
class CLoginDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CLoginDlg)
    DECLARE_MESSAGE_MAP()

public:
	CLoginDlg(CService* pService);                                          // 构造函数
	~CLoginDlg();                                                           // 析构函数

    // 对话框数据
	enum { IDD = IDD_00002_LOGIN_DLG };
	virtual void OnServiceBusy(bool busy);
private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                    // 画面创建函数
    void OnDraw(CDC* pDC);                                                  // 画面绘制函数

	afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                            // 处理按钮消息
	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);							// 编辑内容发生改变时的处理
    bool CheckInput();                                                      // 检查输入操作员ID是否正确
    CXButton* m_btnCommon;				                                    // 普通业务按钮
    CXButton* m_btnMaintain;				                                // 维护业务按钮

};
