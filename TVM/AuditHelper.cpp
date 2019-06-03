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
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAuditHelper::CAuditHelper(CService &service):CServiceHelper(service)
{
	DeviceType = theMAINTENANCE_INFO.GetDeviceType();					// ��ȡ�豸����
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CAuditHelper::~CAuditHelper()
{
}


CAuditHelper::BALANCE_DATA CAuditHelper::LoadBalanceData()
{
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("��ʼ�����ս�����"));
	m_curPurchase.settlementData = _US_SETTLEMENT_DATA();
	m_BalanceData = _balance_data();
	//��ȡҵ���������
	PurchaseData curDayPurchase = theSCAudit_MGR.GetCurrentDayPurchase();
	CString beginTime = /*_T("20160508040000")*/ curDayPurchase.auditTimeFrom.ToString(_T("%04d%02d%02d%02d%02d%02d"));
	CString	endTime = /*_T("20160508230000");*/  ComGetCurTime().ToString(_T("%04d%02d%02d%02d%02d%02d"));
	m_BalanceData.dtOriginTime = curDayPurchase.auditTimeFrom/*.ToString(_T("%04d-%02d-%02d %02d%:02d%:02d"))*/;
	m_BalanceData.dtCurrentTime = curDayPurchase.auditTimeTo = ComGetCurTime();
	vector<TransactionRecord> vec_transactionInfo;
	DATA_HELPER->DbSelectTransactionInfo(beginTime, endTime, vec_transactionInfo,false);	//����ͳ���������ݿ�ӿ�
	vector<TransactionRecord> ::iterator iter_MainTrans = vec_transactionInfo.begin();
	PRODUCT_TRANS_DATA  product;
	m_BalanceData.mapBCIssue.clear();
	while ( iter_MainTrans != vec_transactionInfo.end() )
	{
		if(iter_MainTrans->iTransactionDetail.iState != 2){ // ��Ϊȡ������ʱ��ͳ��
			// ��Ʊͳ��
			if(iter_MainTrans->iTransactionDetail.iUdType == CARD_ISSUE){
				if(iter_MainTrans->iTransactionDetail.iState == 1 || iter_MainTrans->iTransactionDetail.iState == 3	){	// ���׹��Ͻ���||����ȡ������
					m_curPurchase.settlementData.lSelledCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
					m_curPurchase.settlementData.lSelledCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount; // ���۽��ϼ�
					// ��ֵ����Ʊ�ۿ�ͳ��
					if (iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_ACC_CARD || iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_YKT_CARD){
						m_curPurchase.settlementData.lSVTPayCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
						m_curPurchase.settlementData.lSVTPayCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					}
					// ��ά�빺Ʊͳ��
					if (iter_MainTrans->iTransactionDetail.iPaymentMethod == PAYMENT_METHOD_WITH_ZHIFUBAO){
						m_curPurchase.settlementData.lSVTPayCardNum += iter_MainTrans->iTransactionDetail.iQuantity;
						m_curPurchase.settlementData.lSVTPayCardSum += iter_MainTrans->iTransactionDetail.iTotalAmount;
					}
				}
				else{
					m_curPurchase.settlementData.lSelledCardNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
					m_curPurchase.settlementData.lSelledCardSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount; // ���۽��ϼ�
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

			// ��ֵͳ��
			if (iter_MainTrans->iTransactionDetail.iUdType == RECHARGE){
				if(iter_MainTrans->iTransactionDetail.iState == 1){
					m_curPurchase.settlementData.lCardRechargeNum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity;
					m_curPurchase.settlementData.lCardRechargeSum += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iUnReleaseedMediaQuantity * iter_MainTrans->iTransactionDetail.iUnitAmount;; // ���۽��ϼ�
				}
				else{
					m_curPurchase.settlementData.lCardRechargeNum += iter_MainTrans->iTransactionDetail.iQuantity;
					m_curPurchase.settlementData.lCardRechargeSum += iter_MainTrans->iTransactionDetail.iTotalAmount;
					m_BalanceData.nSumSale += iter_MainTrans->iTransactionDetail.iTotalAmount; // ���۽��ϼ�
				}
			}

		}
		iter_MainTrans ++;
	}


	// Ӳ�ҽ���(Ӳ�����ɱ�)
	/*
	ƾ֤��ʽ��	��ֵ			����		���
	��
	SUM				�����ϼ�	���ϼ�
	*/
	vector<CoinAcceptRecord> vec_CoinAcceptRecord;
	DATA_HELPER->DbSelectCoinAcceptRecord( beginTime,endTime, vec_CoinAcceptRecord);		// �����ݿ���ȡӲ�ҽ�������
	
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
	

	// ֽ�ҽ���
	/*
	ƾ֤��ʽ��	��ֵ			����		���
	��
	SUM				�����ϼ�	���ϼ�
	*/
	vector<BanknoteAcceptRecord> vec_BanknoteAcceptRecord;
	DATA_HELPER->DbSelectBanknoteAcceptRecord(beginTime, endTime,  vec_BanknoteAcceptRecord);	//ֽ�ҽ��յ����ݿ�ӿ�
	vector<BanknoteAcceptRecord>::iterator iter_Note =  vec_BanknoteAcceptRecord.begin();
	map<int, int> mapNote;// ҵ�������еģ�ֽ�ҽ��սṹ��map<��ֵ������>�� ����ֵ*������
	mapNote[Banknote1Yuan] = mapNote[Banknote5Yuan] = mapNote[Banknote10Yuan] = mapNote[Banknote20Yuan] = mapNote[Banknote50Yuan] = mapNote[Banknote100Yuan] = 0;
	while(  iter_Note !=  vec_BanknoteAcceptRecord.end())
	{
		mapNote[iter_Note->iCoinTypeCode] += iter_Note->iQuantity;	// ��ֵ -> ����
		iter_Note++;
	}
	CASH_DATA Note;
	for (map<int, int>::iterator itMap = mapNote.begin(); itMap != mapNote.end(); itMap++)
	{
		Note.value = (BankNoteAndCoinType_t)itMap->first;
		Note.nCount = itMap->second;
		Note.lAmount = FormatCashToINT((BankNoteAndCoinType_t)itMap->first) * itMap->second; //����ֵ*������
		// ������ݣ�ֽ�ҽ��ս��
		m_curPurchase.settlementData.lNoteIncomeSum += Note.lAmount;
		if(Note.nCount > 0)m_BalanceData.vecNotesReceived.push_back(Note);
	}

	// Ӳ������
	// Ӳ������ͳ�Ƶ����ݴ���ʽ�롰ֽ�ҽ��ա���ͬ
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
		// ����ã������ܽ��
		m_curPurchase.settlementData.lCoinChangeSum += Change.lAmount;
		if(Change.nCount > 0)m_BalanceData.vecCoinReleased.push_back(Change);
	}

	// ֽ������
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
	// Ӳ�Ҳ���
	/* ƾ֤��ʽ:		
	  1Ԫ��			����		���
	0.5Ԫ:			����		���
	
	�����������������Ĺ���:�ĵ�������ʽ����(Lib ���ݿ�).docx��2.2.1Ӳ�������䲹�������¼��tb_CoinAddOperationRecord��
	*/
	m_BalanceData.mapCoinAdded.clear();
	vector<CoinAddOperationRecord> vec_CoinAddOperationRecord;
	DATA_HELPER-> DbSelectCoinAddOperationRecord( beginTime, endTime, vec_CoinAddOperationRecord);
	vector<CoinAddOperationRecord>::iterator it_CoinAdded =  vec_CoinAddOperationRecord.begin();
	CASH_BOX_DATA coin_add;

	while(it_CoinAdded !=  vec_CoinAddOperationRecord.end()){
		coin_add.value = (BankNoteAndCoinType_t)it_CoinAdded->iCoinTypeCode;	// ��ֵ
		coin_add.amount = it_CoinAdded->iQuantityAdd;							// ����
		
		// ��ΪһԪΪA�����ΪB
		if(coin_add.value == Coin1Yuan){
			m_BalanceData.mapCoinAdded[Coin1Yuan] += coin_add.amount;
		}
		else if(coin_add.value == CoinHalfYuan){
			m_BalanceData.mapCoinAdded[Coin1Yuan/*CoinHalfYuan*/] += coin_add.amount;
		}
		// ����ã�Ӳ�Ҳ�����
		m_curPurchase.settlementData.lCoinSupplementSum += coin_add.amount * FormatCashToINT(coin_add.value);
		it_CoinAdded ++;
	}

	map<int ,int> mapCoinAllRecycle;
	mapCoinAllRecycle[CoinHalfYuan] = mapCoinAllRecycle[Coin1Yuan] = 0;
	// Ӳ�������䰲װ(ж����ζ�Ż���)
	vector<CoinChangeBoxOperationRecord> vec_CoinChangeboxOperationRecord;
	DATA_HELPER->DbSelectCoinChangeBoxOperationRecord(beginTime,endTime,vec_CoinChangeboxOperationRecord);
	auto iterCoinChangeBoxOperationRecord = vec_CoinChangeboxOperationRecord.begin();
	while(iterCoinChangeBoxOperationRecord != vec_CoinChangeboxOperationRecord.end()){
		coin_add = _CASH_BOX_DATA();
		// ��װ����
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

	// ֽ�Ҳ���
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


	// Ӳ�һ��� 
	// �������ͬ��Ӳ�Ҳ��䡱
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
	//	// ��Ҫ�ж�ѭ����������������
	//	//if(iter_CoinColl->txtCoinChangeBoxID.Right(2) == _T("03")) //zhengxianle������Ҫ�޸ģ�����
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
	//	// ����ã�Ӳ�һ��ս��
	//	m_curPurchase.settlementData.lCoinRecoverSum = coin_collect.amount * FormatCashToINT(coin_collect.value);
	//	m_BalanceData.vecCoinCollection.push_back(coin_collect);
	//	iter_CoinColl++;
	//}

	// Ӳ�һ���(ʱ���ͳ��,������λ���,ж��)
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
		m_curPurchase.settlementData.lCoinRecoverSum += CoinColl.lAmount;// Ӳ���ۼƻ��ս��
		if( CoinColl.lAmount > 0 )m_BalanceData.vecCoinCollectionBox.push_back(CoinColl);// �����л��ղŻ��ӡ����
	}

	// ֽ�һ���(�����估������)
	// ���ơ�ֽ�ҽ��ա�����ʽ
	vector<BanknoteBoxRemoveOperationRecord> vec_BanknoteBoxRemoveOperationRecord;
	DATA_HELPER->DbSelectBanknoteBoxRemoveOperationRecord(beginTime, endTime,  vec_BanknoteBoxRemoveOperationRecord);
	vector<BanknoteBoxRemoveOperationRecord>::iterator iter_NoteColl =  vec_BanknoteBoxRemoveOperationRecord.begin();

	map<int, int> mapNoteColl;// first: value , second: count per value
	mapNoteColl[Banknote1Yuan] = mapNoteColl[Banknote5Yuan] = mapNoteColl[Banknote10Yuan] = mapNoteColl[Banknote20Yuan] = mapNoteColl[Banknote50Yuan] = mapNoteColl[Banknote100Yuan] = 0;
	while (iter_NoteColl !=  vec_BanknoteBoxRemoveOperationRecord.end())
	{
		m_BalanceData.nNoteCollection += iter_NoteColl->banknoteBoxRemoveOperationDetail.iTotalAmount;//�����ܽ��
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

	// ֽ��������ж��
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
	// ͳ�ƻ����˶���Ǯ�����豸�����ߵ�Ǯ��
	CASH_DATA NoteColl;
	for ( map<int, int>::iterator iterMapColl =  mapNoteColl.begin(); iterMapColl != mapNoteColl.end();iterMapColl++)
	{
		NoteColl.value =(BankNoteAndCoinType_t) iterMapColl->first;
		NoteColl.nCount = iterMapColl->second;
		NoteColl.lAmount = FormatCashToINT(NoteColl.value) * NoteColl.nCount;
		m_curPurchase.settlementData.lNoteRecoverSum += NoteColl.lAmount;
		if(NoteColl.lAmount > 0)m_BalanceData.vecCashCollection.push_back(NoteColl);
	}

	// �ֽ�������Ϣ
	m_BalanceData.vec_BanknoteRecyclerReservInfo.clear();
	m_BalanceData.vec_BanknoteChangeReservInfo.clear();
	m_BalanceData.vec_BanknoteCollectionReservInfo.clear();
	m_BalanceData.vec_CoinReservInfo.clear();
	// ֽ��ѭ���������ֽ����
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

	// ֽ���������ֽ����
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


	// ֽ�һ��������
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

	// Ӳ�������
	// ������A
	if(theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString() != INVALID_ID){
		cashBoxData.type = CHANGE_BOX_A;
		cashBoxData.nBoxID = theCOIN_COUNT.GetChangeCoinboxAInfo().coinBoxID.ToString();
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHChangeBoxAType());
		cashBoxData.amount = theCOIN_COUNT.GetChangeCoinboxAInfo().ulCount;
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}
	
	// ������B
	if(theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString() != INVALID_ID){
		cashBoxData.type = CHANGE_BOX_B;
		cashBoxData.nBoxID = theCOIN_COUNT.GetChangeCoinboxBInfo().coinBoxID.ToString();
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHChangeBoxBType());
		cashBoxData.amount = theCOIN_COUNT.GetChangeCoinboxBInfo().ulCount;
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}
	
	// ѭ��������A�д���
	if(theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount != 0){
		cashBoxData.type = CYCLE_BOX_A;
		cashBoxData.nBoxID = theCOIN_COUNT.GetCoinHopperboxAInfo().coinBoxID.ToString();
		cashBoxData.amount = theCOIN_COUNT.GetCoinHopperboxAInfo().ulCurCount;
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHHopper1Type());
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}

	// ѭ��������B�д���
	if(theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount != 0){
		cashBoxData.type = CYCLE_BOX_B;
		cashBoxData.nBoxID = theCOIN_COUNT.GetCoinHopperboxBInfo().coinBoxID.ToString();
		cashBoxData.amount = theCOIN_COUNT.GetCoinHopperboxBInfo().ulCurCount;
		cashBoxData.value  = TransCoinType(theTVM_INFO.GetCHHopper2Type());
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}

	// ������A(���޸ģ����������һԪ�����)��ʱֻдһԪ������������ǵĴ���
	if(theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString() != INVALID_ID){
		cashBoxData.type = COLLECTION_BOX_A;
		cashBoxData.amount = theCOIN_COUNT.GetCollectionCoinboxAInfo().GetCurCount();
		cashBoxData.nBoxID = theCOIN_COUNT.GetCollectionCoinboxAInfo().coinBoxID.ToString();
		cashBoxData.value  = Coin1Yuan;
		m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
		if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	}

	//// ������B(���޸ģ����������һԪ�����)
	//if(theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString() != INVALID_ID){
	//	cashBoxData.type = COLLECTION_BOX_B;
	//	cashBoxData.amount = theCOIN_COUNT.GetCollectionCoinboxBInfo().GetCurCount();
	//	cashBoxData.nBoxID = theCOIN_COUNT.GetCollectionCoinboxBInfo().coinBoxID.ToString();
	//	cashBoxData.value  = Coin1Yuan;
	//	m_curPurchase.settlementData.lCoinCurrentLeftSum += cashBoxData.amount * FormatCashToINT(cashBoxData.value);
	//	if(cashBoxData.amount > 0 ) m_BalanceData.vec_CoinReservInfo.push_back(cashBoxData);
	//}

	// ���Ͻ���ͳ��
	/* 
	���ݽṹ��	map<EXCEPTION_TRANS, pair<int, int>> mapException;
	map<�������ͣ�pait<�������������׽��>>mapException
	*/
	// ��ʼ������
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

	// ��Ʊͳ��
	vector<CardCollectBoxRemoveOperationRecord> vec_cardwastedInfo;
	DATA_HELPER->DbSelectCardCollectBoxRemoveOperationRecord(beginTime,endTime,vec_cardwastedInfo);

	auto iterCardWasted = vec_cardwastedInfo.begin();
	while(iterCardWasted != vec_cardwastedInfo.end()){
		m_curPurchase.settlementData.lCardCurrentInvalidateNum += iterCardWasted->iQuantity;
		iterCardWasted++;
	}

	// Ʊ������
	vector<CardBoxInstallOperationRecord> vec_cardInstallInfo;
	DATA_HELPER->DbSelectCardBoxInstallOperationRecord(beginTime,endTime,vec_cardInstallInfo);

	auto iterCardInstall = vec_cardInstallInfo.begin();
	while(iterCardInstall != vec_cardInstallInfo.end()){
		m_curPurchase.settlementData.lCardCurrrntSupplementNum += iterCardInstall->iQuantity;
		iterCardInstall++;
	}

	// Ʊ������
	vector<CardBoxRemoveOperationRecord> vec_cardUninstall;
	DATA_HELPER->DbSelectCardBoxRemoveOperationRecord(beginTime,endTime,vec_cardUninstall);

	auto iterCardUninstall = vec_cardUninstall.begin();

	while(iterCardUninstall != vec_cardUninstall.end()){
		m_curPurchase.settlementData.lCardCurrentRecoverNum += iterCardUninstall->iOriginalQuantity;
		iterCardUninstall++;
	}

	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�����ս��ļ���ɡ�"));
	// ����ҵ�����ݵ���λ
	SendTVMAuditData();

	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));
	return  m_BalanceData;	
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		ҵ������ģ�廯 

@param      

@retval     CStringArray&  ҵ������

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int CAuditHelper::PrintBalanceBill(bool bIsPrint /* = true */)
{
	try{
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("��ʼ��ӡ�ս��ļ���"));
		//theSERVICE_MGR.SetForeServiceBusy(true);
		BALANCE_DATA m_BalanceData = LoadBalanceData();	//��ȡҵ������
		//theSERVICE_MGR.SetForeServiceBusy(false);

		CStringArray saBillTemplate;
		CStringArray saPrintText;
		CStringArray saReception;

		map<ProductType, pair<int, int>>::iterator itIssue = m_BalanceData.mapIssue.begin();			//��Ʊ	
		map<ProductType, pair<int, int>>::iterator itRchg = m_BalanceData.mapRecharge.begin();			//��ֵ	
		//map<ProductType, pair<int, int>>::iterator itAddpd = m_BalanceData.mapIssuePdt.begin();			//�۲�Ʒ

		map<ProductType, pair<int, int>>::iterator itCashIssue = m_BalanceData.mapCashIssue.begin();	//�ֽ���Ʊ
		map<ProductType, pair<int, int>>::iterator itCashRchrg = m_BalanceData.mapCashRecharge.begin();	//�ֽ��ֵ
		//map<ProductType, pair<int, int>>::iterator itCashAddPd = m_BalanceData.mapCashProdIssue.begin();//�ֽ��۲�Ʒ
		
		vector<CASH_DATA>::iterator itCoinRecv =  m_BalanceData.vecCoinReceived.begin();				//Ӳ�ҽ���
		vector<CASH_DATA>::iterator	itBkntRecv =  m_BalanceData.vecNotesReceived.begin();				//ֽ�ҽ���
		vector<CASH_DATA>::iterator itCoinChang = m_BalanceData.vecCoinReleased.begin();				//Ӳ������
		vector<CASH_BOX_DATA>::iterator itBHChange	= m_BalanceData.vecBanknoteChangeInfo.begin();		//ֽ������

		//vector<CASH_BOX_DATA>::iterator itCoinAdd = m_BalanceData.vecCoinAdded.begin();					//Ӳ�Ҳ���
		auto itCoinAdd = m_BalanceData.mapCoinAdded.begin();
		auto itBankAdd = m_BalanceData.mapBanknotAdded.begin();
		//vector<CASH_BOX_DATA>::iterator itBanknoteAdd = m_BalanceData.vecBanknoteAdded.begin();			//ֽ�Ҳ���
		vector<CASH_BOX_DATA> vecCommonBoxAdd;			//��ͨ��Ӳ�Ҳ���

		vector<CASH_BOX_DATA>::iterator itCoinRcyl = m_BalanceData.vecCoinCollection.begin();			//Ӳ�һ���
		vector<CASH_BOX_DATA> vecCommonBoxColl;			//��ͨ��Ӳ�һ���
		CASH_BOX_DATA	Recycle_box_Coll;				//ѭ�������

		vector<CASH_DATA>::iterator itBkntRcyl =  m_BalanceData.vecCashCollection.begin();				//ֽ�һ���
		vector<CASH_DATA>::iterator itCoinColl =  m_BalanceData.vecCoinCollectionBox.begin();			//Ӳ�һ���
		
		auto iterBHReReserv	= m_BalanceData.vec_BanknoteRecyclerReservInfo.begin();						//ֽ��ѭ������������
		auto iterBHReserv	= m_BalanceData.vec_BanknoteChangeReservInfo.begin();						//ֽ������������
		auto iterBHCollBoxReserv = m_BalanceData.vec_BanknoteCollectionReservInfo.begin();				//ֽ�һ���������
		//map<ProductType, pair<int, int>>::iterator itDCIssue = m_BalanceData.mapDCIssue.begin();		//���ÿ��ۿ�
		//map<ProductType, pair<int, int>>::iterator itDCRechrg = m_BalanceData.mapDCRecharge.begin();	//���ÿ���ֵ
		//map<ProductType, pair<int, int>>::iterator itDCAddpdt = m_BalanceData.mapDCIssuePdt.begin();	//���ÿ��۲�Ʒ
		//map<ProductType, pair<int, int>>::iterator itBCIssue = m_BalanceData.mapBCIssue.begin();		//���п��ۿ�	
		//map<ProductType, pair<int, int>>::iterator itBCRchrg = m_BalanceData.mapBCRecharge.begin();		//���п���ֵ
		//map<ProductType, pair<int, int>>::iterator itBCAddPdt = m_BalanceData.mapBCIssuePdt.begin();	//���п��۲�Ʒ
// 		int nCoinAddSum = 0, nCoinCollection = 0;
// 		while (itCoinAdd != m_BalanceData.vecCoinAdded.end())	// ����ѭ������������ͨ���Ӳ�Ҳ������
// 		{
// 			if(itCoinAdd->type == COMMON_BOX)
// 			{
// 				vecCommonBoxAdd.push_back(*itCoinAdd);
// 			}
// 			
// 			itCoinAdd++;
// 		}
// 		vector<CASH_BOX_DATA>::iterator iter_cmAdd =  vecCommonBoxAdd.begin();
// 		while( itCoinRcyl != m_BalanceData.vecCoinCollection.end())  // ����ѭ������������ͨ���Ӳ�һ��ղ���
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
		// ����ģ��
		theTEMPLATE_HELPER.LoadBillTemplate(_T("TVM_BALANCE_BILL.template"),saBillTemplate);

		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T(""));

		int baseEraseCount = 0;
		// �޸�ģ��
		for(int index = 0;index < saBillTemplate.GetCount();index++){
			CString strLine = saBillTemplate.ElementAt(index);
			// ֽ������
			if(strLine.Find(_T("<ֽ������>")) >= 0){
				if(m_BalanceData.vec_BanknoteRecyclerReservInfo.size() == 0 && m_BalanceData.vec_BanknoteChangeReservInfo.size() == 0 && m_BalanceData.vec_BanknoteCollectionReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index,16);
					index--;
					continue;
				}
			}
			if(strLine.Find(_T("{ֽ��ѭ��������}")) >= 0){
				if(m_BalanceData.vec_BanknoteRecyclerReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index, 2);
					index--;
					continue;
				}
			}
			if(strLine.Find(_T("{ֽ��������}")) >= 0){
				if(m_BalanceData.vec_BanknoteChangeReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index, 4);
					index--;
					continue;
				}
			}
			if(strLine.Find(_T("{������}")) >= 0){
				if(m_BalanceData.vec_BanknoteCollectionReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index, 7);
					index--;
					continue;
				}
			}
			
			if(strLine.Find(_T("<Ӳ������>")) >= 0){
				if(m_BalanceData.vec_CoinReservInfo.size() == 0){
					saBillTemplate.RemoveAt(index,9);
					index--;
					continue;
				}
			}
		}



		for(int i=0;i<saBillTemplate.GetCount();i++){
			CString strLine = saBillTemplate.ElementAt(i);
			strLine.Replace(_T("{��ʼʱ��}"), m_BalanceData.dtOriginTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")) );
			strLine.Replace(_T("{����ʱ��}"), m_BalanceData.dtCurrentTime.ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			
			strLine.Replace(_T("{��Ʊ����}"),FromInt(m_curPurchase.settlementData.lSelledCardNum));
			strLine.Replace(_T("{��Ʊ�ܶ�}"),FromInt(m_curPurchase.settlementData.lSelledCardSum/100));					// ��λ��Ԫ
			strLine.Replace(_T("{�ֽ�Ʊ����}"),FromInt(m_curPurchase.settlementData.lSelledCardNum - m_curPurchase.settlementData.lSVTPayCardNum - m_curPurchase.settlementData.lNETPayCardNum));
			strLine.Replace(_T("{�ֽ�Ʊ�ܶ�}"),FromInt(m_curPurchase.settlementData.lSelledCardSum/100 - m_curPurchase.settlementData.lSVTPayCardSum/100 - m_curPurchase.settlementData.lNETPayCardSum/100));// ��λ��Ԫ
			strLine.Replace(_T("{��ֵ����Ʊ����}"),FromInt(m_curPurchase.settlementData.lSVTPayCardNum));
			strLine.Replace(_T("{��ֵ����Ʊ�ܶ�}"),FromInt(m_curPurchase.settlementData.lSVTPayCardSum/100));			// ��λ��Ԫ
			strLine.Replace(_T("{��ά�빺Ʊ����}"),FromInt(m_curPurchase.settlementData.lNETPayCardNum));
			strLine.Replace(_T("{��ά�빺Ʊ�ܶ�}"),FromInt(m_curPurchase.settlementData.lNETPayCardSum/100));			// ��λ��Ԫ
		
			strLine.Replace(_T("{��ֵ����}"),FromInt(m_curPurchase.settlementData.lCardRechargeNum));
			strLine.Replace(_T("{��ֵ�ܶ�}"),FromInt(m_curPurchase.settlementData.lCardRechargeSum/100));				// ��λ��Ԫ

			strLine.Replace(_T("{�����ܼ�}"),FromInt(m_BalanceData.nSumSale/100));										// �ϼ�


			if (strLine.Find(_T("{Ӳ�ҽ���}")) >= 0)
			{
				if (itCoinRecv !=  m_BalanceData.vecCoinReceived.end())  //Ӳ�ҽ���
				{
					//{Ӳ�ҽ���}             {Ӳ�ҽ�������}          {Ӳ�ҽ��ս��}MOP
					strLine.Replace(_T("{Ӳ�ҽ���}"),CashValueToString(itCoinRecv->value));
					strLine.Replace(_T("{Ӳ�ҽ�������}"),ComFormatCount(itCoinRecv->nCount));
					strLine.Replace(_T("{Ӳ�ҽ��ս��}"),ComFormatAmount(itCoinRecv->lAmount));
					itCoinRecv++;
				}else
					continue;
			}

			strLine.Replace(_T("{Ӳ�ҽ�������}"),GetSum(m_BalanceData.vecCoinReceived).first);
			strLine.Replace(_T("{Ӳ�ҽ����ܶ�}"),GetSum(m_BalanceData.vecCoinReceived).second);
			

			if (strLine.Find(_T("{ֽ�ҽ���}")) >= 0)
			{
				if (itBkntRecv != m_BalanceData.vecNotesReceived.end()) //ֽ�ҽ���
				{
					strLine.Replace(_T("{ֽ�ҽ���}"),CashValueToString(itBkntRecv->value));
					strLine.Replace(_T("{ֽ�ҽ�������}"),ComFormatCount(itBkntRecv->nCount));
					strLine.Replace(_T("{ֽ�ҽ��ս��}"),ComFormatAmount(itBkntRecv->lAmount));
					itBkntRecv ++;
				}else
					continue;
			}
			strLine.Replace(_T("{ֽ�ҽ�������}"),GetSum(m_BalanceData.vecNotesReceived).first);
			strLine.Replace(_T("{ֽ�ҽ����ܶ�}"),GetSum(m_BalanceData.vecNotesReceived).second);


			strLine.Replace(_T("{�ֽ���ս��SUM}"),ComFormatAmount(m_curPurchase.settlementData.lNoteIncomeSum + m_curPurchase.settlementData.lCoinIncomeSum));


			
			if (strLine.Find(_T("{Ӳ������}")) >= 0)
			{
				if (itCoinChang != m_BalanceData.vecCoinReleased.end()) //Ӳ������
				{
					strLine.Replace(_T("{Ӳ������}"),CashValueToString(itCoinChang->value));
					strLine.Replace(_T("{Ӳ����������}"),ComFormatCount(itCoinChang->nCount));
					strLine.Replace(_T("{Ӳ��������}"),ComFormatAmount(itCoinChang->lAmount));
					itCoinChang++;
				}
				else continue;
			}
			strLine.Replace(_T("{Ӳ������������}"),GetSum(m_BalanceData.vecCoinReleased).first);	// Ӳ������ϼ�
			strLine.Replace(_T("{Ӳ�������ܽ��}"),GetSum(m_BalanceData.vecCoinReleased).second);

			//strLine.Replace(_T("{ֽ����������}"),FromInt(m_BalanceData.nBnChangeCount));//:{ֽ��������}
			//strLine.Replace(_T("{ֽ��������}"),ComFormatAmount(m_BalanceData.nBnChangeCount*500));

			// ֽ������
			if(strLine.Find(_T("{ֽ������}")) >= 0){

				if(itBHChange != m_BalanceData.vecBanknoteChangeInfo.end()){
					strLine.Replace(_T("{ֽ������}"),CashValueToString(itBHChange->value));
					strLine.Replace(_T("{ֽ����������}"),ComFormatCount(itBHChange->amount));
					strLine.Replace(_T("{ֽ��������}"),ComFormatAmount(FormatCashToINT(itBHChange->value) * itBHChange->amount));
					itBHChange++;
				}
				else continue;
			}

			strLine.Replace(_T("{ֽ������������}"),ComFormatCount(m_BalanceData.nBnChangeCount));	// Ӳ������ϼ�
			strLine.Replace(_T("{ֽ�������ܽ��}"),ComFormatAmount(m_curPurchase.settlementData.lNoteChangeSum));

			strLine.Replace(_T("{����ϼƽ��}"),ComFormatAmount(m_BalanceData.nCashChangeSum));


			// �����䲹��
			if(strLine.Find(_T("{Ӳ�Ҳ���}")) >= 0)
			{
				if(itCoinAdd != m_BalanceData.mapCoinAdded.end()){
					strLine.Replace(_T("{Ӳ�Ҳ���}"),CashValueToString((BankNoteAndCoinType_t)itCoinAdd->first));
					strLine.Replace(_T("{��������}"),ComFormatCount(itCoinAdd->second));
					strLine.Replace(_T("{������}"),ComFormatAmount(itCoinAdd->second * FormatCashToINT(itCoinAdd->first)));
					itCoinAdd++;
				}
				else continue;
			}
			
			// Ӳ�ҺϼƲ�����
			strLine.Replace(_T("{Ӳ�Ҳ�����}"),ComFormatAmount(m_curPurchase.settlementData.lCoinSupplementSum));

			// ֽ�Ҳ���

			if(strLine.Find(_T("{ֽ�Ҳ���}")) >= 0){
				if(itBankAdd != m_BalanceData.mapBanknotAdded.end()){

					strLine.Replace(_T("{ֽ�Ҳ���}"),CashValueToString((BankNoteAndCoinType_t)itBankAdd->first));
					strLine.Replace(_T("{��������}"),ComFormatCount(itBankAdd->second));
					strLine.Replace(_T("{������}"),ComFormatAmount(itBankAdd->second * FormatCashToINT(itBankAdd->first)));

					itBankAdd++;
				}
				else continue;
			}

			// ֽ�ҺϼƲ�����
			strLine.Replace(_T("{ֽ�Ҳ�����}"),ComFormatAmount(m_curPurchase.settlementData.lNoteSupplementSum));


			strLine.Replace(_T("{����ϼƽ��}"),ComFormatAmount(m_curPurchase.settlementData.lCoinSupplementSum + m_curPurchase.settlementData.lNoteSupplementSum));



			// ����
			if(strLine.Find(_T("{Ӳ�һ���}")) >= 0){
				if(itCoinColl != m_BalanceData.vecCoinCollectionBox.end()){
					strLine.Replace(_T("{Ӳ�һ���}"),CashValueToString(itCoinColl->value));
					strLine.Replace(_T("{��������}"),ComFormatCount(itCoinColl->nCount));
					strLine.Replace(_T("{���ս��}"),ComFormatAmount(itCoinColl->lAmount));

					itCoinColl++;
				}
				else continue;
			}


			strLine.Replace(_T("{Ӳ�һ��ս��}"),ComFormatAmount(m_curPurchase.settlementData.lCoinRecoverSum));

			// ֽ�һ���
			if(strLine.Find(_T("{ֽ�һ���}")) >= 0)
			{
				if (itBkntRcyl != m_BalanceData.vecCashCollection.end() ) //ֽ�һ���
				{
					strLine.Replace(_T("{ֽ�һ���}"),CashValueToString(itBkntRcyl->value));
					strLine.Replace(_T("{ֽ�һ�������}"),ComFormatCount(itBkntRcyl->nCount));
					strLine.Replace(_T("{ֽ�һ��ս��}"),ComFormatAmount(itBkntRcyl->lAmount));
					itBkntRcyl++;
				}else
					continue;
			}
			strLine.Replace(_T("{ֽ�һ��ս��}"),ComFormatAmount(m_curPurchase.settlementData.lNoteRecoverSum));		// ֽ�һ��ս��ϼ�,��ԪΪ��λ

			strLine.Replace(_T("{���պϼƽ��}"),ComFormatAmount(m_curPurchase.settlementData.lCoinRecoverSum + m_curPurchase.settlementData.lNoteRecoverSum));



			// �ֽ�����
			if(m_BalanceData.vec_BanknoteRecyclerReservInfo.size() > 0){
				// ֽ���ֽ�����
				if(strLine.Find(_T("{ֽ��ѭ��������}")) >=0 ){
					if(iterBHReReserv != m_BalanceData.vec_BanknoteRecyclerReservInfo.end()){
						if(iterBHReReserv->type == RECYCLE_CHANGE_BOX){
							strLine.Replace(_T("{ֽ��ѭ��������}"),_T("ֽ��ѭ��������"));
						}
						strLine.Replace(_T("{ֽ��ѭ��������ID}"),iterBHReReserv->nBoxID);
					}
					else continue;
				}

				if(strLine.Find(_T("{ֽ��ѭ�����������}")) >= 0){
					if(iterBHReReserv != m_BalanceData.vec_BanknoteRecyclerReservInfo.end()){
						strLine.Replace(_T("{ֽ��ѭ�����������}"),_T(""));
						strLine.Replace(_T("{����}"),ComFormatCount(iterBHReReserv->amount));
						strLine.Replace(_T("{���}"),ComFormatAmount((int)(iterBHReReserv->value)));
						iterBHReReserv++;
					}
					else continue;
				}
			}

			if(m_BalanceData.vec_BanknoteChangeReservInfo.size() > 0){
				// ֽ���ֽ�����
				if(strLine.Find(_T("{ֽ��������}")) >=0 ){
					if(iterBHReserv != m_BalanceData.vec_BanknoteChangeReservInfo.end()){
						if(iterBHReserv->type == CHANGE_BOX_A){
							strLine.Replace(_T("{ֽ��������}"),_T("ֽ��������"));
						}
						else if(iterBHReserv->type == CHANGE_BOX_B){
							strLine.Replace(_T("{ֽ��������}"),_T("ֽ��������B"));
						}
						strLine.Replace(_T("{ֽ��������ID}"),iterBHReserv->nBoxID);
					}
					else continue;
				}

				if(strLine.Find(_T("{���������}")) >= 0){
					if(iterBHReserv != m_BalanceData.vec_BanknoteChangeReservInfo.end()){
						strLine.Replace(_T("{���������}"),CashValueToString(iterBHReserv->value));
						strLine.Replace(_T("{����}"),ComFormatCount(iterBHReserv->amount));
						strLine.Replace(_T("{���}"),ComFormatAmount(iterBHReserv->amount * FormatCashToINT(iterBHReserv->value)));
						iterBHReserv++;
					}
					else continue;
				}
			}
			
			if(m_BalanceData.vec_BanknoteCollectionReservInfo.size() > 0){
				if(strLine.Find(_T("{������}")) >= 0){
					strLine.Replace(_T("{������}"),_T("ֽ�һ�����"));
					strLine.Replace(_T("{ֽ�һ�����ID}"),theBANKNOTE_COUNT.GetBanknoteboxInfo().banknoteBoxID.ToString());
				}

				if(strLine.Find(_T("{ֽ������}")) >= 0){
					if(iterBHCollBoxReserv != m_BalanceData.vec_BanknoteCollectionReservInfo.end()){
						strLine.Replace(_T("{ֽ������}"),CashValueToString(iterBHCollBoxReserv->value));
						strLine.Replace(_T("{ֽ����������}"),ComFormatCount(iterBHCollBoxReserv->nCount));
						strLine.Replace(_T("{ֽ��������}"),ComFormatAmount(iterBHCollBoxReserv->lAmount));
						iterBHCollBoxReserv++;
					}
					else continue;
				}
			}
			
			strLine.Replace(_T("{ֽ������ϼ�}"),ComFormatAmount(m_curPurchase.settlementData.lNoteCurrentLeftSum));

			// Ӳ��������Ϣ
			if(m_BalanceData.vec_CoinReservInfo.size() > 0){
				bool bFindReplace = false;
				if(strLine.Find(_T("{Ӳ��������}")) >= 0){
					auto iterTemp = m_BalanceData.vec_CoinReservInfo.begin();
					while(iterTemp != m_BalanceData.vec_CoinReservInfo.end()){
						if(iterTemp->type == CHANGE_BOX_A){
							strLine.Replace(_T("{Ӳ��������}"),_T("Ӳ��������A"));
							strLine.Replace(_T("{Ӳ����ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{���}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{����}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}

						if(iterTemp->type == CHANGE_BOX_B){
							strLine.Replace(_T("{Ӳ��������}"),_T("Ӳ��������B"));
							strLine.Replace(_T("{Ӳ����ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{���}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{����}"),ComFormatCount(iterTemp->amount));
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

				if(strLine.Find(_T("{ѭ��������}")) >= 0){

					auto iterTemp = m_BalanceData.vec_CoinReservInfo.begin();
					while(iterTemp != m_BalanceData.vec_CoinReservInfo.end()){
						if(iterTemp->type == CYCLE_BOX_A){
							strLine.Replace(_T("{ѭ��������}"),_T("ѭ��������A"));
							strLine.Replace(_T("{Ӳ����ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{���}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{����}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}

						if(iterTemp->type == CYCLE_BOX_B){
							strLine.Replace(_T("{ѭ��������}"),_T("ѭ��������B"));
							strLine.Replace(_T("{Ӳ����ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{���}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{����}"),ComFormatCount(iterTemp->amount));
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

				if(strLine.Find(_T("{Ӳ�һ�����}")) >= 0){
					auto iterTemp = m_BalanceData.vec_CoinReservInfo.begin();
					while(iterTemp != m_BalanceData.vec_CoinReservInfo.end()){
						if(iterTemp->type == COLLECTION_BOX_A){
							strLine.Replace(_T("{Ӳ�һ�����}"),_T("Ӳ�һ�����"));
							strLine.Replace(_T("{Ӳ����ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{���}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{����}"),ComFormatCount(iterTemp->amount));
							m_BalanceData.vec_CoinReservInfo.erase(iterTemp);
							bFindReplace = true;
							break;
						}

						if(iterTemp->type == COLLECTION_BOX_B){
							strLine.Replace(_T("{Ӳ�һ�����}"),_T("Ӳ�һ�����B"));
							strLine.Replace(_T("{Ӳ����ID}"),iterTemp->nBoxID);
							strLine.Replace(_T("{���}"),CashValueToString(iterTemp->value));
							strLine.Replace(_T("{����}"),ComFormatCount(iterTemp->amount));
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
			// �޳���Щû�������
			else{
				if(strLine.Find(_T("{Ӳ��������}")) >= 0 || strLine.Find(_T("{ѭ��������}")) >= 0 || strLine.Find(_T("{Ӳ�һ�����}")) >= 0	){
					continue;
				}
			}
			
			strLine.Replace(_T("{Ӳ������ϼ�}"),ComFormatAmount(m_curPurchase.settlementData.lCoinCurrentLeftSum));

			//���Ͻ���
			strLine.Replace(_T("{����ʧ������}"),ComFormatCount(m_BalanceData.mapException[CARD_FAILURE].first));
			strLine.Replace(_T("{����ʧ�ܽ��}"),ComFormatAmount(m_BalanceData.mapException[CARD_FAILURE].second));
			//strLine.Replace(_T("{��Tokenʧ����}"),ComFormatCount(m_BalanceData.mapException[TOKEN_FAILURE].first));
			//strLine.Replace(_T("{��Tokenʧ�ܽ��}"),ComFormatAmount(m_BalanceData.mapException[TOKEN_FAILURE].second));

			strLine.Replace(_T("{����ʧ������}"),ComFormatCount(m_BalanceData.mapException[COIN_CHANGE_FAILURE].first));
			strLine.Replace(_T("{����ʧ�ܽ��}"),ComFormatAmount(m_BalanceData.mapException[COIN_CHANGE_FAILURE].second));
			

			strLine.Replace(_T("{���������ϼ�}"),ComFormatCount(m_BalanceData.nSumExceptionCount));
			strLine.Replace(_T("{���Ͻ��ϼ�}"),ComFormatAmount(m_BalanceData.nSumExceptionAmount));

			CString strExchange(_T(""));
			strLine.Replace(_T("{����ʱ��}"),ComGetCurTime().ToString(_T("%04d-%02d-%02d %02d:%02d:%02d")));
			strExchange.Format(_T("%d"),m_BalanceData.nOperatorID);
			strLine.Replace(_T("{����Ա���}"),theAPP_SESSION.GetUserInfo().sUserID);
			strLine.Replace(_T("{��վ����_CN}"), theAPP_SESSION.GetStationName(LANGUAGE_ID_CHINESE));
			strLine.Replace(_T("{��վ����_EN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_ENGLISH));
			strLine.Replace(_T("{��վ����_PN}"),theAPP_SESSION.GetStationName(LANGUAGE_ID_PORTUGUESE));
			strLine.Replace(_T("{�豸���}"),theMAINTENANCE_INFO.GetCurrentDevice().ToString());

			//�����ݴ洢��ƾ֤��

			CString strSave = _T("");
			if (strLine.GetLength()>0)
			{
				strSave = strLine.Left(strLine.GetLength() - 1);
			}
			saReception.Add(strSave);//ȥ��/r

			//�����ݸ�ʽ��Ϊ��ӡ��Helper��Ҫ�ĸ�ʽ
			CStringArray printLines;
			ComPrintLineFormatForTemplate(strLine,printLines);
			saPrintText.Append(printLines);
		}
		theAPP_SESSION.SetReceiptPrintType(RECEIPT_PRINT_TYPE::PRINT_TYPE_BALANCE);
		//����ҵ�ᵥ���ݵ����أ��ṩ��ƾ֤������ʹ��
		PRINTER_HELPER->SaveToPrintHistoryFile(&saReception);
		
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�ս��ļ�����ɹ���"));

		// ���ô�ӡ����ӡҵ�ᵥ����
		long ret = 0;
		if(bIsPrint){
			ret  = PRINTER_HELPER->PrintReceiptByText(&saPrintText,false,true);
		}
		
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�ս��ļ���ӡ�ɹ���"));
		return ret;
	}
	catch(CSysException& e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�ս��ļ���ӡʧ�ܣ�"));
	}

	//theSERVICE_MGR.SetForeServiceBusy(false);
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�ս��ļ���ӡʧ�ܣ�"));
	// ʧ��
	return 1;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief		����Ʒ����תΪCString��ʽ 

@param      ProductType& prodtype�� ��Ʒ����

@retval     CString��ʽ�Ĳ�Ʒ����

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
@brief		����ֵתΪCString��ʽ

@param      BankNoteAndCoinType_t& value

@retval     CString��ʽ����ֵ

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CString CAuditHelper::CashValueToString(BankNoteAndCoinType_t& value)
{
	CString strValue;
	switch(value)
	{
	case Coin1Yuan:strValue = _T("1Ԫ");break;
		//case Coin2MOP:strValue = _T("  2MOP");break;
		//case Coin5MOP:strValue = _T("  5MOP");break;
	case CoinHalfYuan:strValue = _T("5��");break;
	case Banknote1Yuan:strValue = _T("  1Ԫ");break;
	case Banknote5Yuan:strValue = _T("  5Ԫ");break;
	case Banknote10Yuan:strValue = _T(" 10Ԫ");break;
	case Banknote20Yuan:strValue = _T(" 20Ԫ");break;
	case Banknote50Yuan:strValue = _T(" 50Ԫ");break;
	case Banknote100Yuan:strValue = _T("100Ԫ");break;
	default: break;
	}
	return strValue;
}

// ���ֽ���ֵתΪint��ʽ��������
// ��λ�� ��
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

// ��ͺ���������������ͳ��
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

// ��ͺ������������ֽ����
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

// ��ͺ�����������Ǯ�����
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
@brief      ת��Ӳ������Ϊֵ

@param      int valueӲ������

@retval     DWORD ��λAVOS

@exception  ��
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
@brief      ����AVM��TVM���ݵ�������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CAuditHelper::SendTVMAuditData(){
	try{
		// ��ʼʱ��
		m_curPurchase.auditTimeFrom = m_BalanceData.dtOriginTime;
		m_curPurchase.auditTimeTo   = m_BalanceData.dtCurrentTime;
		// �ۿ��ܽ�����ֵ�ܽ����ͳ��

		// ���Ͻ������������
		m_curPurchase.settlementData.lFailBusinessNum = m_BalanceData.nSumExceptionCount;
		m_curPurchase.settlementData.lFailBusinessSum = m_BalanceData.nSumExceptionAmount;

		// ------------------------------Ӳ�����------------------------------------
		CTCoinCountInfo::COIN_BOX_INFO hopperA = theCOIN_COUNT.GetCoinHopperboxAInfo();
		CTCoinCountInfo::COIN_BOX_INFO hopperB = theCOIN_COUNT.GetCoinHopperboxBInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeboxA = theCOIN_COUNT.GetChangeCoinboxAInfo();
		CTCoinCountInfo::COIN_CHANGE_BOX_INFO changeboxB = theCOIN_COUNT.GetChangeCoinboxBInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxA = theCOIN_COUNT.GetCollectionCoinboxAInfo();
		CTCoinCountInfo::COIN_COLLECTION_BOX_INFO collectionBoxB = theCOIN_COUNT.GetCollectionCoinboxBInfo();
		// Ӳ���ϴ�����
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

		// ����Ӳ�ҽ��(�Ѿ�ͳ��)
		// �ۼƻ��ս��Ѿ�ͳ�ƣ�
		// �ۼ�����Ӳ�ҽ���ͳ�ƣ�
		// �ۼ��������ͳ�ƣ�
		// ��������Ӳ�ҽ���ͳ�ƣ�

		theCOIN_COUNT.SetCoinHopperboxAInfo(hopperA);
		theCOIN_COUNT.SetCoinHopperboxBInfo(hopperB);
		theCOIN_COUNT.SetChangeCoinboxAInfo(changeboxA);
		theCOIN_COUNT.SetChangeCoinboxBInfo(changeboxB);
		theCOIN_COUNT.SetCollectionCoinboxAInfo(collectionBoxA);
		theCOIN_COUNT.SetCollectionCoinboxBInfo(collectionBoxB);

		//--------------------------------ֽ�����--------------------------------------
		CTBanknoteCountInfo::BANKNOTE_BOX_INFO banknoteBox = theBANKNOTE_COUNT.GetBanknoteboxInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_a = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxAInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_b = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxBInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_c = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxCInfo();
		CTBanknoteCountInfo::BANKNOTE_RECYCLER_BOX_INFO recycler_box_d = theBANKNOTE_COUNT.GetBanknoteRecyclerBoxDInfo();

		// ֽ���ϴ�����
		m_curPurchase.settlementData.lNoteLastLeftSum  = banknoteBox.ulLastLeftAmount + 
			recycler_box_a.ulLastLeftAmount + recycler_box_b.ulLastLeftAmount + 
			recycler_box_c.ulLastLeftAmount + recycler_box_d.ulLastLeftAmount;

		// ���䣨��ͳ�ƣ�
		// ���գ���ͳ�ƣ�
		// ���루��ͳ�ƣ�
		// ���㣨��ͳ�ƣ�
		// �������棨��ͳ�ƣ�

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

		//-------------------------------Ʊ�����---------------------------------------
		//CTCardCountInfo::CARD_BOX_INFO cardBoxA = theCARD_COUNT.GetCardboxAInfo();
		//CTCardCountInfo::CARD_BOX_INFO cardBoxB = theCARD_COUNT.GetCardboxBInfo();
		//CTCardCountInfo::CARD_BOX_INFO cardWasted = theCARD_COUNT.GetCardboxWastedInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardBoxA = theTOKEN_COUNT.GetHopperAInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardBoxB = theTOKEN_COUNT.GetHopperBInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardWasted = theTOKEN_COUNT.GetTokenboxWastedInfo();
		CTTokenCountInfo::TOKEN_BOX_INFO cardCollected = theTOKEN_COUNT.GetTokenboxCollectedInfo();

		// �ϴ�����
		m_curPurchase.settlementData.lCardLastLeftNum = cardBoxA.ulInitCount + cardBoxB.ulInitCount + cardWasted.ulInitCount;
		// ���η�Ʊ��
		m_curPurchase.settlementData.lCardCurrentInvalidateNum += cardWasted.ulCurCount;
		// ���β����ۼ�(��ͳ��)
		// �����ۼ��۳�
		m_curPurchase.settlementData.lCardCurrentSelledNum = m_curPurchase.settlementData.lSelledCardNum;
		// ���λ���(��ͳ��)
		// ��������
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

		// ����ҵ���������
		theSERVICE_MGR.GetService<CEventDataSvc>(EVENT_MSG_SVC)->SendTVMAuditData(m_curPurchase);

		// ����Ϊ�ѷ���
		theSCAudit_MGR.SetDayPurchaseSended(m_curPurchase.businessDate);

		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�ս��ļ��ѷ�������λ��"));
	}
	catch(CSysException e){
		theEXCEPTION_MGR.ProcessException(e);
	}
	catch(...){
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("�ս��ļ����÷���ʧ�ܡ�"));
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
@brief      ����ֽ����ֵ���� BIN	1��1Ԫ��2��5Ԫ��3��10Ԫ��4��20Ԫ��5��50Ԫ��6��100Ԫ

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
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
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("ֽ�������Ϊ��Ч������countInfo��¼��������"));
		return VALUE_UNKNOWN;
	}
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ����Ӳ����ֵ���� BIN	0����Ч��1��0.5Yuan��2��1Yuan

@param      ��

@retval     ֽ�ҽ��ܽ�� �Է�Ϊ��λ

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
BankNoteAndCoinType_t CAuditHelper::TransCoinType(int value) {
	switch (value) {
	case 1: return CoinHalfYuan;
	case 2: return Coin1Yuan;
	default: 
		theEXCEPTION_LOG.WriteData(CExceptionLog::LEVEL_WARNING,0,_T(__FILE__),__LINE__,_T("Ӳ�������Ϊ��Ч������countInfo��¼��������"));
		return VALUE_UNKNOWN;
	}
}