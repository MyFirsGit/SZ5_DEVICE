#ifndef __TOKENDEF_H_2011_06_02__
#define __TOKENDEF_H_2011_06_02__

#include "CommonDefine.h"
#include "TOKENLib.h"

#ifdef TOKEN_EXPORTS
#define TOKEN_API __declspec(dllexport)
#else
#define TOKEN_API __declspec(dllimport)
#endif



typedef enum _th_box_no_{
	TOKEN_BOX_ALL	= 0x00,
	TOKEN_BOX_A		= 0x01,
	TOKEN_BOX_B		= 0x02,
	TOKEN_BOX_C		= 0x03,
}TOKEN_BOX_NO;

typedef enum _th_box_rfid_no_{
	TOKEN_BOX_RFID_A	= 0x04,
	TOKEN_BOX_RFID_B	= 0x03,
	TOKEN_BOX_RFID_C	= 0x06,
}TOKEN_BOX_RFID_NO;

///** 
//@brief      ����������
//*/
//typedef enum TH_CMD_SENSOR
//{
//	SENSOR01_ID = 61000,	// 61000 (0:����;1:����) Ʊ�����ڼ�⴫����
//	SENSOR02_ID = 61001,    // 61001 (0:����;1:����) ��д����⴫����
//	SENSOR07_ID = 61006,    // 61006 (0:��λ;1:�Ƴ�) Ʊ�� A ��λ��⴫����
//	SENSOR08_ID = 61007,    // 61007 (0:��;1:�ر�) Ʊ�� A ���Ǽ�⴫����������Ʊ��δʹ�ã�
//	SENSOR09_ID = 61008,    // 61008 (0:����;1:����) Ʊ�� A ���մ�����   
//	SENSOR10_ID = 61009,    // 61009 (0:����;1:�ѿ�) Ʊ�� A �մ�����
//	SENSOR13_ID = 61012,    // 61012 (0:��λ;1:�Ƴ�) Ʊ�� B ��λ��⴫����
//	SENSOR14_ID = 61013,    // 61013 (0:��;1:�ر�) Ʊ�� B ���Ǽ�⴫����������Ʊ��δʹ�ã� 
//	SENSOR15_ID = 61014,    // 61014 (0:����;1:����) Ʊ�� B ���մ�����
//	SENSOR16_ID = 61015,    // 61015 (0:����;1:�ѿ�) Ʊ�� B �մ�����
//	SENSOR19_ID = 61018,    // 61018 (0:��λ;1:�Ƴ�) ��Ʊ�䵽λ��⴫����
//	SENSOR20_ID = 61019,    // 61019 (0:��;1:�ر�) ��Ʊ�䶥�Ǽ�⴫����������Ʊ��δʹ�ã�
//	SENSOR21_ID = 61020,    // 61020 (0:����;1:����) ��Ʊ������⴫����
//};


/** 
@brief        ͨѶ���������ṹ
@param     (i)UINT uiCommPort    �˿ں�
@param     (i)UINT uiBaudRate    ������
*/
typedef struct _tem_token_cmd_comm_open
{
	UINT uiCommPort;    // �˿ں�
	UINT uiBaudRate;    // ������

	_tem_token_cmd_comm_open(){
		uiCommPort = 0;
		uiBaudRate = 0;
	}
} TOKEN_CMD_COMM_OPEN, *LPTOKEN_CMD_COMM_OPEN;

/** 
@brief      ģ���ʼ�������ṹ-����
*/
typedef enum TOKEN_CMD_INIT
{
	CMD_INIT_CLEAN			= 1,    // ͨ����Ʊ
	CMD_INIT_NOT_CLEAN		= 2,    // ͨ������Ʊ
};

/** 
@brief      ��ȡ�豸״̬�����ṹ
*/
typedef struct _token_rsp_get_dev_status
{
	// ����ģ���״̬��Ϣ
	bool bBoxANearEmpty;        // Ʊ��A����
	bool bBoxAExist;			// Ʊ��A����
	bool bBoxBNearEmpty;        // Ʊ��B����
	bool bBoxBExist;			// Ʊ��B����
	bool bBoxAEmpty;			// Ʊ��A��
	bool bBoxBEmpty;			// Ʊ��B��
	bool bCardInReadArea;       // �п��ڶ�д��
	// --------------
	bool bHopperAException;     // HopperA�Ƿ����
	bool bHopperBException;     // HopperB�Ƿ����
	bool bHasCardAtTemporaryA;	// A�ݴ����Ƿ���Ʊ�����ݴ���ģ��ר�ã�
	bool bHasCardAtTemporaryB;	// B�ݴ����Ƿ���Ʊ�����ݴ���ģ��ר�ã�

	_token_rsp_get_dev_status()
	{
		bBoxANearEmpty			= false;
		bBoxAExist				= false;
		bBoxBNearEmpty			= false;
		bBoxBExist				= false;
		bBoxAEmpty				= false;
		bBoxBEmpty				= false;
		bCardInReadArea			= false;
		bHopperAException		= false;
		bHopperBException		= false;
		bHasCardAtTemporaryA	= false;
		bHasCardAtTemporaryB	= false;
	}
} TOKEN_RSP_GET_DEV_STATUS, *LPTOKEN_RSP_GET_DEV_STATUS;;


/** 
@brief      ��ȡ�豸�����汾��Ϣ
*/
typedef struct _token_rsp_get_version
{
	// ����ģ���״̬��Ϣ
	unsigned char ucModNum[8];			// ģ���
	unsigned char ucFwVer[7];

	_token_rsp_get_version(){
		memset(ucModNum, 0, sizeof(ucModNum));
		memset(ucFwVer, 0, sizeof(ucFwVer));
	}
} TOKEN_RSP_GET_VERSION, *LPTOKEN_RSP_GET_VERSION;


typedef struct _token_rfid_data
{
	unsigned char uBoxNo[8+1];
	_token_rfid_data(){
		memset(&uBoxNo, 0, sizeof(uBoxNo));
	}
} TOKEN_RFID_DATA, *LPTOKEN_RFID_DATA;

// ����RFID
typedef struct _tem_token_read_write_rfid
{
	unsigned char ucRfidNo;//0x04 = A �� 0x03 = B �� 0x05 = ��Ʊ�� 0x06 = ��Ʊ��
	TOKEN_RFID_DATA rfidData;

	_tem_token_read_write_rfid(){
		ucRfidNo = 0;
		memset(&rfidData, 0, sizeof(rfidData));
	}
} TOKEN_READ_WRITE_RFID, *LPTOKEN_READ_WRITE_RFID;

#endif
