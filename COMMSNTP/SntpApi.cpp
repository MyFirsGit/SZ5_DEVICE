//
// Create Date : 2010.01.20
// File Name : SntpApi.cpp
//

#include "stdafx.h"
#include <math.h>
#include "sntp.h"
#include "SntpApi.h"
//#include "LogFileMgr.h"
//#include "trace.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static CString sGetExeRunPath(void)
{
	CString   strExePath;
	TCHAR tchExeFile[MAX_PATH];
	memset(tchExeFile, 0, sizeof(tchExeFile));
	GetModuleFileName(NULL, tchExeFile, MAX_PATH);   
	strExePath   =   tchExeFile;   
	strExePath   =   strExePath.Left(strExePath.ReverseFind('\\'));   

	strExePath += _T("\\");

	return strExePath;

}
//强制时钟同步，不返回故障或警告
int SNTP_API TimeSynchronize()
{
	CString strIni = sGetExeRunPath() + _T("Maintenance.INI");
	TCHAR szSvrAddr[MAX_PATH];
	memset(szSvrAddr, 0, sizeof(szSvrAddr));
	GetPrivateProfileString(_T("NTP"), _T("HostID"), _T(""), szSvrAddr, MAX_PATH, strIni);

	CString strAddr = szSvrAddr;
	if (strAddr.IsEmpty()){
		return NTP_SVR_ADDR_EMPTY;
	}
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);   
	if (WSAStartup(wVersionRequested, &wsaData) != 0){
		return NTP_LOAD_WINSOCKET_FAILURE;
	}
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1){
		return NTP_WINSOCKET_VER_NOT_SUPPORT;
	}
	CSNTPClient sntp;
	NtpServerResponse response;
	int nRet = NTP_SUCCESSED;
	for(int i=0;i<3;i++)
	{
		nRet = sntp.GetServerTime(strAddr, response);
		if(nRet == NTP_SUCCESSED)
		{
			break;
		}
		::Sleep(100);
	}
	
	if (nRet == NTP_SUCCESSED) 
	{
		CNtpTime newTime(CNtpTime::GetCurrentTime() + response.m_LocalClockOffset);
		if (!sntp.SetClientTime(newTime)){
			nRet = NTP_SET_SYSTIME_FAILURE;
		}
	}
	WSACleanup();
	return nRet;
}

int SNTP_API TimeSynchronize(int nOffsetAlarm /*= 60*/, int nOffFault /*= 300*/)
{
	CString strIni = sGetExeRunPath() + _T("Maintenance.INI");
	TCHAR szSvrAddr[MAX_PATH];
	memset(szSvrAddr, 0, sizeof(szSvrAddr));
	GetPrivateProfileString(_T("NTP"), _T("HostID"), _T(""), szSvrAddr, MAX_PATH, strIni);

	CString strAddr = szSvrAddr;
	if (strAddr.IsEmpty())
		return NTP_SVR_ADDR_EMPTY;

	return TimeSynchronize(strAddr, nOffsetAlarm, nOffFault);
}

int SNTP_API TimeSynchronize(LPCTSTR pszServiceAdd, int nOffsetAlarm /*= 60*/, int nOffFault /*= 300*/)
{
	if (pszServiceAdd == NULL)
		return NTP_ERROR_OTHER;

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);   
	if (WSAStartup(wVersionRequested, &wsaData) != 0) 
		return NTP_LOAD_WINSOCKET_FAILURE;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
		return NTP_WINSOCKET_VER_NOT_SUPPORT;

	CSNTPClient sntp;
	NtpServerResponse response;
	int nRet =  NTP_SUCCESSED;
	for(int i=0;i<3;i++)
	{
		nRet =  sntp.GetServerTime(pszServiceAdd, response);
		if(nRet == NTP_SUCCESSED)
		{
			break;
		}
		::Sleep(100);
	}
	
	if (nRet == NTP_SUCCESSED) 
	{
		int nOffset = abs(response.m_LocalClockOffset);
		if (nOffset < nOffFault)
		{
			CNtpTime newTime(CNtpTime::GetCurrentTime() + response.m_LocalClockOffset);
			if (!sntp.SetClientTime(newTime))
				nRet = NTP_SET_SYSTIME_FAILURE;

			if (nOffset >= nOffsetAlarm)
				nRet = NTP_SUCCESSED_AND_ALARM;
		}
		else
		{
			nRet = NTP_TIME_DIFF_FAULT;
		}
	}
	
// 	LogFile &file = gGetLogHandle(MANTIANCEOP_LOG);
// 	file<<_T("时钟同步\t")<<nRet;
// 	file.endl();

	WSACleanup();

	return nRet;
}