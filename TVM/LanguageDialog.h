#pragma once
#include "resource.h"
#include "UtilTimer.h"

// LanguageDialog �Ի���
#define TIMER_LAN 1001						// ��ʱ��ID
const static UINT TIMER_INTERVAL = 5*1000;	// ʱ��ˢ�¼��(5 second)

/**
@brief      ����ѡ����
*/
class LanguageDialog : public CDialog
{
	DECLARE_DYNAMIC(LanguageDialog)
	DECLARE_MESSAGE_MAP()

public:
	LanguageDialog();   // ��׼���캯��
	~LanguageDialog();

	// �Ի�������
	enum { IDD = IDD_LANGUAGE_DLG };

protected:
	void OnDraw(CDC* pDC);					// ������ƺ���
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	afx_msg void OnPaint();					// ������ƺ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnChinese(WPARAM,LPARAM);
	afx_msg LRESULT OnEnglish(WPARAM,LPARAM);
	afx_msg LRESULT OnPortuguese(WPARAM,LPARAM);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);	// ʱ�����

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	CXButton* BuildOperationButton(CRect rect,CString text,CString msg);

	COLORREF        m_bkColor;				// ������ɫ
	CXButton*       m_btnChinese;			// �������İ�ť
	CXButton*       m_btnEnglish;			// Ӣ�İ�ť
	CXButton*       m_btnPortuguese;		// �������İ�ť
	 
	LANGUAGETYPE_T m_DefautLanguageType;
};

#define ON_BTN_CHINESE_CLICK(memberFxn)\
{ WM_XBUTTON, 0, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL LanguageDialog::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_ENGLISH_CLICK(memberFxn)\
{ WM_XBUTTON, 1, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL LanguageDialog::*)(WPARAM, LPARAM) > \
	(memberFxn)) },

#define ON_BTN_PORTUGUESE_CLICK(memberFxn)\
{ WM_XBUTTON, 2, 0, 0, AfxSig_lwl, \
	(AFX_PMSG)(AFX_PMSGW) \
	(static_cast< LRESULT (AFX_MSG_CALL LanguageDialog::*)(WPARAM, LPARAM) > \
	(memberFxn)) },