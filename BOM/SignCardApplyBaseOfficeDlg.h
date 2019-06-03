#pragma once
#include "OperationDlg.h"

/**
@brief ����������-��������
*/
static const char			USER_INFO_SIZE			=	9;			// վԱ������ʾ��Ϣ������

class  CSignCardApplyBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CSignCardApplyBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CSignCardApplyBaseOfficeDlg(CService* pService);
	~CSignCardApplyBaseOfficeDlg();
	void  ClickCredentialsTypeButton(int index);
	void  ClickSexButton(int index);
	void  ClickSevTypeButton(int index);
	
	enum { IDD = IDD_02801_SIGN_CARD_APPLY_BASE_DLG };
	virtual BOOL OnInitDialog();										// �Ի����ʼ��
private:
	LRESULT OnSevBtnClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnSexClick(WPARAM wParam,LPARAM lParam);
	LRESULT OnCertificateTypeClick(WPARAM wParam,LPARAM lParam);

	BTN_INFO* pCheckReasonBtnInfo;									    // ����ѡ��ť��Ϣ����
	BTN_INFO* pSexBtnInfo;											 	// �Ա�ť��Ϣ����
	BTN_INFO* pCertificatetypeBtnInfo;									// ֤�����Ͱ�ť��Ϣ����
	void InitialButtons();												// ��ʼ��ԭ�򰴼�
	void  Reset();
	bool ValidateInput();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);				// �Ի����ʼ��
	afx_msg LRESULT OnOK(WPARAM,LPARAM);
	afx_msg LRESULT OnReset(WPARAM,LPARAM);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT  OnEditChange(WPARAM wParam,LPARAM lParam);
};