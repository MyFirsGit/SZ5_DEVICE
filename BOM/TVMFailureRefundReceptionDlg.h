#pragma once
#include "ReceptionDlg.h"

/**
@brief		�˿�ҵ����
*/
class CTVMFailureRefundReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CTVMFailureRefundReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���洴������		
	CTVMFailureRefundReceptionDlg(CService* pService);		// ��׼���캯��
	~CTVMFailureRefundReceptionDlg();

	// �Ի�������
	enum { IDD = IDD_11601_TVMFAILURE_REFUND_DLG };

private:

};
