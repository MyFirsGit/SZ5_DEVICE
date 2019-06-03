#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"
namespace Data
{
	class SupplyTime
	{
	public:
		CString  txtSupplyTime;			// 供货时间，格式：YYYYMM
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
			CString strSql = _T("select DISTINCT txtSupplyTime from tb_blankcard_info order by txtSupplyTime DESC");
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
			txtSupplyTime = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,0));
			return 0;
		}
	};
}