#pragma once
#include "BaseDlg.h"

/**
@brief   ��ӭ���ٻ��棨�û��治��ʾ��������ʾ��COutOfReceptionDlg�ϣ�
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
};
