#pragma once

#include "ReceptionDlg.h"

// CRefreshReceptionDlg �Ի���

/**
@brief      �˿ͼ���ҵ����
*/
class CRefreshReceptionDlg : public CReceptionDlg
{
    DECLARE_DYNAMIC(CRefreshReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
    CRefreshReceptionDlg(CService* pService);           // ��׼���캯��
    virtual ~CRefreshReceptionDlg();
    // �Ի�������
	enum { IDD = IDD_10701_REFRESH_DLG };     
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	void UpdateUI();
};
