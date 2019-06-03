#pragma once
#include "OperationDlg.h"

/**
@brief      �豸ģʽ�л�����
*/
class CDeviceModeSwitchDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CDeviceModeSwitchDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDeviceModeSwitchDlg(CService* pService);                               // ��׼���캯��
	~CDeviceModeSwitchDlg();
	enum { IDD = IDD_20116_MODE_SWITCH_DLG };                               // �Ի�������

private:
	LABEL_GROUP m_modeSwitchInfo[LABEL_GROUP_EDIT_ROWS_2];                       // ������ʾ��ģʽ�л���Ϣ

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);									// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnModeItemClick(WPARAM,LPARAM);							// ��ť���º�Ĵ���
	void InitModeInfo();                                                    // ��ʼ��������Ϣ

	int m_CurrentMode;
	int m_SelectedMode;
};
