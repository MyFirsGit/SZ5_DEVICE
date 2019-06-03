#pragma once
#include "BaseDlg.h"
#include "service.h"
#include "receptionstaticinfo.h"
#include "SysException.h"
#include "InnerException.h"
#include "ExceptionMgr.h"


/**
@brief      �˿͸���Ի���
*/

class CReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:

	static const ULONG BUSINESS_AREA = 0x0001;					   //ҵ������

    CReceptionStaticInfo* m_BusinessArea;
	PMSG_INFO m_BusinessInfo;                                      // ҵ������

	CReceptionDlg(UINT ID, CService* pService);                     // ���캯��
	~CReceptionDlg(void);                                           // ��������

    void ShowCompoment(ULONG ulCompomentFlag);                      // ��ʾ���
    void HideCompoment(ULONG ulCompomentFlag);                      // �������
	void ShowCompoment(CWnd *pCompoment);                           // ��ʾ���
	void HideCompoment(CWnd *pCompoment);                           // �������

protected:
	virtual void OnDraw(CDC* pDC);                                  // ������ƺ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulAreaFlag = 0x00);   // ���洴������
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); // ���漤���Ĵ���


private:
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);    // ��ʾ���������
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);         // ��ʾ���������
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);         // ȡ�����������

};
