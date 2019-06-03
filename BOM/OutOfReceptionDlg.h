#pragma once
#include "BaseDlg.h"

/**
@brief      暂停服务画面
*/
class COutOfReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(COutOfReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	COutOfReceptionDlg(CService* pService = NULL);							// 构造函数
	~COutOfReceptionDlg();

    // 对话框数据
	enum { IDD = IDD_10001_OUTOF_DLG };

    enum CONTENT_TYPE {
        OUT_OF_SERVICE,
        WELCOME
    };

    void Show(CONTENT_TYPE contentType);									// 显示窗口

private:
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// 画面激活后的处理
    void OnDraw(CDC* pDC);													// 画面绘制函数


    CONTENT_TYPE m_contentType;												// 画面内容

	HBITMAP m_hLogoBmp;														// Logo句柄
};
