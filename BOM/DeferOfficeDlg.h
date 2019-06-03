#pragma once
#include "OperationDlg.h"

// CRefreshBaseOfficeDlg �Ի���
/**
@brief      �����������
*/
class CDeferOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CDeferOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDeferOfficeDlg(CService* pService);          // ��׼���캯��
	~CDeferOfficeDlg();

	enum { IDD = IDD_03101_DEFER_DLG };          // �Ի�������

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
protected:
	void UpdateUI();
private:
	LABEL_GROUP m_DeferInfo[LABEL_GROUP_EDIT_ROWS_6];
	LRESULT OnEditChange(WPARAM,LPARAM);
	bool ValidateInput();
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	
};
