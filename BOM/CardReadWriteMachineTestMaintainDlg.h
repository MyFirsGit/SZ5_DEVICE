#pragma once
#include "OperationDlg.h"

/**
@brief      ��д�����Ի���
*/
class CCardReadWriteMachineTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCardReadWriteMachineTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCardReadWriteMachineTestMaintainDlg(CService* pService);			// ��׼���캯��
	~CCardReadWriteMachineTestMaintainDlg();

	// �Ի�������
	enum { IDD = IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG };

	LABEL_GROUP m_CardReadWriteInfo[LABEL_GROUP_EDIT_ROWS_10];			// ��д��������Ϣ
private:
	BOOL OnInitDialog();												// �Ի����ʼ��
	void InitialButtons();												// ��ʼ������

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// ȷ����ť����Ӧ����
	LRESULT OnRWTestBtnClick(WPARAM wParam,LPARAM lParam);				// ��д�����Ͱ�ťѡ��
	LRESULT OnRWDetailTestBtnClick(WPARAM wParam,LPARAM lParam);		// ��д����ϸ���԰�ťѡ��

	WORD m_RWType;
	WORD m_RWDetailType;
};
