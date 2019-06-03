#pragma once
#include "BaseDlg.h"
#include "XButton.h"
#include "UIDefinition.h"
#include "LanguageInfo.h"

// 全部线路按钮显示对话框

/**
@brief    消息框
*/

class CAllLineShowBox : public CReceptionDlg
{
	DECLARE_DYNAMIC(CAllLineShowBox)
    DECLARE_MESSAGE_MAP()

public:
    // 对话框数据
    enum { IDD = IDD_ALLLINESHOW_DLG };

public:
	CAllLineShowBox(UINT ID, CService* pService,CRect rect = REC_FRMAE_RECT,bool needFunctionArea = true);   // 构造函数
	~CAllLineShowBox();																			// 析构函数

	BOOL OnInitDialog();
    INT_PTR Show();                    										 // 显示消息框
    void End(int nResult = IDOK);                                            // 结束消息框

protected:
	enum{
		DEFAULT_SHOW_TIMER = 1,
	};
	
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                            // 画面创建函数
    afx_msg void OnDestroy();                                                       // 画面销毁函数
    void OnDraw(CDC* pDC);                                                          // 画面绘制函数
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // 处理按钮消息


	static const int BUTTON_TOP_MARGIN	= 480;			// 按钮上边距
	static const int BUTTONS_DISTANCE	= 120;			// 两按钮间距

private:
	bool m_bShowing;
	bool hasSetFocus;
    CXButton* m_btnClose;						// 关闭按钮
	vector<CXButton*> m_vecLineBtn;				// 线路按钮组

	CImage m_Image;
};
