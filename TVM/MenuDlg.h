#pragma once
#include "XButton.h"
#include "uidefinition.h"
#include "servicedefinition.h"
#include "ButtonGroup.h"
#include "menumgr.h"
#include "BaseDlg.h"


class CMainSvc;

class CMenuDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(CMenuDlg)
	DECLARE_MESSAGE_MAP()

public:
	CMenuDlg();                           // 构造函数
	virtual ~CMenuDlg();                                    // 析构函数
	enum { IDD = IDD_MENU_LOGIN_DLG };                        // 对话框编号
	void SetMainMenuItems(vector<Menu_Item*>& vecMenuItem);
	void ShowLanguageButtonGroup(bool isShow);
	void ShowServiceButtonGroup(bool isShow);
	void ShowCancleButton(bool isShow);
	void ShowPayMethodButton(bool showNet/*显示扫码支付按钮*/,bool showCash=true/*显示现金支付按钮*/);
	void SetCancleButtonEnable(bool isEnable);
	void ShowChargeButton(bool isShow);
	CButtonGroup* GetMainGroup();
	virtual void OnServiceBusy(bool busy);
	void RefreshCancelButton();
	bool IsCancleButtonEnable();
	void ConvertPng(CImage* image);                           // 透明处理
	void SetNotifyContent(CString szCont = _T(""));
protected:
	virtual void  OnDraw(CDC* pDC);
	BOOL OnEraseBkgnd(CDC* pDC);
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);          // 画面创建函数
	afx_msg LRESULT OnCancel(WPARAM,LPARAM);
	LRESULT MainMenuClick(WPARAM wParam, LPARAM lParam);          // 主菜单按下后的处理
	LRESULT LanguageMenuClick(WPARAM wParam, LPARAM lParam);      // 二级菜单按下后的处理
	LRESULT ChargeMenuClick(WPARAM wParam, LPARAM lParam);		  // 充值按钮按下
	LRESULT CashPayMenuClisk(WPARAM wParam, LPARAM lParam);
	LRESULT NetPayMenuClish(WPARAM wParam, LPARAM lParam);
	void OnMenuKindChanged(CMenuMgr::MENU_KIND&,CMenuMgr::MENU_KIND&);
	void OnMenuItemChanged(Menu_Item*);

	void  OnStartingService(CService* service);
	void  OnStartedService(CService* service);
	void  OnButtonClicking(CXButton*,bool*);
	bool  ConfirmDoService(SERVICE_ID& serviceID);
	void  DoService(SERVICE_ID& serviceID);
	bool  IsSupportRecharge();										 //当点击菜单按钮时，判断是否支持充值
	bool  IsAnalyzeSupported();										 // 当前是否支持查询业务
	bool  IsIssueProductSupported();								 // 当前是否支持售产品业务
	const static int SERVICE_MENU_BUTTON_COUNT  = 4;                 // 业务按钮个数
	const static int LANGUAGE_MENU_BUTTON_COUNT = 2;                // 语言按钮个数
	CString m_strVersion;
	int m_nVerFrontSize;
	CButtonGroup* m_MenuGroup;

	CButtonGroup* m_LanguageGroup;
	vector<Menu_Item*>  m_LanguageMenus;
	CXButton* m_CancelBtn;
	CXButton* m_btnLanguage;
	CXButton* m_btnCharge;
	CXButton* m_btnCashPay;
	CXButton* m_btnNetPay;

	BTN_INFO* m_CurrentMainMenuInfo ;
private:
	CImage m_bkg;
	CImage m_statusImg;
	CImage m_moduleStatusRed;
	CImage m_moduleStatusGreen;
	CImage m_moduleStatusYellow;
	CImage m_moduleStatusGray;
	CImage m_imgBtnBkg;
	CImage m_imgBtnCancel;
};
