#pragma once
#include <assert.h>

typedef struct 
{
	//���ں�
	DWORD m_dwPort;
	//������
	DWORD m_dwBaudRate;
	//ֹͣλ
	DWORD m_dwStopBit;
	//��żУ��
	//DWORD m_dwParity;
	char m_chrParity;
	//�ֽڴ�С
	BYTE m_bytByteSize;
	//�������뻺���С
	DWORD m_dwInputSize;
	//������������С
	DWORD m_dwOutputSize;
	//���ھ��
	HANDLE m_hCommHandle;
	//�Ƿ�ͬ��
	BOOL m_bSync;
}UDT_PortParam;

class  CSerialPort 
{
public:
	CSerialPort(UDT_PortParam udtPortParam);
	CSerialPort();
	virtual ~CSerialPort();

public:
	//��ȡ�������ò���
	UDT_PortParam GetParam();
	//���ô�������
	void SetParam(UDT_PortParam udtParam);
	//�򴮿�д����
	DWORD Write(const char* pszBuffer, DWORD dwBufferLen);
	//�Ӵ��ڶ�����
	DWORD Read(char* pszBuffer, DWORD dwBufferLen, DWORD dwWaitTime = 20);
	//�رմ���
	BOOL Close();
	//�򿪴���
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
