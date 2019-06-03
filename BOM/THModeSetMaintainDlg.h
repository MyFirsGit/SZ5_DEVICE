#pragma once
#include "OperationDlg.h"

/**
@brief      ��Ʊģʽ�趨����
*/
class CTHModeSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTHModeSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTHModeSetMaintainDlg(CService* pService);
	~CTHModeSetMaintainDlg();
	enum { IDD = IDD_20120_TH_MODE_SET_DLG };                               // �Ի�������

private:
	LABEL_GROUP m_lableTHModeInfo[LABEL_GROUP_EDIT_ROWS_2];					// ������ʾ��ģʽ�л���Ϣ

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg LRESULT OnModeItemClick(WPARAM,LPARAM);							// ��ť���º�Ĵ���
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// ����ȷ�ϰ�ť��Ϣ
	void InitTHModeInfo();                                                  // ��ʼ��������Ϣ

	int m_CurrentMode;
	int m_SelectedMode;
};
