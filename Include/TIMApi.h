#ifndef _TIM_API_H_
#define _TIM_API_H_

/*****************************************************************************/
/*                                                                           */
/*  Include Files                                                            */
/*                                                                           */
/*****************************************************************************/
// apidef.h�ж����˽ӿڵĵ���/����
#include "Apidef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/*                                                                           */
/*  Definitions                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef __UINT
#define __UINT
	typedef unsigned int UINT;
#endif

#ifndef __BYTE
#define __BYTE
	typedef unsigned char BYTE;
#endif

#ifndef uchar
#define uchar	unsigned char	// 1 byte
#endif

	// provider ID
#define PROVIDER_ID_LEGATE          1
#define PROVIDER_ID_GRG				2

	// error type
#define TIM_ERR_TYPE_OK                 0
#define TIM_ERR_TYPE_WARNING            41420 // �������ַ
#define TIM_ERR_TYPE_FAILURE            41440 // �������ַ

	// fault error
#define TIM_FAULT_NORMAL                0
#define TIM_FAULT_CMD                   1
#define TIM_FAULT_HARDWARE              2
#define TIM_FAULT_CHANNEL               3
#define TIM_FAULT_CARDBOX               4

	// error code
#define TIM_ERR_CODE_NONE               0     // No error

	//FAULT_CMD
#define TIM_ERR_CODE_INVALID_CMD        41441 // ��Ч����(û�е��ô��ڴ�ʱҲ���ظô������)
#define TIM_ERR_CODE_INVALID_PARAM      41442 // ��Ч����
#define TIM_ERR_CODE_DEVICE_BUSY	    41484 // �豸æ
#define TIM_ERR_CODE_UNKNOWN			41490 // δ֪��
#define TIM_ERR_CODE_COMCONFIGURE       41493 // �������ô�
#define TIM_ERR_CODE_SEND_DATA_ERR		41494 // �յ�NAK,���������д�
#define TIM_ERR_CODE_RECIEVE_ERR        41495 // ���յ�����������
#define TIM_ERR_CODE_SEND_ENQ_ERR		41496 // ����ENQ����
#define TIM_ERR_CODE_CMD_SEND_ERR		41497 // ���������
#define TIM_ERR_CODE_RECIVE_ACK_TIMEOUT 41498 // �ȴ�ACK��ʱ
#define TIM_ERR_CODE_RECIEVE_TIMEOUT    41499 // �ȴ���Ӧ��ʱ
#define TIM_ERR_CODE_CMD_ERR			31000 // �������
#define TIM_ERR_CODE_SYSTEM_BUSY		31001 // ϵͳæ
#define TIM_ERR_CODE_CONTINUE_ERR		31002 // ������������쳣
#define TIM_ERR_CODE_PARAM_ERR			31003 // ��������

	//FAULT_HARDWARE
#define TIM_HARDWARE_ERR_ELE_ID1			41443	// ����ID1����
#define TIM_HARDWARE_ERR_ELE_ID2			41444	// ����ID2����
#define TIM_HARDWARE_ERR_ELE_ID3			41445	// ����ID3����
#define TIM_HARDWARE_ERR_READ_BOXID			41446	// ������IDʧ��
#define TIM_HARDWARE_ERR_WRITE_BOXID		41447	// д����IDʧ��
#define TIM_HARDWARE_ERR_READ_DEV_TYPE		41448	// ���豸�ͺ�ʧ��
#define TIM_HARDWARE_ERR_WRITE_DEV_TYPE		41449	// д�豸�ͺ�ʧ��
#define TIM_HARDWARE_ERR_READ_LOG			41450	// ��Logʧ��
#define TIM_HARDWARE_ERR_WRITE_LOG			41451	// дLogʧ��
#define TIM_HARDWARE_ERR_EEPROM_ERR			41452	// EEPROM����
#define TIM_HARDWARE_ERR_READ_FLASH_ERR		41474	// ��flashʧ��
#define TIM_HARDWARE_ERR_WRITE_FLASH_ERR	41475	// ����flashоƬ��
#define TIM_HARDWARE_ERR_NO_LOG				41476	// �޸�����Log
#define TIM_HARDWARE_ERR_TRANS_SENSER		41478	// ͨ������������
#define TIM_HARDWARE_ERR_MAIN_MOTOR			41479	// ������ת
#define TIM_HARDWARE_ERR_COMMUTATOR			41480	// ����������
#define TIM_HARDWARE_ERR_NO_RFID			41485	// �޷���⵽Ʊ���RFID��
#define TIM_HARDWARE_ERR_RFID_AUTH_ERR		41486	// Ʊ��RFID����֤ʧ��
#define TIM_HARDWARE_ERR_INVALID_RFID_PARAM	41487	// ��дƱ��RFID��������
#define TIM_HARDWARE_ERR_BOX_SN_ERR			41488	// ��дƱ��SN�Ŵ�
#define TIM_HARDWARE_ERR_ERR				31004	// Ӳ������
#define TIM_HARDWARE_ERR_RFID_ERR			31006	// RFID����
#define TIM_HARDWARE_ERR_SCRAPER1_UP		31007	// ��Ʊ��1û��ѹ��
#define TIM_HARDWARE_ERR_SCRAPER2_UP		31008	// ��Ʊ��2û��ѹ��
#define TIM_HARDWARE_ERR_SCRAP1_FAIL		31009	// ��Ʊ1ʧ��
#define TIM_HARDWARE_ERR_SCRAP2_FAIL		31010	// ��Ʊ2ʧ��
#define TIM_HARDWARE_ERR_BOX1_TRANS_ERR		31011	// Ʊ��1����ʧ��
#define TIM_HARDWARE_ERR_BOX2_TRANS_ERR		31012	// Ʊ��2����ʧ��
#define TIM_HARDWARE_ERR_INVALID_BOX_NOTOPEN	31015	// ��Ʊ���൵δ��
#define TIM_HARDWARE_ERR_ISSUE_MOTOR_JAM_CARD	31016	// ��Ʊ�����Ʊ
#define TIM_HARDWARE_ERR_RECOVERY_MOTOR_ERR	31017	// ���յ������

	//FAULT_CHANNEL
#define TIM_CANNEL_ERR_CARD_AT_EXIT_POSITION	41453	// �п��ڳ���λ�ö�����
#define TIM_CANNEL_ERR_CARD_ON_READING_AREA		41454	// �п��ڶ���λ�ö�����
#define TIM_CANNEL_ERR_CARD_IN_PASSAGEWAY		41457	// ������ͨ����
#define TIM_CANNEL_ERR_ENTRANCE_A_BLOCKING		41458	// Ħ��A���ͨ������
#define TIM_CANNEL_ERR_ENTRANCE_B_BLOCKING		41459	// Ħ��B���ͨ������
#define TIM_CANNEL_ERR_RECOVERY_BOX_FULL		41472	// ��������/�п��ڻ���λ�ö�����
#define TIM_CANNEL_ERR_INDUCTION_NO_CARD		41477	// ��Ӧ���޿�
#define TIM_CANNEL_ERR_READING_AREA_NO_CARD		41421	// ����λ���޿�����
#define TIM_CANNEL_ERR_PASSAGEWAY_HAVE_CARD		31034	// ͨ�����в�����Ʊ

	//FAULT_CARDBOX
#define TIM_ERR_CODE_CASEBOXA_ISSUE_FAIL			41455	// A����Ħ��ʧ��
#define TIM_ERR_CODE_CASEBOXA_HANDL_UNRISE			41460	// A�������δ����
#define TIM_ERR_CODE_CASEBOXB_ISSUE_FAIL			41456	// B����Ħ��ʧ��
#define TIM_ERR_CODE_CASEBOXB_HANDL_UNRISE			41461	// B�������δ����
#define TIM_ERR_CODE_CASEBOX_HANDL_UNRISE			41462	// ��������־�δ��
#define TIM_ERR_CODE_CASEBOXA_COVER					41463	// ������A��δ��
#define TIM_ERR_CODE_CASEBOXB_COVER					41464	// ������B��δ��
#define TIM_ERR_CODE_CASEBOX_COVER					41465	// ������Ǿ�δ��
#define TIM_ERR_CODE_CASEBOXA_EM					41466	// A�����
#define TIM_ERR_CODE_CASEBOXB_EM					41467	// B�����
#define TIM_ERR_CODE_CASEBOX_EM						41468	// ���������
#define TIM_ERR_CODE_CASEBOXA_NOT_EXIST				41469	// A���䲻����
#define TIM_ERR_CODE_CASEBOXB_NOT_EXIST				41470	// B���䲻����
#define TIM_ERR_CODE_CASEBOX_NOT_EXIST				41471	// �������������
#define TIM_ERR_CODE_RECOVERY_CASEBOXA_NOT_EXIST	41473	// �����䲻����
#define TIM_ERR_CODE_CASEBOXA_AE					41481	// A���佫��
#define TIM_ERR_CODE_CASEBOXB_AE					41482	// B���佫��
#define TIM_ERR_CODE_CASEBOX_AE						41483	// �����䶼����
#define TIM_ERR_CODE_INVALID_CASEBOX_UNINSTALL		31023	// ��Ʊ�䰲װ���
#define TIM_ERR_CODE_INVALID_CASEBOX_FULL			31025	// ��Ʊ�������
#define TIM_ERR_CODE_INVALID_CASEBOX_AE				31033	// ��Ʊ�佫�����

	// command parameter macro
#define CARD_RETURN_TO_REJECTION    1     // card return to rejection opening
#define CARD_RETURN_TO_FAREBOX      2     // card return to fare box

#define SELECT_ALLBOXES             0     // select all the boxes
#define SELECT_CASEBOXA             1     // select to use case box A
#define SELECT_CASEBOXB             2     // select to use case box B
#define SELECT_FAREBOX              3     // select to use fare box

#define CASEBOX_UNINSTALL           1     // uninstall case box
#define CASEBOX_INSTALL             2     // install case box

#define USE_RFID_S50_CARD           3     // use S50 RFID card
#define USE_RFID_UL_CARD            4     // use UL RFID card

#define WORK_MODE_NORMAL            0     // normal work mode
#define WORK_MODE_MAINTENANCE       1     // maintenance mode

#define BLOCKA                      1     // A block of RFID
#define BLOCKB                      2     // B block of RFID

#define TEST_STOP                   0     // stop current test
#define TEST_SENSORS                3     // sensor test
#define TEST_TICKSCRUB_MOTOR_BOXA   4     // ticket scrub motor of case box A test
#define TEST_TICKSCRUB_MOTOR_BOXB   5     // ticket scrub motor of case box B test
#define TEST_TRANSTICK_MOTOR_BOXA   6     // ticket transfer motor of case box A test
#define TEST_TRANSTICK_MOTOR_BOXB   7     // ticket transfer motor of case box B test
#define TEST_TRANSTICK_MOTOR        8     // ticket transfer motor test
#define TEST_CONT_TRANSCARD_BOXA    9     // continuously transfer ticket from case box A test
#define TEST_CONT_TRANSCARD_BOXB    10    // continuously transfer ticket from case box B test
#define TEST_CONT_TRANSCARD_BOXAB   11    // continuously transfer card from case box AB test
#define TEST_CONT_RETURNCARD_BOXA   12    // continuously return card from case box A test
#define TEST_CONT_RETURNCARD_BOXB   13    // continuously return card from case box B test
#define TEST_A_TRAY_UP              14    // boxA tray up test
#define TEST_A_TRAY_DOWN            15    // boxA tray down test
#define TEST_B_TRAY_UP              16    // boxB tray up test
#define TEST_B_TRAY_DOWN            17    // boxB tray down test
#define TEST_READ_RFID_BOXA         23    // case box A read RFID test
#define TEST_READ_RFID_BOXB         24    // case box B read RFID test
#define TEST_READ_RFID_FAREBOX      25    // fare box read RFID test
#define TEST_WRITE_RFID_BOXA        26    // case box A write RFID test
#define TEST_WRITE_RFID_BOXB        27    // case box B write RFID test
#define TEST_WRITE_RFID_FAREBOX     28    // fare box write RFID test
#define TEST_MOTOR_LIFE             101   // motor life test
#define TEST_CHANNEL_CLEAN          102   // channel clean test
#define TEST_MAX                    102

	//sensor ID

#define CODE_SENSOR0_ID 							31000	// Ʊ��2�൵�رռ��
#define CODE_SENSOR1_ID 							31001	// ��Ʊ�佫�����-
#define CODE_SENSOR2_ID 							31002	// ��Ʊ���൵�ر�
#define CODE_SENSOR3_ID 							31003	// Ʊ��һ����
#define CODE_SENSOR4_ID 							31004	// Ʊ�������
#define CODE_SENSOR5_ID 							31005	// Ʊ��1��Ʊ�ų���λ
#define CODE_SENSOR6_ID 							31006	// Ʊ��1��װ���
#define CODE_SENSOR7_ID 							31007	// Ʊ��2��װ���
#define CODE_SENSOR8_ID 							31008	// Ʊ��2��Ʊ�ų���λ
#define CODE_SENSOR9_ID								31009	// ��Ʊ�䰲װ���
#define CODE_SENSOR10_ID							31010	// Ʊ��1�ṩλ�ü��
#define CODE_SENSOR11_ID							31011	// Ʊ��1���޴�����
#define CODE_SENSOR12_ID							31012	// Ʊ��2���޴�����
#define CODE_SENSOR13_ID							31013	// Ʊ��2�ṩλ�ü��
#define CODE_SENSOR14_ID							31014	// Ʊ��1����צ������
#define CODE_SENSOR15_ID							31015	// Ʊ��2����צ������
#define CODE_SENSOR16_ID							31016	// Ʊ��2�ӽ�����
#define CODE_SENSOR17_ID							31017	// ��Ʊ������צ������
#define CODE_SENSOR18_ID							31018	// ȡ���ڳ�ȡ��⴫��
#define CODE_SENSOR19_ID							31019	// �ų�������⴫����
#define CODE_SENSOR20_ID							31020	// ��Ʊ���ս������
#define CODE_SENSOR21_ID							31021	// IC��R/Wֹͣλ��
#define CODE_SENSOR22_ID							31022	// Ʊ��1��ȡ���
#define CODE_SENSOR23_ID							31023	// Ʊ��2��ȡ���
#define CODE_SENSOR24_ID							31024	// Ʊ��1�ṩ��λ��
#define CODE_SENSOR25_ID 							31025	// RESERVED
#define CODE_SENSOR26_ID 							31026	// Ʊ��2�ṩ��λ��
#define CODE_SENSOR27_ID 							31027	// RESERVED
#define CODE_SENSOR28_ID 							31028	// ����൵λ�ü��
#define CODE_SENSOR29_ID 							31029	// RESERVED
#define CODE_SENSOR30_ID 							31030	// Ʊ��1�϶˼��
#define CODE_SENSOR31_ID 							31031	// Ʊ��2�϶˼��-
#define CODE_SENSOR32_ID 							31032	// Ʊ��1��Ʊ���޼��
#define CODE_SENSOR33_ID 							31033	// Ʊ��2��Ʊ���޼��
#define CODE_SENSOR34_ID							31034	// ��Ʊ�������-
#define CODE_SENSOR35_ID							31035	// Ʊ��1�൵�򿪼��
#define CODE_SENSOR36_ID							31036	// Ʊ��2�൵��
#define CODE_SENSOR37_ID							31037	// Ʊ��1�ӽ�����
#define CODE_SENSOR38_ID							31038	// ��Ʊ���൵��
#define CODE_SENSOR39_ID							31039	// Ʊ��1�൵�ر�
#define CODE_SENSOR40_ID							31040	// BOM��λ����
#define CODE_SENSOR41_ID							31041	// TVM��λ����
#define CODE_SENSOR42_ID							31042	// REVERSE1
#define CODE_SENSOR43_ID							31043	// REVERSE2
#define CODE_SENSOR44_ID							31044	// REVERSE3
#define CODE_SENSOR45_ID							31045	// REVERSE4
#define CODE_SENSOR46_ID							31046	// REVERSE5
#define CODE_SENSOR47_ID							31047	// REVERSE6
#define CODE_SENSOR48_ID							41001	// ����ID2	0:��ⲻ������ID2 1:��⵽����ID2�ڶԵ���ID2�������״̬�Ÿ��¡�
#define CODE_SENSOR49_ID							41002	// ����ID3	0:��ⲻ������ID3 1:��⵽����ID3�ڶԵ���ID3�������״̬�Ÿ��¡�
#define CODE_SENSOR50_ID							41003	// EEPROM״̬������	0: EEPROM���� 1:EEPROM�����ڶ�EEPROM�������״̬�Ÿ���
#define CODE_SENSOR51_ID							41004	// BCSA	����A����ͨ��������	0:����1:����
#define CODE_SENSOR52_ID							41005	// BCSB	����B����ͨ��������	0:����1:����
#define CODE_SENSOR53_ID							41006	// RWPS	����дλ�ô�����	0:����1:����
#define CODE_SENSOR54_ID							41007	// ECS	���ڼ���������	0:����1:����
#define CODE_SENSOR55_ID							41008	// BAAES	Ʊ��A���ռ�⴫����	0������ 1������
#define CODE_SENSOR56_ID							41009	// BAPS	Ʊ��A���ڴ�����	0������ 1��������
#define CODE_SENSOR57_ID							41010	// BAES	Ʊ��A�ռ�⴫����	0������ 1��Ʊ���
#define CODE_SENSOR58_ID							41011	// 	Ʊ��A������������	0������ 1������
#define CODE_SENSOR59_ID							41012	// 	Ʊ��A����ͨ��������	0������ 1������
#define CODE_SENSOR60_ID							41013	// 	Ʊ��A�ڿ鴫����	0������ 1������
#define CODE_SENSOR61_ID							41014	// BBAES	Ʊ��B���ռ�⴫����	0������ 1������
#define CODE_SENSOR62_ID							41015	// BBPS	Ʊ��B���ڴ�����	0������ 1��������
#define CODE_SENSOR63_ID							41016	// BBES	Ʊ��B�ռ�⴫����	0������ 1��Ʊ���
#define CODE_SENSOR64_ID							41017	// 	Ʊ��B������������	0������ 1������
#define CODE_SENSOR65_ID							41018	// 	Ʊ��B����ͨ��������	0������ 1������
#define CODE_SENSOR66_ID							41019	// 	Ʊ��B�ڿ鴫����	0������ 1������
#define CODE_SENSOR67_ID							41020	// TRPS	Ʊ���ڶ�д����λ������	0������ 1������
#define CODE_SENSOR68_ID							41021	// 	���ڼ���������	0������ 1������
#define CODE_SENSOR69_ID							41022	// TSCS	���Ʊ��C����������������	0������ 1������
#define CODE_SENSOR70_ID							41023	// 	ͨ��������������	0������ 1������
#define CODE_SENSOR71_ID							41024	// BCPS	����������ڴ�����	0������ 1��������
#define CODE_SENSOR72_ID							41025	// BCTPS	������C��Ʊ����������	0������ 1��Ʊ����
#define CODE_SENSOR73_ID							41026	// 	�Խ��������C��Ʊ�����м���������	0������ 1������
#define CODE_SENSOR74_ID							41027	// BACPS	���Ʊ��A�ϸǿ���״̬	0������ 1��δ�ϸ�
#define CODE_SENSOR75_ID							41028	// BBCPS	���Ʊ��B�ϸǿ���״̬	0������ 1��δ�ϸ�
#define CODE_SENSOR76_ID							41029	// BCCPS	���Ʊ��C�ϸǿ���״̬	0������ 1��δ�ϸ�
#define CODE_SENSOR77_ID							41030	// TIS	���Ʊ���Ƿ�ɹ�����
#define CODE_SENSOR78_ID							41031	// TAPS	���Ʊ����Ʊ��A�ڳ�λ��	0������ 1������
#define CODE_SENSOR79_ID							41032	// TABPS	���Ʊ����Ʊ��A��B֮���λ��	0������ 1������
#define CODE_SENSOR80_ID							41033	// TBPS	���Ʊ����Ʊ��B�ڳ�λ��	0������ 1������
#define CODE_SENSOR81_ID							41034	// BAHPS	���Ʊ��A�ֱ�λ��	0������ 1������
#define CODE_SENSOR82_ID							41035	// BBHPS	���Ʊ��B�ֱ�λ��	0������ 1������
#define CODE_SENSOR83_ID							41036	// WAP1S	����ڿ���Aλ��״̬	0������ 1������
#define CODE_SENSOR84_ID							41037	// WAP2S	����ڿ���Aλ��״̬	0������ 1������
#define CODE_SENSOR85_ID							41038	// WBP1S	����ڿ���Bλ��״̬	0������ 1������
#define CODE_SENSOR86_ID							41039	// WBP2S	����ڿ���Bλ��״̬	0������ 1������

	//sensors status
#define SENSOR_OK                   0
#define SENSOR_FAULT                1
#define SENSOR_NO_TRIGGER           3
#define SENSOR_TRIGGER              4
#define SENSOR_ABSENT               0xff

	// module type
#define MOD_TYPE                    0x06

	// sensors count
#define SENSOR_COUNT                30
#define SENSOR_PORT_ALL             256

	// case box status
#define CASEBOX_NORMAL              0
#define CASEBOX_ABOUT_EMPTY         1
#define CASEBOX_EMPTY               2

#define FAREBOX_NORMAL              0
#define FAREBOX_FULL                2

	// SN lenth
#define LENTH_SN                    20

#define UNINSTALLBOX                0x61
#define INSTALLBOX                  0x62


/** 
@brief      ��ȡƱ�������ṹ-����
*/
// API�ĵ��ж���AƱ��Ϊ1����ʵ����AƱ��Ϊ2
typedef enum _TH_CMD_CARD_OUT
{
	CMD_CARD_OUT_BOX_A = 1,    // A Ʊ��
	CMD_CARD_OUT_BOX_B = 2,    // B Ʊ��

}TH_CMD_CARD_OUT;

typedef enum _TH_CMD_CARD_RETRACT
{
	CMD_CARD_RETRACT_C = 3,		// 3: ��Ʊ��
}TH_CMD_CARD_RETRACT;

typedef enum _TH_BOX_TYPE
{
	TH_BOX_A = 1,			// AƱ��
	TH_BOX_B = 2,			// BƱ��
	TH_BOX_C = 3,			// CƱ��
}TH_BOX_TYPE;

/** 
@brief      ����ģ�鹤��ģʽ�����ṹ-����
*/
typedef enum _TH_CMD_SET_WORK_MODE
{
	CMD_WORK_MODE_NORMAL      = 0,    // 0������ģʽ
	CMD_WORK_MODE_MAINTENANCE = 1,    // 1��ά��ģʽ
}TH_CMD_SET_WORK_MODE;

/** 
@brief      ����ģ����Բ����ṹ-����
*/
typedef enum _TH_CMD_TEST_SETTING
{
	CMD_TEST_STOP                 = 0,     // ֹͣ��ǰ����
	CMD_TEST_SENSORS              = 3,     // ����������
	CMD_TEST_TICKSCRUB_MOTOR_BOXA = 5,     // A Ʊ���Ʊ������
	CMD_TEST_TICKSCRUB_MOTOR_BOXB = 4,     // B Ʊ���Ʊ������
	CMD_TEST_TRANSTICK_MOTOR_BOXA = 6,     // A Ʊ���Ʊ�����ԣ�������У�
	CMD_TEST_TRANSTICK_MOTOR_BOXB = 7,     // B Ʊ���Ʊ�����ԣ�������У�
	CMD_TEST_TRANSTICK_MOTOR      = 8,     // ��Ʊ������
	CMD_TEST_CONT_TRANSCARD_BOXB  = 9,     // A Ʊ��������Ʊ����
	CMD_TEST_CONT_TRANSCARD_BOXA  = 10,    // B Ʊ��������Ʊ����
	CMD_TEST_CONT_TRANSCARD_BOXAB = 11,    // 2 ��Ʊ��������Ʊ����
	CMD_TEST_CONT_RETURNCARD_BOXB = 12,    // A Ʊ��������������
	CMD_TEST_CONT_RETURNCARD_BOXA = 13,    // B Ʊ��������������
	CMD_TEST_A_TRAY_UP            = 14,    // A Ʊ������װ����������
	CMD_TEST_A_TRAY_DOWN          = 15,    // A Ʊ������װ���½�����
	CMD_TEST_B_TRAY_UP            = 16,    // B Ʊ������װ����������
	CMD_TEST_B_TRAY_DOWN          = 17,    // B Ʊ������װ���½�����
	CMD_TEST_READ_RFID_BOXB       = 23,    // A Ʊ�� RFID ������
	CMD_TEST_READ_RFID_BOXA       = 24,    // B Ʊ�� RFID ������
	CMD_TEST_READ_RFID_FAREBOX    = 25,    // ��Ʊ�� RFID �����ԣ�������У�
	CMD_TEST_WRITE_RFID_BOXB      = 26,    // A Ʊ�� RFID д����
	CMD_TEST_WRITE_RFID_BOXA      = 27,    // B Ʊ�� RFID д����
	CMD_TEST_WRITE_RFID_FAREBOX   = 28,    // ��Ʊ�� RFID д���ԣ�������У�
	CMD_TEST_MOTOR_LIFE           = 101,   // motor life test
	CMD_TEST_CHANNEL_CLEAN        = 102,   // ͨ���������
	CMD_TEST_MAX                  = 102,
}TH_CMD_TEST_SETTING;
/** 
@brief      ģ���ʼ�������ṹ-����
*/
typedef enum _TH_CMD_INIT_RESET
{
	CMD_INIT_REJECTION  = 1,    // 1������Ʊ���˳�
	CMD_INIT_FAREBOX    = 2,    // 2��������Ʊ��
}TH_CMD_INIT_RESET;
/************************************************************************/
/* Ʊ�������ʽ                                                         */
/************************************************************************/
typedef enum _TH_BOX_ACTION_TYPE
{
	TH_UNINSTALL = 1,			// ȥƱ��
	TH_INSTALL	 = 2,			// װƱ��
}TH_BOX_ACTION_TYPE;
#pragma pack(push,1)
//////////////////////////////////////////////////////////////////////////
///*Ϊҵ�����ýӿڴ����ṩ�����ݶ���
/** 
@brief        ͨѶ���������ṹ
@param     (i)UINT uiCommPort    �˿ں�
@param     (i)UINT uiBaudRate    ������
*/
typedef struct _th_cmd_comm_open
{
	UINT uiCommPort;    // �˿ں�
	UINT uiBaudRate;    // ������

	_th_cmd_comm_open()
	{
		uiCommPort = 0;
		uiBaudRate = 0;
	}
} TH_CMD_COMM_OPEN, *LPTH_CMD_COMM_OPEN;
/** 
@brief      ��ȡ�豸״̬�����ṹ DLL��Ӧ��
*/
typedef struct _th_rsp_get_dev_status
{
	// ����ģ���״̬��Ϣ
	bool bCardInReadArea;         // �Ƿ��п��ڶ�������TRUE:  �п���FALSE: �޿�
	char cCassetteAStatus;        // A Ʊ��״̬, 0:����, 1:����, 2:ȫ��
	char cCassetteBStatus;        // B Ʊ��״̬, 0:����, 1:����, 2:ȫ��
	char cRejectCassetteStatus;   // ��Ʊ��״̬, 0:����, 1:����, 2:��
	UINT uiCassetteACardNum;     // card number of recycle case A
	UINT uiCassetteBCardNum;     // card number of recycle case B
	UINT uiRejectCassetteNum;    // card number of reject case
	BYTE cRwAreaDetect;			//  ��д����⴫����
	bool bBoxAArrive;			//  Ʊ�� A ��λ��⴫����
	bool bBoxACoverArrive;		//  Ʊ�� A ���Ǽ�⴫����������Ʊ��δʹ�ã�
	bool bBoxANearlyEmpty;		//  Ʊ�� A ���մ�����
	bool bBoxAEmpty;			//  Ʊ�� A �մ�����
	bool bBoxBArrive;			//  Ʊ�� B ��λ��⴫����
	bool bBoxBCoverArrive;		//  Ʊ�� B ���Ǽ�⴫����������Ʊ��δʹ�ã�
	bool bBoxBNearlyEmpty;		//  Ʊ�� B ���մ�����
	bool bBoxBEmpty;			//  Ʊ�� B �մ�����
	bool bBoxCArrive;			//  ��Ʊ�䵽λ��⴫����
	BYTE bBoxCCoverArrive;		//  ��Ʊ�䶥�Ǽ�⴫����������Ʊ��δʹ�ã�
	BYTE bBoxCFull;				//  ��Ʊ������⴫����
	// ����״̬��Ϣ
	UINT uiType;                  // error type: 0-OK; 1-warning; 2-error
	UINT uiErrorType;             // fault type
	UINT uiErrorCode;             // error code
	_th_rsp_get_dev_status()
	{
		bCardInReadArea       = 0;
		cCassetteAStatus      = 0;
		cCassetteBStatus      = 0;
		cRejectCassetteStatus = 0;
		uiCassetteACardNum    = 0;
		uiCassetteBCardNum    = 0;
		uiRejectCassetteNum   = 0;
		cRwAreaDetect         = 0;
		bBoxAArrive           = false;
		bBoxACoverArrive      = false;
		bBoxANearlyEmpty      = false;
		bBoxAEmpty            = false;
		bBoxBArrive           = false;
		bBoxBCoverArrive      = false;
		bBoxBNearlyEmpty      = false;
		bBoxBEmpty            = false;
		bBoxCArrive           = false;
		bBoxCCoverArrive      = 0;
		bBoxCFull             = 0;
		uiType                = 0;
		uiErrorType           = 0;
		uiErrorCode           = 0;
	}
} TH_RSP_GET_DEV_STATUS, *LPTH_RSP_GET_DEV_STATUS;


/** 
@brief      ��ȡ�豸�����汾��Ϣ�����ṹ
@param      (o)char cFirmWareVersion[32]    ���ع̼��汾��
@param      (o)char cDriverVersion[32]    ���������汾��
*/
typedef struct _th_rsp_get_version
{
	char cFirmWareVersion[32];    // ���ع̼��汾��
	char cDriverVersion[32];      // ���������汾��

	_th_rsp_get_version()
	{
		memset(cFirmWareVersion, 0, sizeof(cFirmWareVersion));
		memset(cDriverVersion, 0, sizeof(cDriverVersion));
	}
} TH_RSP_GET_VERSION, *LPTH_RSP_GET_VERSION;


/** 
@brief      ��ȡ���кŲ����ṹ
@param      (o)char cModuleSN[20]    ģ������кţ�����20
*/
typedef struct _th_rsp_get_sn
{
	char cModuleSN[20];    // ���к�

	_th_rsp_get_sn()
	{
		memset(cModuleSN, 0, sizeof(cModuleSN));
	}
} TH_RSP_GET_SN, *LPTH_RSP_GET_SN;

// ����Ʊ���������ݽṹ
typedef struct _sc_cmd_moveBox_process
{
	BYTE             boxId;		   // Ʊ��ID 1��Ʊ��1�� 2��Ʊ��2��
	BYTE             actionType;   // �������� 1�������� 2���½�
	_sc_cmd_moveBox_process(){
		boxId  = 0;
		actionType  = 0;
	}
}TH_MOVE_BOX_CMD_INFO,*LPTH_MOVE_BOX_CMD_INFO;

// ����������ݽṹ
typedef struct _sc_motorTest_data
{
	BYTE Switch;					// ���Կ���    1������ʼ  2����ֹͣ
	BYTE testType;					// ��������    1����Ʊ��1 ���������ת
	// 2����Ʊ��1 ���������ת
	// 3����Ʊ��2 ���������ת
	// 4����Ʊ��2 ���������ת
	// 5����Ʊ��1 ��Ʊ�����ת
	// 6����Ʊ��1 ��Ʊ�����ת
	// 7����Ʊ��2 ��Ʊ�����ת
	// 8����Ʊ��2 ��Ʊ�����ת
	// 9�����ݴ�����ת
	// 10�����ݴ�����ת
	// 11���������������ת
	// 12���������������ת
	_sc_motorTest_data()
	{
		Switch = 0;
		testType = 0;
	}
} TH_MOTORTEST_DATA, *LPTH_MOTORTEST_DATA;
typedef struct _sc_replace_box_cmd_info
{
	TH_BOX_TYPE boxId;
	TH_BOX_ACTION_TYPE actionType;

}TH_REPLACE_BOX_CMD_INFO,*LPTH_REPLACE_BOX_CMD_INFO;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
///*Ϊ��������ýӿڴ����ṩ�����ݶ���

//////////////////////////////////////////////////////////////////////////
// ����״̬�ṹ��
typedef struct _tim_dev_return_
{
	UINT uiProviderId;	//ģ�鳧�ұ�ʶ����0x02�����0x04������0x06��ŷķ��0x05��
	UINT uiModuleType;	//ģ�����ͱ�ʶ 0x06��Ʊ����ģ��
	UINT uiType;		//�������ͣ�0-������1-���棻2-����
	UINT uiErrorType;	//�������� 0-������1-���������; 2-Ӳ������;3-ͨ���쳣����;4-Ʊ�����;5-����ģ�鿨Ʊ
	UINT uiErrorCode;	//������루�ɲ�ͬģ�鳧���ṩ��
	BYTE cReverse[128]; //Ԥ����Ϣ
}tTIMDevReturn;

// Ʊ��������Ϣ�ṹ��
typedef struct _tim_card_num_
{
	UINT uiCassetteACardNum;	// Ʊ��A ��Ƭ����
	UINT uiCassetteBCardNum;	// Ʊ��B ��Ƭ����
	UINT uiRejectCassetteNum;	// ��Ʊ�俨Ƭ����
}tTIMCardNum;


typedef struct _tim_sensor_type_
{
	UINT uiSensotID;		//������ID
	BYTE btSensorStatus;	// ������״̬(0:���� 1:����  3:δ�ڵ�  4���ڵ�  0xff�������ڸô�����)
}tTIMSensorType;			// ʵ��ʹ�÷�Χ��tSensorType[0]--- tSensorType[uiSensorCount-1]
// ģ��״̬�ṹ��
typedef struct _tim_module_status
{
	bool bCardInReadArea;		// �Ƿ��п��ڶ�������TRUE: �п���FALSE: �޿�
	char cCassetteAStatus;		// A Ʊ��״̬, 0:����, 1:����, 2:ȫ��, 3:δ��װ, 4:��װ����λ, 5:Ʊ�����
	char cCassetteBStatus;		// B Ʊ��״̬, 0:����, 1: ����, 2:ȫ��, 3:δ��װ, 4:��װ����λ, 5:Ʊ�����
	char cRejectCassetteStatus; // ��Ʊ��״̬, 0:����1:����, 2:��, 3:δ��װ, 4:��װ����λ, 5:Ʊ�����
	UINT uiSensorCount;			//����������
	tTIMSensorType tSensorType[256];			// ʵ��ʹ�÷�Χ��tSensorType[0]--- tSensorType[uiSensorCount-1]
	tTIMCardNum tCardNum;		// Ʊ��Ʊ������
	char cReserve[256];			// ������Ϣ	
} tTIMModuleStatus;

// �����Ϣ�ṹ��
typedef struct _tim_audit_card_num
{
	UINT wCassetteAAuditCardNum; // Ʊ��A�ۼƷ�����
	UINT wCassetteBAuditCardNum; // Ʊ��B�ۼƷ�����
	UINT wRejectCassetteAuditNum; // ��Ʊ�俨Ƭ����
} tTIMAuditCardNum;

//RFID��Ϣ�ṹ��
typedef struct _tim_rfid_info_
{
	unsigned char ucBoxID[4];                // Ʊ����
	unsigned char ucOperateID[3];            // ����ԱID
	unsigned char ucDeviceID[4];             // �豸ID
	unsigned char ucBoxStatus;               // Ʊ��λ��״̬
	unsigned char ucBoxOperateStatus;        // ������Ʊ��״̬
	unsigned char ucCardProviderID[4];        // Ʊ��������ID
	unsigned char ucCardType;               // Ʊ����������
	unsigned char ucTicketClass[2];            // ��Ʊ��Ʒ����
	unsigned char ucPreValueType;            // Ԥ��ֵ����
	unsigned char ucDerivedID;               // ������ƷID
	unsigned char ucCardNum[2];             // Ʊ������
	unsigned char ucStationCode[2];            // ��վ����
	unsigned char ucInstallPosit;             // ��װλ��
	unsigned char ucOperateTime[7];           // ������ʱ��
	unsigned char ucReserve[12];              // Ԥ���ֶ�
} tTIMRFIDInfo;

#pragma pack(pop)


/*****************************************************************************/
/*                                                                           */
/*  Function Declarations                                                    */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
// �����������DLL��ʽ�ṩģ������������Ҫʹ��CTHCommandsʵ�ֵ����ӿ�
// ������������ṩͨѶЭ�飬��CTHCommands�ӿ���Ҫ����ʵ��
/* # wang feng                                                               */
/*****************************************************************************/

//ͨѶ����
DLLIMP int TIM_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName);
//ģ���ʼ��
DLLIMP int TIM_Init(char cClearMode,UINT* uiRetractNum,tTIMModuleStatus* pModuleStatus,tTIMDevReturn* pDevStatus);
//ģ�鸴λ
DLLIMP int(TIM_Reset)(char cClearMode, UINT* uiRetractNum,tTIMModuleStatus* pModuleStatus,tTIMDevReturn* pDevStatus);
//��ȡ�豸�����汾��Ϣ
DLLIMP int(TIM_GetVersion)(char cFirmWareVersion[32], char cDriverVersion[32], tTIMDevReturn* pDevStatus);
//��ȡ�豸״̬
DLLIMP int(TIM_GetDevStatus)(tTIMModuleStatus* pModuleStatus,tTIMDevReturn* pDevStatus);
//��ȡƱ��
DLLIMP int(TIM_CardOut)(char cBoxNo, tTIMModuleStatus* pModuleStatus,UINT* uiTransportNum, tTIMDevReturn* pDevStatus);
//�ͳ�Ʊ��
DLLIMP int(TIM_SendCard)(tTIMModuleStatus* pModuleStatus,UINT* uiTransportNum, tTIMDevReturn* pDevStatus);
//����Ʊ��
DLLIMP int(TIM_RetractCard)(tTIMModuleStatus* pModuleStatus,UINT* uiRetractNum,tTIMDevReturn* pDevStatus);
//��ȡ����ۼ�ֵ
DLLIMP int(TIM_GetAuditData)(tTIMAuditCardNum* pAuditCardNum,tTIMDevReturn *pDevStatus);
//��ȡƱ��RFID��Ϣ
DLLIMP int(TIM_GetTicketBoxRFIDInfo)(char cTicketBoxNo, tTIMRFIDInfo* pRFIDInfo, tTIMDevReturn *pDevStatus);
//����Ʊ��RFID��Ϣ
DLLIMP int(TIM_SetTicketBoxRFIDInfo)(char cTicketBoxNo, tTIMRFIDInfo* pRFIDInfo, tTIMDevReturn *pDevStatus);
//��ȡƱ��������Ϣ
DLLIMP int(TIM_GetTicketBoxCountInfo)(char cTicketBoxNo,tTIMCardNum * pCardNum, tTIMDevReturn *pDevStatus);
//����Ʊ��������Ϣ
DLLIMP int(TIM_SetTicketBoxCountInfo)(char cTicketBoxNo,tTIMCardNum* pCardNum,tTIMDevReturn *pDevStatus);
//����Ʊ��
DLLIMP int(TIM_ReplaceTBox)(char cBoxNo, char cBoxMode, tTIMDevReturn* pDevStatus);
//��ȡ���к�
DLLIMP int(TIM_GetSN)(char* cModuleSN, tTIMDevReturn *pDevStatus);
//�ر�ͨѶ
DLLIMP int(TIM_CommClose)();
//����ģ�鹤��ģʽ
DLLIMP int(TIM_SetWorkMode)(char cMode, tTIMDevReturn* pDevStatus);
//ģ���������
DLLIMP int(TIM_TestSetting)(char cTestType, tTIMModuleStatus* pModuleStatus, tTIMDevReturn* pDevStatus);
/**
 * [TIM_TrayContral Ʊ�����̿���]
 * @param  p_iTrayType [�������]
 * @param  p_iCommand  [��������]
 		p_iTrayType     ������� 1:A����  2:B����  0xff ȫ��
		int		p_iCommand      �������� 1:�½�   2����
 * @param  pDevStatus  [����ִ�н��]
 * @return             [�ɹ�����0 ����ʧ��]
 */
DLLIMP int TIM_TrayContral(int p_iTrayType,int p_iCommand,tTIMDevReturn* pDevStatus);

#if defined (__cplusplus)
} // end of extern "C"
#endif




#endif