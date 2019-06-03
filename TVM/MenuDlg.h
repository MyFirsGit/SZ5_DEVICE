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
	CMenuDlg();                           // ���캯��
	virtual ~CMenuDlg();                                    // ��������
	enum { IDD = IDD_MENU_LOGIN_DLG };                        // �Ի�����
	void SetMainMenuItems(vector<Menu_Item*>& vecMenuItem);
	void ShowLanguageButtonGroup(bool isShow);
	void ShowServiceButtonGroup(bool isShow);
	void ShowCancleButton(bool isShow);
	void ShowPayMethodButton(bool showNet/*��ʾɨ��֧����ť*/,bool showCash=true/*��ʾ�ֽ�֧����ť*/);
	void SetCancleButtonEnable(bool isEnable);
	void ShowChargeButton(bool isShow);
	CButtonGroup* GetMainGroup();
	virtual void OnServiceBusy(bool busy);
	void RefreshCancelButton();
	bool IsCancleButtonEnable();
	void ConvertPng(CImage* image);                           // ͸������
	void SetNotifyContent(CString szCont = _T(""));
protected:
	virtual void  OnDraw(CDC* pDC);
	BOOL OnEraseBkgnd(CDC* pDC);
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);          // ���洴������
	afx_msg LRESULT OnCancel(WPARAM,LPARAM);
	LRESULT MainMenuClick(WPARAM wParam, LPARAM lParam);          // ���˵����º�Ĵ���
	LRESULT LanguageMenuClick(WPARAM wParam, LPARAM lParam);      // �����˵����º�Ĵ���
	LRESULT ChargeMenuClick(WPARAM wParam, LPARAM lParam);		  // ��ֵ��ť����
	LRESULT CashPayMenuClisk(WPARAM wParam, LPARAM lParam);
	LRESULT NetPayMenuClish(WPARAM wParam, LPARAM lParam);
	void OnMenuKindChanged(CMenuMgr::MENU_KIND&,CMenuMgr::MENU_KIND&);
	void OnMenuItemChanged(Menu_Item*);

	void  OnStartingService(CService* service);
	void  OnStartedService(CService* service);
	void  OnButtonClicking(CXButton*,bool*);
	bool  ConfirmDoService(SERVICE_ID& serviceID);
	void  DoService(SERVICE_ID& serviceID);
	bool  IsSupportRecharge();										 //������˵���ťʱ���ж��Ƿ�֧�ֳ�ֵ
	bool  IsAnalyzeSupported();										 // ��ǰ�Ƿ�֧�ֲ�ѯҵ��
	bool  IsIssueProductSupported();								 // ��ǰ�Ƿ�֧���۲�Ʒҵ��
	const static int SERVICE_MENU_BUTTON_COUNT  = 4;                 // ҵ��ť����
	const static int LANGUAGE_MENU_BUTTON_COUNT = 2;                // ���԰�ť����
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
