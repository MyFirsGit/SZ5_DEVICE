// TIM.h : TIM DLL ����ͷ�ļ�
//

#pragma once
#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "TIMApi.h"
#include "SerialPort.h"
#include "AFCLogApi.h"

#define TIMTRS_RES_OK 0L          //�ɹ�
#define TIMTRS_RES_FAILED 1L     //ʧ��
#define MAXRETRYCNT 3            //������Դ���
#define MAX_BUFFER_SIZE 100     //��������ʱ���BUFFER
#define DEFAULTINTERVAL 5000   //Ĭ�ϳ�ʱ5��
#define TIM_HEAD_LEN 2        //��ͷ����
#define TIM_HEAD_AND_END_LEN 5 //��ͷβ���ȣ�����BCC

//TIMģ��Ӧ��״̬��
#define TIMTRS_CMD_SUCCESS 's'
#define TIMTRS_CMD_FAIL 'e'
#define TIMTRS_CMD_WARNING 'w'


//TIMģ��������
typedef enum _tim_cmd_detail
{
	TIM_CMD_INIT = 0x81,				//��ʼ��
	TIM_CMD_GETSTATUS = 0x82,			//��ȡ״̬
	TIM_CMD_CLEARPASSAGE = 0x83,		//����ͨ��
	TIM_CMD_OUTCARD = 0x84,			//����
	TIM_CMD_SENTCARD = 0x85,		//�Ϳ�
	TIM_CMD_RETRACTCARD = 0x86,		//���տ�
	TIM_CMD_RESET = 0x87,				//��λ
	TIM_CMD_GETVERSION = 0x88,        //��ȡ�汾
	TIM_CMD_GETSN = 0x89,				//��ȡ���к�
	TIM_CMD_SETTICKETBOXCOUNT = 0x97, //����Ʊ���Ʊ������
	TIM_CMD_REPLACEBOX = 0x98,		//����Ʊ��
	TIM_CMD_TEST = 0xb1,				//����
	TIM_CMD_WRITERFID = 0xE3,		//�� RFID дһ�� Block ���� 
	TIM_CMD_READRFID = 0xE4,		//�� RFID ��һ�� Block ����
	TIM_CMD_WRITESECTTOR = 0xE5,	//�� RFID дһ�� Sector ����
	TIM_CMD_READSECTTOR = 0xE6,		//�� RFID дһ�� Sector ����
	TIM_CMD_READPHYSIClNUMBER = 0xE7, //��ȡ RFID ���������� 
	TIM_CMD_GETAUDITDATA = 0xF1,      //������ۼ�ֵ
}TIM_CMD_DETAIL;

typedef struct _cmd_status
{
	int Phase;
	byte CurrentByte;
	byte BeforeByte;
	int RetryCnt;
	byte WriteBuff[1024];
	byte ReadBuff[1024];
	int WriteLength;
	int ReadLength;
	bool ComIsOpen;//�����Ƿ��
	bool ComIsNeedRecv;//�Ƿ���Ҫ�Ӵ��ڶ�ȡ����
	bool ComIsIncludeReturnBytes;//�Ƿ���Ҫ��������
	bool DwBaseTimeIsReset;//��ʱ��ʱ�Ƿ���Ҫ����
	bool ComIsTimeOut;//�Ƿ�ʱ
}CMDSTATUS;

enum _control_table
{
	STX = 0x02,//������ʼ��
	ETX = 0x03,//���������
	EOT = 0x04,//ͨѶ��ֹ��
	ENQ = 0x05,//����ִ��ȷ��
	ACK = 0x06,//����ȷ���ַ�
	DLE = 0x10,//���Ʒ� 
	NAK = 0x15//��������ַ�
};

//Ʊ������
typedef enum _tim_box_type
{
	TIM_BOX_A = 0x01,			// AƱ��
	TIM_BOX_B = 0x02,			// BƱ��
	TIM_BOX_C = 0x03,			// CƱ��|��Ʊ��
}TIM_BOX_TYPE;

//�ֻ�Ʊ�䷽ʽ
typedef enum _tim_box_model
{
	TIM_BOX_REMOVE = 0x61,      //�Ƴ�
	TIM_BOX_INSTALL = 0x62      //��װ
}TIM_BOX_MODEL;

bool TIM_IsTimeUp(IN DWORD dwTimer, IN DWORD dwBaseTime);
void TIM_GetWritePackets(DWORD nDataLength, BYTE* pDataBuffer, BYTE* pWriteBuff, DWORD& nWriteLength);
byte TIM_BccCalc(BYTE* pData, WORD dwLength);
bool TIM_WriteBuffToCom(BYTE* writeBuffer, int len);
static unsigned int CALLBACK TIM_OnRecv(void* LPParam);
int TIM_SentOneCommand(DWORD nDataLength, BYTE* pDataBuffer, BYTE* pReadBuff, DWORD& nReadLength);
void TIM_GetByteBits(BYTE mByte, BYTE* pByte, int len);
int TIM_Escape(BYTE* data, const int& len);
int TIM_Reverse_Escape(BYTE* data, const int& len);
void TIM_SetDevReturn(int nResult, tTIMDevReturn* pDevStatus, BYTE* readBuff);
void TIM_SetReturns(int nResult, tTIMModuleStatus* pModuleStatus, tTIMDevReturn* pDevStatus, BYTE* readBuff, int readLength);
void TIM_SetStatus(int nResult, tTIMModuleStatus* pModuleStatus, tTIMDevReturn* pDevStatus, BYTE* readBuff);