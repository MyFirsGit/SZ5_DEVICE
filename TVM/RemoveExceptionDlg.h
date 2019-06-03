#pragma once
#include "OperationDlg.h"
#include "RemoveExceptionSvc.h"

/**
@brief     �쳣�������
*/
class CRemoveExceptionDlg : public COperationDlg
{
	DECLARE_DYNAMIC(CRemoveExceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	CRemoveExceptionDlg(CService* pService);				// ��׼���캯��
	~CRemoveExceptionDlg();

	// �Ի�������
	enum { IDD = IDD_20701_REMOVE_EXCEPTION_DLG };

public:
	void UpdateUI();                        //ˢ�½��棨�ⲿ�ӿڣ�
protected:
	void InitDialogUI();                    //��ʼ����

	void UpdateDialogData();                //���½�������
	void ClearLabelData();                  //�������
private:
	LABEL_GROUP_FOUR  m_LableGroupFour[_MAX_ROW];

	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);					// ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // ���漤��
};
