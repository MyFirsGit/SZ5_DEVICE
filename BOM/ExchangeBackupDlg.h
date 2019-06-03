#pragma once

#include "OperationDlg.h"
#include "ExchangeSvc.h"

// CExchangeBackupDlg �Ի���

/**
@brief      �滻���ݻ���
*/
class CExchangeBackupDlg : public COperationDlg
{
    DECLARE_DYNAMIC(CExchangeBackupDlg)
    DECLARE_MESSAGE_MAP()

public:
    CExchangeBackupDlg(CService* pService = NULL);      // ��׼���캯��
    virtual ~CExchangeBackupDlg();
   
    enum { IDD = IDD_00502_EXCHANGE_BACKUP_DLG };       // �Ի�������
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);
	afx_msg LRESULT XGroupClick(WPARAM,LPARAM);

private:
	bool CheckInput();
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    PGSTATIC_INFO m_purseInfo;                          // Ǯ����Ʒ����
    CGStatic* m_purseInfoArea;                          // Ǯ����Ʒ��Ϣ����

protected:
    void OnDraw(CDC* pDC);
};