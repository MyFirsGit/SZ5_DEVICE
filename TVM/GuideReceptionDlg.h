#pragma once
#include "BaseDlg.h"

/**
@brief      �˿��򵼶Ի���
*/
class CGuideReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGuideReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	CGuideReceptionDlg(CWnd* pParent = NULL);   // ���캯��
	~CGuideReceptionDlg();                      // ��������

    // �Ի�������
	enum { IDD = IDD_GUIDE_RECEPTION_DLG };     // ��ʾ����Ϣ

    void ShowGuideMsg(CString guideMsg, CString guideEnglishMsg , CString guidePortugueseMsg);   // ��ʾ����Ϣ
	void ShowGuideMsg(DWORD guideMsgID);        // ��ʾ����Ϣ



private:
    void OnDraw(CDC* pDC);                      // ������ƺ���

	DWORD m_guideMsgID;					        // ����Ϣ
    CString     m_guideChineseMsg;			    // ��������Ϣ
    CString     m_guideEnglishMsg;			    // Ӣ������Ϣ
	//CString     m_guidePortugueseMsg;			// ������������Ϣ
    COLORREF    m_color;			            // ��ʾ��ɫ
};
