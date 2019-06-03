#pragma once
#include "GetIni.h"
#include "UIDefinition.h"
#include "Util.h"



#define theTPUERROR_INFO TpuErrorInfo::GetInstance()

class  TpuErrorInfo
{
public:

	static TpuErrorInfo&  GetInstance();

	int Initialize();                              // ���ڻ�

	// ������ȡ�����ݵķ���

	//[OPERATION_MSG]
	CString GetTpuErrorInfo(int);

private:
	static TpuErrorInfo theInstance;
	CGetIni* m_guideIni;
	CMap<DWORD,DWORD,CString,CString> m_mapTpuErrorInfo;
	
	// ���ع��캯��
	TpuErrorInfo();
	~TpuErrorInfo();
	TpuErrorInfo(const TpuErrorInfo&);
	TpuErrorInfo& operator=(const TpuErrorInfo&);

};