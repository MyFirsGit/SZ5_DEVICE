// TIM.h : TIM DLL 的主头文件
//

#pragma once
#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "TIMApi.h"
#include "SerialPort.h"
#include "AFCLogApi.h"

#define TIMTRS_RES_OK 0L          //成功
#define TIMTRS_RES_FAILED 1L     //失败
#define MAXRETRYCNT 3            //最大重试次数
#define MAX_BUFFER_SIZE 100     //创建数组时最大BUFFER
#define DEFAULTINTERVAL 5000   //默认超时5秒
#define TIM_HEAD_LEN 2        //包头长度
#define TIM_HEAD_AND_END_LEN 5 //包头尾长度，包括BCC

//TIM模块应答状态码
#define TIMTRS_CMD_SUCCESS 's'
#define TIMTRS_CMD_FAIL 'e'
#define TIMTRS_CMD_WARNING 'w'


//TIM模块命令码
typedef enum _tim_cmd_detail
{
	TIM_CMD_INIT = 0x81,				//初始化
	TIM_CMD_GETSTATUS = 0x82,			//读取状态
	TIM_CMD_CLEARPASSAGE = 0x83,		//清理通道
	TIM_CMD_OUTCARD = 0x84,			//出卡
	TIM_CMD_SENTCARD = 0x85,		//送卡
	TIM_CMD_RETRACTCARD = 0x86,		//回收卡
	TIM_CMD_RESET = 0x87,				//复位
	TIM_CMD_GETVERSION = 0x88,        //获取版本
	TIM_CMD_GETSN = 0x89,				//获取序列号
	TIM_CMD_SETTICKETBOXCOUNT = 0x97, //设置票箱的票卡数量
	TIM_CMD_REPLACEBOX = 0x98,		//更换票箱
	TIM_CMD_TEST = 0xb1,				//测试
	TIM_CMD_WRITERFID = 0xE3,		//向 RFID 写一个 Block 数据 
	TIM_CMD_READRFID = 0xE4,		//从 RFID 读一个 Block 数据
	TIM_CMD_WRITESECTTOR = 0xE5,	//向 RFID 写一个 Sector 数据
	TIM_CMD_READSECTTOR = 0xE6,		//向 RFID 写一个 Sector 数据
	TIM_CMD_READPHYSIClNUMBER = 0xE7, //读取 RFID 卡的物理编号 
	TIM_CMD_GETAUDITDATA = 0xF1,      //读审计累计值
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
	bool ComIsOpen;//串口是否打开
	bool ComIsNeedRecv;//是否需要从串口读取数据
	bool ComIsIncludeReturnBytes;//是否需要返回数据
	bool DwBaseTimeIsReset;//超时基时是否需要重置
	bool ComIsTimeOut;//是否超时
}CMDSTATUS;

enum _control_table
{
	STX = 0x02,//命令起始符
	ETX = 0x03,//命令结束符
	EOT = 0x04,//通讯中止符
	ENQ = 0x05,//命令执行确认
	ACK = 0x06,//命令确认字符
	DLE = 0x10,//控制符 
	NAK = 0x15//命令否认字符
};

//票箱类型
typedef enum _tim_box_type
{
	TIM_BOX_A = 0x01,			// A票箱
	TIM_BOX_B = 0x02,			// B票箱
	TIM_BOX_C = 0x03,			// C票箱|废票箱
}TIM_BOX_TYPE;

//现换票箱方式
typedef enum _tim_box_model
{
	TIM_BOX_REMOVE = 0x61,      //移除
	TIM_BOX_INSTALL = 0x62      //安装
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