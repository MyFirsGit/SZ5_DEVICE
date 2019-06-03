#pragma once

#include <vector>
#include "Util.h"
#include "CommonDefine.h"

using namespace std;

#define MAX_SEND_ITEMS_COUNT		10		 // 发送队列最大元素个数
#define MAX_RECEIVE_ITEMS_COUNT	    1		 // 接收队列最大元素个数

//static bool isBHAbort = false;		// 纸币接收模块是否需要中止动作

const int BOX_ID_LEN = 16;			// 钱箱ID长度为16个字节
const int RES_MAX = 256;			// 纸币接收模块的回复不超过256字节
const int ERR_MAX = 9;				// 状态信息中错误码的最大个数
const int REJ_MAX = 3;				// 状态信息中拒收原因的最大个数
const int VALUENO_NONE = 0xFF;		// 此值代表没有Dxx
const long TIME_BH_ANSWER = 20000;	// 纸币接收模块应在20秒内对有效消息应答

const int GET_ELEM_NONE = -1;		// 取状态回复信息元素：未取得元素
const int GET_ELEM_ERR = -2;		// 取状态回复信息元素：取得元素失败
const int GET_ELEM_END = -3;		// 取状态回复信息元素：遍历完成

#define BH_MOP "MOP"							// 澳门币名称
const static TCHAR* const BH_CNY = _T("CNY");	// 人民币名称

// 命令
#define CMD_BH_RESET			"TF"		// 复位
#define CMD_BH_GET_STATUS		"ST"		// 状态请求
#define CMD_BH_STOP				"V0"		// 进入待命模式
#define CMD_BH_START			"V1"		// 进入识别模式
#define CMD_BH_RETURN_ONE		"B0"		// 退一张：缓冲区->出口
#define CMD_BH_COLLECT			"B1"		// 压钱箱：（缓冲区+暂存器）->钱箱
#define CMD_BH_RETURN_ALL		"B2"		// 退全部：（缓冲区+暂存器）->出口
#define CMD_BH_HOLD				"B3"		// 压暂存：缓冲区->暂存器
#define CMD_BH_TRANS_HOLD		"BD"		// 传送区->暂存器
#define CMD_BH_TRANS_COLLECT	"BE"		// 传送区->钱箱
#define CMD_BH_TRANS_RETURN		"BF"		// 传送区->出口
#define CMD_BH_INFO				"IN"		// 请求信息
#define CMD_BH_FACEVALUE		"IND"		// 请求面值
#define CMD_BH_TRANS_MOD		"D0,0"		// 指定信息：传送系统软件方式字符
#define CMD_BH_ALLOWED_CNT		"D1,1"		// 指定信息：可识别的面值个数
#define CMD_BH_ACTIVE_CNT		"D1,2"		// 指定信息：已激活的面值个数
#define CMD_BH_ACTIVE_LIST		"D1,3"		// 指定信息：已激活的面值列表
#define CMD_BH_ID_MONITOR		"D3,1"		// 指定信息：钱箱ID的监视
#define CMD_BH_MEM_DATA			"D4,0"		// 指定信息：钱箱存储器的数据
#define CMD_BH_MEM_ADDR			"D4,1"		// 指定信息：钱箱存储器的地址
#define CMD_BH_BOX_ID			"D4,2"		// 指定信息：钱箱ID
#define CMD_BH_USER_ID			"D4,3"		// 指定信息：消费者ID
#define CMD_BH_VER_TRANS_TOP	"RS0"		// 获取版本：上部传动系统版本
#define CMD_BH_VER_OS			"RS1"		// 获取版本：识别器OS版本
#define CMD_BH_VER_ALGORITHM	"RS2"		// 获取版本：识别器算法版本
#define CMD_BH_VER_TRANS_BOTTOM	"RS3"		// 获取版本：下部传动系统版本
#define CMD_BH_VER_ODS			"RS5"		// 获取版本：ODS固件版本
#define CMD_BH_READ				"R"			// 读取
#define CMD_BH_WRITE			"W"			// 写入
#define CMD_BH_SET_TIME			"MAW"		// 时间配置
#define CMD_BH_GET_TIME			"MAR"		// 读取时间

// 清除钞票的目的地
typedef enum _bh_clean_dest {
	CLEAN_TO_REGISTER,
	CLEAN_TO_BOX,
	CLEAN_TO_RETURN_SHUTTER,
} BH_CLEAN_DEST;

// 动作信息(Axx)
typedef enum _bh_action {
	ACT_BH_IDLE				= 0x00,		// 空闲
	ACT_BH_INSERT_NG		= 0x01,		// 进钞结果：失败
	ACT_BH_INSERT_OK		= 0x02,		// 进钞结果：成功
	ACT_BH_COLLECT			= 0x03,		// Bx处理结果：压钱箱成功
	ACT_BH_RETURN_BUFF		= 0x04,		// Bx处理结果：缓冲区的一张纸币被退回
	ACT_BH_HOLD				= 0x05,		// Bx处理结果：压暂存成功
	ACT_BH_RETURN_STACK		= 0x06,		// Bx处理结果：暂存器的一张或多张纸币被退回
	ACT_BH_CLEAR_TRANS_NG	= 0x10,		// Bx处理结果：清除传动系统中纸币失败
	ACT_BH_CLEAR_TRANS_OK	= 0x11,		// Bx处理结果：清除传动系统中纸币成功
	ACT_BH_CLEARING_TRANS	= 0x12,		// Bx处理结果：清除传动系统中纸币尚未完成，请重试
} BH_ACTION;

// 纸币接收模块返回的错误码（Exx）
typedef enum _bh_error {
	ERR_BH_IN_TRANS			= 0xE00,	// 传送系统中有纸币
	ERR_BH_RETURN_ALL		= 0xE01,	// 押钞卡钞
	ERR_BH_RETURN_ONE		= 0xE02,	// 退钞卡钞
	ERR_BH_INSERT_JAM		= 0xE03,	// 退钞失败
	ERR_BH_JAM				= 0xE04,	// 暂存钞票到卷筒时失败，或其它任意卡钞
	ERR_BH_COLLECT			= 0xE05,	// 压钱箱失败
	ERR_BH_RETURN_SHUTTER	= 0xE0F,	// 退钞口上有纸币				（警告）
	ERR_BH_INSERT_SHUTTER	= 0xE10,	// 进钞口上有纸币，无法退出
	ERR_BH_ALIGN			= 0xE20,	// 无法对齐卷筒
	ERR_BH_NO_RECOGNIZER	= 0xE30,	// 不能连接纸币识别器
	ERR_BH_RECOGNIZING		= 0xE33,	// 纸币识别器正在启动			（警告）
	ERR_BH_FRAME			= 0xE50,	// 与钱箱架通信超时
	ERR_BH_BOX				= 0xE52,	// 钱箱异常（有纸币从钱箱内退出）
	ERR_BH_NO_BOX			= 0xE54,	// 未装入钱箱或钱箱未锁好
	ERR_BH_FULL				= 0xE55,	// 钱箱已满
	ERR_BH_NEARLY_FULL		= 0xE56,	// 钱箱将满						（警告）
	ERR_BH_HARDWARE			= 0xE80,	// 泛指硬件错误
} BH_ERROR;

// 拒收纸币的原因(Rxx)
typedef enum _bh_reject {
	REJECT_BH_NO_RESULT		= 0x01,		// 无识别结果
	REJECT_BH_MULTI			= 0x22,		// 重钞或纸币太脏
	REJECT_BH_NO_VALUE		= 0x30,		// 未检测出纸币面值
	REJECT_BH_NO_PERMISSION	= 0x31,		// 不允许该面额
	REJECT_BH_COUNTERFEIT	= 0x42,		// 伪钞
	REJECT_BH_UNKNOW_ERR	= 0x60,		// 未知错误
} BH_REJECT;

// 模式信息(Sxx)
typedef enum _bh_mode {
	MOD_BH_STANDBY_NO_BOX	= 0x00,		// 待命模式，无钱箱
	MOD_BH_VALIDATE_NO_BOX	= 0x01,		// 识别模式，无钱箱
	MOD_BH_STANDBY			= 0x10,		// 待命模式，有钱箱
	MOD_BH_VALIDATE			= 0x11,		// 识别模式，有钱箱
} BH_MODE;

// 返回时执行完成的阶段，用于失败时定位错误
// 原因：返回代码错误范围较大，在调用多个原子操作时，无法准确定位哪个操作出现了错误
typedef enum _bh_done_phase {
	DONE_PHASE_READY			= 0x0000, // 初始状态
	DONE_PHASE_ACT_START_ACCEPT	= 0x0001, // 命令完成：开始接收纸币
	DONE_PHASE_ACT_HOLD			= 0x0002, // 命令完成：压暂存
	DONE_PHASE_ACT_COLLECT		= 0x0004, // 命令完成：压钱箱
	DONE_PHASE_ACT_RETURN_ONE	= 0x0008, // 命令完成：退缓冲单张
	DONE_PHASE_ACT_RETURN_ALL	= 0x0010, // 命令完成：退缓冲暂存
	DONE_PHASE_ACT_CLEAN		= 0x0020, // 命令完成：清除钞票
	DONE_PHASE_ACT_GET_STATUS	= 0x0040, // 命令完成：获取状态
	DONE_PHASE_ACT_GET_FACE		= 0x0080, // 命令完成：获取面值
	DONE_PHASE_ACT_ABORT		= 0x0100, // 命令完成：中止轮询
	DONE_PHASE_ACTION			= 0x0200, // 状态返回：需要判断Action状态的使用此标志
	DONE_PHASE_SUCCESS			= 0x0400, // 全部完成
} BH_DONE_PHASE;

// 状态回复信息
typedef struct _bh_sts {
	BH_ACTION			action;			// 最后一个动作
	BYTE				valueNo[2];		// 纸币面值编号
	vector<BH_ERROR>	error;			// 错误码，无错误时vector为空
	int					cnt;			// 暂存器中纸币张数
	vector<BH_REJECT>	reject;			// 拒收钞票的原因，无拒收时vector为空
	BH_MODE				mode;			// 待命/识别模式

	_bh_sts()
	{
		action = ACT_BH_IDLE;
		memset(valueNo, 0xFF, sizeof(valueNo));
		error.reserve(ERR_MAX);
		error.clear();
		cnt = 0;
		reject.reserve(REJ_MAX);
		reject.clear();
		mode = MOD_BH_STANDBY;
	}

	// 解析状态回复信息
	int Deserialize(const BYTE*);

	bool IsValidFaceValue()
	{
		if (valueNo[0] == 0xFF && valueNo[1] == 0xFF)
			return false;
		return true;
	}

} BH_STS;

//////////////////////////////////////////////////////////////////////////
/**
 @brief		解析状态信息

 @param		(i)const BYTE *lpStatus		状态信息

 @retval	BYTE  0 : 正常  >0 : 纸币接收模块返回的错误码  <0 : 解析异常

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
inline int BH_STS::Deserialize(const BYTE* lpStatus)
{
	// -----------------------------------------------
	// 纸币接收模块返回的状态信息格式如下：
	// S11,N01,A00,D03,P02,E00
	// 在此把一个字母开头带2个数字的形式称为一个元素
	// 其中Sxx,Nxx,Axx元素必须有且只有一个
	//     Dxx元素不超过1个，可以没有
	//     Exx元素不超过9个，可以没有
	//     Rxx元素不超过3个，可以没有
	// 其它元素在此不检查
	// -----------------------------------------------
	LPSTR junk = NULL;
	const BYTE* lpData = lpStatus;
	while (*lpData)
	{
		// 解析前缀
		BYTE prefix = *lpData;

		// 解析代码
		if (prefix == 'D')
		{
			memcpy(valueNo, lpData + 1, 2);
		}
		else
		{
			WORD code = strtoul((char*)(lpData + 1), &junk, 16);
			switch (prefix)
			{
			case 'S':
				mode = (BH_MODE)code;
				break;
			case 'A':
				action = (BH_ACTION)code;
				break;
			case 'N':
				cnt = code;
				break;
			case 'R':
				reject.push_back((BH_REJECT)code);
				break;
			case 'E':
				code = (0x0E << 8) + code;
				error.push_back((BH_ERROR)code);
				break;
			}
		}

		// 跳过前缀+代码+逗号
		lpData += 3;
		if (*lpData == ',') ++lpData;
	}

	return 0;
}

// 纸币面值信息
typedef struct _bh_facevalue {
	BankNoteAndCoinType_t	faceValue;		// 纸币面值

	_bh_facevalue()
	{
		faceValue = VALUE_UNKNOWN;
	}

	// 解析状态回复信息
	int Deserialize(const BYTE*);

} BH_FACEVALUE;

//纸币面值接收表
typedef struct _bh_facevalue_table {
	bool bIsBanknote10MOP;
	bool bIsBanknote20MOP;
	bool bIsBanknote50MOP;
	bool bIsBanknote100MOP;
	bool bIsBanknote500MOP;
	bool bIsBanknote1000MOP;
	_bh_facevalue_table(){
		bIsBanknote10MOP	= false;
		bIsBanknote20MOP	= false;
		bIsBanknote50MOP	= false;
		bIsBanknote100MOP	= false;
		bIsBanknote500MOP	= false;
		bIsBanknote1000MOP	= false;
	}
}BH_FACEVALUE_TABLE;

//人民币面额接收列表 (由于业务层目前涉及到接收表的地方较多，不便修改，暂重新定义于此)
typedef struct bh_face_accpted_table {
	bool bIsBanknote1Yuan;
	bool bIsBanknote5Yuan;
	bool bIsBanknote10Yuan;
	bool bIsBanknote20Yuan;
	bool bIsBanknote50Yuan;
	bool bIsBanknote100Yuan;
	bh_face_accpted_table()
	{
		bIsBanknote1Yuan = false;
		bIsBanknote5Yuan = false;
		bIsBanknote10Yuan = false;
		bIsBanknote20Yuan = false;
		bIsBanknote50Yuan = false;
		bIsBanknote100Yuan = false;
	}

}BH_FACE_ACEPTED_TABLE;
//////////////////////////////////////////////////////////////////////////
/**
 @brief		解析纸币面值信息

 @param		(i)const BYTE *lpInfo	纸币面值信息

 @retval	BYTE  0 : 正常  非0 : 解析异常

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
inline int BH_FACEVALUE::Deserialize(const BYTE* lpInfo)
{
	// -----------------------------------------------
	// 纸币接收模块返回的纸币面值信息格式如下：
	// MOP 1,000 B
	// 开头用3个字母表示货币名称（ISO4127标准）
	// 紧跟着一个空格字符，后面是面值
	// 如有必要，在另一个空个字符后面再附加一个表示不同类型的字符。
	// -----------------------------------------------

	CStringA INDxxInfo(lpInfo);

	// 检查合法性
	if ("" == INDxxInfo || " " == INDxxInfo || "?" == INDxxInfo || "-" == INDxxInfo){
		faceValue = VALUE_UNKNOWN;
		return -1;
	}

	// 检查是否澳门币,是则去掉MOP
	CStringA sName = INDxxInfo.Mid(0, 3);
	if (BH_CNY != sName){
		faceValue = VALUE_UNKNOWN;
		return -1;
	}
	INDxxInfo = INDxxInfo.Right(INDxxInfo.GetLength() - 4);

	// 检查是否存在附加字母，存在则去掉
	int pos = INDxxInfo.Find(' ');
	if (-1 != pos){
		INDxxInfo = INDxxInfo.Left(pos);
	}

	// 面值字符串转为数值型
	INDxxInfo.Remove(',');
	int value = atoi(INDxxInfo);
	switch (value){
	case 5:
		faceValue = BankNoteAndCoinType_t::Banknote5Yuan;
		break;
	case 10:
		faceValue = BankNoteAndCoinType_t::Banknote10Yuan;
		break;
	case 20:
		faceValue = BankNoteAndCoinType_t::Banknote20Yuan;
		break;
	case 50:
		faceValue = BankNoteAndCoinType_t::Banknote50Yuan;
		break;
	case 100:
		faceValue = BankNoteAndCoinType_t::Banknote100Yuan;
		break;
	default:
		faceValue = VALUE_UNKNOWN;
	}

	return 0;
}

// 状态信息
typedef struct _bh_status {
	BH_STS					bh_sts;			// 纸币接收模块状态
	BH_FACEVALUE			bh_faceValue;	// 识别的面值
} BH_STATUS;


// 钱箱ID
typedef struct _bh_box_id {
	BYTE user_id[BOX_ID_LEN];	// 顾客ID
	BYTE manu_id[BOX_ID_LEN];	// 制造商ID
	_bh_box_id()
	{
		memset(user_id, 0, sizeof(user_id));
		memset(manu_id, 0, sizeof(user_id));
	}
} BH_BOX_ID;

// 版本信息
typedef struct _bh_version {
	BYTE trans_top[RES_MAX];		// 上部传动系统版本
	BYTE os[RES_MAX];				// 识别器OS版本
	BYTE algorithm[RES_MAX];		// 识别器算法版本
	BYTE trans_bottom[RES_MAX];		// 下部传动系统版本
	BYTE ods[RES_MAX];				// ODS固件版本

	_bh_version()
	{
		memset(trans_top, 0, sizeof(trans_top));
		memset(os, 0, sizeof(os));
		memset(algorithm, 0, sizeof(algorithm));
		memset(trans_bottom, 0, sizeof(trans_bottom));
		memset(ods, 0, sizeof(ods));
	}

	// 解析版本回复信息
	int Deserialize(const BYTE *, BYTE *);

} BH_VERSION;

//////////////////////////////////////////////////////////////////////////
/**
 @brief		解析版本信息

 @param		(i)const BYTE *lpInfo	版本信息

 @retval	BYTE  0 : 正常  非0 : 解析异常

 @exception	无
*/
//////////////////////////////////////////////////////////////////////////
inline int BH_VERSION::Deserialize(const BYTE* lpInfo, BYTE* verInfo)
{
	CStringA slpInfo(lpInfo);

	// 检查合法性
	if (NULL == verInfo || NULL == lpInfo){
		return -1;
	}

	// 检查版本合法性
	if ("" == slpInfo || " " == slpInfo || "?" == slpInfo || "-" == slpInfo){
		return -1;
	}

	memcpy(verInfo, (BYTE *)slpInfo.GetBuffer(slpInfo.GetLength()), slpInfo.GetLength());
	return 0;
}
