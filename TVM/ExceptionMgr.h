#pragma once
#include "Service.h"
#include "SysException.h"
#include "ExceptionLog.h"
#include <map>
#include "ServiceDefinition.h"
#include "ExceptionInfo.h"
#include "exceptionprocessor.h"

using namespace std;

#define theEXCEPTION_MGR CExceptionMgr::GetInstance()

/**
@brief      �쳣�����ඨ��
*/
class CExceptionMgr : public CExceptionProcessor
{
public:
	
	static CExceptionMgr& GetInstance();                           // ��ȡ����Ķ���
	
	bool OnlyHasHardwareException();                                // �Ƿ�ֻ��Ӳ���쳣
	bool HasMaintainException();                                        // �Ƿ���ά���쳣
	
	// д�쳣��־
	void WriteExceptionLog(CSysException& e,CExceptionLog::EXCEPTION_LEVEL exLevel = CExceptionLog::LEVEL_WARNING);  
	virtual void ProcessException(CSysException& e);				   // �����쳣
	virtual bool RepareException(bool bMenualRepare = false);          // �쳣���
	unsigned long GetMTC(CSysException&) const;				    // ��ȡMTC��(��������״̬��)
	unsigned short GetEC(CSysException&) const;						// ��ȡEC��
	unsigned long GetActualMTC(CSysException&) const;			// ��ȡMTC��(������״̬��)
	SERVICE_STATE GetServiceState(CSysException&) const;       // ��ȡ�쳣�����з���״̬��
	CString GetDescription(CSysException&) const;						// ��ȡ�쳣����
	CString GetPrompt(CSysException&) const;							// ��ȡ�쳣������
	
	BYTE GetDevicePartTypeOfException(CSysException& e);

private:
	CExceptionMgr();                                            // ���캯��
	virtual ~CExceptionMgr();                                   // ��������
	bool m_isBusy;												// �Ƿ����쳣���ڴ���	
	static CExceptionMgr theInstance;                           // �����Ψһ�ӿ�
	void ResetTimesOfRepareException(); // �����Զ��쳣�޸����ƴ���
	int m_nTimesRepareRWRecharge;		// ��ֵ����������쳣����						
	int m_nTimesRepareRWCard;			// Ʊ���������쳣�������
	int m_nTimesRepareRWToken;			// Token�������쳣�������
	int m_nTimesReparePrint;			// ��ӡ���쳣�������
	int m_nTimesRepareSNTP;				// NTP��ʱ�����쳣�������
	int m_nTimesRepareFTP;				// FTP�쳣�������
	int m_nTimesRepareBankcard;			// ���п��쳣�������
	int m_nTimesRepareBanknote;			// ֽ��ģ���쳣�������
	int m_nTimesRepareBanknoteChange;	// ֽ������ģ���쳣�������
	int m_nTimesRepareCard;				// Ʊ��ģ���쳣�������
	int m_nTimesRepareCoin;				// Ӳ��ģ���쳣�������
	int m_nTimesRepareCoinShutter;		// Ӳ��Ͷ�ҿ��쳣�������
	int m_nTimesRepareIO;				// IO�쳣�������
	int m_nTimesReparePinPad;			// ��������쳣�������
	int m_nTimesRepareDisplay;			// ��ʾ���쳣�������
	int m_nTimesRepareToken;			// Tokenģ���쳣�������
	int m_nTimerRepareRFID;				// ֽ������RFID
};
