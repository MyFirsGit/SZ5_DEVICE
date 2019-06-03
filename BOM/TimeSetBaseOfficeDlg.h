#pragma once
#include "OperationDlg.h"

/**
@brief  ʱ���趨
*/
class  CTimeSetBaseOfficeDlg : public  COperationDlg
{
	DECLARE_DYNAMIC(CTimeSetBaseOfficeDlg)
	DECLARE_MESSAGE_MAP()

public:
	CTimeSetBaseOfficeDlg(CService*pService) ;
	~CTimeSetBaseOfficeDlg() ;

	enum { IDD = IDD_01801_TIMESET_BASE_DLG };

private:
	LABEL_GROUP timeSetInfo[LABEL_GROUP_EDIT_ROWS_5];		// �¼���Ϣ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// ���洴������
	afx_msg LRESULT OnOK(WPARAM,LPARAM);					// ����ȷ�ϰ�ť��Ϣ
	afx_msg LRESULT OnReset(WPARAM,LPARAM);					// �������ð�ť��Ϣ
	void InitlabelTimeInfo();								// ��ʼ��BASE����LABEL���������
	bool CheckInput();										// ���������Ϣ�Ƿ�Ϸ�
	void HideGUIModule(ULONG ulCompomentFlag);
};