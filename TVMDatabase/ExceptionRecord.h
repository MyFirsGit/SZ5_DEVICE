#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     Ʊ��������־ʵ����
	*/
	class ExceptionRecord
	{
	public:
		int iTransID;			//���ױ��
		_DATE_TIME dtOperationTime;	//����ʱ��
		int/*CString*/ iExceptionType;		//��������   lws���޸Ĺ��������ֶεı�������
		int IExceptionAmount;		//���Ͻ��
		CString txtReserve;			//Ԥ��

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
		@brief    ƴ��SQL�������

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQLɾ�����

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQLɾ�����,������where���

		@param

		@retval   CString SQL���   

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
		@brief    ƴ��SQL��ѯ���

		@param

		@retval  CString SQL��� 

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
			iExceptionType = sqlite3_column_int/*ComUtf8ToUtf16((char*)sqlite3_column_text*/(stmt,2)/*)*/;
			IExceptionAmount = sqlite3_column_int(stmt,3);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			return 0;
		}
	};
}