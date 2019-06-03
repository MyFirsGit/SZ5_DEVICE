#pragma once
#include "ReceptionDlg.h"
#include "ChargeSvc.h"

/**
@brief      乘客充值业务画面
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

	LRESULT OnOK(WPARAM wParam,LPARAM lParam);					// 点击取消按钮
	LRESULT OnOkAndCancel1(WPARAM wParam,LPARAM lParam);		// 点击确定按钮(确定与取消按钮中的确定按钮）
	LRESULT OnOkAndCancel2(WPARAM wParam,LPARAM lParam);		// 点击取消按钮(确定与取消按钮中的取消按钮）
	afx_msg LRESULT OnNotifyCharge(WPARAM wParam,LPARAM lParam);// 收到充值通知
	afx_msg LRESULT OnUpdateGuide(WPARAM wParam,LPARAM lParam);

	virtual void preShow();
	virtual void showComplete();
private:

	CChargeSvc* pSvc;
	LANGUAGETYPE_T language_mode;

	void InitializeBaseInfo();
	void InitializeGuideArea();
	void UpdateUI();
	LABEL_GROUP m_ChargeInfoGroup[MAX_CHARGE_INFO_ROWS];	// 车票信息区域数据

	CString BanknoteShowword();
	CString CoinShowword();
	void	ShowGuideMsgOfPayInfo();						// 点击确认后显示支付信息
	bool m_bIsShowFinshedInsertMoney;						// 是否完成投钱
	bool m_bIsShowCoinMaxCount;		// 是否显示硬币最大数量
	bool m_bIsShowBanknoteMaxCount; // 是否显示纸币最大数量
	void InitDataModel();			// 初始化数据模型
	void ClearLabelData();
	CImage m_bkgImage;

};
