#pragma once;
#include "stdafx.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief		Ʊ��������ϸ��
	*/
	class TicketOperationRecord
	{
		public:
		int iTransID;			//���ױ��
		_DATE_TIME dtOperationTime;	//����ʱ��
		int iMediaType;				//Ʊ������
		CString txtTicketBoxID;		//Ʊ����
		int iCardSerialNumber;		//Ʊ�����
		int iEncodeState;			//����״̬
		int iReleaseState;			//�ų�״̬
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
			strSql.Format(_T("insert into tb_TicketOperationRecord(\
						  iTransID,\
						  dtOperationTime,\
						  iMediaType,\
						  txtTicketBoxID,\
						  iCardSerialNumber,\
						  iEncodeState,\
						  iReleaseState,\
						  txtReserve)values('%d','%s','%d', '%s','%d','%d', '%d','%s')"),
						  iTransID,
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iMediaType,		
						  txtTicketBoxID,	
						  iCardSerialNumber,
						  iEncodeState,		
						  iReleaseState,	
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
			strSql.Format(_T("update tb_TicketOperationRecord set \
						  dtOperationTime='%s',\
						  iMediaType,\
						  txtTicketBoxID='%s,\
						  iCardSerialNumber='%d',\
						  iEncodeState='%d',\
						  iReleaseState='%d',\
						  txtReserve='%s', where iTransID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iMediaType,		
						  txtTicketBoxID,	
						  iCardSerialNumber,
						  iEncodeState,		
						  iReleaseState,
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
			strSql.Format(_T("delete from tb_TicketOperationRecord where iTransID='%d'"),
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
			strSql.Format(_T("delete from tb_TicketOperationRecord "));
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
			CString strSql = _T("select iTransID, dtOperationTime,iMediaType,\
								txtTicketBoxID,\
								iCardSerialNumber,\
								iEncodeState,\
								iReleaseState,\
								txtReserve \
								from tb_TicketOperationRecord");
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

			iMediaType = sqlite3_column_int(stmt,2);

			txtTicketBoxID = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));

			iCardSerialNumber = sqlite3_column_int(stmt,4);
			iEncodeState = sqlite3_column_int(stmt,5);
			iReleaseState = sqlite3_column_int(stmt,6);
			txtReserve = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));
			return 0;
		}

		
	};
}