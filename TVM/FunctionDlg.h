#pragma once
#include "BaseDlg.h"
#include "ButtonGroup.h"

class CFunctionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CFunctionDlg)
	DECLARE_MESSAGE_MAP()
private:
	LRESULT ForwardGroupMessage(WPARAM wParam,LPARAM lParam);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CFunctionDlg(UINT ID,CRect rect,UINT rows,UINT cols,UINT groupID,CWnd* pParent = NULL);           // 构造函数
	~CFunctionDlg();                              // 析构函数
	void ShowButtons(bool);
	CButtonGroup* pButtonGroup;
};