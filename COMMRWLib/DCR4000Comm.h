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
#define BUF_SIZE		1024			// ���������ܷŵ�����ֽڸ��� (0x00~0xFF)=256.
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
//��Ӧ�����:TPU��ִ������Ľ����0x00 ��ʾ�ɹ�,���㼴Ϊ�������.
//// 
//
//***************************************************************************
#define CE_OK 							0x00 //sucess 
#define LOGON_YUEPIAO_UPDATE			26028	//��Ʊ�汾��Ҫ����
#define LOGON_DEVICE_UPDATE				26029	//��Ʊ�汾��Ҫ����

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
#define EDEVICE						0xFF20 //��������豸���Ͳ���ȷʱ.
#define EPACKET						0xFF21 //���񲻵����е����ݰ�.
#define ERECV						0xFF22 //����ȡ��������ʱ
#define E_NOT_ENOUGH_BUF			0xFF23 //���ӿں����ṩ���ݵ�������治��ʱ
#define E_EMPTY						0xFF24 //��û������Ҫ����ʱ
#define E_ALLOC_MEMORY				0xFF25 //�����ڴ������
#define E_FILE						0xFF26 //�ļ�����
#define E_UP_UDDATA					0xFF31 //���ϴ����һ����
#define E_CHECK_DEAL				0xFF32 //�豸��Ҫ����(ֻ��CPU������)
#define E_END_CHECK					0xFF33 //�������(ֻ��M1������)
#define E_CARD						0xFF34 //��Ƭ����
#define E_POINTTER					0xFF35 //ָ�����
#define E_TIMEOUT_DEAL				0xFF36 //��ʱ����
#define E_NOT_ENOUGH_DATA			0xFF37 //���յ����ݳ��Ȳ���
#define E_CONNECT_SERVER_ERROR		0xFF38 //���ӷ�����ʧ��
#define E_FUN_PARA					0xFF39 //������������
#define E_CONFIG_FILE				0xFF3A
#define E_CONNECT_CENTERHOST		0xFF3B
#define E_RESPONSE_CENTERHOST		0xFF3C
#define E_CHECK_CENTERHOST			0xFF3D
#define E_COMMAND					0xFF3E
#define E_PACKET					0xFF3F

#define E_LN_CMD_RTN				0xFF40 //������Ӧ���ļ��е����������

#define E_LN_RTN_CMD				0xFF50 //������Ӧ���ļ��е����������

#define E_LN_RTN_ERR				0xE000 //��������������˴������
#define E_LN_RTN_MAC2_LEN			0xFF52 //������������ص�MAC2���ȴ���(���Ȳ�����4)
#define E_LN_READ_APPTYPE			0xFF53 //�����������ص�"��Ƭ����"����.
#define E_LN_READ_CNT_FL_MD			0xFF54 //�����������ص�"���޸��ļ��ĸ���"����.
#define E_LN_SEND					0xFF61 //����������������ʧ�ܡ�
#define E_LN_RECV					0xFF62 //�����������ϻ�ȡ����ʧ�ܡ�

#define E_LN_KEYTYPE				0xFF64 //�����ۿ�(��ֵ)ʱ,��TPU�ж�ȡ����Կ���Ͳ���ȷ

#define E_OK						0x0000
#define LNE_TERMPARA_UP 26028
#define LNE_POSTPARA_UP 26029 

	//end
#define TPU_POWERDOWN                   0xFFF4 //��TPU�ϵ�����

//==��������======================================
#define CMDLN_CPU_PUTVALUE 0x2041 //һ��ͨ��ֵƱ��ֵ
#define CMDLN_CPU_REVOKE 0x2042  //һ��ͨ��ֵƱ����
#define CMDLN_CPU_UPDATE 0x2043 //һ��ͨ��ֵƱ�����ļ�Ȩ������
#define CMDLN_CPU_AUTHEN 0x2044 //CPU��Ȩ����֤����(��)
#define CMDLN_SAM_AUTHEN 0x2051 //SAM��Ȩ����֤����(��)

#define CMDLN_DPST_PUTVALUE 0x2045 //��ֵƱ��ֵȨ������
#define CMDLN_DPST_REVOKE 0x2046  //��ֵƱ����Ȩ������
#define CMDLN_DPST_UPDATE 0x2047 //��ֵƱ�����ļ�Ȩ������

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
desc:��ʾ������Ϣ
*/
DllUseport char * WINAPI GetErrorString(DWORD dwErr);
//====
	
	
	/*
���庯�� 
int OpenReader( UCHAR nPort, ULONG ulSpeed, BOOL bIsUSB,HANDLE * phComm )
����˵�� 
������д���ⲿ��֤��ͨѶ��
����
nPort�������Ͷ�����ͨѶ�Ķ˿ں�(1 ���豸�����˿ں�) 
ulSpeed��ͨѶ�˿�����(1200, 2400, 9600, 19200, 38400, 57600, 115200)
bIsUSB :������ʶ ��Com�����ӻ���USB ����
phComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI OpenReader(UCHAR nPort, ULONG ulSpeed, bool bIsUSB,HANDLE &hComm);

/*
���庯�� 
int CloseReader(HANDLE hComm )
����˵�� 
ֹͣ��д���ⲿ��֤��ͨѶ��
����
hComm����Ҫ��ֹͣͨ�ŵĶ������ľ������OpenReader�ṩ��
����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
�������
��ο���¼��������
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI CloseReader(HANDLE hComm );


/************************************************************************/
/*                                                                     */
/************************************************************************/
BOOL beOpened(uchar nPort);

//-------------------------------------------------------------------------
/*INITSTRUCT �ṹ��*/

typedef struct _INITSTRUCT {  
	ulong DelayTime; //����ָ��ȴ�ʱ�䡣
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
���庯�� 
int Init_TPU(INITSTRUCT InitData,uchar &Statue,HANDLE hComm);
����˵�� 
��TPU���г�ʼ��������TPU�����л����������ã���ʼ�������ݰ�����������·�롢վ�롢����ģʽ��TPU��Ҫ���������û���������
����
	IniData	��ų�ʼ���������ݣ���ʽΪ�ṹ��
	Statue 	������ִ�гɹ�ʱ�����ڴ�ŷ��ص��豸״̬
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Init_TPU(INITSTRUCT InitData,uchar &Statue,HANDLE hComm);
	
//-------------------------------------------------------------------------
/*
desc:����λ�������Ժ����ȶ�TPU���С���λ����ʼ������ǩ��(Logon_Tpu).
	Logon_Tpu:Ϊ���ϲ�������Ҫ����������������ͨ�š�
*/
DllUseport int WINAPI Logon_Tpu(byte &byState,HANDLE hComm);
/*
desc:����λ���ػ�֮ǰ��Ҫ��TPU����ǩ��(Exit_Tpu).
Exit_Tpu:Ϊ���ϲ�������Ҫ����������������ͨ�š�
*/
DllUseport int WINAPI Exit_Tpu(byte &byState,HANDLE hComm);
/*
���庯�� 
int Reset_TPU(uchar &Statue,HANDLE hComm);
����˵�� 
SLE������TPUʱ������Ҫ���³�ʼ��TPUʱ���͸��������ݣ�TPUִ�и�λ�����󷵻�Ӧ�����ݡ�TPU��λ�����ľ��嶯����������SAM�����и�λ��������ȡ��SAM���ţ���TPU�ڵ�RAM���и�λ������
����
	Statue 	������ִ�гɹ�ʱ�����ڴ�ŷ��ص��豸״̬
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Reset_TPU(uchar &Statue,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int Stop_TPU(uchar &Statue,HANDLE hComm);
����˵�� 
SLE��Ҫ��TPU���ص���δ��ʼ��״̬��ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
	Statue 	������ִ�гɹ�ʱ�����ڴ�ŷ��ص��豸״̬
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Stop_TPU(uchar &Statue,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int Abort _TPU(uchar &Statue,HANDLE hComm);
����˵�� 
SLE��Ҫ��TPU��ֹ��ǰ��ҵ����������ؿ���״̬ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
	Statue 	������ִ�гɹ�ʱ�����ڴ�ŷ��ص��豸״̬
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Abort_TPU(uchar &Statue,HANDLE hComm);

  DllUseport int WINAPI Erase_LogFile(uchar &Statue,unsigned short &fileType,HANDLE hComm);
  DllUseport int WINAPI Erase_Para(uchar &Statue,unsigned short &fileType,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int SetTimer(uchar* SetTime,uchar &Statue,HANDLE hComm);
����˵�� 
SLE��Ҫ����TPUʱ��ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
	Time  Ҫ���õ�TPUʱ����Ϣ��BCD�룬��ʽΪ��YYYYMMDDHHmmSS��
TPUTime 	������ִ�гɹ�ʱ�����ڴ�ŷ��ص�TPUʱ����Ϣ,BCD�룬��ʽΪ��YYYYMMDDHHmmSS��
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI SetReadTimer(uchar* SetTime,uchar *TPUTime,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int GetTime(uchar *Time,HANDLE hComm);
����˵�� 
SLE��Ҫ��ȡTPUʱ��ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ������
����
	Time  ���ڴ��TPUʱ����Ϣ��BCD�룬��ʽΪ��YYYYMMDDHHmmSS��
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
������� 
��ο���¼��������
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
���庯�� 
int WINAPI GetTPUInfo(TPUINFO &info,HANDLE hComm);
����˵�� 
SLE��Ҫ��ȡTPU�Ļ�����Ϣʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ���������ȡTPU�Ļ�����Ϣ��������SAM��ID,TPU����汾��TPUӲ���汾���豸ID�ȡ�
����
	info ���ڴ��TPU��Ϣ�Ľṹ
	hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
/*PARAINFO�ṹ˵��*/
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
���庯�� 
int GetParaInfo(ushort ParaNum,STRUCT PARAINFO *Parainfo,HANDLE hComm);
����˵�� 
SLE��Ҫ��ȡTPU��ǰ���в����İ汾��Ϣʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�ͬһ��������ļ���TPU�ڲ��п��ܴ���2�ݣ���ǰ�汾��δ���汾����ָ���ȡ�����汾ʱ�������ز����ļ��ĵ�ǰ�汾��δ���汾����Ϣ��
����
	ParaNum     ������ɹ�ִ��ʱ�����ڴ�Ų�������
	ParaInfo 	PARAINFO�ṹ��������ִ�гɹ�ʱ�����ڴ�ŷ��صĲ����汾��Ϣ
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI GetParaInfo(ushort &ParaNum,PARAINFO *Parainfo,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int Para_Downdload (uchar* ParaType,uchar bContinue,ushort PacketNumber,ushort PacketLen,uchar * Data,ushort &ReceivePacketNumber,HANDLE hComm);
����˵�� 
SLE��Ҫ��TPU�·������ļ�ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
	ParaType  �����ļ����ͣ�����Ϊ2�ֽ�
	bContinue  ��������־����ʾĳ�������ļ��Ƿ��к�����Ҫ�������أ�0x00 ��ʾ�����ݰ��ǽ��������޺�������0x01 ��ʾ�к�������
	PacketNumber �����ļ�����ʱ�ķָ������ݰ���š���һ������0x0000��ʼ�����������ۼӣ����һ������OxFFFF��
PacketLen���ݰ���Ϣ���ȡ��������ļ�����256bytesʱ����Ҫ�ְ����͡�ÿ�����ݰ��ĳ������Ϊ256�������һ�����ݰ�����256bytesʱ������ʵ�ʳ��ȷ��͡�
Data ������Ϣ�����ص��������ݣ����256 byte��
ReceivePacketNumber ���ڴ�ŷ���Ӧ����ָʾӦ��ɹ��İ���š�
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int WINAPI EXEC_Firmware_Downdload (ushort PacketNumber,uchar * Data,HANDLE hComm)
����˵�� 
SLE��Ҫ����TPU���ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ�����TPU��Ӧ�ó��򡣣�˵����TPU�ĳ����Ϊ�������֣����س����Ӧ�ó������س�����Ҫ�������������͸��£�Ӧ�ó�����Ҫ��������ҵ����
����
	PacketNumber �����ļ�����ʱ�ķָ������ݰ���š���һ������0x0000��ʼ�����������ۼӣ����һ������OxFFFF��
	Data ������Ϣ�����ص��������ݣ����128 byte,���һ��������128�ֽڣ���������0x00����
	hcomm ��ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI EXEC_Firmware_Downdload (ushort wPkOrder,uchar * Data,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int EXEC_Firmware_Update (HANDLE hComm);
����˵�� 
SLE��Ҫ����TPU���ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ�����TPU��Ӧ�ó��򡣣�˵����TPU�ĳ����Ϊ�������֣����س����Ӧ�ó������س�����Ҫ�������������͸��£�Ӧ�ó�����Ҫ��������ҵ������
����
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI EXEC_Firmware_Update(HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int APP_Firmware_Downdload (ushort PacketNumber,uchar * Data,HANDLE hComm);
����˵�� 
SLE��Ҫ����TPU���ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ�����TPU��Ӧ�ó��򡣣�˵����TPU�ĳ����Ϊ�������֣����س����Ӧ�ó������س�����Ҫ�������������͸��£�Ӧ�ó�����Ҫ��������ҵ����
����
	PacketNumber �����ļ�����ʱ�ķָ������ݰ���š���һ������0x0000��ʼ�����������ۼӣ����һ������OxFFFF��
Data ������Ϣ�����ص��������ݣ����128 byte,���һ��������128�ֽڣ���������0x00��
ReceivePacketNumber ���ڴ�ŷ���Ӧ����ָʾӦ��ɹ��İ���š�
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

 DllUseport int WINAPI APP_Firmware_Downdload(ushort wPkOrder,uchar * Data,HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int APP_Firmware_Update (HANDLE hComm);
����˵�� 
SLE��Ҫ����TPU���ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ�����TPU��Ӧ�ó��򡣣�˵����TPU�ĳ����Ϊ�������֣����س����Ӧ�ó������س�����Ҫ�������������͸��£�Ӧ�ó�����Ҫ��������ҵ����
����
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int GetStatue(uchar &Statue);
����˵�� 
SLE��Ҫ��ȡTPU��ǰ״̬ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLE����ͨ��ÿ��һ��ʱ���ȡTPU״̬�����SLE��TPU֮���ͨ������״��
����
	Statue 	������ִ�гɹ�ʱ�����ڴ�ŷ��ص��豸״̬
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int Poll_Card(uchar AntennaMark,uchar &CardType,uchar *CardPHId,HANDLE hComm);
����˵�� 
SLE��Ҫ��TPU����ѭ��Ѱ��״̬ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�AGʹ�ø���������֪ͨTPU��ʼѰ����TPU�յ���ָ���ͣ���ظ�Ѱ�������û��Ѱ������TPU״̬�������ڡ�Ѱ��״̬��������Ѱ��,���Ѱ����Ƭ�����л�������Ѱ����״̬��������Ѱ�������
����
	CardType���ڴ�ŷ��صĿ�Ƭ����
	CardPHId ���ڴ�ŷ��صĿ�Ƭ����ID������Ϊ8
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int int WINAPI EnterStation_AG(uchar* tranTime, ulong SerialNO,uchar bRetrunUD,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

  ����˵�� 
	AG֪ͨTPU���н�վҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�AGͨ����ָ��֪ͨTPU����AG��վ���ף�TPU��ɽ�վҵ������д����ɺ��AG����ִ�н����ע�⣺SLE��ͨ����վ���������еġ��Ƿ���������UD��������TPU�Ƿ���������UD��ϸ��Ϣ��
����
	STATION_INFO *pStation [in] //
	
	SerAccnum UDSN��һ��ͨ������ˮ���ۼ�ֵ
	UDData �������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
	UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
	ARData ���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
	ARDataLen ������أ����ڴ��AR���ݳ���
	hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
	�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
typedef struct _STATION_INFO 
{
	uchar mTime[7];//e���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
	ulong uSerialNo;//UDSN��һ��ͨ������ˮ��
	uchar bReturnUd;//�Ƿ񷵻�UD�ı�ʶ
	uchar mCardBoxsId[4];//Ʊ����
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
���庯�� 
int ExitStation_AG(uchar* tranTime, ulong SerialNO,uchar bRetrunUD,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
SLE֪ͨTPU���г�վҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�AGͨ����ָ��֪ͨTPU����AG��վ���ף�TPU��ɳ�վҵ������д����ɺ��AG����ִ�н����ע�⣺SLE��ͨ����վ���������еġ��Ƿ���������UD��������TPU�Ƿ���������UD��ϸ��Ϣ��
����
	tranTime���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
	SerialNO UDSN��һ��ͨ������ˮ��
	bRetrunUD �Ƿ񷵻�UD�ı�ʶ
	SerAccnum UDSN��һ��ͨ������ˮ���ۼ�ֵ
	UDData �������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
	UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
	ARData ���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
	ARDataLen ������أ����ڴ��AR���ݳ���
	hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI ExitStation_AG(STATION_INFO *pStation,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);


//-------------------------------------------------------------------------
/*
���庯�� 
int WINAPI GetUDData(uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
SLE��Ҫ��TPU��ȡδ�ϴ���SLE��UD���ݣ�����һ��ͨ��ACC�������ݣ�ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerAccnum UDSN��һ��ͨ������ˮ���ۼ�ֵ
UDData �������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
ARData ���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
ARDataLen ������أ����ڴ��AR���ݳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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

//========�޸ļ������ֿ�����Ϣ==========
typedef struct STRUCT_RENMCD_CHG
{
	DWORD uUdsn;//UDSN
	byte mDateTime[7];//����ʱ��
	byte byTypeDsg;//�ֿ������ͱ�ʶ
	byte byStaffDsg;//�ֿ���ְ����ʶ
	char szName[20];//�ֿ�������
	char szCreditNo[32];//�ֿ���֤������
	byte byCreditType;//�ֿ���֤������
	WORD wLfAmt;//Ѻ����֣�
	byte bySex;//�ֿ����Ա�
	byte byAppVer;//Ӧ�ð汾
	//֤����Ч��	7	BCD
	byte mCertificateValidity[7];
	//�ֿ��˵绰	10 	BCD	
	byte mTel[10];
	//�ֿ���Email	60	ASCII
	char mEmail[60];
	//�ֿ��˵�ַ1	128	ASCII
	char mAddr1[128];
	//�ֿ��˵�ַ2	128	ASCII
	char mAddr2[128];
	//Ԥ�� 24 bytes
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
���庯�� 
int WINAPI UpdateRealNameInfo(uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� :�޸�ʵ�����е��û���Ϣ.
����
STT_RENMCD_CHG &sttReNmInfo [in] �������
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
DllUseport int WINAPI UpdateRealNameInfo(STT_RENMCD_CHG &sttReNmInfo,uchar &SerAccnum,TRANRESULT &TranResult,	ushort &UDDatalen,
										 uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

//========================================




//-------------------------------------------------------------------------
/*CARDINFO �ṹ��˵��*/
/*
uchar LogInfo[256]:���鳤����ԭ����256��Ϊ2048��,����<SLE-TPU �ӿ� ������.doc>
*/
typedef struct _CARDINFO
{
	uchar CardPHType;//��Ʊ��������
	ushort CardInfoLen;//�����ݳ���
	uchar PayAreaReticketCode;//��������Ʊ�������
	uchar NoPayAreaReticketCode;//�Ƿ�������Ʊ�������
	ulong RepayTicketAmount;//���鲹Ʊ���
	ulong ReturnTicketAmount;//������Ʊ���
	ulong ReuturnAmount;//[4];�������ʽ��
	ulong ReturnCardFee;//[4];������Ʊ�����ѽ��
	uchar isBigMoneyCard;//�Ƿ�Ϊ��
	uchar AntennaMark; // ���߱�־
	uchar CardInfo[512];//����Ϣ
	uchar ucLogNum;//ʷ��������
	uchar LogInfo[32];//ʷ��������
}CARDINFO;

typedef struct CARDREADMISC
{
	//CPU ��Ƭ��������
	uchar ValidCheckMark;//�Ϸ��Լ���־(1 bytes)
	uchar TransCode;//ҵ���ܴ���
	uchar AreaPaidMark;//��������־
	uchar TransTime[7];//��ǰʱ��
	uchar AntennaMark;//����Ѱ����־
	uchar ucReadFlag;//��Ʊǰ�������
	uchar ucStation[4];//��Ʊǰ��վվ��
	uchar ucGetTransLog;//��ʷ���׶�ȡ���
}CardReadMisc_t, *pCardReadMisc_t;

//-------------------------------------------------------------------------
/*
desc:SLE��Ҫͨ��TPU��ȡ��Ƭ��Ϣʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ��ʹTPU���ж�����������TPU�յ���һ����ʱ�����������߷�Χ��Ѱ����Ѱ�����Ժ����Ȱ���ؽ���������ɶԿ��Ļָ��������ϵ��������ͨ��Ӧ�����ݰ���AFC�豸����,������ָ�Ŀ�����һ��ͨ����һƱͨ������,���ڲ�ͬ�Ŀ�,TPU��Ӧ�������ǲ�ͬ�ġ����TPUû���ҵ����������������޿�Ӧ���SLE��
����
	bCheck ָʾ�Ƿ��鿨Ƭ�Ϸ���
	TranCode ָʾ������������
	CardInfo Ϊһ�ṹ�壬���ڴ�ŷ��صĿ�Ƭ��Ϣ
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI ReadCardInfo (pCardReadMisc_t pCardReadMisc,
	  /*uchar bCheck,uchar TranCode,uchar PayFlag,uchar *TranDateTime,*/
	  CARDINFO &CardInfo,HANDLE hComm);
 //-------------------------------------------------------------------------
#define LN_FILE_SIZE 360
  typedef struct STRUCT_LN_WRITECARD_FOR_VALUE
  {
	  byte byKeyType;//��Ƭ����//��Կ����
	  byte byBusyType;//ҵ������
	  //ʣ���޸��ļ�����
	  byte byLeafCntFile;
	  //�ļ�ID
	  WORD wFileId;
	  byte  mKey[4];//��Կ
	  WORD wFileLen;//�ļ�����
	  byte mFileData[LN_FILE_SIZE];//�ļ�����
	  byte mReserve[16];//Ԥ��
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
	  uchar CardApplicationType;//��Ƭ����
	  uchar ProductyType;//��Ʊ��Ʒ����
	  ushort ProductyCategory;//��Ʊ��Ʒ����
	  ushort ProductSubType;//��Ʊ��Ʒ������
	  uchar SaleMode;//��Ʊ��ʽ
	  uchar PassengerType;//�˿�����
	  ulong TicketAmount;//��Ʊ���
	  uchar BegainStation[4];//���վ�������Σ�վ��
	  uchar EndStation[4];//�յ�վ�������Σ�վ��
	  ushort CountTicketUseCount;//�ƴ��೵Ʊʹ�ô���
	  ulong ValidityPeriod;//��Ч�ڵ�ʱ�䳤��
	  uchar byGroupSize;
	  uchar byPayType;//֧����ʽ
	  uchar PayCardSerialNumber[8];//֧��Ʊ���߼�����
  }SALEINFO;
//-------------------------------------------------------------------------
/*
���庯�� 
int SaleCard (ulong &SerialNo,uchar *TranDateTime,SALEINFO SaleInfo,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
SLE��Ҫ�����ۿ�ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ��֪ͨTPU�����ۿ�����BOM/TVM���豸ͬAG��ͬ�������˵ʵʱ��Ҫ�����ر�ߣ����Ա��������ݱ�TPU��ȷִ�к�ֱ�ӵ���SLE�����ۿ��ɹ����UD���ݣ�������Ҫ������ȡUDָ������ȡUD���ݡ�
����x
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
SaleInfo Ϊһ�ṹ�壬����ָʾ���ۿ�����Ϣ
SerAccnum UDSN��һ��ͨ������ˮ���ۼ�ֵ
UDData �������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
ARData ���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
ARDataLen ������أ����ڴ��AR���ݳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//��Ƭ����
#define CDTY_YKT_S50S70 0x01 //
#define CDTY_YKT_M1 CDTY_YKT_S50S70  //һ��ͨS50/S70��Ƭ
#define CDTY_YKT_CPU 0x02 //һ��ͨCPU��
#define CDTY_YPT_S50S70 0x11//һƱͨS50/S70��Ƭ
#define CDTY_YPT_UL 0x12 //һƱͨUL��
//��ֵ����
#define PVTY_YKT_M1_PBPS 0x14 //����Ǯ����Ʒ
#define PVTY_YKT_M1_LTPS 0x15 //СǮ��
#define PVTY_YKT_M1_STPS 0x16 //ѧ��Ǯ��
#define PVTY_YKT_M1_TMPS 0x17 //���ڲ�Ʒ
//============
//-------------------------------------------------------------------------

  DllUseport int WINAPI SaleCard (
	  ulong dwUdsn/*in,һ��ͨ������ˮ��*/,
	  uchar *TranDateTime/*in,����ʱ��*/,
	  SALEINFO &sttSaleInfo/*in,��Ƭ��Ϣ*/,
	  uchar &SerAccnum/*out,������ˮ���ۼ�ֵ*/,
	  TRANRESULT &TranResult/*out,tpu ִ��״̬*/,
	  ushort &sArgUdLen/*out,��*/,
	  uchar *pArgUdBuf/*out,*/,
	  ushort &sArgArLen/*out,��*/,
	  ARDATA &sttArgAr/*out,��*/,
	  HANDLE hComm/*in,*/
	  );
  /*
  desc: SLE��Ҫ�����ۿ�ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�SLEͨ����ָ��֪ͨTPU�����ۿ�����BOM/TVM���豸ͬAG��ͬ�������˵ʵʱ��Ҫ�����ر�ߣ����Ա��������ݱ�TPU��ȷִ�к�ֱ�ӵ���SLE�����ۿ��ɹ����UD���ݣ�������Ҫ������ȡUDָ������ȡUD���ݡ�
  ����
  ulong dwUdsn				[in]	UDSN��һ��ͨ������ˮ��
  uchar *TranDateTime		[in]	���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
  SALEINFO &sttSaleInfo		[in]	Ϊһ�ṹ�壬����ָʾ���ۿ�����Ϣ
  uchar &sSerAccnum			[out]	UDSN��һ��ͨ������ˮ���ۼ�ֵ
  TRANRESULT &sttTranResult	[out]
  uchar *pArgUdBuf			[out]	�������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
  ushort &sArgUdLen			[out]	�������UD�����ڴ�ŷ���UD���ݵĳ���
  ARDATA &sttArgAr			[out]	���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
  ushort &sArgArLen			[out]	������أ����ڴ��AR���ݳ���
  HANDLE hComm				[out]	��ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
  ����ֵ							�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
  */
  DllUseport int WINAPI SaleCardOnline (
	  ulong dwUdsn/*in,һ��ͨ������ˮ��*/,
	  uchar *TranDateTime/*in,����ʱ��*/,
	  SALEINFO &sttSaleInfo/*in,��Ƭ��Ϣ*/,
	  uchar &SerAccnum/*out,������ˮ���ۼ�ֵ*/,
	  TRANRESULT &TranResult/*out,tpu ִ��״̬*/,
	  ushort &sArgUdLen/*out,��*/,
	  uchar *pArgUdBuf/*out,*/,
	  ushort &sArgArLen/*out,��*/,
	  ARDATA &sttArgAr/*out,��*/,
	  HANDLE hComm/*in,*/
	  );

  typedef struct STRUCT_DEPOSIT_EXPENSE
  {
	  DWORD uUdsn;
	  byte mDateTime[7];
	  DWORD uAmount;//���ѽ��
	  STRUCT_DEPOSIT_EXPENSE()
	  {
		  uUdsn=0;
		  memset(mDateTime,0,sizeof(mDateTime));
		  uAmount=0;
	  }
  } STT_DEPOSIT_EXPENSE;
/*
���庯�� 
DllUseport int WINAPI DepositExpense(STT_DEPOSIT_EXPENSE &sttExpense,
uchar &SerAccnum,TRANRESULT &TranResult,
ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

����˵��
SLE ʹ�ô����UL��ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
info Ϊһ�ṹ�壬����ָʾ�Ĳ�Ʊ����Ϣ
SerAccnum UDSN��һ��ͨ������ˮ���ۼ�ֵ
UDData �������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
ARData ���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
ARDataLen ������أ����ڴ��AR���ݳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int CompensationPaid (ulong &SerialNo,uchar *TranDateTime, COMPENSATIONPAID info,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
BOM��Ҫ���в�Ʊҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
info Ϊһ�ṹ�壬����ָʾ�Ĳ�Ʊ����Ϣ
SerAccnum UDSN��һ��ͨ������ˮ���ۼ�ֵ
UDData �������UD�����ڴ�ŷ��ص�UD���ݣ������UD���أ�Ϊ��
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
ARData ���ڴ�ŷ��ص�AR���ݣ������AR���أ�Ϊ��
ARDataLen ������أ����ڴ��AR���ݳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
		uchar CardType;//��Ƭ����
		ushort RevalueType;//��ֵ���͡���Ʊ������
		uchar PayMethed;	//
		ulong Amount;//
		ulong Count;//
		ulong Period;//���ڿ���Ч���ӳ����ȣ���λ����
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
/*���庯�� 
desc: SLE��Ҫ���г�ֵҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
  ����
  ulong dwUsdn [in] UDSN��һ��ͨ������ˮ��
  TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
  info Ϊһ�ṹ�壬����ָʾ��ֵ����Ϣ
  UDData ���ڴ�ŷ��ص�UD����
  UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
  hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
  ����ֵ 
  �ɹ��򷵻�0�����д����򷵻ط���ֵerrno
 */
DllUseport int WINAPI Revalue (
		ulong dwUsdn/*in,һ��ͨ������ˮ��*/,
		uchar *TranDateTime/*in,����ʱ��*/,
		REVALUEINFO sttPutValInfo/*in,��ֵ��Ϣ*/,
		uchar &bySerAccnum/*out,������ˮ���ۼ�ֵ*/,
		TRANRESULT &sttTranRlt/*out,tpu ִ��״̬*/,
		ushort &sArgUdLen/*out*/,
		uchar *pArgUdBuf/*out*/,
		ushort &sArgArLen/*out*/,
		ARDATA &sttArgAr/*out*/,
		HANDLE hComm);



/*���庯�� 
  desc :���߳�ֵ
  SerialNo UDSN��һ��ͨ������ˮ��
  TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
  info Ϊһ�ṹ�壬����ָʾ��ֵ����Ϣ
  UDData ���ڴ�ŷ��ص�UD����
  UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
  hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
  ����ֵ 
  �ɹ��򷵻�0�����д����򷵻ط���ֵerrno
 */
int WINAPI RevalueOnline(
						 ulong dwUsdn/*in,һ��ͨ������ˮ��*/,
						 uchar *TranDateTime/*in,����ʱ��*/,
						 REVALUEINFO sttPutValInfo/*in,��ֵ��Ϣ*/,
						 uchar &bySerAccnum/*out,������ˮ���ۼ�ֵ*/,
						 TRANRESULT &sttTranRlt/*out,tpu ִ��״̬*/,
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
	uchar mReceiptNumber[10];//�վݺ�
	uchar IsLimitSurrender;//�Ƿ��޶����˿01��Yes��02��NO
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
	memset(mReceiptNumber,0,sizeof(mReceiptNumber));//�վݺ�
	IsLimitSurrender=0;//�Ƿ��޶����˿01��Yes��02��NO
	}
} SURRENDERINFO;

/************************************************************************/
/*
���庯�� 
int CardSurrender (ulong &SerialNo,uchar *TranDateTime, SURRENDERINFO info,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm)
����˵�� 
BOM��Ҫ�����˿�����ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
info Ϊһ�ṹ�壬����ָʾ�˿����ʵ���Ϣ
UDData ���ڴ�ŷ��ص�UD����
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
/************************************************************************/
DllUseport int WINAPI CardSurrender (ulong &SerialNo,uchar *TranDateTime, SURRENDERINFO info,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* ���庯�� 
int ReplaceOldCard (ulong &SerialNo,uchar *TranDateTime, uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
BOM��Ҫ�����滻��ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
UDData ���ڴ�ŷ��ص�UD����
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
                                                                   */
/************************************************************************/
DllUseport int WINAPI ReplaceOldCard (ulong &SerialNo,uchar *TranDateTime, uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* ���庯�� 
int ReplaceNewCard (ulong &SerialNo,uchar *TranDateTime,uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
BOM��Ҫ�����滻��ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
UDData ���ڴ�ŷ��ص�UD����
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
	                                                                    */
/************************************************************************/
DllUseport int WINAPI ReplaceNewCard (ulong &SerialNo,uchar *TranDateTime,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* ���庯�� 
int ActiveCard(uchar *TranDateTime,ushort ProductyCategory /uchar Category/,uchar ProductType,uchar &Statue,HANDLE hComm);
����˵�� 
BOM��Ҫ�����滻��ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
Category����Ƭ���࣬������Ƭ��Ӧ�ô����0x01��һ��ͨS50/S70��Ƭ��0x02��һ��ͨCPU����0x11��һƱͨS50/S70��Ƭ��0x12��һƱͨUL��
ProductType ��Ʊ��Ʒ����
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
                                                                     */
/************************************************************************/
DllUseport int WINAPI ActiveCard(uchar *TranDateTime, uchar Category,ushort ProductyCategory,
								 uchar &SerAccnum,TRANRESULT &TranResult,ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
/************************************************************************/
/* ���庯�� 
int Defercard (uchar *TranDateTime,ushort DeferTime,uchar &Statue,HANDLE hComm);
����˵�� 
��Ҫ������BOM��Ҫ��������ҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
BOM�豸ʹ�õ�������������ֻ���һ��ͨ��Ƭ�����⿨��������ѧ�����������ڲ�����
�����ڵ�ʱ�䳤�����ɲ���Ա�ֹ����룬���ò���UD��
����
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
DeferTime��Ч��˳�ӵĳ��ȣ�����Ϊ��λ
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
		                                                                */
/************************************************************************/
typedef struct STRUCT_DEFERCARD
{
	DWORD uUdsn;//UDSN
	uchar mDataTime[7];//����ʱ��
	DWORD uCommissionCharge;//������
	ushort wDeferTime;//���ڳ���
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
����˵�� 
BOM��Ҫ���й�ʧҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
Category����Ƭ���࣬������Ƭ��Ӧ�ô����0x01��һ��ͨS50/S70��Ƭ��0x02��һ��ͨCPU����0x11��һƱͨS50/S70��Ƭ��0x12��һƱͨUL��
ProductType ��Ʊ��Ʒ����
CardNo��һ��ͨ��Ƭ���кŻ���һƱͨ��SerialNumber
CredentialsType��֤������
CredentialsNo��֤������
UDData ���ڴ�ŷ��ص�UD����
UDDataLen �������UD�����ڴ�ŷ���UD���ݵĳ���
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
                                                                    */
/************************************************************************/
typedef struct _TAG_STRUCT_REPORTING_LOST
{
	ulong uSerialNo;//UDSN��һ��ͨ������ˮ��
	uchar mDateTime[BCD_TIME_LEN];//���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
	uchar byCardType;//��Ƭ����
	ushort wTicketType;//��Ʊ����
	uchar mCardNo[16];//һ��ͨ��Ƭ���кŻ���һƱͨ��SerialNumber������Ϊ16
	uchar CredentialsType;//֤������
	uchar mCredentialsNo[20];//֤������,����Ϊ20
	uchar byCardHolderSex;//�ֿ����Ա�: 01���У�02��Ů
	uchar mCardHolderName[20];//�ֿ����������20���ֽ�
} STT_REPORTING_LOST;
DllUseport int WINAPI ReportingLost (STT_REPORTING_LOST &sttInfo,
									 uchar &SerAccnum,TRANRESULT &TranResult,
									 ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* ���庯�� 
int int WINAPI ReverseTicket (ulong &SerialNo,uchar *TranDateTime, uchar Category,uchar ProductType,uchar *ReasonCode,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);
����˵�� 
BOM��Ҫ���й�ʧҵ����ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
SerialNo UDSN��һ��ͨ������ˮ��
TranDateTime ���׷���������ʱ�䣬BCD�룬��ʽYYMMDDHHmm
Category����Ƭ���࣬������Ƭ��Ӧ�ô����0x01��һ��ͨS50/S70��Ƭ��0x02��һ��ͨCPU����0x11��һƱͨS50/S70��Ƭ��0x12��һƱͨUL��
ProductType ��Ʊ��Ʒ����
ReasonCode������ԭ����룺��дACC��׼�涨�ĵ���ԭ����룬�μ�ACC��׼
UDData ���ڴ�ŷ��ص�UD����
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
                                                                     */
/************************************************************************/
DllUseport int WINAPI ReverseTicket (ulong &SerialNo,uchar *TranDateTime, uchar Category,ushort ProductyCategory,/*uchar ProductType, by chenliye*/uchar *ReasonCode,uchar &SerAccnum,TRANRESULT &TranResult,
	  ushort &UDDatalen,uchar *UDData,ushort &ARDataLen,ARDATA &ARData,HANDLE hComm);

/************************************************************************/
/* ���庯�� 
int CalculateMAC (uchar *KeyVer,uchar *Data,uchar* MAC,HANDLE hComm);
����˵�� 
SLE����Ҫ��������һƱͨMACʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�
����
KeyVer����MAC��Ҫ����Կ�汾��ʾ
Data��Ҫ����MAC��Hash���ݡ�
MAC�����ڴ�ŷ��ص�MAC����
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
                                                                     */
/************************************************************************/
DllUseport int WINAPI CalculateMAC (uchar *KeyVer,uchar *Data,uchar* MAC,HANDLE hComm);


/************************************************************************/
/* ���庯�� 
int SetBusinessTime (uchar ControlCode,ulong *Time,HANDLE hComm);
����˵�� 
SLE�ڽ��յ���λ���͵�24Сʱ��Ӫ���ӳ���Ӫ�Ŀ��������ͨ���ÿ�������ʹTPU����24Сʱ��Ӫ���ӳ���Ӫ��״̬��
����
Time�ӳ���Ӫ��ʱ�䣬��λ�룻
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
                                                                     */
/************************************************************************/
DllUseport int WINAPI SetBusinessTime (uchar ControlCode,ulong Time,uchar &statue,HANDLE hComm);

/************************************************************************/
/* ���庯�� 
int SetBusinessMode(uchar ControlCode,uchar *ModeCode,HANDLE hComm);
����˵�� 
SLE�ڽ��յ���λ���͵���Ӫģʽ��������ɹ��޸�������Ӫģʽ״̬��ͨ���ÿ��������޸�TPU����Ӫģʽ��
����
ControlCode  ���ƴ��룺����24Сʱ��Ӫ���ӳ���Ӫ�Ŀ��ƴ��룬�������£�0x01������ģʽ��0x02�����ģʽ��
ModeCOde   ģʽ���룬����Ϊ2���ֽ�
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int WINAPI sReaderStart(UCHAR ucNumCom,ULONG ulSpeed,CSC_HANDLE* phCsc)
����˵�� 
������д���ⲿ��֤��ͨѶ��
����
ucNumCom�������Ͷ�����ͨѶ�Ķ˿ں�(1 ���豸�����˿ں�) 
ulSpeed��ͨѶ�˿�����(1200, 2400, 9600, 19200, 38400, 57600, 115200) 
phCsc����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

 DllUseport short WINAPI sReaderStart(UCHAR ucNumCom,ULONG ulSpeed,CSC_HANDLE* phCsc);

//-------------------------------------------------------------------------
/*
���庯�� 
short WINAPI sReaderStop (CSC_HANDLE hCsc)
����˵�� 
ֹͣ��д���ⲿ��֤��ͨѶ��
����
hCsc����Ҫ��ֹͣͨ�ŵĶ������ľ������sReaderStart�ṩ��
����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport short WINAPI sReaderStop (CSC_HANDLE hCsc);

//-------------------------------------------------------------------------
/*
���庯�� 
unsigned short sSelectCard(USHORT  usIndex,CSC_HANDLE hCSC)
����˵�� 
ѡ����Ҫ��֤�Ŀ���
����
usindex�������š�ȡֵ��Χ:1~5(1��YPT ISAM��, 2��YKT ISAM��,3��YPT PSAM��, 4��YKT PSAM��, 5��YPT����)
hCSC���������ľ������sReaderStart�ṩ��
����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport unsigned short WINAPI sSelectCard(USHORT usIndex, CSC_HANDLE hCSC);

//-------------------------------------------------------------------------
/*
���庯�� 
unsigned short sISamReadATR( UCHAR * ucSAMATR,SHORT  *Length,CSC_HANDLE hcsc) 
����˵�� 
SAM����λȡ��ATR ֵ����ȡ��������ָ��SAM ��ATR��
����
	ucSAMATR�����ڱ���SAM ATR��Ϣ��ָ��
	Length�����ص�ATR����
	hcsc�������������
����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
// //-------------------------------------------------------------------------
  DllUseport unsigned short WINAPI sISamReadATR( UCHAR * ucSAMATR,SHORT  *Length,CSC_HANDLE hcsc);
//-------------------------------------------------------------------------
/*
���庯�� 
unsigned short sISamAPDU(USHORT usInDataLen, const UCHAR * pucDataIn, USHORT * pusOutDataLen, UCHAR * pucDataOut, CSC_HANDLE hCSC )
����˵�� 
��ISAM ������ APDUָ������������ISO 7816 �����Щ���ֱ�Ӵ���һ��ͨISAM����
����
	usInDataLen���������ݳ���
	pucDataIn��ָ���������ݵ�ָ��
	pusOutDataLen��ָ��������ݳ��ȵ�ָ�룬����ʱ�����ʼ��
	pucDataOut��ָ��������ݵ�ָ�� 
	hCSC�����������

����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int  GetTicketPrice(TICKETINFO sTicketInfo, ulong &TicketPrice, HANDLE hComm)
����˵��:
������Ӧ�ĳ�Ʊ���ԣ�TPU������Ӧ�ĳ�Ʊ�۸�

����:
sTicketInfo��һ���ṹ�壬���복Ʊ��������Ϣ
TicketPrice�������Ʊ�ļ۸�
hComm�����������

����ֵ��
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

DllUseport int WINAPI GetTicketPrice (TICKETINFO sTicketInfo, ulong &TicketPrice, HANDLE hComm);

//-------------------------------------------------------------------------
/*
���庯�� 
int M1_LoadKey(uchar ucSector, uchar *pKey, HANDLE hComm)
����˵��:
װ��ָ����������Կ���Կտ�������������ֻ�ʺ���KEYA��ʽ�Կ�Ƭ����ͨ�õĶ�����ָ��

����:
ucSector�������š�0��15
pKey��װ�����Կ��KEY A ,����Ϊ 6 bytes
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
���庯�� 
int M1_ReadBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm)
����˵��:
��M1��ָ���顣�Կտ�������������ֻ�ʺ���KEYA��ʽ�Կ�Ƭ����ͨ�õ�д����ָ�

����:
sBlockInfo��һ���ṹ�壬������������صĲ�����Ϣ
pData�����صĿ�����,����Ϊ 16 bytes
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int M1_ReadBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm);
//-------------------------------------------------------------------------

/*
���庯�� 
int M1_WriteBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm)
����˵��:
дM1��ָ����

����:
sBlockInfo��һ���ṹ�壬����д�������صĲ�����Ϣ
pData��д��Ŀ�����,����Ϊ 16 bytes
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int M1_WriteBlock(BLOCKINFO sBlockInfo, uchar *pData, HANDLE hComm);
//-------------------------------------------------------------------------
/*
���庯�� 
int GetErrorCode(int &iErrorCode, HANDLE hComm)
����˵��:
��ȡ������ ִ�н��������

����:
iErrorCode��������ִ�н��������
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int GetErrorCode(int &iErrorCode, HANDLE hComm);
//-------------------------------------------------------------------------
/*
���庯�� 
int GetDeviceMode(ushort &DeviceMode, HANDLE hComm)
����˵��:
��ȡ������ ��ǰ����ģʽ

����:
iErrorCode����������ǰ����ģʽ
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int GetDeviceMode(ushort &DeviceMode, HANDLE hComm);
//-------------------------------------------------------------------------


/*
���庯�� 
int Para_GetPara (uchar* ParaType, uchar &bContinue, ushort PacketNumber,ushort PacketLen,uchar * Data, ushort &BackPacketNumber , ushort &BackPacketLen,HANDLE hComm);

����˵��:
����TPU�� CD�����ļ�

����:
ParaType  �����ļ����ͣ�����Ϊ2�ֽ�
bContinue  ��������־����ʾĳ�������ļ��Ƿ��к�����Ҫ�������أ�0x00 ��ʾ�����ݰ��ǽ��������޺�������0x01 ��ʾ�к�������
PacketNumber Ҫ��ȡ�Ĳ����ļ��ķָ������ݰ���š���һ������0x0000��ʼ�����������ۼӡ�
PacketLenҪ��ȡ�����ݰ���Ϣ���ȡ��������ļ�����1024 byteʱ����Ҫ�ְ����͡�ÿ�����ݰ��ĳ������Ϊ1024�������һ�����ݰ�����1024bytesʱ������ʵ�ʳ��ȷ��͡�
Data ������Ϣ�����ص��������ݣ���� 1024 byte��
BackPacketNumber ���ڴ�ŷ���Ӧ����ָʾӦ��ɹ��İ���ţ�����һ��Ϊ0xFFFF��
BackPacketLen ���ص����ݰ��ĳ��ȡ�
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
DllUseport int Para_GetPara(ushort wFileType, uchar &bContinue, ushort PacketNumber,
			  ushort PacketLen,uchar *Data, ushort &BackPacketNumber , ushort &BackPacketLen,HANDLE hComm);
DllUseport int SetLogFileConfig(uchar &Statue,uchar logLevel,  byte *pBitsCmd,UINT uBitsLen,HANDLE hComm);

DllUseport int Para_GetLogFile(ushort wFileType, uchar &bContinue, ushort PacketNumber,
							ushort PacketLen,uchar *Data, ushort &BackPacketNumber , ushort &BackPacketLen,HANDLE hComm);


/*
���庯�� 
int LedCtrl(uchar ucLedCode, uchar ucCtrlCode, HANDLE hComm)
����˵��:
���ƶ����� Led ��

����:
ucLedCode��Led ��ʶ��
ucCtrlCode��Led������
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI LedCtrl(uchar ucLedCode, uchar ucCtrlCode, HANDLE hComm);
//-------------------------------------------------------------------------
/*
���庯�� 
int BuzzerCtrl(uchar ucCtrlCode,HANDLE hComm)
����˵��:
���ƶ����� ������

����:

ucCtrlCode��������������
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI BuzzerCtrl(uchar ucCtrlCode, HANDLE hComm);
//-------------------------------------------------------------------------
/*
���庯�� 
int Poll_CardTest(uchar AntennaMark,uchar &CardType,uchar *CardPHId,HANDLE hComm);
����˵�� 
SLE��Ҫ��TPU����ѭ��Ѱ��״̬ʱ���͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݡ�AGʹ�ø���������֪ͨTPU��ʼѰ����TPU�յ���ָ���ͣ���ظ�Ѱ�������û��Ѱ������TPU״̬�������ڡ�Ѱ��״̬��������Ѱ��,���Ѱ����Ƭ�����л�������Ѱ����״̬��������Ѱ�������
����
	CardType���ڴ�ŷ��صĿ�Ƭ����
	CardPHId ���ڴ�ŷ��صĿ�Ƭ����ID������Ϊ8
hComm����ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
����ֵ 
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------

  DllUseport int WINAPI Poll_CardTest(uchar AntennaMark/*by chen liye*/, uchar &CardType, uchar &AntennaFindMark, uchar *CardPHId,HANDLE hComm);
//---------------------------------------------------------------------------
  /*
���庯�� 
int WINAPI AntennaOn(HANDLE hComm)
����˵��:

����:
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI AntennaOn(HANDLE hComm);
//-------------------------------------------------------------------------
  /*
���庯�� 
int WINAPI AntennaOff(HANDLE hComm)
����˵��:

����:
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
//-------------------------------------------------------------------------
DllUseport int WINAPI AntennaOff(HANDLE hComm);
//-------------------------------------------------------------------------

/*
���庯�� 
int WINAPI AntennaOff(HANDLE hComm)
����˵��:

  ����:
  hComm�����������
  
	����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
 /*���庯�� 
int WINAPI SetOrGetBMACAuthenCtr(AuthenCtr_t &AuthenCtr, HANDLE hComm)
����˵��:

����:
AuthenCtr ���û��ȡ��Ȩ������
hComm�����������

����ֵ:
�ɹ��򷵻�0�����д����򷵻ط���ֵerrno
*/
DllUseport int WINAPI SetOrGetBMACAuthenCtr(AuthenCtr_t &AuthenCtr, HANDLE hComm);


typedef struct tag_BMACDirectConstructionSaleOrIncreaceTransaction
{
	uchar Obligate[33];
}BMAC_DIRECT_CONSTRUCTION_SALE_OR_INCREACE_TRANSACTION;
/***************************************************************************************************************************
�������壺
int WINAPI BMACDirectConstructionSaleOrIncreaceTransaction(BMAC_DIRECT_CONSTRUCTION_SALE_OR_INCREACE_TRANSACTION BMAC_DCSIT, 
															TRANRESULT &TranResult, 
															ushort &UDDatalen, 
															uchar *UDData, 
															ushort &ARDataLen,
															ARDATA &ARData,
															HANDLE hComm);
�������ܣ�
��һ��ͨ�ۿ����ֵ���׹����У�Ǯ����ֵ���������쳣�ҿ�Ƭ���ƿ��޷���������ʱ��SLE�ɷ��͸��������ݣ�TPUִ�в����󷵻�Ӧ�����ݣ�Ӧ�������а���������UD���ݣ�

����˵����
BMAC_DCSIT��һ��ֱͨ�������ۿ����ֵ����Ԥ������
TranResult�����׽�� 
UDDatalen�� UD���ݳ���
UDData�� UD����
ARDataLen��AR���ݳ���
ARData��AR����
hComm�����������

����ֵ���ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
 
 desc��ǩ���ɹ�������TPUһ��ͨ��Ȩ��ȡ���Ȩʱ�䡢У��ʱ�ӣ�ͬʱ���һ��ͨ������ݡ�
 
 ����˵����
 SamId		[IN]	Sam��			UCHAR *	ASCII	12	ISam����
 SysDatetime	[IN]	ϵͳʱ��		UCHAR *	ASCII	14	��ʽ��YYYYMMDDhhmmss
 LimitAmt	[IN]	��Ȩ���		ULONG 				һ��ǩ��ǩ��֮�������ֵ�ܶ��λ���֣���Ǯ������ʱ�۳���ͬ���
 LimitTime	[IN]	��Ȩ����ʱ��	UCHAR *	ASCII	14	��ʽ��YYYYMMDDhhmmss
 hComm�����������
 ����ֵ���ɹ��򷵻�0�����д����򷵻ط���ֵerrno
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
 desc��ǩ��֮ǰ��TPU�ж�ȡһ��ͨ������ݡ�
 ����˵����
 SamId		[IN]	Sam��			UCHAR *	ASCII	12	ISam����
 AuditData	[Out]	�������	UCHAR *			�ṹ���±�
 												  ��ֵ�����ܱ���	4	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 
 												  ��ֵ�����ܽ��	8	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 												  �ۿ��ܱ���		4	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 												  �ۿ���Ѻ��		8	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 												  �ƴ�Ʊ�����ܱ���	4	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 												  �ƴ�Ʊ�����ܽ��	8	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 												  ��Ȩ������		8	ASCII_H	��Ӹ��ֽ����ҵ��ֽ���
 hComm�����������
 ����ֵ���ɹ��򷵻�0�����д����򷵻ط���ֵerrno
 */
 DllUseport ushort WINAPI sYKYReadLimit (UCHAR *pSamId, UCHAR *pAuditData, HANDLE hComm);
//===============================================================================================================

//===============================================================================================================

/* 
   ����::TPU��ʼ����
	 byte *pTime,[in] ����ʱ��,�̶�����7������
   byte byIsCheck,[in] �Ƿ�ʱ���г���
   HANDLE hComm,[in] ���ھ��
   byte byTpuState [out] ����tpu��״̬��
*/
DllUseport int WINAPI SetCheck(byte *pTime,byte byIsCheck,HANDLE hComm,byte &byTpuState);
/*
	����:��ʱ����
	DWORD dwUdsn,[in] UDSN��һ��ͨ������ˮ��
	byte *pTime,[in] ����ʱ��,�̶�����7������
	byte byDo,[in] �Ƿ�ʱ���г��촦��
	HANDLE hComm,[in] ���ھ��
	uchar &bySerAccnum[out],������ˮ���ۼ�ֵ,
	TRANRESULT &sttTranRlt[out],tpu ִ��״̬,
	ushort &sArgUdLen[out],
	uchar *pArgUdBuf[out],
	ushort &sArgArLen[out],
	ARDATA &sttArgAr[out],
	
*/

DllUseport int WINAPI SetTimeOutDeal(DWORD dwUdsn,byte *pTime,byte byDo,HANDLE hComm,
									 uchar &bySerAccnum/*out,������ˮ���ۼ�ֵ*/,
									 TRANRESULT &sttTranRlt/*out,tpu ִ��״̬*/,
									 ushort &sArgUdLen/*out*/,
									 uchar *pArgUdBuf/*out*/,
									 ushort &sArgArLen/*out*/,
									 ARDATA &sttArgAr/*out*/);


/*
desc:UL������д
byte bySamYctType	[in] //һ��ͨSAM�����
byte bySamYptType	[in]//һƱͨSAM�����
bool bWrite			[in] // false Ϊ������true Ϊд��
byte *psCardData	[in/out]//��Ƭ����
DWORD *puCardDataLen [in/out]//��Ƭ���ݳ���
HANDLE hComm [in] ��ʶΨһ�������ľ����������������������ʱ��������ʶָ����������
*/
#define CARDTYPE_YCT_M1 0x01//һ��ͨM1
#define CARDTYPE_YCT_CPU 0x02//һ��ͨCPU
#define CARDTYPE_YPT_M1 0x11//һƱͨM1;
#define CARDTYPE_YPT_UL 0x12//һƱͨUL��
#define CARDTYPE_YPT_CPU 0x13//һƱͨCPU��
//
#define CARDTYPE_CPU_ADF1 0x01 //���貿��Կ  ADF1
#define CARDTYPE_CPU_ADF2 0x02 //�����ͨ��Կ ADF2

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