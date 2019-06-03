#pragma once
#include "Func.h"
#include "Panel.h"
#include "Bag.h"
#include "delegate.h"
#include "ctype.h"

class CGEdit : public CEdit
{
	DECLARE_MESSAGE_MAP()

public:
	typedef enum _char_scope {
		DIGIT_CHAR,      // 10进制字符
		HEX_CHAR,        // 16进制字符
		DIGIT_DOT_CHAR,	 // 10进制字符包括小数点
		HEX_DOT_CHAR,    // 16进制字符包括小数点
		NUMANDCHAR,      // 数字和字母
		CASH,
		NAME,
		ALL_CHAR         // 所有字符
	} CHAR_SCOPE;

public:

	static delegate<void(CGEdit*,CString)> EditTextChanged;

	CGEdit(CWnd* pParentWnd, const RECT& rect, bool isLeftAlign);   // 构造函数

	CGEdit(UIINFO& uiInfo);

	UIINFO GetUIInfo();

	virtual void SetUIInfo(UIINFO& uiInfo);

	virtual BOOL Create();

	~CGEdit();                                                      // 析构函数
	void SetScope(CHAR_SCOPE scope);                                // 设置允许输入的范围
	bool IsValidChar(UINT nChar);                                   // 判断输入的符号是否是合法字母（a-f, A-F, x, X）

	BOOL IsEnabled();
	void SetEnabled(BOOL enabled);

	CBag* GetBag();
	CBaseDlg* GetParentDlg();                                   // 获取按钮所属的Dialog
private:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);     // 字符输入响应函数
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);       // 创建按钮
	afx_msg void	OnDestroy();
	HBRUSH CtlColor(CDC*, UINT);
	UIINFO m_UIInfo;

	CHAR_SCOPE m_scope;                                             // 允许输入的范围

	CBag* m_pBag;

};
