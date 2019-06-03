#include "stdafx.h"
#include "ExchangeSvc.h"
#include "ExchangeBackupDlg.h"
#include "ExceptionMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//现金信息显示编辑框坐标组合
static CRect editCrash[] = {
	BASE_INFO_RECT(2,6),
};

IMPLEMENT_DYNAMIC(CExchangeBackupDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CExchangeBackupDlg, COperationDlg)
    ON_WM_CREATE()
    ON_WM_ACTIVATE()
	ON_MESSAGE(WM_XGROUP,XGroupClick)
    ON_MESSAGE(WM_XBUTTON,XButtonClick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBackupDlg::CExchangeBackupDlg(CService* pService)
: COperationDlg(CExchangeBackupDlg::IDD, pService)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
// 充值信息显示区域
	m_baseInfo->titleInfo.titleName = TITLE_EXCHANGE_INFO;                    // 标题
	m_baseInfo->detailInfo.row = pExchangeSvc->GetExchangeInfoSize();         // 行数
	m_baseInfo->detailInfo.labelGroup = pExchangeSvc->GetExchangeInfo();      // 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 0;                               // 编辑框数目

	// 现金信息显示区域
	//m_cashInfo->titleInfo.titleName = TITLE_CASH_INFO;                      // 标题
	//m_cashInfo->detailInfo.row = pExchangeSvc->GetCrashInfoSize();             // 行数
	//m_cashInfo->detailInfo.labelGroup = pExchangeSvc->GetCrashInfo();          // 标签文字（2列）
	//m_cashInfo->detailInfo.editGroup.cnt = 1;                                // 编辑框数目
	//m_cashInfo->detailInfo.editGroup.editRect = editCrash;                   // 编辑框坐标 

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBackupDlg::~CExchangeBackupDlg()
{
   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面绘制函数

@param      (i) CDC* pDC 设备上下文

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeBackupDlg::OnDraw(CDC* pDC)
{
    __super::OnDraw(pDC);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面创建函数

@param      (i)LPCREATESTRUCT lpCreateStruct  

@retval     int    \n
0:创建成功    -1:创建失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
int CExchangeBackupDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    try{
        // 创建基本信息区域、返回键、确认键
        if(-1 == __super::OnCreate(lpCreateStruct, BASE_AREA | CASH_AREA | BTN_BACK | BTN_OK)){
            return -1;
        }
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        //theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CExchangeBackupDlg::XButtonClick(WPARAM wParam,LPARAM lParam)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    CString sMsg = (LPCTSTR)lParam;
    try {
       
    }
    catch (CSysException e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
//        theEXCEPTION_MGR.ProcessException(CInnerException(pExchangeSvc->GetServiceID(), CInnerException::OTHER_ERR, __FILE__, __LINE__));
    }

    // 画面切换
    return __super::XButtonClick(wParam,lParam);   
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      数字键盘回车键按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CExchangeBackupDlg::XGroupClick(WPARAM wParam,LPARAM lParam)
{
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      画面激活后的处理

@param      (i)UINT nState      不使用
@param      (i)CWnd* pWndOther  不使用
@param      (i)BOOL bMinimized  不使用

@retval     无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CExchangeBackupDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      检测cash编辑框的输入是否正确

@param      none

@retval     bool		\n
TRUE:正确  FALSE:不正确

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CExchangeBackupDlg::CheckInput()
{
	bool ret = false;
	
	return ret;
}

