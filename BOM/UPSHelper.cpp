#include "stdafx.h"
#include "UPSHelper.h"

//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      (i)CService& service

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
CUPSHelper::CUPSHelper(CService &service):CServiceHelper(service)
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
CUPSHelper::~CUPSHelper()
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
long CUPSHelper::UPSOpen()
{
	int port = theBOM_INFO.GetUPSCommPort();
	int baud = theBOM_INFO.GetUPSBaudRate();
	CUPSOpen pUPSCmd(port, baud);
	long errCode = pUPSCmd.ExecuteCommand();
	if(errCode != UPS_SUCCESSED){
		throw CUPSException(CUPSException::ERROR_UPS_OPEN_COM, _T(__FILE__), __LINE__);
	}
	return errCode;
}


//////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡ����״ֵ̬

@param      (o)_tagUPSBatteryInfo& info ��ȡUPS��Ϣ

@retval     long  0:�ɹ�  1:��ȡʧ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CUPSHelper::UPSRead(_tagUPSBatteryInfo &info)
{
	CUPSRead pUPSCmd;
	long errCode = pUPSCmd.ExecuteCommand();
	if(errCode != UPS_SUCCESSED){
		throw CUPSException(CUPSException::ERROR_UPS_READ_COM, _T(__FILE__), __LINE__);
	}
	pUPSCmd.GetUPSInfo(info);
	return errCode;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      �رղ���

@param      none

@retval     long     0:�ɹ�  1:��ʼ��ʧ�� 

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
long CUPSHelper::UPSClose()
{
	// �رղ���
	CUPSClose pUPSCmd;
	long errCode = pUPSCmd.ExecuteCommand();
	if(errCode != UPS_SUCCESSED){
		throw CUPSException(CUPSException::ERROR_UPS_CLOSE_COM, _T(__FILE__), __LINE__);
	}
	return errCode;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡʣ������ٷֱ�

@param      none

@retval     DWORD     ʣ������ٷֱ�

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DWORD CUPSHelper::GetUPSBatteryCapacity()
{
	_tagUPSBatteryInfo mUPSInfo;
	long errCode = UPSRead(mUPSInfo);
	return mUPSInfo.dBatteryCapacity;
}

/////////////////////////////////////////////////////////////////////////
/**
@brief      ��ȡʣ��ʱ��

@param      none

@retval     DWORD     ʣ��ʱ��(��λ������)

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
DWORD CUPSHelper::GetUPSLeftStandbyTime()
{
	_tagUPSBatteryInfo mUPSInfo;
	long errCode = UPSRead(mUPSInfo);
	return mUPSInfo.dwLeftStandbyTime/60;
}
