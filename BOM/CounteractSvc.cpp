#include "stdafx.h"
#include "CounteractSvc.h"
#include "CounteractConfirmOfficeDlg.h"
#include "waitingofficedlg.h"
#include "tickethelper.h"
#include "guideinfo.h"

// ����ڴ�й©
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CCounteractSvc, CBOMForeService)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCounteractSvc::CCounteractSvc()
    :CBOMForeService(COUNTERACT_SVC)
{

	// ��ʼ�����������Ϣ��
	m_counteractConfirmInfoSize = InitCounteractConfirmInfo();

	m_pDialogFlow->AddDialog(IDD_01102_COUNTERACT_CONFIRM_DLG, new CCounteractConfirmOfficeDlg(this));
	
	// ���ó˿ͻ���

	// ���û������
    // �ȴ���������
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_WAIT, theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_PUT_CARD));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_WAIT, IDD_01102_COUNTERACT_CONFIRM_DLG);
	
    // ȷ�ϻ���
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_CONFIRM, theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_CONFIRM));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_CONFIRM, IDD_01102_COUNTERACT_CONFIRM_DLG);
	
	m_pDialogFlow->AddDialogGroup(DIALOG_GROUP_ERROR,_T(""));
	m_pDialogFlow->AddDialogGroupMember(DIALOG_GROUP_ERROR, IDD_01102_COUNTERACT_CONFIRM_DLG);


	// ���û�����ת
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_COUNTERACT_WAIT_TO_CONFIRM, DIALOG_GROUP_CONFIRM);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_WAIT, WM_COUNTERACT_WAIT_TO_ERROR, DIALOG_GROUP_ERROR);
	m_pDialogFlow->AddFlow(DIALOG_GROUP_CONFIRM,WM_COUNTERACT_CONFIRM_TO_ERROR,DIALOG_GROUP_ERROR);
	// ���ó�ʼ������
	m_pDialogFlow->SetFirstDialogGroup(DIALOG_GROUP_WAIT);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CCounteractSvc::~CCounteractSvc()
{
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		������������    

@param		��    

@retval		��   

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnStart()
{
	__super::OnStart();
	StartReadCard();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����ȷ�ϰ�ť��Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnOK()
{
	if(m_pDialogFlow->GetCurDialogGroupID() == DIALOG_GROUP_CONFIRM)
	{
		Counteract();
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��������      

@param		��     

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::Counteract()
{
	RW_CMD_COUNTERACT rwCmdCounteract;
	rwCmdCounteract.CardType = m_RWReadCardResponse.CardPHType;
	rwCmdCounteract.ProductyType = m_ULCard.applicationData.CardAppMainType;
	rwCmdCounteract.ReasonCode = 0x01;
	rwCmdCounteract.SerialNo = theCOUNT_INFO.GetUDSNForDisplay();
	rwCmdCounteract.TranDateTime = ComGetCurTime();
	RW_BUSSINESS_RESPONSE rwBusinessResponse;
	long counteractResult = m_pRWHelper->Counteract(rwCmdCounteract,rwBusinessResponse);
	if(counteractResult!=RW_RSP_OK)
	{
		theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(rwBusinessResponse.DetailErrorCode));
		m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_CONFIRM_TO_ERROR);
		//theAPP_SESSION.ShowOfficeGuide(m_pRWHelper->GetTpuErrorInfo(rwBusinessResponse.DetailErrorCode));
		return;
	}
	theCOUNT_INFO.UpdateUDSN(rwBusinessResponse.SerialAddnum);
	if(rwBusinessResponse.UDlen>0)
	{
		m_pRWHelper->SaveRWUDData(rwBusinessResponse.UDlen,rwBusinessResponse.UDData);
	}
	if(rwBusinessResponse.ARLen>0)
	{
		m_pRWHelper->SaveRWARData(rwBusinessResponse.ARLen,rwBusinessResponse.ARData,m_ULCard.applicationData.CardAppMainType);
	}
	
	SaveBusinessData(&rwBusinessResponse);
	theAPP_SESSION.ShowOfficeGuide(theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_CARD_NOT_FOUND));
	m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_CONFIRM_TO_ERROR);
	theSERVICE_MGR.SetState(FINISH);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		����ҵ������      

@param      (i)LPRW_BUSSINESS_RESPONSE lpRWBusinessResponse		ҵ���д��������Ϣ

@retval		��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::SaveBusinessData(LPRW_BUSSINESS_RESPONSE lpRWBusinessResponse)
{
	

	SCAuditKey purchaseKey;
	purchaseKey.serviceID = COUNTERACT_SVC;
	purchaseKey.cardIssuancer = ACC;
	purchaseKey.ticketType = TICKET_SINGLE_RIDE;

	SCAuditValue purchaseValue;
	purchaseValue.transCount = 1;
	purchaseValue.transAmount = 0-m_ULCard.applicationData.Balance;


	theSCAudit_MGR.AddLoginPurchase(purchaseKey,purchaseValue);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��ȷ�ϻ��泵Ʊ��Ϣ���ݹ���

@param      none

@retval     ����Label���

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
UINT CCounteractSvc::InitCounteractConfirmInfo()
{
	int i = 0;
	// ��Ʊ���
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_NUMBER,MAX_SBC_CASE_LEN);
	// ��Ʊ����
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_KIND,MAX_SBC_CASE_LEN);
	// ��Ч��
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_VALIDATE_DATE,MAX_SBC_CASE_LEN);
	// Ʊ�����
	m_counteractConfirmInfo[i++].nameLeft = add_wsp(TXT_TICKET_AMOUNT,MAX_SBC_CASE_LEN);
	return i;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���µ���ȷ����Ϣ

@param      (o)CULCard & ulCard		UL����Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::UpdateCounteractConfirmInfo(CULCard & ulCard)
{
	int i = 0;
	// ��Ʊ���
	m_counteractConfirmInfo[i++].nameRight.Format(_T("%.9u."),ulCard.applicationData.CardLogicalNumber);
	// ��Ʊ����
	CAccTicketParam::TICKET_TYPE ticketTypeDetail;
	theACC_TICKET.GetTicketInfo(ulCard.applicationData.CardAppMainType,ticketTypeDetail);
	m_counteractConfirmInfo[i++].nameRight = ticketTypeDetail.ticketTypeName_cn;
	// ��Ч��
	m_counteractConfirmInfo[i++].nameRight = ulCard.applicationData.ValidEndTime.ToString(_T("%.4d/%.2d/%.2d %.2d:%.2d:%.2d"));
	// Ʊ�����
	m_counteractConfirmInfo[i++].nameRight = ComFormatAmount(ulCard.applicationData.Balance) + MONEY_UNIT;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��õ���ȷ����Ϣ��С      

@param      ��

@retval     ����ȷ����Ϣ��С

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
UINT CCounteractSvc::GetCounteractConfirmInfoSize()
{
	return m_counteractConfirmInfoSize;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ��ȷ�ϻ��泵Ʊ��Ϣ

@param      ��

@retval     PLABEL_GROUP

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
PLABEL_GROUP CCounteractSvc::GetCounteractConfirmInfo()
{
	return m_counteractConfirmInfo;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��ȡ��Ƭ��Ϣ    

@param      (i)UINT  resultCode			������
			(i)CRWReadCard * pRWReadCard	��Ƭ��д��ָ��

@retval     none

@exception  ��Ƭ��д�쳣
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnReadCard(UINT resultCode, CRWReadCard * pRWReadCard)
{
	CMessageBox::End();
	auto_ptr<RW_READ_CARD_RESPONSE> pRWReadCardResponse(new RW_READ_CARD_RESPONSE);
	pRWReadCard->GetRWResponse(pRWReadCardResponse.get());
	if(resultCode!=RW_RSP_OK)
	{
		if(pRWReadCardResponse->DetailErrorCode != RW_ERROR_CARD_NO_CARD)
		{
			OnCardError(m_pRWHelper->GetTpuErrorInfo(resultCode));
			return;
		}
		OnCardError(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL));
		return;
	}
	try
	{
		memcpy(&m_RWReadCardResponse,pRWReadCardResponse.get(),sizeof(RW_READ_CARD_RESPONSE));
		m_ULCard.GetCardInfo(pRWReadCardResponse->CardInfo);
		if(!thePRE_TK.CheckIsPreTicket(m_ULCard.applicationData.CardLogicalNumber))
		{
			OnCardError(theGUIDE_INFO.GetOperationMessage(GUIDE_CANCEL_NOT_PRE_SELL));
			return;
		}
		UpdateCounteractConfirmInfo(m_ULCard);
		m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_WAIT_TO_CONFIRM);
	}
	catch (CSysException& e)
	{
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...)
	{
		theEXCEPTION_MGR.ProcessException(CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__));
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		��ʼ����  

@param		��

@retval		��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::StartReadCard()
{
	theSERVICE_MGR.SetState(ISSUEING);
	m_pRWHelper->ReadCard(true);
	INT_PTR ret = CMessageBox::Show(theAPP_SESSION.GetServiceNameCH((SERVICE_ID)this->GetServiceID()),GUIDE_WAIT_CARD,CMessageBox::BTN_CANCEL,DT_CENTER,CMessageBox::DISPLAY_TIME_180);
	if(ret == IDCANCEL)
	{
		m_pRWHelper->StopReadCard();
		theSERVICE_MGR.SetState(FINISH);
		//m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_WAIT_TO_ERROR);
		//// ��ʾ�򵼱���
		theAPP_SESSION.ShowAllGuide(theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageCn(GUIDE_CANCEL), theGUIDE_INFO.GetPassengerMessageEn(GUIDE_CANCEL));	
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief		����������Ӧ����    

@param      (i)CString errorMsg		������Ϣ

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CCounteractSvc::OnCardError(CString errorMsg)
{
	theSERVICE_MGR.SetState(FINISH);
	m_pDialogFlow->ShowNextDialogGroup(WM_COUNTERACT_WAIT_TO_ERROR);
	theAPP_SESSION.ShowOfficeGuide(errorMsg);
}