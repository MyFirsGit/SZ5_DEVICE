#pragma once
#include "OperationDlg.h"
#include "GStatic.h"

// CMoneyBoxTestMaintainDlg �Ի���

/**
@brief      Ǯ����Ի���
*/
class CMoneyBoxTestMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CMoneyBoxTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMoneyBoxTestMaintainDlg(CService* pService);			// ��׼���캯��
	~CMoneyBoxTestMaintainDlg();

	// �Ի�������
	enum { IDD = IDD_20541_MONEYBOX_TEST_DLG };
private:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);					// ȷ����ť��Ӧ����	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���崴������
};
