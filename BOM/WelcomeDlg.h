#pragma once
#include "OperationDlg.h"
#include "Lable.h"


/**
@brief      Login画面
*/
class CWelcomeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CWelcomeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CWelcomeDlg(CService* pService);                                          // 构造函数
	~CWelcomeDlg();                                                           // 析构函数

	// 对话框数据
	enum { IDD = IDD_WELCOME_DLG };

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                    // 画面创建函数
	
	CLable* lblMessage;

};
