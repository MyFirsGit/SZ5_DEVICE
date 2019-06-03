#pragma once
#include "BaseDlg.h"
#include "util.h"

#define TIMER_STS 1000              ///< 事件

class CReceptionHeadDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CReceptionHeadDlg)
    DECLARE_MESSAGE_MAP()

public:
    CReceptionHeadDlg(CWnd* pParent = NULL);   // 标准构造函数
	~CReceptionHeadDlg();

    // 对话框数据
	enum { IDD = IDD_RECEPTION_HEAD_DLG };

    void SetStationName(CString stationNameCN, CString stationNameEN);  // 设置车站名称

private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    void OnDraw(CDC* pDC);

private:
    const static UINT TIMER_INTERVAL = 1000;                            // 时间显示间隔（1 second）

    HBITMAP m_hLogoBmp;                                                 // Logo图片
    CString m_stationNameCH;                                            // 业务名称（中文）
    CString m_stationNameEN;                                            // 业务名称（英文）
    _DATE_TIME m_preTime;                                               // 前次刷新时间
};
