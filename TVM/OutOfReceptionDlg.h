#pragma once
#include "BaseDlg.h"
#include "ReceptionHeadDlg.h"
#include "PictureEx.h"
#include "LanguageInfo.h"
/**
@brief      暂停服务画面
*/
typedef struct _tag_END_SVC_UI
{
	BOOL isTxtOnly;
	CString strPicPath;
	CString strTxtCN;
	CString strTxtEN;
	//CString strTxtPN;
	_tag_END_SVC_UI()
	{
		isTxtOnly = TRUE;
		strPicPath = _T("");
		strTxtCN = _cn(TXT_GUIDE_MSG_SYSTEM_INITIALIZING);
		strTxtEN = _en(TXT_GUIDE_MSG_SYSTEM_INITIALIZING);
		//strTxtPN = _po(TXT_GUIDE_MSG_SYSTEM_INITIALIZING);
	}
	_tag_END_SVC_UI(BOOL isTxt, CString strPath, CString strCN, CString strEN, CString strPN)
	{
		isTxtOnly = isTxt;
		strPicPath = strPath;
		strTxtCN = strCN;
		strTxtEN = strEN;
		//strTxtPN = strPN;
	}

}END_SVC_UI, *PEND_SVC_UI;

//服務狀態
typedef enum _enum_service_type{
	SERVICE_STATUES_INIT		= 0,
	SERVICE_STATUES_SHUTING		= 1,
	SERVICE_STATUES_OUTOF_SVC	= 2,
	SERVICE_STATUES_STOP_SVC	= 3,
}SERVICE_OF_STATUES;

class COutOfReceptionDlg : public CBaseDlg
{
	DECLARE_DYNAMIC(COutOfReceptionDlg)
	DECLARE_MESSAGE_MAP()

public:
	COutOfReceptionDlg(CService* pService = NULL);							// 构造函数
	COutOfReceptionDlg(CService* pService,END_SVC_UI& ui);
	~COutOfReceptionDlg();

	// 对话框数据
	enum { IDD = IDD_10001_OUTOF_DLG };

	void Show();									// 显示窗口
	virtual BOOL OnInitDialog();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:

	void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);			// 画面激活后的处理
	void OnDraw(CDC* pDC);													// 画面绘制函数,不让继承

	END_SVC_UI m_ui;														// 界面信息
	SERVICE_OF_STATUES m_curStatues;										// 當前狀態
	CReceptionHeadDlg *m_pHeadDLg;

	CPictureEx			m_picex;
};
