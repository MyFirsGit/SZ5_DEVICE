#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief      纸币找零箱操作记录实体类
	*/
	class BanknoteOperationRecord
	{
	public:

		int iID;					//记录编号
		_DATE_TIME dtTransTime;		//操作时间
		int iOperationType;			//操作类型
		CString txtBanknoteBoxID;	//补充箱编号
		int iBanknoteTypeCode;		//币种
		int iQuantity;				//数量
		CString txtOperatorID;		//操作员ID
		CString txtComment;			//备注
		CString txtReserve;			//预留

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
		@brief    拼接SQL更新语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL查询语句

		@param

		@retval  CString SQL语句   

		@exception  
		*/
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
		@brief    填充实体

		@param	  sqlite3_stmt* stmt 结果集

		@retval   long 0：执行成功，非0：执行失败

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