#pragma once;

#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     Token箱卸载操作记录表实体类
	*/
	class TokenBoxRemoveOperationRecord
	{
	public:
		int iID;					//记录编号	
		_DATE_TIME dtOperationTime;		//操作时间	
		CString txtTokenBoxID;			//Token箱编号
		int iOriginalQuantity;			//原始数量
		int iRemoveQuantity;			//卸载数量
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
		@brief    拼接SQL更新语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句,不带有where语句

		@param

		@retval   CString SQL语句   

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
		@brief    拼接SQL查询语句

		@param

		@retval  CString SQL语句   

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
		@brief    填充实体

		@param	  sqlite3_stmt* stmt 结果集

		@retval   long 0：执行成功，非0：执行失败

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