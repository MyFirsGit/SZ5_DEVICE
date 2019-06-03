#pragma once

#include <vector>
#include "Util.h"
#include "CommonDefine.h"

using namespace std;

#define MAX_SEND_ITEMS_COUNT		10		 // ���Ͷ������Ԫ�ظ���
#define MAX_RECEIVE_ITEMS_COUNT	    1		 // ���ն������Ԫ�ظ���

//static bool isBHAbort = false;		// ֽ�ҽ���ģ���Ƿ���Ҫ��ֹ����

const int BOX_ID_LEN = 16;			// Ǯ��ID����Ϊ16���ֽ�
const int RES_MAX = 256;			// ֽ�ҽ���ģ��Ļظ�������256�ֽ�
const int ERR_MAX = 9;				// ״̬��Ϣ�д������������
const int REJ_MAX = 3;				// ״̬��Ϣ�о���ԭ���������
const int VALUENO_NONE = 0xFF;		// ��ֵ����û��Dxx
const long TIME_BH_ANSWER = 20000;	// ֽ�ҽ���ģ��Ӧ��20���ڶ���Ч��ϢӦ��

const int GET_ELEM_NONE = -1;		// ȡ״̬�ظ���ϢԪ�أ�δȡ��Ԫ��
const int GET_ELEM_ERR = -2;		// ȡ״̬�ظ���ϢԪ�أ�ȡ��Ԫ��ʧ��
const int GET_ELEM_END = -3;		// ȡ״̬�ظ���ϢԪ�أ��������

#define BH_MOP "MOP"							// ���ű�����
const static TCHAR* const BH_CNY = _T("CNY");	// ���������

// ����
#define CMD_BH_RESET			"TF"		// ��λ
#define CMD_BH_GET_STATUS		"ST"		// ״̬����
#define CMD_BH_STOP				"V0"		// �������ģʽ
#define CMD_BH_START			"V1"		// ����ʶ��ģʽ
#define CMD_BH_RETURN_ONE		"B0"		// ��һ�ţ�������->����
#define CMD_BH_COLLECT			"B1"		// ѹǮ�䣺��������+�ݴ�����->Ǯ��
#define CMD_BH_RETURN_ALL		"B2"		// ��ȫ������������+�ݴ�����->����
#define CMD_BH_HOLD				"B3"		// ѹ�ݴ棺������->�ݴ���
#define CMD_BH_TRANS_HOLD		"BD"		// ������->�ݴ���
#define CMD_BH_TRANS_COLLECT	"BE"		// ������->Ǯ��
#define CMD_BH_TRANS_RETURN		"BF"		// ������->����
#define CMD_BH_INFO				"IN"		// ������Ϣ
#define CMD_BH_FACEVALUE		"IND"		// ������ֵ
#define CMD_BH_TRANS_MOD		"D0,0"		// ָ����Ϣ������ϵͳ�����ʽ�ַ�
#define CMD_BH_ALLOWED_CNT		"D1,1"		// ָ����Ϣ����ʶ�����ֵ����
#define CMD_BH_ACTIVE_CNT		"D1,2"		// ָ����Ϣ���Ѽ������ֵ����
#define CMD_BH_ACTIVE_LIST		"D1,3"		// ָ����Ϣ���Ѽ������ֵ�б�
#define CMD_BH_ID_MONITOR		"D3,1"		// ָ����Ϣ��Ǯ��ID�ļ���
#define CMD_BH_MEM_DATA			"D4,0"		// ָ����Ϣ��Ǯ��洢��������
#define CMD_BH_MEM_ADDR			"D4,1"		// ָ����Ϣ��Ǯ��洢���ĵ�ַ
#define CMD_BH_BOX_ID			"D4,2"		// ָ����Ϣ��Ǯ��ID
#define CMD_BH_USER_ID			"D4,3"		// ָ����Ϣ��������ID
#define CMD_BH_VER_TRANS_TOP	"RS0"		// ��ȡ�汾���ϲ�����ϵͳ�汾
#define CMD_BH_VER_OS			"RS1"		// ��ȡ�汾��ʶ����OS�汾
#define CMD_BH_VER_ALGORITHM	"RS2"		// ��ȡ�汾��ʶ�����㷨�汾
#define CMD_BH_VER_TRANS_BOTTOM	"RS3"		// ��ȡ�汾���²�����ϵͳ�汾
#define CMD_BH_VER_ODS			"RS5"		// ��ȡ�汾��ODS�̼��汾
#define CMD_BH_READ				"R"			// ��ȡ
#define CMD_BH_WRITE			"W"			// д��
#define CMD_BH_SET_TIME			"MAW"		// ʱ������
#define CMD_BH_GET_TIME			"MAR"		// ��ȡʱ��

// �����Ʊ��Ŀ�ĵ�
typedef enum _bh_clean_dest {
	CLEAN_TO_REGISTER,
	CLEAN_TO_BOX,
	CLEAN_TO_RETURN_SHUTTER,
} BH_CLEAN_DEST;

// ������Ϣ(Axx)
typedef enum _bh_action {
	ACT_BH_IDLE				= 0x00,		// ����
	ACT_BH_INSERT_NG		= 0x01,		// ���������ʧ��
	ACT_BH_INSERT_OK		= 0x02,		// ����������ɹ�
	ACT_BH_COLLECT			= 0x03,		// Bx��������ѹǮ��ɹ�
	ACT_BH_RETURN_BUFF		= 0x04,		// Bx����������������һ��ֽ�ұ��˻�
	ACT_BH_HOLD				= 0x05,		// Bx��������ѹ�ݴ�ɹ�
	ACT_BH_RETURN_STACK		= 0x06,		// Bx���������ݴ�����һ�Ż����ֽ�ұ��˻�
	ACT_BH_CLEAR_TRANS_NG	= 0x10,		// Bx���������������ϵͳ��ֽ��ʧ��
	ACT_BH_CLEAR_TRANS_OK	= 0x11,		// Bx���������������ϵͳ��ֽ�ҳɹ�
	ACT_BH_CLEARING_TRANS	= 0x12,		// Bx���������������ϵͳ��ֽ����δ��ɣ�������
} BH_ACTION;

// ֽ�ҽ���ģ�鷵�صĴ����루Exx��
typedef enum _bh_error {
	ERR_BH_IN_TRANS			= 0xE00,	// ����ϵͳ����ֽ��
	ERR_BH_RETURN_ALL		= 0xE01,	// Ѻ������
	ERR_BH_RETURN_ONE		= 0xE02,	// �˳�����
	ERR_BH_INSERT_JAM		= 0xE03,	// �˳�ʧ��
	ERR_BH_JAM				= 0xE04,	// �ݴ泮Ʊ����Ͳʱʧ�ܣ����������⿨��
	ERR_BH_COLLECT			= 0xE05,	// ѹǮ��ʧ��
	ERR_BH_RETURN_SHUTTER	= 0xE0F,	// �˳�������ֽ��				�����棩
	ERR_BH_INSERT_SHUTTER	= 0xE10,	// ����������ֽ�ң��޷��˳�
	ERR_BH_ALIGN			= 0xE20,	// �޷������Ͳ
	ERR_BH_NO_RECOGNIZER	= 0xE30,	// ��������ֽ��ʶ����
	ERR_BH_RECOGNIZING		= 0xE33,	// ֽ��ʶ������������			�����棩
	ERR_BH_FRAME			= 0xE50,	// ��Ǯ���ͨ�ų�ʱ
	ERR_BH_BOX				= 0xE52,	// Ǯ���쳣����ֽ�Ҵ�Ǯ�����˳���
	ERR_BH_NO_BOX			= 0xE54,	// δװ��Ǯ���Ǯ��δ����
	ERR_BH_FULL				= 0xE55,	// Ǯ������
	ERR_BH_NEARLY_FULL		= 0xE56,	// Ǯ�佫��						�����棩
	ERR_BH_HARDWARE			= 0xE80,	// ��ָӲ������
} BH_ERROR;

// ����ֽ�ҵ�ԭ��(Rxx)
typedef enum _bh_reject {
	REJECT_BH_NO_RESULT		= 0x01,		// ��ʶ����
	REJECT_BH_MULTI			= 0x22,		// �س���ֽ��̫��
	REJECT_BH_NO_VALUE		= 0x30,		// δ����ֽ����ֵ
	REJECT_BH_NO_PERMISSION	= 0x31,		// ����������
	REJECT_BH_COUNTERFEIT	= 0x42,		// α��
	REJECT_BH_UNKNOW_ERR	= 0x60,		// δ֪����
} BH_REJECT;

// ģʽ��Ϣ(Sxx)
typedef enum _bh_mode {
	MOD_BH_STANDBY_NO_BOX	= 0x00,		// ����ģʽ����Ǯ��
	MOD_BH_VALIDATE_NO_BOX	= 0x01,		// ʶ��ģʽ����Ǯ��
	MOD_BH_STANDBY			= 0x10,		// ����ģʽ����Ǯ��
	MOD_BH_VALIDATE			= 0x11,		// ʶ��ģʽ����Ǯ��
} BH_MODE;

// ����ʱִ����ɵĽ׶Σ�����ʧ��ʱ��λ����
// ԭ�򣺷��ش������Χ�ϴ��ڵ��ö��ԭ�Ӳ���ʱ���޷�׼ȷ��λ�ĸ����������˴���
typedef enum _bh_done_phase {
	DONE_PHASE_READY			= 0x0000, // ��ʼ״̬
	DONE_PHASE_ACT_START_ACCEPT	= 0x0001, // ������ɣ���ʼ����ֽ��
	DONE_PHASE_ACT_HOLD			= 0x0002, // ������ɣ�ѹ�ݴ�
	DONE_PHASE_ACT_COLLECT		= 0x0004, // ������ɣ�ѹǮ��
	DONE_PHASE_ACT_RETURN_ONE	= 0x0008, // ������ɣ��˻��嵥��
	DONE_PHASE_ACT_RETURN_ALL	= 0x0010, // ������ɣ��˻����ݴ�
	DONE_PHASE_ACT_CLEAN		= 0x0020, // ������ɣ������Ʊ
	DONE_PHASE_ACT_GET_STATUS	= 0x0040, // ������ɣ���ȡ״̬
	DONE_PHASE_ACT_GET_FACE		= 0x0080, // ������ɣ���ȡ��ֵ
	DONE_PHASE_ACT_ABORT		= 0x0100, // ������ɣ���ֹ��ѯ
	DONE_PHASE_ACTION			= 0x0200, // ״̬���أ���Ҫ�ж�Action״̬��ʹ�ô˱�־
	DONE_PHASE_SUCCESS			= 0x0400, // ȫ�����
} BH_DONE_PHASE;

// ״̬�ظ���Ϣ
typedef struct _bh_sts {
	BH_ACTION			action;			// ���һ������
	BYTE				valueNo[2];		// ֽ����ֵ���
	vector<BH_ERROR>	error;			// �����룬�޴���ʱvectorΪ��
	int					cnt;			// �ݴ�����ֽ������
	vector<BH_REJECT>	reject;			// ���ճ�Ʊ��ԭ���޾���ʱvectorΪ��
	BH_MODE				mode;			// ����/ʶ��ģʽ

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

	// ����״̬�ظ���Ϣ
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
 @brief		����״̬��Ϣ

 @param		(i)const BYTE *lpStatus		״̬��Ϣ

 @retval	BYTE  0 : ����  >0 : ֽ�ҽ���ģ�鷵�صĴ�����  <0 : �����쳣

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
inline int BH_STS::Deserialize(const BYTE* lpStatus)
{
	// -----------------------------------------------
	// ֽ�ҽ���ģ�鷵�ص�״̬��Ϣ��ʽ���£�
	// S11,N01,A00,D03,P02,E00
	// �ڴ˰�һ����ĸ��ͷ��2�����ֵ���ʽ��Ϊһ��Ԫ��
	// ����Sxx,Nxx,AxxԪ�ر�������ֻ��һ��
	//     DxxԪ�ز�����1��������û��
	//     ExxԪ�ز�����9��������û��
	//     RxxԪ�ز�����3��������û��
	// ����Ԫ���ڴ˲����
	// -----------------------------------------------
	LPSTR junk = NULL;
	const BYTE* lpData = lpStatus;
	while (*lpData)
	{
		// ����ǰ׺
		BYTE prefix = *lpData;

		// ��������
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

		// ����ǰ׺+����+����
		lpData += 3;
		if (*lpData == ',') ++lpData;
	}

	return 0;
}

// ֽ����ֵ��Ϣ
typedef struct _bh_facevalue {
	BankNoteAndCoinType_t	faceValue;		// ֽ����ֵ

	_bh_facevalue()
	{
		faceValue = VALUE_UNKNOWN;
	}

	// ����״̬�ظ���Ϣ
	int Deserialize(const BYTE*);

} BH_FACEVALUE;

//ֽ����ֵ���ձ�
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

//������������б� (����ҵ���Ŀǰ�漰�����ձ�ĵط��϶࣬�����޸ģ������¶����ڴ�)
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
 @brief		����ֽ����ֵ��Ϣ

 @param		(i)const BYTE *lpInfo	ֽ����ֵ��Ϣ

 @retval	BYTE  0 : ����  ��0 : �����쳣

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
inline int BH_FACEVALUE::Deserialize(const BYTE* lpInfo)
{
	// -----------------------------------------------
	// ֽ�ҽ���ģ�鷵�ص�ֽ����ֵ��Ϣ��ʽ���£�
	// MOP 1,000 B
	// ��ͷ��3����ĸ��ʾ�������ƣ�ISO4127��׼��
	// ������һ���ո��ַ�����������ֵ
	// ���б�Ҫ������һ���ո��ַ������ٸ���һ����ʾ��ͬ���͵��ַ���
	// -----------------------------------------------

	CStringA INDxxInfo(lpInfo);

	// ���Ϸ���
	if ("" == INDxxInfo || " " == INDxxInfo || "?" == INDxxInfo || "-" == INDxxInfo){
		faceValue = VALUE_UNKNOWN;
		return -1;
	}

	// ����Ƿ���ű�,����ȥ��MOP
	CStringA sName = INDxxInfo.Mid(0, 3);
	if (BH_CNY != sName){
		faceValue = VALUE_UNKNOWN;
		return -1;
	}
	INDxxInfo = INDxxInfo.Right(INDxxInfo.GetLength() - 4);

	// ����Ƿ���ڸ�����ĸ��������ȥ��
	int pos = INDxxInfo.Find(' ');
	if (-1 != pos){
		INDxxInfo = INDxxInfo.Left(pos);
	}

	// ��ֵ�ַ���תΪ��ֵ��
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

// ״̬��Ϣ
typedef struct _bh_status {
	BH_STS					bh_sts;			// ֽ�ҽ���ģ��״̬
	BH_FACEVALUE			bh_faceValue;	// ʶ�����ֵ
} BH_STATUS;


// Ǯ��ID
typedef struct _bh_box_id {
	BYTE user_id[BOX_ID_LEN];	// �˿�ID
	BYTE manu_id[BOX_ID_LEN];	// ������ID
	_bh_box_id()
	{
		memset(user_id, 0, sizeof(user_id));
		memset(manu_id, 0, sizeof(user_id));
	}
} BH_BOX_ID;

// �汾��Ϣ
typedef struct _bh_version {
	BYTE trans_top[RES_MAX];		// �ϲ�����ϵͳ�汾
	BYTE os[RES_MAX];				// ʶ����OS�汾
	BYTE algorithm[RES_MAX];		// ʶ�����㷨�汾
	BYTE trans_bottom[RES_MAX];		// �²�����ϵͳ�汾
	BYTE ods[RES_MAX];				// ODS�̼��汾

	_bh_version()
	{
		memset(trans_top, 0, sizeof(trans_top));
		memset(os, 0, sizeof(os));
		memset(algorithm, 0, sizeof(algorithm));
		memset(trans_bottom, 0, sizeof(trans_bottom));
		memset(ods, 0, sizeof(ods));
	}

	// �����汾�ظ���Ϣ
	int Deserialize(const BYTE *, BYTE *);

} BH_VERSION;

//////////////////////////////////////////////////////////////////////////
/**
 @brief		�����汾��Ϣ

 @param		(i)const BYTE *lpInfo	�汾��Ϣ

 @retval	BYTE  0 : ����  ��0 : �����쳣

 @exception	��
*/
//////////////////////////////////////////////////////////////////////////
inline int BH_VERSION::Deserialize(const BYTE* lpInfo, BYTE* verInfo)
{
	CStringA slpInfo(lpInfo);

	// ���Ϸ���
	if (NULL == verInfo || NULL == lpInfo){
		return -1;
	}

	// ���汾�Ϸ���
	if ("" == slpInfo || " " == slpInfo || "?" == slpInfo || "-" == slpInfo){
		return -1;
	}

	memcpy(verInfo, (BYTE *)slpInfo.GetBuffer(slpInfo.GetLength()), slpInfo.GetLength());
	return 0;
}
