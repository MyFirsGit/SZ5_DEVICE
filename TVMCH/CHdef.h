#pragma once

#include <vector>
#include "Util.h"
#include "CommonDefine.h"
#include "CHAApi.h"
using namespace std;

#define E_NO_CH_ERROR 0 
#define MAX_SEND_ITEMS_COUNT		10		 // ���Ͷ������Ԫ�ظ���
#define MAX_RECEIVE_ITEMS_COUNT	    10		 // ���ն������Ԫ�ظ���
const int CH_RES_MAX = 1500;			// ֽ�ҽ���ģ��Ļظ�������1500�ֽ�

// ������λ���ݴ�������
typedef enum _CH_BOX_NO {
	Undefine,
	Hopper,//ѭ����
	Preparation, //������
	Recover, //������
} CH_BOX_NO;






// ͨ��-Ӳ����Ϣ
typedef struct _ch_common_response_coininfo
{
	WORD num_OneY_inHopper;        //1ԪHopper�����
	WORD num_HalfY_inHopper;        //5��Hopper�����
	WORD num_OneY_inPreparation;        //1Ԫ���������
	WORD num_HalfY_inPreparation;        //5�Ǳ��������
	WORD num_OneY_inRecover;        //1Ԫ���������
	WORD num_HalfY_inRecover;        //5�ǻ��������
	WORD num_OneY_inTemporary;        //1Ԫ�ݴ������
	WORD num_HalfY_inTemporary;        //5���ݴ������



	_ch_common_response_coininfo(){
		num_OneY_inHopper        = 0;
		num_HalfY_inHopper        = 0;
		num_OneY_inPreparation        = 0;
		num_HalfY_inPreparation        = 0;
		num_OneY_inRecover        = 0;
		num_HalfY_inRecover        = 0;
		num_OneY_inTemporary        = 0;
		num_HalfY_inTemporary        = 0;

	}
	void Serialize(LPBYTE lpByte)
	{
		char* buf = (char*)lpByte;
		*(unsigned short*)buf = num_OneY_inHopper;                buf += sizeof num_OneY_inHopper;
		*(unsigned short*)buf = num_HalfY_inHopper;                buf += sizeof num_HalfY_inHopper;
		*(unsigned short*)buf = num_OneY_inPreparation;                 buf += sizeof num_OneY_inPreparation;   
		*(unsigned short*)buf = num_OneY_inPreparation;                 buf += sizeof num_OneY_inPreparation;   
		*(unsigned short*)buf = num_OneY_inRecover;                buf += sizeof num_OneY_inRecover;
		*(unsigned short*)buf = num_HalfY_inRecover;                buf += sizeof num_HalfY_inRecover;
		*(unsigned short*)buf = num_OneY_inTemporary;                 buf += sizeof num_OneY_inTemporary;   
		*(unsigned short*)buf = num_OneY_inTemporary;                 buf += sizeof num_OneY_inTemporary;   

	}
	inline int Deserialize(const char* lpByte)
	{
		const char* cp = lpByte;  
		num_OneY_inHopper       = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inHopper;
		num_HalfY_inHopper       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inHopper;
		num_OneY_inRecover      = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inRecover;
		num_HalfY_inRecover       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inRecover;
		num_OneY_inPreparation       = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inPreparation;
		num_HalfY_inPreparation       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inPreparation;
		num_OneY_inTemporary       = (cp[0]<<8)|cp[1];           cp += sizeof num_OneY_inTemporary;
		num_HalfY_inTemporary       = (cp[0]<<8)|cp[1];           cp += sizeof num_HalfY_inTemporary;
		return (cp - lpByte);
	}
} CH_COMMON_RSP_COININFO,*LPCH_COMMON_RSP_STATUS;


/*******************************************************************************
// 	����Ӳ����Ϣ-���ؽ���
*******************************************************************************/	
typedef struct _ch_common_response_accept_coin_info
{
	unsigned short escrow1;
	unsigned short escrow2_05;
	unsigned short reject;


	_ch_common_response_accept_coin_info()
	{
		escrow1   =0;    //�ݴ�1-1Ԫ����
		escrow2_05=0;    //�ݴ�2-0.5Ԫ����
		reject    =0;    //���ν����˱�

	}
	inline int Deserialize(const char* lpByte)
	{
		const char* cp = lpByte;

		escrow1   = (cp[0]<<8)|cp[1];               cp  += sizeof escrow1;
		escrow2_05= (cp[0]<<8)|cp[1];               cp  += sizeof escrow2_05;
		reject    = (cp[0]<<8)|cp[1];                cp  += sizeof reject;
		return (cp - lpByte);
	}
} CH_COMMON_RSP_ACCEPT_COIN_INFO;


//*******************************************************************************/
// ���ؽ���-ͨ��������Ӧ����
//*******************************************************************************/
typedef struct _ch_common_response
{
	BYTE ID[2];
	BYTE Lng[2];
	BYTE Res[2];             // 0x00 ������0x80 �쳣��0xFF �����쳣��0x70 ����
	
	BYTE Err_Info[4];         //������Ϣ
	BYTE MechanismStatus[10]; //��е״̬��Ϣ
	WORD OneY_inHopper;       //1Ԫѭ����������Ӳ������
	WORD HalfY_inHopper;	  //5��ѭ����������Ӳ������
	WORD OneY_inPreparation;  //1Ԫ��������Ӳ������
	WORD HalfY_inPreparation; //5�Ǳ�������Ӳ������
	WORD OneY_inRecover;      //1Ԫ��������Ӳ������
	WORD HalfY_inRecover;     //5�ǻ�������Ӳ������
	BYTE Reserve1[2];
	BYTE Reserve2[2];

	// ��е״̬��ϸ��Ϣ
	// ����
	bool bIsCoinModuleStandby;	// Ӳ��ģ���Ƿ�λ��true:��λ��false��δ��λ
	// Hopper A
	bool bIsHopperAFull;		// Hopper �Ƿ���
	bool bIsHopperAEmpty;		// Hopper �Ƿ��
	// Hopper B
	bool bIsHopperBFull;		// Hopper �Ƿ���
	bool bIsHopperBEmpty;		// Hopper �Ƿ��
	// ����Ǯ��A
	bool bIsChangeBoxAInstall;	// Change BoxA�Ƿ�װ
	bool bIsChangeBoxAFull;		// Change BoxA�Ƿ���
	bool bIsChangeBoxAEmpty;	// Change BoxA�Ƿ��
	// ����Ǯ��B
	bool bIsChangeBoxBInstall;	// Change BoxB�Ƿ�װ
	bool bIsChangeBoxBFull;		// Change BoxB�Ƿ���
	bool bIsChangeBoxBEmpty;	// Change BoxB�Ƿ��

	inline int Deserialize(const BYTE* data)
	{
		memcpy(ID,data,2);
		memcpy(Lng,data + 2,2);
		memcpy(Res,data + 4,2);
		memcpy(Err_Info,data + 6,4);
		memcpy(MechanismStatus,data + 10,10);
		OneY_inHopper = (data[20]<<8)|data[21];
		HalfY_inHopper = (data[22]<<8)|data[23];
		OneY_inPreparation = (data[24]<<8)|data[25];
		HalfY_inPreparation = (data[26]<<8)|data[27];
		OneY_inRecover = (data[28]<<8)|data[29];
		HalfY_inRecover = (data[30]<<8)|data[31];
		memcpy(Reserve1,data+32,2);
		memcpy(Reserve2,data+34,2);

		bIsCoinModuleStandby = !(MechanismStatus[0] & BYTE_IN_CH_POS_03);
		bIsHopperAFull		 = (MechanismStatus[2] & BYTE_IN_CH_POS_04);
		bIsHopperAEmpty		 = (MechanismStatus[2] & BYTE_IN_CH_POS_00);

		bIsHopperBFull		 = (MechanismStatus[3] & BYTE_IN_CH_POS_04);
		bIsHopperBEmpty		 = (MechanismStatus[3] & BYTE_IN_CH_POS_00);

		bIsChangeBoxAInstall = !(MechanismStatus[4] & BYTE_IN_CH_POS_05);
		bIsChangeBoxAFull	 = (MechanismStatus[4] & BYTE_IN_CH_POS_04);
		bIsChangeBoxAEmpty	 = (MechanismStatus[4] & BYTE_IN_CH_POS_00);

		bIsChangeBoxBInstall = !(MechanismStatus[5] & BYTE_IN_CH_POS_05);
		bIsChangeBoxBFull	 = (MechanismStatus[5] & BYTE_IN_CH_POS_04);
		bIsChangeBoxBEmpty	 = (MechanismStatus[5] & BYTE_IN_CH_POS_00);

		return 36;
	}

} CH_COMMON_RSP,*LPCH_COMMON_RSP;

//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ�������״̬��ȡ���������Ӧ
//*******************************************************************************/
typedef struct _ch_spec1_response
{
	CH_COMMON_RSP Common;    //��������
	BYTE ActionStatus[2];    //����״̬
	//���½�����Ϣ
	WORD OneY_inTemporary;   //�ݴ�����1ԪӲ�ҵĸ���
	WORD HalfY_inTemporary;   //�ݴ�����5��Ӳ�ҵĸ���
	WORD num_Rejected;		  //�ܾ�����
	WORD num_Overrun;		  //��������
	bool bBusyRecycle;		  //�Զ�����æµ��
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		memcpy(ActionStatus,data+offset,2);
		bBusyRecycle = (data[offset] & 0x80)&&(data[offset+1]&0x02);
		OneY_inTemporary = (data[offset + 2]<<4)|data[offset + 2 + 1];
		HalfY_inTemporary = (data[offset + 2 + 2]<<4)|data[offset + 2 + 3];
		num_Rejected = (data[offset + 2 + 4]<<4)|data[offset + 2 + 5];
		num_Overrun = (data[offset + 2 + 6]<<4)|data[offset + 2 + 7];
		return offset+2+8;
	}

} CH_SPEC1_RSP,*LPCH_SPEC1_RSP;

//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ���������ֹͶ�ҡ��������Ӧ
//*******************************************************************************/
typedef struct _ch_spec2_response
{
	CH_COMMON_RSP Common;			//��������
	//BYTE CoinInserted_Info[8];	//������Ϣ
	WORD wOneYuanInCome;			// һԪ��������
	WORD wHalfYuanInCome;			// ��ǽ�������
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		//memcpy(CoinInserted_Info,data+offset,8);
		wOneYuanInCome = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;
		wHalfYuanInCome = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;
		return offset+4;
	}

} CH_SPEC2_RSP,*LPCH_SPEC2_RSP;

//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ������������趨���������Ӧ
//*******************************************************************************/
typedef struct _ch_spec3_response
{
	CH_COMMON_RSP Common;    //��������
	BYTE SeriesNO[2];   //���б��
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		memcpy(SeriesNO,data+offset,2);

		return offset+2;
	}

} CH_SPEC3_RSP,*LPCH_SPEC3_RSP;


//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ����������Ҽ������������Ӧ
//*******************************************************************************/
typedef struct _ch_spec4_response
{
	CH_COMMON_RSP Common;    //��������
	BYTE CoinComeout_Info[8];   //������Ϣ
	WORD wOneYuanCount;			// ����һԪ����
	WORD wHalfYuanCount;		// �����������
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		memcpy(CoinComeout_Info,data+offset,8);

		wOneYuanCount = MAKEWORD(CoinComeout_Info[1],CoinComeout_Info[0]);
		wHalfYuanCount = MAKEWORD(CoinComeout_Info[3],CoinComeout_Info[2]);
		return offset+8;
	}

} CH_SPEC4_RSP,*LPCH_SPEC4_RSP;

//*******************************************************************************/
// ���ؽ���-������Ӧ���ݣ����������ա��������Ӧ
//*******************************************************************************/
typedef struct _ch_spec5_response
{
	CH_COMMON_RSP Common;    //��������
	// ʵ�ʻ�������
	WORD wOneHopperCount;		// ���(HOPPER B)
	WORD wHalfHopperCount;		// �ұ�(HOPPER A)
	WORD wOneChangeBoxCount;	// �ұ�(������ A)
	WORD wHalfChangeBoxCount;	// ���(������ B)

//	BYTE num_inRecover[8];   //���տ���ո���
	inline int Deserialize(const BYTE* data)
	{
		int offset = Common.Deserialize(data);
		//memcpy(num_inRecover,data+offset,8);
		wOneHopperCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		wHalfHopperCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		wOneChangeBoxCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		wHalfChangeBoxCount = MAKEWORD(*(data + offset + 1),*(data + offset));
		offset += 2;

		return offset;
	}

} CH_SPEC5_RSP,*LPCH_SPEC5_RSP;




//zhengxianle add for now
//*******************************************************************************/
//����ͷ
//*******************************************************************************/
typedef struct _ch_common_response_head {
public:
	enum {
		RESP_INITIAL              =0x81,//1.��ʼ��           
		RESP_STATUS_READ          =0x82,//2.״̬��ȡ
		RESP_ENABLE               =0x83,//3.��ʼ����Ӳ��                        
		RESP_DISABLE              =0x84,//4.��������Ӳ��
		RESP_GET_INSERT_INFO      =0x85,//5.��ȡ���ս��
		RESP_ACCEPT               =0x86,//6.����Ӳ��
		RESP_RELEASE              =0x87,//7.����
		RESP_DATAWRITE            =0x88,//8.���ý��               
		RESP_SAVE                 =0x89,//9.���   	       
		RESP_VERSION_SENSE        =0x95,//10.���汾��
		RESP_HOPPER_START         =0x96,//11.��ʼ�ӱ�                  
		RESP_HOPPER_END           =0x97,//12.�����ӱ�    
		RESP_RFID_EN_READ         =0xE1,//13.��ȡǮ��SN �ţ�RFID��
		RESP_RFID_EN_SET          =0xE2,//13.��ȡǮ��SN �ţ�RFID��
		RESP_RFID_SET             =0xE3,//14.дiButton����                        
		RESP_RFID_READ            =0xE4,//15.��iButton����          
		RESP_COMMON               =0x99, 
		CNTL_SAFE_LOCK            =0x9A,//16.�ӱҰ�ȫ��
	};
	unsigned char   resp;              // 1byte(Bynary)
	unsigned char   rslt;              // 1byte(Bynary)
	unsigned char   mtc;               // 1byte(Bynary)
	_ch_common_response_head()
	{
		resp = 0x00; 
		rslt = 0x00; 
		mtc  = 0x00;

	}

	inline int Deserialize(const char* lpByte)
	{
		/*const char* cp = lpByte;
		resp   = *(unsigned char*)cp; cp += sizeof resp;
		rslt   = *(unsigned char*)cp; cp += sizeof rslt;
		mtc    = *(unsigned char*)cp; cp += sizeof mtc;
		return (cp - lpByte); */
	}

}CH_COMMON_RSP_HEAD;
//*******************************************************************************/
//���ؽ���-��ȡ���ܽ��������Ӧ����-4.8.5��ȡ���ܽ��
//*******************************************************************************/
typedef struct _ch_response_insert_info
{

	CH_COMMON_RSP_HEAD                       head;
	CH_COMMON_RSP_ACCEPT_COIN_INFO    insert_info;
	inline int Deserialize(const char* data)
	{

		/*const char* bufp = data;
		int sz = head.Deserialize(bufp);         bufp += sz;
		sz = insert_info.Deserialize(bufp);  bufp += sz;

		return int(bufp-data);*/
		return 0;
	}
} CH_RSP_ACCEPT_INSERT_INFO;
