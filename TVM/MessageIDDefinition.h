#pragma once


#define WM_XBUTTON                  WM_USER + 150           ///< ���°�ť����������Ϣ
#define WM_XGROUP                   WM_USER + 151           ///< ���°�ť������������Ϣ
#define WM_EDIT_SETFOCUSE           WM_USER + 152           ///< ����༭���ý������Ϣ
#define WM_START_SERVICE			WM_USER + 153           ///< ����service����Ϣ
#define WM_STOP_SERVICE				WM_USER + 154           ///< ֹͣservice����Ϣ
#define WM_XBUTTON_CLICK            WM_USER + 155           ///< (XBUTTON�ڲ�)���°�ť����������Ϣ
#define WM_XLINEGROUP               WM_USER + 156           ///< ������·ͼ��ť������������Ϣ
#define WM_EDIT_CHANGE				WM_USER	+ 161			///< �༭�����ݸı�ʱ����������Ϣ��
#define WM_NOTIFY_DIALOG_UPDATE		WM_USER + 171			///< ǰ̨����֪ͨ������½���
// ����人ҵ�����̣�����
#define WM_NOTIFY_ISSUING			WM_USER + 172			///< ֪ͨǰ̨���ڣ����۳�Ʊ���ֵ
#define WM_START_ISSUING			WM_USER + 173			///< ��ʼ���۳�Ʊ
#define WM_NOTIFY_ACCEPTED			WM_USER + 174			///< ֪ͨǰ̨���ڣ����ܵ��ֽ�
#define WM_NOTIFY_START_RECEIVE		WM_USER + 175			///< ֪ͨǰ̨���ڣ���ʼ�����ֽ�
#define WM_NOTIFY_START_CIRCLE_READ	WM_USER + 176			///< ֪ͨǰ̨���ڣ���ʼѭ������
#define WM_UPDATE_RECEPITON_GUIDE	WM_USER + 177			///< ����ǰ̨����ҵ����ʾ
#define WM_NOTIFY_ERWEIMA_ORDER_RES	WM_USER + 178			///< ����ǰ̨���ڶ�ά�붩��������

#define WM_MAINTENANCE_MENU			WM_USER + 190			///< ά��ҵ��˵���Ϣ

// ���̰�������ʱ������������Ϣ
#define WM_KEYBOARD_NUMBER			WM_USER + 200			///< 0-9  ע����ΪС�������ּ�
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
#define SM_FORCE_LOGOUT	(SP_USER + 0x1000)											// ǿ�Ƶǳ���Ϣ
#define SM_NET_STATUS (SP_USER + 0x1001)											// ����״̬�仯��Ϣ
#define SM_MAIN_SHUT_DOWN (SP_USER + 0x1002)										// �ػ���Ϣ
#define SM_MAIN_START_NTP_TIMER (SP_USER + 0x1003)									// ����NTP��ʱ��ʱ����Ϣ
#define SM_MAIN_START_SWITH_BUSINESS_DAY_TIMER (SP_USER + 0x1004)					// ������Ӫ���л���ʱ����Ϣ
#define SM_MAIN_NEW_WORK_MODE (SP_USER + 0x1005)									// ����ģʽ�仯��Ϣ
#define SM_MAIN_CLOSE_OPERATION (SP_USER + 0x1006)									// ��Ӫ������Ϣ
#define SM_MAIN_NEW_OPERATION_MODE (SP_USER + 0x1007)								// ��Ӫģʽ�仯��Ϣ
#define SM_MAIN_DEVICE_CONTROL (SP_USER + 0x1008)									// ���п���������Ϣ
#define SM_MAIN_IO_MONITOR (SP_USER + 0x1009)										// IO���������Ϣ
#define SM_MAIN_RETURN_INDICATOR (SP_USER + 0x1010)									// ����ָʾ�Ƽ�ʱ����Ϣ		
#define SM_MAIN_RETURN_LIGHT (SP_USER + 0x1011)										// ���������Ƽ�ʱ����Ϣ
#define SM_MAIN_READY_TO_REST (SP_USER + 0x1012)									// �ȴ��е���������Ϣ
#define SM_MAIN_DOOR_OPEN_WITH_OUT_LOGIN (SP_USER + 0x1013)							// ��ά���ſ���δ��½��ʱ����Ϣ
#define SM_MAIN_DOOR_OPEN_OUT_OF_TIME_OPERATION (SP_USER + 0x1014)					// ��ά���ſ�������Ա������ʱ��ʱ����Ϣ
#define SM_MAIN_BLIND_MAN_OPERATION (SP_USER + 0x1015)								// ä�˹�Ʊҵ�������Ϣ
#define SM_MAIN_ON_SWITCH_BUSINESS_DAY_OPERATION (SP_USER + 0x1016)					// ִ����Ӫ�л�����
#define SM_MAIN_TIMER_CONTROL (SP_USER + 0x1017)									// ������ƶ�ʱ����Ϣ
#define SM_MAIN_WEB_IDLE_PROCESS (SP_USER + 0x1018)									// ��ά�붩�����ش���ʱ��
#define SM_MAIN_RECHARGE_SERVER_TIMER (SP_USER + 0x1019)							// ��ֵ��������ⶨʱ��
#define SM_MAIN_PARAMETER_UPDATE_TIMER (SP_USER + 0x101A)							// SC���ز������ض�ʱ��
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SM_ASSITANCE_BUTTON_DOWN (SP_USER + 0x1031)											// ��Ԯ��ť����
#define SM_ASSITANCE_BUTTON_UP (SP_USER + 0x1032)											// ��Ԯ��ť����

#define SM_DOOR_CLOSED (SP_USER + 0x1033)													// ά���Źر�
#define SM_DOOR_OPENED (SP_USER + 0x1034)													// ά���Ŵ�

#define SM_ISSUE_OF_BLIND_BEGIN_OPERATION (SP_USER + 0x1035)								// ä����Ʊ��ʼ��Ϣ
