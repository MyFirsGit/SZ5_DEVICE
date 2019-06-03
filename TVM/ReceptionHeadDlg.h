#pragma once
#include "BaseDlg.h"
#include "util.h"
#include <atlimage.h>
#include "StatusDisplayModuleHelper.h"

#define TIMER_STS 1000              ///< �¼�

class CReceptionHeadDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CReceptionHeadDlg)
    DECLARE_MESSAGE_MAP()

public:
    CReceptionHeadDlg(CWnd* pParent = NULL);   // ��׼���캯��
	~CReceptionHeadDlg();

    // �Ի�������
	enum { IDD = IDD_RECEPTION_HEAD_DLG };

    void SetStationName(CString stationNameCN, CString stationNameEN = _T(""), CString stationNamePN = _T(""));  // ���ó�վ����
	void SetServiceStatuse(OPERATION_MODE operationMode);                                     
                            // ���÷���״̬
	virtual void OnClientLanguageModeChanged(LANGUAGETYPE_T&);

	void OnChangeChanged();
	void OnPaymentChanged();
	void OnChangeOrPaymentChanged();
	void updataStatusData();
	void OnStatusChanged();				// TVM��Ԯ��Ϣ�仯
private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    void OnDraw(CDC* pDC);
	void ConvertPng(CImage* image);                           // ͸������
private:
    const static UINT TIMER_INTERVAL = 1000;                              // ʱ����ʾ�����1 second��

	CImage m_Logo;                                                        // Logo
	CImage m_bkg;														  // ����ͼƬ
	CString m_deviceId;													  // �豸��źͰ汾
    CString m_status;                                                     // ҵ��״̬
	CString m_stationName;                                                // ��վ����
	CString m_stationNameCN;                                              // ��վ����
	CString m_stationNameEN;                                              // ��վ����
	CString m_stationNamePN;                                              // ��վ����
	CString m_strWeekDay;												  // ���ڼ�
	CString m_strStatusDispMessage;										  // ��ʾ��Ϣ��������Ԯ��Ϣ�ȣ�
	OPERATION_MODE m_operationMode;
	//CPictureEx	m_gif;
    CImage m_hStatusPic;
    _DATE_TIME m_preTime;                                                 // ǰ��ˢ��ʱ��
};
