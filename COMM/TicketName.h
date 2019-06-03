#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"


#define theTICKET_NAME CTicketName::GetInstance()

class COMMON_API CTicketName
{
public:

	static CTicketName&  GetInstance();

	int Initialize();                              // 初期化

	// 以下是取得数据的方法
	CString GetTicketNameCn(int NO);             // 获取车票名称
	CString GetTicketNameEn(int NO);             // 获取车票名称
	

private:
	static CTicketName theInstance;
	CGetIni* m_DeviceIni;

	// 隐藏构造函数
	CTicketName();
	~CTicketName();
	CTicketName(const CTicketName&);
	CTicketName& operator=(const CTicketName&);
};