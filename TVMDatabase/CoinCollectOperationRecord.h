#pragma once
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief      Ӳ�һ��ղ�����¼ʵ����
	*/
	class CoinCollectOperationRecord
	{
	public:

		int iID;					//��¼���	
		_DATE_TIME dtOperationTime;		//����ʱ��		
		CString txtCoinChangeBoxID;		//��������	
		int iCoinTypeCode;				//����		
		int iQuantityBeforeCollect;		//����ǰ����	
		int iQuantityCollect;			//��������	
		int iQuantityAfterCollect;		//���պ�����	
		CString txtCoinCollectBoxID;	//��������
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
			strSql.Format(_T("insert into tb_CoinCollectOperationRecord(\
						  dtOperationTime,		\
						  txtCoinChangeBoxID,		\
						  iCoinTypeCode,	\
						  iQuantityBeforeCollect,		\
						  iQuantityCollect,			\
						  iQuantityAfterCollect,			\
						  txtCoinCollectBoxID,		\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%d','%d','%d','%d','%s','%s','%s','%s')"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),			
						  txtCoinChangeBoxID,	
						  iCoinTypeCode,
						  iQuantityBeforeCollect,	
						  iQuantityCollect,
						  iQuantityAfterCollect,
						  txtCoinCollectBoxID,
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
			strSql.Format(_T("update tb_CoinCollectOperationRecord set \
						  dtOperationTime='%s',		\
						  txtCoinChangeBoxID='%s',	\
						  iCoinTypeCode='%d',	\
						  iQuantityBeforeCollect='%d',	\
						  iQuantityCollect='%d',			\
						  iQuantityAfterCollect	='%d',			\
						  txtCoinCollectBoxID='%s',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtCoinChangeBoxID,
						  iCoinTypeCode	,
						  iQuantityBeforeCollect,
						  iQuantityCollect,
						  iQuantityAfterCollect	,
						  txtCoinCollectBoxID,
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
			strSql.Format(_T("delete from tb_CoinCollectOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_CoinCollectOperationRecord "));
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
							 txtCoinChangeBoxID,		 \
							 iCoinTypeCode,	 \
							 iQuantityBeforeCollect,		 \
							 iQuantityCollect,			 \
							 iQuantityAfterCollect,			 \
							 txtCoinCollectBoxID,	\
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_CoinCollectOperationRecord");
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
			txtCoinChangeBoxID	 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iCoinTypeCode		 = sqlite3_column_int(stmt,3);
			iQuantityBeforeCollect	 = sqlite3_column_int(stmt,4);
			iQuantityCollect		 = sqlite3_column_int(stmt,5);
			iQuantityAfterCollect	 = sqlite3_column_int(stmt,6);
			txtCoinCollectBoxID  = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,8));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,9));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,10));

			return 0;
		}
	};
}