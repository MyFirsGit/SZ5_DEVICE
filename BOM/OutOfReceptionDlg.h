#pragma once
#include "BaseDlg.h"

/**
@brief      ��ͣ������
*/
class COutOfReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(COutOfReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:
	COutOfReceptionDlg(CService* pService = NULL);							// ���캯��
	~COutOfReceptionDlg();

    // �Ի�������
	enum { IDD = IDD_10001_OUTOF_DLG };

    enum CONTENT_TYPE {
        OUT_OF_SERVICE,
        WELCOME
    };

    void Show(CONTENT_TYPE contentType);									// ��ʾ����

private:
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// ���漤���Ĵ���
    void OnDraw(CDC* pDC);													// ������ƺ���


    CONTENT_TYPE m_contentType;												// ��������

	HBITMAP m_hLogoBmp;														// Logo���
};
