#pragma once
#include "resource.h"
#include "UIDefinition.h"
class CService;     // ��ǰ����

/**
@brief      ���жԻ�����
*/
class CBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CBaseDlg)
    DECLARE_MESSAGE_MAP()

public:
    // �Ի�������
    enum { IDD = IDD_BASE_DLG };

    CBaseDlg(UINT ID, CWnd* pParent = NULL, CRect position = CRect(0,0,1920,1080)/*CRect(0,0,1024,768)*/, 
        COLORREF bkColor = GRAY, CService* pService = NULL, bool isOfficeDlg = true);                   // ���캯��
	virtual ~CBaseDlg();                                                                                // ��������
    virtual void OnOK() {};                                                                             // ����Eenter��
    virtual void OnCancel() {};                                                                         // ����Esc��
    CService* GetService();                                                                             // ��ȡDialog��Service

	template<class T>
	T* GetService()
	{
		return (T*)m_pService;		
	};

    UINT GetID();                                                                                       // ��ȡDialog ID
	
	void DrawRect(CDC* pDC, const CRect& rect, int nBorderWidth = ROUNDRECT_BORDER,
        COLORREF crBorderColor = DARKGRAY, COLORREF crFillColor = GRAY);                                // ������
    void DrawRoundRect(CDC* pDC, const CRect& rect, int nBorderWidth = ROUNDRECT_BORDER,
        COLORREF crBorderColor = DARKGRAY, COLORREF crFillColor = GRAY, CPoint point = ROUND_POINT);    // ��Բ�Ǿ���
    void DrawText(CDC* pDC, int x, int y, CString sText, CString sFont = TITLE_FONT,
        int nFontSize = TITLE_FONT_SIZE, int nFontWeight = FW_BOLD, COLORREF crColor = BLACK);                                     // ��ָ��λ����ʾ����
    void DrawText(CDC* pDC, CRect& rect, CString sText, CString sFont = TITLE_FONT,
        int nFontSize = TITLE_FONT_SIZE, int nFontWeight = FW_BOLD, COLORREF crColor = BLACK, UINT nFormat = DT_CENTER);           // ��ָ��λ����ʾ����
    void DrawText(CDC* pDC, CPoint pos, CString upText, CString downText);                              // ��ָ��λ����ʾ����
	void DrawText(CDC* pDC,CRect& rect,CString sText,LOGFONT logFont = DEFAULT_FONT,COLORREF crColor = BLACK,UINT nFormat = DT_CENTER|DT_SINGLELINE|DT_VCENTER);

    bool IsOfficeDlg();	                                                                                // ���ظ�Dialog�Ƿ���վԱDialog

    LRESULT CallWndProc(UINT nMsg, WPARAM wParam = 0, LPARAM lParam = 0);                               // ֱ�ӵ��ø�������¼���Ӧ���򣨱ܿ�MFC���쳣����

protected:
    static const int BORDER_MAX_SIZE = 3;                                                               // �߿�������

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                                                // ���洴������
    virtual void OnDraw(CDC* pDC);                                                                      // ������ƺ���
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void UpdateUI();
	
private:
    afx_msg void OnPaint();                                                                             // ������ƺ���
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);                                                                // �ػ�Dialog����
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);                                    // �ӿؼ��ػ�ʱ����
    LRESULT OnServiceDataModelChanged(WPARAM,LPARAM);

private:
    CRect       m_rectDlg;                              // �Ի���λ��
    COLORREF    m_bkColor;                              // ������ɫ
    bool        m_isOfficeDlg;                          // վԱ�ͳ˿�Dialog��־��true��վԱDialog��false���˿�Dialog
    UINT        m_ID;                                   // Dialog ID
    CService*   m_pService;                             // �Ի��������Server����
};

#define ON_GROUP_BUTTON_CLICK(groupID, memberFxn)\
{ WM_XGROUP, groupID, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_OK_CLICK(memberFxn)\
{ WM_XBUTTON, 1, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_BACK_CLICK(memberFxn)\
{ WM_XBUTTON, 2, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_PRINT_CLICK(memberFxn)\
{ WM_XBUTTON, 3, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_RESET_CLICK(memberFxn)\
{ WM_XBUTTON, 4, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL CBaseDlg::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

