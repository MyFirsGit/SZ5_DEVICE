#pragma once
#include "ReceptionDlg.h"

/**
@brief		乘客业务画面
*/
class CTVMFailureRefundReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CTVMFailureRefundReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// 画面创建函数		
	CTVMFailureRefundReceptionDlg(CService* pService);		// 标准构造函数
	~CTVMFailureRefundReceptionDlg();

	// 对话框数据
	enum { IDD = IDD_11601_TVMFAILURE_REFUND_DLG };

private:

};
