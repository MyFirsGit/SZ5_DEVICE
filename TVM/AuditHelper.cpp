#include "stdafx.h"
#include "AuditHelper.h"
#include "Templatehelper.h"
//#include "CAccBusinessParam.h"
#include "SCAuditManager.h"
/**
@brief RW Helper
*/

#define INVALID_ID	_T("FFFFFFFF")

//////////////////////////////////////////////////////////////////////////
/**
@brief      构造函数

@param      (i)CService& service

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAuditHelper::CAuditHelper(CService &service):CServiceHelper(service)
{
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();					// 获取设备类型
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      析构函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CAuditHelper::~CAuditHelper()
{
}


CAuditHelper::BALANCE_DATA CAuditHelper::LoadBalanceData()
{
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("开始加载日结内容"));
	m_curPurchase.settlementData = _US_SETTLEMENT_DATA();
	m_BalanceData = _balance_data();
	//获取业务结算数据
	PurchaseData curDayPurchase = theSCAudit_MGR.GetCurrentDayPurchase();
	CString beginTime = /*_T("20160508040000")*/ curDayPurchase.auditTimeFrom.ToString(_T("%04d%02d%02d%02d%02d%02d"));
	CString	endTime = /*_T("20160508230000");*/  ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d"));
	m_BalanceData.dtOriginTime = curDayPurchase.auditTimeFrom/*.ToString(_T("%04d-%02d-%02d %02d%:02d%:02d"))*/;
	m_BalanceData.dtCurrentTime = curDayPurchase.auditTimeTo = ComGetCurTime();
	vector<TransactionRecord> vec_transactionInfo;
	DATA_HELPER->DbSelectTransactionInfo(beginTime, endTime, vec_transactionInfo,false);	//销售统计主表数据库接口
	vector<TransactionRecord> ::iterator iter_MainTrans = vec_transactionInfo.begin();
	PRODUCT_TRANS_DATA  product;
	m_BalanceData.mapBCIssue.clear();
	while ( iter_MainTrans != vec_transactionInfo.end() )
	{
		if(iter_MainTrans->iTransactionDetail.iState != 2){ // 不为取消交易时才统计
			// 售票统计
			if(iter_MainTrans->iTransactionDetail.iUdType == CARD_ISSUE){
				if(iter_MainTrans->iTransactionDetail.iState == 1 || iter_MainTrans->iTransactionDetail.iState == 3	){	// 交易故障结束||交易取消故障
					m_curPurchase.settlementData.lSelledCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
					m_curPurchase.settlementData.lSelledCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount; // 销售金额合计
					// 储值卡购票扣款统计
					if (iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_ACC_CARD || iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_YKT_CARD){
						m_curPurchase.settlementData.lSVTPayCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
						m_curPurchase.settlementData.lSVTPayCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					}
					// 二维码购票统计
					if (iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
						m_curPurchase.settlementData.lSVTPayCardNum += iter_MainTrans->iTransactionDetail.iQuantity;
						m_curPurchase.settlementData.lSVTPayCardSum += iter_MainTrans->iTransactionDetail.iTotalAmount;
					}
				}
				else{
					m_curPurchase.settlementData.lSelledCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
					m_curPurchase.settlementData.lSelledCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount; // 销售金额合计
					if (iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_ACC_CARD || iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_YKT_CARD){
						m_curPurchase.settlementData.lSVTPayCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
						m_curPurchase.settlementData.lSVTPayCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					}
					if (iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
						m_curPurchase.settlementData.lSVTPayCardNum += iter_MainTrans->iTransactionDetail.iQuantity;
						m_curPurchase.settlementData.lSVTPayCardSum += iter_MainTrans->iTransactionDetail.iTotalAmount;
					}
				}
			}

			// 充值统计
			if (iter_MainTrans->iTransactionDetail.iUdType == RECHARGE){
				if(iter_MainTrans->iTransactionDetail.iState == 1){
					m_curPurchase.settlementData.lCardRechargeNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
					m_curPurchase.settlementData.lCardRechargeSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;; // 销售金额合计
				}
				else{
					m_curPurchase.settlementData.lCardRechargeNum += iter_MainTrans->iTransactionDetail.iQuantity;
					m_curPurchase.settlementData.lCardRechargeSum += iter_MainTrans->iTransactionDetail.iTotalAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iTotalAmount; // 销售金额合计
				}
			}

		}
		iter_MainTrans ++;
	}


	// 硬币接收(硬币收纳表)
	/*
	凭证格式：	面值			数量		金额
	…
	SUM				数量合计	金额合计
	*/
	vector<CoinAcceptRecord> vec_CoinAcceptRecord;
	DATA_HELPER->DbSelectCoinAcceptRecord( beginTime,endTime, vec_CoinAcceptRecord);		// 从数据库中取硬币接收数据
	
	auto iterInsertRecord = vec_CoinAcceptRecord.begin();

	DWORD dwOneYuanCount = 0,dwHalfYuanCount = 0;
	while(iterInsertRecord != vec_CoinAcceptRecord.end()){

		if(iterInsertRecord->iCoinTypeCode == CoinHalfYuan){
			dwHalfYuanCount += iterInsertRecord->iQuantity;
		}
		else if(iterInsertRecord->iCoinTypeCode == Coin1Yuan){
			dwOneYuanCount += iterInsertRecord->iQuantity;
		}

		iterInsertRecord++;
	}
	m_BalanceData.vecCoinReceived.clear();
	CASH_DATA cash;

	if(dwOneYuanCount > 0 || dwHalfYuanCount > 0){
		cash.value = Coin1Yuan; 
		cash.nCount = dwOneYuanCount + dwHalfYuanCount;
		cash.lAmount = FormatCashToINT(cash.value)*cash.nCount;
		m_curPurchase.settlementData.lCoinIncomeSum += cash.lAmount;
		m_BalanceData.vecCoinReceived.push_back(cash);
	}
	
	//if(dwHalfYuanCount > 0){
	//	cash.value = CoinHalfYuan; 
	//	cash.nCount = dwHalfYuanCount;
	//	cash.lAmount = FormatCashToINT(cash.value)*cash.nCount;
	//	m_curPurchase.settlementData.lCoinIncomeSum += cash.lAmount;
	//	m_BalanceData.vecCoinReceived.push_back(cash);
	//}
	

	// 纸币接收
	/*
	凭证格式：	面值			数量		金额
	…
	SUM				数量合计	金额合计
	*/
	vector<BanknoteAcceptRecord> vec_BanknoteAcceptRecord;
	DATA_HELPER->DbSelectBanknoteAcceptRecord(beginTime, endTime,  vec_BanknoteAcceptRecord);	//纸币接收的数据库接口
	vector<BanknoteAcceptRecord>::iterator iter_Note =  vec_BanknoteAcceptRecord.begin();
	map<int, int> mapNote;// 业结数据中的，纸币接收结构：map<面值，数量>， 金额（面值*数量）
	mapNote[Banknote1Yuan] = mapNote[Banknote5Yuan] = mapNote[Banknote10Yuan] = mapNote[Banknote20Yuan] = mapNote[Banknote50Yuan] = mapNote[Banknote100Yuan] = 0;
	while(  iter_Note !=  vec_BanknoteAcceptRecord.end())
	{
		mapNote[iter_Note->iCoinTypeCode] += iter_Note->iQuantity;	// 面值 -> 数量
		iter_Note++;
	}
	CASH_DATA Note;
	for (map<int, int>::iterator itMap = mapNote.begin(); itMap != mapNote.end(); itMap++)
	{
		Note.value = (BankNoteAndCoinType_t)itMap->first;
		Note.nCount = itMap->second;
		Note.lAmount = FormatCashToINT((BankNoteAndCoinType_t)itMap->first) * itMap->second; //金额（面值*数量）
		// 审计数据，纸币接收金额
		m_curPurchase.settlementData.lNoteIncomeSum += Note.lAmount;
		if(Note.nCount > 0)m_BalanceData.vecNotesReceived.push_back(Note);
	}

	// 硬币找零
	// 硬币找零统计的数据处理方式与“纸币接收”相同
	vector<CoinChangeRecord> vec_CoinChangeRecord;
	DATA_HELPER->DbSelectCoinChangeRecord(beginTime, endTime, vec_CoinChangeRecord);
	vector<CoinChangeRecord>::iterator iter_Change = vec_CoinChangeRecord.begin();
	map<int, int> mapChange;
	mapChange[Coin1Yuan] = mapChange[CoinHalfYuan] = 0;
	while(iter_Change != vec_CoinChangeRecord.end())
	{
		if(iter_Change->iCoinTypeCode == CoinHalfYuan || iter_Change->iCoinTypeCode == Coin1Yuan){
			mapChange[Coin1Yuan/*iter_Change->iCoinTypeCode*/] += iter_Change->iQuantity;
		}
		iter_Change++;
	}
	CASH_DATA Change;
	for(map<int, int>::iterator itChangeMap = mapChange.begin(); itChangeMap != mapChange.end(); itChangeMap++)
	{
		Change.value =  (BankNoteAndCoinType_t) itChangeMap->first;
		Change.nCount = itChangeMap->second;
		Change.lAmount =FormatCashToINT( (BankNoteAndCoinType_t)itChangeMap->first) * itChangeMap->second;
		// 审计用，找零总金额
		m_curPurchase.settlementData.lCoinChangeSum += Change.lAmount;
		if(Change.nCount > 0)m_BalanceData.vecCoinReleased.push_back(Change);
	}

	// 纸币找零
	vector<BanknoteChangeRecord> vTransBanknoteChangeDetail;
	DATA_HELPER->DbSelectBanknoteChangeRecord(beginTime, endTime,  vTransBanknoteChangeDetail);
	vector<BanknoteChangeRecord>::iterator itBnChange = vTransBanknoteChangeDetail.begin();
	CASH_BOX_DATA cash_box_info;
	for (; itBnChange != vTransBanknoteChangeDetail.end(); itBnChange++)
	{
		cash_box_info.type = COMMON_BOX;
		cash_box_info.nBoxID = itBnChange->txtCoinBoxID;
		cash_box_info.value	 = (BankNoteAndCoinType_t)itBnChange->iCoinTypeCode;
		cash_box_info.amount = itBnChange->iQuantity;
		m_BalanceData.nBnChangeCount += itBnChange->iQuantity;
		m_curPurchase.settlementData.lNoteChangeSum += cash_box_info.amount * FormatCashToINT(cash_box_info.value) ;
	}
	cash_box_info.amount = m_BalanceData.nBnChangeCount;if(cash_box_info.amount > 0)m_BalanceData.vecBanknoteChangeInfo.push_back(cash_box_info);

	m_BalanceData.nCashChangeSum = m_curPurchase.settlementData.lNoteChangeSum + m_curPurchase.settlementData.lCoinChangeSum;
	// 硬币补充
	/* 凭证格式:		
	  1元：			数量		金额
	0.5元:			数量		金额
	
	区分找零箱与回收箱的规则:文档《构造式样书(Lib 数据库).docx》2.2.1硬币找零箱补充操作记录表（tb_CoinAddOperationRecord）
	*/
	m_BalanceData.mapCoinAdded.clear();
	vector<CoinAddOperationRecord> vec_CoinAddOperationRecord;
	DATA_HELPER-> DbSelectCoinAddOperationRecord( beginTime, endTime, vec_CoinAddOperationRecord);
	vector<CoinAddOperationRecord>::iterator it_CoinAdded =  vec_CoinAddOperationRecord.begin();
	CASH_BOX_DATA coin_add;

	while(it_CoinAdded !=  vec_CoinAddOperationRecord.end()){
		coin_add.value = (BankNoteAndCoinType_t)it_CoinAdded->iCoinTypeCode;	// 面值
		coin_add.amount = it_CoinAdded->iQuantityAdd;							// 数量
		
		// 认为一元为A，五角为B
		if(coin_add.value == Coin1Yuan){
			m_BalanceData.mapCoinAdded[Coin1Yuan] += coin_add.amount;
		}
		else if(coin_add.value == CoinHalfYuan){
			m_BalanceData.mapCoinAdded[Coin1Yuan/*CoinHalfYuan*/] += coin_add.amount;
		}
		// 审计用，硬币补充金额
		m_curPurchase.settlementData.lCoinSupplementSum += coin_add.amount * FormatCashToINT(coin_add.value);
		it_CoinAdded ++;
	}

	map<int ,int> mapCoinAllRecycle;
	mapCoinAllRecycle[CoinHalfYuan] = mapCoinAllRecycle[Coin1Yuan] = 0;
	// 硬币找零箱安装(卸载意味着回收)
	vector<CoinChangeBoxOperationRecord> vec_CoinChangeboxOperationRecord;
	DATA_HELPER->DbSelectCoinChangeBoxOperationRecord(beginTime,endTime,vec_CoinChangeboxOperationRecord);
	auto iterCoinChangeBoxOperationRecord = vec_CoinChangeboxOperationRecord.begin();
	while(iterCoinChangeBoxOperationRecord != vec_CoinChangeboxOperationRecord.end()){
		coin_add = _CASH_BOX_DATA();
		// 安装操作
		if(iterCoinChangeBoxOperationRecord->iOperationType == INSTALL_CHANGE_BOX){
			coin_add.value  = (BankNoteAndCoinType_t)iterCoinChangeBoxOperationRecord->iCoinTypeCode;
			coin_add.amount = iterCoinChangeBoxOperationRecord->iQuantity;
			
			if(iterCoinChangeBoxOperationRecord->iCoinTypeCode == Coin1Yuan){
				m_BalanceData.mapCoinAdded[Coin1Yuan] += coin_add.amount;
			}
			else if(iterCoinChangeBoxOperationRecord->iCoinTypeCode == CoinHalfYuan){
				m_BalanceData.mapCoinAdded[Coin1Yuan/*CoinHalfYuan*/] += coin_add.amount;
			}
			m_curPurchase.settlementData.lCoinSupplementSum += coin_add.amount * FormatCashToINT(coin_add.value);
		}
		else if(iterCoinChangeBoxOperationRecord->iOperationType == UNINSTALL_CHANGE_BOX){
			if(iterCoinChangeBoxOperationRecord->iCoinTypeCode == CoinHalfYuan){
				mapCoinAllRecycle[Coin1Yuan/*CoinHalfYuan*/] += iterCoinChangeBoxOperationRecord->iQuantity;
			}
			else if(iterCoinChangeBoxOperationRecord->iCoinTypeCode == Coin1Yuan){
				mapCoinAllRecycle[Coin1Yuan] += iterCoinChangeBoxOperationRecord->iQuantity;
			}
		}
		iterCoinChangeBoxOperationRecord++;
	}

	// 纸币补充
	m_BalanceData.mapBanknotAdded.clear();
	vector<BanknoteOperationRecord> vec_BanknoteOperationRecord;
	DATA_HELPER->DbSelectBanknoteOperationRecord(beginTime,endTime,vec_BanknoteOperationRecord);
	auto iterBHOperRecord = vec_BanknoteOperationRecord.begin();
	CASH_BOX_DATA banknoteBoxInfo;
	while(iterBHOperRecord != vec_BanknoteOperationRecord.end()){

		banknoteBoxInfo = _CASH_BOX_DATA();
		if(iterBHOperRecord->iOperationType == INSTALL_CHANGE_BOX){
			//banknoteBoxInfo.nBoxID = iterBHOperRecord->txtBanknoteBoxID;
			banknoteBoxInfo.value  = (BankNoteAndCoinType_t)iterBHOperRecord->iBanknoteTypeCode;
			banknoteBoxInfo.amount = iterBHOperRecord->iQuantity;
			//m_BalanceData.vecBanknoteAdded.push_back(banknoteBoxInfo);

			if(banknoteBoxInfo.value == Banknote1Yuan){
				m_BalanceData.mapBanknotAdded[Banknote1Yuan] += banknoteBoxInfo.amount;
			}
			else if(banknoteBoxInfo.value == Banknote5Yuan){
				m_BalanceData.mapBanknotAdded[Banknote5Yuan] += banknoteBoxInfo.amount;
			}
			else if(banknoteBoxInfo.value == Banknote10Yuan){
				m_BalanceData.mapBanknotAdded[Banknote10Yuan] += banknoteBoxInfo.amount;
			}
			else if(banknoteBoxInfo.value == Banknote20Yuan){
				m_BalanceData.mapBanknotAdded[Banknote20Yuan] += banknoteBoxInfo.amount;
			}
			else if(banknoteBoxInfo.value == Banknote50Yuan){
				m_BalanceData.mapBanknotAdded[Banknote50Yuan] += banknoteBoxInfo.amount;
			}
			else if(banknoteBoxInfo.value == Banknote100Yuan){
				m_BalanceData.mapBanknotAdded[Banknote100Yuan] += banknoteBoxInfo.amount;
			}
			m_curPurchase.settlementData.lNoteSupplementSum += banknoteBoxInfo.amount * FormatCashToINT(banknoteBoxInfo.value);
		}
		iterBHOperRecord++;
	}


	// 硬币回收 
	// 处理规则同“硬币补充”
	//vector<CoinCollectOperationRecord> vec_CoinCollectOperationRecord;
	//DATA_HELPER->DbSelectCoinCollectOperationRecord(beginTime, endTime,vec_CoinCollectOperationRecord);
	//vector<CoinCollectOperationRecord>::iterator iter_CoinColl = vec_CoinCollectOperationRecord.begin();
	//CASH_BOX_DATA coin_collect;
	//coin_collect.nBoxID = _T("--------");
	//coin_collect.type = COMMON_BOX;
	//coin_collect.value = VALUE_UNKNOWN;
	//coin_collect.amount = 0;
	//while(iter_CoinColl != vec_CoinCollectOperationRecord.end())
	//{
	//	// 需要判断循环找零箱与找零箱
	//	//if(iter_CoinColl->txtCoinChangeBoxID.Right(2) == _T("03")) //zhengxianle这里需要修改！！！
	//	//{
	//	//	coin_collect.nBoxID = iter_CoinColl->txtCoinChangeBoxID;
	//	//	coin_collect.type = RECYCLE_CHANGE_BOX;
	//	//}
	//	//else
	//	{
	//		coin_collect.nBoxID = iter_CoinColl->txtCoinChangeBoxID;
	//		coin_collect.type = COMMON_BOX;
	//	}
	//	coin_collect.value = (BankNoteAndCoinType_t)iter_CoinColl->iCoinTypeCode;
	//	coin_collect.amount = iter_CoinColl->iQuantityCollect;
	//	// 审计用，硬币回收金额
	//	m_curPurchase.settlementData.lCoinRecoverSum = coin_collect.amount * FormatCashToINT(coin_collect.value);
	//	m_BalanceData.vecCoinCollection.push_back(coin_collect);
	//	iter_CoinColl++;
	//}

	// 硬币回收(时间段统计,包括多次回收,卸载)
	vector<CoinCollectBoxRemoveOperationRecord> vec_CoinCollectBoxRemoveOperationRecord;
	DATA_HELPER->DbSelectCoinCollectBoxRemoveOperationRecord(beginTime,endTime,vec_CoinCollectBoxRemoveOperationRecord);
	vector<CoinCollectBoxRemoveOperationRecord>::iterator it_CoinCycle = vec_CoinCollectBoxRemoveOperationRecord.begin();

	while(it_CoinCycle != vec_CoinCollectBoxRemoveOperationRecord.end()){
		m_BalanceData.nCoinCollection += it_CoinCycle->coinCollectBoxRemoveOperationDetail.iTotalAmount;
		for(vector<CoinBoxCashDetail>::iterator it_coinDetail = it_CoinCycle->v_coinBoxCashDetail.begin();it_coinDetail != it_CoinCycle->v_coinBoxCashDetail.end();++it_coinDetail){
			if(it_coinDetail->iCoinTypeCode == CoinHalfYuan){
				mapCoinAllRecycle[Coin1Yuan/*CoinHalfYuan*/] += it_coinDetail->iQuantity;
			}
			else if(it_coinDetail->iCoinTypeCode == Coin1Yuan){
				mapCoinAllRecycle[Coin1Yuan] += it_coinDetail->iQuantity;
			}
		}
		++it_CoinCycle;
	}
	
	int nTotalAmount = 0;
	m_BalanceData.nCoinCollection += nTotalAmount;
	CASH_DATA CoinColl;
	for ( map<int, int>::iterator iterMapCoinColl =  mapCoinAllRecycle.begin(); iterMapCoinColl != mapCoinAllRecycle.end();iterMapCoinColl++){
		CoinColl.value =(BankNoteAndCoinType_t) iterMapCoinColl->first;
		CoinColl.nCount = iterMapCoinColl->second;
		CoinColl.lAmount = FormatCashToINT(CoinColl.value) * CoinColl.nCount;
		m_curPurchase.settlementData.lCoinRecoverSum += CoinColl.lAmount;// 硬币累计回收金额
		if( CoinColl.lAmount > 0 )m_BalanceData.vecCoinCollectionBox.push_back(CoinColl);// 真正有回收才会打印出来
	}

	// 纸币回收(回收箱及找零箱)
	// 类似“纸币接收”处理方式
	vector<BanknoteBoxRemoveOperationRecord> vec_BanknoteBoxRemoveOperationRecord;
	DATA_HELPER->DbSelectBanknoteBoxRemoveOperationRecord(beginTime, endTime,  vec_BanknoteBoxRemoveOperationRecord);
	vector<BanknoteBoxRemoveOperationRecord>::iterator iter_NoteColl =  vec_BanknoteBoxRemoveOperationRecord.begin();

	map<int, int> mapNoteColl;// first: value , second: count per value
	mapNoteColl[Banknote1Yuan] = mapNoteColl[Banknote5Yuan] = mapNoteColl[Banknote10Yuan] = mapNoteColl[Banknote20Yuan] = mapNoteColl[Banknote50Yuan] = mapNoteColl[Banknote100Yuan] = 0;
	while (iter_NoteColl !=  vec_BanknoteBoxRemoveOperationRecord.end())
	{
		m_BalanceData.nNoteCollection += iter_NoteColl->banknoteBoxRemoveOperationDetail.iTotalAmount;//回收总金额
		for ( vector<BanknoteBoxCashDetail> ::iterator iter_NoteOp =  iter_NoteColl->v_banknoteBoxCashDetail.begin(); 
			iter_NoteOp !=  iter_NoteColl->v_banknoteBoxCashDetail.end(); iter_NoteOp++)
		{
			if(iter_NoteOp->iBanknoteTypeCode == Banknote1Yuan)
			{
				mapNoteColl[Banknote1Yuan] += iter_NoteOp->iQuantity;
			}else if (iter_NoteOp->iBanknoteTypeCode == Banknote5Yuan)
			{
				mapNoteColl[Banknote5Yuan] += iter_NoteOp->iQuantity;
			}else if (iter_NoteOp->iBanknoteTypeCode == Banknote10Yuan)
			{
				mapNoteColl[Banknote10Yuan] += iter_NoteOp->iQuantity;
			}else if (iter_NoteOp->iBanknoteTypeCode == Banknote20Yuan)
			{
				mapNoteColl[Banknote20Yuan] += iter_NoteOp->iQuantity;
			}else if (iter_NoteOp->iBanknoteTypeCode == Banknote50Yuan)
			{
				mapNoteColl[Banknote50Yuan] += iter_NoteOp->iQuantity;
			}else if (iter_NoteOp->iBanknoteTypeCode == Banknote100Yuan)
			{
				mapNoteColl[Banknote100Yuan] += iter_NoteOp->iQuantity;
			}

		} 
		iter_NoteColl++;
	}

	// 纸币找零箱卸载
	vector<BanknoteOperationRecord> vec_bankchangeRecord;
	DATA_HELPER->DbSelectBanknoteOperationRecord(beginTime,endTime,vec_bankchangeRecord);

	auto iterBanknoteOpera = vec_bankchangeRecord.begin();
	while(iterBanknoteOpera != vec_bankchangeRecord.end()){
		
		if(iterBanknoteOpera->iOperationType == UNINSTALL_CHANGE_BOX){
			if(iterBanknoteOpera->iBanknoteTypeCode == Banknote1Yuan){
				mapNoteColl[Banknote1Yuan] += iterBanknoteOpera->iQuantity;
			}
			else if(iterBanknoteOpera->iBanknoteTypeCode == Banknote5Yuan){
				mapNoteColl[Banknote5Yuan] += iterBanknoteOpera->iQuantity;
			}
			else if(iterBanknoteOpera->iBanknoteTypeCode == Banknote10Yuan){
				mapNoteColl[Banknote10Yuan] += iterBanknoteOpera->iQuantity;
			}
			else if(iterBanknoteOpera->iBanknoteTypeCode == Banknote20Yuan){
				mapNoteColl[Banknote20Yuan] += iterBanknoteOpera->iQuantity;
			}
			else if(iterBanknoteOpera->iBanknoteTypeCode == Banknote50Yuan){
				mapNoteColl[Banknote50Yuan] += iterBanknoteOpera->iQuantity;
			}
		}
		
		iterBanknoteOpera++;
	}

	m_BalanceData.vecCashCollection.clear();
	// 统计回收了多少钱（从设备上拿走的钱）
	CASH_DATA NoteColl;
	for ( map<int, int>::iterator iterMapColl =  mapNoteColl.begin(); iterMapColl != mapNoteColl.end();iterMapColl++)
	{
		NoteColl.value =(BankNoteAndCoinType_t) iterMapColl->first;
		NoteColl.nCount = iterMapColl->second;
		NoteColl.lAmount = FormatCashToINT(NoteColl.value) * NoteColl.nCount;
		m_curPurchase.settlementData.lNoteRecoverSum += NoteColl.lAmount;
		if(NoteColl.lAmount > 0)m_BalanceData.vecCashCollection.push_back(NoteColl);
	}

	// 现金留存信息
	m_BalanceData.vec_BanknoteRecyclerReservInfo.clear();
	m_BalanceData.vec_BanknoteChangeReservInfo.clear();
	m_BalanceData.vec_BanknoteCollectionReservInfo.clear();
	m_BalanceData.vec_CoinReservInfo.clear();
	// 纸币循环找零箱现金存留
	int tempCount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().ulCurCount + theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().ulCurCount
		+ theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().ulCurCount + theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().ulCurCount;
	int tempAmount = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().BankNotesCountAvos() + theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo().BankNotesCountAvos()
		+ theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo().BankNotesCountAvos() + theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo().BankNotesCountAvos();
	if(tempCount>0){
		CASH_BOX_DATA cashBoxData;
		cashBoxData.type = RECYCLE_CHANGE_BOX;
		cashBoxData.nBoxID = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo().banknoteBoxID.ToString();
		cashBoxData.amount = tempCount;
		cashBoxData.value = (BankNoteAndCoinType_t)tempAmount;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += tempAmount;
		m_BalanceData.vec_BanknoteRecyclerReservInfo.push_back(cashBoxData);
	}

	// 纸币找零箱现金存留
	CASH_BOX_DATA cashBoxData;
	if(theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().banknoteBoxID.ToString() != INVALID_ID){
		cashBoxData.type = CHANGE_BOX_A;
		cashBoxData.nBoxID = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().banknoteBoxID.ToString();
		cashBoxData.amount = theBANKNOTE_COUNT.GetBanknoteChangeBoxInfo().ulCurCount;
		cashBoxData.value =  TransBanknoteType(theTVM_SETTING.GetBNRChangeBoxAmount());
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0) {
			LOG("cashBoxData.amount = %d", cashBoxData.amount);
			m_BalanceData.vec_BanknoteChangeReservInfo.push_back(cashBoxData);
		}
	}

	/*if(theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().banknoteBoxID.ToString() != INVALID_ID){
	cashBoxData.type = CHANGE_BOX_B;
	cashBoxData.nBoxID = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().banknoteBoxID.ToString();
	cashBoxData.amount = theBANKNOTE_COUNT.GetBanknoteChangeBoxBInfo().ulCurCount;
	cashBoxData.value  = TransBanknoteType(theTVM_INFO.GetBHChangeBoxBType());
	m_curPurchase.settlementData.lNoteCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
	if(cashBoxData.amount > 0) m_BalanceData.vec_BanknoteChangeReservInfo.push_back(cashBoxData);
	}*/


	// 纸币回收箱存留
	CASH_DATA cashData;
	if(theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString() != INVALID_ID){
		
		cashData.value   = Banknote1Yuan;
		cashData.nCount  = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_CNY1;
		cashData.lAmount = cashData.nCount * 100;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashData.lAmount;
		if(cashData.nCount > 0) m_BalanceData.vec_BanknoteCollectionReservInfo.push_back(cashData);

		cashData.value	 = Banknote5Yuan;
		cashData.nCount  = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_CNY5;
		cashData.lAmount = cashData.nCount * 500;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashData.lAmount;
		if(cashData.nCount > 0) m_BalanceData.vec_BanknoteCollectionReservInfo.push_back(cashData);

		cashData.value   = Banknote10Yuan;
		cashData.nCount	 = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_CNY10;
		cashData.lAmount = cashData.nCount * 1000;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashData.lAmount;
		if(cashData.nCount > 0) m_BalanceData.vec_BanknoteCollectionReservInfo.push_back(cashData);

		cashData.value	 = Banknote20Yuan;
		cashData.nCount	 = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_CNY20;
		cashData.lAmount = cashData.nCount * 2000;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashData.lAmount;
		if(cashData.nCount > 0) m_BalanceData.vec_BanknoteCollectionReservInfo.push_back(cashData);

		cashData.value	 = Banknote50Yuan;
		cashData.nCount	 = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_CNY50;
		cashData.lAmount = cashData.nCount * 5000;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashData.lAmount;
		if(cashData.nCount > 0) m_BalanceData.vec_BanknoteCollectionReservInfo.push_back(cashData);

		cashData.value   = Banknote100Yuan;
		cashData.nCount	 = theBANKNOTE_COUNT.GetBanknoteboxInfo().ulCount_CNY100;
		cashData.lAmount = cashData.nCount * 10000;
		m_curPurchase.settlementData.lNoteCurrentLeftSum += cashData.lAmount;
		if(cashData.nCount > 0) m_BalanceData.vec_BanknoteCollectionReservInfo.push_back(cashData);
	}

	// 硬币箱存留
	// 找零箱A
	if(theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() != INVALID_ID){
		cashBoxData.type = CHANGE_BOX_A;
		cashBoxData.nBoxID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
		cashBoxData.amount = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}
	
	// 找零箱B
	if(theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() != INVALID_ID){
		cashBoxData.type = CHANGE_BOX_B;
		cashBoxData.nBoxID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
		cashBoxData.amount = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}
	
	// 循环找零箱A有存留
	if(theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount != 0){
		cashBoxData.type = CYCLE_BOX_A;
		cashBoxData.nBoxID = theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();
		cashBoxData.amount = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHHopper1Type());
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}

	// 循环找零箱B有存留
	if(theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount != 0){
		cashBoxData.type = CYCLE_BOX_B;
		cashBoxData.nBoxID = theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();
		cashBoxData.amount = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHHopper2Type());
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}

	// 回收箱A(待修改：回收箱包括一元与五角)暂时只写一元，后续加上五角的处理
	if(theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() != INVALID_ID){
		cashBoxData.type = COLLECTION_BOX_A;
		cashBoxData.amount = theCOIN_COUNT.GetCollectionCoinboxAInfo().GetCurCount();
		cashBoxData.nBoxID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
		cashBoxData.value  = Coin1Yuan;
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}

	//// 回收箱B(待修改：回收箱包括一元与五角)
	//if(theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() != INVALID_ID){
	//	cashBoxData.type = COLLECTION_BOX_B;
	//	cashBoxData.amount = theCOIN_COUNT.GetCollectionCoinboxBInfo().GetCurCount();
	//	cashBoxData.nBoxID = theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
	//	cashBoxData.value  = Coin1Yuan;
	//	m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
	//	if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	//}

	// 故障交易统计
	/* 
	数据结构：	map<EXCEPTION_TRANS, pair<int, int>> mapException;
	map<故障类型，pait<交易数量，交易金额>>mapException
	*/
	// 初始化操作
	make_pair(0,0) =  m_BalanceData.mapException[CARD_FAILURE] = m_BalanceData.mapException[COIN_CHANGE_FAILURE] ;


	vector<ExceptionRecord> vec_TransFailureRecord;
	if(SQLITE_OK == DATA_HELPER-> DbSelectTransFailureRecord( beginTime, endTime, vec_TransFailureRecord))
	{
		for (vector<ExceptionRecord>::iterator iter_exp =  vec_TransFailureRecord.begin();  iter_exp !=  vec_TransFailureRecord.end(); iter_exp++ )
		{
			if (iter_exp->iExceptionType == CARD_FAILURE ){
				m_BalanceData.mapException[CARD_FAILURE].first ++;
				m_BalanceData.mapException[CARD_FAILURE].second += iter_exp->IExceptionAmount;

				m_BalanceData.nSumExceptionAmount += iter_exp->IExceptionAmount;
				m_BalanceData.nSumExceptionCount ++;
			}
			if (iter_exp->iExceptionType == WRITE_CARD_FAILED ){
				m_BalanceData.mapException[WRITE_CARD_FAILED].first ++;
				m_BalanceData.mapException[WRITE_CARD_FAILED].second += iter_exp->IExceptionAmount;

				m_BalanceData.nSumExceptionAmount += iter_exp->IExceptionAmount;
				m_BalanceData.nSumExceptionCount ++;
			}
			if (iter_exp->iExceptionType == COIN_CHANGE_FAILURE || iter_exp->iExceptionType == NOTE_CHANGE_FAILURE){
				m_BalanceData.mapException[COIN_CHANGE_FAILURE].first ++;
				m_BalanceData.mapException[COIN_CHANGE_FAILURE].second += iter_exp->IExceptionAmount;

				m_BalanceData.nSumExceptionAmount += iter_exp->IExceptionAmount;
				m_BalanceData.nSumExceptionCount ++;
			}
		}
	}

	// 废票统计
	vector<CardCollectBoxRemoveOperationRecord> vec_cardwastedInfo;
	DATA_HELPER->DbSelectCardCollectBoxRemoveOperationRecord(beginTime,endTime,vec_cardwastedInfo);

	auto iterCardWasted = vec_cardwastedInfo.begin();
	while(iterCardWasted != vec_cardwastedInfo.end()){
		m_curPurchase.settlementData.lCardCurrentInvalidateNum += iterCardWasted->iQuantity;
		iterCardWasted++;
	}

	// 票卡补充
	vector<CardBoxInstallOperationRecord> vec_cardInstallInfo;
	DATA_HELPER->DbSelectCardBoxInstallOperationRecord(beginTime,endTime,vec_cardInstallInfo);

	auto iterCardInstall = vec_cardInstallInfo.begin();
	while(iterCardInstall != vec_cardInstallInfo.end()){
		m_curPurchase.settlementData.lCardCurrrntSupplementNum += iterCardInstall->iQuantity;
		iterCardInstall++;
	}

	// 票卡回收
	vector<CardBoxRemoveOperationRecord> vec_cardUninstall;
	DATA_HELPER->DbSelectCardBoxRemoveOperationRecord(beginTime,endTime,vec_cardUninstall);

	auto iterCardUninstall = vec_cardUninstall.begin();

	while(iterCardUninstall != vec_cardUninstall.end()){
		m_curPurchase.settlementData.lCardCurrentRecoverNum += iterCardUninstall->iOriginalQuantity;
		iterCardUninstall++;
	}

	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("加载日结文件完成。"));
	// 发送业结数据到上位
	SendTVMAuditData();

	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
	return  m_BalanceData;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		业结数据模板化 

@param      

@retval     CStringArray&  业结数据

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAuditHelper::PrintBalanceBill(bool bIsPrint /* = true */)
{
	try{
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("开始打印日结文件。"));
		//theSERVICE_MGR.SetForeServiceBusy(true);
		BALANCE_DATA m_BalanceData = LoadBalanceData();	//获取业结数据
		//theSERVICE_MGR.SetForeServiceBusy(false);

		CStringArray saBillTemplate;
		CStringArray saPrintText;
		CStringArray saReception;

		map<ProductType, pair<int, int>>::iterator itIssue = m_BalanceData.mapIssue.begin();			//售票	
		map<ProductType, pair<int, int>>::iterator itRchg = m_BalanceData.mapRecharge.begin();			//充值	
		//map<ProductType, pair<int, int>>::iterator itAddpd = m_BalanceData.mapIssuePdt.begin();			//售产品

		map<ProductType, pair<int, int>>::iterator itCashIssue = m_BalanceData.mapCashIssue.begin();	//现金售票
		map<ProductType, pair<int, int>>::iterator itCashRchrg = m_BalanceData.mapCashRecharge.begin();	//现金充值
		//map<ProductType, pair<int, int>>::iterator itCashAddPd = m_BalanceData.mapCashProdIssue.begin();//现金售产品
		
		vector<CASH_DATA>::iterator itCoinRecv =  m_BalanceData.vecCoinReceived.begin();				//硬币接收
		vector<CASH_DATA>::iterator	itBkntRecv =  m_BalanceData.vecNotesReceived.begin();				//纸币接受
		vector<CASH_DATA>::iterator itCoinChang = m_BalanceData.vecCoinReleased.begin();				//硬币找零
		vector<CASH_BOX_DATA>::iterator itBHChange	= m_BalanceData.vecBanknoteChangeInfo.begin();		//纸币找零

		//vector<CASH_BOX_DATA>::iterator itCoinAdd = m_BalanceData.vecCoinAdded.begin();					//硬币补充
		auto itCoinAdd = m_BalanceData.mapCoinAdded.begin();
		auto itBankAdd = m_BalanceData.mapBanknotAdded.begin();
		//vector<CASH_BOX_DATA>::iterator itBanknoteAdd = m_BalanceData.vecBanknoteAdded.begin();			//纸币补充
		vector<CASH_BOX_DATA> vecCommonBoxAdd;			//普通箱硬币补充

		vector<CASH_BOX_DATA>::iterator itCoinRcyl = m_BalanceData.vecCoinCollection.begin();			//硬币回收
		vector<CASH_BOX_DATA> vecCommonBoxColl;			//普通箱硬币回收
		CASH_BOX_DATA	Recycle_box_Coll;				//循环箱回收

		vector<CASH_DATA>::iterator itBkntRcyl =  m_BalanceData.vecCashCollection.begin();				//纸币回收
		vector<CASH_DATA>::iterator itCoinColl =  m_BalanceData.vecCoinCollectionBox.begin();			//硬币回收
		
		auto iterBHReReserv	= m_BalanceData.vec_BanknoteRecyclerReservInfo.begin();						//纸币循环找零箱留存
		auto iterBHReserv	= m_BalanceData.vec_BanknoteChangeReservInfo.begin();						//纸币找零箱留存
		auto iterBHCollBoxReserv = m_BalanceData.vec_BanknoteCollectionReservInfo.begin();				//纸币回收箱留存
		//map<ProductType, pair<int, int>>::iterator itDCIssue = m_BalanceData.mapDCIssue.begin();		//信用卡售卡
		//map<ProductType, pair<int, int>>::iterator itDCRechrg = m_BalanceData.mapDCRecharge.begin();	//信用卡充值
		//map<ProductType, pair<int, int>>::iterator itDCAddpdt = m_BalanceData.mapDCIssuePdt.begin();	//信用卡售产品
		//map<ProductType, pair<int, int>>::iterator itBCIssue = m_BalanceData.mapBCIssue.begin();		//银行卡售卡	
		//map<ProductType, pair<int, int>>::iterator itBCRchrg = m_BalanceData.mapBCRecharge.begin();		//银行卡充值
		//map<ProductType, pair<int, int>>::iterator itBCAddPdt = m_BalanceData.mapBCIssuePdt.begin();	//银行卡售产品
// 		int nCoinAddSum = 0, nCoinCollection = 0;
// 		while (itCoinAdd != m_BalanceData.vecCoinAdded.end())	// 区分循环找零箱与普通箱的硬币补充操作
// 		{
// 			if(itCoinAdd->type == COMMON_BOX)
// 			{
// 				vecCommonBoxAdd.push_back(*itCoinAdd);
// 			}
// 			
// 			itCoinAdd++;
// 		}
// 		vector<CASH_BOX_DATA>::iterator iter_cmAdd =  vecCommonBoxAdd.begin();
// 		while( itCoinRcyl != m_BalanceData.vecCoinCollection.end())  // 区分循环找零箱与普通箱的硬币回收操作
// 		{
// 			if(itCoinRcyl->type == COMMON_BOX)
// 			{
// 				vecCommonBoxColl.push_back(*itCoinRcyl);
// 			}
// 			else if (itCoinRcyl->type == RECYCLE_CHANGE_BOX)
// 			{
// 				Recycle_box_Coll = *itCoinRcyl;
// 			}
// 			itCoinRcyl++;
// 		}
		vector<CASH_BOX_DATA>::iterator iter_comCol = vecCommonBoxColl.begin();
	
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
		// 加载模板
		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BALANCE_BILL.template"),saBillTemplate);

		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));

		int baseEraseCount = 0;
		// 修改模板
		for(int index = 0;index < saBillTemplate.GetCount();index++){
			CString strLine = saBillTemplate.ElementAt(index);
			// 纸币留存
			if(strLine.Find(_T("<纸币留存>")) >= 0){
				if(m_BalanceData.vec_BanknoteRecyclerReservInfo.size() == 0 && m_BalanceData.vec_BanknoteChangeReservInfo.size() == 0 && m_BalanceData.vec_BanknoteCollectionReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index,16);
					index--;
					continue;
				}
			}
			if(strLine.Find(_T("{纸币循环找零箱}")) >= 0){
				if(m_BalanceData.vec_BanknoteRecyclerReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index, 2);
					index--;
					continue;
				}
			}
			if(strLine.Find(_T("{纸币找零箱}")) >= 0){
				if(m_BalanceData.vec_BanknoteChangeReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index, 4);
					index--;
					continue;
				}
			}
			if(strLine.Find(_T("{回收箱}")) >= 0){
				if(m_BalanceData.vec_BanknoteCollectionReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index, 7);
					index--;
					continue;
				}
			}
			
			if(strLine.Find(_T("<硬币留存>")) >= 0){
				if(m_BalanceData.vec_CoinReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index,9);
					index--;
					continue;
				}
			}
		}



		for(int i=0;i<saBillTemplate.GetCount();i++){
			CString strLine = saBillTemplate.ElementAt(i);
			strLine.Replace(_T("{开始时间}"), m_BalanceData.dtOriginTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")) );
			strLine.Replace(_T("{结束时间}"), m_BalanceData.dtCurrentTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			
			strLine.Replace(_T("{售票总数}"),FromInt(m_curPurchase.settlementData.lSelledCardNum));
			strLine.Replace(_T("{售票总额}"),FromInt(m_curPurchase.settlementData.lSelledCardSum/100));					// 单位：元
			strLine.Replace(_T("{现金购票总数}"),FromInt(m_curPurchase.settlementData.lSelledCardNum - m_curPurchase.settlementData.lSVTPayCardNum - m_curPurchase.settlementData.lNETPayCardNum));
			strLine.Replace(_T("{现金购票总额}"),FromInt(m_curPurchase.settlementData.lSelledCardSum/100 - m_curPurchase.settlementData.lSVTPayCardSum/100 - m_curPurchase.settlementData.lNETPayCardSum/100));// 单位：元
			strLine.Replace(_T("{储值卡购票总数}"),FromInt(m_curPurchase.settlementData.lSVTPayCardNum));
			strLine.Replace(_T("{储值卡购票总额}"),FromInt(m_curPurchase.settlementData.lSVTPayCardSum/100));			// 单位：元
			strLine.Replace(_T("{二维码购票总数}"),FromInt(m_curPurchase.settlementData.lNETPayCardNum));
			strLine.Replace(_T("{二维码购票总额}"),FromInt(m_curPurchase.settlementData.lNETPayCardSum/100));			// 单位：元
		
			strLine.Replace(_T("{充值总数}"),FromInt(m_curPurchase.settlementData.lCardRechargeNum));
			strLine.Replace(_T("{充值总额}"),FromInt(m_curPurchase.settlementData.lCardRechargeSum/100));				// 单位：元

			strLine.Replace(_T("{销售总计}"),FromInt(m_BalanceData.nSumSale/100));										// 合计


			if (strLine.Find(_T("{硬币接收}")) >= 0)
			{
				if (itCoinRecv !=  m_BalanceData.vecCoinReceived.end())  //硬币接收
				{
					//{硬币接受}             {硬币接收数量}          {硬币接收金额}MOP
					strLine.Replace(_T("{硬币接收}"),CashValueToString(itCoinRecv->value));
					strLine.Replace(_T("{硬币接收数量}"),ComFormatCount(itCoinRecv->nCount));
					strLine.Replace(_T("{硬币接收金额}"),ComFormatAmount(itCoinRecv->lAmount));
					itCoinRecv++;
				}else
					continue;
			}

			strLine.Replace(_T("{硬币接收总数}"),GetSum(m_BalanceData.vecCoinReceived).first);
			strLine.Replace(_T("{硬币接收总额}"),GetSum(m_BalanceData.vecCoinReceived).second);
			

			if (strLine.Find(_T("{纸币接收}")) >= 0)
			{
				if (itBkntRecv != m_BalanceData.vecNotesReceived.end()) //纸币接收
				{
					strLine.Replace(_T("{纸币接收}"),CashValueToString(itBkntRecv->value));
					strLine.Replace(_T("{纸币接收数量}"),ComFormatCount(itBkntRecv->nCount));
					strLine.Replace(_T("{纸币接收金额}"),ComFormatAmount(itBkntRecv->lAmount));
					itBkntRecv ++;
				}else
					continue;
			}
			strLine.Replace(_T("{纸币接收总数}"),GetSum(m_BalanceData.vecNotesReceived).first);
			strLine.Replace(_T("{纸币接收总额}"),GetSum(m_BalanceData.vecNotesReceived).second);


			strLine.Replace(_T("{现金接收金额SUM}"),ComFormatAmount(m_curPurchase.settlementData.lNoteIncomeSum + m_curPurchase.settlementData.lCoinIncomeSum));


			
			if (strLine.Find(_T("{硬币找零}")) >= 0)
			{
				if (itCoinChang != m_BalanceData.vecCoinReleased.end()) //硬币找零
				{
					strLine.Replace(_T("{硬币找零}"),CashValueToString(itCoinChang->value));
					strLine.Replace(_T("{硬币找零数量}"),ComFormatCount(itCoinChang->nCount));
					strLine.Replace(_T("{硬币找零金额}"),ComFormatAmount(itCoinChang->lAmount));
					itCoinChang++;
				}
				else continue;
			}
			strLine.Replace(_T("{硬币找零总数量}"),GetSum(m_BalanceData.vecCoinReleased).first);	// 硬币找零合计
			strLine.Replace(_T("{硬币找零总金额}"),GetSum(m_BalanceData.vecCoinReleased).second);

			//strLine.Replace(_T("{纸币找零数量}"),FromInt(m_BalanceData.nBnChangeCount));//:{纸币找零金额}
			//strLine.Replace(_T("{纸币找零金额}"),ComFormatAmount(m_BalanceData.nBnChangeCount*500));

			// 纸币找零
			if(strLine.Find(_T("{纸币找零}")) >= 0){

				if(itBHChange != m_BalanceData.vecBanknoteChangeInfo.end()){
					strLine.Replace(_T("{纸币找零}"),CashValueToString(itBHChange->value));
					strLine.Replace(_T("{纸币找零数量}"),ComFormatCount(itBHChange->amount));
					strLine.Replace(_T("{纸币找零金额}"),ComFormatAmount(FormatCashToINT(itBHChange->value) * itBHChange->amount));
					itBHChange++;
				}
				else continue;
			}

			strLine.Replace(_T("{纸币找零总数量}"),ComFormatCount(m_BalanceData.nBnChangeCount));	// 硬币找零合计
			strLine.Replace(_T("{纸币找零总金额}"),ComFormatAmount(m_curPurchase.settlementData.lNoteChangeSum));

			strLine.Replace(_T("{找零合计金额}"),ComFormatAmount(m_BalanceData.nCashChangeSum));


			// 找零箱补充
			if(strLine.Find(_T("{硬币补充}")) >= 0)
			{
				if(itCoinAdd != m_BalanceData.mapCoinAdded.end()){
					strLine.Replace(_T("{硬币补充}"),CashValueToString((BankNoteAndCoinType_t)itCoinAdd->first));
					strLine.Replace(_T("{补充数量}"),ComFormatCount(itCoinAdd->second));
					strLine.Replace(_T("{补充金额}"),ComFormatAmount(itCoinAdd->second * FormatCashToINT(itCoinAdd->first)));
					itCoinAdd++;
				}
				else continue;
			}
			
			// 硬币合计补充金额
			strLine.Replace(_T("{硬币补充金额}"),ComFormatAmount(m_curPurchase.settlementData.lCoinSupplementSum));

			// 纸币补充

			if(strLine.Find(_T("{纸币补充}")) >= 0){
				if(itBankAdd != m_BalanceData.mapBanknotAdded.end()){

					strLine.Replace(_T("{纸币补充}"),CashValueToString((BankNoteAndCoinType_t)itBankAdd->first));
					strLine.Replace(_T("{补充数量}"),ComFormatCount(itBankAdd->second));
					strLine.Replace(_T("{补充金额}"),ComFormatAmount(itBankAdd->second * FormatCashToINT(itBankAdd->first)));

					itBankAdd++;
				}
				else continue;
			}

			// 纸币合计补充金额
			strLine.Replace(_T("{纸币补充金额}"),ComFormatAmount(m_curPurchase.settlementData.lNoteSupplementSum));


			strLine.Replace(_T("{补充合计金额}"),ComFormatAmount(m_curPurchase.settlementData.lCoinSupplementSum + m_curPurchase.settlementData.lNoteSupplementSum));



			// 回收
			if(strLine.Find(_T("{硬币回收}")) >= 0){
				if(itCoinColl != m_BalanceData.vecCoinCollectionBox.end()){
					strLine.Replace(_T("{硬币回收}"),CashValueToString(itCoinColl->value));
					strLine.Replace(_T("{回收数量}"),ComFormatCount(itCoinColl->nCount));
					strLine.Replace(_T("{回收金额}"),ComFormatAmount(itCoinColl->lAmount));

					itCoinColl++;
				}
				else continue;
			}


			strLine.Replace(_T("{硬币回收金额}"),ComFormatAmount(m_curPurchase.settlementData.lCoinRecoverSum));

			// 纸币回收
			if(strLine.Find(_T("{纸币回收}")) >= 0)
			{
				if (itBkntRcyl != m_BalanceData.vecCashCollection.end() ) //纸币回收
				{
					strLine.Replace(_T("{纸币回收}"),CashValueToString(itBkntRcyl->value));
					strLine.Replace(_T("{纸币回收数量}"),ComFormatCount(itBkntRcyl->nCount));
					strLine.Replace(_T("{纸币回收金额}"),ComFormatAmount(itBkntRcyl->lAmount));
					itBkntRcyl++;
				}else
					continue;
			}
			strLine.Replace(_T("{纸币回收金额}"),ComFormatAmount(m_curPurchase.settlementData.lNoteRecoverSum));		// 纸币回收金额合计,以元为单位

			strLine.Replace(_T("{回收合计金额}"),ComFormatAmount(m_curPurchase.settlementData.lCoinRecoverSum + m_curPurchase.settlementData.lNoteRecoverSum));



			// 现金留存
			if(m_BalanceData.vec_BanknoteRecyclerReservInfo.size() > 0){
				// 纸币现金留存
				if(strLine.Find(_T("{纸币循环找零箱}")) >=0 ){
					if(iterBHReReserv != m_BalanceData.vec_BanknoteRecyclerReservInfo.end()){
						if(iterBHReReserv->type == RECYCLE_CHANGE_BOX){
							strLine.Replace(_T("{纸币循环找零箱}"),_T("纸币循环找零箱"));
						}
						strLine.Replace(_T("{纸币循环找零箱ID}"),iterBHReReserv->nBoxID);
					}
					else continue;
				}

				if(strLine.Find(_T("{纸币循环找零箱面额}")) >= 0){
					if(iterBHReReserv != m_BalanceData.vec_BanknoteRecyclerReservInfo.end()){
						strLine.Replace(_T("{纸币循环找零箱面额}"),_T(""));
						strLine.Replace(_T("{数量}"),ComFormatCount(iterBHReReserv->amount));
						strLine.Replace(_T("{金额}"),ComFormatAmount((int)(iterBHReReserv->value)));
						iterBHReReserv++;
					}
					else continue;
				}
			}

			if(m_BalanceData.vec_BanknoteChangeReservInfo.size() > 0){
				// 纸币现金留存
				if(strLine.Find(_T("{纸币找零箱}")) >=0 ){
					if(iterBHReserv != m_BalanceData.vec_BanknoteChangeReservInfo.end()){
						if(iterBHReserv->type == CHANGE_BOX_A){
							strLine.Replace(_T("{纸币找零箱}"),_T("纸币找零箱"));
						}
						else if(iterBHReserv->type == CHANGE_BOX_B){
							strLine.Replace(_T("{纸币找零箱}"),_T("纸币找零箱B"));
						}
						strLine.Replace(_T("{纸币找零箱ID}"),iterBHReserv->nBoxID);
					}
					else continue;
				}

				if(strLine.Find(_T("{找零箱面额}")) >= 0){
					if(iterBHReserv != m_BalanceData.vec_BanknoteChangeReservInfo.end()){
						strLine.Replace(_T("{找零箱面额}"),CashValueToString(iterBHReserv->value));
						strLine.Replace(_T("{数量}"),ComFormatCount(iterBHReserv->amount));
						strLine.Replace(_T("{金额}"),ComFormatAmount(iterBHReserv->amount * FormatCashToINT(iterBHReserv->value)));
						iterBHReserv++;
					}
					else continue;
				}
			}
			
			if(m_BalanceData.vec_BanknoteCollectionReservInfo.size() > 0){
				if(strLine.Find(_T("{回收箱}")) >= 0){
					strLine.Replace(_T("{回收箱}"),_T("纸币回收箱"));
					strLine.Replace(_T("{纸币回收箱ID}"),theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString());
				}

				if(strLine.Find(_T("{纸币留存}")) >= 0){
					if(iterBHCollBoxReserv != m_BalanceData.vec_BanknoteCollectionReservInfo.end()){
						strLine.Replace(_T("{纸币留存}"),CashValueToString(iterBHCollBoxReserv->value));
						strLine.Replace(_T("{纸币留存数量}"),ComFormatCount(iterBHCollBoxReserv->nCount));
						strLine.Replace(_T("{纸币留存金额}"),ComFormatAmount(iterBHCollBoxReserv->lAmount));
						iterBHCollBoxReserv++;
					}
					else continue;
				}
			}
			
			strLine.Replace(_T("{纸币留存合计}"),ComFormatAmount(m_curPurchase.settlementData.lNoteCurrentLeftSum));

			// 硬币留存信息
			if(m_BalanceData.vec_CoinReservInfo.size() > 0){
				bool bFindReplace = false;
				if(strLine.Find(_T("{硬币找零箱}")) >= 0){
					auto iterTemp = m_BalanceData.vec_CoinReservInfo.begin();
					while(iterTemp != m_BalanceData.vec_CoinReservInfo.end()){
						if(iterTemp->type == CHANGE_BOX_A){
							strLine.Replace(_T("{硬币找零箱}"),_T("硬币找零箱A"));
							strLine.Replace(_T("{硬币箱ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{面额}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{数量}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}

						if(iterTemp->type == CHANGE_BOX_B){
							strLine.Replace(_T("{硬币找零箱}"),_T("硬币找零箱B"));
							strLine.Replace(_T("{硬币箱ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{面额}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{数量}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}
						iterTemp++;
					}

					if(!bFindReplace){
						continue;
					}
				}

				if(strLine.Find(_T("{循环找零箱}")) >= 0){

					auto iterTemp = m_BalanceData.vec_CoinReservInfo.begin();
					while(iterTemp != m_BalanceData.vec_CoinReservInfo.end()){
						if(iterTemp->type == CYCLE_BOX_A){
							strLine.Replace(_T("{循环找零箱}"),_T("循环找零箱A"));
							strLine.Replace(_T("{硬币箱ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{面额}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{数量}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}

						if(iterTemp->type == CYCLE_BOX_B){
							strLine.Replace(_T("{循环找零箱}"),_T("循环找零箱B"));
							strLine.Replace(_T("{硬币箱ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{面额}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{数量}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}
						iterTemp++;
					}
					if(!bFindReplace){
						continue;
					}
				}

				if(strLine.Find(_T("{硬币回收箱}")) >= 0){
					auto iterTemp = m_BalanceData.vec_CoinReservInfo.begin();
					while(iterTemp != m_BalanceData.vec_CoinReservInfo.end()){
						if(iterTemp->type == COLLECTION_BOX_A){
							strLine.Replace(_T("{硬币回收箱}"),_T("硬币回收箱"));
							strLine.Replace(_T("{硬币箱ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{面额}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{数量}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}

						if(iterTemp->type == COLLECTION_BOX_B){
							strLine.Replace(_T("{硬币回收箱}"),_T("硬币回收箱B"));
							strLine.Replace(_T("{硬币箱ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{面额}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{数量}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}
						iterTemp++;
					}
					if(!bFindReplace){
						continue;
					}
				}
			}
			// 剔除那些没有留存的
			else{
				if(strLine.Find(_T("{硬币找零箱}")) >= 0 || strLine.Find(_T("{循环找零箱}")) >= 0 || strLine.Find(_T("{硬币回收箱}")) >= 0	){
					continue;
				}
			}
			
			strLine.Replace(_T("{硬币留存合计}"),ComFormatAmount(m_curPurchase.settlementData.lCoinCurrentLeftSum));

			//故障交易
			strLine.Replace(_T("{发卡失败数量}"),ComFormatCount(m_BalanceData.mapException[CARD_FAILURE].first));
			strLine.Replace(_T("{发卡失败金额}"),ComFormatAmount(m_BalanceData.mapException[CARD_FAILURE].second));
			//strLine.Replace(_T("{发Token失败数}"),ComFormatCount(m_BalanceData.mapException[TOKEN_FAILURE].first));
			//strLine.Replace(_T("{发Token失败金额}"),ComFormatAmount(m_BalanceData.mapException[TOKEN_FAILURE].second));

			strLine.Replace(_T("{找零失败数量}"),ComFormatCount(m_BalanceData.mapException[COIN_CHANGE_FAILURE].first));
			strLine.Replace(_T("{找零失败金额}"),ComFormatAmount(m_BalanceData.mapException[COIN_CHANGE_FAILURE].second));
			

			strLine.Replace(_T("{故障数量合计}"),ComFormatCount(m_BalanceData.nSumExceptionCount));
			strLine.Replace(_T("{故障金额合计}"),ComFormatAmount(m_BalanceData.nSumExceptionAmount));

			CString strExchange(_T(""));
			strLine.Replace(_T("{操作时间}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			strExchange.Format(_T("%d"),m_BalanceData.nOperatorID);
			strLine.Replace(_T("{操作员编号}"),theAPP_SESSION.GetUserInfo().sUserID);
			strLine.Replace(_T("{车站名称_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			strLine.Replace(_T("{车站名称_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			strLine.Replace(_T("{车站名称_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			strLine.Replace(_T("{设备编号}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			//将数据存储到凭证里

			CString strSave = _T("");
			if (strLine.GetLength()>0)
			{
				strSave = strLine.Left(strLine.GetLength() - 1);
			}
			saReception.Add(strSave);//去掉/r

			//将数据格式化为打印机Helper需要的格式
			CStringArray printLines;
			ComPrintLineFormatForTemplate(strLine,printLines);
			saPrintText.Append(printLines);
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BALANCE);
		//保存业结单数据到本地，提供给凭证补打功能使用
		PRINTER_HELPER->SaveToPrintHistoryFile(&saReception);
		
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("日结文件保存成功。"));

		// 调用打印机打印业结单数据
		long ret = 0;
		if(bIsPrint){
			ret  = PRINTER_HELPER->PrintReceiptByText(&saPrintText,false,true);
		}
		
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("日结文件打印成功。"));
		return ret;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("日结文件打印失败！"));
	}

	//theSERVICE_MGR.SetForeServiceBusy(false);
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("日结文件打印失败！"));
	// 失败
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		将产品类型转为CString格式 

@param      ProductType& prodtype： 产品类型

@retval     CString格式的产品类型

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CAuditHelper::GetProductName(ProductType prodtype)
{
	CString strProduct;
	switch(prodtype)
	{
	/*case SJT: strProduct = _T("SJT");break;
	case SVT: strProduct = _T("SVT");break;
	case PST: strProduct = _T("PST");break;
	case MST: strProduct = _T("MST");break;*/
	default: break;
	}
	return strProduct;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		将面值转为CString格式

@param      BankNoteAndCoinType_t& value

@retval     CString格式的面值

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CAuditHelper::CashValueToString(BankNoteAndCoinType_t& value)
{
	CString strValue;
	switch(value)
	{
	case Coin1Yuan:strValue = _T("1元");break;
		//case Coin2MOP:strValue = _T("  2MOP");break;
		//case Coin5MOP:strValue = _T("  5MOP");break;
	case CoinHalfYuan:strValue = _T("5角");break;
	case Banknote1Yuan:strValue = _T("  1元");break;
	case Banknote5Yuan:strValue = _T("  5元");break;
	case Banknote10Yuan:strValue = _T(" 10元");break;
	case Banknote20Yuan:strValue = _T(" 20元");break;
	case Banknote50Yuan:strValue = _T(" 50元");break;
	case Banknote100Yuan:strValue = _T("100元");break;
	default: break;
	}
	return strValue;
}

// 讲现金面值转为int格式方便运算
// 单位： 分
int CAuditHelper::FormatCashToINT(BankNoteAndCoinType_t cash)
{
	int nCash = 0;
	switch(cash)
	{
	case CoinHalfYuan:		nCash =  100;break;
	case Coin1Yuan:			nCash =  100;break;
		//case Coin5MOP:			return 500;
		//case Coin50AVOS:		return 50;
	case Banknote1Yuan:		nCash =  100;break;
	case Banknote5Yuan:		nCash =  500;break;
	case Banknote10Yuan:	nCash =  1000;break;
	case Banknote20Yuan:	nCash =  2000;break;
	case Banknote50Yuan:	nCash =  5000;break;
	case Banknote100Yuan:	nCash =  10000;break;
	default: break;
	}
	return nCash;
}

// 求和函数：适用于销售统计
pair<CString, CString> CAuditHelper::GetSum( map<ProductType, pair<int, int>> mapData)
{
	pair<CString, CString> sum_pair;
	CString strAmount;
	map<ProductType, pair<int,int>>::iterator iter =  mapData.begin();
	int nSumCount(0), nAmountSum(0);
	while(iter != mapData.end())
	{
		nSumCount += iter->second.first;
		nAmountSum += iter->second.second;
		iter ++;
	}

	sum_pair.first = ComFormatCount(nSumCount);
	sum_pair.second = ComFormatAmount(nAmountSum);
	return sum_pair;
}

// 求和函数：适用于现金操作
pair<CString, CString> CAuditHelper::GetSum( vector<CASH_DATA>& vec)
{
	pair<CString, CString> sum_pair;
	CString strAmount;
	vector<CASH_DATA>::iterator iter =  vec.begin();
	int nSumCount(0), nAmountSum(0);
	while(iter != vec.end())
	{
		nSumCount += iter->nCount;
		nAmountSum += iter->lAmount;
		iter ++;
	}
	sum_pair.first = ComFormatCount(nSumCount);
	sum_pair.second = ComFormatAmount(nAmountSum );
	return sum_pair;
}

// 求和函数：适用于钱箱操作
pair<CString, CString> CAuditHelper::GetSum( vector<CASH_BOX_DATA>& vec)
{
	pair<CString, CString> sum_pair;
	CString strAmount;
	vector<CASH_BOX_DATA>::iterator iter =  vec.begin();
	int nSumCount(0), nAmountSum(0);
	while(iter != vec.end())
	{
		nSumCount += iter->amount;
		//nAmountSum += iter->lAmount;
		iter ++;
	}
	sum_pair.first = ComFormatCount(nSumCount);
	sum_pair.second = ComFormatAmount(nAmountSum / 100);
	return sum_pair;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      转换硬币类型为值

@param      int value硬币类型

@retval     DWORD 面额单位AVOS

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
DWORD CAuditHelper::TransTypeToValue(int value){
	switch (value) {
	case 1: return 50;
	case 2: return 100;
	case 3: return 200;
	case 4: return 500;
	default: 
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      发送AVM、TVM数据到服务器

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CAuditHelper::SendTVMAuditData(){
	try{
		// 起始时间
		m_curPurchase.auditTimeFrom = m_BalanceData.dtOriginTime;
		m_curPurchase.auditTimeTo   = m_BalanceData.dtCurrentTime;
		// 售卡总金额与充值总金额已统计

		// 故障交易数量及金额
		m_curPurchase.settlementData.lFailBusinessNum = m_BalanceData.nSumExceptionCount;
		m_curPurchase.settlementData.lFailBusinessSum = m_BalanceData.nSumExceptionAmount;

		// ------------------------------硬币相关------------------------------------
		CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
		CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeboxA = theCOIN_COUNT.GetChangeCoinboxAInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeboxB = theCOIN_COUNT.GetChangeCoinboxBInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxB = theCOIN_COUNT.GetCollectionCoinboxBInfo();
		// 硬币上次留存
		m_curPurchase.settlementData.lCoinLastLeftSum = 
			hopperB.ulInitCount * TransTypeToValue(theTVM_INFO.GetCHHopper1Type()) 
		  + hopperA.ulInitCount * TransTypeToValue(theTVM_INFO.GetCHHopper2Type()) 
		  + changeboxA.ulLastLeftCount * TransTypeToValue(theTVM_INFO.GetCHChangeBoxAType())
		  + changeboxB.ulLastLeftCount * TransTypeToValue(theTVM_INFO.GetCHChangeBoxBType())
		  + collectionBoxA.ulInitCount + collectionBoxB.ulInitCount;

		hopperA.ulInitCount = hopperA.ulCurCount;
		hopperB.ulInitCount = hopperB.ulCurCount;
		changeboxA.ulLastLeftCount = changeboxA.ulCount;
		changeboxB.ulLastLeftCount = changeboxB.ulCount;
		collectionBoxA.ulInitCount = collectionBoxA.ulCount_1yuan * 100 + collectionBoxA.ulCount_5jiao * 100;
		collectionBoxB.ulInitCount = collectionBoxB.ulCount_1yuan * 100 + collectionBoxB.ulCount_5jiao * 100;

		// 补充硬币金额(已经统计)
		// 累计回收金额（已经统计）
		// 累计收入硬币金额（已统计）
		// 累计找零金额（已统计）
		// 本次留存硬币金额（已统计）

		theCOIN_COUNT.SetCoinHopperboxAInfo(hopperA);
		theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);
		theCOIN_COUNT.SetChangeCoinboxAInfo(changeboxA);
		theCOIN_COUNT.SetChangeCoinboxBInfo(changeboxB);
		theCOIN_COUNT.SetCollectionCoinboxAInfo(collectionBoxA);
		theCOIN_COUNT.SetCollectionCoinboxBInfo(collectionBoxB);

		//--------------------------------纸币相关--------------------------------------
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteBox = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_a = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_b = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_c = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_d = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

		// 纸币上次留存
		m_curPurchase.settlementData.lNoteLastLeftSum  = banknoteBox.ulLastLeftAmount + 
			recycler_box_a.ulLastLeftAmount + recycler_box_b.ulLastLeftAmount + 
			recycler_box_c.ulLastLeftAmount + recycler_box_d.ulLastLeftAmount;

		// 补充（已统计）
		// 回收（已统计）
		// 收入（已统计）
		// 找零（已统计）
		// 本次留存（已统计）

		banknoteBox.ulLastLeftAmount = banknoteBox.BankNotesCountAvos();
		recycler_box_a.ulLastLeftAmount = recycler_box_a.BankNotesCountAvos();
		recycler_box_b.ulLastLeftAmount = recycler_box_b.BankNotesCountAvos();
		recycler_box_c.ulLastLeftAmount = recycler_box_c.BankNotesCountAvos();
		recycler_box_d.ulLastLeftAmount = recycler_box_d.BankNotesCountAvos();

		theBANKNOTE_COUNT.SetBanknoteboxInfo(banknoteBox);
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxAInfo(recycler_box_a);
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxBInfo(recycler_box_b);
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxCInfo(recycler_box_c);
		theBANKNOTE_COUNT.SetBanknoteRecyclerBoxDInfo(recycler_box_d);

		//-------------------------------票卡相关---------------------------------------
		//CTCardCountInfo::CARD_BOX_INFO cardBoxA = theCARD_COUNT.GetCardboxAInfo();
		//CTCardCountInfo::CARD_BOX_INFO cardBoxB = theCARD_COUNT.GetCardboxBInfo();
		//CTCardCountInfo::CARD_BOX_INFO cardWasted = theCARD_COUNT.GetCardboxWastedInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardBoxA = theTOKEN_COUNT.GetHopperAInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardBoxB = theTOKEN_COUNT.GetHopperBInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardWasted = theTOKEN_COUNT.GetTokenboxWastedInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardCollected = theTOKEN_COUNT.GetTokenboxCollectedInfo();

		// 上次留存
		m_curPurchase.settlementData.lCardLastLeftNum = cardBoxA.ulInitCount + cardBoxB.ulInitCount + cardWasted.ulInitCount;
		// 本次废票数
		m_curPurchase.settlementData.lCardCurrentInvalidateNum += cardWasted.ulCurCount;
		// 本次补充累计(已统计)
		// 本次累计售出
		m_curPurchase.settlementData.lCardCurrentSelledNum = m_curPurchase.settlementData.lSelledCardNum;
		// 本次回收(已统计)
		// 本次留存
		m_curPurchase.settlementData.lCardCurrentLeftNum = cardWasted.ulCurCount + cardBoxA.ulCurCount + cardBoxB.ulCurCount;

		cardBoxA.ulInitCount = cardBoxA.ulCurCount;
		cardBoxB.ulInitCount = cardBoxB.ulCurCount;
		cardWasted.ulInitCount = cardWasted.ulCurCount;
		cardCollected.ulInitCount = cardCollected.ulCurCount;

		//theCARD_COUNT.SetCardboxAInfo(cardBoxA);
		//theCARD_COUNT.SetCardboxBInfo(cardBoxB);
		//theCARD_COUNT.SetCardboxWastedInfo(cardWasted);
		theTOKEN_COUNT.SetHopperAInfo(cardBoxA);
		theTOKEN_COUNT.SetHopperBInfo(cardBoxB);
		theTOKEN_COUNT.SetTokenboxWastedInfo(cardWasted);
		theTOKEN_COUNT.SetTokenboxCollectedInfo(cardCollected);

		// 发送业结审计数据
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(m_curPurchase);

		// 设置为已发送
		theSCAudit_MGR.SetDayPurchaseSended(m_curPurchase.businessDate);

		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("日结文件已发送至上位。"));
	}
	catch(CSysException e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("日结文件设置发送失败。"));
	}
}

CString CAuditHelper::FromInt(int num)
{
	CString str;
	str.Format(_T("%d"),num);
	return str;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析纸币面值代码 BIN	1：1元，2：5元，3：10元，4：20元，5：50元，6：100元

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CAuditHelper::TransBanknoteType(int value) {
	switch (value) {
	case 1: return Banknote1Yuan;
	case 5: return Banknote5Yuan;
	case 10: return Banknote10Yuan;
	case 20: return Banknote20Yuan;
	case 50: return Banknote50Yuan;
	case 100: return Banknote100Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("纸币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      解析硬币面值代码 BIN	0：无效，1：0.5Yuan，2：1Yuan

@param      无

@retval     纸币接受金额 以分为单位

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CAuditHelper::TransCoinType(int value) {
	switch (value) {
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("硬币箱币种为无效，但是countInfo记录数量非零"));
		return VALUE_UNKNOWN;
	}
}