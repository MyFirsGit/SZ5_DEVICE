#pragma once

#include "OperationDlg.h"
#include "ExchangeSvc.h"

// CExchangeBaseOfficeDlg �Ի���

/**
@brief      �滻��������
*/
class CExchangeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CExchangeBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CExchangeBaseOfficeDlg(CService* pService = NULL);   // ��׼���캯��
	virtual ~CExchangeBaseOfficeDlg();
   
	enum { IDD = IDD_00501_EXCHANGE_BASE_DLG };          // �Ի�������
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg LRESULT OnOK(WPARAM,LPARAM);
private:
	bool CheckInput();
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

protected:
    void OnDraw(CDC* pDC);
};
