#include "stdafx.h"
#include "NavigationDlg.h"
#include "TVM.h"
#include "appsession.h"
#include "ServiceMgr.h"
#include "MenuMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CNavigationDlg, COperationDlg)
BEGIN_MESSAGE_MAP(CNavigationDlg, COperationDlg)
	ON_WM_CREATE()
END_MESSAGE_MAP()

#define theKB_LOG CFileLog::GetInstance(FILTER_KEYBOARD_LOG)

const static TCHAR* LEFT_SQUARE_BRACKET = _T("[");		// 左方括号
const static TCHAR* RIGHT_SQUARE_BRACKET = _T("]");		// 右方括号

DWORD CNavigationDlg::m_hook_keys = 0;
HHOOK CNavigationDlg::m_keyboard_hook = NULL;

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService  Dialog对应的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNavigationDlg::CNavigationDlg(CService* pService)
	: COperationDlg(CNavigationDlg::IDD, OPERATOR_NAVI_RECT , OPERATOR_GUIDE_FUNC_COLOR/*OPERATOR_HEAD_BK_COLOR*/, pService)
	, m_navi_rect(CRect(0,0,800 * RESOLUTION,30 * HRESOLUTION))
{
	theSERVICE_MGR.StartingService.AddHandler(this,&CNavigationDlg::OnStartingService);
	theSERVICE_MGR.StartedService.AddHandler(this,&CNavigationDlg::OnStartedService);
	theMENU_MGR.MenuKindChanged.AddHandler(this,&CNavigationDlg::OnMenuKindChanged);
	theMENU_MGR.MenuItemStatusChanged.AddHandler(this,&CNavigationDlg::OnMenuItemStatusChanged);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CNavigationDlg::~CNavigationDlg()
{
	UnhookWindowsHookEx(m_keyboard_hook);
	m_keyboard_hook = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:创建成功  -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CNavigationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	try {
		if (__super::OnCreate(lpCreateStruct) == -1) {
			return -1;
		}
		ShowWindow(TRUE);
		UpdateWindow();

		m_hook_keys = KEY_NOBODY;
		m_keyboard_hook = SetWindowsHookEx(WH_KEYBOARD, CNavigationDlg::KeyboardProc, NULL, GetCurrentThreadId());
	}
	catch (CSysException& e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(GetService()->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制

@param      (i)CDC* pDC 设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	DrawText(pDC, m_navi_rect, m_auto_level + m_manual_level, NORMAL_FONT_NAME,NORMAL_FONT_SIZE, FW_NORMAL, WHITE, DT_VCENTER|DT_SINGLELINE);           // 在指定位置显示文字
	//DrawText(pDC, m_navi_rect, m_auto_level, NORMAL_FONT_NAME, WHITE, DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      菜单种类改变时的响应（一般用于乘客业务菜单、初始化菜单、异常菜单、登陆菜单和操作员菜单的切换）

@param      (i)CMenuMgr::MENU_KIND& oldMenuKind 旧的菜单种类
@param      (i)CMenuMgr::MENU_KIND& newMenuKind 新的菜单种类

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnMenuKindChanged(CMenuMgr::MENU_KIND& oldMenuKind,CMenuMgr::MENU_KIND& newMenuKind)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      菜单选项状态改变时的响应（一般用于操作员权限、异常情况下是否可用的设置）

@param      (i)Menu_Item* menuItem 菜单项

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnMenuItemStatusChanged(Menu_Item* menuItem)
{
	vector<Menu_Item*> menu_list = theMENU_MGR.GetMaintainMenuItems();
	vector<Menu_Item*>::iterator iter = menu_list.begin();
	for (; iter != menu_list.end(); ++iter)
	{
		if ((*iter)->btnID == menuItem->btnID)
		{
			(*iter)->isEnable = menuItem->isEnable;
			(*iter)->isVisible = menuItem->isVisible;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动前的响应（一般用于多个二级功能共享一个SVC类，例如：售票业务多个子业务共享一个SVC）

@param      CService* pSvc 将要启动的服务

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnStartingService(CService* pSvc)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      服务启动后的响应（一般用于判断当前业务是不是默认业务，以便于开启等待中状态，例如：售票业务为默认业务，开启等待中状态）

@param      CService* pSvc 已经启动的服务

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnStartedService(CService* pSvc)
{
	SERVICE_STATE currentState = theSERVICE_MGR.GetState();
	if(currentState == INIT || currentState == OUT_OF || currentState == MAINTENANCE)
	{
		return;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      启动业务

@param      (i)SERVICE_ID& serviceID 服务ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::DoService(SERVICE_ID& serviceID)
{
	if(serviceID != UNKNOWN_SVC){
		bool cancel = false;
		CMainSvc* pSvc = GetService<CMainSvc>();
		pSvc->StartingForeService(serviceID,&cancel);
		if(!cancel)
		{
			theSERVICE_MGR.StartService(serviceID);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      查找id对应的菜单

@param      (i)SERVICE_ID& serviceID 服务

@retval     Menu_Item*

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
Menu_Item* CNavigationDlg::SearchService(SERVICE_ID& serviceID)
{
	vector<Menu_Item*>& menu_list = theMENU_MGR.GetMaintainMenuItems();
	for (vector<Menu_Item*>::iterator iter = menu_list.begin(); iter != menu_list.end(); ++iter)
	{
		// 是否一级菜单
		Menu_Item* first_menu = *iter;
		if (first_menu->serviceID == serviceID) return first_menu;

		// 是否二级菜单
		for (vector<Menu_Item*>::iterator iter = first_menu->children.begin(); iter != first_menu->children.end(); ++iter)
		{
			Menu_Item* second_menu = *iter;
			if (second_menu->serviceID == serviceID) return second_menu;
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示菜单级别

@param      (i)SERVICE_ID& serviceID 服务ID

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::ShowLevel(SERVICE_ID& serviceID)
{
	m_auto_level = _T("");
	m_manual_level = _T("");
	Menu_Item* menu1 = NULL;
	switch (serviceID)
	{
	case INIT_SVC:
		m_auto_level += LEFT_SQUARE_BRACKET + _opl(TXT_ID_INIT_SVC) + RIGHT_SQUARE_BRACKET;
		break;
	/*case WAIT_MAINTENANCE_SVC:
		m_auto_level += LEFT_SQUARE_BRACKET + _opl(TXT_ID_SELECT_LANGUAGE_SVC) + RIGHT_SQUARE_BRACKET;
		break;*/
	case MAINTENANCE_SVC:
		m_auto_level += LEFT_SQUARE_BRACKET + _opl(TXT_ID_MAINTENANCE_MENU_SVC) + RIGHT_SQUARE_BRACKET;
		break;
	case LOGIN_SVC:
		m_auto_level += LEFT_SQUARE_BRACKET + _opl(TXT_ID_LOGIN_SVC) + RIGHT_SQUARE_BRACKET;
		break;
	default:
		{
			// 通过SERVICE_ID查找Menu_Item
			Menu_Item* menu = SearchService(serviceID);
			if (menu == NULL || !menu->isEnable || !menu->isVisible) break;

			// 通过语言种类设置分隔符
			LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentOperatorLanguageType();
			CString split = (language == LANGUAGE_ID_CHINESE) ? _T("（") : _T("(");
			if(menu->pParent != NULL)
			{
				//menu = menu->pParent;
			}
			
			// 自动合成菜单级别
			do 
			{
				int split_pos = menu->text.Find(split);
				CString level = LEFT_SQUARE_BRACKET;
				if (split_pos < 0)
					level += menu->text;
				else
					level += menu->text.Left(split_pos);
				level += RIGHT_SQUARE_BRACKET;
				m_auto_level = level + m_auto_level;

				menu = menu->pParent;
			} while (menu);
			m_auto_level = LEFT_SQUARE_BRACKET + _opl(TXT_ID_MAINTENANCE_MENU_SVC) + RIGHT_SQUARE_BRACKET + m_auto_level;
		}
	}

	// 手动追加菜单级别
	if (m_manual_level != _T(""))
	{
		//m_auto_level += LEFT_SQUARE_BRACKET + m_manual_level + RIGHT_SQUARE_BRACKET;
	}

	// 刷新菜单导航栏
	if (IsWindow(m_hWnd))
	{
		InvalidateRect(&m_navi_rect);
		if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		{
			UpdateWindow();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      追加菜单级别

@param      (i)CString level 级别

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::AppendLevel(CString level)
{
	m_manual_level = level;

	if(!m_manual_level.IsEmpty()){
		// 手动追加菜单级别
		m_manual_level = LEFT_SQUARE_BRACKET + m_manual_level + RIGHT_SQUARE_BRACKET;
	}

	// 刷新菜单导航栏
	if (IsWindow(m_hWnd))
	{
		InvalidateRect(&m_navi_rect);
		if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		{
			UpdateWindow();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启用指定按键

@param      (i)DWORD keys  启动的按键（常量 按位与）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::SetKeyboard(DWORD keys)
{
	m_hook_keys = keys;
	theKB_LOG->WriteData(_T("CNavigationDlg"),_T("SetKeyboard"), __LINE__, _T("State = [0x%04x]"), keys);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      禁用指定按键

@param      (i)DWORD keys  禁用的按键

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::DiableKeyboard(DWORD keys)
{
	m_hook_keys &= ~keys;
	theKB_LOG->WriteData(_T("CNavigationDlg"),_T("DisableKeyboard"), __LINE__, _T("State = [0x%04x]"), keys);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      局部钩子回调函数

@param      (i)int code
@param      (i)WPARAM wParam 虚拟键值
@param      (i)LPARAM lParam 按键状态

@retval     LRESULT  TRUE:处理  FALSE:不处理

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CNavigationDlg::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	// 0.忙碌状态计时器计时器关闭
	if(theSERVICE_MGR.IsForeServiceBusy()){
		if(ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){// 加币
			// 可以响应Enter
		}
		else if(ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
			// 可以响应Enter
		}
		else return FALSE;
	}
	else{
		DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
		if (dwOutOfTime > 0)
		{
			// 1.自动登出时间大于0时重置自动登出计时器
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF,dwOutOfTime);
		}
	}
	// 2.正常状态
	if((m_hook_keys != KEY_NOBODY) && ((DWORD)lParam & 0x40000000) && (code == HC_ACTION))
	{
		if ((m_hook_keys & KEY_NUMBER) &&((
			wParam == VK_NUMPAD1 || wParam == VK_NUMPAD2 || wParam == VK_NUMPAD3 ||
			wParam == VK_NUMPAD4 || wParam == VK_NUMPAD5 || wParam == VK_NUMPAD6 ||
			wParam == VK_NUMPAD7 || wParam == VK_NUMPAD8 || wParam == VK_NUMPAD9 || wParam == VK_NUMPAD0) || (
			wParam == '1' || wParam == '2' || wParam == '3' ||wParam == '4' ||
			wParam == '5' || wParam == '6' || wParam == '7' || wParam == '8' || wParam == '9' || wParam == '0')))
		{
			int number = (wParam > '9') ? (wParam - VK_NUMPAD0) : (wParam - '0');
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_NUMBER, number, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [%d]"), number);
		}
		else if ((wParam == VK_DECIMAL || wParam == VK_OEM_PERIOD) && (m_hook_keys & KEY_DOT))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_DOT, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [.]"));
		}
		else if ((wParam == VK_BACK) && (m_hook_keys & KEY_CLEAR))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_CLEAR, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [Clear]"));
		}
		else if ((wParam == VK_RETURN) && (m_hook_keys & KEY_ENTER))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_ENTER, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [Enter]"));
		}
		else if ((wParam == VK_ESCAPE) && (m_hook_keys & KEY_ESC))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_ESC, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [Esc]"));
		}
		else if ((wParam == VK_F1) && (m_hook_keys & KEY_F1))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_F1, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [F1]"));
		}
		else if (((wParam == VK_F2)|| (wParam == VK_MENU)) && (m_hook_keys & KEY_F2))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_F2, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [F2/Alt]"));
		}
		else if (((wParam == VK_F3)||(wParam == VK_CONTROL)) && (m_hook_keys & KEY_F3))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_F3, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [F3/Ctrl]"));
		}
		else if ((wParam == VK_F4) && (m_hook_keys & KEY_F4))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_F4, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [F4]"));
		}
		else if (((wParam == VK_F5) || (wParam == VK_SUBTRACT)) && (m_hook_keys & KEY_F5))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_F5, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [F5]"));
		}
		else if (((wParam == VK_F6) || wParam == VK_ADD) && (m_hook_keys & KEY_F6))
		{
			theSERVICE_MGR.GetCurService()->SendMessage(WM_KEYBOARD_F6, NULL, NULL);
			theKB_LOG->WriteData(_T("CNavigationDlg"),_T("KeyboardProc"), __LINE__, _T("Key = [F6]"));
		}
		else
			return FALSE;

		return TRUE;
	}

	return FALSE;
}
