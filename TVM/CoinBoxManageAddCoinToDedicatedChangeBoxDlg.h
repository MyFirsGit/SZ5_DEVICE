#pragma once
#include "OperationDlg.h"

/**
@brief Ӳ�Ҳ���
*/
class CCoinBoxManageAddCoinToDedicatedChangeBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageAddCoinToDedicatedChangeBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAddCoinToDedicatedChangeBoxDlg(CService* pService);
	~CCoinBoxManageAddCoinToDedicatedChangeBoxDlg();

	enum { IDD = IDD_20112_STATION_SET_DLG };

	void InitDedicatedChangeBoxInfo();
	void ShowDedicatedChangeBoxData();						// ��ʾӲ��ר��������ID������
	void ShowAddingCoinToDedicatedChangeBox();				// ��ʾӲ��ר��������ӱ��н���
	void ShowFinishAddingCoinToDedicatedChangeBox();		// ��ʾӲ��ר��������ӱ���ɽ���
protected:
	virtual void UpdateUI();								// ������ʾ��ɻص�����

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
	CString CashValueToString(BankNoteAndCoinType_t type);
	void ShowTips();										// ��ʾ�ӱ���ʾ
};