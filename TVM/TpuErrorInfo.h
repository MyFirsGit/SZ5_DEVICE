#pragma once
#include "GetIni.h"
#include "UIDefinition.h"
#include "Util.h"



#define theTPUERROR_INFO TpuErrorInfo::GetInstance()

class  TpuErrorInfo
{
public:

	static TpuErrorInfo&  GetInstance();

	int Initialize();                              // 初期化

	// 以下是取得数据的方法

	//[OPERATION_MSG]
	CString GetTpuErrorInfo(int);

private:
	static TpuErrorInfo theInstance;
	CGetIni* m_guideIni;
	CMap<DWORD,DWORD,CString,CString> m_mapTpuErrorInfo;
	
	// 隐藏构造函数
	TpuErrorInfo();
	~TpuErrorInfo();
	TpuErrorInfo(const TpuErrorInfo&);
	TpuErrorInfo& operator=(const TpuErrorInfo&);

};