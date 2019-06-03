#pragma once

#include "ReceptionDlg.h"
#include "RefundApplySvc.h"

// CARefundReceptionDlg �Ի���

/**
@brief      �˿�ҵ��˿ͻ���
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
