#pragma once;
#include "sqlite3.h"
#include "Util.h"
#include "CoinBoxCashDetail.h"
#include <vector>

namespace Data
{
	/**
	@brief     个性化任务实体类
	*/
	class CoinCollectBoxRemoveOperationDetail
	{
	public:
		int iID;					//记录编号	
		_DATE_TIME dtTransTime;			//操作时间		
		CString txtCoinCollectBoxID;	//回收箱编号
		int iTotalAmount;				//硬币回收箱金额
		CString txtOperatorID;			//操作员ID	
		CString txtComment;				//备注		
		CString txtReserve;				//预留	
		
	public:
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    拼接SQL插入语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL更新语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句,不带有where语句

		@param

		@retval   CString SQL语句   

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
		@brief    拼接SQL查询语句

		@param

		@retval  CString SQL语句   

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
		@brief    填充实体

		@param	  sqlite3_stmt* stmt 结果集

		@retval   long 0：执行成功，非0：执行失败

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