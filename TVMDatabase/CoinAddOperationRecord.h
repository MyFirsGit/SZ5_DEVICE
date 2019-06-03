#pragma once;
#include "sqlite3.h"
#include "Util.h"

namespace Data
{
	/**
	@brief     硬币找零箱补充操作记录实体类
	*/
	class CoinAddOperationRecord
	{
	public:

		int iID;					//记录编号	
		_DATE_TIME dtOperationTime;		//操作时间	
		CString txtCoinAddBoxID;		//补充箱编号(预留)
		CString txtCoinChangeBoxID;		//找零箱编号	
		int iCoinTypeCode;				//币种		
		int iQuantityBeforeAdd;			//补充前数量	
		int iQuantityAdd;				//补充数量	
		int iQuantityAfterAdd;			//补充后数量	
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
		@brief    拼接SQL更新语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句

		@param

		@retval  CString SQL语句   

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
		@brief    拼接SQL删除语句,不带有where语句

		@param

		@retval   CString SQL语句   

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