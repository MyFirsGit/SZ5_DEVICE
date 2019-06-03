#pragma once
#include "OperationDlg.h"
#include "GStatic.h"
#include "GroupBox.h"
#include "Table.h"
#include "CompositGroupBox.h"

/**
@brief      Login����
*/
class CLoginDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CLoginDlg)
    DECLARE_MESSAGE_MAP()

public:
	CLoginDlg(CService* pService);                                          // ���캯��
	~CLoginDlg();                                                           // ��������

    // �Ի�������
	enum { IDD = IDD_00002_LOGIN_DLG };
	virtual void OnServiceBusy(bool busy);
private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                    // ���洴������
    void OnDraw(CDC* pDC);                                                  // ������ƺ���

	afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                            // ����ť��Ϣ
	afx_msg LRESULT OnEditChange(WPARAM,LPARAM);							// �༭���ݷ����ı�ʱ�Ĵ���
    bool CheckInput();                                                      // ����������ԱID�Ƿ���ȷ
    CXButton* m_btnCommon;				                                    // ��ͨҵ��ť
    CXButton* m_btnMaintain;				                                // ά��ҵ��ť

};
