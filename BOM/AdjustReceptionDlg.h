#pragma once

#include "ReceptionDlg.h"

/**
@brief      �˿ͻ���
*/
class CAdjustReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CAdjustReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CAdjustReceptionDlg(CService* pService);   // ��׼���캯��
	virtual ~CAdjustReceptionDlg();

	// �Ի�������
	enum { IDD = IDD_10201_ADJUST_DLG };

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void InitBaseInfo();
	void UpdateUI();
};
