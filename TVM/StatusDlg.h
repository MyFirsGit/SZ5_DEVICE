#pragma once
#include "BaseDlg.h"
#include "util.h"

/**
@brief ����Ա״̬���Ի���
*/
class CStatusDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CStatusDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStatusDlg(CWnd* pParent = NULL);
	~CStatusDlg();

	enum { IDD = IDD_STATUS_DLG };

	void SetOperatorID(CString id);							// ���ò���ԱID
	void SetNetWorkStatus(LANGUAGE_MSG networkStatus);		// ��������״̬
	void SetOperatingMode(CString systemMode);				// ��������ģʽ
	void ShowStatusMsg(LANGUAGE_MSG strMsg,bool bIsShowup = true);				// ��ʾ״̬��Ϣ
	
	void SetUserID(CString userID);// TODO:�ȴ�������ͨ��ɾ��
	void SetVMIssueAmount(DWORD money);// TODO:�ȴ�������ͨ��ɾ��
	void ReFlashStatusDlg();			// ˢ��״̬��ʾ
private:
	void OnDraw(CDC* pDC);									// �������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���洴��
	afx_msg void OnDestroy();								// ��������
	afx_msg void OnTimer(UINT nIDEvent);					// ϵͳʱ��ˢ�¿���

private:
	CTime m_system_clock_time;					// ϵͳʱ��
	CString m_operator_id;						// ����ԱID
	LANGUAGE_MSG m_network_status;				// ����״̬
	CString m_operating_mode;					// ����ģʽ
	LANGUAGE_MSG m_status_msg;					// ״̬��Ϣ

	CRect m_system_clock_rc;					// ϵͳʱ����ʾ����
	CRect m_operator_id_rc;						// ����ԱID��ʾ����
	CRect m_network_status_rc;					// ����״̬��ʾ����
	CRect m_operating_mode_rc;					// ����ģʽ��ʾ����

	COLORREF m_network_status_color;			// ����״̬��ɫ
	_DATE_TIME m_old_time;						// �ϴ�ˢ��ʱ��
	const static UINT TIMER_INTERVAL = 1000;	// ϵͳʱ��ˢ�¼����1�룩
	bool m_bIsShowMsg;
};
