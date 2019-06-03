#pragma once
#include "OperationDlg.h"

/**
  @brief  TVM�����˿�-��������
*/

static const char        EDIT_COUNT                 = 3;				// �༭��ĸ���

class  CTVMFailureRefundBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CTVMFailureRefundBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
   CTVMFailureRefundBaseOfficeDlg(CService* pService);
   ~CTVMFailureRefundBaseOfficeDlg();

   bool CheckValid();													// ���������Ч��

   enum { IDD = IDD_01601_TVMFAILURE_REFUND_BASE_DLG };

   CString m_strTransNum;												// ������ˮ�� 
   CString m_strDeviceCode;												// �豸����

private:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// �Ի����ʼ��
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);
public:
	virtual BOOL OnInitDialog();
	

};