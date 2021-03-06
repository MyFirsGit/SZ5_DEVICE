#pragma once;
#include "sqlite3.h"
#include "Util.h"
#include "CoinBoxCashDetail.h"
#include <vector>

namespace Data
{
	/**
	@brief     ���Ի�����ʵ����
	*/
	class CoinCollectBoxRemoveOperationDetail
	{
	public:
		int iID;					//��¼���	
		_DATE_TIME dtTransTime;			//����ʱ��		
		CString txtCoinCollectBoxID;	//��������
		int iTotalAmount;				//Ӳ�һ�������
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
			strSql.Format(_T("insert into tb_CoinCollectBoxRemoveOperationRecord(\
						  dtTransTime,		\
						  txtCoinCollectBoxID,		\
						  iTotalAmount,	\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%d','%s','%s','%s')"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),			
						  txtCoinCollectBoxID,	
						  iTotalAmount,
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
			strSql.Format(_T("update tb_CoinCollectBoxRemoveOperationRecord set \
						  dtTransTime='%s',		\
						  txtCoinCollectBoxID='%s',	\
						  iTotalAmount='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinCollectBoxID,
						  iTotalAmount,
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
			strSql.Format(_T("delete from tb_CoinCollectBoxRemoveOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_CoinCollectBoxRemoveOperationRecord "));
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
							 txtCoinCollectBoxID,	\
							 iTotalAmount,			\
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_CoinCollectBoxRemoveOperationRecord");
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
			txtCoinCollectBoxID  = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iTotalAmount		 = sqlite3_column_int(stmt,3);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));

			return 0;
		}
		
	};
	class CoinCollectBoxRemoveOperationRecord
	{
	public:
		CoinCollectBoxRemoveOperationDetail coinCollectBoxRemoveOperationDetail;
		vector<CoinBoxCashDetail> v_coinBoxCashDetail;

	};
}