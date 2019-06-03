#pragma once

#include "ReceptionDlg.h"
#include "RefundSvc.h"

// CRefundReceptionDlg 对话框

/**
@brief      退款业务乘客画面
*/
class CRefundReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CRefundReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CRefundReceptionDlg(CService* pService);
	virtual ~CRefundReceptionDlg();

	enum { IDD = IDD_10601_REFUND_DLG };

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    void OnDraw(CDC* pDC);
	void UpdateUI();
private:
	void InitBaseInfo();
};
