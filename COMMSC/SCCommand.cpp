#include "stdafx.h"
#include "SCCommand.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SysException.h"
#include "SCException.h"
#include "Util.h"
#include "DebugLog.h"
#include "validation.h"
#include "scclient.h"
#include "scresponsecommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
/**
@brief      CSCCommands构造函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CSCCommand::CSCCommand()
{
	m_pContent = new VARIABLE_DATA;
	m_IsRemote = FALSE;
	m_IsReceived = FALSE;
	m_ReplyMsgCode=-1;
	m_ReplyCommand = NULL;
	m_InterruptedCommand = NULL;
	m_NeedReply = FALSE;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      CSCCommands析构函数
@param      无
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CSCCommand::~CSCCommand()
{
	if(m_pContent!=NULL)
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	if(m_ReplyCommand!=NULL)
	{
		delete m_ReplyCommand;
		m_ReplyCommand = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      电文头部更新
@param      CMD_HEADER  头部数据
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetHeader(CMD_HEADER *pHeader)
{
	memcpy(&m_Header, pHeader, sizeof CMD_HEADER);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      取得电文头部
@param      CMD_HEADER&   头部数据
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
CMD_HEADER& CSCCommand::GetHeader()
{
	return m_Header;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      电文内容更新
@param      LPVARIABLE_DATA  body数据
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetContent(LPVARIABLE_DATA pBody)
{
	if(m_pContent!=NULL)
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	m_pContent = new VARIABLE_DATA;
	m_pContent->lpData = new BYTE[pBody->nLen];
	memcpy(m_pContent->lpData,pBody->lpData,pBody->nLen);
	m_pContent->nLen = pBody->nLen;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      电文内容更新
@param      LPBYTE body数据
@param      UINT length 长度
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetContent(LPBYTE body,UINT length)
{
	if(m_pContent!=NULL)
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	m_pContent = new VARIABLE_DATA;
	m_pContent->lpData = new BYTE[length];
	memcpy(m_pContent->lpData,body,length);
	m_pContent->nLen =length;
}

LPVARIABLE_DATA CSCCommand::GetContent()
{
	return m_pContent;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      序列化电文内容体
@param      LPVARIABLE_DATA 电文内容
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SerializeBody(VARIABLE_DATA& buffer)
{
	// 发送数据包有问题
	if(NULL == m_pContent){
		buffer.nLen = 0;
		return;
	}
	if(m_pContent->nLen <= 0){
		buffer.nLen = 0;
		return;
	}
	if(buffer.lpData!=NULL)
	{
		delete buffer.lpData;
		buffer.lpData = NULL;
	}
	buffer.nLen = 0;
	buffer.lpData = new BYTE[m_pContent->nLen];
	if(m_pContent->lpData && m_pContent->nLen > 0)
		memcpy(buffer.lpData,m_pContent->lpData,m_pContent->nLen);
	buffer.nLen = m_pContent->nLen;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      反序列化电文内容体
@param      LPVARIABLE_DATA 电文内容
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::DeserializeBody(LPBYTE bodyData,UINT length)
{
	if(m_pContent!=NULL)
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	m_pContent = new VARIABLE_DATA;
	m_pContent->lpData = new BYTE[length];
	memcpy(m_pContent->lpData,bodyData,length);
	m_pContent->nLen =length;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置该命令是否是远程请求命令
@param      BOOL isRemote 数据
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetIsRemote(BOOL isRemote)
{
	m_IsRemote = isRemote;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得该命令是否是远程请求命令
@param     void 
@retval     BOOL isRemote 数据
*/
//////////////////////////////////////////////////////////////////////////
BOOL CSCCommand::IsRemote()
{
	return m_IsRemote;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置该命令是否传输成功
@param      BOOL isReceived 数据
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetIsReceived(BOOL isReceived)
{
	m_IsReceived= isReceived;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得该命令是否已经传输成功
@param     void 
@retval     BOOL 
*/
//////////////////////////////////////////////////////////////////////////
BOOL CSCCommand::IsReceived()
{
	return m_IsReceived;
}

BOOL CSCCommand::IsNeedReply()
{
	return m_NeedReply;
}

void CSCCommand::SetIsNeedReply(BOOL needReply)
{
	m_NeedReply = needReply;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置响应命令

@param      CSCResponseCommand* command响应命令

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetReplyCommand(CSCResponseCommand* command)
{
	m_ReplyCommand = command;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      取得响应命令

@param      无

@retval  CSCResponseCommand*   响应命令

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCResponseCommand* CSCCommand::GetReplyCommand()
{
	return m_ReplyCommand;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      执行命令
@param      无    
@retval     0
*/
//////////////////////////////////////////////////////////////////////////
long CSCCommand::ExecuteCommand()
{
	if(!m_IsRemote)
	{
		theClient.Send(this);
	}
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      是否是合法命令代码
@param      无    
@retval     0
*/
//////////////////////////////////////////////////////////////////////////
long CSCCommand::IsValidCommand()
{
	return SP_SUCCESS;
}

