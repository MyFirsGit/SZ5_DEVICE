#pragma once
#include "OperationDlg.h"

/**
@brief      �������ѡ����
*/
class CRemoveExceptionDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRemoveExceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionDlg(CService* pService);				// ��׼���캯��
	~CRemoveExceptionDlg();

	// �Ի�������
	enum { IDD = IDD_20701_REMOVE_EXCEPTION_DLG };

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);					// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT XButtonClick(WPARAM,LPARAM);
	void InitExceptionInfo();
	void RefreshExceptionInfo();							// ˢ���쳣�б�

	CXButton* m_btnPageUp;
	CXButton* m_btnPageDown;
	LABEL_GROUP_FOUR m_LabelExceptionInfo[11];
	vector<CSysException> m_ExceptionList;
	vector<CSysException>::size_type m_ExceptionNum;
	int m_TotalPageNum;
	int m_CurrentPageNum;
};
