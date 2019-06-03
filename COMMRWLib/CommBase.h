#if !defined(_COMMBASE_H_INCLUDED_)
#define _COMMBASE_H_INCLUDED_

#define PRO_TJ 01
#define PRO_SY2 02
#define  PROJECT_STYLE PRO_TJ

#define BCD_TIME_LEN 7
//--------------------------
//---------------------------------------------------------------------------
//
// Library Name:		DCR4000Comm
//
// Library Description:	DAT RW Driver for Bejing Metro Line 5/13
//
// File Name:			CommBase.h
//
// File Description:	DLL base
//
// Copyright:			DAT
//
// Date Created:		2009.06.08
//
// Create by:			lizhencheng
//---------------------------------------------------------------------------


#pragma pack(1)

//***************************************************************************
//
// data type define
//
//***************************************************************************
#ifndef uchar
	#define uchar	unsigned char	// 1 byte
#endif

#ifndef ushort
	#define ushort	unsigned short  // 2 bytes
#endif

#ifndef ulong
	#define ulong	unsigned long	// 4 bytes
#endif
#ifndef byte
	typedef unsigned char byte;
#endif
#ifndef WORD
	typedef unsigned short WORD;
#endif
//
// message control
//
#define BUF_SIZE		1024			// 缓冲区存能放的最大字节个数 (0x00~0xFF)=256.
#define SEND_TIMES		1			// 发送次数
//#define COMMAND_DEBUG_OUTTIME 500000				// 500s = 8.3minutes, only for testing.
#define COMMAND_NORMAL_OUTTIME 3000				// 5s,一般命令的超时时间，即DLL发送命令后等待读写器响应返回的时间。
#define COMMAND_RESETREADER_OUTTIME 3000			// 5s, only for reset reader.
#define COMMAND_FIRMWAREUPDATESTART_OUTTIME 45000	// 45s, only for firmware update start.

//
// Multi Port Communication
//
#define MAX_COMMUNICATION_NUMBER 8	// max communication number 支持最大数量的通讯(串口)通道号。目前此值是人为规定的。

#define MAX_COMMAND_NUMBER 8	// max COMMAND number 支持最大接收包的数量。目前此值是人为规定的。

//
//***************************************************************************
//
// TPU status values
//
//***************************************************************************

#define STATUE_INIT					0xFF //初始状态	TPU上电后首先进入该状态
#define STATUE_NOTINIT				0xFE //未初始化状态	TPU复位后进入该状态
#define STATUE_IDLE					0x00 //空闲状态	TPU初始化完毕，具备工作条件
#define STATUE_POLLCARD				0x01 //寻卡状态	TPU正在寻卡
#define STATUE_FINDED				0x02 //已寻到卡状态	TPU已经寻到卡片，正等待下一步操作指令
#define STATUE_TRANSACTION			0x03 //交易状态	TPU处于具体的交易处理状态中
#define STATUE_GETUDDATA			0x04 //0x04		等待取UD状态	表示TPU业务处理完成，但UD数据尚未取出
//---------------------------------------------------------------------------

//
//***************************************************************************
//
// file type
//
//***************************************************************************
#define FILE_ACC_SYSPARA 	"1101"	//0x1101
#define FILE_ACC_BUSINESS 	"1102"	//0x1102
#define FILE_ACC_VERSION 	"1103"	//0x1103
#define FILE_ACC_BLACKLIST 	"1104"	//0x1104
#define FILE_ACC_PRODUCT 	"1105"	//0x1105
#define FILE_ACC_LOCATION 	"1106"	//0x1106
#define FILE_ACC_CALENDAR 	"1107"	//0x1107
#define FILE_ACC_CONSUMEFARE 	"1108"//0x1108
#define FILE_ACC_SALESFARE 	"1109"//0x1109

//BMAC文件标识定义
#define FILE_BMAC_BLACKLISTG1 	"1901"//0x1901
#define FILE_BMAC_BLACKLISTG2 	"1902"//0x1902
#define FILE_BMAC_CARDPROPERTY 	"1912"//0x1912
#define FILE_BMAC_CARDTRANSPROPERTY 	"1913"//0x1913
#define FILE_BMAC_PURSECARDLOAD 	"1914"//0x1914
#define FILE_BMAC_MULTIRIDECARDLOAD 	"1915"//0x1915
#define FILE_BMAC_PERIODCARDLOAD 	"1916"//0x1916
#define FILE_BMAC_CARDRETURN 		"1917"//0x1917
#define FILE_BMAC_LOADTERMINAL 	"1918"//0x1918
#define FILE_BMAC_CARDPURCHASE 	"1919"//0x1919
#define FILE_BMAC_CARDTRANSFER 	"1920"//0x1920
#define FILE_BMAC_QUERYTERMINAL 	"1921"//0x1921

#define FILE_AFC_MODECALENDAR 	"1002"//0x1002
#define FILE_BMAC_MAPPING 	"1097"//
#define FILE_BMAC_SUPPLEMENT 	"1001"//
//---------------------------------------------------------------------------

#define RESENDTHREETIMES 1  //0 不重发，3 重发3次
//***************************************************************************
//
//	Data struct
//
//***************************************************************************
typedef struct _COMPORTINFO
{
	UCHAR nPortnum;
	HANDLE hDev;
	BOOL beOpened;
	HANDLE hEvent;
	HANDLE hMutex;
	HANDLE hPollCard;
	HANDLE hCancelPollCard;
	HANDLE hbeStop;
	//add by djm
	HANDLE hThrd;
	DWORD dwThrdId;
	bool bThrdRun;
	_COMPORTINFO()
	{
		nPortnum=0;
		hDev=NULL;
		beOpened=FALSE;
		hEvent=NULL;
		hMutex=NULL;
		hPollCard=NULL;
		hCancelPollCard=NULL;
		hbeStop=NULL;
		//add by djm
		hThrd=NULL;
		dwThrdId=NULL;
		bThrdRun=false;
	}
}COMPORTINFO,CMT_DEVICE;

static COMPORTINFO cominfo[MAX_COMMUNICATION_NUMBER];

typedef struct _RECEIVEDATA
{
	uchar data[BUF_SIZE*2];
	ushort datalen;
	HANDLE hd;
	ulong runtime;
	HANDLE hMutex;
	HANDLE hEvent;
	HANDLE hPollCard;
}RECEIVEDATA;

typedef struct _RECEIVEPACKET
{
	BOOL   havedata;
	BOOL   havereturn;
	HANDLE hd;
	ushort command;
	uchar  sendtimes;
	uchar  data[BUF_SIZE*2];
	ushort  datalen;
	ulong  runtime;
} RECEIVEPACKET;




//
//***************************************************************************
//
//	function
//
//***************************************************************************

//***************************************************************************
// Function Name:	Open communication port
//
// Parameters:		nPort		- [in]	1. The number of serial port
//					nBaudrate	- [in]	2. Baud rate of serial port
//
// Return Values:	SUCESS:     CE_OK
//					FAILUE:		other errnum.
//
// Remarks:			handle
//
//***************************************************************************
int OpenComport(uchar nPort, ulong ulSpeed,HANDLE &hComm);
int OpenDevice(HANDLE &hUSBDevHandle, WORD wdModelIndex);
//***************************************************************************
// Function Name:	close communication port
//
// Parameters:		communication handle		- [in]	handle of comport which is opened
//
// Return Values:	SUCESS      CE_OK
//					FALSE		other errnum.
//***************************************************************************
int CloseComport(HANDLE hComm);

//***************************************************************************
// Function Name:	is communication port be opened
//
// Parameters:		communication handle		- [in]	handle which be checked
//
// Return Values:	TRUE
//					FALSE
//***************************************************************************
BOOL isOpened(HANDLE hComm);

//***************************************************************************
// Function Name:	is communication port be opened
//
// Parameters:		communication PORT		- [in]	PORT which be checked
//
// Return Values:	TRUE
//					FALSE
//***************************************************************************
BOOL PortisOpened(uchar nPort);
//***************************************************************************
// Functin Name:	Encrypt 加密
//
// Parameters:		buf		- [in/out]	buffer to send 发送缓冲区数据的首地址
//					nLen	- [in]		buffer length 发送缓冲区数据的长度
//
// Return Values:	void
//
// Remarks:			handle send buffer 处理发送缓冲区中的数据，在 [STX(0x02) ETX(0x03) DLE(0x10)] 的前面添加 [DLE]。
//
//***************************************************************************
void Encrypt(uchar * buf,ushort & nLen);

//***************************************************************************
// Function Name:	Decrypt 解密
//
// Parameter:		buf		- buffer
//					nLen	- buffer length
//
// Return Values:	void
//
// Remarks:			handle receive buffer 处理接收缓冲区中的数据，删除 [STX(0x02) ETX(0x03) DLE(0x10)] 前面的 [DLE]。
//
//***************************************************************************
void Decrypt(uchar * buf,ushort & nLen);

//***************************************************************************
// Function Name:	write
//
// Parameter:		lpBuf		- buffer
//					dwCOunt		- buffer length
//
// Return Values:	count of data is writed 
//
//***************************************************************************
ulong Write(const void* lpBuf, ulong dwCount,HANDLE hComm);

//***************************************************************************
// Function Name:	read
//
// Parameter:		lpBuf		- buffer
//					dwCOunt		- buffer length
//
// Return Values:	count of data is readed 
//
//***************************************************************************
ulong Read(void* lpBuf, ulong dwCount,HANDLE hComm);
//***************************************************************************
// Function Name:	Write_USB
//
// Parameter:		lpBuf		- buffer
//					dwCOunt		- buffer length
//
// Return Values:	count of data is writed 
//
//***************************************************************************
ulong Write_USB(const void* lpBuf, ulong dwCount,HANDLE hComm);

//***************************************************************************
// Function Name:	Read_USB
//
// Parameter:		lpBuf		- buffer
//					dwCOunt		- buffer length
//
// Return Values:	count of data is readed 
//
//***************************************************************************
ulong Read_USB(void* lpBuf, ulong dwCount,HANDLE hComm);
//***************************************************************************
// Function Name:	Start Receive
//
//
// Parameter:		hComm		communication handle
//
// Return Values:	
//
// remark: create a thread to receive data 
//***************************************************************************
void StartReceive(COMPORTINFO *pDevInfo);

//***************************************************************************
// Function Name:	Start read
//
//
// Parameter:		lpParam		communication handle
//
// Return Values:	
//
// remark: receive data 
//***************************************************************************
DWORD WINAPI StartRead(LPVOID  lpParam);

char ReceivedataProcess(uchar *Buffer, ushort BufferCount,ulong Runtime,COMPORTINFO *pDevInfo);

DWORD WINAPI DataDealProc(LPVOID  lpParam);

HANDLE getEventHandle(HANDLE hComm);

HANDLE getMutexHandle(HANDLE hComm);

HANDLE getPollcardHandle(HANDLE hComm);
HANDLE getCancelPollcardHandle(HANDLE hComm);
//add by djm
bool IsDeviceUsb();
void SetDeviceUsb(bool bVal);
bool IsPollCard();
void SetPollCard(bool bVal);
CMT_DEVICE *getCmtDevice();
CMT_DEVICE *getCmtDevice(HANDLE hComm);
RECEIVEPACKET * getPacket();
RECEIVEPACKET * getRecvingPacket(WORD wCmd,byte byTimes);
//end 


// int SendCommand(ushort command,uchar sendtimes,uchar* sendbuffer,ushort sendBufferCount,
// 				uchar* recvbuffer,ushort &recvbuffercount,ulong &runtime,HANDLE hComm);

void writeCommdata(char *name,uchar* data,ushort datalen);

int WINAPI Hex_Asc( BYTE *hex, BYTE *asc,long length);
int SendCommand(ushort command,uchar sendtimes,uchar* sendbuffer,ushort sendBufferCount,
				uchar* recvbuffer,ushort &recvbuffercount,ulong &runtime,HANDLE hComm);
#endif