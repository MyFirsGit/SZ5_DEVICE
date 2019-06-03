#pragma once
#include "OperationDlg.h"
#include "BanknoteAcceptModuleTestSvc.h"
/**
@brief ��������
*/


class CBanknoteAcceptMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CBanknoteAcceptMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CBanknoteAcceptMaintainDlg(CService* pService);
	~CBanknoteAcceptMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

public:
	void UpdateUI();                    //ˢ�½���
protected: 
	void InitMenuUI();                  //��ʼ��Menu����
	void UpdateMenuData();              //����Menu����
	void Clear();
private:
	LABEL_GROUP_FOUR m_labelGroupFour[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};