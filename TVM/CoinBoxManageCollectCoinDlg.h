#pragma once
#include "OperationDlg.h"

/**
@brief ��������
*/
class CCoinBoxManageCollectCoinDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageCollectCoinDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageCollectCoinDlg(CService* pService);
	~CCoinBoxManageCollectCoinDlg();

	enum { IDD = IDD_20114_COMMUNICATION_SET_DLG };

	void InitCollectCoinInfo();
	void ShowCollectCoinInfo();								// ��ʾӲ�һ�������ֵ��ID������
	void ShowCollectingCoin();								// ��ʾӲ�һ��������Ӳ��ʱ��ֵ��ID������			
	void ShowFinishCollectCoin();							// ��ʾ���Ӳ�һ���

protected:
	virtual void UpdateUI();								// ������ʾ��ɻص�����

private:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};