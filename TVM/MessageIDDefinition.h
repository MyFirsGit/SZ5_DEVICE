#pragma once


#define WM_XBUTTON                  WM_USER + 150           ///< 按下按钮所发出的消息
#define WM_XGROUP                   WM_USER + 151           ///< 按下按钮组所发出的消息
#define WM_EDIT_SETFOCUSE           WM_USER + 152           ///< 输入编辑框获得焦点的消息
#define WM_START_SERVICE			WM_USER + 153           ///< 启动service的消息
#define WM_STOP_SERVICE				WM_USER + 154           ///< 停止service的消息
#define WM_XBUTTON_CLICK            WM_USER + 155           ///< (XBUTTON内部)按下按钮所发出的消息
#define WM_XLINEGROUP               WM_USER + 156           ///< 按下线路图按钮组所发出的消息
#define WM_EDIT_CHANGE				WM_USER	+ 161			///< 编辑框内容改变时所发出的消息。
#define WM_NOTIFY_DIALOG_UPDATE		WM_USER + 171			///< 前台服务通知窗体更新界面
// 针对武汉业务流程，增加
#define WM_NOTIFY_ISSUING			WM_USER + 172			///< 通知前台窗口，发售车票或充值
#define WM_START_ISSUING			WM_USER + 173			///< 开始发售车票
#define WM_NOTIFY_ACCEPTED			WM_USER + 174			///< 通知前台窗口，接受到现金
#define WM_NOTIFY_START_RECEIVE		WM_USER + 175			///< 通知前台窗口，开始接收现金
#define WM_NOTIFY_START_CIRCLE_READ	WM_USER + 176			///< 通知前台窗口，开始循环读卡
#define WM_UPDATE_RECEPITON_GUIDE	WM_USER + 177			///< 更新前台窗口业务提示
#define WM_NOTIFY_ERWEIMA_ORDER_RES	WM_USER + 178			///< 更新前台窗口二维码订单请求结果

#define WM_MAINTENANCE_MENU			WM_USER + 190			///< 维护业务菜单消息

// 键盘按键按下时，所发出的消息
#define WM_KEYBOARD_NUMBER			WM_USER + 200			///< 0-9  注：此为小键盘数字键
#define WM_KEYBOARD_DOT				WM_USER + 201			///< .
#define WM_KEYBOARD_CLEAR			WM_USER + 202			///< Clear
#define WM_KEYBOARD_ENTER			WM_USER + 203			///< Enter
#define WM_KEYBOARD_ESC				WM_USER + 204			///< Esc
#define WM_KEYBOARD_F1				WM_USER + 205			///< F1
#define WM_KEYBOARD_F2				WM_USER + 206			///< F2
#define WM_KEYBOARD_F3				WM_USER + 207			///< F3
#define WM_KEYBOARD_F4				WM_USER + 208			///< F4
#define WM_KEYBOARD_F5				WM_USER + 209			///< F5
#define WM_KEYBOARD_F6				WM_USER + 210			///< F6

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SM_FORCE_LOGOUT	(SP_USER + 0x1000)											// 强制登出消息
#define SM_NET_STATUS (SP_USER + 0x1001)											// 网络状态变化消息
#define SM_MAIN_SHUT_DOWN (SP_USER + 0x1002)										// 关机消息
#define SM_MAIN_START_NTP_TIMER (SP_USER + 0x1003)									// 开启NTP对时计时器消息
#define SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER (SP_USER + 0x1004)					// 开启运营日切换计时器消息
#define SM_MAIN_NEW_WORK_MODE (SP_USER + 0x1005)									// 运行模式变化消息
#define SM_MAIN_CLOSE_OPERATION (SP_USER + 0x1006)									// 运营结束消息
#define SM_MAIN_NEW_OPERATION_MODE (SP_USER + 0x1007)								// 运营模式变化消息
#define SM_MAIN_DEVICE_CONTROL (SP_USER + 0x1008)									// 运行控制命令消息
#define SM_MAIN_IO_MONITOR (SP_USER + 0x1009)										// IO监控命令消息
#define SM_MAIN_RETURN_INDICATOR (SP_USER + 0x1010)									// 找零指示灯计时器消息		
#define SM_MAIN_RETURN_LIGHT (SP_USER + 0x1011)										// 找零照明灯计时器消息
#define SM_MAIN_READY_TO_REST (SP_USER + 0x1012)									// 等待中到待机中消息
#define SM_MAIN_DOOR_OPEN_WITH_OUT_LOGIN (SP_USER + 0x1013)							// 后维护门开启未登陆计时器消息
#define SM_MAIN_DOOR_OPEN_OUT_OF_TIME_OPERATION (SP_USER + 0x1014)					// 后维护门开启操作员操作超时计时器消息
#define SM_MAIN_BLIND_MAN_OPERATION (SP_USER + 0x1015)								// 盲人购票业务调度消息
#define SM_MAIN_ON_SWITCH_BUSINESS_DAY_OPERATION (SP_USER + 0x1016)					// 执行运营切换命令
#define SM_MAIN_TIMER_CONTROL (SP_USER + 0x1017)									// 服务控制定时器消息
#define SM_MAIN_WEB_IDLE_PROCESS (SP_USER + 0x1018)									// 二维码订单本地处理定时器
#define SM_MAIN_RECHARGE_SERVER_TIMER (SP_USER + 0x1019)							// 充值服务器检测定时器
#define SM_MAIN_PARAMETER_UPDATE_TIMER (SP_USER + 0x101A)							// SC下载参数加载定时器
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SM_ASSITANCE_BUTTON_DOWN (SP_USER + 0x1031)											// 招援按钮按下
#define SM_ASSITANCE_BUTTON_UP (SP_USER + 0x1032)											// 招援按钮弹起

#define SM_DOOR_CLOSED (SP_USER + 0x1033)													// 维护门关闭
#define SM_DOOR_OPENED (SP_USER + 0x1034)													// 维护门打开

#define SM_ISSUE_OF_BLIND_BEGIN_OPERATION (SP_USER + 0x1035)								// 盲人售票开始消息
