#pragma once
#include "GetIni.h"
#include "UIDefinition.h"
#include "Util.h"



#define theGUIDE_INFO CGuideInfo::GetInstance()

class  CGuideInfo
{
public:

	static CGuideInfo&  GetInstance();

	int Initialize();                              // ���ڻ�

	// ������ȡ�����ݵķ���

	CString GetOperationMessage(LANGUAGE_MSG);

	CString GetPassengerMessageCn(LANGUAGE_MSG);
	CString GetPassengerMessageEn(LANGUAGE_MSG);
	CString GetPassengerMessagePn(LANGUAGE_MSG);

	CString GetMaintenanceMessage(LANGUAGE_MSG);

private:
	static CGuideInfo theInstance;
	CGetIni* m_guideIni;
	CMap<DWORD,DWORD,CString,CString> m_mapResultCodeMsg;
	CMap<DWORD,DWORD,CString,CString> m_mapOperationMsg;
	CMap<DWORD,DWORD,CString,CString> m_mapPassengerMsgCn;
	CMap<DWORD,DWORD,CString,CString> m_mapPassengerMsgEn;
	CMap<DWORD,DWORD,CString,CString> m_mapMaintenanceMsg;

	// ���ع��캯��
	CGuideInfo();
	~CGuideInfo();
	CGuideInfo(const CGuideInfo&);
	CGuideInfo& operator=(const CGuideInfo&);

};