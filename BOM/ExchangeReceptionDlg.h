#pragma once
#include "ExchangeSvc.h"
#include "ReceptionDlg.h"

// CExchangeReceptionDlg �Ի���

/**
@brief      �滻ҵ��˿ͻ���
*/
class CExchangeReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CExchangeReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CExchangeReceptionDlg(CService* pService);   // ��׼���캯��
	virtual ~CExchangeReceptionDlg();

    // �Ի�������
	enum { IDD = IDD_10501_EXCHANGE_DLG };

	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void ShowCompoment(ULONG ulCompomentFlag);                      // ��ʾ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
    void OnDraw(CDC* pDC);
};
