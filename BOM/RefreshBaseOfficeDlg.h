#pragma once
#include "OperationDlg.h"
#include "RefreshSvc.h"

// CRefreshBaseOfficeDlg �Ի���
/**
@brief      �����������
*/
class CRefreshBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRefreshBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CRefreshBaseOfficeDlg(CService* pService);          // ��׼���캯��
	~CRefreshBaseOfficeDlg();

	enum { IDD = IDD_00701_REFRESH_BASE_DLG };          // �Ի�������

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnOK(WPARAM,LPARAM);

private:
	LABEL_GROUP m_RefreshInfo[LABEL_GROUP_EDIT_ROWS_4];
	void UpdateUI();
};
