#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     票卡处理日志实体类
	*/
	class ExceptionRecord
	{
	public:
		int iTransID;			//交易编号
		_DATE_TIME dtOperationTime;	//故障时间
		int/*CString*/ iExceptionType;		//故障类型   lws：修改故障类型字段的变量类型
		int IExceptionAmount;		//故障金额
		CString txtReserve;			//预留

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
			strSql.Format(_T("insert into tb_ExceptionRecord(\
						  iTransID,\
						  dtOperationTime,\
						  iExceptionType,\
						  IExceptionAmount,\
						  txtReserve)values('%d','%s','%d','%d','%s')"),
						  iTransID,
						  dtOperationTime.ToString(_T("%04d%02d%02d%02d%02d%02d")),
						  iExceptionType,
						  IExceptionAmount,
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
			strSql.Format(_T("update tb_ExceptionRecord set \
						  dtOperationTime='%s',\
						  iExceptionType='%d',\
						  IExceptionAmount='%d',\
						  txtReserve='%s', where iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iExceptionType,
						  IExceptionAmount,
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
			strSql.Format(_T("delete from tb_ExceptionRecord where iTransID='%d'"),
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
			strSql.Format(_T("delete from tb_ExceptionRecord "));
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
			CString strSql = _T("select iTransID, dtOperationTime, iExceptionType, \
								IExceptionAmount, txtReserve \
								from tb_ExceptionRecord");
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
			iExceptionType = sqlite3_column_int/*ComUtf8ToUtf16((char*)sqlite3_column_text*/(stmt,2)/*)*/;
			IExceptionAmount = sqlite3_column_int(stmt,3);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			return 0;
		}
	};
}