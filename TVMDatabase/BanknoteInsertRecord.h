#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	

	/**
	@brief		纸币投入操作明细实体类
	*/
	class BanknoteInsertRecord
	{
	public:
		int iTransID;		//交易编号
		_DATE_TIME dtOperationTime;	//交易时间
		int iCoinTypeCode;		//币种
		int iQuantity;			//数量
		CString txtReserve;		//预留
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
			strSql.Format(_T("insert into tb_BanknoteInsertRecord(iTransID,\
						 dtOperationTime,	\
						 iCoinTypeCode,	\
						 iQuantity,	\
						 txtReserve)		\
						 values('%d', '%s', '%d', '%d', '%s')"),
						 iTransID,
						 dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						 iCoinTypeCode, 
						 iQuantity, 
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
			strSql.Format(_T("update tb_BanknoteInsertRecord set \
						  dtOperationTime='%s',	\
						  iCoinTypeCode='%d',		\
						  iQuantity='%d',			\
						  txtReserve='%s' where \
						  iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iCoinTypeCode,
						  iQuantity, 
						  txtReserve,
						  iTransID);
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
			strSql.Format(_T("delete from tb_BanknoteInsertRecord where iTransID='%d'"),iTransID);
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
			strSql.Format(_T("delete from tb_BanknoteInsertRecord "));
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
							 dtOperationTime,		\
							 iCoinTypeCode,		\
							 iQuantity,		\
							 txtReserve from tb_BanknoteInsertRecord");
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
			dtOperationTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1)));
			iCoinTypeCode = sqlite3_column_int(stmt,2);
			iQuantity = sqlite3_column_int(stmt,3);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			return 0;
		}
	};
}