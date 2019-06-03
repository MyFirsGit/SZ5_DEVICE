#pragma once
#include "BaseDlg.h"

/**
@brief      乘客向导对话框
*/
class CGuideReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGuideReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CGuideReceptionDlg(CWnd* pParent = NULL);   // 构造函数
	~CGuideReceptionDlg();                      // 析构函数

    // 对话框数据
	enum { IDD = IDD_GUIDE_RECEPTION_DLG };     // 显示向导信息

    void ShowGuideMsg(CString guideMsg, CString guideEnglishMsg , CString guidePortugueseMsg);   // 显示向导信息
	void ShowGuideMsg(DWORD guideMsgID);        // 显示向导信息



private:
    void OnDraw(CDC* pDC);                      // 画面绘制函数

	DWORD m_guideMsgID;					        // 向导信息
    CString     m_guideChineseMsg;			    // 中文向导信息
    CString     m_guideEnglishMsg;			    // 英文向导信息
	//CString     m_guidePortugueseMsg;			// 葡萄牙文向导信息
    COLORREF    m_color;			            // 显示颜色
};
