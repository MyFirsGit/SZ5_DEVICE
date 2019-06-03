#pragma once
#include "OperationDlg.h"

/**
@brief 记名卡申请-基本画面
*/
static const char			USER_INFO_SIZE			=	9;			// 站员画面显示信息的行数

class  CSignCardApplyBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSignCardApplyBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSignCardApplyBaseOfficeDlg(CService* pService);
	~CSignCardApplyBaseOfficeDlg();
	void  ClickCredentialsTypeButton(int index);
	void  ClickSexButton(int index);
	void  ClickSevTypeButton(int index);
	
	enum { IDD = IDD_02801_SIGN_CARD_APPLY_BASE_DLG };
	virtual BOOL OnInitDialog();										// 对话框初始化
private:
	LRESULT OnSevBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnSexClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnCertificateTypeClick(WPARAM wParam,LPARAM lParam);

	BTN_INFO* pCheckReasonBtnInfo;									    // 功能选择按钮信息数组
	BTN_INFO* pSexBtnInfo;											 	// 性别按钮信息数组
	BTN_INFO* pCertificatetypeBtnInfo;									// 证件类型按钮信息数组
	void InitialButtons();												// 初始化原因按键
	void  Reset();
	bool ValidateInput();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 对话框初始化
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT  OnEditChange(WPARAM wParam,LPARAM lParam);
};