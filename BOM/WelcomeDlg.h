#pragma once
#include "OperationDlg.h"
#include "Lable.h"


/**
@brief      Login����
*/
class CWelcomeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CWelcomeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CWelcomeDlg(CService* pService);                                          // ���캯��
	~CWelcomeDlg();                                                           // ��������

	// �Ի�������
	enum { IDD = IDD_WELCOME_DLG };

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                    // ���洴������
	
	CLable* lblMessage;

};
