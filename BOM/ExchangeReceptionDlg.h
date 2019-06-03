#pragma once
#include "ExchangeSvc.h"
#include "ReceptionDlg.h"

// CExchangeReceptionDlg 对话框

/**
@brief      替换业务乘客画面
*/
class CExchangeReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CExchangeReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CExchangeReceptionDlg(CService* pService);   // 标准构造函数
	virtual ~CExchangeReceptionDlg();

    // 对话框数据
	enum { IDD = IDD_10501_EXCHANGE_DLG };

	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void ShowCompoment(ULONG ulCompomentFlag);                      // 显示组件
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
    void OnDraw(CDC* pDC);
};
