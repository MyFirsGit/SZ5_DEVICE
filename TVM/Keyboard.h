#pragma once
#include "stdafx.h"
#include "BaseDlg.h"

class CKeyboard:public CBaseDlg
{
	DECLARE_DYNAMIC(CKeyboard)
	DECLARE_MESSAGE_MAP()
public:
	CKeyboard(UINT ID,CRect rect,CWnd* pParent = NULL);           // 构造函数
	~CKeyboard();                              // 析构函数
	void ShowKeys(bool);
	void SetEnable(bool);
protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	LRESULT OnKeyboardClick(WPARAM,LPARAM);
	CButtonGroup* pButtonGroup;
};