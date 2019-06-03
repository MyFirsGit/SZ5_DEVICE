#pragma once
#include "ReceptionDlg.h"
#include "ChargeSvc.h"

/**
@brief      �˿ͳ�ֵҵ����
*/
class CChargeReceptionDlg : public CReceptionDlg
{
	DECLARE_DYNAMIC(CChargeReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:

	CChargeReceptionDlg(CService* pService);
	virtual ~CChargeReceptionDlg();

	enum { IDD = IDD_00301_CHARGE_RECEPT_DLG };
	enum { MAX_CHARGE_INFO_ROWS = 13 };

protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNotifyAccepted(WPARAM wParam,LPARAM lParam);

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// ���ȡ����ť
	LRESULT OnOkAndCancel1(WPARAM wParam,LPARAM lParam);		// ���ȷ����ť(ȷ����ȡ����ť�е�ȷ����ť��
	LRESULT OnOkAndCancel2(WPARAM wParam,LPARAM lParam);		// ���ȡ����ť(ȷ����ȡ����ť�е�ȡ����ť��
	afx_msg LRESULT OnNotifyCharge(WPARAM wParam,LPARAM lParam);// �յ���ֵ֪ͨ
	afx_msg LRESULT OnUpdateGuide(WPARAM wParam,LPARAM lParam);

	virtual void preShow();
	virtual void showComplete();
private:

	CChargeSvc* pSvc;
	LANGUAGETYPE_T language_mode;

	void InitializeBaseInfo();
	void InitializeGuideArea();
	void UpdateUI();
	LABEL_GROUP m_ChargeInfoGroup[MAX_CHARGE_INFO_ROWS];	// ��Ʊ��Ϣ��������

	CString BanknoteShowword();
	CString CoinShowword();
	void	ShowGuideMsgOfPayInfo();						// ���ȷ�Ϻ���ʾ֧����Ϣ
	bool m_bIsShowFinshedInsertMoney;						// �Ƿ����ͶǮ
	bool m_bIsShowCoinMaxCount;		// �Ƿ���ʾӲ���������
	bool m_bIsShowBanknoteMaxCount; // �Ƿ���ʾֽ���������
	void InitDataModel();			// ��ʼ������ģ��
	void ClearLabelData();
	CImage m_bkgImage;

};
