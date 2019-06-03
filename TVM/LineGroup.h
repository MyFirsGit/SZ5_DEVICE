#pragma once;
#include "XButton.h"
#include "Panel.h"

class CLineGroup:public CBaseDlg
{
	DECLARE_DYNAMIC(CLineGroup)
	DECLARE_MESSAGE_MAP()

public:
	enum { IDD = IDD_STATION_DLG };
public:
	CLineGroup(UIINFO areaUIInfo);
	virtual ~CLineGroup();
	void SetButtonUIInfos(vector<UIINFO>& buttonUIInfos);
	void SetButtonInfos(vector<BTN_INFO>& buttonInfos);
	void BuildButton(vector<UIINFO>& buttonUIInfos,vector<BTN_INFO>& buttonInfos);
	void BuildButton(vector<UIINFO>& buttonUIInfos,vector<BTN_INFO>& buttonInfos,vector<TEXT_INFO>& textinfos);

	vector<CXButton*>& getBtns();
	vector<BTN_INFO>& getBtnsInfo();

private:
	UIINFO m_UIInfo;
	vector<CXButton*> m_vecButton;
	vector<UIINFO> m_vecButtonUIInfo;
	vector<BTN_INFO> m_vecButtonInfo;

	UINT m_ID;

	BTN_INFO& FindBtnInfo(UINT ID,CString msg);
	void DeleteButton();
	afx_msg LRESULT XButtonClick(WPARAM, LPARAM);
	//afx_msg void OnPaint();
	void OnDraw(CDC* pDC);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	void ShowFunc(CDC* pDc);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	double getCurrentX(int x);
	double getCurrentY(int y);
	void CheckLFBtnSelect(CPoint point);
private:
	CWnd* m_pParent;
	bool m_bEnlarge;
	bool m_bPushDown;
	bool m_bDragMap;
	POINT m_curPoint;
	ULONG_PTR m_gdiplusToken;
	float m_fXstart;					
	float m_fYstart;
	float m_fXOffset;
	float m_fYOffset;
	double m_fScale;
};