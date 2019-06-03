#pragma once
#include "BHExp.h"
#include "Command.h"
#include "BHdef.h"
#include "BHLib.h"

/**
 @brief  BH命令父类
*/
class BH_API CBHCommand : public CCommand
{
public:
	CBHCommand(void);   
    ~CBHCommand(void);
public:
	BOOL         IsTimeUp(IN DWORD dwTimer,IN DWORD	dwBaseTime);
	BHTRS_RESULT SendOneCommand(DWORD nSendLength,BYTE* pSendBuffer, BYTE* pRecvBuff,DWORD& nRecvLength,DWORD dwTimer = 10000);
	BHTRS_RESULT SendOneCommand_NoRrcv(DWORD nSendLength,BYTE* pSendBuffer);
};
/**
 @brief	原子命令——打开串口
*/
class BH_API CBHConnect : public CBHCommand
{
public:
	CBHConnect(int nPort, int nBaud, int nSize);
	~CBHConnect();
public:
	long ExecuteCommand();
private:
	int m_nPort;						// 端口号
	int m_nBaud;						// 波特率
	int m_nSize;						// 字长
	BHTRS_CONNECT_PARAM m_connectParam;	// 连接参数
private:
	long ConnectFunction();
};

/**
 @brief	原子命令——关闭串口
*/
class BH_API CBHClose : public CBHCommand
{
public:
	CBHClose();
	~CBHClose();
public:
	long ExecuteCommand();
private:
	long CloseFunction();
};

/**
 @brief	原子命令——复位（TF）
*/
class BH_API CBHReset : public CBHCommand
{
public:
	CBHReset();
	~CBHReset();
	DWORD done_phase; // 完成阶段
public:
	long ExecuteCommand();
private:
	long ResetFunction();
};

/**
 @brief	原子命令——获取状态（ST）
*/
class BH_API CBHGetSts : public CBHCommand
{
public:
	CBHGetSts();
	~CBHGetSts();
public:
	long ExecuteCommand();
	long GetResponse(BH_STS*);
private:
	long GetStsFunction();
	BH_STS m_bhSts;
};

/**
 @brief	原子命令——获取面值（INDxx）
*/
class BH_API CBHGetFaceValue : public CBHCommand
{
public:
	CBHGetFaceValue(BYTE* face_id);
	~CBHGetFaceValue();
public:
	long ExecuteCommand();
	long GetResponse(BH_FACEVALUE*);
private:
	long GetFaceValueFunction();
	BH_FACEVALUE m_face_value;			// 实际面值大小
	BYTE* m_face_id;					// 纸币面值编号
};

/**
 @brief	原子命令——进入识别模式，开始接受纸币（V1）
*/
class BH_API CBHStartValidation : public CBHCommand
{
public:
	CBHStartValidation();
	~CBHStartValidation();
public:
	long ExecuteCommand();
private:
	long StartValidationFunction();
};

/**
 @brief	原子命令——进入待命模式，停止接收纸币（V0）
*/
class BH_API CBHStopValidation : public CBHCommand
{
public:
	CBHStopValidation();
	~CBHStopValidation();
public:
	long ExecuteCommand();
private:
	long StopValidationFunction();
};

/**
 @brief	组合命令——退回缓冲区的纸币（缓冲区->出口）（B0+ST，是否等待退钞口钞票被取走，构造时确定）
*/
class BH_API CBHReturnOne : public CBHCommand
{
public:
	CBHReturnOne(bool isCheckShutter);
	~CBHReturnOne();
	DWORD done_phase; // 完成阶段
public:
	long ExecuteCommand();
private:
	long ReturnOneFunction();
	bool m_isCheckShutter;
};

/**
 @brief	组合命令——压钱箱（缓冲区+暂存器->钱箱）（B1+ST,不作钱箱将满、满判断）
*/
class BH_API CBHCollect : public CBHCommand
{
public:
	CBHCollect();
	~CBHCollect();
	DWORD done_phase; // 完成阶段
public:
	long ExecuteCommand();
private:
	long CollectFunction();
};

/**
 @brief	组合命令——退回全部纸币（缓冲区+暂存器->出口）（B2+ST，是否检查退钞口有钞票在构造时确认）
*/
class BH_API CBHReturnAll : public CBHCommand
{
public:
	CBHReturnAll(bool isCheckShutter);
	~CBHReturnAll();
	DWORD done_phase; // 完成阶段
public:
	long ExecuteCommand();
private:
	long ReturnAllFunction();
	bool m_isCheckShutter;
};

/**
 @brief	组合命令——压暂存（缓冲区->暂存器）（B3+ST）
*/
class BH_API CBHHold : public CBHCommand
{
public:
	CBHHold();
	~CBHHold();
	DWORD done_phase; // 完成阶段
public:
	long ExecuteCommand();
private:
	long HoldFunction();
};

/**
 @brief 原子命令——清除钞票（传送区->暂存器）（BD）
*/
class BH_API CBHTransHold : public CBHCommand
{
public:
	CBHTransHold();
	~CBHTransHold();
public:
	long ExecuteCommand();
private:
	long TransHoldFunction();
};

/**
 @brief	原子命令——清除钞票（传送区->钱箱）（BE）
*/
class BH_API CBHTransCollect : public CBHCommand
{
public:
	CBHTransCollect();
	~CBHTransCollect();
public:
	long ExecuteCommand();
private:
	long TransCollectFunction();
};

/**
 @brief	原子命令——清除钞票（传送区->出口）（BF）
*/
class BH_API CBHTransReturn : public CBHCommand
{
public:
	CBHTransReturn();
	~CBHTransReturn();
public:
	long ExecuteCommand();
private:
	long TransReturnFunction();	
};

/**
 @brief	原子命令——中止轮询动作（全局变量）
*/
class BH_API CBHAbortAction : public CBHCommand
{
public:
	CBHAbortAction();
	~CBHAbortAction();
public:
	long ExecuteCommand();
};

BH_API void  BH_EnableInsert();
BH_API void  BH_DisableInsert();

/**
 @brief	组合命令——等待投入识别（V1+ST）
*/
class BH_API CBHOpenAndValidate : public CBHCommand
{
public:
	CBHOpenAndValidate();
	CBHOpenAndValidate(BH_FACE_ACEPTED_TABLE& bhTable);
	~CBHOpenAndValidate();
public:
	long ExecuteCommand();	
	long GetResponse(BH_STATUS*);
public:
	DWORD done_phase;				// 执行结果
private:
	long OpenAndValidateFunction();
	long GetFace();
	bool IsAcceptFaceValue(BankNoteAndCoinType_t& thisFaceValue);
	BH_STATUS m_bhStatus;
	BH_FACE_ACEPTED_TABLE m_bhTable;
};

/**
 @brief	组合命令——清空传送系统（BD/BE/BF+ST）
*/
class BH_API CBHClearTrans : public CBHCommand
{
public:
	CBHClearTrans(BH_CLEAN_DEST dest = CLEAN_TO_BOX);
	~CBHClearTrans();
public:
	long ExecuteCommand();
public:
	DWORD done_phase;				// 执行结果
private:
	long ClearTransFunction();
	BH_CLEAN_DEST m_clean_dest;
};

/**
 @brief 组合命令——获取版本（RS0+RS1+RS2+RS3+RS5）
*/
class BH_API CBHGetVersion : public CBHCommand
{
public:
	CBHGetVersion();
	~CBHGetVersion();
public:
	long ExecuteCommand();
	long GetResponse(BH_VERSION*);
private:
	long GetVersionFunction();
	BH_VERSION m_bhVer;
};

/**
 @brief	组合命令——获取钱箱ID（D4,2+R+D4,3+R）
*/
class BH_API CBHGetBoxID : public CBHCommand
{
public:
	CBHGetBoxID();
	~CBHGetBoxID();
public:
	long ExecuteCommand();
	long GetResponse(BH_BOX_ID*);
private:
	long GetBoxIDFunction();
	BH_BOX_ID m_box_id;
};

/**
 @brief	原子命令——时间配置（MAW*）
*/
class BH_API CBHSetTime : public CBHCommand
{
public:
	CBHSetTime(_DATE_TIME current);
	~CBHSetTime();
public:
	long ExecuteCommand();
	long GetResponse(_DATE_TIME*);
private:
	long SetTimeFunction();
	_DATE_TIME m_current_datetime;
	_DATE_TIME m_bh_datetime;
};
