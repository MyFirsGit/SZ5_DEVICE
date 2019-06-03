#include "stdafx.h"
#include "ConnectCommand.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief     构造函数 

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Data::CConnectCommand::CConnectCommand(CString mainDBPath,CString backupDBPath)
	:CDataCommand()
	,pMainDBPath(NULL)
	,pBackupDBPath(NULL)
{
	pMainDBPath = ComUtf16ToUtf8(mainDBPath);
	pBackupDBPath =ComUtf16ToUtf8(backupDBPath);
}

//////////////////////////////////////////////////////////////////////////
/*
@brief   析构函数   

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
Data::CConnectCommand::~CConnectCommand()
{
	delete pMainDBPath;
	pMainDBPath = NULL;
	delete pBackupDBPath;
	pBackupDBPath = NULL;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief  执行命令，打开主数据库连接和备份数据库连接    

@param

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long Data::CConnectCommand::ExecuteCommand()
{
	sqlite3_initialize();
	int sqlite_result = SQLITE_OK;
	sqlite_result = sqlite3_open(pMainDBPath,&m_pConnection);
	if(sqlite_result!=SQLITE_OK)
	{

		SetErrorMessage(sqlite3_errmsg(m_pConnection));
		return sqlite_result;
	}
	sqlite_result = sqlite3_open(pBackupDBPath,&m_pBackupConnection);
	if(sqlite_result!=SQLITE_OK)
	{
		SetErrorMessage(sqlite3_errmsg(m_pBackupConnection));
		return sqlite_result;
	}
	return sqlite_result;
};