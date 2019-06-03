#pragma once
#include "UPSExp.h"
#include "UPSDef.h"
#include "UPSLog.h"
#include "UPSException.h"
#include "SerialPort.h"
#include "Command.h"

/**
@brief  ����UPS����
*/
class UPS_API CUPSCommand : public CCommand
{
public:
	CUPSCommand();   
	~CUPSCommand();
	virtual bool Open(int,int);
	virtual bool Close();
	virtual int GetUPSBatteryInfo(_tagUPSBatteryInfo &);
protected:
	virtual long ExecuteCommand();
	virtual long IsValidCommand();
	static CSerialPort m_com;
private:
	int ParseBatteryInfo(const char *pInfo, int nInfoLen, _tagUPSBatteryInfo &info);
	bool	IsStartFlag(const char *pInfo);
	bool	IsEndFlag(const char *pInfo);
	double GetFloatData(const char*pInfo, int nLen);
	int	   GetIntData(const char*pInfo, int nLen);
};


/**
@brief  ����UPS����--�򿪴���
*/
class UPS_API CUPSOpen : public CUPSCommand
{
public:
	CUPSOpen(int, int);   
	~CUPSOpen(void);
	long ExecuteCommand(void);
private:
	int m_nPort;                       //���ڶ˿ں�
	int m_nBaud;                       //������
};

/**
@brief  ����UPS����--����UPS��Ϣ
*/
class UPS_API CUPSRead : public CUPSCommand
{
public:
	CUPSRead();
	~CUPSRead();
	long ExecuteCommand();           //ִ������
	void GetUPSInfo(_tagUPSBatteryInfo &info);
private:
	_tagUPSBatteryInfo m_sUPSInfo;       //UPS��Ϣ
};


/**
@brief   ����UPS����--�رմ���
*/
class UPS_API CUPSClose : public CUPSCommand
{
public:
	CUPSClose();
	~CUPSClose();
	long ExecuteCommand();
};