#pragma once
#include "GetIni.h"
#include "CommonExp.h"
#include "Util.h"


#define theTICKET_NAME CTicketName::GetInstance()

class COMMON_API CTicketName
{
public:

	static CTicketName&  GetInstance();

	int Initialize();                              // ���ڻ�

	// ������ȡ�����ݵķ���
	CString GetTicketNameCn(int NO);             // ��ȡ��Ʊ����
	CString GetTicketNameEn(int NO);             // ��ȡ��Ʊ����
	

private:
	static CTicketName theInstance;
	CGetIni* m_DeviceIni;

	// ���ع��캯��
	CTicketName();
	~CTicketName();
	CTicketName(const CTicketName&);
	CTicketName& operator=(const CTicketName&);
};