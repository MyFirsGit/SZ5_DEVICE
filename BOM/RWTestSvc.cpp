#include "stdafx.h"
#include "RWTestSvc.h"
#include "CardReadWriteMachineTestMaintainDlg.h"
#include "GuideInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CRWTestSvc, CBOMForeService)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::CRWTestSvc()
:CBOMForeService(OUTRW_TEST_SVC)
{
	// 设置操作员画面
	m_pDialogFlow->AddDialog(IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG,new CCardReadWriteMachineTestMaintainDlg(this));

	//设置TH测试画面组
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CARD_READWRITE_MACHINE,theGUIDE_INFO.GetMaintenanceMessage(GUIDE_RW_TEST_CHOOSE_ITEM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CARD_READWRITE_MACHINE,IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);

	//设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_CARD_READWRITE_MACHINE);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CRWTestSvc::~CRWTestSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      卡片读取回调函数

@param      (i)UINT errCode				错误代码
@param      (i)CRWReadCard* rwReadCard  读取卡片的类的指针

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CRWTestSvc::OnReadCard(UINT errCode,CRWReadCard* rwReadCard)
{
	CMessageBox::End();
	RW_READ_CARD_RESPONSE cardRspInfo ;
	rwReadCard->GetRWResponse(&cardRspInfo);
	// 读卡失败
	if(errCode != RW_RSP_OK){
		if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(cardRspInfo.DetailErrorCode));
		}
	}
	else{
		CCardReadWriteMachineTestMaintainDlg* pReadDlg = (CCardReadWriteMachineTestMaintainDlg*)m_pDialogFlow->GetDialog(IDD_20521_CARD_READ_WRITE_MACHINE_TEST_DLG);
		// UL卡
		if(cardRspInfo.CardPHType == ULTRALIGHT){
			CULCard ulCard;
			// 获得读卡信息 			    
			ulCard.GetCardInfo(cardRspInfo.CardInfo);
			// 获得卡号
			CString strTicketNum;
			strTicketNum.Format(_T("%09u"),ulCard.applicationData.CardLogicalNumber);
			// 显示卡号
			pReadDlg->m_CardReadWriteInfo[0].nameRight = strTicketNum;
			// 显示车票种类
			pReadDlg->m_CardReadWriteInfo[1].nameRight = _T("UL卡");
			// 
			pReadDlg->Invalidate();
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_READ_SUCCESS));
		}
		// CPU卡
		else if(cardRspInfo.CardPHType == CPU_TICKET){
			CCPUCard cpuCard;
			// 获得读卡信息 			    
			cpuCard.GetCardInfo(cardRspInfo.CardInfoLen,cardRspInfo.CardInfo);
			// 获得卡号
			CString strTicketNum;
			strTicketNum.Format(_T("%09u"),cpuCard.applicationData.CardLogicalNumber);
			// 显示卡号
			pReadDlg->m_CardReadWriteInfo[0].nameRight = strTicketNum;
			// 显示车票种类
			pReadDlg->m_CardReadWriteInfo[1].nameRight = _T("CPU卡");
			// 
			pReadDlg->Invalidate();
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_READ_SUCCESS));
		}
		else{
			theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetMaintenanceMessage(GUIDE_IC_UNKNOWN_CARD));
		}
	}
}