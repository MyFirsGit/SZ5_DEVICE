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
#define BUF_SIZE		1024			// ���������ܷŵ�����ֽڸ��� (0x00~0xFF)=256.
#define SEND_TIMES		1			// ���ʹ���
//#define COMMAND_DEBUG_OUTTIME 500000				// 500s = 8.3minutes, only for testing.
#define COMMAND_NORMAL_OUTTIME 3000				// 5s,һ������ĳ�ʱʱ�䣬��DLL���������ȴ���д����Ӧ���ص�ʱ�䡣
#define COMMAND_RESETREADER_OUTTIME 3000			// 5s, only for reset reader.
#define COMMAND_FIRMWAREUPDATESTART_OUTTIME 45000	// 45s, only for firmware update start.

//
// Multi Port Communication
//
#define MAX_COMMUNICATION_NUMBER 8	// max communication number ֧�����������ͨѶ(����)ͨ���š�Ŀǰ��ֵ����Ϊ�涨�ġ�

#define MAX_COMMAND_NUMBER 8	// max COMMAND number ֧�������հ���������Ŀǰ��ֵ����Ϊ�涨�ġ�

//
//***************************************************************************
//
// TPU status values
//
//***************************************************************************

#define STATUE_INIT					0xFF //��ʼ״̬	TPU�ϵ�����Ƚ����״̬
#define STATUE_NOTINIT				0xFE //δ��ʼ��״̬	TPU��λ������״̬
#define STATUE_IDLE					0x00 //����״̬	TPU��ʼ����ϣ��߱���������
#define STATUE_POLLCARD				0x01 //Ѱ��״̬	TPU����Ѱ��
#define STATUE_FINDED				0x02 //��Ѱ����״̬	TPU�Ѿ�Ѱ����Ƭ�����ȴ���һ������ָ��
#define STATUE_TRANSACTION			0x03 //����״̬	TPU���ھ���Ľ��״���״̬��
#define STATUE_GETUDDATA			0x04 //0x04		�ȴ�ȡUD״̬	��ʾTPUҵ������ɣ���UD������δȡ��
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

//BMAC�ļ���ʶ����
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

#define RESENDTHREETIMES 1  //0 ���ط���3 �ط�3��
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
// Functin Name:	Encrypt ����
//
// Parameters:		buf		- [in/out]	buffer to send ���ͻ��������ݵ��׵�ַ
//					nLen	- [in]		buffer length ���ͻ��������ݵĳ���
//
// Return Values:	void
//
// Remarks:			handle send buffer �����ͻ������е����ݣ��� [STX(0x02) ETX(0x03) DLE(0x10)] ��ǰ����� [DLE]��
//
//***************************************************************************
void Encrypt(uchar * buf,ushort & nLen);

//***************************************************************************
// Function Name:	Decrypt ����
//
// Parameter:		buf		- buffer
//					nLen	- buffer length
//
// Return Values:	void
//
// Remarks:			handle receive buffer ������ջ������е����ݣ�ɾ�� [STX(0x02) ETX(0x03) DLE(0x10)] ǰ��� [DLE]��
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