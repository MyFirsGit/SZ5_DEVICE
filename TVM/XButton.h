#pragma once
#include "Panel.h"
#include "delegate.h"
#include "Bag.h"

#define RADIUS  12            // Բ�Ǿ��ΰ뾶
#define FONTDIV 96            // ���������Сʱ��
#define MAX_XBUTTON_SIZE 32   // ���������Сʱ��

typedef struct _text_info {
	CString		face;		// ��������
	int			size;		// �����С
	bool		bold;		// �Ƿ����
	COLORREF	color;		// �ı���ɫ
	POINT		offset;		// �ı�����

	_text_info()
	{
		face = _T("����");
		size = 18;
		bold = true;
		color = RGB(255,255,255);
		memset(&offset, 0, sizeof(offset));
	}
} TEXT_INFO;

/**
@brief ��̬��ť����ز���  
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
    UINT GetID();                                               // ȡ�ð�ť���
	void SetID(UINT ID);
    CString GetMsg();                                           // ȡ�ð�ť������Ϣ
    void SetMsg(CString msg);                                   // ���ð�ť������Ϣ
    CString GetText();                                 // ȡ�ð�ť����
    void SetText(CString text1=_T(""));           // ���ð�ť����
    void SetSelect(bool isSelected);                            // ���ð�ť�Ƿ�ѡ��
    bool IsSelected();                                          // ��ť�Ƿ�ѡ��
    void SetVisible(bool isVisible);                            // ���ð�ť�Ƿ�ɼ�
    bool IsVisible();                                           // ȡ�ð�ť�Ƿ����
    void SetEnabled(bool isEnabled);                            // ���ð�ť�Ƿ����
    bool IsEnabled();                                           // ��ť�Ƿ����

	void SetShortCutKey(UINT shortCutKey);
	UINT GetShortCutKey();

	//TEXT_INFO& getTextInfo();
	//void updateParentText();
	
	CBag* GetBag();

    void ProcessButtonEvent();                                  // ��ť���º����¼��Ĺ���
	
	static delegate<void(CXButton*,bool*)> Clicking;
	static delegate<void(CXButton*)> Clicked;
    static void EnableSound(bool enable);                       // �����Ƿ�������������ť����
    static bool IsEnableSound();                                // �Ƿ�������������ť����
    static void PlayClickSound();                                        // ����������ť����
	
private:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void FormatBtnName();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);       // ������ť
	afx_msg void	OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);                        // ����ɫ�ܷ�ɾ��
	afx_msg LRESULT OnXButtonClick(WPARAM, LPARAM);             // ��ť�����¼�
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void Initialize(UIINFO& uIInfo,BTN_INFO & buttonInfo);  // ���ڻ��������С��
	virtual void PreSubclassWindow();                           // ǰһ������
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);   // �ڴ�����ʾ��ť
	void ShowButtonText(CDC* pDC, CRect rect);                  // ��ʾ��ť����
	virtual CRect GetFrameRect();
	virtual CRect GetContentRect();
	void CalculateDrawRect(CWnd* pParndWnd, DOCKSTYLE dockStyle,CPoint point);
	 static bool s_enableSound;              // �Ƿ�������������ť����
	 CImage* m_pNormal;
	 CImage* m_pPressed;
	 CImage* m_pHover;
	 CImage* m_pDisabled;
	bool    m_isDown;	    // ��ť������ʱ��ֵΪtrue
    bool    m_isProcessing; // �Ƿ���Click�¼���Ӧ��
	UIINFO m_UIInfo;
	BTN_INFO m_ButtonInfo;
	TEXT_INFO m_TextInfo;
	UINT m_Style;
	CBag* m_Bag;
	friend class CButtonGroup;
};
