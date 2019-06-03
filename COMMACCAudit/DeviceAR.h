#pragma once

#include "ACCAuditDef.h"
#include "CACCAuditException.h"
#include "Util.h"
#include "ACCAuditExp.h"


// DeviceAR��Ψһ�ӿ�
#define theDeviceAR DeviceAR::GetInstance()  
// 
class ACCAUDIT_DECLSPEC DeviceAR
{
public:

	static DeviceAR &GetInstance()
	{
		return theInstance;
	}
     
	void MakePackHead(LPBYTE ARCommHead,int ARFileTag);                           //��ȡ������ݱ���ͷ
	void FindPos(long RegistId,long& theUpdataPosition);                          // ��������������ݵ�λ��  
	void MakedataByTicketType(long ticketType,long theUpdataPosition,long updataData,LPBYTE addData);                  //����µĳ�Ʊ������Ϣ

    long InitARFile(CString dataPath=_T(""),CString backupPath=_T(""));                                                                //��ʼ��ACC�ռ��ļ�
	bool AddProcessStatByType(long ticketType,long RegistId,long updatedata);         //���ӷ�Ʊ���Զ���Ʊ������ͳ��
	bool ReadProcessStat(DEVICE_AR_REASON ARFileTag,VARIABLE_DATA& arMesg);                       //����Ʊ�Ͷ�ȡ��Ʊ���Զ���Ʊ������ͳ��
    void UpdateSerialFile();
	
private:
	CString m_dataPath;
	CString m_BackupPath;
	CString currentFileName;
	CString BackFileName;
	CString sequenceFileName;

	long deviceType;
	long TicketCountInBox1;
	long TicketCountInBox2;

    static DeviceAR theInstance;

	DeviceAR(void);
	~DeviceAR(void);
};
