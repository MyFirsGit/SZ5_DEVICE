#pragma once
#include "OperationDlg.h"

/**
@brief ��������
*/
class CCoinBoxManageAddCoinToCycleChangeBoxDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinBoxManageAddCoinToCycleChangeBoxDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinBoxManageAddCoinToCycleChangeBoxDlg(CService* pService);
	~CCoinBoxManageAddCoinToCycleChangeBoxDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

	void InitCycleChangeBoxInfo();
	void ShowCycleChangeBoxData();												// ��ʾӲ��ѭ��������ID������
	void ShowCycleChangeBoxAddingCoin();										// ��ʾӲ��ѭ��������ӱ��н���
	void ShowFinishCycleChangeBoxAddingCoin();									// ��ʾӲ��ѭ��������ӱ���ɽ���
protected:
	virtual void UpdateUI();													// ������ʾ��ɻص�����
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	LABEL_GROUP_SEVEN m_lableGroup[15];
};
