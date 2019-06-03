#pragma once
#include "BaseDlg.h"
#include "util.h"

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

    void SetStationName(CString stationNameCN, CString stationNameEN);  // ���ó�վ����

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    void OnDraw(CDC* pDC);

private:
    const static UINT TIMER_INTERVAL = 1000;                            // ʱ����ʾ�����1 second��

    HBITMAP m_hLogoBmp;                                                 // LogoͼƬ
    CString m_stationNameCH;                                            // ҵ�����ƣ����ģ�
    CString m_stationNameEN;                                            // ҵ�����ƣ�Ӣ�ģ�
    _DATE_TIME m_preTime;                                               // ǰ��ˢ��ʱ��
};
