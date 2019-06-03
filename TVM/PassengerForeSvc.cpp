#include "StdAfx.h"
#include "PassengerForeSvc.h"
#include <assert.h>
#include "ChargeSvc.h"
/*
*乘客业务父类
*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CPassengerForeSvc,CTVMForeService)
	ON_SERVICE_TIMER()	//OnTimer的消息函数
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
/*
@brief          构造函数

@param      (i)SERVICE_ID serviceID              当前serviceID
@param      (i)SERVICE_ID parentServiceID    父serviceID

@retval        none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
CPassengerForeSvc::CPassengerForeSvc(SERVICE_ID serviceID, SERVICE_ID parentServiceID) :CTVMForeService(serviceID,parentServiceID)
{
	// CPassengerForeSvc其实应该是个virtual class,永远m_pCommonModel都指向一个m_pDataModel
	//(在比如ChargeSvc里的构造函数中造),然后在这里downward cast用。所以这里只建一个假的m_pCommonModel初始值。
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
@brief			析构函数

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
@brief      获取Svc模型

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
@brief     服务中数据对象发生变化,通知窗体更新 

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
@brief   读卡实现函数   

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnReadCard(UINT resultCode, CRWReadCard* rwReadCard)
{
	try{

		CMessagePictureBox::End();
		CString strTemp(_T(""));

		// 获取票卡信息
		RW_READ_CARD_RESPONSE cardRspInfo;
		rwReadCard->GetRWResponse(&cardRspInfo);
		//////////////////////////////////////////////////////////////////////////
		SaveCardInfo(&cardRspInfo);
		return;
		//////////////////////////////////////////////////////////////////////////

		// 读卡失败
		if(resultCode != RW_RSP_OK) {
			if(resultCode==RW_ERROR_CARD_NO_CARD){
				// // 取消后退出读卡,返回默认业务
				LOG("读卡反馈无卡,启动默认业务。");
				theSERVICE_MGR.SetState(RECEPT);
				CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
				NOTIFY_PROCESSING_WINDOW();
				pMainSvc->StartDefaultService();	
				return;
			}

			LOG("读卡失败。");		
			// 黑名单车票
			if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {
				LOG("读卡黑名单。");
				//BlockCard();
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(TXT_MSG_GUIDE_THIS_CARD_HAS_BEEN_LOCKED),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				// 将卡弹出
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;
			}
			// 寻卡或读卡失败
			else if((resultCode ==  RW_RSP_READERROR) || (resultCode == RW_RSP_FINDERROR) || (resultCode == RW_RSP_READ_CARD_FAILED)){		
				LOG("读卡或寻卡失败。");	
				CMessagePictureBox::Show(
					theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),
					_tl(TXT_PSG_TICKET_CANT_RECOGNIZE),
					TVM_TIP_WARINGS,
					CMessagePictureBox::BTN_OK,DT_LEFT,
					theAFC_TVM_OPERATION.GetCancelDealTime());

				// 将卡弹出
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;
			}
			// 票卡不满足充值条件
			else{
				strTemp = theTPUERROR_INFO.GetTpuErrorInfo(resultCode);
				LOG("票卡不满足充值条件。");	
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(GUIDE_CARD_CAN_NOT_DO_BUSINESS),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);

				// 将卡弹出
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;	
			}
		}
		// 读卡成功
		else{
			// 判断物理卡类型(物理类型不正确)
			if(cardRspInfo.CardPHType != MIFARE1K && cardRspInfo.CardPHType != MIFARE4K && cardRspInfo.CardPHType != CPU_TICKET){
				LOG("充值票卡类型不正确。");
				CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)GetServiceID()),_tl(GUIDE_IN_CARD_TYPE_ERROR),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime());

				// 将卡弹出
				//IO_HELPER->CheckAndSendOutCard();lichao

				NOTIFY_PROCESSING_WINDOW();
				theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->StartDefaultService();
				return;
			}

			// 保存票卡信息
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
@brief      接收到硬币回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinInserted(UINT res, CCHReadReceivs* chReadReceiveAmount)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;
		tCHACoinValueInfo  chAmountInfo;
		bool bIsCheckOutInsertMoeny = false;

		// 收到现金投入，通知前台界面
		if (m_pCommonModel->operationInfo.banknoteAcceptState==COIN_ACCEPT_STATE_NONE && m_pCommonModel->operationInfo.coinAcceptState==COIN_ACCEPT_STATE_NONE && res){
			GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ACCEPTED,0,(LPARAM)0);
		}

		// 一、如果正常接收
		if(res == 0){
			// 有投入,立即停止计时器。
			EndInsertTimer();
			LOG("有硬币投入。");

			// 计算投入总额		
			chReadReceiveAmount->GetCHResponse(&chAmountInfo);
			if (chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum >0 || chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum > 0)
			{
				LOG("硬币投入成功");
				m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_INSERTED;
				GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ACCEPTED,0,(LPARAM)0);
			}


TRY_READ_AGIAN:
			RenewCoinReceivedAmount(chAmountInfo);
			RenewCoinReceivedCount(chAmountInfo);

			// 更新界面金额信息显示信息
			NotifyDataModelChanged();

			// 1.判断投入金额是否足够？
			if (IsReceiveEnoughMoney()){
				// 1.1 判断是否无需找零？
				// 1.2 判断是否允许找零？
				// 1.3 判断找零箱是否足够找零?
				// 1.4 判断找零数量不超过最大找零数量限制?
				if (m_pCommonModel->paymentInfo.lChangeAmount >0){
					// 1.2判断当前模式是否支持找零？
					if (theTVM_STATUS_MGR.GetChangeMode()!= DEVICE_CONTRL_CODE_NO_CHANGE){
						// 1.2 判断是否足够找零？
						if (CalculateChangeCount()){
							if(m_pCommonModel->operationInfo.finishType == OVER_CHANGE_COUNT_LIMIT){
								LOG("找零数量超限。");
								// 1.4.2退还所有已收钱
								ReturnWhenCoinInserted();
							}
							else{
								// 1.3判断找零数量是否超过最大找零数量限制？
								// 1.4找零金额是否超限？(预留)
								if (!IsCoinChageMaxCount()){// 计算找零时已经判断。
									// 1.3.1关闭投币口、停止识别硬币
									if (m_pCommonModel->operationInfo.isCoinSlotOpen){
										m_pCommonModel->operationInfo.isCoinSlotOpen = false;
										COIN_HELPER->CH_StopInsertInfo();
										COIN_HELPER->CH_EndReceive();
									}
									// 1.3.2停止识别纸币,如果正在识别
									if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
										m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
										BANKNOTE_HELPER->StopValidation();
									}
									// 1.3.3再读一次数据,防止用户投币过快导致多投
									if (!bIsCheckOutInsertMoeny){
										LOG("再读一次金额(硬币投入:需要找零)。");
										long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
										if (errCode == E_NO_CH_ERROR){
											bIsCheckOutInsertMoeny = true;
											goto TRY_READ_AGIAN;
										}
									}
									// 1.3.4设置接收成功
									OnAcceptMoneyOK();
								}
								// 1.4找零数量超限
								else{
									LOG("找零数量超限。");
									// 1.4.1设置完成状态
									m_pCommonModel->operationInfo.finishType = OVER_CHANGE_COUNT_LIMIT;

									// 1.4.2退还所有已收钱
									ReturnWhenCoinInserted();
								}
							}
						}
						// 1.5找零金额超限
						else{
							LOG("找零金额超限。");
							// 1.5.1设置完成状态
							m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;
							// 1.5.2退还所有已收钱
							ReturnWhenCoinInserted();
						}
					}
					// 1.6当前模式不支持找零,直接显示找零不足
					else{
						LOG("当前模式不支持找零。");
						// 1.6.1设置状态
						m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;
						// 1.6.2退还所有已收钱
						ReturnWhenCoinInserted();
					}

				}
				// 1.8无需找零,直接进入下一步
				else{
					// 1.8.1关闭投币口、停止识别硬币
					if (m_pCommonModel->operationInfo.isCoinSlotOpen){
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;
						COIN_HELPER->CH_StopInsertInfo();
						COIN_HELPER->CH_EndReceive();
					}
					// 1.8.2停止识别纸币,如果正在识别
					if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
						m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
						BANKNOTE_HELPER->StopValidation();
					}
					// 1.8.3再读一次数据,防止用户投币过快导致多投
					if (!bIsCheckOutInsertMoeny){
						LOG("再读一次金额(硬币投入:不需要找零)。");
						long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
						if (errCode == E_NO_CH_ERROR){
							bIsCheckOutInsertMoeny = true;
							goto TRY_READ_AGIAN;
						}
					}	
					// 1.8.4设置接收成功
					OnAcceptMoneyOK();
				}
			}
			// 2.判断是否硬币和纸币都达到最大数量？
			else if (IsBothCoinBanknoteMaxCount()){
				LOG("硬币到和纸币都到达最大数量。");
				m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
				// 关闭接收口
				if (m_pCommonModel->operationInfo.isCoinSlotOpen){
					m_pCommonModel->operationInfo.isCoinSlotOpen = false;
					COIN_HELPER->CH_StopInsertInfo();
					COIN_HELPER->CH_EndReceive();
				}
				// 退还所有收的钱(能够走到这里来,硬币、纸币一定都满了)
				StartReturnCash();
				// 界面返回在退币回调函数中执行。
			}
			// 3. 是否硬币数量达到最大？
			else if(IsCoinMaxCount()){
				// 3.1如果纸币口打开,且正在接收...
				if (m_pCommonModel->operationInfo.isBankNoteSlotOpen)
				{
					LOG("硬币数量达到最大值,等待投入纸币。");					
					// 3.2 再读一次金额(硬币投入:硬币投入数量超限)
					if (!bIsCheckOutInsertMoeny){
						// 3.3 关闭硬币接收口
						COIN_HELPER->CH_StopInsertInfo();
						COIN_HELPER->CH_EndReceive();
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;

						LOG("再读一次金额(硬币投入:需要找零)。");
						long errCode = COIN_HELPER->CH_GetInsertInfo(chAmountInfo);
						if (errCode == E_NO_CH_ERROR){
							bIsCheckOutInsertMoeny = true;
							goto TRY_READ_AGIAN;
						}
					}
					m_pCommonModel->operationInfo.isCoinMaxCount = true;
					// 同时需要开启投入超时计时器。
					InsertMoneyNeedStartTimer();
					CString strGuide = _tl(TXT_COIN_MAX_INSERT_BANKNOTE);
					TCHAR* sGuide = strGuide.GetBuffer();
					GetCurReceptionDlg()->PostMessage(WM_UPDATE_RECEPITON_GUIDE, (WPARAM)sGuide, 0);
					// 提示硬币数量投入超过最大值。
					NotifyDataModelChanged();
				}
				// 3.4 如果没有打开,则直接退钱,投入数量超限
				else{
					LOG("当前只有硬币接收,退还所有硬币。");
					// 3.4.1设置完成状态
					m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
					// 3.4.2退还所有已收钱
					ReturnWhenCoinInserted();
				}				
			}
			// 4.继续收钱
			else{
				LOG("继续等待投入,读取金额。");
				m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
				COIN_HELPER->CH_GetInsertInfoAsync(this);
				InsertMoneyNeedStartTimer();
			}
		}
		// 二、如果手动终止：如按下取消键、异常、金额超限、数量超限、找零超限、投入超时、钱投够了等操作。
		else if(res == CCHException::ERROR_CH_STOP_READ_AMOUNT){
			LOG("回调终止读取金额。");
			// 1.收够钱了,由纸币插入后停止识别回调到此函数。
			if (m_pCommonModel->operationInfo.isCoinSlotOpen){
				m_pCommonModel->operationInfo.isCoinSlotOpen = false;
				COIN_HELPER->CH_StopInsertInfo();
				COIN_HELPER->CH_EndReceive();
			}

			// 需要在读一次进币信息
			LOG("再读一次金额(防止投入时点击取消)。");
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
			// 2.如果是取消操作,则执行退钱,硬币回调触发此函数。
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
					// 如果硬币有投入,退硬币
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
			else if(m_pCommonModel->operationInfo.isFinishedReceveMoney){// 如果收到足够多的钱了，开始发票
				goto TRY_READ_AGIAN;
				//OnAcceptMoneyOK();
			}
		}
		//// 三、判断是否是异常情况?
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
@brief      当硬币投入足够数量为达到要求时,退还所收的硬币及纸币

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::ReturnWhenCoinInserted(){
	// 1.关闭接收口,同时停止识别
	if (m_pCommonModel->operationInfo.isCoinSlotOpen){
		m_pCommonModel->operationInfo.isCoinSlotOpen = false;
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
	}

	if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
		m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		COIN_HELPER->CH_RefundAsync(this);
	}

	// 3.停止纸币接收识别:如果正在识别
	if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		BANKNOTE_HELPER->StopValidation();
	}
	// 4.退出纸币,如果投入纸币数量达到最大
	if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
		if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
			m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
			BANKNOTE_HELPER->ReturnAllAsync(this);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      硬币找零回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinChanged(UINT res,CCHChange* chChange)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;

		// 1.找零成功
		if (res == 0){
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				LOG("有硬币找零时打开照明灯。");
				try{
					IO_HELPER->OpenReturnLight();
					IO_HELPER->OpenReturnCupIndicator();
				}
				catch(CSysException& e){
					theEXCEPTION_MGR.ProcessException(e);
				}
				catch(...){
					LOG("打开照明灯失败。");
				}
			}
			if(0 == res){
				LOG("找零返回成功。");
				// 审计数据
				theDeviceRegAR.AddChTransMotorCount();
			}
			else{
				LOG("找零返回失败。");
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

			LOG("---------------------硬币找零前数量状态-------------------");
			LOG("Hopper A 数量：%d",hopperA.ulCurCount);
			LOG("Hopper B 数量：%d",hopperB.ulCurCount);
			LOG("找零箱 A 数量：%d",changeBoxA.ulCount);
			LOG("找零箱 B 数量：%d",changeBoxB.ulCount);
			LOG("---------------------硬币出币计数状态-------------------");
			LOG("出币数量（1元）：%d",rsp.wMHopper1Num);
			LOG("出币数量（5角）：%d",rsp.wMHopper2Num);

			LOG("---------------------硬币找零后数量状态-------------------");
			LOG("Hopper B 数量:%d",rsp2.wHopperBCnt);
			LOG("Hopper A 数量:%d",rsp2.wHopperACnt);
			LOG("找零箱 A 数量:%d",rsp2.wPreparationACnt);
			LOG("找零箱 B 数量:%d",rsp2.wPreparationBCnt);
			LOG("回收箱数量:%d",rsp2.wRecoverCnt);

			// 同步计数
			theCOIN_COUNT.UpdataBoxCountData(
				rsp2.wHopperACnt,			// Hopper 1 数量
				rsp2.wHopperBCnt,			// Hopper 2 数量
				rsp2.wPreparationACnt,		// 找零箱 1 数量
				rsp2.wPreparationBCnt,		// 找零箱 2 数量
				rsp2.wRecoverCnt,			// 回收箱 1 数量
				0			// 回收箱 2 数量
				);

			// 根据返回的结果判断找零是否真的成功？
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


			// 计算插入库找零详细信息的数据

			DWORD dwShouldLeft = 0;
			DWORD dwAlreadySendOut = 0;
			DWORD dwOneLeft = rsp.wMHopper1Num;	// 一元找零最大剩余
			DWORD dwHalfLeft= rsp.wMHopper2Num;	// 五角找零最大剩余


			// 找零正常
			if(bIsHalfYuanOk && bIsOneYuanOk){
				m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.clear();

				// Hopper A 找零
				if(hopperB.ulCurCount > rsp2.wHopperBCnt){ // 有找零
					dwAlreadySendOut = hopperB.ulCurCount - rsp2.wHopperBCnt;

					if(dwAlreadySendOut > rsp.wMHopper2Num){	// 有可能异常
						dwAlreadySendOut = 0;
					}
					else{
						dwHalfLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2] = dwAlreadySendOut;
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}

				// Hopper B 找零
				if(hopperA.ulCurCount > rsp2.wHopperACnt){
					dwAlreadySendOut = hopperA.ulCurCount - rsp2.wHopperACnt;

					if(dwAlreadySendOut > rsp.wMHopper1Num){// 一元
						dwAlreadySendOut = 0;
					}
					else{
						dwOneLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1] = dwAlreadySendOut;
						theDeviceRegAR.AddChCycleChangeCount(dwAlreadySendOut);
					}
				}

				// 找零箱 A找零
				if(changeBoxA.ulCount > rsp2.wPreparationACnt){
					dwAlreadySendOut = changeBoxA.ulCount - rsp2.wPreparationACnt;

					if(dwAlreadySendOut > dwOneLeft){// 一元
						dwAlreadySendOut = 0;
					}
					else{
						dwOneLeft -= dwAlreadySendOut;
						m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1] = dwAlreadySendOut;
						theDeviceRegAR.AddChSupplayBoxAChangeCount(dwAlreadySendOut);
					}
				}
				// 找零箱 B找零
				if(changeBoxB.ulCount > rsp2.wPreparationBCnt){
					dwAlreadySendOut = changeBoxB.ulCount - rsp2.wPreparationBCnt;

					if(dwAlreadySendOut > dwOneLeft){// 一元(BUG：导致没有入库，以前认为是五角的dwHalfLeft)
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
				// 找零详细是否匹配？
				auto iter = m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.find(COIN_HOPPER_2);
				if(iter != m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.end()){
					dwShouldLeft = hopperB.ulCurCount - iter->second;// 找零后应当剩余数量
					if(dwShouldLeft != rsp2.wHopperBCnt){
						dwAlreadySendOut = abs(hopperB.ulCurCount  - rsp2.wHopperBCnt);//应该实际找出的数量
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
			else// 找零完成,返回完成界面
				OnReturnFinish();
		}
		// 2.找零失败
		else{
			LOG("找零返回失败。");
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo.clear();// 找零失败
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
@brief      压硬币到回收箱和找零箱   

@param      UINT res,CCHAccept*

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinReceived(UINT res,CCHAccept* chAccept)
{
	try{

		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;

		// 1.回收成功
		if (res==0){

			LOG("硬币回收成功。");
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_COLLECTED;

			tCHACoinValueInfo rsp;
			chAccept->GetResponse(&rsp);
			int OneY_inHopper = rsp.stCoinBoxValueInfo[0].uiCoinNum;
			int HalfY_inHopper = rsp.stCoinBoxValueInfo[1].uiCoinNum;
			int OneY_inRecover = rsp.stCoinBoxValueInfo[2].uiCoinNum;
			CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collecBoxA = theCOIN_COUNT.GetCollectionCoinboxAInfo();	// 右边（包括五角、一元乘客投入）
			CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collecBoxB = theCOIN_COUNT.GetCollectionCoinboxBInfo();	// 左边（包括五角、一元备用线回收）
			CTCoinCountInfo::COIN_BOX_INFO			  hopperA	 = theCOIN_COUNT.GetCoinHopperboxAInfo();		// 右边（5角）
			CTCoinCountInfo::COIN_BOX_INFO			  hopperB	 = theCOIN_COUNT.GetCoinHopperboxBInfo();		// 左边（1元）
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  changeA	= theCOIN_COUNT.GetChangeCoinboxAInfo();
			CTCoinCountInfo::COIN_CHANGE_BOX_INFO	  changeB	= theCOIN_COUNT.GetChangeCoinboxBInfo();
			LOG("----------------回收前-----------------------");
			LOG("Hopper A 数量:%d",hopperA.ulCurCount);
			LOG("Hopper B 数量:%d",hopperB.ulCurCount);
			LOG("回收箱 A 1元数量:%d",collecBoxA.ulCount_1yuan);
			//LOG("回收箱 A 5角数量:%d",collecBoxA.ulCount_5jiao);
			//LOG("回收箱 B 1元数量:%d",collecBoxB.ulCount_1yuan);
			//LOG("回收箱 B 5角数量:%d",collecBoxB.ulCount_5jiao);


			LOG("----------------回收后-----------------------");
			LOG("Hopper A 数量:%d",OneY_inHopper);
			LOG("Hopper B 数量:%d",HalfY_inHopper);
			//LOG("找零箱 A 数量:%d",rsp.OneY_inPreparation);
			//LOG("找零箱 B 数量:%d",rsp.HalfY_inPreparation);
			LOG("回收箱 1 元数量:%d",OneY_inRecover);
			//LOG("回收箱 5 角数量:%d",rsp.HalfY_inRecover);

			// 同步计数
			theCOIN_COUNT.UpdataBoxCountData(OneY_inHopper,HalfY_inHopper,changeA.ulCount,changeB.ulCount,OneY_inHopper,collecBoxA.ulCount_5jiao);
			m_pCommonModel->paymentInfo.CoinSaveDetail.clear();

			// 同步回收箱计数
			DWORD dwOneInColl = 0,dwHalfInColl = 0,dwCollecOneCount = 0,dwCollecHalfCount = 0;
			bool bAddToOneCollection = false,bAddToHalfCollection = false;
			dwOneInColl = collecBoxA.ulCount_1yuan + collecBoxB.ulCount_1yuan;
			dwHalfInColl = collecBoxA.ulCount_5jiao + collecBoxB.ulCount_5jiao;

			// 计算收纳信息
			if(/*hopperB.ulCurCount != rsp.OneY_inHopper && */hopperB.coinBoxID.ToString() != INVALID_ID){
				m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_1] = m_pCommonModel->paymentInfo.PayCoinInfo[Coin1Yuan];//实际收入等于接收的钱
			}

			if(/*hopperA.ulCurCount != rsp.HalfY_inHopper && */hopperA.coinBoxID.ToString() != INVALID_ID){
				m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_2] = m_pCommonModel->paymentInfo.PayCoinInfo[CoinHalfYuan];//实际投入等于接收的钱
			}

			/*if(bAddToOneCollection){
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_COLL_BOX_1_1] = dwCollecOneCount;
			}

			if(bAddToHalfCollection){
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_COLL_BOX_1_5] = dwCollecHalfCount;
			}*/

			StartCHChange();
		}
		// 2.回收失败(需要考虑计数)
		else{
			LOG("硬币回收失败。");

			// 复位后硬币一般会进入循环找零箱
			m_pCommonModel->paymentInfo.CoinSaveDetail.clear();

			// 收纳信息
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_1] = m_pCommonModel->paymentInfo.PayCoinInfo[Coin1Yuan];//实际收入等于接收的钱
			m_pCommonModel->paymentInfo.CoinSaveDetail[COIN_HOPPER_2] = m_pCommonModel->paymentInfo.PayCoinInfo[CoinHalfYuan];//实际投入等于接收的钱

			// 将暂存计数
			CTCoinCountInfo::COIN_TEMPORARY1_INFO tempB = theCOIN_COUNT.GetCoinTemporary1Info();

			if(tempB.ulCount_1yuan != 0){
				CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
				hopperB.ulCurCount += tempB.ulCount_1yuan;
				theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);

				LOG("暂存数据已计入计数中去：一元枚数：%d",tempB.ulCount_1yuan);

				tempB.ulCount_1yuan = 0;
				theCOIN_COUNT.SetCoinTemporary1Info(tempB);
			}

			// 设置状态为存入失败
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
@brief      退币回调函数 

@param      UINT res 返回值；CCHRefund* 返回状态

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCoinReturned(UINT res,CCHRefund*)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_COIN_BUSY;

		// 1.执行成功
		if (res == 0){
			if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID)){
				LOG("打开照明灯。");
				IO_HELPER->OpenReturnLight();	
				IO_HELPER->OpenReturnCupIndicator();
			}
			// 1.1设置状态
			LOG("硬币退给乘客成功。");
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_RETURNED;
			m_pCommonModel->paymentInfo.lReturnAmount += m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
			//m_pCommonModel->paymentInfo.PayCoinInfo.clear();
			// 2.1返回
			OnReturnFinish();
		}
		// 2.执行失败
		else{
			LOG("硬币退给乘客失败。");
			// 退币失败,需要打印,读一下状态,设置相关硬币数量。
			m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_RETURN_FAILD;
			//if(COIN_HELPER->IsExceptionCode(res)){
			//	throw CCHHardwareException(res,_T(__FILE__),__LINE__);
			//}
		}
	}
	catch(CSysException& e){
		//m_pCommonModel->operationInfo.m_vecException.push_back(e);//添加到异常队列中去
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      接收纸币回调函数

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnBanknoteInserted(UINT resultCode,CBRHOpenAndValidate* bhReadReceiveAmount)
{
	try{
		// 当前纸币模块解除忙碌状态
		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_BUSY;
		//// 收到现金投入，通知前台界面
		//if (m_pCommonModel->operationInfo.banknoteAcceptState==BANKNOTE_ACCEPT_STATE_NONE && m_pCommonModel->operationInfo.coinAcceptState==COIN_ACCEPT_STATE_NONE && resultCode){
		//	GetCurReceptionDlg()->CallWndProc(WM_NOTIFY_ACCEPTED,0,(LPARAM)0);
		//}

		//一. 如果正常接收
		if(resultCode == 0){
			// 有投入,立即停止计时器。
			EndInsertTimer();
			LOG("纸币接收成功");
			// 计算投入总额
			tBncCashInfo feedback;
			bhReadReceiveAmount->GetResponse(&feedback);

			RenewBanknoteReceivedAmount(feedback);
			RenewBanknoteReceivedCount(feedback);
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;

			// 更新界面金额信息显示信息
			NotifyDataModelChanged();


TRY_CHECK_AGIAN:
			// 停止接收纸币
			if(m_bRechargeConfirm){
				LOG("stop receive, Leave!");
				m_bRechargeConfirm = false;
				// 压钞
				if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
					LOG("开始押钞纸币,已接受纸币。");
					m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;// 为了不与其他地方同步压钞冲突，这里异步压钞前，先把标识改为已压钞票。
					BANKNOTE_HELPER->CollectAsync(this);
					m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
				}

				// 乘客确认投入纸币，开始进行充值业务
				if(GetServiceID()==CHARGE_SVC){
					GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ISSUING, 0, 0);
				}
				return ;
			}

			// 投入识别时取消、超时时投入，此时需要将钱退出(将来还需要判断)
			if(m_pCommonModel->operationInfo.finishType == PASSAGE_CANCEL || 
				m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME ||
				CANCELING == theSERVICE_MGR.GetState()){
					ReturnWhenBanknotesInserted();
					return ;
			}

			// 1. 判断是否投入最大总金额超限？
			if (IsBanknoteMaxAmount()) {
				LOG("投入纸币总金额超过最大限制,退钱。");
				// 1.1设置状态 
				m_pCommonModel->operationInfo.finishType = OVER_MONEY_AMOUNT_LIMIT;
				// 1.2退币操作
				ReturnWhenBanknotesInserted();
				// 界面返回在退币回调函数中执行。
			}
			// 3. 投入金额是否足够？
			else if (IsReceiveEnoughMoney()){
				if(GetServiceID()==CHARGE_SVC){
					m_bRechargeOverlimit = true;
					LOG("充值接收金额达到该卡最大可充金额，停止接收！Leave！");
					CString strGuide = _tl(TXT_BANKNOTE_OVERLIMIT_CARD_MAX_REVALUE);
					TCHAR* sGuide = strGuide.GetBuffer();
					GetCurReceptionDlg()->PostMessage(WM_UPDATE_RECEPITON_GUIDE, (WPARAM)sGuide, 0);
					NotifyDataModelChanged();
					return;
				}		

				// 3.1判断是否需要找零？
				if (m_pCommonModel->paymentInfo.lChangeAmount >0){
					// 3.2判断是否支持找零？
					if (theTVM_STATUS_MGR.GetChangeMode() != DEVICE_CONTRL_CODE_NO_CHANGE){
						// 3.3找零是否足够?
						if (CalculateChangeCount()){
							if(m_pCommonModel->operationInfo.finishType == OVER_CHANGE_COUNT_LIMIT){
								LOG("找零数量超限。");
								// 3.4.2退还所有已收钱
								ReturnWhenBanknotesInserted();
							}
							else{
								// 3.4找零数量是否超限？
								// 3.5找零金额是否超限？(预留)
								//if(!IsCoinChageMaxAmount()){}
								if (!IsCoinChageMaxCount()){// 这里已经没有用了，因为在计算找零时都已经判断过了。
									// 正常完成
									LOG("纸币接收完成,需要找零。");
									// 停止识别纸币
									if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
										m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
										BANKNOTE_HELPER->StopValidation();
									}
									// 停止识别硬币
									if (m_pCommonModel->operationInfo.isCoinSlotOpen){
										m_pCommonModel->operationInfo.isCoinSlotOpen = false;
										COIN_HELPER->CH_StopInsertInfo();
										COIN_HELPER->CH_EndReceive();
									}
									// 设置接收成功
									OnAcceptMoneyOK();
								}
								// 3.4找零数量超限
								else{
									LOG("找零数量超限。");
									// 3.4.1设置完成状态
									m_pCommonModel->operationInfo.finishType = OVER_CHANGE_COUNT_LIMIT;

									// 3.4.2退还所有已收钱
									ReturnWhenBanknotesInserted();
								}
							}
						}
						// 3.3找零不足
						else{
							LOG("找零不足。");
							// 3.3.1设置完成状态
							m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;

							// 3.3.2退还所有已收钱
							ReturnWhenBanknotesInserted();
						}
					}
					// 3.2不支持找零,或为找零不足,提示！！！
					else{
						LOG("找零不足或者当前模式不支持找零。");
						// 3.2.1设置完成状态
						m_pCommonModel->operationInfo.finishType = OVER_CHANGE_AMOUNT_LIMIT;
						//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);//通用接口：找零不足。

						// 3.2.2退还所有已收钱
						ReturnWhenBanknotesInserted();
					}
				}
				// 3.1不需要找零,直接进行下一步
				else{
					LOG("纸币接收完成,且不需要找零。");
					// 停止识别纸币
					if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
						m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
						//BANKNOTE_HELPER->StopValidation();
					}
					// 停止识别硬币
					if (m_pCommonModel->operationInfo.isCoinSlotOpen){
						m_pCommonModel->operationInfo.isCoinSlotOpen = false;
						COIN_HELPER->CH_StopInsertInfo();
						//COIN_HELPER->CH_EndReceive();
					}
					// 设置接收成功
					OnAcceptMoneyOK();
				}
			}
			// 2. 判断是否硬币和纸币都达到最大数量？
			else if(IsBothCoinBanknoteMaxCount()){
				LOG("硬币及纸币数量达到最大限制,退钱。");
				// 2.1设置返回状态
				m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
				//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);// Need change,when it's ok!!
				// 2.2退币操作
				ReturnWhenBanknotesInserted();
				// 界面返回在退币回调函数中执行。
			}
			// 4. 投入数量是否超限？
			else if(IsBanknoteMaxCount()){
				// 4.1 如果硬币口打开,正在接收...
				if (m_pCommonModel->operationInfo.isCoinSlotOpen)
				{
					LOG("停止识别纸币,等待硬币投入。");
					// 停止识别纸币
					BANKNOTE_HELPER->StopValidation();
					m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
					m_pCommonModel->operationInfo.isBanknotesMaxCount = true;
					CString strGuide = _tl(TXT_BANKNOTE_MAX_INSERT_COIN);
					TCHAR* sGuide = strGuide.GetBuffer();
					GetCurReceptionDlg()->PostMessage(WM_UPDATE_RECEPITON_GUIDE, (WPARAM)sGuide, 0);
					NotifyDataModelChanged();
					// 需要开启超时计时器
					InsertMoneyNeedStartTimer();
					// 显示投入纸币数量超限提示
					NotifyDataModelChanged();
				}
				// 4.2 硬币没有接收,超过最大限制,退钱。
				else {
					LOG("硬币没有投入,纸币投入数量超限,退币。");
					// 4.2.1设置返回状态
					m_pCommonModel->operationInfo.finishType = OVER_MONEY_COUNT_LIMIT;
					// 4.2.2退币操作
					ReturnWhenBanknotesInserted();			
					// 界面返回在退币回调函数中执行。
				}				
			}
			// 5. 继续收钱
			else{
				LOG("投入金额不足,继续投入。");
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
		//20161229，lichao: 根据错误码，做不同的处理，这里先注释。
		// 二. 如果手动终止：如按下取消键、异常、金额超限、数量超限、找零超限、投入超时、钱投够了等操作。
		//else if(resultCode == BH_RESPONSE_CODE::BH_WORK_CANCLED/*& DONE_PHASE_ACT_ABORT*/){
		else if(resultCode == CBRHHardwareException::ERR_CANCELLED/*& DONE_PHASE_ACT_ABORT*/){
			LOG("回调停止纸币读取金额。");
			// 1.自动退出识别模式(硬币投够,手动取消时触发此回调)停止识别纸币	
			if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
				m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
				BANKNOTE_HELPER->StopValidation();
			}

			//BH_STATUS bh_st;
			//BANKNOTE_HELPER->GetStatus(bh_st);
			// 2.取消操作,则执行退钱,硬币操作回调触发此函数。
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
					// 当有接收到纸币且未压钞时纸币退钱
					if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
						LOG("回调退钱。");
						EndInsertTimer();
						if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID))
						{
							m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
							BANKNOTE_HELPER->ReturnAllAsync(this);
						}
					}
					if (m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_INSERTED/* || bh_st.bh_sts.cnt >0*/) {
						// 此时需要再读一次状态，如果是超时和取消
						if(m_pCommonModel->operationInfo.finishType == PASSAGE_CANCEL		|| // 乘客取消时纸币进入
							m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME || // 乘客操作超时
							m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME	   // 投入超时
							){
								//try{ // xuzan 
								//	// lichao: 查询暂存中是否还有钞票
								//	//BRH_RSP_GET_STATUS status;
								//	tBncDevReturn status;
								//	BANKNOTE_HELPER->GetStatus(status);
								//	if (status.intermediateStackerStatus!=0){
								//		LOG("纸币模块任然有纸币。");
								//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;
								//		// 退还纸币(如果收了钱)
								//		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
								//		BANKNOTE_HELPER->ReturnAllAsync(this);
								//	}
								//}
								//catch(CSysException& e){
								//	theEXCEPTION_MGR.ProcessException(e);
								//}
								//catch(...){
								//	LOG("纸币模块读取状态产生异常。");
								//}
						}
						OnReturnFinish();
					}
			}
			else if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR){
				OnReturnFinish();
			}
			else if(m_pCommonModel->operationInfo.isFinishedReceveMoney){// 如果收到足够多的钱了，开始发票
				// 需要判断一下是否暂存里面有临界情况投入的纸币
				try{
					LOG("errorType==ERR_CANCELLED && isFinishedReceveMoney=true");
					// lichao: 下面暂注释
					//BRH_STATUS bh_st;
					//BANKNOTE_HELPER->GetStatus(bh_st);

					//if(bh_st.bh_sts.validBnCount > 0){
					//	LOG("纸币模块有纸币：%d张。",bh_st.bh_sts.validBnCount);
					//	LOG("设备已确认纸币：%d张。",m_pCommonModel->paymentInfo.lBHReceiveTotalCount);

					//	m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;

					//	// 当前投入的钱与实际不相等
					//	if(m_pCommonModel->paymentInfo.lBHReceiveTotalCount != bh_st.bh_sts.validBnCount){
					//		LOG("纸币投入张数与实际记录不符合，交易取消");
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
					LOG("纸币模块读取状态产生异常。");
				}
				OnAcceptMoneyOK();
			}
		}
		// 投币时全部退出
		//else if(resultCode == CBRHException::BH_RSP_RETURN_MONEY_SUCCESS){
		//	m_pCommonModel->operationInfo.isFinishedReceveMoney = false;

		//	LOG("投入金额面额不符合要求，全部退还，继续等待重新接收纸币或者返回完成界面。");
		//	if(m_pCommonModel->operationInfo.finishType != PASSAGE_CANCEL && 
		//		m_pCommonModel->operationInfo.finishType != INSERT_OUT_OF_TIME &&
		//		m_pCommonModel->operationInfo.finishType != EXCEPTION_ERROR && 
		//		!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
		//		m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//		m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//		m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;
		//		
		//		NotifyDataModelChanged();// 界面刷新

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
		//				LOG("纸币模块任然有纸币：%d张。",bh_st.bh_sts.validBnCount);
		//				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;
		//				// 退还纸币(如果收了钱)
		//				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//				BANKNOTE_HELPER->ReturnAllAsync(this);
		//			}
		//			else{
		//				m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//				m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//				m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//				NotifyDataModelChanged();// 界面刷新
		//			}
		//		}
		//		catch(CSysException& e){
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//			m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//			NotifyDataModelChanged();// 界面刷新
		//			theEXCEPTION_MGR.ProcessException(e);
		//		}
		//		catch(...){
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//			m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//			m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//			NotifyDataModelChanged();// 界面刷新
		//			LOG("纸币模块读取状态产生异常。");
		//		}
		//		OnReturnFinish();
		//	}
		//}
		//else if(resultCode == CBRHException::BH_RSP_RETURN_ONE_SUCCESS){
		//	LOG("投入金额不符合要求，已退一张，继续等待接收纸币。");
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
		//			// 如果有投入钱，需要将钱退给乘客
		//			if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){
		//				// 退还纸币(如果收了钱)
		//				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//				BANKNOTE_HELPER->ReturnAllAsync(this);
		//			}
		//			else{
		//				OnReturnFinish();
		//			}
		//		}
		//	}
		//	else{
		//		LOG("投入完成后，又投入不能识别的金额。");
		//		OnAcceptMoneyOK();
		//	}
		//}
		//// 纸币投入取消，且已经识别
		//else if(resultCode == BH_RESPONSE_CODE::BH_INSTERT_CANCLED){
		//	LOG("纸币投入时取消，面额已经识别，退钱。");
		//	// 计算投入总额
		//	BRH_CMD_FEEDBACK feedback;
		//	bhReadReceiveAmount->GetResponse(&feedback);

		//	RenewBanknoteReceivedAmount(feedback);
		//	RenewBanknoteReceivedCount(feedback);
		//	m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_INSERTED;
		//	NotifyDataModelChanged();

		//	if(!m_pCommonModel->operationInfo.isFinishedReceveMoney){
		//		// 退钱
		//		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//		EndInsertTimer();
		//		BANKNOTE_HELPER->ReturnAllAsync(this);
		//		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		//	}
		//	else{
		//		goto TRY_CHECK_AGIAN;
		//	}
		//}
		//// 钱插进去又拿走
		//else if(resultCode == BH_RESPONSE_CODE::BH_BANKNOTE_TAKEN_AWAY){
		//	LOG("钱在准备插入时拿走，继续等待投入。");
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
		//			// 如果有投入钱，需要将钱退给乘客
		//			if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){
		//				// 退还纸币(如果收了钱)
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
		//// 命令无法执行，纸币模块有异常时
		//else if(resultCode == BH_RESPONSE_CODE::BH_WORK_INVALID){
		//	// 投入时硬件异常
		//	LOG("硬件异常，不可能执行的命令或状态");
		//	throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_WORK_INVALID,_T(__FILE__),__LINE__);
		//}
		//// 三、判断是否是卡钞票？
		//else if(resultCode == ERR_BH_INSERT_JAM || resultCode == ERR_BH_JAM){
		//	// 判断当前是否真的接收到纸币
		//	if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED){// 已接受，要求打故障单退钱
		//		throw CBRHHardwareException(resultCode,_T(__FILE__),__LINE__);
		//	}
		//	else{
		//		// 写故障到异常队列
		//		theEXCEPTION_MGR.ProcessException(CBRHHardwareException(resultCode,_T(__FILE__),__LINE__));		
		//		OnReturnFinish();
		//	}
		//}
		//// 纸币处理时出现故障(尝试退出三次)
		//else if(resultCode == BH_RESPONSE_CODE::BH_HARDWARE_EXCEPTION){

		//	theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.Invoke();
		//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);

		//	long lRetRepare = BANKNOTE_HELPER->TryRecoveryBanknoteModule();

		//	if(lRetRepare == 0){
		//		LOG("纸币退出成功。");
		//	}
		//	// 此时出现故障，需要退纸币，但是是以恢复的方式退币,所以故障单不需要体现
		//	//m_pCommonModel->paymentInfo.lBHReceiveTotalAmount = 0;
		//	//m_pCommonModel->paymentInfo.lBHReceiveTotalCount = 0;
		//	//m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();

		//	m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		//	m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;

		//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
		//	// 抛异常是加入异常队列且如果有硬币投入需要退硬币
		//	throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
		//}
		else 
			throw CBRHHardwareException(resultCode,_T(__FILE__),__LINE__);
	}
	catch (CSysException& e){
		// 如果有异常,执行异常返回
		LOG("纸币卡币异常。");
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
@brief		当纸币投入数量或总额足够多时,退还所有纸币及硬币  

@param      none

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::ReturnWhenBanknotesInserted(){
	// 停止识别纸币
	if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		BANKNOTE_HELPER->StopValidation();
	}

	if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
		m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		BANKNOTE_HELPER->ReturnAllAsync(this);
	}

	// 退换硬币,如果接收了。
	if (m_pCommonModel->operationInfo.isCoinSlotOpen){
		m_pCommonModel->operationInfo.isCoinSlotOpen = false;
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
	}
	// 投入数量达到最大要求数量,在这里处理退币
	if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
		m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		COIN_HELPER->CH_RefundAsync(this);
	}

	OnReturnFinish();
}
//////////////////////////////////////////////////////////////////////////
/*
@brief      纸币压回收箱实现函数

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

		// 一、回收成功
		if (res==0){
			LOG("押钞成功(纸币)。");
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;


			tBncEnCashInfo cashIntoInfo;
			pCashIntoInfo->GetBRHResponse(&cashIntoInfo);

			// 回收箱
			CTBanknoteCountInfo::BANKNOTE_BOX_INFO collBoxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			LOG("-------------------------纸币回收前--------------------------");
			LOG("回收箱1元数量：%d",	collBoxInfo.ulCount_CNY1);
			LOG("回收箱5元数量：%d",	collBoxInfo.ulCount_CNY5);
			LOG("回收箱10元数量：%d",	collBoxInfo.ulCount_CNY10);
			LOG("回收箱20元数量：%d",	collBoxInfo.ulCount_CNY20);
			LOG("回收箱50元数量：%d",	collBoxInfo.ulCount_CNY50);
			LOG("回收箱100元数量：%d",	collBoxInfo.ulCount_CNY100);

			collBoxInfo.ulCount_CNY1 +=   cashIntoInfo.wCashBox_CNY1_IntoCount;
			collBoxInfo.ulCount_CNY5 +=   cashIntoInfo.wCashBox_CNY5_IntoCount;
			collBoxInfo.ulCount_CNY10 +=  cashIntoInfo.wCashBox_CNY10_IntoCount;
			collBoxInfo.ulCount_CNY20 +=  cashIntoInfo.wCashBox_CNY20_IntoCount;
			collBoxInfo.ulCount_CNY50 +=  cashIntoInfo.wCashBox_CNY50_IntoCount;
			collBoxInfo.ulCount_CNY100 += cashIntoInfo.wCashBox_CNY100_IntoCount;

			LOG("-------------------------纸币回收数量--------------------------");
			LOG("回收1元数量：%d",	cashIntoInfo.wCashBox_CNY1_IntoCount);
			LOG("回收5元数量：%d",	cashIntoInfo.wCashBox_CNY5_IntoCount);
			LOG("回收10元数量：%d",	cashIntoInfo.wCashBox_CNY10_IntoCount);
			LOG("回收20元数量：%d",	cashIntoInfo.wCashBox_CNY20_IntoCount);
			LOG("回收50元数量：%d",	cashIntoInfo.wCashBox_CNY50_IntoCount);
			LOG("回收100元数量：%d",cashIntoInfo.wCashBox_CNY100_IntoCount);

			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(collBoxInfo)) {
				theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
			}


			// 循环鼓
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxA = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxB = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxC = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
			CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recyclerBoxD = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

			LOG("-------------------------纸币回收前循环鼓数量--------------------------");
			LOG("循环鼓A数量：%d",	recyclerBoxA.ulCurCount);
			LOG("循环鼓B数量：%d",	recyclerBoxB.ulCurCount);
			LOG("循环鼓C数量：%d",	recyclerBoxC.ulCurCount);
			LOG("循环鼓D数量：%d",	recyclerBoxD.ulCurCount);

			LOG("-------------------------纸币回收进入循环鼓数量--------------------------");
			LOG("循环鼓A数量：%d",	cashIntoInfo.wRecyclerAIntoCount);
			LOG("循环鼓B数量：%d",	cashIntoInfo.wRecyclerBIntoCount);
			LOG("循环鼓C数量：%d",	cashIntoInfo.wRecyclerCIntoCount);
			LOG("循环鼓D数量：%d",	cashIntoInfo.wRecyclerDIntoCount);


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

			// 1.读纸币钱箱的计数信息的计数信息,更新,并重新写入。
			/*CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			boxInfo.ulCount_1 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote1Yuan];
			boxInfo.ulCount_5 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote5Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote10Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote20Yuan];
			boxInfo.ulCount_50 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote50Yuan];
			boxInfo.ulCount_100 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote100Yuan];
			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
			}*/

			//if(m_pCommonModel->lDeviceStatus == 0 && m_pCommonModel->operationInfo.mapException.size() == 0){
			//	theSERVICE_MGR.SetState(FINISH);
			//	theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(true);
			//}

			OnReturnFinish();
		}
		// 二、回收失败
		else{
			LOG("押钞失败(纸币)。");
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED;
			// 押钞失败最终异常解除时还会将其压入回收箱，所以需要计数
			// Need edit zhengxianle
			/*CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			boxInfo.ulCount_1 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote1Yuan];
			boxInfo.ulCount_5 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote5Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote10Yuan];
			boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote20Yuan];
			boxInfo.ulCount_50 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote50Yuan];
			boxInfo.ulCount_100 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote100Yuan];
			if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
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
@brief      纸币退币   

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnBanknoteReturned(UINT res,CBRHCashInRollback* pBHRef)
{
	try{
		m_pCommonModel->lDeviceStatus &= ~DEVICE_BANKNOTE_BUSY;
		theSERVICE_MGR.SetForeServiceBusy(false);
		// 1.退币成功
		if (res == 0){
			LOG("纸币退币命令执行成功。(回调)");
			//BRH_CMD_FEEDBACK refCallBack;
			//long lRet = pBHRef->GetResponse(&refCallBack);

			/*if(lRet == BHTRS_RES_OK)*/{
				// 1.1设置接收状态
				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURNED;
				m_pCommonModel->paymentInfo.lReturnAmount += m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
				//m_pCommonModel->paymentInfo.PayBankNoteInfo.clear();
				// 1.2 转到界面
				OnReturnFinish();
			}
			//else{

			//	// 如果失败，或者退出的钱和收入的钱相等，则此时是一种正常的情况，不需要打故障单
			//	if(lRet == BH_RESPONSE_CODE::BH_HARDWARE_EXCEPTION || refCallBack.nCount == m_pCommonModel->paymentInfo.lBHReceiveTotalCount){
			//		//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.Invoke();
			//		theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURNED;
			//		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
			//		//throw  CBHHardwareException(CBHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
			//		theEXCEPTION_MGR.ProcessException(CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__));
			//		// 调用返回界面
			//		OnReturnFinish();
			//	}
			//	else{
			//		// 如果是退币失败的话，也需要将这次的数量记在文件中
			//		CTBanknoteCountInfo::BANKNOTE_BOX_INFO boxInfo = theBANKNOTE_COUNT.GetBanknoteboxInfo();
			//		boxInfo.ulCount_1 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote1Yuan];
			//		boxInfo.ulCount_5 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote5Yuan];
			//		boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote10Yuan];
			//		boxInfo.ulCount_10 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote20Yuan];
			//		boxInfo.ulCount_50 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote50Yuan];
			//		boxInfo.ulCount_100 += m_pCommonModel->paymentInfo.PayBankNoteInfo[Banknote100Yuan];
			//		if(!theBANKNOTE_COUNT.SetBanknoteboxInfo(boxInfo)) {
			//			theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币钱箱的计数信息写入失败"));
			//		}

			//		if(lRet == BH_FAIL){// 1.3失败
			//			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_RETURN_FAILED;
			//			throw CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
			//		}
			//		else if(lRet == BH_WORK_INVALID){// 不可执行的命令
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
		// 2.退币失败
		else{
			// 退币失败永远都不会打印故障单，这部分钱会退给乘客
			theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_NONE;
			//throw  CBHHardwareException(CBHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__);
			//theEXCEPTION_MGR.ProcessException(CBRHHardwareException(CBRHHardwareException::ERROR_BH_MODULE_EXCEPTION,_T(__FILE__),__LINE__));
			// 调用返回界面
			OnReturnFinish();

			/*
			LOG("纸币退币失败。(回调),需要打印凭条。");
			// 2.1设置状态
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
@brief      纸币找零回调函数

@param      UINT nResult;返回值
CBHChangeSendOutBill* bhChangeSendOut;找零返回指针对象

@retval     无

@exception  无
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
			LOG("纸币找零成功");
			m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;

			LOG("-------------------------纸币找零前--------------------------");
			LOG("循环鼓A找零前：%d",recyclerBoxA.ulCurCount);
			LOG("循环鼓B找零前：%d",recyclerBoxB.ulCurCount);
			LOG("循环鼓C找零前：%d",recyclerBoxC.ulCurCount);
			LOG("循环鼓D找零前：%d",recyclerBoxD.ulCurCount);
			LOG("补币箱找零前：%d",changeBox.ulCurCount);

			CHANGE_RESULT result;
			bhChangeSendOut->GetResponse(&result);

			LOG("-------------------------纸币找零信息--------------------------");
			LOG("循环鼓A出币：%d",result.wRecyclerAOutCount);
			LOG("循环鼓B出币：%d",result.wRecyclerBOutCount);
			LOG("循环鼓C出币：%d",result.wRecyclerCOutCount);
			LOG("循环鼓D出币：%d",result.wRecyclerDOutCount);

			// 20171205，lichao: 由于纸币模块硬件自动补币。因此这里需要考虑循环鼓低水位时，纸币计数的调整。
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
								LOG("循环鼓低水位%d张，A自动补币%d张，补充后%d张；找零箱补充后%d张", theAPP_SESSION.GetBNRRecyclerNearEmpty(1), adjustCount, recyclerBoxA.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_A].count - recyclerBoxA.ulCurCount;
						//if(adjustCount>0){//实际比软计数大，调整计数
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxA.ulCurCount = cashUnit[BNR_RECYCLER_BOX_A].count;	
						//LOG("循环鼓A自动补币%d张，补充后%d张；找零箱补充后%d张", adjustCount, recyclerBoxA.ulCurCount, changeBox.ulCurCount);
					}
					if(result.wRecyclerBOutCount >0){
						recyclerBoxB.ulCurCount -= result.wRecyclerBOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount = result.wRecyclerBOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxBAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(2)-recyclerBoxB.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxB.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("循环鼓B低水位%d张，自动补币%d张，补充后%d张；找零箱补充后%d张", theAPP_SESSION.GetBNRRecyclerNearEmpty(2), adjustCount, recyclerBoxB.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_B].count - recyclerBoxB.ulCurCount;
						//if(adjustCount>0){//实际比软计数大，调整计数
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxB.ulCurCount = cashUnit[BNR_RECYCLER_BOX_B].count;
						//LOG("循环鼓B自动补币%d张，补充后%d张；找零箱补充后%d张", adjustCount, recyclerBoxB.ulCurCount, changeBox.ulCurCount);
					}
					if(result.wRecyclerCOutCount >0){
						recyclerBoxC.ulCurCount -= result.wRecyclerCOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount = result.wRecyclerCOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxCAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(3)-recyclerBoxC.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxC.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("循环鼓C低水位%d张，自动补币%d张，补充后%d张；找零箱补充后%d张", theAPP_SESSION.GetBNRRecyclerNearEmpty(3), adjustCount, recyclerBoxC.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_C].count - recyclerBoxC.ulCurCount;
						//if(adjustCount>0){//实际比软计数大，调整计数
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxC.ulCurCount = cashUnit[BNR_RECYCLER_BOX_C].count;
						//LOG("循环鼓C自动补币%d张，补充后%d张；找零箱补充后%d张", adjustCount, recyclerBoxC.ulCurCount, changeBox.ulCurCount);
					}
					if(result.wRecyclerDOutCount >0){
						recyclerBoxD.ulCurCount -= result.wRecyclerDOutCount;
						m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount = result.wRecyclerDOutCount;
						if (theTVM_SETTING.GetBNRChangeBoxAmount()==theTVM_SETTING.GetBNRRecycleBoxDAmount()){
							int adjustCount = min(theAPP_SESSION.GetBNRRecyclerNearEmpty(4)-recyclerBoxD.ulCurCount, changeBox.ulCurCount);
							if (adjustCount>0){
								recyclerBoxD.ulCurCount += adjustCount;
								changeBox.ulCurCount -= adjustCount;
								LOG("循环鼓D低水位%d张，自动补币%d张，补充后%d张；找零箱补充后%d张", theAPP_SESSION.GetBNRRecyclerNearEmpty(4), adjustCount, recyclerBoxD.ulCurCount, changeBox.ulCurCount);
							}
						}
						//int adjustCount = cashUnit[BNR_RECYCLER_BOX_D].count - recyclerBoxD.ulCurCount;
						//if(adjustCount>0){//实际比软计数大，调计数
						//	changeBox.ulCurCount -= adjustCount;
						//}
						//recyclerBoxD.ulCurCount = cashUnit[BNR_RECYCLER_BOX_D].count;
						//LOG("循环鼓D自动补币%d张，补充后%d张；找零箱补充后%d张", adjustCount, recyclerBoxD.ulCurCount, changeBox.ulCurCount);
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


		//// 纸币找零成功
		//if(0 == nResult){
		//	LOG("纸币找零成功。");
		//	m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_SUCCESS;
		//	// 同步计数
		//	
		//	LOG("-------------------------纸币找零前--------------------------");
		//	LOG("纸币箱A找零前：%d",banknoteAInfo.ulCurCount);
		//	LOG("纸币箱B找零前：%d",banknoteBInfo.ulCurCount);
		//	LOG("纸币箱废币箱找零前：%d",banknoteInvalidInfo.ulCurCount);

		//	bhChangeSendOut->GetChangeResponse(&bankChangeInfo);

		//	LOG("-------------------------纸币找零信息--------------------------");
		//	LOG("纸币箱A出币：%d",bankChangeInfo.dwBoxASendOutCount);
		//	LOG("纸币箱B出币：%d",bankChangeInfo.dwBoxBSendOutCount);

		//	LOG("纸币箱A进废币箱：%d次",bankChangeInfo.dwBoxARejectionCount);
		//	LOG("纸币箱B进废币箱：%d次",bankChangeInfo.dwBoxBRejectionCount);
		//	//banknoteAInfo.ulCurCount -= m_pCommonModel->paymentInfo.dwChangeBHBoxACount;
		//	//banknoteBInfo.ulCurCount -= m_pCommonModel->paymentInfo.dwChangeBHBoxBCount;
		//	
		//	// 判断实际数量
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
		//// 纸币找零失败
		//else{

		//	// 如果是发送或者接收错误，直接打印故障单
		//	if(nResult == CBHChangeException::ERROR_BH_SEND || nResult == CBHChangeException::ERROR_BH_RECEIVE){
		//		m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//		m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//		throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//	}
		//	else{
		//		LOG("-------------------------纸币找零前--------------------------");
		//		LOG("纸币箱A找零前：%d",banknoteAInfo.ulCurCount);
		//		LOG("纸币箱B找零前：%d",banknoteBInfo.ulCurCount);
		//		LOG("纸币箱废币箱找零前：%d",banknoteInvalidInfo.ulCurCount);

		//		// 取一下状态，同步一下计数
		//		bhChangeSendOut->GetChangeResponse(&bankChangeInfo);

		//		LOG("-------------------------纸币找零信息--------------------------");
		//		LOG("纸币箱A出币：%d",bankChangeInfo.dwBoxASendOutCount);
		//		LOG("纸币箱B出币：%d",bankChangeInfo.dwBoxBSendOutCount);

		//		LOG("纸币箱A进废币箱：%d次",bankChangeInfo.dwBoxARejectionCount);
		//		LOG("纸币箱B进废币箱：%d次",bankChangeInfo.dwBoxBRejectionCount);

		//		// 判断实际数量
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

		//		// 剩余没有送出的张数
		//		DWORD dwLastUnSendOut = (dwDisBoxACount > 0 ? dwDisBoxACount : 0) + (dwDisBoxBCount > 0 ? dwDisBoxBCount : 0);
		//		DWORD dwLastUnSendOutAmount = (dwDisBoxACount>0 ? dwDisBoxACount * TransBankNoteValue(theTVM_INFO.GetBHChangeBoxAType()) : 0) + (dwDisBoxBCount>0 ? dwDisBoxBCount * TransBankNoteValue(theTVM_INFO.GetBHChangeBoxBType()) : 0);
		//		// 如果某个箱子空了，切换后再找零，其他的则抛异常
		//		if(CBHChangeHardwareException::ERR_Empty1stCST == nResult || CBHChangeHardwareException::ERR_Empty2ndCST == nResult){
		//			// 如果已经找够了，不需要切换箱子再找
		//			if((bankChangeInfo.dwBoxASendOutCount + bankChangeInfo.dwBoxBSendOutCount) >= (m_pCommonModel->paymentInfo.dwChangeBHBoxACount + m_pCommonModel->paymentInfo.dwChangeBHBoxBCount)){
		//				OnReturnFinish();
		//			}
		//			else{
		//				if(dwLastUnSendOut > 0){
		//					if(CBHChangeHardwareException::ERR_Empty1stCST == nResult && m_pCommonModel->paymentInfo.dwChangeBHBoxACount > 0){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox2NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteBInfo.ulCurCount){
		//							LOG("纸币找零失败，金额：%d",dwLastUnSendOutAmount);
		//							m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//							m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//							throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//						}
		//						else{// 找零足够，再从B找零
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxACount = 0;
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = dwLastUnSendOut;
		//							StartBHChange();
		//						}
		//					}
		//					else if(CBHChangeHardwareException::ERR_Empty2ndCST == nResult && m_pCommonModel->paymentInfo.dwChangeBHBoxBCount > 0){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteAInfo.ulCurCount){
		//							LOG("纸币找零失败，金额：%d",dwLastUnSendOutAmount);
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
		//		// 进废币箱，需要重试3次
		//		else if((bankChangeInfo.dwBoxARejectionCount > 0 || bankChangeInfo.dwBoxBRejectionCount > 0) && 
		//			(bankChangeInfo.dwBoxASendOutCount == 0 && bankChangeInfo.dwBoxBSendOutCount == 0) &&
		//			(m_pCommonModel->paymentInfo.dwBanknoteChangeRetryCount < 3)){
		//				m_pCommonModel->paymentInfo.dwBanknoteChangeRetryCount++;
		//				// 如果原有的箱子数量与安装状态正常,则还按照原来的方式去找零
		//				if((theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || banknoteAInfo.ulCurCount ==0 ) &&
		//					m_pCommonModel->paymentInfo.dwChangeBHBoxACount >0){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox1NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox2NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteBInfo.ulCurCount){
		//							LOG("纸币找零失败，金额：%d",dwLastUnSendOutAmount);
		//							m_pCommonModel->operationInfo.banknoteChangeState = BANKNOTE_CHANGE_STATE_FAIL;
		//							m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount = dwLastUnSendOutAmount;//m_pCommonModel->paymentInfo.dwChangeBHAmout;
		//							throw CBHChangeHardwareException(nResult,_T(__FILE__),__LINE__);
		//						}
		//						else{// 找零足够，再从B找零
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxACount = 0;
		//							m_pCommonModel->paymentInfo.dwChangeBHBoxBCount = dwLastUnSendOut;
		//							StartBHChange();
		//						}
		//				}
		//				else if((theTVM_STATUS_MGR.GetBanknoteChangeBox2NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || banknoteBInfo.ulCurCount == 0) &&
		//					m_pCommonModel->paymentInfo.dwChangeBHBoxBCount >0 ){
		//						theTVM_STATUS_MGR.SetBanknoteChangeBox2NumStatus(MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY);
		//						if(theTVM_STATUS_MGR.GetBanknoteChangeBox1NumStatus() == MAGAZINE_NUM_STATUS::MAG_NUM_EMPTY || dwLastUnSendOut > banknoteAInfo.ulCurCount){
		//							LOG("纸币找零失败，金额：%d",dwLastUnSendOutAmount);
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
		//					StartBHChange();// 按照原先的计划重新找零一次
		//				}
		//		}
		//		else{
		//			LOG("纸币找零失败，金额：%d",dwLastUnSendOutAmount);
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
		LOG("纸币找零失败。");
		OnError(e);
	}
	catch(...){
		LOG("纸币找零异常不能正常捕获，找零失败。");
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      银行卡插入实现函数

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
@brief      银行卡退出实现函数

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
@brief      密码键盘按下实现函数

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
@brief      保存票卡数据到DataModel

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW读卡反馈信息

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::SaveCardInfo(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      清理DataModel中票卡数据

@param      void

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::EmptyCardInfo()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      完成EP支付剩余的工作

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW读卡反馈信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::FinishEPPayment(LPRW_READ_CARD_RESPONSE pReadCardResponse){
	return 0;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      确认卡信息,并且执行充值操作

@param      LPRW_READ_CARD_RESPONSE pReadCardResponse RW读卡反馈信息

@retval     无

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::ConfirmCard(LPRW_READ_CARD_RESPONSE pReadCardResponse)
{
	return true;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      黑名单锁卡

@param      (o)RW_BUSSINESS_RESPONSE&	response	锁卡反馈数据

@retval     long	错误代码

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::BlockCard()
{
	UINT resultCode;
	RW_BUSSINESS_RESPONSE response;	
	try{	
		LOG("黑名单锁卡。");
		RW_CMD_BLOCK rw_cmd_block;
		rw_cmd_block.SerialNo = m_pCommonModel->dwUDSN;
		rw_cmd_block.TranDateTime = m_pCommonModel->transTime;
		resultCode = RECHARGERW_HELPER->Block(rw_cmd_block,response);
		// 锁卡成功
		if(resultCode == RW_RSP_OK) {
			LOG("锁卡成功！");
			// 更新交易流水号
			theCOUNT_INFO.UpdateUDSN(response.SerialAddnum);
			// 更新UD数据
			if(response.UDlen != 0) {
				RECHARGERW_HELPER->SaveRWUDData(response.UDlen,response.UDData);
			}
			// 更新AR数据
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
@brief      获取产品票价(这里只包含从TPU计算的部分。特别情形需在具体Svc中具体写出)

@param		(o)long& price 票价

@retval     bool	true:成功	false:失败

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::CalculatePrice() {

	// 计算票价
	RW_CMD_TICKET_PRICE query;
	RW_CMD_TICKET_PRICE_RES result;
	//计算税费
	RW_CMD_TICKET_TAX taxInput;
	RW_CMD_TICKET_TAX_RES taxRsp;
	taxInput.CardType = m_pCommonModel->mediumInfo.cardPHType;

	query.CardType = m_pCommonModel->mediumInfo.cardPHType; // 查阅了接口文档,这里需要的是CardType_t类型。
	query.ProductCategory = static_cast<WORD>(m_pCommonModel->productInfo.productCategory);
	query.ProductType = m_pCommonModel->productInfo.productType;
	query.ProductSubType = m_pCommonModel->productInfo.productSubType;
	if(query.ProductType == SJT){
		query.PriceType = 0x01; // 单程票按消费票价查询票价/*:暂时改为01,因为接口文档与实际不符合！！！zhengxianle*/
	}
	else{
		query.PriceType = 0x02; // 其他按销售票价表查询票价
	}
	query.PassengerType = m_pCommonModel->mediumInfo.passengerType;
	query.Time = ComGetCurTime();

	memcpy(query.BegainStation, &m_pCommonModel->productInfo.beginLocation, 4);
	memcpy(query.EndStation, &m_pCommonModel->productInfo.endLocation, 4);
	long resultCode = -1;
	//	PhyCardType_t cardPHType = PhyCardType_t::CARD_TYPE_MIFARE_1_S70;

	try{
		// 1.售票
		if(theSERVICE_MGR.GetCurService()->GetServiceID() == ISSUE_SVC){
			// 1.1单程票
			if (m_pCommonModel->productInfo.productType == SJT)
			{
				LOG("单程票查询票价。");
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
			// 1.2其他票种
			else
			{
				LOG("非单程票查询票价。");
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
				LOG("票价查询成功。(售票)");
				// 产品销售单价
				m_pCommonModel->productInfo.lProductSaleAmount = result.TicketPrice;
				//m_pCommonModel->paymentInfo.lTaxAmount		   = theACC_BUSINESS.GetCardHolderFee(cardPHType,CARDHOLDER_FEE_TYPE_TAX,result.TicketPrice);
				m_pCommonModel->paymentInfo.lTaxAmount		   = taxRsp.TicketTax;
			}
			else{
				LOG("票价查询失败。(售票)");
			}
		}
		// 2.充值
		else{
			LOG("充值查询票价。");
			// 无硬件异常
			if(!theEXCEPTION_MGR.HasException(CRWException::TVM_RECHARGE_RW_MODULE_ID)&&
				!theEXCEPTION_MGR.HasException(CRWHardwareException::TVM_RECHARGE_RW_MODULE_ID)){	
					resultCode = RECHARGERW_HELPER->GetTicketPrice(query, result);
					if(resultCode == RW_RSP_OK){
						taxInput.SaleOrAddAmount = result.TicketPrice;
						resultCode = RECHARGERW_HELPER->GetTicketPriceTax(taxInput,taxRsp);
					}
			}
			if (resultCode == RW_RSP_OK) {
				LOG("票价查询成功。(充值)");
				m_pCommonModel->paymentInfo.lPriceAmount = result.TicketPrice;
				//m_pCommonModel->paymentInfo.lTaxAmount		   = theACC_BUSINESS.GetCardHolderFee(cardPHType,CARDHOLDER_FEE_TYPE_TAX,result.TicketPrice);
				m_pCommonModel->paymentInfo.lTaxAmount		   = taxRsp.TicketTax;
			}
			else {
				LOG("票价查询失败。(充值)");
				//theAPP_SESSION.ShowOfficeGuide(RECHARGERW_HELPER->GetTpuErrorInfo(result.DetailErrorCode));
			}
		}
	}
	catch(CSysException& e){
		LOG("票价查询产生异常");
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		LOG("票价查询产生异常");
	}


	return resultCode;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      开始收钱

@param      无

@retval     void

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartReceiveMoney()
{
	try{
		m_bRechargeConfirm = false;
		m_pCommonModel->operationInfo.isBeginBuyProduct = true;
		// 现金支付：需要判断是否可以接收纸币或硬币??这里暂无判断
		if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
			// 启动计数中计时器,等待接收钱超时
			LOG("开始收钱,现金。");
			InsertMoneyNeedStartTimer();
			StartReceiveBanknote();
			StartReceiveCoin();

		} 
		// 银行卡支付
		else if(m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD){
			StartReceiveBankCard();
		}
		// EP支付

		theSERVICE_MGR.SetState(COUNTING);		
		//// 计数中可以显示取消按钮
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
@brief      开始收硬币

@param      无

@retval     无

@exception  CSysException& e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartReceiveCoin()
{
	if (theSERVICE_MGR.GetCurService()->GetServiceID()==CHARGE_SVC){
		return;
	}

	// 当前模式支持接受硬币 并 没有硬币模块异常,则开始接收硬币
	if (((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_COIN) == DEVICE_CONTRL_CODE_PAYMENT_COIN)
		&& (!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CCHException::MODULE_ID))) {	
			try {
				LOG("开始接受硬币的投入。");
				//判断当前可接收的硬币种类
				CTCoinCountInfo::COIN_TEMPORARY1_INFO tempB = theCOIN_COUNT.GetCoinTemporary1Info();
				tempB.ulCount_1yuan = tempB.ulCount_5jiao = 0;
				theCOIN_COUNT.SetCoinTemporary1Info(tempB);
				COIN_HELPER->CH_StartReceive(this,GetAcceptCoinFaceTable()); // 忽略错误返回值
				m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
				m_pCommonModel->operationInfo.isCoinSlotOpen = true;
			}
			catch (CSysException& e){
				// 接收命令发出，有异常，需要停止接收硬币
				OnError(theEXCEPTION_MGR.GetDescription(e));
				theEXCEPTION_MGR.ProcessException(e);
				NotifyDataModelChanged();
			}
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始收纸币

@param      无

@retval     无

@exception  CSysException& e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartReceiveBanknote()
{
	// 当前模式支持接受纸币则开始接收纸币
	if (((theTVM_STATUS_MGR.GetPaymentMode()&DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE) == DEVICE_CONTRL_CODE_PAYMENT_BANKNOTE)
		&& (!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID) && !theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID))) {
			try {
				LOG("开始接受纸币的投入。");
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
@brief      开始接收银行卡

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::StartReceiveBankCard()
//{
//	// 无需判断硬件状态,因为如不符合条件,CTicketHelper::GetAvailablePayTypeList已经自动不显示选项
//	//BANKCARD_HELPER->TurnOnInsertTrigger(this);
//	theAudio_HELPER.PlayAudio(AUDIO_ID_INSERT_BANKCARD);
//	issuePayDetailInfo.isBankCardOpen = true;
//}

//////////////////////////////////////////////////////////////////////////
/*
@brief      开始用储值卡支付

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartPayUsingEPCard()
{
	//?? 这里应该是“用EP卡支付”的函数,但是这里现在只是读卡,所以一定是不对。

	// 无需判断硬件状态,因为如不符合条件,CTicketHelper::GetAvailablePayTypeList已经自动不显示选项
	/*RECHARGERW_HELPER->ReadCard(true);	
	theAudio_HELPER.PlayAudio(AUDIO_ID_SWIPE_EP_CARD);
	m_pCommonModel->operationInfo.isWaitingEPCard = true;*/
	// 弹出请刷卡的命令,直接进行消费扣款
	try{
		m_pCommonModel->operationInfo.isWaitingEPCard = true;
		// 这里调用异步读卡命令,一直做寻卡判断
		RECHARGERW_HELPER->ReadCard((SERVICE_ID)theSERVICE_MGR.GetCurService()->GetServiceID(),false,true);
		// 显示等待读卡消息框
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
@brief      现金接收完成

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnAcceptMoneyOK() // should be OnReceiveMoneyOK by name
{

	//theAPP_SESSION.GetChildViewRec()->m_pMenuDlg->ShowCancleButton(false);// 隐藏取消按钮
	LOG("OnAcceptMoneyOK");
	m_pCommonModel->operationInfo.isFinishedReceveMoney = true;
	//EndRecvTimer();
	// 开启等待超时计时器,当按钮按下后,计时器结束。
	//if (!theAPP_SESSION.IsBlindManOperation())
	if(m_bNeedClickOkButton){
		LOG("现金接受完成,显示确认按钮。");
		GetCurReceptionDlg()->ShowCompoment(CReceptionDlg::BTN_OK);
		InsertMoneyNeedStartTimer();
	}
	else{
		// 无硬件动作
		if(m_pCommonModel->lDeviceStatus == 0){
			LOG("现金接受完成,开始发票。");
			//GetCurReceptionDlg()->CallWndProc(WM_XBUTTON,2,(LPARAM)WM_BTN_OK);
			GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ISSUING,0,(LPARAM)0);
		}
		else{
			LOG("当前有硬件动作，等待完成后发票，状态为：%d",m_pCommonModel->lDeviceStatus);
		}
	}
	theSERVICE_MGR.SetForeServiceBusy(false);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      支付完成处理(关闭纸币硬币口及退出银行卡等)

@param      无

@retval     无

@exception  CSysException e
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::AcceptMoney()
{
	try{
		if (m_ePayType==PAY_WITH_SVT){
			return;
		}
		// 如果是现金支付,压钞(随后找零,进入成功支付页面)
		if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_CASH){
			// 点击开始收钱后,结束计时器。
			EndInsertTimer();
			LOG("现金支付,开始押钞。");
			StartAcceptCash();
		}
		// 如果是银行卡或者储值卡,无需压钞找零,等待发票完成。
		else if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
			OnReturnFinish();
		}		

		//?? 不过不是应该在完成支付(issueing之前)就关闭所有的口,而不应该等到压钞票这里？？
		// 例如,银行卡应该在售票/充值/等具体操作前早已退出。

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
@brief      压钞和回收

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartAcceptCash()
{
	try{
		EndRecvTimer();//钱收购了，停止计时器
		// 1.硬币回收
		if (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED) {
			// 硬币回收需要判断当前循环找零箱的数量,在CoinModuleHelper中判断。
			LOG("开始押钞硬币,已接受硬币。");
			COIN_HELPER->CH_AcceptAsync(this,m_bCoinHopperAccept); 
			m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
		}
		else{
			StartCHChange();
		}
		// 2.纸币回收		
		if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
			LOG("开始押钞纸币,已接受纸币。");
			m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;// 为了不与其他地方同步压钞冲突，这里异步压钞前，先把标识改为已压钞票。
			BANKNOTE_HELPER->CollectAsync(this);
			m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
		}
		// 纸币找零:独立于其他硬件，可以直接找零。
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
@brief      现金收纳完成

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnReceiveMoneyOK()
{
	try {
		// 1.开始找零
		if (m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_NONE) {
			//StartChange();
		}
	}
	catch (CSysException& e){
		// 找零出现异常
		OnError(e);
	}
	catch(...){
		CInnerException e = CInnerException(GetServiceID(),CInnerException::OTHER_ERR,_T(__FILE__),__LINE__);
		OnError(e);
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      找零

@param      无

@retval     bool	true:成功	false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//bool CPassengerForeSvc::StartChange()
//{
//	// 1.判断是否需要找零
//	if(m_pCommonModel->paymentInfo.lChangeAmount == 0 ){
//		LOG("无需找零,直接返回。");
//		m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_SUCCESS;
//		// 执行返回成功界面
//		OnReturnFinish();
//		return true;
//	}
//	// 2.打开找零指示灯
//	if (theAPP_SESSION.IsIOConnected() && !theEXCEPTION_MGR.HasException(CIOException::MODULE_ID))
//	{
//		LOG("打开找零指示和照明灯。");
//		/*IO_HELPER->OpenReturnLight();*/
//		IO_HELPER->OpenReturnCupIndicator();
//	}
//	// 3.开始找零(传入各个找零箱需要找出的枚数)
//	if (theAPP_SESSION.IsCHConnected() && 
//		!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)&&
//		!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){	
//		Sleep(200);//提供停止读取金额缓冲时间,200ms为一个读取金额周期.
//		// 找零前先读取一下状态
////		COIN_HELPER->ReadStatus(m_pCommonModel->paymentInfo.rspCoinModule);
//		LOG("开始找零,异步调用。");
////		COIN_HELPER->CH_Change(this,m_pCommonModel->paymentInfo.dwChangeHopper1Count, m_pCommonModel->paymentInfo.dwChangeHopper2Count, m_pCommonModel->paymentInfo.dwChangeStockCount); 
//	}
//	// 4.无法找零,打印故障单
//	else{
//		LOG("无法找零,请取故障单。");
//		m_pCommonModel->operationInfo.coinChangeState = COIN_CHANGE_STATE_FAIL;
//		OnReturnFinish();
//	}
//	return true;
//}

//////////////////////////////////////////////////////////////////////////
/**
@brief      交易成功,显示结束画面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnSuccessFinish()
{
	try{
		LOG("交易成功,返回到成功界面。");
		// 保存SC数据
		SaveSCData(); 
		if(!m_pCommonModel->isInsertToDatabase){
			// 保存数据库数据
			Data::TransactionRecord dbInfo;
			WriteDBData(dbInfo); // 写入数据库参数细节
			//dbInfo.iTransactionDetail.iState = 0; // 0：交易正常结束 1：交易故障结束 2：取消正常结束 3：取消故障结束
			DATA_HELPER->DbInsertTransactionInfo(dbInfo);
			m_pCommonModel->isInsertToDatabase = true;
		}

		// 画面流转
		//theAudio_HELPER.PlayAudio(AUDIO_ID_THANKS);
		//theSERVICE_MGR.SetState(FINISH);

		FlowToFinishDlg();
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
		FlowToFinishDlg();
		LOG("写入数据库失败。");
	}
	catch(...){

		LOG("写入数据库失败。");
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      保存SC数据

@param      none

@retval  	无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::SaveSCData()
{
	LOG("保存SC数据。");
	theSCAudit_MGR.addCoinChangeSum(m_pCommonModel->paymentInfo.lCHReceiveTotalAmount); // 硬币找零金额
	theSCAudit_MGR.addCoinIncomeSum(m_pCommonModel->paymentInfo.lChangeAmount);				// 硬币收入金额
	theSCAudit_MGR.addNoteIncomeSum(m_pCommonModel->paymentInfo.lBHReceiveTotalAmount); // 纸币收入金额
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      写入数据库数据

@param      none

@retval  	无

@exception  CSysException
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::WriteDBData(Data::TransactionRecord &dbInfo) {
	LOG("写入数据");
	// 交易信息主表部分
	dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"), m_pCommonModel->dwUDSN); 
	dbInfo.iTransactionDetail.dtTransTime = m_pCommonModel->transTime;

	// 售票
	if(ISSUE_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
		// 售卡
		dbInfo.iTransactionDetail.iUdType = 2;
		dbInfo.iTransactionDetail.iUdSubtype = 0;
		// 金额
		dbInfo.iTransactionDetail.iUnitAmount = static_cast<int>(m_pCommonModel->productInfo.lProductSaleAmount);
		// 数量
		dbInfo.iTransactionDetail.iQuantity   = static_cast<int>(m_pCommonModel->productInfo.nProductPiece);
		dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = m_pCommonModel->productInfo.nProductIssuedPiece;
	}
	// 充值
	else if(CHARGE_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
		dbInfo.iTransactionDetail.iUdType = 4;
		// 金额
		dbInfo.iTransactionDetail.iUnitAmount = static_cast<int>(m_pCommonModel->paymentInfo.lPaidAmount);
		m_pCommonModel->paymentInfo.lDueAmount = m_pCommonModel->paymentInfo.lPaidAmount;
		// 数量
		dbInfo.iTransactionDetail.iQuantity   = 1;
		dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = 0;
	}
	// 售产品
	else if(ISSUE_PRODUCT_SVC == theSERVICE_MGR.GetCurService()->GetServiceID()){
		dbInfo.iTransactionDetail.iUdType = 3;
		// 金额
		dbInfo.iTransactionDetail.iUnitAmount = static_cast<int>(m_pCommonModel->paymentInfo.lDueAmount);
		// 数量
		dbInfo.iTransactionDetail.iQuantity   = 1;
		dbInfo.iTransactionDetail.iUnReleaseedMediaQuantity = 0;
	}

	dbInfo.iTransactionDetail.iCardType = static_cast<int>(m_pCommonModel->mediumInfo.cardPHType); // CardType_t
	dbInfo.iTransactionDetail.iProductType = static_cast<int>(m_pCommonModel->productInfo.productType);
	dbInfo.iTransactionDetail.iProductSubType = static_cast<int>(m_pCommonModel->productInfo.productSubType);
	dbInfo.iTransactionDetail.iOrigin = static_cast<int>(m_pCommonModel->productInfo.beginLocation);
	dbInfo.iTransactionDetail.iDestination = static_cast<int>(m_pCommonModel->productInfo.endLocation);
	dbInfo.iTransactionDetail.DepositAmount = static_cast<int>(m_pCommonModel->mediumInfo.lCardDeposit); //?? 票卡押金还是产品押金？
	dbInfo.iTransactionDetail.TaxAmount = static_cast<int>(m_pCommonModel->paymentInfo.lTaxAmount); 
	dbInfo.iTransactionDetail.CardFeeAmount = static_cast<int>(m_pCommonModel->mediumInfo.lCardFee);
	dbInfo.iTransactionDetail.iPaymentMethod = static_cast<int>(m_pCommonModel->paymentInfo.paymentMethod);
	dbInfo.iTransactionDetail.iTotalAmount = static_cast<int>(m_pCommonModel->paymentInfo.lDueAmount); 
	dbInfo.iTransactionDetail.CoinInsertAmount = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
	dbInfo.iTransactionDetail.CoinState = m_pCommonModel->operationInfo.coinAcceptState; // 0：已投入 1：已收纳  2：已退出
	dbInfo.iTransactionDetail.BankNoteAcceptAmount = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
	dbInfo.iTransactionDetail.BanknoteAcceptState = m_pCommonModel->operationInfo.banknoteAcceptState; // 0：已投入 1：已收纳  2：已退出
	dbInfo.iTransactionDetail.iChangeAmount = m_pCommonModel->paymentInfo.lChangeAmount;// 应找零金额
	dbInfo.iTransactionDetail.iCoinChangeAmount = static_cast<int>(m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount * 100 + m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount * 50);
	dbInfo.iTransactionDetail.iCoinChangeState = m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_SUCCESS ? 1:0; // 0：未放出  1：已放出
	dbInfo.iTransactionDetail.iBanknoteChangeAmount = m_pCommonModel->paymentInfo.dwChangeBHAmout;
	dbInfo.iTransactionDetail.iBanknoteChangeState = m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_SUCCESS ? 1:0; // 0：未放出  1：已放出
	dbInfo.iTransactionDetail.iPaymentState = m_pCommonModel->operationInfo.paymentState; // 0：未支付 1：已支付 2：支付失败
	if (m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_BANK_CARD) {
		for (int i = 0; i < 10; ++i) {
			CString strTemp(_T(""));
			strTemp.Format(_T("%d"), m_pCommonModel->paymentInfo.BankCardNum[i]);
			dbInfo.iTransactionDetail.iPayCardNo += strTemp;
		}
	}

	dbInfo.iTransactionDetail.iPrintState = m_pCommonModel->operationInfo.failurePrintState; // 0：未打印 1：已打印 2：打印失败
	//dbInfo.iTransactionDetail.iState; // 对于不同结束方式的具体情形具体写出
	switch (m_pCommonModel->operationInfo.finishType){
	case DEFAULT:
		dbInfo.iTransactionDetail.iState = 0;// 交易正常结束
		break;		
	case NOT_ENOUGH_TICKETS_FOR_BOOKING:
	case TRANSACTION_STATE_FAILED:
	case CHANGE_FALIED_RETURN:
		dbInfo.iTransactionDetail.iState = 1;// 交易故障结束
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
		dbInfo.iTransactionDetail.iState = 2;// 取消正常结束
		break;
	case SERVICE_ERROR_CANCEL:
	case EXCEPTION_ERROR:		
		// 不退钱且已发票：正常结束
		if(m_pCommonModel->operationInfo.mapException.size() == 0 && m_pCommonModel->productInfo.nProductIssuedPiece > 0){
			dbInfo.iTransactionDetail.iState = 0;
		}
		// 需要退钱的
		else if(m_pCommonModel->operationInfo.mapException.size() > 0){
			// 已发票
			if(m_pCommonModel->productInfo.nProductIssuedPiece > 0){
				dbInfo.iTransactionDetail.iState = 1;// 交易故障结束
				dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"),m_pCommonModel->dwTransactionID);
			}
			// 未发票
			else{
				dbInfo.iTransactionDetail.iState = 3;// 取消故障结束
				dbInfo.iTransactionDetail.iUDSN.Format(_T("%010u"),m_pCommonModel->dwTransactionID);
			}
		}
		else{
			dbInfo.iTransactionDetail.iState = 2;// 取消正常结束
			dbInfo.iTransactionDetail.iUDSN = _T("0"); 
		}
		break;
	default:
		break;
	}
	dbInfo.vTransCoinDetail.clear();
	map<BankNoteAndCoinType_t,long>::iterator it;

	// 记录硬币投入信息
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
	// 记录硬币收纳信息
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


	// 记录硬币找零信息
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
				// 非硬币找零，则直接跳过，不插库
				continue;
			}
			transCoin.iQuantity = static_cast<int>(changeIter->second);
			dbInfo.vTransCoinChangeDetail.push_back(transCoin);
		}
	}

	// 记录纸币投入信息
	for (it = m_pCommonModel->paymentInfo.PayBankNoteInfo.begin(); it != m_pCommonModel->paymentInfo.PayBankNoteInfo.end(); ++it) {
		Data::BanknoteInsertRecord transCoin;
		transCoin.dtOperationTime = m_pCommonModel->transTime;
		transCoin.iCoinTypeCode = static_cast<int>(it->first);
		transCoin.iQuantity = static_cast<int>(it->second);
		dbInfo.vTransBanknoteDetail.push_back(transCoin);
	}

	// 记录纸币收纳信息
	for (it = m_pCommonModel->paymentInfo.PayBankNoteInfo.begin(); it != m_pCommonModel->paymentInfo.PayBankNoteInfo.end(); ++it) {
		if(dbInfo.iTransactionDetail.iState != 2){	// 取消不入库
			Data::BanknoteAcceptRecord transCoin;
			transCoin.dtOperationTime = m_pCommonModel->transTime;
			transCoin.iCoinTypeCode = static_cast<int>(it->first);
			transCoin.iQuantity = static_cast<int>(it->second);
			transCoin.txtAcceptCoinBoxID = theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString();
			dbInfo.vTransBanknoteAcceptDetail.push_back(transCoin);
		}
	}

	// 纸币箱找零明细(只有找零成功才可以入库)
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

	// 添加故障明细表
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
@brief      退币(纸币硬币缓存区)

@param      无

@retval     bool	true:成功	false:失败

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::StartReturnCash()
{
	try{
		LOG("退币。");
		int res = 0;
		// 当有接收到硬币时硬币退币
		if (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED) {
			// 无异常时,执行退币操作。
			if (!theEXCEPTION_MGR.HasException(CCHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){	
				if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
					m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
					COIN_HELPER->CH_RefundAsync(this);
				}
			}
		}
		// 当有接收到纸币时纸币退币
		if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
			if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
				BANKNOTE_HELPER->ReturnAllAsync(this);
			}
		}

		// 保存数据库数据,why???zxl
		if (theSERVICE_MGR.GetState() == COUNTING || theSERVICE_MGR.GetState() == ISSUEING) {
			//CDataHelper transData(*this);
			//Data::TransactionRecord dbInfo;
			//WriteDBData(dbInfo); // 写入数据库参数细节
			////dbInfo.iTransactionDetail.iState = (res==0)?2:3; // 0：交易正常结束 1：交易故障结束 2：取消正常结束 3：取消故障结束
			//transData.DbInsertTransactionInfo(dbInfo);
			//?? 这里对于硬币退硬是否成功无法给出判断。如果退币成功,OnCoinReturned会被回调(但是此处无从而知)
			// 如果失败,这个函数将通过调用FlowToErrorDlg的函数(比如onCancel或者各种超限处理)
			// 得到exception并进入异常结束页面。
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
@brief      取消处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnCancel()
{
	try{	
#if 1
		theSERVICE_MGR.SetState(CANCELING);
		//设置取消标记
		m_bIsPassengerCanceled = true;
		m_pCommonModel->operationInfo.finishType = PASSAGE_CANCEL;
		NotifyDataModelChanged();
		EndRecvTimer();
		OnReturnFinish();
#else
		LOG("取消处理。");
		StopCircleReadCard();
		IO_HELPER->ReturnRechargeCard();
		// 取消一定会自己判断返回,当再次点击时不响应
		if (CANCELING == theSERVICE_MGR.GetState() || (m_pCommonModel->operationInfo.finishType != DEFAULT && m_pCommonModel->operationInfo.finishType != QR_ORDER_GENERATE_FAILED)){
			return;
		}
		// 王峰 20170627隐藏控件后画面难看还会有部分残留，所以此处不再隐藏
		//theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC)->hideAllCompoment.Invoke();// 隐藏当前不需要显示的控件

		theSERVICE_MGR.SetState(CANCELING);
		//设置取消标记
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
@brief      无法充值异常处理

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnServiceErrorCancel()
{
	try{
		theSERVICE_MGR.SetForeServiceBusy(false);
		LOG("无发正常交易,异常。");
		//设置错误标记
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
@brief      出错处理

@param      (i)CSysException  系统抛出错误信息

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnError(CSysException e)
{
	try{
		// 这里只处理纸币与硬币模块异常！！！
		theSERVICE_MGR.SetForeServiceBusy(false);
		LOG("异常处理。");
		// 设置完成状态,这里设置成这个为了能够回调退钱。
		m_pCommonModel->operationInfo.finishType = SERVICE_ERROR_CANCEL;
		// 添加到异常队列
		theEXCEPTION_MGR.ProcessException(e);
		theSERVICE_MGR.SetForeServiceBusy(true);

		// 1.硬币异常:投入,找零,退币。
		if(e.GetModuleID() == CCHHardwareException::MODULE_ID || e.GetModuleID() == CCHException::MODULE_ID){
			// 找零
			if(m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_FAIL){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_COIN] = m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount;
				m_pCommonModel->operationInfo.finishType = CHANGE_FALIED_RETURN;
			}
			// 退币
			else if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_RETURN_FAILD ){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
			}
			// 收纳
			else if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_COLLECTED_FAILD){
				// 收纳失败无法找零，异常金额应该为找零金额
				if(m_pCommonModel->paymentInfo.lChangeAmount != 0){
					m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount = m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount * 100 + m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount * 50;
					m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_COIN] = m_pCommonModel->paymentInfo.dwCoinChangeErrorAmount;
				}
				else{
					theSERVICE_MGR.SetForeServiceBusy(false);
				}
			}
			// 投入异常:需要停止接收硬币和纸币zhengxianle
			else{
				// 投入失败时需要停止计时器
				EndInsertTimer();

				m_pCommonModel->operationInfo.finishType = EXCEPTION_ERROR;

				if(!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
					// 纸币检测(无纸币异常)
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
				LOG("硬币模块当前接收状态：%d",m_pCommonModel->operationInfo.coinAcceptState);
				// 硬币检测
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
					// 退币失败呀
					if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED){
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
					}
				}
				catch(...){
					LOG("停止识别失败。");
				}

				// 将暂存计数
				CTCoinCountInfo::COIN_TEMPORARY1_INFO tempB = theCOIN_COUNT.GetCoinTemporary1Info();
				if(tempB.ulCount_1yuan != 0){
					CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
					hopperB.ulCurCount += tempB.ulCount_1yuan;
					theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);
					tempB.ulCount_1yuan = 0;
					theCOIN_COUNT.SetCoinTemporary1Info(tempB);
				}

				if(m_pCommonModel->paymentInfo.lCHReceiveTotalAmount >0){
					// 需要打印故障单
					if(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED){
						LOG("需要打印硬币异常的收据单，金额：%d",m_pCommonModel->paymentInfo.lCHReceiveTotalAmount);
						m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_COIN] = m_pCommonModel->paymentInfo.lCHReceiveTotalAmount;
					}
				}
				else{
					LOG("此时没有接收到硬币，设置状态为未接收。");
					m_pCommonModel->operationInfo.coinAcceptState = COIN_ACCEPT_STATE_NONE;
				}
			}

		}
		// 2.纸币异常:投入,退钞
		if(e.GetModuleID() ==  CBRHHardwareException::MODULE_ID){
			// 找零失败
			if(m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_FAIL && m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount > 0){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_BANKNOTE] = m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount;
				m_pCommonModel->operationInfo.finishType = CHANGE_FALIED_RETURN;
			}

			// 纸币退币失败
			if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURN_FAILED){
				m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RETURN_BANKNOTE] = m_pCommonModel->paymentInfo.lBHReceiveTotalAmount;
			}
			// 纸币押钞失败：不影响找零，所以影响金额为0
			else if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED){
				//m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_BANKNOTE_COLLECTION] = 0;
			}
			// 纸币投入失败:停止接收，并退出所有已收的现金zhengxianle
			else{
				// 投入失败时需要停止计时器
				EndInsertTimer();

				m_pCommonModel->operationInfo.finishType = EXCEPTION_ERROR;

				if(!theEXCEPTION_MGR.HasException(CCHHardwareException::MODULE_ID)){
					// 硬币是否投入？退币
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


				// 纸币卡币
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
		//// 3.纸币找零异常：出币错误
		//else if(e.GetModuleID() == CBHChangeHardwareException::MODULE_ID){
		//	// 找零失败
		//	if(m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_FAIL && m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount > 0){
		//		m_pCommonModel->operationInfo.mapException[TRANSACTION_EXCEPTION_RELEASE_BANKNOTE] = m_pCommonModel->paymentInfo.dwBanknoteChangeErrorAmount;
		//		m_pCommonModel->operationInfo.finishType = CHANGE_FALIED_RETURN;
		//	}
		//}
	}
	catch (CSysException& e){
		// 再次出现异常,需要再次加入异常队列。
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
@brief      出错处理

@param      (i)CString errorMsg  错误信息文言
@param      (ii)DWORD dwReceptMsg

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnError(CString errorMsg, DWORD dwReceptMsg)
{
	try{
		LOG("错误处理,错误提示。%s",errorMsg);
	}
	catch (CSysException& e){
	}
	catch(...){
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      具体出错处理,页面流转

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnErrorFinish(){
	LOG("具体错误处理,细节。");
	if (m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_WITH_WEIXIN || m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_WITH_ZHIFUBAO){
		// 保存数据库数据
		if(!m_pCommonModel->isInsertToDatabase){
			Data::TransactionRecord dbInfo;
			WriteDBData(dbInfo); // 写入数据库参数细节
			DATA_HELPER->DbInsertTransactionInfo(dbInfo);
			m_pCommonModel->isInsertToDatabase = true;
		}		

		// 设置结束状态
		theSERVICE_MGR.SetForeServiceBusy(false);
		// 转到错误界面
		FlowToErrorDlg();
		return;
	}

	m_pCommonModel->paymentInfo.lReturnAmount = 0;
	m_pCommonModel->paymentInfo.lNotChangeAmount = 0;

	// 有异常
	if(m_pCommonModel->operationInfo.mapException.size() > 0){
		auto it = m_pCommonModel->operationInfo.mapException.begin();

		while(it != m_pCommonModel->operationInfo.mapException.end()){
			// 对应类型
			switch(it->first){
			case TRANSACTION_EXCEPTION_CARD_MOUDOL:// 发卡过程遇到异常
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
			case TRANSACTION_EXCEPTION_RELEASE_COIN:// 硬币找零异常
			case TRANSACTION_EXCEPTION_RELEASE_BANKNOTE:// 纸币找零异常
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
		// 添加交易失败信息到业务数据传输审计
		BUSINESS_FAILED_DATA busiess_falied_data;
		busiess_falied_data.DeviceID = theMAINTENANCE_INFO.GetCurrentDevice();		// 设备编号
		busiess_falied_data.DateTime = m_pCommonModel->transTime = ComGetCurTime(); // 故障时间
		busiess_falied_data.DeviceAfcNum = m_pCommonModel->dwTransactionID;			// 故障交易流水号,会自动累加
		busiess_falied_data.TransactionNum = m_pCommonModel->dwUDSN;				// 对应交易流水号
		busiess_falied_data.RefoundAmount = m_pCommonModel->paymentInfo.lReturnAmount;// 应退金额
		// 故障原因
		string strFailedReason = GetFailedReason();
		memcpy(busiess_falied_data.FailedReason,strFailedReason.c_str(),sizeof(busiess_falied_data.FailedReason));
		theSERVICE_MGR.GetService<CBusinessTranslateSvc>(BUSINESS_INTERVAL_SVC)->InsertMsgBussnessFailedData(busiess_falied_data); 

		LOG("故障单金额：%d分",m_pCommonModel->paymentInfo.lReturnAmount);

		// 5.打印故障单
		//if(theTVM_STATUS_MGR.GetPrintMode() == 0)	
		PrinterPrintErrorReceipt();
		if(theEXCEPTION_MGR.HasException(CPrinterException::MODULE_ID)){
			// 打印失败标志
			m_pCommonModel->operationInfo.failurePrintState = FAILURE_PRINT_STATE_FAIL;
		}
		else{
			// 打印成功标志
			m_pCommonModel->operationInfo.failurePrintState = FAILURE_PRINT_STATE_SUCCESS;
		}

		// 6.保存审计数据
		SaveSCData();
		// 7.保存数据库数据
		if(!m_pCommonModel->isInsertToDatabase){
			Data::TransactionRecord dbInfo;
			WriteDBData(dbInfo); // 写入数据库参数细节
			DATA_HELPER->DbInsertTransactionInfo(dbInfo);
			m_pCommonModel->isInsertToDatabase = true;
		}		

		// 8.设置结束状态
		theSERVICE_MGR.SetForeServiceBusy(false);
		// 9.转到错误界面
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
@brief      取交易失败原因

@param      无

@retval     string 错误原因

@exception  无
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
				strFailedReason = "51";//"51:TVM现金卡票";
			}
			else{
				strFailedReason = "54";//"54:非现金卡票";
			}
			break;
		case TRANSACTION_EXCEPTION_RELEASE_COIN:
			//strFailedReason = "Release Coin Exception";
			strFailedReason = "53";//"53:TVM现金少找零";
			break;
		case TRANSACTION_EXCEPTION_RELEASE_BANKNOTE:
			//strFailedReason = "Release Banknote Exception";
			strFailedReason = "53";//"53:TVM现金少找零";
			break;
		case TRANSACTION_EXCEPTION_CREDIT:
			strFailedReason = "99";//"99:其它";
			break;
		case TRANSACTION_EXCEPTION_BANKCARD:
			strFailedReason = "99";//"99:其它";
			break;
		case TRANSACTION_EXCEPTION_EP:
			strFailedReason = "99";//"99:其它";
			break;
		case TRANSACTION_EXCEPTION_RETURN_BANKNOTE:
			//strFailedReason = "Return Banknote Exception";
			strFailedReason = "50";//"50:TVM卡币";
			break;
		case TRANSACTION_EXCEPTION_RETURN_COIN:
			//strFailedReason = "Return Coin Exception";
			strFailedReason = "50";//"50:TVM卡币";
			break;
		case TRANSACTION_EXCEPTION_WRITE_CARD_FAILED:
			//strFailedReason = "Write Card Exception";
			if (m_pCommonModel->paymentInfo.paymentMethod==PAYMENT_METHOD_CASH){
				strFailedReason = "52";//"52:TVM现金发售无效票";
			}
			else{
				strFailedReason = "55";//"55:非现金发售无效票";
			}
			break;
		default:
			strFailedReason = "99";//"99:其它";
			break;
		}
	}
	return strFailedReason;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      关闭所有硬件

@param      (i)CString errorMsg  错误信息文言
@param      (i)bool bContinue    是否继续读卡标记

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::CloseAllDevice()
{
	LOG("关闭所有接收口。");

	try{
		// 关硬币口
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
		// 关纸币口
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
		// 停止读储值卡
		if (m_pCommonModel->operationInfo.isWaitingEPCard){
			RECHARGERW_HELPER->StopReadCard();
			m_pCommonModel->operationInfo.isWaitingEPCard = false;
		}
		// 停止读银行卡,退出银行卡
		if (m_pCommonModel->operationInfo.isBankCardSlotOpen) {
			//BANKCARD_HELPER->TurnOffInsertTrigger(); // 退银行卡,关闭进口
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
@brief      是否投入足够多现金

@param      CRWReadCard * 读卡命令

@retval     无

@exception  
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsReceiveEnoughMoney()
{
	bool res = false;
	EnterCriticalSection(&m_critical_Section); //?? 这个什么用。是不是可以撤掉这个函数？？直接在OnCoinInserted之类里面判断？？
	if (m_pCommonModel->paymentInfo.lChangeAmount >= 0) { // NotifyDataModelChanged();已经更新了m_pCommonModel->paymentInfo.lChangeAmount
		res = true;
	}
	LeaveCriticalSection(&m_critical_Section);
	return res;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief  供OnSuccessFinish使用的一个函数封装

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::FlowToFinishDlg()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转到默认业务

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::FlowToDefaultDlg(){

}
//////////////////////////////////////////////////////////////////////////
/**
@brief   供OnErrorFinish,OnCancel和各种超限使用的一个函数封装

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::FlowToErrorDlg()
{
	try{
		// 防止记录写重复
		if(m_pCommonModel->operationInfo.finishType != TRANSACTION_STATE_FAILED && 
			m_pCommonModel->operationInfo.finishType != NOT_ENOUGH_TICKETS_FOR_BOOKING && !m_pCommonModel->isInsertToDatabase){
				// 取消交易也需要记录在数据库中
				Data::TransactionRecord dbInfo;
				WriteDBData(dbInfo); // 写入数据库参数细节
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
@brief      打印销售凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::PrinterPrintReceipt()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      打印错误凭证

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::PrinterPrintErrorReceipt()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      更新硬币接受金额信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewCoinReceivedAmount(tCHACoinValueInfo chAmountInfo)
{
	// 这个函数可以执行多次
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
	LOG("硬币投入总额：%d分。",m_pCommonModel->paymentInfo.lCHReceiveTotalAmount);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      更新纸币接受金额信息

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewBanknoteReceivedAmount(tBncCashInfo& feedback)
{
	// since "bh_status" only give the information of the very last banknote, 
	// 每次投入一张纸币后,这个函数只能执行一次
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
		// 系统中不接收的面值
		recvAmount = 0;
	}
	LOG("投入纸币：%d 元.",recvAmount/100);
	if (recvAmount != 0)
	{
		BankNoteAndCoinType_t type = (BankNoteAndCoinType_t)feedback.psBncCashInfo[0].iValue;
		m_pCommonModel->paymentInfo.PayBankNoteInfo[type] += 1;
		m_pCommonModel->paymentInfo.lBHReceiveTotalAmount += recvAmount; 
	}

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取硬币接受个数

@param      无

@retval     硬币接受个数 DWARD

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::RenewCoinReceivedCount(tCHACoinValueInfo chAmountInfo)
{
	// 这个函数可以执行多次
	m_pCommonModel->paymentInfo.lCHReceiveTotalCount = 0;
	m_pCommonModel->paymentInfo.lCHReceiveTotalCount += chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	m_pCommonModel->paymentInfo.lCHReceiveTotalCount += chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;
	// 硬币暂存器1计数信息
	CTCoinCountInfo::COIN_TEMPORARY1_INFO cointemporary1;
	cointemporary1 = theCOIN_COUNT.GetCoinTemporary1Info();
	cointemporary1.ulCount_1yuan = chAmountInfo.stCoinBoxValueInfo[0].uiCoinNum;
	cointemporary1.ulCount_5jiao = chAmountInfo.stCoinBoxValueInfo[1].uiCoinNum;
	theCOIN_COUNT.SetCoinTemporary1Info(cointemporary1);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      获取纸币接受张数

@param      无

@retval     纸币接受个数 DWORD

@exception  无
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
		// 系统中不接收的面值
		recvAmount = 0;
	}
	if (recvAmount != 0)
	{
		// 每次投入一张纸币后,这个函数只能执行一次
		m_pCommonModel->paymentInfo.lBHReceiveTotalCount += 1;

	}
}

// 排序规则
bool Compare(DWORD a,DWORD b){ return a > b;}

//////////////////////////////////////////////////////////////////////////
/*
@brief      计算找零枚数

@param      无

@retval     true 有足够的硬币可成功找零
false 硬币不够成功找零

@exception  从changeAmount计算,并存入ChangeStockCount/ChangeHopper1Count/ChangeHopper2Count
此函数依赖币种与金额的对应,采用0：无效,1：0.5MOP,2：1MOP,3：2MOP,4：5MOP
具体参照TVM.INI和Common/Source Files/TVMInfo.cpp,并体现在TransCoinValue中
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::CalculateChangeCount()
{
	// 暂时先不判断找零箱是否在位。zhengxianle
	LOG("计算找零详细.");
	if(theSERVICE_MGR.GetCurService()->GetServiceID() == CHARGE_SVC){
		LOG("充值不需要找零，返回。");
		return true;
	}

	// 1. *** 总找零金额
	long changeAmout = m_pCommonModel->paymentInfo.lChangeAmount;

	// 2. *** 硬币相关
	// 硬币面额
	int nTypeHopper1 = theTVM_INFO.GetCHHopper1Type();			// HopperA箱
	int nTypeHopper2 = theTVM_INFO.GetCHHopper2Type();			// HopperB箱
	int nTypeChangeBox1 = theTVM_INFO.GetCHChangeBoxAType();
	int nTypeChangeBox2 = theTVM_INFO.GetCHChangeBoxBType();
	DWORD dwHopper1Value = TransCoinValue(nTypeHopper1);		// 100分（右边）
	DWORD dwHopper2Value = TransCoinValue(nTypeHopper2);		// 100 分（左边）
	DWORD dwChangeBox1Value = TransCoinValue(nTypeChangeBox1);	// 100 分（右边）
	DWORD dwChangeBox2Value = TransCoinValue(nTypeChangeBox2);	// 100分（左边）
	// 硬币数量
	DWORD dwHopper1Count = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;
	DWORD dwHopper2Count = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;
	DWORD dwChangeBox1Count = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
	DWORD dwChangeBox2Count = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
	// 硬币最大可找零金额和数量
	DWORD dwCoinRealMaxAmount = dwHopper1Value * dwHopper1Count + dwHopper2Value * dwHopper2Count + dwChangeBox1Value * dwChangeBox1Count + dwChangeBox2Value * dwChangeBox2Count;
	DWORD dwCoinRealMaxCount  = dwHopper1Count + dwHopper2Count + dwChangeBox1Count + dwChangeBox2Count;

	// 3. *** 纸币相关
	// 纸币面额(一个找零箱，4个循环箱)
	int nTypeBNRRecycleBoxA = theTVM_SETTING.GetBNRRecycleBoxAAmount();
	int nTypeBNRRecycleBoxB = theTVM_SETTING.GetBNRRecycleBoxBAmount();
	int nTypeBNRRecycleBoxC = theTVM_SETTING.GetBNRRecycleBoxCAmount();
	int nTypeBNRRecycleBoxD = theTVM_SETTING.GetBNRRecycleBoxDAmount();
	int nTypeBNRChangeBox = theTVM_SETTING.GetBNRChangeBoxAmount();
	DWORD dwBNRRecycleBoxAValue = TransBankNoteValue(nTypeBNRRecycleBoxA);	// 100分
	DWORD dwBNRRecycleBoxBValue = TransBankNoteValue(nTypeBNRRecycleBoxB);	// 100分
	DWORD dwBNRRecycleBoxCValue = TransBankNoteValue(nTypeBNRRecycleBoxC);	// 500分
	DWORD dwBNRRecycleBoxDValue = TransBankNoteValue(nTypeBNRRecycleBoxD);	// 500分
	DWORD dwBNRChangeBoxValue = TransBankNoteValue(nTypeBNRChangeBox);		// 500分
	// 纸币数量
	DWORD dwBNRRecycleBoxACount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount;
	DWORD dwBNRRecycleBoxBCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount;
	DWORD dwBNRRecycleBoxCCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount;
	DWORD dwBNRRecycleBoxDCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount;
	DWORD dwBNRChangeBoxCount = 0;//theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().ulCurCount;当期纸币模块找零箱不支持找零
	// 纸币最大可找零金额和数量
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

	// 4. *** 检查配置类型是否有非法的。
	if(dwHopper1Value == 0 || dwHopper2Value == 0 || dwChangeBox1Value == 0 || dwChangeBox2Value == 0 
		|| dwBNRRecycleBoxAValue == 0 || dwBNRRecycleBoxBValue == 0 || dwBNRRecycleBoxCValue == 0 || dwBNRRecycleBoxDValue == 0 || dwBNRChangeBoxValue == 0){
			LOG("配置文件中有非法配置项。");
			return false;
	}

	// 5. *** 参数限定
	DWORD dwMaxBanknoteChangeAmout = theAFC_TVM_OPERATION.GetBHChangeMaxAmount() * 100;		// 纸币最大找零：金额(单位：分)
	DWORD dwMaxBanknoteChangeCount = theAFC_TVM_OPERATION.GetBHChangeMaxPiece();			// 纸币最大找零：数量
	DWORD dwMaxCoinChangeCount	   = theAFC_TVM_OPERATION.GetChageCoinMaxPieceOfIssue();	// 硬币最大找零：数量
	// 硬币实际可找零最大数量和金额
	if((theTVM_STATUS_MGR.GetChangeMode()&DEVICE_CONTRL_CODE_NO_BILL_CHANGE) != DEVICE_CONTRL_CODE_NO_BILL_CHANGE){
		dwMaxCoinChangeCount = 0;
	}
	else{
		dwMaxCoinChangeCount = dwCoinRealMaxCount< dwMaxCoinChangeCount ? dwCoinRealMaxCount : dwMaxCoinChangeCount;// 选取可控最大硬币找零数量
	}
	// 纸币实际可找零最大数量和金额
	if((theTVM_STATUS_MGR.GetChangeMode() & DEVICE_CONTRL_CODE_NO_COIN_CHANGE) != DEVICE_CONTRL_CODE_NO_COIN_CHANGE){
		dwMaxBanknoteChangeAmout = 0;
		dwMaxBanknoteChangeCount = 0;
	}
	else{
		dwMaxBanknoteChangeCount = dwBanknoteRealMaxCount < dwMaxBanknoteChangeCount ? dwBanknoteRealMaxCount : dwMaxBanknoteChangeCount;
		dwMaxBanknoteChangeAmout = dwBanknoteRealMaxAmount < dwMaxBanknoteChangeAmout ? dwBanknoteRealMaxAmount : dwMaxBanknoteChangeAmout;
	}


	CString strBoxInfo;
	LOG("各钱箱面额信息：");
	strBoxInfo.Format(_T("硬币找零箱1（%d），找零箱2（%d），Hopper1（%d），Hopper2（%d）"), dwChangeBox1Value, dwChangeBox2Value, dwHopper1Value, dwHopper2Value);  
	LOG("硬币---%s",strBoxInfo);
	strBoxInfo.Format(_T("纸币循环鼓（%d），纸币循环鼓B（%d），纸币循环鼓C（%d），纸币循环鼓D（%d）， 纸币找零箱（%d）"), 
		dwBNRRecycleBoxAValue,
		dwBNRRecycleBoxBValue,
		dwBNRRecycleBoxCValue,
		dwBNRRecycleBoxDValue,
		dwBNRChangeBoxValue);
	LOG("纸币---%s",strBoxInfo);

	LOG("各钱箱计数信息：");
	// 硬币
	strBoxInfo.Format(_T("硬币箱1：%d; 硬币箱2：%d，其中找零箱1 %d个，找零箱2 %d个，Hopper1 %d个，Hopper2 %d个"), 
		dwHopper1Count + dwChangeBox1Count, 
		dwHopper2Count + dwChangeBox2Count, 
		dwChangeBox1Count,
		dwChangeBox2Count,
		dwHopper1Count,
		dwHopper2Count					
		);
	LOG("硬币---%s",strBoxInfo);
	// 纸币
	strBoxInfo.Format(_T("纸币循环鼓A：%d; 纸币循环鼓B：%d; 纸币循环鼓C：%d; 纸币循环鼓D：%d; 纸币找零箱：%d"), 
		dwBNRRecycleBoxACount,
		dwBNRRecycleBoxBCount,
		dwBNRRecycleBoxCCount,
		dwBNRRecycleBoxDCount,
		dwBNRChangeBoxCount);
	LOG("纸币---%s",strBoxInfo);


	// 6. *** 检查是否足够找零
	if((dwMaxBanknoteChangeAmout + (dwMaxCoinChangeCount > 0 ? dwCoinRealMaxAmount : 0))<changeAmout){
		LOG("总金额不够，找零不足。");
		return false;
	}

	// 7. *** 计算纸币箱子的找零顺序和面额分配
	// 规则：1）纸币优先；2）大面额优先；3）找零箱优先
	// 排序纸币循环找零箱面额
	vector<DWORD> vec_value_type;
	vec_value_type.push_back(dwBNRRecycleBoxAValue);
	vec_value_type.push_back(dwBNRRecycleBoxBValue);
	vec_value_type.push_back(dwBNRRecycleBoxCValue);
	vec_value_type.push_back(dwBNRRecycleBoxDValue);
	sort(vec_value_type.begin(),vec_value_type.end(),Compare);	// 降序(500 -> 500 -> 100 -> 100)
	// 箱子找零顺序队列

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
	// 最前面加上纸币找零箱
	list_Priority.push_front(CHANGE_VALUE(BNR_CHANGE_BOX, dwBNRChangeBoxValue, dwBNRChangeBoxCount,0));
	//  根据优先顺序进行找零数量的分配	
	long nRemainChange = changeAmout; //找零剩余值
	DWORD nBanknoteCount = 0;
	long nBanknoteAmount = 0;
	list<CHANGE_VALUE>::iterator it = list_Priority.begin();
	list<CHANGE_VALUE>::iterator ie = list_Priority.end();
	while (it!=ie){
		if (it->nCurCount>0){
			long nCurCount = nRemainChange/it->dwValue;
			nCurCount = min(nCurCount, it->nCurCount);
			if ((nBanknoteCount + nCurCount > dwMaxBanknoteChangeCount) || (nBanknoteAmount + nCurCount * it->dwValue > dwMaxBanknoteChangeAmout)){
				// 找零数量或金额超限
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


	// 8. *** 计算硬币箱子的找零顺序和面额分配
	// 规则：1）纸币优先；2）大面额优先；3）找零箱优先
	// 排序硬币各箱子
	list<CHANGE_VALUE> list_CoinPriority;
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_CHANGE_BOX_2, dwChangeBox2Value, dwChangeBox2Count, 0));
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_CHANGE_BOX_1, dwChangeBox1Value, dwChangeBox1Count, 0));
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_HOPPER_2, dwHopper2Value, dwHopper2Count, 0));
	list_CoinPriority.push_back(CHANGE_VALUE(COIN_HOPPER_1, dwHopper1Value, dwHopper1Count, 0));

	//  根据优先顺序进行找零数量的分配	
	DWORD nCoinCount = 0;
	long nCoinAmount = 0;
	it = list_CoinPriority.begin();
	ie = list_CoinPriority.end();
	while (it!=ie){
		if (it->nCurCount>0){
			long nCurCount = nRemainChange/it->dwValue;
			nCurCount = min(nCurCount, it->nCurCount);
			if ((nCoinCount + nCurCount > dwMaxCoinChangeCount)/* || (nCoinAmount + nCurCount * it->dwValue > dwMaxCoinChangeAmout)*/){
				// 找零数量或金额超限
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
		// 9. *** 计算找零结果反馈
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

		m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount = tmp_oneYuanCount;//这里的一元指的是右边箱子，5角指的是左边的箱子
		m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount = tmp_halfYuanCount;

		m_pCommonModel->paymentInfo.dwChangeBHAmout = 
			dwBNRChangeBoxValue * m_pCommonModel->paymentInfo.dwChangeBNRBoxCount +
			dwBNRRecycleBoxAValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount +
			dwBNRRecycleBoxBValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount +
			dwBNRRecycleBoxCValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount +
			dwBNRRecycleBoxDValue * m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount;

		// 10. *** 打印日志
		CString strChangeInfo;
		LOG("找零信息：");
		// 硬币
		strChangeInfo.Format(_T("硬币箱1：%d; 硬币箱2：%d，其中找零箱1 %d个，找零箱2 %d个，Hopper1 %d个，Hopper2 %d个"), 
			m_pCommonModel->paymentInfo.dwChangeCoinOneYuanCount, 
			m_pCommonModel->paymentInfo.dwChangeCoinHalfYuanCount,
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_1],
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_CHANGE_BOX_2],			
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_1],
			m_pCommonModel->paymentInfo.ChargeCoinBanknoteInfo[COIN_HOPPER_2]				
		);
		LOG("硬币---%s",strChangeInfo);
		// 纸币
		strChangeInfo.Format(_T("纸币循环鼓A：%d; 纸币循环鼓B：%d; 纸币循环鼓C：%d; 纸币循环鼓D：%d; 纸币找零箱：%d"), 
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxACount,
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxBCount,
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxCCount,
			m_pCommonModel->paymentInfo.dwRecycleBNRBoxDCount,
			m_pCommonModel->paymentInfo.dwChangeBNRBoxCount);
		LOG("纸币---%s",strChangeInfo);

		return true;
	}
	// 能走到这说明金额找零足够，但是硬币找零数量超限。


	return false;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否硬币纸币都达到最大数量,供OnCoinInserted和OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CPassengerForeSvc::IsBothCoinBanknoteMaxCount() {
	// since it is a virtual function, the return value doesn't really matter.
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币达到最大数量,供OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////

bool CPassengerForeSvc::IsBanknoteMaxCount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否硬币达到最大数量,供OnCoinInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsCoinMaxCount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否纸币都达到最大金额,供OnBanknoteInserted用

@param      无

@retval     bool

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
bool CPassengerForeSvc::IsBanknoteMaxAmount() {
	return true;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断找零数量是否超限

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
@brief      判断纸币找零数量是否超限

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
@brief      判断纸币找零金额是否超限

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
@brief      转到完成界面,需要判断当前的状态,根据状态返回相应的界面

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnReturnFinish(){
	try{
		StopCircleReadCard();
		LOG("------------------------------------返回完成界面------------- <IN>");
		CMainSvc* pMainSvc = theSERVICE_MGR.GetService<CMainSvc>(MAIN_SVC);
		// 取消：只会进入这里一次！！！
		if (theSERVICE_MGR.GetState() == CANCELING && m_bIsPassengerCanceled){	
			m_bIsPassengerCanceled = false;
			// 非现金交易取消
			if(m_pCommonModel->paymentInfo.paymentMethod != PaymentMethod_t::PAYMENT_METHOD_CASH){
				if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN|| m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
					LOG("二维码购票交易乘客取消！Leave");
					FlowToErrorDlg();
					return;
				}
				if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN|| m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
					LOG("二维码购票交易乘客取消！Leave");
					FlowToErrorDlg();
					return;
				}
				if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
					m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
					BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
				}
				LOG("启动默认业务。");
				pMainSvc->StartDefaultService();
				return;
			}
			// 现金交易
			else {
				// 0.退币完成
				if(m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_RETURNED ||
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURNED &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE ||
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_RETURNED &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_RETURNED){
						LOG("启动默认业务。");
						if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
							m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
							BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
						}
						pMainSvc->StartDefaultService();
						return;
				}
				// 1.交易未进行取消,返回到默认业务。
				if (!m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen &&
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE)
				{
					LOG("交易未进行取消,返回到默认业务(取消)。");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
// 					pMainSvc->StartDefaultService();//在
				}
				// 2.交易进行未投币取消
				else if ((m_pCommonModel->operationInfo.isCoinSlotOpen || m_pCommonModel->operationInfo.isBankNoteSlotOpen) &&
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_NONE &&
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_NONE)
				{
					LOG("交易进行未投币取消(取消)。");
					// 关闭所有接收口,自动在那里返回默认界面
					CloseAllDevice();
				}
				// 3.交易进行投币取消:正常接收
				else if (m_pCommonModel->operationInfo.isCoinSlotOpen && m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED ||
					m_pCommonModel->operationInfo.isBankNoteSlotOpen && m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)
				{
					LOG("交易进行投币取消:正常接收(取消)");
					// 关闭并退币,在回调函数中返回。
					CloseAllDevice();
					if (m_bRechargeOverlimit){
						m_bRechargeOverlimit = false;
						// 退币操作
						ReturnWhenBanknotesInserted();
					}
				}
				// 4.交易进行,投币取消：部分超限
				else if((!m_pCommonModel->operationInfo.isCoinSlotOpen && m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED &&
					m_pCommonModel->operationInfo.isBankNoteSlotOpen )|| (m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen &&
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
						LOG("交易进行,投币取消：部分超限(取消)");
						// 4.1关闭接收口,如果开了
						CloseAllDevice();
						// 4.2 如果硬币数量超限,而纸币正在接收,退出硬币
						if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
							m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
							COIN_HELPER->CH_RefundAsync(this);
						}
						// 4.3 如果纸币数量超限,而硬币在接收,退出纸币
						if (((m_pCommonModel->lDeviceStatus & DEVICE_BANKNOTE_BUSY)!=DEVICE_BANKNOTE_BUSY) && (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)){
							if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
								m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
								BANKNOTE_HELPER->ReturnAllAsync(this);
							}
						}
				}
				// 5.交易进行投币完成取消
				else if (!m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen && 
					(m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED || 
					m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED)&& 
					m_pCommonModel->operationInfo.isFinishedReceveMoney){
						LOG("交易进行投币完成取消(取消)");
						// 退钱
						StartReturnCash();
				}
				// 6.交易完成取消返回(不在这里做了)
				else if ((m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_COLLECTED || 
					m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_COLLECTED)&& 
					m_pCommonModel->operationInfo.coinChangeState != COIN_CHANGE_STATE_NONE)
				{
					LOG("交易完成取消返回(取消)");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
					pMainSvc->StartDefaultService();
				}
			}
			LOG("------------------------------------返回完成界面------------- <OUT1>");
		}
		// 正常业务逻辑:返回
		else {
			//if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_ACC_CARD || m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_YKT_CARD){
			//由于这两种支付方式取决于TPU返回的卡发行商字段，不可靠，因此不用上面这个作为流程判断依据。
			if ( m_ePayType==PAY_WITH_SVT){
				if (m_pCommonModel->operationInfo.finishType == TRANSACTION_STATE_FAILED){
					LOG("储值票购票交易失败！Leave");
					OnErrorFinish();
				}
				else if(m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME){
					LOG("储值票购票交易超时取消！Leave");
					FlowToErrorDlg();
				}
				else if (m_pCommonModel->operationInfo.finishType ==DEFAULT){
					LOG("储值票购票交易正常完成！Leave");
					OnSuccessFinish();
				}
				LOG("------------------------------------返回完成界面------------- <OUT>储值票");
				return;
			}
			if ( m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_WEIXIN|| m_pCommonModel->paymentInfo.paymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
				if(m_pCommonModel->operationInfo.finishType == INSERT_OUT_OF_TIME){
					LOG("二维码购票交易超时取消！Leave");
					FlowToErrorDlg();
				}
				else if(m_pCommonModel->operationInfo.finishType == TRANSACTION_STATE_FAILED){
					LOG("二维码购票交易失败！Leave");
					FlowToErrorDlg();	
				}
				else if (m_pCommonModel->operationInfo.finishType ==DEFAULT){
					LOG("二维码购票交易成功！Leave");
					OnSuccessFinish();	
				}
				LOG("------------------------------------返回完成界面------------- <OUT>二维码");
				return;
			}

			// 交易未完成,则返回
			if(m_pCommonModel->lDeviceStatus != DEVICE_ALL_OK){
				LOG("交易未完成，返回继续等待。%d",m_pCommonModel->lDeviceStatus);
				return;
			}

			//if (!theEXCEPTION_MGR.HasException(CBRHException::MODULE_ID)&&!theEXCEPTION_MGR.HasException(CBRHHardwareException::MODULE_ID)){
			//	if(m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
			//		m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
			//		BANKNOTE_HELPER->Collect();
			//	}
			//}

			// 有异常，异常返回,或者超时。
			if(m_pCommonModel->operationInfo.mapException.size() > 0 /*|| m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME*/){
				OnErrorFinish();
				if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
					m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
					BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
				}
				LOG("------------------------------------返回完成界面------------- <OUT>");
				return;
			}
			// 找零是否完成呢？
			//else if(m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_SUCCESS && m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_SUCCESS){
			//	if(m_pCommonModel->operationInfo.finishType == TRANSACTION_STATE_FAILED || m_pCommonModel->operationInfo.finishType == RUNNING_OUT_OF_TIME){
			//		OnErrorFinish();
			//		return;
			//	}
			//}
			//// 找零失败！！！
			//else if(m_pCommonModel->operationInfo.coinChangeState == COIN_CHANGE_STATE_FAIL || m_pCommonModel->operationInfo.banknoteChangeState == BANKNOTE_CHANGE_STATE_FAIL){
			//	OnErrorFinish();
			//	return;
			//}

			if(BANKNOTE_ACCEPT_STATE_COLLECTED==m_pCommonModel->operationInfo.banknoteAcceptState && GetServiceID()==CHARGE_SVC){
				LOG("交易正常完成");
				OnSuccessFinish();
			}


			// 1.找零完成,包括不找零
			if (COIN_CHANGE_STATE_SUCCESS		== m_pCommonModel->operationInfo.coinChangeState && 
				BANKNOTE_CHANGE_STATE_SUCCESS   == m_pCommonModel->operationInfo.banknoteChangeState &&
				(BANKNOTE_ACCEPT_STATE_NONE		== m_pCommonModel->operationInfo.banknoteAcceptState ||
				BANKNOTE_ACCEPT_STATE_COLLECTED == m_pCommonModel->operationInfo.banknoteAcceptState ||
				BANKNOTE_ACCEPT_STATE_INSERTED  == m_pCommonModel->operationInfo.banknoteAcceptState || 
				BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED == m_pCommonModel->operationInfo.banknoteAcceptState)){
					LOG("交易正常完成");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
					OnSuccessFinish();
			}
			// 2.找零失败,返回错误页面,打印凭证
			else if((COIN_CHANGE_STATE_FAIL			== m_pCommonModel->operationInfo.coinChangeState ||
				BANKNOTE_CHANGE_STATE_FAIL		== m_pCommonModel->operationInfo.banknoteChangeState)&& 
				(BANKNOTE_ACCEPT_STATE_NONE		== m_pCommonModel->operationInfo.banknoteAcceptState  ||
				BANKNOTE_ACCEPT_STATE_COLLECTED  == m_pCommonModel->operationInfo.banknoteAcceptState  ||
				BANKNOTE_ACCEPT_STATE_INSERTED   == m_pCommonModel->operationInfo.banknoteAcceptState || 
				BANKNOTE_ACCEPT_STATE_COLLECTED_FAILED == m_pCommonModel->operationInfo.banknoteAcceptState)){
					LOG("找零失败,返回错误页面,打印凭证");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
					OnErrorFinish();
			}
			// 3.退钱成功:只有硬币,或者只有纸币接收,或者二者都退币成功
			if ((COIN_ACCEPT_STATE_RETURNED	== m_pCommonModel->operationInfo.coinAcceptState &&	
				BANKNOTE_ACCEPT_STATE_NONE      == m_pCommonModel->operationInfo.banknoteAcceptState && 
				!m_pCommonModel->operationInfo.isBankNoteSlotOpen) ||
				(COIN_ACCEPT_STATE_NONE	    == m_pCommonModel->operationInfo.coinAcceptState &&	
				BANKNOTE_ACCEPT_STATE_RETURNED  == m_pCommonModel->operationInfo.banknoteAcceptState && 
				!m_pCommonModel->operationInfo.isCoinSlotOpen)     ||
				(COIN_ACCEPT_STATE_RETURNED == m_pCommonModel->operationInfo.coinAcceptState &&  
				BANKNOTE_ACCEPT_STATE_RETURNED == m_pCommonModel->operationInfo.banknoteAcceptState && 
				!m_pCommonModel->operationInfo.isBankNoteSlotOpen && !m_pCommonModel->operationInfo.isCoinSlotOpen)){
					LOG("退钱成功:只有硬币,或者只有纸币接收,或者二者都退币成功");
					if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR && m_pCommonModel->paymentInfo.lPaidAmount == 0)
						m_pCommonModel->operationInfo.finishType = PASSAGE_CANCEL;
					else if(m_pCommonModel->operationInfo.finishType == EXCEPTION_ERROR && m_pCommonModel->paymentInfo.lBHReceiveTotalCount >= 15){
						m_pCommonModel->operationInfo.finishType = OVER_MONEY_AMOUNT_LIMIT;
					}
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
					FlowToErrorDlg();
			}
			// 4.退钱失败：纸币或者硬币失败,打印凭证
			if (COIN_ACCEPT_STATE_RETURN_FAILD == m_pCommonModel->operationInfo.coinAcceptState && BANKNOTE_ACCEPT_STATE_INSERTED != m_pCommonModel->operationInfo.banknoteAcceptState ||
				BANKNOTE_ACCEPT_STATE_RETURN_FAILED == m_pCommonModel->operationInfo.banknoteAcceptState && COIN_ACCEPT_STATE_INSERTED != m_pCommonModel->operationInfo.coinAcceptState){
					LOG("退钱失败：纸币或者硬币失败,打印凭证");
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
					OnErrorFinish();
			}
			// 5.如果都没有接收钱,且接收口都已经关闭,直接返回
			if (COIN_ACCEPT_STATE_NONE == m_pCommonModel->operationInfo.coinAcceptState && BANKNOTE_ACCEPT_STATE_NONE == m_pCommonModel->operationInfo.banknoteAcceptState &&
				!m_pCommonModel->operationInfo.isCoinSlotOpen && !m_pCommonModel->operationInfo.isBankNoteSlotOpen){
					LOG("没有接收钱,且接收口都已经关闭,直接返回");//应该直接启用默认业务！！！
					//FlowToErrorDlg();
					if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
						m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;
						BANKNOTE_HELPER->Collect();//武汉MEI纸币cashInStart状态无法用cancel来取消，必须使用cashInEnd。
					}
					theSERVICE_MGR.SetState(FINISH);
					//pMainSvc->StartDefaultService();
			}
			LOG("------------------------------------返回完成界面------------- <OUT2>");
		}
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		LOG("其他类异常。");
	}
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      启动增加计数中到返回中的计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::InsertMoneyNeedStartTimer(){
	//LOG("启动计数中计时器。");
	//// 时间
	//int nTimerTime = theAFC_TVM_OPERATION.GetCancelDealTime();
	//// Default time
	//if(0 == nTimerTime){
	//	nTimerTime = 60;// 表示不启用
	//}
	//else{
	//	StartTimer(TIMER_COUNTING_TO_RETURN,nTimerTime);	
	//}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止计数中到返回中的计时器,接收完成

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::EndInsertTimer(){
	//LOG("停止计数中计时器");
	//// 停止计数中计时器
	//StopTimer(TIMER_COUNTING_TO_RETURN);
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      启动增加计数中到返回中的计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::InsertRecvStartTimer(){
	LOG("启动计数中计时器。");
	// 时间
	int nTimerTime = 60/*theAFC_TVM_OPERATION.GetCancelDealTime()*/;
	// Default time
	if(0 == nTimerTime)
		nTimerTime = 60;// 表示不启用
	else{
		m_timerInterval = nTimerTime;
		UpdateReceptDlg(_T(""));
		StartTimer(TIMERS_COUNTING_BEFORE_ISSUE,1);	
	}
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      停止计数中到返回中的计时器,接收完成

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::EndRecvTimer(){
	LOG("停止计数中计时器");
	// 停止计数中计时器
	StopTimer(TIMERS_COUNTING_BEFORE_ISSUE);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      停止计数中到返回中的计时器,接收完成

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::ResetMoneyNeedTimer()
{
	EndInsertTimer();
	InsertMoneyNeedStartTimer();
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      判断是否都退币成功

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::OnTimer(UINT nEventID){
	try{
		// 1.计数中到返回中的计时处理 
		if (nEventID == TIMER_COUNTING_TO_RETURN){
			LOG("等待接受现金计时器超时,退钱。");
			// 设置状态为取消中
			theSERVICE_MGR.SetState(CANCELING);
			// 先隐藏相关按钮
			CReceptionDlg* pPassDlg = GetDialogFlow()->GetCurReceptionDlg();
			if(NULL != pPassDlg){
				pPassDlg->HideCompoment(CReceptionDlg::BTN_OK);
			}
			// 设置当前状态为取消状态。
			//theSERVICE_MGR.SetState(CANCELING);
			/*if(theAPP_SESSION.IsBlindManOperation()){
			StopTimer(TIMERS_OUT_OF_REPLAY);
			StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
			}*/
			StopTimer(TIMER_COUNTING_TO_RETURN);							// 结束计时
			m_pCommonModel->operationInfo.finishType = INSERT_OUT_OF_TIME;
			//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);		// 提示：交易取消,等待投币超时
			//CloseAllDevice();												// 此时都在等待状态,停止识别接收,准备退币。
			//  1.退出硬币,如果接收了。
			if (m_pCommonModel->operationInfo.isCoinSlotOpen){
				m_pCommonModel->operationInfo.isCoinSlotOpen = false;
				COIN_HELPER->CH_StopInsertInfo();
				COIN_HELPER->CH_EndReceive();
			}
			if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
				m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
				COIN_HELPER->CH_RefundAsync(this);
			}
			// 2.退出纸币,如果接收了
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
			// 3.返回
			OnReturnFinish();// 分两种情况：没有投入？？直接返回；又投入,退币处返回 !!一定有投入才会执行此计时器。
		}

		else if (nEventID == TIMERS_COUNTING_BEFORE_ISSUE){
			if (--m_timerInterval<0){
				LOG("售票操作计时器超时,退钱。");
				EndRecvTimer();
				if (m_ePayType == PAY_WITH_SVT){
					// 停止储值卡购票的循环读卡
					m_bStopCircleRead = true;
				}
				else if (m_ePayType == PAY_WITH_WEIXIN || m_ePayType == PAY_WITH_ZHIFUBAO){
					// 网络购票,先停止订单查询，再发送订单取消
					if (m_pCommonModel->operationInfo.finishType != CPassengerForeSvc::QR_ORDER_GENERATE_FAILED){
						EndQueryOrderTimer();	
						m_bWebPaySuccess = false;//防止取消前，订单已经支付成功
						CancelOrder();
					}
				} 

				// 设置状态为取消中
				theSERVICE_MGR.SetState(CANCELING);
				// 先隐藏相关按钮
				CReceptionDlg* pPassDlg = GetDialogFlow()->GetCurReceptionDlg();
				if(NULL != pPassDlg){
					pPassDlg->HideCompoment(CReceptionDlg::BTN_OK);
					pPassDlg->HideCompoment(CReceptionDlg::BTN_OK_AND_CANCEL1);
					pPassDlg->HideCompoment(CReceptionDlg::BTN_OK_AND_CANCEL2);
				}
				// 设置当前状态为取消状态。
				//theSERVICE_MGR.SetState(CANCELING);
				/*if(theAPP_SESSION.IsBlindManOperation()){
				StopTimer(TIMERS_OUT_OF_REPLAY);
				StopTimer(TIMERS_OPERATION_OUT_OF_TIME);
				}*/
				StopTimer(TIMER_COUNTING_TO_RETURN);							// 结束计时
				m_pCommonModel->operationInfo.finishType = INSERT_OUT_OF_TIME;
				//theAudio_HELPER.PlayAudio(AUDIO_ID_TRANSACTION_CANCELED);		// 提示：交易取消,等待投币超时
				//CloseAllDevice();												// 此时都在等待状态,停止识别接收,准备退币。
				//  1.退出硬币,如果接收了。
				if (m_pCommonModel->operationInfo.isCoinSlotOpen){
					m_pCommonModel->operationInfo.isCoinSlotOpen = false;
					COIN_HELPER->CH_StopInsertInfo();
					COIN_HELPER->CH_EndReceive();
				}
				if (((m_pCommonModel->lDeviceStatus & DEVICE_COIN_BUSY)!=DEVICE_COIN_BUSY) && (m_pCommonModel->operationInfo.coinAcceptState == COIN_ACCEPT_STATE_INSERTED)) {
					m_pCommonModel->lDeviceStatus |= DEVICE_COIN_BUSY;
					COIN_HELPER->CH_RefundAsync(this);
				}
				// 2.退出纸币,如果接收了
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
				// 3.返回
				OnReturnFinish();// 分两种情况：没有投入？？直接返回；又投入,退币处返回 !!一定有投入才会执行此计时器。
			}
			else{
				// 更新界面倒计时显示
				CString strShowTxt;
				strShowTxt.Format(_tl(TXT_COUNT_DOWN) + _T("%d"), m_timerInterval);
				UpdateReceptDlg(strShowTxt);				
			}
		}
		else if (nEventID == TIMERS_WEB_QUERY_ORDER){
			LOG("查询订单计时器。");
			EndQueryOrderTimer();
			if(m_bWebPaySuccess){
				m_bWebPaySuccess = false;
				// 订单支付成功，发售车票
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
@brief      硬币找零

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartCHChange(){

	try{
		LOG("开始硬币找零。");

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
@brief      纸币找零

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::StartBHChange(){

	try{
		LOG("开始纸币找零。");
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
@brief      乘客插卡回调函数

@param      UINT uiResult				(返回结果)

@param      CIOCardPosMonitor* pCommand (返回命令)

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
//void CPassengerForeSvc::OnChargeInsertCard(UINT,CIOCardPosMonitor*){
//
//}



//////////////////////////////////////////////////////////////////////////
/*
@brief      取配置文件中的价格表

@param      无

@retval     vector<int> m_vecTicketAmount;票价集合

@exception  无
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
@brief      取配置文件中的价格表

@param      无

@retval     vector<int> m_vecTicketAmount;票价集合

@exception  无
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
@brief      动态监测IO状态线程

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
			//pSvc->EmptyCardInfo();20180328，改为读卡失败才清空上次读卡信息，读卡成功直接更新
			long resultCode = -1;
			LOG("start read:");
			pSvc->m_bCircleReading = true;
			resultCode = RECHARGERW_HELPER->TryReadCard(pSvc->GetServiceID()==ISSUE_SVC?SVT_ISSUE_SVC:CHARGE_SVC,cardRspInfo,false);
			pSvc->m_bCircleReading = false;
			LOG("end read:");
			// 读卡失败
			if(resultCode != RW_RSP_OK) {
				pSvc->EmptyCardInfo();

				LOG("读卡失败。");
				// 取消后退出读卡
				if(cardRspInfo.DetailErrorCode == RW_ERROR_CARD_NO_CARD){
					LOG("读卡反馈无卡。");			
				}
				// 黑名单车票
				else if (resultCode == RW_RSP_ACC_CARD_BLACKLIST || resultCode == RW_RSP_BLACK_CARD) {
					LOG("读卡黑名单。");
					//pSvc->BlockCard();
					pSvc->m_strReadCardRsp = _tl(GUIDE_IN_BLACK_CARD_ERROR);
					//CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_tl(TXT_MSG_GUIDE_THIS_CARD_HAS_BEEN_LOCKED),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);
				}
				// 寻卡或读卡失败
				else if((resultCode ==  RW_RSP_READERROR) || (resultCode == RW_RSP_FINDERROR) || (resultCode == RW_RSP_FIND_CARD_FAILED) || (resultCode == RW_RSP_READ_CARD_FAILED)){		

				}
				// 票卡不满足充值条件
				else{
					CString strTemp = theTPUERROR_INFO.GetTpuErrorInfo(resultCode);
					LOG("票卡不满足条件。");
					pSvc->m_strReadCardRsp = _tl(GUIDE_CARD_CAN_NOT_DO_BUSINESS);
					/*		CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),strTemp,TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime(),CMessagePictureBox::BTN_OK);*/
				}
			}
			// 读卡成功
			else{
				// 判断物理卡类型(物理类型不正确)
				if(cardRspInfo.CardPHType != MIFARE1K && cardRspInfo.CardPHType != MIFARE4K && cardRspInfo.CardPHType != CPU_TICKET){
					LOG("票卡类型不正确。");
					pSvc->EmptyCardInfo();
					/*		CMessagePictureBox::Show(theAPP_SESSION.GetServiceName((SERVICE_ID)pSvc->GetServiceID()),_tl(GUIDE_IN_CARD_TYPE_ERROR),TVM_TIP_WARINGS,CMessagePictureBox::BTN_OK,DT_LEFT,theAFC_TVM_OPERATION.GetCancelDealTime());*/
					pSvc->m_strReadCardRsp = _tl(GUIDE_CARD_CAN_NOT_DO_BUSINESS);
				}
				else{
					// 保存票卡信息成功
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
@brief      开始循环读卡

@param      无

@retval     long

@exception  无
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
@brief      开始循环读卡

@param      无

@retval     long

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::StopCircleReadCard()
{
	m_bStopCircleRead = true;
	return 0;
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      停止接收现金

@param      无

@retval     long

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
long CPassengerForeSvc::StopReceiveMoney()
{

	m_bRechargeConfirm = true;

	// 停止识别纸币
	if (m_pCommonModel->operationInfo.isBankNoteSlotOpen){
		m_pCommonModel->operationInfo.isBankNoteSlotOpen = false;
		BANKNOTE_HELPER->StopValidation();
		if (m_bRechargeOverlimit){
			// 压钞
			if (m_pCommonModel->operationInfo.banknoteAcceptState == BANKNOTE_ACCEPT_STATE_INSERTED && m_pCommonModel->operationInfo.banknoteAcceptState != BANKNOTE_ACCEPT_STATE_COLLECTED){
				LOG("开始押钞纸币,已接受纸币。");
				m_pCommonModel->operationInfo.banknoteAcceptState = BANKNOTE_ACCEPT_STATE_COLLECTED;// 为了不与其他地方同步压钞冲突，这里异步压钞前，先把标识改为已压钞票。
				BANKNOTE_HELPER->CollectAsync(this);
				m_pCommonModel->lDeviceStatus |= DEVICE_BANKNOTE_BUSY;
			}
			if(GetServiceID()==CHARGE_SVC){
				GetCurReceptionDlg()->PostMessage(WM_NOTIFY_ISSUING, 0, 0);
			}
		}
	}
	// 停止识别硬币
	if (m_pCommonModel->operationInfo.isCoinSlotOpen){
		m_pCommonModel->operationInfo.isCoinSlotOpen = false;
		COIN_HELPER->CH_StopInsertInfo();
		COIN_HELPER->CH_EndReceive();
	}

	return 0;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      启动订单查询计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::InsertQueryOrderTimer()
{
	LOG("启动订单查询计时器");
	int nTimer = 3;// 将来需要从配置文件获取 (3S 查询一次)
	StartTimer(TIMERS_WEB_QUERY_ORDER,nTimer);

	//LOG("启动计数中计时器。");
	//// 时间
	//int nTimerTime = theAFC_TVM_OPERATION.GetCancelDealTime();
	//// Default time
	//if(0 == nTimerTime){
	//	nTimerTime = 60;// 表示不启用
	//}
	//else{
	//	StartTimer(TIMER_COUNTING_TO_RETURN,nTimerTime);	
	//}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      停止订单查询计时器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CPassengerForeSvc::EndQueryOrderTimer()
{
	LOG("停止订单查询计时器");
	StopTimer(TIMERS_WEB_QUERY_ORDER);
}


//////////////////////////////////////////////////////////////////////////
/*
@brief      创建线程来和web服务器来通信

@param      无

@retval     long

@exception  无
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
@brief      动态监测IO状态线程

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
			// 判断是否退出线程
			if(pSvc->m_bExitWebThread){
				break;
			}

			if(pSvc->m_nCurWebOper==WEB_ORDER_REQUEST){
				pSvc->m_bWebPaySuccess = false;
				RETURN_CODE retCode = RSP_CODE_OK;
				LOG("登录开始时间:%s",ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
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
				LOG("登录结束时间:%s",ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
				LOG("发送二维码订单请求结果给界面");
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
					LOG("查询到订单支付成功！");
					pSvc->m_bWebPaySuccess = true;
				}
			}
			else if(pSvc->m_nCurWebOper==WEB_ORDER_CANCEL){
				pSvc->m_bWebPaySuccess = false;//防止取消前，订单已经支付成功
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