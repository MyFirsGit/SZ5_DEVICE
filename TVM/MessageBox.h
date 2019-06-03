#pragma once
#include "BaseDlg.h"
#include "XButton.h"
#include "UIDefinition.h"
#include "LanguageInfo.h"

// CMessageBox �Ի���

/**
@brief    ��Ϣ��
*/

class CMessageBox : public CBaseDlg
{
	DECLARE_DYNAMIC(CMessageBox)
    DECLARE_MESSAGE_MAP()

public:
    // ��ť����
    enum BTN_TYPE {
        BTN_OK,
        BTN_CANCEL,
        BTN_OKCANCEL,
    };

    // �Ի�������
    enum { IDD = IDD_MESSAGEBOX_DLG };
	enum {
		DISPLAY_TIME_1 = 1,
		DISPLAY_TIME_2 = 2,
		DISPLAY_TIME_3 = 3,
		DISPLAY_TIME_4 = 4,
		DISPLAY_TIME_5 = 5,
		DISPLAY_TIME_10 = 10,
		DISPLAY_TIME_20 = 20,
		DISPLAY_TIME_30 = 30,
		DISPLAY_TIME_60 = 60,
		DISPLAY_TIME_180 = 180,
		DISPLAY_TIME_600 = 600,
		DISPLAY_TIME_1800 = 1800,
	};
    const static int MAX_LINE_LENGTH_EN = 40;                                          // ÿ������ַ���
	const static int MAX_LINE_LENGTH_CN = 20;                                          // ÿ������ַ���

public:
	static delegate<void()> OnShowing;
    static INT_PTR Show(CString title = _T(""), CString message = _tl(GUIDE_IN_PUT_CARD_FIRST), 
        BTN_TYPE btnType = BTN_CANCEL, UINT format = DT_CENTER,int showSeconds = INFINITE,BTN_TYPE focusBtnType = BTN_CANCEL);                    // ��ʾ��Ϣ��
    static bool Exist();                                                            // �Ƿ������Ϣ��
    static void End(int nResult = IDOK);                                            // ������Ϣ��

protected:
	enum{
		DEFAULT_SHOW_TIMER = 1,
	};
    CMessageBox(CString title = _T(""), CString message = _tl(GUIDE_IN_PUT_CARD_FIRST), 
        BTN_TYPE btnType = BTN_CANCEL, UINT format = DT_CENTER,int showSeconds=INFINITE,BTN_TYPE focusBtnType = BTN_CANCEL);                    // ���캯��
    ~CMessageBox();                                                                 // ��������
	
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                            // ���洴������
    afx_msg void OnDestroy();                                                       // �������ٺ���
    void OnDraw(CDC* pDC);                                                          // ������ƺ���
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // ����ť��Ϣ
	afx_msg void OnTimer(UINT_PTR);
	int m_showSeconds;
    int GetMessageLineCount();                                                      // ��ȡ��ʾ��Ϣ����

    static const int MSG_BOX_WIDTH      = 400;          // ��Ϣ����
    static const int MSG_BOX_HEIGHT     = 260;          // ��Ϣ��߶�

    static const int TEXT_LEFT_MARGIN   = 30;           // ��ʾ��Ϣ���ұ߾�
    static const int TEXT_TOP_MARGIN    = 70;           // ��ʾ��Ϣ���±߾�
    static const int TEXT_HEIGHT        = 26;           // ��ʾ��Ϣÿ�и߶�

private:

    CString m_title;                    // ����
    CString m_message;                  // ��Ϣ����
    BTN_TYPE m_btnType;                 // ��ť����
	BTN_TYPE m_focusBtnType;
    UINT m_format;                      // ��ʽ��DT_CENTER|DT_VCENTER|DT_SINGLELINE��
	bool hasSetFocus;
    CXButton* m_btnOK;                  // ȷ�ϰ�ť
    CXButton* m_btnCancel;              // ȡ����ť

    static CMessageBox* s_curMsgBox;    // ��ǰ��Ϣ��
};
