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
		DIGIT_CHAR,      // 10�����ַ�
		HEX_CHAR,        // 16�����ַ�
		DIGIT_DOT_CHAR,	 // 10�����ַ�����С����
		HEX_DOT_CHAR,    // 16�����ַ�����С����
		NUMANDCHAR,      // ���ֺ���ĸ
		CASH,
		NAME,
		ALL_CHAR         // �����ַ�
	} CHAR_SCOPE;

public:

	static delegate<void(CGEdit*,CString)> EditTextChanged;

	CGEdit(CWnd* pParentWnd, const RECT& rect, bool isLeftAlign);   // ���캯��

	CGEdit(UIINFO& uiInfo);

	UIINFO GetUIInfo();

	virtual void SetUIInfo(UIINFO& uiInfo);

	virtual BOOL Create();

	~CGEdit();                                                      // ��������
	void SetScope(CHAR_SCOPE scope);                                // ������������ķ�Χ
	bool IsValidChar(UINT nChar);                                   // �ж�����ķ����Ƿ��ǺϷ���ĸ��a-f, A-F, x, X��

	BOOL IsEnabled();
	void SetEnabled(BOOL enabled);

	CBag* GetBag();
	CBaseDlg* GetParentDlg();                                   // ��ȡ��ť������Dialog
private:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);     // �ַ�������Ӧ����
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);       // ������ť
	afx_msg void	OnDestroy();
	HBRUSH CtlColor(CDC*, UINT);
	UIINFO m_UIInfo;

	CHAR_SCOPE m_scope;                                             // ��������ķ�Χ

	CBag* m_pBag;

};
