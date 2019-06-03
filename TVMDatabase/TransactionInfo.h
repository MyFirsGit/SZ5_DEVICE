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
	@brief     交易信息主类
	*/
	class TransactionDetail
	{
	public:
		int iTransID;				//交易编号
		CString iUDSN;					//交易起始序列号
		_DATE_TIME dtTransTime;			//交易时间  //	DATETIME??
		int iUdType;					//交易类型
		int iUdSubtype;					//交易子类型
		int iCardType;					//票卡类型
		int iProductType;				//产品类型
		int iProductSubType;			//产品子类型
		int iOrigin;					//起始站点
		int iDestination;				//目的站点
		int DepositAmount;				//押金单价
		int TaxAmount;					//税费单价
		int CardFeeAmount;				//成本费
		int iUnitAmount;				//交易单价
		int iQuantity;					//交易数量
		int iPaymentMethod;				//支付方式
		int iTotalAmount;				//应收总金额
		int CoinInsertAmount;			//硬币投入金额	
		int CoinState;					//硬币收纳状态
		int BankNoteAcceptAmount;		//纸币投入金额
		int BanknoteAcceptState;		//纸币收纳状态
		int iChangeAmount;				//应找零金额
		int iCoinChangeAmount;			//硬币找零金额
		int iCoinChangeState;			//硬币找零状态
		int iBanknoteChangeAmount;		//纸币找零金额
		int iBanknoteChangeState;		//纸币找零状态	
		int iUnReleaseedMediaQuantity;	//已放出票卡张数	
		int iPaymentState;				//支付状态	
		CString iPayCardNo;				//支付卡号	
		int iPrintState;				//打印状态	
		int iState;						//交易状态	
		CString txtReserve;				//预留
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    拼接SQL插入语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL更新语句

		@param

		@retval   CString SQL语句   

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
		@brief    拼接SQL删除语句

		@param

		@retval   CString SQL语句   

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
		@brief    拼接SQL删除语句,不带有where语句

		@param

		@retval   CString SQL语句   

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
		@brief    拼接SQL查询语句

		@param

		@retval  CString SQL语句 

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
		@brief    填充实体

		@param	  sqlite3_stmt* stmt 结果集

		@retval   long 0：执行成功，非0：执行失败

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
	@brief     交易信息实体类
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