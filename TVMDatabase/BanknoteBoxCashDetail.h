#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     纸币回收箱现金明细实体类
	*/
	class BanknoteBoxCashDetail
	{
	public:
		int iID;					//记录编号
		int iBanknoteTypeCode;				//币种		
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
			strSql.Format(_T("insert into tb_BanknoteBoxCashDetail(\
						  iID,			\
						  iBanknoteTypeCode,		\
						  iQuantity,		\
						  txtReserve)values('%d','%d','%d','%s')"),
						  iID,	
						  iBanknoteTypeCode,	
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
			strSql.Format(_T("update tb_BanknoteBoxCashDetail set \
						  iBanknoteTypeCode='%d',		\
						  iQuantity='%d',	\
						  txtReserve='%s' where iID='%d'"),
						  iBanknoteTypeCode,
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
			strSql.Format(_T("delete from tb_BanknoteBoxCashDetail  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_BanknoteBoxCashDetail "));
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
							 iBanknoteTypeCode,		 \
							 iQuantity,	\
							 txtReserve from tb_BanknoteBoxCashDetail");
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
			iBanknoteTypeCode  = sqlite3_column_int(stmt,1);
			iQuantity		 = sqlite3_column_int(stmt,2);
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));

			return 0;
		}
	};
}