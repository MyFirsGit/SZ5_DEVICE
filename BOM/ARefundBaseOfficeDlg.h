#pragma once

#include "OperationDlg.h"

/**
@brief      �˿��������
*/
class CARefundBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CARefundBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CARefundBaseOfficeDlg(CService* pService);
	virtual ~CARefundBaseOfficeDlg();

	enum { IDD = IDD_00602_REFUND_BASE_DLG };
private:
	
	void InitializeRefundInfo();                                            // ��ʼ���˿����Ϣ

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnRefundReasonClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnSetRefundDespoitClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);
	BOOL OnInitDialog();
	void UpdateUI();
	LABEL_GROUP m_RefundInfo[LABEL_GROUP_EDIT_ROWS_10];                     // վԱ�˿���Ϣ��������
};