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
	NTP_SUCCESSED_AND_ALARM, //当同步时间小于故障值大于报警值时，修改本地时间，并向服务器报警。
	NTP_TIME_DIFF_FAULT	,	 //本地时间与服务器时间差值超过故障差值。

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
本机时间与服务器的时间进行同步处理。
进行时钟同步的服务器地址，从配置文件中获得。
参数说明：nOffsetAlarm：同步时间报警差值， 单位秒，默认60秒；
		  nOffFault : 同步时间故障差值， 单位秒，默认300秒。

当时钟差异大于等于时间报警值并小于等于时间故障值时，下位系统使用服务器时间更改本地时间，并向上位系统报警。
当时钟差异大于等于故障值时，下位系统不更改本地时间，并向上位系统上报故障数据。如果下位系统是SLE，则应暂停乘客服务。
*/
//int SNTP_API TimeSynchronize(int nOffsetAlarm = 60, int nOffFault = 300); 
int SNTP_API TimeSynchronize(int nOffsetAlarm, int nOffFault); 

/*
本机时间与服务器的时间进行同步处理。
pszServiceAdd指进行时钟同步的服务器的地址。
参数说明：nOffsetAlarm：同步时间报警差值， 单位秒，默认60秒；
		  nOffFault : 同步时间故障差值， 单位秒，默认300秒。

当时钟差异大于等于时间报警值并小于等于时间故障值时，下位系统使用服务器时间更改本地时间，并向上位系统报警。
当时钟差异大于等于故障值时，下位系统不更改本地时间，并向上位系统上报故障数据。如果下位系统是SLE，则应暂停乘客服务。
*/
//int SNTP_API TimeSynchronize(LPCTSTR pszServiceAdd, int nOffsetAlarm= 60, int nOffFault = 300); 
int SNTP_API TimeSynchronize(LPCTSTR pszServiceAdd, int nOffsetAlarm, int nOffFault); 

//强制时钟同步，不返回故障或警告
int SNTP_API TimeSynchronize(); 
#endif //__INTERFACE_SNTP_H__