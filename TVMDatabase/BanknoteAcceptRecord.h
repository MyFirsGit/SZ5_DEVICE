#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     ֽ�����ɲ�����ϸʵ����
	*/
	class BanknoteAcceptRecord
	{
	public:

		int iTransID;			//���ױ��
		_DATE_TIME dtOperationTime;	//����ʱ��
		CString txtAcceptCoinBoxID;	//ֽ�һ�������
		int iCoinTypeCode;			//����
		int iQuantity;				//����
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
			strSql.Format(_T("insert into tb_BanknoteAcceptRecord(\
						  iTransID,\
						  dtOperationTime,\
						  txtAcceptCoinBoxID,	\
						  iCoinTypeCode,\
						  iQuantity,\
						  txtReserve)values('%d','%s','%s','%d','%d','%s')"),
						  iTransID,
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtAcceptCoinBoxID,
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
			strSql.Format(_T("update tb_BanknoteAcceptRecord set \
						  dtOperationTime='%s', \
						  txtAcceptCoinBoxID='%s',\
						  iCoinTypeCode='%d', \
						  iQuantity='%d', \
						  txtReserve='%s', where iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtAcceptCoinBoxID,
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
			strSql.Format(_T("delete from tb_BanknoteAcceptRecord where iTransID='%d'"),
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
			strSql.Format(_T("delete from tb_BanknoteAcceptRecord "));
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
			CString strSql = _T("select iTransID, dtOperationTime, txtAcceptCoinBoxID, \
								iCoinTypeCode, iQuantity, txtReserve \
								from tb_BanknoteAcceptRecord");
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
			txtAcceptCoinBoxID = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iCoinTypeCode = sqlite3_column_int(stmt,3);
			iQuantity = sqlite3_column_int(stmt,4);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			return 0;
		}
	};
}