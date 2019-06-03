#pragma once;
#include "sqlite3.h"
#include "Util.h"
#include <vector>
#include "BanknoteBoxCashDetail.h"

namespace Data
{
	/**
	@brief      纸币箱卸载回收操作记录实体类
	*/
	class BanknoteBoxRemoveOperationDetail
	{
	public:

		int iID;					//记录编号	
		_DATE_TIME dtTransTime;		//操作时间	
		CString txtBanknoteBoxID;		//补充箱编号
		int iTotalAmount;				//纸币回收箱金额	
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
			strSql.Format(_T("insert into tb_BanknoteBoxRemoveOperationRecord(\
						  dtTransTime,		\
						  txtBanknoteBoxID,		\
						  iTotalAmount,	\
						  txtOperatorID,			\
						  txtComment,			\
						  txtReserve)values('%s','%s','%d','%s','%s','%s')"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),		
						  txtBanknoteBoxID,			
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
			strSql.Format(_T("update tb_BanknoteBoxRemoveOperationRecord set \
						  dtTransTime='%s',		\
						  txtBanknoteBoxID='%s',		\
						  iTotalAmount='%d',	\
						  txtOperatorID='%s',			\
						  txtComment='%s',			\
						  txtReserve='%s' where iID='%d'"),
						  dtTransTime.ToString(_T("%.4d%.2d%.2d%.2d%.2d%.2d")),
						  txtBanknoteBoxID,	
						  iTotalAmount	,
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
			strSql.Format(_T("delete from tb_BanknoteBoxRemoveOperationRecord  where iID='%d'"),iID);
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
			strSql.Format(_T("delete from tb_BanknoteBoxRemoveOperationRecord "));
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
							 txtBanknoteBoxID,		 \
							 iTotalAmount,	 \
							 txtOperatorID,			 \
							 txtComment,			 \
							 txtReserve from tb_BanknoteBoxRemoveOperationRecord");
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
			txtBanknoteBoxID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,2));
			iTotalAmount		 = sqlite3_column_int(stmt,3);
			txtOperatorID		 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,4));
			txtComment			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,5));
			txtReserve			 = ComUtf8ToUtf16((char*)sqlite3_column_text(stmt,6));

			return 0;
		}
	};
	class BanknoteBoxRemoveOperationRecord
	{
	public:
		BanknoteBoxRemoveOperationDetail banknoteBoxRemoveOperationDetail;
		vector<BanknoteBoxCashDetail> v_banknoteBoxCashDetail;
	};
}