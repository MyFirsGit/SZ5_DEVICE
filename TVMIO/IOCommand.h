//#pragma once
//#include "IOExp.h"
//#include "Command.h"
//#include "IOLib.h"
//#include "IOdef.h"
//#include "IOException.h"
//
///**
// @brief  IO�����
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
// @brief  ��IO
//*/
//class IO_API CIOConnect : public CIOCommand
//{
//public:
//	CIOConnect(IOTRS_CONNECT_PARAM);   
//    ~CIOConnect(void);
//public:
//	long ExecuteCommand(void);   // ִ������
//private:
//	IOTRS_CONNECT_PARAM m_connectParam;// ���Ӳ���
//	long ConnectFunction();           // �򿪴���
//};
//
///**
// @brief  ��ʼ��IO
//*/
//class IO_API CIOInitialize : public CIOCommand
//{
//public:
//	CIOInitialize();   
//    ~CIOInitialize(void);
//public:
//	long ExecuteCommand(void);		// ִ������
//private:
//	long InitializeFunction();				// �򿪴���
//};
//
///**
// @brief  ���õ�·���״̬��������
//*/
//class IO_API CIOSetOneSlotOutput : public CIOCommand
//{
//public:
//	CIOSetOneSlotOutput(IO_OUTPUT_PORT_NO OutputNo,IO_OUTPUT_VALUE OutputValue);
//	~CIOSetOneSlotOutput(void);
//public:
//	long ExecuteCommand(void);				// ִ������
//private:
//	IO_OUTPUT_PORT_NO m_OutputNo;			// ����˿ں�
//	IO_OUTPUT_VALUE		m_OutputValue;		// ���ֵ
//	long SetOutputFunction();							// ���ô���
//};
//
///**
//@brief ���õ�·���״̬����˸��
//*/
//class IO_API CIOSetOneOutputFlash : public CIOCommand
//{
//public:
//	CIOSetOneOutputFlash(IO_OUTPUT_PORT_NO OutputNo,IO_OUTPUT_VALUE OutputValue);
//	~CIOSetOneOutputFlash(void);
//public:
//	long ExecuteCommand(void);				// ִ������
//private:
//	IO_OUTPUT_PORT_NO m_OutputNo;			// ����˿ں�
//	IO_OUTPUT_VALUE	  m_OutputValue;		// ���ֵ
//	long DoSetOutputFun();					// ִ�������
//};
//
///**
//@brief   ��ȡIO״̬
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
// @brief   �ر�IO����
//*/
//class IO_API CIOClose : public CIOCommand
//{
//public:
//	CIOClose(void);
//	~CIOClose(void);
//
//public:
//   long ExecuteCommand(void);          // ִ������
//
//private:
//   long CloseFunction();						 // �رմ���
//};
