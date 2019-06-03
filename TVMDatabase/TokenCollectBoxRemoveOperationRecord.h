#pragma once
#include "stdafx.h"
//#include "Task.h"
//#include "TaskPreissue.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data 
{
	/**
	@brief     Ԥ��ֵ�������������Ա�����ʵ����
	*/
	class TokenCollectBoxRemoveOperationRecord
	{
	public:
		int iID;					//��¼���	
		_DATE_TIME dtTransTime;		//����ʱ��	
		CString txtTokenBoxID;		//��Ʊ����
		int iQuantity;				//��Ʊ������
		CString txtOperatorID;			//����ԱID	
		CString txtComment;				//��ע		
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
			strSql.Format(_T("insert into tb_TokenCollectBoxRemoveOperationRecord(\
						  dtTransTime,		\
						  txtTokenBoxID,		\
						  iQuantity,	\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%d','%s','%s','%s')"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),		
						  txtTokenBoxID,			
						  iQuantity,
						  txtOperatorID,
						  txtComment,		
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
			strSql.Format(_T("update tb_TokenCollectBoxRemoveOperationRecord set \
						  dtTransTime='%s',		\
						  txtTokenBoxID='%s',		\
						  iQuantity='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtTokenBoxID,	
						  iQuantity	,
						  txtOperatorID,
						  txtComment,
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
			strSql.Format(_T("delete from tb_TokenCollectBoxRemoveOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_TokenCollectBoxRemoveOperationRecord "));
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
							 dtTransTime,		 \
							 txtTokenBoxID,		 \
							 iQuantity,	 \
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_TokenCollectBoxRemoveOperationRecord");
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
			dtTransTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1)));
			txtTokenBoxID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iQuantity		 = sqlite3_column_int(stmt,3);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));

			return 0;
		}
	};
};