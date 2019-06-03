#include "stdafx.h"
#include "RefreshBaseOfficeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//信息显示编辑框坐标组合
static CRect editCrash[] = {
	BASE_INFO_RECT(2,6),
};


IMPLEMENT_DYNAMIC(CRefreshBaseOfficeDlg, COperationDlg)

BEGIN_MESSAGE_MAP(CRefreshBaseOfficeDlg, COperationDlg)
	ON_WM_CREATE()
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
CRefreshBaseOfficeDlg::CRefreshBaseOfficeDlg(CService* pService)
	: COperationDlg(CRefreshBaseOfficeDlg::IDD,pService)
{
	// 车票种类
	m_RefreshInfo[0].nameLeft = add_wsp(CString(TXT_TICKET_KIND) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	// 卡内余额 	
	m_RefreshInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
	m_RefreshInfo[1].nameRight = add_hsp(MONEY_UNIT,MAX_CASH_RIGHT_LEN);
	// 有效期
	m_RefreshInfo[2].nameLeft = add_wsp(CString(TXT_TICKET_VALIDATE_DATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);

	// 基本信息显示区域
	m_baseInfo->detailInfo.row = LABEL_GROUP_EDIT_ROWS_4;           // 行数
	m_baseInfo->detailInfo.labelGroup = m_RefreshInfo;        // 标签文字（2列）
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
CRefreshBaseOfficeDlg::~CRefreshBaseOfficeDlg()
{

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
int CRefreshBaseOfficeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    CRefreshSvc* pRefreshSvc = (CRefreshSvc*)GetService();
    try{
        // 创建基本信息区域、返回、确定按钮
        if(-1 == __super::OnCreate(lpCreateStruct, GUIDE_AREA | BTN_OK | BASE_AREA )){
            return -1;
        }
    }
    catch (CSysException& e) {
        theEXCEPTION_MGR.ProcessException(e);
    }
    catch (...) {
        theEXCEPTION_MGR.ProcessException(CInnerException(pRefreshSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理重置、返回按钮消息

@param      (i)WPARAM wParam  消息信息，表示按钮的ID号
@param      (i)LPARAM lParam  消息信息，表示按钮按下发送的附加消息

@retval     LRESULT  \n
            TRUE:处理成功  FALSE:处理失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CRefreshBaseOfficeDlg::OnOK(WPARAM wParam,LPARAM lParam)
{
	CRefreshSvc* pRefreshSvc = (CRefreshSvc*)GetService();
	return pRefreshSvc->DoRefresh();
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      更新画面

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CRefreshBaseOfficeDlg::UpdateUI()
{
	CRefreshSvc* pSvc = (CRefreshSvc*)GetService();
	try{	
		// 不可激活
		if (pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_REFRESH_NOT) {
			HideCompoment(BTN_OK | BASE_AREA);	
			return;
		}
		CRefreshSvc::DataModel& model = pSvc->GetDataModel();
		m_RefreshInfo[0].nameRight =add_hsp( model.ticketTypeDetail.ticketTypeName_cn,MAX_CASH_RIGHT_LEN);
		if(model.cardCategory == PURSE)
		{
			m_RefreshInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_BALANCE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_RefreshInfo[1].nameRight = add_hsp(ComFormatAmount(model.balance) + MONEY_UNIT,MAX_CASH_RIGHT_LEN);
		}
		else if(model.cardCategory == MULTIRIDE)
		{
			m_RefreshInfo[1].nameLeft = add_wsp(CString(TXT_TICKET_RIDES) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_RefreshInfo[1].nameRight = add_hsp(ComFormatCount(model.balance) + TIMES_UNIT,MAX_CASH_RIGHT_LEN);
		}
		else if(model.cardCategory == PERIOD)
		{
			m_RefreshInfo[1].nameLeft = TXT_UNDEFINE;
			m_RefreshInfo[1].nameRight = TXT_UNDEFINE;
		}
		m_RefreshInfo[2].nameRight = add_wsp(model.orignalValidate.ToString(_T("%4d年%2d月%2d日 %2d时%2d分%2d秒")),MAX_SBC_CASE_LEN);
		if(pSvc->GetCurDialogGroupID()==CRefreshSvc::DIALOG_GROUP_FINISH)
		{
			m_RefreshInfo[3].nameLeft = add_wsp(CString(TXT_NEW_VALIDATE) + CString(TXT_COLON),MAX_SBC_CASE_LEN);
			m_RefreshInfo[3].nameRight = add_wsp(model.newValidate.ToString(_T("%4d年%2d月%2d日 %2d时%2d分%2d秒")),MAX_SBC_CASE_LEN);
			HideCompoment( BTN_OK  );
		}
		m_baseInfoArea->Invalidate();
	}
	catch (CSysException e) {
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		theEXCEPTION_MGR.ProcessException(CInnerException(pSvc->GetServiceID(), CInnerException::OTHER_ERR, _T(__FILE__), __LINE__));
	}
}
