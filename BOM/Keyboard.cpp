#include "stdafx.h"
#include "Keyboard.h"
#include "BitmapManager.h"

IMPLEMENT_DYNAMIC(CKeyboard,CBaseDlg)

BEGIN_MESSAGE_MAP(CKeyboard,CBaseDlg)
	ON_WM_CREATE()
	ON_GROUP_BUTTON_CLICK(KEYBOARD_GROUP_ID,OnKeyboardClick)
END_MESSAGE_MAP()

static pair<CString,CString> KeyMsg[] = {
	pair<CString,CString>("A","A"),pair<CString,CString>("B","B"),pair<CString,CString>("C","C"),pair<CString,CString>("D","D"),
	pair<CString,CString>("1","1"),pair<CString,CString>("2","2"),pair<CString,CString>("3","3"),pair<CString,CString>("E","E"),
	pair<CString,CString>("4","4"),pair<CString,CString>("5","5"),pair<CString,CString>("6","6"),pair<CString,CString>("F","F"),
	pair<CString,CString>("7","7"),pair<CString,CString>("8","8"),pair<CString,CString>("9","9"),pair<CString,CString>("X","X"),
	pair<CString,CString>("0","0"),pair<CString,CString>(".","."),pair<CString,CString>("BS","¡û"),pair<CString,CString>("OK","È·¶¨")
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
	groupUI.m_pBackgroundImage = theBitmap_MGR.m_bgGroup;
	UIINFO btnUIInfo;
	btnUIInfo.m_Font.lfHeight = 20;
	btnUIInfo.m_Font.lfWeight = FW_MEDIUM;
	pButtonGroup = new CButtonGroup(groupUI,5,4,KEYBOARD_GROUP_ID,CButtonGroup::NONE,btnUIInfo);
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
		if(btnInfo->btnMsg == "BS")
		{
			c = VK_BACK;
		}
		else if(btnInfo->btnMsg == "OK")
		{
			c = VK_RETURN;
		}
		else if(btnInfo->btnMsg == ".")
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
