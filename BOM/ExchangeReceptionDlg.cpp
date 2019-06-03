#include "stdafx.h"
#include "ExchangeReceptionDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CExchangeReceptionDlg,CReceptionDlg)

BEGIN_MESSAGE_MAP(CExchangeReceptionDlg, CReceptionDlg)
	ON_WM_ACTIVATE()
    ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExchangeReceptionDlg::CExchangeReceptionDlg(CService* pService)
	: CReceptionDlg(CExchangeReceptionDlg::IDD, pService)
{
   m_BusinessInfo->detailList.resize(LABEL_GROUP_EDIT_ROWS_5);     
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CExchangeReceptionDlg::~CExchangeReceptionDlg()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeReceptionDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
 
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
int CExchangeReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    try{
        if (-1 == __super::OnCreate(lpCreateStruct ,BUSINESS_AREA)){
            return -1;
        }
		
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     void

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
	try{	
		// 不能
		if (pExchangeSvc->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_EXCHANGE_NOT) {
			//隐藏所有组件	
			HideCompoment(BUSINESS_AREA);
		}
		// 结束
		else if(pExchangeSvc->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_FINISH)
		{
			m_BusinessInfo->detailList = pExchangeSvc->GetReceptionInfo();
			m_BusinessArea->Invalidate();
		}
		else
		{
			m_BusinessInfo->detailList = pExchangeSvc->GetReceptionInfo();
			m_BusinessArea->Invalidate();
		}
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}


}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示组件

@param      (i)ulong ulCompomentFlag           组件标志

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeReceptionDlg::ShowCompoment(ULONG ulCompomentFlag)
{
	CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
	m_BusinessInfo->detailList = pExchangeSvc->GetReceptionInfo();
	__super::ShowCompoment(ulCompomentFlag);
}
