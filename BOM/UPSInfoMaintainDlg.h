#pragma once
#include "OperationDlg.h"

/**
@brief      UPS��Ϣ��ʾ����
*/
class CUPSInfoMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CUPSInfoMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CUPSInfoMaintainDlg(CService* pService);
	~CUPSInfoMaintainDlg();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// ���洴������

	enum { IDD = IDD_UPS_INFO_DLG };								// �Ի�������

private:
	LABEL_GROUP m_labelUPSInfo[LABEL_GROUP_EDIT_ROWS_2];				// ������ʾ��UPS��Ϣ

	void InitLabelUPSInfo();											// ��ʼ��BASE����LABEL���������
};
