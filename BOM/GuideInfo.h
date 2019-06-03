#pragma once
#include "GetIni.h"
#include "UIDefinition.h"
#include "Util.h"



#define theGUIDE_INFO CGuideInfo::GetInstance()

class  CGuideInfo
{
public:

	static CGuideInfo&  GetInstance();

	int Initialize();                              // 初期化

	// 以下是取得数据的方法

	//[OPERATION_MSG]
	CString GetOperationMessage(OPERATION_MSG);

	//[PASSENGER_MSG]
	CString GetPassengerMessageCn(PASSENGER_MSG);
	CString GetPassengerMessageEn(PASSENGER_MSG);

	//[MAINTENANCE_MSG]
	CString GetMaintenanceMessage(MAINTENANCE_MSG);

private:
	static CGuideInfo theInstance;
	CGetIni* m_guideIni;
	CMap<DWORD,DWORD,CString,CString> m_mapResultCodeMsg;
	CMap<DWORD,DWORD,CString,CString> m_mapOperationMsg;
	CMap<DWORD,DWORD,CString,CString> m_mapPassengerMsgCn;
	CMap<DWORD,DWORD,CString,CString> m_mapPassengerMsgEn;
	CMap<DWORD,DWORD,CString,CString> m_mapMaintenanceMsg;

	// 隐藏构造函数
	CGuideInfo();
	~CGuideInfo();
	CGuideInfo(const CGuideInfo&);
	CGuideInfo& operator=(const CGuideInfo&);

};