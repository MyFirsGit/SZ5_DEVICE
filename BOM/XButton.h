#pragma once
#include "Panel.h"
#include "delegate.h"
#include "Bag.h"

#define RADIUS  12            // Բ�Ǿ��ΰ뾶
#define FONTDIV 96            // ���������Сʱ��
#define MAX_XBUTTON_SIZE 32   // ���������Сʱ��


/**
@brief ��̬��ť����ز���  
*/
class CXButton : public CButton
{
    DECLARE_DYNAMIC(CXButton)
    DECLARE_MESSAGE_MAP()

public:
	
	CXButton(UIINFO& uIInfo,BTN_INFO & buttonInfo);
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
	
	CBag* GetBag();

    void ProcessButtonEvent();                                  // ��ť���º����¼��Ĺ���
	
	static delegate<void(CXButton*,bool*)> Clicking;
	static delegate<void(CXButton*)> Clicked;
    static void EnableSound(bool enable);                       // �����Ƿ�������������ť����
    static bool IsEnableSound();                                // �Ƿ�������������ť����
    static void PlayClickSound();                                        // ����������ť����
	
private:
	void OnLButtonDown(UINT nFlags, CPoint point);
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
    CBitmap* m_bmpOn;       // ��ť����ʱͼƬ
    CBitmap* m_bmpDown;     // ��ť����ʱͼƬ
    CBitmap* m_bmpSelected; // ��ťѡ��ʱͼƬ
	CBitmap* m_bmpFocus;	// ��ý���ʱͼƬ
	bool    m_isDown;	    // ��ť������ʱ��ֵΪtrue
    bool    m_isProcessing; // �Ƿ���Click�¼���Ӧ��
	UIINFO m_UIInfo;
	BTN_INFO m_ButtonInfo;
	UINT m_Style;
	CBag* m_Bag;
	friend class CButtonGroup;
};
