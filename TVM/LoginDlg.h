#pragma once
#include "OperationDlg.h"
#include "GStatic.h"
#include "GroupBox.h"
#include "Table.h"
#include "CompositGroupBox.h"

/**
@brief 登陆画面
*/
class CLoginDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CLoginDlg)
	DECLARE_MESSAGE_MAP()

public:
	CLoginDlg(CService* pService);
	~CLoginDlg();

	enum { IDD = IDD_00002_LOGIN_DLG };

	void SetEditFocusOnSvc(int edIndex = 0);
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// 画面创建
	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);				// 编辑框响应

	bool CheckLength();											// 检查账号密码长度
	void DoLogin(DWORD authority);								// 登陆
	void OnExceptionItemNotify(CSysException& e,bool isInsert);	// 异常通知

	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);//按键响应
	//BOOL PreTranslateMessage(MSG* pMsg);

	bool m_account_flag; // 用来标识账号长度是否第一次为6
	LABEL_GROUP m_labelGroup[LABEL_GROUP_ROWS_15];
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};