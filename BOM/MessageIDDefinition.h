#pragma once

#define SP_MODE_CHANGE 		  (WM_USER + 104)           ///< 模式变化
#define WM_XBUTTON                  WM_USER + 150           ///< 按下按钮所发出的消息
#define WM_XGROUP                   WM_USER + 151           ///< 按下按钮组所发出的消息
#define WM_EDIT_SETFOCUSE           WM_USER + 152           ///< 输入编辑框获得焦点的消息
#define WM_START_SERVICE			WM_USER + 153           ///< 启动service的消息
#define WM_STOP_SERVICE				WM_USER + 154           ///< 停止service的消息
#define WM_XBUTTON_CLICK            WM_USER + 155           ///< (XBUTTON内部)按下按钮所发出的消息
#define WM_EDIT_CHANGE				WM_USER	+ 161			///< 编辑框内容改变时所发出的消息。
#define WM_NOTIFY_DIALOG_UPDATE WM_USER+ 171   ///<前台服务通知窗体更新界面



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SM_FORCE_LOGOUT	(SP_USER + 0x1000)
#define SM_NET_STATUS (SP_USER + 0x1001)
#define SM_MAIN_SHUT_DOWN (SP_USER + 0x1002)
#define SM_MAIN_START_NTP_TIMER (SP_USER + 0x1003)
#define SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER (SP_USER + 0x1004)
#define SM_MAIN_NEW_WORK_MODE (SP_USER + 0x1005)
#define SM_MAIN_CLOSE_OPERATION (SP_USER + 0x1006)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
