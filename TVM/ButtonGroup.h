#pragma once;
#include "Table.h"
#include "XButton.h"

static const int GBUTTON_TITLE_HEIGHT = 70;           // �ı�������߶�

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
	CButtonGroup(UIINFO& uIInfo,UINT rows,UINT cols,UINT ID, SELECTTYPE selectType = SIGLE, BUTTON_TYPE btnType = BUTTON_TYPE_BUTTENGROUP,UIINFO& btnUIInfo=UIINFO(),CString titleName=_T("--"),int groupType = 0, int noticeWidth = 0);
	virtual ~CButtonGroup();
	void SetButtonInfos(vector<BTN_INFO>& buttonInfos);
	void SetButtonTitle(CString titleName);
	void SetSelectType(SELECTTYPE selectType);
	SELECTTYPE GetSelectType();
	virtual BOOL Create();
	const vector<BTN_INFO>& GetSelectedBtnInfo();
	void ShowButtons(bool);
	void ClearSelection();
	BOOL SetSelection(CString msg);					// ���ð�ťѡ��
	vector<BTN_INFO>& GetBtnInfo();
	BTN_INFO& FindBtnInfo(UINT ID,CString msg);
	BOOL EnableButtonGroup(bool enabled);
	BOOL EnableButton(UINT ID,CString msg,bool enabled);
	BOOL EnableButton(int index,bool enabled);
	BOOL ClickButton(int index);
	BOOL ClickButton(CString msg);
	BOOL ClickFirstAvailButton();
	const BTN_INFO* GetFirstAvailBtnInfo();

	vector<CXButton*>& getBtns();
	vector<BTN_INFO>& getBtnsInfo();
	vector<BTN_INFO>& getSelectBtnsInfo();

	UIINFO& getUIInfo();

private:
	vector<BTN_INFO> m_vecEnableButtonInfo;
	vector<CXButton*> m_vecFixedButton;
	vector<BTN_INFO> m_vecButtonInfo;
	vector<BTN_INFO> m_vecSelectedButtonInfo;
	void BuildFixedButton();
	void DeleteFixedButton();
	void BindButton(bool isNext);
	afx_msg LRESULT XButtonClick(WPARAM, LPARAM);
	afx_msg void OnPaint();

	CString m_titleName;
	
	int m_ShowStartIndex;
	int m_ShowCount;
	int m_ShowPreCount;
	bool m_ShowNext;
	bool m_ShowPrevious;
	int m_nNextIndex;
	int m_nPreviousIndex;
	UINT m_ID;
	SELECTTYPE m_SelectType;
	UIINFO m_BtnUIInfo;
	UIINFO m_uIInfo;
	bool m_ShowButtons;
	BUTTON_TYPE m_btnType;

	int m_GroupType;	// 0:ͨ����ť�飻1����������һҳ����һҳ��ť������λ���������̶��İ�ť
	CImage m_Logo;
};