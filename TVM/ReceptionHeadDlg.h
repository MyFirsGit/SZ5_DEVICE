#pragma once
#include "BaseDlg.h"
#include "util.h"
#include <atlimage.h>
#include "StatusDisplayModuleHelper.h"

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

    void SetStationName(CString stationNameCN, CString stationNameEN = _T(""), CString stationNamePN = _T(""));  // 设置车站名称
	void SetServiceStatuse(OPERATION_MODE operationMode);                                     
                            // 设置服务状态
	virtual void OnClientLanguageModeChanged(LANGUAGETYPE_T&);

	void OnChangeChanged();
	void OnPaymentChanged();
	void OnChangeOrPaymentChanged();
	void updataStatusData();
	void OnStatusChanged();				// TVM招援信息变化
private:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    void OnDraw(CDC* pDC);
	void ConvertPng(CImage* image);                           // 透明处理
private:
    const static UINT TIMER_INTERVAL = 1000;                              // 时间显示间隔（1 second）

	CImage m_Logo;                                                        // Logo
	CImage m_bkg;														  // 背景图片
	CString m_deviceId;													  // 设备编号和版本
    CString m_status;                                                     // 业务状态
	CString m_stationName;                                                // 车站名称
	CString m_stationNameCN;                                              // 车站名称
	CString m_stationNameEN;                                              // 车站名称
	CString m_stationNamePN;                                              // 车站名称
	CString m_strWeekDay;												  // 星期几
	CString m_strStatusDispMessage;										  // 提示信息（包括招援信息等）
	OPERATION_MODE m_operationMode;
	//CPictureEx	m_gif;
    CImage m_hStatusPic;
    _DATE_TIME m_preTime;                                                 // 前次刷新时间
};
