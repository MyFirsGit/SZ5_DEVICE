/**
@brief	�˿�����ܴ���
*/
#include "stdafx.h"
#include "BOM.h"
#include "MainFrmRec.h"
#include "bominfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMainFrmRec
static const TCHAR* const WINDOWTEXT = _T("�˿�");

IMPLEMENT_DYNAMIC(CMainFrmRec, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrmRec, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMainFrmRec::CMainFrmRec()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CMainFrmRec::~CMainFrmRec()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���洴������

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:�����ɹ�    -1:����ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
int CMainFrmRec::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1) {
        return -1;
    }

    // ɾ���˵�
	CMenu* pMenu = GetMenu();

	if (pMenu != NULL) {
		pMenu->DestroyMenu();
		SetMenu(NULL);
	}

    // ȥ�����ڱ������ͱ߿�
  	ModifyStyle(WS_OVERLAPPEDWINDOW | WS_BORDER, NULL); 

    // ȥ���ͻ��������������ϲ���ʾ
    if (theBOM_INFO.IsTopmost()) {
        ModifyStyleEx(WS_EX_CLIENTEDGE, WS_EX_TOOLWINDOW);
    }
    else {
        ModifyStyleEx(WS_EX_CLIENTEDGE, NULL);
    }

    // ȡ�õ�ǰ��ʾ���Ĵ�С
    //int iScrCx = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
#ifndef _DEBUG
	//::MoveWindow(this->m_hWnd, 0, 0, CXSCREENREC, CYSCREENREC, TRUE);
	::MoveWindow(this->m_hWnd, CXSCREEN, 0, CXSCREENREC, CYSCREENREC, TRUE);
#else
	::MoveWindow(this->m_hWnd, CXSCREEN, 0, CXSCREENREC, CYSCREENREC, TRUE);
	//::MoveWindow(this->m_hWnd,0,0,CXSCREENREC*2, CYSCREENREC, TRUE);
#endif
    

    ::SetWindowText(this->m_hWnd, WINDOWTEXT);

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW & ~WS_BORDER, 
		    CRect(0, 0, CXSCREENREC, CYSCREENREC), this, AFX_IDW_PANE_FIRST, NULL))
    {
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	return 0;
}
