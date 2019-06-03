/*
DATE: 2016-11-15
AUTHOR: lichao
*/
#pragma once
#include "CommonDefine.h"
//#include "RWException.h"

/* 
���ļ�����ؽṹ������ο���BRH�ײ�ӿ����ݽṹ���塣 
*/


// BRH��ȡ״̬������ṹ
typedef T_XfsCdrStatus BRH_RSP_GET_STATUS, *LPBRH_RSP_GET_STATUS;

// BRH��ȡ��ϸ״̬������ṹ
typedef T_SystemStatusOverview BRH_RSP_GET_SYSTEM_STATUS, *LPBRH_RSP_GET_SYSTEM_STATUS;

// BRH��ȡ����������ṹ
typedef T_XfsCapabilities BRH_RSP_GET_CAPABLILITIES, *LPBRH_RSP_GET_CAPABLILITIES;

// BRH������������ṹ
typedef T_XfsCapabilities BRH_CMD_SET_CAPABLILITIES, *LPBRH_CMD_SET_CAPABLILITIES;

// BRH����ʱ������ṹ
typedef T_TransactionStatus BRH_CMD_GET_TRANSACTIONSTATUS, *LPBRH_CMD_GET_TRANSACTIONSTATUS;

// BRH������������ṹ
typedef T_XfsDispenseRequest BRH_CMD_DISPENSE_REQUEST, *LPBRH_CMD_DISPENSE_REQUEST;

// BRH��ȡ�ֽ�Ԫ��Ϣ����ṹ
typedef T_XfsCashUnit BRH_RSP_QUERY_CASHUNIT, *LPBRH_RSP_QUERY_CASHUNIT;

// BRH�����ֽ�Ԫ��Ϣ����ṹ
typedef T_XfsCashUnit BRH_CMD_UPDATE_CASHUNIT, *LPBRH_CMD_UPDATE_CASHUNIT;

// BRH��ȡ��ֵ����ṹ
typedef T_DenominationList BRH_RSP_QUERY_DENOMINATIONS, *LPBRH_RSP_QUERY_DENOMINATIONS;

// BRH������ֵ����ṹ
typedef T_DenominationList BRH_CMD_UPDATE_DENOMINATIONS, *LPBRH_CMD_UPDATE_DENOMINATIONS;


// ״̬�ظ���Ϣ
typedef struct _brh_sts {
	//BYTE				errorCode;		// 1��	��ǰ���ϴ���					
	//BYTE				warning[2];		// 2��	������� �޾���Ϊ00
	//BH_PROC_NUMBER		processNumber;	// 1��	��ǰֽ�Ҵ������
	//int					validBnCount;	// 1��	ʮ�������ַ� ��ʾ�ݴ�������Чֽ�ҵ�����
	//BYTE				cleanErorMode;	// 1��	�������ģʽ A �Զ� S �û�ָ��
	//BYTE				effectiveMode;	// 1��	��1��Ϊδʹ�ã�ǰ��Чģʽ��
	//BH_WORK_DEADLINE	workDeadLine;	// 1��	��������������ʱ=0��1��2��3��4��1���ַ���
	//bool				bnaSvcLogo;		// 1��	BNA5 NT�����־��ѡ�����0��Ϊ�񣬡�1��Ϊ�ǣ���1���ַ�

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

	//// ����״̬�ظ���Ϣ
	//int Deserialize(const BYTE*);

	//// �ж�ֽ����Ч��
	//bool IsValidFaceValue()
	//{
	//	return true;
	//}

	//// ��������루�ַ�������Ϊ�쳣����
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

// ״̬��Ϣ
typedef struct _brh_status {
	BRH_STS					bh_sts;			// ֽ�ҽ���ģ��״̬
	BankNoteAndCoinType_t	bh_faceValue;	// ʶ�����ֵ
} BRH_STATUS;

// ����ָ��ķ���
typedef struct brh_cmd_feedback{
	//BH_STS						state;		// ��ǰֽ������״̬
	//BH_COMMOND_TYPE				cmdType;	// ����ָ������
	int							nCount;		// ����ֽ������
	BankNoteAndCoinType_t		faceValue;	// ����ֽ�ҵ����(ֻ�н��ճɹ���ʱ��Ż������)

	brh_cmd_feedback()
	{
		//cmdType		= BH_UNKNOW;
		nCount		= 0;
		faceValue	= VALUE_UNKNOWN;
	}

	// �ж�ֽ����Ч��
	bool IsValidFaceValue()
	{
		if (faceValue != VALUE_UNKNOWN){
			return true;
		}
		return false;
	}

}BRH_CMD_FEEDBACK;


// MEI ֽ�һ�����ID
typedef struct _brh_box_id
{
	BYTE serialID[6];	// ���
	BYTE modelID[3];		// �ͺ�
	_brh_box_id()
	{
		memset(serialID, 0, 6);
		memset(modelID, 0, 3);
	}
}BRH_BOX_ID;

// �����Ʊ��Ŀ�ĵ�
typedef enum _brh_clean_dest {
	CLEAN_TO_REGISTER,
	CLEAN_TO_BOX,
	CLEAN_TO_RETURN_SHUTTER,
} BRH_CLEAN_DEST;

// ����ID���壬�ϲ�ֱ��Ӧ��
typedef enum _em_bnr_module_id_{
	BNR_MODULE_CASH_BOX = CASHBOX_CLASS,
	BNR_MODULE_CHANGE_BOX = LOADER_CLASS + 1,
	BNR_RECYCLER_BOX_A = RECYCLER_CLASS + 3,
	BNR_RECYCLER_BOX_B,
	BNR_RECYCLER_BOX_C,
	BNR_RECYCLER_BOX_D,
}BNR_MODULE_ID;

// �����߼�ID����
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

// ����/��ҷ��ؽṹ��
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

// Ѻ������
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

