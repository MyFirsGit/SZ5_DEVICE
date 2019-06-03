#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     ֽ�һ������ֽ���ϸʵ����
	*/
	class BanknoteBoxCashDetail
	{
	public:
		int iID;					//��¼���
		int iBanknoteTypeCode;				//����		
		int iQuantity;					//����	
		CString txtReserve;				//Ԥ��	


	public:
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ƴ��SQL�������

		@param

		@retval  CString SQL���   

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
		@brief    ƴ��SQL�������

		@param

		@retval  CString SQL���   

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
		@brief    ƴ��SQLɾ�����

		@param

		@retval  CString SQL���   

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
		@brief    ƴ��SQLɾ�����,������where���

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQL��ѯ���

		@param

		@retval  CString SQL���   

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
		@brief    ���ʵ��

		@param	  sqlite3_stmt* stmt �����

		@retval   long 0��ִ�гɹ�����0��ִ��ʧ��

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