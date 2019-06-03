#pragma once
#include "BaseDlg.h"

// CLogoDlg 对话框


class CLogoDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CLogoDlg)
    DECLARE_MESSAGE_MAP()

public:
    CLogoDlg(CWnd* pParent = NULL);     // 构造函数
	~CLogoDlg();                        // 析构函数

    // 对话框数据
	enum { IDD = IDD_LOGO_DLG };

private:
    void OnDraw(CDC* pDC);              // 画面绘制函数
	void OnVersionNoChanged(CVersionInfo::VER_TYPE,CVersionInfo::PROG_TYPE,DWORD);//版本号发生改变响应函数。
private:
    HBITMAP m_hLogoBmp;                 // Logo句柄
};
