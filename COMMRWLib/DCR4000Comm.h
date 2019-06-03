#if !defined(_IDCR4000COMM_H_C4DC0841_3813_4642_8B5D_56017322CFF5__)
#define _IDCR4000COMM_H_C4DC0841_3813_4642_8B5D_56017322CFF5__

//---------------------------------------------------------------------------
//
// Library Name:		DCR4000Comm.h
//
// Library Description:	DAT RW Driver for Bejing Metro Line 5/13
//
// File Name:			DCR4000Comm.h
//
// File Description:	DLL API
//
// Copyright:			DAT
//
// Date Created:		2009.06.09
//
// Create by:			lizhencheng
//---------------------------------------------------------------------------
//
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DCR2000COMM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
//   functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
//////////Start by chenliye/////////////////
#include "CommBase.h"
/////////////The following code is from CommBase.h. Rewrite it here for not including CommBase.h in this file/////
//***************************************************************************
//
// data type define
//
/***************************************************************************
#ifndef uchar
	#define uchar	unsigned char	// 1 byte
#endif

#ifndef ushort
	#define ushort	unsigned short  // 2 bytes
#endif

#ifndef ulong
	#define ulong	unsigned long	// 4 bytes
#endif

#ifndef BUF_SIZE
#define BUF_SIZE		1024			// 缓冲区存能放的最大字节个数 (0x00~0xFF)=256.
#endif*/
/////////////////////end by chenliye///////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
//

#define DllImport			__declspec(dllimport)
#define DllExport			__declspec(dllexport)

#ifdef EXPORT_INTERFACE
	#define DllUseport			DllExport
#else
	#define DllUseport			DllExport
#endif

#define CSC_HANDLE HANDLE
#define DOWN_BLOCKSIZE 1024
//
// Following Commands and Functions only for DCR4000 Series Reader.
//
//
//***************************************************************************
//　应答代码:TPU　执行命令的结果　0x00 表示成功,非零即为错误代码.
//// 
//
//***************************************************************************
#define CE_OK 							0x00 //sucess 
#define LOGON_YUEPIAO_UPDATE			26028	//月票版本需要更新
#define LOGON_DEVICE_UPDATE				26029	//月票版本需要更新

#define CLOSECOM_ERR					0xFF11 //close communication error
#define SEND_ERR						0xFF12 //send error
#define RECV_TIMEOUT					0xFF13 //receive timeout
#define RECV_ERR						0xFF14 //receive error
#define DOWNLOAD_ERR					0xFF15 //download parameter error
#define DOWNLOAD_SEQERR					0xFF16 //download serial number error 
#define SEND_WAIT						0xFF17 //command is sended,wait
#define RECV_CANCLE						0xFF18 //command is cancled
#define OPENCOM_ERR						0xFF19 //open port error

	//add by djm
#define EDEVICE						0xFF20 //当输入的设备类型不正确时.
#define EPACKET						0xFF21 //当获不到空闲的数据包.
#define ERECV						0xFF22 //当获取不到数据时
#define E_NOT_ENOUGH_BUF			0xFF23 //当接口函数提供数据的输出缓存不足时
#define E_EMPTY						0xFF24 //当没有数据要返回时
#define E_ALLOC_MEMORY				0xFF25 //分配内存出错了
#define E_FILE						0xFF26 //文件错误
#define E_UP_UDDATA					0xFF31 //须上传最后一交易
#define E_CHECK_DEAL				0xFF32 //设备需要初验(只对CPU卡可用)
#define E_END_CHECK					0xFF33 //初验结束(只对M1卡有用)
#define E_CARD						0xFF34 //卡片错误
#define E_POINTTER					0xFF35 //指针错误
#define E_TIMEOUT_DEAL				0xFF36 //超时处理
#define E_NOT_ENOUGH_DATA			0xFF37 //接收的数据长度不够
#define E_CONNECT_SERVER_ERROR		0xFF38 //连接服务器失败
#define E_FUN_PARA					0xFF39 //函数参数错误
#define E_CONFIG_FILE				0xFF3A
#define E_CONNECT_CENTERHOST		0xFF3B
#define E_RESPONSE_CENTERHOST		0xFF3C
#define E_CHECK_CENTERHOST			0xFF3D
#define E_COMMAND					0xFF3E
#define E_PACKET					0xFF3F

#define E_LN_CMD_RTN				0xFF40 //联机相应报文件中的命令码错误

#define E_LN_RTN_CMD				0xFF50 //联机相应报文件中的命令码错误

#define E_LN_RTN_ERR				0xE000 //联机服务机返回了错误代码
#define E_LN_RTN_MAC2_LEN			0xFF52 //联机服务机返回的MAC2长度错误(长度不等于4)
#define E_LN_READ_APPTYPE			0xFF53 //联机读卡返回的"卡片类型"有误.
#define E_LN_READ_CNT_FL_MD			0xFF54 //联机读卡返回的"需修改文件的个数"有误.
#define E_LN_SEND					0xFF61 //向联机服务发送数据失败。
#define E_LN_RECV					0xFF62 //从联机服务上获取数据失败。

#define E_LN_KEYTYPE				0xFF64 //联机售卡(充值)时,从TPU中读取的密钥类型不正确

#define E_OK						0x0000
#define LNE_TERMPARA_UP 26028
#define LNE_POSTPARA_UP 26029 

	//end
#define TPU_POWERDOWN                   0xFFF4 //给TPU断电重启

//==联机命令======================================
#define CMDLN_CPU_PUTVALUE 0x2041 //一卡通储值票充值
#define CMDLN_CPU_REVOKE 0x2042  //一卡通储值票充正
#define CMDLN_CPU_UPDATE 0x2043 //一卡通储值票更新文件权限申请
#define CMDLN_CPU_AUTHEN 0x2044 //CPU卡权限认证申请(旧)
#define CMDLN_SAM_AUTHEN 0x2051 //SAM卡权限认证申请(旧)

#define CMDLN_DPST_PUTVALUE 0x2045 //储值票充值权限申请
#define CMDLN_DPST_REVOKE 0x2046  //储值票充正权限申请
#define CMDLN_DPST_UPDATE 0x2047 //储值票更新文件权限申请

namespace DAT
{


//***************************************************************************
// 
// Functions
//
//***************************************************************************
//
// --------------------------------------------------------------------------
//
// normal Function
//
// --------------------------------------------------------------------------

//-------------------------------------------------------------------------
//add by djm
/*
desc:显示错误信息
*/
DllUseport char * WINAPI GetErrorString(DWORD dwErr);
//====
	
	
	/*
定义函数 
int OpenReader( UCHAR nPort, ULONG ulSpeed, BOOL bIsUSB,HANDLE * phComm )
函数说明 
启动读写器外部认证的通讯。
参数
nPort：用来和读卡器通讯的端口号(1 至设备的最大端口号) 
ulSpeed：通讯端口速率(1200, 2400, 9600, 19200, 38400, 57600, 115200)
bIsUSB :用来标识 是Com口连接还是USB 连接
phComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值：
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI OpenReader(UCHAR nPort, ULONG ulSpeed, bool bIsUSB,HANDLE &hComm);

/*
定义函数 
int CloseReader(HANDLE hComm )
函数说明 
停止读写器外部认证的通讯。
参数
hComm：将要被停止通信的读卡器的句柄（由OpenReader提供）
返回值：
成功则返回0，若有错误则返回非零值errno
错误代码
请参考附录错误代码表
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI CloseReader(HANDLE hComm );


/************************************************************************/
/*                                                                     */
/************************************************************************/
BOOL beOpened(uchar nPort);

//-------------------------------------------------------------------------
/*INITSTRUCT 结构体*/

typedef struct _INITSTRUCT {  
	ulong DelayTime; //读卡指令，等待时间。
	uchar DeviceCode[6]; 
	uchar DeviceHdType; 
	uchar StationPLInfo[4]; 
	uchar AgentCode;
	uchar OperationDate[4]; 
	uchar CurrentDate[4]; 
	uchar DeviceTestMode; 
	uchar StationOperationMode;
	uchar OperaterID[4];
	uchar  AntennaConfig;
	ulong uArrivalsNo;
	ushort ErrFareCtrMax;
	ushort WaitTimeForCardWR;
	ushort RetryTimesForCardWR;
	uchar Reserver[16];
} INITSTRUCT;

//-------------------------------------------------------------------------
/*
定义函数 
int Init_TPU(INITSTRUCT InitData,uchar &Statue,HANDLE hComm);
函数说明 
对TPU进行初始化（即对TPU的运行环境进行配置）初始化的内容包括：设置线路码、站码、工作模式等TPU需要的运行配置环境参数。
参数
	IniData	存放初始化数据内容，格式为结构体
	Statue 	当命令执行成功时，用于存放返回的设备状态
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Init_TPU(INITSTRUCT InitData,uchar &Statue,HANDLE hComm);
	
//-------------------------------------------------------------------------
/*
desc:对上位机开机以后，首先对TPU进行　复位，初始化，及签到(Logon_Tpu).
	Logon_Tpu:为复合操作，需要和联机服务器进行通信。
*/
DllUseport int WINAPI Logon_Tpu(byte &byState,HANDLE hComm);
/*
desc:对上位机关机之前，要对TPU进行签退(Exit_Tpu).
Exit_Tpu:为复合操作，需要和联机服务器进行通信。
*/
DllUseport int WINAPI Exit_Tpu(byte &byState,HANDLE hComm);
/*
定义函数 
int Reset_TPU(uchar &Statue,HANDLE hComm);
函数说明 
SLE在启动TPU时，或需要重新初始化TPU时发送该命令数据，TPU执行复位操作后返回应答数据。TPU复位操作的具体动作包括：对SAM卡进行复位操作，读取出SAM卡号，对TPU内的RAM进行复位操作。
参数
	Statue 	当命令执行成功时，用于存放返回的设备状态
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Reset_TPU(uchar &Statue,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int Stop_TPU(uchar &Statue,HANDLE hComm);
函数说明 
SLE需要让TPU返回到“未初始化状态”时发送该命令数据，TPU执行操作后返回应答数据。
参数
	Statue 	当命令执行成功时，用于存放返回的设备状态
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Stop_TPU(uchar &Statue,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int Abort _TPU(uchar &Statue,HANDLE hComm);
函数说明 
SLE需要让TPU中止当前的业务操作并返回空闲状态时发送该命令数据，TPU执行操作后返回应答数据。
参数
	Statue 	当命令执行成功时，用于存放返回的设备状态
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Abort_TPU(uchar &Statue,HANDLE hComm);

  DllUseport int WINAPI Erase_LogFile(uchar &Statue,unsigned short &fileType,HANDLE hComm);
  DllUseport int WINAPI Erase_Para(uchar &Statue,unsigned short &fileType,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int SetTimer(uchar* SetTime,uchar &Statue,HANDLE hComm);
函数说明 
SLE需要设置TPU时钟时发送该命令数据，TPU执行操作后返回应答数据。
参数
	Time  要设置的TPU时钟信息，BCD码，格式为：YYYYMMDDHHmmSS。
TPUTime 	当命令执行成功时，用于存放返回的TPU时钟信息,BCD码，格式为：YYYYMMDDHHmmSS。
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI SetReadTimer(uchar* SetTime,uchar *TPUTime,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int GetTime(uchar *Time,HANDLE hComm);
函数说明 
SLE需要获取TPU时钟时发送该命令数据，TPU执行操作后返回应答数据
参数
	Time  用于存放TPU时钟信息，BCD码，格式为：YYYYMMDDHHmmSS。
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
错误代码 
请参考附录错误代码表
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI GetReadTime(uchar *Time,HANDLE hComm);
//-------------------------------------------------------------------------

typedef struct _TPUINFO
{
	byte DeviceCode[4];
	byte AppSoftVer[2];
	byte MainSoftVer[2];
	byte HardwareVer[2];
	byte BMACPSAMID[6];
	byte BMACISAMID[6];
	byte ACCPSAMID[6];
	byte ACCISAMID[6];
	_TPUINFO()
	{
		memset(DeviceCode,0,sizeof(DeviceCode));
		memset(AppSoftVer,0,sizeof(AppSoftVer));
		memset(MainSoftVer,0,sizeof(MainSoftVer));
		memset(HardwareVer,0,sizeof(HardwareVer));
		memset(BMACPSAMID,0,sizeof(BMACPSAMID));
		memset(BMACISAMID,0,sizeof(BMACISAMID));
		memset(ACCPSAMID,0,sizeof(ACCPSAMID));
		memset(ACCISAMID,0,sizeof(ACCISAMID));
	}
}TPUINFO;
/*
定义函数 
int WINAPI GetTPUInfo(TPUINFO &info,HANDLE hComm);
函数说明 
SLE需要获取TPU的基本信息时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该命令获取TPU的基本信息，包括：SAM卡ID,TPU软件版本，TPU硬件版本、设备ID等。
参数
	info 用于存放TPU信息的结构
	hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI GetTPUInfo(TPUINFO &info,HANDLE hComm);


typedef struct _TRANS_RECORD_INFO
{
	ulong transRecordCtr;
	uchar haveTransRecord2Upload;
}TRANS_RECORD_INFO;


DllUseport int WINAPI GetTransRecordInfo(TRANS_RECORD_INFO &info,HANDLE hComm);

// --------------------------------------------------------------------------
//
// Function of parameter
//
// --------------------------------------------------------------------------
/*PARAINFO结构说明*/
/*
typedef struct _PARASTRUCT {  
uchar YPTParams_Cu[8]; 
uchar YPTBlackList_Cu[8]; 
uchar YPTCalendars_Cu[8];
uchar YPTFareTable_Cu [8];
uchar YPTLocations_Cu [8];
uchar YPTManifest_Cu [8];
uchar YPTProductParams_Cu [8];
uchar YPTSaleTable_Cu [8];
uchar YKTBlackList_Cu [8];
uchar YKTExpendCardtype_Cu [8];
uchar YKTCardAttributy_Cu [8];
uchar YKTValueCardBusiness_Cu [8];
uchar YKTCountCardBusiness_Cu [8];
uchar YKTPeriodCardBusiness_Cu [8];
uchar YKTReturnCard_Cu [8];
uchar YKTExpendLimitAmount_Cu [8];
uchar YKTConjunctionPrice_Cu[8];
uchar YKTAdditionalPara[8];
}PARA; 
*/
typedef struct _PARAINFOSTRUCT {  
	uchar ParaID[2];
	uchar ParaVer[4];
	uchar TakeEffectTime[4];
	uchar VerType;
	_PARAINFOSTRUCT()
	{
		memset(ParaID,0,sizeof(ParaVer));
		memset(ParaVer,0,sizeof(ParaVer));
		memset(TakeEffectTime,0,sizeof(TakeEffectTime));
		VerType=0;
	}
}PARAINFO;

//-------------------------------------------------------------------------
/*
定义函数 
int GetParaInfo(ushort ParaNum,STRUCT PARAINFO *Parainfo,HANDLE hComm);
函数说明 
SLE需要获取TPU当前所有参数的版本信息时发送该命令数据，TPU执行操作后返回应答数据。同一种类参数文件，TPU内部有可能存在2份：当前版本和未来版本。本指令获取参数版本时，将返回参数文件的当前版本和未来版本的信息。
参数
	ParaNum     当命令成功执行时，用于存放参数个数
	ParaInfo 	PARAINFO结构，当命令执行成功时，用于存放返回的参数版本信息
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI GetParaInfo(ushort &ParaNum,PARAINFO *Parainfo,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int Para_Downdload (uchar* ParaType,uchar bContinue,ushort PacketNumber,ushort PacketLen,uchar * Data,ushort &ReceivePacketNumber,HANDLE hComm);
函数说明 
SLE需要给TPU下发参数文件时发送该命令数据，TPU执行操作后返回应答数据。
参数
	ParaType  参数文件类型，长度为2字节
	bContinue  后续包标志，表示某个参数文件是否有后续包要继续下载：0x00 表示该数据包是结束包，无后续包。0x01 表示有后续包。
	PacketNumber 参数文件下载时的分隔的数据包编号。第一个包从0x0000开始，后续依次累加，最后一个包填OxFFFF。
PacketLen数据包信息长度。当参数文件大于256bytes时，需要分包发送。每个数据包的长度最大为256，当最后一个数据包不足256bytes时，按照实际长度发送。
Data 数据内息，下载的数据内容，最大256 byte。
ReceivePacketNumber 用于存放返回应答中指示应答成功的包序号。
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Para_Downdload (ushort wFileType,uchar chIsHaveNext,ushort wPktOrderNumber,
	  ushort wPktLen,uchar * pPktData,ushort &wRvOrderNumber,HANDLE hComm);


// --------------------------------------------------------------------------
//
// maintenance Function
//
// --------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
定义函数 
int WINAPI EXEC_Firmware_Downdload (ushort PacketNumber,uchar * Data,HANDLE hComm)
函数说明 
SLE需要更新TPU软件时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令更新TPU的应用程序。（说明：TPU的程序分为两个部分，主控程序和应用程序。主控程序主要负责程序的引导和更新，应用程序主要负责具体的业务处理）
参数
	PacketNumber 参数文件下载时的分隔的数据包编号。第一个包从0x0000开始，后续依次累加，最后一个包填OxFFFF。
	Data 数据内息，下载的数据内容，最大128 byte,最后一块若不足128字节，则后面填充0x00。。
	hcomm 标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI EXEC_Firmware_Downdload (ushort wPkOrder,uchar * Data,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int EXEC_Firmware_Update (HANDLE hComm);
函数说明 
SLE需要更新TPU软件时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令更新TPU的应用程序。（说明：TPU的程序分为两个部分，主控程序和应用程序。主控程序主要负责程序的引导和更新，应用程序主要负责具体的业务处理）。
参数
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI EXEC_Firmware_Update(HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int APP_Firmware_Downdload (ushort PacketNumber,uchar * Data,HANDLE hComm);
函数说明 
SLE需要更新TPU软件时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令更新TPU的应用程序。（说明：TPU的程序分为两个部分，主控程序和应用程序。主控程序主要负责程序的引导和更新，应用程序主要负责具体的业务处理）
参数
	PacketNumber 参数文件下载时的分隔的数据包编号。第一个包从0x0000开始，后续依次累加，最后一个包填OxFFFF。
Data 数据内息，下载的数据内容，最大128 byte,最后一块若不足128字节，则后面填充0x00。
ReceivePacketNumber 用于存放返回应答中指示应答成功的包序号。
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

 DllUseport int WINAPI APP_Firmware_Downdload(ushort wPkOrder,uchar * Data,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int APP_Firmware_Update (HANDLE hComm);
函数说明 
SLE需要更新TPU软件时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令更新TPU的应用程序。（说明：TPU的程序分为两个部分，主控程序和应用程序。主控程序主要负责程序的引导和更新，应用程序主要负责具体的业务处理）
参数
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
 DllUseport int WINAPI APP_Firmware_Update(HANDLE hComm);

// --------------------------------------------------------------------------
//
// transaction Function
//
// --------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
定义函数 
int GetStatue(uchar &Statue);
函数说明 
SLE需要获取TPU当前状态时发送该命令数据，TPU执行操作后返回应答数据。SLE可以通过每隔一段时间获取TPU状态，检测SLE与TPU之间的通信连接状况
参数
	Statue 	当命令执行成功时，用于存放返回的设备状态
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI GetStatue(uchar &byTpuStatue,HANDLE hComm);

  DllUseport int WINAPI EraseTransRecord(uchar &Statue,HANDLE hComm);

//-------------------------------------------------------------------------
/*typedef struct __POOLCARD //add by chenliye
{
uchar AntennaMark;
uchar Reserved;
}POOLCARD; */

/*
定义函数 
int Poll_Card(uchar AntennaMark,uchar &CardType,uchar *CardPHId,HANDLE hComm);
函数说明 
SLE需要让TPU进入循环寻卡状态时发送该命令数据，TPU执行操作后返回应答数据。AG使用该命令数据通知TPU开始寻卡，TPU收到该指令将不停的重复寻卡，如果没有寻到卡，TPU状态将保持在“寻卡状态”并继续寻卡,如果寻到卡片，将切换到“已寻到卡状态”并返回寻卡结果。
参数
	CardType用于存放返回的卡片类型
	CardPHId 用于存放返回的卡片物理ID，长度为8
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Poll_Card(uchar AntennaMark/*by chen liye*/, uchar &CardType, uchar &AntennaFindMark, uchar *CardPHId,HANDLE hComm);


 //-------------------------------------------------------------------------
typedef struct __TRANRESULT
{
	uchar RetractFlag;
	uchar HighBlacklist;
	uchar CardType;
	ushort ProductyCategory;
	uchar CardNo[8];
	ulong BeforAmount;
	ulong AfterAmount;
	uchar ucBlockFlag;
	uchar Reserved[10];
	__TRANRESULT()
	{
		RetractFlag=0;
		HighBlacklist=0;
		CardType=0;
		ProductyCategory=0;
		memset(CardNo,0,sizeof(CardNo));
		BeforAmount=0;
		AfterAmount=0;
		ucBlockFlag=0;
		memset(Reserved,0,sizeof(Reserved));
	}
}TRANRESULT;

typedef struct __UDDATA 
{
	uchar UDnum;
	uchar TranType;
	ushort tranlen;
	uchar trandata[BUF_SIZE];
}UDDATA;

typedef struct __ARDATA 
{
	uchar ARnum;
	uchar ARdata[BUF_SIZE];
}ARDATA;



//-------------------------------------------------------------------------
/*
定义函数 
int int WINAPI EnterStation_AG(uchar* tranTime, ulong SerialNO,uchar bRetrunUD,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

  函数说明 
	AG通知TPU进行进站业务处理时发送该命令数据，TPU执行操作后返回应答数据。AG通过该指令通知TPU进行AG进站交易，TPU完成进站业务处理，并写卡完成后给AG返回执行结果。注意：SLE可通过进站控制命令中的“是否立即返回UD”来控制TPU是否立即返回UD详细信息。
参数
	STATION_INFO *pStation [in] //
	
	SerAccnum UDSN或一卡通交易流水号累加值
	UDData 如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
	UDDataLen 如果返回UD，用于存放返回UD数据的长度
	ARData 用于存放返回的AR数据，如果无AR返回，为空
	ARDataLen 如果返回，用于存放AR数据长度
	hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
	成功则返回0，若有错误则返回非零值errno
*/
typedef struct _STATION_INFO 
{
	uchar mTime[7];//e交易发生的日期时间，BCD码，格式YYMMDDHHmm
	ulong uSerialNo;//UDSN或一卡通交易流水号
	uchar bReturnUd;//是否返回UD的标识
	uchar mCardBoxsId[4];//票箱编号
	_STATION_INFO()
	{
		memset(mTime,0,sizeof(mTime));
		uSerialNo=0;
		bReturnUd=0;
		memset(mCardBoxsId,0,sizeof(mCardBoxsId));
	}
} STATION_INFO;

//-------------------------------------------------------------------------


  DllUseport int WINAPI EnterStation_AG(STATION_INFO *pStation,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int ExitStation_AG(uchar* tranTime, ulong SerialNO,uchar bRetrunUD,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
SLE通知TPU进行出站业务处理时发送该命令数据，TPU执行操作后返回应答数据。AG通过该指令通知TPU进行AG出站交易，TPU完成出站业务处理，并写卡完成后给AG反馈执行结果。注意：SLE可通过出站控制命令中的“是否立即返回UD”来控制TPU是否立即返回UD详细信息。
参数
	tranTime交易发生的日期时间，BCD码，格式YYMMDDHHmm
	SerialNO UDSN或一卡通交易流水号
	bRetrunUD 是否返回UD的标识
	SerAccnum UDSN或一卡通交易流水号累加值
	UDData 如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
	UDDataLen 如果返回UD，用于存放返回UD数据的长度
	ARData 用于存放返回的AR数据，如果无AR返回，为空
	ARDataLen 如果返回，用于存放AR数据长度
	hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI ExitStation_AG(STATION_INFO *pStation,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);


//-------------------------------------------------------------------------
/*
定义函数 
int WINAPI GetUDData(uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
SLE需要从TPU提取未上传给SLE的UD数据（包括一卡通和ACC交易数据）时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerAccnum UDSN或一卡通交易流水号累加值
UDData 如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
UDDataLen 如果返回UD，用于存放返回UD数据的长度
ARData 用于存放返回的AR数据，如果无AR返回，为空
ARDataLen 如果返回，用于存放AR数据长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
  DllUseport int WINAPI GetUDData(uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);


typedef struct	_TRANS_RECORD_HDR
{
	uchar transLen[2];
	uchar transcomm[2];
	uchar retryCtr;
	uchar respons;
	uchar udsnCtr;
	uchar transResult[32];
}TRANS_RECORD_HDR;

DllUseport int WINAPI GetTransRecordData(ulong transRecordSN,TRANS_RECORD_HDR &transHdr, ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

//========修改记名卡持卡人信息==========
typedef struct STRUCT_RENMCD_CHG
{
	DWORD uUdsn;//UDSN
	byte mDateTime[7];//交易时间
	byte byTypeDsg;//持卡人类型标识
	byte byStaffDsg;//持卡人职工标识
	char szName[20];//持卡人姓名
	char szCreditNo[32];//持卡人证件号码
	byte byCreditType;//持卡人证件类型
	WORD wLfAmt;//押金金额（分）
	byte bySex;//持卡人性别
	byte byAppVer;//应用版本
	//证件有效期	7	BCD
	byte mCertificateValidity[7];
	//持卡人电话	10 	BCD	
	byte mTel[10];
	//持卡人Email	60	ASCII
	char mEmail[60];
	//持卡人地址1	128	ASCII
	char mAddr1[128];
	//持卡人地址2	128	ASCII
	char mAddr2[128];
	//预留 24 bytes
	byte mReserve[24];
	STRUCT_RENMCD_CHG()
	{
		uUdsn=0;
		byTypeDsg=0;
		byStaffDsg=0;
		memset(szName,0,sizeof(szName));
		memset(szCreditNo,0,sizeof(szCreditNo));
		byCreditType=0;
		wLfAmt=0;
		bySex=0;
		byAppVer=0;
		memset(mCertificateValidity,0,sizeof(mCertificateValidity));
		memset(mTel,0,sizeof(mTel));
		memset(mEmail,0,sizeof(mEmail));
		memset(mAddr1,0,sizeof(mAddr1));
		memset(mAddr2,0,sizeof(mAddr2));
		memset(mReserve,0,sizeof(mReserve));
	}
	
} STT_RENMCD_CHG;
//-------------------------------------------------------------------------
/*
定义函数 
int WINAPI UpdateRealNameInfo(uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 :修改实名卡中的用户信息.
参数
STT_RENMCD_CHG &sttReNmInfo [in] 输入参数
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
DllUseport int WINAPI UpdateRealNameInfo(STT_RENMCD_CHG &sttReNmInfo,uchar &SerAccnum,TRANRESULT &TranResult,	ushort &UDDatalen,
										 uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

//========================================




//-------------------------------------------------------------------------
/*CARDINFO 结构体说明*/
/*
uchar LogInfo[256]:数组长度以原来的256改为2048　,根据<SLE-TPU 接口 天津地铁.doc>
*/
typedef struct _CARDINFO
{
	uchar CardPHType;//车票物理类型
	ushort CardInfoLen;//卡数据长度
	uchar PayAreaReticketCode;//付费区补票建议代码
	uchar NoPayAreaReticketCode;//非服费区补票建议代码
	ulong RepayTicketAmount;//建议补票金额
	ulong ReturnTicketAmount;//建议退票金额
	ulong ReuturnAmount;//[4];建议退资金额
	ulong ReturnCardFee;//[4];建议退票手续费金额
	uchar isBigMoneyCard;//是否为大额卡
	uchar AntennaMark; // 天线标志
	uchar CardInfo[512];//卡信息
	uchar ucLogNum;//史交易数据
	uchar LogInfo[32];//史交易数据
}CARDINFO;

typedef struct CARDREADMISC
{
	//CPU 卡片基本数据
	uchar ValidCheckMark;//合法性检查标志(1 bytes)
	uchar TransCode;//业务功能代码
	uchar AreaPaidMark;//付费区标志
	uchar TransTime[7];//当前时间
	uchar AntennaMark;//天线寻卡标志
	uchar ucReadFlag;//补票前读卡标记
	uchar ucStation[4];//补票前车站站码
	uchar ucGetTransLog;//历史交易读取标记
}CardReadMisc_t, *pCardReadMisc_t;

//-------------------------------------------------------------------------
/*
desc:SLE需要通过TPU读取卡片信息时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令使TPU进行读卡操作。当TPU收到这一命令时，它将在天线范围内寻卡。寻到卡以后首先按相关交易流程完成对卡的恢复并将卡上的相关数据通过应答数据包向AFC设备传送,这里所指的卡包括一卡通卡和一票通卡两种,对于不同的卡,TPU的应答数据是不同的。如果TPU没有找到卡，将立即返回无卡应答给SLE。
参数
	bCheck 指示是否检查卡片合法性
	TranCode 指示后续交易类型
	CardInfo 为一结构体，用于存放返回的卡片信息
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI ReadCardInfo (pCardReadMisc_t pCardReadMisc,
	  /*uchar bCheck,uchar TranCode,uchar PayFlag,uchar *TranDateTime,*/
	  CARDINFO &CardInfo,HANDLE hComm);
 //-------------------------------------------------------------------------
#define LN_FILE_SIZE 360
  typedef struct STRUCT_LN_WRITECARD_FOR_VALUE
  {
	  byte byKeyType;//卡片类型//密钥类型
	  byte byBusyType;//业务类型
	  //剩余修改文件个数
	  byte byLeafCntFile;
	  //文件ID
	  WORD wFileId;
	  byte  mKey[4];//密钥
	  WORD wFileLen;//文件长度
	  byte mFileData[LN_FILE_SIZE];//文件数据
	  byte mReserve[16];//预留
	  STRUCT_LN_WRITECARD_FOR_VALUE()
	  {
		  byKeyType=0;
		  byBusyType=0;
		  byLeafCntFile=0;
		  wFileId=0;
		  memset(mKey,0,4);
		  wFileLen=0;
		  memset(mFileData,0,sizeof(mFileData));
		  memset(mReserve,0,sizeof(mReserve));
	  }
  }  STT_LN_WTCD_FR_VL;

  DllUseport int  WINAPI _InLnWriteCardforValue( STT_LN_WTCD_FR_VL &sttLnWtCdFrVl,HANDLE hComm);
  
  //-------------------------------------------------------------------------
  typedef struct _SALECARD
  {
	  uchar CardApplicationType;//卡片种类
	  uchar ProductyType;//车票产品类型
	  ushort ProductyCategory;//车票产品种类
	  ushort ProductSubType;//车票产品子类型
	  uchar SaleMode;//售票方式
	  uchar PassengerType;//乘客类型
	  ulong TicketAmount;//售票金额
	  uchar BegainStation[4];//起点站（或区段）站码
	  uchar EndStation[4];//终点站（或区段）站码
	  ushort CountTicketUseCount;//计次类车票使用次数
	  ulong ValidityPeriod;//有效期的时间长度
	  uchar byGroupSize;
	  uchar byPayType;//支付方式
	  uchar PayCardSerialNumber[8];//支付票卡逻辑卡号
  }SALEINFO;
//-------------------------------------------------------------------------
/*
定义函数 
int SaleCard (ulong &SerialNo,uchar *TranDateTime,SALEINFO SaleInfo,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
SLE需要进行售卡业务处理时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令通知TPU进行售卡处理。BOM/TVM等设备同AG不同，相对来说实时性要求不是特别高，所以本命令数据被TPU正确执行后将直接导致SLE返回售卡成功后的UD数据，而不需要单独的取UD指令来获取UD数据。
参数x
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
SaleInfo 为一结构体，用于指示的售卡的信息
SerAccnum UDSN或一卡通交易流水号累加值
UDData 如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
UDDataLen 如果返回UD，用于存放返回UD数据的长度
ARData 用于存放返回的AR数据，如果无AR返回，为空
ARDataLen 如果返回，用于存放AR数据长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//卡片种类
#define CDTY_YKT_S50S70 0x01 //
#define CDTY_YKT_M1 CDTY_YKT_S50S70  //一卡通S50/S70卡片
#define CDTY_YKT_CPU 0x02 //一卡通CPU卡
#define CDTY_YPT_S50S70 0x11//一票通S50/S70卡片
#define CDTY_YPT_UL 0x12 //一票通UL卡
//充值类型
#define PVTY_YKT_M1_PBPS 0x14 //公共钱包产品
#define PVTY_YKT_M1_LTPS 0x15 //小钱包
#define PVTY_YKT_M1_STPS 0x16 //学生钱包
#define PVTY_YKT_M1_TMPS 0x17 //定期产品
//============
//-------------------------------------------------------------------------

  DllUseport int WINAPI SaleCard (
	  ulong dwUdsn/*in,一卡通交易流水号*/,
	  uchar *TranDateTime/*in,交易时间*/,
	  SALEINFO &sttSaleInfo/*in,卡片信息*/,
	  uchar &SerAccnum/*out,交易流水号累加值*/,
	  TRANRESULT &TranResult/*out,tpu 执行状态*/,
	  ushort &sArgUdLen/*out,　*/,
	  uchar *pArgUdBuf/*out,*/,
	  ushort &sArgArLen/*out,　*/,
	  ARDATA &sttArgAr/*out,　*/,
	  HANDLE hComm/*in,*/
	  );
  /*
  desc: SLE需要进行售卡业务处理时发送该命令数据，TPU执行操作后返回应答数据。SLE通过该指令通知TPU进行售卡处理。BOM/TVM等设备同AG不同，相对来说实时性要求不是特别高，所以本命令数据被TPU正确执行后将直接导致SLE返回售卡成功后的UD数据，而不需要单独的取UD指令来获取UD数据。
  参数
  ulong dwUdsn				[in]	UDSN或一卡通交易流水号
  uchar *TranDateTime		[in]	交易发生的日期时间，BCD码，格式YYMMDDHHmm
  SALEINFO &sttSaleInfo		[in]	为一结构体，用于指示的售卡的信息
  uchar &sSerAccnum			[out]	UDSN或一卡通交易流水号累加值
  TRANRESULT &sttTranResult	[out]
  uchar *pArgUdBuf			[out]	如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
  ushort &sArgUdLen			[out]	如果返回UD，用于存放返回UD数据的长度
  ARDATA &sttArgAr			[out]	用于存放返回的AR数据，如果无AR返回，为空
  ushort &sArgArLen			[out]	如果返回，用于存放AR数据长度
  HANDLE hComm				[out]	标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
  返回值							成功则返回0，若有错误则返回非零值errno
  */
  DllUseport int WINAPI SaleCardOnline (
	  ulong dwUdsn/*in,一卡通交易流水号*/,
	  uchar *TranDateTime/*in,交易时间*/,
	  SALEINFO &sttSaleInfo/*in,卡片信息*/,
	  uchar &SerAccnum/*out,交易流水号累加值*/,
	  TRANRESULT &TranResult/*out,tpu 执行状态*/,
	  ushort &sArgUdLen/*out,　*/,
	  uchar *pArgUdBuf/*out,*/,
	  ushort &sArgArLen/*out,　*/,
	  ARDATA &sttArgAr/*out,　*/,
	  HANDLE hComm/*in,*/
	  );

  typedef struct STRUCT_DEPOSIT_EXPENSE
  {
	  DWORD uUdsn;
	  byte mDateTime[7];
	  DWORD uAmount;//消费金额
	  STRUCT_DEPOSIT_EXPENSE()
	  {
		  uUdsn=0;
		  memset(mDateTime,0,sizeof(mDateTime));
		  uAmount=0;
	  }
  } STT_DEPOSIT_EXPENSE;
/*
定义函数 
DllUseport int WINAPI DepositExpense(STT_DEPOSIT_EXPENSE &sttExpense,
uchar &SerAccnum,TRANRESULT &TranResult,
ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

函数说明
SLE 使用储蓄卡售UL卡时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
info 为一结构体，用于指示的补票的信息
SerAccnum UDSN或一卡通交易流水号累加值
UDData 如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
UDDataLen 如果返回UD，用于存放返回UD数据的长度
ARData 用于存放返回的AR数据，如果无AR返回，为空
ARDataLen 如果返回，用于存放AR数据长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//#define  GetUlCardByDepositCard DepositExpense
DllUseport int WINAPI DepositExpense(STT_DEPOSIT_EXPENSE &sttExpense,
	  uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

//-------------------------------------------------------------------------
typedef struct  _COMPENSATIONPAID{
	uchar CardTrantype;
	ushort ProductyCategory;// modified by chenliye//uchar ProductyType;
	uchar PayMethed;
	ulong TicketAmount;
	uchar PayAttribute;
	uchar OperationType;
	uchar BegainStation[4];
	uchar EndStation[4];
	uchar PayCardLogicID[8];
} COMPENSATIONPAID;
//-------------------------------------------------------------------------
/*
定义函数 
int CompensationPaid (ulong &SerialNo,uchar *TranDateTime, COMPENSATIONPAID info,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
BOM需要进行补票业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
info 为一结构体，用于指示的补票的信息
SerAccnum UDSN或一卡通交易流水号累加值
UDData 如果返回UD，用于存放返回的UD数据，如果无UD返回，为空
UDDataLen 如果返回UD，用于存放返回UD数据的长度
ARData 用于存放返回的AR数据，如果无AR返回，为空
ARDataLen 如果返回，用于存放AR数据长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI CompensationPaid (ulong &SerialNo,uchar *TranDateTime, COMPENSATIONPAID info,
	uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);


DllUseport int WINAPI CardBlock (ulong &SerialNo,uchar *TranDateTime, uchar &SerAccnum,TRANRESULT &TranResult,
							 ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
//-------------------------------------------------------------------------

	typedef struct _REVALUEINFO
	{
		uchar CardType;//卡片类型
		ushort RevalueType;//充值类型、及票种类型
		uchar PayMethed;	//
		ulong Amount;//
		ulong Count;//
		ulong Period;//定期卡有效期延长长度，单位：天
		uchar PayCardLogicID[8];
		_REVALUEINFO()
		{
			CardType=0;
			PayMethed=0;
			RevalueType=0;
			PayMethed=0;
			Amount=0;
			Count=0;
			Period=0;
			
		}
	} REVALUEINFO;
/*定义函数 
desc: SLE需要进行充值业务处理时发送该命令数据，TPU执行操作后返回应答数据。
  参数
  ulong dwUsdn [in] UDSN或一卡通交易流水号
  TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
  info 为一结构体，用于指示充值的信息
  UDData 用于存放返回的UD数据
  UDDataLen 如果返回UD，用于存放返回UD数据的长度
  hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
  返回值 
  成功则返回0，若有错误则返回非零值errno
 */
DllUseport int WINAPI Revalue (
		ulong dwUsdn/*in,一卡通交易流水号*/,
		uchar *TranDateTime/*in,交易时间*/,
		REVALUEINFO sttPutValInfo/*in,充值信息*/,
		uchar &bySerAccnum/*out,交易流水号累加值*/,
		TRANRESULT &sttTranRlt/*out,tpu 执行状态*/,
		ushort &sArgUdLen/*out*/,
		uchar *pArgUdBuf/*out*/,
		ushort &sArgArLen/*out*/,
		ARDATA &sttArgAr/*out*/,
		HANDLE hComm);



/*定义函数 
  desc :在线充值
  SerialNo UDSN或一卡通交易流水号
  TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
  info 为一结构体，用于指示充值的信息
  UDData 用于存放返回的UD数据
  UDDataLen 如果返回UD，用于存放返回UD数据的长度
  hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
  返回值 
  成功则返回0，若有错误则返回非零值errno
 */
int WINAPI RevalueOnline(
						 ulong dwUsdn/*in,一卡通交易流水号*/,
						 uchar *TranDateTime/*in,交易时间*/,
						 REVALUEINFO sttPutValInfo/*in,充值信息*/,
						 uchar &bySerAccnum/*out,交易流水号累加值*/,
						 TRANRESULT &sttTranRlt/*out,tpu 执行状态*/,
						 ushort &sArgUdLen/*out*/,
						 uchar *pArgUdBuf/*out*/,
						 ushort &sArgArLen/*out*/,
						 ARDATA &sttArgAr/*out*/,
						 HANDLE hComm);
// --------------------------------------------------------------------------
typedef struct _SURRENDERINFO
{
	uchar PayMethed;
	uchar CardApplicationType;
	ushort ProductyCategory;//modified by chenliye//uchar ProductType;
	uchar SurrenderFlag;
	uchar YPTSurrenderOperateType;
	uchar YKTSurrenderOperateType;
	uchar ReasoneCode;
	uchar BadCardNo[16];
	uchar CardPhyID[7]; //add by chenliye
	ulong RefundMoney;
	ulong CardBalance;
	ulong CardTransCtr;
	uchar FareProductType;
	ushort FareProductyCategory;
	ulong CardDeposit;
	ulong ServiceFee;
	ulong CardCost;    //add by chenliye
	uchar mReceiptNumber[10];//收据号
	uchar IsLimitSurrender;//是否限额内退款，01：Yes，02：NO
	_SURRENDERINFO()
	{
		PayMethed=0;
		CardApplicationType=0;
		ProductyCategory=0;
		SurrenderFlag=0;
		YPTSurrenderOperateType=0;
		YKTSurrenderOperateType=0;
		ReasoneCode=0;
		memset(BadCardNo,0,sizeof(BadCardNo));
		memset(CardPhyID,0,sizeof(CardPhyID));
		RefundMoney=0;
		CardBalance=0;
		CardTransCtr=0;
		FareProductType=0;
		FareProductyCategory=0;
		CardDeposit=0;
		ServiceFee=0;
		CardCost=0;
	memset(mReceiptNumber,0,sizeof(mReceiptNumber));//收据号
	IsLimitSurrender=0;//是否限额内退款，01：Yes，02：NO
	}
} SURRENDERINFO;

/************************************************************************/
/*
定义函数 
int CardSurrender (ulong &SerialNo,uchar *TranDateTime, SURRENDERINFO info,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm)
函数说明 
BOM需要进行退卡退资业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
info 为一结构体，用于指示退卡退资的信息
UDData 用于存放返回的UD数据
UDDataLen 如果返回UD，用于存放返回UD数据的长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
/************************************************************************/
DllUseport int WINAPI CardSurrender (ulong &SerialNo,uchar *TranDateTime, SURRENDERINFO info,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* 定义函数 
int ReplaceOldCard (ulong &SerialNo,uchar *TranDateTime, uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
BOM需要进行替换卡业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
UDData 用于存放返回的UD数据
UDDataLen 如果返回UD，用于存放返回UD数据的长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                   */
/************************************************************************/
DllUseport int WINAPI ReplaceOldCard (ulong &SerialNo,uchar *TranDateTime, uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* 定义函数 
int ReplaceNewCard (ulong &SerialNo,uchar *TranDateTime,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
BOM需要进行替换卡业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
UDData 用于存放返回的UD数据
UDDataLen 如果返回UD，用于存放返回UD数据的长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
	                                                                    */
/************************************************************************/
DllUseport int WINAPI ReplaceNewCard (ulong &SerialNo,uchar *TranDateTime,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* 定义函数 
int ActiveCard(uchar *TranDateTime,ushort ProductyCategory /uchar Category/,uchar ProductType,uchar &Statue,HANDLE hComm);
函数说明 
BOM需要进行替换卡业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
Category：卡片种类，描述卡片的应用大类别：0x01：一卡通S50/S70卡片；0x02：一卡通CPU卡；0x11：一票通S50/S70卡片；0x12：一票通UL卡
ProductType 车票产品类型
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                     */
/************************************************************************/
DllUseport int WINAPI ActiveCard(uchar *TranDateTime, uchar Category,ushort ProductyCategory,
								 uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
/************************************************************************/
/* 定义函数 
int Defercard (uchar *TranDateTime,ushort DeferTime,uchar &Statue,HANDLE hComm);
函数说明 
概要描述：BOM需要进行延期业务处理时发送该命令数据，TPU执行操作后返回应答数据。
BOM设备使用的延期命令数据只针对一卡通卡片的特殊卡，单独对学生卡进行延期操作，
且延期的时间长度是由操作员手工输入，不用产生UD；
参数
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
DeferTime有效期顺延的长度，以天为单位
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
		                                                                */
/************************************************************************/
typedef struct STRUCT_DEFERCARD
{
	DWORD uUdsn;//UDSN
	uchar mDataTime[7];//交易时间
	DWORD uCommissionCharge;//手续费
	ushort wDeferTime;//延期长度
	STRUCT_DEFERCARD()
	{
		uUdsn=0;
		memset(mDataTime,0,sizeof(mDataTime));
		uCommissionCharge=0;
		wDeferTime=0;
	}

}STT_DEFERCARD;
DllUseport int WINAPI Defercard (STT_DEFERCARD &sttDefer,
								 uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* 
int ReportingLost (ulong &SerialNo,uchar *TranDateTime, uchar Category,uchar ProductType,uchar *CardNo,uchar CredentialsType,uchar *CredentialsNo,uchar &SerAccnum,TRANRESULT &TranResult,
ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
BOM需要进行挂失业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
Category：卡片种类，描述卡片的应用大类别：0x01：一卡通S50/S70卡片；0x02：一卡通CPU卡；0x11：一票通S50/S70卡片；0x12：一票通UL卡
ProductType 车票产品类型
CardNo：一卡通卡片发行号或者一票通卡SerialNumber
CredentialsType：证件类型
CredentialsNo：证件号码
UDData 用于存放返回的UD数据
UDDataLen 如果返回UD，用于存放返回UD数据的长度
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                    */
/************************************************************************/
typedef struct _TAG_STRUCT_REPORTING_LOST
{
	ulong uSerialNo;//UDSN或一卡通交易流水号
	uchar mDateTime[BCD_TIME_LEN];//交易发生的日期时间，BCD码，格式YYMMDDHHmm
	uchar byCardType;//卡片种类
	ushort wTicketType;//车票种类
	uchar mCardNo[16];//一卡通卡片发行号或者一票通卡SerialNumber，长度为16
	uchar CredentialsType;//证件类型
	uchar mCredentialsNo[20];//证件号码,长度为20
	uchar byCardHolderSex;//持卡人性别: 01：男；02：女
	uchar mCardHolderName[20];//持卡人姓名：最长20个字节
} STT_REPORTING_LOST;
DllUseport int WINAPI ReportingLost (STT_REPORTING_LOST &sttInfo,
									 uchar &SerAccnum,TRANRESULT &TranResult,
									 ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* 定义函数 
int int WINAPI ReverseTicket (ulong &SerialNo,uchar *TranDateTime, uchar Category,uchar ProductType,uchar *ReasonCode,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
函数说明 
BOM需要进行挂失业务处理时发送该命令数据，TPU执行操作后返回应答数据。
参数
SerialNo UDSN或一卡通交易流水号
TranDateTime 交易发生的日期时间，BCD码，格式YYMMDDHHmm
Category：卡片种类，描述卡片的应用大类别：0x01：一卡通S50/S70卡片；0x02：一卡通CPU卡；0x11：一票通S50/S70卡片；0x12：一票通UL卡
ProductType 车票产品类型
ReasonCode：抵消原因代码：填写ACC标准规定的抵消原因代码，参见ACC标准
UDData 用于存放返回的UD数据
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                     */
/************************************************************************/
DllUseport int WINAPI ReverseTicket (ulong &SerialNo,uchar *TranDateTime, uchar Category,ushort ProductyCategory,/*uchar ProductType, by chenliye*/uchar *ReasonCode,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* 定义函数 
int CalculateMAC (uchar *KeyVer,uchar *Data,uchar* MAC,HANDLE hComm);
函数说明 
SLE在需要独立计算一票通MAC时发送该命令数据，TPU执行操作后返回应答数据。
参数
KeyVer计算MAC需要的密钥版本表示
Data需要计算MAC的Hash数据。
MAC：用于存放返回的MAC数据
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                     */
/************************************************************************/
DllUseport int WINAPI CalculateMAC (uchar *KeyVer,uchar *Data,uchar* MAC,HANDLE hComm);


/************************************************************************/
/* 定义函数 
int SetBusinessTime (uchar ControlCode,ulong *Time,HANDLE hComm);
函数说明 
SLE在接收到上位发送的24小时运营或延长运营的控制命令后，通过该控制命令使TPU进入24小时运营或延长运营的状态。
参数
Time延长运营的时间，单位秒；
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                     */
/************************************************************************/
DllUseport int WINAPI SetBusinessTime (uchar ControlCode,ulong Time,uchar &statue,HANDLE hComm);

/************************************************************************/
/* 定义函数 
int SetBusinessMode(uchar ControlCode,uchar *ModeCode,HANDLE hComm);
函数说明 
SLE在接收到上位发送的运营模式控制命令并成功修改自身运营模式状态后，通过该控制命令修改TPU的运营模式。
参数
ControlCode  控制代码：设置24小时运营或延长运营的控制代码，具体如下：0x01：设置模式；0x02：解除模式；
ModeCOde   模式代码，长度为2个字节
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
                                                                     */
/************************************************************************/
DllUseport int WINAPI SetBusinessMode(uchar ControlCode,uchar *ModeCode,uchar &statue,HANDLE hComm);



// --------------------------------------------------------------------------
//
// manage card Function
//
// --------------------------------------------------------------------------
//-------------------------------------------------------------------------
/*
定义函数 
int WINAPI sReaderStart(UCHAR ucNumCom,ULONG ulSpeed,CSC_HANDLE* phCsc)
函数说明 
启动读写器外部认证的通讯。
参数
ucNumCom：用来和读卡器通讯的端口号(1 至设备的最大端口号) 
ulSpeed：通讯端口速率(1200, 2400, 9600, 19200, 38400, 57600, 115200) 
phCsc：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值：
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

 DllUseport short WINAPI sReaderStart(UCHAR ucNumCom,ULONG ulSpeed,CSC_HANDLE* phCsc);

//-------------------------------------------------------------------------
/*
定义函数 
short WINAPI sReaderStop (CSC_HANDLE hCsc)
函数说明 
停止读写器外部认证的通讯。
参数
hCsc：将要被停止通信的读卡器的句柄（由sReaderStart提供）
返回值：
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport short WINAPI sReaderStop (CSC_HANDLE hCsc);

//-------------------------------------------------------------------------
/*
定义函数 
unsigned short sSelectCard(USHORT  usIndex,CSC_HANDLE hCSC)
函数说明 
选择需要认证的卡。
参数
usindex：卡座号。取值范围:1~5(1：YPT ISAM卡, 2：YKT ISAM卡,3：YPT PSAM卡, 4：YKT PSAM卡, 5：YPT管理卡)
hCSC：读卡器的句柄（由sReaderStart提供）
返回值：
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport unsigned short WINAPI sSelectCard(USHORT usIndex, CSC_HANDLE hCSC);

//-------------------------------------------------------------------------
/*
定义函数 
unsigned short sISamReadATR( UCHAR * ucSAMATR,SHORT  *Length,CSC_HANDLE hcsc) 
函数说明 
SAM卡复位取得ATR 值，读取读卡器中指定SAM 的ATR。
参数
	ucSAMATR：用于保存SAM ATR信息的指针
	Length：返回的ATR长度
	hcsc：读卡器句柄；
返回值：
成功则返回0，若有错误则返回非零值errno
*/
// //-------------------------------------------------------------------------
  DllUseport unsigned short WINAPI sISamReadATR( UCHAR * ucSAMATR,SHORT  *Length,CSC_HANDLE hcsc);
//-------------------------------------------------------------------------
/*
定义函数 
unsigned short sISamAPDU(USHORT usInDataLen, const UCHAR * pucDataIn, USHORT * pusOutDataLen, UCHAR * pucDataOut, CSC_HANDLE hCSC )
函数说明 
向ISAM 卡发送 APDU指令，向读卡器发送ISO 7816 命令。这些命令将直接传给一卡通ISAM卡。
参数
	usInDataLen：输入数据长度
	pucDataIn：指向输入数据的指针
	pusOutDataLen：指向输出数据长度的指针，输入时必须初始化
	pucDataOut：指向输出数据的指针 
	hCSC：读卡器句柄

返回值：
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport unsigned short WINAPI sISamAPDU(USHORT usInDataLen, const UCHAR * pucDataIn, USHORT * pusOutDataLen, UCHAR * pucDataOut, CSC_HANDLE hCSC );

  typedef struct _TICKETINFO 
  {
	  uchar CardType;
	  uchar ProductType;
	  ushort ProductCategory;
	  ushort ProductSubType;
	  uchar PriceType;
	  uchar PassengerType;
	  uchar Time[7];
	  uchar BegainStation[4];
	  uchar EndStation[4];
  } TICKETINFO;
/*
定义函数 
int  GetTicketPrice(TICKETINFO sTicketInfo, ulong &TicketPrice, HANDLE hComm)
函数说明:
根据相应的车票属性，TPU返回相应的车票价格

参数:
sTicketInfo：一个结构体，输入车票的属性信息
TicketPrice：输出车票的价格
hComm：读卡器句柄

返回值：
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI GetTicketPrice (TICKETINFO sTicketInfo, ulong &TicketPrice, HANDLE hComm);

//-------------------------------------------------------------------------
/*
定义函数 
int M1_LoadKey(uchar ucSector, uchar *pKey, HANDLE hComm)
函数说明:
装入指定扇区的密钥。对空卡操作，此命令只适合用KEYA方式对卡片进行通用的读操作指令

参数:
ucSector：扇区号。0～15
pKey：装入的密钥。KEY A ,长度为 6 bytes
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int M1_LoadKey(uchar ucSector, uchar *pKey, HANDLE hComm);
//-------------------------------------------------------------------------
typedef  struct _BLOCKINFO{
	uchar ucDeviceType;
	uchar ucCardType;
	uchar ucSector;
	uchar ucBlock;
} BLOCKINFO;

/*
定义函数 
int M1_ReadBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm)
函数说明:
读M1卡指定块。对空卡操作，此命令只适合用KEYA方式对卡片进行通用的写操作指令。

参数:
sBlockInfo：一个结构体，输入读块操作相关的参数信息
pData：返回的块数据,长度为 16 bytes
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int M1_ReadBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm);
//-------------------------------------------------------------------------

/*
定义函数 
int M1_WriteBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm)
函数说明:
写M1卡指定块

参数:
sBlockInfo：一个结构体，输入写块操作相关的参数信息
pData：写入的块数据,长度为 16 bytes
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int M1_WriteBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm);
//-------------------------------------------------------------------------
/*
定义函数 
int GetErrorCode(int &iErrorCode, HANDLE hComm)
函数说明:
获取读卡器 执行结果错误码

参数:
iErrorCode：读卡器执行结果错误码
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int GetErrorCode(int &iErrorCode, HANDLE hComm);
//-------------------------------------------------------------------------
/*
定义函数 
int GetDeviceMode(ushort &DeviceMode, HANDLE hComm)
函数说明:
获取读卡器 当前运行模式

参数:
iErrorCode：读卡器当前运行模式
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int GetDeviceMode(ushort &DeviceMode, HANDLE hComm);
//-------------------------------------------------------------------------


/*
定义函数 
int Para_GetPara (uchar* ParaType, uchar &bContinue, ushort PacketNumber,ushort PacketLen,uchar * Data, ushort &BackPacketNumber , ushort &BackPacketLen,HANDLE hComm);

函数说明:
返回TPU内 CD参数文件

参数:
ParaType  参数文件类型，长度为2字节
bContinue  后续包标志，表示某个参数文件是否有后续包要继续返回：0x00 表示该数据包是结束包，无后续包。0x01 表示有后续包。
PacketNumber 要获取的参数文件的分隔的数据包编号。第一个包从0x0000开始，后续依次累加。
PacketLen要获取的数据包信息长度。当参数文件大于1024 byte时，需要分包发送。每个数据包的长度最大为1024，当最后一个数据包不足1024bytes时，按照实际长度发送。
Data 数据信息，返回的数据内容，最大 1024 byte。
BackPacketNumber 用于存放返回应答中指示应答成功的包序号，最有一个为0xFFFF。
BackPacketLen 返回的数据包的长度。
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。

返回值:
成功则返回0，若有错误则返回非零值errno
*/
DllUseport int Para_GetPara(ushort wFileType, uchar &bContinue, ushort PacketNumber,
			  ushort PacketLen,uchar *Data, ushort &BackPacketNumber , ushort &BackPacketLen,HANDLE hComm);
DllUseport int SetLogFileConfig(uchar &Statue,uchar logLevel,  byte *pBitsCmd,UINT uBitsLen,HANDLE hComm);

DllUseport int Para_GetLogFile(ushort wFileType, uchar &bContinue, ushort PacketNumber,
							ushort PacketLen,uchar *Data, ushort &BackPacketNumber , ushort &BackPacketLen,HANDLE hComm);


/*
定义函数 
int LedCtrl(uchar ucLedCode, uchar ucCtrlCode, HANDLE hComm)
函数说明:
控制读卡器 Led 灯

参数:
ucLedCode：Led 标识码
ucCtrlCode：Led开、关
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI LedCtrl(uchar ucLedCode, uchar ucCtrlCode, HANDLE hComm);
//-------------------------------------------------------------------------
/*
定义函数 
int BuzzerCtrl(uchar ucCtrlCode,HANDLE hComm)
函数说明:
控制读卡器 蜂鸣器

参数:

ucCtrlCode：蜂鸣器开、关
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI BuzzerCtrl(uchar ucCtrlCode, HANDLE hComm);
//-------------------------------------------------------------------------
/*
定义函数 
int Poll_CardTest(uchar AntennaMark,uchar &CardType,uchar *CardPHId,HANDLE hComm);
函数说明 
SLE需要让TPU进入循环寻卡状态时发送该命令数据，TPU执行操作后返回应答数据。AG使用该命令数据通知TPU开始寻卡，TPU收到该指令将不停的重复寻卡，如果没有寻到卡，TPU状态将保持在“寻卡状态”并继续寻卡,如果寻到卡片，将切换到“已寻到卡状态”并返回寻卡结果。
参数
	CardType用于存放返回的卡片类型
	CardPHId 用于存放返回的卡片物理ID，长度为8
hComm：标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
返回值 
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Poll_CardTest(uchar AntennaMark/*by chen liye*/, uchar &CardType, uchar &AntennaFindMark, uchar *CardPHId,HANDLE hComm);
//---------------------------------------------------------------------------
  /*
定义函数 
int WINAPI AntennaOn(HANDLE hComm)
函数说明:

参数:
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI AntennaOn(HANDLE hComm);
//-------------------------------------------------------------------------
  /*
定义函数 
int WINAPI AntennaOff(HANDLE hComm)
函数说明:

参数:
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI AntennaOff(HANDLE hComm);
//-------------------------------------------------------------------------

/*
定义函数 
int WINAPI AntennaOff(HANDLE hComm)
函数说明:

  参数:
  hComm：读卡器句柄
  
	返回值:
成功则返回0，若有错误则返回非零值errno
*/
DllUseport int WINAPI BMACAuthenCtr(HANDLE hComm);
/*
	the end of function
*/
 
DllUseport double WINAPI GetRunTime();

typedef struct tag_AuthenCtr
{
	bool operateType;
	uchar SystemTime[8];
	uchar authenAmount[5];
	uchar UpdteTime[8];
	uchar UpdteFlag[5];
}AuthenCtr_t;
 /*定义函数 
int WINAPI SetOrGetBMACAuthenCtr(AuthenCtr_t &AuthenCtr, HANDLE hComm)
函数说明:

参数:
AuthenCtr 设置或获取授权额度余额
hComm：读卡器句柄

返回值:
成功则返回0，若有错误则返回非零值errno
*/
DllUseport int WINAPI SetOrGetBMACAuthenCtr(AuthenCtr_t &AuthenCtr, HANDLE hComm);


typedef struct tag_BMACDirectConstructionSaleOrIncreaceTransaction
{
	uchar Obligate[33];
}BMAC_DIRECT_CONSTRUCTION_SALE_OR_INCREACE_TRANSACTION;
/***************************************************************************************************************************
函数定义：
int WINAPI BMACDirectConstructionSaleOrIncreaceTransaction(BMAC_DIRECT_CONSTRUCTION_SALE_OR_INCREACE_TRANSACTION BMAC_DCSIT, 
															TRANRESULT &TranResult, 
															ushort &UDDatalen, 
															uchar *UDData, 
															ushort &ARDataLen,
															ARDATA &ARData,
															HANDLE hComm);
函数功能：
当一卡通售卡或充值交易过程中，钱包加值操作发生异常且卡片已移开无法继续操作时，SLE可发送该命令数据，TPU执行操作后返回应答数据，应答数据中包含完整的UD数据；

参数说明：
BMAC_DCSIT：一卡通直接生成售卡或充值交易预留属性
TranResult：交易结果 
UDDatalen： UD数据长度
UDData： UD数据
ARDataLen：AR数据长度
ARData：AR数据
hComm：读卡器句柄

返回值：成功则返回0，若有错误则返回非零值errno
****************************************************************************************************************************/

DllUseport int WINAPI BMACDirectConstructionSaleOrIncreaceTransaction(BMAC_DIRECT_CONSTRUCTION_SALE_OR_INCREACE_TRANSACTION BMAC_DCSIT, 
																	  uchar &SerAccnum,
																	  TRANRESULT &TranResult, 
																	  ushort &UDDatalen, 
																	  uchar *UDData, 
																	  ushort &ARDataLen,
																	  ARDATA &ARData,
																	  HANDLE hComm);
//===============================================================================================================
	//note by djm
//===============================================================================================================
 /*
 
 desc：签到成功后设置TPU一卡通授权额度、授权时间、校对时钟，同时清空一卡通审计数据。
 
 参数说明：
 SamId		[IN]	Sam号			UCHAR *	ASCII	12	ISam卡号
 SysDatetime	[IN]	系统时间		UCHAR *	ASCII	14	格式：YYYYMMDDhhmmss
 LimitAmt	[IN]	授权额度		ULONG 				一次签到签退之间的最大充值总额，单位：分；对钱包增加时扣除相同金额
 LimitTime	[IN]	授权截至时间	UCHAR *	ASCII	14	格式：YYYYMMDDhhmmss
 hComm：读卡器句柄
 返回值：成功则返回0，若有错误则返回非零值errno
 */
 DllUseport ushort WINAPI sYKYSetLimite (UCHAR *pSamId, UCHAR *pSysDatetime, ULONG ulLimitAmt,  UCHAR *pLimitTime, HANDLE hComm);
 
 typedef struct tag_AUDITDATA
 {
 	UCHAR IncreaceTrancationNumber[4];
 	UCHAR IncreaceTrancationValue[8];
 	UCHAR SaleCardNumber[4];
 	UCHAR SalecardValue[8];
 	UCHAR MultiRideTrancationNumber[4];
 	UCHAR MultiRideTrancationValue[8];
 	UCHAR RemainningLimitAmt[8];
 } AUDITDATA_t;
 /*
 desc：签退之前从TPU中读取一卡通审计数据。
 参数说明：
 SamId		[IN]	Sam号			UCHAR *	ASCII	12	ISam卡号
 AuditData	[Out]	审核数据	UCHAR *			结构见下表
 												  充值交易总笔数	4	ASCII_H	遵从高字节在右的字节序
 
 												  充值交易总金额	8	ASCII_H	遵从高字节在右的字节序
 												  售卡总笔数		4	ASCII_H	遵从高字节在右的字节序
 												  售卡总押金		8	ASCII_H	遵从高字节在右的字节序
 												  计次票交易总笔数	4	ASCII_H	遵从高字节在右的字节序
 												  计次票交易总金额	8	ASCII_H	遵从高字节在右的字节序
 												  授权额度余额		8	ASCII_H	遵从高字节在右的字节序
 hComm：读卡器句柄
 返回值：成功则返回0，若有错误则返回非零值errno
 */
 DllUseport ushort WINAPI sYKYReadLimit (UCHAR *pSamId, UCHAR *pAuditData, HANDLE hComm);
//===============================================================================================================

//===============================================================================================================

/* 
   定义::TPU开始初验
	 byte *pTime,[in] 日期时间,固定长度7个节字
   byte byIsCheck,[in] 是否时进行初检
   HANDLE hComm,[in] 串口句柄
   byte byTpuState [out] 返回tpu的状态码
*/
DllUseport int WINAPI SetCheck(byte *pTime,byte byIsCheck,HANDLE hComm,byte &byTpuState);
/*
	定义:超时处理
	DWORD dwUdsn,[in] UDSN或一卡通交易流水号
	byte *pTime,[in] 日期时间,固定长度7个节字
	byte byDo,[in] 是否时进行初检处理
	HANDLE hComm,[in] 串口句柄
	uchar &bySerAccnum[out],交易流水号累加值,
	TRANRESULT &sttTranRlt[out],tpu 执行状态,
	ushort &sArgUdLen[out],
	uchar *pArgUdBuf[out],
	ushort &sArgArLen[out],
	ARDATA &sttArgAr[out],
	
*/

DllUseport int WINAPI SetTimeOutDeal(DWORD dwUdsn,byte *pTime,byte byDo,HANDLE hComm,
									 uchar &bySerAccnum/*out,交易流水号累加值*/,
									 TRANRESULT &sttTranRlt/*out,tpu 执行状态*/,
									 ushort &sArgUdLen/*out*/,
									 uchar *pArgUdBuf/*out*/,
									 ushort &sArgArLen/*out*/,
									 ARDATA &sttArgAr/*out*/);


/*
desc:UL整卡读写
byte bySamYctType	[in] //一卡通SAM卡类别
byte bySamYptType	[in]//一票通SAM卡类别
bool bWrite			[in] // false 为读卡，true 为写卡
byte *psCardData	[in/out]//卡片数据
DWORD *puCardDataLen [in/out]//卡片数据长度
HANDLE hComm [in] 标识唯一读卡器的句柄。当驱动处理多个读卡器时，用来标识指定读卡器。
*/
#define CARDTYPE_YCT_M1 0x01//一卡通M1
#define CARDTYPE_YCT_CPU 0x02//一卡通CPU
#define CARDTYPE_YPT_M1 0x11//一票通M1;
#define CARDTYPE_YPT_UL 0x12//一票通UL；
#define CARDTYPE_YPT_CPU 0x13//一票通CPU；
//
#define CARDTYPE_CPU_ADF1 0x01 //建设部密钥  ADF1
#define CARDTYPE_CPU_ADF2 0x02 //轨道交通密钥 ADF2

DllUseport int WINAPI ReadWriteCardInfo(byte bySamYctType,byte bySamYptType,bool bWrite,
							 byte byCardType,HANDLE hComm,byte *psCardData,WORD *pwCardDataLen,
							 WORD wCardSize);

}

#ifdef __cplusplus
}
#endif
#endif
//-------------------------------------------------------------------------
//add by djm
//int SendCmd(WORD wCmd,uchar &byTpuStatue,HANDLE hComm);
//int SendCmd(WORD wCmd,uchar* pData/*in*/,ushort wDataLen,uchar &byTpuStatue/*out*/,HANDLE hComm);

// int DllUseport SendCmd2(WORD wCmd,HANDLE hComm,byte * pRvBuf/*out*/,WORD *pwRvLen/*out*/,WORD wRvSize);
// int DllUseport SendCmd2(WORD wCmd,uchar* pData/*in*/,ushort wDataLen,
// 			HANDLE hComm,byte * pRvBuf/*out*/,WORD *pwRvLen/*out*/,WORD wRvSize);
//end
// 
// //---------------------------------------------------------------------------