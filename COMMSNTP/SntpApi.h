//
// Create FileName : SntpApi.h
// 

#ifndef __INTERFACE_SNTP_H__
#define __INTERFACE_SNTP_H__

#ifdef SNTP_EXPORTS
#define SNTP_API __declspec(dllexport)
#else
#define SNTP_API __declspec(dllimport)
#endif

enum _NTP_ERROR
{
	NTP_SUCCESSED			= 0,
	NTP_SUCCESSED_AND_ALARM, //��ͬ��ʱ��С�ڹ���ֵ���ڱ���ֵʱ���޸ı���ʱ�䣬���������������
	NTP_TIME_DIFF_FAULT	,	 //����ʱ���������ʱ���ֵ�������ϲ�ֵ��

	NTP_TIMEOUT				,
	NTP_CONNECT_FAILURE		,
	NTP_CREAT_SOCKET_FAILURE, 
	NTP_SEND_REQUEST_FAILURE,
	NTP_READ_APLY_FAILURE	,
	NTP_SET_SYSTIME_FAILURE	,
	NTP_LOAD_WINSOCKET_FAILURE	,
	NTP_WINSOCKET_VER_NOT_SUPPORT,
	NTP_SVR_ADDR_EMPTY	,
	NTP_ERROR_OTHER		,
};

/*
����ʱ�����������ʱ�����ͬ������
����ʱ��ͬ���ķ�������ַ���������ļ��л�á�
����˵����nOffsetAlarm��ͬ��ʱ�䱨����ֵ�� ��λ�룬Ĭ��60�룻
		  nOffFault : ͬ��ʱ����ϲ�ֵ�� ��λ�룬Ĭ��300�롣

��ʱ�Ӳ�����ڵ���ʱ�䱨��ֵ��С�ڵ���ʱ�����ֵʱ����λϵͳʹ�÷�����ʱ����ı���ʱ�䣬������λϵͳ������
��ʱ�Ӳ�����ڵ��ڹ���ֵʱ����λϵͳ�����ı���ʱ�䣬������λϵͳ�ϱ��������ݡ������λϵͳ��SLE����Ӧ��ͣ�˿ͷ���
*/
//int SNTP_API TimeSynchronize(int nOffsetAlarm = 60, int nOffFault = 300); 
int SNTP_API TimeSynchronize(int nOffsetAlarm, int nOffFault); 

/*
����ʱ�����������ʱ�����ͬ������
pszServiceAddָ����ʱ��ͬ���ķ������ĵ�ַ��
����˵����nOffsetAlarm��ͬ��ʱ�䱨����ֵ�� ��λ�룬Ĭ��60�룻
		  nOffFault : ͬ��ʱ����ϲ�ֵ�� ��λ�룬Ĭ��300�롣

��ʱ�Ӳ�����ڵ���ʱ�䱨��ֵ��С�ڵ���ʱ�����ֵʱ����λϵͳʹ�÷�����ʱ����ı���ʱ�䣬������λϵͳ������
��ʱ�Ӳ�����ڵ��ڹ���ֵʱ����λϵͳ�����ı���ʱ�䣬������λϵͳ�ϱ��������ݡ������λϵͳ��SLE����Ӧ��ͣ�˿ͷ���
*/
//int SNTP_API TimeSynchronize(LPCTSTR pszServiceAdd, int nOffsetAlarm= 60, int nOffFault = 300); 
int SNTP_API TimeSynchronize(LPCTSTR pszServiceAdd, int nOffsetAlarm, int nOffFault); 

//ǿ��ʱ��ͬ���������ع��ϻ򾯸�
int SNTP_API TimeSynchronize(); 
#endif //__INTERFACE_SNTP_H__