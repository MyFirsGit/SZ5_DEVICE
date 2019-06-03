#pragma once

#include "ReceptionDlg.h"
#include "RefundApplySvc.h"

// CARefundReceptionDlg 对话框

/**
@brief      退款业务乘客画面
*/
class CARefundReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CARefundReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CARefundReceptionDlg(CService* pService);
	virtual ~CARefundReceptionDlg();

	enum { IDD = IDD_10601_REFUND_DLG };

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void OnDraw(CDC* pDC);
	void InitBaseInfo();
	void UpdateUI();
};
