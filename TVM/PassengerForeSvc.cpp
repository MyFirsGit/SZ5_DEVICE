#include "StdAfx.h"
#include "PassengerForeSvc.h"
#include <assert.h>
#include "ChargeSvc.h"
/*
*�˿�ҵ����
*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CPassengerForeSvc,CTVMForeService)
	ON_SERVICE_TIMER()	//OnTimer����Ϣ����
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief          ���캯��

@param      (i)SERVICE_ID serviceID              ��ǰserviceID
@param      (i)SERVICE_ID parentServiceID    ��serviceID

@retval        none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CPassengerForeSvc::CPassengerForeSvc(SERVICE_ID serviceID, SERVICE_ID parentServiceID) :CTVMForeService(serviceID,parentServiceID)
{
	// CPassengerForeSvc��ʵӦ���Ǹ�virtual class,��Զm_pCommonModel��ָ��һ��m_pDataModel
	//(�ڱ���ChargeSvc��Ĺ��캯������),Ȼ��������downward cast�á���������ֻ��һ���ٵ�m_pCommonModel��ʼֵ��
	m_pCommonModel = NULL;
	m_nReadedCardCount = 0;
	m_bIsFinishedWriteData = false;
	bIsConfirmReadCard = false;
	m_bNeedClickOkButton = theFunction_INFO.IsNeedPassengerClickOkButton();

	m_timerInterval = 0;
	m_bRechargeConfirm = false;
	m_bRechargeOverlimit = false;
	m_bStopCircleRead = true;

	m_bExitWebThread = false;
	m_hWebEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_nCurWebOper = 0;

	m_bWebPaySuccess = false;

	m_bCoinHopperAccept = true;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief			��������

@param		none

@retval			none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CPassengerForeSvc::~CPassengerForeSvc()
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
CPassengerForeSvc::CommonModel& CPassengerForeSvc::GetCommonModel()
{
	return *m_pCommonModel;
}

////////////////////////////////////////////////////////////////////////
/**
@brief     ���������ݶ������仯,֪ͨ������� 

@param      none

@retval     none

@exception  none
*/
////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::NotifyDataModelChanged()
{
	if (m_pCommonModel->paymentInfo.paymentMethod == PaymentMethod_t::PAYMENT_METHOD_CASH)
	{
		m_pCommonModel->paymentInfo.lDueAmount = m_pCommonModel->paymentInfo.lPriceAmount + m_pCommonModel->paymentInfo.lTaxAmount;
		m_pCommonModel->paymentInfo.lPaidAmount = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount + m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
	}
	m_pCommonModel->paymentInfo.lChangeAmount = m_pCommonModel->paymentInfo.lPaidAmount - m_pCommonModel->paymentInfo.lDueAmount;

	__super::NotifyDataModelChanged();
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   ����ʵ�ֺ���   

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnReadCard(UINT resultCode, CRWReadCard* rwReadCard)
{
	try{

		CMessagePictureBox::End();
		CString strTemp(_T(""));

		// ��ȡƱ����Ϣ
		RW_READ_CARD_RESPONSE cardRspInfo;
		rwReadCard->GetRWResponse(&cardRspInfo);
		//////////////////////////////////////////////////////////////////////////
		SaveCardInfo(&cardRspInfo);
		return;
		//////////////////////////////////////////////////////////////////////////

		// ����ʧ��
		if(resultCode != RW_RSP_OK) {
			if(resultCode==RW_ERROR_CARD_NO_CARD){
				// // ȡ�����˳�����,����Ĭ��ҵ��
				LOG("���������޿�,����Ĭ��ҵ��");
				theSERVICE_MGR.SetState(RECEPT);
				CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
				NOTIFY_PROCESSING_WINDOW();
				pMainSvc->StartDefaultService();	
				return;
			}

			LOG("����ʧ�ܡ�");		
			// ��������Ʊ
			if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {
				LOG("������������");
				//BlockCard();
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(TXT_MSG_GUIDE_THIS_CARD_HAS_BEEN_LOCKED),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				// ��������
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;
			}
			// Ѱ�������ʧ��
			else if((resultCode ==  RW_RSP_READERROR) || (resultCode == RW_RSP_FINDERROR) || (resultCode == RW_RSP_READ_CARD_FAILED)){		
				LOG("������Ѱ��ʧ�ܡ�");	
				CMessagePictureBox::Show(
					theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),
					_tl(TXT_PSG_TICKET_CANT_RECOGNIZE),
					TVM_TIP_WARINGS,
					CMessagePictureBox::BTN_OK,DT_LEFT,
					theAFC_TVM_OPERATION.GetCancelDealTime());

				// ��������
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;
			}
			// Ʊ���������ֵ����
			else{
				strTemp = theTPUERROR_INFO.GetTpuErrorInfo(resultCode);
				LOG("Ʊ���������ֵ������");	
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(GUIDE_CARD_CAN_NOT_DO_BUSINESS),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);

				// ��������
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;	
			}
		}
		// �����ɹ�
		else{
			// �ж���������(�������Ͳ���ȷ)
			if(cardRspInfo.CardPHType != MIFARE1K && cardRspInfo.CardPHType != MIFARE4K && cardRspInfo.CardPHType != CPU_TICKET){
				LOG("��ֵƱ�����Ͳ���ȷ��");
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(GUIDE_IN_CARD_TYPE_ERROR),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime());

				// ��������
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;
			}

			// ����Ʊ����Ϣ
			SaveCardInfo(&cardRspInfo);

			NotifyDataModelChanged();
		}
	}
	catch (CSysException& e){
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch (...) {
		CInnerException e(CInnerException::MODULE_ID, CInnerException::OTHER_ERR, _T(__FILE__), __LINE__);
		OnError(theEXCEPTION_MGR.GetDescription(e));
		theEXCEPTION_MGR.ProcessException(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���յ�Ӳ�һص�����

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinInserted(UINT res, CCHReadReceivs* chReadReceiveAmount)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;
		tCHACoinValueInfo  chAmountInfo;
		bool bIsCheckOutInsertMoeny = false;

		// �յ��ֽ�Ͷ�룬֪ͨǰ̨����
		if (m_pCommonModel->operationInfo.banknoteAcceptState==COIN_ACCEPT_STATE_NONE && m_pCommonModel->operationInfo.coinAcceptState==COIN_ACCEPT_STATE_NONE && res){
			GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ACCEPTED,0,(LPARAM)0);
		}

		// һ�������������
		if(res == 0){
			// ��Ͷ��,����ֹͣ��ʱ����
			EndInsertTimer();
			LOG("��Ӳ��Ͷ�롣");

			// ����Ͷ���ܶ�		
			chReadReceiveAmount->GetCHResponse(&chAmountInfo);
			if (chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum >0 || chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum > 0)
			{
				LOG("Ӳ��Ͷ��ɹ�");
				m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_INSERTED;
				GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ACCEPTED,0,(LPARAM)0);
			}


TRY_READ_AGIAN:
			RenewCoinReceivedAmount(chAmountInfo);
			RenewCoinReceivedCount(chAmountInfo);

			// ���½�������Ϣ��ʾ��Ϣ
			NotifyDataModelChanged();

			// 1.�ж�Ͷ�����Ƿ��㹻��
			if (IsReceiveEnoughMoney()){
				// 1.1 �ж��Ƿ��������㣿
				// 1.2 �ж��Ƿ��������㣿
				// 1.3 �ж��������Ƿ��㹻����?
				// 1.4 �ж������������������������������?
				if (m_pCommonModel->paymentInfo.lChangeAmount >0){
					// 1.2�жϵ�ǰģʽ�Ƿ�֧�����㣿
					if (theTVM_STATUS_MGR.GetChangeMode()!= DEVICE_CONTRL_CODE_NO_CHANGE){
						// 1.2 �ж��Ƿ��㹻���㣿
						if (CalculateChangeCount()){
							if(m_pCommonModel->operationInfo.finishType == OVER_CHANGE_COUNT_LIMIT){
								LOG("�����������ޡ�");
								// 1.4.2�˻���������Ǯ
								ReturnWhenCoinInserted();
							}
							else{
								// 1.3�ж����������Ƿ񳬹���������������ƣ�
								// 1.4�������Ƿ��ޣ�(Ԥ��)
								if (!IsCoinChageMaxCount()){// ��������ʱ�Ѿ��жϡ�
									// 1.3.1�ر�Ͷ�ҿڡ�ֹͣʶ��Ӳ��
									if (m_pCommonModel->operationInfo.isCoinSlotOpen){
										m_pCommonModel->operationInfo.isCoinSlotOpen = false;
										COIN_HELPER->CH_StopInsertInfo();
										COIN_HELPER->CH_EndReceive();
									}
									// 1.3.2ֹͣʶ��ֽ��,�������ʶ��
									if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
										m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
										BANKNOTE_HELPER->StopValidation();
									}
									// 1.3.3�ٶ�һ������,��ֹ�û�Ͷ�ҹ��쵼�¶�Ͷ
									if (!bIsCheckOutInsertMoeny){
										LOG("�ٶ�һ�ν��(Ӳ��Ͷ��:��Ҫ����)��");
										long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
										if (errCode == E_NO_CH_ERROR){
											bIsCheckOutInsertMoeny = true;
											goto TRY_READ_AGIAN;
										}
									}
									// 1.3.4���ý��ճɹ�
									OnAcceptMoneyOK();
								}
								// 1.4������������
								else{
									LOG("�����������ޡ�");
									// 1.4.1�������״̬
									m_pCommonModel->operationInfo.finishType = OVER_CHANGE_COUNT_LIMIT;

									// 1.4.2�˻���������Ǯ
									ReturnWhenCoinInserted();
								}
							}
						}
						// 1.5�������
						else{
							LOG("������ޡ�");
							// 1.5.1�������״̬
							m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;
							// 1.5.2�˻���������Ǯ
							ReturnWhenCoinInserted();
						}
					}
					// 1.6��ǰģʽ��֧������,ֱ����ʾ���㲻��
					else{
						LOG("��ǰģʽ��֧�����㡣");
						// 1.6.1����״̬
						m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;
						// 1.6.2�˻���������Ǯ
						ReturnWhenCoinInserted();
					}

				}
				// 1.8��������,ֱ�ӽ�����һ��
				else{
					// 1.8.1�ر�Ͷ�ҿڡ�ֹͣʶ��Ӳ��
					if (m_pCommonModel->operationInfo.isCoinSlotOpen){
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;
						COIN_HELPER->CH_StopInsertInfo();
						COIN_HELPER->CH_EndReceive();
					}
					// 1.8.2ֹͣʶ��ֽ��,�������ʶ��
					if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
						m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
						BANKNOTE_HELPER->StopValidation();
					}
					// 1.8.3�ٶ�һ������,��ֹ�û�Ͷ�ҹ��쵼�¶�Ͷ
					if (!bIsCheckOutInsertMoeny){
						LOG("�ٶ�һ�ν��(Ӳ��Ͷ��:����Ҫ����)��");
						long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
						if (errCode == E_NO_CH_ERROR){
							bIsCheckOutInsertMoeny = true;
							goto TRY_READ_AGIAN;
						}
					}	
					// 1.8.4���ý��ճɹ�
					OnAcceptMoneyOK();
				}
			}
			// 2.�ж��Ƿ�Ӳ�Һ�ֽ�Ҷ��ﵽ���������
			else if (IsBothCoinBanknoteMaxCount()){
				LOG("Ӳ�ҵ���ֽ�Ҷ��������������");
				m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
				// �رս��տ�
				if (m_pCommonModel->operationInfo.isCoinSlotOpen){
					m_pCommonModel->operationInfo.isCoinSlotOpen = false;
					COIN_HELPER->CH_StopInsertInfo();
					COIN_HELPER->CH_EndReceive();
				}
				// �˻������յ�Ǯ(�ܹ��ߵ�������,Ӳ�ҡ�ֽ��һ��������)
				StartReturnCash();
				// ���淵�����˱һص�������ִ�С�
			}
			// 3. �Ƿ�Ӳ�������ﵽ���
			else if(IsCoinMaxCount()){
				// 3.1���ֽ�ҿڴ�,�����ڽ���...
				if (m_pCommonModel->operationInfo.isBankNoteSlotOpen)
				{
					LOG("Ӳ�������ﵽ���ֵ,�ȴ�Ͷ��ֽ�ҡ�");					
					// 3.2 �ٶ�һ�ν��(Ӳ��Ͷ��:Ӳ��Ͷ����������)
					if (!bIsCheckOutInsertMoeny){
						// 3.3 �ر�Ӳ�ҽ��տ�
						COIN_HELPER->CH_StopInsertInfo();
						COIN_HELPER->CH_EndReceive();
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;

						LOG("�ٶ�һ�ν��(Ӳ��Ͷ��:��Ҫ����)��");
						long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
						if (errCode == E_NO_CH_ERROR){
							bIsCheckOutInsertMoeny = true;
							goto TRY_READ_AGIAN;
						}
					}
					m_pCommonModel->operationInfo.isCoinMaxCount = true;
					// ͬʱ��Ҫ����Ͷ�볬ʱ��ʱ����
					InsertMoneyNeedStartTimer();
					CString strGuide = _tl(TXT_COIN_MAX_INSERT_BANKNOTE);
					TCHAR* sGuide = strGuide.GetBuffer();
					GetCurReceptionDlg()->PostMessage(WM_UPDATE_RECEPITON_GUIDE, (WPARAM)sGuide, 0);
					// ��ʾӲ������Ͷ�볬�����ֵ��
					NotifyDataModelChanged();
				}
				// 3.4 ���û�д�,��ֱ����Ǯ,Ͷ����������
				else{
					LOG("��ǰֻ��Ӳ�ҽ���,�˻�����Ӳ�ҡ�");
					// 3.4.1�������״̬
					m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
					// 3.4.2�˻���������Ǯ
					ReturnWhenCoinInserted();
				}				
			}
			// 4.������Ǯ
			else{
				LOG("�����ȴ�Ͷ��,��ȡ��");
				m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
				COIN_HELPER->CH_GetInsertInfoAsync(this);
				InsertMoneyNeedStartTimer();
			}
		}
		// ��������ֶ���ֹ���簴��ȡ�������쳣�����ޡ��������ޡ����㳬�ޡ�Ͷ�볬ʱ��ǮͶ���˵Ȳ�����
		else if(res == CCHException::ERROR_CH_STOP_READ_AMOUNT){
			LOG("�ص���ֹ��ȡ��");
			// 1.�չ�Ǯ��,��ֽ�Ҳ����ֹͣʶ��ص����˺�����
			if (m_pCommonModel->operationInfo.isCoinSlotOpen){
				m_pCommonModel->operationInfo.isCoinSlotOpen = false;
				COIN_HELPER->CH_StopInsertInfo();
				COIN_HELPER->CH_EndReceive();
			}

			// ��Ҫ�ڶ�һ�ν�����Ϣ
			LOG("�ٶ�һ�ν��(��ֹͶ��ʱ���ȡ��)��");
			long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
			if (errCode == E_NO_CH_ERROR){
				bIsCheckOutInsertMoeny = true;
				if(chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum >0 || chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum > 0)
				{
					m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_INSERTED;
					RenewCoinReceivedAmount(chAmountInfo);
					RenewCoinReceivedCount(chAmountInfo);
					NotifyDataModelChanged();
				}
			}
			// 2.�����ȡ������,��ִ����Ǯ,Ӳ�һص������˺�����
			if( m_pCommonModel->operationInfo.finishType == PASSAGE_CANCEL			 || 
				m_pCommonModel->operationInfo.finishType == SERVICE_ERROR_CANCEL	 ||
				m_pCommonModel->operationInfo.finishType == OVER_MONEY_COUNT_LIMIT	 ||
				m_pCommonModel->operationInfo.finishType == OVER_CHANGE_COUNT_LIMIT  ||
				m_pCommonModel->operationInfo.finishType == OVER_CHANGE_AMOUNT_LIMIT ||
				m_pCommonModel->operationInfo.finishType == OVER_MONEY_AMOUNT_LIMIT  ||
				m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME		 ||
				m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME		 ||
				m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR	&& !theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)
				){
					// ���Ӳ����Ͷ��,��Ӳ��
					if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
						m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
						COIN_HELPER->CH_RefundAsync(this);
					}
					else{
						OnReturnFinish();
					}
			}
			else if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR){
				OnReturnFinish();
			}
			else if(m_pCommonModel->operationInfo.isFinishedReceveMoney){// ����յ��㹻���Ǯ�ˣ���ʼ��Ʊ
				goto TRY_READ_AGIAN;
				//OnAcceptMoneyOK();
			}
		}
		//// �����ж��Ƿ����쳣���?
		//else if(COIN_HELPER->IsExceptionCode(res)){
		//	//m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
		//	throw CCHHardwareException(res,_T(__FILE__),__LINE__);
		//}
		//else{
		//	throw CCHHardwareException(res,_T(__FILE__),__LINE__);
		//}
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}	
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��Ӳ��Ͷ���㹻����Ϊ�ﵽҪ��ʱ,�˻����յ�Ӳ�Ҽ�ֽ��

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::ReturnWhenCoinInserted(){
	// 1.�رս��տ�,ͬʱֹͣʶ��
	if (m_pCommonModel->operationInfo.isCoinSlotOpen){
		m_pCommonModel->operationInfo.isCoinSlotOpen = false;
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
	}

	if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
		m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		COIN_HELPER->CH_RefundAsync(this);
	}

	// 3.ֹֽͣ�ҽ���ʶ��:�������ʶ��
	if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		BANKNOTE_HELPER->StopValidation();
	}
	// 4.�˳�ֽ��,���Ͷ��ֽ�������ﵽ���
	if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
		if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
			m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
			BANKNOTE_HELPER->ReturnAllAsync(this);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      Ӳ������ص�����

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinChanged(UINT res,CCHChange* chChange)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;

		// 1.����ɹ�
		if (res == 0){
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				LOG("��Ӳ������ʱ�������ơ�");
				try{
					IO_HELPER->OpenReturnLight();
					IO_HELPER->OpenReturnCupIndicator();
				}
				catch(CSysException& e){
					theEXCEPTION_MGR.ProcessException(e);
				}
				catch(...){
					LOG("��������ʧ�ܡ�");
				}
			}
			if(0 == res){
				LOG("���㷵�سɹ���");
				// �������
				theDeviceRegAR.AddChTransMotorCount();
			}
			else{
				LOG("���㷵��ʧ�ܡ�");
			}
			m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_SUCCESS;
			tCHAChangeNum rsp;
			chChange->GetResponse(&rsp);
			CCHGetStatus cmd;
			tCHADevStatus rsp2;
			if(0 == cmd.ExecuteCommand())
				cmd.GetResponse(&rsp2);

			CTCoinCountInfo::COIN_BOX_INFO			  hopperA	 = theCOIN_COUNT.GetCoinHopperboxAInfo();
			CTCoinCountInfo::COIN_BOX_INFO			  hopperB	 = theCOIN_COUNT.GetCoinHopperboxBInfo();
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  changeBoxA = theCOIN_COUNT.GetChangeCoinboxAInfo();
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  changeBoxB = theCOIN_COUNT.GetChangeCoinboxBInfo();

			LOG("---------------------Ӳ������ǰ����״̬-------------------");
			LOG("Hopper A ������%d",hopperA.ulCurCount);
			LOG("Hopper B ������%d",hopperB.ulCurCount);
			LOG("������ A ������%d",changeBoxA.ulCount);
			LOG("������ B ������%d",changeBoxB.ulCount);
			LOG("---------------------Ӳ�ҳ��Ҽ���״̬-------------------");
			LOG("����������1Ԫ����%d",rsp.wMHopper1Num);
			LOG("����������5�ǣ���%d",rsp.wMHopper2Num);

			LOG("---------------------Ӳ�����������״̬-------------------");
			LOG("Hopper B ����:%d",rsp2.wHopperBCnt);
			LOG("Hopper A ����:%d",rsp2.wHopperACnt);
			LOG("������ A ����:%d",rsp2.wPreparationACnt);
			LOG("������ B ����:%d",rsp2.wPreparationBCnt);
			LOG("����������:%d",rsp2.wRecoverCnt);

			// ͬ������
			theCOIN_COUNT.UpdataBoxCountData(
				rsp2.wHopperACnt,			// Hopper 1 ����
				rsp2.wHopperBCnt,			// Hopper 2 ����
				rsp2.wPreparationACnt,		// ������ 1 ����
				rsp2.wPreparationBCnt,		// ������ 2 ����
				rsp2.wRecoverCnt,			// ������ 1 ����
				0			// ������ 2 ����
				);

			// ���ݷ��صĽ���ж������Ƿ���ĳɹ���
			DWORD dwChangedOneYuan  = rsp.wMHopper1Num;
			DWORD dwChangedHalfYuan = rsp.wMHopper2Num;
			BOOL bIsOneYuanOk  = true;
			BOOL bIsHalfYuanOk = true;

			if(dwChangedOneYuan != m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount){
				bIsHalfYuanOk = false;
				m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount += (m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount - dwChangedOneYuan)  * 100;
			}
			if(dwChangedHalfYuan != m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount){
				bIsHalfYuanOk = false;
				m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount += (m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount - dwChangedHalfYuan) * 100;
			}


			// ��������������ϸ��Ϣ������

			DWORD dwShouldLeft = 0;
			DWORD dwAlreadySendOut = 0;
			DWORD dwOneLeft = rsp.wMHopper1Num;	// һԪ�������ʣ��
			DWORD dwHalfLeft= rsp.wMHopper2Num;	// ����������ʣ��


			// ��������
			if(bIsHalfYuanOk && bIsOneYuanOk){
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.clear();

				// Hopper A ����
				if(hopperB.ulCurCount > rsp2.wHopperBCnt){ // ������
					dwAlreadySendOut = hopperB.ulCurCount - rsp2.wHopperBCnt;

					if(dwAlreadySendOut > rsp.wMHopper2Num){	// �п����쳣
						dwAlreadySendOut = 0;
					}
					else{
						dwHalfLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2] = dwAlreadySendOut;
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}

				// Hopper B ����
				if(hopperA.ulCurCount > rsp2.wHopperACnt){
					dwAlreadySendOut = hopperA.ulCurCount - rsp2.wHopperACnt;

					if(dwAlreadySendOut > rsp.wMHopper1Num){// һԪ
						dwAlreadySendOut = 0;
					}
					else{
						dwOneLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1] = dwAlreadySendOut;
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}

				// ������ A����
				if(changeBoxA.ulCount > rsp2.wPreparationACnt){
					dwAlreadySendOut = changeBoxA.ulCount - rsp2.wPreparationACnt;

					if(dwAlreadySendOut > dwOneLeft){// һԪ
						dwAlreadySendOut = 0;
					}
					else{
						dwOneLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1] = dwAlreadySendOut;
						theDeviceRegAR.AddChSupplayBoxAChangeCount(dwAlreadySendOut);
					}
				}
				// ������ B����
				if(changeBoxB.ulCount > rsp2.wPreparationBCnt){
					dwAlreadySendOut = changeBoxB.ulCount - rsp2.wPreparationBCnt;

					if(dwAlreadySendOut > dwOneLeft){// һԪ(BUG������û����⣬��ǰ��Ϊ����ǵ�dwHalfLeft)
						dwAlreadySendOut = 0;
					}
					else{
						dwOneLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_2] = dwAlreadySendOut;
						theDeviceRegAR.AddChSupplayBoxBChangeCount(dwAlreadySendOut);
					}
				}
			}
			else{
				// ������ϸ�Ƿ�ƥ�䣿
				auto iter = m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.find(COIN_HOPPER_2);
				if(iter != m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.end()){
					dwShouldLeft = hopperB.ulCurCount - iter->second;// �����Ӧ��ʣ������
					if(dwShouldLeft != rsp2.wHopperBCnt){
						dwAlreadySendOut = abs(hopperB.ulCurCount  - rsp2.wHopperBCnt);//Ӧ��ʵ���ҳ�������
						if(dwAlreadySendOut > iter->second || dwAlreadySendOut > rsp.wMHopper2Num){
							dwAlreadySendOut = 0;
						}
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2] = dwAlreadySendOut;//abs(hopperA.ulCurCount  - rsp.Common.OneY_inHopper);
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}
				else if(hopperB.ulCurCount >= rsp2.wHopperBCnt){
					dwAlreadySendOut = hopperB.ulCurCount - rsp2.wHopperBCnt;
					if(dwAlreadySendOut <= rsp.wMHopper1Num){
						dwOneLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2] = dwAlreadySendOut;
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}

				iter = m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.find(COIN_HOPPER_1);
				if(iter != m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.end()){
					dwShouldLeft = hopperA.ulCurCount - iter->second;
					if(dwShouldLeft != rsp2.wHopperACnt){
						dwAlreadySendOut = abs(hopperA.ulCurCount  - rsp2.wHopperACnt);
						if(dwAlreadySendOut > iter->second || dwAlreadySendOut > rsp.wMHopper1Num){
							dwAlreadySendOut = 0;
						}
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1] = dwAlreadySendOut;//abs(hopperB.ulCurCount  - rsp.Common.HalfY_inHopper);
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}
				else if(hopperA.ulCurCount >= rsp2.wHopperACnt){
					dwAlreadySendOut = abs(hopperA.ulCurCount - rsp2.wHopperACnt);
					if(dwAlreadySendOut <= rsp.wMHopper1Num){
						dwHalfLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1] = dwAlreadySendOut;
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}

				iter = m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.find(COIN_CHANGE_BOX_1);
				if(iter != m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.end()){
					dwShouldLeft = changeBoxA.ulCount - iter->second;
					if(dwShouldLeft != rsp2.wPreparationACnt){
						dwAlreadySendOut = abs(changeBoxA.ulCount - rsp2.wPreparationACnt);
						if(dwAlreadySendOut > iter->second || dwAlreadySendOut > rsp.wMHopper1Num){
							dwAlreadySendOut = 0;
						}
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1] = dwAlreadySendOut;//abs(changeBoxA.ulCount - rsp.Common.OneY_inPreparation);
						theDeviceRegAR.AddChSupplayBoxAChangeCount(dwAlreadySendOut);
					}
				}
				else if(changeBoxA.ulCount != rsp2.wPreparationACnt){
					dwAlreadySendOut = abs(changeBoxA.ulCount - rsp2.wPreparationACnt);
					if(dwAlreadySendOut <= dwOneLeft){
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1] = dwAlreadySendOut;
						theDeviceRegAR.AddChSupplayBoxAChangeCount(dwAlreadySendOut);
					}
				}

				iter = m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.find(COIN_CHANGE_BOX_2);
				if(iter != m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.end())
				{
					dwShouldLeft = changeBoxB.ulCount - iter->second;
					if(dwShouldLeft != rsp2.wPreparationBCnt){
						dwAlreadySendOut = abs(changeBoxB.ulCount - rsp2.wPreparationBCnt);
						if(dwAlreadySendOut > iter->second || dwAlreadySendOut > rsp.wMHopper2Num){
							dwAlreadySendOut = 0;
						}
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_2] = dwAlreadySendOut;//abs(changeBoxB.ulCount - rsp.Common.HalfY_inPreparation);
						theDeviceRegAR.AddChSupplayBoxBChangeCount(dwAlreadySendOut);
					}
				}
				else if(changeBoxB.ulCount != rsp2.wPreparationBCnt){
					dwAlreadySendOut = abs(changeBoxB.ulCount - rsp2.wPreparationBCnt);
					if(dwAlreadySendOut <= dwHalfLeft){
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_2] = dwAlreadySendOut;
						theDeviceRegAR.AddChSupplayBoxBChangeCount(dwAlreadySendOut);
					}
				}
			}

			if(!bIsOneYuanOk || !bIsHalfYuanOk){
				m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_FAIL;
				throw CCHHardwareException(CCHHardwareException::ERROR_CH_CHANGE_CHANGE_FAILED,_T(__FILE__),__LINE__);
			}
			else// �������,������ɽ���
				OnReturnFinish();
		}
		// 2.����ʧ��
		else{
			LOG("���㷵��ʧ�ܡ�");
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.clear();// ����ʧ��
			m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount = m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount * 100 + m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount * 50;
			m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_FAIL;		
			throw CCHHardwareException(res,_T(__FILE__),__LINE__);
		}
	}
	catch (CSysException& e){
		//m_pCommonModel->operationInfo.m_vecException.push_back(e);
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ѹӲ�ҵ��������������   

@param      UINT res,CCHAccept*

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinReceived(UINT res,CCHAccept* chAccept)
{
	try{

		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;

		// 1.���ճɹ�
		if (res==0){

			LOG("Ӳ�һ��ճɹ���");
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_COLLECTED;

			tCHACoinValueInfo rsp;
			chAccept->GetResponse(&rsp);
			int OneY_inHopper = rsp.stCoinBoxValueInfo[0].uiCoinNum;
			int HalfY_inHopper = rsp.stCoinBoxValueInfo[1].uiCoinNum;
			int OneY_inRecover = rsp.stCoinBoxValueInfo[2].uiCoinNum;
			CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collecBoxA = theCOIN_COUNT.GetCollectionCoinboxAInfo();	// �ұߣ�������ǡ�һԪ�˿�Ͷ�룩
			CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collecBoxB = theCOIN_COUNT.GetCollectionCoinboxBInfo();	// ��ߣ�������ǡ�һԪ�����߻��գ�
			CTCoinCountInfo::COIN_BOX_INFO			  hopperA	 = theCOIN_COUNT.GetCoinHopperboxAInfo();		// �ұߣ�5�ǣ�
			CTCoinCountInfo::COIN_BOX_INFO			  hopperB	 = theCOIN_COUNT.GetCoinHopperboxBInfo();		// ��ߣ�1Ԫ��
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  changeA	= theCOIN_COUNT.GetChangeCoinboxAInfo();
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  changeB	= theCOIN_COUNT.GetChangeCoinboxBInfo();
			LOG("----------------����ǰ-----------------------");
			LOG("Hopper A ����:%d",hopperA.ulCurCount);
			LOG("Hopper B ����:%d",hopperB.ulCurCount);
			LOG("������ A 1Ԫ����:%d",collecBoxA.ulCount_1yuan);
			//LOG("������ A 5������:%d",collecBoxA.ulCount_5jiao);
			//LOG("������ B 1Ԫ����:%d",collecBoxB.ulCount_1yuan);
			//LOG("������ B 5������:%d",collecBoxB.ulCount_5jiao);


			LOG("----------------���պ�-----------------------");
			LOG("Hopper A ����:%d",OneY_inHopper);
			LOG("Hopper B ����:%d",HalfY_inHopper);
			//LOG("������ A ����:%d",rsp.OneY_inPreparation);
			//LOG("������ B ����:%d",rsp.HalfY_inPreparation);
			LOG("������ 1 Ԫ����:%d",OneY_inRecover);
			//LOG("������ 5 ������:%d",rsp.HalfY_inRecover);

			// ͬ������
			theCOIN_COUNT.UpdataBoxCountData(OneY_inHopper,HalfY_inHopper,changeA.ulCount,changeB.ulCount,OneY_inHopper,collecBoxA.ulCount_5jiao);
			m_pCommonModel->paymentInfo.CoinSaveDetail.clear();

			// ͬ�����������
			DWORD dwOneInColl = 0,dwHalfInColl = 0,dwCollecOneCount = 0,dwCollecHalfCount = 0;
			bool bAddToOneCollection = false,bAddToHalfCollection = false;
			dwOneInColl = collecBoxA.ulCount_1yuan + collecBoxB.ulCount_1yuan;
			dwHalfInColl = collecBoxA.ulCount_5jiao + collecBoxB.ulCount_5jiao;

			// ����������Ϣ
			if(/*hopperB.ulCurCount != rsp.OneY_inHopper && */hopperB.coinBoxID.ToString() != INVALID_ID){
				m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_1] = m_pCommonModel->paymentInfo.PayCoinInfo[Coin1Yuan];//ʵ��������ڽ��յ�Ǯ
			}

			if(/*hopperA.ulCurCount != rsp.HalfY_inHopper && */hopperA.coinBoxID.ToString() != INVALID_ID){
				m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_2] = m_pCommonModel->paymentInfo.PayCoinInfo[CoinHalfYuan];//ʵ��Ͷ����ڽ��յ�Ǯ
			}

			/*if(bAddToOneCollection){
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_COLL_BOX_1_1] = dwCollecOneCount;
			}

			if(bAddToHalfCollection){
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_COLL_BOX_1_5] = dwCollecHalfCount;
			}*/

			StartCHChange();
		}
		// 2.����ʧ��(��Ҫ���Ǽ���)
		else{
			LOG("Ӳ�һ���ʧ�ܡ�");

			// ��λ��Ӳ��һ������ѭ��������
			m_pCommonModel->paymentInfo.CoinSaveDetail.clear();

			// ������Ϣ
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_1] = m_pCommonModel->paymentInfo.PayCoinInfo[Coin1Yuan];//ʵ��������ڽ��յ�Ǯ
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_2] = m_pCommonModel->paymentInfo.PayCoinInfo[CoinHalfYuan];//ʵ��Ͷ����ڽ��յ�Ǯ

			// ���ݴ����
			CTCoinCountInfo::COIN_TEMPORARY1_INFO tempB = theCOIN_COUNT.GetCoinTemporary1Info();

			if(tempB.ulCount_1yuan != 0){
				CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
				hopperB.ulCurCount += tempB.ulCount_1yuan;
				theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);

				LOG("�ݴ������Ѽ��������ȥ��һԪö����%d",tempB.ulCount_1yuan);

				tempB.ulCount_1yuan = 0;
				theCOIN_COUNT.SetCoinTemporary1Info(tempB);
			}

			// ����״̬Ϊ����ʧ��
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_COLLECTED_FAILD;
			throw CCHHardwareException(res,_T(__FILE__),__LINE__);
		}
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˱һص����� 

@param      UINT res ����ֵ��CCHRefund* ����״̬

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinReturned(UINT res,CCHRefund*)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;

		// 1.ִ�гɹ�
		if (res == 0){
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				LOG("�������ơ�");
				IO_HELPER->OpenReturnLight();	
				IO_HELPER->OpenReturnCupIndicator();
			}
			// 1.1����״̬
			LOG("Ӳ���˸��˿ͳɹ���");
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_RETURNED;
			m_pCommonModel->paymentInfo.lReturnAmount += m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
			//m_pCommonModel->paymentInfo.PayCoinInfo.clear();
			// 2.1����
			OnReturnFinish();
		}
		// 2.ִ��ʧ��
		else{
			LOG("Ӳ���˸��˿�ʧ�ܡ�");
			// �˱�ʧ��,��Ҫ��ӡ,��һ��״̬,�������Ӳ��������
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_RETURN_FAILD;
			//if(COIN_HELPER->IsExceptionCode(res)){
			//	throw CCHHardwareException(res,_T(__FILE__),__LINE__);
			//}
		}
	}
	catch(CSysException& e){
		//m_pCommonModel->operationInfo.m_vecException.push_back(e);//��ӵ��쳣������ȥ
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ�һص�����

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnBanknoteInserted(UINT resultCode,CBRHOpenAndValidate* bhReadReceiveAmount)
{
	try{
		// ��ǰֽ��ģ����æµ״̬
		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_BUSY;
		//// �յ��ֽ�Ͷ�룬֪ͨǰ̨����
		//if (m_pCommonModel->operationInfo.banknoteAcceptState==BANKNOTE_ACCEPT_STATE_NONE && m_pCommonModel->operationInfo.coinAcceptState==COIN_ACCEPT_STATE_NONE && resultCode){
		//	GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ACCEPTED,0,(LPARAM)0);
		//}

		//һ. �����������
		if(resultCode == 0){
			// ��Ͷ��,����ֹͣ��ʱ����
			EndInsertTimer();
			LOG("ֽ�ҽ��ճɹ�");
			// ����Ͷ���ܶ�
			tBncCashInfo feedback;
			bhReadReceiveAmount->GetResponse(&feedback);

			RenewBanknoteReceivedAmount(feedback);
			RenewBanknoteReceivedCount(feedback);
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;

			// ���½�������Ϣ��ʾ��Ϣ
			NotifyDataModelChanged();


TRY_CHECK_AGIAN:
			// ֹͣ����ֽ��
			if(m_bRechargeConfirm){
				LOG("stop receive, Leave!");
				m_bRechargeConfirm = false;
				// ѹ��
				if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
					LOG("��ʼѺ��ֽ��,�ѽ���ֽ�ҡ�");
					m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;// Ϊ�˲��������ط�ͬ��ѹ����ͻ�������첽ѹ��ǰ���Ȱѱ�ʶ��Ϊ��ѹ��Ʊ��
					BANKNOTE_HELPER->CollectAsync(this);
					m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
				}

				// �˿�ȷ��Ͷ��ֽ�ң���ʼ���г�ֵҵ��
				if(GetServiceID()==CHARGE_SVC){
					GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ISSUING, 0, 0);
				}
				return ;
			}

			// Ͷ��ʶ��ʱȡ������ʱʱͶ�룬��ʱ��Ҫ��Ǯ�˳�(��������Ҫ�ж�)
			if(m_pCommonModel->operationInfo.finishType == PASSAGE_CANCEL || 
				m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME ||
				CANCELING == theSERVICE_MGR.GetState()){
					ReturnWhenBanknotesInserted();
					return ;
			}

			// 1. �ж��Ƿ�Ͷ������ܽ��ޣ�
			if (IsBanknoteMaxAmount()) {
				LOG("Ͷ��ֽ���ܽ����������,��Ǯ��");
				// 1.1����״̬ 
				m_pCommonModel->operationInfo.finishType = OVER_MONEY_AMOUNT_LIMIT;
				// 1.2�˱Ҳ���
				ReturnWhenBanknotesInserted();
				// ���淵�����˱һص�������ִ�С�
			}
			// 3. Ͷ�����Ƿ��㹻��
			else if (IsReceiveEnoughMoney()){
				if(GetServiceID()==CHARGE_SVC){
					m_bRechargeOverlimit = true;
					LOG("��ֵ���ս��ﵽ�ÿ����ɳ��ֹͣ���գ�Leave��");
					CString strGuide = _tl(TXT_BANKNOTE_OVERLIMIT_CARD_MAX_REVALUE);
					TCHAR* sGuide = strGuide.GetBuffer();
					GetCurReceptionDlg()->PostMessage(WM_UPDATE_RECEPITON_GUIDE, (WPARAM)sGuide, 0);
					NotifyDataModelChanged();
					return;
				}		

				// 3.1�ж��Ƿ���Ҫ���㣿
				if (m_pCommonModel->paymentInfo.lChangeAmount >0){
					// 3.2�ж��Ƿ�֧�����㣿
					if (theTVM_STATUS_MGR.GetChangeMode() != DEVICE_CONTRL_CODE_NO_CHANGE){
						// 3.3�����Ƿ��㹻?
						if (CalculateChangeCount()){
							if(m_pCommonModel->operationInfo.finishType == OVER_CHANGE_COUNT_LIMIT){
								LOG("�����������ޡ�");
								// 3.4.2�˻���������Ǯ
								ReturnWhenBanknotesInserted();
							}
							else{
								// 3.4���������Ƿ��ޣ�
								// 3.5�������Ƿ��ޣ�(Ԥ��)
								//if(!IsCoinChageMaxAmount()){}
								if (!IsCoinChageMaxCount()){// �����Ѿ�û�����ˣ���Ϊ�ڼ�������ʱ���Ѿ��жϹ��ˡ�
									// �������
									LOG("ֽ�ҽ������,��Ҫ���㡣");
									// ֹͣʶ��ֽ��
									if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
										m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
										BANKNOTE_HELPER->StopValidation();
									}
									// ֹͣʶ��Ӳ��
									if (m_pCommonModel->operationInfo.isCoinSlotOpen){
										m_pCommonModel->operationInfo.isCoinSlotOpen = false;
										COIN_HELPER->CH_StopInsertInfo();
										COIN_HELPER->CH_EndReceive();
									}
									// ���ý��ճɹ�
									OnAcceptMoneyOK();
								}
								// 3.4������������
								else{
									LOG("�����������ޡ�");
									// 3.4.1�������״̬
									m_pCommonModel->operationInfo.finishType = OVER_CHANGE_COUNT_LIMIT;

									// 3.4.2�˻���������Ǯ
									ReturnWhenBanknotesInserted();
								}
							}
						}
						// 3.3���㲻��
						else{
							LOG("���㲻�㡣");
							// 3.3.1�������״̬
							m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;

							// 3.3.2�˻���������Ǯ
							ReturnWhenBanknotesInserted();
						}
					}
					// 3.2��֧������,��Ϊ���㲻��,��ʾ������
					else{
						LOG("���㲻����ߵ�ǰģʽ��֧�����㡣");
						// 3.2.1�������״̬
						m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;
						//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);//ͨ�ýӿڣ����㲻�㡣

						// 3.2.2�˻���������Ǯ
						ReturnWhenBanknotesInserted();
					}
				}
				// 3.1����Ҫ����,ֱ�ӽ�����һ��
				else{
					LOG("ֽ�ҽ������,�Ҳ���Ҫ���㡣");
					// ֹͣʶ��ֽ��
					if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
						m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
						//BANKNOTE_HELPER->StopValidation();
					}
					// ֹͣʶ��Ӳ��
					if (m_pCommonModel->operationInfo.isCoinSlotOpen){
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;
						COIN_HELPER->CH_StopInsertInfo();
						//COIN_HELPER->CH_EndReceive();
					}
					// ���ý��ճɹ�
					OnAcceptMoneyOK();
				}
			}
			// 2. �ж��Ƿ�Ӳ�Һ�ֽ�Ҷ��ﵽ���������
			else if(IsBothCoinBanknoteMaxCount()){
				LOG("Ӳ�Ҽ�ֽ�������ﵽ�������,��Ǯ��");
				// 2.1���÷���״̬
				m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
				//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);// Need change,when it's ok!!
				// 2.2�˱Ҳ���
				ReturnWhenBanknotesInserted();
				// ���淵�����˱һص�������ִ�С�
			}
			// 4. Ͷ�������Ƿ��ޣ�
			else if(IsBanknoteMaxCount()){
				// 4.1 ���Ӳ�ҿڴ�,���ڽ���...
				if (m_pCommonModel->operationInfo.isCoinSlotOpen)
				{
					LOG("ֹͣʶ��ֽ��,�ȴ�Ӳ��Ͷ�롣");
					// ֹͣʶ��ֽ��
					BANKNOTE_HELPER->StopValidation();
					m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
					m_pCommonModel->operationInfo.isBanknotesMaxCount = true;
					CString strGuide = _tl(TXT_BANKNOTE_MAX_INSERT_COIN);
					TCHAR* sGuide = strGuide.GetBuffer();
					GetCurReceptionDlg()->PostMessage(WM_UPDATE_RECEPITON_GUIDE, (WPARAM)sGuide, 0);
					NotifyDataModelChanged();
					// ��Ҫ������ʱ��ʱ��
					InsertMoneyNeedStartTimer();
					// ��ʾͶ��ֽ������������ʾ
					NotifyDataModelChanged();
				}
				// 4.2 Ӳ��û�н���,�����������,��Ǯ��
				else {
					LOG("Ӳ��û��Ͷ��,ֽ��Ͷ����������,�˱ҡ�");
					// 4.2.1���÷���״̬
					m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
					// 4.2.2�˱Ҳ���
					ReturnWhenBanknotesInserted();			
					// ���淵�����˱һص�������ִ�С�
				}				
			}
			// 5. ������Ǯ
			else{
				LOG("Ͷ�����,����Ͷ�롣");
				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
				tBncSetInhibitList table;
				GetAcceptBanknotesFace(&table);
				BANKNOTE_HELPER->OpenAndValidateAsync(this,table);
				if (table.psBncCashInfo)
				{
					delete[] table.psBncCashInfo;
					table.psBncCashInfo = NULL;
				}
				InsertMoneyNeedStartTimer();
			}
		}
		//20161229��lichao: ���ݴ����룬����ͬ�Ĵ���������ע�͡�
		// ��. ����ֶ���ֹ���簴��ȡ�������쳣�����ޡ��������ޡ����㳬�ޡ�Ͷ�볬ʱ��ǮͶ���˵Ȳ�����
		//else if(resultCode == BH_RESPONSE_CODE::BH_WORK_CANCLED/*& DONE_PHASE_ACT_ABORT*/){
		else if(resultCode == CBRHHardwareException::ERR_CANCELLED/*& DONE_PHASE_ACT_ABORT*/){
			LOG("�ص�ֹֽͣ�Ҷ�ȡ��");
			// 1.�Զ��˳�ʶ��ģʽ(Ӳ��Ͷ��,�ֶ�ȡ��ʱ�����˻ص�)ֹͣʶ��ֽ��	
			if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
				m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
				BANKNOTE_HELPER->StopValidation();
			}

			//BH_STATUS bh_st;
			//BANKNOTE_HELPER->GetStatus(bh_st);
			// 2.ȡ������,��ִ����Ǯ,Ӳ�Ҳ����ص������˺�����
			if(m_pCommonModel->operationInfo.finishType == PASSAGE_CANCEL          || 
				m_pCommonModel->operationInfo.finishType == SERVICE_ERROR_CANCEL    ||
				m_pCommonModel->operationInfo.finishType == OVER_MONEY_COUNT_LIMIT  ||
				m_pCommonModel->operationInfo.finishType == OVER_CHANGE_COUNT_LIMIT ||
				m_pCommonModel->operationInfo.finishType == OVER_CHANGE_AMOUNT_LIMIT||
				m_pCommonModel->operationInfo.finishType == OVER_MONEY_AMOUNT_LIMIT ||
				m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME	   ||
				m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME	   ||
				m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR && !theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)
				){
					// ���н��յ�ֽ����δѹ��ʱֽ����Ǯ
					if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
						LOG("�ص���Ǯ��");
						EndInsertTimer();
						if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID))
						{
							m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
							BANKNOTE_HELPER->ReturnAllAsync(this);
						}
					}
					if (m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_INSERTED/* || bh_st.bh_sts.cnt >0*/) {
						// ��ʱ��Ҫ�ٶ�һ��״̬������ǳ�ʱ��ȡ��
						if(m_pCommonModel->operationInfo.finishType == PASSAGE_CANCEL		|| // �˿�ȡ��ʱֽ�ҽ���
							m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME || // �˿Ͳ�����ʱ
							m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME	   // Ͷ�볬ʱ
							){
								//try{ // xuzan 
								//	// lichao: ��ѯ�ݴ����Ƿ��г�Ʊ
								//	//BRH_RSP_GET_STATUS status;
								//	tBncDevReturn status;
								//	BANKNOTE_HELPER->GetStatus(status);
								//	if (status.intermediateStackerStatus!=0){
								//		LOG("ֽ��ģ����Ȼ��ֽ�ҡ�");
								//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;
								//		// �˻�ֽ��(�������Ǯ)
								//		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
								//		BANKNOTE_HELPER->ReturnAllAsync(this);
								//	}
								//}
								//catch(CSysException& e){
								//	theEXCEPTION_MGR.ProcessException(e);
								//}
								//catch(...){
								//	LOG("ֽ��ģ���ȡ״̬�����쳣��");
								//}
						}
						OnReturnFinish();
					}
			}
			else if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR){
				OnReturnFinish();
			}
			else if(m_pCommonModel->operationInfo.isFinishedReceveMoney){// ����յ��㹻���Ǯ�ˣ���ʼ��Ʊ
				// ��Ҫ�ж�һ���Ƿ��ݴ��������ٽ����Ͷ���ֽ��
				try{
					LOG("errorType==ERR_CANCELLED && isFinishedReceveMoney=true");
					// lichao: ������ע��
					//BRH_STATUS bh_st;
					//BANKNOTE_HELPER->GetStatus(bh_st);

					//if(bh_st.bh_sts.validBnCount > 0){
					//	LOG("ֽ��ģ����ֽ�ң�%d�š�",bh_st.bh_sts.validBnCount);
					//	LOG("�豸��ȷ��ֽ�ң�%d�š�",m_pCommonModel->paymentInfo.lBHReceiveTotalCount);

					//	m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;

					//	// ��ǰͶ���Ǯ��ʵ�ʲ����
					//	if(m_pCommonModel->paymentInfo.lBHReceiveTotalCount != bh_st.bh_sts.validBnCount){
					//		LOG("ֽ��Ͷ��������ʵ�ʼ�¼�����ϣ�����ȡ��");
					//		m_pCommonModel->operationInfo.finishType = PASSAGE_CANCEL;
					//		ReturnWhenBanknotesInserted();
					//		return;
					//	}
					//}
				}
				catch(CSysException& e){
					theEXCEPTION_MGR.ProcessException(e);
				}
				catch(...){
					LOG("ֽ��ģ���ȡ״̬�����쳣��");
				}
				OnAcceptMoneyOK();
			}
		}
		// Ͷ��ʱȫ���˳�
		//else if(resultCode == CBRHException::BH_RSP_RETURN_MONEY_SUCCESS){
		//	m_pCommonModel->operationInfo.isFinishedReceveMoney = false;

		//	LOG("Ͷ����������Ҫ��ȫ���˻��������ȴ����½���ֽ�һ��߷�����ɽ��档");
		//	if(m_pCommonModel->operationInfo.finishType != PASSAGE_CANCEL && 
		//		m_pCommonModel->operationInfo.finishType != INSERT_OUT_OF_TIME &&
		//		m_pCommonModel->operationInfo.finishType != EXCEPTION_ERROR && 
		//		!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
		//		m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//		m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//		m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;
		//		
		//		NotifyDataModelChanged();// ����ˢ��

		//		BANKNOTE_HELPER->OpenAndValidateAsync(this,GetAcceptBanknotesFace());
		//		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//		InsertMoneyNeedStartTimer();
		//	}
		//	else{
		//		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//		try{
		//			BRH_STATUS bh_st;
		//			BANKNOTE_HELPER->GetStatus(bh_st);

		//			if(bh_st.bh_sts.validBnCount > 0){
		//				LOG("ֽ��ģ����Ȼ��ֽ�ң�%d�š�",bh_st.bh_sts.validBnCount);
		//				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;
		//				// �˻�ֽ��(�������Ǯ)
		//				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//				BANKNOTE_HELPER->ReturnAllAsync(this);
		//			}
		//			else{
		//				m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//				m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//				m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//				NotifyDataModelChanged();// ����ˢ��
		//			}
		//		}
		//		catch(CSysException& e){
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//			m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//			NotifyDataModelChanged();// ����ˢ��
		//			theEXCEPTION_MGR.ProcessException(e);
		//		}
		//		catch(...){
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//			m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//			NotifyDataModelChanged();// ����ˢ��
		//			LOG("ֽ��ģ���ȡ״̬�����쳣��");
		//		}
		//		OnReturnFinish();
		//	}
		//}
		//else if(resultCode == CBRHException::BH_RSP_RETURN_ONE_SUCCESS){
		//	LOG("Ͷ�������Ҫ������һ�ţ������ȴ�����ֽ�ҡ�");
		//	if(!m_pCommonModel->operationInfo.isFinishedReceveMoney){
		//		if(m_pCommonModel->operationInfo.finishType != PASSAGE_CANCEL  && 
		//			m_pCommonModel->operationInfo.finishType != INSERT_OUT_OF_TIME &&
		//			m_pCommonModel->operationInfo.finishType != EXCEPTION_ERROR && 
		//			!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
		//			BANKNOTE_HELPER->OpenAndValidateAsync(this,GetAcceptBanknotesFace());
		//			m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//			InsertMoneyNeedStartTimer();
		//		}
		//		else{
		//			m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//			// �����Ͷ��Ǯ����Ҫ��Ǯ�˸��˿�
		//			if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){
		//				// �˻�ֽ��(�������Ǯ)
		//				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//				BANKNOTE_HELPER->ReturnAllAsync(this);
		//			}
		//			else{
		//				OnReturnFinish();
		//			}
		//		}
		//	}
		//	else{
		//		LOG("Ͷ����ɺ���Ͷ�벻��ʶ��Ľ�");
		//		OnAcceptMoneyOK();
		//	}
		//}
		//// ֽ��Ͷ��ȡ�������Ѿ�ʶ��
		//else if(resultCode == BH_RESPONSE_CODE::BH_INSTERT_CANCLED){
		//	LOG("ֽ��Ͷ��ʱȡ��������Ѿ�ʶ����Ǯ��");
		//	// ����Ͷ���ܶ�
		//	BRH_CMD_FEEDBACK feedback;
		//	bhReadReceiveAmount->GetResponse(&feedback);

		//	RenewBanknoteReceivedAmount(feedback);
		//	RenewBanknoteReceivedCount(feedback);
		//	m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;
		//	NotifyDataModelChanged();

		//	if(!m_pCommonModel->operationInfo.isFinishedReceveMoney){
		//		// ��Ǯ
		//		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//		EndInsertTimer();
		//		BANKNOTE_HELPER->ReturnAllAsync(this);
		//		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//	}
		//	else{
		//		goto TRY_CHECK_AGIAN;
		//	}
		//}
		//// Ǯ���ȥ������
		//else if(resultCode == BH_RESPONSE_CODE::BH_BANKNOTE_TAKEN_AWAY){
		//	LOG("Ǯ��׼������ʱ���ߣ������ȴ�Ͷ�롣");
		//	if(!m_pCommonModel->operationInfo.isFinishedReceveMoney){
		//		if(m_pCommonModel->operationInfo.finishType != PASSAGE_CANCEL  && 
		//			m_pCommonModel->operationInfo.finishType != INSERT_OUT_OF_TIME &&
		//			m_pCommonModel->operationInfo.finishType != EXCEPTION_ERROR && 
		//			!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
		//			BANKNOTE_HELPER->OpenAndValidateAsync(this,GetAcceptBanknotesFace());
		//			m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//			InsertMoneyNeedStartTimer();
		//		}
		//		else{
		//			m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//			// �����Ͷ��Ǯ����Ҫ��Ǯ�˸��˿�
		//			if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){
		//				// �˻�ֽ��(�������Ǯ)
		//				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//				BANKNOTE_HELPER->ReturnAllAsync(this);
		//			}
		//			else{
		//				OnReturnFinish();
		//			}
		//		}
		//	}
		//	else{
		//		OnAcceptMoneyOK();
		//	}
		//}
		//// �����޷�ִ�У�ֽ��ģ�����쳣ʱ
		//else if(resultCode == BH_RESPONSE_CODE::BH_WORK_INVALID){
		//	// Ͷ��ʱӲ���쳣
		//	LOG("Ӳ���쳣��������ִ�е������״̬");
		//	throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_WORK_INVALID,_T(__FILE__),__LINE__);
		//}
		//// �����ж��Ƿ��ǿ���Ʊ��
		//else if(resultCode == ERR_BH_INSERT_JAM || resultCode == ERR_BH_JAM){
		//	// �жϵ�ǰ�Ƿ���Ľ��յ�ֽ��
		//	if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){// �ѽ��ܣ�Ҫ�����ϵ���Ǯ
		//		throw CBRHHardwareException(resultCode,_T(__FILE__),__LINE__);
		//	}
		//	else{
		//		// д���ϵ��쳣����
		//		theEXCEPTION_MGR.ProcessException(CBRHHardwareException(resultCode,_T(__FILE__),__LINE__));		
		//		OnReturnFinish();
		//	}
		//}
		//// ֽ�Ҵ���ʱ���ֹ���(�����˳�����)
		//else if(resultCode == BH_RESPONSE_CODE::BH_HARDWARE_EXCEPTION){

		//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.Invoke();
		//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);

		//	long lRetRepare = BANKNOTE_HELPER->TryRecoveryBanknoteModule();

		//	if(lRetRepare == 0){
		//		LOG("ֽ���˳��ɹ���");
		//	}
		//	// ��ʱ���ֹ��ϣ���Ҫ��ֽ�ң��������Իָ��ķ�ʽ�˱�,���Թ��ϵ�����Ҫ����
		//	//m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//	//m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//	//m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//	m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//	m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		//	// ���쳣�Ǽ����쳣�����������Ӳ��Ͷ����Ҫ��Ӳ��
		//	throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
		//}
		else 
			throw CBRHHardwareException(resultCode,_T(__FILE__),__LINE__);
	}
	catch (CSysException& e){
		// ������쳣,ִ���쳣����
		LOG("ֽ�ҿ����쳣��");
		theTVM_STATUS_MGR.SetBanknoteModuleStatus(HARDWARE_MODULE_JAM);
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}		
}
//////////////////////////////////////////////////////////////////////////
/**
@brief		��ֽ��Ͷ���������ܶ��㹻��ʱ,�˻�����ֽ�Ҽ�Ӳ��  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::ReturnWhenBanknotesInserted(){
	// ֹͣʶ��ֽ��
	if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		BANKNOTE_HELPER->StopValidation();
	}

	if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		BANKNOTE_HELPER->ReturnAllAsync(this);
	}

	// �˻�Ӳ��,��������ˡ�
	if (m_pCommonModel->operationInfo.isCoinSlotOpen){
		m_pCommonModel->operationInfo.isCoinSlotOpen = false;
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
	}
	// Ͷ�������ﵽ���Ҫ������,�����ﴦ���˱�
	if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
		m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		COIN_HELPER->CH_RefundAsync(this);
	}

	OnReturnFinish();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ��ѹ������ʵ�ֺ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnBanknoteCollect(UINT res,CBRHCashInEnd* pCashIntoInfo)
{
	try
	{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_BUSY;

		// һ�����ճɹ�
		if (res==0){
			LOG("Ѻ���ɹ�(ֽ��)��");
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;


			tBncEnCashInfo cashIntoInfo;
			pCashIntoInfo->GetBRHResponse(&cashIntoInfo);

			// ������
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO collBoxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			LOG("-------------------------ֽ�һ���ǰ--------------------------");
			LOG("������1Ԫ������%d",	collBoxInfo.ulCount_CNY1);
			LOG("������5Ԫ������%d",	collBoxInfo.ulCount_CNY5);
			LOG("������10Ԫ������%d",	collBoxInfo.ulCount_CNY10);
			LOG("������20Ԫ������%d",	collBoxInfo.ulCount_CNY20);
			LOG("������50Ԫ������%d",	collBoxInfo.ulCount_CNY50);
			LOG("������100Ԫ������%d",	collBoxInfo.ulCount_CNY100);

			collBoxInfo.ulCount_CNY1 +=   cashIntoInfo.wCashBox_CNY1_IntoCount;
			collBoxInfo.ulCount_CNY5 +=   cashIntoInfo.wCashBox_CNY5_IntoCount;
			collBoxInfo.ulCount_CNY10 +=  cashIntoInfo.wCashBox_CNY10_IntoCount;
			collBoxInfo.ulCount_CNY20 +=  cashIntoInfo.wCashBox_CNY20_IntoCount;
			collBoxInfo.ulCount_CNY50 +=  cashIntoInfo.wCashBox_CNY50_IntoCount;
			collBoxInfo.ulCount_CNY100 += cashIntoInfo.wCashBox_CNY100_IntoCount;

			LOG("-------------------------ֽ�һ�������--------------------------");
			LOG("����1Ԫ������%d",	cashIntoInfo.wCashBox_CNY1_IntoCount);
			LOG("����5Ԫ������%d",	cashIntoInfo.wCashBox_CNY5_IntoCount);
			LOG("����10Ԫ������%d",	cashIntoInfo.wCashBox_CNY10_IntoCount);
			LOG("����20Ԫ������%d",	cashIntoInfo.wCashBox_CNY20_IntoCount);
			LOG("����50Ԫ������%d",	cashIntoInfo.wCashBox_CNY50_IntoCount);
			LOG("����100Ԫ������%d",cashIntoInfo.wCashBox_CNY100_IntoCount);

			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(collBoxInfo)) {
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
			}


			// ѭ����
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

			LOG("-------------------------ֽ�һ���ǰѭ��������--------------------------");
			LOG("ѭ����A������%d",	recyclerBoxA.ulCurCount);
			LOG("ѭ����B������%d",	recyclerBoxB.ulCurCount);
			LOG("ѭ����C������%d",	recyclerBoxC.ulCurCount);
			LOG("ѭ����D������%d",	recyclerBoxD.ulCurCount);

			LOG("-------------------------ֽ�һ��ս���ѭ��������--------------------------");
			LOG("ѭ����A������%d",	cashIntoInfo.wRecyclerAIntoCount);
			LOG("ѭ����B������%d",	cashIntoInfo.wRecyclerBIntoCount);
			LOG("ѭ����C������%d",	cashIntoInfo.wRecyclerCIntoCount);
			LOG("ѭ����D������%d",	cashIntoInfo.wRecyclerDIntoCount);


			if(cashIntoInfo.wRecyclerAIntoCount > 0){
				recyclerBoxA.ulCurCount += cashIntoInfo.wRecyclerAIntoCount;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(recyclerBoxA);
			}

			if(cashIntoInfo.wRecyclerBIntoCount > 0){
				recyclerBoxB.ulCurCount += cashIntoInfo.wRecyclerBIntoCount;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(recyclerBoxB);
			}
			if(cashIntoInfo.wRecyclerCIntoCount > 0){
				recyclerBoxC.ulCurCount += cashIntoInfo.wRecyclerCIntoCount;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(recyclerBoxC);
			}
			if(cashIntoInfo.wRecyclerDIntoCount > 0){
				recyclerBoxD.ulCurCount += cashIntoInfo.wRecyclerDIntoCount;
				theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(recyclerBoxD);
			}

			// 1.��ֽ��Ǯ��ļ�����Ϣ�ļ�����Ϣ,����,������д�롣
			/*CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			boxInfo.ulCount_1 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote1Yuan];
			boxInfo.ulCount_5 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote5Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote10Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote20Yuan];
			boxInfo.ulCount_50 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote50Yuan];
			boxInfo.ulCount_100 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote100Yuan];
			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
			}*/

			//if(m_pCommonModel->lDeviceStatus == 0 && m_pCommonModel->operationInfo.mapException.size() == 0){
			//	theSERVICE_MGR.SetState(FINISH);
			//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
			//}

			OnReturnFinish();
		}
		// ��������ʧ��
		else{
			LOG("Ѻ��ʧ��(ֽ��)��");
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED;
			// Ѻ��ʧ�������쳣���ʱ���Ὣ��ѹ������䣬������Ҫ����
			// Need edit zhengxianle
			/*CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			boxInfo.ulCount_1 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote1Yuan];
			boxInfo.ulCount_5 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote5Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote10Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote20Yuan];
			boxInfo.ulCount_50 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote50Yuan];
			boxInfo.ulCount_100 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote100Yuan];
			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
			}*/

			throw CBRHHardwareException(res,_T(__FILE__),__LINE__);
		}
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ���˱�   

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnBanknoteReturned(UINT res,CBRHCashInRollback* pBHRef)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_BUSY;
		theSERVICE_MGR.SetForeServiceBusy(false);
		// 1.�˱ҳɹ�
		if (res == 0){
			LOG("ֽ���˱�����ִ�гɹ���(�ص�)");
			//BRH_CMD_FEEDBACK refCallBack;
			//long lRet = pBHRef->GetResponse(&refCallBack);

			/*if(lRet == BHTRS_RES_OK)*/{
				// 1.1���ý���״̬
				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURNED;
				m_pCommonModel->paymentInfo.lReturnAmount += m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
				//m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();
				// 1.2 ת������
				OnReturnFinish();
			}
			//else{

			//	// ���ʧ�ܣ������˳���Ǯ�������Ǯ��ȣ����ʱ��һ�����������������Ҫ����ϵ�
			//	if(lRet == BH_RESPONSE_CODE::BH_HARDWARE_EXCEPTION || refCallBack.nCount == m_pCommonModel->paymentInfo.lBHReceiveTotalCount){
			//		//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.Invoke();
			//		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURNED;
			//		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
			//		//throw  CBHHardwareException(CBHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
			//		theEXCEPTION_MGR.ProcessException(CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__));
			//		// ���÷��ؽ���
			//		OnReturnFinish();
			//	}
			//	else{
			//		// ������˱�ʧ�ܵĻ���Ҳ��Ҫ����ε����������ļ���
			//		CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			//		boxInfo.ulCount_1 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote1Yuan];
			//		boxInfo.ulCount_5 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote5Yuan];
			//		boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote10Yuan];
			//		boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote20Yuan];
			//		boxInfo.ulCount_50 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote50Yuan];
			//		boxInfo.ulCount_100 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote100Yuan];
			//		if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
			//			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ��Ǯ��ļ�����Ϣд��ʧ��"));
			//		}

			//		if(lRet == BH_FAIL){// 1.3ʧ��
			//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURN_FAILED;
			//			throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
			//		}
			//		else if(lRet == BH_WORK_INVALID){// ����ִ�е�����
			//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURN_FAILED;
			//			throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_WORK_INVALID,_T(__FILE__),__LINE__);
			//		}
			//		else{
			//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURN_FAILED;
			//			throw CBRHHardwareException(lRet,_T(__FILE__),__LINE__);
			//		}
			//	}
			//}
		}
		// 2.�˱�ʧ��
		else{
			// �˱�ʧ����Զ�������ӡ���ϵ����ⲿ��Ǯ���˸��˿�
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;
			//throw  CBHHardwareException(CBHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
			//theEXCEPTION_MGR.ProcessException(CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__));
			// ���÷��ؽ���
			OnReturnFinish();

			/*
			LOG("ֽ���˱�ʧ�ܡ�(�ص�),��Ҫ��ӡƾ����");
			// 2.1����״̬
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURN_FAILED;
			throw CBHHardwareException(res,_T(__FILE__),__LINE__);*/
		}
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ������ص�����

@param      UINT nResult;����ֵ
CBHChangeSendOutBill* bhChangeSendOut;���㷵��ָ�����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnBanknoteChangeSendOut(UINT nResult,CBRHDispenseRequest* bhChangeSendOut){
	try{

		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_CHANGE_BUSY;
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO changeBox	 = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo();

		if(0 == nResult){
			LOG("ֽ������ɹ�");
			m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;

			LOG("-------------------------ֽ������ǰ--------------------------");
			LOG("ѭ����A����ǰ��%d",recyclerBoxA.ulCurCount);
			LOG("ѭ����B����ǰ��%d",recyclerBoxB.ulCurCount);
			LOG("ѭ����C����ǰ��%d",recyclerBoxC.ulCurCount);
			LOG("ѭ����D����ǰ��%d",recyclerBoxD.ulCurCount);
			LOG("����������ǰ��%d",changeBox.ulCurCount);

			CHANGE_RESULT result;
			bhChangeSendOut->GetResponse(&result);

			LOG("-------------------------ֽ��������Ϣ--------------------------");
			LOG("ѭ����A���ң�%d",result.wRecyclerAOutCount);
			LOG("ѭ����B���ң�%d",result.wRecyclerBOutCount);
			LOG("ѭ����C���ң�%d",result.wRecyclerCOutCount);
			LOG("ѭ����D���ң�%d",result.wRecyclerDOutCount);

			// 20171205��lichao: ����ֽ��ģ��Ӳ���Զ����ҡ����������Ҫ����ѭ���ĵ�ˮλʱ��ֽ�Ҽ����ĵ�����
			do{
				//map<BNR_MODULE_ID,T_XfsPhysicalCashUnit> cashUnit;
				try{
					/*				cashUnit = BANKNOTE_HELPER->GetEmptyFull();*/
					if(result.wRecyclerAOutCount >0){
						recyclerBoxA.ulCurCount -= result.wRecyclerAOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount = result.wRecyclerAOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxAAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(1)-recyclerBoxA.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxA.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("ѭ���ĵ�ˮλ%d�ţ�A�Զ�����%d�ţ������%d�ţ������䲹���%d��", theAPP_SESSION.GetBNRRecyclerNearEmpty(1), adjustCount, recyclerBoxA.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_A].count - recyclerBoxA.ulCurCount;
						//if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxA.ulCurCount = cashUnit[BNR_RECYCLER_BOX_A].count;	
						//LOG("ѭ����A�Զ�����%d�ţ������%d�ţ������䲹���%d��", adjustCount, recyclerBoxA.ulCurCount, changeBox.ulCurCount);
					}
					if(result.wRecyclerBOutCount >0){
						recyclerBoxB.ulCurCount -= result.wRecyclerBOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount = result.wRecyclerBOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxBAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(2)-recyclerBoxB.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxB.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("ѭ����B��ˮλ%d�ţ��Զ�����%d�ţ������%d�ţ������䲹���%d��", theAPP_SESSION.GetBNRRecyclerNearEmpty(2), adjustCount, recyclerBoxB.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_B].count - recyclerBoxB.ulCurCount;
						//if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxB.ulCurCount = cashUnit[BNR_RECYCLER_BOX_B].count;
						//LOG("ѭ����B�Զ�����%d�ţ������%d�ţ������䲹���%d��", adjustCount, recyclerBoxB.ulCurCount, changeBox.ulCurCount);
					}
					if(result.wRecyclerCOutCount >0){
						recyclerBoxC.ulCurCount -= result.wRecyclerCOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount = result.wRecyclerCOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxCAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(3)-recyclerBoxC.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxC.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("ѭ����C��ˮλ%d�ţ��Զ�����%d�ţ������%d�ţ������䲹���%d��", theAPP_SESSION.GetBNRRecyclerNearEmpty(3), adjustCount, recyclerBoxC.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_C].count - recyclerBoxC.ulCurCount;
						//if(adjustCount>0){//ʵ�ʱ�������󣬵�������
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxC.ulCurCount = cashUnit[BNR_RECYCLER_BOX_C].count;
						//LOG("ѭ����C�Զ�����%d�ţ������%d�ţ������䲹���%d��", adjustCount, recyclerBoxC.ulCurCount, changeBox.ulCurCount);
					}
					if(result.wRecyclerDOutCount >0){
						recyclerBoxD.ulCurCount -= result.wRecyclerDOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount = result.wRecyclerDOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxDAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(4)-recyclerBoxD.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxD.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("ѭ����D��ˮλ%d�ţ��Զ�����%d�ţ������%d�ţ������䲹���%d��", theAPP_SESSION.GetBNRRecyclerNearEmpty(4), adjustCount, recyclerBoxD.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_D].count - recyclerBoxD.ulCurCount;
						//if(adjustCount>0){//ʵ�ʱ�������󣬵�����
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxD.ulCurCount = cashUnit[BNR_RECYCLER_BOX_D].count;
						//LOG("ѭ����D�Զ�����%d�ţ������%d�ţ������䲹���%d��", adjustCount, recyclerBoxD.ulCurCount, changeBox.ulCurCount);
					}
					break;
				}
				catch(...){

				}
			}while(0);

			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(recyclerBoxA);
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(recyclerBoxB);
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(recyclerBoxC);
			theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(recyclerBoxD);
			theBANKNOTE_COUNT.SetBanknoteChangeBoxInfo(changeBox);

			OnReturnFinish();
		}
		else{
			m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
			m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = m_pCommonModel->paymentInfo.dwChangeBHAmout;
			//throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
			throw CBRHHardwareException(nResult,_T(__FILE__),__LINE__);
		}
		//m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_CHANGE_BUSY;
		//
		//BANKNOTE_CHANGE_DETAIL bankChangeInfo;
		//CTBanknoteCountInfo::BANKNOTE_CHANGE_BOX_INFO banknoteAInfo,banknoteBInfo,banknoteInvalidInfo;
		//banknoteAInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxAInfo();
		//banknoteBInfo = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo();
		//banknoteInvalidInfo = theBANKNOTE_COUNT.GetBanknoteInvalidBoxInfo();


		//// ֽ������ɹ�
		//if(0 == nResult){
		//	LOG("ֽ������ɹ���");
		//	m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;
		//	// ͬ������
		//	
		//	LOG("-------------------------ֽ������ǰ--------------------------");
		//	LOG("ֽ����A����ǰ��%d",banknoteAInfo.ulCurCount);
		//	LOG("ֽ����B����ǰ��%d",banknoteBInfo.ulCurCount);
		//	LOG("ֽ����ϱ�������ǰ��%d",banknoteInvalidInfo.ulCurCount);

		//	bhChangeSendOut->GetChangeResponse(&bankChangeInfo);

		//	LOG("-------------------------ֽ��������Ϣ--------------------------");
		//	LOG("ֽ����A���ң�%d",bankChangeInfo.dwBoxASendOutCount);
		//	LOG("ֽ����B���ң�%d",bankChangeInfo.dwBoxBSendOutCount);

		//	LOG("ֽ����A���ϱ��䣺%d��",bankChangeInfo.dwBoxARejectionCount);
		//	LOG("ֽ����B���ϱ��䣺%d��",bankChangeInfo.dwBoxBRejectionCount);
		//	//banknoteAInfo.ulCurCount -= m_pCommonModel->paymentInfo.dwChangeBHBoxACount;
		//	//banknoteBInfo.ulCurCount -= m_pCommonModel->paymentInfo.dwChangeBHBoxBCount;
		//	
		//	// �ж�ʵ������
		//	if(bankChangeInfo.dwBoxASendOutCount > 0){
		//		banknoteAInfo.ulCurCount -= bankChangeInfo.dwBoxASendOutCount;
		//		theBANKNOTE_COUNT.SetBanknoteChangeBoxAInfo(banknoteAInfo);
		//		theDeviceRegAR.AddBhcBoxAChangeCount(bankChangeInfo.dwBoxASendOutCount);
		//	}

		//	if(bankChangeInfo.dwBoxBSendOutCount > 0){
		//		banknoteBInfo.ulCurCount -= bankChangeInfo.dwBoxBSendOutCount;
		//		theBANKNOTE_COUNT.SetBanknoteChangeBoxBInfo(banknoteBInfo);
		//		theDeviceRegAR.AddBhcBoxBChangeCount(bankChangeInfo.dwBoxBSendOutCount);
		//	}

		//	if(bankChangeInfo.dwBoxARejectionCount > 0 || bankChangeInfo.dwBoxBRejectionCount > 0){
		//		banknoteInvalidInfo.ulCurCount += bankChangeInfo.dwBoxARejectionCount + bankChangeInfo.dwBoxBRejectionCount;
		//		theBANKNOTE_COUNT.SetBanknoteInvalidBoxInfo(banknoteInvalidInfo);
		//		if(bankChangeInfo.dwBoxARejectionCount > 0)
		//			theDeviceRegAR.AddBhcBoxAChangeCount(bankChangeInfo.dwBoxARejectionCount);
		//		else
		//			theDeviceRegAR.AddBhcBoxBChangeCount(bankChangeInfo.dwBoxBRejectionCount);
		//	}

		//	OnReturnFinish();
		//}
		//// ֽ������ʧ��
		//else{

		//	// ����Ƿ��ͻ��߽��մ���ֱ�Ӵ�ӡ���ϵ�
		//	if(nResult == CBHChangeException::ERROR_BH_SEND || nResult == CBHChangeException::ERROR_BH_RECEIVE){
		//		m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//		m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//		throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//	}
		//	else{
		//		LOG("-------------------------ֽ������ǰ--------------------------");
		//		LOG("ֽ����A����ǰ��%d",banknoteAInfo.ulCurCount);
		//		LOG("ֽ����B����ǰ��%d",banknoteBInfo.ulCurCount);
		//		LOG("ֽ����ϱ�������ǰ��%d",banknoteInvalidInfo.ulCurCount);

		//		// ȡһ��״̬��ͬ��һ�¼���
		//		bhChangeSendOut->GetChangeResponse(&bankChangeInfo);

		//		LOG("-------------------------ֽ��������Ϣ--------------------------");
		//		LOG("ֽ����A���ң�%d",bankChangeInfo.dwBoxASendOutCount);
		//		LOG("ֽ����B���ң�%d",bankChangeInfo.dwBoxBSendOutCount);

		//		LOG("ֽ����A���ϱ��䣺%d��",bankChangeInfo.dwBoxARejectionCount);
		//		LOG("ֽ����B���ϱ��䣺%d��",bankChangeInfo.dwBoxBRejectionCount);

		//		// �ж�ʵ������
		//		if(bankChangeInfo.dwBoxASendOutCount > 0){
		//			banknoteAInfo.ulCurCount -= bankChangeInfo.dwBoxASendOutCount;
		//			theBANKNOTE_COUNT.SetBanknoteChangeBoxAInfo(banknoteAInfo);
		//			theDeviceRegAR.AddBhcBoxAChangeCount(bankChangeInfo.dwBoxASendOutCount);
		//		}

		//		if(bankChangeInfo.dwBoxBSendOutCount > 0){
		//			banknoteBInfo.ulCurCount -= bankChangeInfo.dwBoxBSendOutCount;
		//			theBANKNOTE_COUNT.SetBanknoteChangeBoxBInfo(banknoteBInfo);
		//			theDeviceRegAR.AddBhcBoxBChangeCount(bankChangeInfo.dwBoxBSendOutCount);
		//		}

		//		if(bankChangeInfo.dwBoxARejectionCount > 0 || bankChangeInfo.dwBoxBRejectionCount > 0){
		//			banknoteInvalidInfo.ulCurCount += bankChangeInfo.dwBoxARejectionCount + bankChangeInfo.dwBoxBRejectionCount;
		//			theBANKNOTE_COUNT.SetBanknoteInvalidBoxInfo(banknoteInvalidInfo);
		//			if(bankChangeInfo.dwBoxARejectionCount > 0)
		//				theDeviceRegAR.AddBhcBoxAChangeCount(bankChangeInfo.dwBoxARejectionCount);
		//			else
		//				theDeviceRegAR.AddBhcBoxBChangeCount(bankChangeInfo.dwBoxBRejectionCount);
		//		}

		//		if(bankChangeInfo.dwBoxARejectionCount > 0){
		//			banknoteAInfo.ulCurCount -= bankChangeInfo.dwBoxARejectionCount;
		//			theBANKNOTE_COUNT.SetBanknoteChangeBoxAInfo(banknoteAInfo);
		//		}

		//		if(bankChangeInfo.dwBoxBRejectionCount > 0){
		//			banknoteBInfo.ulCurCount -= bankChangeInfo.dwBoxBRejectionCount;
		//			theBANKNOTE_COUNT.SetBanknoteChangeBoxBInfo(banknoteBInfo);
		//		}

		//		DWORD dwDisBoxACount = m_pCommonModel->paymentInfo.dwChangeBHBoxACount - bankChangeInfo.dwBoxASendOutCount;if(dwDisBoxACount > m_pCommonModel->paymentInfo.dwChangeBHBoxACount) dwDisBoxACount = 0;
		//		DWORD dwDisBoxBCount = m_pCommonModel->paymentInfo.dwChangeBHBoxBCount - bankChangeInfo.dwBoxBSendOutCount;if(dwDisBoxBCount > m_pCommonModel->paymentInfo.dwChangeBHBoxBCount) dwDisBoxBCount = 0;

		//		// ʣ��û���ͳ�������
		//		DWORD dwLastUnSendOut = (dwDisBoxACount > 0 ? dwDisBoxACount : 0) + (dwDisBoxBCount > 0 ? dwDisBoxBCount : 0);
		//		DWORD dwLastUnSendOutAmount = (dwDisBoxACount>0 ? dwDisBoxACount * TransBankNoteValue(theTVM_INFO.GetBHChangeBoxAType()) : 0) + (dwDisBoxBCount>0 ? dwDisBoxBCount * TransBankNoteValue(theTVM_INFO.GetBHChangeBoxBType()) : 0);
		//		// ���ĳ�����ӿ��ˣ��л��������㣬�����������쳣
		//		if(CBHChangeHardwareException::ERR_Empty1stCST == nResult || CBHChangeHardwareException::ERR_Empty2ndCST == nResult){
		//			// ����Ѿ��ҹ��ˣ�����Ҫ�л���������
		//			if((bankChangeInfo.dwBoxASendOutCount + bankChangeInfo.dwBoxBSendOutCount) >= (m_pCommonModel->paymentInfo.dwChangeBHBoxACount + m_pCommonModel->paymentInfo.dwChangeBHBoxBCount)){
		//				OnReturnFinish();
		//			}
		//			else{
		//				if(dwLastUnSendOut > 0){
		//					if(CBHChangeHardwareException::ERR_Empty1stCST == nResult && m_pCommonModel->paymentInfo.dwChangeBHBoxACount > 0){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox2NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteBInfo.ulCurCount){
		//							LOG("ֽ������ʧ�ܣ���%d",dwLastUnSendOutAmount);
		//							m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//							m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//							throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//						}
		//						else{// �����㹻���ٴ�B����
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxACount = 0;
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = dwLastUnSendOut;
		//							StartBHChange();
		//						}
		//					}
		//					else if(CBHChangeHardwareException::ERR_Empty2ndCST == nResult && m_pCommonModel->paymentInfo.dwChangeBHBoxBCount > 0){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteAInfo.ulCurCount){
		//							LOG("ֽ������ʧ�ܣ���%d",dwLastUnSendOutAmount);
		//							m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//							m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//							throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//						}
		//						else{
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = 0;
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxACount = dwLastUnSendOut;
		//							StartBHChange();
		//						}
		//					}
		//				}
		//				else{
		//					OnReturnFinish();
		//				}
		//			}
		//		}
		//		// ���ϱ��䣬��Ҫ����3��
		//		else if((bankChangeInfo.dwBoxARejectionCount > 0 || bankChangeInfo.dwBoxBRejectionCount > 0) && 
		//			(bankChangeInfo.dwBoxASendOutCount == 0 && bankChangeInfo.dwBoxBSendOutCount == 0) &&
		//			(m_pCommonModel->paymentInfo.dwBanknoteChangeRetryCount < 3)){
		//				m_pCommonModel->paymentInfo.dwBanknoteChangeRetryCount++;
		//				// ���ԭ�е����������밲װ״̬����,�򻹰���ԭ���ķ�ʽȥ����
		//				if((theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || banknoteAInfo.ulCurCount ==0 ) &&
		//					m_pCommonModel->paymentInfo.dwChangeBHBoxACount >0){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox2NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteBInfo.ulCurCount){
		//							LOG("ֽ������ʧ�ܣ���%d",dwLastUnSendOutAmount);
		//							m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//							m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//							throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//						}
		//						else{// �����㹻���ٴ�B����
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxACount = 0;
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = dwLastUnSendOut;
		//							StartBHChange();
		//						}
		//				}
		//				else if((theTVM_STATUS_MGR.GetBanknoteChangeBox2NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || banknoteBInfo.ulCurCount == 0) &&
		//					m_pCommonModel->paymentInfo.dwChangeBHBoxBCount >0 ){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteAInfo.ulCurCount){
		//							LOG("ֽ������ʧ�ܣ���%d",dwLastUnSendOutAmount);
		//							m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//							m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//							throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//						}
		//						else{
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = 0;
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxACount = dwLastUnSendOut;
		//							StartBHChange();
		//						}
		//				}
		//				else{
		//					StartBHChange();// ����ԭ�ȵļƻ���������һ��
		//				}
		//		}
		//		else{
		//			LOG("ֽ������ʧ�ܣ���%d",dwLastUnSendOutAmount);
		//			if(dwLastUnSendOutAmount == 0 ){
		//				m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;
		//			}
		//			else{
		//				m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//			}
		//			m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//			throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//		}
		//	}
		//}
	}
	catch(CSysException& e){
		LOG("ֽ������ʧ�ܡ�");
		OnError(e);
	}
	catch(...){
		LOG("ֽ�������쳣����������������ʧ�ܡ�");
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���п�����ʵ�ֺ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::OnBankcardInserted(UINT res,CCommand*)
//{
//	OnAcceptMoneyOK();
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���п��˳�ʵ�ֺ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::OnBankcardReturned(UINT res,CCommand*)
//{
//	
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ������̰���ʵ�ֺ���

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::OnPinPadPressed(UINT res,CTVMPinPadStartInput*)
//{
//
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����Ʊ�����ݵ�DataModel

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW����������Ϣ

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����DataModel��Ʊ������

@param      void

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::EmptyCardInfo()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ���EP֧��ʣ��Ĺ���

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW����������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::FinishEPPayment(LPRW_READ_CARD_RESPONSE pReadCardResponse){
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ȷ�Ͽ���Ϣ,����ִ�г�ֵ����

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW����������Ϣ

@retval     ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::ConfirmCard(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������

@param      (o)RW_BUSSINESS_RESPONSE&	response	������������

@retval     long	�������

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::BlockCard()
{
	UINT resultCode;
	RW_BUSSINESS_RESPONSE response;	
	try{	
		LOG("������������");
		RW_CMD_BLOCK rw_cmd_block;
		rw_cmd_block.SerialNo = m_pCommonModel->dwUDSN;
		rw_cmd_block.TranDateTime = m_pCommonModel->transTime;
		resultCode = RECHARGERW_HELPER->Block(rw_cmd_block,response);
		// �����ɹ�
		if(resultCode == RW_RSP_OK) {
			LOG("�����ɹ���");
			// ���½�����ˮ��
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			// ����UD����
			if(response.UDlen != 0) {
				RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
			}
			// ����AR����
			if(response.ARLen != 0) {
				RECHARGERW_HELPER->SaveRWARData(response.ARLen,response.ARData);
			}
		}
	}
	catch(...)	{	
		CString txtGuideInfo = (resultCode==RW_RSP_OK)?_tl(GUIDE_CHARGE_TPU_ERROR_BLACKCARD):RECHARGERW_HELPER->GetTpuErrorInfo(response.DetailErrorCode);
		OnError(txtGuideInfo, GUIDE_CHARGE_TPU_ERROR_BLACKCARD);
	}
	return resultCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ��ƷƱ��(����ֻ������TPU����Ĳ��֡��ر��������ھ���Svc�о���д��)

@param		(o)long& price Ʊ��

@retval     bool	true:�ɹ�	false:ʧ��

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::CalculatePrice() {

	// ����Ʊ��
	RW_CMD_TICKET_PRICE query;
	RW_CMD_TICKET_PRICE_RES result;
	//����˰��
	RW_CMD_TICKET_TAX taxInput;
	RW_CMD_TICKET_TAX_RES taxRsp;
	taxInput.CardType = m_pCommonModel->mediumInfo.cardPHType;

	query.CardType = m_pCommonModel->mediumInfo.cardPHType; // �����˽ӿ��ĵ�,������Ҫ����CardType_t���͡�
	query.ProductCategory = static_cast<WORD>(m_pCommonModel->productInfo.productCategory);
	query.ProductType = m_pCommonModel->productInfo.productType;
	query.ProductSubType = m_pCommonModel->productInfo.productSubType;
	if(query.ProductType == SJT){
		query.PriceType = 0x01; // ����Ʊ������Ʊ�۲�ѯƱ��/*:��ʱ��Ϊ01,��Ϊ�ӿ��ĵ���ʵ�ʲ����ϣ�����zhengxianle*/
	}
	else{
		query.PriceType = 0x02; // ����������Ʊ�۱��ѯƱ��
	}
	query.PassengerType = m_pCommonModel->mediumInfo.passengerType;
	query.Time = ComGetCurTime();

	memcpy(query.BegainStation, &m_pCommonModel->productInfo.beginLocation, 4);
	memcpy(query.EndStation, &m_pCommonModel->productInfo.endLocation, 4);
	long resultCode = -1;
	//	PhyCardType_t cardPHType = PhyCardType_t::CARD_TYPE_MIFARE_1_S70;

	try{
		// 1.��Ʊ
		if(theSERVICE_MGR.GetCurService()->GetServiceID() == ISSUE_SVC){
			// 1.1����Ʊ
			if (m_pCommonModel->productInfo.productType == SJT)
			{
				LOG("����Ʊ��ѯƱ�ۡ�");
				//			cardPHType = PhyCardType_t::CARD_TYPE_MIFARE_ULTRALIGHT;
				if(!theEXCEPTION_MGR.HasException(CRWException::TVM_TOKEN_RW_MODULE_ID)
					&&!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_TOKEN_RW_MODULE_ID))
				{
					resultCode = CARDRW_HELPER->GetTicketPrice(query, result);
					if(resultCode == RW_RSP_OK){
						taxInput.SaleOrAddAmount = result.TicketPrice;
						//					resultCode = TOKENRW_HELPER->GetTicketPriceTax(taxInput,taxRsp);
					}
				}
			}
			// 1.2����Ʊ��
			else
			{
				LOG("�ǵ���Ʊ��ѯƱ�ۡ�");
				if(!theEXCEPTION_MGR.HasException(CRWException::TVM_CARD_RW_MODULE_ID)
					&&!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_CARD_RW_MODULE_ID))
				{
					resultCode = CARDRW_HELPER->GetTicketPrice(query, result);
					if(resultCode == RW_RSP_OK){
						taxInput.SaleOrAddAmount = result.TicketPrice;
						resultCode = CARDRW_HELPER->GetTicketPriceTax(taxInput,taxRsp);
					}
				}
			}
			if(resultCode == RW_RSP_OK){
				LOG("Ʊ�۲�ѯ�ɹ���(��Ʊ)");
				// ��Ʒ���۵���
				m_pCommonModel->productInfo.lProductSaleAmount = result.TicketPrice;
				//m_pCommonModel->paymentInfo.lTaxAmount		   = theACC_BUSINESS.GetCardHolderFee(cardPHType,CARDHOLDER_FEE_TYPE_TAX,result.TicketPrice);
				m_pCommonModel->paymentInfo.lTaxAmount		   = taxRsp.TicketTax;
			}
			else{
				LOG("Ʊ�۲�ѯʧ�ܡ�(��Ʊ)");
			}
		}
		// 2.��ֵ
		else{
			LOG("��ֵ��ѯƱ�ۡ�");
			// ��Ӳ���쳣
			if(!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)&&
				!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)){	
					resultCode = RECHARGERW_HELPER->GetTicketPrice(query, result);
					if(resultCode == RW_RSP_OK){
						taxInput.SaleOrAddAmount = result.TicketPrice;
						resultCode = RECHARGERW_HELPER->GetTicketPriceTax(taxInput,taxRsp);
					}
			}
			if (resultCode == RW_RSP_OK) {
				LOG("Ʊ�۲�ѯ�ɹ���(��ֵ)");
				m_pCommonModel->paymentInfo.lPriceAmount = result.TicketPrice;
				//m_pCommonModel->paymentInfo.lTaxAmount		   = theACC_BUSINESS.GetCardHolderFee(cardPHType,CARDHOLDER_FEE_TYPE_TAX,result.TicketPrice);
				m_pCommonModel->paymentInfo.lTaxAmount		   = taxRsp.TicketTax;
			}
			else {
				LOG("Ʊ�۲�ѯʧ�ܡ�(��ֵ)");
				//theAPP_SESSION.ShowOfficeGuide(RECHARGERW_HELPER->GetTpuErrorInfo(result.DetailErrorCode));
			}
		}
	}
	catch(CSysException& e){
		LOG("Ʊ�۲�ѯ�����쳣");
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		LOG("Ʊ�۲�ѯ�����쳣");
	}


	return resultCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ��Ǯ

@param      ��

@retval     void

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartReceiveMoney()
{
	try{
		m_bRechargeConfirm = false;
		m_pCommonModel->operationInfo.isBeginBuyProduct = true;
		// �ֽ�֧������Ҫ�ж��Ƿ���Խ���ֽ�һ�Ӳ��??���������ж�
		if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
			// ���������м�ʱ��,�ȴ�����Ǯ��ʱ
			LOG("��ʼ��Ǯ,�ֽ�");
			InsertMoneyNeedStartTimer();
			StartReceiveBanknote();
			StartReceiveCoin();

		} 
		// ���п�֧��
		else if(m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD){
			StartReceiveBankCard();
		}
		// EP֧��

		theSERVICE_MGR.SetState(COUNTING);		
		//// �����п�����ʾȡ����ť
		//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��Ӳ��

@param      ��

@retval     ��

@exception  CSysException& e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartReceiveCoin()
{
	if (theSERVICE_MGR.GetCurService()->GetServiceID()==CHARGE_SVC){
		return;
	}

	// ��ǰģʽ֧�ֽ���Ӳ�� �� û��Ӳ��ģ���쳣,��ʼ����Ӳ��
	if (((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN)
		&& (!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHException::MODULE_ID))) {	
			try {
				LOG("��ʼ����Ӳ�ҵ�Ͷ�롣");
				//�жϵ�ǰ�ɽ��յ�Ӳ������
				CTCoinCountInfo::COIN_TEMPORARY1_INFO tempB = theCOIN_COUNT.GetCoinTemporary1Info();
				tempB.ulCount_1yuan = tempB.ulCount_5jiao = 0;
				theCOIN_COUNT.SetCoinTemporary1Info(tempB);
				COIN_HELPER->CH_StartReceive(this,GetAcceptCoinFaceTable()); // ���Դ��󷵻�ֵ
				m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
				m_pCommonModel->operationInfo.isCoinSlotOpen = true;
			}
			catch (CSysException& e){
				// ��������������쳣����Ҫֹͣ����Ӳ��
				OnError(theEXCEPTION_MGR.GetDescription(e));
				theEXCEPTION_MGR.ProcessException(e);
				NotifyDataModelChanged();
			}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ��ֽ��

@param      ��

@retval     ��

@exception  CSysException& e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartReceiveBanknote()
{
	// ��ǰģʽ֧�ֽ���ֽ����ʼ����ֽ��
	if (((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
		&& (!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID))) {
			try {
				LOG("��ʼ����ֽ�ҵ�Ͷ�롣");
				tBncSetInhibitList table;
				GetAcceptBanknotesFace(&table);
				BANKNOTE_HELPER->OpenAndValidateAsync(this,table);
				if (table.psBncCashInfo)
				{
					delete[] table.psBncCashInfo;
					table.psBncCashInfo = NULL;
				}

				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
				m_pCommonModel->operationInfo.isBankNoteSlotOpen = true;
			}
			catch (CSysException& e){
				OnError(theEXCEPTION_MGR.GetDescription(e));
				theEXCEPTION_MGR.ProcessException(e);
			}
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ�������п�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::StartReceiveBankCard()
//{
//	// �����ж�Ӳ��״̬,��Ϊ�粻��������,CTicketHelper::GetAvailablePayTypeList�Ѿ��Զ�����ʾѡ��
//	//BANKCARD_HELPER->TurnOnInsertTrigger(this);
//	theAudio_HELPER.PlayAudio(AUDIO_ID_INSERT_BANKCARD);
//	issuePayDetailInfo.isBankCardOpen = true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼ�ô�ֵ��֧��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartPayUsingEPCard()
{
	//?? ����Ӧ���ǡ���EP��֧�����ĺ���,������������ֻ�Ƕ���,����һ���ǲ��ԡ�

	// �����ж�Ӳ��״̬,��Ϊ�粻��������,CTicketHelper::GetAvailablePayTypeList�Ѿ��Զ�����ʾѡ��
	/*RECHARGERW_HELPER->ReadCard(true);	
	theAudio_HELPER.PlayAudio(AUDIO_ID_SWIPE_EP_CARD);
	m_pCommonModel->operationInfo.isWaitingEPCard = true;*/
	// ������ˢ��������,ֱ�ӽ������ѿۿ�
	try{
		m_pCommonModel->operationInfo.isWaitingEPCard = true;
		// ��������첽��������,һֱ��Ѱ���ж�
		RECHARGERW_HELPER->ReadCard((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID(),false,true);
		// ��ʾ�ȴ�������Ϣ��
		int iReadTimeLongth = theAFC_TVM_OPERATION.GetCancelDealTime();		
		INT_PTR ret = CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID()),
			_tl(TXT_PLEASE_PUT_THE_SVT_ON_RECHARGE_READER),
			TVM_TIP_SWIPE_CARD,
			CMessagePictureBox::BTN_CANCEL,DT_LEFT,
			iReadTimeLongth);

		if (IDCANCEL == ret || IDABORT == ret) 
		{
			RECHARGERW_HELPER->StopReadCard();  
		}
	}
	catch(CSysException& e){

	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ�������

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnAcceptMoneyOK() // should be OnReceiveMoneyOK by name
{

	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);// ����ȡ����ť
	LOG("OnAcceptMoneyOK");
	m_pCommonModel->operationInfo.isFinishedReceveMoney = true;
	//EndRecvTimer();
	// �����ȴ���ʱ��ʱ��,����ť���º�,��ʱ��������
	//if (!theAPP_SESSION.IsBlindManOperation())
	if(m_bNeedClickOkButton){
		LOG("�ֽ�������,��ʾȷ�ϰ�ť��");
		GetCurReceptionDlg()->ShowCompoment(CReceptionDlg::BTN_OK);
		InsertMoneyNeedStartTimer();
	}
	else{
		// ��Ӳ������
		if(m_pCommonModel->lDeviceStatus == 0){
			LOG("�ֽ�������,��ʼ��Ʊ��");
			//GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_OK);
			GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ISSUING,0,(LPARAM)0);
		}
		else{
			LOG("��ǰ��Ӳ���������ȴ���ɺ�Ʊ��״̬Ϊ��%d",m_pCommonModel->lDeviceStatus);
		}
	}
	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֧����ɴ���(�ر�ֽ��Ӳ�ҿڼ��˳����п���)

@param      ��

@retval     ��

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::AcceptMoney()
{
	try{
		if (m_ePayType==PAY_WITH_SVT){
			return;
		}
		// ������ֽ�֧��,ѹ��(�������,����ɹ�֧��ҳ��)
		if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
			// �����ʼ��Ǯ��,������ʱ����
			EndInsertTimer();
			LOG("�ֽ�֧��,��ʼѺ����");
			StartAcceptCash();
		}
		// ��������п����ߴ�ֵ��,����ѹ������,�ȴ���Ʊ��ɡ�
		else if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
			OnReturnFinish();
		}		

		//?? ��������Ӧ�������֧��(issueing֮ǰ)�͹ر����еĿ�,����Ӧ�õȵ�ѹ��Ʊ�����
		// ����,���п�Ӧ������Ʊ/��ֵ/�Ⱦ������ǰ�����˳���

		NotifyDataModelChanged();
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ѹ���ͻ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartAcceptCash()
{
	try{
		EndRecvTimer();//Ǯ�չ��ˣ�ֹͣ��ʱ��
		// 1.Ӳ�һ���
		if (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED) {
			// Ӳ�һ�����Ҫ�жϵ�ǰѭ�������������,��CoinModuleHelper���жϡ�
			LOG("��ʼѺ��Ӳ��,�ѽ���Ӳ�ҡ�");
			COIN_HELPER->CH_AcceptAsync(this,m_bCoinHopperAccept); 
			m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		}
		else{
			StartCHChange();
		}
		// 2.ֽ�һ���		
		if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
			LOG("��ʼѺ��ֽ��,�ѽ���ֽ�ҡ�");
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;// Ϊ�˲��������ط�ͬ��ѹ����ͻ�������첽ѹ��ǰ���Ȱѱ�ʶ��Ϊ��ѹ��Ʊ��
			BANKNOTE_HELPER->CollectAsync(this);
			m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		}
		// ֽ������:����������Ӳ��������ֱ�����㡣
		StartBHChange();

	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      �ֽ��������

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnReceiveMoneyOK()
{
	try {
		// 1.��ʼ����
		if (m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_NONE) {
			//StartChange();
		}
	}
	catch (CSysException& e){
		// ��������쳣
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����

@param      ��

@retval     bool	true:�ɹ�	false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//bool CPassengerForeSvc::StartChange()
//{
//	// 1.�ж��Ƿ���Ҫ����
//	if(m_pCommonModel->paymentInfo.lChangeAmount == 0 ){
//		LOG("��������,ֱ�ӷ��ء�");
//		m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_SUCCESS;
//		// ִ�з��سɹ�����
//		OnReturnFinish();
//		return true;
//	}
//	// 2.������ָʾ��
//	if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID))
//	{
//		LOG("������ָʾ�������ơ�");
//		/*IO_HELPER->OpenReturnLight();*/
//		IO_HELPER->OpenReturnCupIndicator();
//	}
//	// 3.��ʼ����(���������������Ҫ�ҳ���ö��)
//	if (theAPP_SESSION.IsCHConnected() && 
//		!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)&&
//		!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){	
//		Sleep(200);//�ṩֹͣ��ȡ����ʱ��,200msΪһ����ȡ�������.
//		// ����ǰ�ȶ�ȡһ��״̬
////		COIN_HELPER->ReadStatus(m_pCommonModel->paymentInfo.rspCoinModule);
//		LOG("��ʼ����,�첽���á�");
////		COIN_HELPER->CH_Change(this,m_pCommonModel->paymentInfo.dwChangeHopper1Count, m_pCommonModel->paymentInfo.dwChangeHopper2Count, m_pCommonModel->paymentInfo.dwChangeStockCount); 
//	}
//	// 4.�޷�����,��ӡ���ϵ�
//	else{
//		LOG("�޷�����,��ȡ���ϵ���");
//		m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_FAIL;
//		OnReturnFinish();
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���׳ɹ�,��ʾ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnSuccessFinish()
{
	try{
		LOG("���׳ɹ�,���ص��ɹ����档");
		// ����SC����
		SaveSCData(); 
		if(!m_pCommonModel->isInsertToDatabase){
			// �������ݿ�����
			Data::TransactionRecord dbInfo;
			WriteDBData(dbInfo); // д�����ݿ����ϸ��
			//dbInfo.iTransactionDetail.iState = 0; // 0�������������� 1�����׹��Ͻ��� 2��ȡ���������� 3��ȡ�����Ͻ���
			DATA_HELPER->DbInsertTransactionInfo(dbInfo);
			m_pCommonModel->isInsertToDatabase = true;
		}

		// ������ת
		//theAudio_HELPER.PlayAudio(AUDIO_ID_THANKS);
		//theSERVICE_MGR.SetState(FINISH);

		FlowToFinishDlg();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		FlowToFinishDlg();
		LOG("д�����ݿ�ʧ�ܡ�");
	}
	catch(...){

		LOG("д�����ݿ�ʧ�ܡ�");
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ����SC����

@param      none

@retval  	��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::SaveSCData()
{
	LOG("����SC���ݡ�");
	theSCAudit_MGR.addCoinChangeSum(m_pCommonModel->paymentInfo.lCHReceiveTotalAmount); // Ӳ��������
	theSCAudit_MGR.addCoinIncomeSum(m_pCommonModel->paymentInfo.lChangeAmount);				// Ӳ��������
	theSCAudit_MGR.addNoteIncomeSum(m_pCommonModel->paymentInfo.lBHReceiveTotalAmount); // ֽ��������
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      д�����ݿ�����

@param      none

@retval  	��

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::WriteDBData(Data::TransactionRecord &dbInfo) {
	LOG("д������");
	// ������Ϣ������
	dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"), m_pCommonModel->dwUDSN); 
	dbInfo.iTransactionDetail.dtTransTime = m_pCommonModel->transTime;

	// ��Ʊ
	if(ISSUE_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
		// �ۿ�
		dbInfo.iTransactionDetail.iUdType = 2;
		dbInfo.iTransactionDetail.iUdSubtype = 0;
		// ���
		dbInfo.iTransactionDetail.iUnitAmount = static_cast<int>(m_pCommonModel->productInfo.lProductSaleAmount);
		// ����
		dbInfo.iTransactionDetail.iQuantity   = static_cast<int>(m_pCommonModel->productInfo.nProductPiece);
		dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = m_pCommonModel->productInfo.nProductIssuedPiece;
	}
	// ��ֵ
	else if(CHARGE_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
		dbInfo.iTransactionDetail.iUdType = 4;
		// ���
		dbInfo.iTransactionDetail.iUnitAmount = static_cast<int>(m_pCommonModel->paymentInfo.lPaidAmount);
		m_pCommonModel->paymentInfo.lDueAmount = m_pCommonModel->paymentInfo.lPaidAmount;
		// ����
		dbInfo.iTransactionDetail.iQuantity   = 1;
		dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = 0;
	}
	// �۲�Ʒ
	else if(ISSUE_PRODUCT_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
		dbInfo.iTransactionDetail.iUdType = 3;
		// ���
		dbInfo.iTransactionDetail.iUnitAmount = static_cast<int>(m_pCommonModel->paymentInfo.lDueAmount);
		// ����
		dbInfo.iTransactionDetail.iQuantity   = 1;
		dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = 0;
	}

	dbInfo.iTransactionDetail.iCardType = static_cast<int>(m_pCommonModel->mediumInfo.cardPHType); // CardType_t
	dbInfo.iTransactionDetail.iProductType = static_cast<int>(m_pCommonModel->productInfo.productType);
	dbInfo.iTransactionDetail.iProductSubType = static_cast<int>(m_pCommonModel->productInfo.productSubType);
	dbInfo.iTransactionDetail.iOrigin = static_cast<int>(m_pCommonModel->productInfo.beginLocation);
	dbInfo.iTransactionDetail.iDestination = static_cast<int>(m_pCommonModel->productInfo.endLocation);
	dbInfo.iTransactionDetail.DepositAmount = static_cast<int>(m_pCommonModel->mediumInfo.lCardDeposit); //?? Ʊ��Ѻ���ǲ�ƷѺ��
	dbInfo.iTransactionDetail.TaxAmount = static_cast<int>(m_pCommonModel->paymentInfo.lTaxAmount); 
	dbInfo.iTransactionDetail.CardFeeAmount = static_cast<int>(m_pCommonModel->mediumInfo.lCardFee);
	dbInfo.iTransactionDetail.iPaymentMethod = static_cast<int>(m_pCommonModel->paymentInfo.paymentMethod);
	dbInfo.iTransactionDetail.iTotalAmount = static_cast<int>(m_pCommonModel->paymentInfo.lDueAmount); 
	dbInfo.iTransactionDetail.CoinInsertAmount = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
	dbInfo.iTransactionDetail.CoinState = m_pCommonModel->operationInfo.coinAcceptState; // 0����Ͷ�� 1��������  2�����˳�
	dbInfo.iTransactionDetail.BankNoteAcceptAmount = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
	dbInfo.iTransactionDetail.BanknoteAcceptState = m_pCommonModel->operationInfo.banknoteAcceptState; // 0����Ͷ�� 1��������  2�����˳�
	dbInfo.iTransactionDetail.iChangeAmount = m_pCommonModel->paymentInfo.lChangeAmount;// Ӧ������
	dbInfo.iTransactionDetail.iCoinChangeAmount = static_cast<int>(m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount * 100 + m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount * 50);
	dbInfo.iTransactionDetail.iCoinChangeState = m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_SUCCESS ? 1:0; // 0��δ�ų�  1���ѷų�
	dbInfo.iTransactionDetail.iBanknoteChangeAmount = m_pCommonModel->paymentInfo.dwChangeBHAmout;
	dbInfo.iTransactionDetail.iBanknoteChangeState = m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_SUCCESS ? 1:0; // 0��δ�ų�  1���ѷų�
	dbInfo.iTransactionDetail.iPaymentState = m_pCommonModel->operationInfo.paymentState; // 0��δ֧�� 1����֧�� 2��֧��ʧ��
	if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
		for (int i = 0; i < 10; ++i) {
			CString strTemp(_T(""));
			strTemp.Format(_T("%d"), m_pCommonModel->paymentInfo.BankCardNum[i]);
			dbInfo.iTransactionDetail.iPayCardNo += strTemp;
		}
	}

	dbInfo.iTransactionDetail.iPrintState = m_pCommonModel->operationInfo.failurePrintState; // 0��δ��ӡ 1���Ѵ�ӡ 2����ӡʧ��
	//dbInfo.iTransactionDetail.iState; // ���ڲ�ͬ������ʽ�ľ������ξ���д��
	switch (m_pCommonModel->operationInfo.finishType){
	case DEFAULT:
		dbInfo.iTransactionDetail.iState = 0;// ������������
		break;		
	case NOT_ENOUGH_TICKETS_FOR_BOOKING:
	case TRANSACTION_STATE_FAILED:
	case CHANGE_FALIED_RETURN:
		dbInfo.iTransactionDetail.iState = 1;// ���׹��Ͻ���
		dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"),m_pCommonModel->dwTransactionID);
		break;
	case PASSAGE_CANCEL:
	case INSERT_OUT_OF_TIME:
	case OVER_MONEY_AMOUNT_LIMIT:
	case OVER_MONEY_COUNT_LIMIT:
	case OVER_CHANGE_AMOUNT_LIMIT:
	case OVER_CHANGE_COUNT_LIMIT:
	case RUNNING_OUT_OF_TIME:
		dbInfo.iTransactionDetail.iUDSN = _T("0");
		dbInfo.iTransactionDetail.iState = 2;// ȡ����������
		break;
	case SERVICE_ERROR_CANCEL:
	case EXCEPTION_ERROR:		
		// ����Ǯ���ѷ�Ʊ����������
		if(m_pCommonModel->operationInfo.mapException.size() == 0 && m_pCommonModel->productInfo.nProductIssuedPiece > 0){
			dbInfo.iTransactionDetail.iState = 0;
		}
		// ��Ҫ��Ǯ��
		else if(m_pCommonModel->operationInfo.mapException.size() > 0){
			// �ѷ�Ʊ
			if(m_pCommonModel->productInfo.nProductIssuedPiece > 0){
				dbInfo.iTransactionDetail.iState = 1;// ���׹��Ͻ���
				dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"),m_pCommonModel->dwTransactionID);
			}
			// δ��Ʊ
			else{
				dbInfo.iTransactionDetail.iState = 3;// ȡ�����Ͻ���
				dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"),m_pCommonModel->dwTransactionID);
			}
		}
		else{
			dbInfo.iTransactionDetail.iState = 2;// ȡ����������
			dbInfo.iTransactionDetail.iUDSN = _T("0"); 
		}
		break;
	default:
		break;
	}
	dbInfo.vTransCoinDetail.clear();
	map<BankNoteAndCoinType_t,long>::iterator it;

	// ��¼Ӳ��Ͷ����Ϣ
	for (it = m_pCommonModel->paymentInfo.PayCoinInfo.begin(); it != m_pCommonModel->paymentInfo.PayCoinInfo.end(); ++it) {
		if(it->second > 0){
			Data::CoinInsertRecord transCoin;
			transCoin.dtOperationTime = m_pCommonModel->transTime;
			transCoin.iCoinTypeCode = static_cast<int>(it->first);
			transCoin.iQuantity = static_cast<int>(it->second);
			dbInfo.vTransCoinDetail.push_back(transCoin);
		}
	}

	int hopper1Type = theTVM_INFO.GetCHHopper1Type();
	int hopper2Type = theTVM_INFO.GetCHHopper2Type();
	// ��¼Ӳ��������Ϣ
	for (auto coinSaveIter = m_pCommonModel->paymentInfo.CoinSaveDetail.begin() ; coinSaveIter != m_pCommonModel->paymentInfo.CoinSaveDetail.end();++coinSaveIter){
		if(coinSaveIter->second > 0){
			Data::CoinAcceptRecord transCoin;
			transCoin.dtOperationTime = m_pCommonModel->transTime;
			if(coinSaveIter->first == COIN_HOPPER_1){
				transCoin.iCoinTypeCode = Coin1Yuan;
				transCoin.txtAcceptCoinBoxID = theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();
				transCoin.iQuantity = coinSaveIter->second;
			}
			else if(coinSaveIter->first == COIN_HOPPER_2){
				transCoin.iCoinTypeCode = CoinHalfYuan;
				transCoin.txtAcceptCoinBoxID = theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();
				transCoin.iQuantity = coinSaveIter->second;
			}
			else if(coinSaveIter->first == COIN_COLL_BOX_1_1){
				transCoin.iCoinTypeCode = Coin1Yuan;
				transCoin.txtAcceptCoinBoxID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
				transCoin.iQuantity = coinSaveIter->second;
			}
			else if(coinSaveIter->first == COIN_COLL_BOX_1_5){
				transCoin.iCoinTypeCode = CoinHalfYuan;
				transCoin.txtAcceptCoinBoxID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
				transCoin.iQuantity = coinSaveIter->second;
			}
			dbInfo.vTransCoinAcceptDetail.push_back(transCoin);
		}
	}


	// ��¼Ӳ��������Ϣ
	for (auto changeIter = m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.begin(); changeIter != m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.end(); ++changeIter) {
		if(changeIter->second > 0){
			Data::CoinChangeRecord transCoin;
			transCoin.dtOperationTime = m_pCommonModel->transTime;

			if(COIN_HOPPER_1 == changeIter->first){
				transCoin.iCoinTypeCode = Coin1Yuan;
				CTCoinCountInfo::COIN_BOX_INFO hopper1 = theCOIN_COUNT.GetCoinHopperboxAInfo();
				transCoin.txtCoinBoxID = hopper1.coinBoxID.ToString();
			}
			else if(COIN_HOPPER_2 == changeIter->first){
				transCoin.iCoinTypeCode = CoinHalfYuan;
				CTCoinCountInfo::COIN_BOX_INFO hopper2 = theCOIN_COUNT.GetCoinHopperboxBInfo();
				transCoin.txtCoinBoxID = hopper2.coinBoxID.ToString();
			}
			else if(COIN_CHANGE_BOX_1 == changeIter->first){
				transCoin.iCoinTypeCode = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
				transCoin.txtCoinBoxID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
			}
			else if(COIN_CHANGE_BOX_2 == changeIter->first){
				transCoin.iCoinTypeCode = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
				transCoin.txtCoinBoxID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
			}
			else{
				// ��Ӳ�����㣬��ֱ�������������
				continue;
			}
			transCoin.iQuantity = static_cast<int>(changeIter->second);
			dbInfo.vTransCoinChangeDetail.push_back(transCoin);
		}
	}

	// ��¼ֽ��Ͷ����Ϣ
	for (it = m_pCommonModel->paymentInfo.PayBankNoteInfo.begin(); it != m_pCommonModel->paymentInfo.PayBankNoteInfo.end(); ++it) {
		Data::BanknoteInsertRecord transCoin;
		transCoin.dtOperationTime = m_pCommonModel->transTime;
		transCoin.iCoinTypeCode = static_cast<int>(it->first);
		transCoin.iQuantity = static_cast<int>(it->second);
		dbInfo.vTransBanknoteDetail.push_back(transCoin);
	}

	// ��¼ֽ��������Ϣ
	for (it = m_pCommonModel->paymentInfo.PayBankNoteInfo.begin(); it != m_pCommonModel->paymentInfo.PayBankNoteInfo.end(); ++it) {
		if(dbInfo.iTransactionDetail.iState != 2){	// ȡ�������
			Data::BanknoteAcceptRecord transCoin;
			transCoin.dtOperationTime = m_pCommonModel->transTime;
			transCoin.iCoinTypeCode = static_cast<int>(it->first);
			transCoin.iQuantity = static_cast<int>(it->second);
			transCoin.txtAcceptCoinBoxID = theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString();
			dbInfo.vTransBanknoteAcceptDetail.push_back(transCoin);
		}
	}

	// ֽ����������ϸ(ֻ������ɹ��ſ������)
	Data::BanknoteChangeRecord BNRRecycleARecord,BNRRecycleBRecord,BNRRecycleCRecord,BNRRecycleDRecord;
	if(m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount > 0 && dbInfo.iTransactionDetail.iBanknoteChangeState == 1){
		BNRRecycleARecord.dtOperationTime = m_pCommonModel->transTime;
		BNRRecycleARecord.iCoinTypeCode   = TransBanknoteType(theTVM_SETTING.GetBNRRecycleBoxAAmount());
		BNRRecycleARecord.iQuantity		 = m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount;
		BNRRecycleARecord.txtCoinBoxID	 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().banknoteBoxID.ToString();
		dbInfo.vTransBanknoteChangeDetail.push_back(BNRRecycleARecord);
	}

	if(m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount > 0 && dbInfo.iTransactionDetail.iBanknoteChangeState == 1){
		BNRRecycleBRecord.dtOperationTime = m_pCommonModel->transTime;
		BNRRecycleBRecord.iCoinTypeCode   = TransBanknoteType(theTVM_SETTING.GetBNRRecycleBoxBAmount());
		BNRRecycleBRecord.iQuantity		 = m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount;
		BNRRecycleBRecord.txtCoinBoxID	 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().banknoteBoxID.ToString();
		dbInfo.vTransBanknoteChangeDetail.push_back(BNRRecycleBRecord);
	}
	if(m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount > 0 && dbInfo.iTransactionDetail.iBanknoteChangeState == 1){
		BNRRecycleCRecord.dtOperationTime = m_pCommonModel->transTime;
		BNRRecycleCRecord.iCoinTypeCode   = TransBanknoteType(theTVM_SETTING.GetBNRRecycleBoxCAmount());
		BNRRecycleCRecord.iQuantity		 = m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount;
		BNRRecycleCRecord.txtCoinBoxID	 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().banknoteBoxID.ToString();
		dbInfo.vTransBanknoteChangeDetail.push_back(BNRRecycleCRecord);
	}
	if(m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount > 0 && dbInfo.iTransactionDetail.iBanknoteChangeState == 1){
		BNRRecycleDRecord.dtOperationTime = m_pCommonModel->transTime;
		BNRRecycleDRecord.iCoinTypeCode   = TransBanknoteType(theTVM_SETTING.GetBNRRecycleBoxDAmount());
		BNRRecycleDRecord.iQuantity		 = m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount;
		BNRRecycleDRecord.txtCoinBoxID	 = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().banknoteBoxID.ToString();
		dbInfo.vTransBanknoteChangeDetail.push_back(BNRRecycleDRecord);
	}

	// ��ӹ�����ϸ��
	map<TRANSACTION_EXCEPTION,long>::iterator itor = m_pCommonModel->operationInfo.mapException.begin();
	for(;itor != m_pCommonModel->operationInfo.mapException.end();itor++){
		Data::ExceptionRecord excep;
		excep.dtOperationTime = m_pCommonModel->transTime;
		excep.iExceptionType = itor->first/*.Format(_T("%d"),itor->first)*/;
		excep.IExceptionAmount = (int)itor->second;
		dbInfo.vExceptionDetail.push_back(excep);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˱�(ֽ��Ӳ�һ�����)

@param      ��

@retval     bool	true:�ɹ�	false:ʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::StartReturnCash()
{
	try{
		LOG("�˱ҡ�");
		int res = 0;
		// ���н��յ�Ӳ��ʱӲ���˱�
		if (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED) {
			// ���쳣ʱ,ִ���˱Ҳ�����
			if (!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){	
				if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
					m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
					COIN_HELPER->CH_RefundAsync(this);
				}
			}
		}
		// ���н��յ�ֽ��ʱֽ���˱�
		if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
			if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
				BANKNOTE_HELPER->ReturnAllAsync(this);
			}
		}

		// �������ݿ�����,why???zxl
		if (theSERVICE_MGR.GetState() == COUNTING || theSERVICE_MGR.GetState() == ISSUEING) {
			//CDataHelper transData(*this);
			//Data::TransactionRecord dbInfo;
			//WriteDBData(dbInfo); // д�����ݿ����ϸ��
			////dbInfo.iTransactionDetail.iState = (res==0)?2:3; // 0�������������� 1�����׹��Ͻ��� 2��ȡ���������� 3��ȡ�����Ͻ���
			//transData.DbInsertTransactionInfo(dbInfo);
			//?? �������Ӳ����Ӳ�Ƿ�ɹ��޷������жϡ�����˱ҳɹ�,OnCoinReturned�ᱻ�ص�(���Ǵ˴��޴Ӷ�֪)
			// ���ʧ��,���������ͨ������FlowToErrorDlg�ĺ���(����onCancel���߸��ֳ��޴���)
			// �õ�exception�������쳣����ҳ�档
		}
		return true;
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCancel()
{
	try{	
#if 1
		theSERVICE_MGR.SetState(CANCELING);
		//����ȡ�����
		m_bIsPassengerCanceled = true;
		m_pCommonModel->operationInfo.finishType = PASSAGE_CANCEL;
		NotifyDataModelChanged();
		EndRecvTimer();
		OnReturnFinish();
#else
		LOG("ȡ������");
		StopCircleReadCard();
		IO_HELPER->ReturnRechargeCard();
		// ȡ��һ�����Լ��жϷ���,���ٴε��ʱ����Ӧ
		if (CANCELING == theSERVICE_MGR.GetState() || (m_pCommonModel->operationInfo.finishType != DEFAULT && m_pCommonModel->operationInfo.finishType != QR_ORDER_GENERATE_FAILED)){
			return;
		}
		// ���� 20170627���ؿؼ������ѿ������в��ֲ��������Դ˴���������
		//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.Invoke();// ���ص�ǰ����Ҫ��ʾ�Ŀؼ�

		theSERVICE_MGR.SetState(CANCELING);
		//����ȡ�����
		m_bIsPassengerCanceled = true;
		m_pCommonModel->operationInfo.finishType = PASSAGE_CANCEL;
		NotifyDataModelChanged();
		EndRecvTimer();
		OnReturnFinish();
#endif
		
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �޷���ֵ�쳣����

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnServiceErrorCancel()
{
	try{
		theSERVICE_MGR.SetForeServiceBusy(false);
		LOG("�޷���������,�쳣��");
		//���ô�����
		m_pCommonModel->operationInfo.finishType = SERVICE_ERROR_CANCEL;

		OnErrorFinish();
	}
	catch (CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������

@param      (i)CSysException  ϵͳ�׳�������Ϣ

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnError(CSysException e)
{
	try{
		// ����ֻ����ֽ����Ӳ��ģ���쳣������
		theSERVICE_MGR.SetForeServiceBusy(false);
		LOG("�쳣����");
		// �������״̬,�������ó����Ϊ���ܹ��ص���Ǯ��
		m_pCommonModel->operationInfo.finishType = SERVICE_ERROR_CANCEL;
		// ��ӵ��쳣����
		theEXCEPTION_MGR.ProcessException(e);
		theSERVICE_MGR.SetForeServiceBusy(true);

		// 1.Ӳ���쳣:Ͷ��,����,�˱ҡ�
		if(e.GetModuleID() == CCHHardwareException::MODULE_ID || e.GetModuleID() == CCHException::MODULE_ID){
			// ����
			if(m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_FAIL){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_COIN] = m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount;
				m_pCommonModel->operationInfo.finishType = CHANGE_FALIED_RETURN;
			}
			// �˱�
			else if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_RETURN_FAILD ){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
			}
			// ����
			else if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_COLLECTED_FAILD){
				// ����ʧ���޷����㣬�쳣���Ӧ��Ϊ������
				if(m_pCommonModel->paymentInfo.lChangeAmount != 0){
					m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount = m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount * 100 + m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount * 50;
					m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_COIN] = m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount;
				}
				else{
					theSERVICE_MGR.SetForeServiceBusy(false);
				}
			}
			// Ͷ���쳣:��Ҫֹͣ����Ӳ�Һ�ֽ��zhengxianle
			else{
				// Ͷ��ʧ��ʱ��Ҫֹͣ��ʱ��
				EndInsertTimer();

				m_pCommonModel->operationInfo.finishType = EXCEPTION_ERROR;

				if(!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
					// ֽ�Ҽ��(��ֽ���쳣)
					try{
						if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
							m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
							BANKNOTE_HELPER->StopValidation();
						}
						if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
							if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
								m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
								BANKNOTE_HELPER->ReturnAllAsync(this);
							}
						}
					}
					catch(CSysException& e){
						theEXCEPTION_MGR.ProcessException(e);
					}
					catch(...){

					}
				}
				LOG("Ӳ��ģ�鵱ǰ����״̬��%d",m_pCommonModel->operationInfo.coinAcceptState);
				// Ӳ�Ҽ��
				try{
					if (m_pCommonModel->operationInfo.isCoinSlotOpen){
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;
						COIN_HELPER->CH_StopInsertInfo();
						COIN_HELPER->CH_EndReceive();
					}

					if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
						m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
						COIN_HELPER->CH_RefundAsync(this);
					}
				}
				catch(CSysException& e){
					theEXCEPTION_MGR.ProcessException(e);
					// �˱�ʧ��ѽ
					if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED){
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
					}
				}
				catch(...){
					LOG("ֹͣʶ��ʧ�ܡ�");
				}

				// ���ݴ����
				CTCoinCountInfo::COIN_TEMPORARY1_INFO tempB = theCOIN_COUNT.GetCoinTemporary1Info();
				if(tempB.ulCount_1yuan != 0){
					CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
					hopperB.ulCurCount += tempB.ulCount_1yuan;
					theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);
					tempB.ulCount_1yuan = 0;
					theCOIN_COUNT.SetCoinTemporary1Info(tempB);
				}

				if(m_pCommonModel->paymentInfo.lCHReceiveTotalAmount >0){
					// ��Ҫ��ӡ���ϵ�
					if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED){
						LOG("��Ҫ��ӡӲ���쳣���վݵ�����%d",m_pCommonModel->paymentInfo.lCHReceiveTotalAmount);
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
					}
				}
				else{
					LOG("��ʱû�н��յ�Ӳ�ң�����״̬Ϊδ���ա�");
					m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_NONE;
				}
			}

		}
		// 2.ֽ���쳣:Ͷ��,�˳�
		if(e.GetModuleID() ==  CBRHHardwareException::MODULE_ID){
			// ����ʧ��
			if(m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_FAIL && m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount > 0){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_BANKNOTE] = m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount;
				m_pCommonModel->operationInfo.finishType = CHANGE_FALIED_RETURN;
			}

			// ֽ���˱�ʧ��
			if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURN_FAILED){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_BANKNOTE] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
			}
			// ֽ��Ѻ��ʧ�ܣ���Ӱ�����㣬����Ӱ����Ϊ0
			else if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED){
				//m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_BANKNOTE_COLLECTION] = 0;
			}
			// ֽ��Ͷ��ʧ��:ֹͣ���գ����˳��������յ��ֽ�zhengxianle
			else{
				// Ͷ��ʧ��ʱ��Ҫֹͣ��ʱ��
				EndInsertTimer();

				m_pCommonModel->operationInfo.finishType = EXCEPTION_ERROR;

				if(!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
					// Ӳ���Ƿ�Ͷ�룿�˱�
					try{
						if (m_pCommonModel->operationInfo.isCoinSlotOpen){
							m_pCommonModel->operationInfo.isCoinSlotOpen = false;
							COIN_HELPER->CH_StopInsertInfo();
							COIN_HELPER->CH_EndReceive();
						}
						if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
							m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
							COIN_HELPER->CH_RefundAsync(this);
						}
					}
					catch(...){
						// Do nothing
					}
				}


				// ֽ�ҿ���
				try{
					if(m_pCommonModel->operationInfo.isBankNoteSlotOpen){
						BANKNOTE_HELPER->StopValidation();
						m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_BANKNOTE_INSERT] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
					}
					if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
						if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
							m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
							BANKNOTE_HELPER->ReturnAllAsync(this);
							/*m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_BANKNOTE] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
							m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_ERROR;*/
						}
					}
					if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURN_FAILED){
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_BANKNOTE] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
					}
				}
				catch(...){
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE){
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_BANKNOTE_INSERT] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
					}
					if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_BANKNOTE] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_ERROR;
					}
					else if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURN_FAILED){
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_BANKNOTE] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
					}
				}
			}
		}
		//// 3.ֽ�������쳣�����Ҵ���
		//else if(e.GetModuleID() == CBHChangeHardwareException::MODULE_ID){
		//	// ����ʧ��
		//	if(m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_FAIL && m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount > 0){
		//		m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_BANKNOTE] = m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount;
		//		m_pCommonModel->operationInfo.finishType = CHANGE_FALIED_RETURN;
		//	}
		//}
	}
	catch (CSysException& e){
		// �ٴγ����쳣,��Ҫ�ٴμ����쳣���С�
		theEXCEPTION_MGR.ProcessException(e);
		OnErrorFinish();
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		theEXCEPTION_MGR.ProcessException(e);
		OnErrorFinish();
	}

	OnReturnFinish();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ������

@param      (i)CString errorMsg  ������Ϣ����
@param      (ii)DWORD dwReceptMsg

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnError(CString errorMsg, DWORD dwReceptMsg)
{
	try{
		LOG("������,������ʾ��%s",errorMsg);
	}
	catch (CSysException& e){
	}
	catch(...){
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���������,ҳ����ת

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnErrorFinish(){
	LOG("���������,ϸ�ڡ�");
	if (m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_WITH_WEIXIN || m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_WITH_ZHIFUBAO){
		// �������ݿ�����
		if(!m_pCommonModel->isInsertToDatabase){
			Data::TransactionRecord dbInfo;
			WriteDBData(dbInfo); // д�����ݿ����ϸ��
			DATA_HELPER->DbInsertTransactionInfo(dbInfo);
			m_pCommonModel->isInsertToDatabase = true;
		}		

		// ���ý���״̬
		theSERVICE_MGR.SetForeServiceBusy(false);
		// ת���������
		FlowToErrorDlg();
		return;
	}

	m_pCommonModel->paymentInfo.lReturnAmount = 0;
	m_pCommonModel->paymentInfo.lNotChangeAmount = 0;

	// ���쳣
	if(m_pCommonModel->operationInfo.mapException.size() > 0){
		auto it = m_pCommonModel->operationInfo.mapException.begin();

		while(it != m_pCommonModel->operationInfo.mapException.end()){
			// ��Ӧ����
			switch(it->first){
			case TRANSACTION_EXCEPTION_CARD_MOUDOL:// �������������쳣
			case TRANSACTION_EXCEPTION_RETURN_COIN:
			case TRANSACTION_EXCEPTION_RETURN_BANKNOTE:
			case TRANSACTION_EXCEPTION_COIN_INSERT:
			case TRANSACTION_EXCEPTION_COIN_COLLECTION:
			case TRANSACTION_EXCEPTION_BANKNOTE_INSERT:
			case TRANSACTION_EXCEPTION_BANKNOTE_COLLECTION:
			case TRANSACTION_EXCEPTION_WRITE_CARD_FAILED:
				{
					m_pCommonModel->paymentInfo.lReturnAmount += it->second;
				}
				break;
			case TRANSACTION_EXCEPTION_RELEASE_COIN:// Ӳ�������쳣
			case TRANSACTION_EXCEPTION_RELEASE_BANKNOTE:// ֽ�������쳣
				{
					m_pCommonModel->paymentInfo.lReturnAmount	 += it->second;
					m_pCommonModel->paymentInfo.lNotChangeAmount += it->second;
				}
				break;
			default:
				break;
			}
			it++;
		}
	}


	try{
		// ��ӽ���ʧ����Ϣ��ҵ�����ݴ������
		BUSINESS_FAILED_DATA busiess_falied_data;
		busiess_falied_data.DeviceID = theMAINTENANCE_INFO.GetCurrentDevice();		// �豸���
		busiess_falied_data.DateTime = m_pCommonModel->transTime = ComGetCurTime(); // ����ʱ��
		busiess_falied_data.DeviceAfcNum = m_pCommonModel->dwTransactionID;			// ���Ͻ�����ˮ��,���Զ��ۼ�
		busiess_falied_data.TransactionNum = m_pCommonModel->dwUDSN;				// ��Ӧ������ˮ��
		busiess_falied_data.RefoundAmount = m_pCommonModel->paymentInfo.lReturnAmount;// Ӧ�˽��
		// ����ԭ��
		string strFailedReason = GetFailedReason();
		memcpy(busiess_falied_data.FailedReason,strFailedReason.c_str(),sizeof(busiess_falied_data.FailedReason));
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBussnessFailedData(busiess_falied_data); 

		LOG("���ϵ���%d��",m_pCommonModel->paymentInfo.lReturnAmount);

		// 5.��ӡ���ϵ�
		//if(theTVM_STATUS_MGR.GetPrintMode() == 0)	
		PrinterPrintErrorReceipt();
		if(theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID)){
			// ��ӡʧ�ܱ�־
			m_pCommonModel->operationInfo.failurePrintState = FAILURE_PRINT_STATE_FAIL;
		}
		else{
			// ��ӡ�ɹ���־
			m_pCommonModel->operationInfo.failurePrintState = FAILURE_PRINT_STATE_SUCCESS;
		}

		// 6.�����������
		SaveSCData();
		// 7.�������ݿ�����
		if(!m_pCommonModel->isInsertToDatabase){
			Data::TransactionRecord dbInfo;
			WriteDBData(dbInfo); // д�����ݿ����ϸ��
			DATA_HELPER->DbInsertTransactionInfo(dbInfo);
			m_pCommonModel->isInsertToDatabase = true;
		}		

		// 8.���ý���״̬
		theSERVICE_MGR.SetForeServiceBusy(false);
		// 9.ת���������
		FlowToErrorDlg();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		FlowToErrorDlg();
	}
	catch(...){
		//
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ����ʧ��ԭ��

@param      ��

@retval     string ����ԭ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
string CPassengerForeSvc::GetFailedReason(){
	string strFailedReason("");

	auto iter = m_pCommonModel->operationInfo.mapException.begin();
	if(iter != m_pCommonModel->operationInfo.mapException.end()){
		switch(iter->first){
			//case TRANSACTION_EXCEPTION_TOKEN_MOUDOL:
			//	strFailedReason = "Token Model Exception";
			//	break;
		case TRANSACTION_EXCEPTION_CARD_MOUDOL:
			//strFailedReason = "Card Model Exception";
			if (m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_CASH){
				strFailedReason = "51";//"51:TVM�ֽ�Ʊ";
			}
			else{
				strFailedReason = "54";//"54:���ֽ�Ʊ";
			}
			break;
		case TRANSACTION_EXCEPTION_RELEASE_COIN:
			//strFailedReason = "Release Coin Exception";
			strFailedReason = "53";//"53:TVM�ֽ�������";
			break;
		case TRANSACTION_EXCEPTION_RELEASE_BANKNOTE:
			//strFailedReason = "Release Banknote Exception";
			strFailedReason = "53";//"53:TVM�ֽ�������";
			break;
		case TRANSACTION_EXCEPTION_CREDIT:
			strFailedReason = "99";//"99:����";
			break;
		case TRANSACTION_EXCEPTION_BANKCARD:
			strFailedReason = "99";//"99:����";
			break;
		case TRANSACTION_EXCEPTION_EP:
			strFailedReason = "99";//"99:����";
			break;
		case TRANSACTION_EXCEPTION_RETURN_BANKNOTE:
			//strFailedReason = "Return Banknote Exception";
			strFailedReason = "50";//"50:TVM����";
			break;
		case TRANSACTION_EXCEPTION_RETURN_COIN:
			//strFailedReason = "Return Coin Exception";
			strFailedReason = "50";//"50:TVM����";
			break;
		case TRANSACTION_EXCEPTION_WRITE_CARD_FAILED:
			//strFailedReason = "Write Card Exception";
			if (m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_CASH){
				strFailedReason = "52";//"52:TVM�ֽ�����ЧƱ";
			}
			else{
				strFailedReason = "55";//"55:���ֽ�����ЧƱ";
			}
			break;
		default:
			strFailedReason = "99";//"99:����";
			break;
		}
	}
	return strFailedReason;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �ر�����Ӳ��

@param      (i)CString errorMsg  ������Ϣ����
@param      (i)bool bContinue    �Ƿ�����������

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::CloseAllDevice()
{
	LOG("�ر����н��տڡ�");

	try{
		// ��Ӳ�ҿ�
		if (m_pCommonModel->operationInfo.isCoinSlotOpen){
			m_pCommonModel->operationInfo.isCoinSlotOpen = false;
			COIN_HELPER->CH_StopInsertInfo();
			COIN_HELPER->CH_EndReceive();
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}

	try{
		// ��ֽ�ҿ�
		if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
			m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
			BANKNOTE_HELPER->StopValidation();
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}
	try{
		// ֹͣ����ֵ��
		if (m_pCommonModel->operationInfo.isWaitingEPCard){
			RECHARGERW_HELPER->StopReadCard();
			m_pCommonModel->operationInfo.isWaitingEPCard = false;
		}
		// ֹͣ�����п�,�˳����п�
		if (m_pCommonModel->operationInfo.isBankCardSlotOpen) {
			//BANKCARD_HELPER->TurnOffInsertTrigger(); // �����п�,�رս���
			m_pCommonModel->operationInfo.isBankCardSlotOpen = false;
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){

	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �Ƿ�Ͷ���㹻���ֽ�

@param      CRWReadCard * ��������

@retval     ��

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsReceiveEnoughMoney()
{
	bool res = false;
	EnterCriticalSection(&m_critical_Section); //?? ���ʲô�á��ǲ��ǿ��Գ��������������ֱ����OnCoinInserted֮�������жϣ���
	if (m_pCommonModel->paymentInfo.lChangeAmount >= 0) { // NotifyDataModelChanged();�Ѿ�������m_pCommonModel->paymentInfo.lChangeAmount
		res = true;
	}
	LeaveCriticalSection(&m_critical_Section);
	return res;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief  ��OnSuccessFinishʹ�õ�һ��������װ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::FlowToFinishDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ת��Ĭ��ҵ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::FlowToDefaultDlg(){

}
//////////////////////////////////////////////////////////////////////////
/**
@brief   ��OnErrorFinish,OnCancel�͸��ֳ���ʹ�õ�һ��������װ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::FlowToErrorDlg()
{
	try{
		// ��ֹ��¼д�ظ�
		if(m_pCommonModel->operationInfo.finishType != TRANSACTION_STATE_FAILED && 
			m_pCommonModel->operationInfo.finishType != NOT_ENOUGH_TICKETS_FOR_BOOKING && !m_pCommonModel->isInsertToDatabase){
				// ȡ������Ҳ��Ҫ��¼�����ݿ���
				Data::TransactionRecord dbInfo;
				WriteDBData(dbInfo); // д�����ݿ����ϸ��
				DATA_HELPER->DbInsertTransactionInfo(dbInfo);
				m_pCommonModel->isInsertToDatabase = true;
		}		
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ����ƾ֤

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::PrinterPrintReceipt()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ӡ����ƾ֤

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::PrinterPrintErrorReceipt()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ�ҽ��ܽ����Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewCoinReceivedAmount(tCHACoinValueInfo chAmountInfo)
{
	// �����������ִ�ж��
	m_pCommonModel->paymentInfo.PayCoinInfo.clear();
	m_pCommonModel->paymentInfo.PayCoinInfo[Coin1Yuan] = chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	m_pCommonModel->paymentInfo.PayCoinInfo[CoinHalfYuan] = chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;

	m_pCommonModel->paymentInfo.lCHReceiveTotalAmount = 0;
	m_pCommonModel->paymentInfo.lCHReceiveTotalAmount += 100*chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	m_pCommonModel->paymentInfo.lCHReceiveTotalAmount += 50*chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;
	CTCoinCountInfo::COIN_TEMPORARY1_INFO HopperB = theCOIN_COUNT.GetCoinTemporary1Info();
	//CTCoinCountInfo::COIN_TEMPORARY2_INFO HopperA = theCOIN_COUNT.GetCoinTemporary2Info();
	HopperB.ulCount_1yuan = chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	HopperB.ulCount_5jiao = chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;
	theCOIN_COUNT.SetCoinTemporary1Info(HopperB);
	LOG("Ӳ��Ͷ���ܶ%d�֡�",m_pCommonModel->paymentInfo.lCHReceiveTotalAmount);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����ֽ�ҽ��ܽ����Ϣ

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewBanknoteReceivedAmount(tBncCashInfo& feedback)
{
	// since "bh_status" only give the information of the very last banknote, 
	// ÿ��Ͷ��һ��ֽ�Һ�,�������ֻ��ִ��һ��
	long recvAmount = 0;
	switch (feedback.psBncCashInfo[0].iValue){
	case Banknote1Yuan:
		recvAmount = 100;
		break;
	case Banknote5Yuan:
		recvAmount = 500;
		break;
	case Banknote10Yuan:
		recvAmount = 1000;
		break;
	case Banknote20Yuan:
		recvAmount = 2000;
		break;
	case Banknote50Yuan:
		recvAmount = 5000;
		break;
	case Banknote100Yuan:
		recvAmount = 10000;
		break;
	default:
		// ϵͳ�в����յ���ֵ
		recvAmount = 0;
	}
	LOG("Ͷ��ֽ�ң�%d Ԫ.",recvAmount/100);
	if (recvAmount != 0)
	{
		BankNoteAndCoinType_t type = (BankNoteAndCoinType_t)feedback.psBncCashInfo[0].iValue;
		m_pCommonModel->paymentInfo.PayBankNoteInfo[type] += 1;
		m_pCommonModel->paymentInfo.lBHReceiveTotalAmount += recvAmount; 
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡӲ�ҽ��ܸ���

@param      ��

@retval     Ӳ�ҽ��ܸ��� DWARD

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewCoinReceivedCount(tCHACoinValueInfo chAmountInfo)
{
	// �����������ִ�ж��
	m_pCommonModel->paymentInfo.lCHReceiveTotalCount = 0;
	m_pCommonModel->paymentInfo.lCHReceiveTotalCount += chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	m_pCommonModel->paymentInfo.lCHReceiveTotalCount += chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;
	// Ӳ���ݴ���1������Ϣ
	CTCoinCountInfo::COIN_TEMPORARY1_INFO cointemporary1;
	cointemporary1 = theCOIN_COUNT.GetCoinTemporary1Info();
	cointemporary1.ulCount_1yuan = chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	cointemporary1.ulCount_5jiao = chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;
	theCOIN_COUNT.SetCoinTemporary1Info(cointemporary1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ȡֽ�ҽ�������

@param      ��

@retval     ֽ�ҽ��ܸ��� DWORD

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewBanknoteReceivedCount(tBncCashInfo& bh_status)
{
	long recvAmount = 0;
	switch ((BankNoteAndCoinType_t)bh_status.psBncCashInfo[0].iValue){
	case Banknote1Yuan:
		recvAmount = 100;
		break;
	case Banknote5Yuan:
		recvAmount = 500;
		break;
	case Banknote10Yuan:
		recvAmount = 1000;
		break;
	case Banknote20Yuan:
		recvAmount = 2000;
		break;
	case Banknote50Yuan:
		recvAmount = 5000;
		break;
	case Banknote100Yuan:
		recvAmount = 10000;
		break;
	default:
		// ϵͳ�в����յ���ֵ
		recvAmount = 0;
	}
	if (recvAmount != 0)
	{
		// ÿ��Ͷ��һ��ֽ�Һ�,�������ֻ��ִ��һ��
		m_pCommonModel->paymentInfo.lBHReceiveTotalCount += 1;

	}
}

// �������
bool Compare(DWORD a,DWORD b){ return a > b;}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������ö��

@param      ��

@retval     true ���㹻��Ӳ�ҿɳɹ�����
false Ӳ�Ҳ����ɹ�����

@exception  ��changeAmount����,������ChangeStockCount/ChangeHopper1Count/ChangeHopper2Count
�˺���������������Ķ�Ӧ,����0����Ч,1��0.5MOP,2��1MOP,3��2MOP,4��5MOP
�������TVM.INI��Common/Source Files/TVMInfo.cpp,��������TransCoinValue��
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::CalculateChangeCount()
{
	// ��ʱ�Ȳ��ж��������Ƿ���λ��zhengxianle
	LOG("����������ϸ.");
	if(theSERVICE_MGR.GetCurService()->GetServiceID() == CHARGE_SVC){
		LOG("��ֵ����Ҫ���㣬���ء�");
		return true;
	}

	// 1. *** ��������
	long changeAmout = m_pCommonModel->paymentInfo.lChangeAmount;

	// 2. *** Ӳ�����
	// Ӳ�����
	int nTypeHopper1 = theTVM_INFO.GetCHHopper1Type();			// HopperA��
	int nTypeHopper2 = theTVM_INFO.GetCHHopper2Type();			// HopperB��
	int nTypeChangeBox1 = theTVM_INFO.GetCHChangeBoxAType();
	int nTypeChangeBox2 = theTVM_INFO.GetCHChangeBoxBType();
	DWORD dwHopper1Value = TransCoinValue(nTypeHopper1);		// 100�֣��ұߣ�
	DWORD dwHopper2Value = TransCoinValue(nTypeHopper2);		// 100 �֣���ߣ�
	DWORD dwChangeBox1Value = TransCoinValue(nTypeChangeBox1);	// 100 �֣��ұߣ�
	DWORD dwChangeBox2Value = TransCoinValue(nTypeChangeBox2);	// 100�֣���ߣ�
	// Ӳ������
	DWORD dwHopper1Count = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;
	DWORD dwHopper2Count = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;
	DWORD dwChangeBox1Count = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
	DWORD dwChangeBox2Count = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
	// Ӳ�����������������
	DWORD dwCoinRealMaxAmount = dwHopper1Value * dwHopper1Count + dwHopper2Value * dwHopper2Count + dwChangeBox1Value * dwChangeBox1Count + dwChangeBox2Value * dwChangeBox2Count;
	DWORD dwCoinRealMaxCount  = dwHopper1Count + dwHopper2Count + dwChangeBox1Count + dwChangeBox2Count;

	// 3. *** ֽ�����
	// ֽ�����(һ�������䣬4��ѭ����)
	int nTypeBNRRecycleBoxA = theTVM_SETTING.GetBNRRecycleBoxAAmount();
	int nTypeBNRRecycleBoxB = theTVM_SETTING.GetBNRRecycleBoxBAmount();
	int nTypeBNRRecycleBoxC = theTVM_SETTING.GetBNRRecycleBoxCAmount();
	int nTypeBNRRecycleBoxD = theTVM_SETTING.GetBNRRecycleBoxDAmount();
	int nTypeBNRChangeBox = theTVM_SETTING.GetBNRChangeBoxAmount();
	DWORD dwBNRRecycleBoxAValue = TransBankNoteValue(nTypeBNRRecycleBoxA);	// 100��
	DWORD dwBNRRecycleBoxBValue = TransBankNoteValue(nTypeBNRRecycleBoxB);	// 100��
	DWORD dwBNRRecycleBoxCValue = TransBankNoteValue(nTypeBNRRecycleBoxC);	// 500��
	DWORD dwBNRRecycleBoxDValue = TransBankNoteValue(nTypeBNRRecycleBoxD);	// 500��
	DWORD dwBNRChangeBoxValue = TransBankNoteValue(nTypeBNRChangeBox);		// 500��
	// ֽ������
	DWORD dwBNRRecycleBoxACount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount;
	DWORD dwBNRRecycleBoxBCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount;
	DWORD dwBNRRecycleBoxCCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount;
	DWORD dwBNRRecycleBoxDCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount;
	DWORD dwBNRChangeBoxCount = 0;//theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().ulCurCount;����ֽ��ģ�������䲻֧������
	// ֽ�����������������
	DWORD dwBanknoteRealMaxAmount = dwBNRRecycleBoxACount * dwBNRRecycleBoxAValue
		+ dwBNRRecycleBoxBCount * dwBNRRecycleBoxBValue
		+ dwBNRRecycleBoxCCount * dwBNRRecycleBoxCValue
		+ dwBNRRecycleBoxDCount * dwBNRRecycleBoxDValue
		+ dwBNRChangeBoxCount * dwBNRChangeBoxValue;
	DWORD dwBanknoteRealMaxCount  = dwBNRRecycleBoxACount 
		+ dwBNRRecycleBoxBCount 
		+ dwBNRRecycleBoxCCount 
		+ dwBNRRecycleBoxDCount 
		+ dwBNRChangeBoxCount;

	// 4. *** ������������Ƿ��зǷ��ġ�
	if(dwHopper1Value == 0 || dwHopper2Value == 0 || dwChangeBox1Value == 0 || dwChangeBox2Value == 0 
		|| dwBNRRecycleBoxAValue == 0 || dwBNRRecycleBoxBValue == 0 || dwBNRRecycleBoxCValue == 0 || dwBNRRecycleBoxDValue == 0 || dwBNRChangeBoxValue == 0){
			LOG("�����ļ����зǷ������");
			return false;
	}

	// 5. *** �����޶�
	DWORD dwMaxBanknoteChangeAmout = theAFC_TVM_OPERATION.GetBHChangeMaxAmount() * 100;		// ֽ��������㣺���(��λ����)
	DWORD dwMaxBanknoteChangeCount = theAFC_TVM_OPERATION.GetBHChangeMaxPiece();			// ֽ��������㣺����
	DWORD dwMaxCoinChangeCount	   = theAFC_TVM_OPERATION.GetChageCoinMaxPieceOfIssue();	// Ӳ��������㣺����
	// Ӳ��ʵ�ʿ�������������ͽ��
	if((theTVM_STATUS_MGR.GetChangeMode()&DEVICE_CONTRL_CODE_NO_BILL_CHANGE) != DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
		dwMaxCoinChangeCount = 0;
	}
	else{
		dwMaxCoinChangeCount = dwCoinRealMaxCount< dwMaxCoinChangeCount ? dwCoinRealMaxCount : dwMaxCoinChangeCount;// ѡȡ�ɿ����Ӳ����������
	}
	// ֽ��ʵ�ʿ�������������ͽ��
	if((theTVM_STATUS_MGR.GetChangeMode() & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) != DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
		dwMaxBanknoteChangeAmout = 0;
		dwMaxBanknoteChangeCount = 0;
	}
	else{
		dwMaxBanknoteChangeCount = dwBanknoteRealMaxCount < dwMaxBanknoteChangeCount ? dwBanknoteRealMaxCount : dwMaxBanknoteChangeCount;
		dwMaxBanknoteChangeAmout = dwBanknoteRealMaxAmount < dwMaxBanknoteChangeAmout ? dwBanknoteRealMaxAmount : dwMaxBanknoteChangeAmout;
	}


	CString strBoxInfo;
	LOG("��Ǯ�������Ϣ��");
	strBoxInfo.Format(_T("Ӳ��������1��%d����������2��%d����Hopper1��%d����Hopper2��%d��"), dwChangeBox1Value, dwChangeBox2Value, dwHopper1Value, dwHopper2Value);  
	LOG("Ӳ��---%s",strBoxInfo);
	strBoxInfo.Format(_T("ֽ��ѭ���ģ�%d����ֽ��ѭ����B��%d����ֽ��ѭ����C��%d����ֽ��ѭ����D��%d���� ֽ�������䣨%d��"), 
		dwBNRRecycleBoxAValue,
		dwBNRRecycleBoxBValue,
		dwBNRRecycleBoxCValue,
		dwBNRRecycleBoxDValue,
		dwBNRChangeBoxValue);
	LOG("ֽ��---%s",strBoxInfo);

	LOG("��Ǯ�������Ϣ��");
	// Ӳ��
	strBoxInfo.Format(_T("Ӳ����1��%d; Ӳ����2��%d������������1 %d����������2 %d����Hopper1 %d����Hopper2 %d��"), 
		dwHopper1Count + dwChangeBox1Count, 
		dwHopper2Count + dwChangeBox2Count, 
		dwChangeBox1Count,
		dwChangeBox2Count,
		dwHopper1Count,
		dwHopper2Count					
		);
	LOG("Ӳ��---%s",strBoxInfo);
	// ֽ��
	strBoxInfo.Format(_T("ֽ��ѭ����A��%d; ֽ��ѭ����B��%d; ֽ��ѭ����C��%d; ֽ��ѭ����D��%d; ֽ�������䣺%d"), 
		dwBNRRecycleBoxACount,
		dwBNRRecycleBoxBCount,
		dwBNRRecycleBoxCCount,
		dwBNRRecycleBoxDCount,
		dwBNRChangeBoxCount);
	LOG("ֽ��---%s",strBoxInfo);


	// 6. *** ����Ƿ��㹻����
	if((dwMaxBanknoteChangeAmout + (dwMaxCoinChangeCount > 0 ? dwCoinRealMaxAmount : 0))<changeAmout){
		LOG("�ܽ��������㲻�㡣");
		return false;
	}

	// 7. *** ����ֽ�����ӵ�����˳���������
	// ����1��ֽ�����ȣ�2����������ȣ�3������������
	// ����ֽ��ѭ�����������
	vector<DWORD> vec_value_type;
	vec_value_type.push_back(dwBNRRecycleBoxAValue);
	vec_value_type.push_back(dwBNRRecycleBoxBValue);
	vec_value_type.push_back(dwBNRRecycleBoxCValue);
	vec_value_type.push_back(dwBNRRecycleBoxDValue);
	sort(vec_value_type.begin(),vec_value_type.end(),Compare);	// ����(500 -> 500 -> 100 -> 100)
	// ��������˳�����

	list<CHANGE_VALUE> list_Priority;
	bool bCheckChangeBox = false,bCheckRecycleBoxA = false,bCheckRecycleBoxB = false,bCheckRecycleBoxC = false,bCheckRecycleBoxD = false;
	vector<DWORD>::iterator vit = vec_value_type.begin();
	while(vit != vec_value_type.end()){
		if(*vit == dwBNRRecycleBoxAValue && !bCheckRecycleBoxA && dwBNRRecycleBoxAValue <= changeAmout){
			bCheckRecycleBoxA = true;
			list_Priority.push_back(CHANGE_VALUE(BNR_RECYCLER_BOX_3, dwBNRRecycleBoxAValue, dwBNRRecycleBoxACount, 0));
		}
		else if(*vit == dwBNRRecycleBoxBValue && !bCheckRecycleBoxB && dwBNRRecycleBoxBValue <= changeAmout){
			bCheckRecycleBoxB = true;
			list_Priority.push_back(CHANGE_VALUE(BNR_RECYCLER_BOX_4, dwBNRRecycleBoxBValue, dwBNRRecycleBoxBCount, 0));
		}
		else if(*vit == dwBNRRecycleBoxCValue && !bCheckRecycleBoxC && dwBNRRecycleBoxCValue <= changeAmout){
			bCheckRecycleBoxC = true;
			list_Priority.push_back(CHANGE_VALUE(BNR_RECYCLER_BOX_5, dwBNRRecycleBoxCValue, dwBNRRecycleBoxCCount, 0));
		}
		else if(*vit == dwBNRRecycleBoxDValue && !bCheckRecycleBoxD && dwBNRRecycleBoxDValue <= changeAmout){
			bCheckRecycleBoxD = true;
			list_Priority.push_back(CHANGE_VALUE(BNR_RECYCLER_BOX_6, dwBNRRecycleBoxDValue, dwBNRRecycleBoxDCount, 0));
		}
		*vit++;
	}
	// ��ǰ�����ֽ��������
	list_Priority.push_front(CHANGE_VALUE(BNR_CHANGE_BOX, dwBNRChangeBoxValue, dwBNRChangeBoxCount,0));
	//  ��������˳��������������ķ���	
	long nRemainChange = changeAmout; //����ʣ��ֵ
	DWORD nBanknoteCount = 0;
	long nBanknoteAmount = 0;
	list<CHANGE_VALUE>::iterator it = list_Priority.begin();
	list<CHANGE_VALUE>::iterator ie = list_Priority.end();
	while (it!=ie){
		if (it->nCurCount>0){
			long nCurCount = nRemainChange/it->dwValue;
			nCurCount = min(nCurCount, it->nCurCount);
			if ((nBanknoteCount + nCurCount > dwMaxBanknoteChangeCount) || (nBanknoteAmount + nCurCount * it->dwValue > dwMaxBanknoteChangeAmout)){
				// �������������
				break;
			}
			it->nMoneyCount = nCurCount;
			nRemainChange -= nCurCount * it->dwValue;
			nBanknoteCount += nCurCount;
			nBanknoteAmount += nCurCount * it->dwValue;


			if (nRemainChange<=0){
				break;
			}
		}
		it++;
	}


	// 8. *** ����Ӳ�����ӵ�����˳���������
	// ����1��ֽ�����ȣ�2����������ȣ�3������������
	// ����Ӳ�Ҹ�����
	list<CHANGE_VALUE> list_CoinPriority;
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_CHANGE_BOX_2, dwChangeBox2Value, dwChangeBox2Count, 0));
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_CHANGE_BOX_1, dwChangeBox1Value, dwChangeBox1Count, 0));
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_HOPPER_2, dwHopper2Value, dwHopper2Count, 0));
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_HOPPER_1, dwHopper1Value, dwHopper1Count, 0));

	//  ��������˳��������������ķ���	
	DWORD nCoinCount = 0;
	long nCoinAmount = 0;
	it = list_CoinPriority.begin();
	ie = list_CoinPriority.end();
	while (it!=ie){
		if (it->nCurCount>0){
			long nCurCount = nRemainChange/it->dwValue;
			nCurCount = min(nCurCount, it->nCurCount);
			if ((nCoinCount + nCurCount > dwMaxCoinChangeCount)/* || (nCoinAmount + nCurCount * it->dwValue > dwMaxCoinChangeAmout)*/){
				// �������������
				m_pCommonModel->operationInfo.finishType = OVER_CHANGE_COUNT_LIMIT;
				return true;
			}
			it->nMoneyCount = nCurCount;
			nRemainChange -= nCurCount * it->dwValue;
			nCoinCount += nCurCount;
			nCoinAmount += nCurCount * it->dwValue;


			if (nRemainChange<=0){
				break;
			}
		}
		it++;
	}


	if(nRemainChange <= 0){
		// 9. *** ��������������
		list_Priority.merge(list_CoinPriority);
		it = list_Priority.begin();
		ie = list_Priority.end();
		long tmp_oneYuanCount = 0;
		long tmp_halfYuanCount = 0;
		while (it!=ie){
			if(it->BoxType==BNR_CHANGE_BOX){
				m_pCommonModel->paymentInfo.dwChangeBNRBoxCount = it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[BNR_RECYCLER_BOX_3] = it->nMoneyCount;
			}
			else if(it->BoxType==BNR_RECYCLER_BOX_3){
				m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount = it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[BNR_RECYCLER_BOX_3] = it->nMoneyCount;;
			}
			else if(it->BoxType==BNR_RECYCLER_BOX_4){
				m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount = it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[BNR_RECYCLER_BOX_4] = it->nMoneyCount;
			}
			else if(it->BoxType==BNR_RECYCLER_BOX_5){
				m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount = it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[BNR_RECYCLER_BOX_5] = it->nMoneyCount;
			}
			else if(it->BoxType==BNR_RECYCLER_BOX_6){
				m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount = it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[BNR_RECYCLER_BOX_6] = it->nMoneyCount;
			}

			else if(it->BoxType==COIN_CHANGE_BOX_1){
				tmp_oneYuanCount += it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1] = it->nMoneyCount;;
			}
			else if(it->BoxType==COIN_CHANGE_BOX_2){
				tmp_halfYuanCount += it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_2] = it->nMoneyCount;
			}
			else if(it->BoxType==COIN_HOPPER_1){
				tmp_oneYuanCount += it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1] = it->nMoneyCount;
			}
			else if(it->BoxType==COIN_HOPPER_2){
				tmp_halfYuanCount += it->nMoneyCount;
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2] = it->nMoneyCount;
			}
			it++;
		}

		m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount = tmp_oneYuanCount;//�����һԪָ�����ұ����ӣ�5��ָ������ߵ�����
		m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount = tmp_halfYuanCount;

		m_pCommonModel->paymentInfo.dwChangeBHAmout = 
			dwBNRChangeBoxValue * m_pCommonModel->paymentInfo.dwChangeBNRBoxCount +
			dwBNRRecycleBoxAValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount +
			dwBNRRecycleBoxBValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount +
			dwBNRRecycleBoxCValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount +
			dwBNRRecycleBoxDValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount;

		// 10. *** ��ӡ��־
		CString strChangeInfo;
		LOG("������Ϣ��");
		// Ӳ��
		strChangeInfo.Format(_T("Ӳ����1��%d; Ӳ����2��%d������������1 %d����������2 %d����Hopper1 %d����Hopper2 %d��"), 
			m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount, 
			m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount,
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1],
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_2],			
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1],
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2]				
		);
		LOG("Ӳ��---%s",strChangeInfo);
		// ֽ��
		strChangeInfo.Format(_T("ֽ��ѭ����A��%d; ֽ��ѭ����B��%d; ֽ��ѭ����C��%d; ֽ��ѭ����D��%d; ֽ�������䣺%d"), 
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount,
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount,
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount,
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount,
			m_pCommonModel->paymentInfo.dwChangeBNRBoxCount);
		LOG("ֽ��---%s",strChangeInfo);

		return true;
	}
	// ���ߵ���˵����������㹻������Ӳ�������������ޡ�


	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�Ӳ��ֽ�Ҷ��ﵽ�������,��OnCoinInserted��OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CPassengerForeSvc::IsBothCoinBanknoteMaxCount() {
	// since it is a virtual function, the return value doesn't really matter.
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ֽ�Ҵﵽ�������,��OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

bool CPassengerForeSvc::IsBanknoteMaxCount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�Ӳ�Ҵﵽ�������,��OnCoinInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsCoinMaxCount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ�ֽ�Ҷ��ﵽ�����,��OnBanknoteInserted��

@param      ��

@retval     bool

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsBanknoteMaxAmount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж����������Ƿ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsCoinChageMaxCount(){
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�ֽ�����������Ƿ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsBHChangeMaxCount(){
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж�ֽ���������Ƿ���

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsBHChangeMaxAmout(){
	return true;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ת����ɽ���,��Ҫ�жϵ�ǰ��״̬,����״̬������Ӧ�Ľ���

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnReturnFinish(){
	try{
		StopCircleReadCard();
		LOG("------------------------------------������ɽ���------------- <IN>");
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		// ȡ����ֻ���������һ�Σ�����
		if (theSERVICE_MGR.GetState() == CANCELING && m_bIsPassengerCanceled){	
			m_bIsPassengerCanceled = false;
			// ���ֽ���ȡ��
			if(m_pCommonModel->paymentInfo.paymentMethod != PaymentMethod_t::PAYMENT_METHOD_CASH){
				if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN|| m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
					LOG("��ά�빺Ʊ���׳˿�ȡ����Leave");
					FlowToErrorDlg();
					return;
				}
				if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN|| m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
					LOG("��ά�빺Ʊ���׳˿�ȡ����Leave");
					FlowToErrorDlg();
					return;
				}
				if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
					m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
					BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
				}
				LOG("����Ĭ��ҵ��");
				pMainSvc->StartDefaultService();
				return;
			}
			// �ֽ���
			else {
				// 0.�˱����
				if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_RETURNED ||
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURNED &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE ||
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURNED &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_RETURNED){
						LOG("����Ĭ��ҵ��");
						if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
							m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
							BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
						}
						pMainSvc->StartDefaultService();
						return;
				}
				// 1.����δ����ȡ��,���ص�Ĭ��ҵ��
				if (!m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen &&
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE)
				{
					LOG("����δ����ȡ��,���ص�Ĭ��ҵ��(ȡ��)��");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
// 					pMainSvc->StartDefaultService();//��
				}
				// 2.���׽���δͶ��ȡ��
				else if ((m_pCommonModel->operationInfo.isCoinSlotOpen || m_pCommonModel->operationInfo.isBankNoteSlotOpen) &&
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE)
				{
					LOG("���׽���δͶ��ȡ��(ȡ��)��");
					// �ر����н��տ�,�Զ������ﷵ��Ĭ�Ͻ���
					CloseAllDevice();
				}
				// 3.���׽���Ͷ��ȡ��:��������
				else if (m_pCommonModel->operationInfo.isCoinSlotOpen && m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED ||
					m_pCommonModel->operationInfo.isBankNoteSlotOpen && m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)
				{
					LOG("���׽���Ͷ��ȡ��:��������(ȡ��)");
					// �رղ��˱�,�ڻص������з��ء�
					CloseAllDevice();
					if (m_bRechargeOverlimit){
						m_bRechargeOverlimit = false;
						// �˱Ҳ���
						ReturnWhenBanknotesInserted();
					}
				}
				// 4.���׽���,Ͷ��ȡ�������ֳ���
				else if((!m_pCommonModel->operationInfo.isCoinSlotOpen && m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED &&
					m_pCommonModel->operationInfo.isBankNoteSlotOpen )|| (m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen &&
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
						LOG("���׽���,Ͷ��ȡ�������ֳ���(ȡ��)");
						// 4.1�رս��տ�,�������
						CloseAllDevice();
						// 4.2 ���Ӳ����������,��ֽ�����ڽ���,�˳�Ӳ��
						if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
							m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
							COIN_HELPER->CH_RefundAsync(this);
						}
						// 4.3 ���ֽ����������,��Ӳ���ڽ���,�˳�ֽ��
						if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
							if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
								m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
								BANKNOTE_HELPER->ReturnAllAsync(this);
							}
						}
				}
				// 5.���׽���Ͷ�����ȡ��
				else if (!m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen && 
					(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED || 
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)&& 
					m_pCommonModel->operationInfo.isFinishedReceveMoney){
						LOG("���׽���Ͷ�����ȡ��(ȡ��)");
						// ��Ǯ
						StartReturnCash();
				}
				// 6.�������ȡ������(������������)
				else if ((m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_COLLECTED || 
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_COLLECTED)&& 
					m_pCommonModel->operationInfo.coinChangeState != COIN_CHANGE_STATE_NONE)
				{
					LOG("�������ȡ������(ȡ��)");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
					pMainSvc->StartDefaultService();
				}
			}
			LOG("------------------------------------������ɽ���------------- <OUT1>");
		}
		// ����ҵ���߼�:����
		else {
			//if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_ACC_CARD || m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_YKT_CARD){
			//����������֧����ʽȡ����TPU���صĿ��������ֶΣ����ɿ�����˲������������Ϊ�����ж����ݡ�
			if ( m_ePayType==PAY_WITH_SVT){
				if (m_pCommonModel->operationInfo.finishType == TRANSACTION_STATE_FAILED){
					LOG("��ֵƱ��Ʊ����ʧ�ܣ�Leave");
					OnErrorFinish();
				}
				else if(m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME){
					LOG("��ֵƱ��Ʊ���׳�ʱȡ����Leave");
					FlowToErrorDlg();
				}
				else if (m_pCommonModel->operationInfo.finishType ==DEFAULT){
					LOG("��ֵƱ��Ʊ����������ɣ�Leave");
					OnSuccessFinish();
				}
				LOG("------------------------------------������ɽ���------------- <OUT>��ֵƱ");
				return;
			}
			if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN|| m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
				if(m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME){
					LOG("��ά�빺Ʊ���׳�ʱȡ����Leave");
					FlowToErrorDlg();
				}
				else if(m_pCommonModel->operationInfo.finishType == TRANSACTION_STATE_FAILED){
					LOG("��ά�빺Ʊ����ʧ�ܣ�Leave");
					FlowToErrorDlg();	
				}
				else if (m_pCommonModel->operationInfo.finishType ==DEFAULT){
					LOG("��ά�빺Ʊ���׳ɹ���Leave");
					OnSuccessFinish();	
				}
				LOG("------------------------------------������ɽ���------------- <OUT>��ά��");
				return;
			}

			// ����δ���,�򷵻�
			if(m_pCommonModel->lDeviceStatus != DEVICE_ALL_OK){
				LOG("����δ��ɣ����ؼ����ȴ���%d",m_pCommonModel->lDeviceStatus);
				return;
			}

			//if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
			//	if(m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
			//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
			//		BANKNOTE_HELPER->Collect();
			//	}
			//}

			// ���쳣���쳣����,���߳�ʱ��
			if(m_pCommonModel->operationInfo.mapException.size() > 0 /*|| m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME*/){
				OnErrorFinish();
				if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
					m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
					BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
				}
				LOG("------------------------------------������ɽ���------------- <OUT>");
				return;
			}
			// �����Ƿ�����أ�
			//else if(m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_SUCCESS && m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_SUCCESS){
			//	if(m_pCommonModel->operationInfo.finishType == TRANSACTION_STATE_FAILED || m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME){
			//		OnErrorFinish();
			//		return;
			//	}
			//}
			//// ����ʧ�ܣ�����
			//else if(m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_FAIL || m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_FAIL){
			//	OnErrorFinish();
			//	return;
			//}

			if(BANKNOTE_ACCEPT_STATE_COLLECTED==m_pCommonModel->operationInfo.banknoteAcceptState && GetServiceID()==CHARGE_SVC){
				LOG("�����������");
				OnSuccessFinish();
			}


			// 1.�������,����������
			if (COIN_CHANGE_STATE_SUCCESS		== m_pCommonModel->operationInfo.coinChangeState && 
				BANKNOTE_CHANGE_STATE_SUCCESS   == m_pCommonModel->operationInfo.banknoteChangeState &&
				(BANKNOTE_ACCEPT_STATE_NONE		== m_pCommonModel->operationInfo.banknoteAcceptState ||
				BANKNOTE_ACCEPT_STATE_COLLECTED == m_pCommonModel->operationInfo.banknoteAcceptState ||
				BANKNOTE_ACCEPT_STATE_INSERTED  == m_pCommonModel->operationInfo.banknoteAcceptState || 
				BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED == m_pCommonModel->operationInfo.banknoteAcceptState)){
					LOG("�����������");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
					OnSuccessFinish();
			}
			// 2.����ʧ��,���ش���ҳ��,��ӡƾ֤
			else if((COIN_CHANGE_STATE_FAIL			== m_pCommonModel->operationInfo.coinChangeState ||
				BANKNOTE_CHANGE_STATE_FAIL		== m_pCommonModel->operationInfo.banknoteChangeState)&& 
				(BANKNOTE_ACCEPT_STATE_NONE		== m_pCommonModel->operationInfo.banknoteAcceptState  ||
				BANKNOTE_ACCEPT_STATE_COLLECTED  == m_pCommonModel->operationInfo.banknoteAcceptState  ||
				BANKNOTE_ACCEPT_STATE_INSERTED   == m_pCommonModel->operationInfo.banknoteAcceptState || 
				BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED == m_pCommonModel->operationInfo.banknoteAcceptState)){
					LOG("����ʧ��,���ش���ҳ��,��ӡƾ֤");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
					OnErrorFinish();
			}
			// 3.��Ǯ�ɹ�:ֻ��Ӳ��,����ֻ��ֽ�ҽ���,���߶��߶��˱ҳɹ�
			if ((COIN_ACCEPT_STATE_RETURNED	== m_pCommonModel->operationInfo.coinAcceptState &&	
				BANKNOTE_ACCEPT_STATE_NONE      == m_pCommonModel->operationInfo.banknoteAcceptState && 
				!m_pCommonModel->operationInfo.isBankNoteSlotOpen) ||
				(COIN_ACCEPT_STATE_NONE	    == m_pCommonModel->operationInfo.coinAcceptState &&	
				BANKNOTE_ACCEPT_STATE_RETURNED  == m_pCommonModel->operationInfo.banknoteAcceptState && 
				!m_pCommonModel->operationInfo.isCoinSlotOpen)     ||
				(COIN_ACCEPT_STATE_RETURNED == m_pCommonModel->operationInfo.coinAcceptState &&  
				BANKNOTE_ACCEPT_STATE_RETURNED == m_pCommonModel->operationInfo.banknoteAcceptState && 
				!m_pCommonModel->operationInfo.isBankNoteSlotOpen && !m_pCommonModel->operationInfo.isCoinSlotOpen)){
					LOG("��Ǯ�ɹ�:ֻ��Ӳ��,����ֻ��ֽ�ҽ���,���߶��߶��˱ҳɹ�");
					if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR && m_pCommonModel->paymentInfo.lPaidAmount == 0)
						m_pCommonModel->operationInfo.finishType = PASSAGE_CANCEL;
					else if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR && m_pCommonModel->paymentInfo.lBHReceiveTotalCount >= 15){
						m_pCommonModel->operationInfo.finishType = OVER_MONEY_AMOUNT_LIMIT;
					}
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
					FlowToErrorDlg();
			}
			// 4.��Ǯʧ�ܣ�ֽ�һ���Ӳ��ʧ��,��ӡƾ֤
			if (COIN_ACCEPT_STATE_RETURN_FAILD == m_pCommonModel->operationInfo.coinAcceptState && BANKNOTE_ACCEPT_STATE_INSERTED != m_pCommonModel->operationInfo.banknoteAcceptState ||
				BANKNOTE_ACCEPT_STATE_RETURN_FAILED == m_pCommonModel->operationInfo.banknoteAcceptState && COIN_ACCEPT_STATE_INSERTED != m_pCommonModel->operationInfo.coinAcceptState){
					LOG("��Ǯʧ�ܣ�ֽ�һ���Ӳ��ʧ��,��ӡƾ֤");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
					OnErrorFinish();
			}
			// 5.�����û�н���Ǯ,�ҽ��տڶ��Ѿ��ر�,ֱ�ӷ���
			if (COIN_ACCEPT_STATE_NONE == m_pCommonModel->operationInfo.coinAcceptState && BANKNOTE_ACCEPT_STATE_NONE == m_pCommonModel->operationInfo.banknoteAcceptState &&
				!m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen){
					LOG("û�н���Ǯ,�ҽ��տڶ��Ѿ��ر�,ֱ�ӷ���");//Ӧ��ֱ������Ĭ��ҵ�񣡣���
					//FlowToErrorDlg();
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//�人MEIֽ��cashInStart״̬�޷���cancel��ȡ��������ʹ��cashInEnd��
					}
					theSERVICE_MGR.SetState(FINISH);
					//pMainSvc->StartDefaultService();
			}
			LOG("------------------------------------������ɽ���------------- <OUT2>");
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		LOG("�������쳣��");
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      �������Ӽ����е������еļ�ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::InsertMoneyNeedStartTimer(){
	//LOG("���������м�ʱ����");
	//// ʱ��
	//int nTimerTime = theAFC_TVM_OPERATION.GetCancelDealTime();
	//// Default time
	//if(0 == nTimerTime){
	//	nTimerTime = 60;// ��ʾ������
	//}
	//else{
	//	StartTimer(TIMER_COUNTING_TO_RETURN,nTimerTime);	
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ�����е������еļ�ʱ��,�������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::EndInsertTimer(){
	//LOG("ֹͣ�����м�ʱ��");
	//// ֹͣ�����м�ʱ��
	//StopTimer(TIMER_COUNTING_TO_RETURN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �������Ӽ����е������еļ�ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::InsertRecvStartTimer(){
	LOG("���������м�ʱ����");
	// ʱ��
	int nTimerTime = 60/*theAFC_TVM_OPERATION.GetCancelDealTime()*/;
	// Default time
	if(0 == nTimerTime)
		nTimerTime = 60;// ��ʾ������
	else{
		m_timerInterval = nTimerTime;
		UpdateReceptDlg(_T(""));
		StartTimer(TIMERS_COUNTING_BEFORE_ISSUE,1);	
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ�����е������еļ�ʱ��,�������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::EndRecvTimer(){
	LOG("ֹͣ�����м�ʱ��");
	// ֹͣ�����м�ʱ��
	StopTimer(TIMERS_COUNTING_BEFORE_ISSUE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ�����е������еļ�ʱ��,�������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::ResetMoneyNeedTimer()
{
	EndInsertTimer();
	InsertMoneyNeedStartTimer();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �ж��Ƿ��˱ҳɹ�

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnTimer(UINT nEventID){
	try{
		// 1.�����е������еļ�ʱ���� 
		if (nEventID == TIMER_COUNTING_TO_RETURN){
			LOG("�ȴ������ֽ��ʱ����ʱ,��Ǯ��");
			// ����״̬Ϊȡ����
			theSERVICE_MGR.SetState(CANCELING);
			// ��������ذ�ť
			CReceptionDlg* pPassDlg = GetDialogFlow()->GetCurReceptionDlg();
			if(NULL != pPassDlg){
				pPassDlg->HideCompoment(CReceptionDlg::BTN_OK);
			}
			// ���õ�ǰ״̬Ϊȡ��״̬��
			//theSERVICE_MGR.SetState(CANCELING);
			/*if(theAPP_SESSION.IsBlindManOperation()){
			StopTimer(TIMERS_OUT_OF_REPLAY);
			StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
			}*/
			StopTimer(TIMER_COUNTING_TO_RETURN);							// ������ʱ
			m_pCommonModel->operationInfo.finishType = INSERT_OUT_OF_TIME;
			//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);		// ��ʾ������ȡ��,�ȴ�Ͷ�ҳ�ʱ
			//CloseAllDevice();												// ��ʱ���ڵȴ�״̬,ֹͣʶ�����,׼���˱ҡ�
			//  1.�˳�Ӳ��,��������ˡ�
			if (m_pCommonModel->operationInfo.isCoinSlotOpen){
				m_pCommonModel->operationInfo.isCoinSlotOpen = false;
				COIN_HELPER->CH_StopInsertInfo();
				COIN_HELPER->CH_EndReceive();
			}
			if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
				m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
				COIN_HELPER->CH_RefundAsync(this);
			}
			// 2.�˳�ֽ��,���������
			if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
				m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
				BANKNOTE_HELPER->StopValidation();
			}
			if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
				if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
					m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
					BANKNOTE_HELPER->ReturnAllAsync(this);
				}
			}
			// 3.����
			OnReturnFinish();// �����������û��Ͷ�룿��ֱ�ӷ��أ���Ͷ��,�˱Ҵ����� !!һ����Ͷ��Ż�ִ�д˼�ʱ����
		}

		else if (nEventID == TIMERS_COUNTING_BEFORE_ISSUE){
			if (--m_timerInterval<0){
				LOG("��Ʊ������ʱ����ʱ,��Ǯ��");
				EndRecvTimer();
				if (m_ePayType == PAY_WITH_SVT){
					// ֹͣ��ֵ����Ʊ��ѭ������
					m_bStopCircleRead = true;
				}
				else if (m_ePayType == PAY_WITH_WEIXIN || m_ePayType == PAY_WITH_ZHIFUBAO){
					// ���繺Ʊ,��ֹͣ������ѯ���ٷ��Ͷ���ȡ��
					if (m_pCommonModel->operationInfo.finishType != CPassengerForeSvc::QR_ORDER_GENERATE_FAILED){
						EndQueryOrderTimer();	
						m_bWebPaySuccess = false;//��ֹȡ��ǰ�������Ѿ�֧���ɹ�
						CancelOrder();
					}
				} 

				// ����״̬Ϊȡ����
				theSERVICE_MGR.SetState(CANCELING);
				// ��������ذ�ť
				CReceptionDlg* pPassDlg = GetDialogFlow()->GetCurReceptionDlg();
				if(NULL != pPassDlg){
					pPassDlg->HideCompoment(CReceptionDlg::BTN_OK);
					pPassDlg->HideCompoment(CReceptionDlg::BTN_OK_AND_CANCEL1);
					pPassDlg->HideCompoment(CReceptionDlg::BTN_OK_AND_CANCEL2);
				}
				// ���õ�ǰ״̬Ϊȡ��״̬��
				//theSERVICE_MGR.SetState(CANCELING);
				/*if(theAPP_SESSION.IsBlindManOperation()){
				StopTimer(TIMERS_OUT_OF_REPLAY);
				StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
				}*/
				StopTimer(TIMER_COUNTING_TO_RETURN);							// ������ʱ
				m_pCommonModel->operationInfo.finishType = INSERT_OUT_OF_TIME;
				//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);		// ��ʾ������ȡ��,�ȴ�Ͷ�ҳ�ʱ
				//CloseAllDevice();												// ��ʱ���ڵȴ�״̬,ֹͣʶ�����,׼���˱ҡ�
				//  1.�˳�Ӳ��,��������ˡ�
				if (m_pCommonModel->operationInfo.isCoinSlotOpen){
					m_pCommonModel->operationInfo.isCoinSlotOpen = false;
					COIN_HELPER->CH_StopInsertInfo();
					COIN_HELPER->CH_EndReceive();
				}
				if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
					m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
					COIN_HELPER->CH_RefundAsync(this);
				}
				// 2.�˳�ֽ��,���������
				if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
					m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
					BANKNOTE_HELPER->StopValidation();
				}
				if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
					if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
						m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
						BANKNOTE_HELPER->ReturnAllAsync(this);
					}
				}
				// 3.����
				OnReturnFinish();// �����������û��Ͷ�룿��ֱ�ӷ��أ���Ͷ��,�˱Ҵ����� !!һ����Ͷ��Ż�ִ�д˼�ʱ����
			}
			else{
				// ���½��浹��ʱ��ʾ
				CString strShowTxt;
				strShowTxt.Format(_tl(TXT_COUNT_DOWN) + _T("%d"), m_timerInterval);
				UpdateReceptDlg(strShowTxt);				
			}
		}
		else if (nEventID == TIMERS_WEB_QUERY_ORDER){
			LOG("��ѯ������ʱ����");
			EndQueryOrderTimer();
			if(m_bWebPaySuccess){
				m_bWebPaySuccess = false;
				// ����֧���ɹ������۳�Ʊ
				m_pCommonModel->paymentInfo.lPaidAmount = m_pCommonModel->paymentInfo.lDueAmount;
				GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ISSUING,0,(LPARAM)0);
				return;
			}

			QueryOrder();		
			InsertQueryOrderTimer();
		}		
	}
	catch(CSysException& e){
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      Ӳ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartCHChange(){

	try{
		LOG("��ʼӲ�����㡣");

		if(m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount < 0){
			m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount = 0;
		}
		if(m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount < 0){
			m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount = 0;
		}

		if(m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount == 0 && m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount == 0){
			m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_SUCCESS;
			OnReturnFinish();
			return;
		}
		m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		COIN_HELPER->CH_ChangeAsync(this,m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount,m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount);
	}
	catch(CSysException& e){
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;
		m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_FAIL;
		OnError(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ֽ������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartBHChange(){

	try{
		LOG("��ʼֽ�����㡣");
		if(m_pCommonModel->paymentInfo.dwChangeBHAmout < 0){
			m_pCommonModel->paymentInfo.dwChangeBHAmout = 0;
		}

		if(m_pCommonModel->paymentInfo.dwChangeBHAmout == 0){
			m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;
			return;
		}

		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_CHANGE_BUSY;
		BANKNOTE_HELPER->DispenseAsync(this,m_pCommonModel->paymentInfo.dwChangeBHAmout);

		/*if(m_pCommonModel->paymentInfo.dwChangeBHBoxACount < 0){
		m_pCommonModel->paymentInfo.dwChangeBHBoxACount = 0;
		}
		if(m_pCommonModel->paymentInfo.dwChangeBHBoxBCount < 0){
		m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = 0;
		}

		if(m_pCommonModel->paymentInfo.dwChangeBHBoxACount == 0 && m_pCommonModel->paymentInfo.dwChangeBHBoxBCount == 0){
		m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;
		return;
		}*/
		//m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_CHANGE_BUSY;//lichao:
		//BH_CHANGE_HELPER->SendOutBill(this,m_pCommonModel->paymentInfo.dwChangeBHBoxACount,m_pCommonModel->paymentInfo.dwChangeBHBoxBCount);
		//BH_CHANGE_HELPER->DispenseAsync(this, );
	}
	catch(CSysException& e){
		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_CHANGE_BUSY;
		OnError(e);
	}
	catch(...){

	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      �˿Ͳ忨�ص�����

@param      UINT uiResult				(���ؽ��)

@param      CIOCardPosMonitor* pCommand (��������)

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::OnChargeInsertCard(UINT,CIOCardPosMonitor*){
//
//}



//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����ļ��еļ۸��

@param      ��

@retval     vector<int> m_vecTicketAmount;Ʊ�ۼ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
vector<int> CPassengerForeSvc::GetTicketAmountTable(){
// 	if(m_vecTicketAmount.size() != 0){
// 		return m_vecTicketAmount;
// 	}
	m_vecTicketAmount = theFunction_INFO.GetTicketAmount();
	return m_vecTicketAmount;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ�����ļ��еļ۸��

@param      ��

@retval     vector<int> m_vecTicketAmount;Ʊ�ۼ���

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
vector<int> CPassengerForeSvc::GetTicketCountTable(){
	//if(m_vecTicketCount.size() != 0){
	//	return m_vecTicketCount;
	//}
	m_vecTicketCount = theFunction_INFO.GetTicketCounts();

	return m_vecTicketCount;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��̬���IO״̬�߳�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CPassengerForeSvc::DoCircleReadCard(LPVOID lpParameter)
{
	CPassengerForeSvc* pSvc = (CPassengerForeSvc*)lpParameter;
	RW_READ_CARD_RESPONSE cardRspInfo;

	pSvc->m_bCircleReading = false;
	pSvc->m_strReadCardRsp = _T("");
	while(!pSvc->m_bStopCircleRead)
	{
		try{
			LOG("Circle_Read --- begin:");
			//pSvc->EmptyCardInfo();20180328����Ϊ����ʧ�ܲ�����ϴζ�����Ϣ�������ɹ�ֱ�Ӹ���
			long resultCode = -1;
			LOG("start read:");
			pSvc->m_bCircleReading = true;
			resultCode = RECHARGERW_HELPER->TryReadCard(pSvc->GetServiceID()==ISSUE_SVC?SVT_ISSUE_SVC:CHARGE_SVC,cardRspInfo,false);
			pSvc->m_bCircleReading = false;
			LOG("end read:");
			// ����ʧ��
			if(resultCode != RW_RSP_OK) {
				pSvc->EmptyCardInfo();

				LOG("����ʧ�ܡ�");
				// ȡ�����˳�����
				if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
					LOG("���������޿���");			
				}
				// ��������Ʊ
				else if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {
					LOG("������������");
					//pSvc->BlockCard();
					pSvc->m_strReadCardRsp = _tl(GUIDE_IN_BLACK_CARD_ERROR);
					//CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_tl(TXT_MSG_GUIDE_THIS_CARD_HAS_BEEN_LOCKED),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				}
				// Ѱ�������ʧ��
				else if((resultCode ==  RW_RSP_READERROR) || (resultCode == RW_RSP_FINDERROR) || (resultCode == RW_RSP_FIND_CARD_FAILED) || (resultCode == RW_RSP_READ_CARD_FAILED)){		

				}
				// Ʊ���������ֵ����
				else{
					CString strTemp = theTPUERROR_INFO.GetTpuErrorInfo(resultCode);
					LOG("Ʊ��������������");
					pSvc->m_strReadCardRsp = _tl(GUIDE_CARD_CAN_NOT_DO_BUSINESS);
					/*		CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),strTemp,TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);*/
				}
			}
			// �����ɹ�
			else{
				// �ж���������(�������Ͳ���ȷ)
				if(cardRspInfo.CardPHType != MIFARE1K && cardRspInfo.CardPHType != MIFARE4K && cardRspInfo.CardPHType != CPU_TICKET){
					LOG("Ʊ�����Ͳ���ȷ��");
					pSvc->EmptyCardInfo();
					/*		CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_tl(GUIDE_IN_CARD_TYPE_ERROR),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime());*/
					pSvc->m_strReadCardRsp = _tl(GUIDE_CARD_CAN_NOT_DO_BUSINESS);
				}
				else{
					// ����Ʊ����Ϣ�ɹ�
					if(!pSvc->SaveCardInfo(&cardRspInfo)){
						pSvc->m_readResponse = cardRspInfo;
					}
					else{
						pSvc->EmptyCardInfo();
					}
				}
			}
		}
		catch(CSysException& e){
			pSvc->m_bStopCircleRead = true;
			pSvc->m_bReaderBad = true;
			pSvc->m_strReadCardRsp = _tl(GUIDE_TPU_EXCEPTION);
			theEXCEPTION_MGR.ProcessException(e);
		}
		catch(...){
			pSvc->m_bStopCircleRead = true;
			pSvc->m_strReadCardRsp = _tl(GUIDE_TPU_EXCEPTION);
			LOG("Exception occure when Circle_Read!!!");
		}
		LOG("process read:");
		pSvc->NotifyDataModelChanged();
		LOG("Circle_Read --- end:");
		Sleep(1000);
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼѭ������

@param      ��

@retval     long

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::StartCircleReadCard()
{
	theSERVICE_MGR.SetState(COUNTING);
	DWORD dwThreadID;
	m_bStopCircleRead = false;
	HANDLE m_hHandle = CreateThread(NULL,0,DoCircleReadCard,this,0,&dwThreadID);

	if (m_hHandle == NULL){
		m_bStopCircleRead = true;
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��ʼѭ������

@param      ��

@retval     long

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::StopCircleReadCard()
{
	m_bStopCircleRead = true;
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      ֹͣ�����ֽ�

@param      ��

@retval     long

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::StopReceiveMoney()
{

	m_bRechargeConfirm = true;

	// ֹͣʶ��ֽ��
	if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		BANKNOTE_HELPER->StopValidation();
		if (m_bRechargeOverlimit){
			// ѹ��
			if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
				LOG("��ʼѺ��ֽ��,�ѽ���ֽ�ҡ�");
				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;// Ϊ�˲��������ط�ͬ��ѹ����ͻ�������첽ѹ��ǰ���Ȱѱ�ʶ��Ϊ��ѹ��Ʊ��
				BANKNOTE_HELPER->CollectAsync(this);
				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
			}
			if(GetServiceID()==CHARGE_SVC){
				GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ISSUING, 0, 0);
			}
		}
	}
	// ֹͣʶ��Ӳ��
	if (m_pCommonModel->operationInfo.isCoinSlotOpen){
		m_pCommonModel->operationInfo.isCoinSlotOpen = false;
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ����������ѯ��ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::InsertQueryOrderTimer()
{
	LOG("����������ѯ��ʱ��");
	int nTimer = 3;// ������Ҫ�������ļ���ȡ (3S ��ѯһ��)
	StartTimer(TIMERS_WEB_QUERY_ORDER,nTimer);

	//LOG("���������м�ʱ����");
	//// ʱ��
	//int nTimerTime = theAFC_TVM_OPERATION.GetCancelDealTime();
	//// Default time
	//if(0 == nTimerTime){
	//	nTimerTime = 60;// ��ʾ������
	//}
	//else{
	//	StartTimer(TIMER_COUNTING_TO_RETURN,nTimerTime);	
	//}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ֹͣ������ѯ��ʱ��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::EndQueryOrderTimer()
{
	LOG("ֹͣ������ѯ��ʱ��");
	StopTimer(TIMERS_WEB_QUERY_ORDER);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      �����߳�����web��������ͨ��

@param      ��

@retval     long

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::StartWebThread()
{
	DWORD dwThreadID;
	HANDLE m_hHandle = CreateThread(NULL,0,WebCommunicateFunction,this,0,&dwThreadID);

	if (m_hHandle == NULL){
		return -1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��̬���IO״̬�߳�

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CPassengerForeSvc::WebCommunicateFunction(LPVOID lpParameter)
{
	CPassengerForeSvc* pSvc = (CPassengerForeSvc*)lpParameter;

	if(NULL != pSvc){
		if (pSvc->m_hWebEvent==INVALID_HANDLE_VALUE || pSvc->m_hWebEvent==NULL){
			pSvc->m_hWebEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		}

		while(WAIT_OBJECT_0 == WaitForSingleObject(pSvc->m_hWebEvent,INFINITE)){
			// �ж��Ƿ��˳��߳�
			if(pSvc->m_bExitWebThread){
				break;
			}

			if(pSvc->m_nCurWebOper==WEB_ORDER_REQUEST){
				pSvc->m_bWebPaySuccess = false;
				RETURN_CODE retCode = RSP_CODE_OK;
				LOG("��¼��ʼʱ��:%s",ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
				if (pSvc->m_pCommonModel->paymentInfo.paymentMethod ==PAYMENT_METHOD_WITH_WEIXIN){
					retCode = theWEBSERVER_HELPER.Weixin_RequesetOrder(
						pSvc->m_pCommonModel->productInfo.beginLocation, 
						pSvc->m_pCommonModel->productInfo.endLocation, 
						pSvc->m_pCommonModel->productInfo.lProductSaleAmount, 
						pSvc->m_pCommonModel->productInfo.nProductPiece, 
						0);
				}
				else{
					retCode = theWEBSERVER_HELPER.Zhifubao_RequesetOrder(
						pSvc->m_pCommonModel->productInfo.beginLocation, 
						pSvc->m_pCommonModel->productInfo.endLocation, 
						pSvc->m_pCommonModel->productInfo.lProductSaleAmount, 
						pSvc->m_pCommonModel->productInfo.nProductPiece, 
						0);
				}
				LOG("��¼����ʱ��:%s",ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
				LOG("���Ͷ�ά�붩��������������");
				pSvc->GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ERWEIMA_ORDER_RES, (WPARAM)retCode, 0);
			}
			else if(pSvc->m_nCurWebOper==WEB_ORDER_QUERY){
				pSvc->m_bWebPaySuccess = false;
				RETURN_CODE retCode = RSP_CODE_OK;
				if (pSvc->m_pCommonModel->paymentInfo.paymentMethod ==PAYMENT_METHOD_WITH_WEIXIN){
					retCode = theWEBSERVER_HELPER.Weixin_QueryOrder();
				}
				else{
					retCode = theWEBSERVER_HELPER.Zhifubao_QueryOrder();
				}

				if(RSP_CODE_OK == retCode){
					LOG("��ѯ������֧���ɹ���");
					pSvc->m_bWebPaySuccess = true;
				}
			}
			else if(pSvc->m_nCurWebOper==WEB_ORDER_CANCEL){
				pSvc->m_bWebPaySuccess = false;//��ֹȡ��ǰ�������Ѿ�֧���ɹ�
				if (pSvc->m_pCommonModel->paymentInfo.paymentMethod ==PAYMENT_METHOD_WITH_WEIXIN){
					theWEBSERVER_HELPER.Weixin_CancelOrder();
				}
				else{
					theWEBSERVER_HELPER.Zhifubao_CancelOrder();
				}
			}
			else if(pSvc->m_nCurWebOper==WEB_ORDER_CONFIRM){
				theWEBSERVER_HELPER.Web_ConfirmOrder(
					(int)(pSvc->m_pCommonModel->productInfo.nProductPiece), 
					(int)(pSvc->m_pCommonModel->productInfo.nProductIssuedPiece), 
					pSvc->m_pCommonModel->actrualTicketInfo);
			}

			ResetEvent(pSvc->m_hWebEvent);
		}
	}

	return 0;
}


void CPassengerForeSvc::RequestOrder()
{
	m_nCurWebOper = WEB_ORDER_REQUEST;
	SetEvent(m_hWebEvent);
}

void CPassengerForeSvc::QueryOrder()
{
	m_nCurWebOper = WEB_ORDER_QUERY;
	SetEvent(m_hWebEvent);
}


void CPassengerForeSvc::ConfirmOrder()
{
	m_nCurWebOper = WEB_ORDER_CONFIRM;
	SetEvent(m_hWebEvent);
}

void CPassengerForeSvc::CancelOrder()
{
	m_nCurWebOper = WEB_ORDER_CANCEL;
	SetEvent(m_hWebEvent);
}