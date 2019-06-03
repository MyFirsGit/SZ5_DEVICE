#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
namespace Data
{
	class SupplyBatchNo
	{
	public:
		int iSupplyBatchNo;			// ��������
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
			CString strSql = _T("SELECT DISTINCT iSupplyBatchNo from tb_blankcard_info");
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
			iSupplyBatchNo = sqlite3_column_int(stmt,0);			
			return 0;
		}
	};
}