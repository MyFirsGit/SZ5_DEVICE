#pragma once
#include "stdafx.h"
#include "DataCommand.h"
#include <vector>

namespace Data
{
	//////////////////////////////////////////////////////////////////////////
	/*
	@brief      ���������ݵ����ݿ��������

	*/
	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	class  CQueryCommand:public CDataCommand
	{

	public:

		typedef enum _UE_SQLTYPE
		{
			UE_UNKNOWN=-1,
			UE_WHERE,
			UE_LIMIT
		}UE_SQLTYPE;

		//////////////////////////////////////////////////////////////////////////
		/*
		@brief  ���캯��    

		@param CString& sql ִ�е�sql��䡣

		@retval     

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		CQueryCommand(CString& condition,UE_SQLTYPE uetype=UE_WHERE):CDataCommand()
		{
			T t;
			CString strSql =t.ToSelectSql();
			if (condition!=_T(""))
			{
				switch(uetype)
				{
				case UE_WHERE:
					strSql =  strSql + _T(" WHERE ") + condition;
					break;
				case UE_LIMIT:
					strSql =  strSql + condition;
					break;
				default:
					break;
				}
			}
			sqlStatement = ComUtf16ToUtf8(strSql);
			theDB_Data->WriteData(_T(""),_T(__FUNCTION__),__LINE__,strSql);
		};
		
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ��������  

		@param

		@retval     

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		virtual ~CQueryCommand()
		{
			delete [] sqlStatement;
			sqlStatement = NULL;
		};
		
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief   ȡ�ò�ѯ���

		@param

		@retval     vector<T>& ��ѯ���

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		std::vector<T>& GetResults()
		{
			return queryResult;
		};

	protected:

		//////////////////////////////////////////////////////////////////////////
		/*
		@brief    ִ����������ú�����ִ��SQL���  

		@param

		@retval     

		@exception  
		*/
		//////////////////////////////////////////////////////////////////////////
		virtual long ExecuteCommand()
		{
			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
			int sqlite_result = SQLITE_OK;

			sqlite3_stmt* stmt;
			const char *tail;
			sqlite_result = sqlite3_prepare_v2(m_pConnection,sqlStatement,strlen(sqlStatement) ,&stmt,&tail);
			if(sqlite_result!=SQLITE_OK)
			{
				SetErrorMessage(sqlite3_errmsg(m_pConnection));
				theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
				return sqlite_result;
			}
			queryResult.clear();
			sqlite_result = sqlite3_step(stmt);
			while (sqlite_result == SQLITE_ROW)
			{
				T entity;
				long fillResult = entity.Fill(stmt);
				queryResult.push_back(entity);
				sqlite_result = sqlite3_step(stmt);
			}
			sqlite3_finalize(stmt);
			theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
			return 0;
		}

	private:
		//////////////////////////////////////////////////////////////////////////
		/*
		@brief  ��ѯ������   
		*/
		//////////////////////////////////////////////////////////////////////////
		CHAR* sqlStatement;

		//////////////////////////////////////////////////////////////////////////
		/*
		@brief  ��ѯ�����š�   
		*/
		//////////////////////////////////////////////////////////////////////////
		std::vector<T> queryResult;

	};

};
