#pragma once

#include "ReceptionDlg.h"
#include "RefundSvc.h"

// CRefundReceptionDlg �Ի���

/**
@brief      �˿�ҵ��˿ͻ���
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
