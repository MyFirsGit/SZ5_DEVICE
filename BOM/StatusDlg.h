#pragma once
#include "SysException.h"
#include "BaseDlg.h"
#include "util.h"

// CStatusDlg �Ի���
class CStatusDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CStatusDlg)
	DECLARE_MESSAGE_MAP()

public:
	CStatusDlg(CWnd* pParent = NULL);           // ���캯��
	~CStatusDlg();                              // ��������

	enum { IDD = IDD_STATUS_DLG };

	void SetUserID(CString userID);             // ���ù���ԱID
	void SetNetWorkStatus(CString networkStatus);	//����״̬
	void SetPrinterStatus(CString printerStatus);	//���ô�ӡ��״̬
	void SetOutRWStatus(CString outRWStatus);	//�����ⲿ��д��״̬
	void SetTWStatus(CString twStatus);					//�����ڲ���д��״̬
	void SetTHStatus(CString thStatus);

private:
	const static UINT TIMER_INTERVAL = 1000;    // ʱ��ˢ�¼��(1 second)

	void OnDraw(CDC* pDC);                      // ������ƺ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);    // ���洴������
	afx_msg void OnTimer(UINT nIDEvent);        // ʱ�����
	afx_msg void OnDestroy();                   // �������ٺ���
	void OnExceptionNotify(CSysException& ,bool);
	void OnTHModeChanged(int&);
	void RefreshHardwareStatus();
private:
	CString m_userID;                           // ����ԱID
	CString m_networkStatus;						//����״̬
	CString m_printerStatus;							//��ӡ��״̬
	CString m_outRWStatus;							//��д��״̬
	CString m_TWStatus;								//�ڲ���д��״̬
	CString m_THStatus;								//��Ʊ��״̬

	CRect m_rectUserID;
	CRect m_rectNetworkStatus;
	CRect m_rectPrinterStatus;
	CRect m_rectOutRWStatus;
	CRect m_rectTWStatus;
	CRect m_rectTHStatus;
	CRect m_rectTime;

	_DATE_TIME m_preTime;                       // ǰ��ˢ��ʱ��
	_DATE    m_busiDay;                         // ǰ����Ӫ����
};
