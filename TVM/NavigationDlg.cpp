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

const static TCHAR* LEFT_SQUARE_BRACKET = _T("[");		// ������
const static TCHAR* RIGHT_SQUARE_BRACKET = _T("]");		// �ҷ�����

DWORD CNavigationDlg::m_hook_keys = 0;
HHOOK CNavigationDlg::m_keyboard_hook = NULL;

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService* pService  Dialog��Ӧ��Service

@retval     ��

@exception  ��
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CNavigationDlg::~CNavigationDlg()
{
	UnhookWindowsHookEx(m_keyboard_hook);
	m_keyboard_hook = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int  0:�����ɹ�  -1:����ʧ��

@exception  ��
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
@brief      �������

@param      (i)CDC* pDC �豸������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
	DrawText(pDC, m_navi_rect, m_auto_level + m_manual_level, NORMAL_FONT_NAME,NORMAL_FONT_SIZE, FW_NORMAL, WHITE, DT_VCENTER|DT_SINGLELINE);           // ��ָ��λ����ʾ����
	//DrawText(pDC, m_navi_rect, m_auto_level, NORMAL_FONT_NAME, WHITE, DT_VCENTER|DT_SINGLELINE);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˵�����ı�ʱ����Ӧ��һ�����ڳ˿�ҵ��˵�����ʼ���˵����쳣�˵�����½�˵��Ͳ���Ա�˵����л���

@param      (i)CMenuMgr::MENU_KIND& oldMenuKind �ɵĲ˵�����
@param      (i)CMenuMgr::MENU_KIND& newMenuKind �µĲ˵�����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnMenuKindChanged(CMenuMgr::MENU_KIND& oldMenuKind,CMenuMgr::MENU_KIND& newMenuKind)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˵�ѡ��״̬�ı�ʱ����Ӧ��һ�����ڲ���ԱȨ�ޡ��쳣������Ƿ���õ����ã�

@param      (i)Menu_Item* menuItem �˵���

@retval     ��

@exception  ��
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
@brief      ��������ǰ����Ӧ��һ�����ڶ���������ܹ���һ��SVC�࣬���磺��Ʊҵ������ҵ����һ��SVC��

@param      CService* pSvc ��Ҫ�����ķ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::OnStartingService(CService* pSvc)
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �������������Ӧ��һ�������жϵ�ǰҵ���ǲ���Ĭ��ҵ���Ա��ڿ����ȴ���״̬�����磺��Ʊҵ��ΪĬ��ҵ�񣬿����ȴ���״̬��

@param      CService* pSvc �Ѿ������ķ���

@retval     ��

@exception  ��
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
@brief      ����ҵ��

@param      (i)SERVICE_ID& serviceID ����ID

@retval     ��

@exception  ��
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
@brief      ����id��Ӧ�Ĳ˵�

@param      (i)SERVICE_ID& serviceID ����

@retval     Menu_Item*

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
Menu_Item* CNavigationDlg::SearchService(SERVICE_ID& serviceID)
{
	vector<Menu_Item*>& menu_list = theMENU_MGR.GetMaintainMenuItems();
	for (vector<Menu_Item*>::iterator iter = menu_list.begin(); iter != menu_list.end(); ++iter)
	{
		// �Ƿ�һ���˵�
		Menu_Item* first_menu = *iter;
		if (first_menu->serviceID == serviceID) return first_menu;

		// �Ƿ�����˵�
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
@brief      ��ʾ�˵�����

@param      (i)SERVICE_ID& serviceID ����ID

@retval     ��

@exception  ��
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
			// ͨ��SERVICE_ID����Menu_Item
			Menu_Item* menu = SearchService(serviceID);
			if (menu == NULL || !menu->isEnable || !menu->isVisible) break;

			// ͨ�������������÷ָ���
			LANGUAGETYPE_T language = theAPP_SESSION.GetCurrentOperatorLanguageType();
			CString split = (language == LANGUAGE_ID_CHINESE) ? _T("��") : _T("(");
			if(menu->pParent != NULL)
			{
				//menu = menu->pParent;
			}
			
			// �Զ��ϳɲ˵�����
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

	// �ֶ�׷�Ӳ˵�����
	if (m_manual_level != _T(""))
	{
		//m_auto_level += LEFT_SQUARE_BRACKET + m_manual_level + RIGHT_SQUARE_BRACKET;
	}

	// ˢ�²˵�������
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
@brief      ׷�Ӳ˵�����

@param      (i)CString level ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::AppendLevel(CString level)
{
	m_manual_level = level;

	if(!m_manual_level.IsEmpty()){
		// �ֶ�׷�Ӳ˵�����
		m_manual_level = LEFT_SQUARE_BRACKET + m_manual_level + RIGHT_SQUARE_BRACKET;
	}

	// ˢ�²˵�������
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
@brief      ����ָ������

@param      (i)DWORD keys  �����İ��������� ��λ�룩

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::SetKeyboard(DWORD keys)
{
	m_hook_keys = keys;
	theKB_LOG->WriteData(_T("CNavigationDlg"),_T("SetKeyboard"), __LINE__, _T("State = [0x%04x]"), keys);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ָ������

@param      (i)DWORD keys  ���õİ���

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CNavigationDlg::DiableKeyboard(DWORD keys)
{
	m_hook_keys &= ~keys;
	theKB_LOG->WriteData(_T("CNavigationDlg"),_T("DisableKeyboard"), __LINE__, _T("State = [0x%04x]"), keys);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ֲ����ӻص�����

@param      (i)int code
@param      (i)WPARAM wParam �����ֵ
@param      (i)LPARAM lParam ����״̬

@retval     LRESULT  TRUE:����  FALSE:������

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK CNavigationDlg::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	// 0.æµ״̬��ʱ����ʱ���ر�
	if(theSERVICE_MGR.IsForeServiceBusy()){
		if(ADDING_COIN_TO_CYCLE_CHANGE_COIN_BOX_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){// �ӱ�
			// ������ӦEnter
		}
		else if(ADDING_COIN_TO_DEDICATED_CHANGE_COIN_BOX_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
			// ������ӦEnter
		}
		else return FALSE;
	}
	else{
		DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
		if (dwOutOfTime > 0)
		{
			// 1.�Զ��ǳ�ʱ�����0ʱ�����Զ��ǳ���ʱ��
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF,dwOutOfTime);
		}
	}
	// 2.����״̬
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
