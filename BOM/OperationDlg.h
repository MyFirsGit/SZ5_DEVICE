#pragma once
#include "basedlg.h"
#include "service.h"
#include "CompositGroupBox.h"
#include "ButtonGroup.h"
#include "gedit.h"
#include "BomInfo.h"
#include "MaintenanceInfo.h"
#include "ExceptionMgr.h"
#include "MessageIDDefinition.h"
#include "UIDefinition.h"
#include <vector>
#include "Keyboard.h"
using namespace std;

const static  TCHAR* const BUTTON_LAST_STATUS_KEY = _T("BUTTON_LAST_STATUS_KEY");

/**
@brief      ҵ��Ի�����
*/
class COperationDlg :
	public CBaseDlg
{
	DECLARE_DYNAMIC(COperationDlg)
    DECLARE_MESSAGE_MAP()

public:
    // ��ͬ��ĻԪ�أ���ť�����ּ��̣�����
    static const ULONG BTN_OK               = 0X0001;                               // ȷ����ť
    static const ULONG BTN_BACK             = 0X0002;                               // ���ذ�ť
    static const ULONG BTN_PRINT            = 0X0004;                               // ��ӡ��ť
	static const ULONG BTN_RESET		    = 0X0008;								// ���ð�ť
    static const ULONG BASE_AREA            = 0X0010;                               // ������������
    static const ULONG CASH_AREA            = 0X0020;                               // �ֽ���������
    static const ULONG EDIT_BOX             = 0X0040;                               // �༭��
	static const ULONG GUIDE_AREA			= 0x0080;								// �򵼿�
	static const ULONG KEYBOARD_AREA	 = 0x0100;								//����

	COperationDlg(UINT ID, CService* pService,CRect rect = OPERATION_FRMAE_RECT,bool needFunctionArea = true);                                     // ���캯��
    COperationDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea = true);   // ���캯��
    ~COperationDlg(void);                                                           // ��������
	
	virtual void OnServiceBusy(bool busy);
    void ShowCompoment(ULONG ulCompomentFlag);                                      // ��ʾ���
    void HideCompoment(ULONG ulCompomentFlag);                                      // �������
	
	PGSTATIC_INFO GetBaseInfo();
	PGSTATIC_INFO GetCashInfo();

	CGStatic* GetBaseArea();
	CGStatic* GetCashArea();
	CString GetEditText(int index);                                                 // ��ȡ�༭�����ʾ����
	
protected:
	virtual void OnDraw(CDC* pDC);                                                  // ������ƺ���
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag = 0x00);  // ���洴������
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // ��ť���º�Ĵ���
    afx_msg LRESULT OnEditSetFocus(WPARAM,LPARAM);                                  // �༭��õ��������Ϣ��Ӧ����

    void SetEditFocus(int index);                                                   // �������뽹�㵽�༭��
    void SetEditText(int index, CString text);                                      // ���ñ༭�����ʾ����
    CGEdit* GetEdit(int index);                                                     // ��ȡ�༭�����
	CXButton*       m_btnOK;                                                        // ȷ����ť
	CXButton*       m_btnBack;                                                      // ���ذ�ť
	CXButton*       m_btnPrint;                                                     // ��ӡ��ť
	CXButton*		m_btnReset;														// ���ð�ť
    CGStatic*       m_baseInfoArea;                                                 // ������������
    CGStatic*       m_cashInfoArea;                                                 // �ֽ���������
    PGSTATIC_INFO   m_baseInfo;                                                     // ��������
    PGSTATIC_INFO   m_cashInfo;                                                     // �ֽ�����
	CButtonGroup*		m_Right_1_Group;
	CButtonGroup*		m_Right_2_Group;
	CButtonGroup*		m_Left_1_Group;
	CButtonGroup*		m_Left_2_Group;
	CKeyboard*           m_Keyboard;
	vector<BTN_INFO> m_vec_Left_1_BTN_INFO;
	vector<BTN_INFO> m_vec_Left_2_BTN_INFO;
	vector<BTN_INFO> m_vec_Right_1_BTN_INFO;
	vector<BTN_INFO> m_vec_Right_2_BTN_INFO;
	LABEL_GROUP	m_cashLabelGroup[LABEL_GROUP_EDIT_ROWS_3];
private:
	
    void Initialize();                                                              // ���ڻ�
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);                    // ��ʾ���������
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);                         // ��ʾ���������
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);                         // ��ȡ���������
	CXButton* BuildOperationButton(CRect rect,CString text,CString msg);
	bool needFunctionArea ;
    vector<CGEdit*> m_editArray;                                                    // �༭���б�
    int m_curEditIndex;                                                             // ��ǰ�༭����
};
