#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <crtdbg.h>
#include <stddef.h>
#include <malloc.h>
#include "THUpdateLib.h"   

// TH命令代码定义
typedef enum {
	CMD_STATR_DOWNLOAD		= 0x31,  // IAP 下载开始
	CMD_STATR_DOWNLOAD_EX	= 0xF0,  // 备用IAP 下载开始
	CMD_DOWNLOAD_COUNT		= 0xE0,  // 下发固件总数据量
	CMD_DOWNLOAD_FILE		= 0xE1,  // 固件代码下发
	CMD_DOWNLOAD_END		= 0xE2,  // 固件下发完成
	CMD_SWITCH_TO_NORMAL	= 0xE9   // 切换到正常程序
} TH_CMD;

// TH返回命令代码定义
typedef enum {
	RSP_START_DOWNLOAD_NORMAL	=0x41,	//开始下载命令(备用命令)确认应答
	RSP_START_DOWNLOAD_MSG		=0x42,	//开始下载命令(备用命令)报文错误

	RSP_DOWNLOAD_NORMAL			=0xF0,	// 下发固件确认应答
	RSP_DOWNLOAD_MSG			=0xF1,	// 下发固件报文错误
	RSP_DOWNLOAD_EXT			=0xF2,	// 下发固件执行错误

	RSP_SWITCH_TO_NORMAL_NORMAL = 0xF9,	// 切换到正常程序确认应答
	RSP_SWITCH_TO_NORMAL_MSG	= 0xF1,	// 切换到正常程序报文错误
	RSP_SWITCH_TO_NORMAL_EXT	= 0xF2,	// 切换到正常程序执行错误
} TH_RSP_CODE;


typedef union _lc {
	_lc() : val(0) {}
	long val;
	char cs[4];
} lchar;

//////////////////////////////////////////////////////////////////////////
/**
@brief      BCC计算函数

@param      (i)BYTE* pData     参与计算书数据指针
@param      (i)WORD dwLength   参与计算书数据长度

@retval     none

@exception  none
*/
//////////////////////////////////////////////////////////////////////////
inline BYTE BccCalc(BYTE* pData, WORD dwLength)
{
	BYTE Bcc = 0;
	while (dwLength-- != 0){
		Bcc = Bcc ^ *pData++;
	}
	return Bcc;
}
//////////////////////////////////////////////////////////////////////////
/**
@brief      转换4字节类型数据的存放顺序(请勿使用)

@param      (i)long l 需要转换的数据

@retval     long      转换后的数据

@exception  无
*/
//////////////////////////////////////////////////////////////////////////
inline long lrev(long l)
{
	lchar s; s.val = l;
	lchar d;
	char* sp=s.cs;
	char* dp=&d.cs[3];
	*dp-- = *sp++;
	*dp-- = *sp++;
	*dp-- = *sp++;
	*dp = *sp;
	return d.val;
}