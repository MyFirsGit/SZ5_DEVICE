/*************************************************
File name  :  AFCLogApi.h
Description:  windows平台模块通用写日志API
Author     :  wf
Version    :  1.0
Date       :  2019/4/25
*************************************************/
#ifndef AFCLOGAPI_H
#define AFCLOGAPI_H
#pragma once
#define _AFXDLL
#include <afxwin.h>
#include <Windows.h>

//*使用方法
//LogApi.Initialize(_T("d:\\tmp\\"),_T("mylog_"));
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("这是第一条日志%d"),1);
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("这是第二条日志%d"),2);
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("这是第三条日志%d"),3);
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("这是第四条日志%d"),4);
//LogApi.CloseLog();
#define LogApi	CAFCLogApi::GetInstance()

class CAFCLogApi
{
public:
	static CAFCLogApi& GetInstance(){
		return theInstance;
	}
private:
	HANDLE			m_hFile;					// 日志文件句柄
	CRITICAL_SECTION m_hLock;					// lock for thread-safe access
	CString			 m_strFileName;				// 日志文件名
	CString			 m_strTracePath;			// 日志存放目录
	DWORD			 m_dwLineCount;
private:
	CAFCLogApi(void);
	~CAFCLogApi(void);

	static CAFCLogApi theInstance;
	void	MakeFileName();
	void	MakeFullName(CString &sFullName);
	void	WriteData(CString& data);                                   // 写日志
	void	CheckAndCreatePath();
public:
	// 要写日志，必须先调用Initialize对api初始化
	// path:日志存储路径，如：D:\\TVM_DATA\\LOG\\,路径不存在则自动创建路径
	// filename:日志名，如：TVMIO_
	void Initialize(CString path,CString filename);
	// 需要关闭日志，否则无法上传
	void CloseLog(void);
	// 写日志
	void WriteLog(CString className, CString methodName, int line, CString format, ...);
};

#endif