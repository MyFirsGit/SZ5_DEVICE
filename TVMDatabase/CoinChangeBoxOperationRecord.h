#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief      Ӳ�������������¼ʵ����
	*/
	class CoinChangeBoxOperationRecord
	{
	public:

		int iID;					//��¼���
		_DATE_TIME dtOperationTime;	//����ʱ��
		int iOperationType;			//��������
		CString txtCoinChangeBoxID;	//��������
		int iCoinTypeCode;			//����
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
			strSql.Format(_T("insert into tb_CoinChangeBoxOperationRecord(\
						  dtOperationTime,		\
						  iOperationType,\
						  txtCoinChangeBoxID,	\
						  iCoinTypeCode,	\
						  iQuantity,		\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%d','%s','%d','%d','%s','%s','%s')"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iOperationType,
						  txtCoinChangeBoxID,			
						  iCoinTypeCode,
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
			strSql.Format(_T("update tb_CoinChangeBoxOperationRecord set \
						  dtOperationTime='%s',		\
						  iOperationType='%d',\
						  txtCoinChangeBoxID='%s',		\
						  iBanknoteTypeCode='%d',	\
						  iQuantity='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  iOperationType,
						  txtCoinChangeBoxID,	
						  iCoinTypeCode	,
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
			strSql.Format(_T("delete from tb_CoinChangeBoxOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_CoinChangeBoxOperationRecord "));
			return strSql;
		}
		//////////////////////////////////////////////////////////////////////////
		CString ToSelectSql()
		{
			CString strSql = _T("select iID,			 \
							 dtOperationTime,		 \
							 iOperationType,\
							 txtCoinChangeBoxID,		 \
							 iCoinTypeCode,	 \
							 iQuantity,		 \
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_CoinChangeBoxOperationRecord");
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
			dtOperationTime.TryParse(ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,1)));
			iOperationType		= sqlite3_column_int(stmt,2);
			txtCoinChangeBoxID	= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));
			iCoinTypeCode		= sqlite3_column_int(stmt,4);
			iQuantity			= sqlite3_column_int(stmt,5);
			txtOperatorID		= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));
			txtComment			= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));
			txtReserve			= ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,8));

			return 0;
		}
	};
}