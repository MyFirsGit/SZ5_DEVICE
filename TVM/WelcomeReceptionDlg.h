#pragma once
#include "BaseDlg.h"

/**
@brief   欢迎光临画面
*/
class CWelcomeReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CWelcomeReceptionDlg)
    //DECLARE_MESSAGE_MAP()

public:
	CWelcomeReceptionDlg(CService* pService);   // 构造函数
	~CWelcomeReceptionDlg();

    // 对话框数据
	enum { IDD = IDD_10002_WELCOME_DLG };
	void Show();									// 显示窗口
private:
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// 画面激活后的处理
	void OnDraw(CDC* pDC);													// 画面绘制函数

};
