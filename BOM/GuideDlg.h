#pragma once
#include "BaseDlg.h"
#include "GroupBox.h"
#include "lable.h"

#define theGuideDlg CGuideDlg::GetInstance()

/**
@brief      վԱ�򵼶Ի���
*/
class CGuideDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CGuideDlg)
    DECLARE_MESSAGE_MAP()

public:
	CGuideDlg(CWnd* pParent = NULL);            // ���캯��
	~CGuideDlg();

    // �Ի�������
    enum { IDD = IDD_GUIDE_DLG };

    void ShowGuideMsg(CString guideMsg);        // ��ʾ����Ϣ
    CString GetGuideMsg();                      // ��ȡ����Ϣ


	BOOL Create(CWnd* pParentWnd /* = NULL */);

	static CGuideDlg& GetInstance(){
		return instance;
	};

private:
    void OnDraw(CDC* pDC);                      // ������ƺ���

    CString     m_guideMsg;			            // ����Ϣ(����ɫ����)
	CString     m_msg;			                // ����Ϣ
    COLORREF    m_color;			            // ��ʾ��ɫ

	static CGuideDlg instance;

};
