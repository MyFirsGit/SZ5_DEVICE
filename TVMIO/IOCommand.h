//#pragma once
//#include "IOExp.h"
//#include "Command.h"
//#include "IOLib.h"
//#include "IOdef.h"
//#include "IOException.h"
//
///**
// @brief  IO命令父类
//*/
//class IO_API CIOCommand : public CCommand
//{
//public:
//	CIOCommand(void);   
//    ~CIOCommand(void);
//public:
//	IOTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer = 5000);
//	BOOL IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime);
//};
//
///**
// @brief  打开IO
//*/
//class IO_API CIOConnect : public CIOCommand
//{
//public:
//	CIOConnect(IOTRS_CONNECT_PARAM);   
//    ~CIOConnect(void);
//public:
//	long ExecuteCommand(void);   // 执行命令
//private:
//	IOTRS_CONNECT_PARAM m_connectParam;// 连接参数
//	long ConnectFunction();           // 打开串口
//};
//
///**
// @brief  初始化IO
//*/
//class IO_API CIOInitialize : public CIOCommand
//{
//public:
//	CIOInitialize();   
//    ~CIOInitialize(void);
//public:
//	long ExecuteCommand(void);		// 执行命令
//private:
//	long InitializeFunction();				// 打开串口
//};
//
///**
// @brief  设置单路输出状态（亮或灭）
//*/
//class IO_API CIOSetOneSlotOutput : public CIOCommand
//{
//public:
//	CIOSetOneSlotOutput(IO_OUTPUT_PORT_NO OutputNo,IO_OUTPUT_VALUE OutputValue);
//	~CIOSetOneSlotOutput(void);
//public:
//	long ExecuteCommand(void);				// 执行命令
//private:
//	IO_OUTPUT_PORT_NO m_OutputNo;			// 输出端口号
//	IO_OUTPUT_VALUE		m_OutputValue;		// 输出值
//	long SetOutputFunction();							// 设置处理
//};
//
///**
//@brief 设置单路输出状态（闪烁）
//*/
//class IO_API CIOSetOneOutputFlash : public CIOCommand
//{
//public:
//	CIOSetOneOutputFlash(IO_OUTPUT_PORT_NO OutputNo,IO_OUTPUT_VALUE OutputValue);
//	~CIOSetOneOutputFlash(void);
//public:
//	long ExecuteCommand(void);				// 执行命令
//private:
//	IO_OUTPUT_PORT_NO m_OutputNo;			// 输出端口号
//	IO_OUTPUT_VALUE	  m_OutputValue;		// 输出值
//	long DoSetOutputFun();					// 执行命令函数
//};
//
///**
//@brief   读取IO状态
//*/
//class IO_API CIOGetStatus:public CIOCommand
//{
//public:
//	CIOGetStatus();
//	~CIOGetStatus();
//public:
//	long ExecuteCommand();
//	void GetIOResponse(IO_RSP_STATUS_INFO*);
//private:
//	IO_RSP_STATUS_INFO m_IOStatusInfo;
//	long GetIOStatus();
//};
//
///**
// @brief   关闭IO串口
//*/
//class IO_API CIOClose : public CIOCommand
//{
//public:
//	CIOClose(void);
//	~CIOClose(void);
//
//public:
//   long ExecuteCommand(void);          // 执行命令
//
//private:
//   long CloseFunction();						 // 关闭串口
//};
