#pragma once

#include "OperationDlg.h"
#include "ExchangeSvc.h"

// CExchangeBaseOfficeDlg 对话框

/**
@brief      替换基本画面
*/
class CExchangeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CExchangeBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CExchangeBaseOfficeDlg(CService* pService = NULL);   // 标准构造函数
	virtual ~CExchangeBaseOfficeDlg();
   
	enum { IDD = IDD_00501_EXCHANGE_BASE_DLG };          // 对话框数据
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnOK(WPARAM,LPARAM);
private:
	bool CheckInput();
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

protected:
    void OnDraw(CDC* pDC);
};
