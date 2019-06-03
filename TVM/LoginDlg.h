#pragma once
#include "OperationDlg.h"
#include "GStatic.h"
#include "GroupBox.h"
#include "Table.h"
#include "CompositGroupBox.h"

/**
@brief ��½����
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// ���洴��
	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);				// �༭����Ӧ

	bool CheckLength();											// ����˺����볤��
	void DoLogin(DWORD authority);								// ��½
	void OnExceptionItemNotify(CSysException& e,bool isInsert);	// �쳣֪ͨ

	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);//������Ӧ
	//BOOL PreTranslateMessage(MSG* pMsg);

	bool m_account_flag; // ������ʶ�˺ų����Ƿ��һ��Ϊ6
	LABEL_GROUP m_labelGroup[LABEL_GROUP_ROWS_15];
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};