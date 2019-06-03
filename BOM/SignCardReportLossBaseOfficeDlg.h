#pragma once
#include "OperationDlg.h"

/**
@brief 记名卡挂失-基本画面
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

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 对话框初始化
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);

};