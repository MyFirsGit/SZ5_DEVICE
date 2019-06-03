#include "stdafx.h"
#include "PrinterCommand.h"
#include "PrinterException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
/**
@brief    �����캯��

@param      (i)int nPort ���ڵĶ˿ں�
@param      (i)int nBaud ���ڵĲ�����
@param      (i)int nSize �ֳ�

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

CPrinterConnect::CPrinterConnect(int nPort,int nBaud ,int nSize)
{
	this->m_nBaud = nBaud;
	this->m_nPort = nPort;
	this->m_nSize = nSize;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��������

@param      (i)��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////

CPrinterConnect::~CPrinterConnect(void)
{
}

//////////////////////////////////////////////////////////////////////////
/**
@brief      �򿪴��ڣ���������ز���

@param      (i)��

@retval      int   \n
0:�ɹ�  ��0:ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int CPrinterConnect::ConnectFunction(){

	char szPort[20];
	int iRet = 1;

	memset(szPort,'\0',20);
	::sprintf_s(szPort,20,"COM%d",this->m_nPort);

	iRet = CheckBaudValid(m_nBaud);
	if ( iRet != 0 ) {
		return iRet;
	}
	if(m_hPrinter != NULL){
		if (!CloseHandle(m_hPrinter)) {
			iRet = CPrinterException::ERROR_Printer_CLOSE_COM;
			m_hPrinter = NULL;
			return iRet;
		}
	}
	//�򿪴���
	m_hPrinter = ::CreateFileA(szPort,GENERIC_READ | GENERIC_WRITE ,
		0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,NULL);

	if(m_hPrinter == NULL){
		iRet = CPrinterException::ERROR_Printer_OPEN_COM;
		return iRet;
	}

	if(m_hPrinter == INVALID_HANDLE_VALUE){
		iRet = CPrinterException::ERROR_Printer_OPEN_ERROR;
		m_hPrinter = NULL;
		return iRet;
	}

	COMMTIMEOUTS CommTimeOuts;
	if(!::GetCommTimeouts(m_hPrinter, &CommTimeOuts))
	{
		iRet = CPrinterException::ERROR_Printer_SETTIME ;
		CloseHandle(m_hPrinter);
		m_hPrinter = NULL;
		return iRet;
	}
	//���ó�ʱ
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutConstant = 2000;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.WriteTotalTimeoutConstant = 2000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;

	if(!SetCommTimeouts(m_hPrinter,&CommTimeOuts)){
		iRet = CPrinterException::ERROR_Printer_SETTIME ;
		CloseHandle(m_hPrinter);
		m_hPrinter = NULL;
		return iRet;
	}
	//��մ��ڻ�����
	if(!::PurgeComm(m_hPrinter, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
	{
		iRet = CPrinterException::ERROR_Printer_SETTIME ;
		CloseHandle(m_hPrinter);
		m_hPrinter = NULL;
		return iRet;
	}
	//�õ����ڵ�dcb
	DCB dcb;
	if(!::GetCommState(m_hPrinter,&dcb)){
		iRet = CPrinterException::ERROR_Printer_GETSTATE;
		CloseHandle(m_hPrinter);
		m_hPrinter = NULL;
		return iRet;
	}

	TCHAR chrSet[50];
	swprintf_s(chrSet,50, _T("%d,%s,%d,%d"), m_nBaud, _T("N"), m_nSize, 1);
	BuildCommDCB(chrSet, &dcb);

	//���ô���DCB
	if(!::SetCommState(m_hPrinter,&dcb)){
		iRet = CPrinterException::ERROR_Printer_SETSTATE;
		CloseHandle(m_hPrinter);
		m_hPrinter = NULL;
		return iRet;
	}
	iRet = 0;
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ִ������

@param      (i)��

@retval      long    \n
0:�ɹ�   ��0 : ʧ��

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////

long CPrinterConnect::ExecuteCommand(void){
	long iRet=0;
	iRet = this->ConnectFunction();
	this->OnComplete(iRet);
	return iRet;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      ��鲨���ʵ�ֵ�Ƿ�Ϸ�

@param      (i)int nBaud ������

@retval      int    \n
0: �Ϸ�   ��0 : ���Ϸ� 

@exception   ��
*/
//////////////////////////////////////////////////////////////////////////
int  CPrinterConnect::CheckBaudValid(int nBaud){
	int iRet = 0;
	if ( nBaud % 4800 != 0 ) {
		iRet = CPrinterException::ERROR_Printer_BAUD_INVALID;
	}
	return iRet;
}
