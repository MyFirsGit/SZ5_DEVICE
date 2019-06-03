#pragma once
#include "OperationDlg.h"

/**
@brief 记名卡挂失-结束画面
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

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// 对话框初始化
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);

};