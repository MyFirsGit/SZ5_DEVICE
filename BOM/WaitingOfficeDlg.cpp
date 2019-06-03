#include "stdafx.h"
#include "BOM.h"
#include "WaitingOfficeDlg.h"
#include "bomforeservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CWaitingOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CWaitingOfficeDlg, COperationDlg)
    ON_WM_ACTIVATE()
    ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService*   pService  当前的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWaitingOfficeDlg::CWaitingOfficeDlg(CService* pService, CString strServiceName)
	: COperationDlg(CWaitingOfficeDlg::IDD, pService)
{
    m_strServiceName = strServiceName;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CWaitingOfficeDlg::~CWaitingOfficeDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
             0:创建成功    -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int CWaitingOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    return __super::OnCreate(lpCreateStruct, GUIDE_AREA);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWaitingOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CBOMForeService* pService = (CBOMForeService*)GetService();

    try {
		theSERVICE_MGR.SetState(ISSUEING);
        // 发出读卡命令
		if(m_strServiceName == "TRUE"){
			pService->GetRWHelper()->ReadCard(true);
		}
		else{
			pService->GetRWHelper()->ReadCard();
		}

        // 显示等待读卡消息框
		INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pService->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);

        if (IDCANCEL == ret) 
		{
            pService->GetRWHelper()->StopReadCard();    
        }
    }
    catch (CSysException& e) {
		theSERVICE_MGR.SetState(RECEPT);
       theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
		theSERVICE_MGR.SetState(RECEPT);
        theEXCEPTION_MGR.ProcessException(CInnerException(pService->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC  设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CWaitingOfficeDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);

    if ("" != m_strServiceName && m_strServiceName != _T("TRUE")) {
        DrawRoundRect(pDC, SUBMENU_BTN4_RECT, LINE_BORDER, BLACKGRAY, BLACKGRAY);
        DrawText(pDC, SUBMENU_BTN4_POINT, m_strServiceName, _T(""));
    }
}
