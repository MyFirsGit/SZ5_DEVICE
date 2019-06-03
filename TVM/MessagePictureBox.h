#pragma once
#include "BaseDlg.h"
#include "XButton.h"
#include "UIDefinition.h"
#include "LanguageInfo.h"

// CMessagePictureBox ��ͼƬ�ĶԻ���

/**
@brief    ��Ϣ��
*/

class CMessagePictureBox : public CBaseDlg
{
	DECLARE_DYNAMIC(CMessagePictureBox)
    DECLARE_MESSAGE_MAP()

public:
    // ��ť����
    enum BTN_TYPE {
        BTN_OK,
        BTN_CANCEL,
        BTN_OKCANCEL,
		BTN_NONE,
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
    const static int MAX_LINE_LENGTH_EN = 27;                                          // ÿ������ַ���
	const static int MAX_LINE_LENGTH_CN = 13;                                          // ÿ������ַ���
	const static int MAX_LINE_LENGTH_ONLY_TXT = 60;									   // ��������û��ͼƬ�������ÿ���ַ��������

public:
	static delegate<void()> OnShowing;
    static INT_PTR Show(CString title = _T(""), CString message = _tl(GUIDE_IN_PUT_CARD_FIRST), CString imagePath = _T(""),
        BTN_TYPE btnType = BTN_CANCEL, UINT format = DT_LEFT,int showSeconds = INFINITE,BTN_TYPE focusBtnType = BTN_CANCEL);                    // ��ʾ��Ϣ��
    static bool Exist();                                                            // �Ƿ������Ϣ��
    static void End(int nResult = IDOK);                                            // ������Ϣ��

protected:
	enum{
		DEFAULT_SHOW_TIMER = 1,
	};
    CMessagePictureBox(CString title = _T(""), CString message = _tl(GUIDE_IN_PUT_CARD_FIRST), CString imagePath = _T(""),
        BTN_TYPE btnType = BTN_CANCEL, UINT format = DT_CENTER,int showSeconds=INFINITE,BTN_TYPE focusBtnType = BTN_CANCEL);                    // ���캯��
    ~CMessagePictureBox();                                                                 // ��������
	
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);                                    // �ӿؼ��ػ�ʱ����
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                            // ���洴������
    afx_msg void OnDestroy();                                                       // �������ٺ���
    void OnDraw(CDC* pDC);                                                          // ������ƺ���
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // ����ť��Ϣ
	afx_msg void OnTimer(UINT_PTR);
	int m_showSeconds;
    int GetMessageLineCount();                                                      // ��ȡ��ʾ��Ϣ����

    static const int MSG_BOX_WIDTH      = 800;          // ��Ϣ����
    static const int MSG_BOX_HEIGHT     = 600;          // ��Ϣ��߶�

    static const int TEXT_LEFT_MARGIN   = 500;          // ��ʾ��Ϣ��߾�
	static const int TEXT_RIGHT_MARGIN  = 70;           // ��ʾ��Ϣ�ұ߾�
    static const int TEXT_TOP_MARGIN    = 150;          // ��ʾ��Ϣ���±߾�
    static const int TEXT_HEIGHT        = 26;           // ��ʾ��Ϣÿ�и߶�

	static const int PIC_LEFT_MARGIN	= 70;
	static const int PIC_TOP_MARGIN		= 120;
	static const int PIC_WIDTH			= 250;
	static const int PIC_HEIGHT			= 250;

	static const int BUTTON_TOP_MARGIN	= 480;			// ��ť�ϱ߾�
	static const int BUTTONS_DISTANCE	= 120;			// ����ť���

private:

    CString m_title;                    // ����
    CString m_message;                  // ��Ϣ����
    BTN_TYPE m_btnType;                 // ��ť����
	BTN_TYPE m_focusBtnType;
    UINT m_format;                      // ��ʽ��DT_CENTER|DT_VCENTER|DT_SINGLELINE��
	bool hasSetFocus;
    CXButton* m_btnOK;                  // ȷ�ϰ�ť
    CXButton* m_btnCancel;              // ȡ����ť

	static CMessagePictureBox* s_curMsgBox;    // ��ǰ��Ϣ��

	CImage m_Image;
	CString imagePath;
};
