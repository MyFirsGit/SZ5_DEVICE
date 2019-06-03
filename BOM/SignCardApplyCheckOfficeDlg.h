#pragma once
#include "OperationDlg.h"

/**
@brief ����������-��������
*/
static const char			USER_INFO_CHECK_SIZE    =	2;			// վԱ��ѯ������ʾ��Ϣ������

class  CSignCardApplyCheckOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSignCardApplyCheckOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSignCardApplyCheckOfficeDlg(CService* pService);
	~CSignCardApplyCheckOfficeDlg();

	enum { IDD = IDD_02803_SIGN_CARD_APPLY_CHECK_DLG };
	virtual BOOL OnInitDialog();										// �Ի����ʼ��
private:
	BTN_INFO* pCheckReasonBtnInfo;									    // ����ѡ��ť��Ϣ����
	BTN_INFO* pSexBtnInfo;											 	// �Ա�ť��Ϣ����
	BTN_INFO* pCertificatetypeBtnInfo;									// ֤�����Ͱ�ť��Ϣ����
	void InitialButtons();												// ��ʼ��ԭ�򰴼�
	bool ValidateInput();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// �Ի����ʼ��
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);
	afx_msg LRESULT  OnEditChange(WPARAM wParam,LPARAM lParam); 
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	LRESULT OnSevBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnCertificateTypeClick(WPARAM wParam,LPARAM lParam);

};