#pragma once
#include "OperationDlg.h"

/**
@brief      ��Ʊ��������ʾ����
*/
class CTHTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTHTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTHTestMaintainDlg(CService* pService);
	~CTHTestMaintainDlg();

	enum { IDD = IDD_TH_TEST_DLG };										// �Ի�������

private:
	LABEL_GROUP m_labelTHTestInfo[11];				// ������ʾ�Ĳ�����Ϣ

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// ����ȷ�ϰ�ť��Ϣ
	LRESULT OnReset(WPARAM wParam,LPARAM lParam);						// �������ð�ť��Ϣ
	LRESULT OnSingleTestTypeBtnClick(WPARAM wParam,LPARAM lParam);		// ������԰�ťѡ��
	LRESULT OnDetailTestTypeBtnClick(WPARAM wParam,LPARAM lParam);		// ��ϸ���԰�ťѡ��
	void ShowSensorInfo();

	BOOL OnInitDialog();												// �Ի����ʼ��
	void InitialButtons();												// ��ʼ������
	void InitialBaseInfo();												// ��ʼ��������Ϣ
	void ShowRFIDInfo();												// ��ʾRFID��Ϣ
	void DoRW();														// д��
	bool CheckInputText();												// ���EDIT����������Ƿ�Ϸ�

	WORD        m_wUnitTestFirstNo;										// ����������ͱ��
	WORD        m_wUnitTestSecondNo;									// ��ϸ�������ͱ��
};
