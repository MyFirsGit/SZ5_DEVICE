#pragma once
#include "OperationDlg.h"
#include "GStatic.h"

// CPrinterTestMaintainDlg �Ի���

/**
@brief      ��ӡ�����Ի���
*/
class CPrinterTestMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CPrinterTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestMaintainDlg(CService* pService);				// ��׼���캯��
	~CPrinterTestMaintainDlg();

	// �Ի�������
	enum { IDD = IDD_20531_PRINTER_TEST_DLG };
private:

public:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);						// ����ȷ�ϰ�ť��Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// ���崴������
};
