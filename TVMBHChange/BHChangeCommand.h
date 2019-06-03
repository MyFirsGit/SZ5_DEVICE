#pragma once
#include "BHExp.h"
#include "Command.h"
#include "BHdef.h"
#include "BHCom.h"
#include "BHChangeLib.h"
#include "BHChangeException.h"
#include "BHChangeHardwareException.h"


using namespace BHCHANGE;
/**
 @brief  BH命令父类
*/
class BH_API CBHChangeCommand : public CCommand
{
public:
	CBHChangeCommand(void);   
    ~CBHChangeCommand(void);
public:
	BOOL         IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime);
	BHTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer = 10000);
	BHTRS_RESULT SendOneCommand_NoRrcv(DWORD nSendLength,BYTE* pSendBuffer);
};

/**
 @brief	原子命令——打开串口
*/
class BH_API CBHChangeConnect : public CBHChangeCommand
{
public:
	CBHChangeConnect(int nPort, int nBaud, int nSize);
	~CBHChangeConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// 端口号
	int m_nBaud;						// 波特率
	int m_nSize;						// 字长
	BHCHANGETRS_CONNECT_PARAM m_connectParam;	// 连接参数
private:
	long ConnectFunction();
};

/**
 @brief	原子命令——关闭串口
*/
class BH_API CBHChangeClose : public CBHChangeCommand
{
public:
	CBHChangeClose();
	~CBHChangeClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};


/**
 @brief  设备初始化命令
*/
class BH_API CBHChangeDevInit : public CBHChangeCommand
{
public:
	CBHChangeDevInit(BYTE FrontShutterAction,BYTE RearShutterAction,ParamDevInit& ref,BOOL isEnhanced = TRUE);
	~CBHChangeDevInit();
public:
	long ExecuteCommand();
private:
	long DevInitFunction();
	// 局部参数
	BYTE m_FontShutterAction,m_RearShutterAction;
	BOOL m_isEnhanced;
	PParamDevInit ptrParaDev;
	// 接收数据
	BYTE m_RecvBuffer[RES_MAX];
	DWORD m_RecvLen;
};

/**
 @brief  获取设备状态信息
*/
class BH_API CBHChangeGetStatusInfo : public CBHChangeCommand
{
public:
	CBHChangeGetStatusInfo(/*BOOL isEnhanced = TRUE*/);
	~CBHChangeGetStatusInfo();
public:
	long ExecuteCommand();
	void GetResponse(PDEVICE_STATUS_INFO response);
private:
	long GetStatusInfoFunction();
	// 接收数据
	BYTE m_RecvBuffer[RES_MAX];
	DWORD m_RecvLen;
	DEVICE_STATUS_INFO m_response;
};


/**
 @brief  bill transportation (参考翻译:钞票传输)
*/
class BH_API CBHChangeBillTransportation : public CBHChangeCommand
{
public:
	CBHChangeBillTransportation();
	~CBHChangeBillTransportation();
public:
	long ExecuteCommand();
private:
	long BillTransportationFunction();

	BYTE RecvBuffer[RES_MAX];
	DWORD RecvLen;
};

/**
@brief SendOutBill 纸币送出
*/
class BH_API CBHChangeSendOutBill : public CBHChangeCommand{
public:
	CBHChangeSendOutBill(int nCountBoxA,int nCountBoxB);
	~CBHChangeSendOutBill();

	long ExecuteCommand();
	void GetChangeResponse(PBANKNOTE_CHANGE_DETAIL rsp);
private:
	// Send Content
	long SendOutBill();
	int  m_nCountBoxA; // 纸币找零箱A出币张数
	int  m_nCountBoxB; // 纸币找零箱B出币张数
	BYTE TransforIntToHEXCode(int nCount);	// 转换数字为纸币找零模块要求字序

	long BillTransportation();
	// Recv Content
	BYTE m_RecvBuff[RES_MAX];
	DWORD m_RecvLen;
	BANKNOTE_CHANGE_DETAIL m_changeDetail;
};