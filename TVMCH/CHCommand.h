#pragma once
#include "CHExp.h"
#include "Command.h"
#include "CHdef.h"
#include "CHLib.h"
#include "CHCom.h"
#include "Sync.h"
#include "DebugLog.h"
#include "CHException.h"
#include "CHHardwareException.h"
#include "CHAApi.h"

#ifndef theCH_TRACE
	#define theCH_TRACE CFileLog::GetInstance(_T("CH_"))
#endif


/**
 @brief  CH命令父类
*/
class CH_API CCHCommand : public CCommand
{
public:
	CCHCommand(void){} 
    ~CCHCommand(void){}
public:
	long ExecuteCommand() = 0;
};

/**
 @brief	原子命令——打开串口
*/
class CH_API CCHConnect : public CCommand
{
public:
	CCHConnect(int nPort, int nBaud, int nSize);
	~CCHConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// 端口号
	int m_nBaud;						// 波特率
	int m_nSize;						// 字长
private:
	long ConnectFunction();

};

/**
 @brief	原子命令——关闭串口
*/
class CH_API CCHClose : public CCommand
{
public:
	CCHClose();
	~CCHClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	原子命令——初始化硬币模块
*/
class CH_API CCHInit : public CCommand
{
public:
	CCHInit(BYTE ctl);
	~CCHInit();
public:
	long ExecuteCommand();
private:
	long InitFunction();
	BYTE m_ctl;
};

/**
 @brief	复位初始化
*/
class CH_API CCHReset : public CCHCommand
{
public:
	CCHReset(BYTE XinCtl);
	~CCHReset();
public:
	long ExecuteCommand();
private:
	long ResetFunction();
	BYTE m_Ctl;
};

/**
 @brief	状态读取
*/
class CH_API CCHGetStatus: public CCHCommand
{
public:
	CCHGetStatus();
	~CCHGetStatus();
public:
	long ExecuteCommand();
	long GetResponse(tCHADevStatus* pRes);
private:
	long GetStatusFunction();
	tCHADevStatus m_status;
};

/**
 @brief	设置硬币模块各部件的硬币数
*/
class CH_API CCHSetCoinNum: public CCHCommand
{
public:
	CCHSetCoinNum(tCoinBoxInfo numInfo);
	~CCHSetCoinNum();
public:
	long ExecuteCommand();
private:
	long SetNumFunction();
	tCoinBoxInfo m_numInfo;
};

/**
 @brief	开始接收
*/
class CH_API CCHStartReceive: public CCHCommand
{
public:
	CCHStartReceive();
	~CCHStartReceive();
public:
	long ExecuteCommand();
private:
	long StartReceiveFunction();
};

// 读取接收到的硬币信息
class CH_API CCHReadCoinNum: public CCHCommand
{
public:
	CCHReadCoinNum();
	~CCHReadCoinNum();
public:
	long ExecuteCommand();
	long GetResponse(tCHACoinValueInfo* pRes);
private:
	long ReadCoinNumFunction();
	tCHACoinValueInfo m_res;//依次分别表示了 1 元、5 角、退币的信息；
};


CH_API void CH_EnableInsert();
CH_API void CH_DisableInsert();
// 控制读取接收到的硬币信息
class CH_API CCHReadReceivs : public CCHCommand
{
public:
	CCHReadReceivs ();
	~CCHReadReceivs ();
public:
	long ExecuteCommand();
	void GetCHResponse(tCHACoinValueInfo* rsp);
private:
	long GetCH_Insert_Info();
	bool Ch_Insert();
	tCHACoinValueInfo  m_res;//依次分别表示了 1 元、5 角、退币的信息；
};

/**
 @brief	设置拒收硬币类型
*/
class CH_API CCHSetRejectType : public CCHCommand
{
public:
	CCHSetRejectType (COIN_ACCEPTABLE& acceptTable);
	~CCHSetRejectType ();
public:
	long ExecuteCommand();
private:
	long SetFunction();
	COIN_ACCEPTABLE m_acceptTable;
};

/**
 @brief	停止接收硬币
*/
class CH_API CCHEndReceive : public CCHCommand
{
public:
	CCHEndReceive ();
	~CCHEndReceive ();
public:
	long ExecuteCommand();
private:
	long EndFunction();
};

/**
 @brief	停止读取接收信息
*/
class CH_API CHStopReadRV: public CCHCommand
{
public:
	CHStopReadRV(){
	}
	~CHStopReadRV(){
	}
public:
	long ExecuteCommand(void){
		CH_DisableInsert();
		long lRet = 0;
		OnComplete(lRet);
		return lRet;
	}
};

/**
 @brief	退回硬币
*/
class CH_API CCHRefund: public CCHCommand
{
public:
	CCHRefund(UINT coinnum);
	~CCHRefund();
public:
	long ExecuteCommand();
private:
	long RefundFunction();
	UINT m_returncoinnum;
};

/**
 @brief	存入硬币
*/
class CH_API CCHAccept: public CCHCommand
{
public:
	CCHAccept();
	~CCHAccept();
public:
	long ExecuteCommand();
	long GetResponse(tCHACoinValueInfo* rsp);
private:
	long AcceptFunction();
	BYTE m_bHopperAccept;
	tCHACoinValueInfo m_rsp;
};

/**
 @brief	找零
*/

class CH_API CCHChange:public CCHCommand
{
public:
	CCHChange(WORD num_OneY,WORD num_HalfY);
	~CCHChange();
public:
	long ExecuteCommand();
	long GetResponse(tCHAChangeNum* rsp);
private:
	long ChangeFunction();
	WORD m_num_OneY; 
	WORD m_num_HalfY;
	tCHAChangeNum changeSet;

};


/**
 @brief	清理硬币
*/
class CH_API CCHClearCoin: public CCHCommand
{
public:
	CCHClearCoin();
	~CCHClearCoin();
public:
	long ExecuteCommand();
	long GetResponse (tCoinBoxInfo*);
private:
	long ClearFunction();
	tCoinBoxInfo CoinBoxInfo;
};


/**
 @brief	读RFID
*/
class CH_API CCHReadRfid: public CCHCommand
{
public:
	CCHReadRfid(int boxNo);//:0x01 = 加币箱2  0x02 = 加币箱1  0x03 = 回收箱1 
	~CCHReadRfid();
public:
	long ExecuteCommand();
	long GetResponse(BYTE* rfidData);
private:
	long ReadFunction();
	int m_boxNo;
	BYTE m_RfidData[64];
};


/**
 @brief	写RFID
*/
class CH_API CCHWriteRfid: public CCHCommand
{
public:
	CCHWriteRfid(int boxNo, BYTE* rfidData);//:0x01 加币箱2; 0x02 加币箱1; 0x03 回收箱 
	~CCHWriteRfid();
public:
	long ExecuteCommand();
private:
	long WriteFunction();
	int m_boxNo;
	BYTE m_RfidData[64];
};