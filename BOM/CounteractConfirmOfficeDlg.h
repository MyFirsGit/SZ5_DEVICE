#pragma once
#include "OperationDlg.h"

/**
@brief      抵消确认画面
*/
class CCounteractConfirmOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCounteractConfirmOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCounteractConfirmOfficeDlg (CService* pService);   // 标准构造函数
	~CCounteractConfirmOfficeDlg();
	enum { IDD = IDD_01102_COUNTERACT_CONFIRM_DLG };
   
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
