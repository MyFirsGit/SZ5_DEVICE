#pragma once
#include "OperationDlg.h"

/**
@brief ��������ʧ-��������
*/

class  CSignCardReportLossOverOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSignCardReportLossOverOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSignCardReportLossOverOfficeDlg(CService* pService);
	~CSignCardReportLossOverOfficeDlg();

	enum { IDD = IDD_02902_SIGN_CARD_REPORT_LOSS_OVER_DLG };
private:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// �Ի����ʼ��
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);

};