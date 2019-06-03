#pragma once
#include "OperationDlg.h"
#include "RWTestSvc.h"

#define _MAX_ROW 15   //���������ʾ�������ʾ����
#define _MAX_COL  3   //���������ʾ�������ʾ����


class CCardReadWriteMachineTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCardReadWriteMachineTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCardReadWriteMachineTestMaintainDlg(CService* pService);
	virtual ~CCardReadWriteMachineTestMaintainDlg();

	enum {IDD = IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG };

public:
	//��ʼ������
	void InitMainMenuUI();         //��ʼ�����˵���
	void InitBaseMenuUI();         //��ʼ�������˵�ѡ����ܣ�
	void InitParameterUI();        //��ʼ��Parameter��Ϣ��ʾ
	void InitRWInfoUI();           //��ʼ��RW info��Ϣ��ʾ����
	//��������
	void ShowMainMenuData();       //�������˵�ѡ��ֵ
	void ShowBaseMenuData();       //��ʾ�����˵�ѡ��
	void ShowParameterData();      //��ʾParameterֵ
	void ShowRWInfoData();         //��ʾRWInfoֵ

	void InitParameterBaseData(); //��ʼ��Parameter�ľ�ֵ̬

	void ClearData();             //Clear All Data

protected:
	void UpdataUI();
private:
	int DeviceType;
	LABEL_GROUP_THREE m_LabelGroupThree[20];

	//BaseMenu_Info m_BaseMenuInfo[4];//�˵���ѡ��
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	friend class CRWTestSvc;
};