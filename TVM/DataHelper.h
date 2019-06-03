#pragma once
#include "stdafx.h"
#include <vector>
#include "ServiceHelper.h"
#include "TransactionInfo.h"
#include "CoinAddOperationRecord.h"
#include "CoinCollectOperationRecord.h"
#include "CoinCollectBoxRemoveOperationRecord.h"
#include "CoinBoxCashDetail.h"
#include "CoinChangeBoxOperationRecord.h"
#include "BanknoteOperationRecord.h"
#include "BanknoteBoxInstallOperationRecord.h"
#include "BanknoteBoxRemoveOperationRecord.h"
#include "BanknoteBoxCashDetail.h"
#include "TokenBoxAddOperationRecord.h"
#include "TokenBoxInstallOperationRecord.h"
#include "TokenBoxRemoveOperationRecord.h"
#include "TokenCollectBoxRemoveOperationRecord.h"
#include "TokenCollectOperationRecord.h"
#include "CardBoxInstallOperationRecord.h"
#include "CardBoxRemoveOperationRecord.h"
#include "CardCollectBoxRemoveOperationRecord.h"

#define  TASK_ID_TASK_TYPE_BEGIN_NO			8		// ���������������͵���ʼλ
#define  TASK_ID_TASK_TYPE_LENGTH			2		// ���������������͵ĳ���
#define  TEMAPLE_ID_TEMAPLE_TYPE_BEGIN_NO	8		// ���������������͵���ʼλ
#define  TEMAPLE_ID_TEMAPLE_TYPE_LENGTH		2		// ���������������͵ĳ���
#define  TIME_BEGIN_NO						0		// �����Ż�ģ������ʱ�����ʼλ
#define  TIME_LENGTH						8		// �����Ż�ģ������ʱ��ĳ���
#define  DATABASE_SQL_FILE_NAME		    _T("TVM.sql")// SQL�ű��ļ�����

using namespace Data;
class CDataHelper
{
public:
	// ----------------------------------�˿�ҵ�����ݿ�����ӿڶ���-----------------------------------
	// ���潻����Ϣ
	long DbInsertTransactionInfo(TransactionRecord &transactionInfo);
	// ��ѯָ��ʱ��εĽ�����Ϣ
	long DbSelectTransactionInfo(CString beginTime,CString endTime, vector<TransactionRecord>& vec_transactionInfo,bool bSelectDetail = true);
	// ��ѯ�����N�����׼�¼�����ݽ���ʱ�䣩
	long DbSelectTopNumTransInfo(vector<TransactionRecord>& vec_transactionInfo,long lNum=100);
	// ��ѯָ��ʱ��ε�Ӳ�����ɼ�¼
	long DbSelectCoinAcceptRecord(CString beginTime,CString endTime, vector<CoinAcceptRecord>& vec_CoinAcceptRecord);
	// ��ѯָ��ʱ��ε�Ӳ�������¼
	long DbSelectCoinChangeRecord(CString beginTime,CString endTime, vector<CoinChangeRecord>& vec_CoinChangeRecord);
	// ��ѯָ��ʱ��ε�ֽ�����ɼ�¼
	long DbSelectBanknoteAcceptRecord(CString beginTime,CString endTime, vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord);
	// ��ѯָ��ʱ��ε�ֽ�������¼
	long DbSelectBanknoteChangeRecord(CString beginTime,CString endTime, vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord);
	// ��ѯָ��ʱ��εĹ��ϼ�¼
	long DbSelectTransFailureRecord(CString beginTime,CString endTime, vector<ExceptionRecord>& vec_TransFailureRecord);
	// ɾ��N��ǰ�Ľ�����Ϣ

	// ɾ��N��ǰ�Ľ�����Ϣ
	long DbDeleteTransactionInfoBeforeNdays(WORD dwDays);
	// ɾ��ָ��ʱ��εĽ�����Ϣ��Ԥ�����ݲ�ʹ�ã�
	long DbDeleteTransactionInfo(CString beginTime,CString endTime);

    // ----------------------------------վԱҵ�����ݿ�����ӿڶ���-----------------------------------
	// ����Ӳ�������䲹�������¼
	long DbInsertCoinAddOperationRecord(CoinAddOperationRecord &coinAddOperationRecord);
	// ����Ӳ�һ��ղ�����¼
	long DbInsertCoinCollectOperationRecord(CoinCollectOperationRecord &coinCollectOperationRecord);
	// ����Ӳ�һ�����ж�ز�����¼
	long DbInsertCoinCollectBoxRemoveOperationRecord(CoinCollectBoxRemoveOperationRecord &coinCollectBoxRemoveOperationRecord);

	// ��ѯָ��ʱ��ε�Ӳ��Ͷ����Ϣ
	long DbSelectCoinInsertRecord(CString beginTime,CString endTime,vector<CoinInsertRecord>& vec_CoinInsertRecord);

	// ��ѯָ��ʱ��ε�Ӳ�������䲹�������¼
	long DbSelectCoinAddOperationRecord(CString beginTime,CString endTime, vector<CoinAddOperationRecord>& vec_CoinAddOperationRecord);
	// ��ѯָ��ʱ��ε�Ӳ�һ��ղ�����¼
	long DbSelectCoinCollectOperationRecord(CString beginTime,CString endTime, vector<CoinCollectOperationRecord>& vec_CoinCollectOperationRecord);
	// ��ѯָ��ʱ��ε�Ӳ�һ�����ж�ز�����¼
	long DbSelectCoinCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CoinCollectBoxRemoveOperationRecord>& vec_CoinCollectBoxRemoveOperationRecord);
	// ɾ��N������ǰ������Ӳ�Ҳ�����¼
	long DbDeleteCoinOperationRecordBeforeNdays(WORD dwDays);

	// ��ѯָ��ʱ���Ӳ�Ҳ����������¼
	long DbSelectCoinChangeBoxOperationRecord(CString beginTime,CString endTime,vector<CoinChangeBoxOperationRecord>& vec_coinChangeboxOperationRecord);
	// ����Ӳ�������������¼
	long DbInsertCoinChangeBoxOperationRecord(CoinChangeBoxOperationRecord& coinChangeBoxOperationRecord);

	// ����ֽ�������䲹�������¼
	long DbInsertBanknoteOperationRecord(BanknoteOperationRecord &banknoteAddOperationRecord);
	// ����ֽ�һ����䰲װ������¼
	long DbInsertBanknoteBoxInstallOperationRecord(BanknoteBoxInstallOperationRecord &banknoteBoxInstallOperationRecord);
	// ����ֽ��ж�ػ��ղ�����¼
	long DbInsertBanknoteBoxRemoveOperationRecord(BanknoteBoxRemoveOperationRecord &banknoteBoxRemoveOperationRecord);
	// ��ѯָ��ʱ��ε�ֽ�������䲹�������¼
	long DbSelectBanknoteOperationRecord(CString beginTime,CString endTime, vector<BanknoteOperationRecord>& vec_BanknoteAddOperationRecord);
	// ��ѯָ��ʱ��ε�ֽ�һ����䰲װ������¼
	long DbSelectBanknoteBoxInstallOperationRecord(CString beginTime,CString endTime, vector<BanknoteBoxInstallOperationRecord>& vec_BanknoteBoxInstallOperationRecord);
	// ��ѯָ��ʱ��ε�ֽ��ж�ػ��ղ�����¼
	long DbSelectBanknoteBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<BanknoteBoxRemoveOperationRecord>& vec_BanknoteBoxRemoveOperationRecord);
	// ɾ��N������ǰ������ֽ�Ҳ�����¼
	long DbDeleteBanknoteOperationRecordBeforeNdays(WORD dwDays);

	// ����Token�䰲װ������¼(Ԥ��)
	long DbInsertTokenBoxInstallOperationRecord(TokenBoxInstallOperationRecord &tokenBoxInstallOperationRecord);
	// ����Token��ж�ز�����¼(Ԥ��)
	long DbInsertTokenBoxRemoveOperationRecord(TokenBoxRemoveOperationRecord &tokenBoxRemoveOperationRecord);
	// ����Token�䲹�������¼
	long DbInsertTokenBoxAddOperationRecord(TokenBoxAddOperationRecord &tokenBoxAddOperationRecord);
	// ����Token���ղ�����¼
	long DbInsertTokenCollectOperationRecord(TokenCollectOperationRecord &tokenBoxAddOperationRecord);
	// ����Token��Ʊ��ж�ز�����¼
	long DbInsertTokenCollectBoxRemoveOperationRecord(TokenCollectBoxRemoveOperationRecord &tokenCollectBoxRemoveOperationRecord);
	// ��ѯָ��ʱ��ε�Token�䰲װ������¼(Ԥ��)
	long DbSelectTokenBoxInstallOperationRecord(CString beginTime,CString endTime, vector<TokenBoxInstallOperationRecord>& vec_TokenBoxInstallOperationRecord);
	// ��ѯָ��ʱ��ε�Token��ж�ز�����¼(Ԥ��)
	long DbSelectTokenBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<TokenBoxRemoveOperationRecord>& vec_TokenBoxRemoveOperationRecord);
	// ��ѯָ��ʱ��ε�Token�䲹�������¼
	long DbSelectTokenBoxAddOperationRecord(CString beginTime,CString endTime, vector<TokenBoxAddOperationRecord>& vec_TokenBoxAddOperationRecord);
	// ��ѯָ��ʱ��ε�Token���ղ�����¼
	long DbSelectTokenCollectOperationRecord(CString beginTime,CString endTime, vector<TokenCollectOperationRecord>& vec_TokenCollectOperationRecord);
	// ��ѯָ��ʱ��ε�Token��Ʊ��ж�ز�����¼
	long DbSelectTokenCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<TokenCollectBoxRemoveOperationRecord>& vec_TokenCollectBoxRemoveOperationRecord);
	// ɾ��N������ǰ������Token������¼
	long DbDeleteTokenOperationRecordBeforeNdays(WORD dwDays);


	// ����Card�䰲װ������¼
	long DbInsertCardBoxInstallOperationRecord(CardBoxInstallOperationRecord &cardBoxInstallOperationRecord);
	// ����Card��ж�ز�����¼
	long DbInsertCardBoxRemoveOperationRecord(CardBoxRemoveOperationRecord &cardBoxRemoveOperationRecord);
	// ����Card��Ʊ��ж�ز�����¼
	long DbInsertCardCollectBoxRemoveOperationRecord(CardCollectBoxRemoveOperationRecord &cardCollectBoxRemoveOperationRecord);
	// ��ѯָ��ʱ��ε�Card�䰲װ������¼
	long DbSelectCardBoxInstallOperationRecord(CString beginTime,CString endTime, vector<CardBoxInstallOperationRecord>& vec_CardBoxInstallOperationRecord);
	// ��ѯָ��ʱ��ε�Card��ж�ز�����¼
	long DbSelectCardBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CardBoxRemoveOperationRecord>& vec_CardBoxRemoveOperationRecord);
	// ��ѯָ��ʱ��ε�Card��Ʊ��ж�ز�����¼
	long DbSelectCardCollectBoxRemoveOperationRecord(CString beginTime,CString endTime, vector<CardCollectBoxRemoveOperationRecord>& vec_CardCollectBoxRemoveOperationRecord);
	
	// ɾ��N������ǰ������Card������¼
	long DbDeleteCardOperationRecordBeforeNdays(WORD dwDays);


public:
	CDataHelper();                                 // ���캯��
	~CDataHelper();                                                 // ��������
	long DbInitialize();											// �ú���ִ�У����ݿ��ļ��Ķ��ػ�����������ݿ��ļ��ͱ������ݿ��ļ��������ڣ�����Ҫִ�д������ݿⲢ��ʼ�����ݡ����ػ��ɹ�����Ҫ�����ݿ�����ӡ�
	//û�е���
	//long DbClose();												// �ر����ݿ�����ӡ�


private:
	// ----------------------------------�˿�ҵ�����ݿ�����ӿڶ���-----------------------------------
	// ��ѯָ�����׵�Ӳ��Ͷ�������ϸ
	long DbSelectCoinInsertRecord(int  iTransID, vector<CoinInsertRecord>& vec_CoinInsertRecord);
	// ��ѯָ�����׵�Ӳ�����������ϸ
	long DbSelectCoinChangeRecord(int iTransID, vector<CoinChangeRecord>& vec_CoinChangeRecord);
	// ��ѯָ�����׵�Ӳ�����ɲ�����ϸ
	long DbSelectCoinAcceptRecord(int iTransID, vector<CoinAcceptRecord>& vec_CoinAcceptRecord);
	// ��ѯָ�����׵�ֽ��Ͷ�������ϸ
	long DbSelectBanknoteInsertRecord(int iTransID, vector<BanknoteInsertRecord>& vec_BanknoteInsertRecord);
	// ��ѯָ�����׵�ֽ�����������ϸ��Ԥ����
	long DbSelectBanknoteChangeRecord(int iTransID, vector<BanknoteChangeRecord>& vec_BanknoteChangeRecord);
	// ��ѯָ�����׵�ֽ�����ɲ�����ϸ
	long DbSelectBanknoteAcceptRecord(int iTransID, vector<BanknoteAcceptRecord>& vec_BanknoteAcceptRecord);
	// ��ѯָ�����׵�ֽ�ҷϱҲ�����ϸ��Ԥ����
	long DbSelectBanknoteDiscardRecord(int iTransID, vector<BanknoteDiscardRecord>& vec_BanknoteDiscardRecord);
	// ��ѯָ�����׵Ĺ�����ϸ
	long DbSelectExceptionRecord(int iTransID, vector<ExceptionRecord>& vec_ExceptionRecord);
	// ��ѯָ�����׵�Ʊ��������ϸ
	long DbSelectTicketOperationRecord(int iTransID, vector<TicketOperationRecord>& vec_TicketOperationRecord);

	public:
    // ----------------------------------վԱҵ�����ݿ�����ӿڶ���-----------------------------------
	// ��ѯӲ�һ������ֽ���ϸ
	long DbSelectCoinBoxCashDetail(int iRecordID, vector<CoinBoxCashDetail>& vec_CoinBoxCashDetail);
	// ��ѯֽ�һ������ֽ���ϸ
	long DbSelectBanknoteBoxCashDetail(int iRecordID, vector<BanknoteBoxCashDetail>& vec_BanknoteBoxCashDetail);
};