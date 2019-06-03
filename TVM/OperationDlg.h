#pragma once
#include "basedlg.h"
#include "service.h"
#include "CompositGroupBox.h"
#include "ButtonGroup.h"
#include "gedit.h"
#include "SysInfo.h"
#include "MaintenanceInfo.h"
#include "ExceptionMgr.h"
#include "MessageIDDefinition.h"
#include "UIDefinition.h"
#include <vector>
#include "Keyboard.h"
using namespace std;

const static  TCHAR* const BUTTON_LAST_STATUS_KEY = _T("BUTTON_LAST_STATUS_KEY");

/**
@brief  COperationDlg ά��ҵ��Ի�����
*/
class COperationDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(COperationDlg)
    DECLARE_MESSAGE_MAP()

public:
    // ��ͬ��ĻԪ�أ���ť�����ּ��̣�����
	static const ULONG GUIDE_AREA		= 0X0001; // �򵼿�
	static const ULONG BASE_AREA		= 0X0002; // ������������
	static const ULONG EDIT_BOX			= 0x0004; // �༭��

	COperationDlg(UINT ID, CService* pService, CRect rect = OPERATOR_BASE_RECT);
    COperationDlg(UINT ID, CRect rect, COLORREF bkColor, CService* pService);
    ~COperationDlg(void);
	
	virtual void OnServiceBusy(bool busy);
    void ShowCompoment(ULONG ulCompomentFlag);						// ��ʾ���
    void HideCompoment(ULONG ulCompomentFlag);						// �������
	
	PGSTATIC_INFO GetBaseInfo();									// ��ȡ��������						
	CGStatic* GetBaseArea();										// ��ȡ������������ؼ�
	CString GetEditText(int index);									// ��ȡ�༭����ʾ���ı�
	int GetEditFocusID();											// ��ȡ��ǰ����༭��ID

protected:
	virtual void OnDraw(CDC* pDC);														// �������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag = 0x00);  // ���洴��
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);				// ���ڼ���

	LRESULT OnEditSetFocus(WPARAM,LPARAM);							// �༭��õ��������Ϣ��Ӧ
	void SetEditFocus(int index);									// ���ñ༭�򽹵�
	void SetEditText(int index, CString text);						// ���ñ༭���ı�
	CGEdit* GetEdit(int index);										// ��ȡ�༭�����

	CGStatic* m_baseInfoArea;										// ������������
	PGSTATIC_INFO m_baseInfo;										// ��������

private:
    void Initialize();												// ���ڻ�
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);	// ��ʾ���������
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);			// ��ʾ���������
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);			// ��ȡ���������

    vector<CGEdit*> m_editArray;									// �༭���б�
    int m_curEditIndex;												// ��ǰ�༭����
};
