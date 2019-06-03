#pragma once

#include "OperationDlg.h"

/**
@brief      充值基本画面
*/
class CChargeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CChargeBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CChargeBaseOfficeDlg(CService* pService);
	~CChargeBaseOfficeDlg();

	void SetEditAttr();

	enum { IDD = IDD_00302_CHARGE_STAFF_DLG };
protected:
	void UpdateUI();
private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnOK(WPARAM,LPARAM);
    afx_msg LRESULT OnReset(WPARAM,LPARAM);
	LRESULT  OnEditChange(WPARAM wParam,LPARAM lParam);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnPriceBtnClick(WPARAM wParam,LPARAM lParam);
	
	virtual BOOL OnInitDialog();
	void InitBaseInfo();
	bool ValidateInput();

	LABEL_GROUP m_ChargeInfo[LABEL_GROUP_EDIT_ROWS_7];
};
