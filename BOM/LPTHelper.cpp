#include "stdafx.h"
#include "LPTHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLPTHelper::CLPTHelper(CService &service):CServiceHelper(service)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CLPTHelper::~CLPTHelper()
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ʼ������

@param      none

@retval     long     0:�ɹ�  1:��ʼ��ʧ�� 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CLPTHelper::LPTOpenLPT()
{
	if(!theBOM_INFO.IsLPTUsed()){
		return 0;
	}
	OpenLPT pLPTCmd;
	long errCode = pLPTCmd.ExecuteCommand();
	if(errCode != 0){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
    return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����״ֵ̬

@param      (o)ULONG&    uLptStatusVal ����״ֵ̬

@retval     long  0:�ɹ�  1:��ȡʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CLPTHelper::LPTReadLPT(BYTE& uLptStatusVal)
{
	if(!theBOM_INFO.IsLPTUsed())
	{
		return 1;
	}
	ReadLPT pLPTCmd;
	long errCode = pLPTCmd.ExecuteCommand();
	if(errCode != 0){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	pLPTCmd.GetLPTResponse(uLptStatusVal);
	return errCode;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      �رղ���

@param      none

@retval     long  0:�ɹ�  1:��ȡʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CLPTHelper::LPTCloseLPT()
{
	if(!theBOM_INFO.IsLPTUsed())
	{
		return 0;
	}
	// �رղ���
	CloseLPT pLPTCmd;
	long errCode = pLPTCmd.ExecuteCommand();
	if(errCode != 0){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);
	}
	return errCode;
}


//****************************************************************************
// ȡ�ó�Ʊ���Ž���״̬
// function  : ȡ�ó�Ʊ���Ž���״̬
// input:		
// output:		
// return:		TickboxDoorStatus  �Ž�״̬
//
//****************************************************************************
DOORSTATUS CLPTHelper::GetTicketboxDoorStatus(){
	if(!theBOM_INFO.IsLPTUsed())
	{
		return DOORSTATUS::CLOSED;
	}
	DOORSTATUS status = DOORSTATUS::UNKNOWN;
	BYTE lptValue = 0;
	int ret = LPTReadLPT(lptValue);
	if(ret == 0)
	{
		status = lptValue == theBOM_INFO.GetClosedDoorStatusValue() ? DOORSTATUS::CLOSED : DOORSTATUS::OPENED;
	}
	// ��ȡʧ��
	else if(ret == 2){
		throw CLPTHardwareException(CLPTHardwareException::OTHER_ERR,_T(__FILE__),__LINE__);	
	}
	return status;
}
