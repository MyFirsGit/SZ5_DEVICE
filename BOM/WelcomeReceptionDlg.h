#pragma once
#include "BaseDlg.h"

/**
@brief   欢迎光临画面（该画面不显示，内容显示在COutOfReceptionDlg上）
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
};
