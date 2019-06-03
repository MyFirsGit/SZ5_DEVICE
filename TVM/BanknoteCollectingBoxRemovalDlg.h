#pragma once
#include "BanknoteCollectingBoxRemovalSvc.h"

class CBanknoteCollectingBoxRemovalDlg  : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteCollectingBoxRemovalDlg)
	DECLARE_MESSAGE_MAP()
public:
	CBanknoteCollectingBoxRemovalDlg(CService* pService);
	~CBanknoteCollectingBoxRemovalDlg(void);

	enum { IDD = IDD_20113_STATION_SET_END_DLG }; //?? why use this dialog??

	// ��ʼ����ʽ
	//void InitBaseUI();
	//void InitAttachBoxUI();
	void InitDetachBoxUI();

	// ��ʾ����
	//void ShowBaseData();
	//void ShowAttachBoxData();
	void ShowDetachBoxData();

	// �������
	void Clear();

protected:
	void UpdateUI();

private:

	CBanknoteCollectingBoxRemovalSvc* pSvc;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};

