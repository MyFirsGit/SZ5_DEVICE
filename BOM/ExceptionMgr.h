#pragma once
#include "Service.h"
#include "SysException.h"
#include "ExceptionLog.h"
#include <map>
#include "ServiceDefinition.h"
#include "GlobalParameter.h"
using namespace std;

#define theEXCEPTION_MGR CExceptionMgr::GetInstance()

/**
@brief      �쳣����
*/
//////////////////////////////////////////////////////////////////////////
/**
@brief      ���캯��

@param      ��

@retval     ��

@exception  ��
*/
//////////////////////////////////////////////////////////////////////////
class CExceptionMgr
{
public:
	delegate<void(CSysException &,bool)> ExceptionItemChanged;	//�쳣���з����仯�¼�
	static CExceptionMgr& GetInstance();                                // ��ȡ����Ķ���
	void AddException(CSysException& e,bool needSendStatus = true);     // ��¼�쳣����
	void RemoveException(long moduleID,long detailID = NO_DETAIL_ID);  // ����ģ�������ģ������ϸ����ɾ���쳣����
	void RemoveExceptionExcept(long moduleID,long detailID);			// ɾ��ָ���쳣�����ģ���������쳣��¼
	bool HasException();                                                // �Ƿ����쳣
	bool HasException(long moduleID);                                   // �Ƿ���ָ�������쳣
	bool HasException(vector<long>& moduleIDs);			// �Ƿ����ض����͵��쳣
	bool OnlyHasHardwareException();                                    // �Ƿ�ֻ��Ӳ���쳣
	bool OnlyHasException(long moduleID);							//�Ƿ�ֻ���ض����쳣
	bool OnlyHasException(vector<long>& moduleIDs);		//�Ƿ�ֻ���ض����쳣
	bool HasException(long moduleID, long detailID);                    // �Ƿ���ָ�����ָ����ϸ������쳣
	bool HasMaintainException();                                        // �Ƿ���ά���쳣
	CSysException& GetLastException();                                  // ��ȡ���һ���쳣����
	void WriteExceptionLog(CSysException& e,CExceptionLog::EXCEPTION_LEVEL exLevel = CExceptionLog::LEVEL_WARNING);  // д���쳣Log
	void ProcessException(CSysException& e);							// �����쳣
	bool RepareException();                                             // ���Ͻ��
	unsigned long GetMTC(CSysException&) const;
	unsigned short GetEC(CSysException&) const;
	unsigned long GetActualMTC(CSysException&) const;
	SERVICE_STATE GetServiceState(CSysException&) const;
	CString GetDescription(CSysException&) const;
	CString GetPrompt(CSysException&) const;
	const vector<CSysException>& GetAllException();                     // �õ����е��쳣
	DEVICE_COMPONENT GetDevicePartTypeOfException(CSysException& e);
private:
	CExceptionMgr();                                                    // ���캯��
	~CExceptionMgr();                                                   // ��������
	
	CSysException RemoveLastException();                                // ɾ�����������һ���쳣����
	const static long NO_DETAIL_ID = 0;                                 // ����ϸ����
	vector<CSysException> m_exceptionArr;	                            // ��¼�������쳣����
	bool m_isBusy;                                                      // �Ƿ����쳣���ڴ���	
	static CExceptionMgr theInstance;                                   // �����Ψһ�ӿ�

};
