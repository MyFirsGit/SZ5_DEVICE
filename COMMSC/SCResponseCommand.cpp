#include "stdafx.h"
#include "SCResponseCommand.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief    响应命令基类构造函数  

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCResponseCommand::CSCResponseCommand():CSCCommand()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      析构函数

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCResponseCommand::~CSCResponseCommand()
{

}

//////////////////////////////////////////////////////////////////////////
/**
@brief      设置回复的NTID
@param      NETWORK_TRANS_ID 
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCResponseCommand::SetResponseNTID(NETWORK_TRANS_ID &ntid)
{
	responseNTID = ntid;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      取得回复的NTID
@param      无
@retval     NETWORK_TRANS_ID
*/
//////////////////////////////////////////////////////////////////////////
NETWORK_TRANS_ID& CSCResponseCommand::GetResponseNTID()
{
	return responseNTID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      序列化电文内容体
@param      LPVARIABLE_DATA 电文内容
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCResponseCommand::SerializeBody(VARIABLE_DATA &buffer)
{
	if(buffer.lpData!=NULL)
	{
		delete buffer.lpData;
		buffer.lpData = NULL;
	}
	buffer.nLen = 0;
	buffer.lpData = new BYTE[LEN_NTID+m_pContent->nLen];
	responseNTID.Serialize(buffer.lpData);
	if(m_pContent->nLen>0)
	{
		memcpy(buffer.lpData+LEN_NTID,m_pContent->lpData,m_pContent->nLen);
	}
	buffer.nLen = m_pContent->nLen+LEN_NTID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      反序列化电文内容体
@param      LPVARIABLE_DATA 电文内容
@retval     无
*/
//////////////////////////////////////////////////////////////////////////
void CSCResponseCommand::DeserializeBody(LPBYTE bodyData, UINT length)
{
	responseNTID.Deserialize(bodyData);
	if(m_pContent!=NULL)
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	m_pContent = new VARIABLE_DATA;
	if(length>LEN_NTID)
	{
		m_pContent->lpData = new BYTE[length-LEN_NTID];
		memcpy(m_pContent->lpData,bodyData+LEN_NTID,length-LEN_NTID);
		m_pContent->nLen =length-LEN_NTID;
	}
	
}