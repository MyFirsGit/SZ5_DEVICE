#pragma once
#include "BaseDlg.h"
#include "service.h"
#include "SysException.h"
#include "InnerException.h"
#include "ExceptionMgr.h"
#include "gedit.h"
#include "SysInfo.h"
#include "MaintenanceInfo.h"
#include "ExceptionMgr.h"
#include "MessageIDDefinition.h"
#include "UIDefinition.h"
#include <vector>
#include "ReceptionBitmapInfo.h"
#include "LineGroup.h"
using namespace std;
const static int DEFAULT_FIRST_LAYOUT = 0; // Ĭ�ϵ�һ��layout����

#define ID_TIPS_DLG		1234		// ��ʾtips�Ŀؼ�ID
#define ID_TITLE_DLG	1235		// ��ʾҵ�����Ŀؼ�ID�����ڴ�ֵ����Ʊ��
#define	ID_TIPS_TIMER	1			// ˢ��tips�Ķ�ʱ��ID
#define TXT_COLOR_RED	_tl(LANGUAGE_RED)

//const static  TCHAR* const BUTTON_LAST_STATUS_KEY = _T("BUTTON_LAST_STATUS_KEY");

/**
@brief �˿͸���Ի���
*/

const int MAX_RECP_GROUPBTN_COUNT = 12;            // �ҲఴŦ�鰴Ŧ����

// �Ҳ๦������ť����Ϣ
typedef struct button_group_info
{
	CString title;			// ����
	int rows;				// �м��а�ť
	int cols;				// �м��а�ť
	bool bVisible;			// �Ƿ�ɼ�
	button_group_info(){
		title = _T("--");
		rows = 0;
		cols = 0;
		bVisible = true;
	};
}BUTTON_GROUP_INFO,*PBUTTON_GROUP_INFO;

class CReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CReceptionDlg)
    DECLARE_MESSAGE_MAP()

public:

	CReceptionDlg(UINT ID, CService* pService,CRect rect = REC_FRMAE_RECT,bool needFunctionArea = true);                                     // ���캯��
	CReceptionDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea = true);   // ���캯��
	~CReceptionDlg(void);                                           // ��������

	// ��ͬ��ĻԪ�أ���ť������
	static const ULONG BTN_OK               = 0X0001;                               // ȷ����ť	
	static const ULONG BTN_PRINT            = 0X0002;                               // ��ӡ��ť
	static const ULONG BTN_NOPRINT          = 0X0004;                               // ����ӡ��ť
	static const ULONG BASE_AREA            = 0X0010;                               // ������������
	static const ULONG CASH_AREA            = 0X0020;                               // �ֽ���������
	static const ULONG GUIDE_AREA           = 0X0040;                               // ��ʾ��������
	static const ULONG LINE_AREA            = 0X0080;                               // ��·ѡ��
	static const ULONG PRICES_AREA			= 0X0100;								// Ʊ��ѡ��
	static const ULONG NUMBER_AREA			= 0X0200;								// ����ѡ��
	static const ULONG TOP_STATION_AREA		= 0X0400;								// Ʊ��ѡ��
	static const ULONG FUNC_AREA			= 0X0800;								// ����ѡ��
	static const ULONG BTN_OK_AND_CANCEL1	= 0X2000;								// ȷ����ȡ����ť�ɶԳ��ֵ�ȷ����ť
	static const ULONG BTN_OK_AND_CANCEL2	= 0X4000;								// ȷ����ȡ����ť�ɶԳ��ֵ�ȡ����ť
	static const ULONG CASH_COUNT_AREA		= 0X8000;								// ��Ͷ������ʾ����

	virtual void OnServiceBusy(bool busy);
	void ShowCompoment(ULONG ulCompomentFlag);                                      // ��ʾ���
	void HideCompoment(ULONG ulCompomentFlag);                                      // �������

	PGSTATIC_INFO GetBaseInfo();
	PGSTATIC_INFO GetCashInfo();

	CGStatic* GetBaseArea();
	CGStatic* GetCashArea();
	
	void ShowCompoment(CWnd *pCompoment);                           // ��ʾ���
	void HideCompoment(CWnd *pCompoment);                           // �������

	bool IsButtonEnable(bool bIsOkOrPrint = true);					// �жϰ�ť�Ƿ����

	void HideAllCompoment();										// �ر����п���ʵ�İ�ť
protected:
	virtual void OnDraw(CDC* pDC);                                  // ������ƺ���
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulAreaFlag = 0x00);   // ���洴������
	//afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // ��ť���º�Ĵ���
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void BuideButtonGroup(ULONG ulCompomentFlag);

	virtual void preShow();
	virtual void showComplete();

	CXButton*       m_btnOK;                                                        // ȷ����ť
	CXButton*       m_btnPrint;                                                     // ��ӡ��ť
	CXButton*       m_btnNOPrint;													// ȡ����ӡ��ť
	CXButton*       m_btnOKAndCancel1;                                              // ȷ����ť(ȷ����ȡ���ɶԳ���ʱ��
	CXButton*       m_btnOKAndCancel2;                                              // ȷ����ť(ȷ����ȡ���ɶԳ���ʱ��

	CGStatic* m_baseInfoArea;
	PGSTATIC_INFO m_baseInfo;                                      // ҵ������
	CGStatic* m_cashInfoArea;
	PGSTATIC_INFO m_cashInfo;                                      // ҵ������
	CGStatic* m_titleInfoArea;
	PGSTATIC_INFO m_titleInfo;                                      // ҵ������

	CReceptionGuideInfo* m_guideInfoArea;
	PIMAGE_GROUP_INFO   m_guideInfo;                               // ��ʾ��ͼƬ��Ϣ                      



private:
	bool m_needFunctionArea ;                                         // �Ƿ���ʾ��ʾ����
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);    // ��ʾ���������
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);         // ��ʾ���������
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);         // ȡ�����������
	CXButton* BuildOperationButton(CRect rect,CString text,CString msg,int btnType);
	void Initialize();                                              // ���ڻ�

protected:
	DWORD m_currentLayoutID;
	UIINFO m_LineAreaUIInfo;
	CLineGroup* m_lineGroupArea;
	vector<UIINFO> m_vec_BtnUIInfo;
	vector<BTN_INFO> m_vec_buttonInfos;
	vector<TEXT_INFO> m_vec_textInfos;
	vector<LocationNumber_t> m_vec_hotLocation;
	DWORD m_dwCurPageLayout;
	WORD m_wCurrClickStation;

	// �ϲ� ��·ͼ����
	CButtonGroup* m_Top_1_Group;							// ��๦������1����ť��
	vector<BTN_INFO> m_vec_Top_1_BTN_INFO;					// ��๦������1����ť�鰴ť��Ϣ
	BUTTON_GROUP_INFO m_BTN_Top_1_Group_Info;				// ��๦�ܰ�ť��Ϣ
	CImage* m_pImageBack_top;								// �ϲఴť�鱳��ͼ

	// ���
	CButtonGroup* m_Left_1_Group;							// ��๦������1����ť��
	vector<BTN_INFO> m_vec_Left_1_BTN_INFO;					// ��๦������1����ť�鰴ť��Ϣ
	BUTTON_GROUP_INFO m_BTN_Left_1_Group_Info;				// ��๦�ܰ�ť��Ϣ
	CImage* m_pImageBack_left;								// ��ఴť�鱳��ͼ

	// �Ҳ�
	CButtonGroup* m_Right_1_Group;							// �Ҳ๦������1����ť��
	CButtonGroup* m_Right_2_Group;							// �Ҳ๦������2����ť��

	vector<BTN_INFO> m_vec_Right_1_BTN_INFO;				// �Ҳ๦������1����ť�鰴ť��Ϣ
	vector<BTN_INFO> m_vec_Right_2_BTN_INFO;				// �Ҳ๦������2����ť�鰴ť��Ϣ

	BUTTON_GROUP_INFO m_BTN_Right_1_Group_Info;				// �Ҳ๦������ť����Ϣ
	BUTTON_GROUP_INFO m_BTN_Right_2_Group_Info;				// �Ҳ๦������ť����Ϣ
	CImage* m_pImageBack_right;								// �Ҳఴť�鱳��ͼ

	CImage* m_pImageBack_center;						// �м����򱳾���С�ֱ���

	BUTTON_GROUP_INFO m_BTN_GROUP_INFO[4];					// �Ҳ๦������ť����Ϣ
	LABEL_GROUP	m_cashLabelGroup[LABEL_GROUP_EDIT_ROWS_4];

	virtual void InitializeFunctionBtns(long lComponents);
	virtual bool InitializeLineGroup(DWORD layoutID = DEFAULT_FIRST_LAYOUT);
public:
	// ��ť�����
	typedef enum _tagIssueComponents
	{
		LINE_SELECT_BUTTON			= 0x0001,	// ��·��ť
		PRICE_BUTTON				= 0x0002,	// Ʊ�۰�ť
		NUMBER_BUTTON				= 0x0004,	// ������ť
		TOP_STATION_BUTTON			= 0x0008,	// �ȵ㳵վ��ť		
		FUNC_SELECT_BUTTON			= 0x0010,	// ѡ���ܰ�ť
	};

	enum {MAX_TICKET_INFO_ROWS = 18};

	DWORD m_dwMaxTicketAmount;
	LABEL_GROUP m_TipsInfoGroup[MAX_TICKET_INFO_ROWS];
	LABEL_GROUP m_TicketInfoGroup[MAX_TICKET_INFO_ROWS];
	LABEL_GROUP m_NotifyInfoGroup[2];
	CStatic* m_pTipsDlg;
	CStatic* m_pTitleDlg;
	bool m_bShowBaseArea;
	CString m_strNotifyInfo;
	CString m_strTipInfo;
	void SetShowAreaInfo(ULONG showAreaID, PGSTATIC_INFO showAreaInfo);
	virtual void SetTipsShow(CString strTxt);
	CGStatic* m_cashInInfo;//���Ͻ�Ͷ������ʾ�ؼ�
};
