
#include "stdafx.h"
#include "SCEnquireLink.h"
#include "scclient.h"
#include "HeaderManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCEnquireLink::CSCEnquireLink():CSCCommand()
{
	
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCEnquireLink::~CSCEnquireLink()
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
long CSCEnquireLink::IsValidCommand()
{
	return SP_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
/*
@brief      ִ������

@param      

@retval     

@exception  
*/
//////////////////////////////////////////////////////////////////////////
long CSCEnquireLink::ExecuteCommand()
{
	long lRet = -1;
	m_Header.bDataTransType = PROTOCAL_DATA;
	m_Header.nwTransId = CHeaderManager::AquireNewID(CMD_ENQUIRE_LINK);
	m_Header.bCrcType = 0x00;//������CRC
	
	theClient.Send(this);
	return SP_SUCCESS;
}
