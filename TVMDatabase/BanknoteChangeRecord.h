#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     票箱操作日志实体类
	*/
	class BanknoteChangeRecord
	{
	public:
		int iTransID;		//交易编号
		_DATE_TIME dtOperationTime;	//操作时间
		CString txtCoinBoxID;		//纸币找零箱编号
		int iCoinTypeCode;		//币种
		int iQuantity;			//数量
		CString txtReserve;		//预留

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
			strSql.Format(_T("insert into tb_BanknoteChangeRecord(\
						  iTransID,\
						  dtOperationTime,\
						  txtCoinBoxID,\
						  iCoinTypeCode,\
						  iQuantity,\
						  txtReserve)values('%d','%s','%s','%d','%d','%s')"),
						  iTransID,
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinBoxID,
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
			strSql.Format(_T("update tb_BanknoteChangeRecord set \
						  dtOperationTime='%s',\
						  txtCoinBoxID='%s',\
						  iCoinTypeCode='%d',\
						  iQuantity='%d',\
						  txtReserve='%s', where iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinBoxID,
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
			strSql.Format(_T("delete from tb_BanknoteChangeRecord where iTransID='%d'"),
						  iTransID);
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
			strSql.Format(_T("delete from tb_BanknoteChangeRecord "));
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
			CString strSql = _T("select iTransID, dtOperationTime, txtCoinBoxID, \
								iCoinTypeCode, iQuantity, txtReserve \
								from tb_BanknoteChangeRecord");
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
			txtCoinBoxID = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iCoinTypeCode = sqlite3_column_int(stmt,3);
			iQuantity = sqlite3_column_int(stmt,4);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			return 0;
		}
	};
}