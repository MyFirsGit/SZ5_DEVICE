/*
**	FILENAME			CSerialPort.h
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

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__
#pragma once
#include "LEDApi.h"

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  

class CSerialPort
{														 
public:
	// contruction and destruction
	CSerialPort();
	virtual		~CSerialPort();

	// port initialisation											
	BOOL InitPort(UINT portnr = 2, UINT baud = 38400, char parity = 'N', UINT databits = 8, UINT stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR | EV_CTS,UINT nBufferSize = 128);

	// start/stop comm watching
	BOOL		StartMonitoring();
	BOOL		RestartMonitoring();
	BOOL		StopMonitoring();

	DWORD		GetWriteBufferSize();
	DWORD		GetCommEvents();
	DCB			GetDCB();

	void		WriteToPort(unsigned char* string,UINT size);
	void		ClosePort();

public:

protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);
	static UINT	CommThread(LPVOID pParam);
	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);
	static void	WriteChar(CSerialPort* port);
	static void	AssemblePacket(CSerialPort* port);
	static void	DataProcess(CSerialPort* port, BYTE ch);
	static BOOL ProtocolDesolve(void);

	BYTE BinToBcd(BYTE Bin);
	BYTE BcdToBin(BYTE Bcd);


	// 串口接收数据处理
	// thread
	CWinThread*			m_Thread;

	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;
	BOOL				m_bThreadAlive;

	// handles
	HANDLE				m_hShutdownEvent;
	HANDLE				m_hComm;
	HANDLE				m_hWriteEvent;
	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed. 
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;
	COMMTIMEOUTS		m_CommTimeouts;
	DCB					m_dcb;

	// owner window
	CWnd*				m_pOwner;

	// misc
	UINT				m_nPortNr;
	char*				m_szWriteBuffer;
	UINT                m_size;
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;
};

//11.4.1　通信建立
typedef int(__stdcall* pLED_CommOpen)(UINT uiCommPort, UINT uiBaudRate);
//11.4.2　初始化
typedef int(__stdcall* pLED_Init)(tLEDDevReturn*  pDevStatus);
//11.4.3　模块复位
typedef int(__stdcall* pLED_Reset)(tLEDDevReturn*  pDevStatus);
//11.4.4　下载显示信息
//此命令用于将需要显示的信息下载到LED控制板上或保存在内存变量中
typedef int(__stdcall* pLED_Download)(tLED_Download_Text_Para pLED_Download_Para,tLEDDevReturn *pDevStatus);
//11.4.5　显示信息
//此命令用于根据选择的序号将保存在LED控制板上或内存变量中的字符串内容进行显示。
//要求显示内容切换时，需立即显示
//UINT	nSerialNO	In	需显示的数据编号。与tLED_Download_Text_Para:: nSerialNO对应。
typedef int(__stdcall* pLED_Display)(UINT nSerialNO, tLEDDevReturn * pDevStatus);
//11.4.6　获取模块状态
typedef int(__stdcall* pLED_GetDevStatus)(tLEDModuleStatus *pModuleStatus,tLEDDevReturn *pDevStatus);
//11.4.7　获取模块驱动版本信息
typedef int(__stdcall* pLED_GetVersion)(char* cFirmWareVersion, char* cDriverVersion, tLEDDevReturn *pDevStatus);
//11.4.8　通信关闭
typedef int(__stdcall* pLED_CommClose)();

class CLedDll{
public:
	CLedDll();
	~CLedDll();
	pLED_CommOpen m_pLED_CommOpen;
	pLED_Init m_pLED_Init;
	pLED_Reset m_pLED_Reset;
	pLED_Download m_pLED_Download;
	pLED_Display m_pLED_Display;
	pLED_GetDevStatus m_pLED_GetDevStatus;
	pLED_GetVersion m_pLED_GetVersion;
	pLED_CommClose m_pLED_CommClose;
private:
	HINSTANCE hInst;

};
#endif __SERIALPORT_H__


