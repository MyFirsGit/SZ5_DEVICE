#pragma once

#include "ReceptionDlg.h"

/**
@brief      �˿ͳ�ֵҵ����
*/
class CChargeReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CChargeReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CChargeReceptionDlg(CService* pService);
	virtual ~CChargeReceptionDlg();

	enum { IDD = IDD_10301_CHARGE_DLG };
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	void UpdateUI();
};
