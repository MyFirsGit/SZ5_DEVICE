#pragma once

#include "ReceptionDlg.h"

// CRefreshReceptionDlg �Ի���

/**
@brief      �˿ͼ���ҵ����
*/
class CDeferReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CDeferReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CDeferReceptionDlg(CService* pService);           // ��׼���캯��
	virtual ~CDeferReceptionDlg();
	// �Ի�������
	enum { IDD = IDD_11701_DEFER_DLG };     
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	void UpdateUI();
	void OnDraw(CDC* pDC);
};
