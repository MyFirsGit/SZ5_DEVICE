#include "stdafx.h"
#include "SCResponseCommand.h"

//////////////////////////////////////////////////////////////////////////
/*
@brief    ��Ӧ������๹�캯��  

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCResponseCommand::CSCResponseCommand():CSCCommand()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

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
@brief      ���ûظ���NTID
@param      NETWORK_TRANS_ID 
@retval     ��
*/
//////////////////////////////////////////////////////////////////////////
void CSCResponseCommand::SetResponseNTID(NETWORK_TRANS_ID &ntid)
{
	responseNTID = ntid;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ȡ�ûظ���NTID
@param      ��
@retval     NETWORK_TRANS_ID
*/
//////////////////////////////////////////////////////////////////////////
NETWORK_TRANS_ID& CSCResponseCommand::GetResponseNTID()
{
	return responseNTID;
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���л�����������
@param      LPVARIABLE_DATA ��������
@retval     ��
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
@brief      �����л�����������
@param      LPVARIABLE_DATA ��������
@retval     ��
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