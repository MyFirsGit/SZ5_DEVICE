#include "stdafx.h"
#include "SCConnectRequest.h"
#include "HeaderManager.h"
#include "scclient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/*
@brief     ������֤�����캯�� 

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CSCConnectRequest::CSCConnectRequest():CSCCommand()
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
CSCConnectRequest::~CSCConnectRequest()
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
long CSCConnectRequest::IsValidCommand()
{
	return SP_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ������֤����
@param      ��
@retval     long     0 �ɹ�   < 0 ʧ��
*/
//////////////////////////////////////////////////////////////////////////
long CSCConnectRequest::ExecuteCommand()
{
	long lRet = -1;
	m_Header.bDataTransType = PROTOCAL_DATA;
	//m_Header.nwTransId = CHeaderManager::AquireNewID(CMD_CONNECT_REQUEST);

	theClient.Send(this);
	
	return SP_SUCCESS;
}


