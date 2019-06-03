#pragma once
#include "BaseDlg.h"
#include "XButton.h"
#include "UIDefinition.h"
#include "LanguageInfo.h"

// CMessagePictureBox 有图片的对话框

/**
@brief    消息框
*/

class CMessagePictureBox : public CBaseDlg
{
	DECLARE_DYNAMIC(CMessagePictureBox)
    DECLARE_MESSAGE_MAP()

public:
    // 按钮种类
    enum BTN_TYPE {
        BTN_OK,
        BTN_CANCEL,
        BTN_OKCANCEL,
		BTN_NONE,
    };

    // 对话框数据
    enum { IDD = IDD_MESSAGEBOX_DLG };
	enum {
		DISPLAY_TIME_1 = 1,
		DISPLAY_TIME_2 = 2,
		DISPLAY_TIME_3 = 3,
		DISPLAY_TIME_4 = 4,
		DISPLAY_TIME_5 = 5,
		DISPLAY_TIME_10 = 10,
		DISPLAY_TIME_20 = 20,
		DISPLAY_TIME_30 = 30,
		DISPLAY_TIME_60 = 60,
		DISPLAY_TIME_180 = 180,
		DISPLAY_TIME_600 = 600,
		DISPLAY_TIME_1800 = 1800,
	};
    const static int MAX_LINE_LENGTH_EN = 27;                                          // 每行最大字符数
	const static int MAX_LINE_LENGTH_CN = 13;                                          // 每行最大字符数
	const static int MAX_LINE_LENGTH_ONLY_TXT = 60;									   // 仅有文字没有图片的情况下每行字符的最大数

public:
	static delegate<void()> OnShowing;
    static INT_PTR Show(CString title = _T(""), CString message = _tl(GUIDE_IN_PUT_CARD_FIRST), CString imagePath = _T(""),
        BTN_TYPE btnType = BTN_CANCEL, UINT format = DT_LEFT,int showSeconds = INFINITE,BTN_TYPE focusBtnType = BTN_CANCEL);                    // 显示消息框
    static bool Exist();                                                            // 是否存在消息框
    static void End(int nResult = IDOK);                                            // 结束消息框

protected:
	enum{
		DEFAULT_SHOW_TIMER = 1,
	};
    CMessagePictureBox(CString title = _T(""), CString message = _tl(GUIDE_IN_PUT_CARD_FIRST), CString imagePath = _T(""),
        BTN_TYPE btnType = BTN_CANCEL, UINT format = DT_CENTER,int showSeconds=INFINITE,BTN_TYPE focusBtnType = BTN_CANCEL);                    // 构造函数
    ~CMessagePictureBox();                                                                 // 析构函数
	
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);                                    // 子控件重画时调用
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                            // 画面创建函数
    afx_msg void OnDestroy();                                                       // 画面销毁函数
    void OnDraw(CDC* pDC);                                                          // 画面绘制函数
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // 处理按钮消息
	afx_msg void OnTimer(UINT_PTR);
	int m_showSeconds;
    int GetMessageLineCount();                                                      // 获取提示信息行数

    static const int MSG_BOX_WIDTH      = 800;          // 消息框宽度
    static const int MSG_BOX_HEIGHT     = 600;          // 消息框高度

    static const int TEXT_LEFT_MARGIN   = 500;          // 提示信息左边距
	static const int TEXT_RIGHT_MARGIN  = 70;           // 提示信息右边距
    static const int TEXT_TOP_MARGIN    = 150;          // 提示信息上下边距
    static const int TEXT_HEIGHT        = 26;           // 提示信息每行高度

	static const int PIC_LEFT_MARGIN	= 70;
	static const int PIC_TOP_MARGIN		= 120;
	static const int PIC_WIDTH			= 250;
	static const int PIC_HEIGHT			= 250;

	static const int BUTTON_TOP_MARGIN	= 480;			// 按钮上边距
	static const int BUTTONS_DISTANCE	= 120;			// 两按钮间距

private:

    CString m_title;                    // 标题
    CString m_message;                  // 消息内容
    BTN_TYPE m_btnType;                 // 按钮种类
	BTN_TYPE m_focusBtnType;
    UINT m_format;                      // 格式（DT_CENTER|DT_VCENTER|DT_SINGLELINE）
	bool hasSetFocus;
    CXButton* m_btnOK;                  // 确认按钮
    CXButton* m_btnCancel;              // 取消按钮

	static CMessagePictureBox* s_curMsgBox;    // 当前消息框

	CImage m_Image;
	CString imagePath;
};
