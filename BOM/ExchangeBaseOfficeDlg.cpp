#include "stdafx.h"
#include "ExchangeBaseOfficeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//现金信息显示编辑框坐标组合
static CRect editCrash[] = {
	BASE_INFO_RECT(2,6),
};


IMPLEMENT_DYNAMIC(CExchangeBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CExchangeBaseOfficeDlg, COperationDlg)
    ON_WM_CREATE()
    ON_WM_ACTIVATE()
    ON_BTN_OK_CLICK(OnOK)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBaseOfficeDlg::CExchangeBaseOfficeDlg(CService* pService)
	: COperationDlg(CExchangeBaseOfficeDlg::IDD, pService)
{
	CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();

	// 充值信息显示区域
	m_baseInfo->titleInfo.titleName = TITLE_CHARGE_INFO;                    // 标题
	m_baseInfo->detailInfo.row = pExchangeSvc->GetExchangeInfoSize();         // 行数
	m_baseInfo->detailInfo.labelGroup = pExchangeSvc->GetExchangeInfo();      // 标签文字（2列）
	m_baseInfo->detailInfo.editGroup.cnt = 0;                               // 编辑框数目 
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CExchangeBaseOfficeDlg::~CExchangeBaseOfficeDlg()
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
void CExchangeBaseOfficeDlg::OnDraw(CDC* pDC)
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
int CExchangeBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CExchangeSvc* pExchangeSvc = (CExchangeSvc*)GetService();
    try{
        // 创建基本信息区域、确认键
        if(-1 == __super::OnCreate(lpCreateStruct,GUIDE_AREA| BTN_OK |  BASE_AREA )){
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
@brief      按钮按下后的处理

@param      (i)WPARAM wParam  消息信息
@param      (i)LPARAM lParam  消息信息

@retval     LRESULT  \n
TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CExchangeBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CExchangeSvc* pExchange = (CExchangeSvc*)GetService();
	pExchange->DoOldCardExchange();   
    return __super::XButtonClick(wParam,lParam);
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
void CExchangeBaseOfficeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CExchangeSvc* pExchange = (CExchangeSvc*)GetService();
	try{	
		// 换卡错误画面
		if (pExchange->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_EXCHANGE_NOT) {
			//隐藏所有组件	
			m_vec_Right_2_BTN_INFO.clear();
			HideCompoment( BTN_OK|BASE_AREA|KEYBOARD_AREA );		
		}
		// 结束
		else if(pExchange->GetCurDialogGroupID()==CExchangeSvc::DIALOG_GROUP_FINISH)
		{
			m_vec_Right_2_BTN_INFO.clear();
			HideCompoment( BTN_OK |KEYBOARD_AREA );
			m_baseInfoArea->Invalidate();
		}	
		theSERVICE_MGR.SetState(FINISH);
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pExchange->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
	
}


