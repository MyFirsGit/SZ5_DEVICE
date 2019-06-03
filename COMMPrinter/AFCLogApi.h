/*************************************************
File name  :  AFCLogApi.h
Description:  windowsƽ̨ģ��ͨ��д��־API
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

//*ʹ�÷���
//LogApi.Initialize(_T("d:\\tmp\\"),_T("mylog_"));
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("���ǵ�һ����־%d"),1);
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("���ǵڶ�����־%d"),2);
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("���ǵ�������־%d"),3);
//LogApi.WriteLog(_T(""),CString(__FUNCTION__),__LINE__,_T("���ǵ�������־%d"),4);
//LogApi.CloseLog();
#define LogApi	CAFCLogApi::GetInstance()

class CAFCLogApi
{
public:
	static CAFCLogApi& GetInstance(){
		return theInstance;
	}
private:
	HANDLE			m_hFile;					// ��־�ļ����
	CRITICAL_SECTION m_hLock;					// lock for thread-safe access
	CString			 m_strFileName;				// ��־�ļ���
	CString			 m_strTracePath;			// ��־���Ŀ¼
	DWORD			 m_dwLineCount;
private:
	CAFCLogApi(void);
	~CAFCLogApi(void);

	static CAFCLogApi theInstance;
	void	MakeFileName();
	void	MakeFullName(CString &sFullName);
	void	WriteData(CString& data);                                   // д��־
	void	CheckAndCreatePath();
public:
	// Ҫд��־�������ȵ���Initialize��api��ʼ��
	// path:��־�洢·�����磺D:\\TVM_DATA\\LOG\\,·�����������Զ�����·��
	// filename:��־�����磺TVMIO_
	void Initialize(CString path,CString filename);
	// ��Ҫ�ر���־�������޷��ϴ�
	void CloseLog(void);
	// д��־
	void WriteLog(CString className, CString methodName, int line, CString format, ...);
};

#endif