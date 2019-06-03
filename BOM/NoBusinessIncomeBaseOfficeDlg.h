#pragma once
#include "OperationDlg.h"

/**
  @brief  ��������-��������
*/
class  CNoBusinessIncomeBaseOfficeDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CNoBusinessIncomeBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()
public:
   CNoBusinessIncomeBaseOfficeDlg(CService* pService);
   ~CNoBusinessIncomeBaseOfficeDlg();

   enum { IDD = IDD_02701_NOBUSINESS_INCOME_BASE_DLG };
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);			// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);							// ����ȷ�ϰ�ť��Ϣ
	LRESULT OnIncomeBtnClick(WPARAM wParam,LPARAM lParam);		// ��������ԭ��ť��Ϣ
	LRESULT OnExpendBtnClick(WPARAM wParam,LPARAM lParam);		// ����֧��ԭ��ť��Ϣ

	virtual BOOL OnInitDialog();									// �Ի����ʼ��
	void InitButtons();												// ��ʼ������
	bool CheckInputValid();											// ���༭�������Ƿ���ȷ

	bool m_BtnChecked;
};