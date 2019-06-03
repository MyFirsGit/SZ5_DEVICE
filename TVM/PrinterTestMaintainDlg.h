#pragma once
#include "OperationDlg.h"
#include "PrinterTestSvc.h"

// CPrinterTestMaintainDlg �Ի���

/**
@brief      ��ӡ�����Ի���
*/
class CPrinterTestMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CPrinterTestMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CPrinterTestMaintainDlg(CService* pService);				// ��׼���캯��
	~CPrinterTestMaintainDlg();

	// �Ի�������
	enum { IDD = IDD_20531_PRINTER_TEST_DLG };

public:
	void UpdateUI();                        //ˢ�½��棨�ⲿ�ӿڣ�
protected:
	void InitMenuUI();                      //��ʼ���˵�����
	void UpdateMenuData();                  //���½�������
	void Clear();                           //�������
private:
	LABEL_GROUP m_LableGroup[_MAX_ROW];

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
