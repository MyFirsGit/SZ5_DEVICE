#include "StdAfx.h"
#include "ReceptionDlg.h"
#include "appsession.h"
#include "InnerException.h"
#include "exceptionmgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



static const UINT const REC_EN_FONT_SIZE3 = 14;

IMPLEMENT_DYNAMIC(CReceptionDlg, CBaseDlg)

BEGIN_MESSAGE_MAP(CReceptionDlg, CBaseDlg)
    ON_WM_ACTIVATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
 @brief      构造函数

 @param      (i)UINT ID             对话框ID
 @param      (i)CService* pService  对话框使用的Service

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg::CReceptionDlg(UINT ID, CService* pService)
	: CBaseDlg(ID, NULL, REC_BODY_RECT, RECEPTION_BK_COLOR, pService, false)
{
	m_BusinessArea = NULL;
    m_BusinessInfo = new MSG_INFO;

    m_BusinessInfo->titleName.nameChinese = theAPP_SESSION.GetServiceNameCH((SERVICE_ID)pService->GetServiceID());
	m_BusinessInfo->titleName.nameEnglish = theAPP_SESSION.GetServiceNameEN((SERVICE_ID)pService->GetServiceID());

}

//////////////////////////////////////////////////////////////////////////
/**
 @brief      析构函数

 @param      无

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
CReceptionDlg::~CReceptionDlg(void)
{
    delete m_BusinessInfo;
    m_BusinessInfo = NULL;

    delete m_BusinessArea;
    m_BusinessArea = NULL;
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
int CReceptionDlg::OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulAreaFlag)
{
    try {
	    if (__super::OnCreate(lpCreateStruct) == -1) {
		    return -1;
	    }

         //基本数据区域
        if (ulAreaFlag & BUSINESS_AREA) {
            m_BusinessArea = new CReceptionStaticInfo(this, m_BusinessInfo);
        }
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
 @brief      画面绘制函数

 @param      (i) CDC* pDC  设备上下文

 @retval     无

 @exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::OnDraw(CDC* pDC)
{

	__super::OnDraw(pDC);
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
void CReceptionDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	if(theSERVICE_MGR.GetState()!=READY)
	{
		// 显示乘客画面标题与向导
		theAPP_SESSION.GetChildViewRec()->m_pReceptionHeadDlg->ShowWindow(SW_SHOW);
		theAPP_SESSION.GetChildViewRec()->m_pReceptionGuideDlg->ShowWindow(SW_SHOW);

		// 隐藏“欢迎光临”画面
		theAPP_SESSION.GetChildViewRec()->m_pOutOfReceptionDlg->ShowWindow(SW_HIDE);
		__super::OnActivate(nState,pWndOther,bMinimized);
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
void CReceptionDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示组件

@param      (i)CWnd *pCompoment                组件对象

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏组件

@param      (i)ulong ulCompomentFlag           组件标志

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏组件

@param      (i)CWnd *pCompoment                组件对象

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::HideCompoment(CWnd *pCompoment)
{
    ShowHideCompoment(pCompoment, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示/隐藏组件

@param      (i)ulong ulCompomentFlag           组件标志
@param      (i)int nCmdShow                    命令(SW_SHOW/SW_HIDE)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
    if (ulCompomentFlag & BUSINESS_AREA) {
        ShowHideCompoment(m_BusinessArea, nCmdShow);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示/隐藏组件

@param      (i)CWnd *pCompoment                组件对象
@param      (i)int nCmdShow                    命令(SW_SHOW/SW_HIDE)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::ShowHideCompoment(CWnd *pCompoment, int nCmdShow)
{
    CRect rect;

    if (pCompoment != NULL) {
        pCompoment->ShowWindow(nCmdShow);

        GetCompomentRect(pCompoment, &rect);

        InvalidateRect(&rect, TRUE);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得组件所在的区域

@param      (i)CWnd *pCompoment                组件对象
@param      (o)CRect* lpRect                   区域

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CReceptionDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);

    // 转换成相对于当前OperationDlg的位置
    ScreenToClient(lpRect);

    // 加上边框的大小
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);
}
