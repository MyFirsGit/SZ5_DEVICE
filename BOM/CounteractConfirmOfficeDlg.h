#pragma once
#include "OperationDlg.h"

/**
@brief      ����ȷ�ϻ���
*/
class CCounteractConfirmOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCounteractConfirmOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCounteractConfirmOfficeDlg (CService* pService);   // ��׼���캯��
	~CCounteractConfirmOfficeDlg();
	enum { IDD = IDD_01102_COUNTERACT_CONFIRM_DLG };
   
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
