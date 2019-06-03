
#include "stdafx.h"
#include "SCGenericAck.h"
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
@brief      ACK命令构造函数

@param      无

@retval     无

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
CSCGenericAck::CSCGenericAck():CSCResponseCommand()
{
	//m_pContent = new VARIABLE_DATA;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief     析构函数 

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
CSCGenericAck::~CSCGenericAck()
{

}

//////////////////////////////////////////////////////////////////////////
/*
@brief      执行命令

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSCGenericAck::ExecuteCommand()
{
	long lRet = -1;
	m_Header.bDataTransType = PROTOCAL_DATA;
	m_Header.nwTransId = CHeaderManager::AquireNewID(CMD_GENERIC_ACK);
	theClient.Send(this);
	return SP_SUCCESS;
}

long CSCGenericAck::IsValidCommand()
{
	return SP_SUCCESS;
}
