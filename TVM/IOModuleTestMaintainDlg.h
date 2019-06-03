#pragma once
#include "OperationDlg.h"
#include "IOModuleTestSvc.h"
/**
@brief      IO���Ի���
*/
class CIOModuleTestMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CIOModuleTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CIOModuleTestMaintainDlg(CService* pService);
	~CIOModuleTestMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };			// �Ի�������

public:
	void UpdateUI();                        //ˢ�½��棨�ⲿ�ӿڣ�
protected:
	void InitMenuUI();                      //��ʼ���˵�����
	void InitStatusUI();                    //��ʼ��������״̬����

	void UpdateMenuData();                  //���½�������
	void UpdateStatusData();                //���´���������
	void Clear();                           //�������
private:
	int DeviceType;
	LABEL_GROUP   m_LableGroup[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
