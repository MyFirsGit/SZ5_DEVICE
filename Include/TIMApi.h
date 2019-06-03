#ifndef _TIM_API_H_
#define _TIM_API_H_

/*****************************************************************************/
/*                                                                           */
/*  Include Files                                                            */
/*                                                                           */
/*****************************************************************************/
// apidef.h中定义了接口的导入/导出
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
#define TIM_ERR_TYPE_WARNING            41420 // 警告码基址
#define TIM_ERR_TYPE_FAILURE            41440 // 错误码基址

	// fault error
#define TIM_FAULT_NORMAL                0
#define TIM_FAULT_CMD                   1
#define TIM_FAULT_HARDWARE              2
#define TIM_FAULT_CHANNEL               3
#define TIM_FAULT_CARDBOX               4

	// error code
#define TIM_ERR_CODE_NONE               0     // No error

	//FAULT_CMD
#define TIM_ERR_CODE_INVALID_CMD        41441 // 无效命令(没有调用串口打开时也返回该错误代码)
#define TIM_ERR_CODE_INVALID_PARAM      41442 // 无效参数
#define TIM_ERR_CODE_DEVICE_BUSY	    41484 // 设备忙
#define TIM_ERR_CODE_UNKNOWN			41490 // 未知错
#define TIM_ERR_CODE_COMCONFIGURE       41493 // 串口配置错
#define TIM_ERR_CODE_SEND_DATA_ERR		41494 // 收到NAK,发送数据有错
#define TIM_ERR_CODE_RECIEVE_ERR        41495 // 接收到的数据有误
#define TIM_ERR_CODE_SEND_ENQ_ERR		41496 // 发送ENQ出错
#define TIM_ERR_CODE_CMD_SEND_ERR		41497 // 发送命令错
#define TIM_ERR_CODE_RECIVE_ACK_TIMEOUT 41498 // 等待ACK超时
#define TIM_ERR_CODE_RECIEVE_TIMEOUT    41499 // 等待响应超时
#define TIM_ERR_CODE_CMD_ERR			31000 // 命令错误
#define TIM_ERR_CODE_SYSTEM_BUSY		31001 // 系统忙
#define TIM_ERR_CODE_CONTINUE_ERR		31002 // 继续处理可能异常
#define TIM_ERR_CODE_PARAM_ERR			31003 // 参数错误

	//FAULT_HARDWARE
#define TIM_HARDWARE_ERR_ELE_ID1			41443	// 电子ID1故障
#define TIM_HARDWARE_ERR_ELE_ID2			41444	// 电子ID2故障
#define TIM_HARDWARE_ERR_ELE_ID3			41445	// 电子ID3故障
#define TIM_HARDWARE_ERR_READ_BOXID			41446	// 读卡箱ID失败
#define TIM_HARDWARE_ERR_WRITE_BOXID		41447	// 写卡箱ID失败
#define TIM_HARDWARE_ERR_READ_DEV_TYPE		41448	// 读设备型号失败
#define TIM_HARDWARE_ERR_WRITE_DEV_TYPE		41449	// 写设备型号失败
#define TIM_HARDWARE_ERR_READ_LOG			41450	// 读Log失败
#define TIM_HARDWARE_ERR_WRITE_LOG			41451	// 写Log失败
#define TIM_HARDWARE_ERR_EEPROM_ERR			41452	// EEPROM故障
#define TIM_HARDWARE_ERR_READ_FLASH_ERR		41474	// 读flash失败
#define TIM_HARDWARE_ERR_WRITE_FLASH_ERR	41475	// 擦除flash芯片错
#define TIM_HARDWARE_ERR_NO_LOG				41476	// 无该日期Log
#define TIM_HARDWARE_ERR_TRANS_SENSER		41478	// 通道传感器故障
#define TIM_HARDWARE_ERR_MAIN_MOTOR			41479	// 主马达堵转
#define TIM_HARDWARE_ERR_COMMUTATOR			41480	// 换向器故障
#define TIM_HARDWARE_ERR_NO_RFID			41485	// 无法检测到票箱的RFID卡
#define TIM_HARDWARE_ERR_RFID_AUTH_ERR		41486	// 票箱RFID卡认证失败
#define TIM_HARDWARE_ERR_INVALID_RFID_PARAM	41487	// 读写票箱RFID卡参数错
#define TIM_HARDWARE_ERR_BOX_SN_ERR			41488	// 读写票箱SN号错
#define TIM_HARDWARE_ERR_ERR				31004	// 硬件故障
#define TIM_HARDWARE_ERR_RFID_ERR			31006	// RFID故障
#define TIM_HARDWARE_ERR_SCRAPER1_UP		31007	// 刮票器1没有压下
#define TIM_HARDWARE_ERR_SCRAPER2_UP		31008	// 刮票器2没有压下
#define TIM_HARDWARE_ERR_SCRAP1_FAIL		31009	// 刮票1失败
#define TIM_HARDWARE_ERR_SCRAP2_FAIL		31010	// 刮票2失败
#define TIM_HARDWARE_ERR_BOX1_TRANS_ERR		31011	// 票箱1传输失败
#define TIM_HARDWARE_ERR_BOX2_TRANS_ERR		31012	// 票箱2传输失败
#define TIM_HARDWARE_ERR_INVALID_BOX_NOTOPEN	31015	// 废票箱舌档未打开
#define TIM_HARDWARE_ERR_ISSUE_MOTOR_JAM_CARD	31016	// 出票电机卡票
#define TIM_HARDWARE_ERR_RECOVERY_MOTOR_ERR	31017	// 回收电机故障

	//FAULT_CHANNEL
#define TIM_CANNEL_ERR_CARD_AT_EXIT_POSITION	41453	// 有卡在出口位置堵塞错
#define TIM_CANNEL_ERR_CARD_ON_READING_AREA		41454	// 有卡在读卡位置堵塞错
#define TIM_CANNEL_ERR_CARD_IN_PASSAGEWAY		41457	// 卡堵在通道中
#define TIM_CANNEL_ERR_ENTRANCE_A_BLOCKING		41458	// 摩卡A入口通道堵塞
#define TIM_CANNEL_ERR_ENTRANCE_B_BLOCKING		41459	// 摩卡B入口通道堵塞
#define TIM_CANNEL_ERR_RECOVERY_BOX_FULL		41472	// 回收箱满/有卡在回收位置堵塞错
#define TIM_CANNEL_ERR_INDUCTION_NO_CARD		41477	// 感应区无卡
#define TIM_CANNEL_ERR_READING_AREA_NO_CARD		41421	// 读卡位置无卡存在
#define TIM_CANNEL_ERR_PASSAGEWAY_HAVE_CARD		31034	// 通道上有残留车票

	//FAULT_CARDBOX
#define TIM_ERR_CODE_CASEBOXA_ISSUE_FAIL			41455	// A卡箱摩卡失败
#define TIM_ERR_CODE_CASEBOXA_HANDL_UNRISE			41460	// A卡箱把手未升起
#define TIM_ERR_CODE_CASEBOXB_ISSUE_FAIL			41456	// B卡箱摩卡失败
#define TIM_ERR_CODE_CASEBOXB_HANDL_UNRISE			41461	// B卡箱把手未升起
#define TIM_ERR_CODE_CASEBOX_HANDL_UNRISE			41462	// 两卡箱把手均未打开
#define TIM_ERR_CODE_CASEBOXA_COVER					41463	// 发卡箱A盖未打开
#define TIM_ERR_CODE_CASEBOXB_COVER					41464	// 发卡箱B盖未打开
#define TIM_ERR_CODE_CASEBOX_COVER					41465	// 两卡箱盖均未打开
#define TIM_ERR_CODE_CASEBOXA_EM					41466	// A卡箱空
#define TIM_ERR_CODE_CASEBOXB_EM					41467	// B卡箱空
#define TIM_ERR_CODE_CASEBOX_EM						41468	// 两卡箱均空
#define TIM_ERR_CODE_CASEBOXA_NOT_EXIST				41469	// A卡箱不存在
#define TIM_ERR_CODE_CASEBOXB_NOT_EXIST				41470	// B卡箱不存在
#define TIM_ERR_CODE_CASEBOX_NOT_EXIST				41471	// 两卡箱均不存在
#define TIM_ERR_CODE_RECOVERY_CASEBOXA_NOT_EXIST	41473	// 回收箱不存在
#define TIM_ERR_CODE_CASEBOXA_AE					41481	// A卡箱将空
#define TIM_ERR_CODE_CASEBOXB_AE					41482	// B卡箱将空
#define TIM_ERR_CODE_CASEBOX_AE						41483	// 两卡箱都将空
#define TIM_ERR_CODE_INVALID_CASEBOX_UNINSTALL		31023	// 废票箱安装检测
#define TIM_ERR_CODE_INVALID_CASEBOX_FULL			31025	// 废票箱满检测
#define TIM_ERR_CODE_INVALID_CASEBOX_AE				31033	// 废票箱将满检测

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

#define CODE_SENSOR0_ID 							31000	// 票箱2舌档关闭检测
#define CODE_SENSOR1_ID 							31001	// 废票箱将满检测-
#define CODE_SENSOR2_ID 							31002	// 废票箱舌档关闭
#define CODE_SENSOR3_ID 							31003	// 票箱一将空
#define CODE_SENSOR4_ID 							31004	// 票箱二将空
#define CODE_SENSOR5_ID 							31005	// 票箱1车票排出定位
#define CODE_SENSOR6_ID 							31006	// 票箱1安装检测
#define CODE_SENSOR7_ID 							31007	// 票箱2安装检测
#define CODE_SENSOR8_ID 							31008	// 票箱2车票排出定位
#define CODE_SENSOR9_ID								31009	// 废票箱安装检测
#define CODE_SENSOR10_ID							31010	// 票箱1提供位置检测
#define CODE_SENSOR11_ID							31011	// 票箱1下限传感器
#define CODE_SENSOR12_ID							31012	// 票箱2下限传感器
#define CODE_SENSOR13_ID							31013	// 票箱2提供位置检测
#define CODE_SENSOR14_ID							31014	// 票箱1锁定爪传感器
#define CODE_SENSOR15_ID							31015	// 票箱2锁定爪传感器
#define CODE_SENSOR16_ID							31016	// 票箱2接近下限
#define CODE_SENSOR17_ID							31017	// 废票箱锁定爪传感器
#define CODE_SENSOR18_ID							31018	// 取出口抽取检测传感
#define CODE_SENSOR19_ID							31019	// 排出结束检测传感器
#define CODE_SENSOR20_ID							31020	// 废票回收结束检测
#define CODE_SENSOR21_ID							31021	// IC卡R/W停止位置
#define CODE_SENSOR22_ID							31022	// 票箱1提取完成
#define CODE_SENSOR23_ID							31023	// 票箱2提取完成
#define CODE_SENSOR24_ID							31024	// 票箱1提供部位置
#define CODE_SENSOR25_ID 							31025	// RESERVED
#define CODE_SENSOR26_ID 							31026	// 票箱2提供部位置
#define CODE_SENSOR27_ID 							31027	// RESERVED
#define CODE_SENSOR28_ID 							31028	// 清分舌档位置检测
#define CODE_SENSOR29_ID 							31029	// RESERVED
#define CODE_SENSOR30_ID 							31030	// 票箱1上端检测
#define CODE_SENSOR31_ID 							31031	// 票箱2上端检测-
#define CODE_SENSOR32_ID 							31032	// 票箱1车票有无检测
#define CODE_SENSOR33_ID 							31033	// 票箱2车票有无检测
#define CODE_SENSOR34_ID							31034	// 废票箱满检测-
#define CODE_SENSOR35_ID							31035	// 票箱1舌档打开检测
#define CODE_SENSOR36_ID							31036	// 票箱2舌档打开
#define CODE_SENSOR37_ID							31037	// 票箱1接近下限
#define CODE_SENSOR38_ID							31038	// 废票箱舌档打开
#define CODE_SENSOR39_ID							31039	// 票箱1舌档关闭
#define CODE_SENSOR40_ID							31040	// BOM到位开关
#define CODE_SENSOR41_ID							31041	// TVM到位开关
#define CODE_SENSOR42_ID							31042	// REVERSE1
#define CODE_SENSOR43_ID							31043	// REVERSE2
#define CODE_SENSOR44_ID							31044	// REVERSE3
#define CODE_SENSOR45_ID							31045	// REVERSE4
#define CODE_SENSOR46_ID							31046	// REVERSE5
#define CODE_SENSOR47_ID							31047	// REVERSE6
#define CODE_SENSOR48_ID							41001	// 电子ID2	0:检测不到电子ID2 1:检测到电子ID2在对电子ID2操作后此状态才更新。
#define CODE_SENSOR49_ID							41002	// 电子ID3	0:检测不到电子ID3 1:检测到电子ID3在对电子ID3操作后此状态才更新。
#define CODE_SENSOR50_ID							41003	// EEPROM状态传感器	0: EEPROM正常 1:EEPROM故障在对EEPROM操作后此状态才更新
#define CODE_SENSOR51_ID							41004	// BCSA	卡箱A进入通道传感器	0:正常1:故障
#define CODE_SENSOR52_ID							41005	// BCSB	卡箱B进入通道传感器	0:正常1:故障
#define CODE_SENSOR53_ID							41006	// RWPS	卡读写位置传感器	0:正常1:故障
#define CODE_SENSOR54_ID							41007	// ECS	出口计数传感器	0:正常1:故障
#define CODE_SENSOR55_ID							41008	// BAAES	票箱A将空检测传感器	0：正常 1：将空
#define CODE_SENSOR56_ID							41009	// BAPS	票箱A存在传感器	0：正常 1：不存在
#define CODE_SENSOR57_ID							41010	// BAES	票箱A空检测传感器	0：正常 1：票箱空
#define CODE_SENSOR58_ID							41011	// 	票箱A编码器传感器	0：正常 1：故障
#define CODE_SENSOR59_ID							41012	// 	票箱A进入通道传感器	0：正常 1：故障
#define CODE_SENSOR60_ID							41013	// 	票箱A黑块传感器	0：正常 1：故障
#define CODE_SENSOR61_ID							41014	// BBAES	票箱B将空检测传感器	0：正常 1：将空
#define CODE_SENSOR62_ID							41015	// BBPS	票箱B存在传感器	0：正常 1：不存在
#define CODE_SENSOR63_ID							41016	// BBES	票箱B空检测传感器	0：正常 1：票箱空
#define CODE_SENSOR64_ID							41017	// 	票箱B编码器传感器	0：正常 1：故障
#define CODE_SENSOR65_ID							41018	// 	票箱B进入通道传感器	0：正常 1：故障
#define CODE_SENSOR66_ID							41019	// 	票箱B黑块传感器	0：正常 1：故障
#define CODE_SENSOR67_ID							41020	// TRPS	票卡在读写区定位传感器	0：正常 1：故障
#define CODE_SENSOR68_ID							41021	// 	出口计数传感器	0：正常 1：故障
#define CODE_SENSOR69_ID							41022	// TSCS	检测票箱C换向器动作传感器	0：正常 1：故障
#define CODE_SENSOR70_ID							41023	// 	通道编码器传感器	0：正常 1：故障
#define CODE_SENSOR71_ID							41024	// BCPS	回收箱箱存在传感器	0：正常 1：不存在
#define CODE_SENSOR72_ID							41025	// BCTPS	回收箱C内票卡满传感器	0：正常 1：票箱满
#define CODE_SENSOR73_ID							41026	// 	对进入回收箱C的票卡进行计数传感器	0：正常 1：故障
#define CODE_SENSOR74_ID							41027	// BACPS	检测票箱A上盖开闭状态	0：正常 1：未上盖
#define CODE_SENSOR75_ID							41028	// BBCPS	检测票箱B上盖开闭状态	0：正常 1：未上盖
#define CODE_SENSOR76_ID							41029	// BCCPS	检测票箱C上盖开闭状态	0：正常 1：未上盖
#define CODE_SENSOR77_ID							41030	// TIS	检测票卡是否成功发售
#define CODE_SENSOR78_ID							41031	// TAPS	检测票卡从票箱A挖出位置	0：正常 1：故障
#define CODE_SENSOR79_ID							41032	// TABPS	检测票卡在票箱A、B之间的位置	0：正常 1：故障
#define CODE_SENSOR80_ID							41033	// TBPS	检测票卡从票箱B挖出位置	0：正常 1：故障
#define CODE_SENSOR81_ID							41034	// BAHPS	检测票箱A手柄位置	0：正常 1：故障
#define CODE_SENSOR82_ID							41035	// BBHPS	检测票箱B手柄位置	0：正常 1：故障
#define CODE_SENSOR83_ID							41036	// WAP1S	检测挖卡轮A位置状态	0：正常 1：故障
#define CODE_SENSOR84_ID							41037	// WAP2S	检测挖卡轮A位置状态	0：正常 1：故障
#define CODE_SENSOR85_ID							41038	// WBP1S	检测挖卡轮B位置状态	0：正常 1：故障
#define CODE_SENSOR86_ID							41039	// WBP2S	检测挖卡轮B位置状态	0：正常 1：故障

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
@brief      获取票卡参数结构-命令
*/
// API文档中定义A票箱为1，但实际上A票箱为2
typedef enum _TH_CMD_CARD_OUT
{
	CMD_CARD_OUT_BOX_A = 1,    // A 票箱
	CMD_CARD_OUT_BOX_B = 2,    // B 票箱

}TH_CMD_CARD_OUT;

typedef enum _TH_CMD_CARD_RETRACT
{
	CMD_CARD_RETRACT_C = 3,		// 3: 废票箱
}TH_CMD_CARD_RETRACT;

typedef enum _TH_BOX_TYPE
{
	TH_BOX_A = 1,			// A票箱
	TH_BOX_B = 2,			// B票箱
	TH_BOX_C = 3,			// C票箱
}TH_BOX_TYPE;

/** 
@brief      设置模块工作模式参数结构-命令
*/
typedef enum _TH_CMD_SET_WORK_MODE
{
	CMD_WORK_MODE_NORMAL      = 0,    // 0：正常模式
	CMD_WORK_MODE_MAINTENANCE = 1,    // 1：维护模式
}TH_CMD_SET_WORK_MODE;

/** 
@brief      设置模块测试参数结构-命令
*/
typedef enum _TH_CMD_TEST_SETTING
{
	CMD_TEST_STOP                 = 0,     // 停止当前测试
	CMD_TEST_SENSORS              = 3,     // 传感器测试
	CMD_TEST_TICKSCRUB_MOTOR_BOXA = 5,     // A 票箱刮票马达测试
	CMD_TEST_TICKSCRUB_MOTOR_BOXB = 4,     // B 票箱刮票马达测试
	CMD_TEST_TRANSTICK_MOTOR_BOXA = 6,     // A 票箱出票马达测试（如果含有）
	CMD_TEST_TRANSTICK_MOTOR_BOXB = 7,     // B 票箱出票马达测试（如果含有）
	CMD_TEST_TRANSTICK_MOTOR      = 8,     // 出票马达测试
	CMD_TEST_CONT_TRANSCARD_BOXB  = 9,     // A 票箱连续出票测试
	CMD_TEST_CONT_TRANSCARD_BOXA  = 10,    // B 票箱连续出票测试
	CMD_TEST_CONT_TRANSCARD_BOXAB = 11,    // 2 个票箱连续出票测试
	CMD_TEST_CONT_RETURNCARD_BOXB = 12,    // A 票箱连续出卡测试
	CMD_TEST_CONT_RETURNCARD_BOXA = 13,    // B 票箱连续出卡测试
	CMD_TEST_A_TRAY_UP            = 14,    // A 票箱升降装置上升测试
	CMD_TEST_A_TRAY_DOWN          = 15,    // A 票箱升降装置下降测试
	CMD_TEST_B_TRAY_UP            = 16,    // B 票箱升降装置上升测试
	CMD_TEST_B_TRAY_DOWN          = 17,    // B 票箱升降装置下降测试
	CMD_TEST_READ_RFID_BOXB       = 23,    // A 票箱 RFID 读测试
	CMD_TEST_READ_RFID_BOXA       = 24,    // B 票箱 RFID 读测试
	CMD_TEST_READ_RFID_FAREBOX    = 25,    // 废票箱 RFID 读测试（如果含有）
	CMD_TEST_WRITE_RFID_BOXB      = 26,    // A 票箱 RFID 写测试
	CMD_TEST_WRITE_RFID_BOXA      = 27,    // B 票箱 RFID 写测试
	CMD_TEST_WRITE_RFID_FAREBOX   = 28,    // 废票箱 RFID 写测试（如果含有）
	CMD_TEST_MOTOR_LIFE           = 101,   // motor life test
	CMD_TEST_CHANNEL_CLEAN        = 102,   // 通道清理测试
	CMD_TEST_MAX                  = 102,
}TH_CMD_TEST_SETTING;
/** 
@brief      模块初始化参数结构-命令
*/
typedef enum _TH_CMD_INIT_RESET
{
	CMD_INIT_REJECTION  = 1,    // 1、从退票口退出
	CMD_INIT_FAREBOX    = 2,    // 2、清理到废票箱
}TH_CMD_INIT_RESET;
/************************************************************************/
/* 票箱操作方式                                                         */
/************************************************************************/
typedef enum _TH_BOX_ACTION_TYPE
{
	TH_UNINSTALL = 1,			// 去票箱
	TH_INSTALL	 = 2,			// 装票箱
}TH_BOX_ACTION_TYPE;
#pragma pack(push,1)
//////////////////////////////////////////////////////////////////////////
///*为业务层调用接口传参提供的数据定义
/** 
@brief        通讯建立参数结构
@param     (i)UINT uiCommPort    端口号
@param     (i)UINT uiBaudRate    波特率
*/
typedef struct _th_cmd_comm_open
{
	UINT uiCommPort;    // 端口号
	UINT uiBaudRate;    // 波特率

	_th_cmd_comm_open()
	{
		uiCommPort = 0;
		uiBaudRate = 0;
	}
} TH_CMD_COMM_OPEN, *LPTH_CMD_COMM_OPEN;
/** 
@brief      读取设备状态参数结构 DLL层应用
*/
typedef struct _th_rsp_get_dev_status
{
	// 返回模块的状态信息
	bool bCardInReadArea;         // 是否有卡在读卡区，TRUE:  有卡，FALSE: 无卡
	char cCassetteAStatus;        // A 票箱状态, 0:正常, 1:将空, 2:全空
	char cCassetteBStatus;        // B 票箱状态, 0:正常, 1:将空, 2:全空
	char cRejectCassetteStatus;   // 废票箱状态, 0:正常, 1:将满, 2:满
	UINT uiCassetteACardNum;     // card number of recycle case A
	UINT uiCassetteBCardNum;     // card number of recycle case B
	UINT uiRejectCassetteNum;    // card number of reject case
	BYTE cRwAreaDetect;			//  读写区检测传感器
	bool bBoxAArrive;			//  票箱 A 到位检测传感器
	bool bBoxACoverArrive;		//  票箱 A 顶盖检测传感器（塑料票箱未使用）
	bool bBoxANearlyEmpty;		//  票箱 A 将空传感器
	bool bBoxAEmpty;			//  票箱 A 空传感器
	bool bBoxBArrive;			//  票箱 B 到位检测传感器
	bool bBoxBCoverArrive;		//  票箱 B 顶盖检测传感器（塑料票箱未使用）
	bool bBoxBNearlyEmpty;		//  票箱 B 将空传感器
	bool bBoxBEmpty;			//  票箱 B 空传感器
	bool bBoxCArrive;			//  废票箱到位检测传感器
	BYTE bBoxCCoverArrive;		//  废票箱顶盖检测传感器（塑料票箱未使用）
	BYTE bBoxCFull;				//  废票箱满检测传感器
	// 返回状态信息
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
@brief      读取设备驱动版本信息参数结构
@param      (o)char cFirmWareVersion[32]    返回固件版本号
@param      (o)char cDriverVersion[32]    返回驱动版本号
*/
typedef struct _th_rsp_get_version
{
	char cFirmWareVersion[32];    // 返回固件版本号
	char cDriverVersion[32];      // 返回驱动版本号

	_th_rsp_get_version()
	{
		memset(cFirmWareVersion, 0, sizeof(cFirmWareVersion));
		memset(cDriverVersion, 0, sizeof(cDriverVersion));
	}
} TH_RSP_GET_VERSION, *LPTH_RSP_GET_VERSION;


/** 
@brief      读取序列号参数结构
@param      (o)char cModuleSN[20]    模块的序列号，长度20
*/
typedef struct _th_rsp_get_sn
{
	char cModuleSN[20];    // 序列号

	_th_rsp_get_sn()
	{
		memset(cModuleSN, 0, sizeof(cModuleSN));
	}
} TH_RSP_GET_SN, *LPTH_RSP_GET_SN;

// 升降票箱命令数据结构
typedef struct _sc_cmd_moveBox_process
{
	BYTE             boxId;		   // 票箱ID 1：票箱1； 2：票箱2；
	BYTE             actionType;   // 动作类型 1：上升； 2：下降
	_sc_cmd_moveBox_process(){
		boxId  = 0;
		actionType  = 0;
	}
}TH_MOVE_BOX_CMD_INFO,*LPTH_MOVE_BOX_CMD_INFO;

// 电机测试数据结构
typedef struct _sc_motorTest_data
{
	BYTE Switch;					// 测试开关    1——开始  2——停止
	BYTE testType;					// 测试类型    1——票箱1 升降电机正转
	// 2——票箱1 升降电机反转
	// 3——票箱2 升降电机正转
	// 4——票箱2 升降电机反转
	// 5——票箱1 刮票电机正转
	// 6——票箱1 刮票电机反转
	// 7——票箱2 刮票电机正转
	// 8——票箱2 刮票电机反转
	// 9——暂存电机正转
	// 10——暂存电机反转
	// 11——读卡区电机正转
	// 12——读卡区电机反转
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
///*为驱动层调用接口传参提供的数据定义

//////////////////////////////////////////////////////////////////////////
// 返回状态结构体
typedef struct _tim_dev_return_
{
	UINT uiProviderId;	//模块厂家标识三星0x02、广电0x04、中软0x06、欧姆龙0x05；
	UINT uiModuleType;	//模块类型标识 0x06车票发售模块
	UINT uiType;		//错误类型：0-正常；1-警告；2-故障
	UINT uiErrorType;	//故障类型 0-正常；1-命令类错误; 2-硬件故障;3-通道异常故障;4-票箱故障;5-回收模块卡票
	UINT uiErrorCode;	//错误代码（由不同模块厂家提供）
	BYTE cReverse[128]; //预留信息
}tTIMDevReturn;

// 票箱数据信息结构体
typedef struct _tim_card_num_
{
	UINT uiCassetteACardNum;	// 票箱A 卡片数量
	UINT uiCassetteBCardNum;	// 票箱B 卡片数量
	UINT uiRejectCassetteNum;	// 废票箱卡片数量
}tTIMCardNum;


typedef struct _tim_sensor_type_
{
	UINT uiSensotID;		//传感器ID
	BYTE btSensorStatus;	// 传感器状态(0:正常 1:故障  3:未遮挡  4：遮挡  0xff：不存在该传感器)
}tTIMSensorType;			// 实际使用范围从tSensorType[0]--- tSensorType[uiSensorCount-1]
// 模块状态结构体
typedef struct _tim_module_status
{
	bool bCardInReadArea;		// 是否有卡在读卡区，TRUE: 有卡，FALSE: 无卡
	char cCassetteAStatus;		// A 票箱状态, 0:正常, 1:将空, 2:全空, 3:未安装, 4:安装不到位, 5:票箱故障
	char cCassetteBStatus;		// B 票箱状态, 0:正常, 1: 将空, 2:全空, 3:未安装, 4:安装不到位, 5:票箱故障
	char cRejectCassetteStatus; // 废票箱状态, 0:正常1:将满, 2:满, 3:未安装, 4:安装不到位, 5:票箱故障
	UINT uiSensorCount;			//传感器数量
	tTIMSensorType tSensorType[256];			// 实际使用范围从tSensorType[0]--- tSensorType[uiSensorCount-1]
	tTIMCardNum tCardNum;		// 票箱票卡数量
	char cReserve[256];			// 保留信息	
} tTIMModuleStatus;

// 审计信息结构体
typedef struct _tim_audit_card_num
{
	UINT wCassetteAAuditCardNum; // 票箱A累计发卡数
	UINT wCassetteBAuditCardNum; // 票箱B累计发卡数
	UINT wRejectCassetteAuditNum; // 废票箱卡片数量
} tTIMAuditCardNum;

//RFID信息结构体
typedef struct _tim_rfid_info_
{
	unsigned char ucBoxID[4];                // 票箱编号
	unsigned char ucOperateID[3];            // 操作员ID
	unsigned char ucDeviceID[4];             // 设备ID
	unsigned char ucBoxStatus;               // 票箱位置状态
	unsigned char ucBoxOperateStatus;        // 操作后票箱状态
	unsigned char ucCardProviderID[4];        // 票卡发行商ID
	unsigned char ucCardType;               // 票卡物理类型
	unsigned char ucTicketClass[2];            // 车票产品种类
	unsigned char ucPreValueType;            // 预赋值属性
	unsigned char ucDerivedID;               // 衍生产品ID
	unsigned char ucCardNum[2];             // 票卡数量
	unsigned char ucStationCode[2];            // 车站编码
	unsigned char ucInstallPosit;             // 安装位置
	unsigned char ucOperateTime[7];           // 最后操作时间
	unsigned char ucReserve[12];              // 预留字段
} tTIMRFIDInfo;

#pragma pack(pop)


/*****************************************************************************/
/*                                                                           */
/*  Function Declarations                                                    */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
// 如果第三方以DLL方式提供模块驱动，则需要使用CTHCommands实现导出接口
// 如果第三方以提供通讯协议，则CTHCommands接口需要另外实现
/* # wang feng                                                               */
/*****************************************************************************/

//通讯建立
DLLIMP int TIM_CommOpen(UINT uiCommPort, UINT uiBaudRate, CString path, CString fileName);
//模块初始化
DLLIMP int TIM_Init(char cClearMode,UINT* uiRetractNum,tTIMModuleStatus* pModuleStatus,tTIMDevReturn* pDevStatus);
//模块复位
DLLIMP int(TIM_Reset)(char cClearMode, UINT* uiRetractNum,tTIMModuleStatus* pModuleStatus,tTIMDevReturn* pDevStatus);
//读取设备驱动版本信息
DLLIMP int(TIM_GetVersion)(char cFirmWareVersion[32], char cDriverVersion[32], tTIMDevReturn* pDevStatus);
//读取设备状态
DLLIMP int(TIM_GetDevStatus)(tTIMModuleStatus* pModuleStatus,tTIMDevReturn* pDevStatus);
//获取票卡
DLLIMP int(TIM_CardOut)(char cBoxNo, tTIMModuleStatus* pModuleStatus,UINT* uiTransportNum, tTIMDevReturn* pDevStatus);
//送出票卡
DLLIMP int(TIM_SendCard)(tTIMModuleStatus* pModuleStatus,UINT* uiTransportNum, tTIMDevReturn* pDevStatus);
//回收票卡
DLLIMP int(TIM_RetractCard)(tTIMModuleStatus* pModuleStatus,UINT* uiRetractNum,tTIMDevReturn* pDevStatus);
//读取审计累计值
DLLIMP int(TIM_GetAuditData)(tTIMAuditCardNum* pAuditCardNum,tTIMDevReturn *pDevStatus);
//读取票箱RFID信息
DLLIMP int(TIM_GetTicketBoxRFIDInfo)(char cTicketBoxNo, tTIMRFIDInfo* pRFIDInfo, tTIMDevReturn *pDevStatus);
//设置票箱RFID信息
DLLIMP int(TIM_SetTicketBoxRFIDInfo)(char cTicketBoxNo, tTIMRFIDInfo* pRFIDInfo, tTIMDevReturn *pDevStatus);
//读取票箱数量信息
DLLIMP int(TIM_GetTicketBoxCountInfo)(char cTicketBoxNo,tTIMCardNum * pCardNum, tTIMDevReturn *pDevStatus);
//设置票箱数量信息
DLLIMP int(TIM_SetTicketBoxCountInfo)(char cTicketBoxNo,tTIMCardNum* pCardNum,tTIMDevReturn *pDevStatus);
//更换票箱
DLLIMP int(TIM_ReplaceTBox)(char cBoxNo, char cBoxMode, tTIMDevReturn* pDevStatus);
//读取序列号
DLLIMP int(TIM_GetSN)(char* cModuleSN, tTIMDevReturn *pDevStatus);
//关闭通讯
DLLIMP int(TIM_CommClose)();
//设置模块工作模式
DLLIMP int(TIM_SetWorkMode)(char cMode, tTIMDevReturn* pDevStatus);
//模块测试设置
DLLIMP int(TIM_TestSetting)(char cTestType, tTIMModuleStatus* pModuleStatus, tTIMDevReturn* pDevStatus);
/**
 * [TIM_TrayContral 票箱托盘控制]
 * @param  p_iTrayType [托盘序号]
 * @param  p_iCommand  [动作参数]
 		p_iTrayType     托盘序号 1:A卡箱  2:B卡箱  0xff 全部
		int		p_iCommand      动作参数 1:下降   2上升
 * @param  pDevStatus  [命令执行结果]
 * @return             [成功返回0 其余失败]
 */
DLLIMP int TIM_TrayContral(int p_iTrayType,int p_iCommand,tTIMDevReturn* pDevStatus);

#if defined (__cplusplus)
} // end of extern "C"
#endif




#endif