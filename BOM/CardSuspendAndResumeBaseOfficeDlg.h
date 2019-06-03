#pragma once
#include "OperationDlg.h"

/**
@brief      ��ʧ��һ�������
*/
class CCardSuspendAndResumeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCardSuspendAndResumeBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCardSuspendAndResumeBaseOfficeDlg(CService* pService);
	~CCardSuspendAndResumeBaseOfficeDlg();

	enum { IDD = CARD_SUSPEND_AND_RESUME_BASE_DLG };				// �Ի�������

private:
	typedef enum _userinfo_edit_group {									 // �û���Ϣ��Ӧ��EDITGROUP
		USERINFO_EDIT_GROUP_USER_NAME = 0,
		USERINFO_EDIT_GROUP_CERTIFICATE_NUMBER = 1,
	} USERINFO_EDIT_GROUP;

	void InitialButtons();												// ��ʼ������
	bool ValidateInput();												// ������Ч�Լ��

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);								// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnReset(WPARAM,LPARAM);								// �������ð�ť��Ϣ

	LRESULT OnSevBtnClick(WPARAM wParam,LPARAM lParam);					// ��ʧ��Ұ�ť
	LRESULT OnSexTypeBtnClick(WPARAM wParam,LPARAM lParam);				// �Ա�ť
	LRESULT OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam);		// ֤�����Ͱ�ť

	BOOL OnInitDialog();												// �Ի����ʼ��
};