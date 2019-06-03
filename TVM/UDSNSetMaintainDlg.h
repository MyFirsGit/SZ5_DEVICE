#pragma once
#include "OperationDlg.h"
#include "GStatic.h"

/**
@brief      UDSN��Ϣ���뻭��
*/
class CUDSNSetMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CUDSNSetMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CUDSNSetMaintainDlg(CService* pService);
	~CUDSNSetMaintainDlg();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnReset(WPARAM,LPARAM);								// �������ð�ť��Ϣ
	enum { IDD = IDD_20119_UDSN_SET_DLG };								// �Ի�������

private:
	typedef enum _udsninfo_edit_group {									 // ��ˮ�Ŷ�Ӧ��EDITGROUP
		UDSNINFO_EDIT_GROUP_ACCUDSN = 0,
		UDSNINFO_EDIT_GROUP_AFCUDSN = 1,
	} UDSNINFO_EDIT_GROUP;

	LABEL_GROUP m_labelUDSNinfo[LABEL_GROUP_EDIT_ROWS_3];				// ������ʾ����ˮ����Ϣ

	void InitLabelUDSNInfo();											// ��ʼ��BASE����LABEL���������
	void InitEditTextUDSNInfo();										// ��ʼ��BASE����EDIT�����������
	void SetUDSNInfoToSvc();											// ��EDIT���������ݵı��浽SVC����
	bool CheckInputText();												// ���EDIT����������Ƿ�Ϸ�

};
