#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
namespace Data
{
	class TicketKind
	{
	public:
		int iTicketKind;			// 票卡类型，1：M1K FORM  2：UL TOKEN

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
			CString strSql = _T("select DISTINCT iTicketKind from tb_blankcard_info");
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
			iTicketKind = sqlite3_column_int(stmt,0);
			return 0;
		}
	};
}