#pragma once
#include "BaseDlg.h"

/**
@brief   ��ӭ���ٻ���
*/
class CWelcomeReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CWelcomeReceptionDlg)
    //DECLARE_MESSAGE_MAP()

public:
	CWelcomeReceptionDlg(CService* pService);   // ���캯��
	~CWelcomeReceptionDlg();

    // �Ի�������
	enum { IDD = IDD_10002_WELCOME_DLG };
	void Show();									// ��ʾ����
private:
	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// ���漤���Ĵ���
	void OnDraw(CDC* pDC);													// ������ƺ���

};
