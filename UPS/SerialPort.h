#pragma once
#include <assert.h>

typedef struct 
{
	//串口号
	DWORD m_dwPort;
	//波特率
	DWORD m_dwBaudRate;
	//停止位
	DWORD m_dwStopBit;
	//奇偶校验
	//DWORD m_dwParity;
	char m_chrParity;
	//字节大小
	BYTE m_bytByteSize;
	//串口输入缓冲大小
	DWORD m_dwInputSize;
	//串口输出缓冲大小
	DWORD m_dwOutputSize;
	//串口句柄
	HANDLE m_hCommHandle;
	//是否同步
	BOOL m_bSync;
}UDT_PortParam;

class  CSerialPort 
{
public:
	CSerialPort(UDT_PortParam udtPortParam);
	CSerialPort();
	virtual ~CSerialPort();

public:
	//获取串口配置参数
	UDT_PortParam GetParam();
	//设置串口配置
	void SetParam(UDT_PortParam udtParam);
	//向串口写数据
	DWORD Write(const char* pszBuffer, DWORD dwBufferLen);
	//从串口读数据
	DWORD Read(char* pszBuffer, DWORD dwBufferLen, DWORD dwWaitTime = 20);
	//关闭串口
	BOOL Close();
	//打开串口
	BOOL Open(int m_dwPort,int BaudRate);

	BOOL WaitingData(DWORD dwTimeout = INFINITE);

	int  SynReadComData(char *pszBuffer, int nBufferLen);

protected:
	char szPort[20];

private:
	BOOL IsOpen();
	BOOL Setup();

private:
	UDT_PortParam m_udtPortParam;
};
