#include "stdafx.h"
#include "SelectLanguageSvc.h"
#include "SelectLanguageBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CSelectLanguageSvc,CTVMForeService)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_ENTER,OnKeyboardEnter)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageSvc::CSelectLanguageSvc()
	:CTVMForeService(WAIT_MAINTENANCE_SVC)
{
	// 设置默认语言
	theAPP_SESSION.SetCurrentOperatorLanguageType(LANGUAGE_ID_CHINESE);
	// 切换菜单种类
	theMENU_MGR.SetMenuKind(CMenuMgr::MENU_MAINTAIN);

    // 设置操作员画面
    m_pDialogFlow->AddDialog(IDD_20112_STATION_SET_DLG, new CSelectLanguageBaseDlg(this));

    // 设置乘客画面

    // 设置画面分组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE, _opl(GUIDE_ENTER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, CSelectLanguageBaseDlg::IDD);

    // 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_BASE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageSvc::~CSelectLanguageSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSelectLanguageSvc::Model& CSelectLanguageSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSelectLanguageSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [Enter] 键按下消息

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSelectLanguageSvc::OnKeyboardEnter(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// 获取输入内容
	int index = theAPP_SESSION.GetInputToInt();

	// 转换为语言类型
	LANGUAGETYPE_T language;
	switch (index)
	{
	case 1:
		language = LANGUAGE_ID_CHINESE;
		break;
	case 2:
		language = LANGUAGE_ID_ENGLISH;
		break;
		/*case 3:
		language = LANGUAGE_ID_PORTUGUESE;
		break;*/
	default:
		language = LANGUAGE_ID_UNSPECIFIED;
	}

	// 验证输入内容，设置语言，并启动登陆业务。
	if (language != LANGUAGE_ID_UNSPECIFIED)
	{
		theAPP_SESSION.SetCurrentOperatorLanguageType(language);
		theSERVICE_MGR.StartService(LOGIN_SVC);
	}
	else
	{
		theAPP_SESSION.ShowOfficeGuide(GUIDE_MENU_CODE_ILLEGAL);
	}

	return TRUE;
}
