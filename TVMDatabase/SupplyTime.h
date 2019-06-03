#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"
namespace Data
{
	class SupplyTime
	{
	public:
		CString  txtSupplyTime;			// ����ʱ�䣬��ʽ��YYYYMM
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
			CString strSql = _T("select DISTINCT txtSupplyTime from tb_blankcard_info order by txtSupplyTime DESC");
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
			txtSupplyTime = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,0));
			return 0;
		}
	};
}