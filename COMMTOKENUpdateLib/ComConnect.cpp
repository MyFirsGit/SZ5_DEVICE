#include "stdafx.h"
#include "ComConnect.h"
#include "TOKENUpdateLib.h"
#include <stdio.h>

ComConnect::ComConnect()
{
}
ComConnect::~ComConnect()
{
}

//****************************************************************************
// �����Ӵ���
// function  : open
// input:     const char* Para ������������Ĳ���
// output:    int ret          ִ�н�� 0����������0������
//
// note:    ��ָ��������
//****************************************************************************
int ComConnect::open(TOKEN_CONNECT_PARAM Para,HANDLE& hCom)
{
	WCHAR buff[20];
	memset(buff,0,sizeof buff);
    _stprintf(buff, _TEXT("\\\\.\\COM%d"), Para.ComPort);
	hCom = CreateFile(buff,         //COM1��
		GENERIC_READ|GENERIC_WRITE, //�������д
		0,                          //��ռ��ʽ
		NULL,                       //���ð�ȫ�����Խṹ
		OPEN_EXISTING,              //�򿪶����Ǵ���
		0,                          //ͬ����ʽ
		NULL);
	if(INVALID_HANDLE_VALUE == hCom){
		return TOKENTRS_ERR_HANDLE;
	}  

	SetupComm(hCom,3000,3000);                 //���뻺����������������Ĵ�С����3000

	DCB dcb;
	if (FALSE == ::GetCommState(hCom, &dcb)){
		::CloseHandle(hCom);
		return TOKENTRS_ERR_HANDLE;
	}

	COMMTIMEOUTS TimeOuts;
	GetCommState(hCom,&dcb);

	// ����DCB����
	dcb.BaudRate=Para.Speed;               // ������
	dcb.ByteSize=8;                        // ÿ���ֽ���8λ
	dcb.fBinary = TRUE;						// ������ģʽ
	dcb.fParity = FALSE;					// ������żУ��
	dcb.StopBits = ONESTOPBIT;				// ֹͣλ(1)
	dcb.Parity = NOPARITY;				    // ��żУ�鷽��
	dcb.fAbortOnError = TRUE;               // �д�����ʱ��ֹ��д����
	dcb.fOutxCtsFlow = FALSE;				// ָ��CTS�Ƿ����ڼ�ⷢ�Ϳ��ơ���ΪTRUEʱCTSΪOFF�����ͽ������𡣣����������
	dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS��ΪOFF 
	dcb.fDtrControl = DTR_CONTROL_DISABLE;  // DTR��ΪOFF 
	dcb.fOutxDsrFlow = FALSE;				// ָ��DSR�Ƿ����ڼ�ⷢ�Ϳ��ơ���ΪTRUEʱDSRΪOFF�����ͽ�������
	dcb.fDsrSensitivity = FALSE;			// ����ֵΪTRUEʱDSRΪOFFʱ���յ��ֽڱ����� 

	// ���ô���״̬
	SetCommState(hCom,&dcb);

	memset(&TimeOuts, 0, sizeof(TimeOuts));     //�趨����ʱ


	TimeOuts.ReadTotalTimeoutMultiplier = 3;    //1000;
	TimeOuts.ReadTotalTimeoutConstant = 0;      //timeout for read operation
	TimeOuts.ReadIntervalTimeout = 20;          //MAXDWORD;

	TimeOuts.WriteTotalTimeoutConstant = 0;     //�趨д��ʱ
	TimeOuts.WriteTotalTimeoutMultiplier = 3;   //100;

	SetCommTimeouts(hCom,&TimeOuts);            //���ó�ʱ

	::PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );//��ջ�����

	return TOKENTRS_RES_OK;
}

//****************************************************************************
// �ر����Ӵ���
// function  : close
// intput:    int i           δʹ��
// output:    int ret         ִ�н�� 0����������0������
//
// note:    �ر�����
//****************************************************************************
int ComConnect::close(HANDLE& hCom)
{
	int ret(1);
	if(!hCom) return ret;
	if(CloseHandle(hCom)){
		ret = 0;
	}
	return ret;
}

//****************************************************************************
// ��ȡ���ݴ���
// function  : read
// input:     int max_sz      ��󳤶�
//            char* buff      ������
// output:    int ret         ��ȡ�������ݳ��� 0�����󣻷�0�����������ݳ���
//
//****************************************************************************
int ComConnect::read(int max_sz, LPBYTE buff,HANDLE& hCom)
{
	DWORD wCount = 0;      //��ȡ���ֽ���
	BOOL bReadStat(false);
	bReadStat = ::ReadFile(hCom, buff, max_sz, &wCount, NULL);	
	return wCount;
}

//****************************************************************************
// д�����ݴ���
// function  : write
// input:     int sz          ���ݳ���
//            char* buff      ������
// output:    int ret         д������ݳ��� 0�����󣻷�0��д������ݳ���
//
//****************************************************************************
int ComConnect::write(int sz, LPBYTE buff,HANDLE& hCom)
{
	DWORD dwBytesWrite = 0;
	// ��������	
	WriteFile(hCom,buff,sz,&dwBytesWrite,NULL);  
	return dwBytesWrite;
}

