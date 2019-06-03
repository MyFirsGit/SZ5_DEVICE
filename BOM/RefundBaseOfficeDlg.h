#pragma once

#include "OperationDlg.h"

/**
@brief      �˿��������
*/
class CRefundBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRefundBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRefundBaseOfficeDlg(CService* pService);
	virtual ~CRefundBaseOfficeDlg();

	enum { IDD = IDD_00602_REFUND_BASE_DLG };
protected:
	void UpdateUI();
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnRefundReasonBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnSetRefundDespoitClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);
	LABEL_GROUP m_RefundInfo[LABEL_GROUP_EDIT_ROWS_9];                      // վԱ�˿���Ϣ��������
	void InitRefundInfo();
	BOOL OnInitDialog();
};