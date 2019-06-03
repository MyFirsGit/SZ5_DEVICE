#pragma once
#include "OperationDlg.h"
#include "TokenModuleTestSvc.h"
/**
@brief ��������
*/

#define _MAX_ROW 15   //���������ʾ�������ʾ����
#define _MAX_COL  4   //���������ʾ�������ʾ����

class CTokenModuleMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CTokenModuleMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTokenModuleMaintainDlg(CService* pService);
	~CTokenModuleMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

public:
	void UpdateUI();              //ˢ�½��棨�ⲿ�ӿڣ�
protected:
	//����
	void InitMenuUI();            //��ʼ���˵�����
	void InitReadRfidUI();        //��ʼ����RFID����
	void InitWriteRfidUI();       //��ʼ��дRFID����
	//����
	void UpdateMenuData();        //���²˵���ʾ���
	void UpdateReadRfidData();    //���¶�RFID����
	void UpdateWriteRfidData();   //����дRFID����
	void Clear();                 //�����ʾ����
private:
	LABEL_GROUP_FOUR  m_labelGroupFour[_MAX_ROW];  //����������ʾ��

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                       //���洴��
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);    //���漤��
};