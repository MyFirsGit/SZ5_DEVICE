#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     Ӳ�����ɲ�����ϸʵ����
	*/
	class CoinAcceptRecord
	{
	public:

		int iTransID;			//���ױ��
		_DATE_TIME dtOperationTime;	//����ʱ��
		CString txtCoinTempID;		//�ݴ������
		CString txtAcceptCoinBoxID;	//����������
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
			strSql.Format(_T("insert into tb_CoinAcceptRecord(\
						  iTransID,\
						  dtOperationTime,\
						  txtCoinTempID,\
						  txtAcceptCoinBoxID,	\
						  iCoinTypeCode,\
						  iQuantity,\
						  txtReserve)values('%d','%s','%s','%s','%d','%d','%s')"),
						  iTransID,
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinTempID,
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
			strSql.Format(_T("update tb_CoinAcceptRecord set \
						  dtOperationTime='%s', \
						  txtCoinTempID='%s', \
						  txtAcceptCoinBoxID='%s',\
						  iCoinTypeCode='%d', \
						  iQuantity='%d', \
						  txtReserve='%s', where iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinTempID,
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
			strSql.Format(_T("delete from tb_CoinAcceptRecord where iTransID='%d'"),
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
			strSql.Format(_T("delete from tb_CoinAcceptRecord "));
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
			CString strSql = _T("select iTransID, dtOperationTime, txtCoinTempID, txtAcceptCoinBoxID, \
								iCoinTypeCode, iQuantity, txtReserve \
								from tb_CoinAcceptRecord");
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
			txtCoinTempID = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			txtAcceptCoinBoxID = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));
			iCoinTypeCode = sqlite3_column_int(stmt,4);
			iQuantity = sqlite3_column_int(stmt,5);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));
			return 0;
		}
	};
}