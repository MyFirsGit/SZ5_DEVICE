//#include "stdafx.h"
//#include "CTokenManageSvc.h"
//#include "OperationDlg.h"
///*
//*@brief TOKEN��������
//**/
//
//#define MAX_ROW		15					// �����������
//#define MAX_COL		 5					// �����������
//
//class CTokenBoxInfoDlg : public COperationDlg
//{
//	DECLARE_DYNAMIC(CTokenBoxInfoDlg)
//	DECLARE_MESSAGE_MAP()
//
//public:
//	CTokenBoxInfoDlg(CService* pService);
//	~CTokenBoxInfoDlg();
//
//	enum{	IDD = IDD_03001_TICKETBOX_EXCHANGE_BASE_DLG};
//
//	virtual void UpdateUI();													// �������
//protected:
//	// ��ʼ������
//	void InitMainDlg();															// ��ʼ��Main����
//	void InitInstallDlg();														// ��ʼ����װ����
//	void InitInstalledDlg();													// ��ʼ����װ��ɽ���
//	void InitUninstallDlg();													// ��ʼ��ж�ؽ���
//	void InitUninstalledDlg();													// ��ʼ��ж����ɽ���
//	void InitRecycleDlg();														// ��ʼ�����ս���
//	void InitRecycledDlg();														// ��ʼ��������ɽ���
//	// ��ʾ����
//	void ShowUpMainDlg();														// ��ʾMain����
//	void ShowUpInstallDlg();													// ��ʾ��װ����
//	void ShowUpInstalledDlg();													// ��ʾ��װ��ɽ���
//	void ShowUpUninstallDlg();													// ��ʾж�ؽ���
//	void ShowUpUninstalledDlg();												// ��ʾж����ɽ���
//	void ShowUpRecycleDlg();													// ��ʾ���ս���
//	void ShowUpRecycledDlg();													// ��ʾ������ɽ���
//
//	// ����
//	void ClearLabelData();														// �����ʾ��������
//	CString TransforBoxID(const CString strID);									// ת��ID��ʾ
//	CString TransforStatus(const CTokenManageSvc::TOKEN_BOX_STATUS status);		// ת�����ӵ�ǰ״̬
//	inline CString TransforCount(const int nCount){								// ת������ΪCString����
//		CString strCount;
//		strCount.Format(_T("%d"),nCount);
//		return strCount;
//	}
//private:
//	CTokenManageSvc*	pService;												// ������ָ�����
//	CTokenManageSvc::Model model;												// �����ռ�
//	LABEL_GROUP_FIVE	m_labelGroupFive[MAX_ROW];								// ����Label����
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);						// ���ڴ�����Ϣ
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);		// ���ڼ�����Ϣ
//};