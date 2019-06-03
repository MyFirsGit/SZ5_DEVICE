/*
DATE: 2016-11-15
AUTHOR: lichao
*/
#pragma once
#include "CommonDefine.h"
//#include "RWException.h"

/* 
此文件中相关结构，定义参考于BRH底层接口数据结构定义。 
*/


// BRH获取状态命令反馈结构
typedef T_XfsCdrStatus BRH_RSP_GET_STATUS, *LPBRH_RSP_GET_STATUS;

// BRH获取详细状态命令反馈结构
typedef T_SystemStatusOverview BRH_RSP_GET_SYSTEM_STATUS, *LPBRH_RSP_GET_SYSTEM_STATUS;

// BRH获取性能命令反馈结构
typedef T_XfsCapabilities BRH_RSP_GET_CAPABLILITIES, *LPBRH_RSP_GET_CAPABLILITIES;

// BRH设置性能命令结构
typedef T_XfsCapabilities BRH_CMD_SET_CAPABLILITIES, *LPBRH_CMD_SET_CAPABLILITIES;

// BRH设置时间命令结构
typedef T_TransactionStatus BRH_CMD_GET_TRANSACTIONSTATUS, *LPBRH_CMD_GET_TRANSACTIONSTATUS;

// BRH找零请求命令结构
typedef T_XfsDispenseRequest BRH_CMD_DISPENSE_REQUEST, *LPBRH_CMD_DISPENSE_REQUEST;

// BRH获取现金单元信息命令结构
typedef T_XfsCashUnit BRH_RSP_QUERY_CASHUNIT, *LPBRH_RSP_QUERY_CASHUNIT;

// BRH更新现金单元信息命令结构
typedef T_XfsCashUnit BRH_CMD_UPDATE_CASHUNIT, *LPBRH_CMD_UPDATE_CASHUNIT;

// BRH获取面值命令结构
typedef T_DenominationList BRH_RSP_QUERY_DENOMINATIONS, *LPBRH_RSP_QUERY_DENOMINATIONS;

// BRH更新面值命令结构
typedef T_DenominationList BRH_CMD_UPDATE_DENOMINATIONS, *LPBRH_CMD_UPDATE_DENOMINATIONS;


// 状态回复信息
typedef struct _brh_sts {
	//BYTE				errorCode;		// 1字	当前故障代码					
	//BYTE				warning[2];		// 2字	警告代码 无警告为00
	//BH_PROC_NUMBER		processNumber;	// 1字	当前纸币处理序号
	//int					validBnCount;	// 1字	十六进制字符 表示暂存箱中有效纸币的数量
	//BYTE				cleanErorMode;	// 1字	故障清除模式 A 自动 S 用户指定
	//BYTE				effectiveMode;	// 1字	“1”为未使用（前生效模式）
	//BH_WORK_DEADLINE	workDeadLine;	// 1字	“工作”命令限时=0，1，2，3或4（1个字符）
	//bool				bnaSvcLogo;		// 1字	BNA5 NT服务标志（选项）（“0”为否，“1”为是）（1个字符

	//_bh_sts()
	//{
	//	//errorCode = 0;
	//	memset(warning, 0, 2);
	//	processNumber	= BN_FREE_TEMPORARY;
	//	validBnCount	= 0;
	//	cleanErorMode	= CUSTOM_CLEAN_ERROR;
	//	//effectiveMode	= 0;
	//	workDeadLine	= DEADLINE_NORMAL;
	//	bnaSvcLogo		= true;
	//}

	//// 解析状态回复信息
	//int Deserialize(const BYTE*);

	//// 判断纸币有效性
	//bool IsValidFaceValue()
	//{
	//	return true;
	//}

	//// 将警告代码（字符）翻译为异常代码
	//BH_ERROR TranslateWarningCode()
	//{
	//	int nHigh = warning[0]; nHigh = nHigh &0xFF;
	//	int nLow  = warning[1]; nLow  = nLow  & 0xFF;
	//	int nError= nHigh*100 + nLow;
	//	if ((((nError & ERROR_BH_BAREHEADED) == ERROR_BH_BAREHEADED) || ((nError & ERROR_BH_BAREHEADED_G) == ERROR_BH_BAREHEADED_G)) && nError < 7300){
	//		nError = ERROR_BH_BAREHEADED;
	//	}

	//	return (BH_ERROR)nError;
	//}

} BRH_STS;

// 状态信息
typedef struct _brh_status {
	BRH_STS					bh_sts;			// 纸币接收模块状态
	BankNoteAndCoinType_t	bh_faceValue;	// 识别的面值
} BRH_STATUS;

// 操作指令的反馈
typedef struct brh_cmd_feedback{
	//BH_STS						state;		// 当前纸币器的状态
	//BH_COMMOND_TYPE				cmdType;	// 操作指令类型
	int							nCount;		// 处理纸币张数
	BankNoteAndCoinType_t		faceValue;	// 处理纸币的面额(只有接收成功的时候才会有面额)

	brh_cmd_feedback()
	{
		//cmdType		= BH_UNKNOW;
		nCount		= 0;
		faceValue	= VALUE_UNKNOWN;
	}

	// 判断纸币有效性
	bool IsValidFaceValue()
	{
		if (faceValue != VALUE_UNKNOWN){
			return true;
		}
		return false;
	}

}BRH_CMD_FEEDBACK;


// MEI 纸币回收箱ID
typedef struct _brh_box_id
{
	BYTE serialID[6];	// 序号
	BYTE modelID[3];		// 型号
	_brh_box_id()
	{
		memset(serialID, 0, 6);
		memset(modelID, 0, 3);
	}
}BRH_BOX_ID;

// 清除钞票的目的地
typedef enum _brh_clean_dest {
	CLEAN_TO_REGISTER,
	CLEAN_TO_BOX,
	CLEAN_TO_RETURN_SHUTTER,
} BRH_CLEAN_DEST;

// 箱子ID定义，上层直接应用
typedef enum _em_bnr_module_id_{
	BNR_MODULE_CASH_BOX = CASHBOX_CLASS,
	BNR_MODULE_CHANGE_BOX = LOADER_CLASS + 1,
	BNR_RECYCLER_BOX_A = RECYCLER_CLASS + 3,
	BNR_RECYCLER_BOX_B,
	BNR_RECYCLER_BOX_C,
	BNR_RECYCLER_BOX_D,
}BNR_MODULE_ID;

// 箱子逻辑ID定义
typedef enum _em_bnr_logical_id_
{
	BNR_LOGICAL_RECYCLER_A1 = 3,
	BNR_LOGICAL_RECYCLER_A2 = 9,
	BNR_LOGICAL_RECYCLER_B1 = 4,
	BNR_LOGICAL_RECYCLER_B2 = 8,
	BNR_LOGICAL_RECYCLER_C1 = 2,
	BNR_LOGICAL_RECYCLER_C2 = 7,
	BNR_LOGICAL_RECYCLER_D1 = 1,
	BNR_LOGICAL_RECYCLER_D2 = 10,

	BNR_LOGICAL_CB_CNY_1AA   = 22,
	BNR_LOGICAL_CB_CNY_1AB   = 23,
	BNR_LOGICAL_CB_CNY_5AA   = 20,
	BNR_LOGICAL_CB_CNY_5AB   = 24,
	//BNR_LOGICAL_CB_CNY_5AC   = ,
	BNR_LOGICAL_CB_CNY_10AA   = 19,
	BNR_LOGICAL_CB_CNY_10AB   = 25,
	BNR_LOGICAL_CB_CNY_20AA   = 26,
	BNR_LOGICAL_CB_CNY_50AA   = 18,
	BNR_LOGICAL_CB_CNY_50AB   = 17,
	BNR_LOGICAL_CB_CNY_50AC   = 16,
	BNR_LOGICAL_CB_CNY_100AA   = 15,
	BNR_LOGICAL_CB_CNY_100AB   = 14,
	BNR_LOGICAL_CB_CNY_100AC   = 13,
	BNR_LOGICAL_CB_CNY_100AD   = 27,

	//BNR_LOGICAL_CHANGE_BOX = 5,
}BNR_LOGICAL_ID;

// 找零/清币返回结构体
typedef struct _st_change_result_{
	DWORD dwOutAmount;
	WORD wRecyclerAOutCount;
	WORD wRecyclerBOutCount;
	WORD wRecyclerCOutCount;
	WORD wRecyclerDOutCount;

	_st_change_result_(){

		dwOutAmount = 0;
		wRecyclerAOutCount = 0;
		wRecyclerBOutCount = 0;
		wRecyclerCOutCount = 0;
		wRecyclerDOutCount = 0;
	}
}CHANGE_RESULT;

// 押钞反馈
typedef struct _st_cash_into_box_{
	DWORD dwCashIntoAmount;

	WORD wRecyclerAIntoCount;
	WORD wRecyclerBIntoCount;
	WORD wRecyclerCIntoCount;
	WORD wRecyclerDIntoCount;

	WORD wCashBox_CNY1_IntoCount;
	WORD wCashBox_CNY5_IntoCount;
	WORD wCashBox_CNY10_IntoCount;
	WORD wCashBox_CNY20_IntoCount;
	WORD wCashBox_CNY50_IntoCount;
	WORD wCashBox_CNY100_IntoCount;

	_st_cash_into_box_(){
		dwCashIntoAmount = 0;

		wRecyclerAIntoCount = 0;
		wRecyclerBIntoCount = 0;
		wRecyclerCIntoCount = 0;
		wRecyclerDIntoCount = 0;

		wCashBox_CNY1_IntoCount = 0;
		wCashBox_CNY5_IntoCount = 0;
		wCashBox_CNY10_IntoCount = 0;
		wCashBox_CNY20_IntoCount = 0;
		wCashBox_CNY50_IntoCount = 0;
		wCashBox_CNY100_IntoCount = 0;
	}

}CASH_INTO_INFO;

