#pragma once
#include "OperationDlg.h"

/**
@brief ����������-��������
*/

class  CSignCardApplyOverOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSignCardApplyOverOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSignCardApplyOverOfficeDlg(CService* pService);
	~CSignCardApplyOverOfficeDlg();

	enum { IDD = IDD_02802_SIGN_CARD_APPLY_OVER_DLG };
private:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// �Ի����ʼ��
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};