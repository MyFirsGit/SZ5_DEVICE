#include "stdafx.h"
#include "SaleStaticSvc.h"
#include "SaleStaticDetailDlg.h"
#include "SaleStaticOverViewDlg.h"
#include "SCAuditManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR* const WM_SALESTATIC_DETAIL_TO_OVERVIEW= _T("WM_SALESTATIC_DETAIL_TO_OVERVIEW");
static const TCHAR* const WM_SALESTAIC_OVERVIEW_TO_DETAIL = _T("WM_SALESTAIC_OVERVIEW_TO_DETAIL");

BEGIN_MESSAGE_MAP(CSaleStaticSvc,CTVMForeService)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F5,OnKeyboardF5)
	ON_SERVICE_MESSAGE(WM_KEYBOARD_F6,OnKeyboardF6)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::CSaleStaticSvc()
	:CTVMForeService(SALE_STATIC_SVC)
{
    // 设置操作员画面
   m_pDialogFlow->AddDialog(IDD_10901_SALESTATIC_OVERVIEW, new CSaleStaticOverViewDlg(this));
	m_pDialogFlow->AddDialog(IDD_10902_SALESTATIC_DETAIL, new CSaleStaticDetailDlg(this));

    // 设置乘客画面

    // 设置画面分组
    //m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OVERVIEW, _opl(GUIDE_ENTER_F6_ESC2));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OVERVIEW);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OVERVIEW, CSaleStaticOverViewDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DETAIL);
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DETAIL, _opl(GUIDE_ENTER_F5_ESC2));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DETAIL, CSaleStaticDetailDlg::IDD);

	//设置画面扭转
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OVERVIEW, WM_SALESTAIC_OVERVIEW_TO_DETAIL, DIALOG_GROUP_DETAIL);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_DETAIL, WM_SALESTATIC_DETAIL_TO_OVERVIEW, DIALOG_GROUP_OVERVIEW);

    // 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OVERVIEW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      处理 [F3/Ctrl] 键按下消息（返回）

@param      (i)WPARAM wParam  未使用
@param      (i)LPARAM lParam  未使用

@retval     LRESULT  TRUE:处理成功  FALSE:处理失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSaleStaticSvc::OnKeyboardF3(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	__super::OnKeyboardF3(0,0);
	theAPP_SESSION.ReturnMaintainMenu();
	return TRUE;
}

LRESULT CSaleStaticSvc::OnKeyboardEnter(WPARAM wParam, LPARAM lParam)
{
	__super::OnKeyboardEnter(0,0);
	return TRUE;
}

void CSaleStaticSvc::OnStart()
{
	//查詢交易數據
	std::vector<TransactionRecord> vec_transInfo;

	m_DataModel = _sale_statci_data_model_();
	PurchaseData curDayPurchase=theSCAudit_MGR.GetCurrentDayPurchase();
	CString szbegintime = curDayPurchase.auditTimeFrom.ToString(_T("%04d%02d%02d%02d%02d%02d"));
	CString szendtime   = ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d"));
	long lselect = -1;
	try{
		lselect =DATA_HELPER->DbSelectTransactionInfo(szbegintime,szendtime, vec_transInfo);
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}	
	if(lselect==0)
	{
		m_DataModel.strDateFrom.Format(_T("%s-%s-%s %s:%s"),
			szbegintime.Mid(0,4),
			szbegintime.Mid(4,2),
			szbegintime.Mid(6,2),
			szbegintime.Mid(8,2),
			szbegintime.Mid(10,2));
		m_DataModel.strDateTo.Format(_T("%s-%s-%s %s:%s"),
			szendtime.Mid(0,4),
			szendtime.Mid(4,2),
			szendtime.Mid(6,2),
			szendtime.Mid(8,2),
			szendtime.Mid(10,2));
		InitSaleStaticOverView(vec_transInfo);
	}
	else{
		// Message to operator err info.

	}

	// 设置初始画面组
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OVERVIEW);

	m_pDialogFlow->ShowFirstDialogGroup();

	theAPP_SESSION.SetMenuLevel(SALE_STATIC_SVC);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      初始化销售统计一览主界面

@param      vector<TransactionRecord>& vec_transInfo(数据库查询的数据)

@param      Model& dataModel（要初始化的数据体）

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSaleStaticSvc::InitSaleStaticOverView(vector<TransactionRecord>& vec_transInfo){
	try{
		auto iterTransRecord = vec_transInfo.begin();


		// 循环判断当前交易，去统计
		while(iterTransRecord != vec_transInfo.end()){

			// 交易正常
			if(iterTransRecord->iTransactionDetail.iState != 2){	// 交易不为取消交易时才会统计
				
				// 售票
				if(iterTransRecord->iTransactionDetail.iUdType == 2){
					if(iterTransRecord->iTransactionDetail.iState == 1 || iterTransRecord->iTransactionDetail.iState == 3){ // 交易故障结束|| 交易取消故障结束
						/*					m_DataModel.dwSingleTicketTrsCount++;
						m_DataModel.dwSingleTicketAmount += iterTransRecord->iTransactionDetail.iUnReleaseedMediaQuantity * iterTransRecord->iTransactionDetail.iUnitAmount;
						m_DataModel.dwSingleTicketCount  += iterTransRecord->iTransactionDetail.iUnReleaseedMediaQuantity;*/
					}
					else{
						m_DataModel.dwSingleTicketTrsCount++;
						m_DataModel.dwSingleTicketAmount += iterTransRecord->iTransactionDetail.iTotalAmount;
						m_DataModel.dwSingleTicketCount  += iterTransRecord->iTransactionDetail.iQuantity;
					}
				}

				// 充值
				if(iterTransRecord->iTransactionDetail.iUdType == 4){
					if(iterTransRecord->iTransactionDetail.iState == 1 || iterTransRecord->iTransactionDetail.iState == 3){ // 交易故障结束|| 交易取消故障结束
						//m_DataModel.dwRechargeAmount += iterTransRecord->iTransactionDetail.iTotalAmount;
						//m_DataModel.dwRechargeTrsCount++;
					}
					else{
						m_DataModel.dwRechargeAmount += iterTransRecord->iTransactionDetail.iTotalAmount;
						m_DataModel.dwRechargeTrsCount++;
					}
				}
			}

			// 故障交易
			auto iterException = iterTransRecord->vExceptionDetail.begin();
			while(iterException != iterTransRecord->vExceptionDetail.end()){
				m_DataModel.dwFailedAmount		+= iterException->IExceptionAmount;
				m_DataModel.dwFailedTotalAmount += iterException->IExceptionAmount;
				m_DataModel.dwFailedCount++;
				iterException++;
			}

			// 硬币找零
			auto iterCoinChange = iterTransRecord->vTransCoinChangeDetail.begin();
			while(iterCoinChange != iterTransRecord->vTransCoinChangeDetail.end()){

				if(iterCoinChange->iCoinTypeCode == CoinHalfYuan){
					m_DataModel.dwChangeCHFace05Count += iterCoinChange->iQuantity;
					m_DataModel.dwChangeCHTotalCount  += iterCoinChange->iQuantity;
					m_DataModel.dwChangeCHTotalAmount += iterCoinChange->iQuantity * 100;
				}
				else if(iterCoinChange->iCoinTypeCode == Coin1Yuan){
					m_DataModel.dwChangeCHFace1Count  += iterCoinChange->iQuantity;
					m_DataModel.dwChangeCHTotalCount  += iterCoinChange->iQuantity;
					m_DataModel.dwChangeCHTotalAmount += iterCoinChange->iQuantity * 100;
				}
				iterCoinChange++;
			}

			// 硬币接收
			auto iterCoinRecv = iterTransRecord->vTransCoinAcceptDetail.begin();
			while(iterCoinRecv != iterTransRecord->vTransCoinAcceptDetail.end()){

				if(iterCoinRecv->iCoinTypeCode == CoinHalfYuan){
					m_DataModel.dwRecvCHFace05Count += iterCoinRecv->iQuantity;
					m_DataModel.dwRecvCHTotalCount	+= iterCoinRecv->iQuantity;
					m_DataModel.dwRecvCHTotalAmount += iterCoinRecv->iQuantity * 100;
				}
				else if(iterCoinRecv->iCoinTypeCode == Coin1Yuan){
					m_DataModel.dwRecvCHFace1Count  += iterCoinRecv->iQuantity;
					m_DataModel.dwRecvCHTotalCount	+= iterCoinRecv->iQuantity;
					m_DataModel.dwRecvCHTotalAmount += iterCoinRecv->iQuantity * 100;
				}
				iterCoinRecv++;
			}

			// 纸币接收
			auto iterBanknoteRecv = iterTransRecord->vTransBanknoteAcceptDetail.begin();
			while(iterBanknoteRecv != iterTransRecord->vTransBanknoteAcceptDetail.end()){

				switch(iterBanknoteRecv->iCoinTypeCode){
				case Banknote1Yuan:
					{
						m_DataModel.dwRecvBHFace1Count += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalCount += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalAmount+= iterBanknoteRecv->iQuantity * 100;
					}
					break;
				case Banknote5Yuan:
					{
						m_DataModel.dwRecvBHFace5Count += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalCount += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalAmount+= iterBanknoteRecv->iQuantity * 500;
					}
					break;
				case Banknote10Yuan:
					{
						m_DataModel.dwRecvBHFace10Count += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalCount += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalAmount+= iterBanknoteRecv->iQuantity * 1000;
					}
					break;
				case Banknote20Yuan:
					{
						m_DataModel.dwRecvBHFace20Count += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalCount += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalAmount+= iterBanknoteRecv->iQuantity * 2000;
					}
					break;
				case Banknote50Yuan:
					{
						m_DataModel.dwRecvBHFace50Count += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalCount += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalAmount+= iterBanknoteRecv->iQuantity * 5000;
					}
					break;
				case Banknote100Yuan:
					{
						m_DataModel.dwRecvBHFace100Count += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalCount += iterBanknoteRecv->iQuantity;
						m_DataModel.dwRecvBHTotalAmount+= iterBanknoteRecv->iQuantity * 10000;
					}
					break;
				default:
					break;
				}

				iterBanknoteRecv++;
			}

			// 纸币找零
			auto iterBanknoteChange = iterTransRecord->vTransBanknoteChangeDetail.begin();
			while(iterBanknoteChange != iterTransRecord->vTransBanknoteChangeDetail.end()){

				switch(iterBanknoteChange->iCoinTypeCode){
				case Banknote1Yuan:
					{
						m_DataModel.dwChangeBHFace1Count += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalCount += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalAmount+= iterBanknoteChange->iQuantity * 100;
					}
					break;
				case Banknote5Yuan:
					{
						m_DataModel.dwChangeBHFace5Count += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalCount += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalAmount+= iterBanknoteChange->iQuantity * 500;
					}
					break;
				case Banknote10Yuan:
					{
						m_DataModel.dwChangeBHFace10Count += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalCount += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalAmount+= iterBanknoteChange->iQuantity * 1000;
					}
					break;
				case Banknote20Yuan:
					{
						m_DataModel.dwChangeBHFace20Count += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalCount += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalAmount+= iterBanknoteChange->iQuantity * 2000;
					}
					break;
				case Banknote50Yuan:
					{
						m_DataModel.dwChangeBHFace50Count += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalCount += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalAmount+= iterBanknoteChange->iQuantity * 5000;
					}
					break;
				case Banknote100Yuan:
					{
						m_DataModel.dwChangeBHFace100Count += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalCount += iterBanknoteChange->iQuantity;
						m_DataModel.dwChangeBHTotalAmount+= iterBanknoteChange->iQuantity * 10000;
					}
					break;
				default:
					break;
				}

				iterBanknoteChange++;
			}

			iterTransRecord++;
		}

		// 售票
		m_DataModel.dwSaleTotalAmount   = m_DataModel.dwSingleTicketAmount;
		m_DataModel.dwSaleTotalCount    = m_DataModel.dwSingleTicketCount;
		m_DataModel.dwSaleTotalTrsCount = m_DataModel.dwSingleTicketTrsCount;

		// 充值小计
		m_DataModel.dwRechargeTotalAmount   = m_DataModel.dwRechargeAmount;
		m_DataModel.dwRechargeTotalTraCount = m_DataModel.dwRechargeTrsCount;

		// 净收入
		m_DataModel.dwNetIncomAmount = m_DataModel.dwSaleTotalAmount + m_DataModel.dwRechargeTotalAmount /*+ m_DataModel.dwFailedAmount*/;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::~CSaleStaticSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      获取Svc模型

@param      none

@retval     Model

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::Model& CSaleStaticSvc::GetDataModel()
{
	return m_DataModel;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     服务中数据对象发生变化，通知窗体更新 

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CSaleStaticSvc::NotifyDataModelChanged()
{
	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转到销售统计一览界面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSaleStaticSvc::OnKeyboardF5(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_DETAIL){
		DoDialogFlow(WM_SALESTATIC_DETAIL_TO_OVERVIEW);
		NotifyDataModelChanged();
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转到交易一览现金详细界面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
LRESULT CSaleStaticSvc::OnKeyboardF6(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_OVERVIEW){
		DoDialogFlow(WM_SALESTAIC_OVERVIEW_TO_DETAIL);
		NotifyDataModelChanged();
	}	
	return TRUE;
}
