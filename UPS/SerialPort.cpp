
// SerialPort.cpp: implementation of the CSerialPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SerialPort.h"
#include "BomInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialPort::CSerialPort()
{
	m_udtPortParam.m_dwBaudRate = 9600;
	m_udtPortParam.m_dwPort = 0;
	m_udtPortParam.m_bytByteSize = 8;
	m_udtPortParam.m_dwStopBit = 1;
	m_udtPortParam.m_chrParity = 'N';
	m_udtPortParam.m_dwInputSize = 1024;
	m_udtPortParam.m_dwOutputSize = 1024;
	m_udtPortParam.m_hCommHandle = INVALID_HANDLE_VALUE;
	m_udtPortParam.m_bSync = TRUE;
}

CSerialPort::~CSerialPort()
{
	Close();
}

CSerialPort::CSerialPort(UDT_PortParam udtPortParam)
{
	m_udtPortParam = udtPortParam;
}

/***************************************************************************
*功能： 打开串口
*参数： 无
*返回值：执行成功返回TRUE，否则返回FALSE
***************************************************************************/
BOOL CSerialPort::Open(int m_dwPort,int BaudRate)
{
	if(theBOM_INFO.IsUPSUsed())
	{
		assert(m_dwPort >= 1 && m_dwPort <= 20);	
		m_udtPortParam.m_dwPort = m_dwPort;
		//组合成欲打开的串口名
		memset(szPort, '\0', 20);
		sprintf_s(szPort,20, "COM%d", m_dwPort);
		//TRACE1("\n串口为：%s", szPort);
		if(IsOpen())
			Close();

		m_udtPortParam.m_hCommHandle = CreateFileA(
			szPort, 
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, //(m_udtPortParam.m_bSync) ? FILE_ATTRIBUTE_NORMAL : FILE_FLAG_OVERLAPPED,
			NULL
			);

		if(m_udtPortParam.m_hCommHandle == INVALID_HANDLE_VALUE)
			return FALSE;

		//设置缓冲区
		if(!::SetupComm(m_udtPortParam.m_hCommHandle, m_udtPortParam.m_dwInputSize, m_udtPortParam.m_dwOutputSize))
		{

			Close();
			return FALSE; 
		}
		//设置超时时间
		COMMTIMEOUTS _CO;
		if(!::GetCommTimeouts(m_udtPortParam.m_hCommHandle, &_CO))
			return FALSE;
		_CO.ReadIntervalTimeout = 0xFFFFFFFF;
		_CO.ReadTotalTimeoutMultiplier = 0;
		_CO.ReadTotalTimeoutConstant = 0;
		_CO.WriteTotalTimeoutMultiplier = 0;
		_CO.WriteTotalTimeoutConstant = 2000;
		if(!::SetCommTimeouts(m_udtPortParam.m_hCommHandle, &_CO))
		{
			Close();
			return FALSE; 
		}

		//清空串口缓冲区
		if(!::PurgeComm(m_udtPortParam.m_hCommHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
		{
			Close();
			return FALSE; 
		}
		m_udtPortParam.m_dwBaudRate = BaudRate;
		return Setup();

	}
	return true;	
}

/*********************************************************************************
*功能： 关闭串口
*参数： 无
*返回值：无
*********************************************************************************/
BOOL CSerialPort::Close()
{
	BOOL bRet = TRUE;
	if(IsOpen())
	{
		if(::CloseHandle(m_udtPortParam.m_hCommHandle)){
			bRet = TRUE;
			m_udtPortParam.m_hCommHandle = INVALID_HANDLE_VALUE;
		}	
		else{
			bRet = FALSE;
		}
	}
	return bRet;
}

/***************************************************************************
*功能： 读取串口
*参数： pszBuffer    数据存放缓冲区首地址
*        dwBufferLen 缓冲长度
*        dwWaitTime   等待时间，默认20毫秒
*返回值：执行失败返回0，否则为读出数据长度
****************************************************************************/
DWORD CSerialPort::Read(char *pszBuffer, DWORD dwBufferLen, DWORD dwWaitTime)
{
	assert(pszBuffer != NULL);

	if(!IsOpen()){
		//TRACE0("\n串口没有打开！");
		return 0;
	}

	COMSTAT Stat;
	DWORD dwError;

	//清除错误
	if(::ClearCommError(m_udtPortParam.m_hCommHandle, &dwError, &Stat) && dwError > 0){
		//清空输入缓冲区
		::PurgeComm(m_udtPortParam.m_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR);
		//	TRACE0("\n清除错误失败！");
		return 0;
	}
	//Sleep(500);

	pszBuffer[0] = '\0';

	unsigned long uReadLength = 0;
	//dwBufferLen = dwBufferLen - 1 > Stat.cbInQue ? Stat.cbInQue : dwBufferLen - 1;

	//同步读取
	if(m_udtPortParam.m_bSync){  
		if(!::ReadFile(m_udtPortParam.m_hCommHandle, pszBuffer, dwBufferLen, &uReadLength, NULL)){
			//			TRACE0("\n读取串口失败！");
			if(::GetLastError() != ERROR_IO_PENDING){
				uReadLength = 0;
			}
		}
		//		TRACE1("\nReadFile读出%d个字符！", uReadLength);

		TRACE(_T("CSerialPort::Read() >> result :uReadLength:%d\r\n"), uReadLength);
		TRACE(_T("CSerialPort::Read() >> result :pszBuffer:%s\r\n"), pszBuffer);
		//清空缓冲区
		::PurgeComm(m_udtPortParam.m_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR);
	}
	//异步
	else{
		OVERLAPPED _ReadOverlapped;
		memset(&_ReadOverlapped, 0 ,sizeof(_ReadOverlapped));
		_ReadOverlapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		assert(_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE);

		if(!::ReadFile(m_udtPortParam.m_hCommHandle, pszBuffer, dwBufferLen, &uReadLength, &_ReadOverlapped))
		{
			//结束异步IO
			if(::GetLastError() == ERROR_IO_PENDING)
			{
				WaitForSingleObject(_ReadOverlapped.hEvent, dwWaitTime);
				if(!::GetOverlappedResult(m_udtPortParam.m_hCommHandle, &_ReadOverlapped, &uReadLength, false))
				{
					//其他错误
					if(::GetLastError() == ERROR_IO_INCOMPLETE)
						uReadLength = 0;
				}
				else
					uReadLength = 0;
			}
		}
		if(_ReadOverlapped.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(_ReadOverlapped.hEvent);
	}

	pszBuffer[uReadLength] = '\0';
	if(dwBufferLen != uReadLength)
		return 1;
	else
		return 0;
}

/*********************************************************************************
*功能： 向串口写入数据
*参数： pszBuffer   欲写入数据首地址
*        dwBufferLen 欲写入数据长度
*返回值：执行失败返回0，否则为写入数据长度
**********************************************************************************/
DWORD CSerialPort::Write(const char *pszBuffer, DWORD dwBufferLen)
{
	assert(pszBuffer != NULL);

	if(!IsOpen())
		return FALSE;

	DWORD dwError;
	//清除错误
	if(::ClearCommError(m_udtPortParam.m_hCommHandle, &dwError, NULL) && dwError > 0)
		//清空输入缓冲区
		::PurgeComm(m_udtPortParam.m_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR);

	unsigned long uWriteLength = 0;

	//同步写入
	if(m_udtPortParam.m_bSync)
	{
		if(!::WriteFile(m_udtPortParam.m_hCommHandle, pszBuffer, dwBufferLen, &uWriteLength, NULL))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;
	}
	else //异步
	{
		OVERLAPPED _WriteOverlapped;
		memset(&_WriteOverlapped, 0 ,sizeof(_WriteOverlapped));
		_WriteOverlapped.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		assert(_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE);

		if(!::WriteFile(m_udtPortParam.m_hCommHandle, pszBuffer, dwBufferLen, &uWriteLength, &_WriteOverlapped))
			if(::GetLastError() != ERROR_IO_PENDING)
				uWriteLength = 0;

		if(_WriteOverlapped.hEvent != INVALID_HANDLE_VALUE)
			CloseHandle(_WriteOverlapped.hEvent);
	}
	if(dwBufferLen == uWriteLength)
		return 0;
	else
		return 1;
}

void CSerialPort::SetParam(UDT_PortParam udtParam)
{
	m_udtPortParam = udtParam;
}

UDT_PortParam CSerialPort::GetParam()
{
	return m_udtPortParam;
}

BOOL CSerialPort::IsOpen()
{
	return m_udtPortParam.m_hCommHandle != INVALID_HANDLE_VALUE;
}

/****************************************************************************
*功能： 设置波特率、停止位等
*参数： 无
*返回值：执行成功返回TRUE，否则返回FALSE
*****************************************************************************/
BOOL CSerialPort::Setup()
{
	BOOL bResult = FALSE;
	if(IsOpen())
	{
		DCB dcb;
		if(!::GetCommState(m_udtPortParam.m_hCommHandle, &dcb))
			return FALSE;
		char chrSet[50];
		sprintf_s(chrSet,50, "%d,%c,%d,%d", m_udtPortParam.m_dwBaudRate, m_udtPortParam.m_chrParity, m_udtPortParam.m_bytByteSize, m_udtPortParam.m_dwStopBit);
		if(BuildCommDCBA(chrSet, &dcb))
		{
			if(::SetCommState(m_udtPortParam.m_hCommHandle, &dcb))
				bResult = TRUE;
		}

	}
	if(!bResult)
		Close();
	return bResult;
}

BOOL CSerialPort::WaitingData(DWORD dwTimeout)
{
	BOOL bSuccess = FALSE;
	ASSERT(IsOpen());
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hEvent != NULL)
	{
		DWORD dwOldMask = 0, dwNewMask = 0;
		GetCommMask(m_udtPortParam.m_hCommHandle, &dwOldMask);
		SetCommMask(m_udtPortParam.m_hCommHandle, EV_RXCHAR);

		OVERLAPPED overlapped;
		overlapped.hEvent = hEvent;
		bSuccess = WaitCommEvent(m_udtPortParam.m_hCommHandle, &dwNewMask, &overlapped);
		if (bSuccess)
		{
			if (WaitForSingleObject(overlapped.hEvent, dwTimeout) == WAIT_OBJECT_0)
			{
				DWORD dwBytesTransferred;
				if (!GetOverlappedResult(m_udtPortParam.m_hCommHandle, &overlapped, &dwBytesTransferred, FALSE))
				{
					//if (GetLastError() == ERROR_IO_INCOMPLETE)
						bSuccess = FALSE;
				}
			}
			else
			{
				bSuccess = FALSE;
			}
		}
		else
		{
			DWORD dwError = GetLastError();
			TRACE(_T("CSerialPort::WaitingData() ->WaitCommEvent() >>error : %d"), dwError);
		}

		SetCommMask(m_udtPortParam.m_hCommHandle, dwOldMask);
		CloseHandle(hEvent);
	}

	return bSuccess;
}


int   CSerialPort::SynReadComData(char *pszBuffer, int nBufferLen)
{
	if (pszBuffer == NULL || nBufferLen == 0)
		return -1;

	int pos = 0;
	char *pBufTmp = pszBuffer;
	DWORD dwOldMask = 0, dwNewMask = 0;
	GetCommMask(m_udtPortParam.m_hCommHandle, &dwOldMask);
	SetCommMask(m_udtPortParam.m_hCommHandle, EV_RXCHAR);
	for (pos = 0; ;++pos)
	{
		if (WaitCommEvent(m_udtPortParam.m_hCommHandle, &dwNewMask, NULL))
		{
			DWORD nReadLen = 0;
			if (!::ReadFile(m_udtPortParam.m_hCommHandle, pBufTmp+pos, 1, &nReadLen, NULL))
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	SetCommMask(m_udtPortParam.m_hCommHandle, dwOldMask);

	TRACE(_T("CSerialPort::SynReadComData() >> result :pszBuffer:%s\r\n"), pszBuffer);
	TRACE(_T("m_com.Read() >> result : bufferlen = %d"), pos);

	return (pos == nBufferLen) ? 0 : 1;
}