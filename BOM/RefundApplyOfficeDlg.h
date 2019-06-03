#pragma once

#include "OperationDlg.h"

/**
@brief      �Ǽ�ʱ�˿����뻭��
*/
class CRefundApplyOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRefundApplyOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRefundApplyOfficeDlg(CService* pService);
	~CRefundApplyOfficeDlg();


	enum { IDD = IDD_00602_REFUND_APPLY_DLG };
protected:

	void UpdateUI();

private:
	void InitializeApplyInfo();                                             // ��ʼ�����뻭����Ϣ

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT XButtonClick(WPARAM,LPARAM);
	LRESULT OnSexTypeBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnRefundReasonBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnApplyReasonBtnClick(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);
	bool CheckInputStatus();
	void HideGUIModule(ULONG ulCompomentFlag);

	LABEL_GROUP m_ApplyInfo[LABEL_GROUP_EDIT_ROWS_10];                      // վԱ������Ϣ��������
	CXButton* m_btnBadCardApply;				// �������˰�ť
	CXButton* m_btnSuspendCardApply;			// ��ʧ���˰�ť
};