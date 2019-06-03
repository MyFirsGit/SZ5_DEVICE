#include "stdafx.h"
#include "ComConnect.h"
#include "TOKENUpdateLib.h"
#include <stdio.h>

ComConnect::ComConnect()
{
}
ComConnect::~ComConnect()
{
}

//****************************************************************************
// 打开连接处理
// function  : open
// input:     const char* Para 打开连接所必须的参数
// output:    int ret          执行结果 0：正常；非0：错误
//
// note:    打开指定的连接
//****************************************************************************
int ComConnect::open(TOKEN_CONNECT_PARAM Para,HANDLE& hCom)
{
	WCHAR buff[20];
	memset(buff,0,sizeof buff);
    _stprintf(buff, _TEXT("\\\\.\\COM%d"), Para.ComPort);
	hCom = CreateFile(buff,         //COM1口
		GENERIC_READ|GENERIC_WRITE, //允许读和写
		0,                          //独占方式
		NULL,                       //引用安全性属性结构
		OPEN_EXISTING,              //打开而不是创建
		0,                          //同步方式
		NULL);
	if(INVALID_HANDLE_VALUE == hCom){
		return TOKENTRS_ERR_HANDLE;
	}  

	SetupComm(hCom,3000,3000);                 //输入缓冲区和输出缓冲区的大小都是3000

	DCB dcb;
	if (FALSE == ::GetCommState(hCom, &dcb)){
		::CloseHandle(hCom);
		return TOKENTRS_ERR_HANDLE;
	}

	COMMTIMEOUTS TimeOuts;
	GetCommState(hCom,&dcb);

	// 设置DCB参数
	dcb.BaudRate=Para.Speed;               // 波特率
	dcb.ByteSize=8;                        // 每个字节有8位
	dcb.fBinary = TRUE;						// 二进制模式
	dcb.fParity = FALSE;					// 允许奇偶校验
	dcb.StopBits = ONESTOPBIT;				// 停止位(1)
	dcb.Parity = NOPARITY;				    // 奇偶校验方法
	dcb.fAbortOnError = TRUE;               // 有错误发生时中止读写操作
	dcb.fOutxCtsFlow = FALSE;				// 指定CTS是否用于检测发送控制。当为TRUE时CTS为OFF，发送将被挂起。（发送清除）
	dcb.fRtsControl = RTS_CONTROL_DISABLE;  // RTS置为OFF 
	dcb.fDtrControl = DTR_CONTROL_DISABLE;  // DTR置为OFF 
	dcb.fOutxDsrFlow = FALSE;				// 指定DSR是否用于检测发送控制。当为TRUE时DSR为OFF，发送将被挂起。
	dcb.fDsrSensitivity = FALSE;			// 当该值为TRUE时DSR为OFF时接收的字节被忽略 

	// 设置串口状态
	SetCommState(hCom,&dcb);

	memset(&TimeOuts, 0, sizeof(TimeOuts));     //设定读超时


	TimeOuts.ReadTotalTimeoutMultiplier = 3;    //1000;
	TimeOuts.ReadTotalTimeoutConstant = 0;      //timeout for read operation
	TimeOuts.ReadIntervalTimeout = 20;          //MAXDWORD;

	TimeOuts.WriteTotalTimeoutConstant = 0;     //设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 3;   //100;

	SetCommTimeouts(hCom,&TimeOuts);            //设置超时

	::PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );//清空缓冲区

	return TOKENTRS_RES_OK;
}

//****************************************************************************
// 关闭连接处理
// function  : close
// intput:    int i           未使用
// output:    int ret         执行结果 0：正常；非0：错误
//
// note:    关闭连接
//****************************************************************************
int ComConnect::close(HANDLE& hCom)
{
	int ret(1);
	if(!hCom) return ret;
	if(CloseHandle(hCom)){
		ret = 0;
	}
	return ret;
}

//****************************************************************************
// 读取数据处理
// function  : read
// input:     int max_sz      最大长度
//            char* buff      缓冲区
// output:    int ret         读取到的数据长度 0：错误；非0：读到的数据长度
//
//****************************************************************************
int ComConnect::read(int max_sz, LPBYTE buff,HANDLE& hCom)
{
	DWORD wCount = 0;      //读取的字节数
	BOOL bReadStat(false);
	bReadStat = ::ReadFile(hCom, buff, max_sz, &wCount, NULL);	
	return wCount;
}

//****************************************************************************
// 写入数据处理
// function  : write
// input:     int sz          数据长度
//            char* buff      缓冲区
// output:    int ret         写入的数据长度 0：错误；非0：写入的数据长度
//
//****************************************************************************
int ComConnect::write(int sz, LPBYTE buff,HANDLE& hCom)
{
	DWORD dwBytesWrite = 0;
	// 发送数据	
	WriteFile(hCom,buff,sz,&dwBytesWrite,NULL);  
	return dwBytesWrite;
}

