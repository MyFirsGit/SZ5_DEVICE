#pragma once
#include "BaseDlg.h"
#include "XButton.h"
#include "UIDefinition.h"
#include "LanguageInfo.h"

// ȫ����·��ť��ʾ�Ի���

/**
@brief    ��Ϣ��
*/

class CAllLineShowBox : public CReceptionDlg
{
	DECLARE_DYNAMIC(CAllLineShowBox)
    DECLARE_MESSAGE_MAP()

public:
    // �Ի�������
    enum { IDD = IDD_ALLLINESHOW_DLG };

public:
	CAllLineShowBox(UINT ID, CService* pService,CRect rect = REC_FRMAE_RECT,bool needFunctionArea = true);   // ���캯��
	~CAllLineShowBox();																			// ��������

	BOOL OnInitDialog();
    INT_PTR Show();                    										 // ��ʾ��Ϣ��
    void End(int nResult = IDOK);                                            // ������Ϣ��

protected:
	enum{
		DEFAULT_SHOW_TIMER = 1,
	};
	
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);                            // ���洴������
    afx_msg void OnDestroy();                                                       // �������ٺ���
    void OnDraw(CDC* pDC);                                                          // ������ƺ���
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // ����ť��Ϣ


	static const int BUTTON_TOP_MARGIN	= 480;			// ��ť�ϱ߾�
	static const int BUTTONS_DISTANCE	= 120;			// ����ť���

private:
	bool m_bShowing;
	bool hasSetFocus;
    CXButton* m_btnClose;						// �رհ�ť
	vector<CXButton*> m_vecLineBtn;				// ��·��ť��

	CImage m_Image;
};
