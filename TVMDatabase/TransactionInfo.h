#pragma once
#include "stdafx.h"
#include "sqlite3.h"
#include <vector>
#include "Util.h"
#include "CoinInsertRecord.h"
#include "CoinChangeRecord.h"
#include "CoinAcceptRecord.h"
#include "BanknoteInsertRecord.h"
#include "BanknoteChangeRecord.h"
#include "BanknoteAcceptRecord.h"
#include "BanknoteDiscardRecord.h"
#include "TicketOperationRecord.h"
#include "ExceptionRecord.h"


using namespace std;
namespace Data
{	
	/**
	@brief     ������Ϣ����
	*/
	class TransactionDetail
	{
	public:
		int iTransID;				//���ױ��
		CString iUDSN;					//������ʼ���к�
		_DATE_TIME dtTransTime;			//����ʱ��  //	DATETIME??
		int iUdType;					//��������
		int iUdSubtype;					//����������
		int iCardType;					//Ʊ������
		int iProductType;				//��Ʒ����
		int iProductSubType;			//��Ʒ������
		int iOrigin;					//��ʼվ��
		int iDestination;				//Ŀ��վ��
		int DepositAmount;				//Ѻ�𵥼�
		int TaxAmount;					//˰�ѵ���
		int CardFeeAmount;				//�ɱ���
		int iUnitAmount;				//���׵���
		int iQuantity;					//��������
		int iPaymentMethod;				//֧����ʽ
		int iTotalAmount;				//Ӧ���ܽ��
		int CoinInsertAmount;			//Ӳ��Ͷ����	
		int CoinState;					//Ӳ������״̬
		int BankNoteAcceptAmount;		//ֽ��Ͷ����
		int BanknoteAcceptState;		//ֽ������״̬
		int iChangeAmount;				//Ӧ������
		int iCoinChangeAmount;			//Ӳ��������
		int iCoinChangeState;			//Ӳ������״̬
		int iBanknoteChangeAmount;		//ֽ��������
		int iBanknoteChangeState;		//ֽ������״̬	
		int iUnReleaseedMediaQuantity;	//�ѷų�Ʊ������	
		int iPaymentState;				//֧��״̬	
		CString iPayCardNo;				//֧������	
		int iPrintState;				//��ӡ״̬	
		int iState;						//����״̬	
		CString txtReserve;				//Ԥ��
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ƴ��SQL�������

		@param

		@retval  CString SQL���   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToInsertSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("insert into tb_Transaction(iUDSN,	\
						 dtTransTime,	\
						 iUdType,	\
						 iUdSubtype,	\
						 iCardType,	\
						 iProductType, \
						 iProductSubType,	\
						 iOrigin,	\
						 iDestination,	\
						 DepositAmount,	\
						 TaxAmount,	\
						 CardFeeAmount,	\
						 iUnitAmount,\
						 iQuantity,	\
						 iPaymentMethod,	\
						 iTotalAmount,	\
						 CoinInsertAmount,	\
						 CoinState,	\
						 BankNoteAcceptAmount,	\
						 BanknoteAcceptState,	\
						 iChangeAmount,	\
						 iCoinChangeAmount,	\
						 iCoinChangeState,	\
						 iBanknoteChangeAmount,	\
						 iBanknoteChangeState,	\
						 iUnReleaseedMediaQuantity,	\
						 iPaymentState,	\
						 iPayCardNo,	\
						 iPrintState,	\
						 iState,	\
						 txtReserve)		\
						 values('%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%s', '%d', '%d', '%s')"),
						 iUDSN,
						 dtTransTime.ToString(_T("%04d%02d%02d%02d%02d%02d")),
						 iUdType,
						 iUdSubtype,
						 iCardType,
						 iProductType,
						 iProductSubType,
						 iOrigin,		
						 iDestination,
						 DepositAmount,				
						 TaxAmount,			
						 CardFeeAmount,				
						 iUnitAmount,			
						 iQuantity,				
						 iPaymentMethod,				
						 iTotalAmount,			
						 CoinInsertAmount,			
						 CoinState,		
						 BankNoteAcceptAmount,
						 BanknoteAcceptState,
						 iChangeAmount,
						 iCoinChangeAmount,
						 iCoinChangeState,
						 iBanknoteChangeAmount,
						 iBanknoteChangeState,
						 iUnReleaseedMediaQuantity,
						 iPaymentState,
						 iPayCardNo,
						 iPrintState,
						 iState,
						 txtReserve);
			return strSql;
		}
		

		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ƴ��SQL�������

		@param

		@retval   CString SQL���   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToUpdateSql()
		{
			CString strSql = _T("");
			return strSql;
		}

		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ƴ��SQLɾ�����

		@param

		@retval   CString SQL���   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToDeleteSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("delete from tb_Transaction where iTransID='%d'"),iTransID);
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ƴ��SQLɾ�����,������where���

		@param

		@retval   CString SQL���   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToDeleteSqlWithoutWhere()
		{
			CString strSql = _T("");
			strSql.Format(_T("delete from tb_Transaction "));
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ƴ��SQL��ѯ���

		@param

		@retval  CString SQL��� 

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToSelectSql()
		{
			CString strSql = _T("select iTransID, \
							 iUDSN,		\
							 dtTransTime,		\
							 iUdType,	\
							 iUdSubtype,		\
							 iCardType,		\
							 iProductType,	\
							 iProductSubType,	\
							 iOrigin,	\
							 iDestination,	\
							 DepositAmount,	\
							 TaxAmount,	\
							 CardFeeAmount,	\
							 iUnitAmount,	\
							 iQuantity,	\
							 iPaymentMethod,	\
							 iTotalAmount,	\
							 CoinInsertAmount,	\
							 CoinState,	\
							 BankNoteAcceptAmount,	\
							 BanknoteAcceptState,	\
							 iChangeAmount,	\
							 iCoinChangeAmount,	\
							 iCoinChangeState,	\
							 iBanknoteChangeAmount,	\
							 iBanknoteChangeState,	\
							 iUnReleaseedMediaQuantity,	\
							 iPaymentState,	\
							 iPayCardNo,	\
							 iPrintState,	\
							 iState,	\
							 txtReserve from tb_Transaction");
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ���ʵ��

		@param	  sqlite3_stmt* stmt �����

		@retval   long 0��ִ�гɹ�����0��ִ��ʧ��

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		long Fill(sqlite3_stmt* stmt)
		{
			iTransID = sqlite3_column_int(stmt,0);
			iUDSN = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1));
			dtTransTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2)));
			iUdType = sqlite3_column_int(stmt,3);
			iUdSubtype = sqlite3_column_int(stmt,4);
			iCardType = sqlite3_column_int(stmt,5);
			iProductType = sqlite3_column_int(stmt,6);
			iProductSubType = sqlite3_column_int(stmt,7);
			iOrigin = sqlite3_column_int(stmt,8);
			iDestination = sqlite3_column_int(stmt,9);
			DepositAmount = sqlite3_column_int(stmt,10);
			TaxAmount = sqlite3_column_int(stmt,11);
			CardFeeAmount = sqlite3_column_int(stmt,12);
			iUnitAmount = sqlite3_column_int(stmt,13);
			iQuantity = sqlite3_column_int(stmt,14);
			iPaymentMethod = sqlite3_column_int(stmt,15);
			iTotalAmount = sqlite3_column_int(stmt,16);
			CoinInsertAmount = sqlite3_column_int(stmt,17);
			CoinState = sqlite3_column_int(stmt,18);
			BankNoteAcceptAmount = sqlite3_column_int(stmt,19);
			BanknoteAcceptState = sqlite3_column_int(stmt,20);
			iChangeAmount = sqlite3_column_int(stmt,21);
			iCoinChangeAmount = sqlite3_column_int(stmt,22);
			iCoinChangeState = sqlite3_column_int(stmt,23);
			iBanknoteChangeAmount = sqlite3_column_int(stmt,24);
			iBanknoteChangeState = sqlite3_column_int(stmt,25);
			iUnReleaseedMediaQuantity = sqlite3_column_int(stmt,26);
			iPaymentState =sqlite3_column_int(stmt,27);
			iPayCardNo = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,28));
			iPrintState = sqlite3_column_int(stmt,29);
			iState = sqlite3_column_int(stmt,30);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,31));
			return 0;
		}
		
	};

	/**
	@brief     ������Ϣʵ����
	*/
	class TransactionRecord
	{
	public:
		TransactionDetail iTransactionDetail;
		vector<CoinInsertRecord> vTransCoinDetail;
		vector<CoinChangeRecord> vTransCoinChangeDetail;
		vector<CoinAcceptRecord> vTransCoinAcceptDetail;
		
		vector<BanknoteInsertRecord> vTransBanknoteDetail;

		vector<BanknoteChangeRecord> vTransBanknoteChangeDetail;
		vector<BanknoteAcceptRecord> vTransBanknoteAcceptDetail;
		vector<BanknoteDiscardRecord> vTransBanknoteDiscardDetail;


		vector<TicketOperationRecord> vCardDetail;
		vector<ExceptionRecord> vExceptionDetail;
	};
}