#pragma once
#include "ReceptionDlg.h"
#include "ReceptionStaticInfo.h"

// CAnalyzeConfirmReceptionDlg �Ի���

/**
@brief      ����ȷ�ϻ���
*/
class CAnalyzeConfirmReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CAnalyzeConfirmReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CAnalyzeConfirmReceptionDlg(CService* pService);   // ��׼���캯��
	virtual ~CAnalyzeConfirmReceptionDlg();
	
	void SetReadCardResponse(LPRW_READ_CARD_RESPONSE);

// �Ի�������
	enum { IDD = IDD_10802_ANALYZE_CONFIRM_DLG };
  
protected:
	void UpdateUI(); 
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
