#pragma once

/************************************************************************/
/*  Complete message			                                        */
/************************************************************************/
#define SP_CONVERT (WM_USER+100)											///前置转换消息
#define SP_REQUEST_COMPLETE   (WM_USER + 101)           ///< 命令完成
#define SP_REQUEST_ONPROGRESS (WM_USER + 102)           ///< 命令进度
#define SP_EXCEPTION 		  (WM_USER + 103)           ///< 异常发生
#define SP_SENDMESSAGE (WM_USER+104)				
#define ST_SET_SERVICE (WM_USER + 110)
#define ST_SERVICE_TIMER (WM_USER + 111)
#define ST_START_TIMER (WM_USER + 112)
#define ST_STOP_TIMER (WM_USER + 113)

#define  SP_USER (WM_USER + 0x0100)

/************************************************************************/
/*  OnModeChange() WPARM value	                                        */
/************************************************************************/
#define SP_MODE_CPS_CONNECTION                 (1)      ///< CPS连接状态
#define SP_MODE_DELIVER_DATA                   (2)      ///< 有无未送数据
#define SP_MODE_OPERATION                      (3)      ///< 业务模式
#define SP_MODE_SYSTEM                         (4)      ///< 系统模式
#define SP_MODE_CPS_BIND                       (5)      ///< CPS Bind状态
#define SP_PROGRAM_UPDATE                      (6)      ///< 程序更新

/************************************************************************/
/*  OnComplete() input value	                                        */
/************************************************************************/
#define SP_SUCCESS                             (0)
#define SP_ERR_HARDWARE_ERROR                  (0x6001)
#define SP_ERR_INTERNAL_ERROR                  (0x6002)
#define SP_ERR_INVALID_HWND                    (0x6003)
#define SP_ERR_INVALID_REQ_ID                  (0x6004)
#define SP_ERR_INVALID_RESULT                  (0x6005)
#define SP_ERR_INVALID_TIMER                   (0x6006)
#define SP_ERR_INVALID_COMMANDID               (0x6007)
#define SP_ERR_TIMEOUT                         (0x6008)
#define SP_ERR_INVALID_INPUT_DATA              (0x6009)
#define SP_ERR_SOFTWARE_ERROR                  (0x600A)
#define SP_ERR_CONNECTION_LOST                 (0x600B)
#define SP_ERR_INVALID_OPERATION               (0x600C)

/************************************************************************/
/* DoCommand() Execute Mode value                                       */
/************************************************************************/
//#define SP_EXECMODE_QUEUE						(1)
//#define SP_EXECMODE_IMMEDIATE					(2)
typedef enum _sp_execmode{
	SP_EXECMODE_SYNC = 0,
	SP_EXECMODE_QUEUE = 1,
	SP_EXECMODE_IMMEDIATE = 2
}SP_EXECMODE;

#define SP_MAX_TIME_SERVICE						(30) // seconds unit.
#define SP_DEFAULT_IDLE_TIMER					(30) // seconds unit.

// Token式ES设备消息
#define WM_ES1_BASE						(WM_USER + 1500)	// Token式ES基消息
#define WM_ES1_CARD_ARRIVED 			(WM_ES1_BASE + 100)	// 卡到达位置
#define WM_ES1_MAGAZINE_BE_EMPTY		(WM_ES1_BASE + 20)	// 进票箱将无卡
#define WM_ES1_MAGAZINE_EMPTY			(WM_ES1_BASE + 30)	// 进票箱无卡
#define WM_ES1_MAGAZINE_NOT_INSTALL		(WM_ES1_BASE + 40)	// 票箱未安装
#define WM_ES1_DEVICE_ERROR				(WM_ES1_BASE + 50)	// 设备故障