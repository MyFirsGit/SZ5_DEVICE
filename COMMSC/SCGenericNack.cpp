
#include "stdafx.h"
#include "SCGenericNack.h"
#include "HeaderManager.h"
#include "scclient.h"
#include "scexception.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/*
@brief   NACK命令构造函数   

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCGenericNack::CSCGenericNack():CSCResponseCommand()
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
CSCGenericNack::~CSCGenericNack()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      设置NACK错误代码

@param     USHORT nackCode nackCode代码 

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
void CSCGenericNack::SetNACKCode(USHORT nackCode)
{
	this->nackCode = nackCode;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSCGenericNack::IsValidCommand()
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
long CSCGenericNack::ExecuteCommand()
{
	long lRet = -1;
	m_Header.bDataTransType = PROTOCAL_DATA;
	m_Header.nwTransId = CHeaderManager::AquireNewID(CMD_GENERIC_NACK);
	
	if(m_pContent!=NULL)
	{
		delete m_pContent;
		m_pContent = NULL;
	}
	m_pContent = new VARIABLE_DATA;
	m_pContent->nLen = sizeof(nackCode);
	m_pContent->lpData = new BYTE[m_pContent->nLen];
	LPBYTE content = m_pContent->lpData;
	memcpy(content,&nackCode,sizeof(USHORT));
	content+=sizeof(nackCode);
	theClient.Send(this);
	return SP_SUCCESS;
}
