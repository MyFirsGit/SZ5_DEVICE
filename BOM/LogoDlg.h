#pragma once
#include "BaseDlg.h"

// CLogoDlg �Ի���


class CLogoDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CLogoDlg)
    DECLARE_MESSAGE_MAP()

public:
    CLogoDlg(CWnd* pParent = NULL);     // ���캯��
	~CLogoDlg();                        // ��������

    // �Ի�������
	enum { IDD = IDD_LOGO_DLG };

private:
    void OnDraw(CDC* pDC);              // ������ƺ���
	void OnVersionNoChanged(CVersionInfo::VER_TYPE,CVersionInfo::PROG_TYPE,DWORD);//�汾�ŷ����ı���Ӧ������
private:
    HBITMAP m_hLogoBmp;                 // Logo���
};
