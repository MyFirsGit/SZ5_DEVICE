#pragma once
#include "Command.h"
#include "CoinShutterCom.h"
#include "CoinShutterException.h"
#include "CoinShutterHardwareException.h"

#include "Sync.h"
#include "DebugLog.h"

#ifndef theCoinShutter_TRACE
	#define theCoinShutter_TRACE CFileLog::GetInstance(_T("CoinShutte_"))
#endif

#define  E_NO_CS_ERROR	0

/**
 @brief  theCoinShutter_TRACE�����
*/
class CS_API CCoinShutterCommand : public CCommand
{
public:
	CCoinShutterCommand(void){} 
    ~CCoinShutterCommand(void){}
public:
	long ExecuteCommand() = 0;
};

/**
 @brief	ԭ��������򿪴���
*/
class CS_API CCSConnect : public CCoinShutterCommand
{
public:
	CCSConnect(int nPort, int nBaud);
	~CCSConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// �˿ں�
	int m_nBaud;						// ������
private:
	long ConnectFunction();

};

/**
 @brief	ԭ��������رմ���
*/
class CS_API CCSClose : public CCoinShutterCommand
{
public:
	CCSClose();
	~CCSClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	ԭ���������ʼ��Ӳ��ģ��
*/
class CS_API CCSInit : public CCoinShutterCommand
{
public:
	CCSInit();
	~CCSInit();
public:
	long ExecuteCommand();
private:
	long InitFunction();
};

/**
 @brief	��λ��ʼ��
*/
class CS_API CCSReset : public CCoinShutterCommand
{
public:
	CCSReset();
	~CCSReset();
public:
	long ExecuteCommand();
private:
	long ResetFunction();
};

/**
 @brief	��ȡ������״̬
*/
class CS_API CCSGetStatus: public CCoinShutterCommand
{
public:
	CCSGetStatus();
	~CCSGetStatus();
public:
	long ExecuteCommand();
	long GetResponse(tShutterStatus* pSensorStatus);
private:
	long GetStatusFunction();
	tShutterStatus m_sSensorStatus;
};

/**
 @brief	��բ��
*/
class CS_API CCSOpenShutter : public CCoinShutterCommand
{
public:
	CCSOpenShutter();
	~CCSOpenShutter();
public:
	long ExecuteCommand();
private:
	long OpenShutterFunction();
};

/**
 @brief	�ر�բ��
*/
class CS_API CCSCloseShutter : public CCoinShutterCommand
{
public:
	CCSCloseShutter();
	~CCSCloseShutter();
public:
	long ExecuteCommand();
private:
	long CloseShutterFunction();
};