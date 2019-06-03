#include "stdafx.h"
#include "Keyboard.h"
#include "BitmapManager.h"

IMPLEMENT_DYNAMIC(CKeyboard,CBaseDlg)

BEGIN_MESSAGE_MAP(CKeyboard,CBaseDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(KEYBOARD_GROUP_ID,OnKeyboardClick)
END_MESSAGE_MAP()

static pair<CString,CString> KeyMsg[] = {
	pair<CString,CString>(_T("A"),_T("A")),pair<CString,CString>(_T("B"),_T("B")),pair<CString,CString>(_T("C"),_T("C")),pair<CString,CString>(_T("D"),_T("D")),
	pair<CString,CString>(_T("1"),_T("1")),pair<CString,CString>(_T("2"),_T("2")),pair<CString,CString>(_T("3"),_T("3")),pair<CString,CString>(_T("E"),_T("E")),
	pair<CString,CString>(_T("4"),_T("4")),pair<CString,CString>(_T("5"),_T("5")),pair<CString,CString>(_T("6"),_T("6")),pair<CString,CString>(_T("F"),_T("F")),
	pair<CString,CString>(_T("7"),_T("7")),pair<CString,CString>(_T("8"),_T("8")),pair<CString,CString>(_T("9"),_T("9")),pair<CString,CString>(_T("X"),_T("X")),
	pair<CString,CString>(_T("0"),_T("0")),pair<CString,CString>(_T("."),_T(".")),pair<CString,CString>(_T("BS"),_T("¡û")),pair<CString,CString>(_T("OK"),_T("È·¶¨"))
};

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CKeyboard::CKeyboard(UINT ID, CRect rect, CWnd *pParent )
:CBaseDlg(ID,pParent,rect)
{
	UIINFO groupUI;
	groupUI.m_pParentWnd = this;
	groupUI.m_DockStyle = DOCKSTYLE::NONE;
	groupUI.m_Size = CPoint(rect.Width(),rect.Height());
	groupUI.m_BKMode = BKMODE_LAST;
	groupUI.m_BorderWidth=0;
	groupUI.m_pBackgroundImage = &theBitmap_MGR.m_pGroup;
	UIINFO btnUIInfo;
	btnUIInfo.m_Font.lfHeight = 20;
	btnUIInfo.m_Font.lfWeight = FW_MEDIUM;
	pButtonGroup = new CButtonGroup(groupUI,5,4,KEYBOARD_GROUP_ID,CButtonGroup::NONE,BUTTON_TYPE_NORMAL,btnUIInfo);
	vector<BTN_INFO> vecBtnInfo;
	for(int i=0;i<20;i++)
	{
		
		BTN_INFO btnInfo;
		btnInfo.btnMsg=KeyMsg[i].first;
		btnInfo.text = KeyMsg[i].second;
		btnInfo.isVisible = true;
		vecBtnInfo.push_back(btnInfo);
	}
	pButtonGroup->SetButtonInfos(vecBtnInfo);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CKeyboard::~CKeyboard()
{
	delete pButtonGroup;
	pButtonGroup = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CKeyboard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(lpCreateStruct);
	return pButtonGroup->Create();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CKeyboard::SetEnable(bool enabled)
{
	pButtonGroup->EnableWindow(enabled?TRUE:FALSE);
	pButtonGroup->Invalidate(FALSE);
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		pButtonGroup->UpdateWindow();
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief   ÏÔÊ¾¼üÅÌ   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CKeyboard::ShowKeys(bool showKeys)
{
	pButtonGroup->ShowButtons(showKeys);
	if(GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
	{
		pButtonGroup->UpdateWindow();
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CKeyboard::OnKeyboardClick(WPARAM wParam,LPARAM lParam)
{
		BTN_INFO* btnInfo = (BTN_INFO*)lParam;
		char c =0;
		if(btnInfo->btnMsg == _T("BS"))
		{
			c = VK_BACK;
		}
		else if(btnInfo->btnMsg == _T("OK"))
		{
			c = VK_RETURN;
		}
		else if(btnInfo->btnMsg == _T("."))
		{
			c = VK_DECIMAL;
		}
		else{
			c = btnInfo->btnMsg[0];
		}
		INPUT input[2];
		::ZeroMemory(input, sizeof(input));     
		input[0].type = input[1].type = INPUT_KEYBOARD;
		input[0].ki.wVk  = input[1].ki.wVk = c;  
		input[1].ki.dwFlags = KEYEVENTF_KEYUP;  // THIS IS IMPORTANT
		::SendInput(1, input, sizeof(INPUT));   
		::SendInput(1, input+1, sizeof(INPUT));  
		return TRUE;
}
