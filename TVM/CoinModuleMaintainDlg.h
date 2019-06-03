#pragma once
#include "OperationDlg.h"
#include "CoinModuleTestSvc.h"
/**
@brief ��������
*/

#define _MAX_ROW 15   //���������ʾ�������ʾ����
#define _MAX_COL  3   //���������ʾ�������ʾ����

class CCoinModuleMaintainDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CCoinModuleMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CCoinModuleMaintainDlg(CService* pService);
	~CCoinModuleMaintainDlg();

	enum { IDD = IDD_20113_STATION_SET_END_DLG };

public:
	void UpdateUI();              //�ṩ��������Ĺ��ýӿ�
protected:
	void InitMenuUI();            //��ʼ���˵�����
	void InitReturnCoinUI();      //��ʼ���˱ҽ���
	void InitClearCoinUI();       //��ʼ����ҽ���
	void InitSafeLockCtrlUI();    //��ʼ����ȫ�ۿ��ƽ���
	void InitReadIdUI();          //��ʼ����RFID����
	void InitWriteIdUI();         //��ʼ��дRFID����
	void InitAddCoinUI();         //��ʼ���ӱҽ���

	void UpdateMenuData();        //���²˵���������
	void UpdateReturnCoinData();  //�����˱ҽ�������
	void UpdateClearCoinData();   //������ҽ�������
	void UpdateSafeLockCtrlData();//���°�ȫ�ۿ��ƽ�������
	void UpdateReadIdData();      //���¶�RFID��������
	void UpdateWriteIdData();     //����дRFID��������	
	void UpdateAddCoinData();     //���¼ӱҲ�����������

	void Clear();                 //1�����ʾ����
private:
	LABEL_GROUP_THREE m_labelGroupThree[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};