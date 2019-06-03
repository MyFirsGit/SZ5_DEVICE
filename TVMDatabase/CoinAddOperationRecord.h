#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     Ӳ�������䲹�������¼ʵ����
	*/
	class CoinAddOperationRecord
	{
	public:

		int iID;					//��¼���	
		_DATE_TIME dtOperationTime;		//����ʱ��	
		CString txtCoinAddBoxID;		//��������(Ԥ��)
		CString txtCoinChangeBoxID;		//��������	
		int iCoinTypeCode;				//����		
		int iQuantityBeforeAdd;			//����ǰ����	
		int iQuantityAdd;				//��������	
		int iQuantityAfterAdd;			//���������	
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
			strSql.Format(_T("insert into tb_CoinAddOperationRecord(\
						  dtOperationTime,		\
						  txtCoinAddBoxID,		\
						  txtCoinChangeBoxID,		\
						  iCoinTypeCode,	\
						  iQuantityBeforeAdd,		\
						  iQuantityAdd,			\
						  iQuantityAfterAdd,			\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%s','%d','%d','%d','%d','%s','%s','%s')"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),		
						  txtCoinAddBoxID,		
						  txtCoinChangeBoxID,	
						  iCoinTypeCode,
						  iQuantityBeforeAdd,	
						  iQuantityAdd,
						  iQuantityAfterAdd,	
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
			strSql.Format(_T("update tb_CoinAddOperationRecord set \
						  dtOperationTime='%s',		\
						  txtCoinAddBoxID='%s',		\
						  txtCoinChangeBoxID='%s',	\
						  iCoinTypeCode='%d',	\
						  iQuantityBeforeAdd='%d',	\
						  iQuantityAdd='%d',			\
						  iQuantityAfterAdd	='%d',			\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinAddBoxID,	
						  txtCoinChangeBoxID,
						  iCoinTypeCode	,
						  iQuantityBeforeAdd,
						  iQuantityAdd,
						  iQuantityAfterAdd	,
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
			strSql.Format(_T("delete from tb_CoinAddOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_CoinAddOperationRecord "));
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
							 txtCoinAddBoxID,		 \
							 txtCoinChangeBoxID,		 \
							 iCoinTypeCode,	 \
							 iQuantityBeforeAdd,		 \
							 iQuantityAdd,			 \
							 iQuantityAfterAdd,			 \
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_CoinAddOperationRecord");
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
			txtCoinAddBoxID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			txtCoinChangeBoxID	 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));
			iCoinTypeCode		 = sqlite3_column_int(stmt,4);
			iQuantityBeforeAdd	 = sqlite3_column_int(stmt,5);
			iQuantityAdd		 = sqlite3_column_int(stmt,6);
			iQuantityAfterAdd	 = sqlite3_column_int(stmt,7);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,8));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,9));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,10));

			return 0;
		}
	};
}