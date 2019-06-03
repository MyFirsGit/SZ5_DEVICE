#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief      ֽ�������������¼ʵ����
	*/
	class BanknoteOperationRecord
	{
	public:

		int iID;					//��¼���
		_DATE_TIME dtTransTime;		//����ʱ��
		int iOperationType;			//��������
		CString txtBanknoteBoxID;	//��������
		int iBanknoteTypeCode;		//����
		int iQuantity;				//����
		CString txtOperatorID;		//����ԱID
		CString txtComment;			//��ע
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
			strSql.Format(_T("insert into tb_BanknoteOperationRecord(\
						  dtTransTime,		\
						  iOperationType,\
						  txtBanknoteBoxID,		\
						  iBanknoteTypeCode,	\
						  iQuantity,		\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%d','%s','%d','%d','%s','%s','%s')"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iOperationType,
						  txtBanknoteBoxID,			
						  iBanknoteTypeCode,
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
			strSql.Format(_T("update tb_BanknoteOperationRecord set \
						  dtTransTime='%s',		\
						  iOperationType='%d',\
						  txtBanknoteBoxID='%s',		\
						  iBanknoteTypeCode='%d',	\
						  iQuantity='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iOperationType,
						  txtBanknoteBoxID,	
						  iBanknoteTypeCode	,
						  iQuantity,
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
			strSql.Format(_T("delete from tb_BanknoteOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_BanknoteOperationRecord "));
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		CString ToSelectSql()
		{
			CString strSql = _T("select iID,			 \
							 dtTransTime,		 \
							 iOperationType,\
							 txtBanknoteBoxID,		 \
							 iBanknoteTypeCode,	 \
							 iQuantity,		 \
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_BanknoteOperationRecord");
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
			iID					= sqlite3_column_int(stmt,0);
			dtTransTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1)));
			iOperationType		= sqlite3_column_int(stmt,2);
			txtBanknoteBoxID	= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));
			iBanknoteTypeCode	= sqlite3_column_int(stmt,4);
			iQuantity			= sqlite3_column_int(stmt,5);
			txtOperatorID		= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));
			txtComment			= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));
			txtReserve			= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,8));

			return 0;
		}
	};
}