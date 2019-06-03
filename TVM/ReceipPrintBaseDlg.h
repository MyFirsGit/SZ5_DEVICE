#pragma once
#include "OperationDlg.h"

/**
@brief ά��ҵ���������
*/
#define MAX_ROW 15


class CReceipPrintBaseDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CReceipPrintBaseDlg)
	DECLARE_MESSAGE_MAP()

public:
	CReceipPrintBaseDlg(CService* pService);
	~CReceipPrintBaseDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };

protected:
	void UpdateUI();

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LABEL_GROUP m_lableGroup[MAX_ROW];
	CReceipPrintSvc* pSvc;

	void ClearDialogData();

	const CString TransFileNameToDisplay(const CString&) const;	// ת���ļ���Ϊ�˵��� 
};
