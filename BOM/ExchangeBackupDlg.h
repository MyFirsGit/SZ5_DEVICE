#pragma once

#include "OperationDlg.h"
#include "ExchangeSvc.h"

// CExchangeBackupDlg 对话框

/**
@brief      替换备份画面
*/
class CExchangeBackupDlg : public COperationDlg
{
    DECLARE_DYNAMIC(CExchangeBackupDlg)
    DECLARE_MESSAGE_MAP()

public:
    CExchangeBackupDlg(CService* pService = NULL);      // 标准构造函数
    virtual ~CExchangeBackupDlg();
   
    enum { IDD = IDD_00502_EXCHANGE_BACKUP_DLG };       // 对话框数据
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);
	afx_msg LRESULT XGroupClick(WPARAM,LPARAM);

private:
	bool CheckInput();
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    PGSTATIC_INFO m_purseInfo;                          // 钱包产品数据
    CGStatic* m_purseInfoArea;                          // 钱包产品信息区域

protected:
    void OnDraw(CDC* pDC);
};