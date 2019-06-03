#include "StdAfx.h"
#include "OperationDlg.h"
#include "ForeService.h"
#include "bitmapmanager.h"
#include "InnerException.h"
#include "ExceptionMgr.h"
#include "AppSession.h"
#include "GuideDlg.h"
#include "MessageIDDefinition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(COperationDlg, CBaseDlg)
BEGIN_MESSAGE_MAP(COperationDlg, CBaseDlg)
	ON_WM_ACTIVATE()
    ON_MESSAGE(WM_EDIT_SETFOCUSE, OnEditSetFocus)
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
COperationDlg::COperationDlg(UINT ID, CService* pService, CRect rect)
	: CBaseDlg(ID, theAPP_SESSION.GetChildView(), rect, GRAY, pService)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)UINT ID              对话框ID
@param      (i)CRect position       位置大小
@param      (i)COLORREF bkColor     背景色
@param      (i)CService* pService   对话框使用的Service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg::COperationDlg(UINT ID, CRect rect, COLORREF bkColor, CService* pService)
	: CBaseDlg(ID, theAPP_SESSION.GetChildView(), rect, bkColor, pService)
{
    Initialize();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      初期化

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::Initialize()
{
	m_baseInfoArea = NULL;

	m_baseInfo = new GSTATIC_INFO;
	m_baseInfo->point = LEFTUP_AREA_POINT;						// 基本信息区域左上角坐标
	m_baseInfo->titleInfo.titleName = _T("");					// 标题名称（TVM不使用）
	m_baseInfo->titleInfo.color = BLACK;						// 标题颜色（TVM不使用）
	m_baseInfo->detailInfo.color = BLACK;						// 内容颜色
	m_baseInfo->detailInfo.row = 15;							// 内容行数
	m_baseInfo->detailInfo.editGroup.cnt = 0;                   // 编辑框个数
	m_baseInfo->detailInfo.editGroup.isLeftAlign = true;		// 是否左对齐
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
COperationDlg::~COperationDlg(void)
{
	delete m_baseInfoArea;
	m_baseInfoArea = NULL;

	delete m_baseInfo;
	m_baseInfo = NULL;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct 窗口初始化参数
@param      (i)ulong ulCompomentFlag 组件创建标志

@retval     int  0:创建成功  -1:创建失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int COperationDlg::OnCreate(LPCREATESTRUCT lpCreateStruct, ULONG ulCompomentFlag)
{
    try {
		theGuideDlg.ShowWindow(ulCompomentFlag & GUIDE_AREA ? SW_SHOW : SW_HIDE);
	    if (__super::OnCreate(lpCreateStruct) == -1) {
		    return -1;
	    }
		if(ulCompomentFlag & BASE_AREA) {
			m_baseInfoArea = new CGStatic(this, m_baseInfo);
		}

        // 编辑框列表（基本数据区域中有编辑框）
        if (m_baseInfoArea != NULL && m_baseInfo->detailInfo.editGroup.cnt > 0) {  
            for (int i = 0; i < m_baseInfo->detailInfo.editGroup.cnt; i++) {
                m_editArray.push_back(m_baseInfoArea->GetEdit(i));
			}
        }
		if (m_editArray.size() > 0) {
			SetEditFocus(0);	// 设置输入焦点到编辑框
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
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState,pWndOther,bMinimized);
	
	SetEditFocus(m_curEditIndex);	

	if(NULL!=m_baseInfoArea)
	{
		if(IsWindow(m_baseInfoArea->m_hWnd))
		{
			m_baseInfoArea->SetStyle();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      获取基本数据

@param      无

@retval     PGSTATIC_INFO

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
PGSTATIC_INFO COperationDlg::GetBaseInfo()
{
	return m_baseInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取基本数据区域

@param      无

@retval     CGStatic*

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGStatic* COperationDlg::GetBaseArea()
{
	return m_baseInfoArea;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制

@param      (i)CDC* pDC 设备上下文

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnDraw(CDC* pDC)
{
    SetEditFocus(m_curEditIndex);	// 设置输入焦点到编辑框

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      编辑框内容改变的消息相应函数

@param      (i)WPARAM wParam  消息信息，焦点编辑框
@param      (i)LPARAM lParam  暂无意义

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT COperationDlg::OnEditSetFocus(WPARAM wParam, LPARAM lParam)
{
    CGEdit* pEdit = (CGEdit*)wParam;
    for (UINT i = 0; i < m_editArray.size(); i++) {
        if (pEdit == m_editArray[i]) {
			m_curEditIndex = i;
            break;
        }
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::OnServiceBusy(bool busy)
{
	// 后维护忙碌时停止自动登出计时器(当不忙碌时，自动登出时间大于0才可重新启动计时器)
	DWORD dwOutOfTime = theAFC_TVM_OPERATION.GetAutoLogoffTime();
	if (dwOutOfTime > 0)
	{
		busy ?theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF):theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF,dwOutOfTime);
	}
	else
	{
		if (busy)
		{
			theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StopTimer(CMainSvc::TIMER_AUTOMATIC_LOGOFF);
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示组件

@param      (i)ulong ulCompomentFlag 组件标志

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_SHOW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      隐藏组件

@param      (i)ulong ulCompomentFlag 组件标志

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::HideCompoment(ULONG ulCompomentFlag)
{
    ShowHideCompoment(ulCompomentFlag, SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示/隐藏组件

@param      (i)ulong ulCompomentFlag 组件标志
@param      (i)int nCmdShow 命令(SW_SHOW/SW_HIDE)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowHideCompoment(ULONG ulCompomentFlag, int nCmdShow)
{
    // 基本数据区域
    if (ulCompomentFlag & BASE_AREA)
	{
        ShowHideCompoment(m_baseInfoArea, nCmdShow);
    }

    // 编辑框
    if (ulCompomentFlag & EDIT_BOX)
	{
        for (UINT i = 0; i < m_editArray.size(); i++)
		{
            ShowHideCompoment(m_editArray[i], nCmdShow);
        }
        SetEditFocus(0);
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      显示/隐藏组件

@param      (i)CWnd *pCompoment 组件对象
@param      (i)int nCmdShow 命令(SW_SHOW/SW_HIDE)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::ShowHideCompoment(CWnd* pCompoment, int nCmdShow)
{
    if (pCompoment == NULL) 
	{
		return;
	}

	CRect rect;
    pCompoment->ShowWindow(nCmdShow);
    GetCompomentRect(pCompoment, &rect);
    InvalidateRect(&rect, TRUE);
    UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取组件所在的区域

@param      (i)CWnd *pCompoment 组件对象
@param      (o)CRect* lpRect 区域

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::GetCompomentRect(CWnd *pCompoment, CRect* lpRect)
{
    pCompoment->GetWindowRect(lpRect);
    ScreenToClient(lpRect);									// 转换成相对于当前OperationDlg的位置
    lpRect->InflateRect(BORDER_MAX_SIZE, BORDER_MAX_SIZE);	// 加上边框的大小
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置编辑框焦点

@param      (i)int index 编辑框编号

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::SetEditFocus(int index)
{
    if (index >= 0 && index < m_editArray.size())
	{
        m_editArray[index]->SetFocus();
    }
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取获得焦点的编辑框编号

@param      无

@retval     int 获得焦点的编辑框编号

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
int COperationDlg::GetEditFocusID()
{
	return m_curEditIndex;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置编辑框文本

@param      (i)int index 编辑框编号
@param      (i)CString text 编辑框内容

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void COperationDlg::SetEditText(int index, CString text)
{
	ASSERT(index >= 0 && index < m_editArray.size());
    m_editArray[index]->SetWindowText(text);
    SetEditFocus(m_curEditIndex);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取编辑框文本

@param      (i)int index 编辑框编号

@retval     CString 编辑框文本

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CString COperationDlg::GetEditText(int index)
{
	ASSERT(index >= 0 && index < m_editArray.size());
    CString text;
    m_editArray[index]->GetWindowText(text);
    return text;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取编辑框对象

@param      (i)int index 编辑框编号

@retval     CGEdit* 编辑框对象, index无效时返回NULL

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CGEdit* COperationDlg::GetEdit(int index)
{
	ASSERT(index >= 0 && index < m_editArray.size());
	return m_editArray[index];
}
