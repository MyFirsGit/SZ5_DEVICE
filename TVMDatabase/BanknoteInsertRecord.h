#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	

	/**
	@brief		ֽ��Ͷ�������ϸʵ����
	*/
	class BanknoteInsertRecord
	{
	public:
		int iTransID;		//���ױ��
		_DATE_TIME dtOperationTime;	//����ʱ��
		int iCoinTypeCode;		//����
		int iQuantity;			//����
		CString txtReserve;		//Ԥ��
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
		@brief    ƴ��SQL�������

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQLɾ�����

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQLɾ�����,������where���

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQL��ѯ���

		@param

		@retval  CString SQL��� 

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
		@brief    ���ʵ��

		@param	  sqlite3_stmt* stmt �����

		@retval   long 0��ִ�гɹ�����0��ִ��ʧ��

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