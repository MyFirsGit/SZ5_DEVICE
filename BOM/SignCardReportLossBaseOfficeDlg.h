#pragma once
#include "OperationDlg.h"

/**
@brief ��������ʧ-��������
*/

class  CSignCardReportLossBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSignCardReportLossBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSignCardReportLossBaseOfficeDlg(CService* pService);
	~CSignCardReportLossBaseOfficeDlg();

	enum { IDD = IDD_02901_SIGN_CARD_REPORT_LOSS_BASE_DLG };
private:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// �Ի����ʼ��
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);

};