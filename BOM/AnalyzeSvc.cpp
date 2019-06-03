#include "stdafx.h"
#include "AnalyzeSvc.h"
#include "analyzebaseofficedlg.h"
#include "WelcomeReceptionDlg.h"
#include "analyzeconfirmreceptiondlg.h"
#include "waitingofficedlg.h"
#include "ectcardinfo.h"
#include "guideinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_ANALYZE_WAIT_TO_BASE = _T("WM_ANALYZE_WAIT_TO_BASE");
////static const TCHAR* const WM_ANALYZE_BASE_TO_HISTORY = "WM_ANALYZE_BASE_TO_HISTORY";
////static const TCHAR* const WM_ANALYZE_HISTORY_TO_BASE = "WM_ANALYZE_HISTORY_TO_BASE";
////static const TCHAR* const WM_ANALYZE_BASE_TO_PERSONAL = "WM_ANALYZE_BASE_TO_PERSONAL";
////static const TCHAR* const WM_ANALYZE_PERSONAL_TO_BASE = "WM_ANALYZE_PERSONAL_TO_BASE";
static const TCHAR* const WM_ANALYZE_BASE_TO_ERROR = _T("WM_ANALYZE_BASE_TO_ERROR");
static const TCHAR* const WM_ANALYZE_WAIT_TO_ERROR = _T("WM_ANALYZE_WAIT_TO_ERROR");

BEGIN_MESSAGE_MAP(CAnalyzeSvc, CBOMForeService)

END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService* pService

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::CAnalyzeSvc()
    :CBOMForeService(ANALYZE_SVC)
{

    // 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_WAITING_DLG, new CWaitingOfficeDlg(this));
    m_pDialogFlow->AddDialog(IDD_00802_ANALYZE_BASE_DLG, new CAnalyzeBaseOfficeDlg(this));

    // 设置乘客画面
    m_pDialogFlow->AddDialog(IDD_10002_WELCOME_DLG, new CWelcomeReceptionDlg(NULL));
    m_pDialogFlow->AddDialog(IDD_10802_ANALYZE_CONFIRM_DLG, new CAnalyzeConfirmReceptionDlg(this));

    // 设置画面分组
    // 等待画面组
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PUT_CARD));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_WAITING_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_10002_WELCOME_DLG);

    // 基本信息画面组
    m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_BASE,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_SELET_OPERATE),
        theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ANALYZE_CONFIRM_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ANALYZE_CONFIRM_PASSENGER));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_00802_ANALYZE_BASE_DLG);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_BASE, IDD_10802_ANALYZE_CONFIRM_DLG);

	//// 历史信息画面组
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_HISTORY,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PRINT_HISTORY),
	//	theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ANALYZE_CONFIRM_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ANALYZE_CONFIRM_PASSENGER));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_HISTORY, IDD_00802_ANALYZE_BASE_DLG);
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_HISTORY, IDD_10802_ANALYZE_CONFIRM_DLG);

	//// 个人信息画面组
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_PERSONAL,theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_PRINT_HISTORY),
	//	theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ANALYZE_CONFIRM_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ANALYZE_CONFIRM_PASSENGER));
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_PERSONAL, IDD_00802_ANALYZE_BASE_DLG);
	//m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_PERSONAL, IDD_10802_ANALYZE_CONFIRM_DLG);

	// 错误画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR,/*theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_SC_NOTFOUND)*/_T(""),
		theGUIDE_INFO.GetPassengerMessageCn(GUIDE_ANALYZE_FAILED_PASSENGER),theGUIDE_INFO.GetPassengerMessageEn(GUIDE_ANALYZE_FAILED_PASSENGER));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_00802_ANALYZE_BASE_DLG);
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_10802_ANALYZE_CONFIRM_DLG);

    // 设置画面流转
    m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ANALYZE_WAIT_TO_BASE, DIALOG_GROUP_BASE);
	//////m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_ANALYZE_BASE_TO_HISTORY, DIALOG_GROUP_HISTORY);
	//////m_pDialogFlow->AddFlow(DIALOG_GROUP_HISTORY, WM_ANALYZE_HISTORY_TO_BASE, DIALOG_GROUP_BASE);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_BASE, WM_ANALYZE_BASE_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_ANALYZE_WAIT_TO_ERROR, DIALOG_GROUP_ERROR);

    // 设置初始画面组
    m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::~CAnalyzeSvc()
{
	delete m_DataModel.cpuCard;
	m_DataModel.cpuCard = NULL;

	delete m_DataModel.ulCard;
	m_DataModel.ulCard = NULL;
}
 

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CAnalyzeSvc::OnReadCard(UINT resultCode, CRWReadCard * pRWReadCard)
{
	try	{
		CMessageBox::End();
		theSERVICE_MGR.SetState(RECEPT);
		pRWReadCard->GetRWResponse(&(m_DataModel.rwReadResponse));
		// 黑名单票卡
		if(resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_ECT_CARD_BLACKLIST){
			RW_BUSSINESS_RESPONSE response;
			long errCode = BlockCard(resultCode,response); 
			CString strGuideInfo = errCode != RW_RSP_OK ?  m_pRWHelper->GetTpuErrorInfo(response.DetailErrorCode) : theGUIDE_INFO.GetOperationMessage(GUIDE_ANALYZE_BLACKLIST_CARD_BLOCKED);
			OnError(strGuideInfo);
			return;
		} 
		if(RW_RSP_OK!=resultCode){
			CString strGuideInfo = m_DataModel.rwReadResponse.DetailErrorCode == RW_ERROR_CARD_NO_CARD ? theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL) : m_pRWHelper->GetTpuErrorInfo(m_DataModel.rwReadResponse.DetailErrorCode);
			OnError(strGuideInfo);
			return;
		}
		delete m_DataModel.cpuCard;
		m_DataModel.cpuCard = NULL;
		delete m_DataModel.ulCard;
		m_DataModel.ulCard = NULL;
		if(ULTRALIGHT == m_DataModel.rwReadResponse.CardPHType){
			m_DataModel.ulCard = new CULCard;
			m_DataModel.ulCard->GetCardInfo(m_DataModel.rwReadResponse.CardInfo);
			m_DataModel.ulCard->GetHistoryInfo(m_DataModel.rwReadResponse.LogCount,m_DataModel.rwReadResponse.LogInfo);
		}
		else if(CPU_TICKET == m_DataModel.rwReadResponse.CardPHType){
			m_DataModel.cpuCard = new CCPUCard;
			m_DataModel.cpuCard->GetCardInfo(m_DataModel.rwReadResponse.CardInfoLen,m_DataModel.rwReadResponse.CardInfo);
			m_DataModel.cpuCard->GetHistoryInfo(m_DataModel.rwReadResponse.LogCount,m_DataModel.rwReadResponse.LogInfo);
		}
		else{
			OnError(_T("（红）错误的票卡类型"));	
			return;
		}
		m_pDialogFlow->ShowNextDialogGroup(WM_ANALYZE_WAIT_TO_BASE);
	}
	catch (CSysException& e){
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		CInnerException e(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

void CAnalyzeSvc::OnError(CString strMessage)
{
	theSERVICE_MGR.SetState(FINISH);
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_WAIT)
	{
		DoDialogFlow(WM_ANALYZE_WAIT_TO_ERROR);
	}
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_BASE)
	{
		DoDialogFlow(WM_ANALYZE_BASE_TO_ERROR);
	}
	theAPP_SESSION.ShowOfficeGuide(strMessage);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CAnalyzeSvc::BlockCard(UINT resultCode,RW_BUSSINESS_RESPONSE response)
{
	theSERVICE_MGR.SetState(ISSUEING);
	RW_CMD_BLOCK rw_cmd_block;
	if (resultCode == RW_RSP_ACC_CARD_BLACKLIST){
		rw_cmd_block.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
		rw_cmd_block.TranDateTime = ComGetCurTime();
		resultCode = m_pRWHelper->Block(rw_cmd_block,response);
		// 锁卡成功
		if(RW_RSP_OK == resultCode){
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);	

			// 更新UD数据
			if(response.UDlen != 0){
				m_pRWHelper->SaveRWUDData(response.UDlen,response.UDData);
			}

			// 更新AR数据
			if(response.ARLen != 0){
				m_pRWHelper->SaveRWARData(response.ARLen,response.ARData,response.TranResult.CardType);
			}
		}
	} 
	theSERVICE_MGR.SetState(RECEPT);
	return resultCode;
}



//////////////////////////////////////////////////////////////////////////
/*
@brief      取得服务中的数据模型

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CAnalyzeSvc::DataModel& CAnalyzeSvc::GetDataModel()
{
	return m_DataModel;
}


