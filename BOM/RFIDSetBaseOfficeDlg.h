#pragma once
#include "OperationDlg.h"

/**
@brief  RFIDд���趨
*/
class CRFIDSetBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRFIDSetBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRFIDSetBaseOfficeDlg(CService* pService);
	~CRFIDSetBaseOfficeDlg();
	enum { IDD = IDD_20122_RFID_MODE_SET_DLG };                               // �Ի�������

private:
	LABEL_GROUP m_lableRFIDModeInfo[LABEL_GROUP_EDIT_ROWS_2];					// ������ʾ��ģʽ�л���Ϣ

	void InitRFIDModeInfo();                                                  // ��ʼ��������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg LRESULT OnModeItemClick(WPARAM,LPARAM);							// ��ť���º�Ĵ���
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// ����ȷ�ϰ�ť��Ϣ
};
