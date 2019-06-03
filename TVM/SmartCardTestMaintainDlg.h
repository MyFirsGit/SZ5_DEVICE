#pragma once
#include "OperationDlg.h"
#include "SmartCardTestSvc.h"
/**
@brief      Smart-Card���Ի���
*/
class CSmartCardTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CSmartCardTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSmartCardTestMaintainDlg(CService* pService);
	~CSmartCardTestMaintainDlg();

	enum { IDD = IDD_TH_TEST_DLG };			// �Ի�������

public:
	void UpdateUI();                        //ˢ�½��棨�ⲿ�ӿڣ�
protected:
	void InitMenuUI();                      //��ʼ���˵�����
	void InitMotorTestMenuUI();             //��ʼ���˵�����
	void InitReadRFIDTestMenuUI();
	void InitWriteRFIDTestMenuUI();
	void InitSensorTestMenuUI();
	void UpdateMenuData();                  //���½�������
	void UpdateMotorTestMenuData();         //���½�������
	void UpdateReadRFIDTestMenuData();      //���½�������
	void UpdateWriteRFIDMenuData();         //���½�������
	void UpdateSensorTestMenuData();        //���½�������
	void Clear();                           //�������
private:
	LABEL_GROUP_FIVE  m_lableGroupFive[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
