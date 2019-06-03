#pragma once
#include "BaseDlg.h"
#include "GroupBox.h"
#include "lable.h"

#define theGuideDlg CGuideDlg::GetInstance()

/**
@brief 操作员向导文言栏
*/
class CGuideDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGuideDlg)
    DECLARE_MESSAGE_MAP()

public:
    enum { IDD = IDD_GUIDE_DLG };

	BOOL Create(CWnd* pParentWnd /* = NULL */);						// 画面创建
	void OnDraw(CDC* pDC);											// 画面绘制
	void ShowGuideMsg(CString officeMsg, COLORREF color = BLACK);	// 显示向导信息
    CString GetGuideMsg();											// 获取向导信息

	void ShowGuideMsg(DWORD officeMsg);	// TODO：在修改AppSession后删除此函数

	static CGuideDlg& GetInstance()
	{
		return instance;
	};

private:
	CString m_msg;			// 向导信息
    COLORREF m_color;		// 显示颜色

private:
	CGuideDlg(CWnd* pParent = NULL);
	CGuideDlg(const CGuideDlg&);
	CGuideDlg& operator=(const CGuideDlg&);
	~CGuideDlg();

	static CGuideDlg instance;
};
