#pragma once;
#include "Table.h"
#include "XButton.h"

class CButtonGroup:public CTable
{
	DECLARE_DYNAMIC(CButtonGroup)
	DECLARE_MESSAGE_MAP()
public:
	typedef enum _SelectType
	{
		NONE,
		SIGLE,
		MULTIPLE
	} SELECTTYPE;
	CButtonGroup(UIINFO& uIInfo,UINT rows,UINT cols,UINT ID, SELECTTYPE selectType = SIGLE,UIINFO& btnUIInfo=UIINFO());
	virtual ~CButtonGroup();
	void SetButtonInfos(vector<BTN_INFO>& buttonInfos);
	void SetSelectType(SELECTTYPE selectType);
	SELECTTYPE GetSelectType();
	virtual BOOL Create();
	const vector<BTN_INFO>& GetSelectedBtnInfo();
	void ShowButtons(bool);
	void ClearSelection();
	vector<BTN_INFO>& GetBtnInfo();
	BTN_INFO& FindBtnInfo(UINT ID,CString msg);
	BOOL EnableButton(UINT ID,CString msg,bool enabled);
	BOOL EnableButton(int index,bool enabled);
	BOOL ClickButton(int index);
	BOOL ClickFirstAvailButton();
	const BTN_INFO* GetFirstAvailBtnInfo();
private:
	vector<CXButton*> m_vecFixedButton;
	vector<BTN_INFO> m_vecButtonInfo;
	vector<BTN_INFO> m_vecSelectedButtonInfo;
	void BuildFixedButton();
	void DeleteFixedButton();
	void BindButton(bool isNext);
	afx_msg LRESULT XButtonClick(WPARAM, LPARAM);
	afx_msg void OnPaint();
	
	int m_ShowStartIndex;
	int m_ShowCount;
	int m_ShowPreCount;
	bool m_ShowNext;
	bool m_ShowPrevious;
	UINT m_ID;
	SELECTTYPE m_SelectType;
	UIINFO m_BtnUIInfo;
	bool m_ShowButtons;
};