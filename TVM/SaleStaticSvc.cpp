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
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::CSaleStaticSvc()
	:CTVMForeService(SALE_STATIC_SVC)
{
    // ���ò���Ա����
   m_pDialogFlow->AddDialog(IDD_10901_SALESTATIC_OVERVIEW, new CSaleStaticOverViewDlg(this));
	m_pDialogFlow->AddDialog(IDD_10902_SALESTATIC_DETAIL, new CSaleStaticDetailDlg(this));

    // ���ó˿ͻ���

    // ���û������
    //m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OVERVIEW, _opl(GUIDE_ENTER_F6_ESC2));
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_OVERVIEW);
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_OVERVIEW, CSaleStaticOverViewDlg::IDD);

	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DETAIL);
	//m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_DETAIL, _opl(GUIDE_ENTER_F5_ESC2));
    m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_DETAIL, CSaleStaticDetailDlg::IDD);

	//���û���Ťת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_OVERVIEW, WM_SALESTAIC_OVERVIEW_TO_DETAIL, DIALOG_GROUP_DETAIL);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_DETAIL, WM_SALESTATIC_DETAIL_TO_OVERVIEW, DIALOG_GROUP_OVERVIEW);

    // ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OVERVIEW);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���� [F3/Ctrl] ��������Ϣ�����أ�

@param      (i)WPARAM wParam  δʹ��
@param      (i)LPARAM lParam  δʹ��

@retval     LRESULT  TRUE:����ɹ�  FALSE:����ʧ��

@exception  ��
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
	//��ԃ���ה���
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

	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_OVERVIEW);

	m_pDialogFlow->ShowFirstDialogGroup();

	theAPP_SESSION.SetMenuLevel(SALE_STATIC_SVC);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ������ͳ��һ��������

@param      vector<TransactionRecord>& vec_transInfo(���ݿ��ѯ������)

@param      Model& dataModel��Ҫ��ʼ���������壩

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSaleStaticSvc::InitSaleStaticOverView(vector<TransactionRecord>& vec_transInfo){
	try{
		auto iterTransRecord = vec_transInfo.begin();


		// ѭ���жϵ�ǰ���ף�ȥͳ��
		while(iterTransRecord != vec_transInfo.end()){

			// ��������
			if(iterTransRecord->iTransactionDetail.iState != 2){	// ���ײ�Ϊȡ������ʱ�Ż�ͳ��
				
				// ��Ʊ
				if(iterTransRecord->iTransactionDetail.iUdType == 2){
					if(iterTransRecord->iTransactionDetail.iState == 1 || iterTransRecord->iTransactionDetail.iState == 3){ // ���׹��Ͻ���|| ����ȡ�����Ͻ���
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

				// ��ֵ
				if(iterTransRecord->iTransactionDetail.iUdType == 4){
					if(iterTransRecord->iTransactionDetail.iState == 1 || iterTransRecord->iTransactionDetail.iState == 3){ // ���׹��Ͻ���|| ����ȡ�����Ͻ���
						//m_DataModel.dwRechargeAmount += iterTransRecord->iTransactionDetail.iTotalAmount;
						//m_DataModel.dwRechargeTrsCount++;
					}
					else{
						m_DataModel.dwRechargeAmount += iterTransRecord->iTransactionDetail.iTotalAmount;
						m_DataModel.dwRechargeTrsCount++;
					}
				}
			}

			// ���Ͻ���
			auto iterException = iterTransRecord->vExceptionDetail.begin();
			while(iterException != iterTransRecord->vExceptionDetail.end()){
				m_DataModel.dwFailedAmount		+= iterException->IExceptionAmount;
				m_DataModel.dwFailedTotalAmount += iterException->IExceptionAmount;
				m_DataModel.dwFailedCount++;
				iterException++;
			}

			// Ӳ������
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

			// Ӳ�ҽ���
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

			// ֽ�ҽ���
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

			// ֽ������
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

		// ��Ʊ
		m_DataModel.dwSaleTotalAmount   = m_DataModel.dwSingleTicketAmount;
		m_DataModel.dwSaleTotalCount    = m_DataModel.dwSingleTicketCount;
		m_DataModel.dwSaleTotalTrsCount = m_DataModel.dwSingleTicketTrsCount;

		// ��ֵС��
		m_DataModel.dwRechargeTotalAmount   = m_DataModel.dwRechargeAmount;
		m_DataModel.dwRechargeTotalTraCount = m_DataModel.dwRechargeTrsCount;

		// ������
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
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSaleStaticSvc::~CSaleStaticSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡSvcģ��

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
@brief     ���������ݶ������仯��֪ͨ������� 

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
@brief      ת������ͳ��һ������

@param      ��

@retval     ��

@exception  ��
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
@brief      ת������һ���ֽ���ϸ����

@param      ��

@retval     ��

@exception  ��
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
