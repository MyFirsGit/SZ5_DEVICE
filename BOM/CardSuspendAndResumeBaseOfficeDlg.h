#pragma once
#include "OperationDlg.h"

/**
@brief      挂失解挂基本界面
*/
class CCardSuspendAndResumeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCardSuspendAndResumeBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCardSuspendAndResumeBaseOfficeDlg(CService* pService);
	~CCardSuspendAndResumeBaseOfficeDlg();

	enum { IDD = CARD_SUSPEND_AND_RESUME_BASE_DLG };				// 对话框数据

private:
	typedef enum _userinfo_edit_group {									 // 用户信息对应的EDITGROUP
		USERINFO_EDIT_GROUP_USER_NAME = 0,
		USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER = 1,
	} USERINFO_EDIT_GROUP;

	void InitialButtons();												// 初始化按键
	bool ValidateInput();												// 输入有效性检查

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 画面创建函数
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// 处理确认按钮消息
	afx_msg LRESULT OnReset(WPARAM,LPARAM);								// 处理重置按钮消息

	LRESULT OnSevBtnClick(WPARAM wParam,LPARAM lParam);					// 挂失解挂按钮
	LRESULT OnSexTypeBtnClick(WPARAM wParam,LPARAM lParam);				// 性别按钮
	LRESULT OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam);		// 证件类型按钮

	BOOL OnInitDialog();												// 对话框初始化
};