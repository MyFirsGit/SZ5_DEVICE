#pragma once

#include "OperationDlg.h"

/**
@brief      非即时申退查询画面
*/
class CRefundQueryOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRefundQueryOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRefundQueryOfficeDlg(CService* pService);
	~CRefundQueryOfficeDlg();

	enum { IDD = IDD_00602_REFUND_QUERY_DLG };
protected:
	void UpdateUI();
private:
	LABEL_GROUP m_QueryInfo[LABEL_GROUP_EDIT_ROWS_3];                       // 站员查询信息区域数据
	void InitQueryInfo();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	LRESULT OnCertificateTypeBtnClick(WPARAM wParam,LPARAM lParam);
	bool CheckInputStatus();
};