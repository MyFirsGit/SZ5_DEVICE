#pragma once;

#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     Token��ж�ز�����¼��ʵ����
	*/
	class TokenBoxRemoveOperationRecord
	{
	public:
		int iID;					//��¼���	
		_DATE_TIME dtOperationTime;		//����ʱ��	
		CString txtTokenBoxID;			//Token����
		int iOriginalQuantity;			//ԭʼ����
		int iRemoveQuantity;			//ж������
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
			strSql.Format(_T("insert into tb_TokenBoxRemoveOperationRecord(\
						  dtOperationTime,		\
						  txtTokenBoxID,		\
						  iOriginalQuantity,	\
						  iRemoveQuantity,	\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%d','%d','%s','%s','%s')"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),		
						  txtTokenBoxID,			
						  iOriginalQuantity,
						  iRemoveQuantity,
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
			strSql.Format(_T("update tb_TokenBoxRemoveOperationRecord set \
						  dtOperationTime='%s',		\
						  txtTokenBoxID='%s',		\
						  iOriginalQuantity='%d',	\
						  iRemoveQuantity='%d',		\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtTokenBoxID,	
						  iOriginalQuantity	,
						  iRemoveQuantity,
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
			strSql.Format(_T("delete from tb_TokenBoxRemoveOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_TokenBoxRemoveOperationRecord "));
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
							 dtOperationTime,		 \
							 txtTokenBoxID,		 \
							 iOriginalQuantity,	 \
							 iRemoveQuantity,		\
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_TokenBoxRemoveOperationRecord");
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
			dtOperationTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1)));
			txtTokenBoxID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iOriginalQuantity	 = sqlite3_column_int(stmt,3);
			iRemoveQuantity		 = sqlite3_column_int(stmt,4);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));

			return 0;
		}
	};
}