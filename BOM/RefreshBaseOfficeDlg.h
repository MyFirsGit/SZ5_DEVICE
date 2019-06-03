#pragma once
#include "OperationDlg.h"
#include "RefreshSvc.h"

// CRefreshBaseOfficeDlg 对话框
/**
@brief      激活基本画面
*/
class CRefreshBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRefreshBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CRefreshBaseOfficeDlg(CService* pService);          // 标准构造函数
	~CRefreshBaseOfficeDlg();

	enum { IDD = IDD_00701_REFRESH_BASE_DLG };          // 对话框数据

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnOK(WPARAM,LPARAM);

private:
	LABEL_GROUP m_RefreshInfo[LABEL_GROUP_EDIT_ROWS_4];
	void UpdateUI();
};
