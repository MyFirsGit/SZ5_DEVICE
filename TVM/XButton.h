#pragma once
#include "Panel.h"
#include "delegate.h"
#include "Bag.h"

#define RADIUS  12            // 圆角矩形半径
#define FONTDIV 96            // 计算字体大小时用
#define MAX_XBUTTON_SIZE 32   // 计算字体大小时用

typedef struct _text_info {
	CString		face;		// 字体名称
	int			size;		// 字体大小
	bool		bold;		// 是否粗体
	COLORREF	color;		// 文本颜色
	POINT		offset;		// 文本区域

	_text_info()
	{
		face = _T("宋体");
		size = 18;
		bold = true;
		color = RGB(255,255,255);
		memset(&offset, 0, sizeof(offset));
	}
} TEXT_INFO;

/**
@brief 动态按钮的相关操作  
*/
class CXButton : public CButton
{
    DECLARE_DYNAMIC(CXButton)
    DECLARE_MESSAGE_MAP()

public:
	CXButton(UIINFO& uIInfo,BTN_INFO& buttonInfo);
	CXButton(UIINFO& uIInfo,BTN_INFO& buttonInfo,TEXT_INFO& textinfo);
	virtual BOOL Create();
    virtual ~CXButton();
	UIINFO& GetUIInfo();
	void SetUIInfo(UIINFO& uiInfo);
    UINT GetID();                                               // 取得按钮编号
	void SetID(UINT ID);
    CString GetMsg();                                           // 取得按钮附加信息
    void SetMsg(CString msg);                                   // 设置按钮附加信息
    CString GetText();                                 // 取得按钮文字
    void SetText(CString text1=_T(""));           // 设置按钮文字
    void SetSelect(bool isSelected);                            // 设置按钮是否被选中
    bool IsSelected();                                          // 按钮是否被选中
    void SetVisible(bool isVisible);                            // 设置按钮是否可见
    bool IsVisible();                                           // 取得按钮是否可视
    void SetEnabled(bool isEnabled);                            // 设置按钮是否可用
    bool IsEnabled();                                           // 按钮是否可用

	void SetShortCutKey(UINT shortCutKey);
	UINT GetShortCutKey();

	//TEXT_INFO& getTextInfo();
	//void updateParentText();
	
	CBag* GetBag();

    void ProcessButtonEvent();                                  // 按钮按下后处理事件的过程
	
	static delegate<void(CXButton*,bool*)> Clicking;
	static delegate<void(CXButton*)> Clicked;
    static void EnableSound(bool enable);                       // 设置是否允许发出正常按钮声音
    static bool IsEnableSound();                                // 是否允许发出正常按钮声音
    static void PlayClickSound();                                        // 发出正常按钮声音
	
private:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void FormatBtnName();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);       // 创建按钮
	afx_msg void	OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);                        // 背景色能否删除
	afx_msg LRESULT OnXButtonClick(WPARAM, LPARAM);             // 按钮按下事件
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void Initialize(UIINFO& uIInfo,BTN_INFO & buttonInfo);  // 初期化（字体大小）
	virtual void PreSubclassWindow();                           // 前一个窗口
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);   // 在窗口显示按钮
	void ShowButtonText(CDC* pDC, CRect rect);                  // 显示按钮文字
	virtual CRect GetFrameRect();
	virtual CRect GetContentRect();
	void CalculateDrawRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint point);
	 static bool s_enableSound;              // 是否允许发出正常按钮声音
	 CImage* m_pNormal;
	 CImage* m_pPressed;
	 CImage* m_pHover;
	 CImage* m_pDisabled;
	bool    m_isDown;	    // 按钮被按下时该值为true
    bool    m_isProcessing; // 是否在Click事件响应中
	UIINFO m_UIInfo;
	BTN_INFO m_ButtonInfo;
	TEXT_INFO m_TextInfo;
	UINT m_Style;
	CBag* m_Bag;
	friend class CButtonGroup;
};
