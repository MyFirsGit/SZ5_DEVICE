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
const static int DEFAULT_FIRST_LAYOUT = 0; // 默认第一个layout布局

#define ID_TIPS_DLG		1234		// 显示tips的控件ID
#define ID_TITLE_DLG	1235		// 显示业务标题的控件ID（用于储值卡购票）
#define	ID_TIPS_TIMER	1			// 刷新tips的定时器ID
#define TXT_COLOR_RED	_tl(LANGUAGE_RED)

//const static  TCHAR* const BUTTON_LAST_STATUS_KEY = _T("BUTTON_LAST_STATUS_KEY");

/**
@brief 乘客父类对话框
*/

const int MAX_RECP_GROUPBTN_COUNT = 12;            // 右侧按纽组按纽行数

// 右侧功能区按钮组信息
typedef struct button_group_info
{
	CString title;			// 标题
	int rows;				// 有几行按钮
	int cols;				// 有几列按钮
	bool bVisible;			// 是否可见
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

	CReceptionDlg(UINT ID, CService* pService,CRect rect = REC_FRMAE_RECT,bool needFunctionArea = true);                                     // 构造函数
	CReceptionDlg(UINT ID, CRect position, COLORREF bkColor, CService* pService,bool needFunctionArea = true);   // 构造函数
	~CReceptionDlg(void);                                           // 析构函数

	// 共同屏幕元素（按钮）定义
	static const ULONG BTN_OK               = 0X0001;                               // 确定按钮	
	static const ULONG BTN_PRINT            = 0X0002;                               // 打印按钮
	static const ULONG BTN_NOPRINT          = 0X0004;                               // 不打印按钮
	static const ULONG BASE_AREA            = 0X0010;                               // 基本数据区域
	static const ULONG CASH_AREA            = 0X0020;                               // 现金数据区域
	static const ULONG GUIDE_AREA           = 0X0040;                               // 提示画面区域
	static const ULONG LINE_AREA            = 0X0080;                               // 线路选择
	static const ULONG PRICES_AREA			= 0X0100;								// 票价选择
	static const ULONG NUMBER_AREA			= 0X0200;								// 张数选择
	static const ULONG TOP_STATION_AREA		= 0X0400;								// 票价选择
	static const ULONG FUNC_AREA			= 0X0800;								// 功能选择
	static const ULONG BTN_OK_AND_CANCEL1	= 0X2000;								// 确定与取消按钮成对出现的确定按钮
	static const ULONG BTN_OK_AND_CANCEL2	= 0X4000;								// 确定与取消按钮成对出现的取消按钮
	static const ULONG CASH_COUNT_AREA		= 0X8000;								// 已投入金额显示区域

	virtual void OnServiceBusy(bool busy);
	void ShowCompoment(ULONG ulCompomentFlag);                                      // 显示组件
	void HideCompoment(ULONG ulCompomentFlag);                                      // 隐藏组件

	PGSTATIC_INFO GetBaseInfo();
	PGSTATIC_INFO GetCashInfo();

	CGStatic* GetBaseArea();
	CGStatic* GetCashArea();
	
	void ShowCompoment(CWnd *pCompoment);                           // 显示组件
	void HideCompoment(CWnd *pCompoment);                           // 隐藏组件

	bool IsButtonEnable(bool bIsOkOrPrint = true);					// 判断按钮是否可用

	void HideAllCompoment();										// 关闭所有可现实的按钮
protected:
	virtual void OnDraw(CDC* pDC);                                  // 画面绘制函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulAreaFlag = 0x00);   // 画面创建函数
	//afx_msg LRESULT XButtonClick(WPARAM,LPARAM);                                    // 按钮按下后的处理
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void BuideButtonGroup(ULONG ulCompomentFlag);

	virtual void preShow();
	virtual void showComplete();

	CXButton*       m_btnOK;                                                        // 确定按钮
	CXButton*       m_btnPrint;                                                     // 打印按钮
	CXButton*       m_btnNOPrint;													// 取消打印按钮
	CXButton*       m_btnOKAndCancel1;                                              // 确定按钮(确定与取消成对出现时）
	CXButton*       m_btnOKAndCancel2;                                              // 确定按钮(确定与取消成对出现时）

	CGStatic* m_baseInfoArea;
	PGSTATIC_INFO m_baseInfo;                                      // 业务数据
	CGStatic* m_cashInfoArea;
	PGSTATIC_INFO m_cashInfo;                                      // 业务数据
	CGStatic* m_titleInfoArea;
	PGSTATIC_INFO m_titleInfo;                                      // 业务数据

	CReceptionGuideInfo* m_guideInfoArea;
	PIMAGE_GROUP_INFO   m_guideInfo;                               // 提示区图片信息                      



private:
	bool m_needFunctionArea ;                                         // 是否显示提示区域
    void ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow);    // 显示或隐藏组件
    void ShowHideCompoment(CWnd *pCompoment, int nCmdShow);         // 显示或隐藏组件
    void GetCompomentRect(CWnd *pCompoment, CRect* lpRect);         // 取得组件的区域
	CXButton* BuildOperationButton(CRect rect,CString text,CString msg,int btnType);
	void Initialize();                                              // 初期化

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

	// 上侧 线路图区域
	CButtonGroup* m_Top_1_Group;							// 左侧功能区第1个按钮组
	vector<BTN_INFO> m_vec_Top_1_BTN_INFO;					// 左侧功能区第1个按钮组按钮信息
	BUTTON_GROUP_INFO m_BTN_Top_1_Group_Info;				// 左侧功能按钮信息
	CImage* m_pImageBack_top;								// 上侧按钮组背景图

	// 左侧
	CButtonGroup* m_Left_1_Group;							// 左侧功能区第1个按钮组
	vector<BTN_INFO> m_vec_Left_1_BTN_INFO;					// 左侧功能区第1个按钮组按钮信息
	BUTTON_GROUP_INFO m_BTN_Left_1_Group_Info;				// 左侧功能按钮信息
	CImage* m_pImageBack_left;								// 左侧按钮组背景图

	// 右侧
	CButtonGroup* m_Right_1_Group;							// 右侧功能区第1个按钮组
	CButtonGroup* m_Right_2_Group;							// 右侧功能区第2个按钮组

	vector<BTN_INFO> m_vec_Right_1_BTN_INFO;				// 右侧功能区第1个按钮组按钮信息
	vector<BTN_INFO> m_vec_Right_2_BTN_INFO;				// 右侧功能区第2个按钮组按钮信息

	BUTTON_GROUP_INFO m_BTN_Right_1_Group_Info;				// 右侧功能区按钮组信息
	BUTTON_GROUP_INFO m_BTN_Right_2_Group_Info;				// 右侧功能区按钮组信息
	CImage* m_pImageBack_right;								// 右侧按钮组背景图

	CImage* m_pImageBack_center;						// 中间区域背景，小分辨率

	BUTTON_GROUP_INFO m_BTN_GROUP_INFO[4];					// 右侧功能区按钮组信息
	LABEL_GROUP	m_cashLabelGroup[LABEL_GROUP_EDIT_ROWS_4];

	virtual void InitializeFunctionBtns(long lComponents);
	virtual bool InitializeLineGroup(DWORD layoutID = DEFAULT_FIRST_LAYOUT);
public:
	// 按钮组组件
	typedef enum _tagIssueComponents
	{
		LINE_SELECT_BUTTON			= 0x0001,	// 线路按钮
		PRICE_BUTTON				= 0x0002,	// 票价按钮
		NUMBER_BUTTON				= 0x0004,	// 张数按钮
		TOP_STATION_BUTTON			= 0x0008,	// 热点车站按钮		
		FUNC_SELECT_BUTTON			= 0x0010,	// 选择功能按钮
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
	CGStatic* m_cashInInfo;//右上角投入金额显示控件
};
