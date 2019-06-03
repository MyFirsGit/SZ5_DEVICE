#pragma once
#include "OperationDlg.h"
#include "GStatic.h"
#include "NetworkTestSvc.h"
#include "FTPCommand.h"

/**
@brief      ������Ի���
*/
class CNetworkItemMaintainDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CNetworkItemMaintainDlg)
	DECLARE_MESSAGE_MAP()

public:
	CNetworkItemMaintainDlg(CService* pService);				// ��׼���캯��
	~CNetworkItemMaintainDlg();

	// �Ի�������
	enum { IDD = IDD_20551_NETWORK_ITEM_DLG };

private:
	CString	m_sNetworkItem;										// ����ѡ�е����������Ŀ��Ӧ��MSG
	bool    m_bIsShowPingResult;								// �Ƿ���ʾPing�������
	CString m_sPingResult;										// Ping���

protected:
	void OnDraw(CDC* pDC);										// ������ƺ���

public:
	afx_msg LRESULT OnOK(WPARAM,LPARAM);						// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnNetworkItemClick(WPARAM,LPARAM);			// ��ť���º�Ĵ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);		// ���洴������

};