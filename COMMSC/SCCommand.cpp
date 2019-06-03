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
@brief      CSCCommands���캯��
@param      ��
@retval     ��
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
@brief      CSCCommands��������
@param      ��
@retval     ��
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
@brief      ����ͷ������
@param      CMD_HEADER  ͷ������
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetHeader(CMD_HEADER *pHeader)
{
	memcpy(&m_Header, pHeader, sizeof CMD_HEADER);
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�õ���ͷ��
@param      CMD_HEADER&   ͷ������
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
CMD_HEADER& CSCCommand::GetHeader()
{
	return m_Header;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      �������ݸ���
@param      LPVARIABLE_DATA  body����
@retval     ��
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
@brief      �������ݸ���
@param      LPBYTE body����
@param      UINT length ����
@retval     ��
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
@brief      ���л�����������
@param      LPVARIABLE_DATA ��������
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SerializeBody(VARIABLE_DATA& buffer)
{
	// �������ݰ�������
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
@brief      �����л�����������
@param      LPVARIABLE_DATA ��������
@retval     ��
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
@brief      ���ø������Ƿ���Զ����������
@param      BOOL isRemote ����
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetIsRemote(BOOL isRemote)
{
	m_IsRemote = isRemote;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ø������Ƿ���Զ����������
@param     void 
@retval     BOOL isRemote ����
*/
//////////////////////////////////////////////////////////////////////////
BOOL CSCCommand::IsRemote()
{
	return m_IsRemote;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���ø������Ƿ���ɹ�
@param      BOOL isReceived ����
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetIsReceived(BOOL isReceived)
{
	m_IsReceived= isReceived;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ø������Ƿ��Ѿ�����ɹ�
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
@brief      ������Ӧ����

@param      CSCResponseCommand* command��Ӧ����

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCCommand::SetReplyCommand(CSCResponseCommand* command)
{
	m_ReplyCommand = command;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ȡ����Ӧ����

@param      ��

@retval  CSCResponseCommand*   ��Ӧ����

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCResponseCommand* CSCCommand::GetReplyCommand()
{
	return m_ReplyCommand;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������
@param      ��    
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
@brief      �Ƿ��ǺϷ��������
@param      ��    
@retval     0
*/
//////////////////////////////////////////////////////////////////////////
long CSCCommand::IsValidCommand()
{
	return SP_SUCCESS;
}

