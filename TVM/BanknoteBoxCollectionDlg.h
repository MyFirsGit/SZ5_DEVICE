#pragma once
#include "OperationDlg.h"

/**
@brief ֽ��ģ����ս���
*/
class CBanknoteBoxCollectionDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteBoxCollectionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteBoxCollectionDlg(CService* pService);
	~CBanknoteBoxCollectionDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG }; 

	// ��ʼ����ʽ
	void InitBaseUI();
	/*void InitAttachBoxUI();
	void InitDetachBoxUI();*/

	// ��ʾ����
	void ShowBaseData();
	/*void ShowAttachBoxData();
	void ShowDetachBoxData();*/

	// �������
	void Clear();

protected:
	void UpdateUI();

private:
	CBanknoteBoxCollectionSvc* pSvc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};