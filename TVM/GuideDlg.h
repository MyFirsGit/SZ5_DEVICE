#pragma once
#include "BaseDlg.h"
#include "GroupBox.h"
#include "lable.h"

#define theGuideDlg CGuideDlg::GetInstance()

/**
@brief ����Ա��������
*/
class CGuideDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGuideDlg)
    DECLARE_MESSAGE_MAP()

public:
    enum { IDD = IDD_GUIDE_DLG };

	BOOL Create(CWnd* pParentWnd /* = NULL */);						// ���洴��
	void OnDraw(CDC* pDC);											// �������
	void ShowGuideMsg(CString officeMsg, COLORREF color = BLACK);	// ��ʾ����Ϣ
    CString GetGuideMsg();											// ��ȡ����Ϣ

	void ShowGuideMsg(DWORD officeMsg);	// TODO�����޸�AppSession��ɾ���˺���

	static CGuideDlg& GetInstance()
	{
		return instance;
	};

private:
	CString m_msg;			// ����Ϣ
    COLORREF m_color;		// ��ʾ��ɫ

private:
	CGuideDlg(CWnd* pParent = NULL);
	CGuideDlg(const CGuideDlg&);
	CGuideDlg& operator=(const CGuideDlg&);
	~CGuideDlg();

	static CGuideDlg instance;
};
