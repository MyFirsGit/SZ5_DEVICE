#pragma once
#include "stdafx.h"
//#include "Task.h"
//#include "TaskReencode.h"
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief      Card�䰲װ������¼ʵ����
	*/
	class CardBoxInstallOperationRecord
	{
	public:
		int iID;						//��¼���	
		_DATE_TIME dtOperationTime;		//����ʱ��	
		CString txtCardBoxID;			//Token����
		int iQuantity;					//��װ����	
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
			strSql.Format(_T("insert into tb_CardBoxInstallOperationRecord(\
						  dtOperationTime,		\
						  txtCardBoxID,		\
						  iQuantity,	\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%d','%s','%s','%s')"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),		
						  txtCardBoxID,			
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
			strSql.Format(_T("update tb_CardBoxInstallOperationRecord set \
						  dtOperationTime='%s',		\
						  txtCardBoxID='%s',		\
						  iQuantity='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCardBoxID,	
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
			strSql.Format(_T("delete from tb_CardBoxInstallOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_CardBoxInstallOperationRecord "));
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
							 txtCardBoxID,		 \
							 iQuantity,	 \
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_CardBoxInstallOperationRecord");
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
			txtCardBoxID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iQuantity		 = sqlite3_column_int(stmt,3);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));

			return 0;
		}
	};
}