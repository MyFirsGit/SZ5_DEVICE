#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     个性化任务——个人信息实体类
	*/
	class  CoinBoxCashDetail
	{
	public:
		int iID;					//记录编号
		int iCoinTypeCode;				//币种		
		int iQuantity;					//数量	
		CString txtReserve;				//预留	


	public:
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
			strSql.Format(_T("insert into tb_CoinBoxCashDetail(\
						  iID,			\
						  iCoinTypeCode,		\
						  iQuantity,		\
						  txtReserve)values('%d','%d','%d','%s')"),
						  iID,	
						  iCoinTypeCode,	
						  iQuantity,		
						  txtReserve);
			return strSql;
		}
		
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    拼接SQL更新语句

		@param

		@retval  CString SQL语句   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToUpdateSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("update tb_CoinBoxCashDetail set \
						  iCoinTypeCode='%d',		\
						  iQuantity='%d',	\
						  txtReserve='%s' where iID='%d'"),
						  iCoinTypeCode,
						  iQuantity,
						  txtReserve,
						  iID);
			return strSql;
		}


		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    拼接SQL删除语句

		@param

		@retval  CString SQL语句   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToDeleteSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("delete from tb_CoinBoxCashDetail  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_CoinBoxCashDetail "));
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
			CString strSql = _T("select iID,			 \
							 iCoinTypeCode,		 \
							 iQuantity,	\
							 txtReserve from tb_CoinBoxCashDetail");
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
			iID					 = sqlite3_column_int(stmt,0);
			iCoinTypeCode  = sqlite3_column_int(stmt,1);
			iQuantity		 = sqlite3_column_int(stmt,2);
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));

			return 0;
		}
	};
}