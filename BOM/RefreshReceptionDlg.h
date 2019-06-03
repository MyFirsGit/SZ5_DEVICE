#pragma once

#include "ReceptionDlg.h"

// CRefreshReceptionDlg 对话框

/**
@brief      乘客激活业务画面
*/
class CRefreshReceptionDlg : public CReceptionDlg
{
    DECLARE_DYNAMIC(CRefreshReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
    CRefreshReceptionDlg(CService* pService);           // 标准构造函数
    virtual ~CRefreshReceptionDlg();
    // 对话框数据
	enum { IDD = IDD_10701_REFRESH_DLG };     
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	void UpdateUI();
};
