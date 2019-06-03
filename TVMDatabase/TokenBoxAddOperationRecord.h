#pragma once;
#include "stdafx.h"
#include "sqlite3.h"

#include "Util.h"

namespace Data
{
	/**
	@brief    分拣任务和任务属性联合实体类
	*/
	class TokenBoxAddOperationRecord
	{
	public:
		int iID;					//记录编号	
		_DATE_TIME dtOperationTime;		//操作时间	
		CString txtTokenAddBoxID;
		CString txtTokenHopperBoxID;
		int iQuantityBeforeAdd;
		int iQuantityAdd;
		int iQuantityAfterAdd;
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
			strSql.Format(_T("insert into tb_TokenBoxAddOperationRecord(\
						  dtOperationTime,		\
						  txtTokenAddBoxID,\
						  txtTokenHopperBoxID,	\
						  iQuantityBeforeAdd,	\
						  iQuantityAdd,		\
						  iQuantityAfterAdd,	\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%s','%d','%d','%d','%s','%s','%s')"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),		
						  txtTokenAddBoxID,
						  txtTokenHopperBoxID,
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
		@brief    拼接SQL更新语句

		@param

		@retval  CString SQL语句   

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CString ToUpdateSql()
		{
			CString strSql = _T("");
			strSql.Format(_T("update tb_TokenBoxAddOperationRecord set \
						  dtOperationTime='%s',		\
						  txtTokenAddBoxID='%s',	\
						  txtTokenHopperBoxID='%s',	\
						  iQuantityBeforeAdd='%d',	\
						  iQuantityAdd='%d',	\
						  iQuantityAfterAdd='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtOperationTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtTokenAddBoxID,
						  txtTokenHopperBoxID,
						  iQuantityBeforeAdd,
						  iQuantityAdd,
						  iQuantityAfterAdd,
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
			strSql.Format(_T("delete from tb_TokenBoxAddOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_TokenBoxAddOperationRecord "));
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
							 txtTokenAddBoxID,		\
							 txtTokenHopperBoxID,		\
							 iQuantityBeforeAdd,		\
							 iQuantityAdd,		\
							 iQuantityAfterAdd,		\
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_TokenBoxAddOperationRecord");
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
			txtTokenAddBoxID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			txtTokenHopperBoxID	 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,3));
			iQuantityBeforeAdd	 = sqlite3_column_int(stmt,4);
			iQuantityAdd		 = sqlite3_column_int(stmt,5);
			iQuantityAfterAdd	 = sqlite3_column_int(stmt,6);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,7));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,8));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,9));

			return 0;
		}
	};
}