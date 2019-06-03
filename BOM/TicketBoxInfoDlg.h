#pragma once
#include "OperationDlg.h"

class CTicketBoxInfoDlg: public COperationDlg
{
	DECLARE_DYNAMIC(CTicketBoxInfoDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTicketBoxInfoDlg(CService* pService);          // ��׼���캯��
	~CTicketBoxInfoDlg();

	enum { IDD = IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG };          // �Ի�������

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnEditChange(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMoveTypeBtnClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnBoxTypeBtnClick(WPARAM wParam,LPARAM lParam);

private:
	LABEL_GROUP_THREE m_TicketInfoLabel[LABEL_GROUP_EDIT_ROWS_4];					// ������ʾ��ģʽ�л���Ϣ
	void GetTicketInfoBeforeLabel();
	void GetTicketInfoAfterLabel();
	void InitialButtons();
	void InitialEdit();
	void InitialBoxButtons();

	BOOL ValidateCheck();

	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);	
	void UpdateUI();
	void HideAllComponent();
};