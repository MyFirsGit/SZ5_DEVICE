
#include "stdafx.h"
#include "SCDataMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/*
@brief      数据命令构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCDataMsg::CSCDataMsg():CSCCommand()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief     析构函数 

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCDataMsg::~CSCDataMsg()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSCDataMsg::IsValidCommand()
{
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      执行命令

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSCDataMsg::ExecuteCommand()
{
	//return SendDataMsg();
	return __super::ExecuteCommand();
}


