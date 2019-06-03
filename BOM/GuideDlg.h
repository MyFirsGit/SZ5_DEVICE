#pragma once
#include "BaseDlg.h"
#include "GroupBox.h"
#include "lable.h"

#define theGuideDlg CGuideDlg::GetInstance()

/**
@brief      站员向导对话框
*/
class CGuideDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGuideDlg)
    DECLARE_MESSAGE_MAP()

public:
	CGuideDlg(CWnd* pParent = NULL);            // 构造函数
	~CGuideDlg();

    // 对话框数据
    enum { IDD = IDD_GUIDE_DLG };

    void ShowGuideMsg(CString guideMsg);        // 显示向导信息
    CString GetGuideMsg();                      // 获取向导信息


	BOOL Create(CWnd* pParentWnd /* = NULL */);

	static CGuideDlg& GetInstance(){
		return instance;
	};

private:
    void OnDraw(CDC* pDC);                      // 画面绘制函数

    CString     m_guideMsg;			            // 向导信息(含颜色部分)
	CString     m_msg;			                // 向导信息
    COLORREF    m_color;			            // 显示颜色

	static CGuideDlg instance;

};
