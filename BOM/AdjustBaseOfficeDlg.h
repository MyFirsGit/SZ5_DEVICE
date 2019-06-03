#pragma once

#include "OperationDlg.h"

// CChargeBaseOfficeDlg 对话框

/**
@brief      充值基本画面
*/
class CAdjustBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CAdjustBaseOfficeDlg)
    DECLARE_MESSAGE_MAP()

public:
	CAdjustBaseOfficeDlg(CService* pService);   // 标准构造函数
	~CAdjustBaseOfficeDlg();

    // 对话框数据
	enum { IDD = IDD_00204_ADJUST_BASE_DLG };
protected:
	void UpdateUI();
private:
	void BuildPayMathodGroup();
	void BuildOverTimeGroup();
	void InitBaseInfo();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnEditChange(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT OnOK(WPARAM,LPARAM);
   // afx_msg LRESULT OnReset(WPARAM,LPARAM);
	LRESULT OnLineClick(WPARAM,LPARAM);
	LRESULT OnPayMathodClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnStationClick(WPARAM,LPARAM);
	LRESULT OnOverTimeReasonClick(WPARAM wParam,LPARAM lParam);
	void BuildLineGroup();
	bool ValidateInput();
	LABEL_GROUP m_AdjustInfo[LABEL_GROUP_EDIT_ROWS_8];

	int m_AdjustAmount;
	TicketType_t m_TicketType;

};
