/*
**	FILENAME			CSerialPort.cpp
**
**	PURPOSE				This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	12-11-1997
**
**	AUTHOR				Remon Spekreijse
**
**
*/

#include "stdafx.h"
#include "CommonDefine.h"
#include "SerialPort.h"
#include "DebugLog.h"

#include <assert.h>
#define theStatusDisplay_TRACE CFileLog::GetInstance(FILTER_STATUS_DISPLAY_LOG)

unsigned char       begin;
unsigned char       end; 
// 
//extern BYTE * ReceiveData;
BYTE* ReceiveData;
int ReceiveCounter;
BOOL StartReceive, StartRevMsg;
int FWVer;
BOOL ProgEn;

int totalbytes=0;
int  iFillSpace( char* pcontent);
char *mytrim(char *string);

//
// Constructor
//
CSerialPort::CSerialPort()
{
	m_hComm = NULL;

	// initialize overlapped structure members to zero
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_szWriteBuffer = NULL;

	m_bThreadAlive = FALSE;

	ReceiveData = new BYTE[1024];

	StartReceive = FALSE;
	ReceiveCounter = 0;
	
}

//
// Delete dynamic memory
//
CSerialPort::~CSerialPort()
{
//	do
//	{
//	 SetEvent(m_hShutdownEvent);
//	} while (m_bThreadAlive);

	if(m_Thread!=NULL) StopMonitoring();

	if(ReceiveData)
		delete(ReceiveData);
	delete[] ReceiveData;
	ReceiveData=NULL;
	TRACE("Thread ended\n");	
}

//
// Initialize the port. This can be port 1 to 8.
//
BOOL CSerialPort::InitPort(UINT portnr, 
						   UINT baud,
						   char parity, 
		                   UINT databits, 
						   UINT stopbits, 
						   DWORD dwCommEvents,
		                   UINT writebuffersize)
						      	         
{
//	assert(portnr > 0 && portnr < 9);

	// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);
		} while (m_bThreadAlive);
		TRACE("Thread ended\n");
	}

	// create events
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hWriteEvent != NULL)
		ResetEvent(m_hWriteEvent);
	m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// initialize the event objects
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section
	InitializeCriticalSection(&m_csCommunicationSync);
	

	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portnr;

	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;

	BOOL bResult = FALSE;
	char *szPort = new char[50];
	char *szBaud = new char[50];

	// now it critical!
	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	sprintf(szPort, "COM%d", portnr);
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);

	// get a handle to the port
	m_hComm = CreateFileA(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	// set the timeout values
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{						   
		if (SetCommMask(m_hComm, dwCommEvents))
		{
			if (GetCommState(m_hComm, &m_dcb))
			{
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
				m_dcb.fDtrControl = DTR_CONTROL_ENABLE;		// set DTR bit high!
				if (BuildCommDCBA(szBaud, &m_dcb))
				{
					if (SetCommState(m_hComm, &m_dcb))
						; // normal operation... continue
					else
						ProcessErrorMessage("SetCommState()");
				}
				else
					ProcessErrorMessage("BuildCommDCB()");
			}
			else
				ProcessErrorMessage("GetCommState()");
		}
		else
			ProcessErrorMessage("SetCommMask()");
	}
	else
		ProcessErrorMessage("SetCommTimeouts()");

	delete [] szPort;
	delete [] szBaud;

	// flush the port
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// release critical section
	LeaveCriticalSection(&m_csCommunicationSync);

	TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);

	return TRUE;
}

//
//  The CommThread Function.
//
UINT CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of CSerialPort class
	CSerialPort *port = (CSerialPort*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	port->m_bThreadAlive = TRUE;	
		
	// Misc. variables
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
		
	// Clear comm buffers at startup
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// begin forever loop.  This loop will run as long as the thread is alive.
	for (;;) 
	{ 

		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to 
		// be placed in a non-signeled state if there are no bytes available to be read,
		// or to a signeled state if there are bytes available.  If this event handle 
		// is set to the non-signeled state, it will be set to signeled when a 
		// character arrives at the port.

		// we do this for each port!

		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);

		if (!bResult)  
		{ 
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87:
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default:
				{
					// All other error codes indicate a serious error has
					// occured.  Process this error.
					port->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}
		}
		else
		{
			// If WaitCommEvent() returns TRUE, check to be sure there are
			// actually bytes in the buffer to read.  
			//
			// If you are reading more than one byte at a time from the buffer 
			// (which this program does not do) you will have the situation occur 
			// where the first byte to arrive will cause the WaitForMultipleObjects() 
			// function to stop waiting.  The WaitForMultipleObjects() function 
			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
			// as it returns.  
			//
			// If in the time between the reset of this event and the call to 
			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
			// to the signeled state. When the call to ReadFile() occurs, it will 
			// read all of the bytes from the buffer, and the program will
			// loop back around to WaitCommEvent().
			// 
			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
			// but there are no bytes available to read.  If you proceed and call
			// ReadFile(), it will return immediatly due to the async port setup, but
			// GetOverlappedResults() will not return until the next character arrives.
			//
			// It is not desirable for the GetOverlappedResults() function to be in 
			// this state.  The thread shutdown event (event 0) and the WriteFile()
			// event (Event2) will not work if the thread is blocked by GetOverlappedResults().
			//
			// The solution to this is to check the buffer with a call to ClearCommError().
			// This call will reset the event handle, and if there are no bytes to read
			// we can loop back through WaitCommEvent() again, then proceed.
			// If there are really bytes to read, do nothing and proceed.
		
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}	// end if bResult

		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		Event = WaitForMultipleObjects(3, port->m_hEventArray, FALSE, INFINITE);

		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the higest priority and be serviced first.

			 	port->m_bThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better.
//				AfxEndThread(100);
				exit(0);
				break;
			}
		case 1:	// read event
			{
				GetCommMask(port->m_hComm, &CommEvent);
				
				if (CommEvent & EV_RXCHAR)
					// Receive character event from port.
					ReceiveChar(port, comstat);
					
				break;
			}  
		case 2: // write event
			{
				// Write character event from port
				WriteChar(port);
				break;
			}

		} // end switch

	} // close forever loop

	return 0;
}

//
// start comm watching
//
BOOL CSerialPort::StartMonitoring()
{
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
		return FALSE;
	TRACE("Thread started\n");
	return TRUE;	
}

//
// Restart the comm thread
//
BOOL CSerialPort::RestartMonitoring()
{
	TRACE("Thread resumed\n");
	m_Thread->ResumeThread();
	return TRUE;	
}


//
// Suspend the comm thread
//
BOOL CSerialPort::StopMonitoring()
{
	TRACE("Thread suspended\n");
	m_Thread->SuspendThread(); 
	return TRUE;	
}


//
// If there is a error, give the right message
//
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	//sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	//MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete[] Temp;
}

//
// Write a character.
//
void CSerialPort::WriteChar(CSerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	ResetEvent(port->m_hWriteEvent);

	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);

	if (bWrite)
	{
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
        CStringA str=port->m_szWriteBuffer;
		bResult = WriteFile(port->m_hComm,							// Handle to COMM Port
							port->m_szWriteBuffer,					// Pointer to message buffer in calling finction
							port->m_size,										// strlen((char*)port->m_szWriteBuffer) Length of message to send
							&BytesSent,								// Where to store the number of bytes sent
							&port->m_ov);							// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage("WriteFile()");
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&port->m_csCommunicationSync);
		}
/*		CFile file;
		file.Open(".\\log.txt",CFile::modeCreate|CFile::modeWrite|
			CFile::modeNoTruncate);
		CString strcon="";
		CString strsub;
		for(int ii=0;ii<port->m_size;ii++)
		{
			strsub.Format("%02x",port->m_szWriteBuffer[ii]);
			strcon+=strsub;
		}
		file.SeekToEnd();
		file.Write(strcon.GetBuffer(0),strcon.GetLength());
		file.Write("\r\n",2);
		file.Close();*/
	} // end if(bWrite)

	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
									  &port->m_ov,		// Overlapped structure
									  &BytesSent,		// Stores number of bytes sent
									  TRUE); 			// Wait flag

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
		if (!bResult)  
		{
			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	} // end if (!bWrite)

	// Verify that the data size send equals what we tried to send
	if (BytesSent != strlen((char*)port->m_szWriteBuffer))
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
	}
}

//
// Character received. Inform the owner
//
void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff;

	for (;;) 
	{ 
		//防止死锁
		if(WaitForSingleObject(port->m_hShutdownEvent,0)==WAIT_OBJECT_0)
			return;		
		
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solutiion to be the most efficient way to do this.
		
		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}
						
		EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead)
		{
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
							   &RXBuff,				// RX Buffer Pointer
							   1,					// Read one byte
							   &BytesRead,			// Stores number of bytes read
							   &port->m_ov);		// pointer to the m_ov structure
			// deal with the error code 
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress 
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{
							// Another error has occured.  Process this error.
							port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)

		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
										  &port->m_ov,		// Overlapped structure
										  &BytesRead,		// Stores number of bytes read
										  TRUE); 			// Wait flag

			// deal with the error code 
			if (!bResult)  
			{
				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}  // close if (!bRead)
				
		LeaveCriticalSection(&port->m_csCommunicationSync);

		DataProcess(port, RXBuff);
	} // end forever loop

}

BYTE temp[128];
void CSerialPort::DataProcess(CSerialPort* port, BYTE ch)
{
	if(!StartReceive)
	{
		if (ch ==0xbb)
		{
			ReceiveData[0] = (BYTE)ch;
			ReceiveCounter = 1;
			StartReceive = TRUE;
			memset(temp,0,128);
			temp[0]=(BYTE)ch;
			totalbytes=0;
		}
		else
		{
			StartReceive = FALSE;
			totalbytes=0;
		}
	}
	else
	{
		ReceiveData[ReceiveCounter] = (BYTE)ch;
		temp[ReceiveCounter]=(BYTE)ch;
		ReceiveCounter++;

	    if(ReceiveCounter==7)
		{
			totalbytes = (BYTE)ch+8;
		}
		else if(ReceiveCounter==totalbytes-1)
		{
			//ReceiveData[ReceiveCounter]=(BYTE)end;////modified
   //         ReceiveData[ReceiveCounter+1]='\0';
			//ReceiveCounter = 0;
			//StartReceive = FALSE;			
			//ProtocolDesolve();
			//totalbytes=0;			
		}
	}
	return ;
}

void CSerialPort::AssemblePacket(CSerialPort* port)
{
}

BOOL CSerialPort::ProtocolDesolve(void)
{
	BYTE ChkSum;
	int i;
	if(ReceiveData[0]!=0xbb)
		return FALSE;

	ChkSum = ReceiveData[1];
	for(i=2; i<totalbytes; i++)
	{
		ChkSum ^= ReceiveData[i];		
	}

//	if(ChkSum != ReceiveData[i])
//		return FALSE;
	
	if(ReceiveData[1]==0x07)
	{
		if(ReceiveData[7]==0xff)
		{
			ProgEn = TRUE;
		}
		else
		{
			ProgEn = TRUE;
		}
		
		return TRUE;
	}
	else if(ReceiveData[1]==0x10)
	{
		FWVer = (ReceiveData[7]<<8) | ReceiveData[8];
		
		return TRUE;
	}
	return TRUE;
}


//
// Write a string to the port
//
void CSerialPort::WriteToPort(unsigned char* string,UINT size)
{		
 	assert(m_hComm != 0);
	m_size=size;
	memset(m_szWriteBuffer, 0, m_nWriteBufferSize);       
	memcpy(m_szWriteBuffer, string, size);           /////////modified
	// set event for write
	SetEvent(m_hWriteEvent);
}

//
// Return the device control block
//
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

//
// Return the communication event masks
//
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

//
// Return the output buffer size
//
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}




BYTE CSerialPort::BinToBcd(BYTE Bin)
{
	Bin = Bin % 100 ;
	return (Bin / 10) * 16 + Bin % 10;
}

//BCD to bin
BYTE CSerialPort::BcdToBin(BYTE Bcd)
{
	return (Bcd / 16) * 10 + Bcd % 16;
}


void CSerialPort::ClosePort()
{
// if the thread is alive: Kill
	if (m_bThreadAlive)
	{
	   MSG message;
	   while (m_bThreadAlive)
	   {
			if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				 ::TranslateMessage(&message);
				 ::DispatchMessage(&message);
			}
			SetEvent(m_hShutdownEvent);
	   }
	   TRACE("Thread ended/n");
	}

	if(m_szWriteBuffer != NULL)
	{
	   delete [] m_szWriteBuffer;
	   m_szWriteBuffer= NULL;
	}

	if(m_hComm)
	{
	   CloseHandle(m_hComm);
	   m_hComm = NULL;
	}
}
CLedDll::CLedDll()
{
	hInst = NULL;
	hInst = ::LoadLibraryA("LED_DLL.dll");
	if (hInst == NULL)
	{
		int err = GetLastError();
		CString sLogString(_T(""));
		sLogString.Format(_T("LED_DLL.dll加载失败！"));
		theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	}
	else
	{
		m_pLED_CommOpen = (pLED_CommOpen)GetProcAddress(hInst,"LED_CommOpen");
		m_pLED_CommClose = (pLED_CommClose)GetProcAddress(hInst,"LED_CommClose");
		m_pLED_Init = (pLED_Init)GetProcAddress(hInst,"LED_Init");
		m_pLED_Reset = (pLED_Reset)GetProcAddress(hInst,"LED_Reset");
		m_pLED_Download = (pLED_Download)GetProcAddress(hInst,"LED_Download");
		m_pLED_Display = (pLED_Display)GetProcAddress(hInst,"LED_Display");
		m_pLED_GetDevStatus = (pLED_GetDevStatus)GetProcAddress(hInst,"LED_GetDevStatus");
		m_pLED_GetVersion = (pLED_GetVersion)GetProcAddress(hInst,"LED_GetVersion");

	}
}
CLedDll::~CLedDll()
{

}
CLedDll gLedDll;
int  LED_CommOpen(UINT uiCommPort, UINT uiBaudRate)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_CommOpen:CommPort = %d,BaudRate = %d"),uiCommPort,uiBaudRate);
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_CommOpen(uiCommPort,uiBaudRate);
	sLogString.Format(_T("LED_CommOpen:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;
}
//11.4.2　初始化
int  LED_Init(tLEDDevReturn*  pDevStatus)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_Init"));
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_Init(pDevStatus);
	sLogString.Format(_T("LED_Init:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
//11.4.3　模块复位
int  LED_Reset(tLEDDevReturn*  pDevStatus)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_Reset"));
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_Reset(pDevStatus);
	sLogString.Format(_T("LED_Reset:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
//11.4.4　下载显示信息
//此命令用于将需要显示的信息下载到LED控制板上或保存在内存变量中
int  LED_Download(tLED_Download_Text_Para LED_Download_Text_Para,tLEDDevReturn *pDevStatus)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_Download:SerialNO=%d CN=%s EN=%s color=%d"),LED_Download_Text_Para.nSerialNO,CStringW(LED_Download_Text_Para.cCNText),CStringW(LED_Download_Text_Para.cENText),LED_Download_Text_Para.iFontColor);
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	iFillSpace( LED_Download_Text_Para.cCNText);
	iFillSpace(LED_Download_Text_Para.cENText);
	//iFillSpace( LED_Download_Text_Para.cENText);
	lRet = gLedDll.m_pLED_Download(LED_Download_Text_Para,pDevStatus);
	sLogString.Format(_T("LED_Download:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
//11.4.5　显示信息
//此命令用于根据选择的序号将保存在LED控制板上或内存变量中的字符串内容进行显示。
//要求显示内容切换时，需立即显示
//UINT	nSerialNO	In	需显示的数据编号。与tLED_Download_Text_Para:: nSerialNO对应。
int  LED_Display(UINT nSerialNO, tLEDDevReturn * pDevStatus)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_Display:SerialNO=%d"),nSerialNO);
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_Display(nSerialNO,pDevStatus);
	sLogString.Format(_T("LED_Display:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
//11.4.6　获取模块状态
int  LED_GetDevStatus(tLEDModuleStatus *pModuleStatus,tLEDDevReturn *pDevStatus)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_GetDevStatus"));
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_GetDevStatus(pModuleStatus,pDevStatus);
	sLogString.Format(_T("LED_GetDevStatus:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
//11.4.7　获取模块驱动版本信息
int LED_GetVersion(char* cFirmWareVersion, char* cDriverVersion, tLEDDevReturn *pDevStatus)
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_GetVersion"));
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_GetVersion(cFirmWareVersion,cDriverVersion,pDevStatus);
	sLogString.Format(_T("LED_GetVersion:lRet = %d firmWareV =%s DriverV=%s"),lRet,cFirmWareVersion,cDriverVersion);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
//11.4.8　通信关闭
int LED_CommClose()
{
	CString sLogString(_T(""));
	sLogString.Format(_T("LED_CommClose"));
	theStatusDisplay_TRACE->WriteData(_T(">")+sLogString,NULL,0);
	int lRet = LED_NO_ERROR;
	lRet = gLedDll.m_pLED_CommClose();
	sLogString.Format(_T("LED_CommClose:lRet = %d"),lRet);
	theStatusDisplay_TRACE->WriteData(_T("<")+sLogString,NULL,0);
	return lRet;

}
int  iFillSpace( char* pcontent)
{
	int imaxbyte=20;
	int itotalbyte=0;
	int ibyteused=0;

	mytrim(pcontent);

	wchar_t* pWideContent=NULL;
	USES_CONVERSION;
	pWideContent=A2W(pcontent);
	;

	char bthalfscreen[5* 20];
	for (int i=0;i<100;i++)
		bthalfscreen[i]=' ';

	int istrlen=wcslen(pWideContent);
	int iloop1=0;
	int ilastTotalbyte=0;
	for(;iloop1<istrlen;iloop1++)
	{				
		//循环处理每个字符
		byte* pbt=(byte*)(pWideContent+iloop1);
		DWORD dwresult=*pbt * *(pbt+1);
		if(0!=dwresult)
		{
			itotalbyte+=2;
		}else
		{
			itotalbyte++;
		}				
		if(itotalbyte<=imaxbyte)
		{
			ibyteused++;//这样可以统计协议中一共多少个字符,并且忽略中英文
		}else
		{
			//每一幕超长的内容被截断
			if(ilastTotalbyte<imaxbyte)
			{
				//对于之前不够10个的情况,依然需要补相对应的空格,例如之前如果是9个空格,那么还要补一个
				itotalbyte=ilastTotalbyte;
			}else
			{
				itotalbyte=imaxbyte;
			}					
			break;
		}
		ilastTotalbyte=itotalbyte;
	}


	//计算处理一下居中问题
	int ispaceNeed=(imaxbyte-itotalbyte);//确定需要补多少空格
	int istartIndex=(imaxbyte-itotalbyte)%2;
	if(istartIndex!=0)
	{
		//确保优先在前方补空格
		istartIndex=(imaxbyte-itotalbyte)/2+1;
	}else
	{
		istartIndex=(imaxbyte-itotalbyte)/2;
	}
	if(istartIndex!=0)
	{
		memmove(bthalfscreen+istartIndex,pcontent,ibyteused*2);
	}else
	{
	}
	int icopyNum=0;//确定实际应该拷贝多少字节，拷贝ibyteused的数目加上前面ispaceNeed
	icopyNum=ibyteused*2+ispaceNeed;
	memmove(pcontent,bthalfscreen,icopyNum);

	return 0;
}

char *mytrim(char *string)
{
	char *tmp = string;
	int j=0;
	for(int i=0;i<strlen(string);i++)
	{
		if(string[i] == ' ')
		{
			//skip
		}else{
			tmp[j] = string[i];
			j++;
		}
	}
	tmp[j]=0;
	return tmp;
}