#include "stdafx.h"
#include "NonQueryCommand.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief  构造函数    

@param CString& sql 执行的sql语句。

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Data::CNonQueryCommand::CNonQueryCommand(CString& sql):Data::CDataCommand()
{
	affectRows = 0;
	sqlStatement = ComUtf16ToUtf8(sql);
	theDB_Data->WriteData(_T(""),_T(__FUNCTION__),__LINE__,sql);
};

//////////////////////////////////////////////////////////////////////////
/*
@brief    析造函数  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Data::CNonQueryCommand::~CNonQueryCommand()
{
	delete sqlStatement;
	sqlStatement = NULL;
};

//////////////////////////////////////////////////////////////////////////
/*
@brief    取得影响行数 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
int Data::CNonQueryCommand::GetAffectRows()
{
	return affectRows;
};

	
//////////////////////////////////////////////////////////////////////////
/*
@brief    执行命令函数，该函数中执行SQL语句  

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long Data::CNonQueryCommand::ExecuteCommand()
{
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("->"));
	int sqlite_result = SQLITE_OK;
	char* errorMsg = NULL;
	//主库里执行
	sqlite_result = sqlite3_exec(m_pConnection,sqlStatement,NULL,NULL,&errorMsg);
	if(sqlite_result!=SQLITE_OK)
	{
		SetErrorMessage(errorMsg);
		sqlite3_free(errorMsg);
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));

		return sqlite_result;
	}
	//备份数据库里执行
	sqlite_result = sqlite3_exec(m_pBackupConnection,sqlStatement,NULL,NULL,&errorMsg);
	if(sqlite_result!=SQLITE_OK)
	{
		SetErrorMessage(errorMsg);
		sqlite3_free(errorMsg);
		theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
		return sqlite_result;
	}
	affectRows = sqlite3_changes(m_pConnection);
	theDEBUG_LOG->WriteData(_T(""),_T(__FUNCTION__),__LINE__,_T("<-"));
	return 0;
};